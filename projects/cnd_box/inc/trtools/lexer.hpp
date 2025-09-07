///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file C& Lexer
/// @ingroup
/// @brief
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup cnd_compiler_data
/// @{
#pragma once
// clang-format off
#include "use_ccapi.hpp"
#include "use_corevals.hpp"
#include "use_clmsg.hpp"
#include "cldata/tk.hpp"
// clang-format on

/// Set true to enable inline static unit tests during compiler development.
#define CND_ENABLE_STATIC_TEST_MODULE_Lexer true

namespace cnd {
namespace trtools {
using cldev::clmsg::ClMsgBuffer;  //> Result of an error in an intermediate or final lex steps.
using cldev::clmsg::MakeClMsg;
using cxx::IsInRange;     ///> For bounds-checking iterator as the lexer reads forward.
using cxx::StrLiteral;    ///> To allow constexpr lexing and token string literals.
using SrcView = StrView;  // Currently a string view but may allow to std::span to be more versatile.
using SrcViewConstIter = StrView::const_iterator;  ///> Underlying data iterator type.

class Lexer;         ///> Functor-like class which encapsulates the lexing process.
struct LexerCursor;  ///> Result of a successful intermediate lex step. Holds the processed token and read offset.
template <auto STR_LAMBDA>
struct LiteralTkBufferType;  ///> Represents C& extrapolated tokens from a C++ string literal at compile time.

struct LexerCursor {
  Tk processed_tk{eTk::kNONE};
  StrView read_head{""};

  constexpr LexerCursor() noexcept;
  constexpr LexerCursor(StrView read_head, eTk tk, StrView literal) noexcept;
  constexpr LexerCursor(StrView read_head) noexcept;
  constexpr LexerCursor(StrView read_head, eTk tk) noexcept;
  constexpr LexerCursor(eTk tk, const StrView& src, SrcViewConstIter lit_begin, SrcViewConstIter lit_end);
  constexpr LexerCursor(eTk tk, const StrView& src, SrcViewConstIter lit_begin, SrcViewConstIter lit_end, Size beg_line,
                        Size beg_col, Size end_line, Size end_col);
};

class Lexer {
 public:
  using LexerCursorT = LexerCursor;
  using LexerOutputT = Ex<Vec<Tk>, ClMsgBuffer>;       ///> Final output of lexed source chars.
  using LexerFailT = Unex<ClMsgBuffer>;                ///> Lexing failure value.
  using LexerResultT = Ex<LexerCursorT, ClMsgBuffer>;  ///> Intermediate lex step result union.
  using CppSrcLocT = std::source_location;             ///> C++ source location.

 public:  // Lexer public interface.
  constexpr LexerOutputT Process(StrView src_str) noexcept;
  static constexpr LexerOutputT Lex(StrView s) noexcept;
  static constexpr Vec<Tk> Sanitize(const Vec<Tk>& output_tokens) noexcept;
 public:  // Intermediate lexing methods. Only access for testing or exceptional cases.
  constexpr LexerResultT LexNumber(StrView src_str) noexcept;
  constexpr LexerResultT LexIdentifier(StrView src_str) noexcept;
  constexpr LexerResultT LexPunctuator(StrView src_str) noexcept;
  constexpr LexerResultT LexWhitespace(StrView src_str) noexcept;
  constexpr LexerResultT LexNewline(StrView src_str) noexcept;
  constexpr LexerResultT LexEscapedCharSequence(StrView src_str) noexcept;
  constexpr LexerResultT LexRecursiveTokenLiteral(StrView s) noexcept;

 private:  // Internal helper methods for tracking line and col count accross lexing methods.
  constexpr Size& AdvanceLine(const StrView::const_iterator& from, const StrView::const_iterator& to) noexcept;
  constexpr Size& AdvanceCol(const StrView::const_iterator& from, const StrView::const_iterator& to) noexcept;

