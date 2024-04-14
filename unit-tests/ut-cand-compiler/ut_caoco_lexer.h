//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
// Copyright 2024 Anton Yashchenko
// Licensed under the Apache License, Version 2.0(the "License");
//---------------------------------------------------------------------------//
// Author(s): Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.acpp.dev
//---------------------------------------------------------------------------//
// Project: C& Programming Language Environment
// Directory: ut-cand-official-compiler
// File: ut_caoco_lexer.h
//---------------------------------------------------------------------------//
#ifndef HEADER_GUARD_CALE_UT_CAND_OFFICIAL_COMPILER_UT_CAOCO_LEXER_H
#define HEADER_GUARD_CALE_UT_CAND_OFFICIAL_COMPILER_UT_CAOCO_LEXER_H
//---------------------------------------------------------------------------//
// Brief: brief
//---------------------------------------------------------------------------//
// Includes:
#include "cppsextended.h"
#include "minitest.h"
// Testing:
#include "caoco_lexer.h"
//---------------------------------------------------------------------------//

INLINE_MINITEST(Test_Lexer, TestCase_Keywords) {
  using namespace caoco;
  auto keywords = Lexer::Lex(
      "use class private "
      "public const static if else elif while for switch "
      "break continue return none int uint real "
      "byte bit str array"
      "def auto fn default any case");

  TkVector expected_test_result = TkVector(
      {{eTk::kUse},   {eTk::kClass},
       {eTk::kPrivate}, {eTk::kPublic},      {eTk::kConst}, {eTk::kStatic},
       {eTk::kIf},      {eTk::kElse},        {eTk::kElif},  {eTk::kWhile},
       {eTk::kFor},     {eTk::kSwitch},      {eTk::kBreak}, {eTk::kContinue},
       {eTk::kReturn},  {eTk::kNoneLiteral}, {eTk::kInt},   {eTk::kUint},
       {eTk::kReal},    {eTk::kByte},        {eTk::kBit},   {eTk::kStr},
       {eTk::kArray},   {eTk::kDef},         {eTk::kAuto},  {eTk::kFn},
       {eTk::kDefault}, {eTk::kAny},         {eTk::kCase}});

  EXPECT_TRUE(keywords.Valid());

  if (keywords.Valid()) {
    ASSERT_EQ(keywords.Value().size(), expected_test_result.size());
    TkVector result = keywords.Extract();
    for (size_t i = 0; i < result.size(); ++i) {
      EXPECT_EQ(result[i].Type(), expected_test_result[i].Type());
    }
  } 
}
INLINE_END_MINITEST;
MINITEST_REGISTER_CASE(Test_Lexer, TestCase_Keywords);

//---------------------------------------------------------------------------//
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
//---------------------------------------------------------------------------//
// Author(s): Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.acpp.dev
//---------------------------------------------------------------------------//
// Project: C& Programming Language Environment
// Directory: ut-cand-official-compiler
// File: ut_caoco_lexer.h
//---------------------------------------------------------------------------//
#endif HEADER_GUARD_CALE_UT_CAND_OFFICIAL_COMPILER_UT_CAOCO_LEXER_H
//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//