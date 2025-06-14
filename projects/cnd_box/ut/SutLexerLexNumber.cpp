///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the Apache License, Version 2.0(the "License");
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
#include "trtools/Lexer.hpp"
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
// Licensed under the Apache License, Version 2.0(the "License"); you may not use this file except in compliance with
// the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on
// an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////