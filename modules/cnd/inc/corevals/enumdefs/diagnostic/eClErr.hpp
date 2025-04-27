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
/// @brief eClErr enum definition.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
// clang-format off
#include "use_ccapi.hpp"
#include "corevals/reflected_meta_enum.hpp"
// clang-format on

/// @addtogroup cnd_compiler_corevals
/// @{

namespace cnd::corevals::diagnostic {
/// Define applied enum cnd::corevals::diagnostic::eClErr
#define CND_APPLIED_ENUM_eClErr(m, sep, pre, lst) \
  pre m(NoError)                                  \
  sep m(CompilerDevDebugError)                    \
  sep m(FailedToReadFile)                         \
  sep m(ParserExpectedDeclaration)                \
  sep m(DriverInvalidArg)                         \
  sep m(DriverFlagMustHavePostfix)                \
  sep m(DriverFlagExpectedArgs)                   \
  sep m(DriverFlagInvalidArg)                     \
  sep m(DriverDeniedOverwrite)                    \
  sep m(DriverFailedToRedirectStream)             \
  lst

//sep m(CevalRealOverflow) sep m(CevalInvalidBoolLiteral) sep m(CevalInvalidByteLiteral)                  \  sep m(CevalIntegerOverflow)                     \

// Define the enum.
CND_MM_CREATE_ENUM_FROM_APPLIED_ENUM_UNTYPED(eClErr, eClErr);
// Define the enum to cstr conversion.
CND_MM_CREATE_ENUMTOCSTR_FROM_ENUM_LIST(eClErrToCStr, eClErr, eClErr);
// Assert enum to cstr conversion.
#define CND_STATIC_ASSERT_ENUM_TO_CSTR_eClErr(x) CND_MM_STATIC_ASSERT_ENUM_TO_CSTR(x, eClErr, eClErrToCStr)

CND_APPLIED_ENUM_eClErr(CND_STATIC_ASSERT_ENUM_TO_CSTR_eClErr, , , );
static_assert(cxx::StrEq(eClErrToCStr(eClErr::COUNT), "COUNT"));
#undef CND_STATIC_ASSERT_ENUM_TO_CSTR_eClErr
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