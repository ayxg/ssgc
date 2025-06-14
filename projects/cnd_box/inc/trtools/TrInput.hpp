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

/// @addtogroup cnd_compiler_data
/// @{
#pragma once
// clang-format off
#include "use_ccapi.hpp"
#include "use_corevals.hpp"
// clang-format on

namespace cnd {
struct TrInput {
  cldev::util::Logger* cli_stdio{&cldev::util::gStdLog()};       ///> Streams for CLI out/err/in at compile time.
  eDriverVerbosity cli_verbosity_level{eDriverVerbosity::kStd};  ///> Verbosity level for CLI output.
  Bool is_overwrite_allowed{false};                              ///> Overwrite disabled by default.
  Vec<Pair<Str, Str>> predefs{};                                 ///> Predefined values from the CLI.
  Vec<Path> src_files{};                                         ///> Source files to compile.

  Path work_dir{};  ///> Translation Working directory.
  Path out_dir{};   ///> Translation Output directory.
  Path aux_dir{};   ///> Translation Auxiliary directory.

  Vec<Path> src_dirs{};  ///> Additional source directories.
  Vec<Path> inc_dirs{};  ///> Additional include directories.
  Vec<Path> lib_dirs{};  ///> Additional library directories.
  Vec<Path> bin_dirs{};  ///> Additional binary directories.
  Vec<Path> res_dirs{};  ///> Additional resource directories.
  Vec<Path> sys_dirs{};  ///> Additional system directories.

  Bool is_debug_mode{false};     ///> Debug mode disabled by default.
  Bool is_verbose{false};        ///> Verbose mode disabled by default.
  Bool is_error_on_warn{false};  ///> Error on warning disabled by default.

  Int opt_level{0};   ///> Optimization level.
  Int warn_level{0};  ///> Warning level.

  std::set<eClWarning> additional_enabled_warnings{};
  std::set<eClWarning> additional_ignored_warnings{};

  // Target toolchain dictates the "runtime" environment aka "runeval".
  // Target toolchain dictates the "runtime" environment aka "runeval".
  // Host and target are the same by default.
  bool is_host_same_as_target{true};

  eToolchain host_toolchain{};
  eProcArch host_arch{};
  eOpSys host_op_sys{};
  Path host_toolchain_root{};
  Path host_cxx_compiler_exe{};
  Path host_assembler_exe{};
  Path host_linker_exe{};

  eToolchain target_toolchain{};
  eProcArch target_arch{};
  eOpSys target_op_sys{};
  Path target_cxx_compiler_exe{};
  Path target_assembler_exe{};
  Path target_linker_exe{};

  // Debugging options
  bool debug_dump_tokens{false};
};

}  // namespace cnd

/// @} // end of cnd_compiler_data

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
