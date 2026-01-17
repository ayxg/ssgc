// Created: 2025/10/31

#include <algorithm>
#include <array>
#include <expected>
#include <format>
#include <iostream>
#include <map>
#include <string>
#include <string_view>
#include <unordered_map>
#include <variant>
#include <vector>

namespace cnd::driver {
using std::advance;
using std::array;
using std::count_if;
using std::expected;
using std::find_if;
using std::format;
using std::multimap;
using std::size_t;
using std::string;
using std::string_view;
using std::unexpected;
using std::unordered_map;
using std::variant;
using std::vector;

// Input args format.
// Construct from raw `main()` input args like so : `Argv args = { argv, argv +
// argc }`.
using Argv = vector<string_view>;
using ArgvConstIter = Argv::const_iterator;

// Used to retrieve parsed arg values from the output map.
// Negative indices are reserved for gopt.
// All positive integers including 0 are reserved for user flags.
// This allows up to (INT_MAX/2) flags per parser.
using FlagEnum = int;
static constexpr inline FlagEnum kInvalidFlagEnum = -1;

// Parsed args are stored as a variant of allowed types in a std::multimap.
using FlagVar = variant<string_view>;

template <class FlagIdT>
using FlagMap = multimap<FlagIdT, FlagVar>;

// Determines how a flag will be interpreted by the parser.
enum class eFlagInterp {
  kNONE,
  kOpt,     // <opt>    ::= "-" <short-ident> | "--" <long-ident>
  kSingle,  // <single> ::= "-" <short-ident> " " <positional> | "--"
            // <long-ident> " " <positional>
  kMulti,   // <multi>  ::= "-" <short-ident> " " <positional>* | "--"
            // <long-ident> " " <positional>*
  kPositional,
  // Command, short circuits the parser and provides the iterator to start from
  // for the following parser.
  kCmd,
  // Short circuits the parser and calls a validator function. Can be used for
  // `--help` or `--version` flags.
  kDisplay,
  kVarDef
};

// Additional flag properties.
class FlagProperties {
 public:
  enum eProp : std::uint32_t {
    // Applies to eFlagInterp : kSingle, kOpt, kMulti, kCmd, kPositional.
    // By default, all flags are optional.
    kRequired = (1 << 0),
    // Applies to eFlagInterp : kSingle, kOpt, kMulti.
    // kCmd type flags cannot be repeatable, parsing a command short-circuits the parser.
    // kPositional flags marked as repeatable will cause the parser consume the rest of the positional arguments into
    // that flag. Meaning positional flags declared after the repeatable positional will not be parsed.
    // By default, repeated flags will trigger a parsing error.
    kRepeatable = (1 << 1),
    // Applies to eFlagInterp : kSingle, kMulti.
    // By default no value will trigger a parsing error.
    kValueOptional = (1 << 2)
  };

  constexpr FlagProperties(uint32_t bits = 0) : bits_(bits) {}

  constexpr bool IsRequired() const { return bits_ & eProp::kRequired; }
  constexpr bool IsRepeatable() const { return bits_ & eProp::kRepeatable; }
  constexpr bool IsValueOptional() const { return bits_ & eProp::kValueOptional; }

  constexpr FlagProperties Required() const { return bits_ | eProp::kRequired; }
  constexpr FlagProperties Repeatable() const { return bits_ | eProp::kRepeatable; }
  constexpr FlagProperties ValueOptional() const { return bits_ | eProp::kValueOptional; }
  constexpr operator std::uint32_t() const { return bits_; }

 private:
  std::uint32_t bits_;
};

template <class FlagIdT>
struct Flag {
  using FlagIdType = FlagIdT;
  using FlagMapType = FlagMap<FlagIdType>;
  using FlagValidatorType = bool (*)(const FlagMapType&, FlagIdType);
  FlagIdType id{-1};
  eFlagInterp interp{eFlagInterp::kNone};
  char short_name{' '};
  const char* long_name{""};
  const char* desc{""};
  std::uint32_t properties{};
  bool (*validator)(const FlagMapType&, FlagIdType){nullptr};
};

template <auto& FLAG_ARRAY>
class Parser {
 public:
  using FlagMetadataArray = std::decay_t<decltype(FLAG_ARRAY)>;
  using FlagIdType = FlagMetadataArray::value_type::FlagIdType;
  using FlagMapType = FlagMetadataArray::value_type::FlagMapType;

