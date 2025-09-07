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
/// @brief [HEADER] Progress Sequence class ,used to observe the state(progress) of a sequence of labeled operations
///        in a separate thread. The current progress percent(floating 0.0-1.0) and label(std:string_view) are atomic.
///        Also includes the ProgressStep utility virtual functor base for creating sequence steps(optional).
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup coreappfw
/// @{

// clang-format off
#pragma once
#include <atomic>
#include <functional>
#include <string_view>
#include <vector>
#include <memory>
#include <thread>
#include <algorithm>
// clang-format on

namespace caf {
class ProgressSequence {
 public:
  static constexpr float kDone = 1.f;
  static constexpr float kError = -1.f;
  using LoadingStepFuncT = std::function<void(float&, std::string_view&)>;

  ProgressSequence() {}
  template <typename... Ts>
    requires std::conjunction_v<std::is_constructible<LoadingStepFuncT, Ts>...>
  ProgressSequence(Ts&&... args) : steps({args...}) {}

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

  float Progress() const { return progress.load(); }

  std::string_view Label() const { return label.load(); }

 private:
  std::vector<LoadingStepFuncT> steps{};
  std::atomic<float> progress{0};
  std::atomic<std::string_view> label{""};
  bool failed{false};
};

struct ProgressStep {
  float min_progress{};
  float max_progress{};
  std::string_view start_label{""};
  virtual void Load(float& prog, std::string_view& lbl) = 0;

 protected:
  ProgressStep& operator()(float& prog, std::string_view& lbl) {
    prog = std::clamp(prog, min_progress, max_progress);
    lbl = start_label;
    Load(prog, lbl);
    prog = std::clamp(prog, min_progress, max_progress);
    return *this;
  }
};

}  // namespace caf

/// @} // end of coreappfw

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: [CAF] Core Application Framework
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
// @created: 2025/05/15
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
