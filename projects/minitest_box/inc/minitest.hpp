///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2025 Anton Yashchenko
// Licensed under the Apache License, Version 2.0(the "License");
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: Minitest Framework
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup minitest
/// @brief Minitest framework's public api. Only methods and macros listed in
///        this file should be used by users.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup minitest
/// @{
#pragma once
#include "test_framework.hpp"

// clang-format off

/////////////////////////////////////////////////////////////////////////////////
///* Checks and Asserts */
/////////////////////////////////////////////////////////////////////////////////

/// @addtogroup minitest0_macro_api1_checks_and_asserts
/// @{

#define EXPECT_TRUE(x) minitest::gFramework.ExpectTrue(!!(x), #x)
#define EXPECT_FALSE(x) minitest::gFramework.ExpectFalse(!!(x), #x)
#define EXPECT_EQ(x, y) minitest::gFramework.ExpectEq(x, y, #x, #y)
#define EXPECT_NE(x, y) minitest::gFramework.ExpectNe(x, y, #x, #y)
#define EXPECT_GT(x, y) minitest::gFramework.ExpectGt(x, y, #x, #y)
#define EXPECT_GE(x, y) minitest::gFramework.ExpectGe(x, y, #x, #y)
#define EXPECT_LT(x, y) minitest::gFramework.ExpectLt(x, y, #x, #y)
#define EXPECT_LE(x, y) minitest::gFramework.ExpectLe(x, y, #x, #y)
#define EXPECT_THAT(cond,val) minitest::gFramework.ExpectThat(cond,val, #val, #cond)
#define EXPECT_ANY_THROW(stmt) minitest::gFramework.ExpectAnyThrow([=](){stmt},#stmt)
#define EXPECT_NO_THROW(stmt) minitest::gFramework.ExpectNoThrow([=](){stmt},#stmt)


#define ASSERT_FALSE(x) if (!minitest::gFramework.AssertFalse(!!(x), #x)) return
#define ASSERT_TRUE(x) if (!minitest::gFramework.AssertTrue(!!(x), #x)) return
#define ASSERT_EQ(x, y) if (!minitest::gFramework.AssertEq(x, y, #x, #y)) return
#define ASSERT_NE(x, y) if (!minitest::gFramework.AssertNe(x, y, #x, #y)) return
#define ASSERT_GT(x, y) if (!minitest::gFramework.AssertGt(x, y, #x, #y)) return
#define ASSERT_GE(x, y) if (!minitest::gFramework.AssertGe(x, y, #x, #y)) return
#define ASSERT_LT(x, y) if (!minitest::gFramework.AssertLt(x, y, #x, #y)) return
#define ASSERT_LE(x, y) if (!minitest::gFramework.AssertLe(x, y, #x, #y)) return
#define ASSERT_THAT(c,v) if (!minitest::gFramework.AssertThat(c,v, #c, #v)) return
#define ASSERT_ANY_THROW(s) if (!minitest::gFramework.AssertAnyThrow([=](){s},#s)) return
#define ASSERT_NO_THROW(s) if (!minitest::gFramework.AssertNoThrow([=](){s},#s)) return

#define EXPECT_TRUE_LOG(x,...) minitest::gFramework.ExpectTrueLog(!!(x), #x,__VA_ARGS__)
#define EXPECT_FALSE_LOG(x,...) minitest::gFramework.ExpectFalseLog(!!(x), #x,__VA_ARGS__)
#define EXPECT_EQ_LOG(x, y,...) minitest::gFramework.ExpectEqLog(x, y, #x, #y,__VA_ARGS__)
#define EXPECT_NE_LOG(x, y,...) minitest::gFramework.ExpectNeLog(x, y, #x, #y,__VA_ARGS__)
#define EXPECT_GT_LOG(x, y,...) minitest::gFramework.ExpectGtLog(x, y, #x, #y,__VA_ARGS__)
#define EXPECT_GE_LOG(x, y,...) minitest::gFramework.ExpectGeLog(x, y, #x, #y,__VA_ARGS__)
#define EXPECT_LT_LOG(x, y,...) minitest::gFramework.ExpectLtLog(x, y, #x, #y,__VA_ARGS__)
#define EXPECT_LE_LOG(x, y,...) minitest::gFramework.ExpectLeLog(x, y, #x, #y,__VA_ARGS__)
#define EXPECT_THAT_LOG(cond,val,...) minitest::gFramework.ExpectThatLog(cond,val, #val, #cond,__VA_ARGS__)
#define EXPECT_ANY_THROW_LOG(stmt,...) minitest::gFramework.ExpectAnyThrowLog([=](){stmt},#stmt,__VA_ARGS__)
#define EXPECT_NO_THROW_LOG(stmt,...) minitest::gFramework.ExpectNoThrowLog([=](){stmt},#stmt,__VA_ARGS__)


