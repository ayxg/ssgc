#include "trtools/parser.hpp"
#include "minitest.hpp"

INLINE_MINITEST(Test_Lexer, TestCase_Minimum) {
  
  auto loaded_src = cnd::scanner::ReadSourceFile("ut_compiler_Return0.cnd");
  EXPECT_TRUE(loaded_src.has_value());

  auto src_lines = cnd::scanner::SplitSourceLines(loaded_src.value());

  auto lex_res = cnd::lexer::Lex(src_lines.value());


}
INLINE_END_MINITEST;
MINITEST_REGISTER_CASE(Test_Lexer, TestCase_Keywords);
