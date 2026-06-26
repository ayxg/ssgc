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

///////////////////////////////////////////////////////////////////////////////////////////////////
// Define diagnostic enums.
///////////////////////////////////////////////////////////////////////////////////////////////////

#define CND_APPLIED_ENUM_eClErr(m, sep, pre, lst) \
  pre m(NoError)                                  \
  sep m(CompilerDevDebugError)                    \
  sep m(LexerUnclosedStringLiteral)               \
  sep m(LexerUnclosedCharacterLiteral)            \
  sep m(LexerEmptyCharacterLiteral)               \
  sep m(LexerUnknownNumericSuffix)                 \
  sep m(LexerInvalidPunctuator)                   \
  sep m(FailedToReadFile)                         \
  sep m(ParserExpectedDeclaration)                \
  sep m(DriverInvalidArg)                         \
  sep m(DriverFlagMustHavePostfix)                \
  sep m(DriverFlagExpectedArgs)                   \
  sep m(DriverFlagInvalidArg)                     \
  sep m(DriverDeniedOverwrite)                    \
  sep m(DriverFailedToRedirectStream) lst

#define CND_APPLIED_ENUM_eClWarning(m, sep, pre, lst) pre m(NoWarning) lst

#define CND_APPLIED_ENUM_eClGuide(m, sep, pre, lst) pre m(NoGuide) lst

#define CND_APPLIED_ENUM_eClDiagnostic(m, sep, pre, lst) \
  pre m(NoDiagnostic)                                    \
  lst

CND_MM_CREATE_ENUM_FROM_APPLIED_ENUM_UNTYPED(eClErr, eClErr);
CND_MM_CREATE_ENUMTOCSTR_FROM_ENUM_LIST(eClErrToCStr, eClErr, eClErr);
#define CND_STATIC_ASSERT_ENUM_TO_CSTR_eClErr(x) \
  CND_MM_STATIC_ASSERT_ENUM_TO_CSTR(x, eClErr, eClErrToCStr)
CND_APPLIED_ENUM_eClErr(CND_STATIC_ASSERT_ENUM_TO_CSTR_eClErr, , , );
static_assert(cxx::StrEq(eClErrToCStr(eClErr::COUNT), "COUNT"));
#undef CND_STATIC_ASSERT_ENUM_TO_CSTR_eClErr

CND_MM_CREATE_ENUM_FROM_APPLIED_ENUM_UNTYPED(eClWarning, eClWarning);
CND_MM_CREATE_ENUMTOCSTR_FROM_ENUM_LIST(eClWarningToCStr, eClWarning, eClWarning);
#define CND_STATIC_ASSERT_ENUM_TO_CSTR_eClWarning(x) \
  CND_MM_STATIC_ASSERT_ENUM_TO_CSTR(x, eClWarning, eClWarningToCStr)
CND_APPLIED_ENUM_eClWarning(CND_STATIC_ASSERT_ENUM_TO_CSTR_eClWarning, , , );
static_assert(cxx::StrEq(eClWarningToCStr(eClWarning::COUNT), "COUNT"));
#undef CND_STATIC_ASSERT_ENUM_TO_CSTR_eClWarning

CND_MM_CREATE_ENUM_FROM_APPLIED_ENUM_UNTYPED(eClGuide, eClGuide);
CND_MM_CREATE_ENUMTOCSTR_FROM_ENUM_LIST(eClGuideToCStr, eClGuide, eClGuide);
#define CND_STATIC_ASSERT_ENUM_TO_CSTR_eClGuide(x) \
  CND_MM_STATIC_ASSERT_ENUM_TO_CSTR(x, eClGuide, eClGuideToCStr)
CND_APPLIED_ENUM_eClGuide(CND_STATIC_ASSERT_ENUM_TO_CSTR_eClGuide, , , );
static_assert(cxx::StrEq(eClGuideToCStr(eClGuide::COUNT), "COUNT"));
#undef CND_STATIC_ASSERT_ENUM_TO_CSTR_eClGuide

CND_MM_CREATE_ENUM_FROM_APPLIED_ENUM_UNTYPED(eClDiagnostic, eClDiagnostic);
CND_MM_CREATE_ENUMTOCSTR_FROM_ENUM_LIST(eClDiagnosticToCStr, eClDiagnostic, eClDiagnostic);
#define CND_STATIC_ASSERT_ENUM_TO_CSTR_eClDiagnostic(x) \
  CND_MM_STATIC_ASSERT_ENUM_TO_CSTR(x, eClDiagnostic, eClDiagnosticToCStr)
CND_APPLIED_ENUM_eClDiagnostic(CND_STATIC_ASSERT_ENUM_TO_CSTR_eClDiagnostic, , , );
static_assert(cxx::StrEq(eClDiagnosticToCStr(eClDiagnostic::COUNT), "COUNT"));
#undef CND_STATIC_ASSERT_ENUM_TO_CSTR_eClDiagnostic