 private:
  // Metadata validators.
  static constexpr bool AssertFlagEnumsAreUnique(const FlagMetadataArray& flags) {
    for (size_t i = 0; i < flags_.size(); ++i)
      for (size_t j = i + 1; j < flags_.size(); ++j)
        if (flags_[i].id == flags_[j].id) return false;
    return true;
  }
  static constexpr bool AssertNoEmptyFlags(const FlagMetadataArray& flags) {
    for (const auto& flag : flags) {
      if (flag.interp == eFlagInterp::kPositional) continue;
      if (flag.short_name == ' ' && (flag.long_name == nullptr || flag.long_name[0] == '\0')) return false;
    }
    return true;
  }

  // Flags metadata
  static constexpr const FlagMetadataArray& flags_ = {FLAG_ARRAY};
  static_assert(AssertFlagEnumsAreUnique(flags_), "Duplicate flag enums found in FlagMetadataArray.");
  static_assert(AssertNoEmptyFlags(flags_), "Found flags with no short_name and no long_name");

  static constexpr bool HasCommand() {
    return find_if(flags_.cbegin(), flags_.cend(), [](auto& f) { return f.interp == eFlagInterp::kCmd; }) !=
           flags_.cend();
  }

  static constexpr bool HasPositional() {
    return find_if(flags_.cbegin(), flags_.cend(), [](auto& f) { return f.interp == eFlagInterp::kPositional; }) !=
           flags_.cend();
  }

  static constexpr size_t PositionalCount() {
    return count_if(flags_.cbegin(), flags_.cend(), [](auto& f) { return f.interp == eFlagInterp::kPositional; });
  }

  // Lookup table, short ident -> parser flag index
  static const inline unordered_map<char, size_t> lookup_short_{[] {
    unordered_map<char, size_t> ret{};
    for (size_t i = 0; i < flags_.size(); ++i) ret[flags_[i].short_name] = i;
    return ret;
  }()};

  // Lookup table, long ident -> parser flag index
  static const inline unordered_map<string_view, size_t> lookup_long_{[] {
    unordered_map<std::string_view, size_t> ret;
    for (size_t i = 0; i < flags_.size(); ++i) ret[flags_[i].long_name] = i;
    return ret;
  }()};

  // Lookup table, command -> parser flag index
  static const inline unordered_map<string_view, size_t> lookup_cmd_{[] {
    unordered_map<std::string_view, size_t> ret;
    for (size_t i = 0; i < flags_.size(); ++i) {
      if (flags_[i].interp == eFlagInterp::kCmd) ret[flags_[i].long_name] = i;
    }
    return ret;
  }()};

  // Lookup table,positional index -> parser flag index
  static constexpr inline std::array<size_t, PositionalCount()> lookup_pos_{[]() {
    std::array<size_t, PositionalCount()> ret{};
    size_t pos_index = 0;
    for (size_t i = 0; i < flags_.size(); ++i)
      if (flags_[i].interp == eFlagInterp::kPositional) ret[pos_index++] = i;
    return ret;
  }()};

 private:
  std::string CheckRequiredArgs(const FlagMapType& out) {
    for (const auto& flag : flags_) {
      if (FlagProperties(flag.properties).IsRequired()) {
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
            for (size_t i = 0; i < positional_.size(); ++i) {
              if (i < lookup_pos_.size() && lookup_pos_[i] == static_cast<size_t>(flag.id)) {
                found = true;
                break;
              }
            }
            break;

          case eFlagInterp::kCmd:
            // Check if this command was selected
            if (command_ == flag.id) {
              found = true;
            }
            break;

          case eFlagInterp::kDisplay:
            // Check if this display flag was triggered
            if (is_display_ && display_flag_ == flag.id) {
              found = true;
            }
            break;

          case eFlagInterp::kNONE:
            break;
        }

        if (!found) {
          // Generate appropriate error message
          if (flag.short_name != ' ') {
            return std::string("Missing required flag: -") + flag.short_name + " (--" + flag.long_name + ")";
          } else if (flag.interp == eFlagInterp::kPositional) {
            return std::string("Missing required positional argument: ") + flag.long_name;
          } else if (flag.interp == eFlagInterp::kCmd) {
            return std::string("Missing required command: ") + flag.long_name;
          } else if (flag.interp == eFlagInterp::kDisplay) {
            return std::string("Missing required display flag: --") + flag.long_name;
          } else {
            return std::string("Missing required flag: --") + flag.long_name;
          }
        }
      }
    }
    return "";  // All required args are present
  }

