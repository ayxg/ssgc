///////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
///////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language Environment
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup cand_compiler_lexer
/// @brief C& Lexer/Tokenizer.
///////////////////////////////////////////////////////////////////////////////

/// @addtogroup cand_compiler_lexer
/// @{
#ifndef HEADER_GUARD_CALE_CAND_OFFICIAL_COMPILER_CAOCO_LEXER_H
#define HEADER_GUARD_CALE_CAND_OFFICIAL_COMPILER_CAOCO_LEXER_H
#include "cxxx.hpp"

// 
#include "caoco_char_traits.h"
#include "caoco_grammar.h"
//
#include "caoco_compiler_error.h"
#include "caoco_token.h"

namespace caoco {

using CharVector = std::vector<char>;
using CharVectorCIter = CharVector::const_iterator;

/// Lexer Object
class Lexer {
 public:
  static constexpr char kEofChar = grammar::kEofChar;
  using LexMethodResult = cxx::PartialExpected<Tk, CharVectorCIter>;
  using LexerResult = cxx::Expected<TkVector>;
  using LexerSourceLoc = std::tuple<size_t, size_t>;

  /// @defgroup cand_compiler_lexer_interface Public Interface
  /// @ingroup cand_compiler_lexer
  /// 
  /// Static methods for easy lexing of vectors or strings
  /// @{
 public:
  static constexpr inline LexerResult Lex(CharVectorCIter beg,
                                          CharVectorCIter end) {
    Lexer lexer(beg, end);
    return lexer();
  }

  static constexpr inline LexerResult Lex(const CharVector& input) {
    Lexer lexer(input.cbegin(), input.cend());
    return lexer();
  }

  static constexpr inline LexerResult Lex(const std::string& input) {
    CharVector input_vec(input.cbegin(), input.cend());
    Lexer lexer(input_vec.cbegin(), input_vec.cend());
    return lexer();
  }

  constexpr explicit Lexer(CharVectorCIter beg, CharVectorCIter end)
      : beg_(beg), end_(end) {}

  constexpr LexerResult operator()() {
    // Check for empty input
    if (beg_ == end_) {
      return LexerResult::Failure("Empty input");
    }
    return Lex();
  }
  /// @} // end of cand_compiler_lexer_interface

  /// @defgroup cand_compiler_lexer_util Lexing Utils
  /// @ingroup cand_compiler_lexer
  /// 
  /// Lexer's utility methods.
  /// @{
 private:
  constexpr inline LexMethodResult Success(eTk type, CharVectorCIter beg_it,
                                           CharVectorCIter end_it);
  constexpr inline LexMethodResult NoneResult(CharVectorCIter beg_it);
  constexpr inline LexMethodResult FailureResult(CharVectorCIter beg_it,
                                                 const std::string& error);

  // Lexer's Utility functions
  constexpr bool NotAtEof(CharVectorCIter it) const { return it != end_; }
  constexpr char Get(CharVectorCIter it) const;
  constexpr char Peek(CharVectorCIter it, int n);
  constexpr bool FindForward(CharVectorCIter it,
                             std::string_view characters) const;
  constexpr CharVectorCIter& Advance(CharVectorCIter& it, int n = 1);
  constexpr inline CharVectorCIter Begin() const { return beg_; }
  constexpr inline CharVectorCIter End() const { return end_; }

