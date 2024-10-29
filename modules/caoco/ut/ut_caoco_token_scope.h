///////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the Apache License, Version 2.0(the "License");
///////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language Environment
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup unittest1_cand_compiler
/// @brief UT C& Token Scope Method
///////////////////////////////////////////////////////////////////////////////

/// @addtogroup unittest1_cand_compiler
/// @{
#ifndef HEADER_GUARD_CALE_UT_CAND_OFFICIAL_COMPILER_UT_TOKEN_SCOPE_H
#define HEADER_GUARD_CALE_UT_CAND_OFFICIAL_COMPILER_UT_TOKEN_SCOPE_H

// Includes:
#include "cxxx.hpp"
#include "minitest.hpp"
// Testing:
#include "caoco_lexer.h"
#include "caoco_token_scope.h"

INLINE_MINITEST(Test_TkScope, TestCase_ParenScopeFinder) {
  using namespace caoco;
  // Assumes no Error with lexing can occur.
  auto test_source = Lexer::Lex(
                         "()\n"
                         "(1)\n"
                         "((1))\n"
                         "(1 + (a) - (a+(abc+(a+a+a))))\n"
                         "(1 + (a) - (a+{abc+(a+a+a)}))\n"
                         "(1 + (a) - [(a)+{abc+(a+a+a)}])\n"
                         "(() // invalid scope\n\0")
                         .Extract();

  TkScope empty_scope =
      TkScope::FindParen(test_source.cbegin(), test_source.cend());
  TkScope scope_with_1_element =
      TkScope::FindParen(empty_scope.End(), test_source.cend());
  TkScope double_scope =
      TkScope::FindParen(scope_with_1_element.End(), test_source.cend());
  TkScope complex_scope =
      TkScope::FindParen(double_scope.End(), test_source.cend());
  TkScope complex_scope_with_lists =
      TkScope::FindParen(complex_scope.End(), test_source.cend());
  TkScope complex_scope_with_frames_and_lists =
      TkScope::FindParen(complex_scope_with_lists.End(), test_source.cend());

  EXPECT_TRUE_LOG(complex_scope_with_frames_and_lists.Valid(),
                  "Testing complex scope with frames and lists:",
                  "Failed to find complex scope with frames and lists");
  EXPECT_TRUE_LOG(complex_scope_with_lists.Valid(),
                  "Testing complex scope with lists:",
                  "Failed to find complex scope with lists");
  EXPECT_TRUE_LOG(complex_scope.Valid(),
                  "Testing complex scope:", "Failed to find complex scope");
  EXPECT_TRUE_LOG(double_scope.Valid(),
                  "Testing double scope:", "Failed to find double scope");
  EXPECT_TRUE_LOG(scope_with_1_element.Valid(), "Testing scope with 1 element",
                  "Failed to find scope with 1 element");
  EXPECT_TRUE_LOG(empty_scope.Valid(), "Testing empty scope",
                  "Failed to find empty scope ");

  // Invalid scope should be invalid
  auto test_source2 = Lexer::Lex("(()").Extract();

  TkScope invalid_list =
      TkScope::FindParen(test_source2.cbegin(), test_source2.cend());
  EXPECT_FALSE_LOG(invalid_list.Valid(),
                   "Testing invalid scope:", "Invalid scope was valid!");
}
INLINE_END_MINITEST;
MINITEST_REGISTER_CASE(Test_TkScope, TestCase_ParenScopeFinder);

INLINE_MINITEST(Test_TkScope, TestCase_ListScopeFinder) {
  using namespace caoco;
  auto test_source = Lexer::Lex("{}{a}{{}}{({})[{}]{}}{{}");
  auto result = test_source.Extract();
  // empty list
  std::cout << "Testing empty list:";
  TkScope empty_list = TkScope::FindBrace(result.cbegin(), result.cend());
  EXPECT_TRUE(empty_list.Valid());
  TkScope list_with_1_element =
      TkScope::FindBrace(empty_list.End(), result.cend());
  EXPECT_TRUE(list_with_1_element.Valid());
  TkScope double_list =
      TkScope::FindBrace(list_with_1_element.End(), result.cend());
  EXPECT_TRUE(double_list.Valid());
  TkScope complex_list = TkScope::FindBrace(double_list.End(), result.cend());
  EXPECT_TRUE(complex_list.Valid());
  TkScope invalid_list = TkScope::FindBrace(complex_list.End(), result.cend());
  EXPECT_FALSE(invalid_list.Valid());
}
INLINE_END_MINITEST;
MINITEST_REGISTER_CASE(Test_TkScope, TestCase_ListScopeFinder);

