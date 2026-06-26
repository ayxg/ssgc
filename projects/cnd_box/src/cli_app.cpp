#include "cli_app.hpp"

#include "cli_enum_cli_flag.hpp"
#include "cli_parser.hpp"
#include "diagnostics.hpp"
#include "frontend_lexer.hpp"
#include "translation_context.hpp"
#include "util_enum_verbosity.hpp"
#include "util_read_file.hpp"

namespace ssgc::cli {

constexpr int kExitFailure = EXIT_FAILURE;
constexpr int kExitSuccess = EXIT_SUCCESS;
constexpr std::string_view kProgramName = "Sofia Silicon Gold Compiler";
constexpr std::string_view kProgramVersion = "0.0.0";

int run(int argc, char* argv[]);
int runDisplayHelp(TrContext& context);
int runDisplayVersion(TrContext& context);
int runCommandBuild(TrContext& context, CliArgs::const_iterator rest, CliArgs::const_iterator end);
int runCommandDev(TrContext& context, CliArgs::const_iterator rest, CliArgs::const_iterator end);
int runCommandDevLex(TrContext& context, CliArgs::const_iterator rest, CliArgs::const_iterator end);
int runUnknownCommand(TrContext& context, const std::source_location& src_loc, eCliFlag command);

int run(int argc, char* argv[]) {
  // Initialize a translation context and parse global command line arguments.
  TrContext context{};
  CliArgs raw_args = getCommandLineArgs(argc, argv);
  CliParserResult args_parse_result = parseCliMain(raw_args.begin(), raw_args.end(), context.args);

  // Configure verbosity before outputting any potential errors.
  if (context.args.contains(eCliFlag::kDriverIoSilent)) {
    context.log.setVerbosity(util::eVerbosity::kSilent);
  } else if (context.args.contains(eCliFlag::kDriverIoVerbose)) {
    context.log.setVerbosity(util::eVerbosity::kVerbose);
  } else if (context.args.contains(eCliFlag::kDriverIoDebug)) {
    context.log.setVerbosity(util::eVerbosity::kDebug);
  } else if (context.args.contains(eCliFlag::kDriverIoTrace)) {
    context.log.setVerbosity(util::eVerbosity::kTrace);
  } else {
    context.log.setVerbosity(util::eVerbosity::kDefault);
  }

  // Run display flag even if parsing command line arguments failed.
  if (args_parse_result.display_flag) {
    switch (*args_parse_result.display_flag) {
      case eCliFlag::kHelp:
        return runDisplayHelp(context);
      case eCliFlag::kVersion:
        return runDisplayVersion(context);
      default:
        return runUnknownCommand(context, std::source_location::current(),
                                 *args_parse_result.command_flag);
    }
  }

  // Exit early on invalid command line arguments.
  if (args_parse_result.failed()) {
    context.diagnostics->append_range(
        std::ranges::subrange(args_parse_result.errors->begin(), args_parse_result.errors->end()));
    context.log.err << context.diagnostics.format();
    return kExitFailure;
  }

  // Dispatch to chosen command parser.
  CliArgs::const_iterator rest_args_begin = raw_args.begin() + args_parse_result.rest_args;
  switch (*args_parse_result.command_flag) {
    case eCliFlag::kCommandBuild:
      return runCommandBuild(context, rest_args_begin, raw_args.end());
    case eCliFlag::kCommandDev:
      return runCommandDev(context, rest_args_begin, raw_args.end());
    default:
      return runUnknownCommand(context, std::source_location::current(),
                               *args_parse_result.command_flag);
  }
};

int runDisplayHelp(TrContext& context) {
  context.log.out << std::format("{}\n{}\n{}", kProgramName, kProgramVersion, helptextCliMain()) << std::endl;
  return kExitSuccess;
};

int runDisplayVersion(TrContext& context) {
  context.log.out << kProgramVersion << std::endl;
  return kExitSuccess;
};

int runCommandBuild(TrContext& context, CliArgs::const_iterator rest, CliArgs::const_iterator end) {
  CliParserResult args_parse_result = parseCliCommandBuild(rest, end, context.args);
  if (args_parse_result.failed()) {
    context.diagnostics->append_range(
        std::ranges::subrange(args_parse_result.errors->begin(), args_parse_result.errors->end()));
    context.log.err << context.diagnostics.format();
    return kExitFailure;
  }

  return kExitSuccess;
}

int runCommandDev(TrContext& context, CliArgs::const_iterator rest, CliArgs::const_iterator end) {
  CliParserResult args_parse_result = parseCliCommandDev(rest, end, context.args);

  if (args_parse_result.failed()) {
    context.diagnostics->append_range(
        std::ranges::subrange(args_parse_result.errors->begin(), args_parse_result.errors->end()));
    context.log.err << context.diagnostics.format();
    return kExitFailure;
  }

  int exit_code = kExitFailure;
  switch (*args_parse_result.command_flag) {
    case eCliFlag::kDevModeLex:
      return runCommandDevLex(context, rest + args_parse_result.rest_args, end);
    default:
      return runUnknownCommand(context, std::source_location::current(),
                               *args_parse_result.command_flag);
  }

  return exit_code;
}

int runCommandDevLex(TrContext& context, CliArgs::const_iterator rest,
                     CliArgs::const_iterator end) {
  CliParserResult args_parse_result = parseCliCommandDevLex(rest, end, context.args);

  if (args_parse_result.failed()) {
    context.log.err << args_parse_result.errors.format();
    return kExitFailure;
  }

  frontend::Lexer lexer{};
  struct FileLexResult_ {
    const util::SourceFile* source{nullptr};
    std::vector<frontend::Token> tokens{};
    diagnostic::Diagnostics diagnostics{};
  };

  std::vector<FileLexResult_> results{};
  for (const auto& src_file_path : context.args.at(eCliFlag::kSources)) {
    results.push_back(FileLexResult_{});
    auto& this_res = results.back();

    // Skip files which were already loaded.
    if (context.sources.contains(src_file_path)) {
      this_res.diagnostics->push_back(
          diagnostic::Diagnostic(std::format("Duplicate file ignored. Path: {}", src_file_path)));
      continue;
    }

    auto load_file_result = context.sources.load(src_file_path);
    if (!load_file_result) {
      this_res.diagnostics->push_back(load_file_result.error());
      continue;
    }
    this_res.source = *load_file_result;

    auto lex_result = lexer.tokenize(this_res.source->data);
    if (!lex_result) {
      this_res.diagnostics.append(lex_result.error());
    }
    this_res.tokens = std::move(*lex_result);
  }

  std::string debug_print_data{};
  for (const auto& result : results) {
    debug_print_data += "{\n";
    debug_print_data += std::format("\"file\" : \"{}\"\n", context.sources.pathof(result.source));
    if (!result.tokens.empty()) {
      debug_print_data += "\"tokens\" : [\n\t";
      for (auto token_iter = result.tokens.cbegin(); token_iter < result.tokens.cend() - 1;
           token_iter++) {
        debug_print_data += std::format("\"{}\",", [&token_iter] {
          const char* tkcstr = frontend::eTokenToCStr(token_iter->type_);
          return std::string_view{tkcstr + 1};
        }());
        if (token_iter->type_ == frontend::eToken::kNewline) {
          debug_print_data += "\n\t";
        }
      }
      debug_print_data +=
          std::format("\"{}\"\n]\n", frontend::eTokenToCStr(result.tokens.back().type_));
    }
    if (!result.diagnostics->empty()) {
      debug_print_data += "\"diagnostics\" : [\n";
      for (auto diagnostic_iter = result.diagnostics->cbegin();
           diagnostic_iter < result.diagnostics->cend() - 1; diagnostic_iter++) {
        debug_print_data += std::format("\"{}\",", diagnostic_iter->format());
      }
      debug_print_data += std::format("\"{}\"\n]\n", result.diagnostics->back().format());
    }
    debug_print_data += "}\n\n";
  }

  context.log.out << debug_print_data;
  return kExitSuccess;
}

int runUnknownCommand(TrContext& context, const std::source_location& src_loc, eCliFlag command) {
  context.diagnostics->push_back(diagnostic::makeErrorDeveloperBug(
      src_loc,
      std::format("Unknown or unimplemented command line flag: {}.", eCliFlagToCStr(command))));
  context.log.err << context.diagnostics.format();
  return kExitFailure;
}

}  // namespace ssgc::cli
