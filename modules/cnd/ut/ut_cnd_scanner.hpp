//#include "deprecated/cnd_scanner.hpp"
//#include "minitest.hpp"
//
//INLINE_MINITEST(Test_Scanner, TestCase_LoadMultibyteFile) {
//  auto loaded_src =
//      cnd::scanner::ReadSourceFile("ut_scanner_LoadMultibyteFile.cnd");
//  EXPECT_TRUE(loaded_src.has_value());
//  EXPECT_EQ(cnd::Str(loaded_src.value().begin(), loaded_src.value().end()),
//            "123456789qwerty");
//}
//INLINE_END_MINITEST;
//MINITEST_REGISTER_CASE(Test_Scanner, TestCase_LoadMultibyteFile);
//
//INLINE_MINITEST(Test_Scanner, TestCase_SplitSourceLines) {
//  auto loaded_src =
//      cnd::scanner::ReadSourceFile("ut_scanner_SplitSourceLines.cnd");
//  EXPECT_TRUE(loaded_src.has_value());
//
//  auto src_lines = cnd::scanner::SplitSourceLines(loaded_src.value());
//
//  EXPECT_EQ(src_lines.size(), 4);
//}
//INLINE_END_MINITEST;
//MINITEST_REGISTER_CASE(Test_Scanner, TestCase_SplitSourceLines);
