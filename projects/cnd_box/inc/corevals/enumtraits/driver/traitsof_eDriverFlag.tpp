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
/// @brief Traits of eClCliFlag
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "corevals/enumtraits/driver/traitsof_eDriverFlag.hpp"
// clang-format off
#include "use_ccapi.hpp"
#include "corevals/enumdefs/driver/eDriverFlag.hpp"
#include "corevals/enumdefs/driver/eDriverFlagInterp.hpp"
// clang-format on

/// @addtogroup cnd_compiler_corevals
/// @{

namespace cnd::corevals::driver {

constexpr eDriverFlagInterp GetDriverFlagInterp(eDriverFlag flag) noexcept {
#define CND_MM_LOCAL_CASE(n, lt) \
  case eDriverFlag::k##n:         \
    return eDriverFlagInterp::k##lt

  switch (flag) {
    CND_MM_LOCAL_CASE(ModeComp, ModeSwitch);
    CND_MM_LOCAL_CASE(ModeDev, ModeSwitch);
    CND_MM_LOCAL_CASE(ModeHelp, ModeSwitch);
    CND_MM_LOCAL_CASE(ModeVersion, ModeSwitch);
    CND_MM_LOCAL_CASE(Define, VarDef);
    CND_MM_LOCAL_CASE(OutDir, SingleArg);
    CND_MM_LOCAL_CASE(AuxDir, SingleArg);
    CND_MM_LOCAL_CASE(WorkDir, SingleArg);
    CND_MM_LOCAL_CASE(SrcDirs, MultiArg);
    CND_MM_LOCAL_CASE(LibDirs, MultiArg);
    CND_MM_LOCAL_CASE(ResDirs, MultiArg);
    CND_MM_LOCAL_CASE(ExeDirs, MultiArg);
    CND_MM_LOCAL_CASE(SrcFiles, MultiArg);
    CND_MM_LOCAL_CASE(LibFiles, MultiArg);
    CND_MM_LOCAL_CASE(ExeFiles, MultiArg);
    CND_MM_LOCAL_CASE(ResFiles, MultiArg);
    CND_MM_LOCAL_CASE(NoOverwrite, OnOff);
    CND_MM_LOCAL_CASE(CompIoSilent, OnOff);
    CND_MM_LOCAL_CASE(CompIoVerbose, OnOff);
    CND_MM_LOCAL_CASE(CompIoDebug, OnOff);
    CND_MM_LOCAL_CASE(CompIoStd, OnOff);
    CND_MM_LOCAL_CASE(CompIoTrace, OnOff);
    CND_MM_LOCAL_CASE(CompStdinRedir, SingleArg);
    CND_MM_LOCAL_CASE(CompStdoutRedir, SingleArg);
    CND_MM_LOCAL_CASE(CompStderrRedir, SingleArg);
    CND_MM_LOCAL_CASE(DriverIoSilent, OnOff);
    CND_MM_LOCAL_CASE(DriverIoVerbose, OnOff);
    CND_MM_LOCAL_CASE(DriverIoDebug, OnOff);
    CND_MM_LOCAL_CASE(DriverIoStd, OnOff);
    CND_MM_LOCAL_CASE(DriverIoTrace, OnOff);
    CND_MM_LOCAL_CASE(DriverStdinRedir, SingleArg);
    CND_MM_LOCAL_CASE(DriverStdoutRedir, SingleArg);
    CND_MM_LOCAL_CASE(DriverStderrRedir, SingleArg);
    CND_MM_LOCAL_CASE(Dump, TwoArg);
    CND_MM_LOCAL_CASE(HostLinker, SingleArg);
    CND_MM_LOCAL_CASE(HostLinkerType, SingleArg);
    CND_MM_LOCAL_CASE(HostLinkerVersion, SingleArg);
    CND_MM_LOCAL_CASE(HostLinkerDriver, SingleArg);
    CND_MM_LOCAL_CASE(HostLinkerDriverType, SingleArg);
    CND_MM_LOCAL_CASE(HostLinkerDriverExternal, OnOff);
    CND_MM_LOCAL_CASE(HostLinkerImplicitLibs, MultiArg);
    CND_MM_LOCAL_CASE(HostCxxCl, SingleArg);
    CND_MM_LOCAL_CASE(HostCxxClType, SingleArg);
    CND_MM_LOCAL_CASE(HostCxxClVersion, SingleArg);
    CND_MM_LOCAL_CASE(HostCxxClDriver, SingleArg);
    CND_MM_LOCAL_CASE(HostCxxClDriverType, SingleArg);
    CND_MM_LOCAL_CASE(HostCxxClDriverExternal, OnOff);
    CND_MM_LOCAL_CASE(HostCxxPredefs, MultiArg);
    CND_MM_LOCAL_CASE(HostCxxSources, MultiArg);
    CND_MM_LOCAL_CASE(HostCxxIncludeDirs, MultiArg);
    CND_MM_LOCAL_CASE(TargetHost, SingleArg);
    CND_MM_LOCAL_CASE(ProcArch, SingleArg);
    CND_MM_LOCAL_CASE(OpSys, SingleArg);
    CND_MM_LOCAL_CASE(Linker, SingleArg);
    CND_MM_LOCAL_CASE(LinkerType, SingleArg);
    CND_MM_LOCAL_CASE(LinkerVersion, SingleArg);
    CND_MM_LOCAL_CASE(LinkerDriver, SingleArg);
    CND_MM_LOCAL_CASE(LinkerDriverType, SingleArg);
    CND_MM_LOCAL_CASE(LinkerDriverExternal, OnOff);
    CND_MM_LOCAL_CASE(LinkerImplicitLibs, MultiArg);
    CND_MM_LOCAL_CASE(CxxCl, SingleArg);
    CND_MM_LOCAL_CASE(CxxClType, SingleArg);
    CND_MM_LOCAL_CASE(CxxClVersion, SingleArg);
    CND_MM_LOCAL_CASE(CxxClDriver, SingleArg);
    CND_MM_LOCAL_CASE(CxxClDriverType, SingleArg);
    CND_MM_LOCAL_CASE(CxxClDriverExternal, OnOff);
    CND_MM_LOCAL_CASE(CxxPredefs, MultiArg);
    CND_MM_LOCAL_CASE(CxxSources, MultiArg);
    CND_MM_LOCAL_CASE(CxxIncludeDirs, MultiArg); 
    default:
      return eDriverFlagInterp::kNONE;
  }
#undef CND_MM_LOCAL_CASE
}

constexpr StrView GetDriverFlagShortSymbol(eDriverFlag flag) noexcept {
#define CND_MM_LOCAL_CASE(n, lt) \
  case eDriverFlag::k##n:         \
    return lt

  switch (flag) {
    CND_MM_LOCAL_CASE(ModeComp, "c");
    CND_MM_LOCAL_CASE(ModeDev, "z");
    CND_MM_LOCAL_CASE(ModeHelp, "h");
    CND_MM_LOCAL_CASE(ModeVersion, "v");
    CND_MM_LOCAL_CASE(Define, "D");
    CND_MM_LOCAL_CASE(OutDir, "O");
    CND_MM_LOCAL_CASE(AuxDir, "A");
    CND_MM_LOCAL_CASE(WorkDir, "W");
    CND_MM_LOCAL_CASE(SrcFiles, "S");
    CND_MM_LOCAL_CASE(LibFiles, "L");
    CND_MM_LOCAL_CASE(ExeFiles, "E");
    CND_MM_LOCAL_CASE(ResFiles, "R");
    default:
      return "";
  }

#undef CND_MM_LOCAL_CASE
};

constexpr StrView GetDriverFlagLongSymbol(eDriverFlag flag) noexcept {
#define CND_MM_LOCAL_CASE(n, lt) \
  case eDriverFlag::k##n:         \
    return lt

  switch (flag) {
    CND_MM_LOCAL_CASE(ModeComp, "comp");
    CND_MM_LOCAL_CASE(ModeDev, "dev");
    CND_MM_LOCAL_CASE(ModeHelp, "help");
    CND_MM_LOCAL_CASE(ModeVersion, "version");
    CND_MM_LOCAL_CASE(Define, "define");
    CND_MM_LOCAL_CASE(OutDir, "out-dir");
    CND_MM_LOCAL_CASE(AuxDir, "aux-dir");
    CND_MM_LOCAL_CASE(WorkDir, "work-dir");
    CND_MM_LOCAL_CASE(SrcDirs, "src-dirs");
    CND_MM_LOCAL_CASE(LibDirs, "lib-dirs");
    CND_MM_LOCAL_CASE(ResDirs, "res-dirs");
    CND_MM_LOCAL_CASE(ExeDirs, "exe-dirs");
    CND_MM_LOCAL_CASE(SrcFiles, "src-files");
    CND_MM_LOCAL_CASE(LibFiles, "lib-files");
    CND_MM_LOCAL_CASE(ExeFiles, "exe-files");
    CND_MM_LOCAL_CASE(ResFiles, "res-files");
    CND_MM_LOCAL_CASE(NoOverwrite, "no-overwrite");
    CND_MM_LOCAL_CASE(CompIoSilent, "comp-io-silent");
    CND_MM_LOCAL_CASE(CompIoVerbose, "comp-io-verbose");
    CND_MM_LOCAL_CASE(CompIoDebug, "comp-io-debug");
    CND_MM_LOCAL_CASE(CompIoStd, "comp-io-std");
    CND_MM_LOCAL_CASE(CompIoTrace, "comp-io-trace");
    CND_MM_LOCAL_CASE(CompStdinRedir, "comp-stdin-redir");
    CND_MM_LOCAL_CASE(CompStdoutRedir, "comp-stdout-redir");
    CND_MM_LOCAL_CASE(CompStderrRedir, "comp-stderr-redir");
    CND_MM_LOCAL_CASE(DriverIoSilent, "driver-io-silent");
    CND_MM_LOCAL_CASE(DriverIoVerbose, "driver-io-verbose");
    CND_MM_LOCAL_CASE(DriverIoDebug, "driver-io-debug");
    CND_MM_LOCAL_CASE(DriverIoStd, "driver-io-std");
    CND_MM_LOCAL_CASE(DriverIoTrace, "driver-io-trace");
    CND_MM_LOCAL_CASE(DriverStdinRedir, "driver-stdin-redir");
    CND_MM_LOCAL_CASE(DriverStdoutRedir, "driver-stdout-redir");
    CND_MM_LOCAL_CASE(DriverStderrRedir, "driver-stderr-redir");
    CND_MM_LOCAL_CASE(Dump, "dump");
    CND_MM_LOCAL_CASE(HostLinker, "host-linker");
    CND_MM_LOCAL_CASE(HostLinkerType, "host-linker-type");
    CND_MM_LOCAL_CASE(HostLinkerVersion, "host-linker-version");
    CND_MM_LOCAL_CASE(HostLinkerDriver, "host-linker-driver");
    CND_MM_LOCAL_CASE(HostLinkerDriverType, "host-linker-driver-type");
    CND_MM_LOCAL_CASE(HostLinkerDriverExternal, "host-linker-driver-external");
    CND_MM_LOCAL_CASE(HostLinkerImplicitLibs, "host-linker-implicit-libs");
    CND_MM_LOCAL_CASE(HostCxxCl, "host-cxx-cl");
    CND_MM_LOCAL_CASE(HostCxxClType, "host-cxx-cl-type");
    CND_MM_LOCAL_CASE(HostCxxClVersion, "host-cxx-cl-version");
    CND_MM_LOCAL_CASE(HostCxxClDriver, "host-cxx-cl-driver");
    CND_MM_LOCAL_CASE(HostCxxClDriverType, "host-cxx-cl-driver-type");
    CND_MM_LOCAL_CASE(HostCxxClDriverExternal, "host-cxx-cl-driver-external");
    CND_MM_LOCAL_CASE(HostCxxPredefs, "host-cxx-predefs");
    CND_MM_LOCAL_CASE(HostCxxSources, "host-cxx-sources");
    CND_MM_LOCAL_CASE(HostCxxIncludeDirs, "host-cxx-include-dirs");
    CND_MM_LOCAL_CASE(TargetHost, "target-host");
    CND_MM_LOCAL_CASE(ProcArch, "proc-arch");
    CND_MM_LOCAL_CASE(OpSys, "op-sys");
    CND_MM_LOCAL_CASE(Linker, "linker");
    CND_MM_LOCAL_CASE(LinkerType, "linker-type");
    CND_MM_LOCAL_CASE(LinkerVersion, "linker-version");
    CND_MM_LOCAL_CASE(LinkerDriver, "linker-driver");
    CND_MM_LOCAL_CASE(LinkerDriverType, "linker-driver-type");
    CND_MM_LOCAL_CASE(LinkerDriverExternal, "linker-driver-external");
    CND_MM_LOCAL_CASE(LinkerImplicitLibs, "linker-implicit-libs");
    CND_MM_LOCAL_CASE(CxxCl, "cxx-cl");
    CND_MM_LOCAL_CASE(CxxClType, "cxx-cl-type");
    CND_MM_LOCAL_CASE(CxxClVersion, "cxx-cl-version");
    CND_MM_LOCAL_CASE(CxxClDriver, "cxx-cl-driver");
    CND_MM_LOCAL_CASE(CxxClDriverType, "cxx-cl-driver-type");
    CND_MM_LOCAL_CASE(CxxClDriverExternal, "cxx-cl-driver-external");
    CND_MM_LOCAL_CASE(CxxPredefs, "cxx-predefs");
    CND_MM_LOCAL_CASE(CxxSources, "cxx-sources");
    CND_MM_LOCAL_CASE(CxxIncludeDirs, "cxx-include-dirs"); 
    default:
      return "";
  }
#undef CND_MM_LOCAL_CASE
};

constexpr eDriverFlag GetDriverFlagFromStr(StrView str) noexcept {
#define CND_MM_LOCAL_CASE(n, s) else if (str == s) return k##n

  using enum eDriverFlag;
  if (str.empty()) return kNONE;  // Empty string passed.
  CND_MM_LOCAL_CASE(ModeComp, "comp");
  CND_MM_LOCAL_CASE(ModeDev, "dev");
  CND_MM_LOCAL_CASE(ModeHelp, "help");
  CND_MM_LOCAL_CASE(ModeVersion, "version");
  CND_MM_LOCAL_CASE(Define, "define");
  CND_MM_LOCAL_CASE(OutDir, "out-dir");
  CND_MM_LOCAL_CASE(AuxDir, "aux-dir");
  CND_MM_LOCAL_CASE(WorkDir, "work-dir");
  CND_MM_LOCAL_CASE(SrcDirs, "src-dirs");
  CND_MM_LOCAL_CASE(LibDirs, "lib-dirs");
  CND_MM_LOCAL_CASE(ResDirs, "res-dirs");
  CND_MM_LOCAL_CASE(ExeDirs, "exe-dirs");
  CND_MM_LOCAL_CASE(SrcFiles, "src-files");
  CND_MM_LOCAL_CASE(LibFiles, "lib-files");
  CND_MM_LOCAL_CASE(ExeFiles, "exe-files");
  CND_MM_LOCAL_CASE(ResFiles, "res-files");
  CND_MM_LOCAL_CASE(NoOverwrite, "no-overwrite");
  CND_MM_LOCAL_CASE(CompIoSilent, "comp-io-silent");
  CND_MM_LOCAL_CASE(CompIoVerbose, "comp-io-verbose");
  CND_MM_LOCAL_CASE(CompIoDebug, "comp-io-debug");
  CND_MM_LOCAL_CASE(CompIoStd, "comp-io-std");
  CND_MM_LOCAL_CASE(CompIoTrace, "comp-io-trace");
  CND_MM_LOCAL_CASE(CompStdinRedir, "comp-stdin-redir");
  CND_MM_LOCAL_CASE(CompStdoutRedir, "comp-stdout-redir");
  CND_MM_LOCAL_CASE(CompStderrRedir, "comp-stderr-redir");
  CND_MM_LOCAL_CASE(DriverIoSilent, "driver-io-silent");
  CND_MM_LOCAL_CASE(DriverIoVerbose, "driver-io-verbose");
  CND_MM_LOCAL_CASE(DriverIoDebug, "driver-io-debug");
  CND_MM_LOCAL_CASE(DriverIoStd, "driver-io-std");
  CND_MM_LOCAL_CASE(DriverIoTrace, "driver-io-trace");
  CND_MM_LOCAL_CASE(DriverStdinRedir, "driver-stdin-redir");
  CND_MM_LOCAL_CASE(DriverStdoutRedir, "driver-stdout-redir");
  CND_MM_LOCAL_CASE(DriverStderrRedir, "driver-stderr-redir");
  CND_MM_LOCAL_CASE(Dump, "dump");
  CND_MM_LOCAL_CASE(HostLinker, "host-linker");
  CND_MM_LOCAL_CASE(HostLinkerType, "host-linker-type");
  CND_MM_LOCAL_CASE(HostLinkerVersion, "host-linker-version");
  CND_MM_LOCAL_CASE(HostLinkerDriver, "host-linker-driver");
  CND_MM_LOCAL_CASE(HostLinkerDriverType, "host-linker-driver-type");
  CND_MM_LOCAL_CASE(HostLinkerDriverExternal, "host-linker-driver-external");
  CND_MM_LOCAL_CASE(HostLinkerImplicitLibs, "host-linker-implicit-libs");
  CND_MM_LOCAL_CASE(HostCxxCl, "host-cxx-cl");
  CND_MM_LOCAL_CASE(HostCxxClType, "host-cxx-cl-type");
  CND_MM_LOCAL_CASE(HostCxxClVersion, "host-cxx-cl-version");
  CND_MM_LOCAL_CASE(HostCxxClDriver, "host-cxx-cl-driver");
  CND_MM_LOCAL_CASE(HostCxxClDriverType, "host-cxx-cl-driver-type");
  CND_MM_LOCAL_CASE(HostCxxClDriverExternal, "host-cxx-cl-driver-external");
  CND_MM_LOCAL_CASE(HostCxxPredefs, "host-cxx-predefs");
  CND_MM_LOCAL_CASE(HostCxxSources, "host-cxx-sources");
  CND_MM_LOCAL_CASE(HostCxxIncludeDirs, "host-cxx-include-dirs");
  CND_MM_LOCAL_CASE(TargetHost, "target-host");
  CND_MM_LOCAL_CASE(ProcArch, "proc-arch");
  CND_MM_LOCAL_CASE(OpSys, "op-sys");
  CND_MM_LOCAL_CASE(Linker, "linker");
  CND_MM_LOCAL_CASE(LinkerType, "linker-type");
  CND_MM_LOCAL_CASE(LinkerVersion, "linker-version");
  CND_MM_LOCAL_CASE(LinkerDriver, "linker-driver");
  CND_MM_LOCAL_CASE(LinkerDriverType, "linker-driver-type");
  CND_MM_LOCAL_CASE(LinkerDriverExternal, "linker-driver-external");
  CND_MM_LOCAL_CASE(LinkerImplicitLibs, "linker-implicit-libs");
  CND_MM_LOCAL_CASE(CxxCl, "cxx-cl");
  CND_MM_LOCAL_CASE(CxxClType, "cxx-cl-type");
  CND_MM_LOCAL_CASE(CxxClVersion, "cxx-cl-version");
  CND_MM_LOCAL_CASE(CxxClDriver, "cxx-cl-driver");
  CND_MM_LOCAL_CASE(CxxClDriverType, "cxx-cl-driver-type");
  CND_MM_LOCAL_CASE(CxxClDriverExternal, "cxx-cl-driver-external");
  CND_MM_LOCAL_CASE(CxxPredefs, "cxx-predefs");
  CND_MM_LOCAL_CASE(CxxSources, "cxx-sources");
  CND_MM_LOCAL_CASE(CxxIncludeDirs, "cxx-include-dirs"); 
  else return kNONE;  // No match.

#undef CND_MM_LOCAL_CASE
};

constexpr eDriverFlag GetDriverFlagFromChar(Char ch) noexcept {
#define CND_MM_LOCAL_CASE(n, c) \
  case c:        \
    return k##n
    using enum eDriverFlag;
  switch (ch) {
      CND_MM_LOCAL_CASE(ModeComp, 'c');
    CND_MM_LOCAL_CASE(ModeDev, 'z');
      CND_MM_LOCAL_CASE(ModeHelp, 'h');
    CND_MM_LOCAL_CASE(ModeVersion, 'v');
      CND_MM_LOCAL_CASE(Define, 'D');
    //CND_MM_LOCAL_CASE(OutDir, "out-dir");
    //CND_MM_LOCAL_CASE(AuxDir, "aux-dir");
    //CND_MM_LOCAL_CASE(WorkDir, "work-dir");
    //CND_MM_LOCAL_CASE(SrcDirs, "src-dirs");
    //CND_MM_LOCAL_CASE(LibDirs, "lib-dirs");
    //CND_MM_LOCAL_CASE(ResDirs, "res-dirs");
    //CND_MM_LOCAL_CASE(ExeDirs, "exe-dirs");
    //CND_MM_LOCAL_CASE(SrcFiles, "src-files");
    //CND_MM_LOCAL_CASE(LibFiles, "lib-files");
    //CND_MM_LOCAL_CASE(ExeFiles, "exe-files");
    //CND_MM_LOCAL_CASE(ResFiles, "res-files");
    //CND_MM_LOCAL_CASE(NoOverwrite, "no-overwrite");
    //CND_MM_LOCAL_CASE(CompIoSilent, "comp-io-silent");
    //CND_MM_LOCAL_CASE(CompIoVerbose, "comp-io-verbose");
    //CND_MM_LOCAL_CASE(CompIoDebug, "comp-io-debug");
    //CND_MM_LOCAL_CASE(CompIoStd, "comp-io-std");
    //CND_MM_LOCAL_CASE(CompIoTrace, "comp-io-trace");
    //CND_MM_LOCAL_CASE(CompStdinRedir, "comp-stdin-redir");
    //CND_MM_LOCAL_CASE(CompStdoutRedir, "comp-stdout-redir");
    //CND_MM_LOCAL_CASE(CompStderrRedir, "comp-stderr-redir");
    //CND_MM_LOCAL_CASE(DriverIoSilent, "driver-io-silent");
    //CND_MM_LOCAL_CASE(DriverIoVerbose, "driver-io-verbose");
    //CND_MM_LOCAL_CASE(DriverIoDebug, "driver-io-debug");
    //CND_MM_LOCAL_CASE(DriverIoStd, "driver-io-std");
    //CND_MM_LOCAL_CASE(DriverIoTrace, "driver-io-trace");
    //CND_MM_LOCAL_CASE(DriverStdinRedir, "driver-stdin-redir");
    //CND_MM_LOCAL_CASE(DriverStdoutRedir, "driver-stdout-redir");
    //CND_MM_LOCAL_CASE(DriverStderrRedir, "driver-stderr-redir");
    //CND_MM_LOCAL_CASE(Dump, "dump");
    //CND_MM_LOCAL_CASE(HostLinker, "host-linker");
    //CND_MM_LOCAL_CASE(HostLinkerType, "host-linker-type");
    //CND_MM_LOCAL_CASE(HostLinkerVersion, "host-linker-version");
    //CND_MM_LOCAL_CASE(HostLinkerDriver, "host-linker-driver");
    //CND_MM_LOCAL_CASE(HostLinkerDriverType, "host-linker-driver-type");
    //CND_MM_LOCAL_CASE(HostLinkerDriverExternal, "host-linker-driver-external");
    //CND_MM_LOCAL_CASE(HostLinkerImplicitLibs, "host-linker-implicit-libs");
    //CND_MM_LOCAL_CASE(HostCxxCl, "host-cxx-cl");
    //CND_MM_LOCAL_CASE(HostCxxClType, "host-cxx-cl-type");
    //CND_MM_LOCAL_CASE(HostCxxClVersion, "host-cxx-cl-version");
    //CND_MM_LOCAL_CASE(HostCxxClDriver, "host-cxx-cl-driver");
    //CND_MM_LOCAL_CASE(HostCxxClDriverType, "host-cxx-cl-driver-type");
    //CND_MM_LOCAL_CASE(HostCxxClDriverExternal, "host-cxx-cl-driver-external");
    //CND_MM_LOCAL_CASE(HostCxxPredefs, "host-cxx-predefs");
    //CND_MM_LOCAL_CASE(HostCxxSources, "host-cxx-sources");
    //CND_MM_LOCAL_CASE(HostCxxIncludeDirs, "host-cxx-include-dirs");
    default:
      return kNONE;
  }
#undef CND_MM_LOCAL_CASE
};

}  // namespace cnd::corevals::driver

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