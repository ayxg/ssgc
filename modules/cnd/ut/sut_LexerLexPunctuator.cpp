///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the Apache License, Version 2.0(the "License");
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
// Licensed under the Apache License, Version 2.0(the "License"); you may not use this file except in compliance with
// the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on
// an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////