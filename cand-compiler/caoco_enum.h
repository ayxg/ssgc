//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
// Copyright 2024 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
//---------------------------------------------------------------------------//
// Author(s): Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.acpp.dev
//---------------------------------------------------------------------------//
// Project: C& Programming Language Environment
// Directory: cand-official-compiler
// File: ccaoco_enum.h
//---------------------------------------------------------------------------//
#ifndef HEADER_GUARD_CALE_CAND_OFFICIAL_COMPILER_CAOCO_ENUM_H
#define HEADER_GUARD_CALE_CAND_OFFICIAL_COMPILER_CAOCO_ENUM_H
//---------------------------------------------------------------------------//
// Brief: Enumerations used in the C& Compiler
//---------------------------------------------------------------------------//
// Includes: 
#include "cppsextended.h"
//---------------------------------------------------------------------------//

namespace caoco {
enum class eAssoc : int {
kInvalid = -1,
kNone = 0,
kLeft,
kRight,
};

enum class eOperation : int {
kInvalid = -1,
kNone = 0,
kBinary,
kPrefix,
kPostfix,
};

enum class ePriority : int {
kInvalid = -1,
kNone = 0,
kAssignment = 20000,
kLogicalOr = 60000,
kLogicalAnd = 70000,
kBitwiseOr = 80000,
kBitwiseXor = 85000,
kBitwiseAnd = 90000,
kEquality = 100000,
kThreeWayEquality = 110000,
kComparison = 120000,
kBitshift = 130000,
kTerm = 140000,
kFactor = 150000,
kPrefix = 160000,
kPostfix = 170000,
kFunctional = 180000,
kAccess = 190000,
kMax = INT_MAX,
};

enum class eTk : int {
// abstract
kNone = -1,
kInvalid,
kEof,

kLineComment,
kBlockComment,
kNewline,
kWhitespace,

// literals
kStringLiteral,
kNumberLiteral,
kDoubleLiteral,
kCharLiteral,
kByteLiteral,
kBoolLiteral,
kUnsignedLiteral,
kIdentifier,

// assignment operators
kSimpleAssignment,
kAdditionAssignment,
kSubtractionAssignment,
kMultiplicationAssignment,
kDivisionAssignment,
kRemainderAssignment,
kBitwiseAndAssignment,
kBitwiseOrAssignment,
kBitwiseXorAssignment,
kLeftShiftAssignment,
kRightShiftAssignment,

// postfix increment and decrement operators
kIncrement,
kDecrement,

// arithmetic operators
kAddition,
kSubtraction,
kMultiplication,
kDivision,
kRemainder,
kBitwiseAnd,
kBitwiseOr,
kBitwiseXor,
kBitwiseLeftShift,
kBitwiseRightShift,

// prefix operators
kNegation,
kBitwiseNot,

// logical
kLogicalAnd,
kLogicalOr,

// comparison
kEqual,
kNotEqual,
kLessThan,
kGreaterThan,
kLessThanOrEqual,
kGreaterThanOrEqual,
kThreeWayComparison,

// scopes
kOpenParen,
kCloseParen,
kOpenBrace,
kCloseBrace,
kOpenBracket,
kCloseBracket,

// punctuation
kSemicolon,
kColon,
kDoubleColon,
kComma,
kPeriod,
kEllipsis,
kCommercialAt,

// keyword literal values
kNoneLiteral,
kTrueLiteral,
kFalseLiteral,

// keyword types
kAny,
kInt,
kUint,
kReal,
kByte,
kBit,
kStr,
kList,
kArray,
kAuto,
// preprocessor keywords
kDirInclude,
kDirMacro,
kDirEndmacro,
kDirIf,
kDirElse,
kDirElif,
// directive keywords
kLib,
kMain,
kImport,
kUse,
kDef,  // todo: implement
kFn,
kClass,
kNamespace,
// modifier tokens
kPrivate,
kPublic,
kConst,
kStatic,
kRef,

// control flow tokens
kIf,
kElse,
kElif,
kWhile,
kFor,
kSwitch,
kCase,
kDefault,
kBreak,
kContinue,
kReturn,

};

enum class eAst : int {
// abstract
kNone = -1,
kInvalid,
kEof,

// invisible
kLineComment,
kBlockComment,
kNewline,
kWhitespace,

// literals
kStringLiteral,
kNumberLiteral,
kDoubleLiteral,
kByteLiteral,
kBoolLiteral,
kUnsignedLiteral,
kIdentifier,

// assignemnt operators
kSimpleAssignment,
kAdditionAssignment,
kSubtractionAssignment,
kMultiplicationAssignment,
kDivisionAssignment,
kRemainderAssignment,
kBitwiseAndAssignment,
kBitwiseOrAssignment,
kBitwiseXorAssignment,
kLeftShiftAssignment,
kRightShiftAssignment,

// postifx increment and decrement operators
kIncrement,
kDecrement,

// arithmetic operators
kAddition,
kSubtraction,
kMultiplication,
kDivision,
kRemainder,
kBitwiseAnd,
kBitwiseOr,
kBitwiseXor,
kBitwiseLeftShift,
kBitwiseRightShift,

// prefix operators
kBitwiseNot,
kNegation,

// logical
kLogicalAnd,
kLogicalOr,

// comparison
kEqual,
kNotEqual,
kLessThan,
kGreaterThan,
kLessThanOrEqual,
kGreaterThanOrEqual,
kThreeWayComparison,

// scopes
kOpenParen,
kCloseParen,
kOpenBrace,
kCloseBrace,
kOpenBracket,
kCloseBracket,

// special
kSemicolon,
kColon,
kDoubleColon,
kComma,
kPeriod,
kEllipsis,
kCommericalAt,

// keyword literal values
kNoneLiteral,
kTrueLiteral,
kFalseLiteral,

// keyword types
kAny,
kType,
kIdentity,
kValue,
kInt,
kUint,
kReal,
kByte,
kBit,
kStr,
kList,
kArray,
kPointer,
kMemory,
kFunction,
kAuto,

// preprocessor keywords
kInclude,
kMacro,
kEndmacro,

// directive keywords
kEnter,
kStart,
kUse,
kDef,
kFn,
kClass,
kObj,
kLib,
kMain,
kImport,
kNamespace,
// functional keywords
kPrint,

// modifier tokens
kPrivate,
kPublic,
kConst,
kStatic,
kRef,

// control flow tokens
kIf,
kElse,
kElif,
kWhile,
kFor,
kSwitch,
kCase,
kDefault,
kBreak,
kContinue,
kReturn,

// intermediates
kUndefined,
kStatement,
kExpression,
kDeclaration,

// Generated by a 'use' declaration.
kTypeAlias,
kLibraryTypeAlias,
kLibraryObjectInclusion,
kLibraryNamespaceInclusion,
kNamespaceInclusion,
kNamespaceObjectInclusion,

// Generated by a 'def' declaration.
kVariableDeclaration,
kVariableDefinition,

// Generated by a 'fn' declaration.
kMethodDeclaration,
kMethodDefinition,

// Genenrated a 'class' declaration.
kClassDefinition,
kClassDeclaration,

// Generated by import declaration.
kImportDeclaration,

// Generated by the lib declaration.
kLibraryDeclaration,
kLibraryDefinition,

// Generated by a main declaration.
kMainDeclaration,
kMainDefinition,

// Generated by a method signature.
kMethodVoid,       // void indicates no return type. Or no arguments.
kMethodSignature,  //
kMethodParameter,
kMethodParameterList,  // ()
kMethodReturnType,

// Generated by the if keyword.
kIfStatement,

// inferred abstract nodes (do not perform operations, do not have a literal
// and are not an operand or operator.)
kProgram,
kPragmaticBlock,
kFunctionalBlock,
kConditionalBlock,
kIterativeBlock,

kTypeConstraints,  // []
kCaptureList,      // []

// operands
kSubexpression,  // ()
kTypeList,       // []
kGenericList,    // {}

// operators
kFunctionCall,  // <operand><()>
kArguments,     // <(<...><,>)>

kTypeCall,       // <operand><[]>
kTypeArguments,  // <[<...><,>]>

kIndexOperator,    // <operand><{<...><,>}>
kListingOperator,  // <operand><{<...><,>}>
kIndexArguments,   // <{<...><,>}>
kModifiers,        // Modifiers before a declaration or definition.

kUnaryMinus,   // -
kDereference,  // *
kAddressOf,    // &
};

}  // namespace cand

//---------------------------------------------------------------------------//
// Copyright 2024 Anton Yashchenko
//
// Licensed under the GNU Affero General Public License, Version 3.
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
//---------------------------------------------------------------------------//
// Author(s): Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.acpp.dev
//---------------------------------------------------------------------------//
// Project: C& Programming Language Environment
// Directory: cand-official-compiler
// File: cand_compiler_enum.h
//---------------------------------------------------------------------------//
#endif HEADER_GUARD_CALE_CAND_OFFICIAL_COMPILER_CAOCO_ENUM_H
//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
