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
/// @brief Traits of eTk.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "corevals/grammar/traitsof_eTk.tpp"

// clang-format off
#include "use_ccapi.hpp"
#include "corevals/grammar/eTk.hpp"
#include "corevals/grammar/eAssoc.hpp"
#include "corevals/grammar/eOperation.hpp"
#include "corevals/grammar/ePriority.hpp"
// clang-format on

/// @addtogroup cnd_compiler_corevals
/// @{

namespace cnd::corevals::grammar {
constexpr eAssoc GetTkAssoc(eTk t) noexcept {
  using enum eTk;
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
    case kPeriod:
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
      return eAssoc::Left;
    default:
      return eAssoc::NONE;
  }
}

constexpr eOperation GetTkOperation(eTk t) noexcept {
  using enum eTk;
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
    case kDoubleColon:
    case kPeriod:
      return eOperation::Binary;
    case kEllipsis:
      return eOperation::Binary;
    case kCommercialAt:
      return eOperation::Prefix;
    default:
      return eOperation::NONE;
  }
}

constexpr ePriority GetTkPriority(eTk t) noexcept {
  using enum eTk;
  switch (t) {
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
    case kLParen:
    case kRParen:
    case kLBrace:
    case kRBrace:
    case kLBracket:
    case kRBracket:
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

constexpr CStr GetTkSymbol(eTk t) noexcept {
#define CND_MM_LOCAL_LITERAL_CASE_eTk(n, lt) \
  case eTk::k##n:                            \
    return lt;

  switch (t) {
    CND_MM_LOCAL_LITERAL_CASE_eTk(NONE, "");
    CND_MM_LOCAL_LITERAL_CASE_eTk(INVALID, "");
    // Declarative
    CND_MM_LOCAL_LITERAL_CASE_eTk(KwDef, "def");
    CND_MM_LOCAL_LITERAL_CASE_eTk(KwFn, "fn");
    CND_MM_LOCAL_LITERAL_CASE_eTk(KwClass, "class");
    CND_MM_LOCAL_LITERAL_CASE_eTk(KwMain, "main");
    CND_MM_LOCAL_LITERAL_CASE_eTk(KwImport, "import");
    CND_MM_LOCAL_LITERAL_CASE_eTk(KwNamespace, "namespace");
    CND_MM_LOCAL_LITERAL_CASE_eTk(KwUse, "using");
    CND_MM_LOCAL_LITERAL_CASE_eTk(KwLib, "lib");
    CND_MM_LOCAL_LITERAL_CASE_eTk(KwDll, "dll");
    CND_MM_LOCAL_LITERAL_CASE_eTk(KwEnum, "enum");

    // Control
    CND_MM_LOCAL_LITERAL_CASE_eTk(KwIf, "if");
    CND_MM_LOCAL_LITERAL_CASE_eTk(KwElif, "elif");
    CND_MM_LOCAL_LITERAL_CASE_eTk(KwElse, "else");
    CND_MM_LOCAL_LITERAL_CASE_eTk(KwCxif, "cxif");
    CND_MM_LOCAL_LITERAL_CASE_eTk(KwCxelif, "cxelif");
    CND_MM_LOCAL_LITERAL_CASE_eTk(KwCxelse, "cxelse");
    CND_MM_LOCAL_LITERAL_CASE_eTk(KwSwitch, "switch");
    CND_MM_LOCAL_LITERAL_CASE_eTk(KwCase, "case");
    CND_MM_LOCAL_LITERAL_CASE_eTk(KwDefault, "default");
    CND_MM_LOCAL_LITERAL_CASE_eTk(KwWhile, "while");
    CND_MM_LOCAL_LITERAL_CASE_eTk(KwFor, "for");
    CND_MM_LOCAL_LITERAL_CASE_eTk(KwReturn, "return");
    CND_MM_LOCAL_LITERAL_CASE_eTk(KwBreak, "break");
    CND_MM_LOCAL_LITERAL_CASE_eTk(KwContinue, "continue");
    // Types
    CND_MM_LOCAL_LITERAL_CASE_eTk(KwInt, "int");
    CND_MM_LOCAL_LITERAL_CASE_eTk(KwUint, "uint");
    CND_MM_LOCAL_LITERAL_CASE_eTk(KwReal, "real");
    CND_MM_LOCAL_LITERAL_CASE_eTk(KwBool, "bool");
    CND_MM_LOCAL_LITERAL_CASE_eTk(KwChar, "char");
    CND_MM_LOCAL_LITERAL_CASE_eTk(KwByte, "byte");
    CND_MM_LOCAL_LITERAL_CASE_eTk(KwCstr, "cstr");
    CND_MM_LOCAL_LITERAL_CASE_eTk(KwStr, "str");
    CND_MM_LOCAL_LITERAL_CASE_eTk(KwPtr, "ptr");
    CND_MM_LOCAL_LITERAL_CASE_eTk(KwList, "list");
    CND_MM_LOCAL_LITERAL_CASE_eTk(KwArray, "array");
    // Constants
    CND_MM_LOCAL_LITERAL_CASE_eTk(KwTrue, "true");
    CND_MM_LOCAL_LITERAL_CASE_eTk(KwFalse, "false");
    CND_MM_LOCAL_LITERAL_CASE_eTk(KwNone, "none");
    CND_MM_LOCAL_LITERAL_CASE_eTk(KwVoid, "void");
    // Functional
    CND_MM_LOCAL_LITERAL_CASE_eTk(KwIn, "in");
    CND_MM_LOCAL_LITERAL_CASE_eTk(KwAs, "as");
    CND_MM_LOCAL_LITERAL_CASE_eTk(KwCin, "cin");
    CND_MM_LOCAL_LITERAL_CASE_eTk(KwCout, "cout");
    CND_MM_LOCAL_LITERAL_CASE_eTk(KwNative, "native");
    // Modifiers
    CND_MM_LOCAL_LITERAL_CASE_eTk(KwConst, "const");
    CND_MM_LOCAL_LITERAL_CASE_eTk(KwRef, "ref");
    CND_MM_LOCAL_LITERAL_CASE_eTk(KwPrivate, "private");
    CND_MM_LOCAL_LITERAL_CASE_eTk(KwPublic, "public");
    CND_MM_LOCAL_LITERAL_CASE_eTk(KwStatic, "static");
    // Meta Types
    CND_MM_LOCAL_LITERAL_CASE_eTk(KwAny, "any");
    CND_MM_LOCAL_LITERAL_CASE_eTk(KwAuto, "auto");
    CND_MM_LOCAL_LITERAL_CASE_eTk(KwType, "type");
    CND_MM_LOCAL_LITERAL_CASE_eTk(KwValue, "value");
    CND_MM_LOCAL_LITERAL_CASE_eTk(KwTemplate, "template");
    CND_MM_LOCAL_LITERAL_CASE_eTk(DirectiveInclude, "#include");
    CND_MM_LOCAL_LITERAL_CASE_eTk(DirectiveDefMacro, "#defmacro");
    CND_MM_LOCAL_LITERAL_CASE_eTk(DirectiveEndmacro, "#endmacro");
    CND_MM_LOCAL_LITERAL_CASE_eTk(DirectiveIf, "#if");
    CND_MM_LOCAL_LITERAL_CASE_eTk(DirectiveElse, "#else");
    CND_MM_LOCAL_LITERAL_CASE_eTk(DirectiveElif, "#elif");
    CND_MM_LOCAL_LITERAL_CASE_eTk(DirectiveEndif, "#endif");
    CND_MM_LOCAL_LITERAL_CASE_eTk(DirectiveIfdef, "#ifdef");
    CND_MM_LOCAL_LITERAL_CASE_eTk(DirectiveIfndef, "#ifndef");
    CND_MM_LOCAL_LITERAL_CASE_eTk(DirectiveUndef, "#undef");
    // Operators
    CND_MM_LOCAL_LITERAL_CASE_eTk(Hash, "#");
    CND_MM_LOCAL_LITERAL_CASE_eTk(Add, "+");
    CND_MM_LOCAL_LITERAL_CASE_eTk(Sub, "-");
    CND_MM_LOCAL_LITERAL_CASE_eTk(Mul, "*");
    CND_MM_LOCAL_LITERAL_CASE_eTk(Div, "/");
    CND_MM_LOCAL_LITERAL_CASE_eTk(Mod, "%");
    CND_MM_LOCAL_LITERAL_CASE_eTk(And, "&");
    CND_MM_LOCAL_LITERAL_CASE_eTk(Or, "|");
    CND_MM_LOCAL_LITERAL_CASE_eTk(Xor, "^");
    CND_MM_LOCAL_LITERAL_CASE_eTk(Not, "!");
    CND_MM_LOCAL_LITERAL_CASE_eTk(Lsh, "<<");
    CND_MM_LOCAL_LITERAL_CASE_eTk(Rsh, ">>");
    CND_MM_LOCAL_LITERAL_CASE_eTk(Eq, "==");
    CND_MM_LOCAL_LITERAL_CASE_eTk(Neq, "!=");
    CND_MM_LOCAL_LITERAL_CASE_eTk(Lt, "<");
    CND_MM_LOCAL_LITERAL_CASE_eTk(Gt, ">");
    CND_MM_LOCAL_LITERAL_CASE_eTk(Lte, "<=");
    CND_MM_LOCAL_LITERAL_CASE_eTk(Gte, ">=");
    CND_MM_LOCAL_LITERAL_CASE_eTk(Assign, "=");
    CND_MM_LOCAL_LITERAL_CASE_eTk(NewAssign, " : =");
    CND_MM_LOCAL_LITERAL_CASE_eTk(AddAssign, "+=");
    CND_MM_LOCAL_LITERAL_CASE_eTk(SubAssign, "-=");
    CND_MM_LOCAL_LITERAL_CASE_eTk(MulAssign, "*=");
    CND_MM_LOCAL_LITERAL_CASE_eTk(DivAssign, "/=");
    CND_MM_LOCAL_LITERAL_CASE_eTk(ModAssign, "%=");
    CND_MM_LOCAL_LITERAL_CASE_eTk(AndAssign, "&=");
    CND_MM_LOCAL_LITERAL_CASE_eTk(OrAssign, "|=");
    CND_MM_LOCAL_LITERAL_CASE_eTk(XorAssign, "^=");
    CND_MM_LOCAL_LITERAL_CASE_eTk(LshAssign, "<<=");
    CND_MM_LOCAL_LITERAL_CASE_eTk(RshAssign, ">>=");
    CND_MM_LOCAL_LITERAL_CASE_eTk(Inc, "++");
    CND_MM_LOCAL_LITERAL_CASE_eTk(Dec, "--");
    CND_MM_LOCAL_LITERAL_CASE_eTk(Dot, ".");
    CND_MM_LOCAL_LITERAL_CASE_eTk(Bnot, "~");
    CND_MM_LOCAL_LITERAL_CASE_eTk(Band, "&&");
    CND_MM_LOCAL_LITERAL_CASE_eTk(Bor, "||");
    CND_MM_LOCAL_LITERAL_CASE_eTk(Spaceship, "<=>");
    // Scopes
    CND_MM_LOCAL_LITERAL_CASE_eTk(LParen, "(");
    CND_MM_LOCAL_LITERAL_CASE_eTk(RParen, ")");
    CND_MM_LOCAL_LITERAL_CASE_eTk(LBrace, "{");
    CND_MM_LOCAL_LITERAL_CASE_eTk(RBrace, "}");
    CND_MM_LOCAL_LITERAL_CASE_eTk(LBracket, "[");
    CND_MM_LOCAL_LITERAL_CASE_eTk(RBracket, "]");
    CND_MM_LOCAL_LITERAL_CASE_eTk(Semicolon, ";");
    CND_MM_LOCAL_LITERAL_CASE_eTk(Colon, ":");
    CND_MM_LOCAL_LITERAL_CASE_eTk(Comma, ",");
    CND_MM_LOCAL_LITERAL_CASE_eTk(Period, ".");
    CND_MM_LOCAL_LITERAL_CASE_eTk(DoubleColon, "::");
    CND_MM_LOCAL_LITERAL_CASE_eTk(Ellipsis, "...");
    CND_MM_LOCAL_LITERAL_CASE_eTk(CommercialAt, "@");
    // Special
    CND_MM_LOCAL_LITERAL_CASE_eTk(Eofile, "\0");
    CND_MM_LOCAL_LITERAL_CASE_eTk(Whitespace, " ");
    CND_MM_LOCAL_LITERAL_CASE_eTk(Newline, "\n");
    CND_MM_LOCAL_LITERAL_CASE_eTk(BlockComment, "///");
    CND_MM_LOCAL_LITERAL_CASE_eTk(LineComment, "//");
    // Literals
    CND_MM_LOCAL_LITERAL_CASE_eTk(LitInt, "");
    CND_MM_LOCAL_LITERAL_CASE_eTk(LitUint, "");
    CND_MM_LOCAL_LITERAL_CASE_eTk(LitReal, "");
    CND_MM_LOCAL_LITERAL_CASE_eTk(LitBool, "");
    CND_MM_LOCAL_LITERAL_CASE_eTk(LitChar, "");
    CND_MM_LOCAL_LITERAL_CASE_eTk(LitByte, "");
    CND_MM_LOCAL_LITERAL_CASE_eTk(LitCstr, "");
    CND_MM_LOCAL_LITERAL_CASE_eTk(Ident, "");  // Identifier
    default:
      return "";
  }

#undef CND_MM_LOCAL_LITERAL_CASE_eTk
}

constexpr bool IsTkKeyword(eTk t) noexcept {
  switch (t) {
    using enum eTk;
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

constexpr bool IsTkModifier(eTk t) noexcept {
  switch (t) {
    using enum eTk;
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

constexpr bool IsTkDeclarative(eTk t) noexcept {
  // One of: def,fn,class,obj,use,lib,main,import
  switch (t) {
    using enum eTk;
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
    case kKwEnum:
      return true;
    default:
      return false;
  }
}

constexpr bool IsTkLScope(eTk t) noexcept {
  switch (t) {
    case eTk::kLParen:
    case eTk::kLBrace:
    case eTk::kLBracket:
      return true;
    default:
      return false;
  }
}

constexpr bool IsTkRScope(eTk t) noexcept {
  switch (t) {
    case eTk::kRParen:
    case eTk::kRBrace:
    case eTk::kRBracket:
      return true;
    default:
      return false;
  }
}

constexpr bool IsTkRScopeOf(eTk topen, eTk tclose) noexcept {
  using enum eTk;
  return (topen == kLParen && tclose == kRParen) || (topen == kLBrace && tclose == kRBrace) ||
         (topen == kLBracket && tclose == kRBracket);
}

constexpr bool IsTkAnOperand(eTk t) noexcept {
  switch (t) {
    using enum eTk;
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

constexpr bool IsTkAPrefixOperator(eTk t) noexcept {
  switch (t) {
    case eTk::kNot:
    case eTk::kBnot:
    case eTk::kDec:
    case eTk::kInc:
      return true;
    default:
      return false;
  }
}

constexpr bool IsTkPragmatic(eTk t) noexcept { return IsTkModifier(t) || IsTkDeclarative(t); }

constexpr bool IsTkPrimary(eTk t) noexcept { return IsTkAnOperand(t) || IsTkAPrefixOperator(t) || t == eTk::kLParen; }
constexpr bool IsTkPrimarySpecifier(eTk t) noexcept { return IsTkAnOperand(t) || IsTkAPrefixOperator(t) || t == eTk::kLParen || IsTkLScope(t)|| t == eTk::kDoubleColon; }

constexpr eTk GetTkFromKeyword(StrView kw) noexcept {
#define CND_MM_LOCAL_CASE(n, lt) else if (cxx::StrEq(kw, lt)) return eTk::k##n

  if (kw.empty()) return eTk::kNONE;
  // Declarative
  CND_MM_LOCAL_CASE(KwDef, "def");
  CND_MM_LOCAL_CASE(KwFn, "fn");
  CND_MM_LOCAL_CASE(KwClass, "class");
  CND_MM_LOCAL_CASE(KwMain, "main");
  CND_MM_LOCAL_CASE(KwImport, "import");
  CND_MM_LOCAL_CASE(KwNamespace, "namespace");
  CND_MM_LOCAL_CASE(KwUse, "using");
  CND_MM_LOCAL_CASE(KwLib, "lib");
  CND_MM_LOCAL_CASE(KwDll, "dll");
  CND_MM_LOCAL_CASE(KwEnum, "enum");
  CND_MM_LOCAL_CASE(KwProc, "proc");

  // Control
  CND_MM_LOCAL_CASE(KwIf, "if");
  CND_MM_LOCAL_CASE(KwElif, "elif");
  CND_MM_LOCAL_CASE(KwElse, "else");
  CND_MM_LOCAL_CASE(KwCxif, "cxif");
  CND_MM_LOCAL_CASE(KwCxelif, "cxelif");
  CND_MM_LOCAL_CASE(KwCxelse, "cxelse");
  CND_MM_LOCAL_CASE(KwSwitch, "switch");
  CND_MM_LOCAL_CASE(KwCase, "case");
  CND_MM_LOCAL_CASE(KwDefault, "default");
  CND_MM_LOCAL_CASE(KwWhile, "while");
  CND_MM_LOCAL_CASE(KwFor, "for");
  CND_MM_LOCAL_CASE(KwReturn, "return");
  CND_MM_LOCAL_CASE(KwBreak, "break");
  CND_MM_LOCAL_CASE(KwContinue, "continue");
  // Types
  CND_MM_LOCAL_CASE(KwInt, "int");
  CND_MM_LOCAL_CASE(KwUint, "uint");
  CND_MM_LOCAL_CASE(KwReal, "real");
  CND_MM_LOCAL_CASE(KwBool, "bool");
  CND_MM_LOCAL_CASE(KwChar, "char");
  CND_MM_LOCAL_CASE(KwByte, "byte");
  CND_MM_LOCAL_CASE(KwCstr, "cstr");
  CND_MM_LOCAL_CASE(KwStr, "str");
  CND_MM_LOCAL_CASE(KwPtr, "ptr");
  CND_MM_LOCAL_CASE(KwList, "list");
  CND_MM_LOCAL_CASE(KwArray, "array");
  // Constants
  CND_MM_LOCAL_CASE(KwTrue, "true");
  CND_MM_LOCAL_CASE(KwFalse, "false");
  CND_MM_LOCAL_CASE(KwNone, "none");
  CND_MM_LOCAL_CASE(KwVoid, "void");
  // Functional
  CND_MM_LOCAL_CASE(KwIn, "in");
  CND_MM_LOCAL_CASE(KwAs, "as");
  CND_MM_LOCAL_CASE(KwCin, "cin");
  CND_MM_LOCAL_CASE(KwCout, "cout");
  CND_MM_LOCAL_CASE(KwNative, "native");
  // Modifiers
  CND_MM_LOCAL_CASE(KwConst, "const");
  CND_MM_LOCAL_CASE(KwRef, "ref");
  CND_MM_LOCAL_CASE(KwPrivate, "private");
  CND_MM_LOCAL_CASE(KwPublic, "public");
  CND_MM_LOCAL_CASE(KwStatic, "static");
  // Meta Types
  CND_MM_LOCAL_CASE(KwAny, "any");
  CND_MM_LOCAL_CASE(KwAuto, "auto");
  CND_MM_LOCAL_CASE(KwType, "type");
  CND_MM_LOCAL_CASE(KwValue, "value");
  CND_MM_LOCAL_CASE(KwTemplate, "template");
  CND_MM_LOCAL_CASE(DirectiveInclude, "#include");
  CND_MM_LOCAL_CASE(DirectiveDefMacro, "#defmacro");
  CND_MM_LOCAL_CASE(DirectiveEndmacro, "#endmacro");
  CND_MM_LOCAL_CASE(DirectiveIf, "#if");
  CND_MM_LOCAL_CASE(DirectiveElse, "#else");
  CND_MM_LOCAL_CASE(DirectiveElif, "#elif");
  CND_MM_LOCAL_CASE(DirectiveEndif, "#endif");
  CND_MM_LOCAL_CASE(DirectiveIfdef, "#ifdef");
  CND_MM_LOCAL_CASE(DirectiveIfndef, "#ifndef");
  CND_MM_LOCAL_CASE(DirectiveUndef, "#undef");
  // Operators
  CND_MM_LOCAL_CASE(Hash, "#");
  CND_MM_LOCAL_CASE(Add, "+");
  CND_MM_LOCAL_CASE(Sub, "-");
  CND_MM_LOCAL_CASE(Mul, "*");
  CND_MM_LOCAL_CASE(Div, "/");
  CND_MM_LOCAL_CASE(Mod, "%");
  CND_MM_LOCAL_CASE(And, "&");
  CND_MM_LOCAL_CASE(Or, "|");
  CND_MM_LOCAL_CASE(Xor, "^");
  CND_MM_LOCAL_CASE(Not, "!");
  CND_MM_LOCAL_CASE(Lsh, "<<");
  CND_MM_LOCAL_CASE(Rsh, ">>");
  CND_MM_LOCAL_CASE(Eq, "==");
  CND_MM_LOCAL_CASE(Neq, "!=");
  CND_MM_LOCAL_CASE(Lt, "<");
  CND_MM_LOCAL_CASE(Gt, ">");
  CND_MM_LOCAL_CASE(Lte, "<=");
  CND_MM_LOCAL_CASE(Gte, ">=");
  CND_MM_LOCAL_CASE(Assign, "=");
  CND_MM_LOCAL_CASE(NewAssign, " : =");
  CND_MM_LOCAL_CASE(AddAssign, "+=");
  CND_MM_LOCAL_CASE(SubAssign, "-=");
  CND_MM_LOCAL_CASE(MulAssign, "*=");
  CND_MM_LOCAL_CASE(DivAssign, "/=");
  CND_MM_LOCAL_CASE(ModAssign, "%=");
  CND_MM_LOCAL_CASE(AndAssign, "&=");
  CND_MM_LOCAL_CASE(OrAssign, "|=");
  CND_MM_LOCAL_CASE(XorAssign, "^=");
  CND_MM_LOCAL_CASE(LshAssign, "<<=");
  CND_MM_LOCAL_CASE(RshAssign, ">>=");
  CND_MM_LOCAL_CASE(Inc, "++");
  CND_MM_LOCAL_CASE(Dec, "--");
  CND_MM_LOCAL_CASE(Dot, ".");
  CND_MM_LOCAL_CASE(Bnot, "~");
  CND_MM_LOCAL_CASE(Band, "&&");
  CND_MM_LOCAL_CASE(Bor, "||");
  CND_MM_LOCAL_CASE(Spaceship, "<=>");
  // Scopes
  CND_MM_LOCAL_CASE(LParen, "(");
  CND_MM_LOCAL_CASE(RParen, ")");
  CND_MM_LOCAL_CASE(LBrace, "{");
  CND_MM_LOCAL_CASE(RBrace, "}");
  CND_MM_LOCAL_CASE(LBracket, "[");
  CND_MM_LOCAL_CASE(RBracket, "]");
  CND_MM_LOCAL_CASE(Semicolon, ";");
  CND_MM_LOCAL_CASE(Colon, ":");
  CND_MM_LOCAL_CASE(Comma, ",");
  CND_MM_LOCAL_CASE(Period, ".");
  CND_MM_LOCAL_CASE(DoubleColon, "::");
  CND_MM_LOCAL_CASE(Ellipsis, "...");
  CND_MM_LOCAL_CASE(CommercialAt, "@");
  // Special
  CND_MM_LOCAL_CASE(Eofile, "\0");
  CND_MM_LOCAL_CASE(Whitespace, " ");
  CND_MM_LOCAL_CASE(Newline, "\n");
  CND_MM_LOCAL_CASE(BlockComment, "///");
  CND_MM_LOCAL_CASE(LineComment, "//");
#undef CND_MM_LOCAL_CASE
  else return eTk::kNONE;
};

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