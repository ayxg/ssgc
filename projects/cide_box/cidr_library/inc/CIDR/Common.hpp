///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2025 Anton Yashchenko
// Licensed under the Apache License, Version 2.0(the "License");
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: [CIDR] C& integrated development repository library
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup cidrlib
/// @brief Pre-included headers ,common typedefs and constants used in the CIDR interface library.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup cidrlib_common
/// @{

// clang-format off
#pragma once
#include "cxxx.hpp"
#include "caf.hpp"
#include "cgui.hpp"
// clang-format on

namespace cidr {

namespace stdfs = std::filesystem;

using std::expected;
using std::function;
using std::size_t;
using std::string;
using std::string_view;
using std::vector;
using std::wstring;

using cstring = const char*;
using wcstring = const wchar_t*;

// These lambdas serve as default values for callbacks from user interface
// widgets to the ide backend.
static constexpr LAMBDA xNullCallback = []() {};
static constexpr LAMBDA xNullBoolCallback = []() { return false; };

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

}  // namespace cide

/// @} // end of cidrlib_common

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: [CIDR] C& integrated development repository library
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
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
