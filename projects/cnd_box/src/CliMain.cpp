///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup
/// @brief C& Compiler CLI main entry point.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup cnd_compiler_cli
/// @{
#include "CliMain.hpp"

#include "trtools/Composer.hpp"

namespace cnd {

CompilerProcessResult<std::variant<int,TrOutput>> CliMainInternal(int argc, char* argv[], char* envp[]) {
  Ex<driver::IOConfig, int> io_init_res = driver::HandleInitialCliArgs(argc, argv);
  if (!io_init_res) return io_init_res.error();  // Early exit.
  cldev::util::Logger& log = cldev::util::gStdLog();
  log = {driver::CreateConfiguredLogger(*io_init_res)};
  CompilerProcessResult<driver::CommandLineArguments> parsed_cli_args_res =
      driver::ParseCliArgs(argc, argv, *io_init_res);
  if (!parsed_cli_args_res) return log.PrintErrForward(parsed_cli_args_res.error());
  driver::CommandLineArguments& cli_args = *parsed_cli_args_res;

  switch (cli_args.mode) {
    case eDriverFlag::kModeHelp: {
      CompilerProcessResult<int> rt = driver::HandleCliArgsHelpMode(cli_args);
      if (!rt) return log.PrintErrForward(rt.error());
      return *rt;
    }
    case eDriverFlag::kModeVersion: {
      CompilerProcessResult<int> rt = driver::HandleCliArgsVersionMode(cli_args);
      if (!rt) return log.PrintErrForward(rt.error());
      return *rt;
    }
    case eDriverFlag::kModeComp: {
      CompilerProcessResult<TrInput> comp_params_res = driver::HandleCliArgsCompMode(cli_args);
      if (!comp_params_res) return log.PrintErrForward(comp_params_res.error());
      CompilerProcessResult<TrOutput> comp_res = trtools::Compose(*comp_params_res);
      if (!comp_res)
        return log.PrintErrForward(comp_params_res.error());
      else {
        // Handle post-compilation actions...
        // cli::HandleCliArgsPostCompMode(comp_params_res.value(), comp_res.value());
        return *comp_res;
      }
    }
    case eDriverFlag::kModeDev: {
      CompilerProcessResult<int> rt = HandleCliArgsDevMode(cli_args);
      if (!rt) return log.PrintErrForward(rt.error());
      return *rt;
    }
    default: {
      assert(
          false &&
          "Implementation failure in cnd::CliMain method. Local reference 'cli_mode' returned by cli::ChooseCliArgsMode"
          " of type eDriverFlag must be a valid cli mode flag.");
      return EXIT_FAILURE;
    }
  }
}

/// @brief C& Compiler CLI main entry point.
/// @param argc 
/// @param argv 
/// @param envp 
/// @return Translation process result. 0 if successful. Error otherwise.
/// 
int CliMain(int argc, char* argv[], char* envp[]) {
  Vec<Str> args{argv, argv + argc};
  Ex<driver::IOConfig, int> io_init_res = driver::HandleInitialCliArgs(args.cbegin(),args.cend());
  if (!io_init_res) return io_init_res.error();  // Early exit.

  cldev::util::Logger& log = cldev::util::gStdLog();
  log = {driver::CreateConfiguredLogger(io_init_res.value())};
  CompilerProcessResult<driver::CommandLineArguments> parsed_cli_args_res =
      driver::ParseCliArgs(argc, argv, io_init_res.value());
  if (!parsed_cli_args_res) return log.PrintErrForward(parsed_cli_args_res.error());
  driver::CommandLineArguments& cli_args = parsed_cli_args_res.value();

  switch (cli_args.mode) {
    case eDriverFlag::kModeHelp: {
      CompilerProcessResult<int> rt = driver::HandleCliArgsHelpMode(cli_args);
      if (!rt) return log.PrintErrForward(rt.error());
      return rt.value();
    }
    case eDriverFlag::kModeVersion: {
      CompilerProcessResult<int> rt = driver::HandleCliArgsVersionMode(cli_args);
      if (!rt) return log.PrintErrForward(rt.error());
      return rt.value();
    }
    case eDriverFlag::kModeComp: {
      CompilerProcessResult<TrInput> comp_params_res = driver::HandleCliArgsCompMode(cli_args);
      if (!comp_params_res) return log.PrintErrForward(comp_params_res.error());
      CompilerProcessResult<TrOutput> comp_res = trtools::Compose(comp_params_res.value());
      if (!comp_res)
        return log.PrintErrForward(comp_params_res.error());
      else {
        // Handle post-compilation actions...
        // cli::HandleCliArgsPostCompMode(comp_params_res.value(), comp_res.value());
        return comp_res.value().exit_code;
      }
    }
    case eDriverFlag::kModeDev: {
      CompilerProcessResult<int> rt = HandleCliArgsDevMode(cli_args);
      if (!rt) return log.PrintErrForward(rt.error());
      return rt.value();
    }
    default: {
      assert(
          false &&
          "Implementation failure in cnd::CliMain method. Local reference 'cli_mode' returned by cli::ChooseCliArgsMode"
          " of type eDriverFlag must be a valid cli mode flag.");
      return EXIT_FAILURE;
    }
  }
}
}  // namespace cnd

/// @} // end of cnd_compiler_cli

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
//
// This program is free software : you can redistribute it and / or modify it
// under the terms of the GNU Affero General Public License as published by the
// Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////