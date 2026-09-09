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

#include "lexer.hpp"

#include <algorithm>
#include <cassert>
#include <expected>
#include <functional>
#include <string_view>
#include <vector>

#include "../common/diagnostic.hpp"
#include "../common/source_range_raw.hpp"
#include "enum_token.hpp"
#include "token.hpp"

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

class LexerImpl {
 public:
  std::pair<std::vector<Token>, Diagnostics> process(std::size_t file_id,
                                                     std::string_view data) noexcept;

 private:
  void lexNumber() noexcept;
  void lexIdentifier() noexcept;
  void lexPunctuator() noexcept;
  void lexWhitespace() noexcept;
  void lexNewline() noexcept;
  void lexEscapedCharSequence() noexcept;
  void lexCharLiteral() noexcept;
  void lexLineComment() noexcept;
  void lexBlockComment() noexcept;

  constexpr inline void produce(eToken kind) noexcept;
  constexpr inline bool inRange() const noexcept;
  constexpr inline bool inRange(std::size_t lookahead) const noexcept;
  constexpr inline void advance() noexcept;
  constexpr inline void advance(std::size_t count) noexcept;
  constexpr inline bool check(bool (*predicate)(char)) const noexcept;
  constexpr inline bool check(char character) const noexcept;
  constexpr inline bool match(bool (*predicate)(char)) const noexcept;
  constexpr inline bool match(char character) const noexcept;
  constexpr inline char peek() const noexcept;
  constexpr inline bool lookahead(char character, std::size_t lookahead) const noexcept;

