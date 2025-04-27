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
/// @brief cnd::corevals::toolset::eOpSys enum definition.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
// clang-format off
#include "use_ccapi.hpp"
#include "corevals/reflected_meta_enum.hpp"
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
CND_MM_CREATE_ENUM_FROM_APPLIED_ENUM_UNTYPED(eOpSys, eOpSys);
// Define the enum to cstr conversion.
CND_MM_CREATE_ENUMTOCSTR_FROM_ENUM_LIST(eOpSysToCStr, eOpSys, eOpSys);
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
// Licensed under the Apache License, Version 2.0(the "License"); you may not use this file except in compliance with
// the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on
// an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////