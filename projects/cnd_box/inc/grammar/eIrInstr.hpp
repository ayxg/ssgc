///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2025 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup cnd_compiler_corevals
/// @brief eIrInstr enum definition.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
// clang-format off
#include "ccapi/CommonCppApi.hpp"
#include "compiler_utils/ReflectedMetaEnum.hpp"
// clang-format on

/// @addtogroup cnd_compiler_corevals
/// @{

namespace cnd::corevals::grammar {

/// Define applied enum cnd::corevals::grammar::eIrInstr
#define CND_APPLIED_ENUM_eIrInstr(m, sep, pre, lst) \
  m(StartUp)                                    \
  sep m(Restart)                                    \
  sep m(ShutDown)                                   \
  sep m(LoadBootDrive)                              \
  sep m(InstallBootDriver)                          \
  sep m(LoadBootDriver)                             \
  sep m(LoadDriver)                                 \
  sep m(InstallDriver)                              \
  sep m(Label)                                      \
  sep m(Goto)                                       \
  sep m(Block)                                      \
  sep m(Loop)                                       \
  sep m(If)                                         \
  sep m(Branch)                                     \
  sep m(Call)                                       \
  sep m(Return)                                     \
  sep m(Phi)                                        \
  sep m(Select)                                     \
  sep m(Unreachable)                                \
  sep m(Nothing)                                    \
  sep m(Add)                                        \
  sep m(Subtract)                                   \
  sep m(Muliply)                                    \
  sep m(DivideSigned)                               \
  sep m(Negate)                                     \
  sep m(DivideUnsigned)                             \
  sep m(RemainderSigned)                            \
  sep m(RemainderUnsigned)                          \
  sep m(Not)                                        \
  sep m(Increment)                                  \
  sep m(Decrement)                                  \
  sep m(ShiftLeft)                                  \
  sep m(ShiftRightSigned)                           \
  sep m(ShiftRightUnsigned)                         \
  sep m(RotateLeft)                                 \
  sep m(RotateRight)                                \
  sep m(BitwiseAnd)                                 \
  sep m(BitwiseOr)                                  \
  sep m(BitwiseXor)                                 \
  sep m(BitwiseNot)                                 \
  sep m(CountLeadingZeroes)                         \
  sep m(CountTrailingZeroes)                        \
  sep m(PopulationCount)                            \
  sep m(LogicalAnd)                                 \
  sep m(LogicalOr)                                  \
  sep m(EqualZero)                                  \
  sep m(Equal)                                      \
  sep m(NotEqual)                                   \
  sep m(LessThan)                                   \
  sep m(GreaterThan)                                \
  sep m(LessThanEqual)                              \
  sep m(GreaterThanEqual)                           \
  sep m(Extend)                                     \
  sep m(Wrap)                                       \
  sep m(Promote)                                    \
  sep m(Demote)                                     \
  sep m(Convert)                                    \
  sep m(Truncate)                                   \
  sep m(Reinterpret)                                \
  sep m(Min)                                        \
  sep m(Max)                                        \
  sep m(Nearest)                                    \
  sep m(Ciel)                                       \
  sep m(Floor)                                      \
  sep m(Trucate)                                    \
  sep m(Absolute)                                   \
  sep m(SquareRoot)                                 \
  sep m(CopySign)                                   \
  lst

// Define the enum.
enum class eIrInstr {
  kStartUp,
  kRestart,
  kShutDown,
  kLoadBootDrive,
  kInstallBootDriver,
  kLoadBootDriver,
  kLoadDriver,
  kInstallDriver,
  kLabel,
  kGoto,
  kBlock,
  kLoop,
  kIf,
  kBranch,
  kCall,
  kReturn,
  kPhi,
  kSelect,
  kUnreachable,
  kNothing,
  kAdd,
  kSubtract,
  kMuliply,
  kDivideSigned,
  kNegate,
  kDivideUnsigned,
  kRemainderSigned,
  kRemainderUnsigned,
  kNot,
  kIncrement,
  kDecrement,
  kShiftLeft,
  kShiftRightSigned,
  kShiftRightUnsigned,
  kRotateLeft,
  kRotateRight,
  kBitwiseAnd,
  kBitwiseOr,
  kBitwiseXor,
  kBitwiseNot,
  kCountLeadingZeroes,
  kCountTrailingZeroes,
  kPopulationCount,
  kLogicalAnd,
  kLogicalOr,
  kEqualZero,
  kEqual,
  kNotEqual,
  kLessThan,
  kGreaterThan,
  kLessThanEqual,
  kGreaterThanEqual,
  kExtend,
  kWrap,
  kPromote,
  kDemote,
  kConvert,
  kTruncate,
  kReinterpret,
  kMin,
  kMax,
  kNearest,
  kCiel,
  kFloor,
  kTrucate,
  kAbsolute,
  kSquareRoot,
  kCopySign,
  COUNT
};
// Define the enum to cstr conversion.
constexpr const char* eIrInstrToCStr(eIrInstr e) noexcept {
  using enum eIrInstr;
  switch (e) {
    case kStartUp:
      return "kStartUp";
    case kRestart:
      return "kRestart";
    case kShutDown:
      return "kShutDown";
    case kLoadBootDrive:
      return "kLoadBootDrive";
    case kInstallBootDriver:
      return "kInstallBootDriver";
    case kLoadBootDriver:
      return "kLoadBootDriver";
    case kLoadDriver:
      return "kLoadDriver";
    case kInstallDriver:
      return "kInstallDriver";
    case kLabel:
      return "kLabel";
    case kGoto:
      return "kGoto";
    case kBlock:
      return "kBlock";
    case kLoop:
      return "kLoop";
    case kIf:
      return "kIf";
    case kBranch:
      return "kBranch";
    case kCall:
      return "kCall";
    case kReturn:
      return "kReturn";
    case kPhi:
      return "kPhi";
    case kSelect:
      return "kSelect";
    case kUnreachable:
      return "kUnreachable";
    case kNothing:
      return "kNothing";
    case kAdd:
      return "kAdd";
    case kSubtract:
      return "kSubtract";
    case kMuliply:
      return "kMuliply";
    case kDivideSigned:
      return "kDivideSigned";
    case kNegate:
      return "kNegate";
    case kDivideUnsigned:
      return "kDivideUnsigned";
    case kRemainderSigned:
      return "kRemainderSigned";
    case kRemainderUnsigned:
      return "kRemainderUnsigned";
    case kNot:
      return "kNot";
    case kIncrement:
      return "kIncrement";
    case kDecrement:
      return "kDecrement";
    case kShiftLeft:
      return "kShiftLeft";
    case kShiftRightSigned:
      return "kShiftRightSigned";
    case kShiftRightUnsigned:
      return "kShiftRightUnsigned";
    case kRotateLeft:
      return "kRotateLeft";
    case kRotateRight:
      return "kRotateRight";
    case kBitwiseAnd:
      return "kBitwiseAnd";
    case kBitwiseOr:
      return "kBitwiseOr";
    case kBitwiseXor:
      return "kBitwiseXor";
    case kBitwiseNot:
      return "kBitwiseNot";
    case kCountLeadingZeroes:
      return "kCountLeadingZeroes";
    case kCountTrailingZeroes:
      return "kCountTrailingZeroes";
    case kPopulationCount:
      return "kPopulationCount";
    case kLogicalAnd:
      return "kLogicalAnd";
    case kLogicalOr:
      return "kLogicalOr";
    case kEqualZero:
      return "kEqualZero";
    case kEqual:
      return "kEqual";
    case kNotEqual:
      return "kNotEqual";
    case kLessThan:
      return "kLessThan";
    case kGreaterThan:
      return "kGreaterThan";
    case kLessThanEqual:
      return "kLessThanEqual";
    case kGreaterThanEqual:
      return "kGreaterThanEqual";
    case kExtend:
      return "kExtend";
    case kWrap:
      return "kWrap";
    case kPromote:
      return "kPromote";
    case kDemote:
      return "kDemote";
    case kConvert:
      return "kConvert";
    case kTruncate:
      return "kTruncate";
    case kReinterpret:
      return "kReinterpret";
    case kMin:
      return "kMin";
    case kMax:
      return "kMax";
    case kNearest:
      return "kNearest";
    case kCiel:
      return "kCiel";
    case kFloor:
      return "kFloor";
    case kTrucate:
      return "kTrucate";
    case kAbsolute:
      return "kAbsolute";
    case kSquareRoot:
      return "kSquareRoot";
    case kCopySign:
      return "kCopySign";
    case COUNT:
      return "COUNT";
    default:
      return "<invalid>";
  }
};
// Assert enum to cstr conversion.
#define CND_STATIC_ASSERT_ENUM_TO_CSTR_eIrInstr(x) CND_MM_STATIC_ASSERT_ENUM_TO_CSTR(x, eIrInstr, eIrInstrToCStr)

CND_APPLIED_ENUM_eIrInstr(CND_STATIC_ASSERT_ENUM_TO_CSTR_eIrInstr, , , );
static_assert(cxx::StrEq(eIrInstrToCStr(eIrInstr::COUNT), "COUNT"));
#undef CND_STATIC_ASSERT_ENUM_TO_CSTR_eIrInstr
}  // namespace cnd::corevals::grammar

/// @} // end of cnd_compiler_corevals

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2025 Anton Yashchenko
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