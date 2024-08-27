///////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the Apache License, Version 2.0(the "License");
///////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language Environment
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup cand_compiler_data
/// @brief Describes the grammar of the C& programming language.
///        Punctuation, keywords, operators, errors and other syntax elements.
/// 
/// See compiler reference manual for details.
///////////////////////////////////////////////////////////////////////////////

/// @addtogroup cand_compiler_data
/// @{
#ifndef HEADER_GUARD_CALE_CAND_OFFICIAL_COMPILER_CAOCO_GRAMMAR_H
#define HEADER_GUARD_CALE_CAND_OFFICIAL_COMPILER_CAOCO_GRAMMAR_H
// Includes:
#include "cppsextended.h"

///////////////////////////////////////////////////////////////////////////////
// @namespace caoco
///////////////////////////////////////////////////////////////////////////////
namespace caoco {

///////////////////////////////////////////////////////////////////////////
/// Constant building blocks of the C& language syntax. 
///////////////////////////////////////////////////////////////////////////
namespace grammar {

/// @defgroup cand_compiler_data_valid_chars C& Valid Source Symbol Characters
/// @ingroup cand_compiler_data
/// @{

#define DEF_GRAMMAR_CHAR_ELEMENT(n, v) \
  static constexpr inline auto k##n##Char = v;

// Get Grammar Char Element
#define _GGCE(n) k##n##Char

static constexpr inline char kEofChar = '\0';
static constexpr inline char kWhitespaceChar = ' ';
static constexpr inline char kNewlineChar = '\n';
static constexpr inline char kHashChar = '#';
static constexpr inline char kCommercialAtChar = '@';
static constexpr inline char kAddChar = '+';
static constexpr inline char kSubChar = '-';
static constexpr inline char kMulChar = '*';
static constexpr inline char kDivChar = '/';
static constexpr inline char kModChar = '%';
static constexpr inline char kAndChar = '&';
static constexpr inline char kOrChar = '|';
static constexpr inline char kXorChar = '^';
static constexpr inline char kNotChar = '!';
static constexpr inline char kLtChar = '<';
static constexpr inline char kGtChar = '>';
static constexpr inline char kEqChar = '=';
static constexpr inline char kLParenChar = '(';
static constexpr inline char kRParenChar = ')';
static constexpr inline char kLBraceChar = '{';
static constexpr inline char kRBraceChar = '}';
static constexpr inline char kLBracketChar = '[';
static constexpr inline char kRBracketChar = ']';
static constexpr inline char kSemicolonChar = ';';
static constexpr inline char kColonChar = ':';
static constexpr inline char kCommaChar = ',';
static constexpr inline char kPeriodChar = '.';
static constexpr inline char kBacklashChar = '\\';
static constexpr inline char kApostropheChar = '\'';
static constexpr inline char kTildeChar = '~';
static constexpr inline char kDoubleQuoteChar = '"';
static constexpr inline char kBacktickChar = '`';
static constexpr inline char kUnderscoreChar = '_';
static constexpr inline char kSingleQuoteChar = '\'';
;

#define CAOCO_GRAMMAR_CHAR_ELEMENTS                                         \
  _GGCE(Eof), _GGCE(Whitespace), _GGCE(Newline), _GGCE(Hash),               \
      _GGCE(CommercialAt), _GGCE(Add), _GGCE(Sub), _GGCE(Mul), _GGCE(Div),  \
      _GGCE(Mod), _GGCE(And), _GGCE(Or), _GGCE(Xor), _GGCE(Not), _GGCE(Lt), \
      _GGCE(Gt), _GGCE(Eq), _GGCE(LParen), _GGCE(RParen), _GGCE(LBrace),    \
      _GGCE(RBrace), _GGCE(LBracket), _GGCE(RBracket), _GGCE(Semicolon),    \
      _GGCE(Colon), _GGCE(Comma), _GGCE(Period), _GGCE(Backlash),           \
      _GGCE(Apostrophe), _GGCE(Tilde), _GGCE(DoubleQuote), _GGCE(Backtick), \
      _GGCE(Underscore), _GGCE(SingleQuote)

#undef _GGCE
#undef DEF_GRAMMAR_CHAR_ELEMENT
/// @} // end of cand_compiler_data_valid_chars

///////////////////////////////////////////////////////////////////////////
/// @defgroup cand_compiler_data_grammar_elements Grammar Elements
/// @ingroup cand_compiler_data
/// @{
///////////////////////////////////////////////////////////////////////////
#define DEF_GRAMMAR_ELEMENT(n, v)                               \
  using n = mta::string_constant<[]() consteval { return v; }>; \
  static constexpr inline auto k##n = n::data;                  \
  static const size_t k##n##Hash = n::hash;

DEF_GRAMMAR_ELEMENT(NONE, "");
DEF_GRAMMAR_ELEMENT(INVALID, "");
// Declarative
DEF_GRAMMAR_ELEMENT(KwDef, "def");
DEF_GRAMMAR_ELEMENT(KwFn, "fn");
DEF_GRAMMAR_ELEMENT(KwClass, "class");
DEF_GRAMMAR_ELEMENT(KwMain, "main");
DEF_GRAMMAR_ELEMENT(KwImport, "import");
DEF_GRAMMAR_ELEMENT(KwNamespace, "namespace");
DEF_GRAMMAR_ELEMENT(KwUse, "using");
DEF_GRAMMAR_ELEMENT(KwLib, "lib");
DEF_GRAMMAR_ELEMENT(KwDll, "dll");
// Control
DEF_GRAMMAR_ELEMENT(KwIf, "if");
DEF_GRAMMAR_ELEMENT(KwElif, "elif");
DEF_GRAMMAR_ELEMENT(KwElse, "else");
DEF_GRAMMAR_ELEMENT(KwCxif, "cxif");
DEF_GRAMMAR_ELEMENT(KwCxelif, "cxelif");
DEF_GRAMMAR_ELEMENT(KwCxelse, "cxelse");
DEF_GRAMMAR_ELEMENT(KwSwitch, "switch");
DEF_GRAMMAR_ELEMENT(KwCase, "case");
DEF_GRAMMAR_ELEMENT(KwDefault, "default");
DEF_GRAMMAR_ELEMENT(KwWhile, "while");
DEF_GRAMMAR_ELEMENT(KwFor, "for");
DEF_GRAMMAR_ELEMENT(KwReturn, "return");
DEF_GRAMMAR_ELEMENT(KwBreak, "break");
DEF_GRAMMAR_ELEMENT(KwContinue, "continue");
// Types
DEF_GRAMMAR_ELEMENT(KwInt, "int");
DEF_GRAMMAR_ELEMENT(KwUint, "uint");
DEF_GRAMMAR_ELEMENT(KwReal, "real");
DEF_GRAMMAR_ELEMENT(KwBool, "bool");
DEF_GRAMMAR_ELEMENT(KwChar, "char");
DEF_GRAMMAR_ELEMENT(KwByte, "byte");
DEF_GRAMMAR_ELEMENT(KwCstr, "cstr");
DEF_GRAMMAR_ELEMENT(KwStr, "str");
DEF_GRAMMAR_ELEMENT(KwPtr, "ptr");
DEF_GRAMMAR_ELEMENT(KwList, "list");
DEF_GRAMMAR_ELEMENT(KwArray, "array");
// Constants
DEF_GRAMMAR_ELEMENT(KwTrue, "true");
DEF_GRAMMAR_ELEMENT(KwFalse, "false");
DEF_GRAMMAR_ELEMENT(KwNone, "none");
DEF_GRAMMAR_ELEMENT(KwVoid, "void");
// Functional
DEF_GRAMMAR_ELEMENT(KwIn, "in");
DEF_GRAMMAR_ELEMENT(KwAs, "as");
DEF_GRAMMAR_ELEMENT(KwCin, "cin");
DEF_GRAMMAR_ELEMENT(KwCout, "cout");
DEF_GRAMMAR_ELEMENT(KwNative, "native");
// Modifiers
DEF_GRAMMAR_ELEMENT(KwConst, "const");
DEF_GRAMMAR_ELEMENT(KwRef, "ref");
DEF_GRAMMAR_ELEMENT(KwPrivate, "private");
DEF_GRAMMAR_ELEMENT(KwPublic, "public");
DEF_GRAMMAR_ELEMENT(KwStatic, "static");
// Meta Types
DEF_GRAMMAR_ELEMENT(KwAny, "any");
DEF_GRAMMAR_ELEMENT(KwAuto, "auto");
DEF_GRAMMAR_ELEMENT(KwType, "type");
DEF_GRAMMAR_ELEMENT(KwValue, "value");
DEF_GRAMMAR_ELEMENT(KwTemplate, "template");
DEF_GRAMMAR_ELEMENT(DirectiveInclude, "#include");
DEF_GRAMMAR_ELEMENT(DirectiveDefMacro, "#defmacro");
DEF_GRAMMAR_ELEMENT(DirectiveEndmacro, "#endmacro");
DEF_GRAMMAR_ELEMENT(DirectiveIf, "#if");
DEF_GRAMMAR_ELEMENT(DirectiveElse, "#else");
DEF_GRAMMAR_ELEMENT(DirectiveElif, "#elif");
DEF_GRAMMAR_ELEMENT(DirectiveEndif, "#endif");
DEF_GRAMMAR_ELEMENT(DirectiveIfdef, "#ifdef");
DEF_GRAMMAR_ELEMENT(DirectiveIfndef, "#ifndef");
DEF_GRAMMAR_ELEMENT(DirectiveUndef, "#undef");
// Operators
DEF_GRAMMAR_ELEMENT(Hash, "#");
DEF_GRAMMAR_ELEMENT(Add, "+");
DEF_GRAMMAR_ELEMENT(Sub, "-");
DEF_GRAMMAR_ELEMENT(Mul, "*");
DEF_GRAMMAR_ELEMENT(Div, "/");
DEF_GRAMMAR_ELEMENT(Mod, "%");
DEF_GRAMMAR_ELEMENT(And, "&");
DEF_GRAMMAR_ELEMENT(Or, "|");
DEF_GRAMMAR_ELEMENT(Xor, "^");
DEF_GRAMMAR_ELEMENT(Not, "!");
DEF_GRAMMAR_ELEMENT(Lsh, "<<");
DEF_GRAMMAR_ELEMENT(Rsh, ">>");
DEF_GRAMMAR_ELEMENT(Eq, "==");
DEF_GRAMMAR_ELEMENT(Neq, "!=");
DEF_GRAMMAR_ELEMENT(Lt, "<");
DEF_GRAMMAR_ELEMENT(Gt, ">");
DEF_GRAMMAR_ELEMENT(Lte, "<=");
DEF_GRAMMAR_ELEMENT(Gte, ">=");
DEF_GRAMMAR_ELEMENT(Assign, "=");
DEF_GRAMMAR_ELEMENT(NewAssign, " : =");
DEF_GRAMMAR_ELEMENT(AddAssign, "+=");
DEF_GRAMMAR_ELEMENT(SubAssign, "-=");
DEF_GRAMMAR_ELEMENT(MulAssign, "*=");
DEF_GRAMMAR_ELEMENT(DivAssign, "/=");
DEF_GRAMMAR_ELEMENT(ModAssign, "%=");
DEF_GRAMMAR_ELEMENT(AndAssign, "&=");
DEF_GRAMMAR_ELEMENT(OrAssign, "|=");
DEF_GRAMMAR_ELEMENT(XorAssign, "^=");
DEF_GRAMMAR_ELEMENT(LshAssign, "<<=");
DEF_GRAMMAR_ELEMENT(RshAssign, ">>=");
DEF_GRAMMAR_ELEMENT(Inc, "++");
DEF_GRAMMAR_ELEMENT(Dec, "--");
DEF_GRAMMAR_ELEMENT(Dot, ".");
DEF_GRAMMAR_ELEMENT(Bnot, "~");
DEF_GRAMMAR_ELEMENT(Band, "&&");
DEF_GRAMMAR_ELEMENT(Bor, "||");
DEF_GRAMMAR_ELEMENT(Spaceship, "<=>");
// Scopes
DEF_GRAMMAR_ELEMENT(LParen, "(");
DEF_GRAMMAR_ELEMENT(RParen, ")");
DEF_GRAMMAR_ELEMENT(LBrace, "{");
DEF_GRAMMAR_ELEMENT(RBrace, "}");
DEF_GRAMMAR_ELEMENT(LBracket, "[");
DEF_GRAMMAR_ELEMENT(RBracket, "]");
DEF_GRAMMAR_ELEMENT(Semicolon, ";");
DEF_GRAMMAR_ELEMENT(Colon, ":");
DEF_GRAMMAR_ELEMENT(Comma, ",");
DEF_GRAMMAR_ELEMENT(Period, ".");
DEF_GRAMMAR_ELEMENT(DoubleColon, "::");
DEF_GRAMMAR_ELEMENT(Ellipsis, "...");
DEF_GRAMMAR_ELEMENT(CommercialAt, "@");
// Special
DEF_GRAMMAR_ELEMENT(Eofile, "\0");
DEF_GRAMMAR_ELEMENT(Whitespace, " ");
DEF_GRAMMAR_ELEMENT(Newline, "\n");
DEF_GRAMMAR_ELEMENT(BlockComment, "///");
DEF_GRAMMAR_ELEMENT(LineComment, "//");
// Literals
DEF_GRAMMAR_ELEMENT(LitInt, "");
DEF_GRAMMAR_ELEMENT(LitUint, "");
DEF_GRAMMAR_ELEMENT(LitReal, "");
DEF_GRAMMAR_ELEMENT(LitBool, "");
DEF_GRAMMAR_ELEMENT(LitChar, "");
DEF_GRAMMAR_ELEMENT(LitByte, "");
DEF_GRAMMAR_ELEMENT(LitCstr, "");
DEF_GRAMMAR_ELEMENT(Ident, "");  // Identifier

#undef DEF_GRAMMAR_ELEMENT


#define COMPILER_UTIL_GRAMMAR_ELEMENTS NONE, INVALID

#define DIRECTIVE_GRAMMAR_ELEMENTS                                     \
  DirectiveInclude, DirectiveDefMacro, DirectiveEndmacro, DirectiveIf, \
      DirectiveElse, DirectiveElif, DirectiveEndif, DirectiveIfdef,    \
      DirectiveIfndef, DirectiveUndef

#define KEYWORD_GRAMMAR_ELEMENTS                                               \
  KwDef, KwFn, KwClass, KwMain, KwImport, KwNamespace, KwUse, KwLib, KwDll,    \
      KwIf, KwElif, KwElse, KwCxif, KwCxelif, KwCxelse, KwSwitch, KwCase,      \
      KwDefault, KwWhile, KwFor, KwReturn, KwBreak, KwContinue, KwInt, KwUint, \
      KwReal, KwBool, KwChar, KwByte, KwCstr, KwStr, KwPtr, KwList, KwArray,   \
      KwTrue, KwFalse, KwNone, KwVoid, KwIn, KwAs, KwCin, KwCout, KwNative,    \
      KwConst, KwRef, KwPrivate, KwPublic, KwStatic, KwAny, KwAuto, KwType,    \
      KwValue, KwTemplate

#define OPERATOR_GRAMMAR_ELEMENTS                                              \
  Hash, Add, Sub, Mul, Div, Mod, And, Or, Xor, Not, Lsh, Rsh, Eq, Neq, Lt, Gt, \
      Lte, Gte, Spaceship, Assign, NewAssign, AddAssign, SubAssign, MulAssign, \
      DivAssign, ModAssign, AndAssign, OrAssign, XorAssign, LshAssign,         \
      RshAssign, Inc, Dec, Dot, Bnot, Band, Bor, LParen, RParen, LBrace,       \
      RBrace, LBracket, RBracket, Semicolon, Colon, Comma, Period,             \
      DoubleColon, Ellipsis, CommercialAt

#define LITERAL_GRAMMAR_ELEMENTS \
  LitInt, LitUint, LitReal, LitBool, LitChar, LitByte, LitCstr, Ident

#define INVISIBLE_GRAMMAR_ELEMENTS \
  Eofile, Whitespace, Newline, BlockComment, LineComment

#define ALL_GRAMMAR_ELEMENTS                                 \
  COMPILER_UTIL_GRAMMAR_ELEMENTS, KEYWORD_GRAMMAR_ELEMENTS,  \
      DIRECTIVE_GRAMMAR_ELEMENTS, OPERATOR_GRAMMAR_ELEMENTS, \
      LITERAL_GRAMMAR_ELEMENTS, INVISIBLE_GRAMMAR_ELEMENTS

#define INTERMERDIATE_GRAMMAR_ELEMENTS                                         \
  UndefinedObject, Statement, Expression, Declaration, TypeAlias,              \
      LibraryTypeAlias, LibraryObjectInclusion, LibraryNamespaceInclusion,     \
      NamespaceInclusion, NamespaceObjectInclusion, VariableDeclaration,       \
      VariableDefinition, MethodDeclaration, MethodDefinition,                 \
      ClassDefinition, ClassDeclaration, ImportDeclaration,                    \
      LibraryDeclaration, LibraryDefinition, MainDeclaration, MainDefinition,  \
      MethodVoid, MethodSignature, MethodParameter, MethodParameterList,       \
      MethodReturnType, IfStatement, Program, PragmaticBlock, FunctionalBlock, \
      ConditionalBlock, IterativeBlock, TypeConstraints, CaptureList,          \
      Subexpression, TypeList, GenericList, FunctionCall, Arguments, TypeCall, \
      TypeArguments, IndexOperator, ListingOperator, IndexArguments,           \
      Modifiers, UnaryMinus, Dereference, AddressOf

static constexpr inline auto kAllKeywords = array{
    kKwDef,    kKwFn,     kKwClass,  kKwMain,    kKwImport,   kKwNamespace,
    kKwUse,    kKwLib,    kKwDll,    kKwIf,      kKwElif,     kKwElse,
    kKwCxif,   kKwCxelif, kKwCxelse, kKwSwitch,  kKwCase,     kKwDefault,
    kKwWhile,  kKwFor,    kKwReturn, kKwBreak,   kKwContinue, kKwInt,
    kKwUint,   kKwReal,   kKwBool,   kKwChar,    kKwByte,     kKwCstr,
    kKwStr,    kKwPtr,    kKwList,   kKwArray,   kKwTrue,     kKwFalse,
    kKwNone,   kKwVoid,   kKwIn,     kKwAs,      kKwCin,      kKwCout,
    kKwNative, kKwConst,  kKwRef,    kKwPrivate, kKwPublic,   kKwStatic,
    kKwAny,    kKwAuto,   kKwType,   kKwValue,   kKwTemplate};

static constexpr inline auto kAllDirectives = array{
    kDirectiveInclude, kDirectiveDefMacro, kDirectiveEndmacro, kDirectiveIf,
    kDirectiveElse,    kDirectiveElif,     kDirectiveEndif,    kDirectiveIfdef,
    kDirectiveIfndef,  kDirectiveUndef};

static constexpr inline auto kAll = mta::merge_arrays(
    kAllKeywords, kAllDirectives,
    std::array{
        kHash,      kAdd,        kSub,         kMul,          kDiv,
        kMod,       kAnd,        kOr,          kXor,          kNot,
        kLsh,       kRsh,        kEq,          kNeq,          kLt,
        kGt,        kLte,        kGte,         kSpaceship,    kAssign,
        kNewAssign, kAddAssign,  kSubAssign,   kMulAssign,    kDivAssign,
        kModAssign, kAndAssign,  kOrAssign,    kXorAssign,    kLshAssign,
        kRshAssign, kInc,        kDec,         kDot,          kBnot,
        kBand,      kBor,        kLParen,      kRParen,       kLBrace,
        kRBrace,    kLBracket,   kRBracket,    kSemicolon,    kColon,
        kComma,     kPeriod,     kDoubleColon, kEllipsis,     kCommercialAt,
        kEofile,    kWhitespace, kNewline,     kBlockComment, kLineComment,
        kLitInt,    kLitUint,    kLitReal,     kLitBool,      kLitChar,
        kLitByte,   kLitCstr,    kIdent});

///////////////////////////////////////////////////////////////////////////
/// @} // end of cand_compiler_data_grammar_elements
///////////////////////////////////////////////////////////////////////////

}  // namespace grammar

///////////////////////////////////////////////////////////////////////////
/// @defgroup cand_compiler_data_enums Compiler Enums
/// @ingroup cand_compiler_data
/// 
/// Enums used in the lexing and parsing phases of the compiler.
/// @{
///////////////////////////////////////////////////////////////////////////

enum class eTk { ALL_GRAMMAR_ELEMENTS };

enum class eAst { ALL_GRAMMAR_ELEMENTS, INTERMERDIATE_GRAMMAR_ELEMENTS };

enum class eAssoc : int {
  Invalid = -1,
  None = 0,
  Left,
  Right,
};

enum class eOperation : int {
  Invalid = -1,
  None = 0,
  Binary,
  Prefix,
  Postfix,
};

enum class ePriority : int {
  Invalid = -1,
  None = 0,
  Assignment = 20000,
  LogicalOr = 60000,
  LogicalAnd = 70000,
  BitwiseOr = 80000,
  BitwiseXor = 85000,
  BitwiseAnd = 90000,
  Equality = 100000,
  ThreeWayEquality = 110000,
  Comparison = 120000,
  Bitshift = 130000,
  Term = 140000,
  Factor = 150000,
  Prefix = 160000,
  Postfix = 170000,
  Functional = 180000,
  Access = 190000,
  Max = INT_MAX,
};

///////////////////////////////////////////////////////////////////////////
/// @} // end of cand_compiler_data_enums
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
/// @defgroup cand_compiler_data_token_traits Token Traits
/// @ingroup cand_compiler_data
///
/// Various properties of the 'eTk' token type enum.
/// @{
///////////////////////////////////////////////////////////////////////////

constexpr eAst eTkToAstEnum(eTk t);

constexpr eAssoc eTkAssoc(eTk t);

constexpr eOperation eTkOperation(eTk t);

constexpr ePriority eTkPriority(eTk t);

constexpr std::string_view eTkLiteral(eTk t);

constexpr bool eTkIsKeyword(eTk t);

constexpr bool eTkIsLScope(eTk t);

constexpr bool eTkIsRScope(eTk t);

constexpr bool eTkIsRScopeOf(eTk topen, eTk tclose);

constexpr bool eTkIsModifier(eTk t);

constexpr bool eTkIsDeclarative(eTk t);

constexpr bool eTkIsAnOperand(eTk t);

constexpr bool eTkIsAPrefixOperator(eTk t);

constexpr std::string_view eTkEnumStr(eTk t);

constexpr string_view eAstEnumStr(eAst t);

constexpr bool eTkIsPrimary(eTk t) {
  return eTkIsAnOperand(t) || eTkIsAPrefixOperator(t) || t == eTk::LParen;
}

constexpr eAssoc eAstAssoc(eAst t);

constexpr eOperation eAstOperation(eAst t);

constexpr ePriority eAstPriority(eAst t);

constexpr std::string_view eAstLiteral(eAst t);

constexpr bool eAstIsKeyword(eAst t);

constexpr bool eAstIsLScope(eAst t);

constexpr bool eAstIsRScope(eAst t);

constexpr bool eAstIsRScopeOf(eAst topen, eAst tclose);

constexpr bool eAstIsModifier(eAst t);

constexpr bool eAstIsDeclarative(eAst t);

constexpr bool eAstIsAnOperand(eAst t);

constexpr bool eAstIsAPrefixOperator(eAst t);

constexpr bool eAstIsPrimary(eAst t) {
  return eAstIsAnOperand(t) || eAstIsAPrefixOperator(t) || t == eAst::LParen;
}

constexpr bool eTkIsPragmatic(eTk t) {
  return eTkIsModifier(t) || eTkIsDeclarative(t);
}

constexpr bool eAstIsPragmatic(eAst t) {
  return eAstIsModifier(t) || eAstIsDeclarative(t);
}

///////////////////////////////////////////////////////////////////////////
// @section Token Traits Implementation
///////////////////////////////////////////////////////////////////////////
constexpr eAst eTkToAstEnum(eTk t) {
#define ETK_TO_AST(n) \
  case eTk::n:        \
    return eAst::n
  switch (t) {
    ETK_TO_AST(NONE);
    ETK_TO_AST(INVALID);
    ETK_TO_AST(KwDef);
    ETK_TO_AST(KwFn);
    ETK_TO_AST(KwClass);
    ETK_TO_AST(KwMain);
    ETK_TO_AST(KwImport);
    ETK_TO_AST(KwNamespace);
    ETK_TO_AST(KwUse);
    ETK_TO_AST(KwLib);
    ETK_TO_AST(KwDll);
    ETK_TO_AST(KwIf);
    ETK_TO_AST(KwElif);
    ETK_TO_AST(KwElse);
    ETK_TO_AST(KwCxif);
    ETK_TO_AST(KwCxelif);
    ETK_TO_AST(KwCxelse);
    ETK_TO_AST(KwSwitch);
    ETK_TO_AST(KwCase);
    ETK_TO_AST(KwDefault);
    ETK_TO_AST(KwWhile);
    ETK_TO_AST(KwFor);
    ETK_TO_AST(KwReturn);
    ETK_TO_AST(KwBreak);
    ETK_TO_AST(KwContinue);
    ETK_TO_AST(KwInt);
    ETK_TO_AST(KwUint);
    ETK_TO_AST(KwReal);
    ETK_TO_AST(KwBool);
    ETK_TO_AST(KwChar);
    ETK_TO_AST(KwByte);
    ETK_TO_AST(KwCstr);
    ETK_TO_AST(KwStr);
    ETK_TO_AST(KwPtr);
    ETK_TO_AST(KwList);
    ETK_TO_AST(KwArray);
    ETK_TO_AST(KwTrue);
    ETK_TO_AST(KwFalse);
    ETK_TO_AST(KwNone);
    ETK_TO_AST(KwVoid);
    ETK_TO_AST(KwIn);
    ETK_TO_AST(KwAs);
    ETK_TO_AST(KwCin);
    ETK_TO_AST(KwCout);
    ETK_TO_AST(KwNative);
    ETK_TO_AST(KwConst);
    ETK_TO_AST(KwRef);
    ETK_TO_AST(KwPrivate);
    ETK_TO_AST(KwPublic);
    ETK_TO_AST(KwStatic);
    ETK_TO_AST(KwAny);
    ETK_TO_AST(KwAuto);
    ETK_TO_AST(KwType);
    ETK_TO_AST(KwValue);
    ETK_TO_AST(KwTemplate);
    ETK_TO_AST(DirectiveInclude);
    ETK_TO_AST(DirectiveDefMacro);
    ETK_TO_AST(DirectiveEndmacro);
    ETK_TO_AST(DirectiveIf);
    ETK_TO_AST(DirectiveElse);
    ETK_TO_AST(DirectiveElif);
    ETK_TO_AST(DirectiveEndif);
    ETK_TO_AST(DirectiveIfdef);
    ETK_TO_AST(DirectiveIfndef);
    ETK_TO_AST(DirectiveUndef);
    ETK_TO_AST(Hash);
    ETK_TO_AST(Add);
    ETK_TO_AST(Sub);
    ETK_TO_AST(Mul);
    ETK_TO_AST(Div);
    ETK_TO_AST(Mod);
    ETK_TO_AST(And);
    ETK_TO_AST(Or);
    ETK_TO_AST(Xor);
    ETK_TO_AST(Not);
    ETK_TO_AST(Lsh);
    ETK_TO_AST(Rsh);
    ETK_TO_AST(Eq);
    ETK_TO_AST(Neq);
    ETK_TO_AST(Lt);
    ETK_TO_AST(Gt);
    ETK_TO_AST(Lte);
    ETK_TO_AST(Gte);
    ETK_TO_AST(Assign);
    ETK_TO_AST(NewAssign);
    ETK_TO_AST(AddAssign);
    ETK_TO_AST(SubAssign);
    ETK_TO_AST(MulAssign);
    ETK_TO_AST(DivAssign);
    ETK_TO_AST(ModAssign);
    ETK_TO_AST(AndAssign);
    ETK_TO_AST(OrAssign);
    ETK_TO_AST(XorAssign);
    ETK_TO_AST(LshAssign);
    ETK_TO_AST(RshAssign);
    ETK_TO_AST(Inc);
    ETK_TO_AST(Dec);
    ETK_TO_AST(Dot);
    ETK_TO_AST(Bnot);
    ETK_TO_AST(Band);
    ETK_TO_AST(Bor);
    ETK_TO_AST(LParen);
    ETK_TO_AST(RParen);
    ETK_TO_AST(LBrace);
    ETK_TO_AST(RBrace);
    ETK_TO_AST(LBracket);
    ETK_TO_AST(RBracket);
    ETK_TO_AST(Semicolon);
    ETK_TO_AST(Colon);
    ETK_TO_AST(Comma);
    ETK_TO_AST(Period);
    ETK_TO_AST(DoubleColon);
    ETK_TO_AST(Ellipsis);
    ETK_TO_AST(CommercialAt);
    ETK_TO_AST(Eofile);
    ETK_TO_AST(Whitespace);
    ETK_TO_AST(Newline);
    ETK_TO_AST(BlockComment);
    ETK_TO_AST(LineComment);
    ETK_TO_AST(LitInt);
    ETK_TO_AST(LitUint);
    ETK_TO_AST(LitReal);
    ETK_TO_AST(LitBool);
    ETK_TO_AST(LitChar);
    ETK_TO_AST(LitByte);
    ETK_TO_AST(LitCstr);
    ETK_TO_AST(Ident);
    default:
      return eAst::INVALID;
  }

#undef ETK_TO_AST
}

constexpr eAssoc eTkAssoc(eTk t) {
  switch (t) {
    case eTk::Assign:
    case eTk::AddAssign:
    case eTk::SubAssign:
    case eTk::MulAssign:
    case eTk::DivAssign:
    case eTk::ModAssign:
    case eTk::AndAssign:
    case eTk::OrAssign:
    case eTk::XorAssign:
    case eTk::LshAssign:
    case eTk::RshAssign:
    case eTk::Not:
    case eTk::Bnot:
    case eTk::Ellipsis:
    case eTk::CommercialAt:
      return eAssoc::Right;
    case eTk::Add:
    case eTk::Sub:
    case eTk::Mul:
    case eTk::Div:
    case eTk::Mod:
    case eTk::Band:
    case eTk::Bor:
    case eTk::Xor:
    case eTk::Lsh:
    case eTk::Rsh:
    case eTk::And:
    case eTk::Or:
    case eTk::Eq:
    case eTk::Neq:
    case eTk::Lt:
    case eTk::Gt:
    case eTk::Lte:
    case eTk::Gte:
    case eTk::Inc:
    case eTk::Dec:
    case eTk::DoubleColon:
    case eTk::Period:
      return eAssoc::Left;
    default:
      return eAssoc::None;
  }
}

constexpr eOperation eTkOperation(eTk t) {
  switch (t) {
    case eTk::Assign:
    case eTk::AddAssign:
    case eTk::SubAssign:
    case eTk::MulAssign:
    case eTk::DivAssign:
    case eTk::ModAssign:
    case eTk::AndAssign:
    case eTk::OrAssign:
    case eTk::XorAssign:
    case eTk::LshAssign:
    case eTk::RshAssign:
      return eOperation::Binary;
      break;
    case eTk::Inc:
    case eTk::Dec:
      return eOperation::Postfix;
      break;
    case eTk::Add:
    case eTk::Sub:
    case eTk::Mul:
    case eTk::Div:
    case eTk::Mod:
    case eTk::Band:
    case eTk::Bor:
    case eTk::Xor:
    case eTk::Lsh:
    case eTk::Rsh:
      return eOperation::Binary;
      break;
    case eTk::Not:
    case eTk::Bnot:
      return eOperation::Prefix;
      break;
    case eTk::And:
    case eTk::Or:
    case eTk::Eq:
    case eTk::Neq:
    case eTk::Lt:
    case eTk::Gt:
    case eTk::Lte:
    case eTk::Gte:
      return eOperation::Binary;
    case eTk::LParen:
    case eTk::RParen:
    case eTk::LBrace:
    case eTk::RBrace:
    case eTk::LBracket:
    case eTk::RBracket:
      return eOperation::Postfix;
    case eTk::Period:
    case eTk::DoubleColon:
      return eOperation::Binary;
    case eTk::Ellipsis:
      return eOperation::Binary;
    case eTk::CommercialAt:
      return eOperation::Prefix;
    default:
      return eOperation::None;
  }
}

constexpr ePriority eTkPriority(eTk t) {
  switch (t) {
    using enum eTk;
    case NONE:
    case INVALID:
    case Eofile:
    case LineComment:
    case BlockComment:
    case Newline:
    case Whitespace:
      return ePriority::None;
      break;
    case LitCstr:
    case LitInt:
    case LitUint:
    case LitReal:
    case LitBool:
    case LitChar:
    case LitByte:
    case Ident:
      return ePriority::Max;
      break;
    case Assign:
    case AddAssign:
    case SubAssign:
    case MulAssign:
    case DivAssign:
    case ModAssign:
    case AndAssign:
    case OrAssign:
    case XorAssign:
    case LshAssign:
    case RshAssign:
      return ePriority::Assignment;
      break;
    case Inc:
    case Dec:
      return ePriority::Postfix;
      break;
    case Add:
    case Sub:
      return ePriority::Term;
      break;
    case Mul:
    case Div:
    case Mod:
      return ePriority::Factor;
      break;
    case Not:
    case Bnot:
      return ePriority::Prefix;
      break;
    case Band:
    case Bor:
    case Xor:
    case Lsh:
    case Rsh:
    case And:
    case Or:
    case Eq:
    case Neq:
    case Lt:
    case Gt:
    case Lte:
    case Gte:
      return ePriority::Comparison;
      break;
    case LParen:
    case RParen:
    case LBrace:
    case RBrace:
    case LBracket:
    case RBracket:
      return ePriority::Postfix;
      break;
    case Semicolon:
    case Colon:
    case Comma:
    case Ellipsis:
    case CommercialAt:
      return ePriority::Max;
      break;
    case Period:
    case DoubleColon:
      return ePriority::Access;
      break;
    case KwDef:
    case KwFn:
    case KwClass:
    case KwMain:
    case KwImport:
    case KwNamespace:
    case KwUse:
    case KwLib:
    case KwDll:
    case KwIf:
    case KwElif:
    case KwElse:
    case KwCxif:
    case KwCxelif:
    case KwCxelse:
    case KwSwitch:
    case KwCase:
    case KwDefault:
    case KwWhile:
    case KwFor:
    case KwReturn:
    case KwBreak:
    case KwContinue:
    case KwInt:
    case KwUint:
    case KwReal:
    case KwBool:
    case KwChar:
    case KwByte:
    case KwCstr:
    case KwStr:
    case KwPtr:
    case KwList:
    case KwArray:
    case KwTrue:
    case KwFalse:
    case KwNone:
    case KwVoid:
    case KwIn:
    case KwAs:
    case KwCin:
    case KwCout:
    case KwNative:
    case KwConst:
    case KwRef:
    case KwPrivate:
    case KwPublic:
    case KwStatic:
    case KwAny:
    case KwAuto:
    case KwType:
    case KwValue:
    case KwTemplate:
      return ePriority::Max;
      break;
    default:
      return ePriority::None;
  }
}

constexpr std::string_view eTkLiteral(eTk t) {
#define ETK_LITERAL_CASE(n) \
  case eTk::n:              \
    return grammar::k##n

  switch (t) {
    ETK_LITERAL_CASE(NONE);
    ETK_LITERAL_CASE(INVALID);
    ETK_LITERAL_CASE(KwDef);
    ETK_LITERAL_CASE(KwFn);
    ETK_LITERAL_CASE(KwClass);
    ETK_LITERAL_CASE(KwMain);
    ETK_LITERAL_CASE(KwImport);
    ETK_LITERAL_CASE(KwNamespace);
    ETK_LITERAL_CASE(KwUse);
    ETK_LITERAL_CASE(KwLib);
    ETK_LITERAL_CASE(KwDll);
    ETK_LITERAL_CASE(KwIf);
    ETK_LITERAL_CASE(KwElif);
    ETK_LITERAL_CASE(KwElse);
    ETK_LITERAL_CASE(KwCxif);
    ETK_LITERAL_CASE(KwCxelif);
    ETK_LITERAL_CASE(KwCxelse);
    ETK_LITERAL_CASE(KwSwitch);
    ETK_LITERAL_CASE(KwCase);
    ETK_LITERAL_CASE(KwDefault);
    ETK_LITERAL_CASE(KwWhile);
    ETK_LITERAL_CASE(KwFor);
    ETK_LITERAL_CASE(KwReturn);
    ETK_LITERAL_CASE(KwBreak);
    ETK_LITERAL_CASE(KwContinue);
    ETK_LITERAL_CASE(KwInt);
    ETK_LITERAL_CASE(KwUint);
    ETK_LITERAL_CASE(KwReal);
    ETK_LITERAL_CASE(KwBool);
    ETK_LITERAL_CASE(KwChar);
    ETK_LITERAL_CASE(KwByte);
    ETK_LITERAL_CASE(KwCstr);
    ETK_LITERAL_CASE(KwStr);
    ETK_LITERAL_CASE(KwPtr);
    ETK_LITERAL_CASE(KwList);
    ETK_LITERAL_CASE(KwArray);
    ETK_LITERAL_CASE(KwTrue);
    ETK_LITERAL_CASE(KwFalse);
    ETK_LITERAL_CASE(KwNone);
    ETK_LITERAL_CASE(KwVoid);
    ETK_LITERAL_CASE(KwIn);
    ETK_LITERAL_CASE(KwAs);
    ETK_LITERAL_CASE(KwCin);
    ETK_LITERAL_CASE(KwCout);
    ETK_LITERAL_CASE(KwNative);
    ETK_LITERAL_CASE(KwConst);
    ETK_LITERAL_CASE(KwRef);
    ETK_LITERAL_CASE(KwPrivate);
    ETK_LITERAL_CASE(KwPublic);
    ETK_LITERAL_CASE(KwStatic);
    ETK_LITERAL_CASE(KwAny);
    ETK_LITERAL_CASE(KwAuto);
    ETK_LITERAL_CASE(KwType);
    ETK_LITERAL_CASE(KwValue);
    ETK_LITERAL_CASE(KwTemplate);
    ETK_LITERAL_CASE(DirectiveInclude);
    ETK_LITERAL_CASE(DirectiveDefMacro);
    ETK_LITERAL_CASE(DirectiveEndmacro);
    ETK_LITERAL_CASE(DirectiveIf);
    ETK_LITERAL_CASE(DirectiveElse);
    ETK_LITERAL_CASE(DirectiveElif);
    ETK_LITERAL_CASE(DirectiveEndif);
    ETK_LITERAL_CASE(DirectiveIfdef);
    ETK_LITERAL_CASE(DirectiveIfndef);
    ETK_LITERAL_CASE(DirectiveUndef);
    ETK_LITERAL_CASE(Hash);
    ETK_LITERAL_CASE(Add);
    ETK_LITERAL_CASE(Sub);
    ETK_LITERAL_CASE(Mul);
    ETK_LITERAL_CASE(Div);
    ETK_LITERAL_CASE(Mod);
    ETK_LITERAL_CASE(And);
    ETK_LITERAL_CASE(Or);
    ETK_LITERAL_CASE(Xor);
    ETK_LITERAL_CASE(Not);
    ETK_LITERAL_CASE(Lsh);
    ETK_LITERAL_CASE(Rsh);
    ETK_LITERAL_CASE(Eq);
    ETK_LITERAL_CASE(Neq);
    ETK_LITERAL_CASE(Lt);
    ETK_LITERAL_CASE(Gt);
    ETK_LITERAL_CASE(Lte);
    ETK_LITERAL_CASE(Gte);
    ETK_LITERAL_CASE(Assign);
    ETK_LITERAL_CASE(NewAssign);
    ETK_LITERAL_CASE(AddAssign);
    ETK_LITERAL_CASE(SubAssign);
    ETK_LITERAL_CASE(MulAssign);
    ETK_LITERAL_CASE(DivAssign);
    ETK_LITERAL_CASE(ModAssign);
    ETK_LITERAL_CASE(AndAssign);
    ETK_LITERAL_CASE(OrAssign);
    ETK_LITERAL_CASE(XorAssign);
    ETK_LITERAL_CASE(LshAssign);
    ETK_LITERAL_CASE(RshAssign);
    ETK_LITERAL_CASE(Inc);
    ETK_LITERAL_CASE(Dec);
    ETK_LITERAL_CASE(Dot);
    ETK_LITERAL_CASE(Bnot);
    ETK_LITERAL_CASE(Band);
    ETK_LITERAL_CASE(Bor);
    ETK_LITERAL_CASE(LParen);
    ETK_LITERAL_CASE(RParen);
    ETK_LITERAL_CASE(LBrace);
    ETK_LITERAL_CASE(RBrace);
    ETK_LITERAL_CASE(LBracket);
    ETK_LITERAL_CASE(RBracket);
    ETK_LITERAL_CASE(Semicolon);
    ETK_LITERAL_CASE(Colon);
    ETK_LITERAL_CASE(Comma);
    ETK_LITERAL_CASE(Period);
    ETK_LITERAL_CASE(DoubleColon);
    ETK_LITERAL_CASE(Ellipsis);
    ETK_LITERAL_CASE(CommercialAt);
    ETK_LITERAL_CASE(Eofile);
    ETK_LITERAL_CASE(Whitespace);
    ETK_LITERAL_CASE(Newline);
    ETK_LITERAL_CASE(BlockComment);
    ETK_LITERAL_CASE(LineComment);
    ETK_LITERAL_CASE(LitInt);
    ETK_LITERAL_CASE(LitUint);
    ETK_LITERAL_CASE(LitReal);
    ETK_LITERAL_CASE(LitBool);
    ETK_LITERAL_CASE(LitChar);
    ETK_LITERAL_CASE(LitByte);
    ETK_LITERAL_CASE(LitCstr);
    ETK_LITERAL_CASE(Ident);
  }

#undef ETK_LITERAL_CASE
}

constexpr std::string_view eTkEnumStr(eTk t) {
#define ETK_LITERAL_CASE(n) \
  case eTk::n:              \
    return #n

  switch (t) {
    ETK_LITERAL_CASE(NONE);
    ETK_LITERAL_CASE(INVALID);
    ETK_LITERAL_CASE(KwDef);
    ETK_LITERAL_CASE(KwFn);
    ETK_LITERAL_CASE(KwClass);
    ETK_LITERAL_CASE(KwMain);
    ETK_LITERAL_CASE(KwImport);
    ETK_LITERAL_CASE(KwNamespace);
    ETK_LITERAL_CASE(KwUse);
    ETK_LITERAL_CASE(KwLib);
    ETK_LITERAL_CASE(KwDll);
    ETK_LITERAL_CASE(KwIf);
    ETK_LITERAL_CASE(KwElif);
    ETK_LITERAL_CASE(KwElse);
    ETK_LITERAL_CASE(KwCxif);
    ETK_LITERAL_CASE(KwCxelif);
    ETK_LITERAL_CASE(KwCxelse);
    ETK_LITERAL_CASE(KwSwitch);
    ETK_LITERAL_CASE(KwCase);
    ETK_LITERAL_CASE(KwDefault);
    ETK_LITERAL_CASE(KwWhile);
    ETK_LITERAL_CASE(KwFor);
    ETK_LITERAL_CASE(KwReturn);
    ETK_LITERAL_CASE(KwBreak);
    ETK_LITERAL_CASE(KwContinue);
    ETK_LITERAL_CASE(KwInt);
    ETK_LITERAL_CASE(KwUint);
    ETK_LITERAL_CASE(KwReal);
    ETK_LITERAL_CASE(KwBool);
    ETK_LITERAL_CASE(KwChar);
    ETK_LITERAL_CASE(KwByte);
    ETK_LITERAL_CASE(KwCstr);
    ETK_LITERAL_CASE(KwStr);
    ETK_LITERAL_CASE(KwPtr);
    ETK_LITERAL_CASE(KwList);
    ETK_LITERAL_CASE(KwArray);
    ETK_LITERAL_CASE(KwTrue);
    ETK_LITERAL_CASE(KwFalse);
    ETK_LITERAL_CASE(KwNone);
    ETK_LITERAL_CASE(KwVoid);
    ETK_LITERAL_CASE(KwIn);
    ETK_LITERAL_CASE(KwAs);
    ETK_LITERAL_CASE(KwCin);
    ETK_LITERAL_CASE(KwCout);
    ETK_LITERAL_CASE(KwNative);
    ETK_LITERAL_CASE(KwConst);
    ETK_LITERAL_CASE(KwRef);
    ETK_LITERAL_CASE(KwPrivate);
    ETK_LITERAL_CASE(KwPublic);
    ETK_LITERAL_CASE(KwStatic);
    ETK_LITERAL_CASE(KwAny);
    ETK_LITERAL_CASE(KwAuto);
    ETK_LITERAL_CASE(KwType);
    ETK_LITERAL_CASE(KwValue);
    ETK_LITERAL_CASE(KwTemplate);
    ETK_LITERAL_CASE(DirectiveInclude);
    ETK_LITERAL_CASE(DirectiveDefMacro);
    ETK_LITERAL_CASE(DirectiveEndmacro);
    ETK_LITERAL_CASE(DirectiveIf);
    ETK_LITERAL_CASE(DirectiveElse);
    ETK_LITERAL_CASE(DirectiveElif);
    ETK_LITERAL_CASE(DirectiveEndif);
    ETK_LITERAL_CASE(DirectiveIfdef);
    ETK_LITERAL_CASE(DirectiveIfndef);
    ETK_LITERAL_CASE(DirectiveUndef);
    ETK_LITERAL_CASE(Hash);
    ETK_LITERAL_CASE(Add);
    ETK_LITERAL_CASE(Sub);
    ETK_LITERAL_CASE(Mul);
    ETK_LITERAL_CASE(Div);
    ETK_LITERAL_CASE(Mod);
    ETK_LITERAL_CASE(And);
    ETK_LITERAL_CASE(Or);
    ETK_LITERAL_CASE(Xor);
    ETK_LITERAL_CASE(Not);
    ETK_LITERAL_CASE(Lsh);
    ETK_LITERAL_CASE(Rsh);
    ETK_LITERAL_CASE(Eq);
    ETK_LITERAL_CASE(Neq);
    ETK_LITERAL_CASE(Lt);
    ETK_LITERAL_CASE(Gt);
    ETK_LITERAL_CASE(Lte);
    ETK_LITERAL_CASE(Gte);
    ETK_LITERAL_CASE(Assign);
    ETK_LITERAL_CASE(NewAssign);
    ETK_LITERAL_CASE(AddAssign);
    ETK_LITERAL_CASE(SubAssign);
    ETK_LITERAL_CASE(MulAssign);
    ETK_LITERAL_CASE(DivAssign);
    ETK_LITERAL_CASE(ModAssign);
    ETK_LITERAL_CASE(AndAssign);
    ETK_LITERAL_CASE(OrAssign);
    ETK_LITERAL_CASE(XorAssign);
    ETK_LITERAL_CASE(LshAssign);
    ETK_LITERAL_CASE(RshAssign);
    ETK_LITERAL_CASE(Inc);
    ETK_LITERAL_CASE(Dec);
    ETK_LITERAL_CASE(Dot);
    ETK_LITERAL_CASE(Bnot);
    ETK_LITERAL_CASE(Band);
    ETK_LITERAL_CASE(Bor);
    ETK_LITERAL_CASE(LParen);
    ETK_LITERAL_CASE(RParen);
    ETK_LITERAL_CASE(LBrace);
    ETK_LITERAL_CASE(RBrace);
    ETK_LITERAL_CASE(LBracket);
    ETK_LITERAL_CASE(RBracket);
    ETK_LITERAL_CASE(Semicolon);
    ETK_LITERAL_CASE(Colon);
    ETK_LITERAL_CASE(Comma);
    ETK_LITERAL_CASE(Period);
    ETK_LITERAL_CASE(DoubleColon);
    ETK_LITERAL_CASE(Ellipsis);
    ETK_LITERAL_CASE(CommercialAt);
    ETK_LITERAL_CASE(Eofile);
    ETK_LITERAL_CASE(Whitespace);
    ETK_LITERAL_CASE(Newline);
    ETK_LITERAL_CASE(BlockComment);
    ETK_LITERAL_CASE(LineComment);
    ETK_LITERAL_CASE(LitInt);
    ETK_LITERAL_CASE(LitUint);
    ETK_LITERAL_CASE(LitReal);
    ETK_LITERAL_CASE(LitBool);
    ETK_LITERAL_CASE(LitChar);
    ETK_LITERAL_CASE(LitByte);
    ETK_LITERAL_CASE(LitCstr);
    ETK_LITERAL_CASE(Ident);
    default:
      return "INVALID";
  }

#undef ETK_LITERAL_CASE
}

constexpr string_view eAstEnumStr(eAst t) {
#define ETK_LITERAL_CASE(n) \
  case eAst::n:             \
    return #n

  switch (t) {
    ETK_LITERAL_CASE(NONE);
    ETK_LITERAL_CASE(INVALID);
    ETK_LITERAL_CASE(KwDef);
    ETK_LITERAL_CASE(KwFn);
    ETK_LITERAL_CASE(KwClass);
    ETK_LITERAL_CASE(KwMain);
    ETK_LITERAL_CASE(KwImport);
    ETK_LITERAL_CASE(KwNamespace);
    ETK_LITERAL_CASE(KwUse);
    ETK_LITERAL_CASE(KwLib);
    ETK_LITERAL_CASE(KwDll);
    ETK_LITERAL_CASE(KwIf);
    ETK_LITERAL_CASE(KwElif);
    ETK_LITERAL_CASE(KwElse);
    ETK_LITERAL_CASE(KwCxif);
    ETK_LITERAL_CASE(KwCxelif);
    ETK_LITERAL_CASE(KwCxelse);
    ETK_LITERAL_CASE(KwSwitch);
    ETK_LITERAL_CASE(KwCase);
    ETK_LITERAL_CASE(KwDefault);
    ETK_LITERAL_CASE(KwWhile);
    ETK_LITERAL_CASE(KwFor);
    ETK_LITERAL_CASE(KwReturn);
    ETK_LITERAL_CASE(KwBreak);
    ETK_LITERAL_CASE(KwContinue);
    ETK_LITERAL_CASE(KwInt);
    ETK_LITERAL_CASE(KwUint);
    ETK_LITERAL_CASE(KwReal);
    ETK_LITERAL_CASE(KwBool);
    ETK_LITERAL_CASE(KwChar);
    ETK_LITERAL_CASE(KwByte);
    ETK_LITERAL_CASE(KwCstr);
    ETK_LITERAL_CASE(KwStr);
    ETK_LITERAL_CASE(KwPtr);
    ETK_LITERAL_CASE(KwList);
    ETK_LITERAL_CASE(KwArray);
    ETK_LITERAL_CASE(KwTrue);
    ETK_LITERAL_CASE(KwFalse);
    ETK_LITERAL_CASE(KwNone);
    ETK_LITERAL_CASE(KwVoid);
    ETK_LITERAL_CASE(KwIn);
    ETK_LITERAL_CASE(KwAs);
    ETK_LITERAL_CASE(KwCin);
    ETK_LITERAL_CASE(KwCout);
    ETK_LITERAL_CASE(KwNative);
    ETK_LITERAL_CASE(KwConst);
    ETK_LITERAL_CASE(KwRef);
    ETK_LITERAL_CASE(KwPrivate);
    ETK_LITERAL_CASE(KwPublic);
    ETK_LITERAL_CASE(KwStatic);
    ETK_LITERAL_CASE(KwAny);
    ETK_LITERAL_CASE(KwAuto);
    ETK_LITERAL_CASE(KwType);
    ETK_LITERAL_CASE(KwValue);
    ETK_LITERAL_CASE(KwTemplate);
    ETK_LITERAL_CASE(DirectiveInclude);
    ETK_LITERAL_CASE(DirectiveDefMacro);
    ETK_LITERAL_CASE(DirectiveEndmacro);
    ETK_LITERAL_CASE(DirectiveIf);
    ETK_LITERAL_CASE(DirectiveElse);
    ETK_LITERAL_CASE(DirectiveElif);
    ETK_LITERAL_CASE(DirectiveEndif);
    ETK_LITERAL_CASE(DirectiveIfdef);
    ETK_LITERAL_CASE(DirectiveIfndef);
    ETK_LITERAL_CASE(DirectiveUndef);
    ETK_LITERAL_CASE(Hash);
    ETK_LITERAL_CASE(Add);
    ETK_LITERAL_CASE(Sub);
    ETK_LITERAL_CASE(Mul);
    ETK_LITERAL_CASE(Div);
    ETK_LITERAL_CASE(Mod);
    ETK_LITERAL_CASE(And);
    ETK_LITERAL_CASE(Or);
    ETK_LITERAL_CASE(Xor);
    ETK_LITERAL_CASE(Not);
    ETK_LITERAL_CASE(Lsh);
    ETK_LITERAL_CASE(Rsh);
    ETK_LITERAL_CASE(Eq);
    ETK_LITERAL_CASE(Neq);
    ETK_LITERAL_CASE(Lt);
    ETK_LITERAL_CASE(Gt);
    ETK_LITERAL_CASE(Lte);
    ETK_LITERAL_CASE(Gte);
    ETK_LITERAL_CASE(Assign);
    ETK_LITERAL_CASE(NewAssign);
    ETK_LITERAL_CASE(AddAssign);
    ETK_LITERAL_CASE(SubAssign);
    ETK_LITERAL_CASE(MulAssign);
    ETK_LITERAL_CASE(DivAssign);
    ETK_LITERAL_CASE(ModAssign);
    ETK_LITERAL_CASE(AndAssign);
    ETK_LITERAL_CASE(OrAssign);
    ETK_LITERAL_CASE(XorAssign);
    ETK_LITERAL_CASE(LshAssign);
    ETK_LITERAL_CASE(RshAssign);
    ETK_LITERAL_CASE(Inc);
    ETK_LITERAL_CASE(Dec);
    ETK_LITERAL_CASE(Dot);
    ETK_LITERAL_CASE(Bnot);
    ETK_LITERAL_CASE(Band);
    ETK_LITERAL_CASE(Bor);
    ETK_LITERAL_CASE(LParen);
    ETK_LITERAL_CASE(RParen);
    ETK_LITERAL_CASE(LBrace);
    ETK_LITERAL_CASE(RBrace);
    ETK_LITERAL_CASE(LBracket);
    ETK_LITERAL_CASE(RBracket);
    ETK_LITERAL_CASE(Semicolon);
    ETK_LITERAL_CASE(Colon);
    ETK_LITERAL_CASE(Comma);
    ETK_LITERAL_CASE(Period);
    ETK_LITERAL_CASE(DoubleColon);
    ETK_LITERAL_CASE(Ellipsis);
    ETK_LITERAL_CASE(CommercialAt);
    ETK_LITERAL_CASE(Eofile);
    ETK_LITERAL_CASE(Whitespace);
    ETK_LITERAL_CASE(Newline);
    ETK_LITERAL_CASE(BlockComment);
    ETK_LITERAL_CASE(LineComment);
    ETK_LITERAL_CASE(LitInt);
    ETK_LITERAL_CASE(LitUint);
    ETK_LITERAL_CASE(LitReal);
    ETK_LITERAL_CASE(LitBool);
    ETK_LITERAL_CASE(LitChar);
    ETK_LITERAL_CASE(LitByte);
    ETK_LITERAL_CASE(LitCstr);
    ETK_LITERAL_CASE(Ident);
    ETK_LITERAL_CASE(UndefinedObject);
    ETK_LITERAL_CASE(Statement);
    ETK_LITERAL_CASE(Expression);
    ETK_LITERAL_CASE(Declaration);
    ETK_LITERAL_CASE(TypeAlias);
    ETK_LITERAL_CASE(LibraryTypeAlias);
    ETK_LITERAL_CASE(LibraryObjectInclusion);
    ETK_LITERAL_CASE(LibraryNamespaceInclusion);
    ETK_LITERAL_CASE(NamespaceInclusion);
    ETK_LITERAL_CASE(NamespaceObjectInclusion);
    ETK_LITERAL_CASE(VariableDeclaration);
    ETK_LITERAL_CASE(VariableDefinition);
    ETK_LITERAL_CASE(MethodDeclaration);
    ETK_LITERAL_CASE(MethodDefinition);
    ETK_LITERAL_CASE(ClassDefinition);
    ETK_LITERAL_CASE(ClassDeclaration);
    ETK_LITERAL_CASE(ImportDeclaration);
    ETK_LITERAL_CASE(LibraryDeclaration);
    ETK_LITERAL_CASE(LibraryDefinition);
    ETK_LITERAL_CASE(MainDeclaration);
    ETK_LITERAL_CASE(MainDefinition);
    ETK_LITERAL_CASE(MethodVoid);
    ETK_LITERAL_CASE(MethodSignature);
    ETK_LITERAL_CASE(MethodParameter);
    ETK_LITERAL_CASE(MethodParameterList);
    ETK_LITERAL_CASE(MethodReturnType);
    ETK_LITERAL_CASE(IfStatement);
    ETK_LITERAL_CASE(Program);
    ETK_LITERAL_CASE(PragmaticBlock);
    ETK_LITERAL_CASE(FunctionalBlock);
    ETK_LITERAL_CASE(ConditionalBlock);
    ETK_LITERAL_CASE(IterativeBlock);
    ETK_LITERAL_CASE(TypeConstraints);
    ETK_LITERAL_CASE(CaptureList);
    ETK_LITERAL_CASE(Subexpression);
    ETK_LITERAL_CASE(TypeList);
    ETK_LITERAL_CASE(GenericList);
    ETK_LITERAL_CASE(FunctionCall);
    ETK_LITERAL_CASE(Arguments);
    ETK_LITERAL_CASE(TypeCall);
    ETK_LITERAL_CASE(TypeArguments);
    ETK_LITERAL_CASE(IndexOperator);
    ETK_LITERAL_CASE(ListingOperator);
    ETK_LITERAL_CASE(IndexArguments);
    ETK_LITERAL_CASE(Modifiers);
    ETK_LITERAL_CASE(UnaryMinus);
    ETK_LITERAL_CASE(Dereference);
    ETK_LITERAL_CASE(AddressOf);
    default:
      return "INVALID";
  }
}

constexpr bool eTkIsKeyword(eTk t) {
  switch (t) {
    using enum eTk;
    case KwDef:
    case KwFn:
    case KwClass:
    case KwMain:
    case KwImport:
    case KwNamespace:
    case KwUse:
    case KwLib:
    case KwDll:
    case KwIf:
    case KwElif:
    case KwElse:
    case KwCxif:
    case KwCxelif:
    case KwCxelse:
    case KwSwitch:
    case KwCase:
    case KwDefault:
    case KwWhile:
    case KwFor:
    case KwReturn:
    case KwBreak:
    case KwContinue:
    case KwInt:
    case KwUint:
    case KwReal:
    case KwBool:
    case KwChar:
    case KwByte:
    case KwCstr:
    case KwStr:
    case KwPtr:
    case KwList:
    case KwArray:
    case KwTrue:
    case KwFalse:
    case KwNone:
    case KwVoid:
    case KwIn:
    case KwAs:
    case KwCin:
    case KwCout:
    case KwNative:
    case KwConst:
    case KwRef:
    case KwPrivate:
    case KwPublic:
    case KwStatic:
    case KwAny:
    case KwAuto:
    case KwType:
    case KwValue:
    case KwTemplate:
      return true;
    default:
      return false;
  }
}

constexpr bool eTkIsModifier(eTk t) {
  switch (t) {
    using enum eTk;
    case KwPrivate:
    case KwPublic:
    case KwConst:
    case KwStatic:
    case KwRef:
      return true;
    default:
      return false;
  }
}

constexpr bool eTkIsDeclarative(eTk t) {
  // One of: def,fn,class,obj,use,lib,main,import
  switch (t) {
    using enum eTk;
    case KwDef:
    case KwFn:
    case KwClass:
    case KwMain:
    case KwImport:
    case KwNamespace:
    case KwUse:
    case KwLib:
    case KwDll:
    case KwIf:
    case KwElif:
    case KwElse:
    case KwCxif:
    case KwCxelif:
    case KwCxelse:
    case KwSwitch:
    case KwCase:
    case KwDefault:
    case KwWhile:
    case KwFor:
    case KwReturn:
    case KwBreak:
    case KwContinue:
    case KwTemplate:
      return true;
    default:
      return false;
  }
}

constexpr bool eTkIsLScope(eTk t) {
  switch (t) {
    case eTk::LParen:
    case eTk::LBrace:
    case eTk::LBracket:
      return true;
    default:
      return false;
  }
}

constexpr bool eTkIsRScope(eTk t) {
  switch (t) {
    case eTk::RParen:
    case eTk::RBrace:
    case eTk::RBracket:
      return true;
    default:
      return false;
  }
}

constexpr bool eTkIsRScopeOf(eTk topen, eTk tclose) {
  using enum eTk;
  return (topen == LParen && tclose == RParen) ||
         (topen == LBrace && tclose == RBrace) ||
         (topen == LBracket && tclose == RBracket);
}

constexpr bool eTkIsAnOperand(eTk t) {
  switch (t) {
    using enum eTk;
      // Literals
    case LitCstr:
    case LitInt:
    case LitUint:
    case LitReal:
    case LitBool:
    case LitChar:
    case LitByte:
    case Ident:
      // Keyword Types which are operands.
    case KwInt:
    case KwUint:
    case KwReal:
    case KwBool:
    case KwChar:
    case KwByte:
    case KwCstr:
    case KwStr:
    case KwPtr:
    case KwList:
    case KwArray:
    case KwTrue:
    case KwFalse:
    case KwNone:
    case KwVoid:
      return true;
    default:
      return false;
  }
}

constexpr bool eTkIsAPrefixOperator(eTk t) {
  switch (t) {
    case eTk::Not:
    case eTk::Bnot:
      return true;
    default:
      return false;
  }
}

constexpr eAssoc eAstAssoc(eAst t) {
  switch (t) {
    case eAst::Assign:
    case eAst::AddAssign:
    case eAst::SubAssign:
    case eAst::MulAssign:
    case eAst::DivAssign:
    case eAst::ModAssign:
    case eAst::AndAssign:
    case eAst::OrAssign:
    case eAst::XorAssign:
    case eAst::LshAssign:
    case eAst::RshAssign:
    case eAst::Not:
    case eAst::Bnot:
    case eAst::Ellipsis:
    case eAst::CommercialAt:
      return eAssoc::Right;
    case eAst::Add:
    case eAst::Sub:
    case eAst::Mul:
    case eAst::Div:
    case eAst::Mod:
    case eAst::Band:
    case eAst::Bor:
    case eAst::Xor:
    case eAst::Lsh:
    case eAst::Rsh:
    case eAst::And:
    case eAst::Or:
    case eAst::Eq:
    case eAst::Neq:
    case eAst::Lt:
    case eAst::Gt:
    case eAst::Lte:
    case eAst::Gte:
    case eAst::Inc:
    case eAst::Dec:
    case eAst::DoubleColon:
    case eAst::Period:
      return eAssoc::Left;
    default:
      return eAssoc::None;
  }
}

constexpr eOperation eAstOperation(eAst t) {
  switch (t) {
    case eAst::Assign:
    case eAst::AddAssign:
    case eAst::SubAssign:
    case eAst::MulAssign:
    case eAst::DivAssign:
    case eAst::ModAssign:
    case eAst::AndAssign:
    case eAst::OrAssign:
    case eAst::XorAssign:
    case eAst::LshAssign:
    case eAst::RshAssign:
      return eOperation::Binary;
      break;
    case eAst::Inc:
    case eAst::Dec:
      return eOperation::Postfix;
      break;
    case eAst::Add:
    case eAst::Sub:
    case eAst::Mul:
    case eAst::Div:
    case eAst::Mod:
    case eAst::Band:
    case eAst::Bor:
    case eAst::Xor:
    case eAst::Lsh:
    case eAst::Rsh:
      return eOperation::Binary;
      break;
    case eAst::Not:
    case eAst::Bnot:
      return eOperation::Prefix;
      break;
    case eAst::And:
    case eAst::Or:
    case eAst::Eq:
    case eAst::Neq:
    case eAst::Lt:
    case eAst::Gt:
    case eAst::Lte:
    case eAst::Gte:
      return eOperation::Binary;
    case eAst::LParen:
    case eAst::RParen:
    case eAst::LBrace:
    case eAst::RBrace:
    case eAst::LBracket:
    case eAst::RBracket:
      return eOperation::Postfix;
    case eAst::Period:
    case eAst::DoubleColon:
      return eOperation::Binary;
    case eAst::Ellipsis:
      return eOperation::Binary;
    case eAst::CommercialAt:
      return eOperation::Prefix;
    default:
      return eOperation::None;
  }
}

constexpr ePriority eAstPriority(eAst t) {
  switch (t) {
    using enum eAst;
    case NONE:
    case INVALID:
    case Eofile:
    case LineComment:
    case BlockComment:
    case Newline:
    case Whitespace:
      return ePriority::None;
      break;
    case LitCstr:
    case LitInt:
    case LitUint:
    case LitReal:
    case LitBool:
    case LitChar:
    case LitByte:
    case Ident:
      return ePriority::Max;
      break;
    case Assign:
    case AddAssign:
    case SubAssign:
    case MulAssign:
    case DivAssign:
    case ModAssign:
    case AndAssign:
    case OrAssign:
    case XorAssign:
    case LshAssign:
    case RshAssign:
      return ePriority::Assignment;
      break;
    case Inc:
    case Dec:
      return ePriority::Postfix;
      break;
    case Add:
    case Sub:
      return ePriority::Term;
      break;
    case Mul:
    case Div:
    case Mod:
      return ePriority::Factor;
      break;
    case Not:
    case Bnot:
      return ePriority::Prefix;
      break;
    case Band:
    case Bor:
    case Xor:
    case Lsh:
    case Rsh:
    case And:
    case Or:
    case Eq:
    case Neq:
    case Lt:
    case Gt:
    case Lte:
    case Gte:
      return ePriority::Comparison;
      break;
    case LParen:
    case RParen:
    case LBrace:
    case RBrace:
    case LBracket:
    case RBracket:
      return ePriority::Postfix;
      break;
    case Semicolon:
    case Colon:
    case Comma:
    case Ellipsis:
    case CommercialAt:
      return ePriority::Max;
      break;
    case Period:
    case DoubleColon:
      return ePriority::Access;
      break;
    case KwDef:
    case KwFn:
    case KwClass:
    case KwMain:
    case KwImport:
    case KwNamespace:
    case KwUse:
    case KwLib:
    case KwDll:
    case KwIf:
    case KwElif:
    case KwElse:
    case KwCxif:
    case KwCxelif:
    case KwCxelse:
    case KwSwitch:
    case KwCase:
    case KwDefault:
    case KwWhile:
    case KwFor:
    case KwReturn:
    case KwBreak:
    case KwContinue:
    case KwInt:
    case KwUint:
    case KwReal:
    case KwBool:
    case KwChar:
    case KwByte:
    case KwCstr:
    case KwStr:
    case KwPtr:
    case KwList:
    case KwArray:
    case KwTrue:
    case KwFalse:
    case KwNone:
    case KwVoid:
    case KwIn:
    case KwAs:
    case KwCin:
    case KwCout:
    case KwNative:
    case KwConst:
    case KwRef:
    case KwPrivate:
    case KwPublic:
    case KwStatic:
    case KwAny:
    case KwAuto:
    case KwType:
    case KwValue:
    case KwTemplate:
      return ePriority::Max;
      break;
    default:
      return ePriority::None;
  }
}

constexpr std::string_view eAstLiteral(eAst t) {
#define ETK_LITERAL_CASE(n) \
  case eAst::n:              \
    return grammar::k##n

  switch (t) {
    ETK_LITERAL_CASE(NONE);
    ETK_LITERAL_CASE(INVALID);
    ETK_LITERAL_CASE(KwDef);
    ETK_LITERAL_CASE(KwFn);
    ETK_LITERAL_CASE(KwClass);
    ETK_LITERAL_CASE(KwMain);
    ETK_LITERAL_CASE(KwImport);
    ETK_LITERAL_CASE(KwNamespace);
    ETK_LITERAL_CASE(KwUse);
    ETK_LITERAL_CASE(KwLib);
    ETK_LITERAL_CASE(KwDll);
    ETK_LITERAL_CASE(KwIf);
    ETK_LITERAL_CASE(KwElif);
    ETK_LITERAL_CASE(KwElse);
    ETK_LITERAL_CASE(KwCxif);
    ETK_LITERAL_CASE(KwCxelif);
    ETK_LITERAL_CASE(KwCxelse);
    ETK_LITERAL_CASE(KwSwitch);
    ETK_LITERAL_CASE(KwCase);
    ETK_LITERAL_CASE(KwDefault);
    ETK_LITERAL_CASE(KwWhile);
    ETK_LITERAL_CASE(KwFor);
    ETK_LITERAL_CASE(KwReturn);
    ETK_LITERAL_CASE(KwBreak);
    ETK_LITERAL_CASE(KwContinue);
    ETK_LITERAL_CASE(KwInt);
    ETK_LITERAL_CASE(KwUint);
    ETK_LITERAL_CASE(KwReal);
    ETK_LITERAL_CASE(KwBool);
    ETK_LITERAL_CASE(KwChar);
    ETK_LITERAL_CASE(KwByte);
    ETK_LITERAL_CASE(KwCstr);
    ETK_LITERAL_CASE(KwStr);
    ETK_LITERAL_CASE(KwPtr);
    ETK_LITERAL_CASE(KwList);
    ETK_LITERAL_CASE(KwArray);
    ETK_LITERAL_CASE(KwTrue);
    ETK_LITERAL_CASE(KwFalse);
    ETK_LITERAL_CASE(KwNone);
    ETK_LITERAL_CASE(KwVoid);
    ETK_LITERAL_CASE(KwIn);
    ETK_LITERAL_CASE(KwAs);
    ETK_LITERAL_CASE(KwCin);
    ETK_LITERAL_CASE(KwCout);
    ETK_LITERAL_CASE(KwNative);
    ETK_LITERAL_CASE(KwConst);
    ETK_LITERAL_CASE(KwRef);
    ETK_LITERAL_CASE(KwPrivate);
    ETK_LITERAL_CASE(KwPublic);
    ETK_LITERAL_CASE(KwStatic);
    ETK_LITERAL_CASE(KwAny);
    ETK_LITERAL_CASE(KwAuto);
    ETK_LITERAL_CASE(KwType);
    ETK_LITERAL_CASE(KwValue);
    ETK_LITERAL_CASE(KwTemplate);
    ETK_LITERAL_CASE(DirectiveInclude);
    ETK_LITERAL_CASE(DirectiveDefMacro);
    ETK_LITERAL_CASE(DirectiveEndmacro);
    ETK_LITERAL_CASE(DirectiveIf);
    ETK_LITERAL_CASE(DirectiveElse);
    ETK_LITERAL_CASE(DirectiveElif);
    ETK_LITERAL_CASE(DirectiveEndif);
    ETK_LITERAL_CASE(DirectiveIfdef);
    ETK_LITERAL_CASE(DirectiveIfndef);
    ETK_LITERAL_CASE(DirectiveUndef);
    ETK_LITERAL_CASE(Hash);
    ETK_LITERAL_CASE(Add);
    ETK_LITERAL_CASE(Sub);
    ETK_LITERAL_CASE(Mul);
    ETK_LITERAL_CASE(Div);
    ETK_LITERAL_CASE(Mod);
    ETK_LITERAL_CASE(And);
    ETK_LITERAL_CASE(Or);
    ETK_LITERAL_CASE(Xor);
    ETK_LITERAL_CASE(Not);
    ETK_LITERAL_CASE(Lsh);
    ETK_LITERAL_CASE(Rsh);
    ETK_LITERAL_CASE(Eq);
    ETK_LITERAL_CASE(Neq);
    ETK_LITERAL_CASE(Lt);
    ETK_LITERAL_CASE(Gt);
    ETK_LITERAL_CASE(Lte);
    ETK_LITERAL_CASE(Gte);
    ETK_LITERAL_CASE(Assign);
    ETK_LITERAL_CASE(NewAssign);
    ETK_LITERAL_CASE(AddAssign);
    ETK_LITERAL_CASE(SubAssign);
    ETK_LITERAL_CASE(MulAssign);
    ETK_LITERAL_CASE(DivAssign);
    ETK_LITERAL_CASE(ModAssign);
    ETK_LITERAL_CASE(AndAssign);
    ETK_LITERAL_CASE(OrAssign);
    ETK_LITERAL_CASE(XorAssign);
    ETK_LITERAL_CASE(LshAssign);
    ETK_LITERAL_CASE(RshAssign);
    ETK_LITERAL_CASE(Inc);
    ETK_LITERAL_CASE(Dec);
    ETK_LITERAL_CASE(Dot);
    ETK_LITERAL_CASE(Bnot);
    ETK_LITERAL_CASE(Band);
    ETK_LITERAL_CASE(Bor);
    ETK_LITERAL_CASE(LParen);
    ETK_LITERAL_CASE(RParen);
    ETK_LITERAL_CASE(LBrace);
    ETK_LITERAL_CASE(RBrace);
    ETK_LITERAL_CASE(LBracket);
    ETK_LITERAL_CASE(RBracket);
    ETK_LITERAL_CASE(Semicolon);
    ETK_LITERAL_CASE(Colon);
    ETK_LITERAL_CASE(Comma);
    ETK_LITERAL_CASE(Period);
    ETK_LITERAL_CASE(DoubleColon);
    ETK_LITERAL_CASE(Ellipsis);
    ETK_LITERAL_CASE(CommercialAt);
    ETK_LITERAL_CASE(Eofile);
    ETK_LITERAL_CASE(Whitespace);
    ETK_LITERAL_CASE(Newline);
    ETK_LITERAL_CASE(BlockComment);
    ETK_LITERAL_CASE(LineComment);
    ETK_LITERAL_CASE(LitInt);
    ETK_LITERAL_CASE(LitUint);
    ETK_LITERAL_CASE(LitReal);
    ETK_LITERAL_CASE(LitBool);
    ETK_LITERAL_CASE(LitChar);
    ETK_LITERAL_CASE(LitByte);
    ETK_LITERAL_CASE(LitCstr);
    ETK_LITERAL_CASE(Ident);
  }

#undef ETK_LITERAL_CASE
}

constexpr bool eAstIsKeyword(eAst t) {
  switch (t) {
    using enum eAst;
    case KwDef:
    case KwFn:
    case KwClass:
    case KwMain:
    case KwImport:
    case KwNamespace:
    case KwUse:
    case KwLib:
    case KwDll:
    case KwIf:
    case KwElif:
    case KwElse:
    case KwCxif:
    case KwCxelif:
    case KwCxelse:
    case KwSwitch:
    case KwCase:
    case KwDefault:
    case KwWhile:
    case KwFor:
    case KwReturn:
    case KwBreak:
    case KwContinue:
    case KwInt:
    case KwUint:
    case KwReal:
    case KwBool:
    case KwChar:
    case KwByte:
    case KwCstr:
    case KwStr:
    case KwPtr:
    case KwList:
    case KwArray:
    case KwTrue:
    case KwFalse:
    case KwNone:
    case KwVoid:
    case KwIn:
    case KwAs:
    case KwCin:
    case KwCout:
    case KwNative:
    case KwConst:
    case KwRef:
    case KwPrivate:
    case KwPublic:
    case KwStatic:
    case KwAny:
    case KwAuto:
    case KwType:
    case KwValue:
    case KwTemplate:
      return true;
    default:
      return false;
  }
}

constexpr bool eAstIsModifier(eAst t) {
  switch (t) {
    using enum eAst;
    case KwPrivate:
    case KwPublic:
    case KwConst:
    case KwStatic:
    case KwRef:
      return true;
    default:
      return false;
  }
}

constexpr bool eAstIsDeclarative(eAst t) {
  // One of: def,fn,class,obj,use,lib,main,import
  switch (t) {
    using enum eAst;
    case KwDef:
    case KwFn:
    case KwClass:
    case KwMain:
    case KwImport:
    case KwNamespace:
    case KwUse:
    case KwLib:
    case KwDll:
    case KwIf:
    case KwElif:
    case KwElse:
    case KwCxif:
    case KwCxelif:
    case KwCxelse:
    case KwSwitch:
    case KwCase:
    case KwDefault:
    case KwWhile:
    case KwFor:
    case KwReturn:
    case KwBreak:
    case KwContinue:
    case KwTemplate:
      return true;
    default:
      return false;
  }
}

constexpr bool eAstIsLScope(eAst t) {
  switch (t) {
    case eAst::LParen:
    case eAst::LBrace:
    case eAst::LBracket:
      return true;
    default:
      return false;
  }
}

constexpr bool eAstIsRScope(eAst t) {
  switch (t) {
    case eAst::RParen:
    case eAst::RBrace:
    case eAst::RBracket:
      return true;
    default:
      return false;
  }
}

constexpr bool eAstIsRScopeOf(eAst topen, eAst tclose) {
  using enum eAst;
  return (topen == LParen && tclose == RParen) ||
         (topen == LBrace && tclose == RBrace) ||
         (topen == LBracket && tclose == RBracket);
}

constexpr bool eAstIsAnOperand(eAst t) {
  switch (t) {
    using enum eAst;
      // Literals
    case LitCstr:
    case LitInt:
    case LitUint:
    case LitReal:
    case LitBool:
    case LitChar:
    case LitByte:
    case Ident:
      // Keyword Types which are operands.
    case KwInt:
    case KwUint:
    case KwReal:
    case KwBool:
    case KwChar:
    case KwByte:
    case KwCstr:
    case KwStr:
    case KwPtr:
    case KwList:
    case KwArray:
    case KwTrue:
    case KwFalse:
    case KwNone:
    case KwVoid:
      return true;
    default:
      return false;
  }
}

constexpr bool eAstIsAPrefixOperator(eAst t) {
  switch (t) {
    case eAst::Not:
    case eAst::Bnot:
      return true;
    default:
      return false;
  }
}

template <eTk TkType>
struct TkTrait {
  static constexpr eTk type = TkType;
  static constexpr string_view enum_str = eTkEnumStr(TkType);
  static constexpr string_view literal = eTkLiteral(TkType);
  static constexpr eAst ast_type = eTkToAstEnum(TkType);
  static constexpr ePriority priority = eTkPriority(TkType);
  static constexpr eOperation operation = eTkOperation(TkType);
  static constexpr eAssoc assoc = eTkAssoc(TkType);
  // Keyword Properties
  static constexpr bool keyword = eTkIsKeyword(TkType);
  static constexpr bool modifier = eTkIsModifier(TkType);
  static constexpr bool declarative = eTkIsDeclarative(TkType);
  // Scope Properties
  static constexpr bool lscope = eTkIsLScope(TkType);
  static constexpr bool rscope = eTkIsRScope(TkType);
  // Primary Expression Properties
  static constexpr bool an_operand = eTkIsAnOperand(TkType);
  static constexpr bool prefix = eTkIsAPrefixOperator(TkType);
  static constexpr bool primary_expr_begin = eTkIsPrimary(TkType);
  template <eTk OpenScopeTkType>
  static constexpr bool rscope_of = eTkIsRScopeOf(OpenScopeTkType, TkType);
};

template <eTk... TkType>
using TraitsOf = tuple<TkTrait<TkType>...>;

namespace tk_traits {
using enum eTk;
static constexpr auto kKeywordTraits = TraitsOf<KEYWORD_GRAMMAR_ELEMENTS>{};
static constexpr auto kDirectiveTraits = TraitsOf<DIRECTIVE_GRAMMAR_ELEMENTS>{};

}  // namespace tk_traits

///////////////////////////////////////////////////////////////////////////
/// @} // end of cand_compiler_data_token_traits
///////////////////////////////////////////////////////////////////////////

}  // namespace caoco

#endif HEADER_GUARD_CALE_CAND_OFFICIAL_COMPILER_CAOCO_GRAMMAR_H
/// @} // end of cand_compiler_data

///////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language Environment
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
//
// Licensed under the Apache License, Version 2.0(the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
///////////////////////////////////////////////////////////////////////////////