//=-------------------------------------------------------------------------=//
//---------------------------------------------------------------------------//
// Author: Anton Yashchenko
// Email: acppdev@gmail.com
// Website: https://www.acpp.dev
//---------------------------------------------------------------------------//
// File: minitest.h
//---------------------------------------------------------------------------//
// MIT License
//
// Copyright(c)[2024][Anton Yashchenko]
//
// Permission is hereby granted,free of charge,to any person obtaining a copy of
// this software and associated documentation files(the "Software"),to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so,subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT,TORT OR OTHERWISE, ARISING
// FROM,OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//---------------------------------------------------------------------------//
//=-------------------------------------------------------------------------=//
//
// Mini-Test: A Minimal Unit Testing Framework for C++20
// Requires:
//   - Compiler flag minimum /std:c++20
//   - P0315R4 : Lambdas in unevaluated contexts
//     https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0315r4.pdf
// Brief:
//  This is a single header file unit testing framework. Tests are defined as
//  lambdas in a template parameter, which is executed upon initialization of
//  static variables. As a result the order of tests is not guaranteed but the
//  usual order of static initialization is followed.
//
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
//---------------------------------------------------------------------------//
#ifndef HEADER_GUARD_MINITEST_H
#define HEADER_GUARD_MINITEST_H
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
// Used as a seperator between sections in the output.
static constexpr auto kDashedLine =
    "------------------------------------------------------------"
    "---------------------------------------------------------\n";

// Concept to check if a type is streamable.
// Non-streamable objects are output as their pointer address.
template <typename T>
concept Streamable = requires(std::ostream os, std::decay_t<T> value) {
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
    std::cout << kDashedLine << "[Begin Mini Test] " << test_name << " [Case]"
              << test_case_name << "\n"
              << kDashedLine;
    test_impl();
    std::cout << kDashedLine << "[End Mini Test] " << test_name << " [Case]"
              << test_case_name << "\n"
              << kDashedLine;
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
    std::cout << "[FAIL] Expected TRUE." << std::endl;
    return false;
  }
  return true;
}

