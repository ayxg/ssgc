///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file C& LexerImpl
/// @ingroup
/// @brief
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup cnd_compiler_data
/// @{
#pragma once

#include "frontend_lexer.hpp"

#include <algorithm>
#include <expected>
#include <functional>
#include <string_view>
#include <vector>

#include "diagnostics.hpp"
#include "frontend_enum_token.hpp"
#include "frontend_token.hpp"

namespace ssgc::frontend {

constexpr bool isCharAlpha(char c) noexcept;
constexpr bool isCharNumeric(char c) noexcept;
constexpr bool isCharUnderscore(char c) noexcept;
constexpr bool isCharAlphanumeric(char c) noexcept;
constexpr bool isCharAlphaUnderscore(char c) noexcept;
constexpr bool isCharAlnumus(char c) noexcept;
constexpr bool isCharPunctuator(char c) noexcept;
constexpr bool isCharSpace(char c) noexcept;
constexpr bool isCharNewline(char c) noexcept;
constexpr bool isCharWhitespace(char c) noexcept;
constexpr bool isCharControl(char c) noexcept;
constexpr bool isCharValid(char c) noexcept;
constexpr bool isCharUpper(char c) noexcept;
constexpr eToken getTokenFromLiteral(std::string_view literal) noexcept;

/// Result of a successful intermediate lex step. Holds the processed token and read offset.
struct LexerCursor {
  Token processed_tk{eToken::kNone};
  std::string_view read_head{""};

  LexerCursor(eToken tk, const std::string_view& src, std::string_view::const_iterator lit_begin,
              std::string_view::const_iterator lit_end) noexcept;

  LexerCursor(eToken tk, const std::string_view& src, std::string_view::const_iterator lit_begin,
              std::string_view::const_iterator lit_end, std::size_t beg_line, std::size_t beg_col,
              std::size_t end_line, std::size_t end_col) noexcept;
};

class LexerImpl {
  // LexerImpl public interface.
 public:
  std::expected<std::vector<Token>, diagnostic::Diagnostics> process(
      std::string_view src_str) noexcept;

  // Intermediate lexing methods.
 private:
  LexerCursor lexNumber(std::string_view src_str) noexcept;
  LexerCursor lexIdentifier(std::string_view src_str) noexcept;
  LexerCursor lexPunctuator(std::string_view src_str) noexcept;
  LexerCursor lexWhitespace(std::string_view src_str) noexcept;
  LexerCursor lexNewline(std::string_view src_str) noexcept;
  LexerCursor lexEscapedCharSequence(std::string_view src_str) noexcept;
  LexerCursor lexCharLiteral(std::string_view src_str) noexcept;
  LexerCursor lexLineComment(std::string_view src_str) noexcept;
  LexerCursor lexBlockComment(std::string_view src_str) noexcept;

  // Internal helper methods for tracking line and col count across
  // lexing methods, reducing code duplication and improving readability.
 private:
  void advanceSourceLocation(std::string_view source,
                             std::string_view::const_iterator passed_char) noexcept;

  LexerCursor produceToken(eToken token_type, std::string_view source,
                           const std::string_view::const_iterator& from,
                           const std::string_view::const_iterator& to) noexcept;

  void pushAndAdvance(const LexerCursor& lex_result) {
    tokens_.push_back({lex_result.processed_tk});
    head_ = lex_result.read_head;
  }

  static constexpr inline bool inRange(const std::string_view::const_iterator& iter,
                                       const std::string_view& container) noexcept;
  static constexpr inline bool inRange(std::size_t index, const std::string_view& container);
  static inline bool checkChar(std::string_view source, std::string_view::const_iterator where,
                               char what) noexcept;

  static inline bool checkChar(std::string_view source, std::string_view::const_iterator where,
                               const std::function<bool(char)>& pred) noexcept;

  constexpr inline void pushToken(eToken kind) {
    tokens_.push_back(
        Token{.kind = kind, .file = file_, .offset = offset_, .size = head_ - offset_});
  }

 private:
  std::string_view source_{""};
  std::size_t size_{};

