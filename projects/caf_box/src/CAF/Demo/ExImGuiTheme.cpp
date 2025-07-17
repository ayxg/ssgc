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
int ExImGuiTheme() {
  using namespace caf::sys;
  caf::imgui::Theme t{caf::imgui::Theme::DefaultLight()};
  t.font_path = "font/DroidSans/DroidSans.ttf";
  t.font_size = 15.f;

  // Rounding
  t.style->WindowRounding = 1.0f;
  t.style->ChildRounding = 1.0f;
  t.style->PopupRounding = 1.0f;
  t.style->FrameRounding = 1.0f;
  t.style->ScrollbarRounding = 1.0f;
  t.style->GrabRounding = 1.0f;
  t.style->TabRounding = 1.0f;

  // Borders
  t.style->WindowBorderSize = 1.0f;
  t.style->ChildBorderSize = 1.0f;
  t.style->PopupBorderSize = 1.0f;
  t.style->FrameBorderSize = 1.0f;
  t.style->TabBorderSize = 1.0f;
  t.style->TabBarBorderSize = 1.0f;
  t.style->TabBarOverlineSize = 2.0f;

  // Colors (Generated code)
  ImVec4* colors = t.style->Colors;
  colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
  colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
  colors[ImGuiCol_WindowBg] = ImVec4(0.98f, 0.95f, 0.94f, 1.00f);
  colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
  colors[ImGuiCol_PopupBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.98f);
  colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.30f);
  colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.04f);
  colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.85f, 0.65f, 0.10f);
  colors[ImGuiCol_FrameBgHovered] = ImVec4(0.16f, 0.85f, 0.65f, 0.04f);
  colors[ImGuiCol_FrameBgActive] = ImVec4(0.16f, 0.85f, 0.65f, 0.48f);
  colors[ImGuiCol_TitleBg] = ImVec4(0.83f, 0.94f, 0.91f, 1.00f);
  colors[ImGuiCol_TitleBgActive] = ImVec4(0.58f, 0.94f, 0.83f, 1.00f);
  colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
  colors[ImGuiCol_MenuBarBg] = ImVec4(0.27f, 0.52f, 0.45f, 0.26f);
  colors[ImGuiCol_ScrollbarBg] = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
  colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.85f, 0.85f, 0.85f, 0.80f);
  colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.16f, 0.85f, 0.65f, 0.48f);
  colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.16f, 0.85f, 0.65f, 0.30f);
  colors[ImGuiCol_CheckMark] = ImVec4(0.16f, 0.85f, 0.65f, 1.00f);
  colors[ImGuiCol_SliderGrab] = ImVec4(0.16f, 0.85f, 0.65f, 0.53f);
  colors[ImGuiCol_SliderGrabActive] = ImVec4(0.27f, 0.52f, 0.45f, 1.00f);
  colors[ImGuiCol_Button] = ImVec4(0.16f, 0.85f, 0.65f, 0.26f);
  colors[ImGuiCol_ButtonHovered] = ImVec4(0.16f, 0.85f, 0.65f, 0.48f);
  colors[ImGuiCol_ButtonActive] = ImVec4(0.16f, 0.85f, 0.65f, 1.00f);
  colors[ImGuiCol_Header] = ImVec4(0.16f, 0.85f, 0.65f, 0.48f);
  colors[ImGuiCol_HeaderHovered] = ImVec4(0.16f, 0.85f, 0.65f, 0.48f);
  colors[ImGuiCol_HeaderActive] = ImVec4(0.16f, 0.85f, 0.65f, 1.00f);
  colors[ImGuiCol_Separator] = ImVec4(0.39f, 0.39f, 0.39f, 0.62f);
  colors[ImGuiCol_SeparatorHovered] = ImVec4(0.16f, 0.85f, 0.27f, 1.00f);
  colors[ImGuiCol_SeparatorActive] = ImVec4(0.85f, 0.31f, 0.16f, 1.00f);
  colors[ImGuiCol_ResizeGrip] = ImVec4(0.35f, 0.35f, 0.35f, 0.17f);
  colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.16f, 0.85f, 0.27f, 1.00f);
  colors[ImGuiCol_ResizeGripActive] = ImVec4(0.85f, 0.31f, 0.16f, 1.00f);
  colors[ImGuiCol_TabHovered] = ImVec4(0.16f, 0.85f, 0.65f, 1.00f);
  colors[ImGuiCol_Tab] = ImVec4(0.16f, 0.85f, 0.65f, 0.26f);
  colors[ImGuiCol_TabSelected] = ImVec4(0.16f, 0.85f, 0.65f, 0.48f);
  colors[ImGuiCol_TabSelectedOverline] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
  colors[ImGuiCol_TabDimmed] = ImVec4(0.92f, 0.93f, 0.94f, 0.99f);
  colors[ImGuiCol_TabDimmedSelected] = ImVec4(0.74f, 0.82f, 0.91f, 1.00f);
  colors[ImGuiCol_TabDimmedSelectedOverline] = ImVec4(0.26f, 0.59f, 1.00f, 1.00f);
  colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
  colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
  colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
  colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.45f, 0.00f, 1.00f);
  colors[ImGuiCol_TableHeaderBg] = ImVec4(0.16f, 0.85f, 0.65f, 0.48f);
  colors[ImGuiCol_TableBorderStrong] = ImVec4(0.57f, 0.57f, 0.64f, 1.00f);
  colors[ImGuiCol_TableBorderLight] = ImVec4(0.68f, 0.68f, 0.74f, 1.00f);
  colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
  colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.30f, 0.30f, 0.30f, 0.09f);
  colors[ImGuiCol_TextLink] = ImVec4(0.27f, 0.52f, 0.45f, 1.00f);
  colors[ImGuiCol_TextSelectedBg] = ImVec4(0.16f, 0.85f, 0.65f, 0.48f);
  colors[ImGuiCol_DragDropTarget] = ImVec4(0.16f, 0.85f, 0.65f, 0.95f);
  colors[ImGuiCol_NavHighlight] = ImVec4(0.16f, 0.85f, 0.65f, 0.94f);
  colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.70f, 0.70f, 0.70f, 0.70f);
  colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.20f);
  colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);

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
  win->PushEventHandler([](auto& e) {
    if (win->IsOpen()) caf::imgui::ProcessEvent(Windows::GetCurrent(), e)
    if (e.type == sf::Event::Closed) {
        Windows::Destroy(Windows::GetCurrent());
      }
  });
  caf::imgui::Init(win, true);
  win->SetDeallocCallback([win]() { caf::imgui::Shutdown(win); });



  t.Apply(ImGui::GetStyle());

  // main loop
  sf::Clock delta_clock{};
  sf::Time delta_time{};
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
