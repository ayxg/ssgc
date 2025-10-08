///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2025 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
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
