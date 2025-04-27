///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the Apache License, Version 2.0(the "License");
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup
/// @brief
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup cnd_compiler_cli
/// @{
#pragma once
#include "use_ccapi.hpp"

namespace cnd::driver::messages {
constexpr StrView kMainHeader = R"(
C& Compositor CLI v0.0.0.0
Copyright (c) 2024 Anton Yashchenko. All rights reserved.)";

constexpr StrView kCliVersionNumber = "0.0.0.0";

constexpr StrView kBriefHelp = R"(
Usage: 
  cnd [mode?] [ [positional-args...] | [flag-args...] ]

A single mode must be passed as the first positional or a flag arg.
    [ -c | --comp | comp ]: Composition mode. Positional args are input source files.
    [ -h | --help | help ]: Display help. Use "--help topics" to get a list of help topics.
    [ -v  | --ver  | ver ]: Display version string of the C& Compositor CLI.
    [ -z  | --dev  | dev ]: Compiler Developer mode. Undocumented.

Common Flags:
    [ -d | --debug_build ]: Enable debug mode. Default is release.
    [ -o | --out_dir ]: Specify output directory.
    [ -a | --aux_dir ]: Specify auxillary directory.

)";

}  // namespace cnd::cli::messages

/// @} // end of cnd_compiler_cli

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
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