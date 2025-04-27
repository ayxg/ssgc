///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the Apache License, Version 2.0(the "License");
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup
/// @brief
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup cnd_compiler
/// @{
#include "cnd_lexer.hpp"

// clang-format off
#include "cnd_implicit.hpp"
#include "cnd_constdef.hpp"
#include "cnd_clerr.hpp"
#include "cnd_src_traits.hpp"
#include "cnd_src_lines.hpp"
#include "cnd_tk.hpp"
// clang-format on

namespace cnd {
namespace lexer {

ClRes<Vec<Tk>> Lex(const SrcLines& cand_src_lines) {
  using namespace clerr;
  using namespace src_traits;
  using enum eClErr;
  ConstLineIter it{cand_src_lines.cbegin()};
  const ConstLineIter src_end = cand_src_lines.cend();
  Vec<Tk> out{};
  while (it != src_end) {
    SrcChar c = *it;
    LexRes res_buff;  // Intentionally uninitialized.
    if (IsNewline(c))
      res_buff = LexNewline(it);
    else if (IsSpace(c))
      res_buff = LexWhitespace(it);
    else if (IsAlphaUnderscore(c))
      res_buff = LexIdentifier(it);
    else if (IsNumeric(c))
      res_buff = LexNumber(it);
    else if (IsPunctuator(c))
      res_buff = LexPunctuator(it);
    else
      return ClFail(MakeClErr<kUnknownSrcChar>(c));  // Unknown init character.

    if (!res_buff) return ClFail(res_buff.error());
    out.push_back(res_buff.value().res);
    it = res_buff.value().offset;
  }
  return out;
}

LexRes LexNewline(SrcLinesConstIter c) {
  using namespace clerr;
  using namespace src_traits;
  using enum eClErr;
  CND_DEBUG_ASSERT(IsNewline(*c));
  auto c_next = c.Next();
  return {{c_next, Tk(eTk::kNewline, c, c_next)}};
}
LexRes LexWhitespace(ConstLineIter c) {
  using namespace clerr;
  using namespace src_traits;
  using enum eClErr;
  CND_DEBUG_ASSERT(IsSpace(*c));
  auto c_next = c.Next();
  return {{c_next, Tk(eTk::kWhitespace, c, c_next)}};
};
LexRes LexIdentifier(ConstLineIter c) {
  using namespace clerr;
  using namespace src_traits;
  using enum eClErr;
  CND_DEBUG_ASSERT(IsAlphaUnderscore(*c));
  auto id_it = c;
  while (IsAlnumus(id_it.CheckedGet())) ++id_it;

  return {{id_it, Tk(eTk::kIdent, c, id_it)}};
}
LexRes LexNumber(ConstLineIter c) {
  using namespace clerr;
  using namespace src_traits;
  using enum eClErr;
  CND_DEBUG_ASSERT(IsNumeric(*c));
  auto begin = c;

  if (c.Next().CheckedGet() == 'b')
    if (*c == '1' || *c == '0') return {{c.Next(2), Tk(eTk::kLitBool, c, c.Next(2))}};

  while (IsNumeric(c.CheckedGet())) ++c;  // Consume whole number digits.

  // Case for unsigned literal (overflow is handled by the parser)
  if (c.CheckedGet() == 'u') return {{c.Next(), Tk(eTk::kLitUint, begin, c.Next())}};

  // Case for byte literal(overflow is handled by the parser)
  if (c.CheckedGet() == 'c') return {{c.Next(), Tk(eTk::kLitByte, begin, c.Next())}};

  // If number is followed by elipsis. Return the number.
  if (c.CheckedGet() == '.' && c.Next().CheckedGet() == '.' && c.Next(2).CheckedGet() == '.')
    return {{c.Next(), Tk(eTk::kLitInt, begin, c.Next())}};

  // Process a floating literal if decimal found.
  if (c.CheckedGet() == '.') {
    ++c;
    while (IsNumeric(c.CheckedGet())) ++c;  // Consume the fractional digits.
    return {{c, Tk(eTk::kLitReal, begin, c)}};
  }

  return {{c, Tk(eTk::kLitInt, begin, c)}};
}
LexRes LexPunctuator(ConstLineIter c) {
  using namespace clerr;
  using namespace src_traits;
  using enum eClErr;
  CND_DEBUG_ASSERT(IsPunctuator(*c));
  // Switch over all valid initial punctuators which may form a symbol token.
  // The maximum length of any punctuator is 3.
  auto beg_c = c;
  switch (*c) {
    case '=':
      switch (*c.Next()) {
        case '=':
          ++c;
          return {{eTk::kEq, beg_c, c}};
        default:
          return {{eTk::kAssign, beg_c, c}};
      }

    case '+':
      switch (*c.Next()) {
        case '+':
          ++c;
          return {{eTk::kInc, beg_c, c}};
        case '=':
          ++c;
          return {{eTk::kAddAssign, beg_c, c}};
        default:
          return {{eTk::kAdd, beg_c, c}};
      }

    case '-':
      switch (*c.Next()) {
        case '-':
          ++c;
          return {{eTk::kDec, beg_c, c}};
        case '=':
          ++c;
          return {{eTk::kSubAssign, beg_c, c}};
        default:
          return {{eTk::kSub, beg_c, c}};
      }

    case '*':
      switch (*c.Next()) {
        case '=':
          ++c;
          return {{eTk::kMulAssign, beg_c, c}};
        default:
          return {{eTk::kMul, beg_c, c}};
      }

    case '/':
      switch (*c.Next()) {
        case '=':
          ++c;
          return {{eTk::kDivAssign, beg_c, c}};
        default:
          return {{eTk::kDiv, beg_c, c}};
      }

    case '%':
      switch (*c.Next()) {
        case '=':
          ++c;
          return {{eTk::kModAssign, beg_c, c}};
        default:
          return {{eTk::kMod, beg_c, c}};
      }

    case '&':
      switch (*c.Next()) {
        case '=':
          ++c;
          return {{eTk::kAndAssign, beg_c, c}};
        case '&':
          ++c;
          return {{eTk::kAnd, beg_c, c}};
        default:
          return {{eTk::kBand, beg_c, c}};
      }

    case '|':
      switch (*c.Next()) {
        case '=':
          ++c;
          return {{eTk::kOrAssign, beg_c, c}};
        case '|':
          ++c;
          return {{eTk::kOr, beg_c, c}};
        default:
          return {{eTk::kBor, beg_c, c}};
      }

    case '^':
      switch (*c.Next()) {
        case '=':
          ++c;
          return {{eTk::kXorAssign, beg_c, c}};
        default:
          return {{eTk::kXor, beg_c, c}};
      }

    case '<':
      switch (*c.Next()) {
        case '<':
          switch (*c.Next(2)) {
            case '=':
              ++ ++c;
              return {{eTk::kLshAssign, beg_c, c}};
            default:
              ++c;
              return {{eTk::kLsh, beg_c, c}};
          }
        case '=':
          switch (*c.Next()) {
            case '>':
              ++ ++c;
              return {{eTk::kSpaceship, beg_c, c}};
            default:
              ++c;
              return {{eTk::kLte, beg_c, c}};
          }
        default:
          ++c;
          return {{eTk::kLt, beg_c, c}};
      }

    case '>':
      switch (*c.Next()) {
        case '>':
          switch (*c.Next()) {
            case '=':
              ++ ++c;
              return {{eTk::kRshAssign, beg_c, c}};
            default:
              ++c;
              return {{eTk::kRsh, beg_c, c}};
          }
        case '=':
          ++c;
          return {{eTk::kGte, beg_c, c}};
        default:
          return {{eTk::kGt, beg_c, c}};
      }

    case '!':
      switch (*c.Next()) {
        case '=':
          ++c;
          return {{eTk::kNeq, beg_c, c}};
        default:
          return {{eTk::kNot, beg_c, c}};
      }

    case '~':
      return {{eTk::kBnot, beg_c, c}};
    case '@':
      return {{eTk::kCommercialAt, beg_c, c}};
    case '#':
      return {{eTk::kHash, beg_c, c}};
    case '$':
      return {{eTk::kDollar, beg_c, c}};
    case '?':
      return {{eTk::kQuestion, beg_c, c}};
    case ':':
      return {{eTk::kColon, beg_c, c}};
    case ';':
      return {{eTk::kSemicolon, beg_c, c.Next()}};
    case ',':
      return {{eTk::kComma, beg_c, c}};
    case '.':
      return {{eTk::kPeriod, beg_c, c}};
    case '(':
      return {{eTk::kLParen, beg_c, c}};
    case ')':
      return {{eTk::kRParen, beg_c, c}};
    case '[':
      return {{eTk::kLBracket, beg_c, c}};
    case ']':
      return {{eTk::kRBracket, beg_c, c}};
    case '{':
      return {{eTk::kLBrace, beg_c, c}};
    case '}':
      return {{eTk::kRBrace, beg_c, c}};
    // case '\\':
    //   switch (*c.Next()) {
    //     case '\`':
    //       return LexMultilineComment(c);
    //     default:
    //       return {{eTk::kBacklash, beg_c, c}};
    //   }
    // case '\'':
    //   return LexLitChar(c);
    // case '\"':
    //   return LexLitStr(c);
    // case '`':
    //   return LexLineComment(c);
    default:
      assert(false && "lexer::LexPunctuator implementation failure in switch case.");
      __assume(false);
  }
}

}  // namespace lexer

}  // namespace cnd

/// @} // end of cnd_compiler

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
//
// Licensed under the Apache License, Version 2.0(the "License");
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
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////