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
/// @brief cnd::corevals::toolset::eOpSys enum definition.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
// clang-format off
#include "ccapi/CommonCppApi.hpp"
#include "compiler_utils/ReflectedMetaEnum.hpp"
// clang-format on

/// @addtogroup cnd_compiler_corevals
/// @{

namespace cnd::corevals::toolset {
/// Define applied enum cnd::corevals::toolset::eOpSys
#define CND_APPLIED_ENUM_eOpSys(m, sep, pre, lst) \
  pre m(INVALID)                                  \
  sep m(NONE)                                     \
  sep m(Custom)                                   \
  sep m(Embedded)                                 \
  sep m(AIX)                                      \
  sep m(Android)                                  \
  sep m(AmdahlUTS)                                \
  sep m(AmigaOS)                                  \
  sep m(ApolloAEGIS)                              \
  sep m(ApolloDomainOS)                           \
  sep m(Bada)                                     \
  sep m(BeOS)                                     \
  sep m(BlueGene)                                 \
  sep m(BSDOpen)                                  \
  sep m(BSDNet)                                   \
  sep m(BSD)                                      \
  sep m(ConvexOS)                                 \
  sep m(CygwinEnvironment)                        \
  sep m(DGUX)                                     \
  sep m(DragonFly)                                \
  sep m(DYNIXPtx)                                 \
  sep m(ECos)                                     \
  sep m(EMXEnvironment)                           \
  sep m(FreeBSD)                                  \
  sep m(GNUHurd)                                  \
  sep m(GNUkFreeBSD)                              \
  sep m(GNULinux)                                 \
  sep m(HIUXMPP)                                  \
  sep m(HPUX)                                     \
  sep m(IBMOS400)                                 \
  sep m(INTEGRITY)                                \
  sep m(InterixEnvironment)                       \
  sep m(IRIX)                                     \
  sep m(IOS)                                      \
  sep m(Linux)                                    \
  sep m(LynxOS)                                   \
  sep m(MacOS)                                    \
  sep m(MicrowareOS9)                             \
  sep m(MINIX)                                    \
  sep m(MorphOS)                                  \
  sep m(MPEiX)                                    \
  sep m(MSDOS)                                    \
  sep m(NetBSD)                                   \
  sep m(NonStop)                                  \
  sep m(NucleusRTOS)                              \
  sep m(OpenBSD)                                  \
  sep m(OS2)                                      \
  sep m(PalmOS)                                   \
  sep m(Plan9)                                    \
  sep m(PyramidDCOSx)                             \
  sep m(QNX)                                      \
  sep m(ReliantUNIX)                              \
  sep m(SCOOpenServer)                            \
  sep m(Solaris)                                  \
  sep m(StratusVOS)                               \
  sep m(SVR4Environment)                          \
  sep m(Syllable)                                 \
  sep m(SymbianOS)                                \
  sep m(Tru64OSF1)                                \
  sep m(Ultrix)                                   \
  sep m(UNICOS)                                   \
  sep m(UNICOSmp)                                 \
  sep m(UNIXEnvironment)                          \
  sep m(UnixWare)                                 \
  sep m(UWinEnvironment)                          \
  sep m(VMS)                                      \
  sep m(VxWorks)                                  \
  sep m(Windows)                                  \
  sep m(WindowsCE)                                \
  sep m(WindUEnvironment)                         \
  sep m(zOS)                                      \
  lst

// Define the enum.
enum class eOpSys {
  kINVALID,
  kNONE,
  kCustom,
  kEmbedded,
  kAIX,
  kAndroid,
  kAmdahlUTS,
  kAmigaOS,
  kApolloAEGIS,
  kApolloDomainOS,
  kBada,
  kBeOS,
  kBlueGene,
  kBSDOpen,
  kBSDNet,
  kBSD,
  kConvexOS,
  kCygwinEnvironment,
  kDGUX,
  kDragonFly,
  kDYNIXPtx,
  kECos,
  kEMXEnvironment,
  kFreeBSD,
  kGNUHurd,
  kGNUkFreeBSD,
  kGNULinux,
  kHIUXMPP,
  kHPUX,
  kIBMOS400,
  kINTEGRITY,
  kInterixEnvironment,
  kIRIX,
  kIOS,
  kLinux,
  kLynxOS,
  kMacOS,
  kMicrowareOS9,
  kMINIX,
  kMorphOS,
  kMPEiX,
  kMSDOS,
  kNetBSD,
  kNonStop,
  kNucleusRTOS,
  kOpenBSD,
  kOS2,
  kPalmOS,
  kPlan9,
  kPyramidDCOSx,
  kQNX,
  kReliantUNIX,
  kSCOOpenServer,
  kSolaris,
  kStratusVOS,
  kSVR4Environment,
  kSyllable,
  kSymbianOS,
  kTru64OSF1,
  kUltrix,
  kUNICOS,
  kUNICOSmp,
  kUNIXEnvironment,
  kUnixWare,
  kUWinEnvironment,
  kVMS,
  kVxWorks,
  kWindows,
  kWindowsCE,
  kWindUEnvironment,
  kzOS,
  COUNT
};
// Define the enum to cstr conversion.
constexpr const char* eOpSysToCStr(eOpSys e) noexcept {
  using enum eOpSys;
  switch (e) {
    case kINVALID:
      return "kINVALID";
    case kNONE:
      return "kNONE";
    case kCustom:
      return "kCustom";
    case kEmbedded:
      return "kEmbedded";
    case kAIX:
      return "kAIX";
    case kAndroid:
      return "kAndroid";
    case kAmdahlUTS:
      return "kAmdahlUTS";
    case kAmigaOS:
      return "kAmigaOS";
    case kApolloAEGIS:
      return "kApolloAEGIS";
    case kApolloDomainOS:
      return "kApolloDomainOS";
    case kBada:
      return "kBada";
    case kBeOS:
      return "kBeOS";
    case kBlueGene:
      return "kBlueGene";
    case kBSDOpen:
      return "kBSDOpen";
    case kBSDNet:
      return "kBSDNet";
    case kBSD:
      return "kBSD";
    case kConvexOS:
      return "kConvexOS";
    case kCygwinEnvironment:
      return "kCygwinEnvironment";
    case kDGUX:
      return "kDGUX";
    case kDragonFly:
      return "kDragonFly";
    case kDYNIXPtx:
      return "kDYNIXPtx";
    case kECos:
      return "kECos";
    case kEMXEnvironment:
      return "kEMXEnvironment";
    case kFreeBSD:
      return "kFreeBSD";
    case kGNUHurd:
      return "kGNUHurd";
    case kGNUkFreeBSD:
      return "kGNUkFreeBSD";
    case kGNULinux:
      return "kGNULinux";
    case kHIUXMPP:
      return "kHIUXMPP";
    case kHPUX:
      return "kHPUX";
    case kIBMOS400:
      return "kIBMOS400";
    case kINTEGRITY:
      return "kINTEGRITY";
    case kInterixEnvironment:
      return "kInterixEnvironment";
    case kIRIX:
      return "kIRIX";
    case kIOS:
      return "kIOS";
    case kLinux:
      return "kLinux";
    case kLynxOS:
      return "kLynxOS";
    case kMacOS:
      return "kMacOS";
    case kMicrowareOS9:
      return "kMicrowareOS9";
    case kMINIX:
      return "kMINIX";
    case kMorphOS:
      return "kMorphOS";
    case kMPEiX:
      return "kMPEiX";
    case kMSDOS:
      return "kMSDOS";
    case kNetBSD:
      return "kNetBSD";
    case kNonStop:
      return "kNonStop";
    case kNucleusRTOS:
      return "kNucleusRTOS";
    case kOpenBSD:
      return "kOpenBSD";
    case kOS2:
      return "kOS2";
    case kPalmOS:
      return "kPalmOS";
    case kPlan9:
      return "kPlan9";
    case kPyramidDCOSx:
      return "kPyramidDCOSx";
    case kQNX:
      return "kQNX";
    case kReliantUNIX:
      return "kReliantUNIX";
    case kSCOOpenServer:
      return "kSCOOpenServer";
    case kSolaris:
      return "kSolaris";
    case kStratusVOS:
      return "kStratusVOS";
    case kSVR4Environment:
      return "kSVR4Environment";
    case kSyllable:
      return "kSyllable";
    case kSymbianOS:
      return "kSymbianOS";
    case kTru64OSF1:
      return "kTru64OSF1";
    case kUltrix:
      return "kUltrix";
    case kUNICOS:
      return "kUNICOS";
    case kUNICOSmp:
      return "kUNICOSmp";
    case kUNIXEnvironment:
      return "kUNIXEnvironment";
    case kUnixWare:
      return "kUnixWare";
    case kUWinEnvironment:
      return "kUWinEnvironment";
    case kVMS:
      return "kVMS";
    case kVxWorks:
      return "kVxWorks";
    case kWindows:
      return "kWindows";
    case kWindowsCE:
      return "kWindowsCE";
    case kWindUEnvironment:
      return "kWindUEnvironment";
    case kzOS:
      return "kzOS";
    case COUNT:
      return "COUNT";
    default:
      return "<invalid>";
  }
};
// Assert enum to cstr conversion.
#define CND_STATIC_ASSERT_ENUM_TO_CSTR_eOpSys(x) CND_MM_STATIC_ASSERT_ENUM_TO_CSTR(x, eOpSys, eOpSysToCStr)

CND_APPLIED_ENUM_eOpSys(CND_STATIC_ASSERT_ENUM_TO_CSTR_eOpSys, , , );
static_assert(cxx::StrEq(eOpSysToCStr(eOpSys::COUNT), "COUNT"));
#undef CND_STATIC_ASSERT_ENUM_TO_CSTR_eOpSys
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