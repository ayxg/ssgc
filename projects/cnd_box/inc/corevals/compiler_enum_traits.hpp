///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
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
#include "ccapi/CommonCppApi.hpp"

///////////////////////////////////////////////////////////
/* Include enum implementation headers. */
///////////////////////////////////////////////////////////

// cnd::toolset
#include "toolset/eCharEncoding.hpp"
#include "toolset/eOpSys.hpp"
#include "toolset/eProcArch.hpp"
#include "toolset/eProgLang.hpp"
#include "toolset/eToolChain.hpp"

// cnd::driver
//#include "driver/eDriverFlag.hpp"
//#include "driver/eDriverFlagInterp.hpp"
//#include "driver/eDriverValueType.hpp"
//#include "driver/eDriverVerbosity.hpp"

// cnd::grammar
#include "grammar/eAssoc.hpp"
#include "grammar/eAst.hpp"
#include "grammar/eOperation.hpp"
#include "grammar/ePriority.hpp"
#include "grammar/eSrcChar.hpp"
#include "grammar/eTk.hpp"

// cnd::diagnostic
#include "diagnostic/eClDiagnostic.hpp"
#include "diagnostic/eClDiagnosticCategory.hpp"
#include "diagnostic/eClErr.hpp"
#include "diagnostic/eClErrCategory.hpp"
#include "diagnostic/eClGuide.hpp"
#include "diagnostic/eClGuideCategory.hpp"
#include "diagnostic/eClMsg.hpp"
#include "diagnostic/eClMsgType.hpp"
#include "diagnostic/eClWarning.hpp"
#include "diagnostic/eClWarningCategory.hpp"

///////////////////////////////////////////////////////////
/* Include enum traits headers. */
///////////////////////////////////////////////////////////

// cnd::driver
//#include "driver/traitsof_eDriverFlag.hpp"

// cnd::grammar
#include "grammar/traitsof_eSrcChar.hpp"
#include "grammar/traitsof_eTk.hpp"
#include "grammar/traitsof_eAst.hpp"

// cnd::diagnostic
#include "diagnostic/traitsof_eClErr.hpp"
#include "diagnostic/traitsof_eClWarning.hpp"
#include "diagnostic/traitsof_eClGuide.hpp"
#include "diagnostic/traitsof_eClDiagnostic.hpp"
#include "diagnostic/traitsof_eClMsgType.hpp"
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
//using corevals::driver::eDriverFlagToCStr;
//using corevals::driver::GetDriverFlagInterp;
//using corevals::driver::GetDriverFlagShortSymbol;
//using corevals::driver::GetDriverFlagLongSymbol;
//using corevals::driver::GetDriverFlagFromStr;
//using corevals::driver::GetDriverFlagFromChar;
//
//using corevals::driver::eDriverFlagInterpToCStr;
//using corevals::driver::eDriverValueTypeToCStr;
//using corevals::driver::eDriverVerbosityToCStr;

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