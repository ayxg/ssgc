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
/// @brief Traits of cCst
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
// clang-format off
#include "ccapi/CommonCppApi.hpp"
#include "grammar/eTk.hpp"
#include "grammar/eAssoc.hpp"
#include "grammar/eOperation.hpp"
#include "grammar/ePriority.hpp"
#include "grammar/eAst.hpp"
// clang-format on

/// @addtogroup cnd_compiler_corevals
/// @{

namespace cnd::corevals::grammar {
constexpr eAssoc GetAstAssoc(eAst t) noexcept;
constexpr eOperation GetAstOperation(eAst t) noexcept;
constexpr ePriority GetAstPriority(eAst t) noexcept;
constexpr CStr GetAstSymbol(eAst t) noexcept;
constexpr bool IsAstKeyword(eAst t) noexcept;
constexpr bool IsAstLScope(eAst t) noexcept;
constexpr bool IsAstRScope(eAst t) noexcept;
constexpr bool IsAstRScopeOf(eAst topen, eAst tclose) noexcept;
constexpr bool IsAstModifier(eAst t) noexcept;
constexpr bool IsAstDeclarative(eAst t) noexcept;
constexpr bool IsAstAnOperand(eAst t) noexcept;
constexpr bool IsAstAPrefixOperator(eAst t) noexcept;
constexpr bool IsAstPrimary(eAst t) noexcept;
constexpr bool IsAstPragmatic(eAst t) noexcept;
constexpr eAst GetAstFromTk(eTk t) noexcept;
}  // namespace cnd::corevals::grammar

