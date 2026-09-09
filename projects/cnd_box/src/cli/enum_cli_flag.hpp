///////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
///////////////////////////////////////////////////////////////////////////////////////////////////
// @project: Sophia Silicon Gold Compiler
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup
/// @brief
///////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup ssgc_cli
/// @{

#pragma once
#include "../common/meta_applied_enum.hpp"

/// @addtogroup cnd_compiler_corevals
/// @{

namespace ssgc::cli {

#define SSGC_AppliedEnum_eCliFlag(m, sep, pre, lst) \
  pre m(kInvalid)                                   \
  sep m(kNone)                                      \
  sep m(kHelp)                                      \
  sep m(kVersion)                                   \
  sep m(kSilent)                                    \
  sep m(kNoColor)                                   \
  sep m(kCommandBuild)                              \
  sep m(kCommandDev)                                \
  sep m(kCommandDevParse)                            \
  sep m(kModeHelp)                                  \
  sep m(kModeVersion)                               \
  sep m(kSources)                                   \
  sep m(kDefine)                                    \
  sep m(kOutDir)                                    \
  sep m(kAuxDir)                                    \
  sep m(kWorkDir)                                   \
  sep m(kSrcDirs)                                   \
  sep m(kLibDirs)                                   \
  sep m(kResDirs)                                   \
  sep m(kExeDirs)                                   \
  sep m(kSrcFiles)                                  \
  sep m(kLibFiles)                                  \
  sep m(kExeFiles)                                  \
  sep m(kResFiles)                                  \
  sep m(kNoOverwrite)                               \
  sep m(kCompIoSilent)                              \
  sep m(kCompIoVerbose)                             \
  sep m(kCompIoDebug)                               \
  sep m(kCompIoStd)                                 \
  sep m(kCompIoTrace)                               \
  sep m(kCompStdinRedir)                            \
  sep m(kCompStdoutRedir)                           \
  sep m(kCompStderrRedir)                           \
  sep m(kDriverIoSilent)                            \
  sep m(kDriverIoVerbose)                           \
  sep m(kDriverIoDebug)                             \
  sep m(kDriverIoStd)                               \
  sep m(kDriverIoTrace)                             \
  sep m(kDriverStdinRedir)                          \
  sep m(kDriverStdoutRedir)                         \
  sep m(kDriverStderrRedir)                         \
  sep m(kDump)                                      \
  sep m(kHostLinker)                                \
  sep m(kHostLinkerType)                            \
  sep m(kHostLinkerVersion)                         \
  sep m(kHostLinkerDriver)                          \
  sep m(kHostLinkerDriverType)                      \
  sep m(kHostLinkerDriverExternal)                  \
  sep m(kHostLinkerImplicitLibs)                    \
  sep m(kHostCxxCl)                                 \
  sep m(kHostCxxClType)                             \
  sep m(kHostCxxClVersion)                          \
  sep m(kHostCxxClDriver)                           \
  sep m(kHostCxxClDriverType)                       \
  sep m(kHostCxxClDriverExternal)                   \
  sep m(kHostCxxPredefs)                            \
  sep m(kHostCxxSources)                            \
  sep m(kHostCxxIncludeDirs)                        \
  sep m(kTargetHost)                                \
  sep m(kProcArch)                                  \
  sep m(kOpSys)                                     \
  sep m(kLinker)                                    \
  sep m(kLinkerType)                                \
  sep m(kLinkerVersion)                             \
  sep m(kLinkerDriver)                              \
  sep m(kLinkerDriverType)                          \
  sep m(kLinkerDriverExternal)                      \
  sep m(kLinkerImplicitLibs)                        \
  sep m(kCxxCl)                                     \
  sep m(kCxxClType)                                 \
  sep m(kCxxClVersion)                              \
  sep m(kCxxClDriver)                               \
  sep m(kCxxClDriverType)                           \
  sep m(kCxxClDriverExternal)                       \
  sep m(kCxxPredefs)                                \
  sep m(kCxxSources)                                \
  sep m(kCxxIncludeDirs)                            \
  sep m(kDevModeLex)                                \
  lst

SSGC_MACRO_DefineEnumFromAppliedList(SSGC_AppliedEnum_eCliFlag, eCliFlag);

}  // namespace ssgc::cli

/// @} // end of ssgc_cli

///////////////////////////////////////////////////////////////////////////////////////////////////
// @project: Sophia Silicon Gold Compiler
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
//
// Licensed under the GNU Affero General Public License, Version 3.
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.gnu.org/licenses/agpl-3.0.html
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
///////////////////////////////////////////////////////////////////////////////////////////////////