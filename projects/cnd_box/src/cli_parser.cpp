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
#include "cli_parser.hpp"

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

#include "cli_enum_cli_flag.hpp"
#include "diagnostics.hpp"

namespace ssgc::cli {

/// Returns a collection of the command line arguments excluding the first
/// argument which is the executable path.
CliArgs getCommandLineArgs(int argc, char* argv[]) {
  if (argc < 2) return {};
  return {&argv[1], argv + argc};
}

// Determines how a flag will be interpreted by the parser.
enum class eFlagInterp {
  kNONE,
  kOpt,     // <opt>    ::= "-" <short-ident> | "--" <long-ident>
  kSingle,  // <single> ::= "-" <short-ident> " " <positional> | "--"
            // <long-ident> " " <positional>
  kMulti,   // <multi>  ::= "-" <short-ident> " " <positional>* | "--"
            // <long-ident> " " <positional>*
  kPositional,
  kCommand,  // Command, short circuits the parser and provides the iterator
             // to start from for the following parser.

  kDisplay,  // Short circuits the parser and calls a validator function. Can be
             // used for `--help` or `--version` flags.
  kVarDef
};

// Additional flag properties.
class CliFlagProperties {
 public:
  enum eProperty : std::uint32_t {
    // Applies to eFlagInterp : kSingle, kOpt, kMulti, kCommand, kPositional.
    // By default, all flags are optional.
    kRequired = (1 << 0),

    // Applies to eFlagInterp : kSingle, kOpt, kMulti.
    // kCommand type flags cannot be repeatable, parsing a command
    // short-circuits
    // the parser.
    // kPositional flags marked as repeatable will cause the parser consume the
    // rest of the positional arguments into
    // that flag. Meaning positional flags declared after the repeatable
    // positional will not be parsed.
    // By default, repeated flags will trigger a parsing error.
    kRepeatable = (1 << 1),

    // Applies to eFlagInterp : kSingle, kMulti.
    // By default no value will trigger a parsing error.
    kValueOptional = (1 << 2)
  };

  constexpr CliFlagProperties(std::uint32_t bits = 0) : bits_(bits) {}

  constexpr bool IsRequired() const { return bits_ & eProperty::kRequired; }
  constexpr bool IsRepeatable() const { return bits_ & eProperty::kRepeatable; }
  constexpr bool IsValueOptional() const { return bits_ & eProperty::kValueOptional; }

  constexpr CliFlagProperties Required() const { return bits_ | eProperty::kRequired; }
  constexpr CliFlagProperties Repeatable() const { return bits_ | eProperty::kRepeatable; }
  constexpr CliFlagProperties ValueOptional() const { return bits_ | eProperty::kValueOptional; }
  constexpr operator std::uint32_t() const { return bits_; }

 private:
  std::uint32_t bits_;
};

struct CliFlag {
  using eCliFlag = eCliFlag;
  using CliFlagMap = std::unordered_map<eCliFlag, std::vector<std::string>>;
  using FlagValidatorType = bool (*)(const CliFlagMap&, eCliFlag);
  eCliFlag id{0};
  eFlagInterp interp{eFlagInterp::kNONE};
  char short_name{' '};
  const char* long_name{""};
  const char* desc{""};
  std::uint32_t properties{};
  bool (*validator)(const CliFlagMap&, eCliFlag){nullptr};
};

struct ParserState {
  bool is_done{false};
  std::span<const std::string_view> args{};
  std::size_t current_index{0};
  std::size_t current_positional{0};
  CliFlagMap* output{nullptr};

  std::string_view currentArg() const {
    if (current_index < args.size()) return args[current_index];
    return {};
  }
  void pushToOutput(eCliFlag id, std::string_view value) {
    if (output == nullptr) return;
    (*output)[id].push_back(std::string{value});
  }
};

template <auto& FLAG_ARRAY>
class CliParser {
 public:
  using FlagMetadataArrayType = std::decay_t<decltype(FLAG_ARRAY)>;

  static CliParserResult parse(CliArgs::const_iterator beg, CliArgs::const_iterator end,
                               CliFlagMap& out);
  static std::string helptext();

