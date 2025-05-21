///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the Apache License, Version 2.0(the "License");
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
#include "use_ccapi.hpp"
#include "corevals/reflected_meta_enum.hpp"
#include "corevals/enumdefs/grammar/eTk.hpp"
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
  sep m(ParenSubexpr)                                       \
  sep m(SquareSubexpr)                                      \
  sep m(CurlySubexpr)                                       \
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
  sep m(UnaryPlus)\
  sep m(Dereference)                                         \
  sep m(AddressOf)                                           \
  sep m(EnumDeclaration)                                     \
  sep m(EnumDefinition)                                      \
  sep m(EnumAssociation)                                     \
  sep m(EnumBlock)                                           \
  sep m(EnumCategory)                                        \
  sep m(EnumEntry)                                           \
  sep m(UnnamedScope)                                        \
  lst

// Define the enum.
CND_MM_CREATE_ENUM_FROM_APPLIED_ENUM_UNTYPED(eAst, eAst);
// Define the enum to cstr conversion.
CND_MM_CREATE_ENUMTOCSTR_FROM_ENUM_LIST(eAstToCStr, eAst, eAst);
// Assert enum to cstr conversion.
#define CND_STATIC_ASSERT_ENUM_TO_CSTR_eAst(x) CND_MM_STATIC_ASSERT_ENUM_TO_CSTR(x, eAst, eAstToCStr)

CND_APPLIED_ENUM_eAst(CND_STATIC_ASSERT_ENUM_TO_CSTR_eAst, , , );
static_assert(cxx::StrEq(eAstToCStr(eAst::COUNT), "COUNT"));
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
// Licensed under the Apache License, Version 2.0(the "License"); you may not use this file except in compliance with
// the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on
// an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////