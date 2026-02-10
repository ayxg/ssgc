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
/// @brief eClCliFlag enum definition.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
// clang-format off
#include "ccapi/CommonCppApi.hpp"
#include "compiler_utils/ReflectedMetaEnum.hpp"
// clang-format on

/// @addtogroup cnd_compiler_corevals
/// @{

namespace cnd::driver {
#define CND_MM_AENUM_ENTRY(s, p, m) p m(s)

/// Define applied enum cnd::driver::eFlag
#define CND_APPLIED_ENUM_eFlag(m, s, p, l)     \
  CND_MM_AENUM_ENTRY(INVALID, p, m)                  \
  CND_MM_AENUM_ENTRY(NONE, s, m)                     \
  CND_MM_AENUM_ENTRY(ModeComp, s, m)                 \
  CND_MM_AENUM_ENTRY(ModeDev, s, m)                  \
  CND_MM_AENUM_ENTRY(ModeHelp, s, m)                 \
  CND_MM_AENUM_ENTRY(ModeVersion, s, m)              \
  CND_MM_AENUM_ENTRY(Sources, s, m)              \
  CND_MM_AENUM_ENTRY(Define, s, m)                   \
  CND_MM_AENUM_ENTRY(OutDir, s, m)                   \
  CND_MM_AENUM_ENTRY(AuxDir, s, m)                   \
  CND_MM_AENUM_ENTRY(WorkDir, s, m)                  \
  CND_MM_AENUM_ENTRY(SrcDirs, s, m)                  \
  CND_MM_AENUM_ENTRY(LibDirs, s, m)                  \
  CND_MM_AENUM_ENTRY(ResDirs, s, m)                  \
  CND_MM_AENUM_ENTRY(ExeDirs, s, m)                  \
  CND_MM_AENUM_ENTRY(SrcFiles, s, m)                 \
  CND_MM_AENUM_ENTRY(LibFiles, s, m)                 \
  CND_MM_AENUM_ENTRY(ExeFiles, s, m)                 \
  CND_MM_AENUM_ENTRY(ResFiles, s, m)                 \
  CND_MM_AENUM_ENTRY(NoOverwrite, s, m)              \
  CND_MM_AENUM_ENTRY(CompIoSilent, s, m)             \
  CND_MM_AENUM_ENTRY(CompIoVerbose, s, m)            \
  CND_MM_AENUM_ENTRY(CompIoDebug, s, m)              \
  CND_MM_AENUM_ENTRY(CompIoStd, s, m)                \
  CND_MM_AENUM_ENTRY(CompIoTrace, s, m)              \
  CND_MM_AENUM_ENTRY(CompStdinRedir, s, m)           \
  CND_MM_AENUM_ENTRY(CompStdoutRedir, s, m)          \
  CND_MM_AENUM_ENTRY(CompStderrRedir, s, m)          \
  CND_MM_AENUM_ENTRY(DriverIoSilent, s, m)           \
  CND_MM_AENUM_ENTRY(DriverIoVerbose, s, m)          \
  CND_MM_AENUM_ENTRY(DriverIoDebug, s, m)            \
  CND_MM_AENUM_ENTRY(DriverIoStd, s, m)              \
  CND_MM_AENUM_ENTRY(DriverIoTrace, s, m)            \
  CND_MM_AENUM_ENTRY(DriverStdinRedir, s, m)         \
  CND_MM_AENUM_ENTRY(DriverStdoutRedir, s, m)        \
  CND_MM_AENUM_ENTRY(DriverStderrRedir, s, m)        \
  CND_MM_AENUM_ENTRY(Dump, s, m)                     \
  CND_MM_AENUM_ENTRY(HostLinker, s, m)               \
  CND_MM_AENUM_ENTRY(HostLinkerType, s, m)           \
  CND_MM_AENUM_ENTRY(HostLinkerVersion, s, m)        \
  CND_MM_AENUM_ENTRY(HostLinkerDriver, s, m)         \
  CND_MM_AENUM_ENTRY(HostLinkerDriverType, s, m)     \
  CND_MM_AENUM_ENTRY(HostLinkerDriverExternal, s, m) \
  CND_MM_AENUM_ENTRY(HostLinkerImplicitLibs, s, m)   \
  CND_MM_AENUM_ENTRY(HostCxxCl, s, m)                \
  CND_MM_AENUM_ENTRY(HostCxxClType, s, m)            \
  CND_MM_AENUM_ENTRY(HostCxxClVersion, s, m)         \
  CND_MM_AENUM_ENTRY(HostCxxClDriver, s, m)          \
  CND_MM_AENUM_ENTRY(HostCxxClDriverType, s, m)      \
  CND_MM_AENUM_ENTRY(HostCxxClDriverExternal, s, m)  \
  CND_MM_AENUM_ENTRY(HostCxxPredefs, s, m)           \
  CND_MM_AENUM_ENTRY(HostCxxSources, s, m)           \
  CND_MM_AENUM_ENTRY(HostCxxIncludeDirs, s, m)       \
  CND_MM_AENUM_ENTRY(TargetHost, s, m)               \
  CND_MM_AENUM_ENTRY(ProcArch, s, m)                 \
  CND_MM_AENUM_ENTRY(OpSys, s, m)                    \
  CND_MM_AENUM_ENTRY(Linker, s, m)                   \
  CND_MM_AENUM_ENTRY(LinkerType, s, m)               \
  CND_MM_AENUM_ENTRY(LinkerVersion, s, m)            \
  CND_MM_AENUM_ENTRY(LinkerDriver, s, m)             \
  CND_MM_AENUM_ENTRY(LinkerDriverType, s, m)         \
  CND_MM_AENUM_ENTRY(LinkerDriverExternal, s, m)     \
  CND_MM_AENUM_ENTRY(LinkerImplicitLibs, s, m)       \
  CND_MM_AENUM_ENTRY(CxxCl, s, m)                    \
  CND_MM_AENUM_ENTRY(CxxClType, s, m)                \
  CND_MM_AENUM_ENTRY(CxxClVersion, s, m)             \
  CND_MM_AENUM_ENTRY(CxxClDriver, s, m)              \
  CND_MM_AENUM_ENTRY(CxxClDriverType, s, m)          \
  CND_MM_AENUM_ENTRY(CxxClDriverExternal, s, m)      \
  CND_MM_AENUM_ENTRY(CxxPredefs, s, m)               \
  CND_MM_AENUM_ENTRY(CxxSources, s, m)               \
  CND_MM_AENUM_ENTRY(CxxIncludeDirs, s, m)           \
  l

// Define the enum.
CND_MM_CREATE_ENUM_FROM_APPLIED_ENUM_UNTYPED(eFlag, eFlag);

// Define the enum to cstr conversion.
CND_MM_CREATE_ENUMTOCSTR_FROM_ENUM_LIST(eFlagToCStr, eFlag, eFlag);

// Assert enum to cstr conversion.
#define CND_STATIC_ASSERT_ENUM_TO_CSTR_eFlag(x) \
  CND_MM_STATIC_ASSERT_ENUM_TO_CSTR(x, eFlag, eFlagToCStr)

CND_APPLIED_ENUM_eFlag(CND_STATIC_ASSERT_ENUM_TO_CSTR_eFlag, , , );
static_assert(cxx::StrEq(eFlagToCStr(eFlag::COUNT), "COUNT"));

#undef CND_STATIC_ASSERT_ENUM_TO_CSTR_eClCliFlag
}  // namespace cnd::driver

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