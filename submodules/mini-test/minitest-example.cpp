//// Include the minitest.h file to use the minimal unit testing framework
//#include "minitest.h"  // Minimal Unit Testing Framework
//
////=-------------------------------------------------------------------------=//
//// Project Includes
////---------------------------------------------------------------------------//
//// #include "parser.h" // all the headers of your project to test...
//#include <iostream>  // for example
//
////=-------------------------------------------------------------------------=//
//// Unit Tests Recommended Conventions
////---------------------------------------------------------------------------//
//// 0. Create few headers to organize the unit test depndencies and
//// configurations.
////
//// #include "minitest_flags.h"  // Flags to enable or disable the unit tests
//// #include "minitest_pch.h"    // All pre includes/dependencies for all unit
//// tests
////                             // eg. standard library
//// #include "minitest_util.h"   // Utility methods shared among the all unit
//// tests
////                             // seperate from your project's files.
//// 1. All unit test files should be named as utX_testModuleName.h
////      where X is the iteration of the unit test module.
////
//// 2. All unit test files should include the following headers
////        #include "minitest_flags.h" // Flags to enable or disable the unit
////        tests #include "minitest_pch.h" // All pre includes for each unit test
////        #include "minitest_util.h" // Utility methods shared among the all
////        unit tests
////
//// 3. Unit test headers must NOT EVER include another unit test 'utX_' header.
////    or have dependencies on other unit tests.
////
//// 4. Unit tests auto-register.
////    To disable and enable a test create a flag in minitest_flags.h.
////    The format should be for example unit test of parser_utils.h:
////      1.Inside minitest_flags.h:
////          #define CAOCO_UNIT_TEST0_PARSER_UTILS 1
////      2.Inside your unit test file:
////          #if CAOCO_UNIT_TEST0_PARSER_UTILS
////          #define CAOCO_UNIT_TEST0_PARSER_UTILS_BasicScopeFinder 1
////          #define CAOCO_UNIT_TEST0_PARSER_UTILS_ListScopeFinder 1
////          #define CAOCO_UNIT_TEST0_PARSER_UTILS_FrameScopeFinder 1
////          #define CAOCO_UNIT_TEST0_PARSER_UTILS_StatementScopeFinder 1
////          #endif
////    Surround each unit test with #if #endif.
//// 5. Provide detailed description of the unit test above the include.
////        - Provide the macro to enable or disable the unit test.
////        - Provide the sub-macros to enable or disable the sub-tests.
////        - Provide info on which header file is being tested.
////        - Provide a brief description of the unit test.
////    For example:
////        // Unit Test: parser_utils.h
////        // Header: parser_utils.h
////        // Description: testing scope finders in parser_utils.h
////        // Macros:
////            1.CAOCO_UNIT_TEST0_PARSER_UTILS
////              1.1.CAOCO_UNIT_TEST0_PARSER_UTILS_BasicScopeFinder
////              1.2.CAOCO_UNIT_TEST0_PARSER_UTILS_ListScopeFinder
////              1.3.CAOCO_UNIT_TEST0_PARSER_UTILS_FrameScopeFinder
////              1.4.CAOCO_UNIT_TEST0_PARSER_UTILS_StatementScopeFinder
//
////=-------------------------------------------------------------------------=//
//// Unit Test Includes
////---------------------------------------------------------------------------//
//// #include "ut0_parser_utils.h"
//
//// Example of a unit test.
//// NOTE: the braces{} are optional but recommended for better readability.
//auto my_method() { return true; }
//MINITEST(MyTest,MyTestCase){
//  EXPECT_TRUE(my_method());
//  EXPECT_TRUE(false);
//  EXPECT_FALSE(false);
//  EXPECT_FALSE(true);
//  EXPECT_EQ(1,1);
//  EXPECT_EQ(1,2);
//  EXPECT_NE(1,2);
//  EXPECT_NE(1,1);
//  EXPECT_ANY_THROW([](){ throw std::runtime_error("error"); });
//  EXPECT_ANY_THROW([](){});
//  EXPECT_NO_THROW([](){});
//  EXPECT_NO_THROW([](){ throw std::runtime_error("error"); });
//}
//END_MINITEST;
//
//// Example of a fixture.
//struct MyFixture : minitest::Fixture {
//  int my_foo = 0;
//  void SetUp() override {
//    std::cout << "Setting up the fixture\n";
//  }
//
//  void TearDown() override {
//    std::cout << "Tearing down the fixture\n";
//  }
//};
//
//// Example of a fixture unit test.
//MINITEST_F(MyFixtureTest, MyFixtureTestCase, MyFixture) {
//  EXPECT_TRUE(my_foo == 0);
//  EXPECT_TRUE(my_foo != 1);
//  EXPECT_FALSE(my_foo == 1);
//  EXPECT_FALSE(my_foo != 0);
//  EXPECT_EQ(my_foo,0);
//  EXPECT_EQ(my_foo,1);
//  EXPECT_NE(my_foo,1);
//  EXPECT_NE(my_foo,0);
//  EXPECT_ANY_THROW([](){ throw std::runtime_error("error"); });
//  EXPECT_ANY_THROW([](){});
//  EXPECT_NO_THROW([](){});
//  EXPECT_NO_THROW([](){ throw std::runtime_error("error"); });
//}
//END_MINITEST_F(MyFixtureTestCase);
//
//FINISH_MINITESTS;  // Macro to finish the test suite call before main function.
//// Undefine all the minitest macros except MINITESTS_RESULT
//// Not necessary but recommended.
//#undef MINITEST
//#undef END_MINITEST
//#undef FINISH_MINITESTS
//#undef EXPECT_TRUE
//#undef EXPECT_FALSE
//#undef EXPECT_EQ
//#undef EXPECT_NE
//#undef EXPECT_ANY_THROW
//#undef EXPECT_NO_THROW
//#undef ASSERT_TRUE
//#undef ASSERT_FALSE
//#undef ASSERT_EQ
//#undef ASSERT_NE
//#undef ASSERT_ANY_THROW
//#undef ASSERT_NO_THROW
//#undef EXPECT_TRUE_LOG
//#undef EXPECT_FALSE_LOG
//#undef EXPECT_EQ_LOG
//#undef EXPECT_NE_LOG
//#undef EXPECT_ANY_THROW_LOG
//#undef EXPECT_NO_THROW_LOG
//#undef ASSERT_TRUE_LOG
//#undef ASSERT_FALSE_LOG
//#undef ASSERT_EQ_LOG
//#undef ASSERT_NE_LOG
//#undef ASSERT_ANY_THROW_LOG
//#undef ASSERT_NO_THROW_LOG
////---------------------------------------------------------------------------//
////=-------------------------------------------------------------------------=//
//
//// Main Function is unaffected by the unit tests.
//int main() {
//  std::cout << "Hello World!\n";
//
//  if (MINITESTS_RESULT)
//    std::cout << "Testing was successful.\n";
//  else
//    std::cout << "A test case failed.\n";
//
//  return MINITESTS_RESULT ? 0 : 1; // example of return result based on tests.
//}
