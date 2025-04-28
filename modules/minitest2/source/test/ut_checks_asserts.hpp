///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2025 Anton Yashchenko
// Licensed under the Apache License, Version 2.0(the "License");
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: Minitest Framework
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup minitest4_unittest
/// @brief Unit testing check and assert macros.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup minitest4_unittest
/// @{

#pragma once
#include "minitest.hpp"
#include "ut_dummy_tests.hpp"

///////////////////////////////////////////////////////////////////////////////
/* Unit test "Minitest" check and assert macros. */
///////////////////////////////////////////////////////////////////////////////

// Assert failed checks are detected and result in a recorded test failure.
// This test recursivley calls RunTest() on the MinitestChecks test suite
// above for each check type. Confirming the library as a whole is able
// to processes all failures correctly.
//
// When a 'check' fails within a test:
//  - RunTests must return false if any checks failed.
//
//  - The result of the ran test is false if any checks failed, true
//    otherwise.
//
//  - Upon a failed check, the appropriate log is appended to the active
//    test's log array.
//
//  - Failures display the checked expression in the failure log similar
//    to C 'assert()' method.
namespace ut_fixtures {
struct FailedCheckDetected : public minitest::Fixture {
  void Run() override {
    using minitest::gFramework;
    gFramework.enable_stdout = false;  // Disable connsole output for dummy test.
    EXPECT_FALSE(gFramework.RunUnitTest(ut));
    gFramework.enable_stdout = true;
    if (EXPECT_EQ(gFramework.GetUnitTest(ut)->log.size(),
                  1))  // There should only be one log.
      EXPECT_EQ(gFramework.GetUnitTest(ut)->log.back(), expected_msg);
    EXPECT_FALSE(gFramework.GetUnitTest(ut)->result);
  }

  FailedCheckDetected(const minitest::UnitTestSignature& t,
                      const std::string& expected_msg)
      : ut(t), expected_msg(expected_msg) {};

  ~FailedCheckDetected() override = default;

  minitest::UnitTestSignature ut{};
  std::string expected_msg{};
};

struct FailedLoggingCheckDetected : public minitest::Fixture {
  void Run() override {
    using minitest::gFramework;
    gFramework.enable_stdout = false; 
    EXPECT_FALSE(gFramework.RunUnitTest(ut));
    gFramework.enable_stdout = true;
    if (EXPECT_EQ(gFramework.GetUnitTest(ut)->log.size(), 2)) {
      auto log_it = gFramework.GetUnitTest(ut)->log.cbegin();
      EXPECT_EQ(*log_it, expected_msg);   
      log_it++;
      EXPECT_EQ(*log_it, "[Fail] bad-log");
    }
    EXPECT_FALSE(gFramework.GetUnitTest(ut)->result);
  }

  FailedLoggingCheckDetected(const minitest::UnitTestSignature& t,
                             const std::string& expected_msg)
      : ut(t), expected_msg(expected_msg) {};

  ~FailedLoggingCheckDetected() override = default;

  minitest::UnitTestSignature ut{};
  std::string expected_msg{};
};

}  // namespace ut_fixtures

TEST_FA(FailedCheckIsDetected, CatchUnhandledStdExceptions,
        ut_fixtures::FailedCheckDetected,
        {"DummyUnitTests", "CatchUnhandledStdExceptions"},
        "An unexpected standard exception was caught, with message: "
        "'Testing unexpected std exception.'");

TEST_FA(FailedCheckIsDetected, CatchUnhandledUnknownExceptions,
        ut_fixtures::FailedCheckDetected,
        {"DummyUnitTests", "CatchUnhandledUnknownExceptions"},
        "An unknown exception was caught.");

TEST_FA(FailedCheckIsDetected, FailExpectTrue, ut_fixtures::FailedCheckDetected,
        {"DummyUnitTests", "FailExpectTrue"},
        "[Fail] Expected TRUE boolean value.\n\t--[Condition]: 42 == 41");

