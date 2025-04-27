///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file Static Unit Test : LexerLexPunctuator
/// @ingroup cnd_static_ut
/// @brief Static asserts for cnd::trtools::Lexer::LexPunctuator method.
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
/// Enable or disable static asserts for cnd::trtools::Lexer::LexPunctuator method.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define CND_STATIC_UNIT_TEST_LexerLexPunctuator false
#if CND_STATIC_UNIT_TEST_LexerLexPunctuator || CND_ENABLE_STATIC_TEST_MODULE_Lexer
namespace cnd::trtools {

// '='
static_assert(Lexer().LexPunctuator("=").value().processed_tk.type_ == cnd::eTk::kAssign);
static_assert(Lexer().LexPunctuator("=").value_or(Lexer::LexerCursorT{}).processed_tk.literal_ ==
              std::string_view("="));
static_assert(Lexer().LexPunctuator("= 123").value_or(Lexer::LexerCursorT{"<non-empty-token>"}).read_head ==
              std::string_view(" 123"));

// '=='
static_assert(Lexer().LexPunctuator("==").value().processed_tk.type_ == cnd::eTk::kEq);
static_assert(Lexer().LexPunctuator("==").value_or(Lexer::LexerCursorT{}).processed_tk.literal_ ==
              std::string_view("=="));
static_assert(Lexer().LexPunctuator("===").value_or(Lexer::LexerCursorT{}).processed_tk.literal_ ==
              std::string_view("=="));
static_assert(Lexer().LexPunctuator("== =").value_or(Lexer::LexerCursorT{"<non-empty-token>"}).read_head ==
              std::string_view(" ="));
static_assert(Lexer().LexPunctuator("== 123").value_or(Lexer::LexerCursorT{"<non-empty-token>"}).read_head ==
              std::string_view(" 123"));

}  // namespace cnd::trtools

#endif  // CND_STATIC_UNIT_TEST_LexerLexPunctuator

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