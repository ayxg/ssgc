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
/// @brief [SOURCE] Basic user interface example
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup core_app_framework
/// @{

// clang-format off
#include "CAF/Demo.hpp"

#include <thread> // std::this_thread
#include "cxxx.hpp"
#include "CAF/System/Windows.hpp"
// clang-format on

namespace caf::demo {
/// Demonstrates integration of a UI framework into the CAF system. In this case Dear ImGui.
/// - Shows how to properly initialize and destroy ImGui contexts linked to windows in the window node graph.
int ExBasicUserInterface() {
  using namespace caf::sys;

  struct exEventHandler {
    const Windows::Hints& hints;
    Windows::Node* win;
    void operator()(const Windows::EventType& e) {
      if (win->IsOpen()) caf::imgui::ProcessEvent(Windows::GetCurrent(), e);
      //  On key click
      if (e.type == sf::Event::KeyReleased) {
        // [T] -> Create a new detached window.
        if (e.key.code == sf::Keyboard::T) {
          Windows::Node* new_win = Windows::Create(hints);
          caf::imgui::Init(new_win, true);
          new_win->PushEventHandler(exEventHandler{hints, new_win});
          new_win->SetDeallocCallback([new_win]() { caf::imgui::Shutdown(new_win); });
        }
        // [Y] -> Create a new child window.
        if (e.key.code == sf::Keyboard::Y) {
          Windows::Node* new_win = Windows::Create(hints, Windows::GetCurrent());
          caf::imgui::Init(new_win, true);
          new_win->PushEventHandler(exEventHandler{hints, new_win});
          new_win->SetDeallocCallback([new_win]() { caf::imgui::Shutdown(new_win); });
        }
      }

      // [CLOSE] -> Close the current window, and all child windows, if not already closed.
      if (e.type == sf::Event::Closed) {
        Windows::Destroy(Windows::GetCurrent());
      }
    }
  };

  Windows::Hints hints{};
  hints.InitialTitle = "A Window";
  hints.InitialWidth = 500;
  hints.InitialHeight = 500;
  hints.FrameLimit = 60;
  Windows::Node* win = Windows::Create(hints);
  win->PushEventHandler(exEventHandler{hints, win});
  caf::imgui::Init(win, true);
  win->SetDeallocCallback([win]() { caf::imgui::Shutdown(win); });

  sf::Clock delta_clock{};
  sf::Time delta_time{};
  sf::CircleShape shape(100.f);
  shape.setFillColor(sf::Color::Green);

  while (!Windows::GetWindows().empty()) {
    Windows::ProcessEvents();
    delta_time = delta_clock.restart();

    // Update
    for (auto& lwnd : Windows::GetWindowsMutable()) {
      lwnd.apply([&delta_time](Windows::Node& w) {
        if (w.IsOpen() && !w.IsMarkedForDestruction()) {
          caf::imgui::Update(&w, delta_time);
          caf::imgui::SetCurrentWindow(&w);
          ImGui::SetNextWindowSize(w.Size());
          ImGui::SetNextWindowPos(ImVec2(0.f, 0.f));
          ImGui::Begin("Hello, Dear ImGui!");
          ImGui::Text("Click [T] for a new detached window.\nClick [Y] for a new child window.");
          ImGui::End();
        }
      });
    }

    // Render
    for (auto& lwnd : Windows::GetWindowsMutable()) {
      lwnd.apply([&shape](Windows::Node& w) {
        if (w.IsOpen() && !w.IsMarkedForDestruction()) {
          w.Clear();
          w.Draw(shape);
          caf::imgui::Render(&w);
          w.Display();
        }
      });
    }
  }
  // ImGui::SFML::Shutdown(); // This has to be called ??? idk maybe
  return EXIT_SUCCESS;
}
}  // namespace caf::demo

/// @} // end of core_app_framework

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: [CAF] Core Application Framework
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
// @created: 2025/07/15
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