TEST_FA(FailedCheckIsDetected, FailExpectFalse,
        ut_fixtures::FailedCheckDetected, {"DummyUnitTests", "FailExpectFalse"},
        "[Fail] Expected FALSE boolean value.\n\t--[Condition]: 42 != 41");

TEST_FA(FailedCheckIsDetected, FailExpectEq, ut_fixtures::FailedCheckDetected,
        {"DummyUnitTests", "FailExpectEq"},
        "[Fail] Expected equality between values.\n\t--[Left]: 42"
        "\n\t--[Right]: 41\n\t--[Left-Value]: 42"
        "\n\t--[Right-Value]: 41");

TEST_FA(FailedCheckIsDetected, FailExpectNe, ut_fixtures::FailedCheckDetected,
        {"DummyUnitTests", "FailExpectNe"},
        "[Fail] Expected inequality between values.\n\t--[Left]: 42"
        "\n\t--[Right]: 42\n\t--[Left-Value]: 42"
        "\n\t--[Right-Value]: 42");

TEST_FA(FailedCheckIsDetected, FailExpectGt, ut_fixtures::FailedCheckDetected,
        {"DummyUnitTests", "FailExpectGt"},
        "[Fail] Expected left to be greater than right.\n\t--[Left]: 42"
        "\n\t--[Right]: 43\n\t--[Left-Value]: 42"
        "\n\t--[Right-Value]: 43");

TEST_FA(FailedCheckIsDetected, FailExpectGe, ut_fixtures::FailedCheckDetected,
        {"DummyUnitTests", "FailExpectGe"},
        "[Fail] Expected left to be greater than, or equal, to "
        "right.\n\t--[Left]: 42"
        "\n\t--[Right]: 43\n\t--[Left-Value]: 42"
        "\n\t--[Right-Value]: 43");

TEST_FA(FailedCheckIsDetected, FailExpectLt, ut_fixtures::FailedCheckDetected,
        {"DummyUnitTests", "FailExpectLt"},
        "[Fail] Expected left to be less than right.\n\t--[Left]: 42"
        "\n\t--[Right]: 41\n\t--[Left-Value]: 42"
        "\n\t--[Right-Value]: 41");

TEST_FA(FailedCheckIsDetected, FailExpectLe, ut_fixtures::FailedCheckDetected,
        {"DummyUnitTests", "FailExpectLe"},
        "[Fail] Expected left to be less than, or equal, to "
        "right.\n\t--[Left]: 42"
        "\n\t--[Right]: 41\n\t--[Left-Value]: 42"
        "\n\t--[Right-Value]: 41");

TEST_FA(FailedCheckIsDetected, FailExpectAnyThrow,
        ut_fixtures::FailedCheckDetected,
        {"DummyUnitTests", "FailExpectAnyThrow"},
        "[Fail] Expected any throw (exception). Expression: 1 == 1;");

TEST_FA(FailedCheckIsDetected, FailExpectNoThrow,
        ut_fixtures::FailedCheckDetected,
        {"DummyUnitTests", "FailExpectNoThrow"},
        "[Fail] Expected no exceptions to be thrown. Got unknown exception. "
        "Expression: throw \"oops!\";");

TEST_FA(
    FailedCheckIsDetected, FailExpectNoThrowCatchStd,
    ut_fixtures::FailedCheckDetected,
    {"DummyUnitTests", "FailExpectNoThrowWithStdException"},
    "[Fail] Expected no exceptions to be thrown. Got standard exception with "
    "message: 'Testing expected std exception.'\nExpression: throw "
    "std::exception(\"Testing expected std exception.\");");

TEST_FA(
    FailedCheckIsDetected, FailAssertTrue, ut_fixtures::FailedCheckDetected,
    {"DummyUnitTests", "FailAssertTrue"},
    "[Fail][Assert] Expected TRUE boolean value.\n\t--[Condition]: 42 == 41");

TEST_FA(
    FailedCheckIsDetected, FailAssertFalse, ut_fixtures::FailedCheckDetected,
    {"DummyUnitTests", "FailAssertFalse"},
    "[Fail][Assert] Expected FALSE boolean value.\n\t--[Condition]: 42 != 41");

