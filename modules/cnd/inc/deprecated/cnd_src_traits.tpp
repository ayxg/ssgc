#pragma once
#include "cnd_src_traits.hpp"

namespace cnd {
namespace src_traits {

CND_CX bool IsAlpha(char c) {
  switch (c) {
    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
    case 'g':
    case 'h':
    case 'i':
    case 'j':
    case 'k':
    case 'l':
    case 'm':
    case 'n':
    case 'o':
    case 'p':
    case 'q':
    case 'r':
    case 's':
    case 't':
    case 'u':
    case 'v':
    case 'w':
    case 'x':
    case 'y':
    case 'z':
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
    case 'G':
    case 'H':
    case 'I':
    case 'J':
    case 'K':
    case 'L':
    case 'M':
    case 'N':
    case 'O':
    case 'P':
    case 'Q':
    case 'R':
    case 'S':
    case 'T':
    case 'U':
    case 'V':
    case 'W':
    case 'X':
    case 'Y':
    case 'Z':
      return true;
    default:
      return false;
      break;
  }
}

CND_CX bool IsNumeric(char c) {
  switch (c) {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      return true;
    default:
      return false;
      break;
  }
}

CND_CX bool IsAlphanumeric(char c) { return IsAlpha(c) || IsNumeric(c); }

CND_CX bool IsAlphaUnderscore(char c) { return IsAlpha(c) || c == '_'; }

CND_CX bool IsAlnumus(char c) {
  return IsAlpha(c) || IsNumeric(c) || c == '_';
}

/// '!', '@', '#', '$', '%', '^', '&', '*', '-','+', '=', '{', '}', '[',
/// ']','|', '\\', ';', ':', '\'', '\"', '<', '>', '?', '/', '~', '`', '.',
/// ',','(', ')', '_'
CND_CX bool IsPunctuator(char c) {
  switch (c) {
    case '!':
    case '@':
    case '#':
    case '$':
    case '%':
    case '^':
    case '&':
    case '*':
    case '-':
    case '+':
    case '=':
    case '{':
    case '}':
    case '[':
    case ']':
    case '|':
    case '\\':
    case ';':
    case ':':
    case '\'':
    case '\"':
    case '<':
    case '>':
    case '?':
    case '/':
    case '~':
    case '`':
    case '.':
    case ',':
    case '(':
    case ')':
    case '_':
      return true;
    default:
      return false;
      break;
  }
}

/// Tab or space.
CND_CX bool IsSpace(char c) { return c == ' ' || c == '\t'; }

/// '\n', '\r', '\v', '\f'
CND_CX bool IsNewline(char c) {
  switch (c) {
    case '\n':
    case '\r':
    case '\v':
    case '\f':
      return true;
    default:
      return false;
      break;
  }
}

/// ' ', '\n', '\r', '\v', '\f'
CND_CX bool IsWhitespace(char c) {
  switch (c) {
    case ' ':
    case '\n':
    case '\r':
    case '\v':
    case '\f':
    case '\t':
      return true;
    default:
      return false;
      break;
  }
}

// '\0', '\a', '\b', '\t', '\n', '\v', '\f', '\r', '\x1b'
CND_CX bool IsControl(char c) {
  switch (c) {
    case '\0':
    case '\a':
    case '\b':
    case '\t':
    case '\n':
    case '\v':
    case '\f':
    case '\r':
    case '\x1b':
      return true;
    default:
      return false;
  }
}

CND_CX bool IsValid(char c) {
  return IsAlpha(c) || IsNumeric(c) || IsPunctuator(c) || IsWhitespace(c);
}

CND_CX eAssoc TkAssoc(eTk t) {
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

CND_CX eOperation TkOperation(eTk t) {
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

CND_CX ePriority TkPriority(eTk t) {
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

CND_CX const char* TkLiteral(eTk t) {
#define __CND_MMLOCAL_LITERAL_CASE_eTk(n, lt) \
  case eTk::k##n:                             \
    return lt;

  switch (t) {
    __CND_MMLOCAL_LITERAL_CASE_eTk(NONE, "");
    __CND_MMLOCAL_LITERAL_CASE_eTk(INVALID, "");
    // Declarative
    __CND_MMLOCAL_LITERAL_CASE_eTk(KwDef, "def");
    __CND_MMLOCAL_LITERAL_CASE_eTk(KwFn, "fn");
    __CND_MMLOCAL_LITERAL_CASE_eTk(KwClass, "class");
    __CND_MMLOCAL_LITERAL_CASE_eTk(KwMain, "main");
    __CND_MMLOCAL_LITERAL_CASE_eTk(KwImport, "import");
    __CND_MMLOCAL_LITERAL_CASE_eTk(KwNamespace, "namespace");
    __CND_MMLOCAL_LITERAL_CASE_eTk(KwUse, "using");
    __CND_MMLOCAL_LITERAL_CASE_eTk(KwLib, "lib");
    __CND_MMLOCAL_LITERAL_CASE_eTk(KwDll, "dll");
    __CND_MMLOCAL_LITERAL_CASE_eTk(KwEnum, "enum");

    // Control
    __CND_MMLOCAL_LITERAL_CASE_eTk(KwIf, "if");
    __CND_MMLOCAL_LITERAL_CASE_eTk(KwElif, "elif");
    __CND_MMLOCAL_LITERAL_CASE_eTk(KwElse, "else");
    __CND_MMLOCAL_LITERAL_CASE_eTk(KwCxif, "cxif");
    __CND_MMLOCAL_LITERAL_CASE_eTk(KwCxelif, "cxelif");
    __CND_MMLOCAL_LITERAL_CASE_eTk(KwCxelse, "cxelse");
    __CND_MMLOCAL_LITERAL_CASE_eTk(KwSwitch, "switch");
    __CND_MMLOCAL_LITERAL_CASE_eTk(KwCase, "case");
    __CND_MMLOCAL_LITERAL_CASE_eTk(KwDefault, "default");
    __CND_MMLOCAL_LITERAL_CASE_eTk(KwWhile, "while");
    __CND_MMLOCAL_LITERAL_CASE_eTk(KwFor, "for");
    __CND_MMLOCAL_LITERAL_CASE_eTk(KwReturn, "return");
    __CND_MMLOCAL_LITERAL_CASE_eTk(KwBreak, "break");
    __CND_MMLOCAL_LITERAL_CASE_eTk(KwContinue, "continue");
    // Types
    __CND_MMLOCAL_LITERAL_CASE_eTk(KwInt, "int");
    __CND_MMLOCAL_LITERAL_CASE_eTk(KwUint, "uint");
    __CND_MMLOCAL_LITERAL_CASE_eTk(KwReal, "real");
    __CND_MMLOCAL_LITERAL_CASE_eTk(KwBool, "bool");
    __CND_MMLOCAL_LITERAL_CASE_eTk(KwChar, "char");
    __CND_MMLOCAL_LITERAL_CASE_eTk(KwByte, "byte");
    __CND_MMLOCAL_LITERAL_CASE_eTk(KwCstr, "cstr");
    __CND_MMLOCAL_LITERAL_CASE_eTk(KwStr, "str");
    __CND_MMLOCAL_LITERAL_CASE_eTk(KwPtr, "ptr");
    __CND_MMLOCAL_LITERAL_CASE_eTk(KwList, "list");
    __CND_MMLOCAL_LITERAL_CASE_eTk(KwArray, "array");
    // Constants
    __CND_MMLOCAL_LITERAL_CASE_eTk(KwTrue, "true");
    __CND_MMLOCAL_LITERAL_CASE_eTk(KwFalse, "false");
    __CND_MMLOCAL_LITERAL_CASE_eTk(KwNone, "none");
    __CND_MMLOCAL_LITERAL_CASE_eTk(KwVoid, "void");
    // Functional
    __CND_MMLOCAL_LITERAL_CASE_eTk(KwIn, "in");
    __CND_MMLOCAL_LITERAL_CASE_eTk(KwAs, "as");
    __CND_MMLOCAL_LITERAL_CASE_eTk(KwCin, "cin");
    __CND_MMLOCAL_LITERAL_CASE_eTk(KwCout, "cout");
    __CND_MMLOCAL_LITERAL_CASE_eTk(KwNative, "native");
    // Modifiers
    __CND_MMLOCAL_LITERAL_CASE_eTk(KwConst, "const");
    __CND_MMLOCAL_LITERAL_CASE_eTk(KwRef, "ref");
    __CND_MMLOCAL_LITERAL_CASE_eTk(KwPrivate, "private");
    __CND_MMLOCAL_LITERAL_CASE_eTk(KwPublic, "public");
    __CND_MMLOCAL_LITERAL_CASE_eTk(KwStatic, "static");
    // Meta Types
    __CND_MMLOCAL_LITERAL_CASE_eTk(KwAny, "any");
    __CND_MMLOCAL_LITERAL_CASE_eTk(KwAuto, "auto");
    __CND_MMLOCAL_LITERAL_CASE_eTk(KwType, "type");
    __CND_MMLOCAL_LITERAL_CASE_eTk(KwValue, "value");
    __CND_MMLOCAL_LITERAL_CASE_eTk(KwTemplate, "template");
    __CND_MMLOCAL_LITERAL_CASE_eTk(DirectiveInclude, "#include");
    __CND_MMLOCAL_LITERAL_CASE_eTk(DirectiveDefMacro, "#defmacro");
    __CND_MMLOCAL_LITERAL_CASE_eTk(DirectiveEndmacro, "#endmacro");
    __CND_MMLOCAL_LITERAL_CASE_eTk(DirectiveIf, "#if");
    __CND_MMLOCAL_LITERAL_CASE_eTk(DirectiveElse, "#else");
    __CND_MMLOCAL_LITERAL_CASE_eTk(DirectiveElif, "#elif");
    __CND_MMLOCAL_LITERAL_CASE_eTk(DirectiveEndif, "#endif");
    __CND_MMLOCAL_LITERAL_CASE_eTk(DirectiveIfdef, "#ifdef");
    __CND_MMLOCAL_LITERAL_CASE_eTk(DirectiveIfndef, "#ifndef");
    __CND_MMLOCAL_LITERAL_CASE_eTk(DirectiveUndef, "#undef");
    // Operators
    __CND_MMLOCAL_LITERAL_CASE_eTk(Hash, "#");
    __CND_MMLOCAL_LITERAL_CASE_eTk(Add, "+");
    __CND_MMLOCAL_LITERAL_CASE_eTk(Sub, "-");
    __CND_MMLOCAL_LITERAL_CASE_eTk(Mul, "*");
    __CND_MMLOCAL_LITERAL_CASE_eTk(Div, "/");
    __CND_MMLOCAL_LITERAL_CASE_eTk(Mod, "%");
    __CND_MMLOCAL_LITERAL_CASE_eTk(And, "&");
    __CND_MMLOCAL_LITERAL_CASE_eTk(Or, "|");
    __CND_MMLOCAL_LITERAL_CASE_eTk(Xor, "^");
    __CND_MMLOCAL_LITERAL_CASE_eTk(Not, "!");
    __CND_MMLOCAL_LITERAL_CASE_eTk(Lsh, "<<");
    __CND_MMLOCAL_LITERAL_CASE_eTk(Rsh, ">>");
    __CND_MMLOCAL_LITERAL_CASE_eTk(Eq, "==");
    __CND_MMLOCAL_LITERAL_CASE_eTk(Neq, "!=");
    __CND_MMLOCAL_LITERAL_CASE_eTk(Lt, "<");
    __CND_MMLOCAL_LITERAL_CASE_eTk(Gt, ">");
    __CND_MMLOCAL_LITERAL_CASE_eTk(Lte, "<=");
    __CND_MMLOCAL_LITERAL_CASE_eTk(Gte, ">=");
    __CND_MMLOCAL_LITERAL_CASE_eTk(Assign, "=");
    __CND_MMLOCAL_LITERAL_CASE_eTk(NewAssign, " : =");
    __CND_MMLOCAL_LITERAL_CASE_eTk(AddAssign, "+=");
    __CND_MMLOCAL_LITERAL_CASE_eTk(SubAssign, "-=");
    __CND_MMLOCAL_LITERAL_CASE_eTk(MulAssign, "*=");
    __CND_MMLOCAL_LITERAL_CASE_eTk(DivAssign, "/=");
    __CND_MMLOCAL_LITERAL_CASE_eTk(ModAssign, "%=");
    __CND_MMLOCAL_LITERAL_CASE_eTk(AndAssign, "&=");
    __CND_MMLOCAL_LITERAL_CASE_eTk(OrAssign, "|=");
    __CND_MMLOCAL_LITERAL_CASE_eTk(XorAssign, "^=");
    __CND_MMLOCAL_LITERAL_CASE_eTk(LshAssign, "<<=");
    __CND_MMLOCAL_LITERAL_CASE_eTk(RshAssign, ">>=");
    __CND_MMLOCAL_LITERAL_CASE_eTk(Inc, "++");
    __CND_MMLOCAL_LITERAL_CASE_eTk(Dec, "--");
    __CND_MMLOCAL_LITERAL_CASE_eTk(Dot, ".");
    __CND_MMLOCAL_LITERAL_CASE_eTk(Bnot, "~");
    __CND_MMLOCAL_LITERAL_CASE_eTk(Band, "&&");
    __CND_MMLOCAL_LITERAL_CASE_eTk(Bor, "||");
    __CND_MMLOCAL_LITERAL_CASE_eTk(Spaceship, "<=>");
    // Scopes
    __CND_MMLOCAL_LITERAL_CASE_eTk(LParen, "(");
    __CND_MMLOCAL_LITERAL_CASE_eTk(RParen, ")");
    __CND_MMLOCAL_LITERAL_CASE_eTk(LBrace, "{");
    __CND_MMLOCAL_LITERAL_CASE_eTk(RBrace, "}");
    __CND_MMLOCAL_LITERAL_CASE_eTk(LBracket, "[");
    __CND_MMLOCAL_LITERAL_CASE_eTk(RBracket, "]");
    __CND_MMLOCAL_LITERAL_CASE_eTk(Semicolon, ";");
    __CND_MMLOCAL_LITERAL_CASE_eTk(Colon, ":");
    __CND_MMLOCAL_LITERAL_CASE_eTk(Comma, ",");
    __CND_MMLOCAL_LITERAL_CASE_eTk(Period, ".");
    __CND_MMLOCAL_LITERAL_CASE_eTk(DoubleColon, "::");
    __CND_MMLOCAL_LITERAL_CASE_eTk(Ellipsis, "...");
    __CND_MMLOCAL_LITERAL_CASE_eTk(CommercialAt, "@");
    // Special
    __CND_MMLOCAL_LITERAL_CASE_eTk(Eofile, "\0");
    __CND_MMLOCAL_LITERAL_CASE_eTk(Whitespace, " ");
    __CND_MMLOCAL_LITERAL_CASE_eTk(Newline, "\n");
    __CND_MMLOCAL_LITERAL_CASE_eTk(BlockComment, "///");
    __CND_MMLOCAL_LITERAL_CASE_eTk(LineComment, "//");
    // Literals
    __CND_MMLOCAL_LITERAL_CASE_eTk(LitInt, "");
    __CND_MMLOCAL_LITERAL_CASE_eTk(LitUint, "");
    __CND_MMLOCAL_LITERAL_CASE_eTk(LitReal, "");
    __CND_MMLOCAL_LITERAL_CASE_eTk(LitBool, "");
    __CND_MMLOCAL_LITERAL_CASE_eTk(LitChar, "");
    __CND_MMLOCAL_LITERAL_CASE_eTk(LitByte, "");
    __CND_MMLOCAL_LITERAL_CASE_eTk(LitCstr, "");
    __CND_MMLOCAL_LITERAL_CASE_eTk(Ident, "");  // Identifier
    default:
      return "";
  }

#undef __CND_MMLOCAL_LITERAL_CASE_eTk
}

CND_CX bool TkIsKeyword(eTk t) {
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

CND_CX bool TkIsModifier(eTk t) {
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

CND_CX bool TkIsDeclarative(eTk t) {
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

CND_CX bool TkIsLScope(eTk t) {
  switch (t) {
    case eTk::kLParen:
    case eTk::kLBrace:
    case eTk::kLBracket:
      return true;
    default:
      return false;
  }
}

CND_CX bool TkIsRScope(eTk t) {
  switch (t) {
    case eTk::kRParen:
    case eTk::kRBrace:
    case eTk::kRBracket:
      return true;
    default:
      return false;
  }
}

CND_CX bool TkIsRScopeOf(eTk topen, eTk tclose) {
  using enum eTk;
  return (topen == kLParen && tclose == kRParen) ||
         (topen == kLBrace && tclose == kRBrace) ||
         (topen == kLBracket && tclose == kRBracket);
}

CND_CX bool TkIsAnOperand(eTk t) {
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

CND_CX bool TkIsAPrefixOperator(eTk t) {
  switch (t) {
    case eTk::kNot:
    case eTk::kBnot:
      return true;
    default:
      return false;
  }
}

CND_CX bool TkIsPragmatic(eTk t) {
  return TkIsModifier(t) || TkIsDeclarative(t);
}

CND_CX bool TkIsPrimary(eTk t) {
  return TkIsAnOperand(t) || TkIsAPrefixOperator(t) || t == eTk::kLParen;
}

CND_CX bool AstIsPrimary(eCst t) {
  return AstIsAnOperand(t) || AstIsAPrefixOperator(t) || t == eCst::kLParen;
}

CND_CX bool AstIsPragmatic(eCst t) {
  return AstIsModifier(t) || AstIsDeclarative(t);
}

CND_CX eAssoc AstAssoc(eCst t) {
  using enum eCst;
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

CND_CX eOperation AstOperation(eCst t) {
  using enum eCst;
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

CND_CX ePriority AstPriority(eCst t) {
  switch (t) {
    using enum eCst;
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

CND_CX const char* AstLiteral(eCst t) {
#define CND_LITERAL_AST_ENUM_CASE(n) \
  case eCst::k##n:                   \
    return TkLiteral(eTk::k##n);
  switch (t) {
    CND_APPLIED_ENUM_eTk(CND_LITERAL_AST_ENUM_CASE, , , ) default : return "";
  }
#undef CND_LITERAL_AST_ENUM_CASE
}

CND_CX bool AstIsKeyword(eCst t) {
  switch (t) {
    using enum eCst;
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

CND_CX bool AstIsModifier(eCst t) {
  switch (t) {
    using enum eCst;
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

CND_CX bool AstIsDeclarative(eCst t) {
  // One of: def,fn,class,obj,use,lib,main,import
  switch (t) {
    using enum eCst;
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

CND_CX bool AstIsLScope(eCst t) {
  switch (t) {
    case eCst::kLParen:
    case eCst::kLBrace:
    case eCst::kLBracket:
      return true;
    default:
      return false;
  }
}

CND_CX bool AstIsRScope(eCst t) {
  switch (t) {
    case eCst::kRParen:
    case eCst::kRBrace:
    case eCst::kRBracket:
      return true;
    default:
      return false;
  }
}

CND_CX bool AstIsRScopeOf(eCst topen, eCst tclose) {
  using enum eCst;
  return (topen == kLParen && tclose == kRParen) ||
         (topen == kLBrace && tclose == kRBrace) ||
         (topen == kLBracket && tclose == kRBracket);
}

CND_CX bool AstIsAnOperand(eCst t) {
  switch (t) {
    using enum eCst;
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

CND_CX bool AstIsAPrefixOperator(eCst t) {
  switch (t) {
    case eCst::kNot:
    case eCst::kBnot:
      return true;
    default:
      return false;
  }
}

}  // namespace src_traits
}  // namespace cnd
