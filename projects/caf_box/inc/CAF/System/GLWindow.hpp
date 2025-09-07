///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2025 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: [CAF] Core Application Framework
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup core_app_framework
/// @brief [HEADER]
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup core_app_framework
/// @{

// clang-format off
#pragma once
#include "cxxx.hpp"
#include "CAF/PreConfig.hpp"
#include "CAF/Data/GLWindowHints.hpp"
// clang-format on

namespace caf {

class GLWindow {
 public:
  using RenderBufferType = sf::RenderTarget;
  using OsWindowHandleType = sf::WindowHandle;
  using UnderlyingWindowType = sf::RenderWindow;
  using OsEventType = sf::Event;
  using VideoModeType = sf::VideoMode;
  using OpenGlSettingsType = sf::ContextSettings;

  GLWindow();
  GLWindow(const GLWindowHints& hints);

  // Mutable properties
  sf::Vector2i Position() const;
  void Position(const sf::Vector2i& new_pos);
  sf::Vector2u Size() const;
  void Size(const sf::Vector2u& new_pos);
  const std::string& Title() const;
  void Title(const std::string& new_title);

  // Observable Properties
  UnderlyingWindowType& GetUnderlying() const;
  OsWindowHandleType GetSystemHandle() const;
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
 //   event_handlers_.clear();
  }
  void Freeze(bool enable, GLWindow* freezer = nullptr);
  void Freeze(GLWindow* freezer = nullptr);
  void Close();

  void SetDeallocCallback(const std::function<void()>& cb);
  void SetFrozenEvent(const std::function<void(const OsEventType&)>& cb);
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
  bool PollEvent(OsEventType& event);
  bool WaitEvent(OsEventType& event);
  bool CreateVulkanSurface(const VkInstance& instance, VkSurfaceKHR& surface,
                           const VkAllocationCallbacks* allocator = nullptr);

  constexpr bool operator==(const GLWindow& other) const noexcept;

 public:
  constexpr RenderBufferType* GetRenderBuffer();
  constexpr const RenderBufferType* GetRenderBuffer() const;
  void DefaultFrozenEvent(const OsEventType& ev);

 private:
  std::unique_ptr<UnderlyingWindowType> under_;
  std::string title_;
  bool marked_for_destruction_;
  bool frozen_{false};
  GLWindow* freezer_{nullptr};

  // Callbacks
  std::function<void()> cbOnDealloc_;
  std::function<void(const OsEventType&)> cbFrozenEvent_;
};

}  // namespace caf

/// @} // end of core_app_framework

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: [CAF] Core Application Framework
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
// @created: 2025/07/25
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
