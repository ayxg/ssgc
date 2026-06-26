#pragma once
#include <cassert>
#include <format>
#include <string>
#include <string_view>
#include <variant>

#include "diagnostic_impl.hpp"

namespace ssgc::diagnostic {

constexpr inline Diagnostic makeErrorPlaceholder(std::string_view sv) {
  return Diagnostic{eError::kPlaceholder, {std::string{sv}}};
}

template <>
std::string formatDiagnostic<eError, eError::kPlaceholder>(
    const DiagnosticDataBufferT& data) noexcept {
  return std::get<std::string>(data[0]);
}

constexpr inline Diagnostic makeErrorDeveloperBug(const std::source_location& src_loc,
                                                  std::string message = "") {
  DiagnosticDataBufferT data{};
  data.reserve(5);
  data.push_back(std::string{src_loc.file_name()});
  data.push_back(std::uint64_t{src_loc.line()});
  data.push_back(std::uint64_t{src_loc.column()});
  data.push_back(std::string{src_loc.function_name()});
  data.push_back(message);
  return Diagnostic{eError::kDeveloperBug, data};
}

template <>
std::string formatDiagnostic<eError, eError::kDeveloperBug>(
    const DiagnosticDataBufferT& data) noexcept {
  return std::format(
      "{}[file]: {}\n[line]: {}\n[column]: {}\n[function]: {}",
      std::get<std::string>(data[4]).empty() ? "" : std::get<std::string>(data[4]) + "\n",
      std::get<std::string>(data[0]), std::get<std::uint64_t>(data[1]),
      std::get<std::uint64_t>(data[2]), std::get<std::string>(data[3]));
}

template <>
std::string formatDiagnostic<eError, eError::kCliParserFailure>(
    const DiagnosticDataBufferT& data) noexcept {
  if (data.empty()) {
    return "Failed to parse command line arguments.";
  }
  return "Failed to parse command line arguments: " + std::get<std::string>(data[0]);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///* kLexerUnclosedStringLiteral */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

constexpr Diagnostic makeErrorLexerUnclosedStringLiteral() noexcept {
  return Diagnostic{eError::kLexerUnclosedStringLiteral, {}};
}

template <>
std::string formatDiagnostic<eError, eError::kLexerUnclosedStringLiteral>(
    const DiagnosticDataBufferT& data) noexcept {
  return "String literal is not closed.";
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///* kLexerUnclosedCharacterLiteral */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <>
std::string formatDiagnostic<eError, eError::kLexerUnclosedCharacterLiteral>(
    const DiagnosticDataBufferT& data) noexcept {
  return "Character literal must be closed.";
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///* kLexerEmptyCharacterLiteral */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <>
std::string formatDiagnostic<eError, eError::kLexerEmptyCharacterLiteral>(
    const DiagnosticDataBufferT& data) noexcept {
  return "Empty character literal.";
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///* kLexerUnknownNumericSuffix */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

constexpr Diagnostic makeErrorLexerUnknownNumericSuffix(std::string_view suffix) noexcept {
  return Diagnostic{eError::kLexerUnknownNumericSuffix, {std::string{suffix}}};
}

template <>
std::string formatDiagnostic<eError, eError::kLexerUnknownNumericSuffix>(
    const DiagnosticDataBufferT& data) noexcept {
  return std::format("Unknown scalar size suffix '{}'", std::get<std::string>(data[0]));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///* kLexerInvalidPunctuator */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

constexpr Diagnostic makeErrorLexerInvalidPunctuator(std::string_view punctuator) noexcept {
  return Diagnostic{eError::kLexerInvalidPunctuator, {std::string{punctuator}}};
}

template <>
std::string formatDiagnostic<eError, eError::kLexerInvalidPunctuator>(
    const DiagnosticDataBufferT& data) noexcept {
  return std::format("Invalid punctuator '{}'", std::get<std::string>(data[0]));
}

}  // namespace ssgc::diagnostic