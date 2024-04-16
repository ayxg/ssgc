//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
// Copyright 2024 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
//---------------------------------------------------------------------------//
// Author(s): Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.acpp.dev
//---------------------------------------------------------------------------//
// Project: C& Programming Language Environment
// Directory: cand-official-compiler
// File: caoco_lexer.h
//---------------------------------------------------------------------------//
#ifndef HEADER_GUARD_CALE_CAND_OFFICIAL_COMPILER_CAOCO_LEXER_H
#define HEADER_GUARD_CALE_CAND_OFFICIAL_COMPILER_CAOCO_LEXER_H
//---------------------------------------------------------------------------//
// Brief: C& Lexer/Tokenizer
//---------------------------------------------------------------------------//
#include "cppsextended.h"
// Includes:
#include "caoco_char_traits.h"
#include "caoco_compiler_error.h"
#include "caoco_enum.h"
#include "caoco_grammar.h"
#include "caoco_token.h"
#include "caoco_token_traits.h"
//---------------------------------------------------------------------------//

namespace caoco {

using CharVector = std::vector<char>;
using CharVectorCIter = CharVector::const_iterator;
class Lexer {
 public:
  static constexpr char kEofChar = grammar::characters::kEofile::value;
  using LexMethodResult = cxx::PartialExpected<Tk, CharVectorCIter>;
  using LexerResult = cxx::Expected<TkVector>;

 public:
  // Util static methods for easy lexing of vectors or strings
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

 private:
  constexpr inline LexMethodResult SuccessResult(eTk type,
                                                 CharVectorCIter beg_it,
                                                 CharVectorCIter end_it);

  constexpr inline LexMethodResult NoneResult(CharVectorCIter beg_it);

  constexpr inline LexMethodResult FailureResult(CharVectorCIter beg_it,
                                                 const std::string& error);

  // Members
  CharVectorCIter beg_;
  CharVectorCIter end_;

  // Lexer's Utility functions
  constexpr bool NotAtEof(CharVectorCIter it) const { return it != end_; }

  constexpr char Get(CharVectorCIter it) const;

  constexpr char Peek(CharVectorCIter it, int n);

  constexpr bool FindForward(CharVectorCIter it,
                             std::string_view characters) const;

  constexpr CharVectorCIter& Advance(CharVectorCIter& it, int n = 1);

