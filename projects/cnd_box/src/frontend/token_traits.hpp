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
/// @brief Traits of eToken.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "enum_assoc.hpp"
#include "enum_ast.hpp"
#include "enum_operation.hpp"
#include "enum_priority.hpp"
#include "enum_token.hpp"

/// @addtogroup cnd_compiler_corevalssss
/// @{

namespace ssgc::frontend {
constexpr eAssoc getTokenAssoc(eToken t) noexcept {
  using enum eToken;
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
      return eAssoc::kRight;
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
      return eAssoc::kLeft;
    default:
      return eAssoc::kNone;
  }
}

constexpr eOperation getTokenOperation(eToken t) noexcept {
  using enum eToken;
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
      return eOperation::kBinary;
      break;
    case kInc:
    case kDec:
      return eOperation::kPostfix;
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
      return eOperation::kBinary;
      break;
    case kNot:
    case kBnot:
      return eOperation::kPrefix;
      break;
    case kAnd:
    case kOr:
    case kEq:
    case kNeq:
    case kLt:
    case kGt:
    case kLte:
    case kGte:
      return eOperation::kBinary;
    case kLParen:
    case kRParen:
    case kLBrace:
    case kRBrace:
    case kLBracket:
    case kRBracket:
      return eOperation::kPostfix;
    case kDoubleColon:
    case kPeriod:
      return eOperation::kBinary;
    case kEllipsis:
      return eOperation::kBinary;
    case kCommercialAt:
      return eOperation::kPrefix;
    default:
      return eOperation::kNone;
  }
}

constexpr ePriority getTokenPriority(eToken t) noexcept {
  using enum eToken;
  switch (t) {
    case kNone:
    case kInvalid:
    case kEofile:
    case kLineComment:
    case kBlockComment:
    case kNewline:
    case kWhitespace:
      return ePriority::kNone;
      break;
    case kLitCstr:
    case kLitInt:
    case kLitUint:
    case kLitReal:
    case kLitBool:
    case kLitChar:
    case kLitByte:
    case kIdent:
      return ePriority::kMax;
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
      return ePriority::kAssignment;
      break;
    case kInc:
    case kDec:
      return ePriority::kPostfix;
      break;
    case kAdd:
    case kSub:
      return ePriority::kTerm;
      break;
    case kMul:
    case kDiv:
    case kMod:
      return ePriority::kFactor;
      break;
    case kNot:
    case kBnot:
      return ePriority::kPrefix;
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
      return ePriority::kComparison;
      break;
    case kLParen:
    case kRParen:
    case kLBrace:
    case kRBrace:
    case kLBracket:
    case kRBracket:
      return ePriority::kPostfix;
      break;
    case kSemicolon:
    case kColon:
    case kComma:
    case kEllipsis:
    case kCommercialAt:
      return ePriority::kMax;
      break;
    case kPeriod:
    case kDoubleColon:
      return ePriority::kAccess;
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
      return ePriority::kMax;
      break;
    default:
      return ePriority::kNone;
  }
}

