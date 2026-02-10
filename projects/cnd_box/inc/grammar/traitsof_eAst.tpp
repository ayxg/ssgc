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
/// @brief Traits of eAst
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "grammar/traitsof_eAst.hpp"

// clang-format off
#include "ccapi/CommonCppApi.hpp"
#include "grammar/eTk.hpp"
#include "grammar/eAssoc.hpp"
#include "grammar/eOperation.hpp"
#include "grammar/ePriority.hpp"
#include "grammar/eAst.hpp"

#include "grammar/traitsof_eTk.hpp"
// clang-format on

/// @addtogroup cnd_compiler_corevals
/// @{

namespace cnd::corevals::grammar {
constexpr bool IsAstPrimary(eAst t) noexcept { return IsAstAnOperand(t) || IsAstAPrefixOperator(t) || t == eAst::kLParen; }

constexpr bool IsAstPragmatic(eAst t) noexcept { return IsAstModifier(t) || IsAstDeclarative(t); }

constexpr eAssoc GetAstAssoc(eAst t) noexcept {
  using enum eAst;
  switch (t) {
    case kAssign:
    case kAddAssign:
    case kSubAssign:
    case kMulAssign:
    case kDivAssign:
    case kModAssign:
    case kAndAssign:
    case kOrAssign:
    case kXorAssign:
    case kLshAssign:
    case kRshAssign:
    case kNot:
    case kBnot:
    case kEllipsis:
    case kCommercialAt:
      return eAssoc::Right;
    case kAdd:
    case kSub:
    case kMul:
    case kDiv:
    case kMod:
    case kBand:
    case kBor:
    case kXor:
    case kLsh:
    case kRsh:
    case kAnd:
    case kOr:
    case kEq:
    case kNeq:
    case kLt:
    case kGt:
    case kLte:
    case kGte:
    case kInc:
    case kDec:
    case kDoubleColon:
    case kPeriod:
      return eAssoc::Left;
    default:
      return eAssoc::NONE;
  }
}

constexpr eOperation GetAstOperation(eAst t) noexcept {
  using enum eAst;
  switch (t) {
    case kAssign:
    case kAddAssign:
    case kSubAssign:
    case kMulAssign:
    case kDivAssign:
    case kModAssign:
    case kAndAssign:
    case kOrAssign:
    case kXorAssign:
    case kLshAssign:
    case kRshAssign:
      return eOperation::Binary;
      break;
    case kInc:
    case kDec:
      return eOperation::Postfix;
      break;
    case kAdd:
    case kSub:
    case kMul:
    case kDiv:
    case kMod:
    case kBand:
    case kBor:
    case kXor:
    case kLsh:
    case kRsh:
      return eOperation::Binary;
      break;
    case kNot:
    case kBnot:
      return eOperation::Prefix;
      break;
    case kAnd:
    case kOr:
    case kEq:
    case kNeq:
    case kLt:
    case kGt:
    case kLte:
    case kGte:
      return eOperation::Binary;
    case kLParen:
    case kRParen:
    case kLBrace:
    case kRBrace:
    case kLBracket:
    case kRBracket:
      return eOperation::Postfix;
    case kPeriod:
    case kDoubleColon:
      return eOperation::Binary;
    case kEllipsis:
      return eOperation::Binary;
    case kCommercialAt:
      return eOperation::Prefix;
    default:
      return eOperation::NONE;
  }
}

constexpr ePriority GetAstPriority(eAst t) noexcept {
  switch (t) {
    using enum eAst;
    case kNONE:
    case kINVALID:
    case kEofile:
    case kLineComment:
    case kBlockComment:
    case kNewline:
    case kWhitespace:
      return ePriority::NONE;
      break;
    case kLitCstr:
    case kLitInt:
    case kLitUint:
    case kLitReal:
    case kLitBool:
    case kLitChar:
    case kLitByte:
    case kIdent:
      return ePriority::Max;
      break;
    case kAssign:
    case kAddAssign:
    case kSubAssign:
    case kMulAssign:
    case kDivAssign:
    case kModAssign:
    case kAndAssign:
    case kOrAssign:
    case kXorAssign:
    case kLshAssign:
    case kRshAssign:
      return ePriority::Assignment;
      break;
    case kInc:
    case kDec:
      return ePriority::Postfix;
      break;
    case kAdd:
    case kSub:
      return ePriority::Term;
      break;
    case kMul:
    case kDiv:
    case kMod:
      return ePriority::Factor;
      break;
    case kNot:
    case kBnot:
      return ePriority::Prefix;
      break;
    case kBand:
    case kBor:
    case kXor:
    case kLsh:
    case kRsh:
    case kAnd:
    case kOr:
    case kEq:
    case kNeq:
    case kLt:
    case kGt:
    case kLte:
    case kGte:
      return ePriority::Comparison;
      break;

    case kRParen:
    case kRBrace:
    case kRBracket:
      return ePriority::NONE;
      break;
    case kLBrace:
    case kLParen:
    case kLBracket:
      return ePriority::Postfix;
      break;
    case kSemicolon:
    case kColon:
    case kComma:
    case kEllipsis:
    case kCommercialAt:
      return ePriority::Max;
      break;
    case kPeriod:
    case kDoubleColon:
      return ePriority::Access;
      break;
    case kKwDef:
    case kKwFn:
    case kKwClass:
    case kKwMain:
    case kKwImport:
    case kKwNamespace:
    case kKwUse:
    case kKwLib:
    case kKwDll:
    case kKwIf:
    case kKwElif:
    case kKwElse:
    case kKwCxif:
    case kKwCxelif:
    case kKwCxelse:
    case kKwSwitch:
    case kKwCase:
    case kKwDefault:
    case kKwWhile:
    case kKwFor:
    case kKwReturn:
    case kKwBreak:
    case kKwContinue:
    case kKwInt:
    case kKwUint:
    case kKwReal:
    case kKwBool:
    case kKwChar:
    case kKwByte:
    case kKwCstr:
    case kKwStr:
    case kKwPtr:
    case kKwList:
    case kKwArray:
    case kKwTrue:
    case kKwFalse:
    case kKwNone:
    case kKwVoid:
    case kKwIn:
    case kKwAs:
    case kKwCin:
    case kKwCout:
    case kKwNative:
    case kKwConst:
    case kKwRef:
    case kKwPrivate:
    case kKwPublic:
    case kKwStatic:
    case kKwAny:
    case kKwAuto:
    case kKwType:
    case kKwValue:
    case kKwTemplate:
      return ePriority::Max;
      break;
    default:
      return ePriority::NONE;
  }
}

constexpr CStr GetAstSymbol(eAst t) noexcept {
#define CND_LITERAL_CST_ENUM_CASE(n) \
  case eAst::k##n:                   \
    return GetTkSymbol(eTk::k##n);
  switch (t) { CND_APPLIED_ENUM_eTk(CND_LITERAL_CST_ENUM_CASE, , , ) default : return ""; }
#undef CND_LITERAL_AST_ENUM_CASE
}

constexpr bool IsAstKeyword(eAst t) noexcept {
  switch (t) {
    using enum eAst;
    case kKwDef:
    case kKwFn:
    case kKwClass:
    case kKwMain:
    case kKwImport:
    case kKwNamespace:
    case kKwUse:
    case kKwLib:
    case kKwDll:
    case kKwIf:
    case kKwElif:
    case kKwElse:
    case kKwCxif:
    case kKwCxelif:
    case kKwCxelse:
    case kKwSwitch:
    case kKwCase:
    case kKwDefault:
    case kKwWhile:
    case kKwFor:
    case kKwReturn:
    case kKwBreak:
    case kKwContinue:
    case kKwInt:
    case kKwUint:
    case kKwReal:
    case kKwBool:
    case kKwChar:
    case kKwByte:
    case kKwCstr:
    case kKwStr:
    case kKwPtr:
    case kKwList:
    case kKwArray:
    case kKwTrue:
    case kKwFalse:
    case kKwNone:
    case kKwVoid:
    case kKwIn:
    case kKwAs:
    case kKwCin:
    case kKwCout:
    case kKwNative:
    case kKwConst:
    case kKwRef:
    case kKwPrivate:
    case kKwPublic:
    case kKwStatic:
    case kKwAny:
    case kKwAuto:
    case kKwType:
    case kKwValue:
    case kKwTemplate:
      return true;
    default:
      return false;
  }
}

constexpr bool IsAstModifier(eAst t) noexcept {
  switch (t) {
    using enum eAst;
    case kKwPrivate:
    case kKwPublic:
    case kKwConst:
    case kKwStatic:
    case kKwRef:
      return true;
    default:
      return false;
  }
}

constexpr bool IsAstDeclarative(eAst t) noexcept {
  // One of: def,fn,class,obj,use,lib,main,import
  switch (t) {
    using enum eAst;
    case kKwDef:
    case kKwFn:
    case kKwClass:
    case kKwMain:
    case kKwImport:
    case kKwNamespace:
    case kKwUse:
    case kKwLib:
    case kKwDll:
    case kKwIf:
    case kKwElif:
    case kKwElse:
    case kKwCxif:
    case kKwCxelif:
    case kKwCxelse:
    case kKwSwitch:
    case kKwCase:
    case kKwDefault:
    case kKwWhile:
    case kKwFor:
    case kKwReturn:
    case kKwBreak:
    case kKwContinue:
    case kKwTemplate:
      return true;
    default:
      return false;
  }
}

constexpr bool IsAstLScope(eAst t) noexcept {
  switch (t) {
    case eAst::kLParen:
    case eAst::kLBrace:
    case eAst::kLBracket:
      return true;
    default:
      return false;
  }
}

constexpr bool IsAstRScope(eAst t) noexcept {
  switch (t) {
    case eAst::kRParen:
    case eAst::kRBrace:
    case eAst::kRBracket:
      return true;
    default:
      return false;
  }
}

constexpr bool IsAstRScopeOf(eAst topen, eAst tclose) noexcept {
  using enum eAst;
  return (topen == kLParen && tclose == kRParen) || (topen == kLBrace && tclose == kRBrace) ||
         (topen == kLBracket && tclose == kRBracket);
}

constexpr bool IsAstAnOperand(eAst t) noexcept {
  switch (t) {
    using enum eAst;
      // Literals
    case kLitCstr:
    case kLitInt:
    case kLitUint:
    case kLitReal:
    case kLitBool:
    case kLitChar:
    case kLitByte:
    case kIdent:
      // Keyword Types which are operands.
    case kKwInt:
    case kKwUint:
    case kKwReal:
    case kKwBool:
    case kKwChar:
    case kKwByte:
    case kKwCstr:
    case kKwStr:
    case kKwPtr:
    case kKwList:
    case kKwArray:
    case kKwTrue:
    case kKwFalse:
    case kKwNone:
    case kKwVoid:
      return true;
    default:
      return false;
  }
}

constexpr bool IsAstAPrefixOperator(eAst t) noexcept {
  switch (t) {
    case eAst::kNot:
    case eAst::kBnot:
      return true;
    default:
      return false;
  }
}

constexpr eAst GetAstFromTk(eTk t) noexcept {
// Function local macro.
#define CND_ETK_TO_CST(n) \
  case eTk::k##n:         \
    return eAst::k##n;

  // Manually add a case for the COUNT entry.
  // Switch over all of etk and return the equivalent mapped symbol.
  // Default can't ever happen because eAst must contain all of eTk.
  switch (t) {
    CND_APPLIED_ENUM_eTk(CND_ETK_TO_CST, , , case eTk::COUNT : return eAst::COUNT;) default : __assume(false);
  }

#undef CND_ETK_TO_AST
}

}  // namespace cnd::corevals::grammar

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