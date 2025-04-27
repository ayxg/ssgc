///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file Static Unit Test : LexerLexNumber
/// @ingroup cnd_static_ut
/// @brief Static asserts for cnd::trtools::Lexer::LexNumber method.
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
/// Enable or disable static asserts for cnd::trtools::Lexer::LexNumber method.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define CND_STATIC_UNIT_TEST_LexerLexNumber false
#if CND_STATIC_UNIT_TEST_LexerLexNumber || CND_ENABLE_STATIC_TEST_MODULE_Lexer
namespace cnd::trtools {

// asserts should trigger the rare early return case when eof is right after decimal digits.
static_assert(Lexer().LexNumber("123").value().processed_tk.type_ == cnd::eTk::kLitInt);
static_assert(Lexer().LexNumber("123").value_or(Lexer::LexerCursorT{}).processed_tk.literal_ == StrView("123"));
static_assert(Lexer().LexNumber("123").value_or(Lexer::LexerCursorT{"<non-empty-token>"}).read_head == StrView(""));
static_assert(Lexer().LexNumber("123").value().processed_tk.end_col_ == 3);
static_assert(Lexer().LexNumber("123").value().processed_tk.beg_col_ == 0);

// asserts should trigger literal byte case.
static_assert(Lexer().LexNumber("1b").value().processed_tk.type_ == cnd::eTk::kLitBool);
static_assert(Lexer().LexNumber("1b").value_or(Lexer::LexerCursorT{}).processed_tk.literal_ == StrView("1b"));
static_assert(Lexer().LexNumber("1b").value_or(Lexer::LexerCursorT{"<non-empty-token>"}).read_head == StrView(""));
static_assert(Lexer().LexNumber("0b").value().processed_tk.type_ == cnd::eTk::kLitBool);
static_assert(Lexer().LexNumber("0b").value_or(Lexer::LexerCursorT{}).processed_tk.literal_ == StrView("0b"));
static_assert(Lexer().LexNumber("0b").value_or(Lexer::LexerCursorT{"<non-empty-token>"}).read_head == StrView(""));

// asserts should trigger the unsigned literal case.
static_assert(Lexer().LexNumber("123u").value().processed_tk.type_ == cnd::eTk::kLitUint);
static_assert(Lexer().LexNumber("123u").value_or(Lexer::LexerCursorT{}).processed_tk.literal_ == StrView("123u"));
static_assert(Lexer().LexNumber("123u").value_or(Lexer::LexerCursorT{"<non-empty-token>"}).read_head == StrView(""));

// asserts should trigger the unsigned byte literal case.
static_assert(Lexer().LexNumber("123c").value().processed_tk.type_ == cnd::eTk::kLitByte);
static_assert(Lexer().LexNumber("123c").value_or(Lexer::LexerCursorT{}).processed_tk.literal_ == StrView("123c"));
static_assert(Lexer().LexNumber("123c").value_or(Lexer::LexerCursorT{"<non-empty-token>"}).read_head == StrView(""));

// asserts should trigger the elipsis case.
static_assert(Lexer().LexNumber("123...").value().processed_tk.type_ == cnd::eTk::kLitInt);
static_assert(Lexer().LexNumber("123...").value_or(Lexer::LexerCursorT{}).processed_tk.literal_ == StrView("123"));
static_assert(Lexer().LexNumber("123...").value_or(Lexer::LexerCursorT{}).read_head == StrView("..."));

// asserts should trigger the double literal case.
// @TODO: for now return kLitReal but rememeber to add eTk::kLitDouble enum entry!
static_assert(Lexer().LexNumber("123.321").value().processed_tk.type_ == cnd::eTk::kLitReal);
static_assert(Lexer().LexNumber("123.321").value_or(Lexer::LexerCursorT{}).processed_tk.literal_ == StrView("123.321"));
static_assert(Lexer().LexNumber("123.321").value_or(Lexer::LexerCursorT{}).read_head == StrView(""));

// asserts should trigger the float literal case.
static_assert(Lexer().LexNumber("123.321f").value().processed_tk.type_ == cnd::eTk::kLitReal);
static_assert(Lexer().LexNumber("123.321f").value_or(Lexer::LexerCursorT{}).processed_tk.literal_ ==
              StrView("123.321f"));
static_assert(Lexer().LexNumber("123.321f").value_or(Lexer::LexerCursorT{}).read_head == StrView(""));

// asserts should trigger the final default int literal case.
static_assert(Lexer().LexNumber("123abc").value().processed_tk.type_ == cnd::eTk::kLitInt);
static_assert(Lexer().LexNumber("123abc").value_or(Lexer::LexerCursorT{}).processed_tk.literal_ == StrView("123"));
static_assert(Lexer().LexNumber("123abc").value_or(Lexer::LexerCursorT{}).read_head == StrView("abc"));

}  // namespace cnd::trtools
#endif  // CND_STATIC_UNIT_TEST_LexerLexNumber

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