  constexpr inline CharVectorCIter Begin() const { return beg_; }
  constexpr inline CharVectorCIter End() const { return end_; }

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
};
// Lexer's Utility methods
constexpr inline Lexer::LexMethodResult Lexer::SuccessResult(
    eTk type, CharVectorCIter beg_it, CharVectorCIter end_it) {
  return LexMethodResult::Success(end_it, Tk(type, beg_it, end_it));
}

constexpr inline Lexer::LexMethodResult Lexer::NoneResult(
    CharVectorCIter beg_it) {
  return LexMethodResult::Success(beg_it, Tk(eTk::kNone, beg_it, beg_it));
}

constexpr inline Lexer::LexMethodResult Lexer::FailureResult(
    CharVectorCIter beg_it, const std::string& error) {
  return LexMethodResult::Failure(beg_it, error);
}

constexpr char Lexer::Get(CharVectorCIter it) const {
  // kEofChar if it is anything but a valid iterator
  if (it >= end_) return kEofChar;
  if (it < beg_) return kEofChar;
  return *it;
}

constexpr char Lexer::Peek(CharVectorCIter it, int n) {
  if (std::distance(it, end_) < n) return kEofChar;  // Out of bounds cant Peek
  return Get(it + n);
}

constexpr bool Lexer::FindForward(CharVectorCIter it,
                                  std::string_view characters) const {
  // Searches forward for a complete match of characters. Starting from it,
  // inclusive.
  if (std::distance(it, end_) < static_cast<std::ptrdiff_t>(characters.size()))
    return false;  // Out of bounds cant match
  auto end = std::next(it, static_cast<std::ptrdiff_t>(characters.size()));
  if (std::equal(it, end, characters.cbegin(), characters.cend())) return true;
  return false;
}

constexpr CharVectorCIter& Lexer::Advance(CharVectorCIter& it, int n) {
  // No checks performed. Use with caution.
  std::advance(it, n);
  return it;
}

// Lexers
constexpr Lexer::LexMethodResult Lexer::LexSolidus(CharVectorCIter it) {
  using namespace grammar::characters;
  CharVectorCIter begin = it;
  if (Get(it) == kDiv::value) {
    if (Peek(it, 1) == kDiv::value &&
        Peek(it, 2) !=
            kDiv::value) {  // Line comment two solidus '//' closed by '\n'
      while (!cand_char::IsNewline(Get(it)) && Get(it) != kEofChar) {
        Advance(it);
      }
      return SuccessResult(eTk::kLineComment, begin, it);
    } else if (Peek(it, 1) == kDiv::value &&
               Peek(it, 2) == kDiv::value) {  // Block comment three solidus
      // '///' closed by '///'
      Advance(it, 3);
      while (!FindForward(it, grammar::kBlockComment::value)) {
        Advance(it);
      }
      Advance(it, 3);
      /* Cursor is at end of block,
              1. move to next character, (1)
              2. Advance past block comment closing(3)
              3. then go 1 past to the end.(1)
              Total: 5
      */
      return SuccessResult(eTk::kBlockComment, begin, it);
    } else {
      Advance(it);
      // if the next character is a '=' then we have a division assignment
      // operator
      if (Get(it) == kEq::value) {
        Advance(it);
        return SuccessResult(eTk::kDivisionAssignment, begin, it);
      }
      // otherwise we have a division operator
      else
        return SuccessResult(eTk::kDivision, begin, it);
    }
  } else {
    return NoneResult(begin);
  }
}

constexpr Lexer::LexMethodResult Lexer::LexQuotation(CharVectorCIter it) {
  using namespace grammar::characters;
  CharVectorCIter begin = it;
  if (Get(it) == kApostrophe::value) {
    Advance(it);

    while (
        !(Get(it) == kApostrophe::value && Peek(it, -1) != kBacklash::value)) {
      Advance(it);
    }
    Advance(it);

    // Check for byte literal
    if (Get(it) == u8'c') {
      Advance(it);
      return SuccessResult(eTk::kByteLiteral, begin, it);
    } else
      return SuccessResult(eTk::kStringLiteral, begin, it);
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
    return SuccessResult(eTk::kNewline, begin, it);
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
    return SuccessResult(eTk::kWhitespace, begin, it);
  } else {
    return NoneResult(begin);
  }
}

constexpr Lexer::LexMethodResult Lexer::LexEof(CharVectorCIter it) {
  CharVectorCIter begin = it;
  if (Get(it) == kEofChar) {
    Advance(it);
    return SuccessResult(eTk::kEof, begin, it);
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
      return SuccessResult(eTk::kBoolLiteral, begin, it);
    } else if (Get(it) == '0' && Peek(it, 1) == 'b') {
      Advance(it, 2);
      return SuccessResult(eTk::kBoolLiteral, begin, it);
    }

    while (cand_char::IsNumeric(Get(it))) {
      Advance(it);
    }

    // Special case for unsigned literal (overflow is handled by the parser)
    if (Get(it) == 'u') {
      Advance(it);
      return SuccessResult(eTk::kUnsignedLiteral, begin, it);
    }

    // Special case for byte literal(overflow is handled by the parser)
    if (Get(it) == 'c') {
      Advance(it);
      return SuccessResult(eTk::kByteLiteral, begin, it);
    }

    // If number is followed by elipsis. Return the number.
    if (FindForward(it, scopes::kEllipsis::value)) {
      return SuccessResult(eTk::kNumberLiteral, begin, it);
    }

    // Else process a floating literal.
    if (Get(it) == characters::kPeriod::value) {
      Advance(it);
      while (cand_char::IsNumeric(Get(it))) {
        Advance(it);
      }
      return SuccessResult(eTk::kDoubleLiteral, begin, it);
    }

    return SuccessResult(eTk::kNumberLiteral, begin, it);
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
    return SuccessResult(eTk::kIdentifier, begin, it);
  } else {
    return NoneResult(begin);
  }
}

