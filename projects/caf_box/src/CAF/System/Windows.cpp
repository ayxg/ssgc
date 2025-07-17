///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2025 Anton Yashchenko
// Licensed under the Apache License, Version 2.0(the "License");
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: [CAF] Core Application Framework
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup coreappfw
/// @brief [SOURCE] Static windowing system.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup coreappfw
/// @{

// clang-format off
#include "CAF/System/Windows.hpp"
#include "cxxx.hpp"
#include "CAF/PreConfig.hpp"
// clang-format on

namespace caf::sys {

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Init Windows static data */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::list<Windows::Node> Windows::windows_{};
Windows::Node* Windows::last_window_{nullptr};
Windows::Node* Windows::curr_window_{nullptr};
Windows::EventType Windows::curr_event_{};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Window::Node impl */ 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Windows::Node::Node(Node* parent)
    : under_(std::make_unique<WindowType>(VideoModeType::getDesktopMode(), "")),
      title_(""),
      marked_for_destruction_(false),
      frozen_(std::make_pair(false,nullptr)),
      event_handlers_(),
      cbOnDealloc_([]() {})
{

  if (parent) node_base_stem__ = parent;
  cbFrozenEvent_ = [this](const EventType& ev) { DefaultFrozenEvent(ev); };
}

Windows::Node::Node(const Hints& hints, Node* parent)
    : marked_for_destruction_(false),
      frozen_(std::make_pair(false, nullptr)),
      event_handlers_(),
      cbOnDealloc_([]() {}) {
  if (parent) node_base_stem__ = parent;
  cbFrozenEvent_ = [this](const EventType& ev) { DefaultFrozenEvent(ev); };

  // Get the default configurations from underlying windowing library.
  std::uint32_t style = sf::Style::Default;
  VideoModeType video_mode = VideoModeType::getDesktopMode();
  OpenGlSettingsType gl_settings{};

  // Handle Pre-Launch settings
  // -> set window size
  if (hints.InitialWidth) video_mode.width = hints.InitialWidth.value();
  if (hints.InitialHeight) video_mode.height = hints.InitialHeight.value();

  // -> window border/titlebar/fullscreen flags
  if (hints.Fullscreen.value_or(0))
    style = sf::Style::Fullscreen;
  else if (hints.NoTitleBar.value_or(0))
    style = sf::Style::None;
  else if (hints.NoResize.value_or(0))
    if (hints.NoCloseButton.value_or(0))
      style = sf::Style::Titlebar;
    else
      style = sf::Style::Close;
  else if (hints.NoCloseButton.value_or(0))
    style = sf::Style::Resize;
  else
    style = sf::Style::Default;

  // -> Opengl context hints
  if (hints.GlAntialiasingLevel) gl_settings.antialiasingLevel = hints.GlAntialiasingLevel.value();
  if (hints.GlStencilBits) gl_settings.stencilBits = hints.GlStencilBits.value();
  if (hints.GlMajorVersion) gl_settings.majorVersion = hints.GlMajorVersion.value();
  if (hints.GlMinorVersion) gl_settings.minorVersion = hints.GlMinorVersion.value();
  if (hints.GlAttributeFlags) gl_settings.attributeFlags = hints.GlAttributeFlags.value();
  if (hints.GlsRgbCapable) gl_settings.sRgbCapable = hints.GlsRgbCapable.value();

  // -> Init the window
  title_ = hints.InitialTitle.value_or("");
  under_ = std::make_unique<WindowType>(video_mode, sf::String{title_.data()}, style, gl_settings);

  // Handle post-launch settings
  if (hints.FrameLimit.value_or(0)) under_->setFramerateLimit(hints.FrameLimit.value());
  if (hints.EnableVsync.value_or(0)) under_->setVerticalSyncEnabled(true);
  if (hints.Hidden.value_or(0)) under_->setVisible(false);
  if (hints.RequestFocus.value_or(0)) under_->requestFocus();

  // -> Initial pos
  std::pair<int, int> position{};
  position.first = hints.InitialPositionX.value_or(under_->getPosition().x);
  position.second = hints.InitialPositionY.value_or(under_->getPosition().y);
  under_->setPosition({position.first, position.second});
}

// Windows::Node - Mutable properties

sf::Vector2i Windows::Node::Position() const { return under_->getPosition(); }

void Windows::Node::Position(const sf::Vector2i& new_pos) { under_->setPosition(new_pos); }

sf::Vector2u Windows::Node::Size() const { return under_->getSize(); }

void Windows::Node::Size(const sf::Vector2u& new_pos) { under_->setSize(new_pos); }

const std::string& Windows::Node::Title() const { return title_; }

void Windows::Node::Title(const std::string& new_title) {
  title_ = new_title;
  under_->setTitle(new_title);
}

// Windows::Node - Observable Properties

Windows::WindowType& Windows::Node::GetUnderlying() const { return *under_; }

Windows::SystemWindowHandleType Windows::Node::GetSystemHandle() const { return under_->getSystemHandle(); }

const std::pair<bool, Windows::Node*>& Windows::Node::IsFrozen() const { return frozen_; }

bool Windows::Node::IsAllocated() const { return under_ != nullptr; }

bool Windows::Node::IsMarkedForDestruction() const { return marked_for_destruction_; }

bool Windows::Node::IsOpen() const {
  assert(IsAllocated() && "[Wnd::IsOpen] Window is not allocated!");
  return under_->isOpen();
}

bool Windows::Node::IsFocused() const { return under_->hasFocus(); };

// Windows::Node -  Modification

void Windows::Node::Freeze(bool enable, Node* node) { frozen_ = {enable, node}; }

void Windows::Node::Freeze(Node* node) { frozen_ = {true, node}; }

void Windows::Node::Close() { under_->close(); }

void Windows::Node::SetDeallocCallback(const std::function<void()>& cb) { cbOnDealloc_ = cb; }

void Windows::Node::SetFrozenEvent(const std::function<void(const EventType&)>& cb) { cbFrozenEvent_ = cb; };

void Windows::Node::SetFocused() { return under_->requestFocus(); }

void Windows::Node::SetVSync(bool enabled) { under_->setVerticalSyncEnabled(enabled); }

void Windows::Node::SetFramerateLimit(unsigned int limit) { under_->setFramerateLimit(limit); }

bool Windows::Node::SetActive(bool active) const { return under_->setActive(active); }

void Windows::Node::SetIcon(unsigned int width, unsigned int height, const std::uint8_t* pixels) {
  under_->setIcon(width, height, pixels);
}

void Windows::Node::SetVisible(bool visible) { under_->setVisible(visible); };

void Windows::Node::SetMouseCursorVisible(bool visible) { under_->setMouseCursorVisible(visible); }

void Windows::Node::SetMouseCursorGrabbed(bool grabbed) { under_->setMouseCursorGrabbed(grabbed); }

void Windows::Node::SetMouseCursor(const sf::Cursor& cursor) { under_->setMouseCursor(cursor); }

void Windows::Node::SetKeyRepeatEnabled(bool enabled) { under_->setKeyRepeatEnabled(enabled); }

void Windows::Node::SetJoystickThreshold(float threshold) { under_->setJoystickThreshold(threshold); }

// Windows::Node - Functionality
void Windows::Node::Clear() { GetRenderBuffer()->clear(); }

void Windows::Node::Draw(const sf::Drawable& drawable) { GetRenderBuffer()->draw(drawable); }

void Windows::Node::Display() { under_->display(); }

bool Windows::Node::PollEvent(EventType& event) { return under_->pollEvent(event); }

bool Windows::Node::WaitEvent(EventType& event) { return under_->waitEvent(event); }

void Windows::Node::PushEventHandler(std::function<void(const EventType&)>&& handler) {
  event_handlers_.push_back(std::forward<std::function<void(const EventType&)>>(handler));
};

void Windows::Node::ClearEventHandlers() { event_handlers_.clear(); }

bool Windows::Node::CreateVulkanSurface(const VkInstance& instance, VkSurfaceKHR& surface,
                                       const VkAllocationCallbacks* allocator) {
  return under_->createVulkanSurface(instance, surface, allocator);
}

constexpr Windows::RenderBufferType* Windows::Node::GetRenderBuffer() {
  return static_cast<RenderBufferType*>(under_.get());
}

constexpr const Windows::RenderBufferType* Windows::Node::GetRenderBuffer() const {
  return static_cast<RenderBufferType*>(under_.get());
}

void Windows::Node::DefaultFrozenEvent(const EventType& ev) {
  // Upon any event in the frozen window, request focus to the freezer window if it exists.
  if (ev.type != sf::Event::Closed) {
    if (frozen_.second) frozen_.second->SetFocused();
  }
}

constexpr bool Windows::Node::operator==(const Node& other) const noexcept { return under_ == other.under_; }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Window - static windowing system */ 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Windows::Node* Windows::Create(const Hints& hints, Node* parent) {
  // Create a detached window with no parent
  if (!parent) {
    windows_.push_back(Node(hints));
    last_window_ = &windows_.back();
  }
  // Add as a subnode
  else {
    parent->append(Node(hints, parent));
    // **Parent must be set manually
    last_window_ = &parent->back();
  }
  return last_window_;
}

void Windows::Destroy(Node* node) {
  if (!node) return;
  // Recursivley mark all subnodes for destruction
  node->apply([](Node& win) { win.marked_for_destruction_ = true; });
}

void Windows::Destroy(const Node* node) {
  if (!node) return;
  // Recursivley mark all subnodes for destruction
  const_cast<Node*>(node)->apply([](Node& win) { win.marked_for_destruction_ = true; });
}

void Windows::ProcessEvents() {
  // Destroy any windows that are marked for destruction befroe the next processing frame.
  // Otherwise, mangling the window node graph during event handling may cause a null pointer exception.
  ApplyWindowRemoval();
  // For each root window. Process events or call frozen event handler.
  for (auto& wnd : windows_) {
    curr_window_ = &wnd;
    if (!wnd.IsFrozen().first)
      while (wnd.PollEvent(curr_event_))
        for (auto& handler : wnd.event_handlers_) handler(curr_event_);
    else
      while (wnd.PollEvent(curr_event_)) wnd.cbFrozenEvent_(curr_event_);

    // Recursivley process all child windows.
    if (!wnd.is_leaf())
      wnd.apply_branches([](Windows::Node& cwnd) {
        curr_window_ = &cwnd;
        if (!cwnd.IsFrozen().first)
          while (cwnd.PollEvent(curr_event_))
            for (auto& handler : cwnd.event_handlers_) handler(curr_event_);
        else
          while (cwnd.PollEvent(curr_event_)) cwnd.cbFrozenEvent_(curr_event_);
      });
  }  // end for
}

void Windows::RemoveWindows(Node* node) {
  if (!node) return;
  for (auto& subwin : node->branches() | std::views::reverse) {
    RemoveWindows(&subwin);
  }
  node->cbOnDealloc_();
  if (node->IsOpen()) node->Close();
  node->prune();
}

void Windows::RemoveSubwindows(Node* node) {
  if (!node) return;
  for (auto& subwin : node->branches() | std::views::reverse) RemoveWindows(&subwin);
}

void Windows::ApplyWindowRemoval(Node* node) {
  // Base case : Iterate over all root windows in reverse depth first preorder and remove those and their children
  // marked for destruction. If the node is not marked for destruction, apply recursive case to all branches.
  if (!node) {
    for (auto& wnd : windows_) {
      if (wnd.IsMarkedForDestruction()) {
        if (!wnd.is_leaf()) RemoveSubwindows(&wnd);
        wnd.cbOnDealloc_();
        if (wnd.IsOpen()) wnd.Close();
      } else {
        ApplyWindowRemoval(&wnd);
      }
    }
    windows_.remove_if([](const Node& wnd) { return wnd.IsMarkedForDestruction(); });
  }
  // Recursive case : Iterate over all branches of the current node and remove those marked for destruction.
  else {
    for (auto& subwin : node->branches()) {
      if (subwin.IsMarkedForDestruction()) {
        if (!subwin.is_leaf()) RemoveSubwindows(&subwin);
        subwin.cbOnDealloc_();
        if (subwin.IsOpen()) subwin.Close();
        subwin.prune();
      } else {
        ApplyWindowRemoval(&subwin);
      }
    }
    node->branches().remove_if([](const Node& wnd) { return wnd.IsMarkedForDestruction(); });
  }
}

}  // namespace caf::sys

/// @} // end of coreappfw

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: [CAF] Core Application Framework
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
// @created: 2025/07/13
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2025 Anton Yashchenko
//
// Licensed under the Apache License, Version 2.0(the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////