 private:
  static diagnostic::Diagnostic parsingFail(const std::string& message);

  // Check required args now accepts result as output target.
  static void checkRequiredArgs(const CliFlagMap& out, CliParserResult& result);

  static const CliFlag* lookupFlagMetadata(
      const std::unordered_map<std::string_view, std::size_t>& table, std::string_view key);

  static const CliFlag* lookupFlagMetadata(const std::unordered_map<char, std::size_t>& table,
                                           char key);

  static void parseLongFlag(ParserState& state, CliParserResult& result, CliFlagMap& out);

  static void parseShortFlag(ParserState& state, CliParserResult& result, CliFlagMap& out);

  static void parsePositionalArg(ParserState& state, CliParserResult& result, CliFlagMap& out);

  static void processFlag(const CliFlag& flag_metadata, std::size_t identifier_offset,
                          ParserState& state, CliParserResult& result, CliFlagMap& out);

  static void parseCommandArg(ParserState& state, CliParserResult& result, CliFlagMap& out);

  // Flags metadata
  static constexpr const FlagMetadataArrayType flags_ = FLAG_ARRAY;

  static constexpr inline bool kMetadataHasCommand = []() constexpr {
    for (std::size_t i = 0; i < flags_.size(); ++i) {
      if (flags_[i].interp == eFlagInterp::kCommand) return true;
    }
    return false;
  }();

  static constexpr inline bool kMetadataHasPositional = []() constexpr {
    for (std::size_t i = 0; i < flags_.size(); ++i) {
      if (flags_[i].interp == eFlagInterp::kPositional) return true;
    }
    return false;
  }();

  static constexpr inline std::size_t kMetadataPositionalCount = []() constexpr {
    std::size_t cnt = 0;
    for (std::size_t i = 0; i < flags_.size(); ++i)
      if (flags_[i].interp == eFlagInterp::kPositional) ++cnt;
    return cnt;
  }();

  // Lookup table, short ident -> parser flag index
  static const inline std::unordered_map<char, std::size_t> lookup_short_{[] {
    std::unordered_map<char, size_t> ret{};
    for (size_t i = 0; i < flags_.size(); ++i) ret[flags_[i].short_name] = i;
    return ret;
  }()};

  // Lookup table, long ident -> parser flag index
  static const inline std::unordered_map<std::string_view, std::size_t> lookup_long_{[] {
    std::unordered_map<std::string_view, size_t> ret;
    for (size_t i = 0; i < flags_.size(); ++i) ret[flags_[i].long_name] = i;
    return ret;
  }()};

  // Lookup table, command -> parser flag index
  static const inline std::unordered_map<std::string_view, std::size_t> lookup_command_{[] {
    std::unordered_map<std::string_view, size_t> ret;
    for (size_t i = 0; i < flags_.size(); ++i) {
      if (flags_[i].interp == eFlagInterp::kCommand) ret[flags_[i].long_name] = i;
    }
    return ret;
  }()};

  // Lookup table,positional index -> parser flag index
  static constexpr inline std::array<std::size_t, kMetadataPositionalCount> lookup_pos_{[]() {
    std::array<size_t, kMetadataPositionalCount> ret{};
    size_t pos_index = 0;
    for (size_t i = 0; i < flags_.size(); ++i)
      if (flags_[i].interp == eFlagInterp::kPositional) ret[pos_index++] = i;
    return ret;
  }()};

  // Assert all flag enum values are unique.
  static_assert(
      [](const FlagMetadataArrayType& flags) {
        for (size_t i = 0; i < flags.size(); ++i)
          for (size_t j = i + 1; j < flags.size(); ++j)
            if (flags[i].id == flags[j].id) return false;
        return true;
      }(flags_),
      "Duplicate flag enums found in FlagMetadataArrayType. All flag enum values "
      "must be unique.");

  // Assert all flags have at least a short or long name (except positionals)
  static_assert(
      [](const FlagMetadataArrayType& flags) {
        for (const auto& flag : flags) {
          if (flag.interp == eFlagInterp::kPositional) continue;
          if (flag.short_name == ' ' && (flag.long_name == nullptr || flag.long_name[0] == '\0'))
            return false;
        }
        return true;
      }(flags_),
      "Found flags with no short name and no long name. All non-positional "
      "flags must have a short and/or long name.");

