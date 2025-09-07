///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup cnd_compiler_corevals
/// @brief eTk enum definition.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
// clang-format off
#include "use_ccapi.hpp"
#include "corevals/reflected_meta_enum.hpp"
// clang-format on

/// @addtogroup cnd_compiler_corevals
/// @{

namespace cnd::corevals::grammar {
/// Define applied enum cnd::corevals::grammar::eTk
#define CND_APPLIED_ENUM_eTk(m, sep, pre, lst) \
  pre m(INVALID)                               \
  sep m(NONE)                                  \
  sep m(DirectiveInclude)                      \
  sep m(DirectiveDefMacro)                     \
  sep m(DirectiveEndmacro)                     \
  sep m(DirectiveIf)                           \
  sep m(DirectiveElse)                         \
  sep m(DirectiveElif)                         \
  sep m(DirectiveEndif)                        \
  sep m(DirectiveIfdef)                        \
  sep m(DirectiveIfndef)                       \
  sep m(DirectiveUndef)                        \
  sep m(KwDef)                                 \
  sep m(KwFn)                                  \
  sep m(KwClass)                               \
  sep m(KwMain)                                \
  sep m(KwImport)                              \
  sep m(KwNamespace)                           \
  sep m(KwUse)                                 \
  sep m(KwLib)                                 \
  sep m(KwDll)                                 \
  sep m(KwEnum)                                \
  sep m(KwIf)                                  \
  sep m(KwElif)                                \
  sep m(KwElse)                                \
  sep m(KwCxif)                                \
  sep m(KwCxelif)                              \
  sep m(KwCxelse)                              \
  sep m(KwSwitch)                              \
  sep m(KwCase)                                \
  sep m(KwDefault)                             \
  sep m(KwWhile)                               \
  sep m(KwFor)                                 \
  sep m(KwReturn)                              \
  sep m(KwBreak)                               \
  sep m(KwContinue)                            \
  sep m(KwInt)                                 \
  sep m(KwUint)                                \
  sep m(KwReal)                                \
  sep m(KwBool)                                \
  sep m(KwChar)                                \
  sep m(KwByte)                                \
  sep m(KwCstr)                                \
  sep m(KwStr)                                 \
  sep m(KwPtr)                                 \
  sep m(KwList)                                \
  sep m(KwArray)                               \
  sep m(KwTrue)                                \
  sep m(KwFalse)                               \
  sep m(KwNone)                                \
  sep m(KwVoid)                                \
  sep m(KwIn)                                  \
  sep m(KwAs)                                  \
  sep m(KwCin)                                 \
  sep m(KwCout)                                \
  sep m(KwNative)                              \
  sep m(KwConst)                               \
  sep m(KwRef)                                 \
  sep m(KwPrivate)                             \
  sep m(KwPublic)                              \
  sep m(KwStatic)                              \
  sep m(KwAny)                                 \
  sep m(KwAuto)                                \
  sep m(KwType)                                \
  sep m(KwValue)                               \
  sep m(KwTemplate)                            \
  sep m(KwProc)\
  sep m(Hash)                                  \
  sep m(Add)                                   \
  sep m(Sub)                                   \
  sep m(Mul)                                   \
  sep m(Div)                                   \
  sep m(Mod)                                   \
  sep m(And)                                   \
  sep m(Or)                                    \
  sep m(Xor)                                   \
  sep m(Not)                                   \
  sep m(Lsh)                                   \
  sep m(Rsh)                                   \
  sep m(Eq)                                    \
  sep m(Neq)                                   \
  sep m(Lt)                                    \
  sep m(Gt)                                    \
  sep m(Lte)                                   \
  sep m(Gte)                                   \
  sep m(Spaceship)                             \
  sep m(Assign)                                \
  sep m(NewAssign)                             \
  sep m(AddAssign)                             \
  sep m(SubAssign)                             \
  sep m(MulAssign)                             \
  sep m(DivAssign)                             \
  sep m(ModAssign)                             \
  sep m(AndAssign)                             \
  sep m(OrAssign)                              \
  sep m(XorAssign)                             \
  sep m(LshAssign)                             \
  sep m(RshAssign)                             \
  sep m(Inc)                                   \
  sep m(Dec)                                   \
  sep m(Dot)                                   \
  sep m(Bnot)                                  \
  sep m(Band)                                  \
  sep m(Bor)                                   \
  sep m(LParen)                                \
  sep m(RParen)                                \
  sep m(LBrace)                                \
  sep m(RBrace)                                \
  sep m(LBracket)                              \
  sep m(RBracket)                              \
  sep m(Semicolon)                             \
  sep m(Colon)                                 \
  sep m(Comma)                                 \
  sep m(Period)                                \
  sep m(DoubleColon)                           \
  sep m(Ellipsis)                              \
  sep m(CommercialAt)                          \
  sep m(LitInt)                                \
  sep m(LitUint)                               \
  sep m(LitReal)                               \
  sep m(LitBool)                               \
  sep m(LitChar)                               \
  sep m(LitByte)                               \
  sep m(LitCstr)                               \
  sep m(Ident)                                 \
  sep m(Eofile)                                \
  sep m(Whitespace)                            \
  sep m(Newline)                               \
  sep m(BlockComment)                          \
  sep m(LineComment)                           \
  sep m(Dollar)                                \
  sep m(Backlash)                              \
  sep m(Question)                              \
  lst

// Define the enum.
CND_MM_CREATE_ENUM_FROM_APPLIED_ENUM_UNTYPED(eTk, eTk);
// Define the enum to cstr conversion.
CND_MM_CREATE_ENUMTOCSTR_FROM_ENUM_LIST(eTkToCStr, eTk, eTk);
// Assert enum to cstr conversion.
#define CND_STATIC_ASSERT_ENUM_TO_CSTR_eTk(x) CND_MM_STATIC_ASSERT_ENUM_TO_CSTR(x, eTk, eTkToCStr)

CND_APPLIED_ENUM_eTk(CND_STATIC_ASSERT_ENUM_TO_CSTR_eTk, , , );
static_assert(cxx::StrEq(eTkToCStr(eTk::COUNT), "COUNT"));
#undef CND_STATIC_ASSERT_ENUM_TO_CSTR_eTk
}  // namespace cnd::corevals::grammar

/// @} // end of cnd_compiler_corevals

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