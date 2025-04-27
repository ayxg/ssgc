///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the Apache License, Version 2.0(the "License");
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup
/// @brief
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup cnd_compiler
/// @{
#pragma once
// clang-format off
#include "cnd_implicit.hpp"
#include "cnd_constdef.hpp"
#include "cnd_clerr.hpp"

#include "cnd_tk.hpp"
#include "cnd_cst.hpp"
#include "cnd_tk_closure.hpp"
#include "cnd_tk_cursor.hpp"
#include "cnd_translation_unit.hpp"
//#include "cnd_tk_scope.hpp"
// clang-format on

namespace cnd {
// Concrete AST Parser
namespace parser {
using clerr::ClErr;
using clerr::ClRes;
using clerr::Fail;
using clerr::MakeClErr;

using TkVec = Vec<Tk>;
using OffsetParseResult = Pair<TkCursor, Cst>;
using ParseResult = ClRes<OffsetParseResult>;

class ExprParser;

ParseResult ParseOperand(TkCursor c);
ParseResult ParseArguments(TkCursor current);
ParseResult ParseIndexingArguments(TkCursor current);
ParseResult ParseListingArguments(TkCursor current);
ParseResult ParsePrimaryStatement(TkCursor current);
ParseResult ParseConditionalSubExpression(TkCursor c);
ParseResult ParsePrimaryPreIdentifier(TkCursor current);
ParseResult ParsePrimaryPostIdentifier(TkCursor current);
ParseResult ParseModifiers(TkCursor current);

ParseResult ParseReturnStmt(TkCursor current);
ParseResult ParseMethodParameters(TkCursor current);
ParseResult ParseMethodReturnParameters(TkCursor current);
ParseResult ParseMethodSignature(TkCursor current);
ParseResult ParseMethodDef(TkCursor current);
ParseResult ParseMainDef(TkCursor current);
ParseResult ParseClassDef(TkCursor current);
ParseResult ParseLibDef(TkCursor current);
ParseResult ParsePragmaticStmt(TkCursor current);
ParseResult ParseFunctionalStmt(TkCursor current);
ParseResult ParseConditionalStmt(TkCursor c);
ParseResult ParseIfDecl(TkCursor c);
ParseResult ParseMainDecl(TkCursor current);
ParseResult ParseLibDecl(TkCursor current);
ParseResult ParseImportDecl(TkCursor current);
ParseResult ParseWhileDecl(TkCursor c);
ParseResult ParseForDecl(TkCursor c);
ParseResult ParseUsingDecl(TkCursor current);
ParseResult ParseVariableDecl(TkCursor current);
ParseResult ParseMethodDecl(TkCursor current);
ParseResult ParseClassDecl(TkCursor current);
ParseResult ParseEnumDecl(TkCursor current);
ParseResult ParseEnumDef(TkCursor current);
ParseResult ParseEnumBlock(TkCursor current);
ParseResult ParseProgram(TkCursor current);
ParseResult ParseTranslationUnit(cnd::compiler::TranslationFragment tr_frag, TkCursor tokens);

class ExprParser {
  using TkListT = ClosureBuffer::TkListT;
  using TkListIterT = ClosureBuffer::TkListIterT;
  using CosureListT = ClosureBuffer::CosureListT;
  using ClosureListIterT = ClosureBuffer::ClosureListIterT;

 public:
  static ClRes<Cst> Perform(const TkCursor & c);
  ClRes<Cst> Parse(const TkCursor& c);

 private:
  void FirstOperatorSwitch();
  void ResolvePrefix();
  void ResolvePostfix();
  void ResolveBinaryLeftAssoc();
  void ResolveBinaryRightAssoc();
  void ResolveBinary();
  void ResolveLast();
  ClRes<Bool> ActionSkip(TkCursor& c);
  ClRes<Bool> ActionStore(TkCursor& c);
  ClRes<Bool> ActionCheck(TkCursor& c);
  ClRes<Bool> ChooseAction(TkCursor& c);
  ClRes<TkVec> CreateAndResolveClosures(TkCursor c);
  ClRes<Cst> ParseImpl(TkCursor c);

 private:
  enum class eNextExpectedHeadToken { kOperative, kOperator, kNone };
  static const Tk kOpenParenTk;
  static const Tk kCloseParenTk;
  /// Expected token type on next iteration.
  eNextExpectedHeadToken next_expected_head_token_ = {eNextExpectedHeadToken::kOperative};
  ClosureBuffer closure_buffer_;
  Bool is_first_operator_ = {true};
  Bool is_resolved_ = {false};
};

}  // namespace parser
}  // namespace cnd

/// @} // end of cnd_compiler

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////