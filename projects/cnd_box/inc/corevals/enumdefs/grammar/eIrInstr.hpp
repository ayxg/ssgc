///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2025 Anton Yashchenko
// Licensed under the Apache License, Version 2.0(the "License");
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup cnd_compiler_corevals
/// @brief eIrInstr enum definition.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
// clang-format off
#include "use_ccapi.hpp"
#include "corevals/reflected_meta_enum.hpp"
// clang-format on

/// @addtogroup cnd_compiler_corevals
/// @{

namespace cnd::corevals::grammar {

/// Define applied enum cnd::corevals::grammar::eIrInstr
#define CND_APPLIED_ENUM_eIrInstr(m, sep, pre, lst) \
  m(StartUp)                                    \
  sep m(Restart)                                    \
  sep m(ShutDown)                                   \
  sep m(LoadBootDrive)                              \
  sep m(InstallBootDriver)                          \
  sep m(LoadBootDriver)                             \
  sep m(LoadDriver)                                 \
  sep m(InstallDriver)                              \
  sep m(Label)                                      \
  sep m(Goto)                                       \
  sep m(Block)                                      \
  sep m(Loop)                                       \
  sep m(If)                                         \
  sep m(Branch)                                     \
  sep m(Call)                                       \
  sep m(Return)                                     \
  sep m(Phi)                                        \
  sep m(Select)                                     \
  sep m(Unreachable)                                \
  sep m(Nothing)                                    \
  sep m(Add)                                        \
  sep m(Subtract)                                   \
  sep m(Muliply)                                    \
  sep m(DivideSigned)                               \
  sep m(Negate)                                     \
  sep m(DivideUnsigned)                             \
  sep m(RemainderSigned)                            \
  sep m(RemainderUnsigned)                          \
  sep m(Not)                                        \
  sep m(Increment)                                  \
  sep m(Decrement)                                  \
  sep m(ShiftLeft)                                  \
  sep m(ShiftRightSigned)                           \
  sep m(ShiftRightUnsigned)                         \
  sep m(RotateLeft)                                 \
  sep m(RotateRight)                                \
  sep m(BitwiseAnd)                                 \
  sep m(BitwiseOr)                                  \
  sep m(BitwiseXor)                                 \
  sep m(BitwiseNot)                                 \
  sep m(CountLeadingZeroes)                         \
  sep m(CountTrailingZeroes)                        \
  sep m(PopulationCount)                            \
  sep m(LogicalAnd)                                 \
  sep m(LogicalOr)                                  \
  sep m(EqualZero)                                  \
  sep m(Equal)                                      \
  sep m(NotEqual)                                   \
  sep m(LessThan)                                   \
  sep m(GreaterThan)                                \
  sep m(LessThanEqual)                              \
  sep m(GreaterThanEqual)                           \
  sep m(Extend)                                     \
  sep m(Wrap)                                       \
  sep m(Promote)                                    \
  sep m(Demote)                                     \
  sep m(Convert)                                    \
  sep m(Truncate)                                   \
  sep m(Reinterpret)                                \
  sep m(Min)                                        \
  sep m(Max)                                        \
  sep m(Nearest)                                    \
  sep m(Ciel)                                       \
  sep m(Floor)                                      \
  sep m(Trucate)                                    \
  sep m(Absolute)                                   \
  sep m(SquareRoot)                                 \
  sep m(CopySign)                                   \
  lst

// Define the enum.
CND_MM_CREATE_ENUM_FROM_APPLIED_ENUM_UNTYPED(eIrInstr, eIrInstr);
// Define the enum to cstr conversion.
CND_MM_CREATE_ENUMTOCSTR_FROM_ENUM_LIST(eIrInstrToCStr, eIrInstr, eIrInstr);
// Assert enum to cstr conversion.
#define CND_STATIC_ASSERT_ENUM_TO_CSTR_eIrInstr(x) CND_MM_STATIC_ASSERT_ENUM_TO_CSTR(x, eIrInstr, eIrInstrToCStr)

CND_APPLIED_ENUM_eIrInstr(CND_STATIC_ASSERT_ENUM_TO_CSTR_eIrInstr, , , );
static_assert(cxx::StrEq(eIrInstrToCStr(eIrInstr::COUNT), "COUNT"));
#undef CND_STATIC_ASSERT_ENUM_TO_CSTR_eIrInstr
}  // namespace cnd::corevals::grammar

/// @} // end of cnd_compiler_corevals

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2025 Anton Yashchenko
//
// Licensed under the Apache License, Version 2.0(the "License"); you may not use this file except in compliance with
// the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on
// an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////