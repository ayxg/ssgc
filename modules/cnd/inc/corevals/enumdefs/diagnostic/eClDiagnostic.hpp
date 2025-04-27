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
/// @brief eClDiagnostic enum definition.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
// clang-format off
#include "use_ccapi.hpp"
#include "corevals/reflected_meta_enum.hpp"
// clang-format on

/// @addtogroup cnd_compiler_corevals
/// @{

namespace cnd::corevals::diagnostic {
/// Define applied enum cnd::corevals::diagnostic::eClDiagnostic
#define CND_APPLIED_ENUM_eClDiagnostic(m, sep, pre, lst) \
  pre m(NoDiagnostic)                                \
  lst

// Define the enum.
CND_MM_CREATE_ENUM_FROM_APPLIED_ENUM_UNTYPED(eClDiagnostic, eClDiagnostic);
// Define the enum to cstr conversion.
CND_MM_CREATE_ENUMTOCSTR_FROM_ENUM_LIST(eClDiagnosticToCStr, eClDiagnostic, eClDiagnostic);
// Assert enum to cstr conversion.
#define CND_STATIC_ASSERT_ENUM_TO_CSTR_eClDiagnostic(x) CND_MM_STATIC_ASSERT_ENUM_TO_CSTR(x, eClDiagnostic, eClDiagnosticToCStr)

CND_APPLIED_ENUM_eClDiagnostic(CND_STATIC_ASSERT_ENUM_TO_CSTR_eClDiagnostic, , , );
static_assert(cxx::StrEq(eClDiagnosticToCStr(eClDiagnostic::COUNT), "COUNT"));
#undef CND_STATIC_ASSERT_ENUM_TO_CSTR_eClDiagnostic
}  // namespace cnd::corevals::diagnostic

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