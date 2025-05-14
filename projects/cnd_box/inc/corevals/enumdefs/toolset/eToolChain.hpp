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
#include "use_ccapi.hpp"
#include "corevals/reflected_meta_enum.hpp"
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
CND_MM_CREATE_ENUM_FROM_APPLIED_ENUM_UNTYPED(eToolchain, eToolchain);
// Define the enum to cstr conversion.
CND_MM_CREATE_ENUMTOCSTR_FROM_ENUM_LIST(eToolchainToCStr, eToolchain, eToolchain);
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