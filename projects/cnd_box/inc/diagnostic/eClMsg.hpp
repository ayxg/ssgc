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
/// @brief cnd::corevals::diagnostic::eClDiagnostic enum definition.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
// clang-format off
#include "ccapi/CommonCppApi.hpp"
#include "compiler_utils/ReflectedMetaEnum.hpp"
#include "diagnostic/eClErr.hpp"
#include "diagnostic/eClWarning.hpp"
#include "diagnostic/eClGuide.hpp"
#include "diagnostic/eClDiagnostic.hpp"
// clang-format on

#ifndef CND_APPLIED_ENUM_eClErr
#error Macro symbol 'CND_APPLIED_ENUM_eClErr' from file 'corevals/enumdefs/diagnostic/eClErr.hpp' must be available.
#endif

#ifndef CND_APPLIED_ENUM_eClWarning
#error Macro symbol 'CND_APPLIED_ENUM_eClWarning' from file 'corevals/enumdefs/diagnostic/eClWarning.hpp' must be available.
#endif

#ifndef CND_APPLIED_ENUM_eClGuide
#error Macro symbol 'CND_APPLIED_ENUM_eGuide' from file 'corevals/enumdefs/diagnostic/eClGuide.hpp' must be available.
#endif

#ifndef CND_APPLIED_ENUM_eClDiagnostic
#error Macro symbol 'CND_APPLIED_ENUM_eDiagnostic' from file 'corevals/enumdefs/diagnostic/eClDiagnostic.hpp' must be available.
#endif

/// @addtogroup cnd_compiler_corevals
/// @{

namespace cnd::corevals::diagnostic {

/// Define applied enum cnd::corevals::diagnostic::eClMsg
#define CND_APPLIED_ENUM_eClMsg(m, sep, pre, lst)                                                        \
  pre m(GenericCompilerMessage) CND_APPLIED_ENUM_eClErr(m, sep, sep, ) \
      CND_APPLIED_ENUM_eClWarning(m, sep, sep, ) \
      CND_APPLIED_ENUM_eClGuide(m, sep, sep, ) \
      CND_APPLIED_ENUM_eClDiagnostic(m, sep, sep, ) lst

// Define the enum.
CND_MM_CREATE_ENUM_FROM_APPLIED_ENUM_UNTYPED(eClMsg, eClMsg);

// Define the enum to cstr conversion.
CND_MM_CREATE_ENUMTOCSTR_FROM_ENUM_LIST(eClMsgToCStr, eClMsg, eClMsg);

// Assert enum to cstr conversion.
#define CND_STATIC_ASSERT_ENUM_TO_CSTR_eClMsg(x) \
  CND_MM_STATIC_ASSERT_ENUM_TO_CSTR(x, eClMsg, eClMsgToCStr)

CND_APPLIED_ENUM_eClMsg(CND_STATIC_ASSERT_ENUM_TO_CSTR_eClMsg, , , );
static_assert(cxx::StrEq(eClMsgToCStr(eClMsg::COUNT), "COUNT"));
#undef CND_STATIC_ASSERT_ENUM_TO_CSTR_eClMsg
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