///////////////////////////////////////////////////////////////////////////////////////////////////
// Define diagnostic categories.
///////////////////////////////////////////////////////////////////////////////////////////////////

#define CND_APPLIED_ENUM_eClErrCategory(m, sep, pre, lst) \
  pre m(None)                                             \
  sep m(Warning)                                          \
  sep m(CompilerImpl)                                     \
  sep m(CommandLine)                                      \
  sep m(General)                                          \
  sep m(Scanner)                                          \
  sep m(Lexer)                                            \
  sep m(Parser)                                           \
  sep m(Preprocessor)                                     \
  sep m(ConstEval) lst

#define CND_APPLIED_ENUM_eClWarningCategory(m, sep, pre, lst) \
  pre m(None)                                                 \
  sep m(Warning)                                              \
  sep m(CompilerImpl)                                         \
  sep m(CommandLine)                                          \
  sep m(General)                                              \
  sep m(Scanner)                                              \
  sep m(Lexer)                                                \
  sep m(Parser)                                               \
  sep m(Preprocessor)                                         \
  sep m(ConstEval) lst

#define CND_APPLIED_ENUM_eClGuideCategory(m, sep, pre, lst) \
  pre m(None)                                               \
  sep m(Warning)                                            \
  sep m(CompilerImpl)                                       \
  sep m(CommandLine)                                        \
  sep m(General)                                            \
  sep m(Scanner)                                            \
  sep m(Lexer)                                              \
  sep m(Parser)                                             \
  sep m(Preprocessor)                                       \
  sep m(ConstEval) lst

#define CND_APPLIED_ENUM_eClDiagnosticCategory(m, sep, pre, lst) \
  pre m(None)                                                    \
  sep m(Warning)                                                 \
  sep m(CompilerImpl)                                            \
  sep m(CommandLine)                                             \
  sep m(General)                                                 \
  sep m(Scanner)                                                 \
  sep m(Lexer)                                                   \
  sep m(Parser)                                                  \
  sep m(Preprocessor)                                            \
  sep m(ConstEval)                                               \
  lst

CND_MM_CREATE_ENUM_FROM_APPLIED_ENUM_UNTYPED(eClErrCategory, eClErrCategory);
CND_MM_CREATE_ENUMTOCSTR_FROM_ENUM_LIST(eClErrCategoryToCStr, eClErrCategory, eClErrCategory);
#define CND_STATIC_ASSERT_ENUM_TO_CSTR_eClErrCategory(x) \
  CND_MM_STATIC_ASSERT_ENUM_TO_CSTR(x, eClErrCategory, eClErrCategoryToCStr)
CND_APPLIED_ENUM_eClErrCategory(CND_STATIC_ASSERT_ENUM_TO_CSTR_eClErrCategory, , , );
static_assert(cxx::StrEq(eClErrCategoryToCStr(eClErrCategory::COUNT), "COUNT"));
#undef CND_STATIC_ASSERT_ENUM_TO_CSTR_eClErrCategory

CND_MM_CREATE_ENUM_FROM_APPLIED_ENUM_UNTYPED(eClWarningCategory, eClWarningCategory);
CND_MM_CREATE_ENUMTOCSTR_FROM_ENUM_LIST(eClWarningCategoryToCStr, eClWarningCategory,
                                        eClWarningCategory);
#define CND_STATIC_ASSERT_ENUM_TO_CSTR_eClWarningCategory(x) \
  CND_MM_STATIC_ASSERT_ENUM_TO_CSTR(x, eClWarningCategory, eClWarningCategoryToCStr)
CND_APPLIED_ENUM_eClWarningCategory(CND_STATIC_ASSERT_ENUM_TO_CSTR_eClWarningCategory, , , );
static_assert(cxx::StrEq(eClWarningCategoryToCStr(eClWarningCategory::COUNT), "COUNT"));
#undef CND_STATIC_ASSERT_ENUM_TO_CSTR_eClWarningCategory

CND_MM_CREATE_ENUM_FROM_APPLIED_ENUM_UNTYPED(eClGuideCategory, eClGuideCategory);
CND_MM_CREATE_ENUMTOCSTR_FROM_ENUM_LIST(eClGuideCategoryToCStr, eClGuideCategory, eClGuideCategory);
#define CND_STATIC_ASSERT_ENUM_TO_CSTR_eClGuideCategory(x) \
  CND_MM_STATIC_ASSERT_ENUM_TO_CSTR(x, eClGuideCategory, eClGuideCategoryToCStr)
