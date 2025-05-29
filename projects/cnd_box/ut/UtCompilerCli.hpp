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
/// @brief [UtCompilerCli] Test calling compiler from a user's perspective by using a built executable ssgc cli app.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup cnd_unit_test
/// @{
#pragma once
// !!Keep clang format OFF for this file ,or else expected ast constructors will be unreadable.
// clang-format off
#include "minitest.hpp"
#include "cnd_compiler.hpp"
// clang-format on

namespace cnd_unit_test::compiler {

TEST(UtCompilerCli, Return0) {
  // Minimal main function that returns 0 from a single process.
  int argc = 3;
  char* argv[] = {"cnd", "build", "UtCompilerReturn0.cnd"};
  cnd::compiler::Output cl_out = cnd::compiler::CliMain(argc, argv);
  ASSERT_TRUE(cl_out.exit_code == EXIT_SUCCESS);
  ASSERT_TRUE(cl_out.targets.size() == 1);
  ASSERT_TRUE(cl_out.targets[0].format == cnd::eTargetFormat::kExe);
  auto run_res = wpl::RunExe(cl_out.targets[0].path);
  ASSERT_TRUE(run_res);
  ASSERT_TRUE(run_res.exit_code == 0);
}

TEST(UtCompilerCli, MultiprocessReturn0) {
  // Two seperate processes are produced.
  int argc = 3;
  char* argv[] = {"cnd", "build", "UtCompilerMultiprocessReturn0.cnd"};
  cnd::compiler::Output cl_out = cnd::compiler::CliMain(argc, argv);
  ASSERT_TRUE(cl_out.exit_code == EXIT_SUCCESS);
  ASSERT_TRUE(cl_out.targets.size() == 2);
  ASSERT_TRUE(cl_out.targets[0].format == cnd::eTargetFormat::kExe);
  auto run_res = wpl::RunExe(cl_out.targets[0].path);
  ASSERT_TRUE(run_res);
  ASSERT_TRUE(run_res.exit_code == 0);
  ASSERT_TRUE(cl_out.targets[1].format == cnd::eTargetFormat::kExe);
  auto run_res = wpl::RunExe(cl_out.targets[1].path);
  ASSERT_TRUE(run_res);
  ASSERT_TRUE(run_res.exit_code == 0);
}

TEST(UtCompilerCli, InterprocessReturn0) {
  // One process is evaluated fully at compile time. Second
  // process returns result of the first.
  int argc = 3;
  char* argv[] = {"cnd", "build", "UtCompilerInterprocessReturn0.cnd"};
  cnd::compiler::Output cl_out = cnd::compiler::CliMain(argc, argv);
  ASSERT_TRUE(cl_out.exit_code == EXIT_SUCCESS);
  ASSERT_TRUE(cl_out.targets.size() == 1);
  ASSERT_TRUE(cl_out.targets[0].format == cnd::eTargetFormat::kExe);
  auto run_res = wpl::RunExe(cl_out.targets[0].path);
  ASSERT_TRUE(run_res);
  ASSERT_TRUE(run_res.exit_code == 0);
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
