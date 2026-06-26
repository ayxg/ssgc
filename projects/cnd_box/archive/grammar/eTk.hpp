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
/// @brief eTk enum definition.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
// clang-format off
#include "ccapi/CommonCppApi.hpp"
#include "compiler_utils/ReflectedMetaEnum.hpp"
// clang-format on

/// @addtogroup cnd_compiler_corevals
/// @{

namespace cnd::corevals::grammar {
/// Define applied enum cnd::corevals::grammar::eTk
#define CND_APPLIED_ENUM_eTk(m, sep, pre, lst) \
  pre m(INVALID)                               \
  sep m(NONE)                                  \
  sep m(DirectiveInclude)                      \
  sep m(DirectiveDefMacro)                     \
  sep m(DirectiveEndmacro)                     \
  sep m(DirectiveIf)                           \
  sep m(DirectiveElse)                         \
  sep m(DirectiveElif)                         \
  sep m(DirectiveEndif)                        \
  sep m(DirectiveIfdef)                        \
  sep m(DirectiveIfndef)                       \
  sep m(DirectiveUndef)                        \
  sep m(KwDef)                                 \
  sep m(KwFn)                                  \
  sep m(KwClass)                               \
  sep m(KwMain)                                \
  sep m(KwImport)                              \
  sep m(KwNamespace)                           \
  sep m(KwUse)                                 \
  sep m(KwLib)                                 \
  sep m(KwDll)                                 \
  sep m(KwEnum)                                \
  sep m(KwIf)                                  \
  sep m(KwElif)                                \
  sep m(KwElse)                                \
  sep m(KwCxif)                                \
  sep m(KwCxelif)                              \
  sep m(KwCxelse)                              \
  sep m(KwSwitch)                              \
  sep m(KwCase)                                \
  sep m(KwDefault)                             \
  sep m(KwWhile)                               \
  sep m(KwFor)                                 \
  sep m(KwReturn)                              \
  sep m(KwBreak)                               \
  sep m(KwContinue)                            \
  sep m(KwInt)                                 \
  sep m(KwUint)                                \
  sep m(KwReal)                                \
  sep m(KwBool)                                \
  sep m(KwChar)                                \
  sep m(KwByte)                                \
  sep m(KwCstr)                                \
  sep m(KwStr)                                 \
  sep m(KwPtr)                                 \
  sep m(KwList)                                \
  sep m(KwArray)                               \
  sep m(KwTrue)                                \
  sep m(KwFalse)                               \
  sep m(KwNone)                                \
  sep m(KwVoid)                                \
  sep m(KwIn)                                  \
  sep m(KwAs)                                  \
  sep m(KwCin)                                 \
  sep m(KwCout)                                \
  sep m(KwNative)                              \
  sep m(KwConst)                               \
  sep m(KwRef)                                 \
  sep m(KwPrivate)                             \
  sep m(KwPublic)                              \
  sep m(KwStatic)                              \
  sep m(KwAny)                                 \
  sep m(KwAuto)                                \
  sep m(KwType)                                \
  sep m(KwValue)                               \
  sep m(KwTemplate)                            \
  sep m(KwProc)                                \
  sep m(Hash)                                  \
  sep m(Add)                                   \
  sep m(Sub)                                   \
  sep m(Mul)                                   \
  sep m(Div)                                   \
  sep m(Mod)                                   \
  sep m(And)                                   \
  sep m(Or)                                    \
  sep m(Xor)                                   \
  sep m(Not)                                   \
  sep m(Lsh)                                   \
  sep m(Rsh)                                   \
  sep m(Eq)                                    \
  sep m(Neq)                                   \
  sep m(Lt)                                    \
  sep m(Gt)                                    \
  sep m(Lte)                                   \
  sep m(Gte)                                   \
  sep m(Spaceship)                             \
  sep m(Assign)                                \
  sep m(NewAssign)                             \
  sep m(AddAssign)                             \
  sep m(SubAssign)                             \
  sep m(MulAssign)                             \
  sep m(DivAssign)                             \
  sep m(ModAssign)                             \
  sep m(AndAssign)                             \
  sep m(OrAssign)                              \
  sep m(XorAssign)                             \
  sep m(LshAssign)                             \
  sep m(RshAssign)                             \
  sep m(Inc)                                   \
  sep m(Dec)                                   \
  sep m(Bnot)                                  \
  sep m(Band)                                  \
  sep m(Bor)                                   \
  sep m(LParen)                                \
  sep m(RParen)                                \
  sep m(LBrace)                                \
  sep m(RBrace)                                \
  sep m(LBracket)                              \
  sep m(RBracket)                              \
  sep m(Semicolon)                             \
  sep m(Colon)                                 \
  sep m(Comma)                                 \
  sep m(Period)                                \
  sep m(DoubleColon)                           \
  sep m(Ellipsis)                              \
  sep m(CommercialAt)                          \
  sep m(Dollar)                                \
  sep m(Backlash)                              \
  sep m(Question)                              \
  sep m(LitInt)                                \
  sep m(LitUint)                               \
  sep m(LitBool)                               \
  sep m(LitChar)                               \
  sep m(LitByte)                               \
  sep m(LitCstr)                               \
  sep m(LitU1)                                 \
  sep m(LitU8)                                 \
  sep m(LitU16)                                \
  sep m(LitU32)                                \
  sep m(LitU64)                                \
  sep m(LitI8)                                 \
  sep m(LitI16)                                \
  sep m(LitI32)                                \
  sep m(LitI64)                                \
  sep m(LitF32)                                \
  sep m(LitF64)                                \
  sep m(LitReal)                               \
  sep m(Ident)                                 \
  sep m(Whitespace)                            \
  sep m(Newline)                               \
  sep m(BlockComment)                          \
  sep m(LineComment)                           \
  sep m(Eofile)                                \
  lst

// Define the enum.
enum class eTk {
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
  kKwInclude,
  kKwPragma,
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
  COUNT
};
// Define the enum to cstr conversion.
constexpr const char* eTkToCStr(eTk e) noexcept {
  using enum eTk;
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
    case COUNT:
      return "COUNT";
    default:
      return "<invalid>";
  }
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