constexpr Lexer::LexMethodResult Lexer::LexDirective(CharVectorCIter it) {
  CharVectorCIter beg = it;
  if (Get(it) == grammar::characters::kHash::value) {
    Advance(it);
    while (NotAtEof(it) && cand_char::IsAlnumus(it)) {
      Advance(it);
    }

    LexMethodResult temp_result =
        FailureResult(beg, "Invalid directive:" + std::string(beg, it));
    std::apply(
        [&](const auto&... traits) -> void {
          [](...) {}((
              [&]<typename T>(const T& x) {
                if (FindForward(beg, std::decay_t<T>::literal)) {
                  temp_result = SuccessResult(std::decay_t<T>::type, beg, it);
                }
              }(std::forward<decltype(traits)>(traits)),
              false)...);
        },
        tk_traits::kAllDirectivesTuple);
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
    // What is this black magic??
    // https://stackoverflow.com/questions/43541241/iterate-over-tuple-elements-with-stdapply
    LexMethodResult temp_result = NoneResult(beg);
    std::apply(
        [&](const auto&... traits) -> void {
          [](...) {}((
              [&]<typename T>(const T& x) {
                if (FindForward(beg, std::decay_t<T>::literal)) {
                  temp_result = SuccessResult(std::decay_t<T>::type, beg, it);
                }
              }(std::forward<decltype(traits)>(traits)),
              false)...);
        },
        tk_traits::kAllKeywordsTuple);
    return temp_result;

  } else {
    return NoneResult(beg);
  }
}

