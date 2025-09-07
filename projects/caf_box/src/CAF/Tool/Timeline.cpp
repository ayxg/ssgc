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
/// @brief [SOURCE] Timeline class
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup coreappfw
/// @{

// clang-format off
#include "CAF/Tool/Timeline.hpp"
#include <chrono>
#include <vector>
// clang-format on

namespace caf {

bool Timeline::IsPaused() {
  if (!was_paused_) {  // We never paused.
    return false;
  } else if (pause_points_.size() % 2) {  // We are currently unpaused
    return false;
  } else {
    return true;  // We are currently paused.
  }
}

Timeline::MicroDurationType Timeline::Pause() { return pause_points_.emplace_back(ClockType::now()) - epoch_point_; }

[[nodiscard]] Timeline::MicroDurationType Timeline::ElapsedTime() const {
  // If we never paused, return time since last reset point.
  if (!was_paused_)
    return ClockType::now() - reset_point_;
  else {                             // We paused and/or unpaused at some point since the last reset point.
    if (pause_points_.size() == 1u)  // Single pause point case
      return pause_points_.back() - reset_point_;
    else if (pause_points_.size() % 2) {  // We are currently unpaused.
      // Assume pause_points_ vector is a non-zero divisible by 2 size, with at
      // least one pause/unpause pair. For each pause/unpause pair:
      // 1. Get the difference from the pause point to unpause point to get the
      // duration spent paused.
      // 2. Add result duration value to the total accumulated paused time
      // duration.
      MicroDurationType total_pause_duration = MicroDurationType::zero();
      auto it_p = pause_points_.begin();
      for (auto it_up = pause_points_.begin() + 1; it_up != pause_points_.end(); it_p + 2, it_up + 2) {
        auto paused_duration = (*it_up) - (*it_p);
        total_pause_duration += paused_duration;
      }
      // Subtract the accumulated paused duration from the current elapsed time,
      // and return.
      return (ClockType::now() - reset_point_) - total_pause_duration;
    } else {                           // We are currently paused
      if (pause_points_.size() == 1u)  // Single pause point case
        return pause_points_.back() - reset_point_;
      else {
        // Assume pause_points_ vector is a non-zero odd size, with at least one
        // pause/unpause pair
        MicroDurationType total_pause_duration = MicroDurationType::zero();
        auto it_p = pause_points_.begin();
        for (auto it_up = pause_points_.begin() + 1; it_up != pause_points_.end(); it_p + 2, it_up + 2) {
          auto paused_duration = (*it_up) - (*it_p);
          total_pause_duration += paused_duration;
        }  // Upon exit it_p is equal to the iterator to the last pause.
           // (pause_points.end() - 1).

        // Return the last pause point as the now() parameter for the elapsed
        // time.
        return (*it_p - reset_point_) - total_pause_duration;
      }
    }
  }
}

[[nodiscard]] Timeline::MicroDurationType Timeline::TotalTime() const {
  // TODO: implement this
  return ClockType::now() - epoch_point_;
}

[[nodiscard]] Timeline::MicroDurationType Timeline::RunningTime() const {
  return ClockType::now() - epoch_point_;
}

Timeline::MicroDurationType Timeline::Reset() {
  auto ret_duration = ElapsedTime();
  reset_point_ = ClockType::now();
  was_paused_ = false;
  return ret_duration;
}

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
