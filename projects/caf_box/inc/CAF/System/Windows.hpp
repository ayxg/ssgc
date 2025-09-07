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
/// @brief [HEADER] Static windowing system.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup coreappfw
/// @{

// clang-format off
#pragma once
#include "cxxx.hpp"
#include "CAF/PreConfig.hpp"
// clang-format on

namespace caf::sys {

/// @class Windows
/// @brief Static windowing system. Manages creation and destruction of windows a list of detached window node graphs.
///
/// Windows are stored in a doubly linked list where each root window and all sub-windows are doubly linked tree node
/// graphs. That means a pointer to a window node will stay valid as long as the window exists in the system.
///
/// Window Creation :
///   You should only create a window using Window::Create which adds the node into the total node graph and assigns a
///   parent if needed.
///
/// Window Destruction :
///   Window destruction is a two-phase process to allow the user to trigger de-initialization logic before full
///   de-allocation of the window node. Using Window::Destroy marks a window and all it's children as marked for
///   destruction. The actual de-allocation occurs upon the next call to Window::ProcessEvents, before the events are
///   polled in the method.
class Windows {
 public:
  using RenderBufferType = sf::RenderTarget;
  using SystemWindowHandleType = sf::WindowHandle;
  using WindowType = sf::RenderWindow;
  using EventType = sf::Event;
  using VideoModeType = sf::VideoMode;
  using OpenGlSettingsType = sf::ContextSettings;

  /// Parameters passable to a window upon creation.
  struct Hints {
    std::optional<int> RequestFocus{std::nullopt};      // Request focus
    std::optional<int> Hidden{std::nullopt};            // Window is hidden
    std::optional<int> Fullscreen{std::nullopt};        // Fullscreen window
    std::optional<int> NoTitleBar{std::nullopt};        // No title bar
    std::optional<int> NoResize{std::nullopt};          // Window is resizable
    std::optional<int> NoCloseButton{std::nullopt};     // Show close button
    std::optional<int> InitialWidth{std::nullopt};      // Initial window width
    std::optional<int> InitialHeight{std::nullopt};     // Initial window height
    std::optional<int> InitialPositionX{std::nullopt};  // Initial window width
    std::optional<int> InitialPositionY{std::nullopt};  // Initial window height
    std::optional<int> FrameLimit{std::nullopt};        // default is FalseInt; which means no limit
    std::optional<int> EnableVsync{std::nullopt};       // Enable vertical sync

    std::optional<int> GlDepthBits{std::nullopt};
    std::optional<int> GlAntialiasingLevel{std::nullopt};
    std::optional<int> GlStencilBits{std::nullopt};
    std::optional<int> GlMajorVersion{std::nullopt};
    std::optional<int> GlMinorVersion{std::nullopt};
    std::optional<int> GlAttributeFlags{std::nullopt};  // One of sf::ContextSettings::AttributeFlags
    std::optional<int> GlsRgbCapable{std::nullopt};

    std::optional<std::string_view> InitialTitle{std::nullopt};
  };

  /// Owned pointer to an underlying live system window object part of the window node graph.
  class Node : public cxd::list_node_base<Node> {
   public:
    friend Windows;
    Node() = default;
    Node(Node* parent = nullptr);
    Node(const Hints& hints, Node* parent = nullptr);

   public:
    // Mutable properties
    sf::Vector2i Position() const;
    void Position(const sf::Vector2i& new_pos);
    sf::Vector2u Size() const;
    void Size(const sf::Vector2u& new_pos);
    const std::string& Title() const;
    void Title(const std::string& new_title);

    // Observable Properties
    WindowType& GetUnderlying() const;
    SystemWindowHandleType GetSystemHandle() const;
    bool IsFrozen() const;
    bool IsAllocated() const;
    bool IsMarkedForDestruction() const;
    bool IsOpen() const;
    bool IsFocused() const;

    // Modification
    void Reset() {
      under_.reset();
      marked_for_destruction_ = true;
      frozen_ = false;
      freezer_ = nullptr;
      event_handlers_.clear();
    }
    void Freeze(bool enable, Node* node = nullptr);
    void Freeze(Node* node = nullptr);
    void Close();

