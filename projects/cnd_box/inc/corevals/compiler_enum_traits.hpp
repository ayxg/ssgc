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
/// @brief
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup cnd_compiler_corevals
/// @{
#pragma once
// clang-format off
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

///////////////////////////////////////////////////////////
/* Include enum traits headers. */
///////////////////////////////////////////////////////////

// cnd::driver
#include "corevals/enumtraits/driver/traitsof_eDriverFlag.hpp"

// cnd::grammar
#include "corevals/enumtraits/grammar/traitsof_eSrcChar.hpp"
#include "corevals/enumtraits/grammar/traitsof_eTk.hpp"
#include "corevals/enumtraits/grammar/traitsof_eAst.hpp"

// cnd::diagnostic
#include "corevals/enumtraits/diagnostic/traitsof_eClErr.hpp"
#include "corevals/enumtraits/diagnostic/traitsof_eClWarning.hpp"
#include "corevals/enumtraits/diagnostic/traitsof_eClGuide.hpp"
#include "corevals/enumtraits/diagnostic/traitsof_eClDiagnostic.hpp"
#include "corevals/enumtraits/diagnostic/traitsof_eClMsgType.hpp"
// clang-format on

/// @section Compiler core enum traits.
///
/// @ingroup cnd_compiler_corevals
/// @{
namespace cnd {
///// Toolset : Enums which describe an aspect of a toolset possibly avaialble to the compiler.

using corevals::toolset::eCharEncodingToCStr;
using corevals::toolset::eOpSysToCStr;
using corevals::toolset::eProcArchToCStr;
using corevals::toolset::eProgLangToCStr;
using corevals::toolset::eToolchainToCStr;
//
//// CLI Driver : Enums which describe an aspect of the command line interface abstraction layer.

// eClCliFlag
using corevals::driver::eDriverFlagToCStr;
using corevals::driver::GetDriverFlagInterp;
using corevals::driver::GetDriverFlagShortSymbol;
using corevals::driver::GetDriverFlagLongSymbol;
using corevals::driver::GetDriverFlagFromStr;
using corevals::driver::GetDriverFlagFromChar;

using corevals::driver::eDriverFlagInterpToCStr;
using corevals::driver::eDriverValueTypeToCStr;
using corevals::driver::eDriverVerbosityToCStr;

//// C& Grammar : Enums which describe an aspect of the C& grammar.

using corevals::grammar::eAssocToCStr;
using corevals::grammar::eAstToCStr;
using corevals::grammar::eOperationToCStr;
using corevals::grammar::ePriorityToCStr;
using corevals::grammar::eSrcCharToCStr;

// eSrcChar
using corevals::grammar::IsSrcCharAlnumus;
using corevals::grammar::IsSrcCharAlpha;
using corevals::grammar::IsSrcCharUpper;
using corevals::grammar::IsSrcCharAlphanumeric;
using corevals::grammar::IsSrcCharAlphaUnderscore;
using corevals::grammar::IsSrcCharControl;
using corevals::grammar::IsSrcCharNewline;
using corevals::grammar::IsSrcCharNumeric;
using corevals::grammar::IsSrcCharPunctuator;
using corevals::grammar::IsSrcCharSpace;
using corevals::grammar::IsSrcCharUnderscore;
using corevals::grammar::IsSrcCharValid;
using corevals::grammar::IsSrcCharWhitespace;

// eTk
using corevals::grammar::eTkToCStr;
using corevals::grammar::GetTkAssoc;
using corevals::grammar::GetTkFromKeyword;
using corevals::grammar::GetTkOperation;
using corevals::grammar::GetTkPriority;
using corevals::grammar::GetTkSymbol;

using corevals::grammar::IsTkAnOperand;
using corevals::grammar::IsTkAPrefixOperator;
using corevals::grammar::IsTkDeclarative;
using corevals::grammar::IsTkKeyword;
using corevals::grammar::IsTkLScope;
using corevals::grammar::IsTkModifier;
using corevals::grammar::IsTkPragmatic;
using corevals::grammar::IsTkPrimary;
using corevals::grammar::IsTkRScope;
using corevals::grammar::IsTkRScopeOf;

// eAst
using corevals::grammar::eAstToCStr;
using corevals::grammar::GetAstAssoc;
using corevals::grammar::GetAstFromTk;
using corevals::grammar::GetAstOperation;
using corevals::grammar::GetAstPriority;
using corevals::grammar::GetAstSymbol;

using corevals::grammar::IsAstAnOperand;
using corevals::grammar::IsAstAPrefixOperator;
using corevals::grammar::IsAstDeclarative;
using corevals::grammar::IsAstKeyword;
using corevals::grammar::IsAstLScope;
using corevals::grammar::IsAstModifier;
using corevals::grammar::IsAstPragmatic;
using corevals::grammar::IsAstPrimary;
using corevals::grammar::IsAstRScope;
using corevals::grammar::IsAstRScopeOf;

//// Diagnostics : Enums which describe an aspect of the compiler diagnostics.

using corevals::diagnostic::eClDiagnosticToCStr;
using corevals::diagnostic::eClErrCategoryToCStr;
using corevals::diagnostic::eClErrToCStr;
using corevals::diagnostic::eClGuideToCStr;
using corevals::diagnostic::eClWarningToCStr;
using corevals::diagnostic::GetClDiagnosticCategory;
using corevals::diagnostic::GetClErrCategory;
using corevals::diagnostic::GetClGuideCategory;
using corevals::diagnostic::GetClWarningCategory;
using corevals::diagnostic::GetClMsgType;

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