  constexpr inline string GeneratePrettyErrorLineLocation(
      vector<char>::const_iterator it) {
    // Find the last newline before the current character
    auto last_newline = std::find(std::reverse_iterator(it),
                                  std::reverse_iterator(Begin()), '\n')
                            .base();

    // If there is no newline before the current character, use the start of
    // the string
    if (last_newline == End()) {
      last_newline = Begin();
    }

    // Calculate the character index within the line
    std::size_t current_col =
        static_cast<std::size_t>(std::distance(last_newline, it));

    //// Find the following newline
    auto next_newline = std::find(it, End(), '\n');

    // If there is no newline after the current character, use the end of the
    // string
    if (next_newline == End()) {
      next_newline = End();
    }

    // Calculate the start and end iterators for the pretty error line
    vector<char>::const_iterator pretty_line_start = last_newline;
    vector<char>::const_iterator pretty_line_end = next_newline;

    // Adjust the pretty line start iterator if it exceeds 40 characters to the
    // left
    size_t chars_to_left = std::min(current_col, (std::size_t)40u);
    if (current_col > 40) pretty_line_start = std::prev(it, chars_to_left);

    // Adjust the pretty line end iterator if it exceeds 40 characters to the
    // right
    size_t chars_to_right = std::min(std::distance(it, next_newline), (std::ptrdiff_t)40);
    if (std::distance(it, next_newline) > 40)
      pretty_line_end = std::next(it, chars_to_right);

    // Create the pretty error line with a caret at the current character
    string pretty_error_line(pretty_line_start, pretty_line_end);
    pretty_error_line += '\n';
    size_t caret_position = std::distance(pretty_line_start, it) + 1;
    pretty_error_line.append(caret_position - 1, ' ');
    pretty_error_line += '^';

    return pretty_error_line;
  }

  constexpr inline void CalculateLineColPos(CharVectorCIter it,
                                            std::size_t& line,
                                            std::size_t& col) {
    // Find the current line - count newlines from begin to iter.
    line += std::count(Begin(), it, '\n');

    // Find the last newline before the iter.
    CharVectorCIter last_newline =
        std::find(std::reverse_iterator(it), std::reverse_iterator(Begin()),
                  '\n')
            .base();

    // If there is no newline before the current iter, use the start of
    // the source
    if (last_newline == End()) {
      last_newline = Begin();
    }

    // Calculate the character index within the line
    col = static_cast<std::size_t>(std::distance(last_newline, it));
  }
  /// @} // end of cand_compiler_lexer_util

  /// @defgroup cand_compiler_lexer_lex Lexing Methods
  /// @ingroup cand_compiler_lexer
  ///
  /// Lexing methods.
  /// @{
  // Lexers
  constexpr LexMethodResult LexSolidus(CharVectorCIter it);
  constexpr LexMethodResult LexQuotation(CharVectorCIter it);
  constexpr LexMethodResult LexNewline(CharVectorCIter it);
  constexpr LexMethodResult LexWhitespace(CharVectorCIter it);
  constexpr LexMethodResult LexEof(CharVectorCIter it);
  constexpr LexMethodResult LexNumber(CharVectorCIter it);
  constexpr LexMethodResult LexIdentifier(CharVectorCIter it);
  constexpr LexMethodResult LexDirective(CharVectorCIter it);
  constexpr LexMethodResult LexKeyword(CharVectorCIter it);
  constexpr LexMethodResult LexOperator(CharVectorCIter it);
  constexpr LexMethodResult LexScopes(CharVectorCIter it);
  constexpr LexMethodResult LexSemicolon(CharVectorCIter it);
  constexpr LexMethodResult LexColon(CharVectorCIter it);
  constexpr LexMethodResult LexComma(CharVectorCIter it);
  constexpr LexMethodResult LexPeriod(CharVectorCIter it);

  constexpr LexerResult Lex();
  /// @} // end of cand_compiler_lexer_lex

