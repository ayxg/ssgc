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
      "def class fn import main namespace using lib dll if elif else cxif "
      "cxelif cxelse switch case default while for return break continue int "
      "uint real bool char byte cstr str ptr list array true false none void "
      "in as cin cout native const ref private public static any auto type "
      "value template");
  using enum caoco::eTk;
  TkVector expected_test_result = TkVector(
      {KwDef,    KwClass,  KwFn,     KwImport,  KwMain,     KwNamespace,
       KwUse,    KwLib,    KwDll,    KwIf,      KwElif,     KwElse,
       KwCxif,   KwCxelif, KwCxelse, KwSwitch,  KwCase,     KwDefault,
       KwWhile,  KwFor,    KwReturn, KwBreak,   KwContinue, KwInt,
       KwUint,   KwReal,   KwBool,   KwChar,    KwByte,     KwCstr,
       KwStr,    KwPtr,    KwList,   KwArray,   KwTrue,     KwFalse,
       KwNone,   KwVoid,   KwIn,     KwAs,      KwCin,      KwCout,
       KwNative, KwConst,  KwRef,    KwPrivate, KwPublic,   KwStatic,
       KwAny,    KwAuto,   KwType,   KwValue,   KwTemplate});

  EXPECT_TRUE(keywords.Valid());

  if (keywords.Valid()) {
    ASSERT_EQ(keywords.Value().size(), expected_test_result.size());
    TkVector result = keywords.Extract();
    for (size_t i = 0; i < result.size(); ++i) {
      if (result[i].Type() != expected_test_result[i].Type())
        std::cout << "Expected: " << expected_test_result[i].TypeStr().data()
                  << " Got: " << result[i].TypeStr().data() << std::endl;
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