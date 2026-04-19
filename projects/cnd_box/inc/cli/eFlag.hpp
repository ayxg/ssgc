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
enum class eFlag {
  kINVALID,
  kNONE,
  kModeComp,
  kModeDev,
  kModeHelp,
  kModeVersion,
  kSources,
  kDefine,
  kOutDir,
  kAuxDir,
  kWorkDir,
  kSrcDirs,
  kLibDirs,
  kResDirs,
  kExeDirs,
  kSrcFiles,
  kLibFiles,
  kExeFiles,
  kResFiles,
  kNoOverwrite,
  kCompIoSilent,
  kCompIoVerbose,
  kCompIoDebug,
  kCompIoStd,
  kCompIoTrace,
  kCompStdinRedir,
  kCompStdoutRedir,
  kCompStderrRedir,
  kDriverIoSilent,
  kDriverIoVerbose,
  kDriverIoDebug,
  kDriverIoStd,
  kDriverIoTrace,
  kDriverStdinRedir,
  kDriverStdoutRedir,
  kDriverStderrRedir,
  kDump,
  kHostLinker,
  kHostLinkerType,
  kHostLinkerVersion,
  kHostLinkerDriver,
  kHostLinkerDriverType,
  kHostLinkerDriverExternal,
  kHostLinkerImplicitLibs,
  kHostCxxCl,
  kHostCxxClType,
  kHostCxxClVersion,
  kHostCxxClDriver,
  kHostCxxClDriverType,
  kHostCxxClDriverExternal,
  kHostCxxPredefs,
  kHostCxxSources,
  kHostCxxIncludeDirs,
  kTargetHost,
  kProcArch,
  kOpSys,
  kLinker,
  kLinkerType,
  kLinkerVersion,
  kLinkerDriver,
  kLinkerDriverType,
  kLinkerDriverExternal,
  kLinkerImplicitLibs,
  kCxxCl,
  kCxxClType,
  kCxxClVersion,
  kCxxClDriver,
  kCxxClDriverType,
  kCxxClDriverExternal,
  kCxxPredefs,
  kCxxSources,
  kCxxIncludeDirs,
  COUNT
};

// Define the enum to cstr conversion.
constexpr const char* eFlagToCStr(eFlag e) noexcept {
  using enum eFlag;
  switch (e) {
    case kINVALID:
      return "kINVALID";
    case kNONE:
      return "kNONE";
    case kModeComp:
      return "kModeComp";
    case kModeDev:
      return "kModeDev";
    case kModeHelp:
      return "kModeHelp";
    case kModeVersion:
      return "kModeVersion";
    case kSources:
      return "kSources";
    case kDefine:
      return "kDefine";
    case kOutDir:
      return "kOutDir";
    case kAuxDir:
      return "kAuxDir";
    case kWorkDir:
      return "kWorkDir";
    case kSrcDirs:
      return "kSrcDirs";
    case kLibDirs:
      return "kLibDirs";
    case kResDirs:
      return "kResDirs";
    case kExeDirs:
      return "kExeDirs";
    case kSrcFiles:
      return "kSrcFiles";
    case kLibFiles:
      return "kLibFiles";
    case kExeFiles:
      return "kExeFiles";
    case kResFiles:
      return "kResFiles";
    case kNoOverwrite:
      return "kNoOverwrite";
    case kCompIoSilent:
      return "kCompIoSilent";
    case kCompIoVerbose:
      return "kCompIoVerbose";
    case kCompIoDebug:
      return "kCompIoDebug";
    case kCompIoStd:
      return "kCompIoStd";
    case kCompIoTrace:
      return "kCompIoTrace";
    case kCompStdinRedir:
      return "kCompStdinRedir";
    case kCompStdoutRedir:
      return "kCompStdoutRedir";
    case kCompStderrRedir:
      return "kCompStderrRedir";
    case kDriverIoSilent:
      return "kDriverIoSilent";
    case kDriverIoVerbose:
      return "kDriverIoVerbose";
    case kDriverIoDebug:
      return "kDriverIoDebug";
    case kDriverIoStd:
      return "kDriverIoStd";
    case kDriverIoTrace:
      return "kDriverIoTrace";
    case kDriverStdinRedir:
      return "kDriverStdinRedir";
    case kDriverStdoutRedir:
      return "kDriverStdoutRedir";
    case kDriverStderrRedir:
      return "kDriverStderrRedir";
    case kDump:
      return "kDump";
    case kHostLinker:
      return "kHostLinker";
    case kHostLinkerType:
      return "kHostLinkerType";
    case kHostLinkerVersion:
      return "kHostLinkerVersion";
    case kHostLinkerDriver:
      return "kHostLinkerDriver";
    case kHostLinkerDriverType:
      return "kHostLinkerDriverType";
    case kHostLinkerDriverExternal:
      return "kHostLinkerDriverExternal";
    case kHostLinkerImplicitLibs:
      return "kHostLinkerImplicitLibs";
    case kHostCxxCl:
      return "kHostCxxCl";
    case kHostCxxClType:
      return "kHostCxxClType";
    case kHostCxxClVersion:
      return "kHostCxxClVersion";
    case kHostCxxClDriver:
      return "kHostCxxClDriver";
    case kHostCxxClDriverType:
      return "kHostCxxClDriverType";
    case kHostCxxClDriverExternal:
      return "kHostCxxClDriverExternal";
    case kHostCxxPredefs:
      return "kHostCxxPredefs";
    case kHostCxxSources:
      return "kHostCxxSources";
    case kHostCxxIncludeDirs:
      return "kHostCxxIncludeDirs";
    case kTargetHost:
      return "kTargetHost";
    case kProcArch:
      return "kProcArch";
    case kOpSys:
      return "kOpSys";
    case kLinker:
      return "kLinker";
    case kLinkerType:
      return "kLinkerType";
    case kLinkerVersion:
      return "kLinkerVersion";
    case kLinkerDriver:
      return "kLinkerDriver";
    case kLinkerDriverType:
      return "kLinkerDriverType";
    case kLinkerDriverExternal:
      return "kLinkerDriverExternal";
    case kLinkerImplicitLibs:
      return "kLinkerImplicitLibs";
    case kCxxCl:
      return "kCxxCl";
    case kCxxClType:
      return "kCxxClType";
    case kCxxClVersion:
      return "kCxxClVersion";
    case kCxxClDriver:
      return "kCxxClDriver";
    case kCxxClDriverType:
      return "kCxxClDriverType";
    case kCxxClDriverExternal:
      return "kCxxClDriverExternal";
    case kCxxPredefs:
      return "kCxxPredefs";
    case kCxxSources:
      return "kCxxSources";
    case kCxxIncludeDirs:
      return "kCxxIncludeDirs";
    case COUNT:
      return "COUNT";
    default:
      return "<invalid>";
  }
};