TEST_FA(FailedCheckIsDetected, FailAssertEq, ut_fixtures::FailedCheckDetected,
        {"DummyUnitTests", "FailAssertEq"},
        "[Fail][Assert] Expected equality between values.\n\t--[Left]: 42"
        "\n\t--[Right]: 41\n\t--[Left-Value]: 42"
        "\n\t--[Right-Value]: 41");

TEST_FA(FailedCheckIsDetected, FailAssertNe, ut_fixtures::FailedCheckDetected,
        {"DummyUnitTests", "FailAssertNe"},
        "[Fail][Assert] Expected inequality between values.\n\t--[Left]: 42"
        "\n\t--[Right]: 42\n\t--[Left-Value]: 42"
        "\n\t--[Right-Value]: 42");

TEST_FA(FailedCheckIsDetected, FailAssertGt, ut_fixtures::FailedCheckDetected,
        {"DummyUnitTests", "FailAssertGt"},
        "[Fail][Assert] Expected left to be greater than right.\n\t--[Left]: 42"
        "\n\t--[Right]: 43\n\t--[Left-Value]: 42"
        "\n\t--[Right-Value]: 43");

TEST_FA(FailedCheckIsDetected, FailAssertGe, ut_fixtures::FailedCheckDetected,
        {"DummyUnitTests", "FailAssertGe"},
        "[Fail][Assert] Expected left to be greater than, or equal, to "
        "right.\n\t--[Left]: 42"
        "\n\t--[Right]: 43\n\t--[Left-Value]: 42"
        "\n\t--[Right-Value]: 43");

TEST_FA(FailedCheckIsDetected, FailAssertLt, ut_fixtures::FailedCheckDetected,
        {"DummyUnitTests", "FailAssertLt"},
        "[Fail][Assert] Expected left to be less than right.\n\t--[Left]: 42"
        "\n\t--[Right]: 41\n\t--[Left-Value]: 42"
        "\n\t--[Right-Value]: 41");

TEST_FA(FailedCheckIsDetected, FailAssertLe, ut_fixtures::FailedCheckDetected,
        {"DummyUnitTests", "FailAssertLe"},
        "[Fail][Assert] Expected left to be less than, or equal, to "
        "right.\n\t--[Left]: 42"
        "\n\t--[Right]: 41\n\t--[Left-Value]: 42"
        "\n\t--[Right-Value]: 41");

TEST_FA(FailedCheckIsDetected, FailAssertAnyThrow,
        ut_fixtures::FailedCheckDetected,
        {"DummyUnitTests", "FailAssertAnyThrow"},
        "[Fail][Assert] Expected any throw (exception). Expression: 1 == 1;");

TEST_FA(FailedCheckIsDetected, FailAssertNoThrow,
        ut_fixtures::FailedCheckDetected,
        {"DummyUnitTests", "FailAssertNoThrow"},
        "[Fail][Assert] Expected no exceptions to be thrown. Got unknown "
        "exception. Expression: throw \"oops!\";");

TEST_FA(FailedCheckIsDetected, FailAssertNoThrowCatchStd,
        ut_fixtures::FailedCheckDetected,
        {"DummyUnitTests", "FailAssertNoThrowWithStdException"},
        "[Fail][Assert] Expected no exceptions to be thrown. Got standard "
        "exception with "
        "message: 'Testing expected std exception.'\nExpression: throw "
        "std::exception(\"Testing expected std exception.\");");

// Logging Checks

TEST_FA(FailedCheckIsDetected, FailExpectTrueLog, ut_fixtures::FailedLoggingCheckDetected,
        {"DummyUnitTests", "FailExpectTrueLog"},
        "[Fail] Expected TRUE boolean value.\n\t--[Condition]: 42 == 41");