constexpr Lexer::LexMethodResult Lexer::LexOperator(CharVectorCIter it) {
  using namespace grammar::characters;
  CharVectorCIter begin = it;
  if (Get(it) == kEq::value) {
    if (Peek(it, 1) == kEq::value) {
      Advance(it, 2);
      return SuccessResult(eTk::kEqual, begin, it);
    } else {
      Advance(it);
      return SuccessResult(eTk::kSimpleAssignment, begin, it);
    }
  } else if (Get(it) == kAdd::value) {
    if (Peek(it, 1) == kAdd::value) {
      Advance(it, 2);
      return SuccessResult(eTk::kIncrement, begin, it);
    } else if (Peek(it, 1) == kEq::value) {
      Advance(it, 2);
      return SuccessResult(eTk::kAdditionAssignment, begin, it);
    } else {
      Advance(it);
      return SuccessResult(eTk::kAddition, begin, it);
    }
  } else if (Get(it) == kSub::value) {
    if (Peek(it, 1) == kSub::value) {
      Advance(it, 2);
      return SuccessResult(eTk::kDecrement, begin, it);
    } else if (Peek(it, 1) == kEq::value) {
      Advance(it, 2);
      return SuccessResult(eTk::kSubtractionAssignment, begin, it);
    } else {
      Advance(it);
      return SuccessResult(eTk::kSubtraction, begin, it);
    }
  } else if (Get(it) == kMul::value) {
    if (Peek(it, 1) == kEq::value) {
      Advance(it, 2);
      return SuccessResult(eTk::kMultiplicationAssignment, begin, it);
    } else {
      Advance(it);
      return SuccessResult(eTk::kMultiplication, begin, it);
    }
  } else if (Get(it) == kDiv::value) {
    if (Peek(it, 1) == kEq::value) {
      Advance(it, 2);
      return SuccessResult(eTk::kDivisionAssignment, begin, it);
    } else {
      Advance(it);
      return SuccessResult(eTk::kDivision, begin, it);
    }
  } else if (Get(it) == kMod::value) {
    if (Peek(it, 1) == kEq::value) {
      Advance(it, 2);
      return SuccessResult(eTk::kRemainderAssignment, begin, it);
    } else {
      Advance(it);
      return SuccessResult(eTk::kRemainder, begin, it);
    }
  } else if (Get(it) == kAnd::value) {
    if (Peek(it, 1) == kEq::value) {
      Advance(it, 2);
      return SuccessResult(eTk::kBitwiseAndAssignment, begin, it);
    } else if (Peek(it, 1) == kAnd::value) {
      Advance(it, 2);
      return SuccessResult(eTk::kLogicalAnd, begin, it);
    } else {
      Advance(it);
      return SuccessResult(eTk::kBitwiseAnd, begin, it);
    }
  } else if (Get(it) == kOr::value) {
    if (Peek(it, 1) == kEq::value) {
      Advance(it, 2);
      return SuccessResult(eTk::kBitwiseOrAssignment, begin, it);
    } else if (Peek(it, 1) == kOr::value) {
      Advance(it, 2);
      return SuccessResult(eTk::kLogicalOr, begin, it);
    } else {
      Advance(it);
      return SuccessResult(eTk::kBitwiseOr, begin, it);
    }
  } else if (Get(it) == kXor::value) {
    if (Peek(it, 1) == kEq::value) {
      Advance(it, 2);
      return SuccessResult(eTk::kBitwiseXorAssignment, begin, it);
    } else {
      Advance(it);
      return SuccessResult(eTk::kBitwiseXor, begin, it);
    }
  } else if (Get(it) == kLsh::value) {
    if (Peek(it, 1) == kLsh::value) {
      if (Peek(it, 2) == kEq::value) {
        Advance(it, 3);
        return SuccessResult(eTk::kLeftShiftAssignment, begin, it);
      } else {
        Advance(it, 2);
        return SuccessResult(eTk::kBitwiseLeftShift, begin, it);
      }
    } else if (Peek(it, 1) == kEq::value) {
      if (Peek(it, 2) == kRsh::value) {
        Advance(it, 3);
        return SuccessResult(eTk::kThreeWayComparison, begin, it);
      }
      Advance(it, 2);
      return SuccessResult(eTk::kLessThanOrEqual, begin, it);
    } else {
      Advance(it);
      return SuccessResult(eTk::kLessThan, begin, it);
    }
  } else if (Get(it) == kRsh::value) {
    if (Peek(it, 1) == kRsh::value) {
      if (Peek(it, 2) == kEq::value) {
        Advance(it, 3);
        return SuccessResult(eTk::kRightShiftAssignment, begin, it);
      } else {
        Advance(it, 2);
        return SuccessResult(eTk::kBitwiseRightShift, begin, it);
      }
    } else if (Peek(it, 1) == kEq::value) {
      Advance(it, 2);
      return SuccessResult(eTk::kGreaterThanOrEqual, begin, it);
    } else {
      Advance(it);
      return SuccessResult(eTk::kGreaterThan, begin, it);
    }
  } else if (Get(it) == kNot::value) {
    if (Peek(it, 1) == kEq::value) {
      Advance(it, 2);
      return SuccessResult(eTk::kNotEqual, begin, it);
    } else {
      Advance(it);
      return SuccessResult(eTk::kNegation, begin, it);
    }
  } else if (Get(it) == kTilde::value) {
    Advance(it);
    return SuccessResult(eTk::kBitwiseNot, begin, it);
  } else if (Get(it) == kCommercialAt::value) {
    Advance(it);
    return SuccessResult(eTk::kCommercialAt, begin, it);
  } else {
    return NoneResult(begin);
  }
}

constexpr Lexer::LexMethodResult Lexer::LexScopes(CharVectorCIter it) {
  using namespace grammar::characters;
  CharVectorCIter begin = it;
  if (Get(it) == kLparen::value) {
    Advance(it);
    return SuccessResult(eTk::kOpenParen, begin, it);
  } else if (Get(it) == kRparen::value) {
    Advance(it);
    return SuccessResult(eTk::kCloseParen, begin, it);
  } else if (Get(it) == kLbrace::value) {
    Advance(it);
    return SuccessResult(eTk::kOpenBrace, begin, it);
  } else if (Get(it) == kRbrace::value) {
    Advance(it);
    return SuccessResult(eTk::kCloseBrace, begin, it);
  } else if (Get(it) == kLbracket::value) {
    Advance(it);
    return SuccessResult(eTk::kOpenBracket, begin, it);
  } else if (Get(it) == kRbracket::value) {
    Advance(it);
    return SuccessResult(eTk::kCloseBracket, begin, it);
  } else {
    return NoneResult(begin);
  }
}

constexpr Lexer::LexMethodResult Lexer::LexSemicolon(CharVectorCIter it) {
  CharVectorCIter begin = it;
  if (Get(it) == grammar::characters::kSemicolon::value) {
    Advance(it);
    return SuccessResult(eTk::kSemicolon, begin, it);
  } else {
    return NoneResult(begin);
  }
}

