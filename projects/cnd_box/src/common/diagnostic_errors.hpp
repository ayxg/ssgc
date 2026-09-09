#pragma once
#include <cassert>
#include <filesystem>
#include <format>
#include <string>
#include <string_view>
#include <variant>

#include "diagnostic_impl.hpp"
#include "frontend/enum_token.hpp"
namespace ssgc {

constexpr inline Diagnostic makeErrorPlaceholder(std::string_view sv) {
  return Diagnostic{eError::kPlaceholder, {std::string{sv}}};
}

constexpr inline Diagnostic makeErrorDeveloperBug(const std::source_location& src_loc,
                                                  std::string message) {
  DiagnosticDataBufferT data{};
  data.reserve(5);
  data.push_back(std::string{src_loc.file_name()});
  data.push_back(std::uint64_t{src_loc.line()});
  data.push_back(std::uint64_t{src_loc.column()});
  data.push_back(std::string{src_loc.function_name()});
  data.push_back(message);
  return Diagnostic{eError::kDeveloperBug, data};
}

constexpr inline Diagnostic makeErrorLexerUnclosedStringLiteral(std::size_t file,
                                                                std::size_t byte_offset) noexcept {
  return Diagnostic{eError::kLexerUnclosedStringLiteral,
                    {std::uint64_t(file), std::uint64_t(byte_offset)}};
}

constexpr inline Diagnostic makeErrorLexerUnclosedCharacterLiteral(
    std::size_t file, std::size_t byte_offset) noexcept {
  return Diagnostic{eError::kLexerUnclosedCharacterLiteral,
                    {std::uint64_t(file), std::uint64_t(byte_offset)}};
}

constexpr inline Diagnostic makeErrorLexerEmptyCharacterLiteral(std::size_t file,
                                                                std::size_t byte_offset) noexcept {
  return Diagnostic{eError::kLexerEmptyCharacterLiteral,
                    {std::uint64_t(file), std::uint64_t(byte_offset)}};
}

constexpr inline Diagnostic makeErrorLexerUnknownNumericSuffix(std::size_t file,
                                                               std::size_t byte_offset,
                                                               std::string_view suffix) noexcept {
  return Diagnostic{eError::kLexerUnknownNumericSuffix,
                    {std::uint64_t(file), std::uint64_t(byte_offset), std::string{suffix}}};
}

constexpr inline Diagnostic makeErrorLexerInvalidPunctuator(std::size_t file,
                                                            std::size_t byte_offset,
                                                            std::string_view punctuator) noexcept {
  return Diagnostic{eError::kLexerInvalidPunctuator,
                    {std::uint64_t(file), std::uint64_t(byte_offset), std::string{punctuator}}};
}

constexpr inline Diagnostic makeErrorLexerUnexpectedCodepoint(std::size_t file,
                                                              std::size_t byte_offset,
                                                              char codepoint) noexcept {
  return Diagnostic{eError::kLexerUnexpectedCodepoint,
                    {std::uint64_t(file), std::uint64_t(byte_offset), std::string{codepoint}}};
}

constexpr inline Diagnostic makeErrorLexerUnclosedBlockComment(std::size_t file,
                                                               std::size_t byte_offset) noexcept {
  return Diagnostic{eError::kLexerUnclosedBlockComment,
                    {std::uint64_t(file), std::uint64_t(byte_offset)}};
}

constexpr inline Diagnostic makeErrorParserExpectedToken(
    const std::source_location& src_loc,
    std::size_t file, std::size_t byte_offset, frontend::eToken current_token,
    const std::vector<frontend::eToken>& what_tokens = {}) noexcept {
  DiagnosticDataBufferT args{
      std::string{src_loc.file_name()},
      std::uint64_t{src_loc.line()},
      std::uint64_t{src_loc.column()},
      std::string{src_loc.function_name()},
      std::uint64_t(file),
      std::uint64_t(byte_offset),
      std::int64_t(current_token),

  };
  for (const frontend::eToken& tk : what_tokens) {
    args.push_back(static_cast<std::int64_t>(tk));
  }
  return Diagnostic{eError::kParserExpectedToken, args};
}

constexpr inline Diagnostic makeErrorParserExpectedToken(
    const std::source_location& src_loc,std::size_t file, std::size_t byte_offset,
                                                         frontend::eToken current_token,
                                                         frontend::eToken what_token) noexcept {
  return Diagnostic{
      eError::kParserExpectedToken,
      {std::string{src_loc.file_name()}, std::uint64_t{src_loc.line()},
       std::uint64_t{src_loc.column()}, std::string{src_loc.function_name()}, std::uint64_t(file),
       std::uint64_t(byte_offset), static_cast<std::int64_t>(current_token),
       static_cast<std::int64_t>(what_token)}};
}


constexpr inline Diagnostic makeErrorParserExpectedToken(const std::source_location& src_loc,
                                                         std::size_t file, std::size_t byte_offset,
                                                         frontend::eToken current_token,
                                                         std::string message) noexcept {
  return Diagnostic{
      eError::kParserExpectedToken,
      {std::string{src_loc.file_name()}, std::uint64_t{src_loc.line()},
       std::uint64_t{src_loc.column()}, std::string{src_loc.function_name()}, std::uint64_t(file),
       std::uint64_t(byte_offset), static_cast<std::int64_t>(current_token), message}};
}

constexpr inline Diagnostic makeErrorParserInvalidSyntax(const std::source_location& src_loc,
                                                         std::size_t file, std::size_t byte_offset,
                                                         std::string message) noexcept {
  return Diagnostic{
      eError::kParserInvalidSyntax,
      {std::string{src_loc.file_name()}, std::uint64_t{src_loc.line()},
       std::uint64_t{src_loc.column()}, std::string{src_loc.function_name()}, std::uint64_t(file),
       std::uint64_t(byte_offset), message}};
}

}  // namespace ssgc