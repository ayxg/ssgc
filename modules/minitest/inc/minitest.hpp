///////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
///////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language Environment
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////
/// @file 
/// @ingroup cppmodule1_minitest
/// @brief Mini-Test: A Minimal Unit Testing Framework for C++20
///////////////////////////////////////////////////////////////////////////////
// clang-format off
/// @addtogroup cppmodule1_minitest
/// @{

#ifndef HEADER_GUARD_CALE_MINITEST_MINITEST_H
#define HEADER_GUARD_CALE_MINITEST_MINITEST_H

// @includes
#include <concepts>
#include <iostream>
#include <source_location>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>
#include <functional>
#include <map>

///////////////////////////////////////////////////////////////////////////////
/// @namespace minitest
///////////////////////////////////////////////////////////////////////////////
namespace minitest {

/// @def MINITEST_CONFIG_CUSTOM_SEPARATOR
/// 
/// If defined by user, a custom separator is used between test sections which 
/// should be the macro def. Default is a dashed line. 
/// Definition must be a string literal with a newline.
#ifndef MINITEST_CONFIG_CUSTOM_SEPARATOR
#define MINITEST_CONFIG_CUSTOM_SEPARATOR                         \
  "------------------------------------------------------------" \
  "---------------------------------------------------------\n"
#endif  // !MINITEST_CONFIG_CUSTOM_SEPARATOR

/// Used as a separator between sections in the output.
static constexpr auto kSeparator = MINITEST_CONFIG_CUSTOM_SEPARATOR;


/// @brief Concept to check if a type is streamable.
/// 
/// Non-streamable objects are output as their pointer address.
template <typename T>
concept iStreamable = requires(std::ostream os, std::decay_t<T> value) {
  { os << value } -> std::same_as<std::ostream&>;
};

/// @brief A template for a test case, the main 'testing' structure.
/// 
/// @warning: DO NOT directly create an instance of this template.
///           Always use MINITEST macro.
template <auto TestName, auto TestCaseName, typename TestImpl>
struct Test {
  static constexpr auto test_name = TestName();
  static constexpr auto test_case_name = TestCaseName();
  static inline const TestImpl test_impl{};
  bool is_test_passed = true;

