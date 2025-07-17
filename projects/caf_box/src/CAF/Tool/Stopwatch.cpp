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
/// @brief [SOURCE] Stopwatch class
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup coreappfw
/// @{

// clang-format off
#pragma once
#include "CAF/Tool/Stopwatch.hpp"

#include <chrono>
#include <map>
#include <string_view>
#include <tuple>
#include <vector>
#include <optional>
#include <string>
#include <cassert>
// clang-format on

namespace caf {


  [[nodiscard]] const std::map<int, Stopwatch::EntryType>& Stopwatch::GetTimes() const { return times_; }

  /// Create a new stopwatch entry with a generated unique integer id. The label is NOT used to identify the entry.
  /// To later re-add more entries to the same stopwatch entry, use this same method passing the returned id.
  int Stopwatch::Start(std::string_view label) {
    if (last_id_ == kNullEntryId) {
      start_ = ClockType::now();
      last_id_ = id_counter_;
      times_[id_counter_] = {label, std::vector<TimePointType>{start_},
                             std::vector<std::optional<TimePointType>>{std::nullopt}};
    } else {
      last_id_ = id_counter_;
      times_[id_counter_] = {label, std::vector<TimePointType>{ClockType::now()},
                             std::vector<std::optional<TimePointType>>{std::nullopt}};
    }
    id_counter_++;
    return last_id_;
  }

  /// Restarts a previously started stopwatch entry with the given id. This entry must have been stopped
  /// before re-starting.
  int Stopwatch::Start(int id) {
    assert(last_id_ != kNullEntryId && "[caf][Stopwatch] Cannot re-start a timer, no timer was ever started.");
    assert(times_.contains(id) && "[caf][Stopwatch] Cannot re-start a timer with an id that does not exist.");
    assert(std::get<2>(times_[id]).back().has_value() &&
           "[caf][Stopwatch] Timer must be stopped before restarting again.");
    std::get<1>(times_[id]).push_back(ClockType::now());
    std::get<2>(times_[id]).push_back(std::nullopt);
    return id;
  }

  /// Stops the timer of a given id, completing an entry associated with the id/label. Timer entry may
  /// now be restarted using the Start method.
  void Stopwatch::Stop(int id) {
    assert(last_id_ != kNullEntryId && "[caf][Stopwatch] Cannot stop a timer, no timer was ever started.");
    assert(times_.contains(id) && "[caf][Stopwatch] Cannot stop a timer with an id that does not exist.");
    assert(!(std::get<2>(times_[id]).back().has_value()) &&
           "[caf][Stopwatch] This timer id was already stopped - but not restarted again.");
    std::get<2>(times_[id]).back() = ClockType::now();
  }

  /// Stops the last started timer completing an entry associated with the id/label.
  void Stopwatch::Stop() {
    assert(last_id_ != kNullEntryId && "[caf][Stopwatch] Cannot stop a timer, no timer was ever started.");
    assert(!(std::get<2>(times_[last_id_]).back().has_value()) &&
           "[caf][Stopwatch] This timer id was already stopped - but not restarted again.");
    std::get<2>(times_[last_id_]).back() = ClockType::now();
  }

  /// Get a debug string of all the timer entries.
  [[nodiscard]] std::string Stopwatch::Format() const {
    using namespace std::chrono;
    std::string msg{};
    for (const auto& [id, entry] : times_) {
      const auto& [label, start_times, stop_times] = entry;
      auto entry_start = msg.size();
      milliseconds total{0};
      for (size_t i = 0; i < start_times.size(); ++i) {
        auto duration = duration_cast<milliseconds>(stop_times[i].value() - start_times[i]);
        total += duration;
        msg += std::format(
            "\t--[start]: {}s [stop]: {}s [elapsed]: {}ms\n",
            -duration_cast<seconds>(start_.time_since_epoch() - start_times[i].time_since_epoch()).count(),
            -duration_cast<seconds>(start_.time_since_epoch() - stop_times[i].value().time_since_epoch()).count(),
            duration.count());
      }

      msg.insert(entry_start, std::format("--[{}][{}][{}]\n", id, label, total));
    }
    return msg;
  }


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
