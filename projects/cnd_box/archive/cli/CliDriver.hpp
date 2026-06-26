///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
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
/// - Positional arguments may appear after the options if a -- is passed to indicate end of
/// options.
/// - The [-S | --src-files] flag is an additional alternative to the main positional argument.
///
/// Full details can be seen in the C& compiler reference manual [driver] section. Excerpt:
///
/// [C&CRMS][driver.modes]
///   The C& compiler is primarily used as a command line interface application, directly or as a
///   child process of another application-such as an integrated developer environment. The default
///   name for the compiler’s executable is ‘cnd.exe’ or ‘cnd’. The first argument passed must be
///   the mode to run the compiler in, and is one of:
///
///   -c | --comp | comp :
///     Composition mode receives a list of C& source file paths followed by any flags or variables
///     to apply to this composition. Fully processes the input composing the source code,
///     possibly(often) generating executables and libraries in the output path. Default output path
///     is /out/ relative to the current directory and may be set with a flag.
///
///   -z | --dev | dev : developer debug mode. Unit testing and other utilities related to
///   development and debugging of
///     the compiler implementation. From the user perspective: The is NO guarantee that any
///     functionality will continue to be available API is NOT stable across versions. Documentation
///     for users is provided optionally at the discretion of the compiler developer.
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
#include "ccapi/CommonCppApi.hpp"

#include "compiler_utils/CompilerProcessResult.hpp"
#include "compiler_utils/DevLogger.hpp"
#include "compiler_utils/ReflectedMetaEnum.hpp" 

#include "cli/CliParser.hpp"
#include "cli/eFlag.hpp"
#include "cli/eVerbosity.hpp"

#include "compiler/TranslationInput.hpp"
#include "compiler/TranslationOutput.hpp"
#include "compiler/Compiler.hpp"
#include <format>
// clang-format on

