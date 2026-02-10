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
/// @brief eSrcChar enum definition.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
// clang-format off
#include "ccapi/CommonCppApi.hpp"
#include "compiler_utils/ReflectedMetaEnum.hpp"
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