TEST_FA(FailedCheckIsDetected, FailExpectFalseLog,
        ut_fixtures::FailedLoggingCheckDetected, {"DummyUnitTests", "FailExpectFalseLog"},
        "[Fail] Expected FALSE boolean value.\n\t--[Condition]: 42 != 41");

TEST_FA(FailedCheckIsDetected, FailExpectEqLog, ut_fixtures::FailedLoggingCheckDetected,
        {"DummyUnitTests", "FailExpectEqLog"},
        "[Fail] Expected equality between values.\n\t--[Left]: 42"
        "\n\t--[Right]: 41\n\t--[Left-Value]: 42"
        "\n\t--[Right-Value]: 41");

TEST_FA(FailedCheckIsDetected, FailExpectNeLog, ut_fixtures::FailedLoggingCheckDetected,
        {"DummyUnitTests", "FailExpectNeLog"},
        "[Fail] Expected inequality between values.\n\t--[Left]: 42"
        "\n\t--[Right]: 42\n\t--[Left-Value]: 42"
        "\n\t--[Right-Value]: 42");

TEST_FA(FailedCheckIsDetected, FailExpectGtLog, ut_fixtures::FailedLoggingCheckDetected,
        {"DummyUnitTests", "FailExpectGtLog"},
        "[Fail] Expected left to be greater than right.\n\t--[Left]: 42"
        "\n\t--[Right]: 43\n\t--[Left-Value]: 42"
        "\n\t--[Right-Value]: 43");

TEST_FA(FailedCheckIsDetected, FailExpectGeLog, ut_fixtures::FailedLoggingCheckDetected,
        {"DummyUnitTests", "FailExpectGeLog"},
        "[Fail] Expected left to be greater than, or equal, to "
        "right.\n\t--[Left]: 42"
        "\n\t--[Right]: 43\n\t--[Left-Value]: 42"
        "\n\t--[Right-Value]: 43");

TEST_FA(FailedCheckIsDetected, FailExpectLtLog, ut_fixtures::FailedLoggingCheckDetected,
        {"DummyUnitTests", "FailExpectLtLog"},
        "[Fail] Expected left to be less than right.\n\t--[Left]: 42"
        "\n\t--[Right]: 41\n\t--[Left-Value]: 42"
        "\n\t--[Right-Value]: 41");

TEST_FA(FailedCheckIsDetected, FailExpectLeLog, ut_fixtures::FailedLoggingCheckDetected,
        {"DummyUnitTests", "FailExpectLeLog"},
        "[Fail] Expected left to be less than, or equal, to "
        "right.\n\t--[Left]: 42"
        "\n\t--[Right]: 41\n\t--[Left-Value]: 42"
        "\n\t--[Right-Value]: 41");

TEST_FA(FailedCheckIsDetected, FailExpectAnyThrowLog,
        ut_fixtures::FailedLoggingCheckDetected,
        {"DummyUnitTests", "FailExpectAnyThrowLog"},
        "[Fail] Expected any throw (exception). Expression: 1 == 1;");

TEST_FA(FailedCheckIsDetected, FailExpectNoThrowLog,
        ut_fixtures::FailedLoggingCheckDetected,
        {"DummyUnitTests", "FailExpectNoThrowLog"},
        "[Fail] Expected no exceptions to be thrown. Got unknown exception. "
        "Expression: throw \"oops!\";");

TEST_FA(
    FailedCheckIsDetected, FailExpectNoThrowCatchStdLog,
    ut_fixtures::FailedLoggingCheckDetected,
    {"DummyUnitTests", "FailExpectNoThrowWithStdExceptionLog"},
    "[Fail] Expected no exceptions to be thrown. Got standard exception with "
    "message: 'Testing expected std exception.'\nExpression: throw "
    "std::exception(\"Testing expected std exception.\");");

// Logging Asserts

TEST_FA(
    FailedCheckIsDetected, FailAssertTrueLog, ut_fixtures::FailedLoggingCheckDetected,
    {"DummyUnitTests", "FailAssertTrueLog"},
    "[Fail][Assert] Expected TRUE boolean value.\n\t--[Condition]: 42 == 41");

