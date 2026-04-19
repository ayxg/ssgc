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
/// @brief eClErr enum definition.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
// clang-format off
#include "ccapi/CommonCppApi.hpp"
#include "compiler_utils/ReflectedMetaEnum.hpp"
// clang-format on

/// @addtogroup cnd_compiler_corevals
/// @{

namespace cnd::corevals::diagnostic {
/// Define applied enum cnd::corevals::diagnostic::eClErr
#define CND_APPLIED_ENUM_eClErr(m, sep, pre, lst) \
  pre m(NoError)                                  \
  sep m(CompilerDevDebugError)                    \
  sep m(LexerUnclosedStringLiteral)               \
  sep m(LexerUnclosedCharacterLiteral)            \
  sep m(LexerEmptyCharacterLiteral)               \
  sep m(LexerUnknownScalarSuffix)                 \
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
enum class eClErr {
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
  COUNT
};
// Define the enum to cstr conversion.
constexpr const char* eClErrToCStr(eClErr e) noexcept {
  using enum eClErr;
  switch (e) {
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
    case COUNT:
      return "COUNT";
    default:
      return "<invalid>";
  }
};
// Assert enum to cstr conversion.
#define CND_STATIC_ASSERT_ENUM_TO_CSTR_eClErr(x) CND_MM_STATIC_ASSERT_ENUM_TO_CSTR(x, eClErr, eClErrToCStr)

static_assert(cxx::StrEq(eClErrToCStr(eClErr::kNoError), "kNoError"));
static_assert(cxx::StrEq(eClErrToCStr(eClErr::kCompilerDevDebugError), "kCompilerDevDebugError"));
static_assert(cxx::StrEq(eClErrToCStr(eClErr::kLexerUnclosedStringLiteral), "kLexerUnclosedStringLiteral"));
static_assert(cxx::StrEq(eClErrToCStr(eClErr::kLexerUnclosedCharacterLiteral), "kLexerUnclosedCharacterLiteral"));
static_assert(cxx::StrEq(eClErrToCStr(eClErr::kLexerEmptyCharacterLiteral), "kLexerEmptyCharacterLiteral"));
static_assert(cxx::StrEq(eClErrToCStr(eClErr::kLexerUnknownScalarSuffix), "kLexerUnknownScalarSuffix"));
static_assert(cxx::StrEq(eClErrToCStr(eClErr::kFailedToReadFile), "kFailedToReadFile"));
static_assert(cxx::StrEq(eClErrToCStr(eClErr::kParserExpectedDeclaration), "kParserExpectedDeclaration"));
static_assert(cxx::StrEq(eClErrToCStr(eClErr::kDriverInvalidArg), "kDriverInvalidArg"));
static_assert(cxx::StrEq(eClErrToCStr(eClErr::kDriverFlagMustHavePostfix), "kDriverFlagMustHavePostfix"));
static_assert(cxx::StrEq(eClErrToCStr(eClErr::kDriverFlagExpectedArgs), "kDriverFlagExpectedArgs"));
static_assert(cxx::StrEq(eClErrToCStr(eClErr::kDriverFlagInvalidArg), "kDriverFlagInvalidArg"));
static_assert(cxx::StrEq(eClErrToCStr(eClErr::kDriverDeniedOverwrite), "kDriverDeniedOverwrite"));
static_assert(cxx::StrEq(eClErrToCStr(eClErr::kDriverFailedToRedirectStream), "kDriverFailedToRedirectStream"));
;
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