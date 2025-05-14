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
/// @brief Minitest Dummy Tests. Used in self-validating unit tests for the Minitest library.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup minitest4_unittest
/// @{

#pragma once
#include "minitest.hpp"

// Checks

TEST(DummyUnitTests, CatchUnhandledStdExceptions) {
  throw std::out_of_range("Testing unexpected std exception.");
}

TEST(DummyUnitTests, CatchUnhandledUnknownExceptions) {
  throw "Testing unexpected unknown exception.";
}

TEST(DummyUnitTests, FailExpectTrue) { EXPECT_TRUE(42 == 41); }

TEST(DummyUnitTests, FailExpectFalse) { EXPECT_FALSE(42 != 41); }

TEST(DummyUnitTests, FailExpectEq) { EXPECT_EQ(42, 41); }

TEST(DummyUnitTests, FailExpectNe) { EXPECT_NE(42, 42); }

TEST(DummyUnitTests, FailExpectGt) { EXPECT_GT(42, 43); }

TEST(DummyUnitTests, FailExpectGe) { EXPECT_GE(42, 43); }

TEST(DummyUnitTests, FailExpectLt) { EXPECT_LT(42, 41); }

TEST(DummyUnitTests, FailExpectLe) { EXPECT_LE(42, 41); }

TEST(DummyUnitTests, FailExpectAnyThrow) { EXPECT_ANY_THROW(1 == 1;); }

TEST(DummyUnitTests, FailExpectNoThrow) { EXPECT_NO_THROW(throw "oops!";); }

TEST(DummyUnitTests, FailExpectNoThrowWithStdException) {
  EXPECT_NO_THROW(throw std::exception("Testing expected std exception."););
}

// Asserts

TEST(DummyUnitTests, FailAssertTrue) {
  ASSERT_TRUE(42 == 41);
  EXPECT_TRUE(false && "<unreachable>");
}

TEST(DummyUnitTests, FailAssertFalse) {
  ASSERT_FALSE(42 != 41);
  EXPECT_TRUE(false && "<unreachable>");
}

TEST(DummyUnitTests, FailAssertEq) {
  ASSERT_EQ(42, 41);
  EXPECT_TRUE(false && "<unreachable>");
}

TEST(DummyUnitTests, FailAssertNe) {
  ASSERT_NE(42, 42);
  EXPECT_TRUE(false && "<unreachable>");
}

TEST(DummyUnitTests, FailAssertGt) {
  ASSERT_GT(42, 43);
  EXPECT_TRUE(false && "<unreachable>");
}

TEST(DummyUnitTests, FailAssertGe) {
  ASSERT_GE(42, 43);
  EXPECT_TRUE(false && "<unreachable>");
}

TEST(DummyUnitTests, FailAssertLt) {
  ASSERT_LT(42, 41);
  EXPECT_TRUE(false && "<unreachable>");
}

TEST(DummyUnitTests, FailAssertLe) {
  ASSERT_LE(42, 41);
  EXPECT_TRUE(false && "<unreachable>");
}

TEST(DummyUnitTests, FailAssertAnyThrow) {
  ASSERT_ANY_THROW(1 == 1;);
  EXPECT_TRUE(false && "<unreachable>");
}

TEST(DummyUnitTests, FailAssertNoThrow) {
  ASSERT_NO_THROW(throw "oops!";);
  EXPECT_TRUE(false && "<unreachable>");
}

TEST(DummyUnitTests, FailAssertNoThrowWithStdException) {
  ASSERT_NO_THROW(throw std::exception("Testing expected std exception."););
  EXPECT_TRUE(false && "<unreachable>");
}

// Logging Checks

TEST(DummyUnitTests, FailExpectTrueLog) {
  EXPECT_TRUE_LOG(42 == 41, "bad-log", "good-log");
}

TEST(DummyUnitTests, FailExpectFalseLog) {
  EXPECT_FALSE_LOG(42 != 41, "bad-log", "good-log");
}

