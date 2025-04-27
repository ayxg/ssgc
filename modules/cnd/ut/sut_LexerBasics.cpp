///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file Static Unit Test : LexerBasics
/// @ingroup cnd_static_ut
/// @brief General validating static asserts for cnd::trtools::Lexer.
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
/// Enable or disable static asserts for cnd::trtools::Lexer::LexEscapedCharSequence method.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define CND_STATIC_UNIT_TEST_LexerBasics false
#if CND_STATIC_UNIT_TEST_LexerBasics || CND_ENABLE_STATIC_TEST_MODULE_Lexer
namespace cnd::trtools {

static_assert(cnd::trtools::Lexer().Lex(" ").value()[0].Type() == cnd::eTk::kWhitespace);
static_assert(cnd::trtools::Lexer::Lex(" def int @foo: 1;").value()[1].Type() == cnd::eTk::kKwDef);

constexpr auto tokens = [] {
  constexpr auto buf_size = cnd::trtools::Lexer::Lex(" def int @foo: 1;").value().size();
  auto buf = cnd::trtools::Lexer::Lex(" def int @foo: 1;");
  std::array<cnd::Tk, buf_size> out;
  std::copy(buf.value().data(), buf.value().data() + buf_size, out.begin());
  return out;
}();

}  // namespace cnd::trtools

#endif  // end of CND_STATIC_UNIT_TEST_LexerBasics

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