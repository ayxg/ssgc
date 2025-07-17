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
/// @brief [HEADER] Stopwatch class
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup coreappfw
/// @{

// clang-format off
#pragma once
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

struct Stopwatch {
  using ClockType = std::chrono::steady_clock;
  using TimePointType = std::chrono::time_point<ClockType>;
  using EntryType = std::tuple<std::string_view, std::vector<TimePointType>, std::vector<std::optional<TimePointType>>>;
  static constexpr int kNullEntryId = -1;

 private:
  TimePointType start_{};
  std::map<int, EntryType> times_{};
  int last_id_{kNullEntryId};
  int id_counter_{0};

 public:
  [[nodiscard]] const std::map<int, EntryType>& GetTimes() const;

  /// Create a new stopwatch entry with a generated unique integer id. The label is NOT used to identify the entry.
  /// To later re-add more entries to the same stopwatch entry, use this same method passing the returned id.
  int Start(std::string_view label);

  /// Restarts a previously started stopwatch entry with the given id. This entry must have been stopped
  /// before re-starting.
  int Start(int id);

  /// Stops the timer of a given id, completing an entry associated with the id/label. Timer entry may
  /// now be restarted using the Start method.
  void Stop(int id);

  /// Stops the last started timer completing an entry associated with the id/label.
  void Stop();

  /// Get a debug string of all the timer entries.
  [[nodiscard]] std::string Format() const;
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