namespace cnd::driver {

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Define command line flags metadata.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using FlagMeta = Flag<eFlag>;  ///> Flag metadata format to pass to cli parser definitions.

/// @returns the method of syntax interpretation for a flag enum.
constexpr eFlagInterp GetFlagInterp(eFlag flag) noexcept;

/// @returns short form ident of a flag. Whitespace char if unknown flag.
constexpr char GetFlagShortSymbol(eFlag flag) noexcept;

/// @returns long form ident of a flag. Empty string if unknown flag.
constexpr StrView GetFlagLongSymbol(eFlag flag) noexcept;

/// @returns flag brief help description. Empty string if no description available.
constexpr StrView GetFlagDescription(eFlag flag) noexcept;

constexpr eFlagInterp GetFlagInterp(eFlag flag) noexcept {
  switch (flag) {
    case eFlag::kModeComp:
      return eFlagInterp::kCmd;
    case eFlag::kModeDev:
      return eFlagInterp::kCmd;
    case eFlag::kModeHelp:
      return eFlagInterp::kCmd;
    case eFlag::kModeVersion:
      return eFlagInterp::kCmd;
    case eFlag::kSources:
      return eFlagInterp::kPositional;
    case eFlag::kDefine:
      return eFlagInterp::kVarDef;
    case eFlag::kOutDir:
      return eFlagInterp::kSingle;
    case eFlag::kAuxDir:
      return eFlagInterp::kSingle;
    case eFlag::kWorkDir:
      return eFlagInterp::kSingle;
    case eFlag::kSrcDirs:
      return eFlagInterp::kMulti;
    case eFlag::kLibDirs:
      return eFlagInterp::kMulti;
    case eFlag::kResDirs:
      return eFlagInterp::kMulti;
    case eFlag::kExeDirs:
      return eFlagInterp::kMulti;
    case eFlag::kSrcFiles:
      return eFlagInterp::kMulti;
    case eFlag::kLibFiles:
      return eFlagInterp::kMulti;
    case eFlag::kExeFiles:
      return eFlagInterp::kMulti;
    case eFlag::kResFiles:
      return eFlagInterp::kMulti;
    case eFlag::kNoOverwrite:
      return eFlagInterp::kOpt;
    case eFlag::kCompIoSilent:
      return eFlagInterp::kOpt;
    case eFlag::kCompIoVerbose:
      return eFlagInterp::kOpt;
    case eFlag::kCompIoDebug:
      return eFlagInterp::kOpt;
    case eFlag::kCompIoStd:
      return eFlagInterp::kOpt;
    case eFlag::kCompIoTrace:
      return eFlagInterp::kOpt;
    case eFlag::kCompStdinRedir:
      return eFlagInterp::kSingle;
    case eFlag::kCompStdoutRedir:
      return eFlagInterp::kSingle;
    case eFlag::kCompStderrRedir:
      return eFlagInterp::kSingle;
    case eFlag::kDriverIoSilent:
      return eFlagInterp::kOpt;
    case eFlag::kDriverIoVerbose:
      return eFlagInterp::kOpt;
    case eFlag::kDriverIoDebug:
      return eFlagInterp::kOpt;
    case eFlag::kDriverIoStd:
      return eFlagInterp::kOpt;
    case eFlag::kDriverIoTrace:
      return eFlagInterp::kOpt;
    case eFlag::kDriverStdinRedir:
      return eFlagInterp::kSingle;
    case eFlag::kDriverStdoutRedir:
      return eFlagInterp::kSingle;
    case eFlag::kDriverStderrRedir:
      return eFlagInterp::kSingle;
    case eFlag::kDump:
      return eFlagInterp::kSingle;
    case eFlag::kHostLinker:
      return eFlagInterp::kSingle;
    case eFlag::kHostLinkerType:
      return eFlagInterp::kSingle;
    case eFlag::kHostLinkerVersion:
      return eFlagInterp::kSingle;
    case eFlag::kHostLinkerDriver:
      return eFlagInterp::kSingle;
    case eFlag::kHostLinkerDriverType:
      return eFlagInterp::kSingle;
    case eFlag::kHostLinkerDriverExternal:
      return eFlagInterp::kOpt;
    case eFlag::kHostLinkerImplicitLibs:
      return eFlagInterp::kMulti;
    case eFlag::kHostCxxCl:
      return eFlagInterp::kSingle;
    case eFlag::kHostCxxClType:
      return eFlagInterp::kSingle;
    case eFlag::kHostCxxClVersion:
      return eFlagInterp::kSingle;
    case eFlag::kHostCxxClDriver:
      return eFlagInterp::kSingle;
    case eFlag::kHostCxxClDriverType:
      return eFlagInterp::kSingle;
    case eFlag::kHostCxxClDriverExternal:
      return eFlagInterp::kOpt;
    case eFlag::kHostCxxPredefs:
      return eFlagInterp::kMulti;
    case eFlag::kHostCxxSources:
      return eFlagInterp::kMulti;
    case eFlag::kHostCxxIncludeDirs:
      return eFlagInterp::kMulti;
    case eFlag::kTargetHost:
      return eFlagInterp::kSingle;
    case eFlag::kProcArch:
      return eFlagInterp::kSingle;
    case eFlag::kOpSys:
      return eFlagInterp::kSingle;
    case eFlag::kLinker:
      return eFlagInterp::kSingle;
    case eFlag::kLinkerType:
      return eFlagInterp::kSingle;
    case eFlag::kLinkerVersion:
      return eFlagInterp::kSingle;
    case eFlag::kLinkerDriver:
      return eFlagInterp::kSingle;
    case eFlag::kLinkerDriverType:
      return eFlagInterp::kSingle;
    case eFlag::kLinkerDriverExternal:
      return eFlagInterp::kOpt;
    case eFlag::kLinkerImplicitLibs:
      return eFlagInterp::kMulti;
    case eFlag::kCxxCl:
      return eFlagInterp::kSingle;
    case eFlag::kCxxClType:
      return eFlagInterp::kSingle;
    case eFlag::kCxxClVersion:
      return eFlagInterp::kSingle;
    case eFlag::kCxxClDriver:
      return eFlagInterp::kSingle;
    case eFlag::kCxxClDriverType:
      return eFlagInterp::kSingle;
    case eFlag::kCxxClDriverExternal:
      return eFlagInterp::kOpt;
    case eFlag::kCxxPredefs:
      return eFlagInterp::kMulti;
    case eFlag::kCxxSources:
      return eFlagInterp::kMulti;
    case eFlag::kCxxIncludeDirs:
      return eFlagInterp::kMulti;
    default:
      return eFlagInterp::kNONE;
  }
}

constexpr char GetFlagShortSymbol(eFlag flag) noexcept {
  switch (flag) {
    case eFlag::kModeComp:
      return 'c';
    case eFlag::kModeDev:
      return 'z';
    case eFlag::kModeHelp:
      return 'h';
    case eFlag::kModeVersion:
      return 'v';
    case eFlag::kDefine:
      return 'D';
    case eFlag::kOutDir:
      return 'O';
    case eFlag::kAuxDir:
      return 'A';
    case eFlag::kWorkDir:
      return 'W';
    case eFlag::kSrcFiles:
      return 'S';
    case eFlag::kLibFiles:
      return 'L';
    case eFlag::kExeFiles:
      return 'E';
    case eFlag::kResFiles:
      return 'R';
    default:
      return ' ';
  }
};

constexpr StrView GetFlagLongSymbol(eFlag flag) noexcept {
  switch (flag) {
    case eFlag::kModeComp:
      return "comp";
    case eFlag::kModeDev:
      return "dev";
    case eFlag::kModeHelp:
      return "help";
    case eFlag::kModeVersion:
      return "version";
    case eFlag::kDefine:
      return "define";
    case eFlag::kOutDir:
      return "out-dir";
    case eFlag::kAuxDir:
      return "aux-dir";
    case eFlag::kWorkDir:
      return "work-dir";
    case eFlag::kSrcDirs:
      return "src-dirs";
    case eFlag::kLibDirs:
      return "lib-dirs";
    case eFlag::kResDirs:
      return "res-dirs";
    case eFlag::kExeDirs:
      return "exe-dirs";
    case eFlag::kSrcFiles:
      return "src-files";
    case eFlag::kLibFiles:
      return "lib-files";
    case eFlag::kExeFiles:
      return "exe-files";
    case eFlag::kResFiles:
      return "res-files";
    case eFlag::kNoOverwrite:
      return "no-overwrite";
    case eFlag::kCompIoSilent:
      return "comp-io-silent";
    case eFlag::kCompIoVerbose:
      return "comp-io-verbose";
    case eFlag::kCompIoDebug:
      return "comp-io-debug";
    case eFlag::kCompIoStd:
      return "comp-io-std";
    case eFlag::kCompIoTrace:
      return "comp-io-trace";
    case eFlag::kCompStdinRedir:
      return "comp-stdin-redir";
    case eFlag::kCompStdoutRedir:
      return "comp-stdout-redir";
    case eFlag::kCompStderrRedir:
      return "comp-stderr-redir";
    case eFlag::kDriverIoSilent:
      return "driver-io-silent";
    case eFlag::kDriverIoVerbose:
      return "driver-io-verbose";
    case eFlag::kDriverIoDebug:
      return "driver-io-debug";
    case eFlag::kDriverIoStd:
      return "driver-io-std";
    case eFlag::kDriverIoTrace:
      return "driver-io-trace";
    case eFlag::kDriverStdinRedir:
      return "driver-stdin-redir";
    case eFlag::kDriverStdoutRedir:
      return "driver-stdout-redir";
    case eFlag::kDriverStderrRedir:
      return "driver-stderr-redir";
    case eFlag::kDump:
      return "dump";
    case eFlag::kHostLinker:
      return "host-linker";
    case eFlag::kHostLinkerType:
      return "host-linker-type";
    case eFlag::kHostLinkerVersion:
      return "host-linker-version";
    case eFlag::kHostLinkerDriver:
      return "host-linker-driver";
    case eFlag::kHostLinkerDriverType:
      return "host-linker-driver-type";
    case eFlag::kHostLinkerDriverExternal:
      return "host-linker-driver-external";
    case eFlag::kHostLinkerImplicitLibs:
      return "host-linker-implicit-libs";
    case eFlag::kHostCxxCl:
      return "host-cxx-cl";
    case eFlag::kHostCxxClType:
      return "host-cxx-cl-type";
    case eFlag::kHostCxxClVersion:
      return "host-cxx-cl-version";
    case eFlag::kHostCxxClDriver:
      return "host-cxx-cl-driver";
    case eFlag::kHostCxxClDriverType:
      return "host-cxx-cl-driver-type";
    case eFlag::kHostCxxClDriverExternal:
      return "host-cxx-cl-driver-external";
    case eFlag::kHostCxxPredefs:
      return "host-cxx-predefs";
    case eFlag::kHostCxxSources:
      return "host-cxx-sources";
    case eFlag::kHostCxxIncludeDirs:
      return "host-cxx-include-dirs";
    case eFlag::kTargetHost:
      return "target-host";
    case eFlag::kProcArch:
      return "proc-arch";
    case eFlag::kOpSys:
      return "op-sys";
    case eFlag::kLinker:
      return "linker";
    case eFlag::kLinkerType:
      return "linker-type";
    case eFlag::kLinkerVersion:
      return "linker-version";
    case eFlag::kLinkerDriver:
      return "linker-driver";
    case eFlag::kLinkerDriverType:
      return "linker-driver-type";
    case eFlag::kLinkerDriverExternal:
      return "linker-driver-external";
    case eFlag::kLinkerImplicitLibs:
      return "linker-implicit-libs";
    case eFlag::kCxxCl:
      return "cxx-cl";
    case eFlag::kCxxClType:
      return "cxx-cl-type";
    case eFlag::kCxxClVersion:
      return "cxx-cl-version";
    case eFlag::kCxxClDriver:
      return "cxx-cl-driver";
    case eFlag::kCxxClDriverType:
      return "cxx-cl-driver-type";
    case eFlag::kCxxClDriverExternal:
      return "cxx-cl-driver-external";
    case eFlag::kCxxPredefs:
      return "cxx-predefs";
    case eFlag::kCxxSources:
      return "cxx-sources";
    case eFlag::kCxxIncludeDirs:
      return "cxx-include-dirs";
    default:
      return "";
  }
};

constexpr StrView GetFlagDescription(eFlag flag) noexcept {
  switch (flag) {
    case eFlag::kModeComp:
      return "Composition mode.";
    case eFlag::kModeDev:
      return "dev";
    case eFlag::kModeHelp:
      return "help";
    case eFlag::kModeVersion:
      return "version";
    case eFlag::kDefine:
      return "define";
    case eFlag::kOutDir:
      return "out-dir";
    case eFlag::kAuxDir:
      return "aux-dir";
    case eFlag::kWorkDir:
      return "work-dir";
    case eFlag::kSrcDirs:
      return "src-dirs";
    case eFlag::kLibDirs:
      return "lib-dirs";
    case eFlag::kResDirs:
      return "res-dirs";
    case eFlag::kExeDirs:
      return "exe-dirs";
    case eFlag::kSrcFiles:
      return "src-files";
    case eFlag::kLibFiles:
      return "lib-files";
    case eFlag::kExeFiles:
      return "exe-files";
    case eFlag::kResFiles:
      return "res-files";
    case eFlag::kNoOverwrite:
      return "no-overwrite";
    case eFlag::kCompIoSilent:
      return "comp-io-silent";
    case eFlag::kCompIoVerbose:
      return "comp-io-verbose";
    case eFlag::kCompIoDebug:
      return "comp-io-debug";
    case eFlag::kCompIoStd:
      return "comp-io-std";
    case eFlag::kCompIoTrace:
      return "comp-io-trace";
    case eFlag::kCompStdinRedir:
      return "comp-stdin-redir";
    case eFlag::kCompStdoutRedir:
      return "comp-stdout-redir";
    case eFlag::kCompStderrRedir:
      return "comp-stderr-redir";
    case eFlag::kDriverIoSilent:
      return "driver-io-silent";
    case eFlag::kDriverIoVerbose:
      return "driver-io-verbose";
    case eFlag::kDriverIoDebug:
      return "driver-io-debug";
    case eFlag::kDriverIoStd:
      return "driver-io-std";
    case eFlag::kDriverIoTrace:
      return "driver-io-trace";
    case eFlag::kDriverStdinRedir:
      return "driver-stdin-redir";
    case eFlag::kDriverStdoutRedir:
      return "driver-stdout-redir";
    case eFlag::kDriverStderrRedir:
      return "driver-stderr-redir";
    case eFlag::kDump:
      return "dump";
    case eFlag::kHostLinker:
      return "host-linker";
    case eFlag::kHostLinkerType:
      return "host-linker-type";
    case eFlag::kHostLinkerVersion:
      return "host-linker-version";
    case eFlag::kHostLinkerDriver:
      return "host-linker-driver";
    case eFlag::kHostLinkerDriverType:
      return "host-linker-driver-type";
    case eFlag::kHostLinkerDriverExternal:
      return "host-linker-driver-external";
    case eFlag::kHostLinkerImplicitLibs:
      return "host-linker-implicit-libs";
    case eFlag::kHostCxxCl:
      return "host-cxx-cl";
    case eFlag::kHostCxxClType:
      return "host-cxx-cl-type";
    case eFlag::kHostCxxClVersion:
      return "host-cxx-cl-version";
    case eFlag::kHostCxxClDriver:
      return "host-cxx-cl-driver";
    case eFlag::kHostCxxClDriverType:
      return "host-cxx-cl-driver-type";
    case eFlag::kHostCxxClDriverExternal:
      return "host-cxx-cl-driver-external";
    case eFlag::kHostCxxPredefs:
      return "host-cxx-predefs";
    case eFlag::kHostCxxSources:
      return "host-cxx-sources";
    case eFlag::kHostCxxIncludeDirs:
      return "host-cxx-include-dirs";
    case eFlag::kTargetHost:
      return "target-host";
    case eFlag::kProcArch:
      return "proc-arch";
    case eFlag::kOpSys:
      return "op-sys";
    case eFlag::kLinker:
      return "linker";
    case eFlag::kLinkerType:
      return "linker-type";
    case eFlag::kLinkerVersion:
      return "linker-version";
    case eFlag::kLinkerDriver:
      return "linker-driver";
    case eFlag::kLinkerDriverType:
      return "linker-driver-type";
    case eFlag::kLinkerDriverExternal:
      return "linker-driver-external";
    case eFlag::kLinkerImplicitLibs:
      return "linker-implicit-libs";
    case eFlag::kCxxCl:
      return "cxx-cl";
    case eFlag::kCxxClType:
      return "cxx-cl-type";
    case eFlag::kCxxClVersion:
      return "cxx-cl-version";
    case eFlag::kCxxClDriver:
      return "cxx-cl-driver";
    case eFlag::kCxxClDriverType:
      return "cxx-cl-driver-type";
    case eFlag::kCxxClDriverExternal:
      return "cxx-cl-driver-external";
    case eFlag::kCxxPredefs:
      return "cxx-predefs";
    case eFlag::kCxxSources:
      return "cxx-sources";
    case eFlag::kCxxIncludeDirs:
      return "cxx-include-dirs";
    default:
      return "";
  }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Define command line parsers. Each parser will parse args up to a command, those count as the
// global args. Afterwards, driver will handle the parsed flags and a command specific parser will
// parse starting from after the command. Commands with sub-commands will also parse
// 'command-global' arguments before parsing the sub-command's flags based on the given sub-command.
// Each command and subcommand must define it's own parser. 'MainCliParser' is the initial global
// parser but all flags are shared in the 'eFlag' enum.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace parsers {

/// Initializes flag metadata struct for cli parsers partially filling in data from eFlag metadata
/// methods. Data points filled: flag interpretation, short name, long name, description.
constexpr FlagMeta DefFlag(eFlag f, FlagProperties props = FlagProperties{},
                           FlagMeta::FlagValidatorType validator = nullptr) {
  return FlagMeta{f,
                  GetFlagInterp(f),
                  GetFlagShortSymbol(f),
                  GetFlagLongSymbol(f).data(),
                  GetFlagDescription(f).data(),
                  props,
                  validator};
}

using enum eFlag;
// clang-format off
static constexpr auto kMainParserFlags = GenParserFlags(
  DefFlag(kModeComp), 
  DefFlag(kModeDev), 
  DefFlag(kModeHelp), 
  DefFlag(kModeVersion), 
  DefFlag(kDriverIoSilent),
  DefFlag(kDriverIoVerbose),
  DefFlag(kDriverIoDebug),
  DefFlag(kNoOverwrite)
);

static constexpr auto kCompModeFlags = GenParserFlags(
  DefFlag(kModeHelp),
  DefFlag(kOutDir),                                                   
  DefFlag(kAuxDir),
  DefFlag(kSources,FlagProperties{}.Repeatable()),
  DefFlag(kDefine)    
);


using MainCliParser = Parser<kMainParserFlags>;
using CompModeCliParser = Parser<kCompModeFlags>;

// clang-format on
}  // namespace parsers

/// @brief Configures a Logger object's verbosity based on provided driver I/O flags.
/// @param log `cldev::util::Logger` stream to configure.
/// @param flags Command line flags map.
void ConfigLoggerVerbosity(cldev::util::Logger& log, const FlagMeta::FlagMapType& flags) {
  if (flags.contains(eFlag::kDriverIoSilent))
    log.verbosity = eVerbosity::kSilent;
  else if (flags.contains(eFlag::kDriverIoVerbose))
    log.verbosity = eVerbosity::kSilent;
  else if (flags.contains(eFlag::kDriverIoDebug))
    log.verbosity = eVerbosity::kSilent;
  else if (flags.contains(eFlag::kDriverIoTrace))
    log.verbosity = eVerbosity::kSilent;
  else
    log.verbosity = eVerbosity::kStd;
}

/// @brief Configures a translation input struct based on parsed/generated command line flags.
/// @param trin Translation input struct to fill.
/// @param flags Command line flags map.
ClRes<void> ConfigTranslationInput(TrInput& trin, const FlagMeta::FlagMapType& flags) {
  auto src_files = flags.equal_range(eFlag::kSources);
  for (auto it = src_files.first; it != src_files.second; it++) {
    trin.src_files.push_back(std::get<StrView>(it->second));
  }

  return ClRes<void>{};
};

/// @brief Handles post-translation steps based on the translation output and parsed/generated
/// command line flags.
/// @param tr_out Translation output struct produced by a `Compiler` instance `.Translate()` call.
/// @param flags Command line flags map.
/// @return
ClRes<void> HandlePostCompMode(const TrOutput& tr_out, const FlagMeta::FlagMapType& flags) {
  // Print exit code for debugging.
  std::cout << "Evaluation return value:" << tr_out.return_value << std::endl;
  return ClRes<void>{};
};

/// @brief Main handler for composition mode.
///   - Parses composition mode specific command line flags.
///   - Configures translation input struct based on parsed flags.
///   - Runs the compiler translation process.
///   - Runs post-translation steps based on the translation output and parsed flags.
/// @param rest_args Begin of the rest of cli args. Returned by the main cli parser after parsing is
/// done.
/// @param args_end End of the cli args.
/// @param parsed_flags Command line flags map.
/// @return Exit code to return from the CLI process.
ClRes<int> HandleCompMode(ArgvConstIter rest_args, ArgvConstIter args_end,
                          FlagMeta::FlagMapType& parsed_flags) {
  using cldev::util::gStdLog;
  parsers::CompModeCliParser comp_parser{};
  auto comp_parse_res = comp_parser.Parse(rest_args, args_end, parsed_flags);
  if (!comp_parse_res) return gStdLog().PrintErrForward(comp_parse_res.error(), EXIT_FAILURE);
  if (comp_parser.GetCommand() == eFlag::kModeHelp)
    return gStdLog().PrintForward(comp_parser.GenerateHelpText(), EXIT_SUCCESS);

  TrInput trin{};
  ClRes<void> trin_config_res = ConfigTranslationInput(trin, parsed_flags);
  if (!trin_config_res)
    return gStdLog().PrintErrForward(trin_config_res.error().Format(), EXIT_FAILURE);

  trtools::Compiler compiler{trin};
  ClRes<TrOutput> tr_res = compiler.Translate();
  if (!tr_res) return gStdLog().PrintErrForward(tr_res.error().Format(), EXIT_FAILURE);

  ClRes<void> post_res = HandlePostCompMode(tr_res.value(), parsed_flags);
  if (!post_res) return gStdLog().PrintErrForward(post_res.error().Format(), EXIT_FAILURE);

  return EXIT_SUCCESS;
}

ClRes<TrOutput> CliMain(int argc, char* argv[], char* envp[] = nullptr) {
  using cldev::util::gStdLog;

  // Parse global flags and main command.
  // Skip the first argument when passing to parser.
  Vec<StrView> input_args{argv, argv + argc};
  parsers::MainCliParser::FlagMapType parsed_flags{};
  parsers::MainCliParser main_parser{};
  auto main_parse_res = main_parser.Parse(input_args.begin() + 1, input_args.end(), parsed_flags);

  // Config log before possible error, in-case of '--silent'.
  // Before printing a possible parsing error.
  ConfigLoggerVerbosity(gStdLog(), parsed_flags);
  if (!main_parse_res) return gStdLog().PrintErrForward(main_parse_res.error(), EXIT_FAILURE);

  // Parse and handle further flags based on main command.
  switch (main_parser.GetCommand()) {
    case eFlag::kModeComp:
      return HandleCompMode(main_parse_res.value(), input_args.end(), parsed_flags);
      break;
    case eFlag::kModeHelp:
      return gStdLog().PrintForward(main_parser.GenerateHelpText(), EXIT_SUCCESS);
      break;
    default:
      return gStdLog().PrintErrForward("No command provided.", EXIT_FAILURE);
  }

  return EXIT_SUCCESS;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///* using */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//// Uses compiler common error value
// using ClFail = CompilerProcessFailure;  ///> Driver failure result holds a ClMsgBuffer.
// template <class T>
// using ClResult = ClRes<T>;
// using cldev::clmsg::MakeClMsg;
//
//// Uses dev logger.
// using cldev::util::Logger;
//
//// Data format of arguments which the driver command line interface processes.
// using ArgsBuffer = Vec<Str>;
// using ArgsBufferIter = ArgsBuffer::iterator;
// using ArgsBufferConstIter = ArgsBuffer::const_iterator;
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///* decls */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// struct IOConfig;              ///> Struct holding the initial IO configuration for the driver
// parsed on the first pass. struct CommandLineArguments;  ///> Fully parsed cli args data. Flags
// are stored as a std::multimap.
//
///// @brief Handles early exits, verbosity, and IO redirection cases for the C& CLI.
/////        Call this method before cnd::cli::ParseCliArgs to get the initial CLI configuration.
///// @return Unexpected value is an exit code, upon early exit. Expected value is the inital CLI
///params to pass to
/////         cnd::cli::ParseCliArgs when no early exit occured.
/////
///// Early Cases Handled:
/////   - No arguments passed.          -> Prints header and brief help to stdout and exits.
/////   - Help or Version flag passed.  -> Prints help or version to stdout and exits.
/////   - Redirection flags passed.     -> Redirects stdout, stderr, or stdin to files.
/////   - Invalid redirection paths.    -> Prints error to stderr and exits.
/////   - '--no_overwrite' enabled.     -> Redirection to existing file causes error.
/////   - '--silent' mode is enabled.   -> No output. Help or version flag wont print.
/////
///// @see cnd::cli::IOConfig
// Ex<IOConfig, int> HandleInitialCliArgs(ArgsBufferConstIter beg, ArgsBufferConstIter end);
//
///// @brief Creates a new logger to pass to other translation tools based on driver IOConfig
///parameters.
///// @param init_params IOConfig struct created using by HandleInitialCliArgs.
// Logger CreateConfiguredLogger(const IOConfig& init_params);
//
///// Syntactically parses the command line args passed to the C& CLI based on eDriverFlag data and
///interpretation.
// ClResult<CommandLineArguments> ParseCliArgs(int argc, char* argv[], IOConfig& init_params);
//
///// Configure mode based on the parsed flags and positional arguments.
///// @pre `inargs` must already contain the parsed cli flags.
// void ConfigureCliArgsMode(CommandLineArguments& inargs);
//
// ClResult<int> HandleCliArgsVersionMode(CommandLineArguments& inargs);
// ClResult<int> HandleCliArgsHelpMode(CommandLineArguments& inargs);
// ClResult<int> HandleCliArgsDevMode(CommandLineArguments& inargs);
// ClResult<TrInput> HandleCliArgsCompMode(CommandLineArguments& inargs);
// ClResult<int> HandleCliArgsPostCompMode(TrInput&, TrOutput&);
//
// namespace cliparser {
// struct ParsedFlagOffset;
//
///// Format: [flag_str][[=]|[:]][boolean-string]
///// Valid boolean strings: true, TRUE, 1, ON, on, yes, YES, false, FALSE, 0, OFF, off, no, NO
//// ClResult<ParsedFlagOffset> ParseBooleanFlag(StrView flag_str, ArgsBufferIter flag_it,
///ArgsBufferIter args_end);
//
///// Format: [flag_str][whitespace][value]
// ClResult<ParsedFlagOffset> ParseSingleArgFlag(ArgsBufferIter args_it, ArgsBufferIter args_end,
// Size flag_offset = 0);
//
///// Format: [flag_str][[whitespace][arg]...]
// ClResult<ParsedFlagOffset> ParseMultiArgFlag(ArgsBufferIter args_it, ArgsBufferIter args_end,
// Size flag_offset = 0);
//
/////// Format: [flag_str][arg]
//// ClResult<ParsedFlagOffset> ParseSingleArgTightFlag(StrView flag_str, ArgsBufferIter flag_it,
///ArgsBufferIter / args_end);
////
/////// Format: [flag_str][[=]|[:]][arg]
//// ClResult<ParsedFlagOffset> ParseDefinitionFlag(StrView flag_str, ArgsBufferIter flag_it,
///ArgsBufferIter args_end);
//
///// Dispatches to the appropriate flag parsing method based on the flag interp type.
// ClResult<ParsedFlagOffset> ParseDriverFlagValue(eFlagInterp interp, ArgsBufferIter args_it,
//                                                 ArgsBufferIter args_end, Size arg_offset);
//
// ClResult<eFlag> ChooseCliArgsMode(CommandLineArguments& inargs);
// void RemoveQuotesFromCliArg(char* arg);
// }  // namespace cliparser
//
// namespace debug {
// void PrintParsedCliArgs(CommandLineArguments& cli_args);
// }  // namespace debug
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///* defs */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// struct IOConfig {
//  eDriverVerbosity verbosity_level{eDriverVerbosity::kStd};
//  bool is_help_run{false};        ///> Detailed help run was requested.
//  bool is_version_run{false};     ///> Version info run was requested.
//  bool is_overwrite_files{true};  ///> Overwrite files by default | --no_overwrite to disable
//  Str cli_out_stream = "";        ///> maybe unused if no redirection
//  Str cli_err_stream = "";        ///> maybe unused if no redirection
//  Str cli_in_stream = "";         ///> maybe unused if no redirection
//};
//
// struct CommandLineArguments {
//  IOConfig io_config;
//  eFlag mode{eFlag::kNONE};
//  Vec<Str> args;  ///> Positional arguments.
//  std::multimap<eFlag, Vec<Str>> flags;
//  bool ContainsFlag(eFlag flag) const;
//  bool ContainsFlag(eFlag flag, StrView value) const;
//};
//
// namespace cliparser {
// struct ParsedFlagOffset {
//  Vec<Str> args;                 // Parsed args.
//  ArgsBufferConstIter next_arg;  // Next arg to continue from.
//  Size arg_offset;               // Index offset within the arg to continue from.
//};
//}  // namespace cliparser
//
// namespace detail {
// void DebugPrintParsedCliArgs(CommandLineArguments& cli_args);
//}

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