  // Members
  /// Start of the source string.
  CharVectorCIter beg_;
  /// End of the source string.
  CharVectorCIter end_;
};

// Lexer's Utility methods
constexpr inline Lexer::LexMethodResult Lexer::Success(eTk type,
                                                       CharVectorCIter beg_it,
                                                       CharVectorCIter end_it) {
  return LexMethodResult::Success(
      end_it, Tk(type, beg_it, end_it,
                 std::make_tuple(std::distance(Begin(), beg_it),
                            std::distance(Begin(), end_it))));
}

constexpr inline Lexer::LexMethodResult Lexer::NoneResult(
    CharVectorCIter beg_it) {
  return LexMethodResult::Success(beg_it, Tk(eTk::NONE, beg_it, beg_it));
}

constexpr inline Lexer::LexMethodResult Lexer::FailureResult(
    CharVectorCIter beg_it, const std::string& error) {
  return LexMethodResult::Failure(beg_it, error);
}
/// kEofChar if it is anything but a valid iterator
constexpr char Lexer::Get(CharVectorCIter it) const {
  if (it >= end_) return kEofChar;
  if (it < beg_) return kEofChar;
  return *it;
}

constexpr char Lexer::Peek(CharVectorCIter it, int n) {
  if (std::distance(it, end_) < n) return kEofChar;  // Out of bounds cant Peek
  return Get(it + n);
}

/// Searches forward for a complete match of characters. Starting from it,
/// inclusive.
constexpr bool Lexer::FindForward(CharVectorCIter it,
                                  std::string_view characters) const {
  if (std::distance(it, end_) < static_cast<std::ptrdiff_t>(characters.size()))
    return false;  // Out of bounds cant match
  auto end = std::next(it, static_cast<std::ptrdiff_t>(characters.size()));
  if (std::equal(characters.cbegin(), characters.cend(), it, end)) return true;
  return false;
}

/// No checks performed. Use with caution.
constexpr CharVectorCIter& Lexer::Advance(CharVectorCIter& it, int n) {
  std::advance(it, n);
  return it;
}

// Lexers
constexpr Lexer::LexMethodResult Lexer::LexSolidus(CharVectorCIter it) {
  using namespace grammar;
  CharVectorCIter begin = it;
  if (Get(it) == kDivChar) {
    if (Peek(it, 1) == kDivChar &&
        Peek(it, 2) !=
            kDivChar) {  // Line comment two solidus '//' closed by '\n'
      while (!cand_char::IsNewline(Get(it)) && Get(it) != kEofChar) {
        Advance(it);
      }
      return Success(eTk::LineComment, begin, it);
    } else if (Peek(it, 1) == kDivChar &&
               Peek(it, 2) == kDivChar) {  // Block comment three solidus
      // '///' closed by '///'
      Advance(it, 3);
      while (!FindForward(it, kBlockComment)) {
        Advance(it);
      }
      Advance(it, 3);
      /* Cursor is at end of block,
              1. move to next character, (1)
              2. Advance past block comment closing(3)
              3. then go 1 past to the end.(1)
              Total: 5
      */
      return Success(eTk::BlockComment, begin, it);
    } else {
      Advance(it);
      // if the next character is a '=' then we have a division assignment
      // operator
      if (Get(it) == kEqChar) {
        Advance(it);
        return Success(eTk::DivAssign, begin, it);
      }
      // otherwise we have a division operator
      else
        return Success(eTk::Div, begin, it);
    }
  } else {
    return NoneResult(begin);
  }
}

constexpr Lexer::LexMethodResult Lexer::LexQuotation(CharVectorCIter it) {
  using namespace grammar;
  CharVectorCIter begin = it;
  if (Get(it) == kApostropheChar) {
    Advance(it);

    while (!(Get(it) == kApostropheChar && Peek(it, -1) != kBacklashChar)) {
      Advance(it);
      // Special case '\\'. If there are two backslashes, then it is an escaped
      // backslash. If next is quotation -> escape. Else continue.
      if (Get(it) == kApostropheChar && Peek(it, -1) == kBacklashChar &&
          Peek(it, -2) == kBacklashChar) {
        break;
      }
    }
    if (Get(it) == kApostropheChar) Advance(it);

    // Check for byte literal
    if (Get(it) == u8'c') {
      Advance(it);
      return Success(eTk::LitByte, begin, it);
    } else
      return Success(eTk::LitCstr, begin, it);
  } else {
    return NoneResult(begin);
  }
}

constexpr Lexer::LexMethodResult Lexer::LexNewline(CharVectorCIter it) {
  CharVectorCIter begin = it;
  if (NotAtEof(it) && cand_char::IsNewline(it)) {
    while (NotAtEof(it) && cand_char::IsNewline(it)) {
      Advance(it);
    }
    return Success(eTk::Newline, begin, it);
  } else {
    return NoneResult(begin);
  }
}

constexpr Lexer::LexMethodResult Lexer::LexWhitespace(CharVectorCIter it) {
  CharVectorCIter begin = it;
  if (cand_char::IsWhitespace(Get(it))) {
    while (cand_char::IsWhitespace(Get(it))) {
      Advance(it);
    }
    return Success(eTk::Whitespace, begin, it);
  } else {
    return NoneResult(begin);
  }
}

constexpr Lexer::LexMethodResult Lexer::LexEof(CharVectorCIter it) {
  CharVectorCIter begin = it;
  if (Get(it) == kEofChar) {
    Advance(it);
    return Success(eTk::Eofile, begin, it);
  } else {
    return NoneResult(begin);
  }
}

constexpr Lexer::LexMethodResult Lexer::LexNumber(CharVectorCIter it) {
  using namespace grammar;
  CharVectorCIter begin = it;
  if (cand_char::IsNumeric(Get(it))) {
    // Special case for 1b and 0b
    if (Get(it) == '1' && Peek(it, 1) == 'b') {
      Advance(it, 2);
      return Success(eTk::LitBool, begin, it);
    } else if (Get(it) == '0' && Peek(it, 1) == 'b') {
      Advance(it, 2);
      return Success(eTk::LitBool, begin, it);
    }

    while (cand_char::IsNumeric(Get(it))) {
      Advance(it);
    }

    // Special case for unsigned literal (overflow is handled by the parser)
    if (Get(it) == 'u') {
      Advance(it);
      return Success(eTk::LitUint, begin, it);
    }

    // Special case for byte literal(overflow is handled by the parser)
    if (Get(it) == 'c') {
      Advance(it);
      return Success(eTk::LitByte, begin, it);
    }

    // If number is followed by elipsis. Return the number.
    if (FindForward(it, kEllipsis)) {
      return Success(eTk::LitInt, begin, it);
    }

    // Else process a floating literal.
    if (Get(it) == kPeriodChar) {
      Advance(it);
      while (cand_char::IsNumeric(Get(it))) {
        Advance(it);
      }
      return Success(eTk::LitReal, begin, it);
    }

    return Success(eTk::LitInt, begin, it);
  } else {
    return NoneResult(begin);
  }
}

constexpr Lexer::LexMethodResult Lexer::LexIdentifier(CharVectorCIter it) {
  CharVectorCIter begin = it;
  if (cand_char::IsAlpha(Get(it))) {
    while (cand_char::IsAlnumus(Get(it))) {
      Advance(it);
    }
    return Success(eTk::Ident, begin, it);
  } else {
    return NoneResult(begin);
  }
}

constexpr Lexer::LexMethodResult Lexer::LexDirective(CharVectorCIter it) {
  CharVectorCIter beg = it;
  if (Get(it) == grammar::kHashChar) {
    Advance(it);
    while (NotAtEof(it) && cand_char::IsAlnumus(it)) {
      Advance(it);
    }
    std::string_view temp_sv(beg, it);

    LexMethodResult temp_result = FailureResult(beg, "[Unknown Directive]");

    std::apply(
        [&](const auto&... traits) -> void {
          [](...) {}((
              [&]<typename T>(const T& x) {
                if (temp_sv == std::decay_t<T>::literal) {
                  temp_result = Success(std::decay_t<T>::type, beg, it);
                }
              }(std::forward<decltype(traits)>(traits)),
              false)...);
        },
        tk_traits::kDirectiveTraits);

    return temp_result;
  } else {
    return NoneResult(beg);
  }
}

constexpr Lexer::LexMethodResult Lexer::LexKeyword(CharVectorCIter it) {
  CharVectorCIter beg = it;
  if (cand_char::IsAlpha(it)) {
    Advance(it);
    while (NotAtEof(it) && cand_char::IsAlnumus(it)) {
      Advance(it);
    }

    std::string_view temp_sv(beg, it);

    // C++ 20 template lambda with fold expressions
    LexMethodResult temp_result = NoneResult(beg);
    std::apply(
        [&](const auto&... traits) -> void {
          [](...) {}((
              [&]<typename T>(const T& x) {
                if (temp_sv == std::decay_t<T>::literal) {
                  temp_result = Success(std::decay_t<T>::type, beg, it);
                }
              }(std::forward<decltype(traits)>(traits)),
              false)...);
        },
        tk_traits::kKeywordTraits);

    return temp_result;

  } else {
    return NoneResult(beg);
  }
}

constexpr Lexer::LexMethodResult Lexer::LexOperator(CharVectorCIter it) {
  using namespace grammar;
  using enum eTk;
  CharVectorCIter begin = it;
  if (Get(it) == kEqChar) {
    if (Peek(it, 1) == kEqChar) {
      Advance(it, 2);
      return Success(Eq, begin, it);
    } else {
      Advance(it);
      return Success(Assign, begin, it);
    }
  } else if (Get(it) == kAddChar) {
    if (Peek(it, 1) == kAddChar) {
      Advance(it, 2);
      return Success(Inc, begin, it);
    } else if (Peek(it, 1) == kEqChar) {
      Advance(it, 2);
      return Success(AddAssign, begin, it);
    } else {
      Advance(it);
      return Success(Add, begin, it);
    }
  } else if (Get(it) == kSubChar) {
    if (Peek(it, 1) == kSubChar) {
      Advance(it, 2);
      return Success(Dec, begin, it);
    } else if (Peek(it, 1) == kEqChar) {
      Advance(it, 2);
      return Success(SubAssign, begin, it);
    } else {
      Advance(it);
      return Success(Sub, begin, it);
    }
  } else if (Get(it) == kMulChar) {
    if (Peek(it, 1) == kEqChar) {
      Advance(it, 2);
      return Success(MulAssign, begin, it);
    } else {
      Advance(it);
      return Success(Mul, begin, it);
    }
  } else if (Get(it) == kDivChar) {
    if (Peek(it, 1) == kEqChar) {
      Advance(it, 2);
      return Success(DivAssign, begin, it);
    } else {
      Advance(it);
      return Success(Div, begin, it);
    }
  } else if (Get(it) == kModChar) {
    if (Peek(it, 1) == kEqChar) {
      Advance(it, 2);
      return Success(ModAssign, begin, it);
    } else {
      Advance(it);
      return Success(Mod, begin, it);
    }
  } else if (Get(it) == kAndChar) {
    if (Peek(it, 1) == kEqChar) {
      Advance(it, 2);
      return Success(AndAssign, begin, it);
    } else if (Peek(it, 1) == kAndChar) {
      Advance(it, 2);
      return Success(And, begin, it);
    } else {
      Advance(it);
      return Success(Band, begin, it);
    }
  } else if (Get(it) == kOrChar) {
    if (Peek(it, 1) == kEqChar) {
      Advance(it, 2);
      return Success(OrAssign, begin, it);
    } else if (Peek(it, 1) == kOrChar) {
      Advance(it, 2);
      return Success(Or, begin, it);
    } else {
      Advance(it);
      return Success(Bor, begin, it);
    }
  } else if (Get(it) == kXorChar) {
    if (Peek(it, 1) == kEqChar) {
      Advance(it, 2);
      return Success(XorAssign, begin, it);
    } else {
      Advance(it);
      return Success(Xor, begin, it);
    }
  } else if (Get(it) == kLtChar) {
    if (Peek(it, 1) == kLtChar) {
      if (Peek(it, 2) == kEqChar) {
        Advance(it, 3);
        return Success(LshAssign, begin, it);
      } else {
        Advance(it, 2);
        return Success(Lsh, begin, it);
      }
    } else if (Peek(it, 1) == kEqChar) {
      if (Peek(it, 2) == kGtChar) {
        Advance(it, 3);
        return Success(Spaceship, begin, it);
      }
      Advance(it, 2);
      return Success(Lte, begin, it);
    } else {
      Advance(it);
      return Success(Lt, begin, it);
    }
  } else if (Get(it) == kGtChar) {
    if (Peek(it, 1) == kGtChar) {
      if (Peek(it, 2) == kEqChar) {
        Advance(it, 3);
        return Success(RshAssign, begin, it);
      } else {
        Advance(it, 2);
        return Success(Rsh, begin, it);
      }
    } else if (Peek(it, 1) == kEqChar) {
      Advance(it, 2);
      return Success(Gte, begin, it);
    } else {
      Advance(it);
      return Success(Gt, begin, it);
    }
  } else if (Get(it) == kNotChar) {
    if (Peek(it, 1) == kEqChar) {
      Advance(it, 2);
      return Success(Neq, begin, it);
    } else {
      Advance(it);
      return Success(Not, begin, it);
    }
  } else if (Get(it) == kTildeChar) {
    Advance(it);
    return Success(Bnot, begin, it);
  } else if (Get(it) == kCommercialAtChar) {
    Advance(it);
    return Success(CommercialAt, begin, it);
  } else {
    return NoneResult(begin);
  }
}

constexpr Lexer::LexMethodResult Lexer::LexScopes(CharVectorCIter it) {
  using namespace grammar;
  using enum eTk;
  CharVectorCIter begin = it;
  if (Get(it) == kLParenChar) {
    Advance(it);
    return Success(LParen, begin, it);
  } else if (Get(it) == kRParenChar) {
    Advance(it);
    return Success(RParen, begin, it);
  } else if (Get(it) == kLBraceChar) {
    Advance(it);
    return Success(LBrace, begin, it);
  } else if (Get(it) == kRBraceChar) {
    Advance(it);
    return Success(RBrace, begin, it);
  } else if (Get(it) == kLBracketChar) {
    Advance(it);
    return Success(LBracket, begin, it);
  } else if (Get(it) == kRBracketChar) {
    Advance(it);
    return Success(RBracket, begin, it);
  } else {
    return NoneResult(begin);
  }
}

constexpr Lexer::LexMethodResult Lexer::LexSemicolon(CharVectorCIter it) {
  CharVectorCIter begin = it;
  if (Get(it) == grammar::kSemicolonChar) {
    Advance(it);
    return Success(eTk::Semicolon, begin, it);
  } else {
    return NoneResult(begin);
  }
}

constexpr Lexer::LexMethodResult Lexer::LexColon(CharVectorCIter it) {
  CharVectorCIter begin = it;
  if (Get(it) == grammar::kColonChar) {
    Advance(it);
    if (Get(it) == grammar::kColonChar) {
      Advance(it);
      return Success(eTk::DoubleColon, begin, it);
    }
    return Success(eTk::Colon, begin, it);
  } else {
    return NoneResult(begin);
  }
}

constexpr Lexer::LexMethodResult Lexer::LexComma(CharVectorCIter it) {
  CharVectorCIter begin = it;
  if (Get(it) == grammar::kCommaChar) {
    Advance(it);
    return Success(eTk::Comma, begin, it);
  } else {
    return NoneResult(begin);
  }
}

constexpr Lexer::LexMethodResult Lexer::LexPeriod(CharVectorCIter it) {
  CharVectorCIter begin = it;
  if (FindForward(it, grammar::kEllipsis)) {
    Advance(it, 3);
    return Success(eTk::Ellipsis, begin, it);
  } else if (Get(it) == grammar::kPeriodChar) {
    Advance(it);
    return Success(eTk::Period, begin, it);
  } else {
    return NoneResult(begin);
  }
}

/// Main tokenizer method
constexpr Lexer::LexerResult Lexer::Lex() {
  using namespace caerr;
  CharVectorCIter it = Begin();
  TkVector output_tokens;
  std::size_t current_line = 1;
  std::size_t current_col = 1;

  // Lambda for executing a lexer and updating the iterator.
  LAMBDA xPerformLex = [&](auto lexer) constexpr -> cxx::Expected<bool> {
    using cxx::Expected;
    // Disable warning for uninitialized variable, it is initialized above.
#pragma warning(disable : 6001)
    LexMethodResult lex_result = (this->*lexer)(it);
#pragma warning(default : 6001)
    if (!lex_result.Valid()) {
      CalculateLineColPos(it, current_line, current_col);
      return Expected<bool>::Failure(CaErr::ErrDetail<CaErr::LexerUnknownChar>(
          current_line, current_col, *it, GeneratePrettyErrorLineLocation(it),
          lex_result.Error()));
    }
    Tk result_token = lex_result.Value();
    CharVectorCIter result_end = lex_result.Always();

    if (result_token.Type() == eTk::NONE) {  // No match, try next lexer
      return Expected<bool>::Success(false);
    }

    else {  // Lexing was successful
      CalculateLineColPos(it, current_line, current_col);
      // Set the line and col of the resulting token and emplace it into the
      // output vector
      result_token.SetLine(current_line);
      result_token.SetCol(current_col);
      output_tokens.push_back(result_token);
      it = result_end;  // Advance the iterator to the end of lexing. Note lex
      // end and token end may differ.
      return Expected<bool>::Success(true);
    }
  };  // end xPerformLex

  // Attempt to lex a token using one of the lexers until one succeeds. If none
  // succeed, report error. Order of lexers is important. For example, the
  // identifier lexer will match keywords, so it must come after the keyword
  // lexer.
  while (it != end_) {
    bool match = false;
    for (auto lexer :
         {&Lexer::LexSolidus, &Lexer::LexQuotation, &Lexer::LexNewline,
          &Lexer::LexWhitespace, &Lexer::LexEof, &Lexer::LexKeyword,
          &Lexer::LexDirective, &Lexer::LexNumber, &Lexer::LexIdentifier,
          &Lexer::LexOperator, &Lexer::LexScopes, &Lexer::LexSemicolon,
          &Lexer::LexColon, &Lexer::LexComma, &Lexer::LexPeriod}) {
      auto lex_result = xPerformLex(lexer);
      if (not lex_result) {  // Error inside one of the lexers
        return LexerResult::Failure(lex_result.Error());
      } else if (lex_result.Value()) {
        // Note: The iterator 'it' is advanced in perform_lex lambda.
        match = true;
        break;  // Exit for-loop
      }
    }
    // None of the lexers matched, report an error
    // Update position based on the number of characters consumed
    if (!match) {
      CalculateLineColPos(it, current_line, current_col);
      return LexerResult::Failure(CaErr::ErrDetail<CaErr::LexerUnknownChar>(
          current_line, current_col, *it, GeneratePrettyErrorLineLocation(it)));
    }
  }  // end while

  // Remove redundant tokens after lexing
  // Note: "i" is used instead of "it" to avoid ambiguity with the iterator
  // above.
  TkVector sanitized = [&]() constexpr {
    TkVector new_output;
    for (auto i = output_tokens.cbegin(); i != output_tokens.cend(); ++i) {
      const std::initializer_list<eTk> REDUNDANT_TOKEN_KINDS{
          eTk::Whitespace, eTk::LineComment, eTk::BlockComment, eTk::Newline};

      if (std::any_of(REDUNDANT_TOKEN_KINDS.begin(),
                      REDUNDANT_TOKEN_KINDS.end(),
                      [i](eTk match) { return match == i->Type(); })) {
        continue;
      } else {  // Push back non-redundant tokens
        new_output.push_back(*i);
      }
    }
    return new_output;
  }();  // Note: The lambda is immediately called.

  return LexerResult::Success(sanitized);
}  // end tokenize

}  // namespace caoco

#endif HEADER_GUARD_CALE_CAND_OFFICIAL_COMPILER_CAOCO_LEXER_H
/// @} // end of cand_compiler_lexer

///////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language Environment
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
//
// Licensed under the GNU Affero General Public License, Version 3.
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
///////////////////////////////////////////////////////////////////////////////