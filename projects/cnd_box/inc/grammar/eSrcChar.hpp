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
enum class eSrcChar {
  kNull,
  kStartOfHeader,
  kStartOfText,
  kEndOfText,
  kEndOfTransmission,
  kEnquiry,
  kAcknowledge,
  kBell,
  kBackspace,
  kHorizontalTab,
  kLineFeed,
  kVerticalTab,
  kFormFeed,
  kCarriageReturn,
  kShiftOut,
  kShiftIn,
  kDataLinkEscape,
  kDeviceControl1,
  kDeviceControl2,
  kDeviceControl3,
  kDeviceControl4,
  kNegativeAcknowledge,
  kSynchronousIdle,
  kEndOfTransmissionBlock,
  kCancel,
  kEndOfMedium,
  kSubstitute,
  kEscape,
  kFileSeparator,
  kGroupSeparator,
  kRecordSeparator,
  kUnitSeparator,
  kSpace,
  kExclamationMark,
  kQuotation,
  kHash,
  kDollarSign,
  kPercentSign,
  kAmpersand,
  kApostrophe,
  kLeftParenthesis,
  kRightParenthesis,
  kAsterisk,
  kPlusSign,
  kComma,
  kHyphenMinus,
  kPeriod,
  kSolidus,
  kZero,
  kOne,
  kTwo,
  kThree,
  kFour,
  kFive,
  kSix,
  kSeven,
  kEight,
  kNine,
  kColon,
  kSemicolon,
  kLessThan,
  kEqualSign,
  kGreaterThan,
  kQuestionMark,
  kAtSign,
  kUpperA,
  kUpperB,
  kUpperC,
  kUpperD,
  kUpperE,
  kUpperF,
  kUpperG,
  kUpperH,
  kUpperI,
  kUpperJ,
  kUpperK,
  kUpperL,
  kUpperM,
  kUpperN,
  kUpperO,
  kUpperP,
  kUpperQ,
  kUpperR,
  kUpperS,
  kUpperT,
  kUpperU,
  kUpperV,
  kUpperW,
  kUpperX,
  kUpperY,
  kUpperZ,
  kLeftSquareBracket,
  kBackslash,
  kRightSquareBracket,
  kCaret,
  kUnderscore,
  kBacktick,
  kLowerA,
  kLowerB,
  kLowerC,
  kLowerD,
  kLowerE,
  kLowerF,
  kLowerG,
  kLowerH,
  kLowerI,
  kLowerJ,
  kLowerK,
  kLowerL,
  kLowerM,
  kLowerN,
  kLowerO,
  kLowerP,
  kLowerQ,
  kLowerR,
  kLowerS,
  kLowerT,
  kLowerU,
  kLowerV,
  kLowerW,
  kLowerX,
  kLowerY,
  kLowerZ,
  kLeftCurlyBrace,
  kVerticalBar,
  kRightCurlyBrace,
  kTilde,
  kDelete,
  COUNT
};
// Define the enum to cstr conversion.
constexpr const char* eSrcCharToCStr(eSrcChar e) noexcept {
  using enum eSrcChar;
  switch (e) {
    case kNull:
      return "kNull";
    case kStartOfHeader:
      return "kStartOfHeader";
    case kStartOfText:
      return "kStartOfText";
    case kEndOfText:
      return "kEndOfText";
    case kEndOfTransmission:
      return "kEndOfTransmission";
    case kEnquiry:
      return "kEnquiry";
    case kAcknowledge:
      return "kAcknowledge";
    case kBell:
      return "kBell";
    case kBackspace:
      return "kBackspace";
    case kHorizontalTab:
      return "kHorizontalTab";
    case kLineFeed:
      return "kLineFeed";
    case kVerticalTab:
      return "kVerticalTab";
    case kFormFeed:
      return "kFormFeed";
    case kCarriageReturn:
      return "kCarriageReturn";
    case kShiftOut:
      return "kShiftOut";
    case kShiftIn:
      return "kShiftIn";
    case kDataLinkEscape:
      return "kDataLinkEscape";
    case kDeviceControl1:
      return "kDeviceControl1";
    case kDeviceControl2:
      return "kDeviceControl2";
    case kDeviceControl3:
      return "kDeviceControl3";
    case kDeviceControl4:
      return "kDeviceControl4";
    case kNegativeAcknowledge:
      return "kNegativeAcknowledge";
    case kSynchronousIdle:
      return "kSynchronousIdle";
    case kEndOfTransmissionBlock:
      return "kEndOfTransmissionBlock";
    case kCancel:
      return "kCancel";
    case kEndOfMedium:
      return "kEndOfMedium";
    case kSubstitute:
      return "kSubstitute";
    case kEscape:
      return "kEscape";
    case kFileSeparator:
      return "kFileSeparator";
    case kGroupSeparator:
      return "kGroupSeparator";
    case kRecordSeparator:
      return "kRecordSeparator";
    case kUnitSeparator:
      return "kUnitSeparator";
    case kSpace:
      return "kSpace";
    case kExclamationMark:
      return "kExclamationMark";
    case kQuotation:
      return "kQuotation";
    case kHash:
      return "kHash";
    case kDollarSign:
      return "kDollarSign";
    case kPercentSign:
      return "kPercentSign";
    case kAmpersand:
      return "kAmpersand";
    case kApostrophe:
      return "kApostrophe";
    case kLeftParenthesis:
      return "kLeftParenthesis";
    case kRightParenthesis:
      return "kRightParenthesis";
    case kAsterisk:
      return "kAsterisk";
    case kPlusSign:
      return "kPlusSign";
    case kComma:
      return "kComma";
    case kHyphenMinus:
      return "kHyphenMinus";
    case kPeriod:
      return "kPeriod";
    case kSolidus:
      return "kSolidus";
    case kZero:
      return "kZero";
    case kOne:
      return "kOne";
    case kTwo:
      return "kTwo";
    case kThree:
      return "kThree";
    case kFour:
      return "kFour";
    case kFive:
      return "kFive";
    case kSix:
      return "kSix";
    case kSeven:
      return "kSeven";
    case kEight:
      return "kEight";
    case kNine:
      return "kNine";
    case kColon:
      return "kColon";
    case kSemicolon:
      return "kSemicolon";
    case kLessThan:
      return "kLessThan";
    case kEqualSign:
      return "kEqualSign";
    case kGreaterThan:
      return "kGreaterThan";
    case kQuestionMark:
      return "kQuestionMark";
    case kAtSign:
      return "kAtSign";
    case kUpperA:
      return "kUpperA";
    case kUpperB:
      return "kUpperB";
    case kUpperC:
      return "kUpperC";
    case kUpperD:
      return "kUpperD";
    case kUpperE:
      return "kUpperE";
    case kUpperF:
      return "kUpperF";
    case kUpperG:
      return "kUpperG";
    case kUpperH:
      return "kUpperH";
    case kUpperI:
      return "kUpperI";
    case kUpperJ:
      return "kUpperJ";
    case kUpperK:
      return "kUpperK";
    case kUpperL:
      return "kUpperL";
    case kUpperM:
      return "kUpperM";
    case kUpperN:
      return "kUpperN";
    case kUpperO:
      return "kUpperO";
    case kUpperP:
      return "kUpperP";
    case kUpperQ:
      return "kUpperQ";
    case kUpperR:
      return "kUpperR";
    case kUpperS:
      return "kUpperS";
    case kUpperT:
      return "kUpperT";
    case kUpperU:
      return "kUpperU";
    case kUpperV:
      return "kUpperV";
    case kUpperW:
      return "kUpperW";
    case kUpperX:
      return "kUpperX";
    case kUpperY:
      return "kUpperY";
    case kUpperZ:
      return "kUpperZ";
    case kLeftSquareBracket:
      return "kLeftSquareBracket";
    case kBackslash:
      return "kBackslash";
    case kRightSquareBracket:
      return "kRightSquareBracket";
    case kCaret:
      return "kCaret";
    case kUnderscore:
      return "kUnderscore";
    case kBacktick:
      return "kBacktick";
    case kLowerA:
      return "kLowerA";
    case kLowerB:
      return "kLowerB";
    case kLowerC:
      return "kLowerC";
    case kLowerD:
      return "kLowerD";
    case kLowerE:
      return "kLowerE";
    case kLowerF:
      return "kLowerF";
    case kLowerG:
      return "kLowerG";
    case kLowerH:
      return "kLowerH";
    case kLowerI:
      return "kLowerI";
    case kLowerJ:
      return "kLowerJ";
    case kLowerK:
      return "kLowerK";
    case kLowerL:
      return "kLowerL";
    case kLowerM:
      return "kLowerM";
    case kLowerN:
      return "kLowerN";
    case kLowerO:
      return "kLowerO";
    case kLowerP:
      return "kLowerP";
    case kLowerQ:
      return "kLowerQ";
    case kLowerR:
      return "kLowerR";
    case kLowerS:
      return "kLowerS";
    case kLowerT:
      return "kLowerT";
    case kLowerU:
      return "kLowerU";
    case kLowerV:
      return "kLowerV";
    case kLowerW:
      return "kLowerW";
    case kLowerX:
      return "kLowerX";
    case kLowerY:
      return "kLowerY";
    case kLowerZ:
      return "kLowerZ";
    case kLeftCurlyBrace:
      return "kLeftCurlyBrace";
    case kVerticalBar:
      return "kVerticalBar";
    case kRightCurlyBrace:
      return "kRightCurlyBrace";
    case kTilde:
      return "kTilde";
    case kDelete:
      return "kDelete";
    case COUNT:
      return "COUNT";
    default:
      return "<invalid>";
  }
};
// Assert enum to cstr conversion.
#define CND_STATIC_ASSERT_ENUM_TO_CSTR_eSrcChar(x) CND_MM_STATIC_ASSERT_ENUM_TO_CSTR(x, eSrcChar, eSrcCharToCStr)

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