TEST_FA(
    FailedCheckIsDetected, FailAssertFalseLog, ut_fixtures::FailedLoggingCheckDetected,
    {"DummyUnitTests", "FailAssertFalseLog"},
    "[Fail][Assert] Expected FALSE boolean value.\n\t--[Condition]: 42 != 41");

TEST_FA(FailedCheckIsDetected, FailAssertEqLog, ut_fixtures::FailedLoggingCheckDetected,
        {"DummyUnitTests", "FailAssertEqLog"},
        "[Fail][Assert] Expected equality between values.\n\t--[Left]: 42"
        "\n\t--[Right]: 41\n\t--[Left-Value]: 42"
        "\n\t--[Right-Value]: 41");

TEST_FA(FailedCheckIsDetected, FailAssertNeLog, ut_fixtures::FailedLoggingCheckDetected,
        {"DummyUnitTests", "FailAssertNeLog"},
        "[Fail][Assert] Expected inequality between values.\n\t--[Left]: 42"
        "\n\t--[Right]: 42\n\t--[Left-Value]: 42"
        "\n\t--[Right-Value]: 42");

TEST_FA(FailedCheckIsDetected, FailAssertGtLog, ut_fixtures::FailedLoggingCheckDetected,
        {"DummyUnitTests", "FailAssertGtLog"},
        "[Fail][Assert] Expected left to be greater than right.\n\t--[Left]: 42"
        "\n\t--[Right]: 43\n\t--[Left-Value]: 42"
        "\n\t--[Right-Value]: 43");

TEST_FA(FailedCheckIsDetected, FailAssertGeLog, ut_fixtures::FailedLoggingCheckDetected,
        {"DummyUnitTests", "FailAssertGeLog"},
        "[Fail][Assert] Expected left to be greater than, or equal, to "
        "right.\n\t--[Left]: 42"
        "\n\t--[Right]: 43\n\t--[Left-Value]: 42"
        "\n\t--[Right-Value]: 43");

TEST_FA(FailedCheckIsDetected, FailAssertLtLog, ut_fixtures::FailedLoggingCheckDetected,
        {"DummyUnitTests", "FailAssertLtLog"},
        "[Fail][Assert] Expected left to be less than right.\n\t--[Left]: 42"
        "\n\t--[Right]: 41\n\t--[Left-Value]: 42"
        "\n\t--[Right-Value]: 41");

TEST_FA(FailedCheckIsDetected, FailAssertLeLog, ut_fixtures::FailedLoggingCheckDetected,
        {"DummyUnitTests", "FailAssertLeLog"},
        "[Fail][Assert] Expected left to be less than, or equal, to "
        "right.\n\t--[Left]: 42"
        "\n\t--[Right]: 41\n\t--[Left-Value]: 42"
        "\n\t--[Right-Value]: 41");

TEST_FA(FailedCheckIsDetected, FailAssertAnyThrowLog,
        ut_fixtures::FailedLoggingCheckDetected,
        {"DummyUnitTests", "FailAssertAnyThrowLog"},
        "[Fail][Assert] Expected any throw (exception). Expression: 1 == 1;");

TEST_FA(FailedCheckIsDetected, FailAssertNoThrowLog,
        ut_fixtures::FailedLoggingCheckDetected,
        {"DummyUnitTests", "FailAssertNoThrowLog"},
        "[Fail][Assert] Expected no exceptions to be thrown. Got unknown "
        "exception. Expression: throw \"oops!\";");

TEST_FA(FailedCheckIsDetected, FailAssertNoThrowCatchStdLog,
        ut_fixtures::FailedLoggingCheckDetected,
        {"DummyUnitTests", "FailAssertNoThrowWithStdExceptionLog"},
        "[Fail][Assert] Expected no exceptions to be thrown. Got standard "
        "exception with "
        "message: 'Testing expected std exception.'\nExpression: throw "
        "std::exception(\"Testing expected std exception.\");");
/// @} // end of minitest4_unittest

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