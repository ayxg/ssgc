///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2025 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: [CIDR] C& integrated development repository library
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup cidrlib
/// @brief Launcher window for CIDR executable.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup cidrlib_frontend
/// @{

#pragma once
#include "../Backend.hpp"
#include "../Common.hpp"
#include "../Data/Text/CidrApp.hpp"

namespace cidr {
struct LoadingSequence {
  static constexpr float kLoadingDone = 1.f;
  static constexpr float kLoadingError = -1.f;
  using LoadingStepFuncT = std::function<void(float&, std::string_view&)>;

  std::vector<LoadingStepFuncT> steps{};
  std::atomic<float> progress{0};
  std::atomic<std::string_view> label{""};
  bool failed{false};

  /// Creates a thread to run the loading steps in sequence.
  /// Use this pattern to join:
  ///   @code
  ///     if (loading_thread->joinable() && load_seq.IsDone())
  ///       loading_thread->join();
  ///   @endcode
  std::unique_ptr<std::thread> Dispatch() {
    return std::make_unique<std::thread>([this]() { Run(); });
  }

  /// Executes the loading steps in sequence. Usually passed to a thread or use LoadingSequence::Dispatch().
  /// Progress is clamped 0.f to 1.f after each step. If the progress value is set to LoadingSequence::kLoadingError
  /// the sequence will exit early and member var 'failed' will be set to true.
  void Run() {
    float prog = progress.load();
    std::string_view lbl = label.load();
    for (auto& step : steps) {
      step(prog, lbl);
      progress.store(std::clamp(prog, 0.f, 1.f));
      label.store(lbl);
    }
  }

  /// Checks if the loading sequence is done. Returns true if progress is 1.f.
  /// This will not check if every step has been executed yet.
  bool IsDone() const { return progress.load() >= 1.f; }
};

struct LoadingStep {
  float min_progress{};
  float max_progress{};
  std::string_view start_label{""};
  virtual void Load(float& prog, std::string_view& lbl) = 0;

 protected:
  LoadingStep& operator()(float& prog, std::string_view& lbl) {
    prog = std::clamp(prog, min_progress, max_progress);
    lbl = start_label;
    Load(prog, lbl);
    prog = std::clamp(prog, min_progress, max_progress);
    return *this;
  }
};


}
namespace cidr::ui {


// Displays a basic loading screen with a loading back.
// Callbacks:
// - cbGetLoadingProgress: Returns a float value between 0.0 and 1.0 indicating the loading progress.
// - cbGetLoadingLabel: Returns a string_view with the current loading label to display.
class LoadingScreen {
 public:
  LoadingScreen() {
    subwindow_.Flags().Append(
        cgui::eWindowFlags::ImGuiWindowFlags_NoDecoration, cgui::eWindowFlags::ImGuiWindowFlags_NoMove,
        cgui::eWindowFlags::ImGuiWindowFlags_NoResize, cgui::eWindowFlags::ImGuiWindowFlags_NoScrollbar,
        cgui::eWindowFlags::ImGuiWindowFlags_NoResize, cgui::eWindowFlags::ImGuiWindowFlags_NoTitleBar);
  };
  void Init(caf::Context& ctx) {
     caf::WinHints hints{};
     hints.InitialWidth = 320;
     hints.InitialHeight = 146;
     hints.NoTitleBar = true;
     hints.EnableImGui = true;
     hints.FrameLimit = 60;
     ctx.Create(caf::Window{"C.I.D.R. Loading...", hints});
     auto& wnd = ctx.Last();
     wnd.value().SetPos((sf::VideoMode::getDesktopMode().width / 2) - (wnd.value().GetSize().x / 2),
                        (sf::VideoMode::getDesktopMode().height / 2) - (wnd.value().GetSize().y / 2));
     //theme.Apply(ImGui::GetStyle()); 
  
  }
  void Display(caf::Window& win, sf::Time dt) {
    static int loading_dots = 1;
    static float dot_anim = loading_dots;
    ImGui::SetNextWindowSize(win.Underlying().getSize());
    ImGui::SetNextWindowPos({0, 0});
    if (subwindow_.BeginLate()) {
      if (subcontext_.BeginLate()) {
        ImVec2 textPos = ImGui::GetCursorScreenPos();
        ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), 18.0f, textPos, IM_COL32(0, 0, 0, 255),
                                            data::text::kAppTitle.data());
        ImGui::Dummy(ImVec2(0, 18.0f));
        ImGui::Text(data::text::kAppDesc.data());
        ImGui::Separator();
        ImGui::ProgressBar(std::clamp(cbGetLoadingProgress(), 0.f, 1.f), ImVec2(-FLT_MIN, 0), "");
        dot_anim += 3 * dt.asSeconds();
        loading_dots = static_cast<int>(dot_anim) % 4;
        std::string loading_label_ = std::format("Loading : {} {}", cbGetLoadingLabel() , std::string(loading_dots, '.'));
        ImGui::Text("%s", loading_label_.data());
      }
      subcontext_.EndEarly();
    }
    subwindow_.EndEarly();
  }
 private:
  CguiNamedSubcontext subcontext_{
      CguiNamedSubcontext::Delayed("###LoadingScreen", cgui::kExpandWidgetToRemainingSpaceXY)};
  CguiWindow subwindow_{CguiWindow::Delayed("Loading Screen")};

 public:
  std::function<float()> cbGetLoadingProgress{[]() { return 0.f; }};
  std::function<std::string_view()> cbGetLoadingLabel{[]() { return ""; }};
};

struct LoadingScreenController {

  LoadingScreen& view_;
  LoadingSequence* load_seq_{};
  LoadingScreenController(LoadingScreen& view)
      : view_(view) {
    view_.cbGetLoadingProgress = [this]() -> float { return load_seq_? load_seq_->progress.load() : 0.f; };
    view_.cbGetLoadingLabel = [this]() -> std::string_view { return load_seq_ ? load_seq_->label.load() : ""; };
  }

};

}  // namespace cidr::ui

/// @} // end of cidrlib_frontend

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: [CIDR] C& integrated development repository library
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
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