INLINE_MINITEST(Test_TkScope, TestCase_FrameScopeFinder) {
  using namespace caoco;
  auto test_source = Lexer::Lex("[][a][[]][([])[[]][]][[]");
  auto result = test_source.Extract();

  TkScope empty_frame = TkScope::FindBracket(result.cbegin(), result.cend());
  EXPECT_TRUE(empty_frame.Valid());
  TkScope frame_with_1_element =
      TkScope::FindBracket(empty_frame.End(), result.cend());
  EXPECT_TRUE(frame_with_1_element.Valid());
  TkScope double_frame =
      TkScope::FindBracket(frame_with_1_element.End(), result.cend());
  EXPECT_TRUE(double_frame.Valid());
  TkScope complex_frame =
      TkScope::FindBracket(double_frame.End(), result.cend());
  EXPECT_TRUE(complex_frame.Valid());
  TkScope invalid_frame =
      TkScope::FindBracket(complex_frame.End(), result.cend());
  EXPECT_FALSE(invalid_frame.Valid());
}
INLINE_END_MINITEST;
MINITEST_REGISTER_CASE(Test_TkScope, TestCase_FrameScopeFinder);

INLINE_MINITEST(Test_TkScope, TestCase_StatementScopeFinder) {
  using namespace caoco;
  auto source_file = Lexer::Lex(
      "1;\n"
      "a = 1;\n"
      "a = (1;2;3);\n"
      "a = 1 + ([ 2 ;3 + {4;5;6}]);\n"
      "a = a + a + ([ a ;a + {a;a;a}]);\n");
  auto result = source_file.Extract();

  // Single value statement : 1;
  TkScope empty_statement = TkScope::FindStatement(
      eTk::LitInt, eTk::Semicolon, result.cbegin(), result.cend());
  EXPECT_TRUE(empty_statement.Valid());
  // statement with multiple tokens: a = 1;
  TkScope multiple_token_statement = TkScope::FindStatement(
      eTk::Ident, eTk::Semicolon, empty_statement.End(), result.cend());
  EXPECT_TRUE(multiple_token_statement.Valid());
  // statement with multiple tokens and scopes: a = (1;2;3);
  TkScope multiple_token_scope_statement =
      TkScope::FindStatement(eTk::Ident, eTk::Semicolon,
                             multiple_token_statement.End(), result.cend());
  EXPECT_TRUE(multiple_token_scope_statement.Valid());
  // statement with lists frames and scopes nested in diffrent ways containing
  // end tokens. a = 1 + ([ 2 ;3 + {4;5;6}]);
  TkScope complex_statement = TkScope::FindStatement(
      eTk::Ident, eTk::Semicolon, multiple_token_scope_statement.End(),
      result.cend());
  EXPECT_TRUE(complex_statement.Valid());
  // Test finding an "open" statement which allows for repeated open tokens. ex
  // a = a + a + ([ a ;a + {a;a;a}]);
  TkScope open_statement = TkScope::FindOpenStatement(
      eTk::Ident, eTk::Semicolon,
                             complex_statement.End(), result.cend());
  EXPECT_TRUE(open_statement.Valid());
  EXPECT_TRUE(open_statement.End() == result.cend());
}
INLINE_END_MINITEST;
MINITEST_REGISTER_CASE(Test_TkScope, TestCase_StatementScopeFinder);

#endif HEADER_GUARD_CALE_UT_CAND_OFFICIAL_COMPILER_UT_TOKEN_SCOPE_H
/// @} // end of unittest1_cand_compiler
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
