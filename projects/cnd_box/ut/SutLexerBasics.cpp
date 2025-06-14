///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the Apache License, Version 2.0(the "License");
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
#include "trtools/Lexer.hpp"
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
// Licensed under the Apache License, Version 2.0(the "License"); you may not use this file except in compliance with
// the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on
// an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////