TEST(DummyUnitTests, FailExpectEqLog) {
  EXPECT_EQ_LOG(42, 41, "bad-log", "good-log");
}

TEST(DummyUnitTests, FailExpectNeLog) {
  EXPECT_NE_LOG(42, 42, "bad-log", "good-log");
}

TEST(DummyUnitTests, FailExpectGtLog) {
  EXPECT_GT_LOG(42, 43, "bad-log", "good-log");
}

TEST(DummyUnitTests, FailExpectGeLog) {
  EXPECT_GE_LOG(42, 43, "bad-log", "good-log");
}

TEST(DummyUnitTests, FailExpectLtLog) {
  EXPECT_LT_LOG(42, 41, "bad-log", "good-log");
}

TEST(DummyUnitTests, FailExpectLeLog) {
  EXPECT_LE_LOG(42, 41, "bad-log", "good-log");
}

TEST(DummyUnitTests, FailExpectAnyThrowLog) {
  EXPECT_ANY_THROW_LOG(1 == 1;, "bad-log", "good-log");
}

TEST(DummyUnitTests, FailExpectNoThrowLog) {
  EXPECT_NO_THROW_LOG(throw "oops!";, "bad-log", "good-log");
}

TEST(DummyUnitTests, FailExpectNoThrowWithStdExceptionLog) {
  EXPECT_NO_THROW_LOG(throw std::exception("Testing expected std exception.");
                      , "bad-log", "good-log");
}

// Logging Asserts

TEST(DummyUnitTests, FailAssertTrueLog) {
  ASSERT_TRUE_LOG(42 == 41, "bad-log", "good-log");
  EXPECT_TRUE(false && "<unreachable>");
}

TEST(DummyUnitTests, FailAssertFalseLog) {
  ASSERT_FALSE_LOG(42 != 41, "bad-log", "good-log");
  EXPECT_TRUE(false && "<unreachable>");
}

TEST(DummyUnitTests, FailAssertEqLog) {
  ASSERT_EQ_LOG(42, 41, "bad-log", "good-log");
  EXPECT_TRUE(false && "<unreachable>");
}

TEST(DummyUnitTests, FailAssertNeLog) {
  ASSERT_NE_LOG(42, 42, "bad-log", "good-log");
  EXPECT_TRUE(false && "<unreachable>");
}

TEST(DummyUnitTests, FailAssertGtLog) {
  ASSERT_GT_LOG(42, 43, "bad-log", "good-log");
  EXPECT_TRUE(false && "<unreachable>");
}

TEST(DummyUnitTests, FailAssertGeLog) {
  ASSERT_GE_LOG(42, 43, "bad-log", "good-log");
  EXPECT_TRUE(false && "<unreachable>");
}

TEST(DummyUnitTests, FailAssertLtLog) {
  ASSERT_LT_LOG(42, 41, "bad-log", "good-log");
  EXPECT_TRUE(false && "<unreachable>");
}

TEST(DummyUnitTests, FailAssertLeLog) {
  ASSERT_LE_LOG(42, 41, "bad-log", "good-log");
  EXPECT_TRUE(false && "<unreachable>");
}

TEST(DummyUnitTests, FailAssertAnyThrowLog) {
  ASSERT_ANY_THROW_LOG(1 == 1;, "bad-log", "good-log");
  EXPECT_TRUE(false && "<unreachable>");
}

TEST(DummyUnitTests, FailAssertNoThrowLog) {
  ASSERT_NO_THROW_LOG(throw "oops!";, "bad-log", "good-log");
  EXPECT_TRUE(false && "<unreachable>");
}

TEST(DummyUnitTests, FailAssertNoThrowWithStdExceptionLog) {
  ASSERT_NO_THROW_LOG(throw std::exception("Testing expected std exception.");
                      , "bad-log", "good-log");
  EXPECT_TRUE(false && "<unreachable>");
}

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