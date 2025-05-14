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
/// @brief cnd::corevals::diagnostic::eClDiagnostic enum definition.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
// clang-format off
#include "use_ccapi.hpp"
#include "corevals/reflected_meta_enum.hpp"
#include "corevals/enumdefs/diagnostic/eClErr.hpp"
#include "corevals/enumdefs/diagnostic/eClWarning.hpp"
#include "corevals/enumdefs/diagnostic/eClGuide.hpp"
#include "corevals/enumdefs/diagnostic/eClDiagnostic.hpp"
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
// Licensed under the Apache License, Version 2.0(the "License"); you may not use this file except in compliance with
// the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on
// an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////