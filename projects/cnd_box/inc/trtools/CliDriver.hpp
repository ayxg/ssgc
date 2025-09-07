///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the Apache License, Version 2.0(the "License");
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language Environment
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup cnd_compiler_driver
/// @brief C& Official Compiler Command Line Interface Driver
///
/// Syntax:
/// @code
///     cnd [C& source files] [subcommand] [options] [--|end-of-args]
/// @endcode
///
/// - The main positional argument(s) to the cnd executable is a list of C& source files.
/// - All other arguments are options and flags- which may appear in any order.
/// - Positional arguments may appear after the options if a -- is passed to indicate end of options.
/// - The [-S | --src-files] flag is an additional alternative to the main positional argument.
///
/// Full details can be seen in the C& compiler reference manual [driver] section. Excerpt:
///
/// [C&CRMS][driver.modes]
///   The C& compiler is primarily used as a command line interface application, directly or as a child process of
///   another application-such as an integrated developer environment. The default name for the compiler’s executable is
///   ‘cnd.exe’ or ‘cnd’. The first argument passed must be the mode to run the compiler in, and is one of:
///
///   -c | --comp | comp :
///     Composition mode receives a list of C& source file paths followed by any flags or variables to apply to this
///     composition. Fully processes the input composing the source code, possibly(often) generating executables and
///     libraries in the output path. Default output path is /out/ relative to the current directory and may be set with
///     a flag.
///
///   -z | --dev | dev : developer debug mode. Unit testing and other utilities related to development and debugging of
///     the compiler implementation. From the user perspective: The is NO guarantee that any functionality will continue
///     to be available API is NOT stable across versions. Documentation for users is provided optionally at the
///     discretion of the compiler developer.
///
///   -h | --help | help : Displays help text and exits.
///
///   When passed as the first argument, the dashes ‘—’ preceding the keyword may be omitted.
/// [C&CRMS][driver.modes][end]
///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup cnd_compiler_driver
/// @{
#pragma once
// clang-format off
#include "use_ccapi.hpp"
#include "use_corevals.hpp"
#include "use_clmsg.hpp"
#include "cldev/dev_logger.hpp"
#include "trtools/TrOutput.hpp"
#include "trtools/TrInput.hpp"
#include "corevals/cli_messages.hpp"
// clang-format on

