#pragma once
#include "caf_config.hpp"
#include "cxxx.hpp"

namespace caf {
using SysClock = std::chrono::system_clock;
using StdClock = std::chrono::steady_clock;
template <typename... T>
using StdDuration = std::chrono::duration<T...>;
template <typename... T>
using StdTimePoint = std::chrono::time_point<T...>;
using StdNanoseconds = std::chrono::nanoseconds;
using StdMicroseconds = std::chrono::microseconds;
using StdMicrosecondsD = StdDuration<double, std::micro>;

/// General purpose clock/timer, stores time as double in
/// microseconds.
class Timer {
  StdTimePoint<StdClock> reset_point_;  ///> Time of last reset, in microseconds.
 public:
  Timer() : reset_point_(StdClock::now()) {}
  [[nodiscard]] StdMicrosecondsD ElapsedTime() const;
  [[nodiscard]] StdMicrosecondsD LastResetPoint() const;
  StdMicrosecondsD Reset();
  Timer& Restart();
};


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
/// @code
///  while(program_running) {
///    bool is_rendered{false};
///    StdMicrosecondsD physics_delay;
///    auto time_from_prev_physics_step_ = physics_timeframe.start();
///    while(physics_timeframe.is_updating()){
///        // Do all the physics stuff here
/// 
///        physics_step_delay = physics_timeframe.tryFinish();
///        if(physics_timeframe.isFinished()) {
///            // Do stuff in response to completing the timeframe using the
///  leftover time. is_rendered = false;
///        }
///        else {
///            // Do stuff in response to not completing the step within the
///  allocated timeframe. break;
///        }
///    } // This loop will exit once delta time elapsed.
/// 
///    // Continue doing other things
/// 
///    // Wont happen until the time frame is finished
///    if(physics_timeframe.isFinished()){
///        if(!is_rendered){
///            // Render the stuff
///            is_rendered = true;
///        }
///        else {
///            // Already rendered... do other stuff instead
///        }
///    }
/// 
/// 
///  }
/// @endcode
class Timeframe {
  Timer timer_;
  StdMicrosecondsD delta_;        ///> Time per cycle of this frame.
  StdMicrosecondsD accumulator_;  ///> Time accumulated from end of previous step
  StdMicrosecondsD leftover_;     ///> Left-over time from previous step
                                  ///> negative values indicate a delay time.
  bool is_processing_;
  bool is_finished_;

 public:
  explicit Timeframe(double microseconds_per_frame)
      : timer_(),
        delta_(microseconds_per_frame),
        accumulator_(0.0),
        leftover_(0.0),
        is_processing_(false),
        is_finished_(false) {}
  StdMicrosecondsD Start();
  StdMicrosecondsD TryFinish();
  [[nodiscard]] bool IsUpdating() const;
  [[nodiscard]] bool IsFinished() const;
};

/// @brief A timer which tracks a timeline with pause/unpause functionality. 
/// Use case : for timed events such as animations.
class Timeline {
 public:
  using steady_clock = std::chrono::steady_clock;
  using steady_timepoint = std::chrono::time_point<std::chrono::steady_clock>;
  using micro_duration = std::chrono::duration<double, std::micro>;

 private:
  steady_timepoint epoch_point_;  ///< Time of construction, in microseconds.>
  steady_timepoint reset_point_;  ///< Time of last reset, in microseconds.>
  std::vector<steady_timepoint>
      pause_points_;  ///< Pause/Unpause points always in that order.>
  bool was_paused_;   ///< True if the clock was paused or unpause at any point.>

 public:
  Timeline()
      : epoch_point_(steady_clock::now()),
        reset_point_(steady_clock::now()),
        pause_points_(),
        was_paused_(false) {}

  micro_duration Pause();

  micro_duration Reset();

  bool IsPaused();

  [[nodiscard]] micro_duration ElapsedTime() const;

  [[nodiscard]] micro_duration TotalTime() const;

  [[nodiscard]] micro_duration RunningTime() const;
};  /// <@endclass:gtlTimeframe>


}  // namespace caf
