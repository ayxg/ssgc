///////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
///////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language Compiler
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup ssgc_cli
/// @brief
///////////////////////////////////////////////////////////////////////////////

/// @addtogroup ssgc_cli
/// @{
#include "cli_ansi.hpp"

#include <string_view>

namespace ssgc::cli::ansi {

static bool& isColorEnabled() noexcept {
  static bool colors_enabled = false;
  return colors_enabled;
}

bool enableColor(bool enable) noexcept {
  bool prev_state = isColorEnabled();
  isColorEnabled() = enable;
  return prev_state;
}

std::string_view reset() noexcept { return isColorEnabled() ? "\033[0m" : ""; }

std::string_view red() noexcept { return isColorEnabled() ? "\033[31m" : ""; }

std::string_view yellow() noexcept {
  return isColorEnabled() ? "\033[33m" : "";
}

std::string_view green() noexcept { return isColorEnabled() ? "\033[32m" : ""; }

}  // namespace ssgc::cli::ansi

/// @} // end of ssgc_cli

///////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language Compiler
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
//
// Licensed under the GNU Affero General Public License, Version 3.
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.gnu.org/licenses/agpl-3.0.html
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
///////////////////////////////////////////////////////////////////////////////