// Assert enum to cstr conversion.
#define CND_STATIC_ASSERT_ENUM_TO_CSTR_eFlag(x) \
  CND_MM_STATIC_ASSERT_ENUM_TO_CSTR(x, eFlag, eFlagToCStr)

static_assert(cxx::StrEq(eFlagToCStr(eFlag::kINVALID), "kINVALID"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kNONE), "kNONE"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kModeComp), "kModeComp"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kModeDev), "kModeDev"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kModeHelp), "kModeHelp"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kModeVersion), "kModeVersion"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kSources), "kSources"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kDefine), "kDefine"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kOutDir), "kOutDir"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kAuxDir), "kAuxDir"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kWorkDir), "kWorkDir"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kSrcDirs), "kSrcDirs"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kLibDirs), "kLibDirs"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kResDirs), "kResDirs"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kExeDirs), "kExeDirs"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kSrcFiles), "kSrcFiles"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kLibFiles), "kLibFiles"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kExeFiles), "kExeFiles"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kResFiles), "kResFiles"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kNoOverwrite), "kNoOverwrite"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kCompIoSilent), "kCompIoSilent"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kCompIoVerbose), "kCompIoVerbose"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kCompIoDebug), "kCompIoDebug"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kCompIoStd), "kCompIoStd"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kCompIoTrace), "kCompIoTrace"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kCompStdinRedir), "kCompStdinRedir"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kCompStdoutRedir), "kCompStdoutRedir"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kCompStderrRedir), "kCompStderrRedir"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kDriverIoSilent), "kDriverIoSilent"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kDriverIoVerbose), "kDriverIoVerbose"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kDriverIoDebug), "kDriverIoDebug"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kDriverIoStd), "kDriverIoStd"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kDriverIoTrace), "kDriverIoTrace"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kDriverStdinRedir), "kDriverStdinRedir"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kDriverStdoutRedir), "kDriverStdoutRedir"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kDriverStderrRedir), "kDriverStderrRedir"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kDump), "kDump"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kHostLinker), "kHostLinker"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kHostLinkerType), "kHostLinkerType"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kHostLinkerVersion), "kHostLinkerVersion"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kHostLinkerDriver), "kHostLinkerDriver"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kHostLinkerDriverType), "kHostLinkerDriverType"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kHostLinkerDriverExternal), "kHostLinkerDriverExternal"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kHostLinkerImplicitLibs), "kHostLinkerImplicitLibs"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kHostCxxCl), "kHostCxxCl"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kHostCxxClType), "kHostCxxClType"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kHostCxxClVersion), "kHostCxxClVersion"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kHostCxxClDriver), "kHostCxxClDriver"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kHostCxxClDriverType), "kHostCxxClDriverType"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kHostCxxClDriverExternal), "kHostCxxClDriverExternal"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kHostCxxPredefs), "kHostCxxPredefs"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kHostCxxSources), "kHostCxxSources"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kHostCxxIncludeDirs), "kHostCxxIncludeDirs"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kTargetHost), "kTargetHost"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kProcArch), "kProcArch"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kOpSys), "kOpSys"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kLinker), "kLinker"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kLinkerType), "kLinkerType"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kLinkerVersion), "kLinkerVersion"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kLinkerDriver), "kLinkerDriver"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kLinkerDriverType), "kLinkerDriverType"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kLinkerDriverExternal), "kLinkerDriverExternal"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kLinkerImplicitLibs), "kLinkerImplicitLibs"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kCxxCl), "kCxxCl"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kCxxClType), "kCxxClType"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kCxxClVersion), "kCxxClVersion"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kCxxClDriver), "kCxxClDriver"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kCxxClDriverType), "kCxxClDriverType"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kCxxClDriverExternal), "kCxxClDriverExternal"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kCxxPredefs), "kCxxPredefs"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kCxxSources), "kCxxSources"));
static_assert(cxx::StrEq(eFlagToCStr(eFlag::kCxxIncludeDirs), "kCxxIncludeDirs"));
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