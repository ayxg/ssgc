///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the Apache License, Version 2.0(the "License");
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup cnd_corevals
/// @brief Base C& compiler enum defs, and their [[enum-name]]ToCStr method which returns the given enum entry symbol.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup cnd_compiler_corevals
/// @{
#pragma once
#include "use_ccapi.hpp"

///////////////////////////////////////////////////////////
/* Include enum implementation headers. */
///////////////////////////////////////////////////////////

// cnd::toolset
#include "corevals/enumdefs/toolset/eCharEncoding.hpp"
#include "corevals/enumdefs/toolset/eOpSys.hpp"
#include "corevals/enumdefs/toolset/eProcArch.hpp"
#include "corevals/enumdefs/toolset/eProgLang.hpp"
#include "corevals/enumdefs/toolset/eToolChain.hpp"

// cnd::driver
#include "corevals/enumdefs/driver/eDriverFlag.hpp"
#include "corevals/enumdefs/driver/eDriverFlagInterp.hpp"
#include "corevals/enumdefs/driver/eDriverValueType.hpp"
#include "corevals/enumdefs/driver/eDriverVerbosity.hpp"

// cnd::grammar
#include "corevals/enumdefs/grammar/eAssoc.hpp"
#include "corevals/enumdefs/grammar/eAst.hpp"
#include "corevals/enumdefs/grammar/eOperation.hpp"
#include "corevals/enumdefs/grammar/ePriority.hpp"
#include "corevals/enumdefs/grammar/eSrcChar.hpp"
#include "corevals/enumdefs/grammar/eTk.hpp"

// cnd::diagnostic
#include "corevals/enumdefs/diagnostic/eClDiagnostic.hpp"
#include "corevals/enumdefs/diagnostic/eClDiagnosticCategory.hpp"
#include "corevals/enumdefs/diagnostic/eClErr.hpp"
#include "corevals/enumdefs/diagnostic/eClErrCategory.hpp"
#include "corevals/enumdefs/diagnostic/eClGuide.hpp"
#include "corevals/enumdefs/diagnostic/eClGuideCategory.hpp"
#include "corevals/enumdefs/diagnostic/eClMsg.hpp"
#include "corevals/enumdefs/diagnostic/eClMsgType.hpp"
#include "corevals/enumdefs/diagnostic/eClWarning.hpp"
#include "corevals/enumdefs/diagnostic/eClWarningCategory.hpp"

/// @section Compiler core enum definitions.
///
/// Each enum definition gets a [enum-name]ToCStr method in the same namespace. The method must return a CStr
/// representation of the enum identifier. Default case must return an "<invalid>" string, indicating no conversion
/// is implemented. Smaller enums may be defined manually without the use of the applied enum macros.
///
/// @ingroup cnd_compiler_corevals
/// @{
namespace cnd {
/// Toolset : Enums which describe an aspect of a toolset possibly avaialble to the compiler.
using corevals::toolset::eCharEncoding;
using corevals::toolset::eOpSys;     // large
using corevals::toolset::eProcArch;  // large
using corevals::toolset::eProgLang;
using corevals::toolset::eToolchain;  // large

using corevals::toolset::eCharEncodingToCStr;
using corevals::toolset::eOpSysToCStr;
using corevals::toolset::eProcArchToCStr;
using corevals::toolset::eProgLangToCStr;
using corevals::toolset::eToolchainToCStr;

// CLI Driver : Enums which describe an aspect of the command line interface abstraction layer.
using corevals::driver::eDriverFlag;
using corevals::driver::eDriverFlagInterp;
using corevals::driver::eDriverValueType;
using corevals::driver::eDriverVerbosity;

using corevals::driver::eDriverFlagInterpToCStr;
using corevals::driver::eDriverFlagToCStr;
using corevals::driver::eDriverValueTypeToCStr;
using corevals::driver::eDriverVerbosityToCStr;

// C& Grammar : Enums which describe an aspect of the C& grammar.
using corevals::grammar::eAssoc;
using corevals::grammar::eAst;
using corevals::grammar::eOperation;
using corevals::grammar::ePriority;
using corevals::grammar::eSrcChar;
using corevals::grammar::eTk;

using corevals::grammar::eAssocToCStr;
using corevals::grammar::eAstToCStr;
using corevals::grammar::eOperationToCStr;
using corevals::grammar::ePriorityToCStr;
using corevals::grammar::eSrcCharToCStr;
using corevals::grammar::eTkToCStr;

// Diagnostics : Enums which describe an aspect of the compiler diagnostics.
using corevals::diagnostic::eClDiagnostic;
using corevals::diagnostic::eClErr;
using corevals::diagnostic::eClErrCategory;
using corevals::diagnostic::eClGuide;
using corevals::diagnostic::eClMsg;
using corevals::diagnostic::eClMsgType;
using corevals::diagnostic::eClWarning;

using corevals::diagnostic::eClDiagnosticToCStr;
using corevals::diagnostic::eClErrCategoryToCStr;
using corevals::diagnostic::eClErrToCStr;
using corevals::diagnostic::eClGuideToCStr;
using corevals::diagnostic::eClMsgToCStr;
using corevals::diagnostic::eClWarningToCStr;

}  // namespace cnd

/// @} // end of cnd_compiler_corevals_cl_enums

/// @} // end of cnd_corevals

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