  /// @brief Runs this test and logs result to console(if enabled).
  /// @return bool always true
  /// 
  /// Define MINITEST_CONFIG_NO_CONSOLE_PRINT before including minitest to
  /// disable printing to console when running tests.
  static inline bool Run() {
#ifndef MINITEST_CONFIG_NO_CONSOLE_PRINT
    std::cout << kSeparator << "[Begin Mini Test] " << test_name << " [Case]"
              << test_case_name << "\n"
              << kSeparator;
#endif  // !MINITEST_CONFIG_NO_CONSOLE_PRINT
    test_impl();
#ifndef MINITEST_CONFIG_NO_CONSOLE_PRINT
    std::cout << kSeparator << "[End Mini Test] " << test_name << " [Case]"
              << test_case_name << "\n"
              << kSeparator;
#endif  // !MINITEST_CONFIG_NO_CONSOLE_PRINT
    return true;
  }
};

/// @brief Base class for all fixtures.
/// 
/// Inherit from this class to create a fixture.
/// All members of child class must be public or protected
/// If you wish to access them in your test cases.
struct Fixture {
  virtual void SetUp() {}
  virtual void TearDown() {}
};

/// @defgroup cppmodule1_minitest_internal Internal Methods
/// 
/// !! DO NOT use the methods below directly in test cases.
/// Internal implementations of check and assert static methods.
/// @{

/// DO NOT USE
static inline bool ExpectTrue(bool b) {
  if (!b) {
#ifndef MINITEST_CONFIG_NO_CONSOLE_PRINT
    std::cout << "[FAIL] Expected TRUE." << std::endl;
#endif  // !MINITEST_CONFIG_NO_CONSOLE_PRINT
    return false;
  }
  return true;
}

/// DO NOT USE
static inline bool ExpectFalse(bool b) {
  if (b) {
#ifndef MINITEST_CONFIG_NO_CONSOLE_PRINT
    std::cout << "[FAIL] Expected FALSE." << std::endl;
#endif  // !MINITEST_CONFIG_NO_CONSOLE_PRINT
    return false;
  }
  return true;
}

/// DO NOT USE
template <typename LT, typename RT>
  requires std::equality_comparable_with<LT, RT>
static inline bool ExpectEq(LT lhs, RT rhs) {
  if (lhs == rhs) {
    return true;
  } else {
#ifndef MINITEST_CONFIG_NO_CONSOLE_PRINT
    std::cout << "[FAIL] Expected Equality with: ";
    if constexpr (iStreamable<LT> && iStreamable<RT>)
      std::cout << ">|" << lhs << " Got: " << rhs << std::endl;
    else
      std::cout << ">|" << &lhs << " Got: " << &rhs << std::endl;
#endif  // !MINITEST_CONFIG_NO_CONSOLE_PRINT
    return false;
  }
}

/// DO NOT USE
template <typename LT, typename RT>
  requires std::equality_comparable_with<LT, RT>
static inline bool ExpectNe(LT lhs, RT rhs) {
  if (lhs != rhs) {
    return true;
  } else {
#ifndef MINITEST_CONFIG_NO_CONSOLE_PRINT
    std::cout << "[FAIL] Expected Inequality with";
    if constexpr (iStreamable<LT> && iStreamable<RT>)
      std::cout << ">|" << lhs << std::endl;
    else
      std::cout << ">|" << &lhs << std::endl;
#endif  // !MINITEST_CONFIG_NO_CONSOLE_PRINT
    return false;
  }
}

/// DO NOT USE
static inline bool ExpectAnyThrow(auto&& f) {
  try {
    f();
  } catch (...) {
    return true;
  }
#ifndef MINITEST_CONFIG_NO_CONSOLE_PRINT
  std::cout << "[FAIL] Expected exception but got none." << std::endl;
#endif  // !MINITEST_CONFIG_NO_CONSOLE_PRINT
  return false;
}

/// DO NOT USE
static inline bool ExpectNoThrow(auto&& f) {
  try {
    f();
  } catch (...) {
#ifndef MINITEST_CONFIG_NO_CONSOLE_PRINT
    std::cout << "[FAIL] Expected no exception but one was raised."
              << std::endl;
#endif  // !MINITEST_CONFIG_NO_CONSOLE_PRINT
    return false;
  }
  return true;
}

/// @} // end group cppmodule1_minitest_internal

/// @brief Stores results and metadata of a recorded test.
struct TestResult {
  bool is_test_passed;
  std::string test_name;
  std::string test_case_name;
  std::source_location location;
  std::string log;
};

/// @brief QOL std::ostream overload to directly print test results.
std::ostream& operator<<(std::ostream& ss, const TestResult& t) {
  if (t.is_test_passed) {
    ss << "[PASSED] Test: " << t.test_name << "\n Case: " << t.test_case_name
       << '\n';
  } else {
    ss << "[FAILED] Test: " << t.test_name << "\n Case: " << t.test_case_name
       << t.location.file_name() << '(' << t.location.line() << ':'
       << t.location.column() << ") `" << t.location.function_name() << "`: \n";
  };
  return ss;
}

static std::vector<std::string> gFailedTestLogs; ///! Stores failed test logs.
static std::vector<TestResult> gRecordedTestLogs; ///! Stores all test results if enabled.

/// Stores the function pointers to a modules test cases associated with a registered name.
static std::map<std::string_view, std::map<std::string_view,std::function<bool()>>> gRegisteredTests;
static const char* gLastFailedTestName = ""; ///! DO NOT USE, impl detail.
static const char* gLastFailedTestCaseName = ""; ///! DO NOT USE, impl detail.

/// @brief Adds a test case to a test module with given test_name
/// @param test_name Name of test or test module.
/// @param test Function pointer to the test.
static inline void RegisterTest(const std::string_view & test_name,const std::string_view & test_case_name,
                                std::function<bool()> test) {
  gRegisteredTests[test_name][test_case_name] = test;
}

/// @brief Adds a test case to a test module with given test_name
/// @param test_name Name of test or test module.
/// @param test Function pointer to the test.
static inline bool RunRegisteredTest(const std::string_view & test_name,const std::string_view & test_case_name) {
  return gRegisteredTests[test_name][test_case_name]();
}

/// @brief Runs all the tests associated with a given test name or module name.
/// @param test_name Name of the module to run, must be registered.
/// @return True, unless a test failed - then false.
static inline bool RunRegisteredTestModule(const std::string_view & test_name) {
  bool passed = true;
  for (const auto& test : gRegisteredTests[test_name]) {
    if(not test.second()){
      passed = false;    
    }
  }
  return passed;
};

static inline bool RunAllRegisteredTestModules() {
  bool passed = true;
  for(const auto& test : gRegisteredTests){
    for(const auto& tcase : test.second){
      if(not tcase.second()){
        passed = false;
      }
    }
  }
  return passed;
};

/// @brief For internal mini-test use only. Adds a formatted error log to the
///        gFailedTestLogs
/// @param log Message to log.
/// @param test Test name.
/// @param tcase Test case name.
static inline void AddFailedTestLog(
    const std::string& log, const char* test, const char* tcase,
    const std::source_location location = std::source_location::current()) {
  std::stringstream ss;
  ss << "[FAILURE DETECTED] Test: " << std::string(test)
     << " Case: " << std::string(tcase) << " On Check:" << log
     << "\nfile: " << location.file_name() << '(' << location.line() << ':'
     << location.column() << ") `" << location.function_name() << "`: " << '\n';
  gFailedTestLogs.push_back(ss.str());
}

/// @brief Prints all failed test logs, or "All Tests Passed!" when none.
/// @return True if all tests passed, false otherwise.
static inline bool PrintFailedTestLogs() {
  if (gFailedTestLogs.empty()) {
    std::cout << kSeparator << "All tests passed.\n" << kSeparator;
    return true;
  } else {
    std::cout << kSeparator << "Failed Tests:\n" << kSeparator;
    for (const auto& log : gFailedTestLogs) {
      std::cout << log << std::endl;
    }
    std::cout << kSeparator << "End of Failed Tests:\n" << kSeparator;
    return false;
  }
}

/// @brief Adds a TestResult to 'gRecordedTestLogs'
static inline void RecordTestLog(
    bool passed, const std::string& log, const char* test, const char* tcase,
    const std::source_location location = std::source_location::current()) {
  gRecordedTestLogs.push_back(TestResult{.is_test_passed = passed,
                                         .test_name = std::string(test),
                                         .test_case_name = std::string(tcase),
                                         .location = location,
                                         .log = log});
}

/// @brief Prints all recorded test logs or "No tests were run" if none.
/// @return Returns true if no tests were ran, false otherwise.
static inline bool PrintRecordedTestLogs() {
  if (gRecordedTestLogs.empty()) {
    std::cout << kSeparator << "No test were run.\n" << kSeparator;
    return true;
  } else {
    std::cout << kSeparator << "Tests:\n" << kSeparator;
    for (const auto& log : gRecordedTestLogs) {
      std::cout << log << std::endl;
    }
    std::cout << kSeparator << "End of Tests:\n" << kSeparator;
    return false;
  }
}

/// @brief Access the recorded test logs stored in gRecordedTestLogs
/// @warning: DO NOT ever access gRecordedTestLogs directly.
static inline const std::vector<TestResult>& ViewTestResults() {
  return gRecordedTestLogs;
}
/// @brief Access the failed test logs stored in gFailedTestLogs
/// @warning: DO NOT ever access gFailedTestLogs directly.
static inline const std::vector<std::string>& ViewFailedTestResults() {
  return gFailedTestLogs;
}

/// Clears recorded test logs.
static inline void FlushTestResults() {
  gRecordedTestLogs.clear();
}

/// Clears failed test logs.
static inline void FlushFailedTestResults() {
  gFailedTestLogs.clear();
}


constexpr int CmakeMain(int argc, char* argv[])
{
  std::vector<std::string> args{argv, argv + argc};
  switch (args.size()) {
    case 1:
      return !RunAllRegisteredTestModules();
    case 2:
      return !RunRegisteredTestModule(args[1]);
    case 3:
      return !RunRegisteredTest(args[1], args[2]);
    default:
      return 1;
  }
} 
}
///////////////////////////////////////////////////////////////////////////////
// @end namespace minitest
///////////////////////////////////////////////////////////////////////////////

