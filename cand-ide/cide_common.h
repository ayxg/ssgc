///////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
///////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language Environment
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup cand_cide
/// @brief Common typedefs and constants used in CIDE.
///////////////////////////////////////////////////////////////////////////////

#pragma once
#include <filesystem>
#include <string>
#include <vector>
#include <functional>
#include <string_view>

// C++ Standard Extended
#include "cppsextended.h"

// Minitest Framework - for cpp test explorer
#include "minitest.h"

namespace cide {

/// @addtogroup cand_cide_common
/// @{

namespace stdfs = std::filesystem;

using std::size_t;
using std::function;
using std::string;
using std::vector;
using std::string_view;

using cstring = const char*;
using wcstring = const wchar_t*;

// The lambdas and typedefs below are default values for callbacks 
// from frontend.
// They are used to avoid null checks in the backend code.
static constexpr LAMBDA xNullVoidCallback = []() {};
using VoidCallbackT = function<void(void)>;

// Boolean callback should be used when an error may occur.
// The callback should return false if an error occurred.
static constexpr LAMBDA xNullBoolCallback = []() { return false; };
using BoolCallbackT = function<bool(void)>;

// Defines file extensions used by the IDE.
namespace file_extension {
constexpr string_view kCaIdeSettings = ".caide";
constexpr string_view kCaSolution = ".casln";
constexpr string_view kCaBuild = ".camake";
constexpr string_view kCaSource = ".cand";
constexpr string_view kCaHeader = ".candi";
constexpr string_view kCailCode = ".cail";
constexpr string_view kCppSource = ".cpp";
constexpr string_view kCppHeader = ".hpp";
constexpr string_view kCSource = ".c";
constexpr string_view kCHeader = ".h";
namespace wide {
constexpr wcstring kCaIdeSettings = L".caide";
constexpr wcstring kCaSolution = L".casln";
constexpr wcstring kCaBuild = L".camake";
constexpr wcstring kCaSource = L".cand";
constexpr wcstring kCaHeader = L".candi";
constexpr wcstring kCailCode = L".cail";
constexpr wcstring kCppSource = L".cpp";
constexpr wcstring kCppHeader = L".hpp";
constexpr wcstring kCSource = L".c";
constexpr wcstring kCHeader = L".h";
}  // namespace wide
}  // namespace file_extension

/// @} // end of cand_cide_common

}  // namespace cide

///////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language Environment
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
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
///////////////////////////////////////////////////////////////////////////////