constexpr const char* getTokenSymbol(eToken t) noexcept {
#define CND_MM_LOCAL_LITERAL_CASE_eToken(n, lt) \
  case eToken::k##n:                            \
    return lt;

  switch (t) {
    CND_MM_LOCAL_LITERAL_CASE_eToken(None, "");
    CND_MM_LOCAL_LITERAL_CASE_eToken(Invalid, "");
    // Declarative
    CND_MM_LOCAL_LITERAL_CASE_eToken(KwDef, "def");
    CND_MM_LOCAL_LITERAL_CASE_eToken(KwFn, "fn");
    CND_MM_LOCAL_LITERAL_CASE_eToken(KwClass, "class");
    CND_MM_LOCAL_LITERAL_CASE_eToken(KwMain, "main");
    CND_MM_LOCAL_LITERAL_CASE_eToken(KwImport, "import");
    CND_MM_LOCAL_LITERAL_CASE_eToken(KwNamespace, "namespace");
    CND_MM_LOCAL_LITERAL_CASE_eToken(KwUse, "using");
    CND_MM_LOCAL_LITERAL_CASE_eToken(KwLib, "lib");
    CND_MM_LOCAL_LITERAL_CASE_eToken(KwDll, "dll");
    CND_MM_LOCAL_LITERAL_CASE_eToken(KwEnum, "enum");

    // Control
    CND_MM_LOCAL_LITERAL_CASE_eToken(KwIf, "if");
    CND_MM_LOCAL_LITERAL_CASE_eToken(KwElif, "elif");
    CND_MM_LOCAL_LITERAL_CASE_eToken(KwElse, "else");
    CND_MM_LOCAL_LITERAL_CASE_eToken(KwCxif, "cxif");
    CND_MM_LOCAL_LITERAL_CASE_eToken(KwCxelif, "cxelif");
    CND_MM_LOCAL_LITERAL_CASE_eToken(KwCxelse, "cxelse");
    CND_MM_LOCAL_LITERAL_CASE_eToken(KwSwitch, "switch");
    CND_MM_LOCAL_LITERAL_CASE_eToken(KwCase, "case");
    CND_MM_LOCAL_LITERAL_CASE_eToken(KwDefault, "default");
    CND_MM_LOCAL_LITERAL_CASE_eToken(KwWhile, "while");
    CND_MM_LOCAL_LITERAL_CASE_eToken(KwFor, "for");
    CND_MM_LOCAL_LITERAL_CASE_eToken(KwReturn, "return");
    CND_MM_LOCAL_LITERAL_CASE_eToken(KwBreak, "break");
    CND_MM_LOCAL_LITERAL_CASE_eToken(KwContinue, "continue");
    // Types
    CND_MM_LOCAL_LITERAL_CASE_eToken(KwInt, "int");
    CND_MM_LOCAL_LITERAL_CASE_eToken(KwUint, "uint");
    CND_MM_LOCAL_LITERAL_CASE_eToken(KwReal, "real");
    CND_MM_LOCAL_LITERAL_CASE_eToken(KwBool, "bool");
    CND_MM_LOCAL_LITERAL_CASE_eToken(KwChar, "char");
    CND_MM_LOCAL_LITERAL_CASE_eToken(KwByte, "byte");
    CND_MM_LOCAL_LITERAL_CASE_eToken(KwCstr, "cstr");
    CND_MM_LOCAL_LITERAL_CASE_eToken(KwStr, "str");
    CND_MM_LOCAL_LITERAL_CASE_eToken(KwPtr, "ptr");
    CND_MM_LOCAL_LITERAL_CASE_eToken(KwList, "list");
    CND_MM_LOCAL_LITERAL_CASE_eToken(KwArray, "array");
    // Constants
    CND_MM_LOCAL_LITERAL_CASE_eToken(KwTrue, "true");
    CND_MM_LOCAL_LITERAL_CASE_eToken(KwFalse, "false");
    CND_MM_LOCAL_LITERAL_CASE_eToken(KwNone, "none");
    CND_MM_LOCAL_LITERAL_CASE_eToken(KwVoid, "void");
    // Functional
    CND_MM_LOCAL_LITERAL_CASE_eToken(KwIn, "in");
    CND_MM_LOCAL_LITERAL_CASE_eToken(KwAs, "as");
    CND_MM_LOCAL_LITERAL_CASE_eToken(KwCin, "cin");
    CND_MM_LOCAL_LITERAL_CASE_eToken(KwCout, "cout");
    CND_MM_LOCAL_LITERAL_CASE_eToken(KwNative, "native");
    // Modifiers
    CND_MM_LOCAL_LITERAL_CASE_eToken(KwConst, "const");
    CND_MM_LOCAL_LITERAL_CASE_eToken(KwRef, "ref");
    CND_MM_LOCAL_LITERAL_CASE_eToken(KwPrivate, "private");
    CND_MM_LOCAL_LITERAL_CASE_eToken(KwPublic, "public");
    CND_MM_LOCAL_LITERAL_CASE_eToken(KwStatic, "static");
    // Meta Types
    CND_MM_LOCAL_LITERAL_CASE_eToken(KwAny, "any");
    CND_MM_LOCAL_LITERAL_CASE_eToken(KwAuto, "auto");
    CND_MM_LOCAL_LITERAL_CASE_eToken(KwType, "type");
    CND_MM_LOCAL_LITERAL_CASE_eToken(KwValue, "value");
    CND_MM_LOCAL_LITERAL_CASE_eToken(KwTemplate, "template");
    CND_MM_LOCAL_LITERAL_CASE_eToken(DirectiveInclude, "#include");
    CND_MM_LOCAL_LITERAL_CASE_eToken(DirectiveDefMacro, "#defmacro");
    CND_MM_LOCAL_LITERAL_CASE_eToken(DirectiveEndmacro, "#endmacro");
    CND_MM_LOCAL_LITERAL_CASE_eToken(DirectiveIf, "#if");
    CND_MM_LOCAL_LITERAL_CASE_eToken(DirectiveElse, "#else");
    CND_MM_LOCAL_LITERAL_CASE_eToken(DirectiveElif, "#elif");
    CND_MM_LOCAL_LITERAL_CASE_eToken(DirectiveEndif, "#endif");
    CND_MM_LOCAL_LITERAL_CASE_eToken(DirectiveIfdef, "#ifdef");
    CND_MM_LOCAL_LITERAL_CASE_eToken(DirectiveIfndef, "#ifndef");
    CND_MM_LOCAL_LITERAL_CASE_eToken(DirectiveUndef, "#undef");
    // Operators
    CND_MM_LOCAL_LITERAL_CASE_eToken(Hash, "#");
    CND_MM_LOCAL_LITERAL_CASE_eToken(Add, "+");
    CND_MM_LOCAL_LITERAL_CASE_eToken(Sub, "-");
    CND_MM_LOCAL_LITERAL_CASE_eToken(Mul, "*");
    CND_MM_LOCAL_LITERAL_CASE_eToken(Div, "/");
    CND_MM_LOCAL_LITERAL_CASE_eToken(Mod, "%");
    CND_MM_LOCAL_LITERAL_CASE_eToken(And, "&");
    CND_MM_LOCAL_LITERAL_CASE_eToken(Or, "|");
    CND_MM_LOCAL_LITERAL_CASE_eToken(Xor, "^");
    CND_MM_LOCAL_LITERAL_CASE_eToken(Not, "!");
    CND_MM_LOCAL_LITERAL_CASE_eToken(Lsh, "<<");
    CND_MM_LOCAL_LITERAL_CASE_eToken(Rsh, ">>");
    CND_MM_LOCAL_LITERAL_CASE_eToken(Eq, "==");
    CND_MM_LOCAL_LITERAL_CASE_eToken(Neq, "!=");
    CND_MM_LOCAL_LITERAL_CASE_eToken(Lt, "<");
    CND_MM_LOCAL_LITERAL_CASE_eToken(Gt, ">");
    CND_MM_LOCAL_LITERAL_CASE_eToken(Lte, "<=");
    CND_MM_LOCAL_LITERAL_CASE_eToken(Gte, ">=");
    CND_MM_LOCAL_LITERAL_CASE_eToken(Assign, "=");
    CND_MM_LOCAL_LITERAL_CASE_eToken(NewAssign, " : =");
    CND_MM_LOCAL_LITERAL_CASE_eToken(AddAssign, "+=");
    CND_MM_LOCAL_LITERAL_CASE_eToken(SubAssign, "-=");
    CND_MM_LOCAL_LITERAL_CASE_eToken(MulAssign, "*=");
    CND_MM_LOCAL_LITERAL_CASE_eToken(DivAssign, "/=");
    CND_MM_LOCAL_LITERAL_CASE_eToken(ModAssign, "%=");
    CND_MM_LOCAL_LITERAL_CASE_eToken(AndAssign, "&=");
    CND_MM_LOCAL_LITERAL_CASE_eToken(OrAssign, "|=");
    CND_MM_LOCAL_LITERAL_CASE_eToken(XorAssign, "^=");
    CND_MM_LOCAL_LITERAL_CASE_eToken(LshAssign, "<<=");
    CND_MM_LOCAL_LITERAL_CASE_eToken(RshAssign, ">>=");
    CND_MM_LOCAL_LITERAL_CASE_eToken(Inc, "++");
    CND_MM_LOCAL_LITERAL_CASE_eToken(Dec, "--");
    CND_MM_LOCAL_LITERAL_CASE_eToken(Bnot, "~");
    CND_MM_LOCAL_LITERAL_CASE_eToken(Band, "&&");
    CND_MM_LOCAL_LITERAL_CASE_eToken(Bor, "||");
    CND_MM_LOCAL_LITERAL_CASE_eToken(Spaceship, "<=>");
    // Scopes
    CND_MM_LOCAL_LITERAL_CASE_eToken(LParen, "(");
    CND_MM_LOCAL_LITERAL_CASE_eToken(RParen, ")");
    CND_MM_LOCAL_LITERAL_CASE_eToken(LBrace, "{");
    CND_MM_LOCAL_LITERAL_CASE_eToken(RBrace, "}");
    CND_MM_LOCAL_LITERAL_CASE_eToken(LBracket, "[");
    CND_MM_LOCAL_LITERAL_CASE_eToken(RBracket, "]");
    CND_MM_LOCAL_LITERAL_CASE_eToken(Semicolon, ";");
    CND_MM_LOCAL_LITERAL_CASE_eToken(Colon, ":");
    CND_MM_LOCAL_LITERAL_CASE_eToken(Comma, ",");
    CND_MM_LOCAL_LITERAL_CASE_eToken(Period, ".");
    CND_MM_LOCAL_LITERAL_CASE_eToken(DoubleColon, "::");
    CND_MM_LOCAL_LITERAL_CASE_eToken(Ellipsis, "...");
    CND_MM_LOCAL_LITERAL_CASE_eToken(CommercialAt, "@");
    // Special
    CND_MM_LOCAL_LITERAL_CASE_eToken(Eofile, "\0");
    CND_MM_LOCAL_LITERAL_CASE_eToken(Whitespace, " ");
    CND_MM_LOCAL_LITERAL_CASE_eToken(Newline, "\n");
    CND_MM_LOCAL_LITERAL_CASE_eToken(BlockComment, "///");
    CND_MM_LOCAL_LITERAL_CASE_eToken(LineComment, "//");
    // Literals
    CND_MM_LOCAL_LITERAL_CASE_eToken(LitInt, "");
    CND_MM_LOCAL_LITERAL_CASE_eToken(LitUint, "");
    CND_MM_LOCAL_LITERAL_CASE_eToken(LitReal, "");
    CND_MM_LOCAL_LITERAL_CASE_eToken(LitBool, "");
    CND_MM_LOCAL_LITERAL_CASE_eToken(LitChar, "");
    CND_MM_LOCAL_LITERAL_CASE_eToken(LitByte, "");
    CND_MM_LOCAL_LITERAL_CASE_eToken(LitCstr, "");
    CND_MM_LOCAL_LITERAL_CASE_eToken(Ident, "");  // Identifier
    default:
      return "";
  }

#undef CND_MM_LOCAL_LITERAL_CASE_eToken
}

