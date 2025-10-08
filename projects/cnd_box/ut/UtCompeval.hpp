///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2025 Anton Yashchenko
// Licensed under the Apache License, Version 2.0(the "License");
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup cnd_unit_test
/// @brief 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup cnd_unit_test
/// @{
#pragma once
// !!Keep clang format OFF for this file ,or else expected ast constructors will be unreadable.
// clang-format off
#include "minitest.hpp"
#include "CliMain.hpp"
// clang-format on

namespace cnd_unit_test::compiler {

TEST(UtCompeval, Return0) {
  int argc = 3;
  char* argv[] = {(char*)("cnd"), (char*)("comp"), (char*)("0-return-zero.cnd")};
  auto cl_out_res = cnd::CliMainInternal(argc, argv,nullptr);
  
  ASSERT_TRUE(cl_out_res);
  ASSERT_TRUE(std::get<cnd::TrOutput>(*cl_out_res).return_value == EXIT_SUCCESS);
  ASSERT_TRUE(std::get<cnd::TrOutput>(*cl_out_res).exit_code == EXIT_SUCCESS);
}

TEST(UtCompeval, HelloWorld) {
  int argc = 3;
  char* argv[] = {(char*)("cnd"), (char*)("comp"), (char*)("test-code/compeval/0-hello-world.cnd")};
  auto cl_out_res = cnd::CliMainInternal(argc, argv, nullptr);

  ASSERT_TRUE(cl_out_res);
  ASSERT_TRUE(std::get<cnd::TrOutput>(*cl_out_res).return_value == 1);
  ASSERT_TRUE(std::get<cnd::TrOutput>(*cl_out_res).exit_code == EXIT_SUCCESS);
}

TEST(UtCompeval, FibSequence) {
  int argc = 3;
  char* argv[] = {(char*)("cnd"), (char*)("comp"), (char*)("test-code/compeval/0-fin-sequence.cnd")};
  auto cl_out_res = cnd::CliMainInternal(argc, argv, nullptr);

  ASSERT_TRUE(cl_out_res);
  ASSERT_TRUE(std::get<cnd::TrOutput>(*cl_out_res).return_value == 1);
  ASSERT_TRUE(std::get<cnd::TrOutput>(*cl_out_res).exit_code == EXIT_SUCCESS);
}

}  // namespace cnd_unit_test::compiler

/// @} // end of cnd_unit_test

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2025 Anton Yashchenko
//
// Licensed under the Apache License, Version 2.0(the "License"); you may not use this file except in compliance with
// the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on
// an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
