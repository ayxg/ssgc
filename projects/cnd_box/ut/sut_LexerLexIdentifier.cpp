///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file Static Unit Test : LexerLexIdentifier
/// @ingroup cnd_static_ut
/// @brief Static asserts for cnd::trtools::Lexer::LexIdentifier method.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup cnd_static_ut
/// @{

#pragma once
// clang-format off
#include "use_ccapi.hpp"
#include "use_corevals.hpp"
#include "use_clmsg.hpp"
#include "cldata/tk.hpp"
#include "trtools/lexer.hpp"
// clang-format on

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Enable or disable static asserts for cnd::trtools::Lexer::LexIdentifier method.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define CND_STATIC_UNIT_TEST_LexerLexIdentifier false
#if CND_STATIC_UNIT_TEST_LexerLexIdentifier || CND_ENABLE_STATIC_TEST_MODULE_Lexer
namespace cnd::trtools {
static_assert(Lexer().LexIdentifier("abc").value().processed_tk.type_ == eTk::kIdent);
static_assert(Lexer().LexIdentifier("abc").value_or(Lexer::LexerCursorT{}).processed_tk.literal_ == StrView("abc"));
static_assert(Lexer().LexIdentifier("abc").value_or(Lexer::LexerCursorT{"<non-empty-token>"}).read_head == StrView(""));

static_assert(Lexer().LexIdentifier("def").value().processed_tk.type_ == eTk::kKwDef);
static_assert(Lexer().LexIdentifier("def").value_or(Lexer::LexerCursorT{}).processed_tk.literal_ == StrView("def"));
static_assert(Lexer().LexIdentifier("def").value_or(Lexer::LexerCursorT{"<non-empty-token>"}).read_head == StrView(""));

static_assert(Lexer().LexIdentifier("abc 123").value().processed_tk.type_ == eTk::kIdent);
static_assert(Lexer().LexIdentifier("abc 123").value_or(Lexer::LexerCursorT{}).processed_tk.literal_ == StrView("abc"));
static_assert(Lexer().LexIdentifier("abc 123").value_or(Lexer::LexerCursorT{"<non-empty-token>"}).read_head ==
              StrView(" 123"));

static_assert(Lexer().LexIdentifier("def 123").value().processed_tk.type_ == eTk::kKwDef);
static_assert(Lexer().LexIdentifier("def 123").value_or(Lexer::LexerCursorT{}).processed_tk.literal_ == StrView("def"));
static_assert(Lexer().LexIdentifier("def 123").value_or(Lexer::LexerCursorT{"<non-empty-token>"}).read_head ==
              StrView(" 123"));

static_assert(
    [] {  // We can validate the keyword by testing that every IsTkKeyword has a corresponding GetTkFromKeyword.
      for (std::size_t i = 0; i < static_cast<std::size_t>(cnd::eTk::COUNT); i++)
        if (cnd::IsTkKeyword(static_cast<cnd::eTk>(i)) &&  // is it a keyword?
                                                           // does it have a corresponding token?
            cnd::GetTkFromKeyword(cnd::GetTkSymbol(static_cast<cnd::eTk>(i))) != cnd::eTk::kNONE)
          return true;
      // else you forgot to implement GetTkFromKeyword and GetTkSymbol for a new keyword added to IsTkKeyword.
      return false;
    }());
}  // namespace cnd::trtools
#endif  // CND_STATIC_UNIT_TEST_LexerLexIdentifier

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