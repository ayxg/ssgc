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
/// @brief [HEADER] Timeframe class
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup coreappfw
/// @{

// clang-format off
#pragma once
#include <chrono>
// clang-format on

namespace caf {

/// @class Timeframe
/// @brief Used to lock a section of code to only occur only for an allotted
/// period of time.
///
/// Executes the code in steps which report leftover when attempting
/// to finish the timeframe. Reported negative time indicates amount
/// required to finish the previous step (delay). Positive indicates
/// extra time remaining after completing the previous step.
///
/// Use to lock a game's physics update loop to 60 fps.
/// @TODO: this example next time i used the class.
class Timeframe {
 public:
  using ClockType = std::chrono::steady_clock;
  using TimePointType = std::chrono::time_point<ClockType>;
  using MicroDurationType = std::chrono::duration<double, std::micro>;

  explicit Timeframe(double microseconds_per_frame);

  // Reset the timeframe to its initial state. Meaning the first call to Begin() will have a delta
  // of zero and be the new timeframe epoch.
  void Reset();

  /// Call to start the next frame in the time frame.
  bool Begin();

  /// Time point when the first call of Stary() occurred after construction or after calling Reset().
  [[nodiscard]] TimePointType Epoch() const;

  [[nodiscard]] bool ShouldUpdate() const;

  // The total amount of time passed from the last call to Begin().
  [[nodiscard]] MicroDurationType LiveDelta() const;

  [[nodiscard]] MicroDurationType TargetDelta() const;

  MicroDurationType TargetDelta(const MicroDurationType& delta);

  // If the live delta is greater than the target delta, then the timeframe is delayed.
  [[nodiscard]] bool IsDelayed() const;

  // The amount of delay upon call to the Start() method from the previous frame.
  // eg. if the target delta is 16.67ms and the LiveDelta() is 20ms, then
  //     the Delay() will return -3.33ms.
  [[nodiscard]] MicroDurationType Delay() const;

  // The amount of delayed frames. Calculated by dividing the delay by the target delta.
  [[nodiscard]] int DelayFrames() const;

 private:
  bool NextFrame(TimePointType prev, TimePointType now, bool should_update);
  void NextFrame(TimePointType prev, TimePointType now);

  TimePointType epoch_;
  TimePointType prev_frame_;
  TimePointType frame_start_;
  MicroDurationType accum_delta_;
  MicroDurationType target_delta_;
  bool should_update_{true};
  bool is_init_{false};
};

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