CND_APPLIED_ENUM_eClGuideCategory(CND_STATIC_ASSERT_ENUM_TO_CSTR_eClGuideCategory, , , );
static_assert(cxx::StrEq(eClGuideCategoryToCStr(eClGuideCategory::COUNT), "COUNT"));
#undef CND_STATIC_ASSERT_ENUM_TO_CSTR_eClGuideCategory

CND_MM_CREATE_ENUM_FROM_APPLIED_ENUM_UNTYPED(eClDiagnosticCategory, eClDiagnosticCategory);
CND_MM_CREATE_ENUMTOCSTR_FROM_ENUM_LIST(eClDiagnosticCategoryToCStr, eClDiagnosticCategory,
                                        eClDiagnosticCategory);
#define CND_STATIC_ASSERT_ENUM_TO_CSTR_eClDiagnosticCategory(x) \
  CND_MM_STATIC_ASSERT_ENUM_TO_CSTR(x, eClDiagnosticCategory, eClDiagnosticCategoryToCStr)
CND_APPLIED_ENUM_eClDiagnosticCategory(CND_STATIC_ASSERT_ENUM_TO_CSTR_eClDiagnosticCategory, , , );
static_assert(cxx::StrEq(eClDiagnosticCategoryToCStr(eClDiagnosticCategory::COUNT), "COUNT"));
#undef CND_STATIC_ASSERT_ENUM_TO_CSTR_eClDiagnosticCategory

constexpr eClErrCategory GetClErrCategory(eClErr e) noexcept {
  switch (e) {
    default:
      return eClErrCategory::kNone;
  }
};

constexpr eClWarningCategory GetClWarningCategory(eClWarning e) noexcept {
  switch (e) {
    default:
      return eClWarningCategory::kNone;
  }
};

constexpr eClGuideCategory GetClGuideCategory(eClGuide e) noexcept {
  switch (e) {
    default:
      return eClGuideCategory::kNone;
  }
};

constexpr eClDiagnosticCategory GetClDiagnosticCategory(eClDiagnostic e) noexcept {
  switch (e) {
    default:
      return eClDiagnosticCategory::kNone;
  }
};

///////////////////////////////////////////////////////////////////////////////////////////////////
// Define message type.
///////////////////////////////////////////////////////////////////////////////////////////////////

#define CND_APPLIED_ENUM_eClMsgType(m, sep, pre, lst) \
  pre m(None)                                         \
  sep m(Warning)                                      \
  sep m(Error)                                        \
  sep m(Guideline)                                    \
  sep m(Diagnostic)                                   \
  sep m(Generic)                                      \
  lst

CND_MM_CREATE_ENUM_FROM_APPLIED_ENUM_UNTYPED(eClMsgType, eClMsgType);
CND_MM_CREATE_ENUMTOCSTR_FROM_ENUM_LIST(eClMsgTypeToCStr, eClMsgType, eClMsgType);
#define CND_STATIC_ASSERT_ENUM_TO_CSTR_eClMsgType(x) \
  CND_MM_STATIC_ASSERT_ENUM_TO_CSTR(x, eClMsgType, eClMsgTypeToCStr)
CND_APPLIED_ENUM_eClMsgType(CND_STATIC_ASSERT_ENUM_TO_CSTR_eClMsgType, , , );
static_assert(cxx::StrEq(eClMsgTypeToCStr(eClMsgType::COUNT), "COUNT"));
#undef CND_STATIC_ASSERT_ENUM_TO_CSTR_eClMsgType

constexpr eClMsgType GetClMsgType(eClErr e) noexcept { return eClMsgType::kError; };
constexpr eClMsgType GetClMsgType(eClWarning e) noexcept { return eClMsgType::kWarning; };
constexpr eClMsgType GetClMsgType(eClGuide e) noexcept { return eClMsgType::kGuideline; };
constexpr eClMsgType GetClMsgType(eClDiagnostic e) noexcept { return eClMsgType::kDiagnostic; };

///////////////////////////////////////////////////////////////////////////////////////////////////
// Define message enum.
///////////////////////////////////////////////////////////////////////////////////////////////////

#define CND_APPLIED_ENUM_eClMsg(m, sep, pre, lst)                                         \
  pre m(GenericCompilerMessage) CND_APPLIED_ENUM_eClErr(m, sep, sep, )                    \
      CND_APPLIED_ENUM_eClWarning(m, sep, sep, ) CND_APPLIED_ENUM_eClGuide(m, sep, sep, ) \
          CND_APPLIED_ENUM_eClDiagnostic(m, sep, sep, ) lst

CND_MM_CREATE_ENUM_FROM_APPLIED_ENUM_UNTYPED(eClMsg, eClMsg);
CND_MM_CREATE_ENUMTOCSTR_FROM_ENUM_LIST(eClMsgToCStr, eClMsg, eClMsg);
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