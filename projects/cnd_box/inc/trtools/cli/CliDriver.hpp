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
#include "cldev/clmsg.hpp"
#include "cldev/dev_logger.hpp"
#include "corevals/reflected_meta_enum.hpp" 
#include "trtools/cli/CliParser.hpp"
#include "trtools/cli/eFlag.hpp"
#include "trtools/cli/eVerbosity.hpp"

#include "trtools/TrInput.hpp"
#include "trtools/TrOutput.hpp"
#include "trtools/Compiler.hpp"
// clang-format on

namespace cnd::driver {

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Define command line flags metadata.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using FlagMeta = Flag<eFlag>;  ///> Flag metadata format to pass to cli parser definitions.

/// Returns the method of syntax interpretation for a flag enum.
constexpr eFlagInterp GetFlagInterp(eFlag flag) noexcept;

/// Returns short form ident of a flag. Returns whitespace char if unknown flag.
constexpr char GetFlagShortSymbol(eFlag flag) noexcept;

/// Returns short form ident of a flag. Returns empty string if unknown flag.
constexpr StrView GetFlagLongSymbol(eFlag flag) noexcept;

/// Returns flag brief help description. Returns empty string if no description available.
constexpr StrView GetFlagDescription(eFlag flag) noexcept;

/// Initializes flag metadata struct for cli parsers partially filling in data from eFlag metadata methods.
/// Data points filled: flag interpretation, short name, long name, description.
constexpr FlagMeta DefFlag(eFlag f, FlagProperties props = FlagProperties{},
                           FlagMeta::FlagValidatorType validator = nullptr);

constexpr eFlagInterp GetFlagInterp(eFlag flag) noexcept {
#define CND_MM_LOCAL_CASE(n, lt) \
  case eFlag::k##n:              \
    return eFlagInterp::k##lt

  switch (flag) {
    CND_MM_LOCAL_CASE(ModeComp, Cmd);
    CND_MM_LOCAL_CASE(ModeDev, Cmd);
    CND_MM_LOCAL_CASE(ModeHelp, Cmd);
    CND_MM_LOCAL_CASE(ModeVersion, Cmd);
    CND_MM_LOCAL_CASE(Sources, Positional);    
    CND_MM_LOCAL_CASE(Define, VarDef);
    CND_MM_LOCAL_CASE(OutDir, Single);
    CND_MM_LOCAL_CASE(AuxDir, Single);
    CND_MM_LOCAL_CASE(WorkDir, Single);
    CND_MM_LOCAL_CASE(SrcDirs, Multi);
    CND_MM_LOCAL_CASE(LibDirs, Multi);
    CND_MM_LOCAL_CASE(ResDirs, Multi);
    CND_MM_LOCAL_CASE(ExeDirs, Multi);
    CND_MM_LOCAL_CASE(SrcFiles, Multi);
    CND_MM_LOCAL_CASE(LibFiles, Multi);
    CND_MM_LOCAL_CASE(ExeFiles, Multi);
    CND_MM_LOCAL_CASE(ResFiles, Multi);
    CND_MM_LOCAL_CASE(NoOverwrite, Opt);
    CND_MM_LOCAL_CASE(CompIoSilent, Opt);
    CND_MM_LOCAL_CASE(CompIoVerbose, Opt);
    CND_MM_LOCAL_CASE(CompIoDebug, Opt);
    CND_MM_LOCAL_CASE(CompIoStd, Opt);
    CND_MM_LOCAL_CASE(CompIoTrace, Opt);
    CND_MM_LOCAL_CASE(CompStdinRedir, Single);
    CND_MM_LOCAL_CASE(CompStdoutRedir, Single);
    CND_MM_LOCAL_CASE(CompStderrRedir, Single);
    CND_MM_LOCAL_CASE(DriverIoSilent, Opt);
    CND_MM_LOCAL_CASE(DriverIoVerbose, Opt);
    CND_MM_LOCAL_CASE(DriverIoDebug, Opt);
    CND_MM_LOCAL_CASE(DriverIoStd, Opt);
    CND_MM_LOCAL_CASE(DriverIoTrace, Opt);
    CND_MM_LOCAL_CASE(DriverStdinRedir, Single);
    CND_MM_LOCAL_CASE(DriverStdoutRedir, Single);
    CND_MM_LOCAL_CASE(DriverStderrRedir, Single);
    CND_MM_LOCAL_CASE(Dump, Single);
    CND_MM_LOCAL_CASE(HostLinker, Single);
    CND_MM_LOCAL_CASE(HostLinkerType, Single);
    CND_MM_LOCAL_CASE(HostLinkerVersion, Single);
    CND_MM_LOCAL_CASE(HostLinkerDriver, Single);
    CND_MM_LOCAL_CASE(HostLinkerDriverType, Single);
    CND_MM_LOCAL_CASE(HostLinkerDriverExternal, Opt);
    CND_MM_LOCAL_CASE(HostLinkerImplicitLibs, Multi);
    CND_MM_LOCAL_CASE(HostCxxCl, Single);
    CND_MM_LOCAL_CASE(HostCxxClType, Single);
    CND_MM_LOCAL_CASE(HostCxxClVersion, Single);
    CND_MM_LOCAL_CASE(HostCxxClDriver, Single);
    CND_MM_LOCAL_CASE(HostCxxClDriverType, Single);
    CND_MM_LOCAL_CASE(HostCxxClDriverExternal, Opt);
    CND_MM_LOCAL_CASE(HostCxxPredefs, Multi);
    CND_MM_LOCAL_CASE(HostCxxSources, Multi);
    CND_MM_LOCAL_CASE(HostCxxIncludeDirs, Multi);
    CND_MM_LOCAL_CASE(TargetHost, Single);
    CND_MM_LOCAL_CASE(ProcArch, Single);
    CND_MM_LOCAL_CASE(OpSys, Single);
    CND_MM_LOCAL_CASE(Linker, Single);
    CND_MM_LOCAL_CASE(LinkerType, Single);
    CND_MM_LOCAL_CASE(LinkerVersion, Single);
    CND_MM_LOCAL_CASE(LinkerDriver, Single);
    CND_MM_LOCAL_CASE(LinkerDriverType, Single);
    CND_MM_LOCAL_CASE(LinkerDriverExternal, Opt);
    CND_MM_LOCAL_CASE(LinkerImplicitLibs, Multi);
    CND_MM_LOCAL_CASE(CxxCl, Single);
    CND_MM_LOCAL_CASE(CxxClType, Single);
    CND_MM_LOCAL_CASE(CxxClVersion, Single);
    CND_MM_LOCAL_CASE(CxxClDriver, Single);
    CND_MM_LOCAL_CASE(CxxClDriverType, Single);
    CND_MM_LOCAL_CASE(CxxClDriverExternal, Opt);
    CND_MM_LOCAL_CASE(CxxPredefs, Multi);
    CND_MM_LOCAL_CASE(CxxSources, Multi);
    CND_MM_LOCAL_CASE(CxxIncludeDirs, Multi);
    default:
      return eFlagInterp::kNONE;
  }
#undef CND_MM_LOCAL_CASE
}

constexpr char GetFlagShortSymbol(eFlag flag) noexcept {
#define CND_MM_LOCAL_CASE(n, lt) \
  case eFlag::k##n:              \
    return lt

  switch (flag) {
    CND_MM_LOCAL_CASE(ModeComp, 'c');
    CND_MM_LOCAL_CASE(ModeDev, 'z');
    CND_MM_LOCAL_CASE(ModeHelp, 'h');
    CND_MM_LOCAL_CASE(ModeVersion, 'v');
    CND_MM_LOCAL_CASE(Define, 'D');
    CND_MM_LOCAL_CASE(OutDir, 'O');
    CND_MM_LOCAL_CASE(AuxDir, 'A');
    CND_MM_LOCAL_CASE(WorkDir, 'W');
    CND_MM_LOCAL_CASE(SrcFiles, 'S');
    CND_MM_LOCAL_CASE(LibFiles, 'L');
    CND_MM_LOCAL_CASE(ExeFiles, 'E');
    CND_MM_LOCAL_CASE(ResFiles, 'R');
    default:
      return ' ';
  }

#undef CND_MM_LOCAL_CASE
};

constexpr StrView GetFlagLongSymbol(eFlag flag) noexcept {
#define CND_MM_LOCAL_CASE(n, lt) \
  case eFlag::k##n:              \
    return lt

  switch (flag) {
    CND_MM_LOCAL_CASE(ModeComp, "comp");
    CND_MM_LOCAL_CASE(ModeDev, "dev");
    CND_MM_LOCAL_CASE(ModeHelp, "help");
    CND_MM_LOCAL_CASE(ModeVersion, "version");
    CND_MM_LOCAL_CASE(Define, "define");
    CND_MM_LOCAL_CASE(OutDir, "out-dir");
    CND_MM_LOCAL_CASE(AuxDir, "aux-dir");
    CND_MM_LOCAL_CASE(WorkDir, "work-dir");
    CND_MM_LOCAL_CASE(SrcDirs, "src-dirs");
    CND_MM_LOCAL_CASE(LibDirs, "lib-dirs");
    CND_MM_LOCAL_CASE(ResDirs, "res-dirs");
    CND_MM_LOCAL_CASE(ExeDirs, "exe-dirs");
    CND_MM_LOCAL_CASE(SrcFiles, "src-files");
    CND_MM_LOCAL_CASE(LibFiles, "lib-files");
    CND_MM_LOCAL_CASE(ExeFiles, "exe-files");
    CND_MM_LOCAL_CASE(ResFiles, "res-files");
    CND_MM_LOCAL_CASE(NoOverwrite, "no-overwrite");
    CND_MM_LOCAL_CASE(CompIoSilent, "comp-io-silent");
    CND_MM_LOCAL_CASE(CompIoVerbose, "comp-io-verbose");
    CND_MM_LOCAL_CASE(CompIoDebug, "comp-io-debug");
    CND_MM_LOCAL_CASE(CompIoStd, "comp-io-std");
    CND_MM_LOCAL_CASE(CompIoTrace, "comp-io-trace");
    CND_MM_LOCAL_CASE(CompStdinRedir, "comp-stdin-redir");
    CND_MM_LOCAL_CASE(CompStdoutRedir, "comp-stdout-redir");
    CND_MM_LOCAL_CASE(CompStderrRedir, "comp-stderr-redir");
    CND_MM_LOCAL_CASE(DriverIoSilent, "driver-io-silent");
    CND_MM_LOCAL_CASE(DriverIoVerbose, "driver-io-verbose");
    CND_MM_LOCAL_CASE(DriverIoDebug, "driver-io-debug");
    CND_MM_LOCAL_CASE(DriverIoStd, "driver-io-std");
    CND_MM_LOCAL_CASE(DriverIoTrace, "driver-io-trace");
    CND_MM_LOCAL_CASE(DriverStdinRedir, "driver-stdin-redir");
    CND_MM_LOCAL_CASE(DriverStdoutRedir, "driver-stdout-redir");
    CND_MM_LOCAL_CASE(DriverStderrRedir, "driver-stderr-redir");
    CND_MM_LOCAL_CASE(Dump, "dump");
    CND_MM_LOCAL_CASE(HostLinker, "host-linker");
    CND_MM_LOCAL_CASE(HostLinkerType, "host-linker-type");
    CND_MM_LOCAL_CASE(HostLinkerVersion, "host-linker-version");
    CND_MM_LOCAL_CASE(HostLinkerDriver, "host-linker-driver");
    CND_MM_LOCAL_CASE(HostLinkerDriverType, "host-linker-driver-type");
    CND_MM_LOCAL_CASE(HostLinkerDriverExternal, "host-linker-driver-external");
    CND_MM_LOCAL_CASE(HostLinkerImplicitLibs, "host-linker-implicit-libs");
    CND_MM_LOCAL_CASE(HostCxxCl, "host-cxx-cl");
    CND_MM_LOCAL_CASE(HostCxxClType, "host-cxx-cl-type");
    CND_MM_LOCAL_CASE(HostCxxClVersion, "host-cxx-cl-version");
    CND_MM_LOCAL_CASE(HostCxxClDriver, "host-cxx-cl-driver");
    CND_MM_LOCAL_CASE(HostCxxClDriverType, "host-cxx-cl-driver-type");
    CND_MM_LOCAL_CASE(HostCxxClDriverExternal, "host-cxx-cl-driver-external");
    CND_MM_LOCAL_CASE(HostCxxPredefs, "host-cxx-predefs");
    CND_MM_LOCAL_CASE(HostCxxSources, "host-cxx-sources");
    CND_MM_LOCAL_CASE(HostCxxIncludeDirs, "host-cxx-include-dirs");
    CND_MM_LOCAL_CASE(TargetHost, "target-host");
    CND_MM_LOCAL_CASE(ProcArch, "proc-arch");
    CND_MM_LOCAL_CASE(OpSys, "op-sys");
    CND_MM_LOCAL_CASE(Linker, "linker");
    CND_MM_LOCAL_CASE(LinkerType, "linker-type");
    CND_MM_LOCAL_CASE(LinkerVersion, "linker-version");
    CND_MM_LOCAL_CASE(LinkerDriver, "linker-driver");
    CND_MM_LOCAL_CASE(LinkerDriverType, "linker-driver-type");
    CND_MM_LOCAL_CASE(LinkerDriverExternal, "linker-driver-external");
    CND_MM_LOCAL_CASE(LinkerImplicitLibs, "linker-implicit-libs");
    CND_MM_LOCAL_CASE(CxxCl, "cxx-cl");
    CND_MM_LOCAL_CASE(CxxClType, "cxx-cl-type");
    CND_MM_LOCAL_CASE(CxxClVersion, "cxx-cl-version");
    CND_MM_LOCAL_CASE(CxxClDriver, "cxx-cl-driver");
    CND_MM_LOCAL_CASE(CxxClDriverType, "cxx-cl-driver-type");
    CND_MM_LOCAL_CASE(CxxClDriverExternal, "cxx-cl-driver-external");
    CND_MM_LOCAL_CASE(CxxPredefs, "cxx-predefs");
    CND_MM_LOCAL_CASE(CxxSources, "cxx-sources");
    CND_MM_LOCAL_CASE(CxxIncludeDirs, "cxx-include-dirs");
    default:
      return "";
  }
#undef CND_MM_LOCAL_CASE
};

constexpr StrView GetFlagDescription(eFlag flag) noexcept {
#define CND_MM_LOCAL_CASE(n, lt) \
  case eFlag::k##n:              \
    return lt

  switch (flag) {
    CND_MM_LOCAL_CASE(ModeComp, "comp");
    CND_MM_LOCAL_CASE(ModeDev, "dev");
    CND_MM_LOCAL_CASE(ModeHelp, "help");
    CND_MM_LOCAL_CASE(ModeVersion, "version");
    CND_MM_LOCAL_CASE(Define, "define");
    CND_MM_LOCAL_CASE(OutDir, "out-dir");
    CND_MM_LOCAL_CASE(AuxDir, "aux-dir");
    CND_MM_LOCAL_CASE(WorkDir, "work-dir");
    CND_MM_LOCAL_CASE(SrcDirs, "src-dirs");
    CND_MM_LOCAL_CASE(LibDirs, "lib-dirs");
    CND_MM_LOCAL_CASE(ResDirs, "res-dirs");
    CND_MM_LOCAL_CASE(ExeDirs, "exe-dirs");
    CND_MM_LOCAL_CASE(SrcFiles, "src-files");
    CND_MM_LOCAL_CASE(LibFiles, "lib-files");
    CND_MM_LOCAL_CASE(ExeFiles, "exe-files");
    CND_MM_LOCAL_CASE(ResFiles, "res-files");
    CND_MM_LOCAL_CASE(NoOverwrite, "no-overwrite");
    CND_MM_LOCAL_CASE(CompIoSilent, "comp-io-silent");
    CND_MM_LOCAL_CASE(CompIoVerbose, "comp-io-verbose");
    CND_MM_LOCAL_CASE(CompIoDebug, "comp-io-debug");
    CND_MM_LOCAL_CASE(CompIoStd, "comp-io-std");
    CND_MM_LOCAL_CASE(CompIoTrace, "comp-io-trace");
    CND_MM_LOCAL_CASE(CompStdinRedir, "comp-stdin-redir");
    CND_MM_LOCAL_CASE(CompStdoutRedir, "comp-stdout-redir");
    CND_MM_LOCAL_CASE(CompStderrRedir, "comp-stderr-redir");
    CND_MM_LOCAL_CASE(DriverIoSilent, "driver-io-silent");
    CND_MM_LOCAL_CASE(DriverIoVerbose, "driver-io-verbose");
    CND_MM_LOCAL_CASE(DriverIoDebug, "driver-io-debug");
    CND_MM_LOCAL_CASE(DriverIoStd, "driver-io-std");
    CND_MM_LOCAL_CASE(DriverIoTrace, "driver-io-trace");
    CND_MM_LOCAL_CASE(DriverStdinRedir, "driver-stdin-redir");
    CND_MM_LOCAL_CASE(DriverStdoutRedir, "driver-stdout-redir");
    CND_MM_LOCAL_CASE(DriverStderrRedir, "driver-stderr-redir");
    CND_MM_LOCAL_CASE(Dump, "dump");
    CND_MM_LOCAL_CASE(HostLinker, "host-linker");
    CND_MM_LOCAL_CASE(HostLinkerType, "host-linker-type");
    CND_MM_LOCAL_CASE(HostLinkerVersion, "host-linker-version");
    CND_MM_LOCAL_CASE(HostLinkerDriver, "host-linker-driver");
    CND_MM_LOCAL_CASE(HostLinkerDriverType, "host-linker-driver-type");
    CND_MM_LOCAL_CASE(HostLinkerDriverExternal, "host-linker-driver-external");
    CND_MM_LOCAL_CASE(HostLinkerImplicitLibs, "host-linker-implicit-libs");
    CND_MM_LOCAL_CASE(HostCxxCl, "host-cxx-cl");
    CND_MM_LOCAL_CASE(HostCxxClType, "host-cxx-cl-type");
    CND_MM_LOCAL_CASE(HostCxxClVersion, "host-cxx-cl-version");
    CND_MM_LOCAL_CASE(HostCxxClDriver, "host-cxx-cl-driver");
    CND_MM_LOCAL_CASE(HostCxxClDriverType, "host-cxx-cl-driver-type");
    CND_MM_LOCAL_CASE(HostCxxClDriverExternal, "host-cxx-cl-driver-external");
    CND_MM_LOCAL_CASE(HostCxxPredefs, "host-cxx-predefs");
    CND_MM_LOCAL_CASE(HostCxxSources, "host-cxx-sources");
    CND_MM_LOCAL_CASE(HostCxxIncludeDirs, "host-cxx-include-dirs");
    CND_MM_LOCAL_CASE(TargetHost, "target-host");
    CND_MM_LOCAL_CASE(ProcArch, "proc-arch");
    CND_MM_LOCAL_CASE(OpSys, "op-sys");
    CND_MM_LOCAL_CASE(Linker, "linker");
    CND_MM_LOCAL_CASE(LinkerType, "linker-type");
    CND_MM_LOCAL_CASE(LinkerVersion, "linker-version");
    CND_MM_LOCAL_CASE(LinkerDriver, "linker-driver");
    CND_MM_LOCAL_CASE(LinkerDriverType, "linker-driver-type");
    CND_MM_LOCAL_CASE(LinkerDriverExternal, "linker-driver-external");
    CND_MM_LOCAL_CASE(LinkerImplicitLibs, "linker-implicit-libs");
    CND_MM_LOCAL_CASE(CxxCl, "cxx-cl");
    CND_MM_LOCAL_CASE(CxxClType, "cxx-cl-type");
    CND_MM_LOCAL_CASE(CxxClVersion, "cxx-cl-version");
    CND_MM_LOCAL_CASE(CxxClDriver, "cxx-cl-driver");
    CND_MM_LOCAL_CASE(CxxClDriverType, "cxx-cl-driver-type");
    CND_MM_LOCAL_CASE(CxxClDriverExternal, "cxx-cl-driver-external");
    CND_MM_LOCAL_CASE(CxxPredefs, "cxx-predefs");
    CND_MM_LOCAL_CASE(CxxSources, "cxx-sources");
    CND_MM_LOCAL_CASE(CxxIncludeDirs, "cxx-include-dirs");
    default:
      return "";
  }
#undef CND_MM_LOCAL_CASE
};

constexpr FlagMeta DefFlag(eFlag f, FlagProperties props, FlagMeta::FlagValidatorType validator) {
  return FlagMeta{
      f,     GetFlagInterp(f), GetFlagShortSymbol(f), GetFlagLongSymbol(f).data(), GetFlagDescription(f).data(),
      props, validator};
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Define command line parsers. Each parser will parse args up to a command, those count as the global args.
// Afterwards, driver will handle the parsed flags and a command specific parser will parse starting from
// after the command. Command with sub-commands will also parse 'command-global' arguments before parsing
// the sub-command's flags based on the given sub-command. Each command and subcommand must define it's own
// parser. 'MainCliParser' is the initial global parser.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace parsers {
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
  DefFlag(kOutDir),                                                   
  DefFlag(kAuxDir),
  DefFlag(kSources,FlagProperties{}.Repeatable()),
  DefFlag(kDefine)    
);


using MainCliParser = Parser<kMainParserFlags>;
using CompModeCliParser = Parser<kCompModeFlags>;

// clang-format on
}  // namespace parsers

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
ClRes<void> ConfigTranslationInput(TrInput& trin, const FlagMeta::FlagMapType& flags) { return ClRes<void>{}; };
ClRes<void> HandlePostComplation(const TrOutput& tr_out, const FlagMeta::FlagMapType& flags) {
  return ClRes<void>{};
};