  // Assert all long flag names are unique (ignoring empty long names).
  static_assert(
      [](const FlagMetadataArrayType& flags) {
        for (size_t i = 0; i < flags.size(); ++i) {
          for (size_t j = i + 1; j < flags.size(); ++j) {
            std::string_view long_name_i{flags[i].long_name};
            std::string_view long_name_j{flags[j].long_name};
            if (long_name_i.empty() || long_name_j.empty()) {
              continue;
            }

            if (long_name_i == long_name_j) {
              return false;
            }
          }
        }
        return true;
      }(flags_),
      "Duplicate long names found in FlagMetadataArrayType.");

  // Assert all short flag chars are unique (ignoring ' ').
  static_assert(
      [](const FlagMetadataArrayType& flags) {
        for (size_t i = 0; i < flags.size(); ++i) {
          for (size_t j = i + 1; j < flags.size(); ++j) {
            char short_name_i = flags[i].short_name;
            char short_name_j = flags[j].short_name;
            if (short_name_i == ' ' || short_name_j == ' ') {
              continue;
            }
            if (short_name_i == short_name_j) {
              return false;
            }
          }
        }
        return true;
      }(flags_),
      "Duplicate short names found in FlagMetadataArrayType.");
};

template <auto& FLAG_ARRAY>
std::string CliParser<FLAG_ARRAY>::helptext() {
  auto xIndentParagraph = [](const std::string& text, size_t indent_width) {
    std::string indent(indent_width, ' ');
    std::string indented_text;
    size_t pos = 0;
    while (pos < text.size()) {
      indented_text += indent;
      size_t next_pos = text.find('\n', pos);
      if (next_pos == std::string::npos) {
        indented_text += text.substr(pos);
        break;
      } else {
        indented_text += text.substr(pos, next_pos - pos + 1);
        pos = next_pos + 1;
      }
    }
    return indented_text;
  };

  std::string help_text;

  // Display commands if exist.
  if constexpr (kMetadataHasCommand) {
    help_text += "Commands:\n";
    for (const auto& command_idx : lookup_command_) {
      help_text += std::format("    {}{}{}:\n{}\n", command_idx.first,
                               flags_.at(command_idx.second).short_name != ' '
                                   ? std::format(" | -{}", flags_.at(command_idx.second).short_name)
                                   : "",
                               flags_.at(command_idx.second).long_name[0] != '\0'
                                   ? std::format(" | --{}", flags_.at(command_idx.second).long_name)
                                   : "",
                               xIndentParagraph(flags_.at(command_idx.second).desc, 8));
    }
  }

  // Display positionals if exist.
  if constexpr (kMetadataHasPositional) {
    help_text += "Positional Arguments:\n";
    for (const auto& pos_idx : lookup_pos_) {
      help_text += std::format("    {}:\n{}\n", flags_.at(pos_idx).long_name,
                               xIndentParagraph(flags_.at(pos_idx).desc, 8));
    }
  }

  // Display rest of the flags.
  bool was_flags_title_appended = false;
  for (const auto& flag : flags_) {
    if (flag.interp == eFlagInterp::kCommand || flag.interp == eFlagInterp::kPositional) continue;
    if (!was_flags_title_appended) {
      help_text += "Flags:\n";
      was_flags_title_appended = true;
    }
    std::string flag_repr;
    if (flag.short_name != ' ') flag_repr += std::format("-{}, ", flag.short_name);
    if (flag.long_name[0] != '\0') {
      if (flag.short_name != ' ')
        flag_repr += std::format(" | --{}", flag.long_name);
      else
        flag_repr += std::format("--{}", flag.long_name);
    }
    help_text += std::format("    {}:\n{}\n", flag_repr, xIndentParagraph(flag.desc, 8));
  }
  return help_text;
}

template <auto& FLAG_ARRAY>
CliParserResult CliParser<FLAG_ARRAY>::parse(CliArgs::const_iterator beg,
                                             CliArgs::const_iterator end, CliFlagMap& out) {
  ParserState state{.is_done = false,
                    .args =
                        [&beg, &end] {
                          std::size_t n = static_cast<std::size_t>(std::distance(beg, end));
                          if (n == 0) return std::span<const std::string_view>{};
                          return std::span<const std::string_view>(&*beg, n);
                        }(),
                    .current_index = 0,
                    .current_positional = 0,
                    .output = &out};
  CliParserResult result{std::nullopt, std::nullopt, diagnostic::Diagnostics{}, {}, 0};

  if (state.current_index == state.args.size()) {
    if constexpr (kMetadataHasCommand) {
      result.errors->push_back(parsingFail("Missing required command argument."));
      return result;
    }

    if constexpr (kMetadataHasPositional) {
      result.errors->push_back(parsingFail("Missing required postional argument."));
      return result;
    }
  }

  while (state.current_index < state.args.size()) {
    std::string_view current_arg = state.currentArg();
    if (current_arg.starts_with("--")) {
      parseLongFlag(state, result, out);
      if (state.is_done) {
        return result;
      }
      continue;
    }

    if (current_arg.starts_with("-")) {
      parseShortFlag(state, result, out);
      if (state.is_done) {
        return result;
      }
      continue;
    }

    if constexpr (kMetadataHasPositional) {
      if (state.current_positional < kMetadataPositionalCount) {
        parsePositionalArg(state, result, out);
        if (state.is_done) {
          return result;
        }
        continue;
      }
    }

    if constexpr (kMetadataHasCommand) {
      parseCommandArg(state, result, out);
      if (state.is_done) {
        return result;
      }
      continue;
    }

    // Nothing matched. Unexpected argument.
    result.errors->push_back(
        parsingFail(std::format("Unexpected command line argument '{}' at index {}.",
                                state.currentArg(), state.current_index)));
    state.current_index++;
  }

  checkRequiredArgs(out, result);

  // Make sure command was provided if it exists.
  if constexpr (kMetadataHasCommand) {
    if (!result.command_flag.has_value()) {
      result.errors->push_back(parsingFail("Missing required command argument."));
    }
  }

  return result;
}

template <auto& FLAG_ARRAY>
diagnostic::Diagnostic CliParser<FLAG_ARRAY>::parsingFail(const std::string& message) {
  return diagnostic::Diagnostic{diagnostic::eError::kCliParserFailure, {message}};
}

// Check required args now accepts result as output target.
template <auto& FLAG_ARRAY>
void CliParser<FLAG_ARRAY>::checkRequiredArgs(const CliFlagMap& out, CliParserResult& result) {
  for (const auto& flag : flags_) {
    if (CliFlagProperties(flag.properties).IsRequired()) {
      bool found = false;

      switch (flag.interp) {
        case eFlagInterp::kSingle:
        case eFlagInterp::kOpt:
        case eFlagInterp::kMulti:
          // Check if this flag was parsed into the output map
          for (const auto& parsed_flag : out) {
            if (parsed_flag.first == flag.id) {
              found = true;
              break;
            }
          }
          break;

        case eFlagInterp::kPositional:
          // Check if this positional was provided
          for (std::size_t i = 0; i < result.positionals.size(); ++i) {
            // compare metadata id of the positional slot to this flag.id
            if (i < lookup_pos_.size() && flags_.at(lookup_pos_[i]).id == flag.id) {
              found = true;
              break;
            }
          }
          break;

        case eFlagInterp::kCommand:
          // Check if this command was selected
          if (result.command_flag.has_value() && result.command_flag.value() == flag.id) {
            found = true;
          }
          break;

        case eFlagInterp::kDisplay:
          // Check if this display flag was triggered
          if (result.display_flag.has_value() && result.display_flag.value() == flag.id) {
            found = true;
          }
          break;

        case eFlagInterp::kNONE:
          break;
      }

      if (!found) {
        // Generate appropriate error message
        if (flag.short_name != ' ') {
          result.errors->push_back(parsingFail(std::string("Missing required flag: -") +
                                               flag.short_name + " (--" + flag.long_name + ")"));
        } else if (flag.interp == eFlagInterp::kPositional) {
          result.errors->push_back(
              parsingFail(std::string("Missing required positional argument: ") + flag.long_name));
        } else if (flag.interp == eFlagInterp::kCommand) {
          result.errors->push_back(
              parsingFail(std::string("Missing required command: ") + flag.long_name));
        } else if (flag.interp == eFlagInterp::kDisplay) {
          result.errors->push_back(
              parsingFail(std::string("Missing required display flag: --") + flag.long_name));
        } else {
          result.errors->push_back(
              parsingFail(std::string("Missing required flag: --") + flag.long_name));
        }
      }
    }
  }
}

template <auto& FLAG_ARRAY>
const CliFlag* CliParser<FLAG_ARRAY>::lookupFlagMetadata(
    const std::unordered_map<std::string_view, std::size_t>& table, std::string_view key) {
  auto found = table.find(key);
  if (found == table.cend()) {
    return nullptr;
  }
  return &flags_.at(found->second);
}

template <auto& FLAG_ARRAY>
const CliFlag* CliParser<FLAG_ARRAY>::lookupFlagMetadata(
    const std::unordered_map<char, std::size_t>& table, char key) {
  std::unordered_map<char, std::size_t>::const_iterator found = table.find(key);
  if (found == table.cend()) {
    return nullptr;
  }
  return &flags_.at(found->second);
}

template <auto& FLAG_ARRAY>
void CliParser<FLAG_ARRAY>::parseLongFlag(ParserState& state, CliParserResult& result,
                                          CliFlagMap& out) {
  std::string_view arg = state.currentArg();
  if (arg.size() < 3) {
    result.errors->push_back(parsingFail("Invalid flag: '--'."));
    state.current_index++;
    return;
  }

  // Extract a long flag identifier.
  std::string_view::const_iterator identifier_begin = arg.cbegin() + 2;
  std::string_view::const_iterator identifier_end = identifier_begin;
  while (identifier_end < arg.cend()) {
    // std::isalnum The behavior is undefined if the value of ch is not
    // representable as unsigned char and is not equal to EOF
    // Ref: https://en.cppreference.com/cpp/string/byte/isalnum
    if (std::isalnum(static_cast<unsigned char>(*identifier_end)) || *identifier_end == '-' ||
        *identifier_end == '_') {
      identifier_end++;
    } else {
      break;
    }
  }
  std::string_view identifier = {identifier_begin, identifier_end};

  // Find the associated flag index.
  auto flag_metadata = lookupFlagMetadata(lookup_long_, identifier);

  if (flag_metadata == nullptr) {
    result.errors->push_back(parsingFail(std::format("Unknown flag: --{}.", identifier)));
    state.current_index++;
    return;
  }

  processFlag(*flag_metadata, identifier_end - arg.cbegin(), state, result, out);
}

template <auto& FLAG_ARRAY>
void CliParser<FLAG_ARRAY>::parseShortFlag(ParserState& state, CliParserResult& result,
                                           CliFlagMap& out) {
  std::string_view arg = state.currentArg();
  if (arg.size() < 2) {
    result.errors->push_back(parsingFail("Invalid flag: '-'."));
    state.current_index++;
    return;
  }
  char identifier = arg[1];
  auto flag_metadata = lookupFlagMetadata(lookup_short_, identifier);

  if (flag_metadata == nullptr) {
    result.errors->push_back(parsingFail(std::format("Unknown flag: --{}.", identifier)));
    state.current_index++;
    return;
  }
  processFlag(*flag_metadata, 2, state, result, out);
}

template <auto& FLAG_ARRAY>
void CliParser<FLAG_ARRAY>::parsePositionalArg(ParserState& state, CliParserResult& result,
                                               CliFlagMap& out) {
  if (state.current_positional >= lookup_pos_.size()) {
    // This shouldnt happen because positional count should be checked before
    // calling this method.
    result.errors->push_back(
        diagnostic::makeErrorDeveloperBug(std::source_location::current(),
                                          "Unexpected program location reached. "
                                          "Parsing out of bounds positional argument."));
    state.current_index++;
    return;
  }

  std::size_t positional_flag_index = lookup_pos_[state.current_positional];
  CliFlagProperties postional_props = flags_[positional_flag_index].properties;

  if (postional_props.IsRepeatable()) {
    while (state.current_index < state.args.size()) {
      state.pushToOutput(flags_.at(positional_flag_index).id, state.currentArg());
      result.positionals.push_back(std::string{state.currentArg()});
      state.current_index++;
      if (state.currentArg().starts_with("-")) break;
    }
  } else {
    state.pushToOutput(flags_.at(positional_flag_index).id, state.currentArg());
    result.positionals.push_back(std::string{state.currentArg()});
    state.current_index++;
    state.current_positional++;
  }
}

template <auto& FLAG_ARRAY>
void CliParser<FLAG_ARRAY>::processFlag(const CliFlag& flag_metadata, std::size_t identifier_offset,
                                        ParserState& state, CliParserResult& result,
                                        CliFlagMap& out) {
  eFlagInterp flag_interp = flag_metadata.interp;
  eCliFlag flag_id = flag_metadata.id;
  std::string_view flag_prefix = {state.currentArg().cbegin() + identifier_offset,
                                  state.currentArg().cend()};
  std::size_t next_arg_index = state.current_index + 1;

  switch (flag_interp) {
    case eFlagInterp::kOpt: {
      state.pushToOutput(flag_id, "");
      state.current_index++;
    } break;
    case eFlagInterp::kCommand:
      // This is in case a command is passed prefixed with a '--'.
      if constexpr (kMetadataHasCommand) {
        result.command_flag = flag_id;
        result.rest_args = next_arg_index;
        state.is_done = true;
        return;
      } else {
        // This should never happen.
        result.errors->push_back(diagnostic::makeErrorDeveloperBug(
            std::source_location::current(),
            "Unexpected program location reached."
            "Processing command flag but no command exists in flags metadata."));
        state.current_index++;
        return;
      }
      break;
    case eFlagInterp::kSingle:
      if (next_arg_index >= state.args.size()) {
        result.errors->push_back(parsingFail(std::format(
            "Expected variable after flag: '{}', but got end of arguments.", flag_prefix)));
        state.current_index++;
        return;
      }

      if (state.args[next_arg_index].starts_with("-")) {
        result.errors->push_back(
            parsingFail(std::format("Expected variable after single parameter flag, but got flag: "
                                    "'{}'.",
                                    state.args[next_arg_index])));
        // skip the flag token to avoid infinite loop
        state.current_index++;
        return;
      }
      state.pushToOutput(flag_id, state.args[next_arg_index]);
      state.current_index = state.current_index + 2;
      return;
    case eFlagInterp::kDisplay:
      result.display_flag = flag_id;
      state.is_done = true;
      state.current_index++;
      return;
    default:
      result.errors->push_back(
          diagnostic::makeErrorDeveloperBug(std::source_location::current(),
                                            "Unexpected program location reached. "
                                            "Unknown or unimplemented eFlagInterp value."));
      state.is_done = true;
      return;
  }
}

template <auto& FLAG_ARRAY>
void CliParser<FLAG_ARRAY>::parseCommandArg(ParserState& state, CliParserResult& result,
                                            CliFlagMap& out) {
  std::string_view arg = state.currentArg();
  auto flag_metadata = lookupFlagMetadata(lookup_command_, arg);

  if (flag_metadata == nullptr) {
    result.errors->push_back(parsingFail(std::format("Unknown command: '{}'.", arg)));
    state.current_index++;
    return;
  }

  result.command_flag = flag_metadata->id;
  result.rest_args = state.current_index + 1;
  state.is_done = true;
  return;
}

/// Used to initialize an array of flag metadata to pass to the `CliParser`
/// class template parameter.
template <class FlagT, typename... FlagTs>
constexpr auto initCliFlagMetadata(const FlagT& flag, FlagTs... flags) {
  return std::array<FlagT, sizeof...(FlagTs) + 1>{flag, flags...};
}

static constexpr inline auto kMainFlagsMetadata =
    initCliFlagMetadata<CliFlag>(CliFlag{.id = eCliFlag::kHelp,
                                         .interp = eFlagInterp::kDisplay,
                                         .short_name = 'h',
                                         .long_name = "help",
                                         .desc = "Display this help message and exit.",
                                         .properties = {}},
                                 CliFlag{.id = eCliFlag::kVersion,
                                         .interp = eFlagInterp::kDisplay,
                                         .short_name = 'v',
                                         .long_name = "version",
                                         .desc = "Display application version and exit.",
                                         .properties = {}},
                                 CliFlag{.id = eCliFlag::kSilent,
                                         .interp = eFlagInterp::kOpt,
                                         .short_name = ' ',
                                         .long_name = "silent",
                                         .desc = "Run in silent mode.",
                                         .properties = {}},
                                 CliFlag{.id = eCliFlag::kNoColor,
                                         .interp = eFlagInterp::kOpt,
                                         .short_name = ' ',
                                         .long_name = "no-color",
                                         .desc = "Disable color output.",
                                         .properties = {}},
                                 CliFlag{.id = eCliFlag::kCommandBuild,
                                         .interp = eFlagInterp::kCommand,
                                         .short_name = ' ',
                                         .long_name = "comp",
                                         .desc = "Compile C& source code."},
                                 CliFlag{.id = eCliFlag::kCommandDev,
                                         .interp = eFlagInterp::kCommand,
                                         .short_name = ' ',
                                         .long_name = "dev",
                                         .desc = "Developer sub-commands."});

static constexpr inline auto kCommandBuildFlagsMetadata =
    initCliFlagMetadata<CliFlag>(CliFlag{.id = eCliFlag::kSources,
                                         .interp = eFlagInterp::kPositional,
                                         .short_name = ' ',
                                         .long_name = "",
                                         .desc = "C& Source files to build.",
                                         .properties = CliFlagProperties{}.Repeatable()});

static constexpr inline auto kDevModeFlagsMetadata =
    initCliFlagMetadata<CliFlag>(CliFlag{.id = eCliFlag::kDevModeLex,
                                         .interp = eFlagInterp::kCommand,
                                         .short_name = ' ',
                                         .long_name = "lex",
                                         .desc = "Lex a source file and output JSON result."});

static constexpr inline auto kDevModeLexFlagsMetadata =
    initCliFlagMetadata<CliFlag>(CliFlag{.id = eCliFlag::kSources,
                                         .interp = eFlagInterp::kPositional,
                                         .short_name = ' ',
                                         .long_name = "",
                                         .desc = "C& Source files to lex.",
                                         .properties = CliFlagProperties{}.Repeatable()});

CliParserResult parseCliMain(CliArgs::const_iterator beg, CliArgs::const_iterator end,
                             CliFlagMap& out) noexcept {
  return CliParser<kMainFlagsMetadata>::parse(beg, end, out);
}

CliParserResult parseCliCommandBuild(CliArgs::const_iterator beg, CliArgs::const_iterator end,
                                     CliFlagMap& out) noexcept {
  return CliParser<kCommandBuildFlagsMetadata>::parse(beg, end, out);
}

CliParserResult parseCliCommandDev(CliArgs::const_iterator beg, CliArgs::const_iterator end,
                                   CliFlagMap& out) noexcept {
  return CliParser<kDevModeFlagsMetadata>::parse(beg, end, out);
}

CliParserResult parseCliCommandDevLex(CliArgs::const_iterator beg, CliArgs::const_iterator end,
                                      CliFlagMap& out) noexcept {
  return CliParser<kDevModeLexFlagsMetadata>::parse(beg, end, out);
}

std::string helptextCliMain() noexcept { return CliParser<kMainFlagsMetadata>::helptext(); }

std::string helptextCliCommandBuild() noexcept {
  return CliParser<kCommandBuildFlagsMetadata>::helptext();
}

std::string helptextCliCommandDev() noexcept {
  return CliParser<kDevModeFlagsMetadata>::helptext();
}

std::string helptextCliCommandDevLex() noexcept {
  return CliParser<kDevModeLexFlagsMetadata>::helptext();
}

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