// !! DO NOT USE
static inline bool ExpectFalse(bool b) {
  if (b) {
    std::cout << "[FAIL] Expected FALSE." << std::endl;
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
    std::cout << "[FAIL] Expected Equality with: ";
    if constexpr (Streamable<LT> && Streamable<RT>)
      std::cout << ">|" << lhs << " Got: " << rhs << std::endl;
    else
      std::cout << ">|" << &lhs << " Got: " << &rhs << std::endl;
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
    std::cout << "[FAIL] Expected Inequality with";
    if constexpr (Streamable<LT> && Streamable<RT>)
      std::cout << ">|" << lhs << std::endl;
    else
      std::cout << ">|" << &lhs << std::endl;
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
  std::cout << "[FAIL] Expected exception but got none." << std::endl;
  return false;
}

// !! DO NOT USE
static inline bool ExpectNoThrow(auto&& f) {
  try {
    f();
  } catch (...) {
    std::cout << "[FAIL] Expected no exception but one was raised."
              << std::endl;
    return false;
  }
  return true;
}

struct TestResult {
  bool is_test_passed;
  std::string test_name;
  std::string test_case_name;
  std::source_location location;
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
}

static std::vector<std::string> failed_test_logs;
static std::vector<TestResult> gRecordedTestLogs;
static const char* last_failed_test_name = "";
static const char* last_failed_test_case_name = "";

static inline void AddFailedTestLog(
    const std::string& log, const char* test, const char* tcase,
    const std::source_location location = std::source_location::current()) {
  std::stringstream ss;
  ss << "[FAILURE DETECTED] Test: " << std::string(test)
     << " Case: " << std::string(tcase) << " On Check:" << log
     << "\nfile: " << location.file_name() << '(' << location.line() << ':'
     << location.column() << ") `" << location.function_name() << "`: " << '\n';
  failed_test_logs.push_back(ss.str());
}

static inline bool PrintFailedTestLogs() {
  if (failed_test_logs.empty()) {
    std::cout << kDashedLine << "All tests passed.\n" << kDashedLine;
    return true;
  } else {
    std::cout << kDashedLine << "Failed Tests:\n" << kDashedLine;
    for (const auto& log : failed_test_logs) {
      std::cout << log << std::endl;
    }
    std::cout << kDashedLine << "End of Failed Tests:\n" << kDashedLine;
    return false;
  }
}

static inline void RecordTestLog(
    bool passed, const std::string& log, const char* test, const char* tcase,
    const std::source_location location = std::source_location::current()) {
  gRecordedTestLogs.push_back(TestResult{.is_test_passed = passed,
                                         .test_name = std::string(test),
                                         .test_case_name = std::string(tcase),
                                         .location = location});
}

static inline bool PrintRecordedTestLogs() {
  if (gRecordedTestLogs.empty()) {
    std::cout << kDashedLine << "No test were run.\n" << kDashedLine;
    return true;
  } else {
    std::cout << kDashedLine << "Tests:\n" << kDashedLine;
    for (const auto& log : gRecordedTestLogs) {
      std::cout << log << std::endl;
    }
    std::cout << kDashedLine << "End of Tests:\n" << kDashedLine;
    return false;
  }
}

static inline const std::vector<TestResult>& ViewTestResults() {
  return gRecordedTestLogs;
}

static inline const std::vector<std::string>& ViewFailedTestResults() {
  return failed_test_logs;
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
         minitest::last_failed_test_name = #TestName;\
         minitest::last_failed_test_case_name = #TestCaseName;
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
// 	  3.TestFixtureClass : Name of the fixture class.
//                         Must inherit from minitest::Fixture.
//                         See minitest::Fixture for more details.
// }
// Detail:{
// - { } before and after the test case definition is optional
//   but recommended for visibility.
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
  minitest::last_failed_test_case_name = #TestName;\
  minitest::last_failed_test_name = #TestCaseName;\
  (*this).SetUp();
//-----------------------------------//
//=---------------------------------=//

//=---------------------------------=//
// Macro:{END_MINITEST_F}
// Brief:{Closes a fixture test case. ONLY use after MINITEST_F with the SAME
// TestCaseName.
// }
// Parameters:{
//		1.TestCaseName : Name of the last-called test case which created
//    was created with MINITEST_F. MUST be the same as the last-called test
//    case. Or else it will not compile.
//
//    eg. If you called MINITEST_F(MyTest,MyTestCase,MyFixture)
//     then you must call END_MINITEST_F(MyTestCase);
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
// Macro:{MINITEST}
// Brief:{Defines an inline test case to be executed at a later time.
// Always close with 'INLINE_END_MINITEST;'.}
//-----------------------------------//
#define INLINE_MINITEST(TestName, TestCaseName) \
  auto INLINE_MINITEST_##TestName##TestCaseName = []() -> bool {\
  return minitest::Test < []() consteval -> const char* { return #TestName; },\
       []() consteval -> const char* { return #TestCaseName; },\
       decltype([]() -> void {\
         minitest::last_failed_test_name = #TestName;\
         minitest::last_failed_test_case_name = #TestCaseName;
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
// Macro:{INLINE_END_MINITEST}
// Brief:{}
//-----------------------------------//
#define RUN_INLINE_MINITEST(TestName, TestCaseName) \
  INLINE_MINITEST_##TestName##TestCaseName();       \
//-----------------------------------//
//=---------------------------------=//

//=---------------------------------=//
// Macro:{PRINT_MINITESTS}
// Brief:{ Completes the test suite and prints the result.
//        Must be called right before your main function.
//        This store a boolean true result in MINITESTS_RESULT
//        if all tests passed - else false.
//
//        This does not run the tests, it only prints the result.
//        This must be called LAST, after all tests are defined.
// }
//-----------------------------------//
#define PRINT_MINITESTS                      \
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
#define MINITEST_INTERNAL_CHECK_METHOD(method, msg, ...)                 \
  if (!minitest::method(__VA_ARGS__)) {                                  \
    minitest::AddFailedTestLog(msg, minitest::last_failed_test_name,     \
                               minitest::last_failed_test_case_name);    \
    minitest::RecordTestLog(false, msg, minitest::last_failed_test_name, \
                            minitest::last_failed_test_case_name);       \
  } else {                                                               \
    minitest::RecordTestLog(true, msg, minitest::last_failed_test_name,  \
                            minitest::last_failed_test_case_name);       \
  };
//-----------------------------------//
//=---------------------------------=//

//=---------------------------------=//
// Macro:{MINITEST_INTERNAL_ASSERT_METHOD}
// Brief:{Generates am assertion macro for a given minitest method.
//        DO NOT USE DIRECTLY.
//        This is used internally by the ASSERT_ macros.
// }
#define MINITEST_INTERNAL_ASSERT_METHOD(method, msg, ...)             \
  if (!minitest::method(__VA_ARGS__)) {                               \
    minitest::AddFailedTestLog(msg, minitest::last_failed_test_name,  \
                               minitest::last_failed_test_case_name); \
    return;                                                           \
  }
//-----------------------------------//
//=---------------------------------=//

//---------------------------------------------------------------------------//
// Test Macros
//---------------------------------------------------------------------------//

//=---------------------------------=//
// Checks
//=---------------------------------=//

#define EXPECT_TRUE(b)                       \
  MINITEST_INTERNAL_CHECK_METHOD(ExpectTrue, \
                                 "[EXPECTATION FAILED]: EXPECT_TRUE", b)
#define EXPECT_FALSE(b)                       \
  MINITEST_INTERNAL_CHECK_METHOD(ExpectFalse, \
                                 "[EXPECTATION FAILED]: EXPECT_FALSE", b)
#define EXPECT_EQ(a, b)                                                       \
  MINITEST_INTERNAL_CHECK_METHOD(ExpectEq, "[EXPECTATION FAILED]: EXPECT_EQ", \
                                 a, b)
#define EXPECT_NE(a, b)                                                       \
  MINITEST_INTERNAL_CHECK_METHOD(ExpectNe, "[EXPECTATION FAILED]: EXPECT_NE", \
                                 a, b)
#define EXPECT_ANY_THROW(f)                      \
  MINITEST_INTERNAL_CHECK_METHOD(ExpectAnyThrow, \
                                 "[EXPECTATION FAILED]: EXPECT_ANY_THROW", f)
#define EXPECT_NO_THROW(f)                      \
  MINITEST_INTERNAL_CHECK_METHOD(ExpectNoThrow, \
                                 "[EXPECTATION FAILED]: EXPECT_NO_THROW", f)

//=---------------------------------=//
// Assertions
//=---------------------------------=//

#define ASSERT_TRUE(b)                        \
  MINITEST_INTERNAL_ASSERT_METHOD(ExpectTrue, \
                                  "[ASSERTION FAILED]: ASSERT_TRUE", b)
#define ASSERT_FALSE(b)                        \
  MINITEST_INTERNAL_ASSERT_METHOD(ExpectFalse, \
                                  "[ASSERTION FAILED]: ASSERT_FALSE", b)
#define ASSERT_EQ(a, b)                                                      \
  MINITEST_INTERNAL_ASSERT_METHOD(ExpectEq, "[ASSERTION FAILED]: ASSERT_EQ", \
                                  a, b)
#define ASSERT_NE(a, b)                                                      \
  MINITEST_INTERNAL_ASSERT_METHOD(ExpectNe, "[ASSERTION FAILED]: ASSERT_NE", \
                                  a, b)
#define ASSERT_ANY_THROW(f)                       \
  MINITEST_INTERNAL_ASSERT_METHOD(ExpectAnyThrow, \
                                  "[ASSERTION FAILED]: ASSERT_ANY_THROW", f)
#define ASSERT_NO_THROW(f)                       \
  MINITEST_INTERNAL_ASSERT_METHOD(ExpectNoThrow, \
                                  "[ASSERTION FAILED]: ASSERT_NO_THROW", f)

//---------------------------------------------------------------------------//
// Logging Test Macros
//---------------------------------------------------------------------------//

//=---------------------------------=//
// Logging checks
//=---------------------------------=//

#define EXPECT_TRUE_LOG(b, lg) MINITEST_INTERNAL_CHECK_METHOD(ExpectTrue, lg, b)
#define EXPECT_FALSE_LOG(b, lg) \
  MINITEST_INTERNAL_CHECK_METHOD(ExpectFalse, lg, b)
#define EXPECT_EQ_LOG(a, b, lg) \
  MINITEST_INTERNAL_CHECK_METHOD(ExpectEq, lg, a, b)
#define EXPECT_NE_LOG(a, b, lg) \
  MINITEST_INTERNAL_CHECK_METHOD(ExpectNe, lg, a, b)
#define EXPECT_ANY_THROW_LOG(f, lg) \
  MINITEST_INTERNAL_CHECK_METHOD(ExpectAnyThrow, lg, f)
#define EXPECT_NO_THROW_LOG(f, lg) \
  MINITEST_INTERNAL_CHECK_METHOD(ExpectNoThrow, lg, f)

//=---------------------------------=//
// Logging assertions
//=---------------------------------=//

#define ASSERT_TRUE_LOG(b, lg) \
  MINITEST_INTERNAL_ASSERT_METHOD(ExpectTrue, lg, b)
#define ASSERT_FALSE_LOG(b, lg) \
  MINITEST_INTERNAL_ASSERT_METHOD(ExpectFalse, lg, b)
#define ASSERT_EQ_LOG(a, b, lg) \
  MINITEST_INTERNAL_ASSERT_METHOD(ExpectEq, lg, a, b)
#define ASSERT_NE_LOG(a, b, lg) \
  MINITEST_INTERNAL_ASSERT_METHOD(ExpectNe, lg, a, b)
#define ASSERT_ANY_THROW_LOG(f, lg) \
  MINITEST_INTERNAL_ASSERT_METHOD(ExpectAnyThrow, lg, f)
#define ASSERT_NO_THROW_LOG(f, lg) \
  MINITEST_INTERNAL_ASSERT_METHOD(ExpectNoThrow, lg, f)

//=-------------------------------------------------------------------------=//
//---------------------------------------------------------------------------//
// Author: Anton Yashchenko
// Email: acppdev@gmail.com
// Website: https://www.acpp.dev
//---------------------------------------------------------------------------//
// File: minitest.h
//---------------------------------------------------------------------------//
#endif HEADER_GUARD_MINITEST_H
//---------------------------------------------------------------------------//
//=-------------------------------------------------------------------------=//