#include "caf_time.hpp"

#include "caf_config.hpp"
#include "cxxx.hpp"
namespace caf {

[[nodiscard]] StdMicrosecondsD Timer::ElapsedTime() const {
  return StdClock::now() - reset_point_;
};

[[nodiscard]] StdMicrosecondsD Timer::LastResetPoint() const {
  return reset_point_.time_since_epoch();
};

StdMicrosecondsD Timer::Reset() {
  auto ret_duration = ElapsedTime();
  reset_point_ = StdClock::now();
  return ret_duration;
}

Timer& Timer::Restart() {
  reset_point_ = StdClock::now();
  return *this;
}

StdMicrosecondsD Timeframe::Start() {
  // assert(!is_processing_ && "timex::Timeframe | start() called but the frame
  // step was already started.");
  accumulator_ += timer_.Reset();
  accumulator_ += leftover_;
  is_processing_ = true;
  return accumulator_;  // Time from end of last step
}

[[nodiscard]] bool Timeframe::IsUpdating() const {
  // assert(is_processing_ && "timex::Timeframe | isUpdating() called before a
  // timeframe was ever started.");
  //  Did we NOT accumulate enough time in the previous step?
  //  true -> No, we need to continue stepping.
  //  false -> Yes, the time frame has occurred for delta time.
  return accumulator_.count() >= delta_.count();
}

StdMicrosecondsD Timeframe::TryFinish() {
  assert(is_processing_ &&
         "timex::Timeframe | tryFinish() called before a timeframe was ever "
         "started.");
  leftover_ = accumulator_ - delta_;
  accumulator_ = StdMicrosecondsD::zero();

  // Is another frame step required ? Yes by default.
  // If there is partial negative extra frame time ?
  // Then we need to perform more updates to finish this frame.
  // false -> We need to update more to finish this frame.
  // true -> updating finished, continue to next frame.
  is_finished_ = !std::signbit(leftover_.count());

  is_processing_ = !is_finished_;

  return leftover_;
}

[[nodiscard]] bool Timeframe::IsFinished() const { return is_finished_; }

bool Timeline::IsPaused() {
  if (!was_paused_) {  // We never paused.
    return false;
  } else if (pause_points_.size() % 2) {  // We are currently unpaused
    return false;
  } else {
    return true;  // We are currently paused.
  }
}

Timeline::micro_duration Timeline::Pause() {
  return pause_points_.emplace_back(steady_clock::now()) - epoch_point_;
}

[[nodiscard]] Timeline::micro_duration Timeline::ElapsedTime() const {
  // If we never paused, return time since last reset point.
  if (!was_paused_)
    return std::chrono::steady_clock::now() - reset_point_;
  else {  // We paused and/or unpaused at some point since the last reset point.
    if (pause_points_.size() == 1u)  // Single pause point case
      return pause_points_.back() - reset_point_;
    else if (pause_points_.size() % 2) {  // We are currently unpaused.
      // Assume pause_points_ vector is a non-zero divisible by 2 size, with at
      // least one pause/unpause pair. For each pause/unpause pair:
      // 1. Get the difference from the pause point to unpause point to get the
      // duration spent paused.
      // 2. Add result duration value to the total accumulated paused time
      // duration.
      micro_duration total_pause_duration = micro_duration::zero();
      auto it_p = pause_points_.begin();
      for (auto it_up = pause_points_.begin() + 1; it_up != pause_points_.end();
           it_p + 2, it_up + 2) {
        auto paused_duration = (*it_up) - (*it_p);
        total_pause_duration += paused_duration;
      }
      // Subtract the accumulated paused duration from the current elapsed time,
      // and return.
      return (std::chrono::steady_clock::now() - reset_point_) -
             total_pause_duration;
    } else {                           // We are currently paused
      if (pause_points_.size() == 1u)  // Single pause point case
        return pause_points_.back() - reset_point_;
      else {
        // Assume pause_points_ vector is a non-zero odd size, with at least one
        // pause/unpause pair
        micro_duration total_pause_duration = micro_duration::zero();
        auto it_p = pause_points_.begin();
        for (auto it_up = pause_points_.begin() + 1;
             it_up != pause_points_.end(); it_p + 2, it_up + 2) {
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

[[nodiscard]] Timeline::micro_duration Timeline::TotalTime() const {
  // TODO: implement this
  return std::chrono::steady_clock::now() - epoch_point_;
}

[[nodiscard]] Timeline::micro_duration Timeline::RunningTime() const {
  return std::chrono::steady_clock::now() - epoch_point_;
}

Timeline::micro_duration Timeline::Reset() {
  auto ret_duration = ElapsedTime();
  reset_point_ = std::chrono::steady_clock::now();
  was_paused_ = false;
  return ret_duration;
}

}  // namespace caf