 public:
  std::expected<ArgvConstIter, string> Parse(ArgvConstIter beg, ArgvConstIter end, FlagMapType& out) {
    size_t current_positional = 0;  // Track parsed positional count.
    // Offset of the flag identifier to start parsing from for
    // eFlagInterp::kVariable flags.
    string_view::const_iterator ident_offset{};
    size_t flag_idx{0};  // Flag index in the `flags_` array.

    ArgvConstIter arg_it = beg;
    for (; arg_it < end; arg_it++) {
      // Determine flag interp and offset to start parsing from.
      //
      // -> Long flag...
      if (arg_it->starts_with("--")) {
        auto id_beg{arg_it->cbegin() + 2};  // skip `--`
        auto id_end = id_beg;
        // Tokenize flag ident.
        while (id_end < arg_it->cend()) {
          if (std::isalnum(*id_end) || *id_end == '-' || *id_end == '_')
            id_end++;
          else
            break;
        }
        auto flag_it = lookup_long_.find(string_view{id_beg, id_end});
        if (flag_it == lookup_long_.cend())
          return unexpected{format("Unknown flag: '{}'.", string_view{id_beg, id_end})};
        ident_offset = id_end;
        flag_idx = flag_it->second;
      }
      // -> Short flag...
      else if (arg_it->starts_with("-")) {
        if (arg_it->size() < 2) return unexpected{"Invalid argument '-'."};
        auto flag_it = lookup_short_.find(arg_it->at(1));
        if (flag_it == lookup_short_.cend()) return unexpected{format("Unknown short flag '{}'.", arg_it->at(1))};
        ident_offset = arg_it->cbegin() + 2;  // pass `-` and flag char
        flag_idx = flag_it->second;
      }
      // -> Positional or command...
      else {
        // Receive positional arguments first, if requested.
        if (HasPositional() && current_positional < PositionalCount()) {
          if (!ValidateFlag(lookup_pos_[current_positional], out))
            return unexpected{format("Failed to validate postional argument {}.", current_positional)};
          positional_.push_back(*arg_it);
          current_positional++;
          continue;
        }

        // Expect a command, short circuit argument parsing if found.
        if (HasCommand()) {
          auto cmd_it = lookup_cmd_.find(*arg_it);
          if (cmd_it == lookup_cmd_.cend()) return unexpected{format("Unknown command: '{}'.", *arg_it)};
          command_ = flags_[cmd_it->second].id;
          return arg_it + 1;  // Rest args
        } else                // Unparsed args are denied by default.
          return unexpected{format("Unexpected argument: '{}'.", *arg_it)};
      }

      // Parse flag based on interp, validate and store in output.
      switch (flags_.at(flag_idx).interp) {
        case eFlagInterp::kOpt: {
          out.insert({flags_.at(flag_idx).id, ""});
        } break;
        case eFlagInterp::kSingle: {
          auto flag_var = arg_it + 1;
          if (flag_var->starts_with("-"))
            return unexpected{
                format("Expected variable after single parameter flag, but got "
                       "flag: '{}'.",
                       *flag_var)};
          out.insert({flags_.at(flag_idx).id, *flag_var});
          advance(arg_it, 1);  // Pass value arg.
        } break;
        case eFlagInterp::kDisplay: {
          // Validation result is ignored on eFlagInterp::kDisplay flags.
          ValidateFlag(flag_idx, out);
          is_display_ = true;
          display_flag_ = flags_.at(flag_idx).id;
          return arg_it;  // Short circuit parser.
        } break;
        default:
          std::unreachable();
      }
    }
    auto required_flags_error = CheckRequiredArgs(out);
    if (required_flags_error != "") return unexpected{required_flags_error};
    return arg_it;
  }

  constexpr FlagIdType GetCommand() { return command_; }
  constexpr bool IsDisplayRun() { return is_display_; }
  constexpr FlagIdType GetDisplayFlag() { return display_flag_; }
  constexpr const vector<string_view>& GetPositionalArgs() { return positional_; }

 private:
  // `fwd_map` is the current forwarded output flags map in `Parser.Parse`.
  bool ValidateFlag(size_t flag_idx, const FlagMapType& fwd_map) {
    if (flags_.at(flag_idx).validator != nullptr)
      return flags_.at(flag_idx).validator(fwd_map, flags_.at(flag_idx).id);
    else  // If there is no validator, flag is always valid.
      return true;
  }

 private:
  vector<string_view> positional_{};
  FlagIdType command_{kInvalidFlagEnum};
  bool is_display_{false};
  FlagIdType display_flag_{};
};

template <class FlagT, typename... Flags>
constexpr auto GenParserFlags(const FlagT& flag, Flags... flags) {
  return std::array<FlagT, sizeof...(Flags) + 1>{flag, flags...};
}

}  // namespace cnd::driver
