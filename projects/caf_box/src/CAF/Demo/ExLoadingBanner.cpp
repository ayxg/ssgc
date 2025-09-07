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
/// @brief [SOURCE] Loading banner example
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup core_app_framework
/// @{

// clang-format off
#include "CAF/Demo.hpp"

#include <thread> // std::this_thread
#include "cxxx.hpp"
#include "CAF/System/Windows.hpp"
#include "CAF/Tool/ProgressSequence.hpp"
// clang-format on

namespace caf::demo {

/// Demonstrates how to use the ProgressSequence and ImGui class to create an multithreaded loading progress animation.
/// This specific example show a loading banner for some 'Foo' application, but the same pattern may be used to display
/// or query progress of other program events asynchronously.
int ExLoadingBanner() {
  using caf::sys::Windows;

  static constexpr std::array<std::string_view, 5> kLoadingLabels{"something", "another thing", "resources",
                                                                  "program data", "user data"};

  caf::ProgressSequence init_load_seq{[](float& prog, std::string_view& lbl) {
                                        lbl = kLoadingLabels[0];
                                        prog = 0.2f;
                                      },
                                      [](float& prog, std::string_view& lbl) {
                                        std::this_thread::sleep_for(std::chrono::seconds(2));
                                        lbl = kLoadingLabels[1];
                                        prog = 0.4f;
                                      },
                                      [](float& prog, std::string_view& lbl) {
                                        std::this_thread::sleep_for(std::chrono::seconds(2));
                                        lbl = kLoadingLabels[2];
                                        prog = 0.6f;
                                      },
                                      [](float& prog, std::string_view& lbl) {
                                        std::this_thread::sleep_for(std::chrono::seconds(2));
                                        lbl = kLoadingLabels[3];
                                        prog = 0.8f;
                                      },
                                      [](float& prog, std::string_view& lbl) {
                                        std::this_thread::sleep_for(std::chrono::seconds(2));
                                        lbl = kLoadingLabels[4];
                                        prog = ProgressSequence::kDone;
                                      }};

  // Start loading before the window is created, on a separate thread.
  std::unique_ptr<std::thread> loading_thread{init_load_seq.Dispatch()};

  // Create the loading screen window.
  Windows::Hints hints{};
  hints.InitialTitle = "Loading...";
  hints.InitialWidth = 320;
  hints.InitialHeight = 146;
  hints.NoTitleBar = true;
  hints.FrameLimit = 60;
  Windows::Node* win = Windows::Create(hints);
  if(!caf::imgui::Init(win))  return 1;
  win->PushEventHandler([win](const auto & e) {
    if (win->IsOpen()) caf::imgui::ProcessEvent(Windows::GetCurrent(), e);
  });
  win->SetDeallocCallback([win]() { caf::imgui::Shutdown(win);
    });

  // Link the ui to the backend.
  std::function<float()> cbGetLoadingProgress = [&init_load_seq]() -> float { return init_load_seq.Progress(); };

  std::function<std::string_view()> cbGetLoadingLabel = [&init_load_seq]() -> std::string_view {
    return init_load_seq.Label();
  };

  // Flag - We will turn this off once loading thread has joined.
  bool flag_loading{true};

  // Main loop
  sf::Clock delta_timer{};
  sf::Time frame_delta{};
  while (!Windows::GetWindows().empty()) {
    // Close the loading window once finished.
    // Note that this must be called before Windows::ProcessEvents().
    if (!flag_loading) {
      Windows::Destroy(win);
    }

    Windows::ProcessEvents();
    frame_delta = delta_timer.restart();

    // Join the loading thread once its complete.
    if (flag_loading) {
      if (loading_thread->joinable() && init_load_seq.IsDone()) {
        loading_thread->join();
        flag_loading = false;
      }
    }

    // Update ui.
    if (Windows::IsAvailable(win)) {
      static int loading_dots = 1;
      static float dot_anim = loading_dots;
      caf::imgui::SetCurrentWindow(win);
      caf::imgui::Update(win,frame_delta);
      ImGui::SetNextWindowSize(win->Size());
      ImGui::SetNextWindowPos({0, 0});
      if (ImGui::Begin("###BannerWindow")) {
        if (ImGui::BeginChild("###BannerLayout")) {
          ImVec2 textPos = ImGui::GetCursorScreenPos();
          ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), 18.0f, textPos, IM_COL32(255, 255, 255, 255),
                                              "Foo Application");
          ImGui::Dummy(ImVec2(0, 18.0f));
          ImGui::Text("Application description.");
          ImGui::Separator();
          ImGui::ProgressBar(std::clamp(cbGetLoadingProgress(), 0.f, 1.f), ImVec2(-FLT_MIN, 0), "");
          dot_anim += 3 * (frame_delta.asMicroseconds() * 0.001);
          loading_dots = static_cast<int>(dot_anim) % 4;
          std::string loading_label_ =
              std::format("Loading : {} {}", cbGetLoadingLabel(), std::string(loading_dots, '.'));
          ImGui::Text("%s", loading_label_.data());
        }
        ImGui::EndChild();
      }
      ImGui::End();
    }

    // Render
    if (Windows::IsAvailable(win)) {
      win->Clear();
      caf::imgui::Render(win);
      win->Display();
    }

  }
  return EXIT_SUCCESS;
  return 0;
}
}  // namespace caf::demo

/// @} // end of core_app_framework

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: [CAF] Core Application Framework
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
// @created: 2025/07/20
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
