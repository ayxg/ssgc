//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
// Copyright 2024 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
//---------------------------------------------------------------------------//
// Author(s): Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.acpp.dev
//---------------------------------------------------------------------------//
// Project: C& Programming Language Environment
// Directory: mini-test
// File: minitest.h
//---------------------------------------------------------------------------//
#ifndef HEADER_GUARD_CALE_MINITEST_MINITEST_H
#define HEADER_GUARD_CALE_MINITEST_MINITEST_H
//---------------------------------------------------------------------------//
// Mini-Test: A Minimal Unit Testing Framework for C++20
// Requires:{
//   - Compiler flag minimum /std:c++20
//   - P0315R4 : Lambdas in unevaluated contexts
//     https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0315r4.pdf
// }
// Brief: {
//  This is a single header file unit testing framework. Tests are defined as
//  lambdas in a template parameter, which is executed upon initialization of
//  static variables. As a result the order of tests is not guaranteed but the
//  usual order of static initialization is followed. New: now supports fixtures
//  and inline tests which can be run at a later time and defined inside
//  methods.
// 
// (More examples available at bottom of this file...)
// Sample Use: 
//       auto my_method() { return true; }
//       MINITEST(MyTest,MyTestCase){
//         EXPECT_TRUE(my_method());
//         EXPECT_TRUE(false);
//         EXPECT_FALSE(false);
//         EXPECT_FALSE(true);
//         EXPECT_EQ(1,1);
//         EXPECT_EQ(1,2);
//         EXPECT_NE(1,2);
//         EXPECT_NE(1,1);
//         EXPECT_ANY_THROW([](){ throw std::runtime_error("error"); });
//         EXPECT_ANY_THROW([](){});
//         EXPECT_NO_THROW([](){});
//         EXPECT_NO_THROW([](){ throw std::runtime_error("error"); });
//        }
//       END_MINITEST;
//       FINISH_MINITESTS; // returns a boolean true if all tests passed
//                         // call this right before your main function
//                         // Macro MINITESTS_RESULT will contain result.
//       int main() {
//         return MINITESTS_RESULT ? 0 : 1;
//       }
//
// Configuration Macros:
//   - MINITEST_CONFIG_RECORD_ALL: If true, all test results are recorded and
//                              can be viewed using MINITESTS_RECORDED macro.
//
//   - MINITEST_CONFIG_NO_CONSOLE_PRINT: If true, no output is printed to the
//                                       console during the test run.
//
//   - MINITEST_CONFIG_CUSTOM_SEPARATOR: If defined, a custom separator is used
//   between test sections which should be the macro def. Default is a dashed
//   line. Definition must be a string literal with a newline.
// }
//---------------------------------------------------------------------------//
// Includes:
#include <concepts>
#include <iostream>
#include <source_location>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>
//---------------------------------------------------------------------------//
//=-------------------------------------------------------------------------=//

