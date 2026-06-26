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
/// @brief cnd::corevals::toolset::eToolchain enum definition.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
// clang-format off
#include "ccapi/CommonCppApi.hpp"
#include "compiler_utils/ReflectedMetaEnum.hpp"
// clang-format on

/// @addtogroup cnd_compiler_corevals
/// @{

namespace cnd::corevals::toolset {
/// Define applied enum cnd::corevals::toolset::eToolchain
#define CND_APPLIED_ENUM_eToolchain(m, sep, pre, lst) \
  pre m(INVALID)                                      \
  sep m(NONE)                                         \
  sep m(Custom)                                       \
  sep m(Embedded)                                     \
  sep m(ACC)                                          \
  sep m(AltiumMicroBlazeC)                            \
  sep m(AltiumCToHardware)                            \
  sep m(AmsterdamCompilerKit)                         \
  sep m(ARM)                                          \
  sep m(AztecC)                                       \
  sep m(Bordland)                                     \
  sep m(CC65)                                         \
  sep m(Clang)                                        \
  sep m(Comeau)                                       \
  sep m(Compaq)                                       \
  sep m(ConvexC)                                      \
  sep m(CompCert)                                     \
  sep m(CrayC)                                        \
  sep m(Diab)                                         \
  sep m(DiceC)                                        \
  sep m(DigitalMars)                                  \
  sep m(DignusSystems)                                \
  sep m(DJGPP)                                        \
  sep m(EDGFrontend)                                  \
  sep m(EKOPath)                                      \
  sep m(Fujitsu)                                      \
  sep m(GCC)                                          \
  sep m(GreenHill)                                    \
  sep m(HPAnsiC)                                      \
  sep m(HPa)                                          \
  sep m(IAR)                                          \
  sep m(IBMXL)                                        \
  sep m(IBMXLLegacy)                                  \
  sep m(IBMXLMainframe)                               \
  sep m(ImageCraftC)                                  \
  sep m(Intel)                                        \
  sep m(Kai)                                          \
  sep m(KEILCARM)                                     \
  sep m(KEILC166)                                     \
  sep m(KEILC51)                                      \
  sep m(LCC)                                          \
  sep m(LLVM)                                         \
  sep m(MetaWareHigh)                                 \
  sep m(MetrowerksCodeWarrior)                        \
  sep m(MSVC)                                         \
  sep m(Microtec)                                     \
  sep m(MicrowayNDPC)                                 \
  sep m(MinGW)                                        \
  sep m(MinGW64)                                      \
  sep m(MIPSpro)                                      \
  sep m(MiracleC)                                     \
  sep m(MPW)                                          \
  sep m(NorcroftC)                                    \
  sep m(NWCC)                                         \
  sep m(Open64)                                       \
  sep m(OracleProCPrecompiler)                        \
  sep m(OracleSolarisStudio)                          \
  sep m(PacificC)                                     \
  sep m(Palm)                                         \
  sep m(PellesC)                                      \
  sep m(PortlandGroup)                                \
  sep m(Renesas)                                      \
  sep m(SASC)                                         \
  sep m(SCOOpenServer)                                \
  sep m(SmallDeviceC)                                 \
  sep m(SN)                                           \
  sep m(StratusVOSC)                                  \
  sep m(SymantecC)                                    \
  sep m(TenDRA)                                       \
  sep m(TexasInstruments)                             \
  sep m(THINKC)                                       \
  sep m(TinyC)                                        \
  sep m(Turbo)                                        \
  sep m(Ultimate)                                     \
  sep m(USLC)                                         \
  sep m(VBCC)                                         \
  sep m(WatcomC)                                      \
  sep m(ZortechC)                                     \
  lst

// Define the enum.
enum class eToolchain {
  kINVALID,
  kNONE,
  kCustom,
  kEmbedded,
  kACC,
  kAltiumMicroBlazeC,
  kAltiumCToHardware,
  kAmsterdamCompilerKit,
  kARM,
  kAztecC,
  kBordland,
  kCC65,
  kClang,
  kComeau,
  kCompaq,
  kConvexC,
  kCompCert,
  kCrayC,
  kDiab,
  kDiceC,
  kDigitalMars,
  kDignusSystems,
  kDJGPP,
  kEDGFrontend,
  kEKOPath,
  kFujitsu,
  kGCC,
  kGreenHill,
  kHPAnsiC,
  kHPa,
  kIAR,
  kIBMXL,
  kIBMXLLegacy,
  kIBMXLMainframe,
  kImageCraftC,
  kIntel,
  kKai,
  kKEILCARM,
  kKEILC166,
  kKEILC51,
  kLCC,
  kLLVM,
  kMetaWareHigh,
  kMetrowerksCodeWarrior,
  kMSVC,
  kMicrotec,
  kMicrowayNDPC,
  kMinGW,
  kMinGW64,
  kMIPSpro,
  kMiracleC,
  kMPW,
  kNorcroftC,
  kNWCC,
  kOpen64,
  kOracleProCPrecompiler,
  kOracleSolarisStudio,
  kPacificC,
  kPalm,
  kPellesC,
  kPortlandGroup,
  kRenesas,
  kSASC,
  kSCOOpenServer,
  kSmallDeviceC,
  kSN,
  kStratusVOSC,
  kSymantecC,
  kTenDRA,
  kTexasInstruments,
  kTHINKC,
  kTinyC,
  kTurbo,
  kUltimate,
  kUSLC,
  kVBCC,
  kWatcomC,
  kZortechC,
  COUNT
};
// Define the enum to cstr conversion.
constexpr const char* eToolchainToCStr(eToolchain e) noexcept {
  using enum eToolchain;
  switch (e) {
    case kINVALID:
      return "kINVALID";
    case kNONE:
      return "kNONE";
    case kCustom:
      return "kCustom";
    case kEmbedded:
      return "kEmbedded";
    case kACC:
      return "kACC";
    case kAltiumMicroBlazeC:
      return "kAltiumMicroBlazeC";
    case kAltiumCToHardware:
      return "kAltiumCToHardware";
    case kAmsterdamCompilerKit:
      return "kAmsterdamCompilerKit";
    case kARM:
      return "kARM";
    case kAztecC:
      return "kAztecC";
    case kBordland:
      return "kBordland";
    case kCC65:
      return "kCC65";
    case kClang:
      return "kClang";
    case kComeau:
      return "kComeau";
    case kCompaq:
      return "kCompaq";
    case kConvexC:
      return "kConvexC";
    case kCompCert:
      return "kCompCert";
    case kCrayC:
      return "kCrayC";
    case kDiab:
      return "kDiab";
    case kDiceC:
      return "kDiceC";
    case kDigitalMars:
      return "kDigitalMars";
    case kDignusSystems:
      return "kDignusSystems";
    case kDJGPP:
      return "kDJGPP";
    case kEDGFrontend:
      return "kEDGFrontend";
    case kEKOPath:
      return "kEKOPath";
    case kFujitsu:
      return "kFujitsu";
    case kGCC:
      return "kGCC";
    case kGreenHill:
      return "kGreenHill";
    case kHPAnsiC:
      return "kHPAnsiC";
    case kHPa:
      return "kHPa";
    case kIAR:
      return "kIAR";
    case kIBMXL:
      return "kIBMXL";
    case kIBMXLLegacy:
      return "kIBMXLLegacy";
    case kIBMXLMainframe:
      return "kIBMXLMainframe";
    case kImageCraftC:
      return "kImageCraftC";
    case kIntel:
      return "kIntel";
    case kKai:
      return "kKai";
    case kKEILCARM:
      return "kKEILCARM";
    case kKEILC166:
      return "kKEILC166";
    case kKEILC51:
      return "kKEILC51";
    case kLCC:
      return "kLCC";
    case kLLVM:
      return "kLLVM";
    case kMetaWareHigh:
      return "kMetaWareHigh";
    case kMetrowerksCodeWarrior:
      return "kMetrowerksCodeWarrior";
    case kMSVC:
      return "kMSVC";
    case kMicrotec:
      return "kMicrotec";
    case kMicrowayNDPC:
      return "kMicrowayNDPC";
    case kMinGW:
      return "kMinGW";
    case kMinGW64:
      return "kMinGW64";
    case kMIPSpro:
      return "kMIPSpro";
    case kMiracleC:
      return "kMiracleC";
    case kMPW:
      return "kMPW";
    case kNorcroftC:
      return "kNorcroftC";
    case kNWCC:
      return "kNWCC";
    case kOpen64:
      return "kOpen64";
    case kOracleProCPrecompiler:
      return "kOracleProCPrecompiler";
    case kOracleSolarisStudio:
      return "kOracleSolarisStudio";
    case kPacificC:
      return "kPacificC";
    case kPalm:
      return "kPalm";
    case kPellesC:
      return "kPellesC";
    case kPortlandGroup:
      return "kPortlandGroup";
    case kRenesas:
      return "kRenesas";
    case kSASC:
      return "kSASC";
    case kSCOOpenServer:
      return "kSCOOpenServer";
    case kSmallDeviceC:
      return "kSmallDeviceC";
    case kSN:
      return "kSN";
    case kStratusVOSC:
      return "kStratusVOSC";
    case kSymantecC:
      return "kSymantecC";
    case kTenDRA:
      return "kTenDRA";
    case kTexasInstruments:
      return "kTexasInstruments";
    case kTHINKC:
      return "kTHINKC";
    case kTinyC:
      return "kTinyC";
    case kTurbo:
      return "kTurbo";
    case kUltimate:
      return "kUltimate";
    case kUSLC:
      return "kUSLC";
    case kVBCC:
      return "kVBCC";
    case kWatcomC:
      return "kWatcomC";
    case kZortechC:
      return "kZortechC";
    case COUNT:
      return "COUNT";
    default:
      return "<invalid>";
  }
};
// Assert enum to cstr conversion.
#define CND_STATIC_ASSERT_ENUM_TO_CSTR_eToolchain(x) CND_MM_STATIC_ASSERT_ENUM_TO_CSTR(x, eToolchain, eToolchainToCStr)

CND_APPLIED_ENUM_eToolchain(CND_STATIC_ASSERT_ENUM_TO_CSTR_eToolchain, , , );
static_assert(cxx::StrEq(eToolchainToCStr(eToolchain::COUNT), "COUNT"));
#undef CND_STATIC_ASSERT_ENUM_TO_CSTR_eToolchain
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