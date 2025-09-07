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
/// @brief [SOURCE] Timeframe class
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup coreappfw
/// @{

// clang-format off
#include "CAF/Tool/Timeframe.hpp"
#include <chrono>
// clang-format on

namespace caf {

bool Timeframe::NextFrame(TimePointType prev, TimePointType now, bool should_update) {
  prev_frame_ = prev;
  frame_start_ = now;
  should_update_ = should_update;
  return should_update;
}

void Timeframe::NextFrame(TimePointType prev, TimePointType now) {
  prev_frame_ = prev;
  frame_start_ = now;
}

Timeframe::Timeframe(double microseconds_per_frame) : target_delta_(microseconds_per_frame) {}

void Timeframe::Reset() { is_init_ = false; }

[[nodiscard]] Timeframe::TimePointType Timeframe::Epoch() const { return epoch_; }

bool Timeframe::Begin() {
  //  Initial step. Has to record the timeframe epoch or else we will have a huge delay from the construction of the
  //  timeframe to the start of the loop.
  if (!is_init_) {
    epoch_ = ClockType::now();
    is_init_ = true;
    prev_frame_ = epoch_;
    frame_start_ = epoch_;
    accum_delta_ = target_delta_;
    should_update_ = true;
  }

  // Update the frame delta
  prev_frame_ = frame_start_;
  frame_start_ = ClockType::now();

  // Has delta time elapsed since last Begin() call?
  if (accum_delta_ + LiveDelta() >= target_delta_) {
    // Ready to run next frame, accumulate delta, reduce accumulated delta by target delta for 1 frame.
    should_update_ = true;
    accum_delta_ += LiveDelta();
    accum_delta_ -= target_delta_;
  } else {
    // Not enough time has passed, skip this frame, accumulate delta.
    accum_delta_ += LiveDelta();
    should_update_ = true;
  }

  return should_update_;
}

[[nodiscard]] bool Timeframe::ShouldUpdate() const { return should_update_; }

[[nodiscard]] Timeframe::MicroDurationType Timeframe::LiveDelta() const {
  return std::chrono::duration_cast<MicroDurationType>(frame_start_ - prev_frame_ - target_delta_);
}

[[nodiscard]] Timeframe::MicroDurationType Timeframe::TargetDelta() const { return target_delta_; }

Timeframe::MicroDurationType Timeframe::TargetDelta(const MicroDurationType& delta) {
  target_delta_ = delta;
  return target_delta_;
}

[[nodiscard]] bool Timeframe::IsDelayed() const { return (LiveDelta() > TargetDelta()); }

[[nodiscard]] Timeframe::MicroDurationType Timeframe::Delay() const { return LiveDelta() - TargetDelta(); }

[[nodiscard]] int Timeframe::DelayFrames() const {
  return static_cast<int>(std::floor(MicroDurationType((LiveDelta() - TargetDelta()) / TargetDelta()).count()));
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
