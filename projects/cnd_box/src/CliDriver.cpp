/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// Copyright 2024 Anton Yashchenko
//// Licensed under the GNU Affero General Public License, Version 3.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// @project: C& Programming Language
//// @author(s): Anton Yashchenko
//// @website: https://www.acpp.dev
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///// @file
///// @ingroup
///// @brief
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
///// @addtogroup cnd_compiler_cli
///// @{
//
//// clang-format off
//#include "ccapi/CommonCppApi.hpp"
//#include "use_corevals.hpp"
//#include "use_clmsg.hpp"
//#include "cldev/dev_logger.hpp"
//#include "trtools/TrInput.hpp"
//#include "trtools/TrOutput.hpp"
//#include "corevals/cli_messages.hpp"
//#include "trtools/CliDriver.hpp"
//// clang-format on
//namespace cnd::driver {
//
//
//bool CommandLineArguments::ContainsFlag(eDriverFlag flag) const { return flags.contains(flag); }
//
//bool CommandLineArguments::ContainsFlag(eDriverFlag flag, StrView value) const {
//  auto range = flags.equal_range(flag);
//  for (auto& it = range.first; it != range.second; it++) {
//    if (std::find(it->second.cbegin(), it->second.cend(), value) != it->second.cend()) return true;
//  }
//  return false;
//}
//
//cldev::util::Logger CreateConfiguredLogger(const IOConfig& init_params) {
//  cldev::util::Logger log;
//  log.verbosity = init_params.verbosity_level;
//  if (!init_params.cli_out_stream.empty()) log.SetOutStream(init_params.cli_out_stream);
//  if (!init_params.cli_err_stream.empty()) log.SetErrStream(init_params.cli_err_stream);
//  if (!init_params.cli_in_stream.empty()) log.SetInStream(init_params.cli_in_stream);
//  return log;
//}
//
//namespace cliparser {
//
///// Format: [flag_str][space][value]
//ClResult<ParsedFlagOffset> ParseSingleArgFlag(ArgsBufferConstIter args_it, ArgsBufferConstIter args_end,
//                                              Size flag_offset) {
//  if (args_it == args_end)  // No value to parse
//    return ClFail(MakeClMsg<eClErr::kDriverFlagExpectedArgs>("", "<args-end>"));
//
//  // Begin parsing the flag arg from after the flag offset.
//  auto thisarg_it = args_it->cbegin() + flag_offset;
//  auto thisarg_end = args_it->cend();
//
//  // Skip any whitespace after the flag within this arg.
//  while (thisarg_it != thisarg_end && std::isspace(*thisarg_it)) thisarg_it++;
//
//  // If iter is at end of this arg(only whitespace after flag), and there is no following arg - failure, expected
//  // value.
//  if (thisarg_it == thisarg_end && std::next(args_it) == args_end)
//    return ClFail(MakeClMsg<eClErr::kDriverFlagExpectedArgs>("", "<args-end>"));
//
//  // If there is any data in this arg then the entire value is parsed from this arg.
//  if (thisarg_it != thisarg_end) {
//    auto thisval_it = thisarg_it;
//    // Consume all non-whitespace until end of arg.
//    while (thisval_it != thisarg_end && !std::isspace(*thisval_it)) thisval_it++;
//    return ParsedFlagOffset{.args = {{thisarg_it, thisval_it}}, .next_arg = args_it + 1, .arg_offset = 0};
//  }
//
//  // Advance args iter to the next argument which isn't only whitespace.
//  args_it++;
//  while (args_it != args_end && (args_it->find_first_not_of(' ') == std::string::npos)) args_it++;
//  if (args_it == args_end)  // No following value was found.
//    return ClFail(MakeClMsg<eClErr::kDriverFlagExpectedArgs>("", "<single-arg>"));
//
//  // Skip any preceding whitespace in the next arg.
//  auto nextarg_it = args_it->cbegin();
//  auto nextarg_end = args_it->cend();
//  while (nextarg_it != nextarg_end && std::isspace(*nextarg_it)) nextarg_it++;
//  if (*nextarg_it == '-')  // Cant start with a '-', expected a value not a flag.
//    return ClFail(MakeClMsg<eClErr::kDriverFlagExpectedArgs>("", "<single-arg>"));
//
//  // Entire next arg becomes the value. No parsing necessary.
//  return ParsedFlagOffset{.args = {{nextarg_it, nextarg_end}}, .next_arg = args_it + 1, .arg_offset = 0};
//};
//
///// Format: [flag_str][space][whitespace separated args]...
//ClResult<ParsedFlagOffset> ParseMultiArgFlag(ArgsBufferConstIter args_it, ArgsBufferConstIter args_end,
//                                             Size flag_offset) {
//  using std::make_unique;
//
//  // Multi value must have at-least one arg.
//  ClResult<ParsedFlagOffset> result = ParseSingleArgFlag(args_it, args_end, flag_offset);
//  if (!result) return result;
//
//  // Other args are optional, continue to parse a single arg and append to result until fail.
//  UPtr<ClResult<ParsedFlagOffset>> parse_result_buf{};
//  auto nextarg_it = result.value().next_arg;
//  auto nextarg_offset = result.value().arg_offset;
//  while (true) {
//    parse_result_buf =
//        make_unique<ClResult<ParsedFlagOffset>>(ParseSingleArgFlag(nextarg_it, args_end, nextarg_offset));
//
//    // Failed to parse next value, end of values for this flag arg.
//    if (!*parse_result_buf) break;
//
//    // Else add the value to the result, append to the 'result' variable to avoid temps.
//    result.value().args.append_range(parse_result_buf->value().args);
//
//    // Update loop iterators.
//    nextarg_it = parse_result_buf->value().next_arg;
//    nextarg_offset = parse_result_buf->value().arg_offset;
//  }
//
//  // Update result iterators to match last parsed arg.
//  result.value().next_arg = nextarg_it;
//  result.value().arg_offset = nextarg_offset;
//
//  return result;
//};
//
//// Dispatches to the appropriate flag parsing method based on the flag interp type.
//ClResult<ParsedFlagOffset> ParseDriverFlagValue(eDriverFlagInterp interp, ArgsBufferConstIter args_it,
//                                                ArgsBufferConstIter args_end, Size arg_offset) {
//  switch (interp) {
//    case eDriverFlagInterp::kOnOff:
//      return {{ArgsBuffer{}, cxx::AdvanceIt(args_it, 1)}};
//    case eDriverFlagInterp::kModeSwitch:
//      return {{ArgsBuffer{}, cxx::AdvanceIt(args_it, 1)}};
//    case eDriverFlagInterp::kSingleArg:
//      return ParseSingleArgFlag(args_it, args_end, arg_offset);
//    case eDriverFlagInterp::kMultiArg:
//      return ParseMultiArgFlag(args_it, args_end, arg_offset);
//    default:
//      return ClFail{
//          MakeClMsg<eClErr::kCompilerDevDebugError>(std::source_location{},
//                                                    "[cnd::driver::ParseOptFlag][Unknown or unimplemented "
//                                                    "eDriverFlagInterp enum type, cannot parse driver argument.]")};
//  }
//};
//
//}  // namespace cliparser
//
///// @brief Handles early exits, verbosity, and IO redirection cases for the C& CLI.
/////        Call this method before cnd::cli::ParseCliArgs to get the initial CLI configuration.
///// @return Expected value is exit code upon early exit. Unexpected value is the inital CLI params to pass to
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
//Ex<IOConfig, int> HandleInitialCliArgs(ArgsBufferConstIter beg, ArgsBufferConstIter end) {
//  // No args. Skip the program name(first) arg if it exists.
//  if (beg == end || beg++ == end) {  // 
//    std::cout << messages::kMainHeader << std::endl;
//    std::cout << messages::kBriefHelp << std::endl;
//    return Unex<int>{EXIT_SUCCESS};
//  } 
//
//  // Set defaults.
//  eDriverVerbosity verbosity_level = eDriverVerbosity::kStd;
//  bool is_help_run = false;
//  bool is_version_run = false;
//  bool is_overwrite_files = true;
//  Opt<ArgsBufferConstIter> opt_redir_out = std::nullopt;
//  Opt<ArgsBufferConstIter> opt_redir_err = std::nullopt;
//  Opt<ArgsBufferConstIter> opt_redir_in = std::nullopt;
//
//  for (auto it = beg; it != end; ++it) {
//    if (*it == "-h" || *it == "--help")
//      is_help_run = true;
//    else if (*it == "--silent")
//      verbosity_level = eDriverVerbosity::kSilent;
//    else if (*it == "--verbose")
//      verbosity_level = eDriverVerbosity::kVerbose;
//    else if (*it == "--diagnostic")
//      verbosity_level = eDriverVerbosity::kDebug;
//    else if (*it == "--no_overwrite")
//      is_overwrite_files = false;
//    else if (*it == "--stdout_redir")
//      opt_redir_out = it;
//    else if (*it == "--stderr_redir")
//      opt_redir_err = it;
//    else if (*it == "--stdin_redir")
//      opt_redir_in = it;
//    else if (*it == "--version")
//      is_version_run = true;
//    else
//      continue;
//  }
//
//  // If silent is option appears on a help or version run, we assume the user made a logical error.
//  // Print nothing to be safe. Return failure.
//  if ((is_help_run || is_version_run) && verbosity_level == eDriverVerbosity::kSilent) return Unex<int>{EXIT_FAILURE};
//
//  // Handle redirection of streams to files and validate output paths.
//  UPtr<std::ostream> redir_out_stream = nullptr;
//  UPtr<std::ostream> redir_err_stream = nullptr;
//  UPtr<std::istream> redir_in_stream = nullptr;
//  LAMBDA xValidateRedirPath = [&end, &verbosity_level, &is_overwrite_files](
//                                  StrView redir_flag, ArgsBufferConstIter redir_arg_it) -> Ex<void, int> {
//    if (redir_arg_it == end) {
//      auto this_error = MakeClMsg<eClErr::kDriverFlagExpectedArgs>(redir_flag, "<file-path>");
//      if (verbosity_level != eDriverVerbosity::kSilent) std::cerr << this_error.Format() << std::endl;
//      return Unex<int>{this_error.GetLastMessageCode()};
//    }
//
//    if (redir_arg_it->starts_with("-")) {
//      auto this_error = MakeClMsg<eClErr::kDriverFlagInvalidArg>(redir_flag, "<file-path>", *redir_arg_it);
//      if (verbosity_level != eDriverVerbosity::kSilent) std::cerr << this_error.Format() << std::endl;
//      return Unex<int>{this_error.GetLastMessageCode()};
//    }
//
//    if (!is_overwrite_files)
//      if (stdfs::exists(*redir_arg_it)) {
//        auto this_error = MakeClMsg<eClErr::kDriverDeniedOverwrite>(redir_flag, *redir_arg_it);
//        if (verbosity_level != eDriverVerbosity::kSilent) std::cerr << this_error.Format() << std::endl;
//        return Unex<int>{this_error.GetLastMessageCode()};
//      }
//
//    return Ex<void, int>{};
//  };
//
//  // Stdout
//  if (opt_redir_out) {
//    auto res = xValidateRedirPath("--stdout_redir", std::next(*opt_redir_out));
//    if (!res) return Unex<int>{res.error()};
//    redir_out_stream = std::make_unique<std::ofstream>(*std::next(*opt_redir_out), std::ios::out);
//    if (!*redir_out_stream) {
//      redir_out_stream.reset();
//      auto this_error = MakeClMsg<eClErr::kDriverFailedToRedirectStream>("--stdout_redir", *std::next(*opt_redir_out));
//      if (verbosity_level != eDriverVerbosity::kSilent) std::cerr << this_error.Format() << std::endl;
//      return Unex<int>{this_error.GetLastMessageCode()};
//    }
//  }
//
//  // Stderr
//  if (opt_redir_err) {
//    auto res = xValidateRedirPath("--stderr_redir", std::next(*opt_redir_err));
//    if (!res) return Unex<int>{res.error()};
//    redir_err_stream = std::make_unique<std::ofstream>(*std::next(*opt_redir_err), std::ios::out);
//    if (!*redir_err_stream) {
//      redir_err_stream.reset();
//      auto this_error = MakeClMsg<eClErr::kDriverFailedToRedirectStream>("--stdout_redir", *std::next(*opt_redir_err));
//      if (verbosity_level != eDriverVerbosity::kSilent) std::cerr << this_error.Format() << std::endl;
//      return Unex<int>{this_error.GetLastMessageCode()};
//    }
//  }
//
//  // Stdin
//  if (opt_redir_in) {
//    auto res = xValidateRedirPath("--stdin_redir", std::next(*opt_redir_in));
//    if (!res) return Unex<int>{res.error()};
//
//    redir_in_stream = std::make_unique<std::ifstream>(*std::next(*opt_redir_in), std::ios::in);
//    if (!*redir_in_stream) {
//      redir_in_stream.reset();
//      auto this_error = MakeClMsg<eClErr::kDriverFailedToRedirectStream>("--stdin_redir", *std::next(*opt_redir_in));
//      if (verbosity_level != eDriverVerbosity::kSilent) std::cerr << this_error.Format() << std::endl;
//      return Unex<int>{this_error.GetLastMessageCode()};
//    }
//  }
//
//  // Print help and exit.
//  if (is_help_run) {
//    // Assuming redir_out_stream is valid but double check wont hurt.
//    if (opt_redir_out && redir_out_stream) {
//      *redir_out_stream << messages::kMainHeader << std::endl;
//      *redir_out_stream << messages::kBriefHelp << std::endl;
//    } else {
//      std::cout << messages::kMainHeader << std::endl;
//      std::cout << messages::kBriefHelp << std::endl;
//    }
//    return Unex<int>{EXIT_SUCCESS};
//  }
//
//  // Print version and exit.
//  if (is_version_run) {
//    // Assuming redir_out_stream is valid but double check wont hurt.
//    if (opt_redir_out && redir_out_stream)
//      *redir_out_stream << messages::kCliVersionNumber << std::endl;
//    else
//      std::cout << messages::kCliVersionNumber << std::endl;
//    return Unex<int>{EXIT_SUCCESS};
//  }
//
//  // If we are here, we are not in a special "early exit" case.
//  return Ex<IOConfig, int>{
//      {.verbosity_level = verbosity_level,
//       .is_help_run = is_help_run,
//       .is_version_run = is_version_run,
//       .is_overwrite_files = is_overwrite_files,
//       .cli_out_stream = (opt_redir_out && redir_out_stream) ? *std::next(opt_redir_out.value()) : "",
//       .cli_err_stream = (opt_redir_err && redir_err_stream) ? *std::next(opt_redir_err.value()) : "",
//       .cli_in_stream = (opt_redir_in && redir_in_stream) ? *std::next(opt_redir_in.value()) : ""}};
//}
//
///// @brief Syntactically parses the command line args passed to the C& CLI based on eDriverFlag data
/////        and flag interpretation.
//ClResult<CommandLineArguments> ParseCliArgs(int argc, char* argv[], IOConfig& init_params) {
//  using std::distance;
//  using std::format;
//  using std::get;
//  using namespace cliparser;
//
//  ArgsBuffer in(argv + 1, argv + argc);   // skips the program name (argv[0])
//  CommandLineArguments out{init_params};  // parsed data format
//
//  // For each arg...
//  Size arg_offset{0};
//  for (auto argit = in.cbegin(); argit != in.cend();) {
//    if (argit->starts_with('-')) {                     // Flag type arg.
//      if (argit->size() > 2 && argit->at(1) == '-') {  // Long form "--flag" detected.
//        if (argit->substr(2).empty()) break;           // Empty flag '--' detected, end of options.
//
//        // Tokenize the flag identifier (alphanumeric,'_','-',NOT '--'). Start iterating from after the initial dashes.
//        auto ident_it = argit->begin() + 2;
//        while (ident_it != argit->end()) {
//          if (*ident_it == '-' && *(ident_it + 1) == '-') break;
//          if ((IsSrcCharAlnumus(*ident_it) || *ident_it == '-'))
//            ident_it++;
//          else
//            break;
//        }
//
//        // Get the enum corresponding to the long flag identifier.
//        eDriverFlag this_flag = GetDriverFlagFromStr({argit->begin() + 2, ident_it});
//        if (this_flag == eDriverFlag::kNONE)  // No matching flag.
//          return ClFail{MakeClMsg<eClErr::kDriverInvalidArg>(format("Unknown long flag symbol:{}.", argit->substr(2)))};
//
//        // Get flag interpretation method and offset.
//        eDriverFlagInterp interp = GetDriverFlagInterp(this_flag);
//        arg_offset = distance(argit->begin(), ident_it);
//
//        // Parse the flag.
//        ClResult<ParsedFlagOffset> parse_res = ParseDriverFlagValue(interp, argit, in.end(), arg_offset);
//        if (!parse_res) return ClFail{parse_res.error()};
//
//        out.flags.insert({this_flag, parse_res.value().args});  // Add to output.
//        argit = parse_res.value().next_arg;                     // Advance past result
//
//      } else {  // Short form flag "-F".
//        if (argit->size() < 2)
//          return ClFail{MakeClMsg<eClErr::kDriverInvalidArg>("Empty flag '-' is not a valid C& compiler argument.")};
//
//        // For short form arguments only the first character is significant for detecting the flag.
//        // For non 'kOnOff' flags, the following characters are part of the 'value' passed to the flag.
//        eDriverFlag this_flag = GetDriverFlagFromChar(*(argit->begin() + 1));
//        if (this_flag == eDriverFlag::kNONE)
//          return ClFail{MakeClMsg<eClErr::kDriverInvalidArg>(format("Unknown short flag symbol:{}.", *argit))};
//
//        eDriverFlagInterp interp = GetDriverFlagInterp(this_flag);
//        arg_offset = 2;  // '-' + one alpha char
//        ClResult<ParsedFlagOffset> parse_res = ParseDriverFlagValue(interp, argit, in.end(), arg_offset);
//        if (!parse_res) return ClFail{parse_res.error()};
//        out.flags.insert({this_flag, parse_res.value().args});
//        argit = parse_res.value().next_arg;
//      }
//    } else {  // Positional Arg
//      out.args.push_back(*argit);
//      argit++;
//    }
//  }
//
//  ConfigureCliArgsMode(out);  // Configure mode based on the parsed flags and positional arguments.
//  return out;
//}
//
//void ConfigureCliArgsMode(CommandLineArguments& inargs) {
//  // Configure the mode. First positional arg should be the mode.
//  if (inargs.args.size() > 1) {
//    switch (GetDriverFlagFromStr(inargs.args[0])) {
//      case eDriverFlag::kModeComp:
//        inargs.mode = eDriverFlag::kModeComp;
//        inargs.args.erase(inargs.args.begin());
//        return;
//      case eDriverFlag::kModeDev:
//        inargs.mode = eDriverFlag::kModeDev;
//        inargs.args.erase(inargs.args.begin());
//        return;
//      case eDriverFlag::kModeVersion:
//        inargs.mode = eDriverFlag::kModeVersion;
//        inargs.args.erase(inargs.args.begin());
//        return;
//      case eDriverFlag::kModeHelp:
//        inargs.mode = eDriverFlag::kModeHelp;
//        inargs.args.erase(inargs.args.begin());
//        return;
//    }
//  }
//  // If there is no matching first positional. Find the mode flag, first found decides mode.
//  auto mode_found = std::find_if(inargs.flags.cbegin(), inargs.flags.cend(), [&inargs](const auto& flag) {
//    return flag.first == eDriverFlag::kModeComp || flag.first == eDriverFlag::kModeDev ||
//           flag.first == eDriverFlag::kModeVersion || flag.first == eDriverFlag::kModeHelp;
//  });
//  if (mode_found != inargs.flags.cend()) {
//    inargs.mode = mode_found->first;
//  } else
//    inargs.mode = eDriverFlag::kModeHelp;  // No mode flag found, default to help mode.
//}
//
//ClResult<int> HandleCliArgsVersionMode(CommandLineArguments& inargs) {
//  // Not implemented
//  return cldev::util::gStdLog().PrintErrForward(MakeClMsg<eClErr::kCompilerDevDebugError>(
//      std::source_location::current(), "cnd::cli::HandleCliArgsVersionMode  is not implemented yet."));
//};
//
//ClResult<int> HandleCliArgsHelpMode(CommandLineArguments& inargs) {
//  // Not implemented
//  return cldev::util::gStdLog().PrintErrForward(MakeClMsg<eClErr::kCompilerDevDebugError>(
//      std::source_location::current(), "cnd::cli::HandleCliArgsHelpMode is not implemented yet."));
//};
//
//// - Working directory is set first.
//// - Out dir is absolute relative to the work dir.
//// - Aux dir is absolute or relative to the work dir.
//// - Src dirs are absolute or relative to the work dir.
//// - Get source directories, paths must already exist on the system.
////   Get the positional arguments as source files first.
////   For all non-absolute paths, check all search paths in order:
////      1. Working Directory
////      2. Additional Source Directories
////   Else look for the file at the absolute path.
//// - Predefined values are added.
//// - Based on freestanding or hosted mode, add the appropriate toolchain flags.
////   This data should be available at compile time of this program.
////   Defaults are set for the host triplet and target triple only if found.
////   Default are only set if not passed.
//ClResult<TrInput> HandleCliArgsCompMode(CommandLineArguments& inargs) {
//  using enum eDriverFlag;
//  assert(inargs.mode == eDriverFlag::kModeComp && "cnd::cli::HandleCliArgsCompMode expects composition mode flag.");
//  assert(!inargs.ContainsFlag(kModeDev) && "cnd::cli::HandleCliArgsCompMode does not expect development mode flag.");
//  assert(!inargs.ContainsFlag(kModeHelp) && "cnd::cli::HandleCliArgsCompMode does not expect help mode flag.");
//  assert(!inargs.ContainsFlag(kModeVersion) && "cnd::cli::HandleCliArgsCompMode does not expect version mode flag.");
//
//  auto params = TrInput{&cldev::util::gStdLog()};
//  auto& glog = params.cli_stdio;
//  params.cli_verbosity_level = inargs.io_config.verbosity_level;
//  params.is_overwrite_allowed = inargs.io_config.is_overwrite_files;
//
//  params.work_dir = inargs.ContainsFlag(kWorkDir) ? inargs.flags.find(kWorkDir)->second[0] : stdfs::current_path();
//  params.out_dir = inargs.ContainsFlag(kOutDir) ? inargs.flags.find(kOutDir)->second[0] : "out";
//  params.aux_dir = inargs.ContainsFlag(kAuxDir) ? inargs.flags.find(kAuxDir)->second[0] : "aux";
//
//  auto [add_src_dirs_beg, add_src_dirs_end] = inargs.flags.equal_range(kSrcDirs);
//  for (const auto& [flag, args] : std::ranges::subrange(add_src_dirs_beg, add_src_dirs_end)) {
//    for (const auto& arg : args) {
//      stdfs::path src_dir = arg;
//      if (src_dir.is_absolute()) {
//        if (!stdfs::exists(src_dir))
//          return ClFail(MakeClMsg<eClErr::kFailedToReadFile>(src_dir.string(), "Could not find source directory."));
//      } else {
//        src_dir = params.work_dir / arg;
//        if (!stdfs::exists(src_dir))
//          return ClFail(MakeClMsg<eClErr::kFailedToReadFile>(src_dir.string(), "Could not find source directory."));
//      }
//      params.src_dirs.push_back(src_dir);
//    }
//  }
//
//  for (const auto& arg : inargs.args) {
//    stdfs::path src_file{arg};
//    bool found = false;
//    if (src_file.is_absolute()) {
//      if (!stdfs::exists(src_file))
//        return ClFail(MakeClMsg<eClErr::kFailedToReadFile>(src_file.string(), "Could not find source file."));
//      found = true;  // Absolute path found.
//    } else {
//      auto exists_relative_to_work = stdfs::exists(params.work_dir / src_file);
//      if (exists_relative_to_work) {
//        src_file = params.work_dir / src_file;
//        found = true;  // Relative path found.
//      }
//      // Check all source dirs and find which one contains the file.
//      else {
//        auto exists_relative_to_src =
//            std::find_if(params.src_dirs.cbegin(), params.src_dirs.cend(),
//                         [&src_file](const auto& src_dir) { return stdfs::exists(src_dir / src_file); });
//        if (exists_relative_to_src != params.src_dirs.cend()) {
//          src_file = *exists_relative_to_src / src_file;
//          found = true;
//        }
//      }
//    }
//    if (!found)
//      return ClFail(MakeClMsg<eClErr::kFailedToReadFile>(src_file.string(), "Could not find source file."));
//    else
//      params.src_files.push_back(src_file);  // Add source file to the params.
//  }
//
//  auto [predefs_beg, predefs_end] = inargs.flags.equal_range(eDriverFlag::kDefine);
//  for (const auto& a : std::ranges::subrange(predefs_beg, predefs_end)) {
//    params.predefs.push_back(std::make_pair(a.second[0], a.second[1]));
//  }
//
//  return params;
//};
//
//ClResult<int> HandleCliArgsPostCompMode(TrInput& params, TrOutput& output) {
//  return cldev::util::gStdLog().PrintErrForward(
//      MakeClMsg<eClErr::kCompilerDevDebugError>(std::source_location{}, "cnd::cli::HandleCliArgsPostCompMode"));
//}
//
//ClResult<int> HandleCliArgsDevMode(CommandLineArguments& inargs) {
//  // Not implemented
//  return cldev::util::gStdLog().PrintErrForward(
//      MakeClMsg<eClErr::kCompilerDevDebugError>(std::source_location{}, "cnd::cli::HandleCliArgsDevMode"));
//};
//
//ClResult<eDriverFlag> ChooseCliArgsMode(const CommandLineArguments& inargs) {
//  using enum eDriverFlag;
//  // Help mode overrides all and does not error on invalid args.
//  if (inargs.flags.contains(kModeHelp)) return ClResult<eDriverFlag>{kModeHelp};
//  // Version mode overrides all and does not error on invalid args.
//  if (inargs.flags.contains(kModeVersion)) return ClResult<eDriverFlag>{kModeVersion};
//  // Composition mode
//  if (inargs.ContainsFlag(kModeComp))
//    if (inargs.ContainsFlag(kModeDev))
//      return ClFail{MakeClMsg<eClErr::kDriverInvalidArg>("Cannot specify both composition and development modes.")};
//    else
//      return ClResult<eDriverFlag>{kModeComp};
//  // Development mode
//  if (inargs.ContainsFlag(kModeDev)) return ClResult<eDriverFlag>{kModeDev};
//  // Default to help mode if no mode flags are present and we reached this point.
//  return ClResult<eDriverFlag>{kModeHelp};
//}
//
//namespace detail {
//void RemoveQuotesFromCliArg(char* arg) {
//  Str str{arg};
//  bool changed = false;
//  while (str.size() > 2 && str.front() == '"' && str.back() == '"' &&
//         str[str.size() - 2] != '\\')  // last escaped quote case
//  {
//    str = str.substr(1, str.size() - 2);
//    changed = true;
//  }
//  if (changed) {
//    delete arg;  // clear memory before overwriting
//    arg = new char[str.size() + 1];
//    std::copy(str.begin(), str.end(), arg);
//  }
//}
//
//Pair<Str::const_iterator, Str::const_iterator> ParseQuotedDriverArg(Str::const_iterator beg,
//                                                                    Str::const_iterator end) noexcept {
//  assert(beg != end && *beg == '\"');
//  Str::const_iterator it = beg;
//  // Advance until a non-escaped quotes are detected.
//  while (it != end && !(*it == '"' && *std::prev(it) != '\\')) it++;
//  // Advance past the last quote
//  it++;
//  return std::make_pair(beg, it);
//}
//
//void DebugPrintParsedCliArgs(CommandLineArguments& cli_args) {
//  std::cout << "Parsed " << cli_args.args.size() << " arguments:" << std::endl;
//  int i = 0;
//  for (Str& arg : cli_args.args) {
//    std::cout << "Arg " << i << " : " << arg << std::endl;
//    i++;
//  }
//
//  i = 0;
//  int arg_i = 0;
//  for (auto& f : cli_args.flags) {
//    arg_i = 0;
//    std::cout << "[FLAG " << i << "]: " << GetDriverFlagLongSymbol(f.first) << std::endl;
//    if (f.second.size() != 0) {
//      std::cout << "-->[ARGS]: " << std::endl;
//      for (Str& arg : f.second) {
//        std::cout << "  |-->[ARG " << arg_i << "]: " << arg << std::endl;
//        arg_i++;
//      }
//    }
//    i++;
//  }
//}
//}  // namespace detail
//
//}  // namespace cnd::driver
//
///// @} // end of cnd_compiler_cli
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// @project: C& Programming Language
//// @author(s): Anton Yashchenko
//// @website: https://www.acpp.dev
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// Copyright 2024 Anton Yashchenko
////
//// Licensed under the GNU Affero General Public License, Version 3.
//// you may not use this file except in compliance with the License.
//// You may obtain a copy of the License at
////
//// http://www.apache.org/licenses/LICENSE-2.0
////
//// Unless required by applicable law or agreed to in writing, software
//// distributed under the License is distributed on an "AS IS" BASIS,
//// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//// See the License for the specific language governing permissions and
//// limitations under the License.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////