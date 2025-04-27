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

/// @addtogroup cnd_compiler_data
/// @{
#pragma once
// clang-format off
#include "cnd_implicit.hpp"
#include "cnd_constdef.hpp"
#include "cnd_clerr.hpp"
#include "cnd_src_traits.hpp"
#include "cnd_src_lines.hpp"
#include "cnd_tk.hpp"
// clang-format on

/// Tokenizer(Lexer)
namespace cnd {
namespace lexer {
using clerr::ClErr;
using clerr::ClFail;
using clerr::ClRes;
using clerr::MakeClErr;
using ConstLineIter = SrcLines::const_iterator;

struct OffsetLexResult {
  SrcLinesConstIter offset;
  Tk res;

  OffsetLexResult(SrcLinesConstIter offset, Tk res) : offset(offset), res(res) {}
  OffsetLexResult() = default;
  OffsetLexResult(const OffsetLexResult&) = default;
  OffsetLexResult(OffsetLexResult&&) = default;
  OffsetLexResult& operator=(const OffsetLexResult&) = default;
  OffsetLexResult& operator=(OffsetLexResult&&) = default;

  OffsetLexResult(eTk etk, SrcLinesConstIter beg, SrcLinesConstIter end) : offset(end), res(etk, beg, end) {}
};

using LexRes = ClRes<OffsetLexResult>;

LexRes LexNewline(ConstLineIter crsr);
LexRes LexWhitespace(ConstLineIter crsr);
LexRes LexIdentifier(ConstLineIter crsr);
LexRes LexNumber(ConstLineIter crsr);
LexRes LexPunctuator(ConstLineIter crsr);

LexRes LexSymbol(ConstLineIter crsr);
LexRes LexBacktick(ConstLineIter crsr);
LexRes LexSolidus(ConstLineIter crsr);
LexRes LexQuotation(ConstLineIter crsr);
LexRes LexWhitespace(ConstLineIter crsr);
LexRes LexEof(ConstLineIter crsr);
LexRes LexNumber(ConstLineIter crsr);
LexRes LexIdentifier(ConstLineIter crsr);
LexRes LexDirective(ConstLineIter crsr);
LexRes LexKeyword(ConstLineIter crsr);
LexRes LexOperator(ConstLineIter crsr);
LexRes LexScopes(ConstLineIter crsr);
LexRes LexSemicolon(ConstLineIter crsr);
LexRes LexColon(ConstLineIter crsr);
LexRes LexComma(ConstLineIter crsr);
LexRes LexPeriod(ConstLineIter crsr);
ClRes<Vec<Tk>> Lex(const SrcLines& cand_src_lines);

}  // namespace lexer
}  // namespace cnd

/// @} // end of cnd_compiler_data

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