ClRes<TrOutput> CliMain(int argc, char* argv[], char* envp[] = nullptr) {
  using cldev::util::gStdLog;
  using parsers::MainCliParser;
  using parsers::CompModeCliParser;

  // Parse global flags and main command.
  Vec<StrView> input_args{argv, argv + argc};
  MainCliParser::FlagMapType parsed_flags{};
  MainCliParser main_parser{};
  auto main_parse_res = main_parser.Parse(input_args.begin() + 1, input_args.end(), parsed_flags);
  ConfigLoggerVerbosity(gStdLog(), parsed_flags);  // Config log before possible error, in-case of '--silent'.
  if (!main_parse_res) return gStdLog().PrintErrForward(main_parse_res.error(), EXIT_FAILURE);

  // Parse and handle further flags based on main command.
  switch (main_parser.GetCommand()) {
    case eFlag::kModeComp: {
      CompModeCliParser comp_parser{};
      auto comp_parse_res = comp_parser.Parse(main_parse_res.value(), input_args.end(), parsed_flags);
      if (!comp_parse_res) return gStdLog().PrintErrForward(comp_parse_res.error(), EXIT_FAILURE);
      for (const auto& f : parsed_flags) {
        std::cout << eFlagToCStr(f.first) << ": " << std::get<StrView>(f.second) << std::endl;
      }

      TrInput trin{};
      ClRes<void> trin_config_res = ConfigTranslationInput(trin, parsed_flags);
      if (!trin_config_res) return gStdLog().PrintErrForward(trin_config_res.error().Format(), EXIT_FAILURE);

      trtools::Compiler compiler{trin};
      ClRes<TrOutput> tr_res = compiler.Translate(trin);
      if (!tr_res) return gStdLog().PrintErrForward(tr_res.error().Format(), EXIT_FAILURE);

      ClRes<void> post_res = HandlePostComplation(tr_res.value(), parsed_flags);
      if (!post_res) return gStdLog().PrintErrForward(post_res.error().Format(), EXIT_FAILURE);
    } break;
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
// struct IOConfig;              ///> Struct holding the initial IO configuration for the driver parsed on the first
// pass. struct CommandLineArguments;  ///> Fully parsed cli args data. Flags are stored as a std::multimap.
//
///// @brief Handles early exits, verbosity, and IO redirection cases for the C& CLI.
/////        Call this method before cnd::cli::ParseCliArgs to get the initial CLI configuration.
///// @return Unexpected value is an exit code, upon early exit. Expected value is the inital CLI params to pass to
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
///// @brief Creates a new logger to pass to other translation tools based on driver IOConfig parameters.
///// @param init_params IOConfig struct created using by HandleInitialCliArgs.
// Logger CreateConfiguredLogger(const IOConfig& init_params);
//
///// Syntactically parses the command line args passed to the C& CLI based on eDriverFlag data and interpretation.
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
//// ClResult<ParsedFlagOffset> ParseBooleanFlag(StrView flag_str, ArgsBufferIter flag_it, ArgsBufferIter args_end);
//
///// Format: [flag_str][whitespace][value]
// ClResult<ParsedFlagOffset> ParseSingleArgFlag(ArgsBufferIter args_it, ArgsBufferIter args_end, Size flag_offset = 0);
//
///// Format: [flag_str][[whitespace][arg]...]
// ClResult<ParsedFlagOffset> ParseMultiArgFlag(ArgsBufferIter args_it, ArgsBufferIter args_end, Size flag_offset = 0);
//
/////// Format: [flag_str][arg]
//// ClResult<ParsedFlagOffset> ParseSingleArgTightFlag(StrView flag_str, ArgsBufferIter flag_it, ArgsBufferIter
//// args_end);
////
/////// Format: [flag_str][[=]|[:]][arg]
//// ClResult<ParsedFlagOffset> ParseDefinitionFlag(StrView flag_str, ArgsBufferIter flag_it, ArgsBufferIter args_end);
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