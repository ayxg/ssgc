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
  Ex<driver::IOConfig, int> io_init_res = driver::HandleInitialCliArgs(argc, argv);
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
// Licensed under the Apache License, Version 2.0(the "License"); you may not use this file except in compliance with
// the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on
// an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////