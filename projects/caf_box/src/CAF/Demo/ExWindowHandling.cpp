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
/// @brief [SOURCE] Window handling example
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup core_app_framework
/// @{

// clang-format off
#include "CAF/Demo.hpp"

#include "cxxx.hpp"
#include "CAF/System/Windows.hpp"
// clang-format on

namespace caf::demo {

/// Demonstrates creation and destruction of detached and child windows.
/// Creates two empty windows with events:
///  - Press keyboard key [T] to spawn a detached window.
///  - Press keyboard key [Y] to spawn a child window.
///  - Press the [CLOSE] button in the window to close current window and all child windows.
/// Any new windows will also have the same events, demonstrating how to create and destroy recursive child windows.
int ExWindowHandling() {
  using caf::sys::Windows;
  // Usually you would declare this event handler outside the main function...
  struct exEventHandler {
    const Windows::Hints& hints;

    void operator()(const Windows::EventType& e) {
      // On key click
      if (e.type == sf::Event::KeyReleased) {
        // [T] -> Create a new detached window.
        if (e.key.code == sf::Keyboard::T) {
          Windows::Node* new_window = Windows::Create(hints);
          new_window->PushEventHandler(*this);
        }
        // [T] -> Create a new child window.
        if (e.key.code == sf::Keyboard::Y) {
          // Create new window
          Windows::Node* new_window = Windows::Create(hints, Windows::GetCurrent());
          new_window->PushEventHandler(*this);
        }
      }

      // [CLOSE] -> Close the current window, and all child windows, if not already closed.
      if (e.type == sf::Event::Closed) {
        Windows::GetCurrent()->apply([](Windows::Node& wnd) {
          if (wnd.IsOpen()) wnd.Close();
        });

        // If you wanted to only close the current window but keep child windows open, you would do:
        //    if (Windows::GetCurrent()) Windows::GetCurrent()->Close();

        // Besides closing the window. You can also completely remove all child windows from the window
        // system graph. Do this when a window is no longer needed, and you want to free up resources.
        // Note that when 'Destroy' is called here, actual de-allocation of the window
        // and removal from the graph will occur at the next Wnd::ProcessEvent call.
        Windows::Destroy(Windows::GetCurrent());  // Remove the current window.

        // You can check if a window is marked for destruction inside this current frame by calling:
        //    bool will_be_destroyed = Windows::GetCurrent()->MarkedForDestruction();
      }
    }
  };

  // You are able to create multiple detached 'individual' windows.
  Windows::Hints hints{};
  hints.InitialTitle = "A Window";
  hints.InitialWidth = 320;
  hints.InitialHeight = 146;
  hints.FrameLimit = 60;
  // Note the return type of window creation is not the OwnedWindow itself,
  // but the window's node inside the window system graph.
  Windows::Node* win = Windows::Create(hints);
  hints.InitialTitle = "Another Window";
  Windows::Node* other_win = Windows::Create(hints);

  // Add an event handler to the window.
  // Called per event that is processed for this window.
  // EmplaceEventHandler will accept any callable type with signature void(const caf::sys::Wnd::EventType&)
  win->PushEventHandler(exEventHandler{hints});
  other_win->PushEventHandler(exEventHandler{hints});

  while (!Windows::GetWindows().empty()) {
    // Process events for all windows in the system.
    // Window events are processed in depth first pre order traversal.
    Windows::ProcessEvents();
  }
  return 0;
}
}  // namespace caf::demo

/// @} // end of core_app_framework

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
