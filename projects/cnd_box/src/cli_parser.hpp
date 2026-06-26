///////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
///////////////////////////////////////////////////////////////////////////////////////////////////
// @project: Sophia Silicon Gold Compiler
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup
/// @brief
///////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup ssgc_cli
/// @{

#pragma once
#include <algorithm>
#include <array>
#include <cassert>
#include <expected>
#include <format>
#include <iostream>
#include <map>
#include <span>
#include <string>
#include <string_view>
#include <unordered_map>
#include <variant>
#include <vector>

#include "diagnostics.hpp"
#include "cli_enum_cli_flag.hpp"

namespace ssgc::cli {

// Expected format of command line arguments passed to the parser.
using CliArgs = std::vector<std::string_view>;
using CliFlagMap = std::unordered_map<eCliFlag, std::vector<std::string>>;

struct CliParserResult {
  std::optional<eCliFlag> command_flag{std::nullopt};
  std::optional<eCliFlag> display_flag{std::nullopt};
  diagnostic::Diagnostics errors{};
  std::vector<std::string> positionals{};
  std::size_t rest_args{0};

  constexpr bool failed() const { return !errors->empty(); }
};

CliArgs getCommandLineArgs(int argc, char* argv[]);

CliParserResult parseCliMain(
  CliArgs::const_iterator beg,
  CliArgs::const_iterator end,
  CliFlagMap& out
) noexcept;

CliParserResult parseCliCommandBuild(
  CliArgs::const_iterator beg,
  CliArgs::const_iterator end,
  CliFlagMap& out
) noexcept;

CliParserResult parseCliCommandDev(
  CliArgs::const_iterator beg,
  CliArgs::const_iterator end,
  CliFlagMap& out
) noexcept;

CliParserResult parseCliCommandDevLex(
  CliArgs::const_iterator beg,
  CliArgs::const_iterator end,
  CliFlagMap& out
) noexcept;

std::string helptextCliMain() noexcept;

std::string helptextCliCommandBuild() noexcept;

std::string helptextCliCommandDev() noexcept;

std::string helptextCliCommandDevLex() noexcept;

}  // namespace ssgc::cli

/// @} // end of ssgc_cli

///////////////////////////////////////////////////////////////////////////////////////////////////
// @project: Sophia Silicon Gold Compiler
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////////////////////////////////////