    void SetDeallocCallback(const std::function<void()>& cb);
    void SetFrozenEvent(const std::function<void(const EventType&)>& cb);
    void SetFocused();
    void SetVSync(bool enabled);
    void SetFramerateLimit(unsigned int limit);
    bool SetActive(bool active = true) const;
    void SetSize(const sf::Vector2u& size);
    void SetPosition(const sf::Vector2i& position);
    void SetTitle(const std::string& title);
    void SetIcon(unsigned int width, unsigned int height, const std::uint8_t* pixels);
    void SetVisible(bool visible);
    void SetMouseCursorVisible(bool visible);
    void SetMouseCursorGrabbed(bool grabbed);
    void SetMouseCursor(const sf::Cursor& cursor);
    void SetKeyRepeatEnabled(bool enabled);
    void SetJoystickThreshold(float threshold);

    // Functionality
    void Clear();
    void Draw(const sf::Drawable& drawable);
    void Display();
    bool PollEvent(EventType& event);
    bool WaitEvent(EventType& event);
    void PushEventHandler(std::function<void(const EventType&)>&& handler);
    void ClearEventHandlers();
    bool CreateVulkanSurface(const VkInstance& instance, VkSurfaceKHR& surface,
                             const VkAllocationCallbacks* allocator = nullptr);

    constexpr bool operator==(const Node& other) const noexcept;

   public:
    constexpr RenderBufferType* GetRenderBuffer();
    constexpr const RenderBufferType* GetRenderBuffer() const;
    void DefaultFrozenEvent(const EventType& ev);

   private:
    std::unique_ptr<WindowType> under_;
    std::string title_;
    bool marked_for_destruction_;
    bool frozen_{false};
    Node* freezer_{nullptr};
    std::vector<std::function<void(const EventType&)>> event_handlers_;

    // Callbacks
    std::function<void()> cbOnDealloc_;
    std::function<void(const EventType&)> cbFrozenEvent_;
  };

  /// Create a new window with the given hints and add it the window node graph.
  /// If parent is not provided, the window will be added as a 'detached' window to the root window list.
  static Node* Create(const Hints& hints, Node* parent = nullptr);

  /// Process all window events, starting from the first created detached window. Process the window
  /// events then recursivley process all subwindows before moving to the next detached window.
  static void ProcessEvents();

  // Get the current window for which events are being processed. Only significant inside a
  // window's event handlers.
  static constexpr Node* GetCurrent() { return curr_window_; }

  // Get the last window which was created using Windows::Create.
  static constexpr Node* GetLast() { return last_window_; }

  // Get a view of all windows in the window graph.
  static const std::list<Node>& GetWindows() { return windows_; }

  // Get a view of all windows in the window graph.
  static std::list<Node>& GetWindowsMutable() { return windows_; }

  /// Remove a window node from the window processing graph and destroy it and it's sub-windows.
  /// The destruction occurs before the start of the next event processing loop.
  /// You can check if a window was marked for destruction using Wnd::OwnedWindow::MarkedForDestruction
  static void Destroy(Node* node);
  static void Destroy(const Node* node);

  /// Check if a window exists in the window graph. If you created a window with Create and then destroyed it using
  /// Destroy the raw you pointer you have will be pointing to garbage, not nullptr. So the only way to properly check
  /// if the window pointer still exists is to use this method.
  static bool Exists(Node* pnode);
  static bool Exists(const Node* pnode);

  /// Node exits. Node is not MarkedForDestruction. Window is open.
  static bool IsAvailable(Node* pnode);
  static bool IsAvailable(const Node* pnode);

  static bool IsGraphDirty();
 private:
  /// Recursively removes all sub-windows in reverse order then removes the node itself from the graph.
  static void RemoveWindows(Node* node);

  /// Recursively removes all sub-windows in reverse order from the graph.
  static void RemoveSubwindows(Node* node);

  /// Called only inside Wnd::ProcessEvents before handling events to remove windows that were marked for destruction.
  static void ApplyWindowRemoval(Node* node = nullptr);

 private:
  static std::list<Node> windows_;                       // Detached window node graphs list.
  static Node* last_window_;                             // Last created window by Windows::Create.
  static Node* curr_window_;                             // Current window being processed by Windows::ProcessEvents.
  static EventType curr_event_;                          // Current event being processed by Windows::ProcessEvents.
  static std::unordered_set<const Node*> live_windows_;  // Allows user to query invalidated/deleted windows.
  static bool graph_dirty_;
  static bool graph_changing_;
};

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