namespace cnd::driver {

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* using */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Uses compiler common error value
using ClFail = CompilerProcessFailure;  ///> Driver failure result holds a ClMsgBuffer.
template <class T>
using ClResult = CompilerProcessResult<T>;
using cldev::clmsg::MakeClMsg;

// Uses dev logger.
using cldev::util::Logger;

// Data format of arguments which the driver command line interface processes.
using ArgsBuffer = Vec<Str>;
using ArgsBufferIter = ArgsBuffer::const_iterator;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* decls */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct IOConfig;              ///> Struct holding the initial IO configuration for the driver parsed on the first pass.
struct CommandLineArguments;  ///> Fully parsed cli args data. Flags are stored as a std::multimap.

/// @brief Handles early exits, verbosity, and IO redirection cases for the C& CLI.
///        Call this method before cnd::cli::ParseCliArgs to get the initial CLI configuration.
/// @return Unexpected value is an exit code, upon early exit. Expected value is the inital CLI params to pass to
///         cnd::cli::ParseCliArgs when no early exit occured.
///
/// Early Cases Handled:
///   - No arguments passed.          -> Prints header and brief help to stdout and exits.
///   - Help or Version flag passed.  -> Prints help or version to stdout and exits.
///   - Redirection flags passed.     -> Redirects stdout, stderr, or stdin to files.
///   - Invalid redirection paths.    -> Prints error to stderr and exits.
///   - '--no_overwrite' enabled.     -> Redirection to existing file causes error.
///   - '--silent' mode is enabled.   -> No output. Help or version flag wont print.
///
/// @see cnd::cli::IOConfig
Ex<IOConfig, int> HandleInitialCliArgs(int argc, char* argv[]);

/// @brief Creates a new logger to pass to other translation tools based on driver IOConfig parameters.
/// @param init_params IOConfig struct created using by HandleInitialCliArgs.
Logger CreateConfiguredLogger(const IOConfig& init_params);

/// Syntactically parses the command line args passed to the C& CLI based on eDriverFlag data and interpretation.
ClResult<CommandLineArguments> ParseCliArgs(int argc, char* argv[], IOConfig& init_params);

/// Configure mode based on the parsed flags and positional arguments.
/// @pre `inargs` must already contain the parsed cli flags.
void ConfigureCliArgsMode(CommandLineArguments& inargs);

ClResult<int> HandleCliArgsVersionMode(CommandLineArguments& inargs);
ClResult<int> HandleCliArgsHelpMode(CommandLineArguments& inargs);
ClResult<int> HandleCliArgsDevMode(CommandLineArguments& inargs);
ClResult<TrInput> HandleCliArgsCompMode(CommandLineArguments& inargs);
ClResult<int> HandleCliArgsPostCompMode(TrInput&, TrOutput&);

namespace cliparser {
struct ParsedFlagOffset;

/// Format: [flag_str][[=]|[:]][boolean-string]
/// Valid boolean strings: true, TRUE, 1, ON, on, yes, YES, false, FALSE, 0, OFF, off, no, NO
// ClResult<ParsedFlagOffset> ParseBooleanFlag(StrView flag_str, ArgsBufferIter flag_it, ArgsBufferIter args_end);

/// Format: [flag_str][whitespace][value]
ClResult<ParsedFlagOffset> ParseSingleArgFlag(ArgsBufferIter args_it, ArgsBufferIter args_end, Size flag_offset = 0);

/// Format: [flag_str][[whitespace][arg]...]
ClResult<ParsedFlagOffset> ParseMultiArgFlag(ArgsBufferIter args_it, ArgsBufferIter args_end, Size flag_offset = 0);

///// Format: [flag_str][arg]
// ClResult<ParsedFlagOffset> ParseSingleArgTightFlag(StrView flag_str, ArgsBufferIter flag_it, ArgsBufferIter
// args_end);
//
///// Format: [flag_str][[=]|[:]][arg]
// ClResult<ParsedFlagOffset> ParseDefinitionFlag(StrView flag_str, ArgsBufferIter flag_it, ArgsBufferIter args_end);

/// Dispatches to the appropriate flag parsing method based on the flag interp type.
ClResult<ParsedFlagOffset> ParseDriverFlagValue(eDriverFlagInterp interp, ArgsBufferIter args_it,
                                                ArgsBufferIter args_end, Size arg_offset);

ClResult<eDriverFlag> ChooseCliArgsMode(CommandLineArguments& inargs);
void RemoveQuotesFromCliArg(char* arg);
}  // namespace cliparser

namespace debug {
void PrintParsedCliArgs(CommandLineArguments& cli_args);
}  // namespace debug

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* defs */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct IOConfig {
  eDriverVerbosity verbosity_level{eDriverVerbosity::kStd};
  bool is_help_run{false};        ///> Detailed help run was requested.
  bool is_version_run{false};     ///> Version info run was requested.
  bool is_overwrite_files{true};  ///> Overwrite files by default | --no_overwrite to disable
  Str cli_out_stream = "";        ///> maybe unused if no redirection
  Str cli_err_stream = "";        ///> maybe unused if no redirection
  Str cli_in_stream = "";         ///> maybe unused if no redirection
};

struct CommandLineArguments {
  IOConfig io_config;
  eDriverFlag mode{eDriverFlag::kNONE};
  Vec<Str> args;  ///> Positional arguments.
  std::multimap<eDriverFlag, Vec<Str>> flags;
  bool ContainsFlag(eDriverFlag flag) const;
  bool ContainsFlag(eDriverFlag flag, StrView value) const;
};

namespace cliparser {
struct ParsedFlagOffset {
  Vec<Str> args;            // Parsed args.
  ArgsBufferIter next_arg;  // Next arg to continue from.
  Size arg_offset;          // Index offset within the arg to continue from.
};
}  // namespace cliparser

namespace detail {
void DebugPrintParsedCliArgs(CommandLineArguments& cli_args);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* impl */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}  // namespace cnd::driver

/// @} // end of cnd_compiler_driver

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