  // std::string_view head_{""};
  std::size_t curr_line_{1};
  std::size_t curr_col_{1};
  diagnostic::Diagnostics messages_{};
  std::vector<Token> tokens_;
  std::size_t file_{};
  std::size_t offset_{};
  std::size_t head_{};
};

LexerCursor::LexerCursor(eToken tk, const std::string_view& s,
                         std::string_view::const_iterator lit_begin,
                         std::string_view::const_iterator lit_end) noexcept
    : processed_tk(tk,
                   std::string_view{s.data() + std::distance(s.begin(), lit_begin),
                                    static_cast<std::size_t>(std::distance(s.begin(), lit_end))}),
      read_head(std::string_view{s.data() + std::distance(s.begin(), lit_end)}) {}

LexerCursor::LexerCursor(eToken tk, const std::string_view& s,
                         std::string_view::const_iterator lit_begin,
                         std::string_view::const_iterator lit_end, std::size_t beg_line,
                         std::size_t beg_col, std::size_t end_line, std::size_t end_col) noexcept
    : processed_tk(tk,
                   std::string_view{s.data() + std::distance(s.begin(), lit_begin),
                                    static_cast<std::size_t>(std::distance(s.begin(), lit_end))},
                   beg_line, beg_col, end_line, end_col),
      read_head(std::string_view{s.data() + std::distance(s.begin(), lit_end)}) {}

void LexerImpl::advanceSourceLocation(std::string_view source,
                                      std::string_view::const_iterator passed_char) noexcept {
  if (checkChar(source, passed_char, isCharNewline)) {
    // Handle Windows-style CRLF newlines as a single newline. Skip line
    // advance.
    if (!(*passed_char == '\r' && checkChar(source, std::next(passed_char), '\n'))) curr_line_++;
    curr_col_ = 1;
  } else {
    curr_col_++;
  }
}

LexerCursor LexerImpl::produceToken(eToken token_type, std::string_view source,
                                    const std::string_view::const_iterator& from,
                                    const std::string_view::const_iterator& to) noexcept {
  auto prev_col = curr_col_;
  return LexerCursor(token_type, source, from, to, curr_line_, prev_col, curr_line_,
                     curr_col_ += std::distance(from, to));
}

constexpr inline bool LexerImpl::inRange(const std::string_view::const_iterator& iter,
                                         const std::string_view& container) noexcept {
  return iter != container.end();
};
constexpr inline bool LexerImpl::inRange(std::size_t index,
                                         const std::string_view& container) noexcept {
  return index < container.size();
};
bool LexerImpl::checkChar(std::string_view source, std::string_view::const_iterator where,
                          char what) noexcept {
  return inRange(where, source) && *where == what;
}

bool LexerImpl::checkChar(std::string_view source, std::string_view::const_iterator where,
                          const std::function<bool(char)>& pred) noexcept {
  return inRange(where, source) && pred(*where);
}

LexerCursor LexerImpl::lexNumber(std::string_view s) noexcept {
  using diagnostic::eError;
  using std::next;

  auto beg = s.begin();
  auto curr = s.begin();
  assert(inRange(curr, s) && "Opening char is eof.");
  assert(isCharNumeric(*curr) && "Opening char is not numeric.");

  // Consume the decimal digits.
  while (inRange(curr, s) && isCharNumeric(*curr)) curr++;

  // If at eof and return i32 early. Unlikely but just in case.
  if (!inRange(curr, s)) return produceToken(eToken::kLitI32, s, beg, curr);

  // If followed by ellipsis('...'), return as i32 early. Avoids ambiguity with
  // floating point processing below.
  if ((inRange(curr, s) && *curr == '.') && (inRange(next(curr), s) && *std::next(curr) == '.') &&
      (inRange(next(curr, 2), s) && *std::next(curr, 2) == '.'))
    return produceToken(eToken::kLitI32, s, beg, curr);

  // Check if it's a floating point literal. Read in decimal digits if a period
  // is found.
  if ((inRange(curr, s) && *curr == '.')) {
    curr++;                                                   // Skip '.'
    while (inRange(curr, s) && isCharNumeric(*curr)) curr++;  // Consume the fractional digits.

    // Check for float literal suffixes if followed by alpha. If no suffixes, default to F64.
    if (checkChar(s, curr, isCharAlpha)) {
      auto suffix_end = curr;
      while (checkChar(s, suffix_end, isCharAlnumus)) suffix_end++;
      auto suffix =
          std::string_view{&*curr, static_cast<std::size_t>(std::distance(curr, suffix_end))};

      if (inRange(curr, s) && suffix == "f")
        return produceToken(eToken::kLitF32, s, beg, ++curr);  // 42.f -> F32

      if (inRange(curr, s) && suffix == "r")
        return produceToken(eToken::kLitReal, s, beg, ++curr);  // 42.r -> Real

      messages_->push_back(diagnostic::makeErrorLexerUnknownNumericSuffix(suffix));
      return produceToken(eToken::kInvalid, s, beg, suffix_end);
    }

    return produceToken(eToken::kLitF64, s, beg, curr);  // 40. -> F64.
  }

  // Check for sized scalar literal suffixes if followed by alpha.
  if (checkChar(s, curr, isCharAlpha)) {
    auto suffix_end = curr;
    while (checkChar(s, suffix_end, isCharAlnumus)) suffix_end++;
    auto suffix =
        std::string_view{&*curr, static_cast<std::size_t>(std::distance(curr, suffix_end))};

    if (suffix == "b" || suffix == "u1")  // U1
      return produceToken(eToken::kLitU1, s, beg, suffix_end);
    else if (suffix == "B" || suffix == "u8")  // U8
      return produceToken(eToken::kLitU8, s, beg, suffix_end);
    else if (suffix == "u16")  // U16
      return produceToken(eToken::kLitU16, s, beg, suffix_end);
    else if (suffix == "u" || suffix == "u32")  // U32
      return produceToken(eToken::kLitU32, s, beg, suffix_end);
    else if (suffix == "U" || suffix == "u64")  // U64
      return produceToken(eToken::kLitU64, s, beg, suffix_end);
    else if (suffix == "c" || suffix == "i8")  // I8
      return produceToken(eToken::kLitI8, s, beg, suffix_end);
    else if (suffix == "i16")  // I16
      return produceToken(eToken::kLitI16, s, beg, suffix_end);
    else if (suffix == "i32")  // I32
      return produceToken(eToken::kLitI32, s, beg, suffix_end);
    else if (suffix == "L" || suffix == "i64")  // I64
      return produceToken(eToken::kLitI64, s, beg, suffix_end);
    else {
      // Error for now, change in future if decided to allow user-defined suffixes.
      messages_->push_back(diagnostic::makeErrorLexerUnknownNumericSuffix(suffix));
      return produceToken(eToken::kInvalid, s, beg, suffix_end);
    }
  }

  return produceToken(eToken::kLitI32, s, beg, curr);  // I32 by default.
}

LexerCursor LexerImpl::lexIdentifier(std::string_view src) noexcept {
  auto beg = src.begin();
  auto curr = beg;
  assert(inRange(curr, src) && "Opening char is eof.");
  assert(isCharAlphaUnderscore(*curr) && "Opening char is not an alpha or underscore.");

  while (inRange(curr, src) && isCharAlnumus(*curr)) curr++;

  // Check for matching keyword.
  eToken etk =
      getTokenFromLiteral(src.substr(0, static_cast<std::size_t>(std::distance(beg, curr))));
  if (etk != eToken::kNone) return produceToken(etk, src, beg, curr);
  return produceToken(eToken::kIdent, src, beg, curr);
}

LexerCursor LexerImpl::lexPunctuator(std::string_view s) noexcept {
  using std::distance;
  using std::next;
  auto c = s.begin();
  assert(inRange(c, s) && "Opening char is eof.");
  assert(isCharPunctuator(*c) && "Opening char is not a punctuator.");

  // Switch over all valid initial punctuators which may form a symbol token.
  // The maximum length of any punctuator is 3.
  if (*c == '=') {
    if (checkChar(s, std::next(c), '='))
      return produceToken(eToken::kEq, s, c, std::next(c, 2));
    else
      return produceToken(eToken::kAssign, s, c, std::next(c));
  } else if (*c == '+') {
    if (checkChar(s, std::next(c), '+'))
      return produceToken(eToken::kInc, s, c, std::next(c, 2));
    else if (checkChar(s, std::next(c), '='))
      return produceToken(eToken::kAddAssign, s, c, std::next(c, 2));
    else
      return produceToken(eToken::kAdd, s, c, std::next(c));
  } else if (*c == '-') {
    if (checkChar(s, std::next(c), '-'))
      return produceToken(eToken::kDec, s, c, std::next(c, 2));
    else if (checkChar(s, std::next(c), '='))
      return produceToken(eToken::kSubAssign, s, c, std::next(c, 2));
    else
      return produceToken(eToken::kSub, s, c, std::next(c));
  } else if (*c == '*') {
    if (checkChar(s, std::next(c), '='))
      return produceToken(eToken::kMulAssign, s, c, std::next(c, 2));
    else
      return produceToken(eToken::kMul, s, c, std::next(c));
  } else if (*c == '/') {
    if (checkChar(s, std::next(c), '='))
      return produceToken(eToken::kDivAssign, s, c, std::next(c, 2));
    else
      return produceToken(eToken::kDiv, s, c, std::next(c));
  } else if (*c == '%') {
    if (checkChar(s, std::next(c), '='))
      return produceToken(eToken::kModAssign, s, c, std::next(c, 2));
    else
      return produceToken(eToken::kMod, s, c, std::next(c));
  } else if (*c == '&') {
    if (checkChar(s, std::next(c), '='))
      return produceToken(eToken::kAndAssign, s, c, std::next(c, 2));
    else if (checkChar(s, std::next(c), '&'))
      return produceToken(eToken::kAnd, s, c, std::next(c, 2));
    else
      return produceToken(eToken::kBand, s, c, std::next(c));
  } else if (*c == '|') {
    if (checkChar(s, std::next(c), '='))
      return produceToken(eToken::kOrAssign, s, c, std::next(c, 2));
    else if (checkChar(s, std::next(c), '|'))
      return produceToken(eToken::kOr, s, c, std::next(c, 2));
    else
      return produceToken(eToken::kBor, s, c, std::next(c));
  } else if (*c == '^') {
    if (checkChar(s, std::next(c), '='))
      return produceToken(eToken::kXorAssign, s, c, std::next(c, 2));
    else
      return produceToken(eToken::kXor, s, c, std::next(c));
  } else if (*c == '<') {
    if (checkChar(s, std::next(c), '<')) {
      if (checkChar(s, std::next(c, 2), '='))
        return produceToken(eToken::kLshAssign, s, c, std::next(c, 3));
      else
        return produceToken(eToken::kLsh, s, c, std::next(c, 2));
    } else if (checkChar(s, std::next(c), '=')) {
      if (checkChar(s, std::next(c, 2), '>'))
        return produceToken(eToken::kSpaceship, s, c, std::next(c, 3));
      else
        return produceToken(eToken::kLte, s, c, std::next(c, 2));
    } else
      return produceToken(eToken::kLt, s, c, std::next(c));
  } else if (*c == '>') {
    if (checkChar(s, std::next(c), '>')) {
      if (checkChar(s, std::next(c, 2), '='))
        return produceToken(eToken::kRshAssign, s, c, std::next(c, 3));
      else
        return produceToken(eToken::kRsh, s, c, std::next(c, 2));
    } else if (checkChar(s, std::next(c), '='))
      return produceToken(eToken::kGte, s, c, std::next(c, 2));
    else
      return produceToken(eToken::kGt, s, c, std::next(c));
  } else if (*c == '!') {
    if (checkChar(s, std::next(c), '='))
      return produceToken(eToken::kNeq, s, c, std::next(c, 2));
    else
      return produceToken(eToken::kNot, s, c, std::next(c));
  } else if (*c == '~') {
    return produceToken(eToken::kBnot, s, c, std::next(c));
  } else if (*c == '@') {
    return produceToken(eToken::kCommercialAt, s, c, std::next(c));
  } else if (*c == '#') {
    return produceToken(eToken::kHash, s, c, std::next(c));
  } else if (*c == '$') {
    return produceToken(eToken::kDollar, s, c, std::next(c));
  } else if (*c == '?') {
    return produceToken(eToken::kQuestion, s, c, std::next(c));
  } else if (*c == ':') {
    if (checkChar(s, std::next(c), ':'))
      return produceToken(eToken::kDoubleColon, s, c, std::next(c, 2));
    else if (checkChar(s, std::next(c), '='))
      return produceToken(eToken::kNewAssign, s, c, std::next(c, 2));
    else
      return produceToken(eToken::kColon, s, c, std::next(c));
  } else if (*c == ';') {
    return produceToken(eToken::kSemicolon, s, c, std::next(c));
  } else if (*c == ',') {
    return produceToken(eToken::kComma, s, c, std::next(c));
  } else if (*c == '.') {
    if (checkChar(s, std::next(c), '.') && checkChar(s, std::next(c, 2), '.'))
      return produceToken(eToken::kEllipsis, s, c, std::next(c, 3));
    return produceToken(eToken::kPeriod, s, c, std::next(c));
  } else if (*c == '(') {
    return produceToken(eToken::kLParen, s, c, std::next(c));
  } else if (*c == ')') {
    return produceToken(eToken::kRParen, s, c, std::next(c));
  } else if (*c == '[') {
    return produceToken(eToken::kLBracket, s, c, std::next(c));
  } else if (*c == ']') {
    return produceToken(eToken::kRBracket, s, c, std::next(c));
  } else if (*c == '{') {
    return produceToken(eToken::kLBrace, s, c, std::next(c));
  } else if (*c == '}') {
    return produceToken(eToken::kRBrace, s, c, std::next(c));
  } else if (*c == '\\') {
    return produceToken(eToken::kBacklash, s, c, std::next(c));
  } else {
    // Unexpected punctuator, recover by skipping to next char and returning invalid token.
    messages_->push_back(diagnostic::makeErrorLexerInvalidPunctuator(std::string_view{c, c + 1}));
    return produceToken(eToken::kInvalid, s, c, std::next(c));
  }
  // This should never happen.
  assert(false && "Unexpected compiler program location reached.");
}

LexerCursor LexerImpl::lexWhitespace(std::string_view s) noexcept {
  assert(head_ < size_ && "Opening char is eof.");
  assert(isCharWhitespace(source_[head_]) && "Opening char is not whitespace.");
  while (head_ < size_ && isCharWhitespace(source_[head_])) {
    head_++;
  }
  tokens_.push_back(Token{
      .kind = eToken::kWhitespace, .file = file_, .offset = offset_, .size = head_ - offset_});
  offset_ = head_;
}

LexerCursor LexerImpl::lexNewline(std::string_view s) noexcept {
  auto c = s.begin();
  assert(inRange(c, s) && "Opening char is eof.");
  assert(isCharNewline(*c) && "Opening char is not a newline.");

  auto begin_col = curr_col_;
  auto begin_line = curr_line_;
  while (checkChar(s, c, isCharNewline)) {
    // Handle Windows-style CRLF newlines as a single newline. Skip line advance.
    if (!(*c == '\r' && checkChar(s, std::next(c), '\n'))) curr_line_++;
    c++;
  }
  curr_col_ = 1;  // Reset column to 1 after newline(s).
  return LexerCursor(eToken::kNewline, s, s.begin(), c, begin_line, begin_col, curr_line_,
                     curr_col_);
}

LexerCursor LexerImpl::lexEscapedCharSequence(std::string_view s) noexcept {
  auto c = s.begin();
  assert(inRange(c, s) && "Opening char is eof.");
  assert((*c == '"') && "Opening char is not a quotation.");

  bool is_escape = false;
  auto begin_col = curr_col_;
  auto begin_line = curr_line_;

  c++;  // Pass opening quote.
  curr_col_++;
  while (inRange(c, s)) {
    if (!is_escape) {
      if (*c == '\\')
        is_escape = true;
      else if (*c == '\"' || isCharNewline(*c))
        break;  // End of string. Quote or newline.
    } else {
      is_escape = false;  // Reset escape.
    }
    c++;
    advanceSourceLocation(s, c);
  }

  // Check if ended with a quote.
  if (inRange(c, s) && *c == '"') {
    c++;  // Advance past the closing quotation.
    curr_col_++;
  } else {
    messages_->push_back(diagnostic::makeErrorLexerUnclosedStringLiteral());
  }

  return LexerCursor(eToken::kLitCstr, s, s.begin(), c, begin_line, begin_col, curr_line_,
                     curr_col_);
}

LexerCursor LexerImpl::lexCharLiteral(std::string_view s) noexcept {
  auto c = s.begin();
  assert(inRange(c, s) && "Opening char is eof.");
  assert((*c == '\'') && "Opening char is not a quotation.");

  // Pass opening quote.
  c++;

  // Newline after opening quote.
  if (checkChar(s, c, isCharNewline)) {
    messages_->push_back(diagnostic::eError::kLexerUnclosedCharacterLiteral);
    return produceToken(eToken::kInvalid, s, s.begin(), c);
  } else if (checkChar(s, c, '\\')) {
    c++;
    // Newline or eof after escape char.
    if (checkChar(s, c, isCharNewline) || !inRange(c, s)) {
      messages_->push_back(diagnostic::eError::kLexerUnclosedCharacterLiteral);
      return produceToken(eToken::kInvalid, s, s.begin(), c);
    }
    // TODO: Check for valid escape char?
    c++;  // Pass escaped char.
  } else if (checkChar(s, c, '\'')) {
    messages_->push_back(diagnostic::eError::kLexerEmptyCharacterLiteral);
    c++;
    return produceToken(eToken::kInvalid, s, s.begin(), c);
  } else if (inRange(c, s)) {
    c++;  // Pass char literal content.
  } else {
    messages_->push_back(diagnostic::eError::kLexerUnclosedCharacterLiteral);
    return produceToken(eToken::kInvalid, s, s.begin(), c);
  }

  // Expect closing quote after char literal content.
  if (!checkChar(s, c, '\'')) {
    messages_->push_back(diagnostic::eError::kLexerUnclosedCharacterLiteral);
    return produceToken(eToken::kInvalid, s, s.begin(), c);
  }
  c++;  // Pass closing quote.

  return produceToken(eToken::kLitI8, s, s.begin(), c);
}

LexerCursor LexerImpl::lexLineComment(std::string_view s) noexcept {
  auto c = s.begin();
  assert(inRange(c, s) && "Opening char is eof.");
  assert((*c == '`') && "Opening char is not a backtick.");
  c++;
  while (inRange(c, s) && !isCharNewline(*c)) c++;
  return produceToken(eToken::kLineComment, s, s.begin(), c);
}

LexerCursor LexerImpl::lexBlockComment(std::string_view s) noexcept {
  auto c = s.begin();
  assert(inRange(c, s) && "Opening char is eof.");
  assert(inRange(next(c), s) && "After opening char is eof.");
  assert((*c == '/') && "Opening char is not a forward slash.");
  assert((*std::next(c) == '`') && "After opening char is not a backtick.");

  auto begin_col = curr_col_;
  auto begin_line = curr_line_;

  std::advance(c, 2);  // pass "/`"
  curr_col_ += 2;
  while (inRange(c, s)) {
    // Check for end of block.
    if (*c == '`') {
      if (!inRange(std::next(c), s))
        messages_->push_back(
            diagnostic::makeErrorPlaceholder("Reached eof before end of block comment."));

      if (*std::next(c) == '/') {
        std::advance(c, 2);  // pass "`/"
        curr_col_ += 2;
        return LexerCursor(eToken::kBlockComment, s, s.begin(), c, begin_line, begin_col,
                           curr_line_, curr_col_);
      } else
        advanceSourceLocation(s, c);
    } else
      advanceSourceLocation(s, c);

    // cont.
    c++;
  }

  messages_->push_back(
      diagnostic::makeErrorPlaceholder("Reached eof before end of block comment."));
  return LexerCursor(eToken::kBlockComment, s, s.begin(), c, begin_line, begin_col, curr_line_,
                     curr_col_);
}

std::expected<std::vector<Token>, diagnostic::Diagnostics> LexerImpl::process(
    std::string_view s) noexcept {
  // Configure lexer state for inital or following process.
  if (s.empty()) return std::vector<Token>{};
  curr_line_ = 1;
  curr_col_ = 1;
  tokens_ = {};
  source_ = s;
  head_ = s;
  messages_->clear();

  // Dispatch to appropriate lexing method based on the first char of the head.
  while (head_ != "") {
    if (isCharNewline(head_[0])) {
      pushAndAdvance(lexNewline(head_));
    } else if (isCharSpace(head_[0])) {
      pushAndAdvance(lexWhitespace(head_));
    } else if (isCharAlphaUnderscore(head_[0])) {
      pushAndAdvance(lexIdentifier(head_));
    } else if (isCharNumeric(head_[0])) {
      pushAndAdvance(lexNumber(head_));
    } else if (head_[0] == '"') {
      pushAndAdvance(lexEscapedCharSequence(head_));
    } else if (head_[0] == '\'') {
      pushAndAdvance(lexCharLiteral(head_));
    } else if (head_[0] == '`') {
      pushAndAdvance(lexLineComment(head_));
    } else if (head_[0] == '/' && head_.size() > 1 && head_[1] == '`') {
      pushAndAdvance(lexBlockComment(head_));
    } else if (isCharPunctuator(head_[0])) {
      pushAndAdvance(lexPunctuator(head_));
    } else {
      messages_->push_back(diagnostic::makeErrorPlaceholder(
          std::format("Unexpected codepoint encountered in source: '{}'.", head_[0])));
      return std::unexpected<diagnostic::Diagnostics>{messages_};
    }
  }

  if (!messages_->empty()) return std::unexpected<diagnostic::Diagnostics>{messages_};
  return tokens_;
}

std::expected<std::vector<Token>, diagnostic::Diagnostics> tokenizeSourceCode(
    std::string_view source_code) noexcept {
  return LexerImpl{}.process(source_code);
}

std::vector<Token> sanitizeTokens(const std::vector<Token>& output_tokens) noexcept {
  // Remove redundant tokens_ after lexing
  // Note: "i" is used instead of "it" to avoid ambiguity with the iterator
  // above.
  return [&]() {
    std::vector<Token> new_output;
    for (auto i = output_tokens.cbegin(); i != output_tokens.cend(); ++i) {
      const std::initializer_list<eToken> REDUNDANT_TOKEN_KINDS{
          eToken::kLineComment, eToken::kBlockComment, eToken::kLineComment, eToken::kWhitespace,
          eToken::kNewline};

      if (std::any_of(REDUNDANT_TOKEN_KINDS.begin(), REDUNDANT_TOKEN_KINDS.end(),
                      [i](eToken match) { return match == i->type_; })) {
        // new_output.push_back(eToken::kWhitespace);
        continue;
      } else {  // Push back non-redundant tokens_
        new_output.push_back(*i);
      }
    }
    return new_output;
  }();  // Note: The lambda is immediately called.
}

Lexer::Lexer() noexcept
    : pimpl_(new LexerImpl(), [](void* p) { delete static_cast<LexerImpl*>(p); }) {}

std::expected<std::vector<Token>, diagnostic::Diagnostics> Lexer::tokenize(
    std::string_view source_code) noexcept {
  return static_cast<LexerImpl*>(pimpl_.get())->process(source_code);
}

std::vector<Token> Lexer::sanitize(const std::vector<Token>& output_tokens) noexcept {
  return sanitizeTokens(output_tokens);
};

constexpr bool isCharAlpha(char c) noexcept {
  switch (c) {
    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
    case 'g':
    case 'h':
    case 'i':
    case 'j':
    case 'k':
    case 'l':
    case 'm':
    case 'n':
    case 'o':
    case 'p':
    case 'q':
    case 'r':
    case 's':
    case 't':
    case 'u':
    case 'v':
    case 'w':
    case 'x':
    case 'y':
    case 'z':
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
    case 'G':
    case 'H':
    case 'I':
    case 'J':
    case 'K':
    case 'L':
    case 'M':
    case 'N':
    case 'O':
    case 'P':
    case 'Q':
    case 'R':
    case 'S':
    case 'T':
    case 'U':
    case 'V':
    case 'W':
    case 'X':
    case 'Y':
    case 'Z':
      return true;
    default:
      return false;
      break;
  }
}

constexpr bool isCharUpper(char c) noexcept {
  switch (c) {
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
    case 'G':
    case 'H':
    case 'I':
    case 'J':
    case 'K':
    case 'L':
    case 'M':
    case 'N':
    case 'O':
    case 'P':
    case 'Q':
    case 'R':
    case 'S':
    case 'T':
    case 'U':
    case 'V':
    case 'W':
    case 'X':
    case 'Y':
    case 'Z':
      return true;
    default:
      return false;
      break;
  }
}

constexpr bool isCharNumeric(char c) noexcept {
  switch (c) {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      return true;
    default:
      return false;
      break;
  }
}

constexpr bool isCharAlphanumeric(char c) noexcept { return isCharAlpha(c) || isCharNumeric(c); }

constexpr bool isCharUnderscore(char c) noexcept { return c == '_'; }

constexpr bool isCharAlphaUnderscore(char c) noexcept {
  return isCharAlpha(c) || isCharUnderscore(c);
}

constexpr bool isCharAlnumus(char c) noexcept {
  return isCharAlpha(c) || isCharNumeric(c) || isCharUnderscore(c);
}

/// '!', '@', '#', '$', '%', '^', '&', '*', '-','+', '=', '{', '}', '[',
/// ']','|', '\\', ';', ':', '\'', '\"', '<', '>', '?', '/', '~', '`', '.',
/// ',','(', ')', '_'
constexpr bool isCharPunctuator(char c) noexcept {
  switch (c) {
    case '!':
    case '@':
    case '#':
    case '$':
    case '%':
    case '^':
    case '&':
    case '*':
    case '-':
    case '+':
    case '=':
    case '{':
    case '}':
    case '[':
    case ']':
    case '|':
    case '\\':
    case ';':
    case ':':
    case '\'':
    case '\"':
    case '<':
    case '>':
    case '?':
    case '/':
    case '~':
    case '`':
    case '.':
    case ',':
    case '(':
    case ')':
    case '_':
      return true;
    default:
      return false;
      break;
  }
}

/// Tab or space.
constexpr bool isCharSpace(char c) noexcept { return c == ' ' || c == '\t'; }

/// '\n', '\r', '\v', '\f'
constexpr bool isCharNewline(char c) noexcept {
  switch (c) {
    case '\n':
    case '\r':
    case '\v':
    case '\f':
      return true;
    default:
      return false;
      break;
  }
}

/// ' ', '\n', '\r', '\v', '\f'
constexpr bool isCharWhitespace(char c) noexcept {
  switch (c) {
    case ' ':
    case '\n':
    case '\r':
    case '\v':
    case '\f':
    case '\t':
      return true;
    default:
      return false;
      break;
  }
}

// '\0', '\a', '\b', '\t', '\n', '\v', '\f', '\r', '\x1b'
constexpr bool isCharControl(char c) noexcept {
  switch (c) {
    case '\0':
    case '\a':
    case '\b':
    case '\t':
    case '\n':
    case '\v':
    case '\f':
    case '\r':
    case '\x1b':
      return true;
    default:
      return false;
  }
}

constexpr bool isCharValid(char c) noexcept {
  return isCharAlpha(c) || isCharNumeric(c) || isCharPunctuator(c) || isCharWhitespace(c);
}

constexpr eToken getTokenFromLiteral(std::string_view literal) noexcept {
#define CND_MM_LOCAL_CASE(n, lt) else if (literal == lt) return eToken::n

  if (literal.empty()) return eToken::kNone;
  // Pragmatic
  // CND_MM_LOCAL_CASE(kKwInclude, "include");
  // CND_MM_LOCAL_CASE(kKwPragma, "pragma");
  // Declarative
  CND_MM_LOCAL_CASE(kKwDef, "def");
  CND_MM_LOCAL_CASE(kKwFn, "fn");
  CND_MM_LOCAL_CASE(kKwClass, "class");
  CND_MM_LOCAL_CASE(kKwMain, "main");
  CND_MM_LOCAL_CASE(kKwImport, "import");
  CND_MM_LOCAL_CASE(kKwNamespace, "namespace");
  CND_MM_LOCAL_CASE(kKwUse, "using");
  CND_MM_LOCAL_CASE(kKwLib, "lib");
  CND_MM_LOCAL_CASE(kKwDll, "dll");
  CND_MM_LOCAL_CASE(kKwEnum, "enum");
  CND_MM_LOCAL_CASE(kKwProc, "proc");

  // Control
  CND_MM_LOCAL_CASE(kKwIf, "if");
  CND_MM_LOCAL_CASE(kKwElif, "elif");
  CND_MM_LOCAL_CASE(kKwElse, "else");
  CND_MM_LOCAL_CASE(kKwCxif, "cxif");
  CND_MM_LOCAL_CASE(kKwCxelif, "cxelif");
  CND_MM_LOCAL_CASE(kKwCxelse, "cxelse");
  CND_MM_LOCAL_CASE(kKwSwitch, "switch");
  CND_MM_LOCAL_CASE(kKwCase, "case");
  CND_MM_LOCAL_CASE(kKwDefault, "default");
  CND_MM_LOCAL_CASE(kKwWhile, "while");
  CND_MM_LOCAL_CASE(kKwFor, "for");
  CND_MM_LOCAL_CASE(kKwReturn, "return");
  CND_MM_LOCAL_CASE(kKwBreak, "break");
  CND_MM_LOCAL_CASE(kKwContinue, "continue");
  // Types
  CND_MM_LOCAL_CASE(kKwInt, "int");
  CND_MM_LOCAL_CASE(kKwUint, "uint");
  CND_MM_LOCAL_CASE(kKwReal, "real");
  CND_MM_LOCAL_CASE(kKwBool, "bool");
  CND_MM_LOCAL_CASE(kKwChar, "char");
  CND_MM_LOCAL_CASE(kKwByte, "byte");
  CND_MM_LOCAL_CASE(kKwCstr, "cstr");
  CND_MM_LOCAL_CASE(kKwStr, "str");
  CND_MM_LOCAL_CASE(kKwPtr, "ptr");
  CND_MM_LOCAL_CASE(kKwList, "list");
  CND_MM_LOCAL_CASE(kKwArray, "array");
  // Constants
  CND_MM_LOCAL_CASE(kKwTrue, "true");
  CND_MM_LOCAL_CASE(kKwFalse, "false");
  CND_MM_LOCAL_CASE(kKwNone, "none");
  CND_MM_LOCAL_CASE(kKwVoid, "void");
  // Functional
  CND_MM_LOCAL_CASE(kKwIn, "in");
  CND_MM_LOCAL_CASE(kKwAs, "as");
  CND_MM_LOCAL_CASE(kKwCin, "cin");
  CND_MM_LOCAL_CASE(kKwCout, "cout");
  CND_MM_LOCAL_CASE(kKwNative, "native");
  // Modifiers
  CND_MM_LOCAL_CASE(kKwConst, "const");
  CND_MM_LOCAL_CASE(kKwRef, "ref");
  CND_MM_LOCAL_CASE(kKwPrivate, "private");
  CND_MM_LOCAL_CASE(kKwPublic, "public");
  CND_MM_LOCAL_CASE(kKwStatic, "static");
  // Meta Types
  CND_MM_LOCAL_CASE(kKwAny, "any");
  CND_MM_LOCAL_CASE(kKwAuto, "auto");
  CND_MM_LOCAL_CASE(kKwType, "type");
  CND_MM_LOCAL_CASE(kKwValue, "value");
  CND_MM_LOCAL_CASE(kKwTemplate, "template");
  CND_MM_LOCAL_CASE(kDirectiveInclude, "#include");
  CND_MM_LOCAL_CASE(kDirectiveDefMacro, "#defmacro");
  CND_MM_LOCAL_CASE(kDirectiveEndmacro, "#endmacro");
  CND_MM_LOCAL_CASE(kDirectiveIf, "#if");
  CND_MM_LOCAL_CASE(kDirectiveElse, "#else");
  CND_MM_LOCAL_CASE(kDirectiveElif, "#elif");
  CND_MM_LOCAL_CASE(kDirectiveEndif, "#endif");
  CND_MM_LOCAL_CASE(kDirectiveIfdef, "#ifdef");
  CND_MM_LOCAL_CASE(kDirectiveIfndef, "#ifndef");
  CND_MM_LOCAL_CASE(kDirectiveUndef, "#undef");
  // Operators
  CND_MM_LOCAL_CASE(kHash, "#");
  CND_MM_LOCAL_CASE(kAdd, "+");
  CND_MM_LOCAL_CASE(kSub, "-");
  CND_MM_LOCAL_CASE(kMul, "*");
  CND_MM_LOCAL_CASE(kDiv, "/");
  CND_MM_LOCAL_CASE(kMod, "%");
  CND_MM_LOCAL_CASE(kAnd, "&");
  CND_MM_LOCAL_CASE(kOr, "|");
  CND_MM_LOCAL_CASE(kXor, "^");
  CND_MM_LOCAL_CASE(kNot, "!");
  CND_MM_LOCAL_CASE(kLsh, "<<");
  CND_MM_LOCAL_CASE(kRsh, ">>");
  CND_MM_LOCAL_CASE(kEq, "==");
  CND_MM_LOCAL_CASE(kNeq, "!=");
  CND_MM_LOCAL_CASE(kLt, "<");
  CND_MM_LOCAL_CASE(kGt, ">");
  CND_MM_LOCAL_CASE(kLte, "<=");
  CND_MM_LOCAL_CASE(kGte, ">=");
  CND_MM_LOCAL_CASE(kAssign, "=");
  CND_MM_LOCAL_CASE(kNewAssign, " : =");
  CND_MM_LOCAL_CASE(kAddAssign, "+=");
  CND_MM_LOCAL_CASE(kSubAssign, "-=");
  CND_MM_LOCAL_CASE(kMulAssign, "*=");
  CND_MM_LOCAL_CASE(kDivAssign, "/=");
  CND_MM_LOCAL_CASE(kModAssign, "%=");
  CND_MM_LOCAL_CASE(kAndAssign, "&=");
  CND_MM_LOCAL_CASE(kOrAssign, "|=");
  CND_MM_LOCAL_CASE(kXorAssign, "^=");
  CND_MM_LOCAL_CASE(kLshAssign, "<<=");
  CND_MM_LOCAL_CASE(kRshAssign, ">>=");
  CND_MM_LOCAL_CASE(kInc, "++");
  CND_MM_LOCAL_CASE(kDec, "--");
  CND_MM_LOCAL_CASE(kBnot, "~");
  CND_MM_LOCAL_CASE(kBand, "&&");
  CND_MM_LOCAL_CASE(kBor, "||");
  CND_MM_LOCAL_CASE(kSpaceship, "<=>");
  // Scopes
  CND_MM_LOCAL_CASE(kLParen, "(");
  CND_MM_LOCAL_CASE(kRParen, ")");
  CND_MM_LOCAL_CASE(kLBrace, "{");
  CND_MM_LOCAL_CASE(kRBrace, "}");
  CND_MM_LOCAL_CASE(kLBracket, "[");
  CND_MM_LOCAL_CASE(kRBracket, "]");
  CND_MM_LOCAL_CASE(kSemicolon, ";");
  CND_MM_LOCAL_CASE(kColon, ":");
  CND_MM_LOCAL_CASE(kComma, ",");
  CND_MM_LOCAL_CASE(kPeriod, ".");
  CND_MM_LOCAL_CASE(kDoubleColon, "::");
  CND_MM_LOCAL_CASE(kEllipsis, "...");
  CND_MM_LOCAL_CASE(kCommercialAt, "@");
  // Special
  CND_MM_LOCAL_CASE(kEofile, "\0");
  CND_MM_LOCAL_CASE(kWhitespace, " ");
  CND_MM_LOCAL_CASE(kNewline, "\n");
  CND_MM_LOCAL_CASE(kBlockComment, "///");
  CND_MM_LOCAL_CASE(kLineComment, "//");
#undef CND_MM_LOCAL_CASE
  else return eToken::kNone;
};

}  // namespace ssgc::frontend
/// @} // end of cnd_compiler_data

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