 private:
  Size curr_line_{0};      ///> Used to maintain line count across intermediate lexing methods.
  Size curr_col_{0};       ///> Used to maintain column count across intermediate lexing methods.
  StrView read_head_{""};  ///> Next read location where a token opening pattern is searched from.
};



// String literal token operator.
namespace literals {

/// @brief Token literal operator converts a string to an array of C& tokens at C++ compile time.
/// @tparam SRC_STR Source string to lex.
/// @return std::array<Tk,???> Array of tokens. Size will depend on extrapolated token array size.
///
/// Use by 'using' the trtools::literal namespace to expose the string literal operators - example:
/// @code
/// using namespace cnd::trtools::literals;
/// constexpr auto my_tokens = "def @foo:42;"_cndTkLiteral;
/// @endcode
template <cxx::StrLiteral SRC_STR>
constexpr auto operator"" _cndTkLiteral() noexcept;

/// @brief Represents a lexed source using lambda functions at compile time.
/// @tparam data_lambda A lambda which returns string-convertible const evaluated data to lex.
///         Must be in the format: [] { return "data"; }
/// @see LiteralTkBuffer for the constexpr value version.
/// @see CND_LAMBDA_STRING macro for creating lambda strings.
/// @use LiteralTkBuffer template variable instead when passing a type isn't needed.
///
/// The size of this class is 1 byte and may be passed freely. Unlike the string literal token operator
/// the data is completely static.
template <auto STR_LAMBDA>
struct LiteralTkBufferType {
  static constexpr Size tokens_size = Lexer::Lex(STR_LAMBDA()).value_or(Vec<Tk>{}).size();
  using PackedDataLambdaT = decltype([] {
    using std::array;
    using std::copy;
    auto buf = Lexer::Lex(STR_LAMBDA());
    array<Tk, tokens_size> out;
    copy(buf.value().data(), buf.value().data() + tokens_size, out.begin());
    return out;
  });
  static constexpr auto tokens = PackedDataLambdaT()();
  static constexpr auto data = STR_LAMBDA();
};

}  // namespace literals

constexpr Lexer::LexerOutputT Lexer::Lex(StrView s) noexcept {
  Lexer lx;
  return lx.Process(s);
}

constexpr LexerCursor::LexerCursor(StrView read_head, eTk tk, StrView literal) noexcept
    : read_head(read_head), processed_tk(tk, literal) {}

constexpr LexerCursor::LexerCursor(StrView read_head) noexcept
    : read_head(read_head), processed_tk(eTk::kNONE, read_head.substr(0, 0)) {}

constexpr LexerCursor::LexerCursor(StrView read_head, eTk tk) noexcept
    : read_head(read_head), processed_tk(tk, read_head.substr(0, 0)) {}

constexpr LexerCursor::LexerCursor(eTk tk, const StrView& s, StrView::const_iterator lit_begin,
                                   StrView::const_iterator lit_end)
    : processed_tk(tk, StrView{s.data() + std::distance(s.begin(), lit_begin),
                               static_cast<Size>(std::distance(s.begin(), lit_end))}),
      read_head(StrView{s.data() + std::distance(s.begin(), lit_end)}) {}

constexpr LexerCursor::LexerCursor(eTk tk, const StrView& s, StrView::const_iterator lit_begin,
                                   StrView::const_iterator lit_end, Size beg_line, Size beg_col, Size end_line,
                                   Size end_col)
    : processed_tk(
          tk,
          StrView{s.data() + std::distance(s.begin(), lit_begin), static_cast<Size>(std::distance(s.begin(), lit_end))},
          beg_line, beg_col, end_line, end_col),
      read_head(StrView{s.data() + std::distance(s.begin(), lit_end)}) {}

constexpr LexerCursor::LexerCursor() noexcept = default;

constexpr Size& Lexer::AdvanceLine(const StrView::const_iterator& from, const StrView::const_iterator& to) noexcept {
  curr_line_ += std::distance(from, to);
  return curr_line_;
}

constexpr Size& Lexer::AdvanceCol(const StrView::const_iterator& from, const StrView::const_iterator& to) noexcept {
  curr_col_ += std::distance(from, to);
  return curr_col_;
}

constexpr Lexer::LexerResultT Lexer::LexNumber(StrView s) noexcept {
  using std::next;
  auto beg = s.begin();
  auto c = s.begin();
#if _DEBUG
  if (!IsInRange(c, s))
    return LexerFailT{MakeClMsg<eClErr::kCompilerDevDebugError>(CppSrcLocT{}, " Opening char is eof.")};
  if (!IsSrcCharNumeric(*c))
    return LexerFailT{MakeClMsg<eClErr::kCompilerDevDebugError>(CppSrcLocT{}, " Opening char is not numeric.")};
#endif
  if (*c == '1' || *c == '0')  // Check for a bit literal '0b' or '1b'.
    if (IsInRange(next(c), s) && *next(c) == 'b')
      return LexerCursor(eTk::kLitBool, s, c, next(c, 2), curr_line_, curr_col_, curr_line_, AdvanceCol(c, next(c, 2)));

  while (IsInRange(c, s) && IsSrcCharNumeric(*c)) c++;  // Consume the decimal digits.

  if (!IsInRange(c, s))  // Unlikely. If at eof and return int.
    return LexerCursor(eTk::kLitInt, s, beg, c, curr_line_, curr_col_, curr_line_, AdvanceCol(beg, c));

  if (IsInRange(c, s) && *c == 'u')
    return LexerCursor(eTk::kLitUint, s, beg, ++c, curr_line_, curr_col_, AdvanceCol(beg, c),
                       curr_line_);  // Unsigned int.

  if (IsInRange(c, s) && *c == 'c')
    return LexerCursor(eTk::kLitByte, s, beg, ++c, curr_line_, curr_col_, AdvanceCol(beg, c),
                       curr_line_);  // Unsigned byte.

  if ((IsInRange(c, s) && *c == '.') && (IsInRange(next(c), s) && *next(c) == '.') &&
      (IsInRange(next(c, 2), s) && *next(c, 2) == '.'))  // If decimal is followed by ellipsis('...'),

    return LexerCursor(eTk::kLitInt, s, beg, c, curr_line_, curr_col_, AdvanceCol(beg, c),
                       curr_line_);  // return as a signed int early.

  if ((IsInRange(c, s) && *c == '.')) {                   // Read in decimal digits if a period is found.
    c++;                                                  // Skip '.'
    while (IsInRange(c, s) && IsSrcCharNumeric(*c)) c++;  // Consume the fractional digits.

    if (IsInRange(c, s) && *c == 'f') {
      return LexerCursor(eTk::kLitReal, s, beg, ++c, curr_line_, curr_col_, AdvanceCol(beg, c),
                         curr_line_);  // 42.f -> Float
    }

    if (IsInRange(c, s) && *c == 'r') {
      return LexerCursor(eTk::kLitReal, s, beg, ++c, curr_line_, curr_col_, AdvanceCol(beg, c),
                         curr_line_);  // 42.r -> Real
    }

    return LexerCursor(eTk::kLitReal, s, beg, c, curr_line_, curr_col_, AdvanceCol(beg, c),
                       curr_line_);  // 40. -> Double.
  }

  return LexerCursor(eTk::kLitInt, s, beg, c, curr_line_, curr_col_, AdvanceCol(beg, c),
                     curr_line_);  // Signed int.
}

constexpr Lexer::LexerResultT Lexer::LexIdentifier(StrView s) noexcept {
  using std::distance;
  auto c = s.begin();
  while (IsInRange(c, s) && IsSrcCharAlnumus(*c)) c++;

  // Todo: optimization, if id_size is not any of keyword sizes, can skip
  // keyword check.
  eTk etk = GetTkFromKeyword(s.substr(0, static_cast<Size>(distance(s.begin(), c))));

  if (etk != eTk::kNONE) return LexerCursor(etk, s, s.begin(), c);

  //// Check for string literal operator.
  // if (crsr.Get() == eSrcChar::kQuotation) {
  //   crsr.Advance();
  //   while (true) {
  //     if (crsr.Get() == eSrcChar::kQuotation && crsr.Next().FindForward(id)) {
  //         //end of literal
  //         break;
  //     }
  //     crsr.Advance();
  //     if (crsr.AtEof())
  //       return ClFail(MakeClErr<kUnterminatedStringLiteral>());
  //   }

  //  Tk tk = eTk::kLitCstr;
  //  tk.SetBeginLoc(id_beg);
  //  tk.SetEndLoc(crsr);
  //  return OffsetLexRes{tk, crsr};
  //}

  return LexerCursor(eTk::kIdent, s, s.begin(), c);
}

constexpr Lexer::LexerResultT Lexer::LexPunctuator(StrView s) noexcept {
  using std::distance;
  auto c = s.begin();
#if _DEBUG
  if (!IsInRange(c, s))
    return LexerFailT{MakeClMsg<eClErr::kCompilerDevDebugError>(CppSrcLocT{}, "Opening char is eof.")};
  if (!IsSrcCharPunctuator(*c))
    return LexerFailT{MakeClMsg<eClErr::kCompilerDevDebugError>(CppSrcLocT{}, "Opening char is not a punctuator.")};
#endif

  // Switch over all valid initial punctuators which may form a symbol token.
  // The maximum length of any punctuator is 3.
  if (*c == '=') {
    if (IsInRange(next(c), s) && *next(c) == '=')
      return LexerCursor(eTk::kEq, s, c, next(c, 2));
    else
      return LexerCursor(eTk::kAssign, s, c, next(c));
  } else if (*c == '+') {
    if (IsInRange(next(c), s) && *next(c) == '+')
      return LexerCursor(eTk::kInc, s, c, next(c, 2));
    else if (*next(c) == '=')
      return LexerCursor(eTk::kAddAssign, s, c, next(c, 2));
    else
      return LexerCursor(eTk::kAdd, s, c, next(c));
  } else if (*c == '-') {
    if (IsInRange(next(c), s) && *next(c) == '-')
      return LexerCursor(eTk::kDec, s, c, next(c, 2));
    else if (*next(c) == '=')
      return LexerCursor(eTk::kSubAssign, s, c, next(c, 2));
    else
      return LexerCursor(eTk::kSub, s, c, next(c));
  } else if (*c == '*') {
    if (IsInRange(next(c), s) && *next(c) == '=')
      return LexerCursor(eTk::kMulAssign, s, c, next(c, 2));
    else
      return LexerCursor(eTk::kMul, s, c, next(c));
  } else if (*c == '/') {
    if (IsInRange(next(c), s) && *next(c) == '=')
      return LexerCursor(eTk::kDivAssign, s, c, next(c, 2));
    else
      return LexerCursor(eTk::kDiv, s, c, next(c));
  } else if (*c == '%') {
    if (IsInRange(next(c), s) && *next(c) == '=')
      return LexerCursor(eTk::kModAssign, s, c, next(c, 2));
    else
      return LexerCursor(eTk::kMod, s, c, next(c));
  } else if (*c == '&') {
    if (IsInRange(next(c), s) && *next(c) == '=')
      return LexerCursor(eTk::kAndAssign, s, c, next(c, 2));
    else if (*next(c) == '&')
      return LexerCursor(eTk::kAnd, s, c, next(c, 2));
    else
      return LexerCursor(eTk::kBand, s, c, next(c));
  } else if (*c == '|') {
    if (IsInRange(next(c), s) && *next(c) == '=')
      return LexerCursor(eTk::kOrAssign, s, c, next(c, 2));
    else if (*next(c) == '|')
      return LexerCursor(eTk::kOr, s, c, next(c, 2));
    else
      return LexerCursor(eTk::kBor, s, c, next(c));
  } else if (*c == '^') {
    if (IsInRange(next(c), s) && *next(c) == '=')
      return LexerCursor(eTk::kXorAssign, s, c, next(c, 2));
    else
      return LexerCursor(eTk::kXor, s, c, next(c));
  } else if (*c == '<') {
    if (IsInRange(next(c), s) && *next(c) == '<') {
      if (IsInRange(next(c), s) && *next(c, 2) == '=')
        return LexerCursor(eTk::kLshAssign, s, c, next(c, 3));
      else
        return LexerCursor(eTk::kLsh, s, c, next(c, 2));
    } else if (*next(c) == '=')
      return LexerCursor(eTk::kLte, s, c, next(c, 2));
    else
      return LexerCursor(eTk::kLt, s, c, next(c));
  } else if (*c == '>') {
    if (IsInRange(next(c), s) && *next(c) == '>') {
      if (IsInRange(next(c), s) && *next(c) == '=')
        return LexerCursor(eTk::kRshAssign, s, c, next(c, 3));
      else
        return LexerCursor(eTk::kRsh, s, c, next(c, 2));
    } else if (*next(c) == '=')
      return LexerCursor(eTk::kGte, s, c, next(c, 2));
    else
      return LexerCursor(eTk::kGt, s, c, next(c));
  } else if (*c == '!') {
    if (IsInRange(next(c), s) && *next(c) == '=')
      return LexerCursor(eTk::kNeq, s, c, next(c, 2));
    else
      return LexerCursor(eTk::kNot, s, c, next(c));
  } else if (*c == '~') {
    return LexerCursor(eTk::kBnot, s, c, next(c));
  } else if (*c == '@') {
    return LexerCursor(eTk::kCommercialAt, s, c, next(c));
  } else if (*c == '#') {
    return LexerCursor(eTk::kHash, s, c, next(c));
  } else if (*c == '$') {
    return LexerCursor(eTk::kDollar, s, c, next(c));
  } else if (*c == '?') {
    return LexerCursor(eTk::kQuestion, s, c, next(c));
  } else if (*c == ':') {
    if (IsInRange(next(c), s) && *next(c) == ':')
      return LexerCursor(eTk::kDoubleColon, s, c, next(c, 2));
    else
    return LexerCursor(eTk::kColon, s, c, next(c));
  } else if (*c == ';') {
    return LexerCursor(eTk::kSemicolon, s, c, next(c));
  } else if (*c == ',') {
    return LexerCursor(eTk::kComma, s, c, next(c));
  } else if (*c == '.') {
    return LexerCursor(eTk::kPeriod, s, c, next(c));
  } else if (*c == '(') {
    return LexerCursor(eTk::kLParen, s, c, next(c));
  } else if (*c == ')') {
    return LexerCursor(eTk::kRParen, s, c, next(c));
  } else if (*c == '[') {
    return LexerCursor(eTk::kLBracket, s, c, next(c));
  } else if (*c == ']') {
    return LexerCursor(eTk::kRBracket, s, c, next(c));
  } else if (*c == '{') {
    return LexerCursor(eTk::kLBrace, s, c, next(c));
  } else if (*c == '}') {
    return LexerCursor(eTk::kRBrace, s, c, next(c));
  } else {
    // This should never happen.
    return LexerFailT{
        MakeClMsg<eClErr::kCompilerDevDebugError>(CppSrcLocT{}, " Unexpected compiler program location reached.")};
  }
  // This should never happen.
  return LexerFailT{
      MakeClMsg<eClErr::kCompilerDevDebugError>(CppSrcLocT{}, " Unexpected compiler program location reached.")};
}

constexpr Lexer::LexerResultT Lexer::LexWhitespace(StrView s) noexcept {
  auto c = s.begin();

#if _DEBUG
  if (!IsInRange(c, s))
    return LexerFailT{MakeClMsg<eClErr::kCompilerDevDebugError>(CppSrcLocT{}, " Opening char is eof.")};
  if (!IsSrcCharWhitespace(*c))
    return LexerFailT{MakeClMsg<eClErr::kCompilerDevDebugError>(CppSrcLocT{}, " Opening char is not whitespace.")};
#endif

  while (IsInRange(c, s) && IsSrcCharWhitespace(*c)) c++;
  return LexerCursor(eTk::kWhitespace, s, s.begin(), c);
}

constexpr Lexer::LexerResultT Lexer::LexNewline(StrView s) noexcept {
  using cldev::clmsg::ClMsgBuffer;
  using cldev::clmsg::MakeClMsg;
  using std::source_location;
  auto c = s.begin();
#if _DEBUG
  if (!IsInRange(c, s))
    return LexerFailT{MakeClMsg<eClErr::kCompilerDevDebugError>(CppSrcLocT{}, " Opening char is eof.")};
  if (!IsSrcCharNewline(*c))
    return LexerFailT{MakeClMsg<eClErr::kCompilerDevDebugError>(CppSrcLocT{}, " Opening char is not a newline.")};
#endif

  while (IsInRange(c, s) && IsSrcCharNewline(*c)) c++;
  curr_line_ += std::distance(s.begin(), c);  // increment line count
  return LexerCursor(eTk::kNewline, s, s.begin(), c);
}

constexpr Lexer::LexerResultT Lexer::LexEscapedCharSequence(StrView s) noexcept {
  using cldev::clmsg::ClMsgBuffer;
  using cldev::clmsg::MakeClMsg;
  using std::source_location;
  auto c = s.begin();
#if _DEBUG
  if (!IsInRange(c, s))
    return LexerFailT{MakeClMsg<eClErr::kCompilerDevDebugError>(CppSrcLocT{}, " Opening char is eof.")};
  if (*c != '"')
    return LexerFailT{MakeClMsg<eClErr::kCompilerDevDebugError>(CppSrcLocT{}, " Opening char is not a quotation.")};
#endif

  c++;  // advance past the opening quotation so we are not out of begin range in the loop.
  while (!(IsInRange(c, s) && *c == '"' && *next(c, -1) != '\\')) {
    c++;
    // Special case '\\'. If there are two backslashes, then it is an escaped
    // backslash. If next is quotation -> escape. Else continue.
    // @note: No IsInRange because we are guaranteed to be at least on the 3rd index here.
    if (*c == '"' && *next(c, -1) == '\\' && *next(c, -2) == '\\') {
      break;
    }
  }
  if (*c == '"') c++;  // Advance past the closing quotation.

  return LexerCursor(eTk::kLitCstr, s, s.begin(), c);
}

// format : T"[<delimiter-ident>]([<token-string>])[<delimiter-ident>]"
constexpr Lexer::LexerResultT Lexer::LexRecursiveTokenLiteral(StrView s) noexcept {
  using cldev::clmsg::ClMsgBuffer;
  using cldev::clmsg::MakeClMsg;
  using std::source_location;
  auto c = s.begin();
#if _DEBUG
  if (!IsInRange(c, s))
    return LexerFailT{MakeClMsg<eClErr::kCompilerDevDebugError>(CppSrcLocT{}, " Opening char is eof.")};
  if (*c != '"')
    return LexerFailT{MakeClMsg<eClErr::kCompilerDevDebugError>(CppSrcLocT{}, " Opening char is not a quotation.")};
#endif

  c++;  // advance past the opening quotation so we are not out of begin range in the loop.
  while (!(IsInRange(c, s) && *c == '"' && *next(c, -1) != '\\')) {
    c++;
    // Special case '\\'. If there are two backslashes, then it is an escaped
    // backslash. If next is quotation -> escape. Else continue.
    // @note: No IsInRange because we are guaranteed to be at least on the 3rd index here.
    if (*c == '"' && *next(c, -1) == '\\' && *next(c, -2) == '\\') {
      break;
    }
  }
  if (*c == '"') c++;  // Advance past the closing quotation.

  return LexerCursor(eTk::kLitCstr, s, s.begin(), c);
}

constexpr Lexer::LexerOutputT Lexer::Process(StrView s) noexcept {
  if (s.empty()) return LexerFailT{MakeClMsg<eClErr::kCompilerDevDebugError>(CppSrcLocT{}, "Cannot lex empty source.")};
  read_head_ = s;
  curr_line_ = 0;
  curr_col_ = 0;
  Vec<Tk> tokens;
  auto it = read_head_.begin();
  while (read_head_ != "") {
    // Newline initial
    if (IsSrcCharNewline(read_head_[0])) {
      auto res_buff = LexNewline(read_head_);
      if (!res_buff) return LexerFailT{res_buff.error()};
      tokens.push_back({res_buff.value().processed_tk});
      read_head_ = res_buff.value().read_head;
      // Whitespace initial
    } else if (IsSrcCharSpace(read_head_[0])) {
      auto res_buff = LexWhitespace(read_head_);
      if (!res_buff) return LexerFailT{res_buff.error()};
      tokens.push_back({res_buff.value().processed_tk});
      read_head_ = res_buff.value().read_head;
      // AlphaUnderscore initial
    } else if (IsSrcCharAlphaUnderscore(read_head_[0])) {
      auto res_buff = LexIdentifier(read_head_);
      if (!res_buff) return LexerFailT{res_buff.error()};
      tokens.push_back({res_buff.value().processed_tk});
      read_head_ = res_buff.value().read_head;
      // Numeric initial
    } else if (IsSrcCharNumeric(read_head_[0])) {
      auto res_buff = LexNumber(read_head_);
      if (!res_buff) return LexerFailT{res_buff.error()};
      tokens.push_back({res_buff.value().processed_tk});
      read_head_ = res_buff.value().read_head;
      // Punctuator initial
    } else if (read_head_[0] == '"') {
      auto res_buff = LexEscapedCharSequence(read_head_);
      if (!res_buff) return LexerFailT{res_buff.error()};
      tokens.push_back({res_buff.value().processed_tk});
      read_head_ = res_buff.value().read_head;
      // Unknown beggining of token...
    } else if (IsSrcCharPunctuator(read_head_[0])) {
      auto res_buff = LexPunctuator(read_head_);
      if (!res_buff) return LexerFailT{res_buff.error()};
      tokens.push_back({res_buff.value().processed_tk});
      read_head_ = res_buff.value().read_head;
      // Quotations initial
    } else {
      return LexerFailT{MakeClMsg<eClErr::kCompilerDevDebugError>(
          CppSrcLocT{}, Str{"Unexpected codepoint encountered in source:"} + read_head_[0])};
    }
  }
  return tokens;
}

constexpr Vec<Tk> Lexer::Sanitize(const Vec<Tk>& output_tokens) noexcept {
  // Remove redundant tokens after lexing
  // Note: "i" is used instead of "it" to avoid ambiguity with the iterator
  // above.
  return [&]() constexpr {
    Vec<Tk> new_output;
    for (auto i = output_tokens.cbegin(); i != output_tokens.cend(); ++i) {
      const std::initializer_list<eTk> REDUNDANT_TOKEN_KINDS{eTk::kLineComment, eTk::kBlockComment,eTk::kLineComment,eTk::kWhitespace,eTk::kNewline};

      if (std::any_of(REDUNDANT_TOKEN_KINDS.begin(), REDUNDANT_TOKEN_KINDS.end(),
                      [i](eTk match) { return match == i->Type(); })) {
        //new_output.push_back(eTk::kWhitespace);
        continue;
      } else {  // Push back non-redundant tokens
        new_output.push_back(*i);
      }
    }
    return new_output;
  }();  // Note: The lambda is immediately called.
}


namespace literals {

template <cxx::StrLiteral SRC_STR>
constexpr auto operator"" _cndTkLiteral() noexcept {
  using std::array;
  using std::copy;
  constexpr auto input = StrView(SRC_STR.data, SRC_STR.size());
  constexpr auto buf_size = Lexer::Lex(input).value_or(Vec<Tk>{}).size();
  auto buf = Lexer::Lex(input);
  array<Tk, buf_size> out;
  copy(buf.value().data(), buf.value().data() + buf_size, out.begin());
  return out;
}

template <cxx::StrLiteral SRC_STR>
constexpr auto operator"" _cndTkSanitized() noexcept {
  using std::array;
  using std::copy;
  constexpr auto input = StrView(SRC_STR.data, SRC_STR.size());
  constexpr auto buf_size = Lexer::Sanitize(Lexer::Lex(input).value_or(Vec<Tk>{})).size();
  auto buf = Lexer::Sanitize(Lexer::Lex(input).value_or(Vec<Tk>{}));
  array<Tk, buf_size> out;
  copy(buf.data(), buf.data() + buf_size, out.begin());
  return out;
}

/// @brief Represents a lexed source using lambda functions at compile time.
/// @see LiteralTkBufferType for details.
template <auto STR_LAMBDA>
static constexpr auto LiteralTkBuffer = LiteralTkBufferType<STR_LAMBDA>{};

}  // namespace literals
}  // namespace trtools
}  // namespace cnd

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