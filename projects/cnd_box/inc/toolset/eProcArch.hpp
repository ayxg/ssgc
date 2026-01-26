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
/// @brief cnd::corevals::toolset::eProcArch enum definition.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
// clang-format off
#include "ccapi/CommonCppApi.hpp"
#include "compiler_utils/ReflectedMetaEnum.hpp"
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