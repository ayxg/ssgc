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
/// @brief cnd::corevals::toolset::eProcArch enum definition.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
// clang-format off
#include "ccapi/CommonCppApi.hpp"
#include "compiler_utils/ReflectedMetaEnum.hpp"
// clang-format on

/// @addtogroup cnd_compiler_corevals
/// @{

namespace cnd::corevals::toolset {
/// Define applied enum cnd::corevals::toolset::eProcArch
#define CND_APPLIED_ENUM_eProcArch(m, sep, pre, lst) \
  pre m(INVALID)                                     \
  sep m(NONE)                                        \
  sep m(Custom)                                      \
  sep m(Embedded)                                    \
  sep m(Alpha)                                       \
  sep m(AMD64)                                       \
  sep m(ARM)                                         \
  sep m(ARM64)                                       \
  sep m(Blackfin)                                    \
  sep m(Convex)                                      \
  sep m(Epiphany)                                    \
  sep m(HppaRISC)                                    \
  sep m(Intel86)                                     \
  sep m(IntelItanium)                                \
  sep m(Motorola68k)                                 \
  sep m(MIPS)                                        \
  sep m(PowerPC)                                     \
  sep m(Pyramid9810)                                 \
  sep m(RS6000)                                      \
  sep m(SPARC)                                       \
  sep m(SuperH)                                      \
  sep m(SystemZ)                                     \
  sep m(TMS320)                                      \
  sep m(TMS470)                                      \
  lst

// Define the enum.
enum class eProcArch {
  kINVALID,
  kNONE,
  kCustom,
  kEmbedded,
  kAlpha,
  kAMD64,
  kARM,
  kARM64,
  kBlackfin,
  kConvex,
  kEpiphany,
  kHppaRISC,
  kIntel86,
  kIntelItanium,
  kMotorola68k,
  kMIPS,
  kPowerPC,
  kPyramid9810,
  kRS6000,
  kSPARC,
  kSuperH,
  kSystemZ,
  kTMS320,
  kTMS470,
  COUNT
};

// Define the enum to cstr conversion.
constexpr const char* eProcArchToCStr(eProcArch e) noexcept {
  using enum eProcArch;
  switch (e) {
    case kINVALID:
      return "kINVALID";
    case kNONE:
      return "kNONE";
    case kCustom:
      return "kCustom";
    case kEmbedded:
      return "kEmbedded";
    case kAlpha:
      return "kAlpha";
    case kAMD64:
      return "kAMD64";
    case kARM:
      return "kARM";
    case kARM64:
      return "kARM64";
    case kBlackfin:
      return "kBlackfin";
    case kConvex:
      return "kConvex";
    case kEpiphany:
      return "kEpiphany";
    case kHppaRISC:
      return "kHppaRISC";
    case kIntel86:
      return "kIntel86";
    case kIntelItanium:
      return "kIntelItanium";
    case kMotorola68k:
      return "kMotorola68k";
    case kMIPS:
      return "kMIPS";
    case kPowerPC:
      return "kPowerPC";
    case kPyramid9810:
      return "kPyramid9810";
    case kRS6000:
      return "kRS6000";
    case kSPARC:
      return "kSPARC";
    case kSuperH:
      return "kSuperH";
    case kSystemZ:
      return "kSystemZ";
    case kTMS320:
      return "kTMS320";
    case kTMS470:
      return "kTMS470";
    case COUNT:
      return "COUNT";
    default:
      return "<invalid>";
  }
};

// Assert enum to cstr conversion.
#define CND_STATIC_ASSERT_ENUM_TO_CSTR_eProcArch(x) CND_MM_STATIC_ASSERT_ENUM_TO_CSTR(x, eProcArch, eProcArchToCStr)
CND_APPLIED_ENUM_eProcArch(CND_STATIC_ASSERT_ENUM_TO_CSTR_eProcArch, , , );
static_assert(cxx::StrEq(eProcArchToCStr(eProcArch::COUNT), "COUNT"));
#undef CND_STATIC_ASSERT_ENUM_TO_CSTR_eProcArch

}  // namespace cnd::corevals::toolset

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