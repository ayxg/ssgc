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
enum class eClMsg {
  kGenericCompilerMessage,
  kNoError,
  kCompilerDevDebugError,
  kLexerUnclosedStringLiteral,
  kLexerUnclosedCharacterLiteral,
  kLexerEmptyCharacterLiteral,
  kLexerUnknownScalarSuffix,
  kFailedToReadFile,
  kParserExpectedDeclaration,
  kDriverInvalidArg,
  kDriverFlagMustHavePostfix,
  kDriverFlagExpectedArgs,
  kDriverFlagInvalidArg,
  kDriverDeniedOverwrite,
  kDriverFailedToRedirectStream,
  kNoWarning,
  kNoGuide,
  kNoDiagnostic,
  COUNT
};

// Define the enum to cstr conversion.
constexpr const char* eClMsgToCStr(eClMsg e) noexcept {
  using enum eClMsg;
  switch (e) {
    case kGenericCompilerMessage:
      return "kGenericCompilerMessage";
    case kNoError:
      return "kNoError";
    case kCompilerDevDebugError:
      return "kCompilerDevDebugError";
    case kLexerUnclosedStringLiteral:
      return "kLexerUnclosedStringLiteral";
    case kLexerUnclosedCharacterLiteral:
      return "kLexerUnclosedCharacterLiteral";
    case kLexerEmptyCharacterLiteral:
      return "kLexerEmptyCharacterLiteral";
    case kLexerUnknownScalarSuffix:
      return "kLexerUnknownScalarSuffix";
    case kFailedToReadFile:
      return "kFailedToReadFile";
    case kParserExpectedDeclaration:
      return "kParserExpectedDeclaration";
    case kDriverInvalidArg:
      return "kDriverInvalidArg";
    case kDriverFlagMustHavePostfix:
      return "kDriverFlagMustHavePostfix";
    case kDriverFlagExpectedArgs:
      return "kDriverFlagExpectedArgs";
    case kDriverFlagInvalidArg:
      return "kDriverFlagInvalidArg";
    case kDriverDeniedOverwrite:
      return "kDriverDeniedOverwrite";
    case kDriverFailedToRedirectStream:
      return "kDriverFailedToRedirectStream";
    case kNoWarning:
      return "kNoWarning";
    case kNoGuide:
      return "kNoGuide";
    case kNoDiagnostic:
      return "kNoDiagnostic";
    case COUNT:
      return "COUNT";
    default:
      return "<invalid>";
  }
};

// Assert enum to cstr conversion.
#define CND_STATIC_ASSERT_ENUM_TO_CSTR_eClMsg(x) \
  CND_MM_STATIC_ASSERT_ENUM_TO_CSTR(x, eClMsg, eClMsgToCStr)

static_assert(cxx::StrEq(eClMsgToCStr(eClMsg::kGenericCompilerMessage), "kGenericCompilerMessage"));
static_assert(cxx::StrEq(eClMsgToCStr(eClMsg::kNoError), "kNoError"));
static_assert(cxx::StrEq(eClMsgToCStr(eClMsg::kCompilerDevDebugError), "kCompilerDevDebugError"));
static_assert(cxx::StrEq(eClMsgToCStr(eClMsg::kLexerUnclosedStringLiteral), "kLexerUnclosedStringLiteral"));
static_assert(cxx::StrEq(eClMsgToCStr(eClMsg::kLexerUnclosedCharacterLiteral), "kLexerUnclosedCharacterLiteral"));
static_assert(cxx::StrEq(eClMsgToCStr(eClMsg::kLexerEmptyCharacterLiteral), "kLexerEmptyCharacterLiteral"));
static_assert(cxx::StrEq(eClMsgToCStr(eClMsg::kLexerUnknownScalarSuffix), "kLexerUnknownScalarSuffix"));
static_assert(cxx::StrEq(eClMsgToCStr(eClMsg::kFailedToReadFile), "kFailedToReadFile"));
static_assert(cxx::StrEq(eClMsgToCStr(eClMsg::kParserExpectedDeclaration), "kParserExpectedDeclaration"));
static_assert(cxx::StrEq(eClMsgToCStr(eClMsg::kDriverInvalidArg), "kDriverInvalidArg"));
static_assert(cxx::StrEq(eClMsgToCStr(eClMsg::kDriverFlagMustHavePostfix), "kDriverFlagMustHavePostfix"));
static_assert(cxx::StrEq(eClMsgToCStr(eClMsg::kDriverFlagExpectedArgs), "kDriverFlagExpectedArgs"));
static_assert(cxx::StrEq(eClMsgToCStr(eClMsg::kDriverFlagInvalidArg), "kDriverFlagInvalidArg"));
static_assert(cxx::StrEq(eClMsgToCStr(eClMsg::kDriverDeniedOverwrite), "kDriverDeniedOverwrite"));
static_assert(cxx::StrEq(eClMsgToCStr(eClMsg::kDriverFailedToRedirectStream), "kDriverFailedToRedirectStream"));
static_assert(cxx::StrEq(eClMsgToCStr(eClMsg::kNoWarning), "kNoWarning"));
static_assert(cxx::StrEq(eClMsgToCStr(eClMsg::kNoGuide), "kNoGuide"));
static_assert(cxx::StrEq(eClMsgToCStr(eClMsg::kNoDiagnostic), "kNoDiagnostic"));
;
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