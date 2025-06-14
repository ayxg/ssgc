///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file Static Unit Test : LexerLexEscapedCharSequence
/// @ingroup cnd_static_ut
/// @brief Static asserts for cnd::trtools::Lexer::LexEscapedCharSequence method.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup cnd_static_ut
/// @{

#pragma once
// clang-format off
#include "use_ccapi.hpp"
#include "use_corevals.hpp"
#include "use_clmsg.hpp"
#include "cldata/tk.hpp"
#include "trtools/Lexer.hpp"
// clang-format on

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Enable or disable static asserts for cnd::trtools::Lexer::LexEscapedCharSequence method.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define CND_STATIC_UNIT_TEST_LexerLexEscapedCharSequence false
#if CND_STATIC_UNIT_TEST_LexerLexEscapedCharSequence || CND_ENABLE_STATIC_TEST_MODULE_Lexer
namespace cnd::trtools {

static_assert(cnd::trtools::Lexer().LexEscapedCharSequence("\"aa\"").value().processed_tk.type_ == cnd::eTk::kLitCstr);
static_assert(cnd::trtools::Lexer()
                  .LexEscapedCharSequence("\"aa\"")
                  .value_or(cnd::trtools::Lexer::LexerCursorT{})
                  .processed_tk.literal_ ==
    StrView("\"aa\""));
static_assert(cnd::trtools::Lexer()
                  .LexEscapedCharSequence("\"aa\"")
                  .value_or(cnd::trtools::Lexer::LexerCursorT{"<non-empty-token>"})
                  .read_head == StrView(""));

}  // namespace cnd::trtools

#endif  // CND_STATIC_UNIT_TEST_LexerLexEscapedCharSequence

/// @} // end of cnd_static_ut

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