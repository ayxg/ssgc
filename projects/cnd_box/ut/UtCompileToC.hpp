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

void TestCompileToC(const std::vector<std::string>& cli_args, const std::vector<std::string_view>& expected_output) {
  std::vector<char*> argv{};
  argv.reserve(cli_args.size() + 1);
  for (auto& s : cli_args) argv.push_back(&s[0]);
  cnd::CompilerProcessResult<std::variant<int, cnd::TrOutput>> cl_out_res =
      cnd::CliMainInternal(argv.size(), argv, nullptr);

  ASSERT_TRUE(cl_out_res);                                                         // Compiler process result is valid.
  ASSERT_TRUE(std::get<cnd::TrOutput>(*cl_out_res).return_value == EXIT_SUCCESS);  // VM Interpreter result is 0.
  ASSERT_TRUE(std::get<cnd::TrOutput>(*cl_out_res).exit_code == EXIT_SUCCESS);     // Compiler's exit code is 0.
  

  ASSERT_TRUE(std::get<cnd::TrOutput>(*cl_out_res).output_files.size() >= expected_output.size()); // Output contains source files.
  for (size_t i = 0; i < expected_output.size(); ++i) {
    ASSERT_TRUE(std::filesystem::exists(std::get<cnd::TrOutput>(*cl_out_res).output_files[i])); // Output file exists.
    // Load the output file and validate against expected output.
    std::string output_file = cxx::ReadFile(std::get<cnd::TrOutput>(*cl_out_res).output_files[i]);
    ASSERT_TRUE(output_file == expected_output[i]);
  }
}

TEST(UtCompileToC, Return0) {
  TestCompileToC(
      {"cnd", "comp", "test-code/runtime/0-ReturnZero.cnd"},
      {cxx::Dedent(R"(
        #define __cnd__tn__i32 int
        #define __cnd__cf__return return

        __cnd__tn__i32 __cnd__fn__main() {
          __cnd__cf__return 0;
        }
      )")});
}

TEST(UtCompileToC, Return0) {
  TestCompileToC({"cnd", "comp", "test-code/runtime/1.0-IntAdd.cnd"}, {cxx::Dedent(R"(
        #define __cnd__tn__i32 int
        #define __cnd__cf__return return

        __cnd__tn__i32 __cnd__fn__main() {
          __cnd__tn__i32 __cnd__def__a = 42;
          __cnd__tn__i32 __cnd__def__b = 42;
          __cnd__cf__return __cnd__def__a + __cnd__def__b;
        }
      )")});
}

TEST(UtCompilerCli, Return0) {
  int argc = 3;
  char* argv[] = {(char*)("cnd"), (char*)("comp"), (char*)("0-return-zero.cnd")};
  auto cl_out_res = cnd::CliMainInternal(argc, argv,nullptr);
  
  ASSERT_TRUE(cl_out_res);
  ASSERT_TRUE(std::get<cnd::TrOutput>(*cl_out_res).return_value == EXIT_SUCCESS);
  ASSERT_TRUE(std::get<cnd::TrOutput>(*cl_out_res).exit_code == EXIT_SUCCESS);
}

TEST(UtCompilerCli, HelloWorld) {
  int argc = 3;
  char* argv[] = {(char*)("cnd"), (char*)("comp"), (char*)("test-code/compeval/0-hello-world.cnd")};
  auto cl_out_res = cnd::CliMainInternal(argc, argv, nullptr);

  ASSERT_TRUE(cl_out_res);
  ASSERT_TRUE(std::get<cnd::TrOutput>(*cl_out_res).return_value == 1);
  ASSERT_TRUE(std::get<cnd::TrOutput>(*cl_out_res).exit_code == EXIT_SUCCESS);
}

TEST(UtCompilerCli, FibSequence) {
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