#define ASSERT_FALSE_LOG(x,...) if (!minitest::gFramework.AssertFalseLog(!!(x), #x,__VA_ARGS__)) return
#define ASSERT_TRUE_LOG(x,...) if (!minitest::gFramework.AssertTrueLog(!!(x), #x,__VA_ARGS__)) return
#define ASSERT_EQ_LOG(x, y,...) if (!minitest::gFramework.AssertEqLog(x, y, #x, #y,__VA_ARGS__)) return
#define ASSERT_NE_LOG(x, y,...) if (!minitest::gFramework.AssertNeLog(x, y, #x, #y,__VA_ARGS__)) return
#define ASSERT_GT_LOG(x, y,...) if (!minitest::gFramework.AssertGtLog(x, y, #x, #y,__VA_ARGS__)) return
#define ASSERT_GE_LOG(x, y,...) if (!minitest::gFramework.AssertGeLog(x, y, #x, #y,__VA_ARGS__)) return
#define ASSERT_LT_LOG(x, y,...) if (!minitest::gFramework.AssertLtLog(x, y, #x, #y,__VA_ARGS__)) return
#define ASSERT_LE_LOG(x, y,...) if (!minitest::gFramework.AssertLeLog(x, y, #x, #y,__VA_ARGS__)) return
#define ASSERT_THAT_LOG(c,v,...) if (!minitest::gFramework.AssertThatLog(c,v, #c, #v,__VA_ARGS__)) return
#define ASSERT_ANY_THROW_LOG(s,...) if (!minitest::gFramework.AssertAnyThrowLog([=](){s},#s,__VA_ARGS__)) return
#define ASSERT_NO_THROW_LOG(s,...) if (!minitest::gFramework.AssertNoThrowLog([=](){s},#s,__VA_ARGS__)) return

/// @} // end of minitest0_macro_api1_checks_and_asserts

/////////////////////////////////////////////////////////////////////////////////
///* Unit Test Declarations */
/////////////////////////////////////////////////////////////////////////////////

// clang-format on

/// @addtogroup minitest0_macro_api0_case_definitions
/// @{

/// @def TEST
/// @brief Auto-registered free function unit test.
/// @param test_suite Name of a unit test suite(category).
/// @param test_name Name of the test case,must be unique per suite.
///
/// Must be followed with curly braces {} containing the definition.
#define TEST(test_suite, test_name)                                       \
  /* Declare static method format : [test_suite][test_name]_impl */       \
  static void test_suite##test_name##_impl();                             \
  /* Increment test index and register fn pointer and name into global */ \
  /* test map. */                                                         \
  static int test_suite##test_name##_register = [] {                      \
    minitest::gFramework.RegisterTest(#test_suite, #test_name,            \
                                      test_suite##test_name##_impl);      \
    return 0;                                                             \
  }();                                                                    \
  static void test_suite##test_name##_impl()

/// @def TEST_F
/// @brief Auto-registered fixture unit test with free function.
/// @param test_suite Name of a unit test suite(category).
/// @param test_name Name of the test case,must be unique per suite.
/// @param fixture_class Fixture class inheriting from minitest::Fixture
/// @param ... Value arguments to pass to the fixture's constructor.
///
/// This unit test variant:
///   1. Constructs a fixture with the given __VA_ARGS__
///   2. Calls the member methods 'SetUp' then 'Run'.
///   3. Passes the fixture by reference to the user definition following
///      this declaration as 'self', and calls it.
///   4. Calls member method 'TearDown'.
///
/// @note Must be followed with curly braces {} containing a definition.
/// @note Use 'self' to access fixture inside the definition.
#define TEST_F(test_suite, test_name, fixture_class, ...)                   \
  static void test_suite##test_name##_impl(fixture_class& self);            \
  static void test_suite##test_name##_fixture_dispatch() {                  \
    fixture_class this_fixture{__VA_ARGS__};                                \
    this_fixture.SetUp();                                                   \
    this_fixture.Run();                                                     \
    test_suite##test_name##_impl(this_fixture);                             \
    this_fixture.TearDown();                                                \
  };                                                                        \
  static int test_suite##test_name##_register = [] {                        \
    minitest::gFramework.RegisterTest(                                      \
        #test_suite, #test_name, test_suite##test_name##_fixture_dispatch); \
    return 0;                                                               \
  }();                                                                      \
  static void test_suite##test_name##_impl(fixture_class& self)

/// @def TEST_FA
/// @brief Auto-registered fixture unit test, calls Run() to test.
/// @param test_suite Name of a unit test suite(category).
/// @param test_name Name of the test case,must be unique per suite.
/// @param fixture_class Fixture class inheriting from minitest::Fixture
/// @param ... Value arguments to pass to the fixture's constructor.
///
/// This unit test variant:
///   1. Constructs a fixture with the given __VA_ARGS__
///   2. Calls the member methods 'SetUp', 'Run' then 'TearDown'.
#define TEST_FA(test_suite, test_name, fixture_class, ...)           \
  static void test_suite##test_name##_impl() {                       \
    fixture_class this_fixture{__VA_ARGS__};                         \
    this_fixture.SetUp();                                            \
    this_fixture.Run();                                              \
    this_fixture.TearDown();                                         \
  };                                                                 \
  static int test_suite##test_name##_register = [] {                 \
    minitest::gFramework.RegisterTest(#test_suite, #test_name,       \
                                      test_suite##test_name##_impl); \
    return 0;                                                        \
  }()

/// @def MINITEST_RUN
/// @brief Runs unit tests, returns true if all passed.
/// @see MinitestFramework::RunTests for accepted arg formats.
#define MINITEST_RUN(...) minitest::gFramework.RunTests(__VA_ARGS__)

/// @def MINITEST_CLI_RUN
/// @brief Runs unit tests returns 0 if all passed, otherwise 1.
#define MINITEST_CLI_RUN(argc, argv) minitest::gFramework.CliMain(argc, argv)


/// @} // end of minitest0_macro_api0_case_definitions

/// @} // end of minitest

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: Minitest Framework
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2025 Anton Yashchenko
//
// Licensed under the Apache License, Version 2.0(the "License"); you may not
// use this file except in compliance with the License. You may obtain a copy of
// the License at http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
// WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
// License for the specific language governing permissions and limitations under
// the License.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////