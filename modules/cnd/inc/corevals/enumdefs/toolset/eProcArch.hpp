///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the Apache License, Version 2.0(the "License");
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup cnd_compiler_corevals
/// @brief cnd::corevals::toolset::eProcArch enum definition.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
// clang-format off
#include "use_ccapi.hpp"
#include "corevals/reflected_meta_enum.hpp"
// clang-format on

/// @addtogroup cnd_compiler_corevals
/// @{

namespace cnd::corevals::toolset {
/// Define applied enum cnd::corevals::toolset::eProcArch
#define CND_APPLIED_ENUM_eProcArch(m, sep, pre, lst) \
  pre m(INVALID)                                     \
  sep m(NONE)                                        \
  sep m(Custom)                                      \
  sep m(Embedded)                                    \
  sep m(Alpha)                                       \
  sep m(AMD64)                                       \
  sep m(ARM)                                         \
  sep m(ARM64)                                       \
  sep m(Blackfin)                                    \
  sep m(Convex)                                      \
  sep m(Epiphany)                                    \
  sep m(HppaRISC)                                    \
  sep m(Intel86)                                     \
  sep m(IntelItanium)                                \
  sep m(Motorola68k)                                 \
  sep m(MIPS)                                        \
  sep m(PowerPC)                                     \
  sep m(Pyramid9810)                                 \
  sep m(RS6000)                                      \
  sep m(SPARC)                                       \
  sep m(SuperH)                                      \
  sep m(SystemZ)                                     \
  sep m(TMS320)                                      \
  sep m(TMS470)                                      \
  lst

// Define the enum.
CND_MM_CREATE_ENUM_FROM_APPLIED_ENUM_UNTYPED(eProcArch, eProcArch);

// Define the enum to cstr conversion.
CND_MM_CREATE_ENUMTOCSTR_FROM_ENUM_LIST(eProcArchToCStr, eProcArch, eProcArch);

// Assert enum to cstr conversion.
#define CND_STATIC_ASSERT_ENUM_TO_CSTR_eProcArch(x) CND_MM_STATIC_ASSERT_ENUM_TO_CSTR(x, eProcArch, eProcArchToCStr)
CND_APPLIED_ENUM_eProcArch(CND_STATIC_ASSERT_ENUM_TO_CSTR_eProcArch, , , );
static_assert(cxx::StrEq(eProcArchToCStr(eProcArch::COUNT), "COUNT"));
#undef CND_STATIC_ASSERT_ENUM_TO_CSTR_eProcArch

}  // namespace cnd::corevals::toolset

/// @} // end of cnd_compiler_corevals

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