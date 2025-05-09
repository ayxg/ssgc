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
/// @brief eSrcChar enum definition.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
// clang-format off
#include "use_ccapi.hpp"
#include "corevals/reflected_meta_enum.hpp"
// clang-format on

/// @addtogroup cnd_compiler_corevals
/// @{

namespace cnd::corevals::grammar {
/// Define applied enum cnd::corevals::grammar::eSrcChar
#define CND_APPLIED_ENUM_eSrcChar(m, sep, pre, lst) \
  pre m(Null)                                       \
  sep m(StartOfHeader)                              \
  sep m(StartOfText)                                \
  sep m(EndOfText)                                  \
  sep m(EndOfTransmission)                          \
  sep m(Enquiry)                                    \
  sep m(Acknowledge)                                \
  sep m(Bell)                                       \
  sep m(Backspace)                                  \
  sep m(HorizontalTab)                              \
  sep m(LineFeed)                                   \
  sep m(VerticalTab)                                \
  sep m(FormFeed)                                   \
  sep m(CarriageReturn)                             \
  sep m(ShiftOut)                                   \
  sep m(ShiftIn)                                    \
  sep m(DataLinkEscape)                             \
  sep m(DeviceControl1)                             \
  sep m(DeviceControl2)                             \
  sep m(DeviceControl3)                             \
  sep m(DeviceControl4)                             \
  sep m(NegativeAcknowledge)                        \
  sep m(SynchronousIdle)                            \
  sep m(EndOfTransmissionBlock)                     \
  sep m(Cancel)                                     \
  sep m(EndOfMedium)                                \
  sep m(Substitute)                                 \
  sep m(Escape)                                     \
  sep m(FileSeparator)                              \
  sep m(GroupSeparator)                             \
  sep m(RecordSeparator)                            \
  sep m(UnitSeparator)                              \
  sep m(Space)                                      \
  sep m(ExclamationMark)                            \
  sep m(Quotation)                                  \
  sep m(Hash)                                       \
  sep m(DollarSign)                                 \
  sep m(PercentSign)                                \
  sep m(Ampersand)                                  \
  sep m(Apostrophe)                                 \
  sep m(LeftParenthesis)                            \
  sep m(RightParenthesis)                           \
  sep m(Asterisk)                                   \
  sep m(PlusSign)                                   \
  sep m(Comma)                                      \
  sep m(HyphenMinus)                                \
  sep m(Period)                                     \
  sep m(Solidus)                                    \
  sep m(Zero)                                       \
  sep m(One)                                        \
  sep m(Two)                                        \
  sep m(Three)                                      \
  sep m(Four)                                       \
  sep m(Five)                                       \
  sep m(Six)                                        \
  sep m(Seven)                                      \
  sep m(Eight)                                      \
  sep m(Nine)                                       \
  sep m(Colon)                                      \
  sep m(Semicolon)                                  \
  sep m(LessThan)                                   \
  sep m(EqualSign)                                  \
  sep m(GreaterThan)                                \
  sep m(QuestionMark)                               \
  sep m(AtSign)                                     \
  sep m(UpperA)                                     \
  sep m(UpperB)                                     \
  sep m(UpperC)                                     \
  sep m(UpperD)                                     \
  sep m(UpperE)                                     \
  sep m(UpperF)                                     \
  sep m(UpperG)                                     \
  sep m(UpperH)                                     \
  sep m(UpperI)                                     \
  sep m(UpperJ)                                     \
  sep m(UpperK)                                     \
  sep m(UpperL)                                     \
  sep m(UpperM)                                     \
  sep m(UpperN)                                     \
  sep m(UpperO)                                     \
  sep m(UpperP)                                     \
  sep m(UpperQ)                                     \
  sep m(UpperR)                                     \
  sep m(UpperS)                                     \
  sep m(UpperT)                                     \
  sep m(UpperU)                                     \
  sep m(UpperV)                                     \
  sep m(UpperW)                                     \
  sep m(UpperX)                                     \
  sep m(UpperY)                                     \
  sep m(UpperZ)                                     \
  sep m(LeftSquareBracket)                          \
  sep m(Backslash)                                  \
  sep m(RightSquareBracket)                         \
  sep m(Caret)                                      \
  sep m(Underscore)                                 \
  sep m(Backtick)                                   \
  sep m(LowerA)                                     \
  sep m(LowerB)                                     \
  sep m(LowerC)                                     \
  sep m(LowerD)                                     \
  sep m(LowerE)                                     \
  sep m(LowerF)                                     \
  sep m(LowerG)                                     \
  sep m(LowerH)                                     \
  sep m(LowerI)                                     \
  sep m(LowerJ)                                     \
  sep m(LowerK)                                     \
  sep m(LowerL)                                     \
  sep m(LowerM)                                     \
  sep m(LowerN)                                     \
  sep m(LowerO)                                     \
  sep m(LowerP)                                     \
  sep m(LowerQ)                                     \
  sep m(LowerR)                                     \
  sep m(LowerS)                                     \
  sep m(LowerT)                                     \
  sep m(LowerU)                                     \
  sep m(LowerV)                                     \
  sep m(LowerW)                                     \
  sep m(LowerX)                                     \
  sep m(LowerY)                                     \
  sep m(LowerZ)                                     \
  sep m(LeftCurlyBrace)                             \
  sep m(VerticalBar)                                \
  sep m(RightCurlyBrace)                            \
  sep m(Tilde)                                      \
  sep m(Delete)                                     \
  lst

// Define the enum.
CND_MM_CREATE_ENUM_FROM_APPLIED_ENUM_UNTYPED(eSrcChar, eSrcChar);
// Define the enum to cstr conversion.
CND_MM_CREATE_ENUMTOCSTR_FROM_ENUM_LIST(eSrcCharToCStr, eSrcChar, eSrcChar);
// Assert enum to cstr conversion.
#define CND_STATIC_ASSERT_ENUM_TO_CSTR_eSrcChar(x) CND_MM_STATIC_ASSERT_ENUM_TO_CSTR(x, eSrcChar, eSrcCharToCStr)

CND_APPLIED_ENUM_eSrcChar(CND_STATIC_ASSERT_ENUM_TO_CSTR_eSrcChar, , , );
static_assert(cxx::StrEq(eSrcCharToCStr(eSrcChar::COUNT), "COUNT"));
#undef CND_STATIC_ASSERT_ENUM_TO_CSTR_eSrcChar
}  // namespace cnd::corevals::grammar

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