constexpr Lexer::LexMethodResult Lexer::LexColon(CharVectorCIter it) {
  CharVectorCIter begin = it;
  if (Get(it) == grammar::characters::kColon::value) {
    Advance(it);
    if (Get(it) == grammar::characters::kColon::value) {
      Advance(it);
      return SuccessResult(eTk::kDoubleColon, begin, it);
    }
    return SuccessResult(eTk::kColon, begin, it);
  } else {
    return NoneResult(begin);
  }
}

constexpr Lexer::LexMethodResult Lexer::LexComma(CharVectorCIter it) {
  CharVectorCIter begin = it;
  if (Get(it) == grammar::characters::kComma::value) {
    Advance(it);
    return SuccessResult(eTk::kComma, begin, it);
  } else {
    return NoneResult(begin);
  }
}

constexpr Lexer::LexMethodResult Lexer::LexPeriod(CharVectorCIter it) {
  CharVectorCIter begin = it;
  if (FindForward(it, grammar::scopes::kEllipsis::value)) {
    Advance(it, 3);
    return SuccessResult(eTk::kEllipsis, begin, it);
  } else if (Get(it) == grammar::characters::kPeriod::value) {
    Advance(it);
    return SuccessResult(eTk::kPeriod, begin, it);
  } else {
    return NoneResult(begin);
  }
}

// Main tokenizer method
constexpr Lexer::LexerResult Lexer::Lex() {
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
      return Expected<bool>::Failure(lex_result.Error());
    }
    Tk result_token = lex_result.Value();
    CharVectorCIter result_end = lex_result.Always();

    if (result_token.Type() == eTk::kNone) {  // No match, try next lexer
      return Expected<bool>::Success(false);
    }

    else {  // Lexing was successful
      // Update position based on the number of characters consumed
      current_line += std::count(it, result_end, '\n');

      // Find the last newline before the current character
      CharVectorCIter last_newline =
          std::find(std::reverse_iterator(result_end),
                    std::reverse_iterator(it), '\n')
              .base();
      // If there is no newline before the current character, use the start of
      // the string
      if (last_newline == End()) {
        last_newline = Begin();
      }

      // Calculate the character index within the line
      current_col = static_cast<std::size_t>(std::distance(
          last_newline, result_end));  // Will always be a positive number

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
      if (!lex_result.Valid()) {  // Error inside one of the lexers
        return LexerResult::Failure(
            compiler_error::tokenizer::xLexerSyntaxError(
                current_line, current_col, Get(it), lex_result.Error()));
      } else if (lex_result.Value()) {
        // Note: The iterator 'it' is advanced in perform_lex lambda.
        match = true;
        break;  // Exit for-loop
      }
    }
    // None of the lexers matched, report an error
    // Update position based on the number of characters consumed
    if (!match) {
      // Find the current line - count newlines from begin to iter.
      current_line += std::count(Begin(), it, '\n');

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
      current_col = static_cast<std::size_t>(std::distance(last_newline, it));

      return LexerResult::Failure(compiler_error::tokenizer::xInvalidChar(
          current_line, current_col, Get(it)));
    }
  }  // end while

  // Remove redundant tokens after lexing
  // Note: "i" is used instead of "it" to avoid ambiguity with the iterator
  // above.
  TkVector sanitized = [&]() constexpr {
    TkVector new_output;
    for (auto i = output_tokens.cbegin(); i != output_tokens.cend(); ++i) {
      const std::initializer_list<eTk> REDUNDANT_TOKEN_KINDS{
          eTk::kWhitespace, eTk::kLineComment, eTk::kBlockComment,
          eTk::kNewline};

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

//---------------------------------------------------------------------------//
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
//---------------------------------------------------------------------------//
// Author(s): Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.acpp.dev
//---------------------------------------------------------------------------//
// Project: C& Programming Language Environment
// Directory: cand-official-compiler
// File: caoco_lexer.h
//---------------------------------------------------------------------------//
#endif HEADER_GUARD_CALE_CAND_OFFICIAL_COMPILER_CAOCO_LEXER_H
//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