 private:
  std::string_view source_{""};
  std::size_t size_{};
  Diagnostics messages_{};
  std::vector<Token> tokens_;
  std::size_t file_{};
  std::size_t offset_{};
  std::size_t head_{};
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
  CND_MM_LOCAL_CASE(kKwInclude, "include");
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

constexpr inline void LexerImpl::produce(eToken kind) noexcept {
  tokens_.push_back(Token{
      .kind = kind, .source_range = SourceRangeRaw{.file = file_, .begin = offset_, .end = head_}});
  offset_ = head_;
}

constexpr inline bool LexerImpl::inRange() const noexcept { return head_ < size_; }

constexpr inline bool LexerImpl::inRange(std::size_t lookahead) const noexcept {
  return head_ + lookahead < size_;
}

constexpr inline void LexerImpl::advance() noexcept { head_++; }

constexpr inline void LexerImpl::advance(std::size_t count) noexcept { head_ += count; }

constexpr inline bool LexerImpl::check(bool (*predicate)(char)) const noexcept {
  return inRange() && predicate(source_[head_]);
}

constexpr inline bool LexerImpl::check(char character) const noexcept {
  return inRange() && source_[head_] == character;
}

constexpr inline bool LexerImpl::match(bool (*predicate)(char)) const noexcept {
  return predicate(source_[head_]);
}

constexpr inline bool LexerImpl::match(char character) const noexcept {
  return source_[head_] == character;
}

constexpr inline char LexerImpl::peek() const noexcept { return source_[head_]; }

constexpr inline bool LexerImpl::lookahead(char character, std::size_t lookahead) const noexcept {
  return inRange(lookahead) && source_[head_ + lookahead] == character;
}

void LexerImpl::lexNumber() noexcept {
  assert(inRange() && "Opening char is eof.");
  assert(isCharNumeric(peek()) && "Opening char is not numeric.");

  // Consume the decimal digits.
  while (check(isCharNumeric)) {
    advance();
  }

  // If at eof and return i32 early. Unlikely but just in case.
  if (!inRange()) {
    produce(eToken::kLitI32);
    return;
  }

  // If followed by ellipsis('...'), return as i32 early. Avoids ambiguity with
  // floating point processing below.
  if (check('.') && lookahead('.', 1) && lookahead('.', 2)) {
    produce(eToken::kLitI32);
    return;
  }

  // Check if it's a floating point literal. Read in decimal digits if a period
  // is found.
  if (check('.')) {
    advance();  // Skip '.'

    // Consume the fractional digits.
    while (check(isCharNumeric)) {
      advance();
    }

    // Check for float literal suffixes if followed by alpha. If no suffixes, default to F64.
    if (check(isCharAlpha)) {
      std::size_t suffix_begin = head_;
      while (check(isCharAlnumus)) {
        advance();
      }
      auto suffix = std::string_view{&source_[suffix_begin], head_ - suffix_begin};

      // 42.f -> F32
      if (suffix == "f") {
        produce(eToken::kLitF32);
        return;
      }

      if (suffix == "r") {
        produce(eToken::kLitReal);
        return;
      }

      messages_->push_back(makeErrorLexerUnknownNumericSuffix(file_, head_, suffix));
      produce(eToken::kInvalid);
      return;
    }

    produce(eToken::kLitF64);  // 40. -> F64.
    return;
  }

  // Check for sized scalar literal suffixes if followed by alpha.
  if (check(isCharAlpha)) {
    std::size_t suffix_begin = head_;
    while (check(isCharAlnumus)) {
      advance();
    }
    auto suffix = std::string_view{&source_[suffix_begin], head_ - suffix_begin};

    if (suffix == "b" || suffix == "u1")
      produce(eToken::kLitU1);
    else if (suffix == "B" || suffix == "u8")
      produce(eToken::kLitU8);
    else if (suffix == "u16")
      produce(eToken::kLitU16);
    else if (suffix == "u" || suffix == "u32")
      produce(eToken::kLitU32);
    else if (suffix == "U" || suffix == "u64")
      produce(eToken::kLitU64);
    else if (suffix == "c" || suffix == "i8")
      produce(eToken::kLitI8);
    else if (suffix == "i16")
      produce(eToken::kLitI16);
    else if (suffix == "i32")
      produce(eToken::kLitI32);
    else if (suffix == "L" || suffix == "i64")
      produce(eToken::kLitI64);
    else {
      // Error for now, change in future if decided to allow user-defined suffixes.
      messages_->push_back(makeErrorLexerUnknownNumericSuffix(file_, head_, suffix));
      produce(eToken::kInvalid);
    }
    return;
  }

  produce(eToken::kLitI32);  // I32 by default.
  return;
}

void LexerImpl::lexIdentifier() noexcept {
  assert(inRange() && "Opening char is eof.");
  assert(isCharAlphaUnderscore(peek()) && "Opening char is not an alpha or underscore.");

  while (check(isCharAlnumus)) {
    advance();
  }

  // Check if this identifier is a keyword.
  eToken keyword = getTokenFromLiteral(source_.substr(offset_, head_ - offset_));
  if (keyword != eToken::kNone) {
    return produce(keyword);
  }

  produce(eToken::kIdent);
}

void LexerImpl::lexPunctuator() noexcept {
  assert(inRange() && "Opening char is eof.");
  assert(isCharPunctuator(peek()) && "Opening char is not a punctuator.");

  // Switch over all valid initial punctuators which may form a symbol token.
  // The maximum length of any punctuator is 3.
  char fchar = peek();
  switch (peek()) {
    case '=':
      advance();
      if (check('=')) {
        advance();
        produce(eToken::kEq);
      } else {
        produce(eToken::kAssign);
      }
      return;

    case '+':
      advance();
      if (check('+')) {
        advance();
        produce(eToken::kInc);
      } else if (check('=')) {
        advance();
        produce(eToken::kAddAssign);
      } else {
        produce(eToken::kAdd);
      }
      return;

    case '-':
      advance();
      if (check('-')) {
        advance();
        produce(eToken::kDec);
      } else if (check('=')) {
        advance();
        produce(eToken::kSubAssign);
      } else if (check('>')) {
        advance();
        produce(eToken::kRightArrow);
      } else {
        produce(eToken::kSub);
      }
      return;

    case '*':
      advance();
      if (check('=')) {
        advance();
        produce(eToken::kMulAssign);
      } else {
        produce(eToken::kMul);
      }
      return;

    case '/':
      advance();
      if (check('=')) {
        advance();
        produce(eToken::kDivAssign);
      } else {
        produce(eToken::kDiv);
      }
      return;

    case '%':
      advance();
      if (check('=')) {
        advance();
        produce(eToken::kModAssign);
      } else {
        produce(eToken::kMod);
      }
      return;

    case '&':
      advance();
      if (check('=')) {
        advance();
        produce(eToken::kAndAssign);
      } else if (check('&')) {
        advance();
        produce(eToken::kAnd);
      } else {
        produce(eToken::kBand);
      }
      return;

    case '|':
      advance();
      if (check('=')) {
        advance();
        produce(eToken::kOrAssign);
      } else if (check('|')) {
        advance();
        produce(eToken::kOr);
      } else {
        produce(eToken::kBor);
      }
      return;

    case '^':
      advance();
      if (check('=')) {
        advance();
        produce(eToken::kXorAssign);
      } else {
        produce(eToken::kXor);
      }
      return;

    case '<':
      advance();
      if (check('<')) {
        advance();
        if (check('=')) {
          advance();
          produce(eToken::kLshAssign);
        } else {
          produce(eToken::kLsh);
        }
      } else if (check('=')) {
        advance();
        if (check('>')) {
          advance();
          produce(eToken::kSpaceship);
        } else {
          produce(eToken::kLte);
        }
      } else {
        produce(eToken::kLt);
      }
      return;

    case '>':
      advance();
      if (check('>')) {
        advance();
        if (check('=')) {
          advance();
          produce(eToken::kRshAssign);
        } else {
          produce(eToken::kRsh);
        }
      } else if (check('=')) {
        advance();
        produce(eToken::kGte);
      } else {
        produce(eToken::kGt);
      }
      return;

    case '!':
      advance();
      if (check('=')) {
        advance();
        produce(eToken::kNeq);
      } else {
        produce(eToken::kNot);
      }
      return;

    case '~':
      advance();
      produce(eToken::kBnot);
      return;

    case '@':
      advance();
      produce(eToken::kCommercialAt);
      return;

    case '#':
      advance();
      produce(eToken::kHash);
      return;

    case '$':
      advance();
      produce(eToken::kDollar);
      return;

    case '?':
      advance();
      produce(eToken::kQuestion);
      return;

    case ':':
      advance();
      if (check(':')) {
        advance();
        produce(eToken::kDoubleColon);
      } else if (check('=')) {
        advance();
        produce(eToken::kNewAssign);
      } else {
        produce(eToken::kColon);
      }
      return;

    case ';':
      advance();
      produce(eToken::kSemicolon);
      return;

    case '.':
      advance();
      if (check('.') && lookahead('.', 1)) {
        advance(2);
        produce(eToken::kEllipsis);
      } else {
        produce(eToken::kPeriod);
      }
      return;

    case ',':
      advance();
      produce(eToken::kComma);
      return;

    case '(':
      advance();
      produce(eToken::kLParen);
      return;

    case ')':
      advance();
      produce(eToken::kRParen);
      return;

    case '[':
      advance();
      produce(eToken::kLBracket);
      return;

    case ']':
      advance();
      produce(eToken::kRBracket);
      return;

    case '{':
      advance();
      produce(eToken::kLBrace);
      return;

    case '}':
      advance();
      produce(eToken::kRBrace);
      return;

    case '\\':
      advance();
      produce(eToken::kBacklash);
      return;

    default:
      messages_->push_back(
          makeErrorLexerInvalidPunctuator(file_, head_, std::string_view{&source_[head_], 1}));
      produce(eToken::kInvalid);
      advance();
      return;
  }
}

void LexerImpl::lexWhitespace() noexcept {
  assert(inRange() && "Opening char is eof.");
  assert(isCharWhitespace(peek()) && "Opening char is not whitespace.");
  while (check(isCharWhitespace)) {
    advance();
  }
  produce(eToken::kWhitespace);
}

void LexerImpl::lexNewline() noexcept {
  assert(inRange() && "Opening char is eof.");
  assert(isCharNewline(peek()) && "Opening char is not a newline.");
  while (check(isCharNewline)) {
    advance();
  }
  produce(eToken::kNewline);
  return;
}

void LexerImpl::lexEscapedCharSequence() noexcept {
  assert(inRange() && "Opening char is eof.");
  assert((peek() == '"') && "Opening char is not a quotation.");
  bool is_escape = false;
  bool was_closed = false;
  advance();  // Pass opening quote.
  while (inRange()) {
    if (!is_escape) {
      if (peek() == '\\') {
        is_escape = true;
        advance();
      } else if (peek() == '\"') {
        advance();
        was_closed = true;
        break;  // End of string.
      } else if (isCharNewline(peek())) {
        // Dont advance. Next token is newline.
        messages_->push_back(makeErrorLexerUnclosedStringLiteral(file_, head_));
        break;  // End of string.
      } else {
        advance();
      }
    } else {
      is_escape = false;  // Reset escape.
      advance();
    }
  }
  if (!was_closed) {
    messages_->push_back(makeErrorLexerUnclosedStringLiteral(file_, head_));
  }
  produce(eToken::kLitCstr);
  return;
}

void LexerImpl::lexCharLiteral() noexcept {
  assert(inRange() && "Opening char is eof.");
  assert((peek() == '\'') && "Opening char is not a quotation.");

  // Pass opening quote.
  advance();

  // Newline after opening quote.
  if (check(isCharNewline)) {
    messages_->push_back(makeErrorLexerUnclosedCharacterLiteral(file_, head_));
    return produce(eToken::kInvalid);
  }
  // Escaped char.
  else if (check('\\')) {
    advance();
    // Newline or eof after escape char.
    if (check(isCharNewline) || !inRange()) {
      messages_->push_back(makeErrorLexerUnclosedCharacterLiteral(file_, head_));
      return produce(eToken::kInvalid);
    }
    // TODO: Check for valid escape char?
    advance();  // Pass escaped char.
  }
  // Empty char literal.
  else if (check('\'')) {
    messages_->push_back(makeErrorLexerEmptyCharacterLiteral(file_, head_));
    advance();
    return produce(eToken::kInvalid);
  }
  // Normal.
  else if (inRange()) {
    advance();  // Pass char literal content.
  }
  // Eof after opening quote.
  else {
    messages_->push_back(makeErrorLexerUnclosedCharacterLiteral(file_, head_));
    return produce(eToken::kInvalid);
  }

  // Expect closing quote after char literal content.
  if (!check('\'')) {
    messages_->push_back(makeErrorLexerUnclosedCharacterLiteral(file_, head_));
    return produce(eToken::kInvalid);
  }
  advance();  // Pass closing quote.

  produce(eToken::kLitI8);
}

void LexerImpl::lexLineComment() noexcept {
  assert(inRange() && "Opening char is eof.");
  assert((check('`')) && "Opening char is not a backtick.");
  advance();
  while (inRange() && !isCharNewline(peek())) {
    advance();
  }
  return produce(eToken::kLineComment);
}

void LexerImpl::lexBlockComment() noexcept {
  assert(inRange() && "Opening char is eof.");
  assert(inRange(1) && "After opening char is eof.");
  assert((peek() == '/') && "Opening char is not a forward slash.");
  assert(lookahead('`',1) && "After opening char is not a backtick.");

  advance(2);
  while (inRange()) {
    // Check for end of block.
    if (peek() == '`') {
      advance();
      if (!inRange()) {
        messages_->push_back(makeErrorLexerUnclosedBlockComment(file_, head_));
        produce(eToken::kInvalid);
        return;
      }

      if (peek() == '/') {
        advance();
        produce(eToken::kBlockComment);
        return;
      } else {
        continue;
      }
    } else {
      advance();
    }
  }

  messages_->push_back(makeErrorLexerUnclosedBlockComment(file_, head_));
  produce(eToken::kBlockComment);
  return;
}

std::pair<std::vector<Token>, Diagnostics> LexerImpl::process(std::size_t file_id,
                                                              std::string_view data) noexcept {
  // Configure lexer state for inital or following process.
  if (data.empty()) return std::pair<std::vector<Token>, Diagnostics>{};
  tokens_ = {};
  source_ = data;
  size_ = data.size();
  file_ = file_id;
  head_ = 0;
  offset_ = 0;
  messages_->clear();

  // Dispatch to appropriate lexing method based on the first char of the head.
  while (inRange()) {
    if (isCharNewline(peek())) {
      lexNewline();
    } else if (isCharSpace(peek())) {
      lexWhitespace();
    } else if (isCharAlphaUnderscore(peek())) {
      lexIdentifier();
    } else if (isCharNumeric(peek())) {
      lexNumber();
    } else if (peek() == '"') {
      lexEscapedCharSequence();
    } else if (peek() == '\'') {
      lexCharLiteral();
    } else if (peek() == '`') {
      lexLineComment();
    } else if (peek() == '/' && lookahead('`',1)) {
      lexBlockComment();
    } else if (isCharPunctuator(peek())) {
      lexPunctuator();
    } else if (peek() == '\0') {
      advance();
    } else {
      messages_->push_back(makeErrorLexerUnexpectedCodepoint(file_, head_, peek()));
      advance();  // Skip invalid character.
    }
  }
  return std::make_pair(tokens_, messages_);
  // if (!messages_->empty()) return std::unexpected<Diagnostics>{messages_};
  // return tokens_;
}

Lexer::Lexer() noexcept
    : pimpl_(new LexerImpl(), [](void* p) { delete static_cast<LexerImpl*>(p); }) {}

Diagnostics Lexer::tokenize(std::size_t file_id, std::string_view source_code,
                            std::vector<Token>& output) noexcept {
  auto [tokens, diagnostics] = static_cast<LexerImpl*>(pimpl_.get())->process(file_id, source_code);
  output.append_range(std::ranges::subrange(tokens.begin(),tokens.end()));
  return diagnostics;
}

// std::vector<Token> sanitizeTokens(const std::vector<Token>& output_tokens) noexcept {
//   // Remove redundant tokens_ after lexing
//   // Note: "i" is used instead of "it" to avoid ambiguity with the iterator
//   // above.
//   return [&]() {
//     std::vector<Token> new_output;
//     for (auto i = output_tokens.cbegin(); i != output_tokens.cend(); ++i) {
//       const std::initializer_list<eToken> REDUNDANT_TOKEN_KINDS{
//           eToken::kLineComment, eToken::kBlockComment, eToken::kLineComment, eToken::kWhitespace,
//           eToken::kNewline};
//
//       if (std::any_of(REDUNDANT_TOKEN_KINDS.begin(), REDUNDANT_TOKEN_KINDS.end(),
//                       [i](eToken match) { return match == i->kind; })) {
//         // new_output.push_back(eToken::kWhitespace);
//         continue;
//       } else {  // Push back non-redundant tokens_
//         new_output.push_back(*i);
//       }
//     }
//     return new_output;
//   }();  // Note: The lambda is immediately called.
// }

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