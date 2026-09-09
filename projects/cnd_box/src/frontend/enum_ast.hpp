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
#include "../common/meta_applied_enum.hpp"
#include "enum_token.hpp"
// clang-format on

/// @addtogroup cnd_compiler_corevals
/// @{

namespace ssgc::frontend {

/// @note all of eTk is part of eAst appended at the front.
#ifndef SSGC_AppliedEnum_eToken
#error Macro symbol ''SSGC_AppliedEnum_eToken' from file 'frontend/enum_token.hpp' must be available.
#endif

/// Define applied enum cnd::corevals::grammar::eAst
#define SSGC_AppliedEnum_eAst(m, sep, pre, lst)                 \
  SSGC_AppliedEnum_eToken(m, sep, pre, ) sep m(kUndefinedObject) \
  sep m(kPoison)\
  sep m(kSyntax)\
  sep m(kSourceFile)\
  sep m(kResolutionOp)                                           \
  sep m(kRootNamespace)                                          \
  sep m(kMemberAccess)                                           \
  sep m(kParenSubexpr)                                           \
  sep m(kSquareSubexpr)                                          \
  sep m(kCurlySubexpr)                                           \
  sep m(kStatement)                                              \
  sep m(kExpression)                                             \
  sep m(kDeclaration)                                            \
  sep m(kTypeAlias)                                              \
  sep m(kLibraryTypeAlias)                                       \
  sep m(kLibraryObjectInclusion)                                 \
  sep m(kLibraryNamespaceInclusion)                              \
  sep m(kNamespaceInclusion)                                     \
  sep m(kNamespaceObjectInclusion)                               \
  sep m(kVariableDeclaration)                                    \
  sep m(kVariableDefinition)                                     \
  sep m(kMethodDeclaration)                                      \
  sep m(kMethodDefinition)                                       \
  sep m(kClassDefinition)                                        \
  sep m(kClassDeclaration)                                       \
  sep m(kImportDeclaration)                                      \
  sep m(kProcessDeclaration)                                     \
  sep m(kProcessDefinition)                                      \
  sep m(kLibraryDeclaration)                                     \
  sep m(kLibraryDefinition)                                      \
  sep m(kMainDeclaration)                                        \
  sep m(kMainDefinition)                                         \
  sep m(kMethodVoid)                                             \
  sep m(kMethodSignature)                                        \
  sep m(kMethodParameter)                                        \
  sep m(kMethodParameterList)                                    \
  sep m(kMethodReturnType)                                       \
  sep m(kIfStatement)                                            \
  sep m(kProgram)                                                \
  sep m(kPragmaticBlock)                                         \
  sep m(kFunctionalBlock)                                        \
  sep m(kConditionalBlock)                                       \
  sep m(kIterativeBlock)                                         \
  sep m(kTypeConstraints)                                        \
  sep m(kCaptureList)                                            \
  sep m(kSubexpression)                                          \
  sep m(kTypeList)                                               \
  sep m(kGenericList)                                            \
  sep m(kFunctionCall)                                           \
  sep m(kArguments)                                              \
  sep m(kTypeCall)                                               \
  sep m(kTypeArguments)                                          \
  sep m(kIndexOperator)                                          \
  sep m(kListingOperator)                                        \
  sep m(kIndexArguments)                                         \
  sep m(kModifiers)                                              \
  sep m(kUnaryMinus)                                             \
  sep m(kUnaryPlus)                                              \
  sep m(kDereference)                                            \
  sep m(kAddressOf)                                              \
  sep m(kPointerMemberAccess)                                    \
  sep m(kEnumDeclaration)                                        \
  sep m(kEnumDefinition)                                         \
  sep m(kEnumAssociation)                                        \
  sep m(kEnumBlock)                                              \
  sep m(kEnumCategory)                                           \
  sep m(kEnumEntry)                                              \
  sep m(kIncludeSystemStmt)                                      \
  sep m(kIncludeLocalStmt)                                       \
  sep m(kIQCharSeuquence)                                        \
  sep m(kPFCharSequence)                                         \
  sep m(kIACharSeuquence)                                        \
  sep m(kPragmaEscapedStmt)                                      \
  sep m(kPragmaFunctionalStmt)                                   \
  sep m(kPragmaNamedStmt)                                        \
  sep m(kNamespaceDecl)                                          \
  sep m(kUnnamedScope)                                           \
  lst

SSGC_MACRO_DefineEnumFromAppliedList(SSGC_AppliedEnum_eAst, eAst);

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