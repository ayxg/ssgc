#include "ansi.hpp"
#include "diagnostic_impl.hpp"
#include "../compiler/translation_context.hpp"

namespace ssgc {

// Internal vtable dispatch method for compiler messages.
inline std::string formatDiagnostic(DiagnosticId id, const DiagnosticDataBufferT& data,
                                    const TrContext* ctx) noexcept;

inline std::string formatDiagnostic(Diagnostic diag, const TrContext* ctx) noexcept {
  return formatDiagnostic(diag.id, diag.data, ctx);
}

inline std::string formatDiagnostic(Diagnostics diags, const TrContext* ctx) noexcept {
  std::string ret{};
  for (const auto& diag : *diags) {
    ret += formatDiagnostic(diag, ctx);
    ret += "\n";
  }
  return ret;
}

//////////////////////////////////////////////////////////////////////
/* Forward declare all required formatDiagnostic method specializations. */
//////////////////////////////////////////////////////////////////////
//
// This is required or else the vtable method in formatDiagnostic cannot lookup the method defs.
// Since every format method signature is the same, we can pre-declare them using macros.
//
// @note : When adding new diagnostic enum entries, and the format method has not been defined. A
// compile time error will be triggered as all enum entries must have a formatter method defined.
// @important The typename T paramater must be present or else the enum value template will
// be decayed to an integral so equal values enums will trigger ODR/COMDAT error which is impossible
// to figure out! You will only get this error if you attempt using this header in multiple
// translation units. Generated eWarning formatDiagnostic template specialization decls.

template <typename T, eError DIAGNOSTIC>
inline std::string formatDiagnostic(const DiagnosticDataBufferT& data,
                                    const TrContext* ctx) noexcept;

template <typename T, eWarning DIAGNOSTIC>
inline std::string formatDiagnostic(const DiagnosticDataBufferT& data,
                                    const TrContext* ctx) noexcept;

template <typename T, eGuideline DIAGNOSTIC>
inline std::string formatDiagnostic(const DiagnosticDataBufferT& data,
                                    const TrContext* ctx) noexcept;

template <typename T, eInfo DIAGNOSTIC>
inline std::string formatDiagnostic(const DiagnosticDataBufferT& data,
                                    const TrContext* ctx) noexcept;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Define the manual formatting dispatch method formatDiagnostic(DiagnosticId id, const
 * DiagnosticDataBufferT& data) */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Declare all specializations here first. So they don't get triggered outside.
#define SSGC_LOCAL_MACRO_DeclareFormatDiagnosticSpecialization_eError(en)                    \
  template <>                                                                                \
  inline std::string formatDiagnostic<eError, eError::en>(const DiagnosticDataBufferT& data, \
                                                          const TrContext* ctx) noexcept;

#define SSGC_LOCAL_MACRO_DeclareFormatDiagnosticSpecialization_eGuideline(en) \
  template <>                                                                 \
  inline std::string formatDiagnostic<eGuideline, eGuideline::en>(            \
      const DiagnosticDataBufferT& data, const TrContext* ctx) noexcept;

#define SSGC_LOCAL_MACRO_DeclareFormatDiagnosticSpecialization_eInfo(en)                   \
  template <>                                                                              \
  inline std::string formatDiagnostic<eInfo, eInfo::en>(const DiagnosticDataBufferT& data, \
                                                        const TrContext* ctx) noexcept;

#define SSGC_LOCAL_MACRO_DeclareFormatDiagnosticSpecialization_eWarning(en)                      \
  template <>                                                                                    \
  inline std::string formatDiagnostic<eWarning, eWarning::en>(const DiagnosticDataBufferT& data, \
                                                              const TrContext* ctx) noexcept;

SSGC_AppliedEnum_eError(SSGC_LOCAL_MACRO_DeclareFormatDiagnosticSpecialization_eError, , , );
SSGC_AppliedEnum_eGuideline(SSGC_LOCAL_MACRO_DeclareFormatDiagnosticSpecialization_eGuideline, ,
                            , );
SSGC_AppliedEnum_eInfo(SSGC_LOCAL_MACRO_DeclareFormatDiagnosticSpecialization_eInfo, , , );
SSGC_AppliedEnum_eWarning(SSGC_LOCAL_MACRO_DeclareFormatDiagnosticSpecialization_eWarning, , , );

// The 'manual vtable' for FormatClErr method. Dispatches to the specialized template
// implementations. !warning DO NOT call directly. Called only by 'Diagnostic.Format()'.
inline std::string formatDiagnostic(DiagnosticId id, const DiagnosticDataBufferT& data,
                                    const TrContext* ctx) noexcept {
#define SSGC_LOCAL_MACRO_DispatchDiagnosticFormatError(ec) \
  case ec:                                                 \
    return formatDiagnostic<eError, ec>(data, ctx);
#define SSGC_LOCAL_MACRO_DispatchDiagnosticFormatWarning(ec) \
  case ec:                                                   \
    return formatDiagnostic<eWarning, ec>(data, ctx);
#define SSGC_LOCAL_MACRO_DispatchDiagnosticFormatGuideline(ec) \
  case ec:                                                     \
    return formatDiagnostic<eGuideline, ec>(data, ctx);
#define SSGC_LOCAL_MACRO_DispatchDiagnosticFormatInfo(ec) \
  case ec:                                                \
    return formatDiagnostic<eInfo, ec>(data, ctx);

  // Make sure we are using the correct global specializations.
  using ::ssgc::formatDiagnostic;

  switch (static_cast<eDiagnosticType>(id.msg_type)) {
    case eDiagnosticType::kError: {
      using enum eError;
      switch (static_cast<eError>(id.code)) {
        SSGC_AppliedEnum_eError(SSGC_LOCAL_MACRO_DispatchDiagnosticFormatError, , , );
        default:
          return "<invalid>";
      }
    }
    case eDiagnosticType::kWarning: {
      using enum eWarning;
      switch (static_cast<eWarning>(id.code)) {
        SSGC_AppliedEnum_eWarning(SSGC_LOCAL_MACRO_DispatchDiagnosticFormatWarning, , , );
        default:
          return "<invalid>";
      }
    }

    case eDiagnosticType::kGuideline: {
      using enum eGuideline;
      switch (static_cast<eGuideline>(id.code)) {
        SSGC_AppliedEnum_eGuideline(SSGC_LOCAL_MACRO_DispatchDiagnosticFormatGuideline, , , );
        default:
          return "<invalid>";
      }
    }
    case eDiagnosticType::kInfo: {
      using enum eInfo;
      switch (static_cast<eInfo>(id.code)) {
        SSGC_AppliedEnum_eInfo(SSGC_LOCAL_MACRO_DispatchDiagnosticFormatInfo, , , );
        default:
          return "<invalid>";
      }
    }

    default:
      return "[Uncategorized Compiler Message]";
  }

// undef function-local
#undef SSGC_LOCAL_MACRO_DispatchDiagnosticFormat
}

constexpr DiagnosticDataBufferT convertCppSourceLocationToDiagnosticData(
    const std::source_location& loc) noexcept {
  DiagnosticDataBufferT ret;
  ret.reserve(4);
  // Fill the vector in declaration order
  ret.push_back(std::string{loc.file_name()});      // file name (std::string_view)
  ret.push_back(std::uint64_t{loc.line()});         // line number (size_t)
  ret.push_back(std::uint64_t{loc.column()});       // column number (size_t)
  ret.push_back(std::string{loc.function_name()});  // function name (std::string_view)
  return ret;
}

inline std::string formatCppSourceLocationDiagnosticData(
    DiagnosticDataBufferT::const_iterator loc_begin,
    DiagnosticDataBufferT::const_iterator loc_end) {
  std::span<const DiagnosticDataUnionT> loc = {loc_begin, loc_end};
  return std::format("{{file : {}\nline : {}\ncolumn : {}\nfunction : {}}}",
                     std::get<std::string>(loc[0]), std::get<std::uint64_t>(loc[1]),
                     std::get<std::uint64_t>(loc[2]), std::get<std::string>(loc[3]));
}

std::string formatDiagnosticPrefix(eError e) {
  bool color_state = cli::ansi::enableColor();
  std::string ret{cli::ansi::red()};
  ret += "[";
  ret += eDiagnosticTypeToCStr(getDiagnosticType(e));
  ret += "][";
  auto category = getErrorCategory(e);
  if (category != eErrorCategory::kNone) {
    ret += eErrorCategoryToCStr(category);
    ret += "][";
  }
  ret += eErrorToCStr(e);
  ret += "]: ";
  ret += cli::ansi::reset();
  cli::ansi::enableColor(color_state);
  return ret;
}

constexpr std::string formatDiagnosticPrefix(eWarning e) {
  std::string ret{"["};
  ret += eDiagnosticTypeToCStr(getDiagnosticType(e));
  ret += "][";
  auto category = getWarningCategory(e);
  if (category != eWarningCategory::kNone) {
    ret += eWarningCategoryToCStr(category);
    ret += "][";
  }
  ret += eWarningToCStr(e);
  ret += "]: ";
  return ret;
}

constexpr std::string formatDiagnosticPrefix(eGuideline e) {
  std::string ret{"["};
  ret += eDiagnosticTypeToCStr(getDiagnosticType(e));
  ret += "][";
  auto category = getGuidelineCategory(e);
  if (category != eGuidelineCategory::kNone) {
    ret += eGuidelineCategoryToCStr(category);
    ret += "][";
  }
  ret += eGuidelineToCStr(e);
  ret += "]: ";
  return ret;
}

constexpr std::string formatDiagnosticPrefix(eInfo e) {
  std::string ret{"["};
  ret += eDiagnosticTypeToCStr(getDiagnosticType(e));
  ret += "][";
  auto category = getInfoCategory(e);
  if (category != eInfoCategory::kNone) {
    ret += eInfoCategoryToCStr(category);
    ret += "][";
  }
  ret += eInfoToCStr(e);
  ret += "]: ";
  return ret;
}

std::string formatDiagnosticSourceLocation(std::uint64_t file_id, std::uint64_t byte_offset,
                                           const TrContext* ctx, bool add_line_cursor = false) {
  const SourceFile* src_file = ctx->sources.get(file_id);
  auto relative_path = std::filesystem::relative(ctx->sources.pathof(src_file));
  auto [line, col] = src_file->linecol(byte_offset);

  if (!add_line_cursor) {
    return std::format("{}:{}:{}", relative_path.string(), line, col);
  }

  std::size_t line_cursor_begin = byte_offset;
  while (line_cursor_begin > 0 && src_file->data[line_cursor_begin - 1] != '\n') {
    --line_cursor_begin;
  }

  std::size_t line_cursor_end = byte_offset;
  while (line_cursor_end < src_file->data.size() && src_file->data[line_cursor_end] != '\n') {
    ++line_cursor_end;
  }

  std::string_view line_view = src_file->slice(line_cursor_begin, line_cursor_end);
  std::string pointer_line(line_view.size() + 1, ' ');
  std::size_t caret = byte_offset - line_cursor_begin;
  pointer_line[caret] = '^';

  return std::format("{}:{}:{}\n{}\n{}", relative_path.string(), line, col, line_view,
                     pointer_line);
}

std::string formatDiagnosticCompilerSourceLocation(const std::string& filename, std::uint64_t line,
                                                   std::uint64_t column,const std::string & method) {
  return std::format(
      "[file]: {}\n[line]: {}\n[column]: {}\n[function]: {}", filename, line, column,
                     method);
}

template <>
std::string formatDiagnostic<eError, eError::kError>(const DiagnosticDataBufferT& data,
                                                     const TrContext* ctx) noexcept {
  if (data.size() > 0) {
    return std::get<std::string>(data[0]);
  }
  return "Unknown Error";
}

template <>
std::string formatDiagnostic<eWarning, eWarning::kWarning>(const DiagnosticDataBufferT& data,
                                                           const TrContext* ctx) noexcept {
  if (data.size() > 0) {
    return std::get<std::string>(data[0]);
  }
  return "Unknown Warning";
}

template <>
std::string formatDiagnostic<eGuideline, eGuideline::kGuideline>(const DiagnosticDataBufferT& data,
                                                                 const TrContext* ctx) noexcept {
  if (data.size() > 0) {
    return std::get<std::string>(data[0]);
  }
  return "Unknown Guide";
}

template <>
std::string formatDiagnostic<eInfo, eInfo::kInfo>(const DiagnosticDataBufferT& data,
                                                  const TrContext* ctx) noexcept {
  if (data.size() > 0) {
    return std::get<std::string>(data[0]);
  }
  return "Unknown Diagnostic";
}

template <>
std::string formatDiagnostic<eError, eError::kPlaceholder>(const DiagnosticDataBufferT& data,
                                                           const TrContext* ctx) noexcept {
  return std::get<std::string>(data[0]);
}

template <>
std::string formatDiagnostic<eError, eError::kDeveloperBug>(const DiagnosticDataBufferT& data,
                                                            const TrContext* ctx) noexcept {
  return std::format(
      "{}[file]: {}\n[line]: {}\n[column]: {}\n[function]: {}",
      std::get<std::string>(data[4]).empty() ? "" : std::get<std::string>(data[4]) + "\n",
      std::get<std::string>(data[0]), std::get<std::uint64_t>(data[1]),
      std::get<std::uint64_t>(data[2]), std::get<std::string>(data[3]));
}

template <>
std::string formatDiagnostic<eError, eError::kCliParserFailure>(const DiagnosticDataBufferT& data,
                                                                const TrContext* ctx) noexcept {
  if (data.empty()) {
    return "Failed to parse command line arguments.";
  }
  return "Failed to parse command line arguments: " + std::get<std::string>(data[0]);
}

template <>
std::string formatDiagnostic<eError, eError::kLexerUnclosedStringLiteral>(
    const DiagnosticDataBufferT& data, const TrContext* ctx) noexcept {
  std::uint64_t file_id = std::get<std::uint64_t>(data[0]);
  std::uint64_t byte_offset = std::get<std::uint64_t>(data[1]);

  return std::format("{}{}\nString literal is not closed.",
                     formatDiagnosticPrefix(eError::kLexerUnclosedStringLiteral),
                     formatDiagnosticSourceLocation(file_id, byte_offset, ctx, true));
}

template <>
std::string formatDiagnostic<eError, eError::kLexerUnclosedCharacterLiteral>(
    const DiagnosticDataBufferT& data, const TrContext* ctx) noexcept {
  std::uint64_t file_id = std::get<std::uint64_t>(data[0]);
  std::uint64_t byte_offset = std::get<std::uint64_t>(data[1]);

  return std::format("{}{}\nCharacter literal is not closed.",
                     formatDiagnosticPrefix(eError::kLexerUnclosedCharacterLiteral),
                     formatDiagnosticSourceLocation(file_id, byte_offset, ctx, true));
}

template <>
std::string formatDiagnostic<eError, eError::kLexerEmptyCharacterLiteral>(
    const DiagnosticDataBufferT& data, const TrContext* ctx) noexcept {
  std::uint64_t file_id = std::get<std::uint64_t>(data[0]);
  std::uint64_t byte_offset = std::get<std::uint64_t>(data[1]);

  return std::format("{}{}\nEmpty character literal.",
                     formatDiagnosticPrefix(eError::kLexerEmptyCharacterLiteral),
                     formatDiagnosticSourceLocation(file_id, byte_offset, ctx, true));
}

template <>
std::string formatDiagnostic<eError, eError::kLexerUnknownNumericSuffix>(
    const DiagnosticDataBufferT& data, const TrContext* ctx) noexcept {
  std::uint64_t file_id = std::get<std::uint64_t>(data[0]);
  std::uint64_t byte_offset = std::get<std::uint64_t>(data[1]);
  std::string suffix = std::get<std::string>(data[2]);

  return std::format("{}{}\nUnknown scalar suffix '{}'.",
                     formatDiagnosticPrefix(eError::kLexerUnknownNumericSuffix),
                     formatDiagnosticSourceLocation(file_id, byte_offset, ctx, true), suffix);
}

template <>
std::string formatDiagnostic<eError, eError::kLexerInvalidPunctuator>(
    const DiagnosticDataBufferT& data, const TrContext* ctx) noexcept {
  std::uint64_t file_id = std::get<std::uint64_t>(data[0]);
  std::uint64_t byte_offset = std::get<std::uint64_t>(data[1]);
  std::string punctuator = std::get<std::string>(data[2]);

  return std::format("{}{}\nInvalid punctuator '{}'.",
                     formatDiagnosticPrefix(eError::kLexerInvalidPunctuator),
                     formatDiagnosticSourceLocation(file_id, byte_offset, ctx, true), punctuator);
}

template <>
std::string formatDiagnostic<eError, eError::kLexerUnexpectedCodepoint>(
    const DiagnosticDataBufferT& data, const TrContext* ctx) noexcept {
  std::uint64_t file_id = std::get<std::uint64_t>(data[0]);
  std::uint64_t byte_offset = std::get<std::uint64_t>(data[1]);
  std::string codepoint = std::get<std::string>(data[2]);

  return std::format("{}{}\nUnexpected codepoint '{}'.",
                     formatDiagnosticPrefix(eError::kLexerUnexpectedCodepoint),
                     formatDiagnosticSourceLocation(file_id, byte_offset, ctx, true), codepoint);
}

template <>
std::string formatDiagnostic<eError, eError::kLexerUnclosedBlockComment>(
    const DiagnosticDataBufferT& data, const TrContext* ctx) noexcept {
  std::uint64_t file_id = std::get<std::uint64_t>(data[0]);
  std::uint64_t byte_offset = std::get<std::uint64_t>(data[1]);

  return std::format("{}{}\nUnclosed block comment.",
                     formatDiagnosticPrefix(eError::kLexerUnclosedBlockComment),
                     formatDiagnosticSourceLocation(file_id, byte_offset, ctx, true));
}

template <>
std::string formatDiagnostic<eError, eError::kParserExpectedToken>(
    const DiagnosticDataBufferT& data, const TrContext* ctx) noexcept {
  auto data_it = data.begin();
  const std::string& compiler_source_file_name = std::get<std::string>(*data_it);
  data_it++;
  std::uint64_t compiler_source_line = std::get<std::uint64_t>(*data_it);  
  data_it++;
  std::uint64_t compiler_source_column = std::get<std::uint64_t>(*data_it);
  data_it++;
  std::string compiler_source_function = std::get<std::string>(*data_it);  
  data_it++;
  std::uint64_t file_id = std::get<std::uint64_t>(*data_it);
  data_it++;
  std::uint64_t byte_offset = std::get<std::uint64_t>(*data_it);
  data_it++;
  frontend::eToken current_token = static_cast<frontend::eToken>(std::get<std::int64_t>(*data_it));
  data_it++;



  std::string expected_tokens{};
  if (data_it < data.end() && std::holds_alternative<std::string>(*data_it)) {
    expected_tokens = std::get<std::string>(*data_it);
  } else {
    for (auto& it = data_it; it < data.end(); it++) {
      expected_tokens +=
          frontend::eTokenToCStr(static_cast<frontend::eToken>(std::get<std::int64_t>(*it)));
      if (!(it == data.end() - 1)) {
        expected_tokens += " || ";
      }
    }
  }
  return std::format("{}{}\nParser expected token. Current Token: {} Expected: {}\n{}",
                     formatDiagnosticPrefix(eError::kParserExpectedToken),
                     formatDiagnosticSourceLocation(file_id, byte_offset, ctx, true),
                     frontend::eTokenToCStr(current_token), expected_tokens,
                     formatDiagnosticCompilerSourceLocation(compiler_source_file_name, 
                       compiler_source_line, compiler_source_column, compiler_source_function)
  );
}

template <>
std::string formatDiagnostic<eError, eError::kParserInvalidSyntax>(
    const DiagnosticDataBufferT& data, const TrContext* ctx) noexcept {
  auto data_it = data.begin();
  const std::string& compiler_source_file_name = std::get<std::string>(*data_it);
  data_it++;
  std::uint64_t compiler_source_line = std::get<std::uint64_t>(*data_it);
  data_it++;
  std::uint64_t compiler_source_column = std::get<std::uint64_t>(*data_it);
  data_it++;
  std::string compiler_source_function = std::get<std::string>(*data_it);
  data_it++;
  std::uint64_t file_id = std::get<std::uint64_t>(*data_it);
  data_it++;
  std::uint64_t byte_offset = std::get<std::uint64_t>(*data_it);
  data_it++;

  std::string message = std::get<std::string>(*data_it);
  data_it++;

  return std::format(
      "{}{}\n{}\n{}",
      formatDiagnosticPrefix(eError::kParserInvalidSyntax),
      formatDiagnosticSourceLocation(file_id, byte_offset, ctx, true),
      message,
      formatDiagnosticCompilerSourceLocation(compiler_source_file_name, compiler_source_line,
                                             compiler_source_column, compiler_source_function));
}

}  // namespace ssgc