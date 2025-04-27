#include "cnd_compiler.hpp"
#include "minitest.hpp"

INLINE_MINITEST(Test_Compiler, TestCase_Return0) {
  // Minimal main function that returns 0 from a single process.
  int argc = 3; 
  char* argv[] = {"cnd", "build", "ut_compiler_Return0.cnd"};
  cnd::compiler::Output cl_out = cnd::compiler::CliMain(argc, argv);
  ASSERT_TRUE(cl_out.exit_code == EXIT_SUCCESS);
  ASSERT_TRUE(cl_out.targets.size() == 1);
  ASSERT_TRUE(cl_out.targets[0].format == cnd::eTargetFormat::kExe);
  auto run_res = wpl::RunExe(cl_out.targets[0].path);
  ASSERT_TRUE(run_res);
  ASSERT_TRUE(run_res.exit_code == 0);
}
INLINE_END_MINITEST MINITEST_REGISTER_CASE(Test_Compiler, TestCase_Return0);

INLINE_MINITEST(Test_Compiler, TestCase_MultiprocessReturn0) {
  // Two seperate processes are produced.
  int argc = 3;
  char* argv[] = {"cnd", "build", "ut_compiler_MultiprocessReturn0.cnd"};
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
INLINE_END_MINITEST MINITEST_REGISTER_CASE(Test_Compiler, TestCase_Return0);

INLINE_MINITEST(Test_Compiler, TestCase_InterprocessReturn0) {
  // One process is evaluated fully at compile time. Second
  // process returns result of the first.
  int argc = 3;
  char* argv[] = {"cnd", "build", "ut_InterprocessReturn0.cnd"};
  cnd::compiler::Output cl_out = cnd::compiler::CliMain(argc, argv);
  ASSERT_TRUE(cl_out.exit_code == EXIT_SUCCESS);
  ASSERT_TRUE(cl_out.targets.size() == 1);
  ASSERT_TRUE(cl_out.targets[0].format == cnd::eTargetFormat::kExe);
  auto run_res = wpl::RunExe(cl_out.targets[0].path);
  ASSERT_TRUE(run_res);
  ASSERT_TRUE(run_res.exit_code == 0); 
}
INLINE_END_MINITEST MINITEST_REGISTER_CASE(Test_Compiler, TestCase_Return0);