constexpr bool isTokenKeyword(eToken t) noexcept {
  switch (t) {
    using enum eToken;
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

constexpr bool isTokenModifier(eToken t) noexcept {
  switch (t) {
    using enum eToken;
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

constexpr bool isTokenDeclarative(eToken t) noexcept {
  // One of: def,fn,class,obj,use,lib,main,import
  switch (t) {
    using enum eToken;
    case kKwProc:
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

constexpr bool isTokenOpeningScope(eToken t) noexcept {
  switch (t) {
    case eToken::kLParen:
    case eToken::kLBrace:
    case eToken::kLBracket:
      return true;
    default:
      return false;
  }
}

constexpr bool isTokenClosingScope(eToken t) noexcept {
  switch (t) {
    case eToken::kRParen:
    case eToken::kRBrace:
    case eToken::kRBracket:
      return true;
    default:
      return false;
  }
}

constexpr bool isTokenClosingScopeOf(eToken topen, eToken tclose) noexcept {
  using enum eToken;
  return (topen == kLParen && tclose == kRParen) || (topen == kLBrace && tclose == kRBrace) ||
         (topen == kLBracket && tclose == kRBracket);
}

constexpr bool isTokenAnOperand(eToken t) noexcept {
  switch (t) {
    using enum eToken;
    case kLitU1:
    case kLitU16:
    case kLitU32:
    case kLitU64:
    case kLitU8:
    case kLitI8:
    case kLitI16:
    case kLitI32:
    case kLitI64:
    case kLitF32:
    case kLitF64:
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

constexpr bool isTokenAPrefixOperator(eToken t) noexcept {
  switch (t) {
    case eToken::kNot:
    case eToken::kBnot:
    case eToken::kDec:
    case eToken::kInc:
    case eToken::kAdd:
    case eToken::kSub:
    case eToken::kBand:
    case eToken::kMul:
      return true;
    default:
      return false;
  }
}

constexpr bool isTokenPostfixOperator(eToken t) noexcept {
  switch (t) {
    case eToken::kInc:
    case eToken::kDec:
      return true;
    default:
      return false;
  }
}

constexpr bool isTokenPragmatic(eToken t) noexcept {
  return isTokenModifier(t) || isTokenDeclarative(t);
}

constexpr bool isTokenPrimary(eToken t) noexcept {
  return isTokenAnOperand(t) || isTokenAPrefixOperator(t) || t == eToken::kLParen;
}

constexpr bool isTokenPrimarySpecifier(eToken t) noexcept {
  return isTokenAnOperand(t) || isTokenAPrefixOperator(t) || isTokenOpeningScope(t) ||
         t == eToken::kDoubleColon;
}

constexpr eToken getTokenFromKeyword(std::string_view kw) noexcept {
#define CND_MM_LOCAL_CASE(n, lt) else if (kw == lt) return eToken::n

  if (kw.empty()) return eToken::kNone;
  // Declarative
  CND_MM_LOCAL_CASE(kKwDef, "def");
  CND_MM_LOCAL_CASE(kKwFn, "fn");
  CND_MM_LOCAL_CASE(kKwClass, "class");
  CND_MM_LOCAL_CASE(kKwMain, "main");
  CND_MM_LOCAL_CASE(kKwImport, "import");
  CND_MM_LOCAL_CASE(kKwNamespace, "namespace");
  CND_MM_LOCAL_CASE(kKwUse, "using");
  CND_MM_LOCAL_CASE(kKwLib, "lib");
  CND_MM_LOCAL_CASE(kKwDll, "dll");
  CND_MM_LOCAL_CASE(kKwEnum, "enum");
  CND_MM_LOCAL_CASE(kKwProc, "proc");

  // Control
  CND_MM_LOCAL_CASE(kKwIf, "if");
  CND_MM_LOCAL_CASE(kKwElif, "elif");
  CND_MM_LOCAL_CASE(kKwElse, "else");
  CND_MM_LOCAL_CASE(kKwCxif, "cxif");
  CND_MM_LOCAL_CASE(kKwCxelif, "cxelif");
  CND_MM_LOCAL_CASE(kKwCxelse, "cxelse");
  CND_MM_LOCAL_CASE(kKwSwitch, "switch");
  CND_MM_LOCAL_CASE(kKwCase, "case");
  CND_MM_LOCAL_CASE(kKwDefault, "default");
  CND_MM_LOCAL_CASE(kKwWhile, "while");
  CND_MM_LOCAL_CASE(kKwFor, "for");
  CND_MM_LOCAL_CASE(kKwReturn, "return");
  CND_MM_LOCAL_CASE(kKwBreak, "break");
  CND_MM_LOCAL_CASE(kKwContinue, "continue");
  // Types
  CND_MM_LOCAL_CASE(kKwInt, "int");
  CND_MM_LOCAL_CASE(kKwUint, "uint");
  CND_MM_LOCAL_CASE(kKwReal, "real");
  CND_MM_LOCAL_CASE(kKwBool, "bool");
  CND_MM_LOCAL_CASE(kKwChar, "char");
  CND_MM_LOCAL_CASE(kKwByte, "byte");
  CND_MM_LOCAL_CASE(kKwCstr, "cstr");
  CND_MM_LOCAL_CASE(kKwStr, "str");
  CND_MM_LOCAL_CASE(kKwPtr, "ptr");
  CND_MM_LOCAL_CASE(kKwList, "list");
  CND_MM_LOCAL_CASE(kKwArray, "array");
  // Constants
  CND_MM_LOCAL_CASE(kKwTrue, "true");
  CND_MM_LOCAL_CASE(kKwFalse, "false");
  CND_MM_LOCAL_CASE(kKwNone, "none");
  CND_MM_LOCAL_CASE(kKwVoid, "void");
  // Functional
  CND_MM_LOCAL_CASE(kKwIn, "in");
  CND_MM_LOCAL_CASE(kKwAs, "as");
  CND_MM_LOCAL_CASE(kKwCin, "cin");
  CND_MM_LOCAL_CASE(kKwCout, "cout");
  CND_MM_LOCAL_CASE(kKwNative, "native");
  // Modifiers
  CND_MM_LOCAL_CASE(kKwConst, "const");
  CND_MM_LOCAL_CASE(kKwRef, "ref");
  CND_MM_LOCAL_CASE(kKwPrivate, "private");
  CND_MM_LOCAL_CASE(kKwPublic, "public");
  CND_MM_LOCAL_CASE(kKwStatic, "static");
  // Meta Types
  CND_MM_LOCAL_CASE(kKwAny, "any");
  CND_MM_LOCAL_CASE(kKwAuto, "auto");
  CND_MM_LOCAL_CASE(kKwType, "type");
  CND_MM_LOCAL_CASE(kKwValue, "value");
  CND_MM_LOCAL_CASE(kKwTemplate, "template");
  CND_MM_LOCAL_CASE(kDirectiveInclude, "#include");
  CND_MM_LOCAL_CASE(kDirectiveDefMacro, "#defmacro");
  CND_MM_LOCAL_CASE(kDirectiveEndmacro, "#endmacro");
  CND_MM_LOCAL_CASE(kDirectiveIf, "#if");
  CND_MM_LOCAL_CASE(kDirectiveElse, "#else");
  CND_MM_LOCAL_CASE(kDirectiveElif, "#elif");
  CND_MM_LOCAL_CASE(kDirectiveEndif, "#endif");
  CND_MM_LOCAL_CASE(kDirectiveIfdef, "#ifdef");
  CND_MM_LOCAL_CASE(kDirectiveIfndef, "#ifndef");
  CND_MM_LOCAL_CASE(kDirectiveUndef, "#undef");
  // Operators
  CND_MM_LOCAL_CASE(kHash, "#");
  CND_MM_LOCAL_CASE(kAdd, "+");
  CND_MM_LOCAL_CASE(kSub, "-");
  CND_MM_LOCAL_CASE(kMul, "*");
  CND_MM_LOCAL_CASE(kDiv, "/");
  CND_MM_LOCAL_CASE(kMod, "%");
  CND_MM_LOCAL_CASE(kAnd, "&");
  CND_MM_LOCAL_CASE(kOr, "|");
  CND_MM_LOCAL_CASE(kXor, "^");
  CND_MM_LOCAL_CASE(kNot, "!");
  CND_MM_LOCAL_CASE(kLsh, "<<");
  CND_MM_LOCAL_CASE(kRsh, ">>");
  CND_MM_LOCAL_CASE(kEq, "==");
  CND_MM_LOCAL_CASE(kNeq, "!=");
  CND_MM_LOCAL_CASE(kLt, "<");
  CND_MM_LOCAL_CASE(kGt, ">");
  CND_MM_LOCAL_CASE(kLte, "<=");
  CND_MM_LOCAL_CASE(kGte, ">=");
  CND_MM_LOCAL_CASE(kAssign, "=");
  CND_MM_LOCAL_CASE(kNewAssign, " : =");
  CND_MM_LOCAL_CASE(kAddAssign, "+=");
  CND_MM_LOCAL_CASE(kSubAssign, "-=");
  CND_MM_LOCAL_CASE(kMulAssign, "*=");
  CND_MM_LOCAL_CASE(kDivAssign, "/=");
  CND_MM_LOCAL_CASE(kModAssign, "%=");
  CND_MM_LOCAL_CASE(kAndAssign, "&=");
  CND_MM_LOCAL_CASE(kOrAssign, "|=");
  CND_MM_LOCAL_CASE(kXorAssign, "^=");
  CND_MM_LOCAL_CASE(kLshAssign, "<<=");
  CND_MM_LOCAL_CASE(kRshAssign, ">>=");
  CND_MM_LOCAL_CASE(kInc, "++");
  CND_MM_LOCAL_CASE(kDec, "--");
  CND_MM_LOCAL_CASE(kBnot, "~");
  CND_MM_LOCAL_CASE(kBand, "&&");
  CND_MM_LOCAL_CASE(kBor, "||");
  CND_MM_LOCAL_CASE(kSpaceship, "<=>");
  // Scopes
  CND_MM_LOCAL_CASE(kLParen, "(");
  CND_MM_LOCAL_CASE(kRParen, ")");
  CND_MM_LOCAL_CASE(kLBrace, "{");
  CND_MM_LOCAL_CASE(kRBrace, "}");
  CND_MM_LOCAL_CASE(kLBracket, "[");
  CND_MM_LOCAL_CASE(kRBracket, "]");
  CND_MM_LOCAL_CASE(kSemicolon, ";");
  CND_MM_LOCAL_CASE(kColon, ":");
  CND_MM_LOCAL_CASE(kComma, ",");
  CND_MM_LOCAL_CASE(kPeriod, ".");
  CND_MM_LOCAL_CASE(kDoubleColon, "::");
  CND_MM_LOCAL_CASE(kEllipsis, "...");
  CND_MM_LOCAL_CASE(kCommercialAt, "@");
  // Special
  CND_MM_LOCAL_CASE(kEofile, "\0");
  CND_MM_LOCAL_CASE(kWhitespace, " ");
  CND_MM_LOCAL_CASE(kNewline, "\n");
  CND_MM_LOCAL_CASE(kBlockComment, "///");
  CND_MM_LOCAL_CASE(kLineComment, "//");
#undef CND_MM_LOCAL_CASE
  else return eToken::kNone;
};

constexpr eAst getAstFromToken(eToken t) noexcept {
// Function local macro.
#define SSGC_LOCAL_TokenToAst(n) \
  case eToken::n:                \
    return eAst::n;

  // Manually add a case for the COUNT entry.
  // Switch over all of etk and return the equivalent mapped symbol.
  // Default can't ever happen because eAst must contain all of eTk.
  switch (t) {
    SSGC_AppliedEnum_eToken(SSGC_LOCAL_TokenToAst, , ,
                            case eToken::COUNT : return eAst::COUNT;) default : __assume(false);
  }

#undef SSGC_LOCAL_TokenToAst
}

// Valid first terminal in a top level syntax statement.
constexpr bool isTokenDirectiveSpecifier(eToken t) noexcept {
  return isTokenModifier(t) || isTokenDeclarative(t) || isTokenPrimary(t);
}

constexpr bool isTokenInsignificant(eToken t) {
  switch (t) {
    case eToken::kLineComment:
    case eToken::kBlockComment:
    case eToken::kWhitespace:
    case eToken::kNewline:
      return true;
    default:
      return false;
  }
}

constexpr bool isTokenAssignmentOperator(eToken t) {
    switch (t) {
      case eToken::kAssign:
      case eToken::kNewAssign:
      case eToken::kAddAssign:
      case eToken::kSubAssign:
      case eToken::kMulAssign:
      case eToken::kModAssign:
      case eToken::kDivAssign:
      case eToken::kAndAssign:
      case eToken::kOrAssign:
      case eToken::kXorAssign:
      case eToken::kLshAssign:
      case eToken::kRshAssign:
        return true;
      default:
        return false;
    };
}
}  // namespace ssgc::frontend

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