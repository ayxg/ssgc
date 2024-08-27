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
/// @brief Includes all unit tests for the C& compiler.
///////////////////////////////////////////////////////////////////////////////

/// @addtogroup unittest1_cand_compiler
/// @{
#ifndef HEADER_GUARD_CAOCO_UNIT_TESTS_UT_CAND_COMPILER_H
#define HEADER_GUARD_CAOCO_UNIT_TESTS_UT_CAND_COMPILER_H
#include "ut_caoco_lexer.h"
#include "ut_caoco_token_scope.h"
#include "ut_caoco_parser_basic.h"
#include "ut_caoco_build.h"

namespace ut {
  static void UnitTestCandCompiler() { 
    MINITEST_RUN_REGISTERED_MODULE(Test_Lexer);
    MINITEST_RUN_REGISTERED_MODULE(Test_TkScope);
    MINITEST_RUN_REGISTERED_MODULE(Test_ParserBasics);
    MINITEST_RUN_REGISTERED_MODULE(Test_Build);
  }
}

#endif HEADER_GUARD_CAOCO_UNIT_TESTS_UT_CAND_COMPILER_H
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