//---------------------------------------------------------------------------//
// namespace minitest
//---------------------------------------------------------------------------//
namespace minitest {

#ifndef MINITEST_CONFIG_CUSTOM_SEPARATOR
#define MINITEST_CONFIG_CUSTOM_SEPARATOR                         \
  "------------------------------------------------------------" \
  "---------------------------------------------------------\n"
#endif  // !MINITEST_CONFIG_CUSTOM_SEPARATOR

// Used as a seperator between sections in the output.
static constexpr auto kSeparator = MINITEST_CONFIG_CUSTOM_SEPARATOR;

// Concept to check if a type is streamable.
// Non-streamable objects are output as their pointer address.
template <typename T>
concept iStreamable = requires(std::ostream os, std::decay_t<T> value) {
  { os << value } -> std::same_as<std::ostream&>;
};

// A template for a test case, the main 'testing' structure.
// !! DO NOT directly create an instance of this template.
// Always use MINITEST macro.
template <auto TestName, auto TestCaseName, typename TestImpl>
struct Test {
  static constexpr auto test_name = TestName();
  static constexpr auto test_case_name = TestCaseName();
  static inline const TestImpl test_impl{};
  bool is_test_passed = true;
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

// Base class for all fixtures.
// Inherit from this class to create a fixture.
// All members of child class must be public or protected
// If you wish to access them in your test cases.
struct Fixture {
  virtual void SetUp() {}
  virtual void TearDown() {}
};

// !! DO NOT use the methods below directly in test cases.
// Internal implementations of check and assert static methods.

// !! DO NOT USE
static inline bool ExpectTrue(bool b) {
  if (!b) {
#ifndef MINITEST_CONFIG_NO_CONSOLE_PRINT
    std::cout << "[FAIL] Expected TRUE." << std::endl;
#endif  // !MINITEST_CONFIG_NO_CONSOLE_PRINT
    return false;
  }
  return true;
}

// !! DO NOT USE
static inline bool ExpectFalse(bool b) {
  if (b) {
#ifndef MINITEST_CONFIG_NO_CONSOLE_PRINT
    std::cout << "[FAIL] Expected FALSE." << std::endl;
#endif  // !MINITEST_CONFIG_NO_CONSOLE_PRINT
    return false;
  }
  return true;
}

// !! DO NOT USE
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

// !! DO NOT USE
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

// !! DO NOT USE
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

// !! DO NOT USE
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

struct TestResult {
  bool is_test_passed;
  std::string test_name;
  std::string test_case_name;
  std::source_location location;
  std::string log;
};

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

static std::vector<std::string> gFailedTestLogs;
static std::vector<TestResult> gRecordedTestLogs;
static std::map<std::string_view, std::vector<std::function<bool()>>> gRegisteredTests;
static const char* gLastFailedTestName = "";
static const char* gLastFailedTestCaseName = "";

static inline void RegisterTest(const std::string_view & test_name,
                                std::function<bool()> test) {
  gRegisteredTests[test_name].push_back(test);
}

static inline bool RunRegisteredTestModule(const std::string_view & test_name) {
  bool passed = true;
  for (const auto& test : gRegisteredTests[test_name]) {
    if(not test()){
      passed = false;    
    }
  }
  return passed;
};

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

static inline void RecordTestLog(
    bool passed, const std::string& log, const char* test, const char* tcase,
    const std::source_location location = std::source_location::current()) {
  gRecordedTestLogs.push_back(TestResult{.is_test_passed = passed,
                                         .test_name = std::string(test),
                                         .test_case_name = std::string(tcase),
                                         .location = location,
                                         .log = log});
}

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

static inline const std::vector<TestResult>& ViewTestResults() {
  return gRecordedTestLogs;
}

static inline const std::vector<std::string>& ViewFailedTestResults() {
  return gFailedTestLogs;
}

static inline void FlushTestResults() {
  gRecordedTestLogs.clear();
}

static inline void FlushFailedTestResults() {
  gFailedTestLogs.clear();
}

}  // namespace minitest
//---------------------------------------------------------------------------//
//=-------------------------------------------------------------------------=//

//=---------------------------------=//
// Macro:{MINITEST}
// Brief:{Defines a test case. Always close with 'END_MINITEST;'.}
// Parameters:{
//		1.TestName : Name of the test.
//		2.TestCaseName : Name of the test case,must be unique per test.
// }
// Detail:{
// - { } before and after the test case definition is optional
//   but recommended for visibility.
// }
//-----------------------------------//
#define MINITEST(TestName, TestCaseName) \
  namespace minitest_unit_test {         \
  namespace TestName {                   \
  bool MINITEST_TEST_##TestCaseName = []() -> bool {\
  return minitest::Test < []() consteval -> const char* { return #TestName; },\
       []() consteval -> const char* { return #TestCaseName; },\
       decltype([]() -> void {\
         minitest::gLastFailedTestName = #TestName;\
         minitest::gLastFailedTestCaseName = #TestCaseName;
//-----------------------------------//
//=---------------------------------=//

//=---------------------------------=//
// Macro:{END_MINITEST}
// Brief:{Completes and runs a test case.
//        Must be called after your test case definition.
// }
//-----------------------------------//
#define END_MINITEST \
  }                  \
  )\
  >\
  ::Run();           \
  }                  \
  ();                \
  }                  \
  ;                  \
  }
//-----------------------------------//
//=---------------------------------=//

//=---------------------------------=//
// Macro:{MINITEST_F}
// Brief:{Defines a fixture test case.
//        Always close with 'END_MINITEST_F(TestCaseName,NameOfFixture);'.
// }
// Parameters:{
//		1.TestName : Name of the test.
//		2.TestCaseName : Name of the test case,must be unique per test.
// 	    3.TestFixtureClass : Name of the fixture class.
//                         Must inherit from minitest::Fixture.
//                         See minitest::Fixture for more details.
// }
//-----------------------------------//
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
//-----------------------------------//
//=---------------------------------=//

//=---------------------------------=//
// Macro:{END_MINITEST_F}
// Brief:{Closes a fixture test case. ONLY use after MINITEST_F with the SAME
// TestCaseName.
// }
// Parameters:{
//	    1.TestCaseName : Name of the last-called test case which created
//      was created with MINITEST_F. MUST be the same as the last-called test
//      case. Or else it will not compile.
//
//      eg. If you called MINITEST_F(MyTest,MyTestCase,MyFixture)
//      then you must call END_MINITEST_F(MyTestCase);
// }
//-----------------------------------//
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
//-----------------------------------//
//=---------------------------------=//

//=---------------------------------=//
// Macro:{INLINE_MINITEST}
// Brief:{Defines an inline test case to be executed at a later time.
// Always close with 'INLINE_END_MINITEST;'.}
//-----------------------------------//
#define INLINE_MINITEST(TestName, TestCaseName) \
  auto INLINE_MINITEST_##TestName##TestCaseName = []() -> bool {\
  return minitest::Test < []() consteval -> const char* { return #TestName; },\
       []() consteval -> const char* { return #TestCaseName; },\
       decltype([]() -> void {\
         minitest::gLastFailedTestName = #TestName;\
         minitest::gLastFailedTestCaseName = #TestCaseName;
//-----------------------------------//
//=---------------------------------=//

//=---------------------------------=//
// Macro:{INLINE_END_MINITEST}
// Brief:{}
//-----------------------------------//
#define INLINE_END_MINITEST \
  }                         \
  )\
  >\
  ::Run();                  \
  }                         \
  ;
//-----------------------------------//
//=---------------------------------=//

//=---------------------------------=//
// Macro:{MINITEST_REGISTER_CASE}
// Brief:{}
//-----------------------------------//
#define MINITEST_REGISTER_CASE(TestName, TestCaseName) \
  const bool REGISTER_INLINE_MINITEST_##TestName##TestCaseName \
  = []{minitest::RegisterTest(#TestName, INLINE_MINITEST_##TestName##TestCaseName);return true;}()
//-----------------------------------//

//=---------------------------------=//
// Macro:{MINITEST_RUN_REGISTERED_MODULE}
// Brief:{}
//-----------------------------------//
#define MINITEST_RUN_REGISTERED_MODULE(TestName) \
  minitest::RunRegisteredTestModule(#TestName)

//=---------------------------------=//
// Macro:{REGISTERED_TEST_MODULE_METHOD}
//-----------------------------------//
#define MINITEST_FUNCTOR_RUN_INLINE(TestName) \
  []()->bool{return minitest::RunRegisteredTestModule(#TestName);}

//=---------------------------------=//
// Macro:{MINITEST_RUN_INLINE}
// Brief:{Runs an inline minitest.}
//-----------------------------------//
#define MINITEST_RUN_INLINE(TestName, TestCaseName) \
  INLINE_MINITEST_##TestName##TestCaseName();       \
//-----------------------------------//
//=---------------------------------=//

//=---------------------------------=//
// Macro:{FINISH_MINITESTS}
// Brief:{ Completes the test suite and prints the result.
//        Must be called right before your main function.
//        This store a boolean true result in MINITESTS_RESULT
//        if all tests passed - else false.
//
//        This does not run the tests, it only prints the result.
//        This must be called LAST, after all tests are defined.
// }
//-----------------------------------//
#define FINISH_MINITESTS                      \
  namespace minitest {                       \
  static const bool minitest_result = []() { \
    return minitest::PrintFailedTestLogs();  \
  }();                                       \
  }  // namespace minitest
//-----------------------------------//
//=---------------------------------=//

//=---------------------------------=//
// Macro:{MINITESTS_RESULT}
// Brief:{
//        Only valid after calling FINISH_MINITESTS.
//        This macro contains the result of the tests as a static const bool.
//        It is set to true if all tests passed, else false.
// }
#define MINITESTS_RESULT minitest::minitest_result
//-----------------------------------//
//=---------------------------------=//

//=---------------------------------=//
// Macro:{MINITESTS_RECORDED}
#define MINITESTS_RECORDED minitest::ViewTestResults()
//-----------------------------------//
//=---------------------------------=//

//=---------------------------------=//
// Macro:{MINITESTS_FAILED}
#define MINITESTS_FAILED minitest::ViewFailedTestResults()
//-----------------------------------//
//=---------------------------------=//

//---------------------------------------------------------------------------//
// Check Macros
//---------------------------------------------------------------------------//

//=---------------------------------=//
// Macro:{MINITEST_INTERNAL_CHECK_METHOD}
// Brief:{Generates a checking macro for a given minitest method.
//        DO NOT USE DIRECTLY.
//        This is used internally by the EXPECT_ macros.
// }
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
//-----------------------------------//
//=---------------------------------=//

//=---------------------------------=//
// Macro:{MINITEST_INTERNAL_ASSERT_METHOD}
// Brief:{Generates am assertion macro for a given minitest method.
//        DO NOT USE DIRECTLY.
//        This is used internally by the ASSERT_ macros.
// }
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

//-----------------------------------//
//=---------------------------------=//

//---------------------------------------------------------------------------//
// Test Macros
//---------------------------------------------------------------------------//

//=---------------------------------=//
// Checks
//=---------------------------------=//

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

//=---------------------------------=//
// Assertions
//=---------------------------------=//

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

//---------------------------------------------------------------------------//
// Logging Test Macros
//---------------------------------------------------------------------------//

//=---------------------------------=//
// Logging checks
//=---------------------------------=//

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

//=---------------------------------=//
// Logging assertions
//=---------------------------------=//

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


//---------------------------------------------------------------------------//
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
//---------------------------------------------------------------------------//
// Author(s): Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.acpp.dev
//---------------------------------------------------------------------------//
// Project: C& Programming Language Environment
// Directory: mini-test
// File: minitest.h
//---------------------------------------------------------------------------//
#endif HEADER_GUARD_CALE_MINITEST_MINITEST_H
//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
// More examples...
//---------------------------------------------------------------------------//

//-----------------------------------//
// ex1. Using inline test cases.
//-----------------------------------//
/*
INLINE_MINITEST(A, B) {
  // Test code here.
  int i;
}
INLINE_END_MINITEST;

void ut_expected() {
  INLINE_MINITEST(A, C) {
    // Test code here.
    int i;
  }
  INLINE_END_MINITEST;

  MINITEST_RUN_INLINE(A, B)

  MINITEST_RUN_INLINE(A, C)
}
*/
//-----------------------------------//