namespace cnd::corevals::grammar {
constexpr bool IsAstPrimary(eAst t) noexcept {
  return IsAstAnOperand(t) || IsAstAPrefixOperator(t) || t == eAst::kLParen;
}

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
    case kLitU1:
    case kLitU8:
    case kLitU16: 
    case kLitU32:
    case kLitU64:
    case kLitI8: 
    case kLitI16:
    case kLitI32:
    case kLitI64:
    case kLitF32:
    case kLitF64:     
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
    case eTk::kINVALID:
      return eAst::kINVALID;
    case eTk::kNONE:
      return eAst::kNONE;
    case eTk::kDirectiveInclude:
      return eAst::kDirectiveInclude;
    case eTk::kDirectiveDefMacro:
      return eAst::kDirectiveDefMacro;
    case eTk::kDirectiveEndmacro:
      return eAst::kDirectiveEndmacro;
    case eTk::kDirectiveIf:
      return eAst::kDirectiveIf;
    case eTk::kDirectiveElse:
      return eAst::kDirectiveElse;
    case eTk::kDirectiveElif:
      return eAst::kDirectiveElif;
    case eTk::kDirectiveEndif:
      return eAst::kDirectiveEndif;
    case eTk::kDirectiveIfdef:
      return eAst::kDirectiveIfdef;
    case eTk::kDirectiveIfndef:
      return eAst::kDirectiveIfndef;
    case eTk::kDirectiveUndef:
      return eAst::kDirectiveUndef;
    case eTk::kKwDef:
      return eAst::kKwDef;
    case eTk::kKwFn:
      return eAst::kKwFn;
    case eTk::kKwClass:
      return eAst::kKwClass;
    case eTk::kKwMain:
      return eAst::kKwMain;
    case eTk::kKwImport:
      return eAst::kKwImport;
    case eTk::kKwNamespace:
      return eAst::kKwNamespace;
    case eTk::kKwUse:
      return eAst::kKwUse;
    case eTk::kKwLib:
      return eAst::kKwLib;
    case eTk::kKwDll:
      return eAst::kKwDll;
    case eTk::kKwEnum:
      return eAst::kKwEnum;
    case eTk::kKwIf:
      return eAst::kKwIf;
    case eTk::kKwElif:
      return eAst::kKwElif;
    case eTk::kKwElse:
      return eAst::kKwElse;
    case eTk::kKwCxif:
      return eAst::kKwCxif;
    case eTk::kKwCxelif:
      return eAst::kKwCxelif;
    case eTk::kKwCxelse:
      return eAst::kKwCxelse;
    case eTk::kKwSwitch:
      return eAst::kKwSwitch;
    case eTk::kKwCase:
      return eAst::kKwCase;
    case eTk::kKwDefault:
      return eAst::kKwDefault;
    case eTk::kKwWhile:
      return eAst::kKwWhile;
    case eTk::kKwFor:
      return eAst::kKwFor;
    case eTk::kKwReturn:
      return eAst::kKwReturn;
    case eTk::kKwBreak:
      return eAst::kKwBreak;
    case eTk::kKwContinue:
      return eAst::kKwContinue;
    case eTk::kKwInt:
      return eAst::kKwInt;
    case eTk::kKwUint:
      return eAst::kKwUint;
    case eTk::kKwReal:
      return eAst::kKwReal;
    case eTk::kKwBool:
      return eAst::kKwBool;
    case eTk::kKwChar:
      return eAst::kKwChar;
    case eTk::kKwByte:
      return eAst::kKwByte;
    case eTk::kKwCstr:
      return eAst::kKwCstr;
    case eTk::kKwStr:
      return eAst::kKwStr;
    case eTk::kKwPtr:
      return eAst::kKwPtr;
    case eTk::kKwList:
      return eAst::kKwList;
    case eTk::kKwArray:
      return eAst::kKwArray;
    case eTk::kKwTrue:
      return eAst::kKwTrue;
    case eTk::kKwFalse:
      return eAst::kKwFalse;
    case eTk::kKwNone:
      return eAst::kKwNone;
    case eTk::kKwVoid:
      return eAst::kKwVoid;
    case eTk::kKwIn:
      return eAst::kKwIn;
    case eTk::kKwAs:
      return eAst::kKwAs;
    case eTk::kKwCin:
      return eAst::kKwCin;
    case eTk::kKwCout:
      return eAst::kKwCout;
    case eTk::kKwNative:
      return eAst::kKwNative;
    case eTk::kKwConst:
      return eAst::kKwConst;
    case eTk::kKwRef:
      return eAst::kKwRef;
    case eTk::kKwPrivate:
      return eAst::kKwPrivate;
    case eTk::kKwPublic:
      return eAst::kKwPublic;
    case eTk::kKwStatic:
      return eAst::kKwStatic;
    case eTk::kKwAny:
      return eAst::kKwAny;
    case eTk::kKwAuto:
      return eAst::kKwAuto;
    case eTk::kKwType:
      return eAst::kKwType;
    case eTk::kKwValue:
      return eAst::kKwValue;
    case eTk::kKwTemplate:
      return eAst::kKwTemplate;
    case eTk::kKwProc:
      return eAst::kKwProc;
    case eTk::kHash:
      return eAst::kHash;
    case eTk::kAdd:
      return eAst::kAdd;
    case eTk::kSub:
      return eAst::kSub;
    case eTk::kMul:
      return eAst::kMul;
    case eTk::kDiv:
      return eAst::kDiv;
    case eTk::kMod:
      return eAst::kMod;
    case eTk::kAnd:
      return eAst::kAnd;
    case eTk::kOr:
      return eAst::kOr;
    case eTk::kXor:
      return eAst::kXor;
    case eTk::kNot:
      return eAst::kNot;
    case eTk::kLsh:
      return eAst::kLsh;
    case eTk::kRsh:
      return eAst::kRsh;
    case eTk::kEq:
      return eAst::kEq;
    case eTk::kNeq:
      return eAst::kNeq;
    case eTk::kLt:
      return eAst::kLt;
    case eTk::kGt:
      return eAst::kGt;
    case eTk::kLte:
      return eAst::kLte;
    case eTk::kGte:
      return eAst::kGte;
    case eTk::kSpaceship:
      return eAst::kSpaceship;
    case eTk::kAssign:
      return eAst::kAssign;
    case eTk::kNewAssign:
      return eAst::kNewAssign;
    case eTk::kAddAssign:
      return eAst::kAddAssign;
    case eTk::kSubAssign:
      return eAst::kSubAssign;
    case eTk::kMulAssign:
      return eAst::kMulAssign;
    case eTk::kDivAssign:
      return eAst::kDivAssign;
    case eTk::kModAssign:
      return eAst::kModAssign;
    case eTk::kAndAssign:
      return eAst::kAndAssign;
    case eTk::kOrAssign:
      return eAst::kOrAssign;
    case eTk::kXorAssign:
      return eAst::kXorAssign;
    case eTk::kLshAssign:
      return eAst::kLshAssign;
    case eTk::kRshAssign:
      return eAst::kRshAssign;
    case eTk::kInc:
      return eAst::kInc;
    case eTk::kDec:
      return eAst::kDec;
    case eTk::kBnot:
      return eAst::kBnot;
    case eTk::kBand:
      return eAst::kBand;
    case eTk::kBor:
      return eAst::kBor;
    case eTk::kLParen:
      return eAst::kLParen;
    case eTk::kRParen:
      return eAst::kRParen;
    case eTk::kLBrace:
      return eAst::kLBrace;
    case eTk::kRBrace:
      return eAst::kRBrace;
    case eTk::kLBracket:
      return eAst::kLBracket;
    case eTk::kRBracket:
      return eAst::kRBracket;
    case eTk::kSemicolon:
      return eAst::kSemicolon;
    case eTk::kColon:
      return eAst::kColon;
    case eTk::kComma:
      return eAst::kComma;
    case eTk::kPeriod:
      return eAst::kPeriod;
    case eTk::kDoubleColon:
      return eAst::kDoubleColon;
    case eTk::kEllipsis:
      return eAst::kEllipsis;
    case eTk::kCommercialAt:
      return eAst::kCommercialAt;
    case eTk::kDollar:
      return eAst::kDollar;
    case eTk::kBacklash:
      return eAst::kBacklash;
    case eTk::kQuestion:
      return eAst::kQuestion;
    case eTk::kLitInt:
      return eAst::kLitInt;
    case eTk::kLitUint:
      return eAst::kLitUint;
    case eTk::kLitBool:
      return eAst::kLitBool;
    case eTk::kLitChar:
      return eAst::kLitChar;
    case eTk::kLitByte:
      return eAst::kLitByte;
    case eTk::kLitCstr:
      return eAst::kLitCstr;
    case eTk::kLitU1:
      return eAst::kLitU1;
    case eTk::kLitU8:
      return eAst::kLitU8;
    case eTk::kLitU16:
      return eAst::kLitU16;
    case eTk::kLitU32:
      return eAst::kLitU32;
    case eTk::kLitU64:
      return eAst::kLitU64;
    case eTk::kLitI8:
      return eAst::kLitI8;
    case eTk::kLitI16:
      return eAst::kLitI16;
    case eTk::kLitI32:
      return eAst::kLitI32;
    case eTk::kLitI64:
      return eAst::kLitI64;
    case eTk::kLitF32:
      return eAst::kLitF32;
    case eTk::kLitF64:
      return eAst::kLitF64;
    case eTk::kLitReal:
      return eAst::kLitReal;
    case eTk::kIdent:
      return eAst::kIdent;
    case eTk::kWhitespace:
      return eAst::kWhitespace;
    case eTk::kNewline:
      return eAst::kNewline;
    case eTk::kBlockComment:
      return eAst::kBlockComment;
    case eTk::kLineComment:
      return eAst::kLineComment;
    case eTk::kEofile:
      return eAst::kEofile;
    case eTk::COUNT:
      return eAst::COUNT;
    default:
      return eAst::kINVALID;
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