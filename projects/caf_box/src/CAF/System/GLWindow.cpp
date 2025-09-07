///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2025 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
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
#include "CAF/System/GLWindow.hpp"
#include "cxxx.hpp"
#include "CAF/PreConfig.hpp"
#include "CAF/Data/GLWindowHints.hpp"
namespace caf {

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* GLWindow */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

GLWindow::GLWindow()
    : under_(std::make_unique<UnderlyingWindowType>(VideoModeType::getDesktopMode(), "")),
      title_(""),
      marked_for_destruction_(false),
      frozen_(false),
      freezer_(nullptr),
      cbOnDealloc_([]() {}) {
  cbFrozenEvent_ = [this](const OsEventType& ev) { DefaultFrozenEvent(ev); };
}

GLWindow::GLWindow(const GLWindowHints& hints)
    : marked_for_destruction_(false), frozen_(false), freezer_(nullptr), cbOnDealloc_([]() {}) {
  cbFrozenEvent_ = [this](const OsEventType& ev) { DefaultFrozenEvent(ev); };

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
  under_ = std::make_unique<UnderlyingWindowType>(video_mode, sf::String{title_.data()}, style, gl_settings);

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


///////////////////////////////////////////////////////////////////////////////
/* Mutable properties */
///////////////////////////////////////////////////////////////////////////////

sf::Vector2i GLWindow::Position() const { return under_->getPosition(); }

void GLWindow::Position(const sf::Vector2i& new_pos) { under_->setPosition(new_pos); }

sf::Vector2u GLWindow::Size() const { return under_->getSize(); }

void GLWindow::Size(const sf::Vector2u& new_pos) { under_->setSize(new_pos); }

const std::string& GLWindow::Title() const { return title_; }

void GLWindow::Title(const std::string& new_title) {
  title_ = new_title;
  under_->setTitle(new_title);
}

constexpr GLWindow::RenderBufferType* GLWindow::GetRenderBuffer() {
  return static_cast<RenderBufferType*>(under_.get());
}

constexpr const GLWindow::RenderBufferType* GLWindow::GetRenderBuffer() const {
  return static_cast<RenderBufferType*>(under_.get());
}


///////////////////////////////////////////////////////////////////////////////
/* Observable Properties */
///////////////////////////////////////////////////////////////////////////////
GLWindow::UnderlyingWindowType& GLWindow::GetUnderlying() const { return *under_; }

GLWindow::OsWindowHandleType GLWindow::GetSystemHandle() const {
  assert(IsAllocated() && "[Wnd::GetSystemHandle] Window is not allocated!");
  return under_->getSystemHandle();
}

bool GLWindow::IsFrozen() const { return frozen_; }

bool GLWindow::IsAllocated() const { return under_ != nullptr; }

bool GLWindow::IsMarkedForDestruction() const { return marked_for_destruction_; }

bool GLWindow::IsOpen() const {
  assert(IsAllocated() && "[Wnd::IsOpen] Window is not allocated!");
  return under_->isOpen();
}

bool GLWindow::IsFocused() const { return under_->hasFocus(); };


///////////////////////////////////////////////////////////////////////////////
/* Modification */
///////////////////////////////////////////////////////////////////////////////

void GLWindow::Freeze(bool enable, GLWindow* freezer) {
  if (enable) {
    frozen_ = true;
    freezer_ = freezer;
    cbFrozenEvent_ = [this](const OsEventType& ev) { DefaultFrozenEvent(ev); };
  } else {
    frozen_ = false;
    freezer_ = nullptr;
    cbFrozenEvent_ = [](const OsEventType&) {};
  }
}

// Freeze this window setting the passed 'freezer' window as the owner of the freeze.
void GLWindow::Freeze(GLWindow* freezer) { Freeze(true, freezer); }

void GLWindow::Close() { under_->close(); }

void GLWindow::SetDeallocCallback(const std::function<void()>& cb) { cbOnDealloc_ = cb; }

void GLWindow::SetFrozenEvent(const std::function<void(const OsEventType&)>& cb) { cbFrozenEvent_ = cb; };

void GLWindow::SetFocused() { return under_->requestFocus(); }

void GLWindow::SetVSync(bool enabled) { under_->setVerticalSyncEnabled(enabled); }

void GLWindow::SetFramerateLimit(unsigned int limit) { under_->setFramerateLimit(limit); }

bool GLWindow::SetActive(bool active) const { return under_->setActive(active); }

void GLWindow::SetIcon(unsigned int width, unsigned int height, const std::uint8_t* pixels) {
  under_->setIcon(width, height, pixels);
}

void GLWindow::SetVisible(bool visible) { under_->setVisible(visible); };

void GLWindow::SetMouseCursorVisible(bool visible) { under_->setMouseCursorVisible(visible); }

void GLWindow::SetMouseCursorGrabbed(bool grabbed) { under_->setMouseCursorGrabbed(grabbed); }

void GLWindow::SetMouseCursor(const sf::Cursor& cursor) { under_->setMouseCursor(cursor); }

void GLWindow::SetKeyRepeatEnabled(bool enabled) { under_->setKeyRepeatEnabled(enabled); }

void GLWindow::SetJoystickThreshold(float threshold) { under_->setJoystickThreshold(threshold); }


///////////////////////////////////////////////////////////////////////////////
/* Functionality */
///////////////////////////////////////////////////////////////////////////////

void GLWindow::Clear() { GetRenderBuffer()->clear(); }

void GLWindow::Draw(const sf::Drawable& drawable) { GetRenderBuffer()->draw(drawable); }

void GLWindow::Display() { under_->display(); }

bool GLWindow::PollEvent(OsEventType& event) { return under_->pollEvent(event); }

bool GLWindow::WaitEvent(OsEventType& event) { return under_->waitEvent(event); }

bool GLWindow::CreateVulkanSurface(const VkInstance& instance, VkSurfaceKHR& surface,
                                        const VkAllocationCallbacks* allocator) {
  return under_->createVulkanSurface(instance, surface, allocator);
}

void GLWindow::DefaultFrozenEvent(const OsEventType& ev) {
  // Upon any event in the frozen window, request focus to the freezer window if it exists.
  if (ev.type != sf::Event::Closed) {
    if (freezer_) freezer_->SetFocused();
  }
}

constexpr bool GLWindow::operator==(const GLWindow& other) const noexcept { return under_ == other.under_; }

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
// Licensed under the GNU Affero General Public License, Version 3.
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