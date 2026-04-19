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
/// @brief eAst enum definition.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
// clang-format off
#include "ccapi/CommonCppApi.hpp"
#include "compiler_utils/ReflectedMetaEnum.hpp"
#include "grammar/eTk.hpp"
// clang-format on

/// @addtogroup cnd_compiler_corevals
/// @{

namespace cnd::corevals::grammar {

/// @note all of eTk is part of eAst appended at the front.
#ifndef CND_APPLIED_ENUM_eTk
#error Macro symbol ''CND_APPLIED_ENUM_eTk' from file 'corevals/enumdefs/grammar/eTk.hpp' must be available.
#endif

/// Define applied enum cnd::corevals::grammar::eAst
#define CND_APPLIED_ENUM_eAst(m, sep, pre, lst)              \
  CND_APPLIED_ENUM_eTk(m, sep, pre, ) sep m(UndefinedObject) \
  sep m(ResolutionOp)                                        \
  sep m(RootNamespace)                                       \
  sep m(MemberAccess)                                        \
  sep m(ParenSubexpr)                                        \
  sep m(SquareSubexpr)                                       \
  sep m(CurlySubexpr)                                        \
  sep m(Statement)                                           \
  sep m(Expression)                                          \
  sep m(Declaration)                                         \
  sep m(TypeAlias)                                           \
  sep m(LibraryTypeAlias)                                    \
  sep m(LibraryObjectInclusion)                              \
  sep m(LibraryNamespaceInclusion)                           \
  sep m(NamespaceInclusion)                                  \
  sep m(NamespaceObjectInclusion)                            \
  sep m(VariableDeclaration)                                 \
  sep m(VariableDefinition)                                  \
  sep m(MethodDeclaration)                                   \
  sep m(MethodDefinition)                                    \
  sep m(ClassDefinition)                                     \
  sep m(ClassDeclaration)                                    \
  sep m(ImportDeclaration)                                   \
  sep m(ProcessDeclaration)                                  \
  sep m(ProcessDefinition)                                   \
  sep m(LibraryDeclaration)                                  \
  sep m(LibraryDefinition)                                   \
  sep m(MainDeclaration)                                     \
  sep m(MainDefinition)                                      \
  sep m(MethodVoid)                                          \
  sep m(MethodSignature)                                     \
  sep m(MethodParameter)                                     \
  sep m(MethodParameterList)                                 \
  sep m(MethodReturnType)                                    \
  sep m(IfStatement)                                         \
  sep m(Program)                                             \
  sep m(PragmaticBlock)                                      \
  sep m(FunctionalBlock)                                     \
  sep m(ConditionalBlock)                                    \
  sep m(IterativeBlock)                                      \
  sep m(TypeConstraints)                                     \
  sep m(CaptureList)                                         \
  sep m(Subexpression)                                       \
  sep m(TypeList)                                            \
  sep m(GenericList)                                         \
  sep m(FunctionCall)                                        \
  sep m(Arguments)                                           \
  sep m(TypeCall)                                            \
  sep m(TypeArguments)                                       \
  sep m(IndexOperator)                                       \
  sep m(ListingOperator)                                     \
  sep m(IndexArguments)                                      \
  sep m(Modifiers)                                           \
  sep m(UnaryMinus)                                          \
  sep m(UnaryPlus)                                           \
  sep m(Dereference)                                         \
  sep m(AddressOf)                                           \
  sep m(EnumDeclaration)                                     \
  sep m(EnumDefinition)                                      \
  sep m(EnumAssociation)                                     \
  sep m(EnumBlock)                                           \
  sep m(EnumCategory)                                        \
  sep m(EnumEntry)                                           \
  sep m(IncludeSystemStmt)                                   \
  sep m(IncludeLocalStmt)                                    \
  sep m(IQCharSeuquence)                                     \
  sep m(PFCharSequence)                                      \
  sep m(IACharSeuquence)                                     \
  sep m(PragmaEscapedStmt)                                   \
  sep m(PragmaFunctionalStmt)                                \
  sep m(PragmaNamedStmt)                                     \
  sep m(NamespaceDecl)\
  sep m(UnnamedScope)                                        \
  lst

// Define the enum.
enum class eAst {
  kINVALID,
  kNONE,
  kDirectiveInclude,
  kDirectiveDefMacro,
  kDirectiveEndmacro,
  kDirectiveIf,
  kDirectiveElse,
  kDirectiveElif,
  kDirectiveEndif,
  kDirectiveIfdef,
  kDirectiveIfndef,
  kDirectiveUndef,
  kKwDef,
  kKwFn,
  kKwClass,
  kKwMain,
  kKwImport,
  kKwNamespace,
  kKwUse,
  kKwLib,
  kKwDll,
  kKwEnum,
  kKwIf,
  kKwElif,
  kKwElse,
  kKwCxif,
  kKwCxelif,
  kKwCxelse,
  kKwSwitch,
  kKwCase,
  kKwDefault,
  kKwWhile,
  kKwFor,
  kKwReturn,
  kKwBreak,
  kKwContinue,
  kKwInt,
  kKwUint,
  kKwReal,
  kKwBool,
  kKwChar,
  kKwByte,
  kKwCstr,
  kKwStr,
  kKwPtr,
  kKwList,
  kKwArray,
  kKwTrue,
  kKwFalse,
  kKwNone,
  kKwVoid,
  kKwIn,
  kKwAs,
  kKwCin,
  kKwCout,
  kKwNative,
  kKwConst,
  kKwRef,
  kKwPrivate,
  kKwPublic,
  kKwStatic,
  kKwAny,
  kKwAuto,
  kKwType,
  kKwValue,
  kKwTemplate,
  kKwProc,
  kHash,
  kAdd,
  kSub,
  kMul,
  kDiv,
  kMod,
  kAnd,
  kOr,
  kXor,
  kNot,
  kLsh,
  kRsh,
  kEq,
  kNeq,
  kLt,
  kGt,
  kLte,
  kGte,
  kSpaceship,
  kAssign,
  kNewAssign,
  kAddAssign,
  kSubAssign,
  kMulAssign,
  kDivAssign,
  kModAssign,
  kAndAssign,
  kOrAssign,
  kXorAssign,
  kLshAssign,
  kRshAssign,
  kInc,
  kDec,
  kBnot,
  kBand,
  kBor,
  kLParen,
  kRParen,
  kLBrace,
  kRBrace,
  kLBracket,
  kRBracket,
  kSemicolon,
  kColon,
  kComma,
  kPeriod,
  kDoubleColon,
  kEllipsis,
  kCommercialAt,
  kDollar,
  kBacklash,
  kQuestion,
  kLitInt,
  kLitUint,
  kLitBool,
  kLitChar,
  kLitByte,
  kLitCstr,
  kLitU1,
  kLitU8,
  kLitU16,
  kLitU32,
  kLitU64,
  kLitI8,
  kLitI16,
  kLitI32,
  kLitI64,
  kLitF32,
  kLitF64,
  kLitReal,
  kIdent,
  kWhitespace,
  kNewline,
  kBlockComment,
  kLineComment,
  kEofile,
  kUndefinedObject,
  kResolutionOp,
  kRootNamespace,
  kMemberAccess,
  kParenSubexpr,
  kSquareSubexpr,
  kCurlySubexpr,
  kStatement,
  kExpression,
  kDeclaration,
  kTypeAlias,
  kLibraryTypeAlias,
  kLibraryObjectInclusion,
  kLibraryNamespaceInclusion,
  kNamespaceInclusion,
  kNamespaceObjectInclusion,
  kVariableDeclaration,
  kVariableDefinition,
  kMethodDeclaration,
  kMethodDefinition,
  kClassDefinition,
  kClassDeclaration,
  kImportDeclaration,
  kProcessDeclaration,
  kProcessDefinition,
  kLibraryDeclaration,
  kLibraryDefinition,
  kMainDeclaration,
  kMainDefinition,
  kMethodVoid,
  kMethodSignature,
  kMethodParameter,
  kMethodParameterList,
  kMethodReturnType,
  kIfStatement,
  kProgram,
  kPragmaticBlock,
  kFunctionalBlock,
  kConditionalBlock,
  kIterativeBlock,
  kTypeConstraints,
  kCaptureList,
  kSubexpression,
  kTypeList,
  kGenericList,
  kFunctionCall,
  kArguments,
  kTypeCall,
  kTypeArguments,
  kIndexOperator,
  kListingOperator,
  kIndexArguments,
  kModifiers,
  kUnaryMinus,
  kUnaryPlus,
  kDereference,
  kAddressOf,
  kEnumDeclaration,
  kEnumDefinition,
  kEnumAssociation,
  kEnumBlock,
  kEnumCategory,
  kEnumEntry,
  kIncludeSystemStmt,
  kIncludeLocalStmt,
  kIQCharSeuquence,
  kPFCharSequence,
  kIACharSeuquence,
  kPragmaEscapedStmt,
  kPragmaFunctionalStmt,
  kPragmaNamedStmt,
  kNamespaceDecl,
  kUnnamedScope,
  COUNT
};
// Define the enum to cstr conversion.
constexpr const char* eAstToCStr(eAst e) noexcept {
  using enum eAst;
  switch (e) {
    case kINVALID:
      return "kINVALID";
    case kNONE:
      return "kNONE";
    case kDirectiveInclude:
      return "kDirectiveInclude";
    case kDirectiveDefMacro:
      return "kDirectiveDefMacro";
    case kDirectiveEndmacro:
      return "kDirectiveEndmacro";
    case kDirectiveIf:
      return "kDirectiveIf";
    case kDirectiveElse:
      return "kDirectiveElse";
    case kDirectiveElif:
      return "kDirectiveElif";
    case kDirectiveEndif:
      return "kDirectiveEndif";
    case kDirectiveIfdef:
      return "kDirectiveIfdef";
    case kDirectiveIfndef:
      return "kDirectiveIfndef";
    case kDirectiveUndef:
      return "kDirectiveUndef";
    case kKwDef:
      return "kKwDef";
    case kKwFn:
      return "kKwFn";
    case kKwClass:
      return "kKwClass";
    case kKwMain:
      return "kKwMain";
    case kKwImport:
      return "kKwImport";
    case kKwNamespace:
      return "kKwNamespace";
    case kKwUse:
      return "kKwUse";
    case kKwLib:
      return "kKwLib";
    case kKwDll:
      return "kKwDll";
    case kKwEnum:
      return "kKwEnum";
    case kKwIf:
      return "kKwIf";
    case kKwElif:
      return "kKwElif";
    case kKwElse:
      return "kKwElse";
    case kKwCxif:
      return "kKwCxif";
    case kKwCxelif:
      return "kKwCxelif";
    case kKwCxelse:
      return "kKwCxelse";
    case kKwSwitch:
      return "kKwSwitch";
    case kKwCase:
      return "kKwCase";
    case kKwDefault:
      return "kKwDefault";
    case kKwWhile:
      return "kKwWhile";
    case kKwFor:
      return "kKwFor";
    case kKwReturn:
      return "kKwReturn";
    case kKwBreak:
      return "kKwBreak";
    case kKwContinue:
      return "kKwContinue";
    case kKwInt:
      return "kKwInt";
    case kKwUint:
      return "kKwUint";
    case kKwReal:
      return "kKwReal";
    case kKwBool:
      return "kKwBool";
    case kKwChar:
      return "kKwChar";
    case kKwByte:
      return "kKwByte";
    case kKwCstr:
      return "kKwCstr";
    case kKwStr:
      return "kKwStr";
    case kKwPtr:
      return "kKwPtr";
    case kKwList:
      return "kKwList";
    case kKwArray:
      return "kKwArray";
    case kKwTrue:
      return "kKwTrue";
    case kKwFalse:
      return "kKwFalse";
    case kKwNone:
      return "kKwNone";
    case kKwVoid:
      return "kKwVoid";
    case kKwIn:
      return "kKwIn";
    case kKwAs:
      return "kKwAs";
    case kKwCin:
      return "kKwCin";
    case kKwCout:
      return "kKwCout";
    case kKwNative:
      return "kKwNative";
    case kKwConst:
      return "kKwConst";
    case kKwRef:
      return "kKwRef";
    case kKwPrivate:
      return "kKwPrivate";
    case kKwPublic:
      return "kKwPublic";
    case kKwStatic:
      return "kKwStatic";
    case kKwAny:
      return "kKwAny";
    case kKwAuto:
      return "kKwAuto";
    case kKwType:
      return "kKwType";
    case kKwValue:
      return "kKwValue";
    case kKwTemplate:
      return "kKwTemplate";
    case kKwProc:
      return "kKwProc";
    case kHash:
      return "kHash";
    case kAdd:
      return "kAdd";
    case kSub:
      return "kSub";
    case kMul:
      return "kMul";
    case kDiv:
      return "kDiv";
    case kMod:
      return "kMod";
    case kAnd:
      return "kAnd";
    case kOr:
      return "kOr";
    case kXor:
      return "kXor";
    case kNot:
      return "kNot";
    case kLsh:
      return "kLsh";
    case kRsh:
      return "kRsh";
    case kEq:
      return "kEq";
    case kNeq:
      return "kNeq";
    case kLt:
      return "kLt";
    case kGt:
      return "kGt";
    case kLte:
      return "kLte";
    case kGte:
      return "kGte";
    case kSpaceship:
      return "kSpaceship";
    case kAssign:
      return "kAssign";
    case kNewAssign:
      return "kNewAssign";
    case kAddAssign:
      return "kAddAssign";
    case kSubAssign:
      return "kSubAssign";
    case kMulAssign:
      return "kMulAssign";
    case kDivAssign:
      return "kDivAssign";
    case kModAssign:
      return "kModAssign";
    case kAndAssign:
      return "kAndAssign";
    case kOrAssign:
      return "kOrAssign";
    case kXorAssign:
      return "kXorAssign";
    case kLshAssign:
      return "kLshAssign";
    case kRshAssign:
      return "kRshAssign";
    case kInc:
      return "kInc";
    case kDec:
      return "kDec";
    case kBnot:
      return "kBnot";
    case kBand:
      return "kBand";
    case kBor:
      return "kBor";
    case kLParen:
      return "kLParen";
    case kRParen:
      return "kRParen";
    case kLBrace:
      return "kLBrace";
    case kRBrace:
      return "kRBrace";
    case kLBracket:
      return "kLBracket";
    case kRBracket:
      return "kRBracket";
    case kSemicolon:
      return "kSemicolon";
    case kColon:
      return "kColon";
    case kComma:
      return "kComma";
    case kPeriod:
      return "kPeriod";
    case kDoubleColon:
      return "kDoubleColon";
    case kEllipsis:
      return "kEllipsis";
    case kCommercialAt:
      return "kCommercialAt";
    case kDollar:
      return "kDollar";
    case kBacklash:
      return "kBacklash";
    case kQuestion:
      return "kQuestion";
    case kLitInt:
      return "kLitInt";
    case kLitUint:
      return "kLitUint";
    case kLitBool:
      return "kLitBool";
    case kLitChar:
      return "kLitChar";
    case kLitByte:
      return "kLitByte";
    case kLitCstr:
      return "kLitCstr";
    case kLitU1:
      return "kLitU1";
    case kLitU8:
      return "kLitU8";
    case kLitU16:
      return "kLitU16";
    case kLitU32:
      return "kLitU32";
    case kLitU64:
      return "kLitU64";
    case kLitI8:
      return "kLitI8";
    case kLitI16:
      return "kLitI16";
    case kLitI32:
      return "kLitI32";
    case kLitI64:
      return "kLitI64";
    case kLitF32:
      return "kLitF32";
    case kLitF64:
      return "kLitF64";
    case kLitReal:
      return "kLitReal";
    case kIdent:
      return "kIdent";
    case kWhitespace:
      return "kWhitespace";
    case kNewline:
      return "kNewline";
    case kBlockComment:
      return "kBlockComment";
    case kLineComment:
      return "kLineComment";
    case kEofile:
      return "kEofile";
    case kUndefinedObject:
      return "kUndefinedObject";
    case kResolutionOp:
      return "kResolutionOp";
    case kRootNamespace:
      return "kRootNamespace";
    case kMemberAccess:
      return "kMemberAccess";
    case kParenSubexpr:
      return "kParenSubexpr";
    case kSquareSubexpr:
      return "kSquareSubexpr";
    case kCurlySubexpr:
      return "kCurlySubexpr";
    case kStatement:
      return "kStatement";
    case kExpression:
      return "kExpression";
    case kDeclaration:
      return "kDeclaration";
    case kTypeAlias:
      return "kTypeAlias";
    case kLibraryTypeAlias:
      return "kLibraryTypeAlias";
    case kLibraryObjectInclusion:
      return "kLibraryObjectInclusion";
    case kLibraryNamespaceInclusion:
      return "kLibraryNamespaceInclusion";
    case kNamespaceInclusion:
      return "kNamespaceInclusion";
    case kNamespaceObjectInclusion:
      return "kNamespaceObjectInclusion";
    case kVariableDeclaration:
      return "kVariableDeclaration";
    case kVariableDefinition:
      return "kVariableDefinition";
    case kMethodDeclaration:
      return "kMethodDeclaration";
    case kMethodDefinition:
      return "kMethodDefinition";
    case kClassDefinition:
      return "kClassDefinition";
    case kClassDeclaration:
      return "kClassDeclaration";
    case kImportDeclaration:
      return "kImportDeclaration";
    case kProcessDeclaration:
      return "kProcessDeclaration";
    case kProcessDefinition:
      return "kProcessDefinition";
    case kLibraryDeclaration:
      return "kLibraryDeclaration";
    case kLibraryDefinition:
      return "kLibraryDefinition";
    case kMainDeclaration:
      return "kMainDeclaration";
    case kMainDefinition:
      return "kMainDefinition";
    case kMethodVoid:
      return "kMethodVoid";
    case kMethodSignature:
      return "kMethodSignature";
    case kMethodParameter:
      return "kMethodParameter";
    case kMethodParameterList:
      return "kMethodParameterList";
    case kMethodReturnType:
      return "kMethodReturnType";
    case kIfStatement:
      return "kIfStatement";
    case kProgram:
      return "kProgram";
    case kPragmaticBlock:
      return "kPragmaticBlock";
    case kFunctionalBlock:
      return "kFunctionalBlock";
    case kConditionalBlock:
      return "kConditionalBlock";
    case kIterativeBlock:
      return "kIterativeBlock";
    case kTypeConstraints:
      return "kTypeConstraints";
    case kCaptureList:
      return "kCaptureList";
    case kSubexpression:
      return "kSubexpression";
    case kTypeList:
      return "kTypeList";
    case kGenericList:
      return "kGenericList";
    case kFunctionCall:
      return "kFunctionCall";
    case kArguments:
      return "kArguments";
    case kTypeCall:
      return "kTypeCall";
    case kTypeArguments:
      return "kTypeArguments";
    case kIndexOperator:
      return "kIndexOperator";
    case kListingOperator:
      return "kListingOperator";
    case kIndexArguments:
      return "kIndexArguments";
    case kModifiers:
      return "kModifiers";
    case kUnaryMinus:
      return "kUnaryMinus";
    case kUnaryPlus:
      return "kUnaryPlus";
    case kDereference:
      return "kDereference";
    case kAddressOf:
      return "kAddressOf";
    case kEnumDeclaration:
      return "kEnumDeclaration";
    case kEnumDefinition:
      return "kEnumDefinition";
    case kEnumAssociation:
      return "kEnumAssociation";
    case kEnumBlock:
      return "kEnumBlock";
    case kEnumCategory:
      return "kEnumCategory";
    case kEnumEntry:
      return "kEnumEntry";
    case kIncludeSystemStmt:
      return "kIncludeSystemStmt";
    case kIncludeLocalStmt:
      return "kIncludeLocalStmt";
    case kIQCharSeuquence:
      return "kIQCharSeuquence";
    case kPFCharSequence:
      return "kPFCharSequence";
    case kIACharSeuquence:
      return "kIACharSeuquence";
    case kPragmaEscapedStmt:
      return "kPragmaEscapedStmt";
    case kPragmaFunctionalStmt:
      return "kPragmaFunctionalStmt";
    case kPragmaNamedStmt:
      return "kPragmaNamedStmt";
    case kNamespaceDecl:
      return "kNamespaceDecl";
    case kUnnamedScope:
      return "kUnnamedScope";
    case COUNT:
      return "COUNT";
    default:
      return "<invalid>";
  }
};
// Assert enum to cstr conversion.
#define CND_STATIC_ASSERT_ENUM_TO_CSTR_eAst(x) CND_MM_STATIC_ASSERT_ENUM_TO_CSTR(x, eAst, eAstToCStr)

#undef CND_STATIC_ASSERT_ENUM_TO_CSTR_eAst
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