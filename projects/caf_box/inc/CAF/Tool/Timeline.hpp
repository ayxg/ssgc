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
/// @brief [HEADER] Timeline class
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup coreappfw
/// @{

// clang-format off
#pragma once
#include <chrono>
#include <vector>
// clang-format on

namespace caf {

/// A timer which tracks a timeline with pause/resume functionality.
/// Use case : for timed events such as animations.
class Timeline {
 public:
  using ClockType = std::chrono::steady_clock;
  using TimePointType = std::chrono::time_point<ClockType>;
  using MicroDurationType = std::chrono::duration<double, std::micro>;

 private:
  TimePointType epoch_point_;                   ///< Time of construction, in microseconds.>
  TimePointType reset_point_;                   ///< Time of last reset, in microseconds.>
  std::vector<TimePointType> pause_points_;     ///< Pause/Unpause points always in that order.>
  bool was_paused_;                             ///< True if the clock was paused or unpause at any point.>

 public:
  Timeline() : epoch_point_(ClockType::now()), reset_point_(ClockType::now()), pause_points_(), was_paused_(false) {}

  MicroDurationType Pause();

  MicroDurationType Reset();

  [[nodiscard]] bool IsPaused();

  [[nodiscard]] MicroDurationType ElapsedTime() const;

  [[nodiscard]] MicroDurationType TotalTime() const;

  [[nodiscard]] MicroDurationType RunningTime() const;
};  /// <@endclass:gtlTimeframe>


}  // namespace caf

/// @} // end of coreappfw

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: [CAF] Core Application Framework
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
// @created: 2025/05/09
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