/// @def MINITEST
/// @brief Defines a static test case. Always close with 'END_MINITEST;'.
/// @param TestName Name of the test.
/// @param TestCaseName Name of the test case,must be unique per test.
/// 
/// { } before and after the test case definition is optional
/// but recommended for visibility.
#define MINITEST(TestName, TestCaseName) \
  namespace minitest_unit_test {         \
  namespace TestName {                   \
  bool MINITEST_TEST_##TestCaseName = []() -> bool {\
  return minitest::Test < []() consteval -> const char* { return #TestName; },\
       []() consteval -> const char* { return #TestCaseName; },\
       decltype([]() -> void {\
         minitest::gLastFailedTestName = #TestName;\
         minitest::gLastFailedTestCaseName = #TestCaseName;

/// @def END_MINITEST
/// @brief Completes and runs a test case. 
/// 
/// Must be called after your test case definition.
#define END_MINITEST \
  }                  \
  )                  \
  >                  \
  ::Run();           \
  }                  \
  ();                \
  }                  \
  ;                  \
  }

/// @def MINITEST_F}
/// @brief Defines a fixture test case.
///        Always close with 'END_MINITEST_F(NameOfFixture);'.
/// @param TestName Name of the test.
/// @param TestCaseName Name of the test case,must be unique per test.
/// @param TestFixtureClass Name of the fixture class.
/// 
/// Must inherit from minitest::Fixture.
/// @see minitest::Fixture
#define MINITEST_F(TestName, TestCaseName, TestFixtureClass) \
  namespace minitest_unit_test {                             \
  namespace TestName {                                       \
  bool MINITEST_TEST_##TestCaseName = []() -> bool {\
  return minitest::Test < []() consteval -> const char* { return #TestName; },\
       []() consteval -> const char* { return #TestCaseName; },\
       decltype([]() -> void {\
  const static struct MINITEST_FIXTURE_##TestCaseName : TestFixtureClass {\
  void RunFixture() {\
  minitest::gLastFailedTestCaseName = #TestName;\
  minitest::gLastFailedTestName = #TestCaseName;\
  (*this).SetUp();


/// @def END_MINITEST_F
/// @brief Closes a fixture test case. ONLY use after MINITEST_F with the SAME
///        TestCaseName.
///	@param TestCaseName : Name of the last-called test case which created
///         was created with MINITEST_F. MUST be the same as the last-called 
///         test case. Or else it will not compile.
/// 
/// ex. If you called MINITEST_F(MyTest,MyTestCase,MyFixture)
/// then you must call END_MINITEST_F(MyTestCase);
#define END_MINITEST_F(TestCaseName)                                        \
  (*this).TearDown();                                                       \
  }                                                                         \
  }                                                                         \
  ;                                                                         \
  MINITEST_FIXTURE_##TestCaseName MINITEST_FIXTURE_INSTANCE_##TestCaseName; \
  MINITEST_FIXTURE_INSTANCE_##TestCaseName.RunFixture();                    \
  }                                                                         \
  )\
  >::Run();                                                                 \
  }                                                                         \
  ();                                                                       \
  }                                                                         \
  ;                                                                         \
  }

/// @def INLINE_MINITEST
/// @brief Defines an inline test case to be executed at a later time.
///        Always close with 'INLINE_END_MINITEST;'.
#define INLINE_MINITEST(TestName, TestCaseName) \
  auto INLINE_MINITEST_##TestName##TestCaseName = []() -> bool {\
  return minitest::Test < []() consteval -> const char* { return #TestName; },\
       []() consteval -> const char* { return #TestCaseName; },\
       decltype([]() -> void {\
         minitest::gLastFailedTestName = #TestName;\
         minitest::gLastFailedTestCaseName = #TestCaseName;

/// @def INLINE_END_MINITEST
#define INLINE_END_MINITEST \
  }                         \
  )                         \
  >                         \
  ::Run();                  \
  }                         \
  ;

/// @def MINITEST_REGISTER_CASE
/// @see minitest::RegisterTest
#define MINITEST_REGISTER_CASE(TestName, TestCaseName) \
  const bool REGISTER_INLINE_MINITEST_##TestName##TestCaseName \
  = []{minitest::RegisterTest(#TestName,#TestCaseName,INLINE_MINITEST_##TestName##TestCaseName);return true;}()

/// @def MINITEST_RUN_REGISTERED_MODULE
/// @see RunRegisteredTestModule
#define MINITEST_RUN_REGISTERED_MODULE(TestName) \
  minitest::RunRegisteredTestModule(#TestName)

/// @def REGISTERED_TEST_MODULE_METHOD
/// @brief Lambda functor of a MINITEST_RUN_REGISTERED_MODULE call.
#define MINITEST_FUNCTOR_RUN_INLINE(TestName) \
  []()->bool{return minitest::RunRegisteredTestModule(#TestName);}

/// @def MINITEST_RUN_INLINE
/// @brief Runs an inline minitest.
#define MINITEST_RUN_INLINE(TestName, TestCaseName) \
  INLINE_MINITEST_##TestName##TestCaseName();       \


/// @def FINISH_MINITESTS
/// @brief  Completes the test suite and prints the result.
///
/// Must be called right before your main function.
/// This store a boolean true result in MINITESTS_RESULT
/// if all tests passed - else false.
//
/// This macro does not run the tests, it only prints the result.
/// This macro must be called LAST, after all tests are defined.
#define FINISH_MINITESTS                      \
  namespace minitest {                       \
  static const bool minitest_result = []() { \
    return minitest::PrintFailedTestLogs();  \
  }();                                       \
  }  

/// @def MINITESTS_RESULT 
/// @brief Result of the tests as a bool. True if all passed, else false.
/// 
/// Only valid after calling FINISH_MINITESTS.
/// This macro contains the result of the tests as a static const bool.
#define MINITESTS_RESULT minitest::minitest_result

/// @def MINITESTS_RECORDED
/// @see minitest::ViewTestResults
#define MINITESTS_RECORDED minitest::ViewTestResults()

/// @def MINITESTS_FAILED
/// @see minitest::ViewFailedTestResults
#define MINITESTS_FAILED minitest::ViewFailedTestResults()

/// @def MINITEST_INTERNAL_CHECK_METHOD
/// @brief Generates a checking macro for a given minitest method.
///        DO NOT USE DIRECTLY.
///        This is used internally by the EXPECT_ macros.
#ifdef MINITEST_CONFIG_RECORD_ALL
#define MINITEST_INTERNAL_CHECK_METHOD(method, pmsg, msg, ...)         \
  if (!minitest::method(__VA_ARGS__)) {                                \
    minitest::AddFailedTestLog(msg, minitest::gLastFailedTestName,     \
                               minitest::gLastFailedTestCaseName);     \
    minitest::RecordTestLog(false, msg, minitest::gLastFailedTestName, \
                            minitest::gLastFailedTestCaseName);        \
  } else {                                                             \
    minitest::RecordTestLog(true, pmsg, minitest::gLastFailedTestName, \
                            minitest::gLastFailedTestCaseName);        \
  }
#else
#define MINITEST_INTERNAL_CHECK_METHOD(method, pmsg, msg, ...)     \
  if (!minitest::method(__VA_ARGS__)) {                            \
    minitest::AddFailedTestLog(msg, minitest::gLastFailedTestName, \
                               minitest::gLastFailedTestCaseName); \
  }
#endif

/// @def MINITEST_INTERNAL_ASSERT_METHOD
/// @brief Generates am assertion macro for a given minitest method.
///        DO NOT USE DIRECTLY.
///        This is used internally by the ASSERT_ macros.
#ifdef MINITEST_CONFIG_RECORD_ALL
#define MINITEST_INTERNAL_ASSERT_METHOD(method, pmsg, msg, ...)        \
  if (!minitest::method(__VA_ARGS__)) {                                \
    minitest::AddFailedTestLog(msg, minitest::gLastFailedTestName,     \
                               minitest::gLastFailedTestCaseName);     \
    minitest::RecordTestLog(false, msg, minitest::gLastFailedTestName, \
                            minitest::gLastFailedTestCaseName);        \
    return;                                                            \
  } else {                                                             \
    minitest::RecordTestLog(true, pmsg, minitest::gLastFailedTestName, \
                            minitest::gLastFailedTestCaseName);        \
  }
#else
#define MINITEST_INTERNAL_ASSERT_METHOD(method, pmsg, msg, ...)    \
  if (!minitest::method(__VA_ARGS__)) {                            \
    minitest::AddFailedTestLog(msg, minitest::gLastFailedTestName, \
                               minitest::gLastFailedTestCaseName); \
    return;                                                        \
  }
#endif

///////////////////////////////////////////////////////////////////////////////
// Test Macros
///////////////////////////////////////////////////////////////////////////////

/// @defgroup cppmodule1_minitest_tests Check and Assertion Macros
/// 
/// Macros used to perform checks and assertions within a MINITEST definition.
/// @{

///////////////////////////////////////
// Checks
///////////////////////////////////////

/// @defgroup cppmodule1_minitest_tests_checks Checks
/// @{
#define EXPECT_TRUE(b)                                               \
  MINITEST_INTERNAL_CHECK_METHOD(ExpectTrue, "[PASSED] EXPECT_TRUE", \
                                 "[EXPECTATION FAILED]: EXPECT_TRUE", b)
#define EXPECT_FALSE(b)                                                \
  MINITEST_INTERNAL_CHECK_METHOD(ExpectFalse, "[PASSED] EXPECT_FALSE", \
                                 "[EXPECTATION FAILED]: EXPECT_FALSE", b)
#define EXPECT_EQ(a, b)                                          \
  MINITEST_INTERNAL_CHECK_METHOD(ExpectEq, "[PASSED] EXPECT_EQ", \
                                 "[EXPECTATION FAILED]: EXPECT_EQ", a, b)
#define EXPECT_NE(a, b)                                          \
  MINITEST_INTERNAL_CHECK_METHOD(ExpectNe, "[PASSED] EXPECT_NE", \
                                 "[EXPECTATION FAILED]: EXPECT_NE", a, b)
#define EXPECT_ANY_THROW(f)                                                   \
  MINITEST_INTERNAL_CHECK_METHOD(ExpectAnyThrow, "[PASSED] EXPECT_ANY_THROW", \
                                 "[EXPECTATION FAILED]: EXPECT_ANY_THROW", f)
#define EXPECT_NO_THROW(f)                                                  \
  MINITEST_INTERNAL_CHECK_METHOD(ExpectNoThrow, "[PASSED] EXPECT_NO_THROW", \
                                 "[EXPECTATION FAILED]: EXPECT_NO_THROW", f)
/// @} // end cppmodule1_minitest_tests_checks

///////////////////////////////////////
// Assertions
///////////////////////////////////////

/// @defgroup cppmodule1_minitest_tests_asserts Assertions
/// @{
#define ASSERT_TRUE(b)                                                \
  MINITEST_INTERNAL_ASSERT_METHOD(ExpectTrue, "[PASSED] ASSERT_TRUE", \
                                  "[ASSERTION FAILED]: ASSERT_TRUE", b)
#define ASSERT_FALSE(b)                                                 \
  MINITEST_INTERNAL_ASSERT_METHOD(ExpectFalse, "[PASSED] ASSERT_FALSE", \
                                  "[ASSERTION FAILED]: ASSERT_FALSE", b)
#define ASSERT_EQ(a, b)                                           \
  MINITEST_INTERNAL_ASSERT_METHOD(ExpectEq, "[PASSED] ASSERT_EQ", \
                                  "[ASSERTION FAILED]: ASSERT_EQ", a, b)
#define ASSERT_NE(a, b)                                           \
  MINITEST_INTERNAL_ASSERT_METHOD(ExpectNe, "[PASSED] ASSERT_NE", \
                                  "[ASSERTION FAILED]: ASSERT_NE", a, b)
#define ASSERT_ANY_THROW(f)                                                    \
  MINITEST_INTERNAL_ASSERT_METHOD(ExpectAnyThrow, "[PASSED] ASSERT_ANY_THROW", \
                                  "[ASSERTION FAILED]: ASSERT_ANY_THROW", f)
#define ASSERT_NO_THROW(f)                                                   \
  MINITEST_INTERNAL_ASSERT_METHOD(ExpectNoThrow, "[PASSED] ASSERT_NO_THROW", \
                                  "[ASSERTION FAILED]: ASSERT_NO_THROW", f)
/// @} // end cppmodule1_minitest_tests_asserts

///////////////////////////////////////
// Logging checks
///////////////////////////////////////

/// @defgroup cppmodule1_minitest_tests_lchecks Logging Checks
/// @{
#define EXPECT_TRUE_LOG(b, plg, lg) \
  MINITEST_INTERNAL_CHECK_METHOD(ExpectTrue, plg, lg, b)
#define EXPECT_FALSE_LOG(b, plg, lg) \
  MINITEST_INTERNAL_CHECK_METHOD(ExpectFalse, plg, lg, b)
#define EXPECT_EQ_LOG(a, b, plg, lg) \
  MINITEST_INTERNAL_CHECK_METHOD(ExpectEq, lg, plg, a, b)
#define EXPECT_NE_LOG(a, b, plg, lg) \
  MINITEST_INTERNAL_CHECK_METHOD(ExpectNe, lg, plg, a, b)
#define EXPECT_ANY_THROW_LOG(f, plg, lg) \
  MINITEST_INTERNAL_CHECK_METHOD(ExpectAnyThrow, plg, lg, f)
#define EXPECT_NO_THROW_LOG(f, plg, lg) \
  MINITEST_INTERNAL_CHECK_METHOD(ExpectNoThrow, plg, lg, f)
/// @} // end cppmodule1_minitest_tests_lchecks

///////////////////////////////////////
// Logging assertions
///////////////////////////////////////

/// @defgroup cppmodule1_minitest_tests_lasserts Logging Asserts
/// @{
#define ASSERT_TRUE_LOG(b, plg, lg) \
  MINITEST_INTERNAL_ASSERT_METHOD(ExpectTrue, plg, lg, b)
#define ASSERT_FALSE_LOG(b, plg, lg) \
  MINITEST_INTERNAL_ASSERT_METHOD(ExpectFalse, plg, lg, b)
#define ASSERT_EQ_LOG(a, b, plg, lg) \
  MINITEST_INTERNAL_ASSERT_METHOD(ExpectEq, lg, plg, a, b)
#define ASSERT_NE_LOG(a, b, plg, lg) \
  MINITEST_INTERNAL_ASSERT_METHOD(ExpectNe, lg, plg, a, b)
#define ASSERT_ANY_THROW_LOG(f, plg, lg) \
  MINITEST_INTERNAL_ASSERT_METHOD(ExpectAnyThrow, plg, lg, f)
#define ASSERT_NO_THROW_LOG(f, plg, lg) \
  MINITEST_INTERNAL_ASSERT_METHOD(ExpectNoThrow, plg, lg, f)
/// @} // end group cppmodule1_minitest_tests_lasserts

///////////////////////////////////////////////////////////////////////////////
// End Test Macros
///////////////////////////////////////////////////////////////////////////////

#endif HEADER_GUARD_CALE_MINITEST_MINITEST_H
/// @} // end of cppmodule1_minitest
// clang-format on
///////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language Environment
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
//
// Licensed under the GNU Affero General Public License, Version 3.
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
