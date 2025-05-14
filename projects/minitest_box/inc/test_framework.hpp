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
/// @brief Minitest internal implementation.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
// clang-format off
#include "common.hpp"
#include "fixture.hpp"
#include "form.hpp"
#include "unit_test.hpp"
// clang-format on

namespace minitest {

/// @addtogroup minitest2_framework_impl
/// @{

/// Dynamic array which stores unit test definitions.
using UnitTestArray = vector<UnitTest>;

/// Map cross referencing unit test names to indexes in a unit test array.
using UnitTestIndexMap = map<UnitTestSignature, size_t, std::less<>>;

struct MinitestFramework {  // NOSONAR - class is big because it models entire
                            // lib.
  UnitTestArray& tests;
  UnitTestIndexMap test_indices{};  // maps test names to index in tests array.
  UnitTestArray::iterator curr_test{tests.end()};
  bool enable_stdout{true};
  reference_wrapper<ostream> target_stdout{std::ref(std::cout)};

 public:
  MinitestFramework() = delete;
  explicit MinitestFramework(UnitTestArray& unit_tests) : tests(unit_tests) {}

  ostream& TargetStdout() const { return target_stdout.get(); }

  ostream& TargetStdout(ostream& oss) {
    target_stdout = std::ref(oss);
    return target_stdout.get();
  }

  UnitTestArray::iterator GetUnitTest(const string& suite_name,
                                      const string& test_name) {
    return tests.begin() + test_indices.at({suite_name, test_name});
  }

  UnitTestArray::iterator GetUnitTest(const UnitTestSignature& ut_signature) {
    return tests.begin() + test_indices.at(ut_signature);
  }

  void RecordFailure(const string& msg) const {
    if (enable_stdout) target_stdout.get() << msg << std::endl;
    curr_test->log.emplace_back(msg);
    curr_test->result = false;
  }

  void RecordMessage(const string& msg) const {
    if (enable_stdout) target_stdout.get() << msg << std::endl;
    curr_test->log.emplace_back(msg);
  }

  void RegisterTest(
      const string& suite, const string& name,
      const UnitTestFunction& impl = [] { /* empty unit test */ },
      bool result = true, const UnitTestLog& log = {}) {
    using std::make_pair;
    using std::move;
    auto signature = UnitTestSignature{suite, name};
    assert(test_indices.count(signature) == 0 &&
           "Failed to register existing test, name is not unique.");
    tests.push_back(UnitTest{tests.size(), suite, name, impl, result, log});
    test_indices.emplace(make_pair(move(signature), tests.size() - 1));
  }

  /// Sets a recorded test in the global test map as the current active test and
  /// runs it. Returns true if current test ran with no errors.
  bool SetTestActiveAndRun(const UnitTestArray::iterator& it) {
    UnitTestArray::iterator prev_state = curr_test;  // Store previous state.
    curr_test = it;                                  // Set as active test.
    bool is_test_passed = true;  // Passed unless an error occurs.
    it->result =
        true;  // Reset the initial state of this test in the results map.

    // Print [Run] test header.
    if (enable_stdout)
      target_stdout.get() << FmtRunTest(it->suite, it->name) << std::endl;

    // Surround in try block in-case an unexpected user exception occurs.
    try {
      it->fn();                          // Run the user method.
    } catch (const std::exception& e) {  // NOSONAR
      RecordFailure(FmtStdExceptionFail(e.what()));
      is_test_passed = false;
    } catch (...) {  // NOSONAR
      RecordFailure(FmtUnknownExceptionFail());
      is_test_passed = false;
    }

    // Check if any user checks failed for this test, if so mark test as failed.
    if (!it->result) is_test_passed = false;

    // Restore previous test state.
    curr_test = prev_state;

    if (!is_test_passed) {
      return false;
    } else {
      if (enable_stdout)
        target_stdout.get() << FmtPassTest(it->suite, it->name) << std::endl;
      return true;
    }
  }

  /// Run all recorded tests.
  /// If a fail occurred during any test, the end result of RunTests is false.
  bool RunAllTests() {
    bool is_failure_detected = false;
    for (auto it = tests.begin(); it != tests.end(); it++)
      if (!SetTestActiveAndRun(it)) is_failure_detected = true;
    return !is_failure_detected;
  }

  /// Run all recorded tests in a given suite.
  bool RunTestSuite(const string& suite_name) {
    bool is_failure_detected = false;
    for (auto it = tests.begin(); it != tests.end(); it++)
      if (it->suite == suite_name && !SetTestActiveAndRun(it))
        is_failure_detected = true;
    return !is_failure_detected;
  }

  /// Run a test with a given suite and test name.
  bool RunUnitTest(const string& suite_name, const string& test_name) {
    return SetTestActiveAndRun(GetUnitTest(suite_name, test_name));
  }

  /// Run a test with a given a unit-test signature.
  bool RunUnitTest(const UnitTestSignature& ut_signature) {
    return SetTestActiveAndRun(GetUnitTest(ut_signature));
  }

  /// Run all tests listed in a vector of strings, by name, from a given suite.
  bool RunUnitTestRange(const string& suite_name,
                        vector<string>::const_iterator test_list_beg,
                        vector<string>::const_iterator test_list_end) {
    bool is_failure_detected = false;
    for (auto it = tests.begin(); it != tests.end(); it++)
      if (it->suite == suite_name &&
          std::any_of(test_list_beg, test_list_end,
                      [&it](const auto& t) { return it->name == t; }) &&
          !SetTestActiveAndRun(it))
        is_failure_detected = true;
    return !is_failure_detected;
  }

  bool RunTests() { return RunAllTests(); }

  bool RunTests(const string& suite_name) {
    return RunTestSuite(suite_name);
  }

  bool RunTests(const string& suite_name, const string& test_name) {
    return RunUnitTest(suite_name, test_name);
  };

  bool RunTests(const string& suite_name,
                       vector<string>::const_iterator test_list_beg,
                       vector<string>::const_iterator test_list_end) {
    return RunUnitTestRange(suite_name, test_list_beg,
                                       test_list_end);
  };

  bool RunTests(const UnitTestSignature& ut_signature) {
    return RunUnitTest(ut_signature);
  }

  /// Command line interface main method.
  /// @note unlike 'RunTests' this method returns 0 on success, non-zero on
  /// failure.
  int CliMain(int argc, char* argv[]) {
    vector<string> args{argv, argv + argc};
    // Handle special case cli args.
    if (args.size() > 1) {
      // Scan mode
      if (args[1] == "--scan") {
        std::stringstream ss{""};
        if (args.size() > 2) {
          for (auto& tcase : tests) {
            if (std::any_of(args.begin() + 2, args.end(),
                            [&tcase](auto& t) { return t == tcase.suite; })) {
              ss << tcase.suite << ";" << tcase.name << ";";
            }
          }
        } else {
          for (auto& tcase : tests) {
            ss << tcase.suite << ";" << tcase.name << ";";
          }
        }
        std::cout << ss.str();
        return 0;
      }
    }
    // else run tests...
    switch (args.size()) {
      case 1:
        return !RunAllTests();
      case 2:
        return !RunTestSuite(args[1]);
      case 3:
        return !RunUnitTest(args[1], args[2]);
      default:
        return !RunUnitTestRange(args[1], args.cbegin() + 2, args.cend());
    }
  }

  template <class LhsT, class RhsT, class StrT0, class StrT1, class EqualityT,
            class FormatFnT>
  inline bool GenericExpectEquality(LhsT&& lhs, RhsT&& rhs, StrT0&& lhs_code,
                                    StrT1&& rhs_code, EqualityT&& equality_fn,
                                    FormatFnT&& fail_format) {
    if (equality_fn(lhs, rhs)) {
      return true;
    }
    RecordFailure(FmtTagFail(
        fail_format(forward<StrT0>(lhs_code), forward<StrT1>(rhs_code),
                    OverloadToString(forward<LhsT>(lhs)),
                    OverloadToString(forward<RhsT>(rhs)))));
    return false;
  }

  template <class LhsT, class RhsT, class StrT0, class StrT1, class EqualityT,
            class FormatFnT>
  inline bool GenericAssertEquality(LhsT&& lhs, RhsT&& rhs, StrT0&& lhs_code,
                                    StrT1&& rhs_code, EqualityT&& equality_fn,
                                    FormatFnT&& fail_format) {
    if (equality_fn(lhs, rhs)) {
      return true;
    }
    RecordFailure(FmtTagFail(FmtTagAssert(
        fail_format(forward<StrT0>(lhs_code), forward<StrT1>(rhs_code),
                    OverloadToString(forward<LhsT>(lhs)),
                    OverloadToString(forward<RhsT>(rhs))))));
    return false;
  }

  template <class LhsT, class RhsT, class StrT0, class StrT1, class GoodLogT,
            class BadLogT, class EqualityT, class FormatFnT>
  inline bool GenericExpectEqualityLog(LhsT&& lhs, RhsT&& rhs, StrT0&& lhs_code,
                                       StrT1&& rhs_code, BadLogT&& badlog,
                                       GoodLogT&& goodlog,
                                       EqualityT&& equality_fn,
                                       FormatFnT&& fail_format) {
    if (equality_fn(lhs, rhs)) {
      RecordMessage(forward<GoodLogT>(goodlog));
      return true;
    }
    RecordFailure(FmtTagFail(
        fail_format(forward<StrT0>(lhs_code), forward<StrT1>(rhs_code),
                    OverloadToString(forward<LhsT>(lhs)),
                    OverloadToString(forward<RhsT>(rhs)))));
    RecordFailure(FmtTagFail(forward<BadLogT>(badlog)));
    return false;
  }

  template <class LhsT, class RhsT, class StrT0, class StrT1, class GoodLogT,
            class BadLogT, class EqualityT, class FormatFnT>
  inline bool GenericAssertEqualityLog(LhsT&& lhs, RhsT&& rhs, StrT0&& lhs_code,
                                       StrT1&& rhs_code, BadLogT&& badlog,
                                       GoodLogT&& goodlog,
                                       EqualityT&& equality_fn,
                                       FormatFnT&& fail_format) {
    if (equality_fn(lhs, rhs)) {
      RecordMessage(forward<GoodLogT>(goodlog));
      return true;
    }
    RecordFailure(FmtTagFail(FmtTagAssert(
        fail_format(forward<StrT0>(lhs_code), forward<StrT1>(rhs_code),
                    OverloadToString(forward<LhsT>(lhs)),
                    OverloadToString(forward<RhsT>(rhs))))));
    RecordFailure(FmtTagFail(forward<BadLogT>(badlog)));
    return false;
  }

  template <class StrT>
  bool ExpectTrue(bool v, StrT&& value_code) {
    if (!v) {
      RecordFailure(FmtTagFail(FmtExpectTrue(forward<StrT>(value_code))));
      return false;
    }
    return true;
  }

  template <class StrT>
  bool ExpectFalse(bool v, StrT&& value_code) {
    if (v) {
      RecordFailure(FmtTagFail(FmtExpectFalse(forward<StrT>(value_code))));
      return false;
    }
    return true;
  }

  template <class LhsT, class RhsT, class StrT0, class StrT1>
  bool ExpectEq(LhsT&& lhs, RhsT&& rhs, StrT0&& lhs_code, StrT1&& rhs_code) {
    return GenericExpectEquality(
        forward<LhsT>(lhs), forward<RhsT>(rhs), forward<StrT0>(lhs_code),
        forward<StrT1>(rhs_code), std::equal_to<>{}, FmtExpectEq);
  }

  template <class LhsT, class RhsT, class StrT0, class StrT1>
  bool ExpectNe(LhsT&& lhs, RhsT&& rhs, StrT0&& lhs_code, StrT1&& rhs_code) {
    return GenericExpectEquality(
        forward<LhsT>(lhs), forward<RhsT>(rhs), forward<StrT0>(lhs_code),
        forward<StrT1>(rhs_code), std::not_equal_to<>{}, FmtExpectNe);
  }

  template <class LhsT, class RhsT, class StrT0, class StrT1>
  bool ExpectGt(LhsT&& lhs, RhsT&& rhs, StrT0&& lhs_code, StrT1&& rhs_code) {
    return GenericExpectEquality(
        forward<LhsT>(lhs), forward<RhsT>(rhs), forward<StrT0>(lhs_code),
        forward<StrT1>(rhs_code), std::greater<>{}, FmtExpectGt);
  }

  template <class LhsT, class RhsT, class StrT0, class StrT1>
  bool ExpectGe(LhsT&& lhs, RhsT&& rhs, StrT0&& lhs_code, StrT1&& rhs_code) {
    return GenericExpectEquality(
        forward<LhsT>(lhs), forward<RhsT>(rhs), forward<StrT0>(lhs_code),
        forward<StrT1>(rhs_code), std::greater_equal<>{}, FmtExpectGe);
  }

  template <class LhsT, class RhsT, class StrT0, class StrT1>
  bool ExpectLt(LhsT&& lhs, RhsT&& rhs, StrT0&& lhs_code, StrT1&& rhs_code) {
    return GenericExpectEquality(
        forward<LhsT>(lhs), forward<RhsT>(rhs), forward<StrT0>(lhs_code),
        forward<StrT1>(rhs_code), std::less<>{}, FmtExpectLt);
  }

  template <class LhsT, class RhsT, class StrT0, class StrT1>
  bool ExpectLe(LhsT&& lhs, RhsT&& rhs, StrT0&& lhs_code, StrT1&& rhs_code) {
    return GenericExpectEquality(
        forward<LhsT>(lhs), forward<RhsT>(rhs), forward<StrT0>(lhs_code),
        forward<StrT1>(rhs_code), std::less_equal<>{}, FmtExpectLe);
  }

  template <class FnT, class StrT>
  bool ExpectAnyThrow(FnT&& fn, StrT&& fn_code) {
    try {
      fn();
    } catch (...) {  // NOSONAR
      return true;
    }
    RecordFailure(FmtTagFail(FmtExpectAnyThrow(forward<StrT>(fn_code))));
    return false;
  }

  template <class FnT, class StrT>
  bool ExpectNoThrow(FnT&& fn, StrT&& fn_code) {
    try {
      fn();
    } catch (const std::exception& e) {  // NOSONAR
      RecordFailure(
          FmtTagFail(FmtExpectNoThrowStd(forward<StrT>(fn_code), e.what())));
      return false;
    } catch (...) {
      RecordFailure(FmtTagFail(FmtExpectNoThrow(forward<StrT>(fn_code))));
      return false;
    }
    return true;
  }

  template <class CondT, class ValT, class StrT0, class StrT1>
  enable_if_t<iIsInvokableV<bool, CondT, ValT>, bool> ExpectThat(
      CondT&& condition, ValT&& v, StrT0&& cond_code, StrT1&& val_code) {
    if (!condition(forward<ValT>(v))) {
      RecordFailure(FmtTagFail(
          FmtExpectThat(forward<StrT0>(cond_code), forward<StrT1>(val_code))));
      return false;
    }
    return true;
  }

  template <class StrT>
  bool AssertTrue(bool v, StrT&& value_code) {
    if (!v) {
      RecordFailure(
          FmtTagFail(FmtTagAssert(FmtExpectTrue(forward<StrT>(value_code)))));
      return false;
    }
    return true;
  }

  template <class StrT>
  bool AssertFalse(bool v, StrT&& value_code) {
    if (v) {
      RecordFailure(
          FmtTagFail(FmtTagAssert(FmtExpectFalse(forward<StrT>(value_code)))));
      return false;
    }
    return true;
  }

  template <class LhsT, class RhsT, class StrT0, class StrT1>
  bool AssertEq(LhsT&& lhs, RhsT&& rhs, StrT0&& lhs_code, StrT1&& rhs_code) {
    return GenericAssertEquality(
        forward<LhsT>(lhs), forward<RhsT>(rhs), forward<StrT0>(lhs_code),
        forward<StrT1>(rhs_code), std::equal_to<>{}, FmtExpectEq);
  }

  template <class LhsT, class RhsT, class StrT0, class StrT1>
  bool AssertNe(LhsT&& lhs, RhsT&& rhs, StrT0&& lhs_code, StrT1&& rhs_code) {
    return GenericAssertEquality(
        forward<LhsT>(lhs), forward<RhsT>(rhs), forward<StrT0>(lhs_code),
        forward<StrT1>(rhs_code), std::not_equal_to<>{}, FmtExpectNe);
  }

  template <class LhsT, class RhsT, class StrT0, class StrT1>
  bool AssertGt(LhsT&& lhs, RhsT&& rhs, StrT0&& lhs_code, StrT1&& rhs_code) {
    return GenericAssertEquality(
        forward<LhsT>(lhs), forward<RhsT>(rhs), forward<StrT0>(lhs_code),
        forward<StrT1>(rhs_code), std::greater<>{}, FmtExpectGt);
  }

  template <class LhsT, class RhsT, class StrT0, class StrT1>
  bool AssertGe(LhsT&& lhs, RhsT&& rhs, StrT0&& lhs_code, StrT1&& rhs_code) {
    return GenericAssertEquality(
        forward<LhsT>(lhs), forward<RhsT>(rhs), forward<StrT0>(lhs_code),
        forward<StrT1>(rhs_code), std::greater_equal<>{}, FmtExpectGe);
  }

  template <class LhsT, class RhsT, class StrT0, class StrT1>
  bool AssertLt(LhsT&& lhs, RhsT&& rhs, StrT0&& lhs_code, StrT1&& rhs_code) {
    return GenericAssertEquality(
        forward<LhsT>(lhs), forward<RhsT>(rhs), forward<StrT0>(lhs_code),
        forward<StrT1>(rhs_code), std::less<>{}, FmtExpectLt);
  }

  template <class LhsT, class RhsT, class StrT0, class StrT1>
  bool AssertLe(LhsT&& lhs, RhsT&& rhs, StrT0&& lhs_code, StrT1&& rhs_code) {
    return GenericAssertEquality(
        forward<LhsT>(lhs), forward<RhsT>(rhs), forward<StrT0>(lhs_code),
        forward<StrT1>(rhs_code), std::less_equal<>{}, FmtExpectLe);
  }

  template <class FnT, class StrT>
  bool AssertAnyThrow(FnT&& fn, StrT&& fn_code) {
    try {
      fn();
    } catch (...) {  // NOSONAR
      return true;
    }
    RecordFailure(
        FmtTagFail(FmtTagAssert(FmtExpectAnyThrow(forward<StrT>(fn_code)))));
    return false;
  }

  template <class FnT, class StrT>
  bool AssertNoThrow(FnT&& fn, StrT&& fn_code) {
    try {
      fn();
    } catch (const std::exception& e) {  // NOSONAR
      RecordFailure(FmtTagFail(
          FmtTagAssert(FmtExpectNoThrowStd(forward<StrT>(fn_code), e.what()))));
      return false;
    } catch (...) {
      RecordFailure(
          FmtTagFail(FmtTagAssert(FmtExpectNoThrow(forward<StrT>(fn_code)))));
      return false;
    }
    return true;
  }

  template <class CondT, class ValT, class StrT0, class StrT1>
  enable_if_t<iIsInvokableV<bool, CondT, ValT>, bool> AssertThat(
      CondT&& condition, ValT&& v, StrT0&& cond_code, StrT1&& val_code) {
    if (!condition(forward<ValT>(v))) {
      RecordFailure(FmtTagFail(FmtTagAssert(
          FmtExpectThat(forward<StrT0>(cond_code), forward<StrT1>(val_code)))));
      return false;
    }
    return true;
  }

  template <class StrT, class GoodLogT, class BadLogT>
  bool ExpectTrueLog(bool v, StrT&& value_code, BadLogT&& badlog,
                     GoodLogT&& goodlog) {
    if (!v) {
      RecordFailure(FmtTagFail(FmtExpectTrue(forward<StrT>(value_code))));
      RecordFailure(FmtTagFail(forward<BadLogT>(badlog)));
      return false;
    }
    RecordMessage(forward<GoodLogT>(goodlog));
    return true;
  }

  template <class StrT, class GoodLogT, class BadLogT>
  bool ExpectFalseLog(bool v, StrT&& value_code, BadLogT&& badlog,
                      GoodLogT&& goodlog) {
    if (v) {
      RecordFailure(FmtTagFail(FmtExpectFalse(forward<StrT>(value_code))));
      RecordFailure(FmtTagFail(forward<BadLogT>(badlog)));
      return false;
    }
    RecordMessage(forward<GoodLogT>(goodlog));
    return true;
  }

  template <class LhsT, class RhsT, class StrT0, class StrT1, class GoodLogT,
            class BadLogT>
  bool ExpectEqLog(LhsT&& lhs, RhsT&& rhs, StrT0&& lhs_code, StrT1&& rhs_code,
                   BadLogT&& badlog, GoodLogT&& goodlog) {
    return GenericExpectEqualityLog(
        forward<LhsT>(lhs), forward<RhsT>(rhs), forward<StrT0>(lhs_code),
        forward<StrT1>(rhs_code), forward<BadLogT>(badlog),
        forward<GoodLogT>(goodlog), std::equal_to<>{}, FmtExpectEq);
  }

  template <class LhsT, class RhsT, class StrT0, class StrT1, class GoodLogT,
            class BadLogT>
  bool ExpectNeLog(LhsT&& lhs, RhsT&& rhs, StrT0&& lhs_code, StrT1&& rhs_code,
                   BadLogT&& badlog, GoodLogT&& goodlog) {
    return GenericExpectEqualityLog(
        forward<LhsT>(lhs), forward<RhsT>(rhs), forward<StrT0>(lhs_code),
        forward<StrT1>(rhs_code), forward<BadLogT>(badlog),
        forward<GoodLogT>(goodlog), std::not_equal_to<>{}, FmtExpectNe);
  }

  template <class LhsT, class RhsT, class StrT0, class StrT1, class GoodLogT,
            class BadLogT>
  bool ExpectGtLog(LhsT&& lhs, RhsT&& rhs, StrT0&& lhs_code, StrT1&& rhs_code,
                   BadLogT&& badlog, GoodLogT&& goodlog) {
    return GenericExpectEqualityLog(
        forward<LhsT>(lhs), forward<RhsT>(rhs), forward<StrT0>(lhs_code),
        forward<StrT1>(rhs_code), forward<BadLogT>(badlog),
        forward<GoodLogT>(goodlog), std::greater<>{}, FmtExpectGt);
  }

  template <class LhsT, class RhsT, class StrT0, class StrT1, class GoodLogT,
            class BadLogT>
  bool ExpectGeLog(LhsT&& lhs, RhsT&& rhs, StrT0&& lhs_code, StrT1&& rhs_code,
                   BadLogT&& badlog, GoodLogT&& goodlog) {
    return GenericExpectEqualityLog(
        forward<LhsT>(lhs), forward<RhsT>(rhs), forward<StrT0>(lhs_code),
        forward<StrT1>(rhs_code), forward<BadLogT>(badlog),
        forward<GoodLogT>(goodlog), std::greater_equal<>{}, FmtExpectGe);
  }

  template <class LhsT, class RhsT, class StrT0, class StrT1, class GoodLogT,
            class BadLogT>
  bool ExpectLtLog(LhsT&& lhs, RhsT&& rhs, StrT0&& lhs_code, StrT1&& rhs_code,
                   BadLogT&& badlog, GoodLogT&& goodlog) {
    return GenericExpectEqualityLog(
        forward<LhsT>(lhs), forward<RhsT>(rhs), forward<StrT0>(lhs_code),
        forward<StrT1>(rhs_code), forward<BadLogT>(badlog),
        forward<GoodLogT>(goodlog), std::less<>{}, FmtExpectLt);
  }

  template <class LhsT, class RhsT, class StrT0, class StrT1, class GoodLogT,
            class BadLogT>
  bool ExpectLeLog(LhsT&& lhs, RhsT&& rhs, StrT0&& lhs_code, StrT1&& rhs_code,
                   BadLogT&& badlog, GoodLogT&& goodlog) {
    return GenericExpectEqualityLog(
        forward<LhsT>(lhs), forward<RhsT>(rhs), forward<StrT0>(lhs_code),
        forward<StrT1>(rhs_code), forward<BadLogT>(badlog),
        forward<GoodLogT>(goodlog), std::less_equal<>{}, FmtExpectLe);
  }

  template <class FnT, class StrT, class GoodLogT, class BadLogT>
  bool ExpectAnyThrowLog(FnT&& fn, StrT&& fn_code, BadLogT&& badlog,
                         GoodLogT&& goodlog) {
    try {
      fn();
    } catch (...) {  // NOSONAR
      RecordMessage(forward<GoodLogT>(goodlog));
      return true;
    }
    RecordFailure(FmtTagFail(FmtExpectAnyThrow(forward<StrT>(fn_code))));
    RecordFailure(FmtTagFail(forward<BadLogT>(badlog)));
    return false;
  }

  template <class FnT, class StrT, class GoodLogT, class BadLogT>
  bool ExpectNoThrowLog(FnT&& fn, StrT&& fn_code, BadLogT&& badlog,
                        GoodLogT&& goodlog) {
    try {
      fn();
    } catch (const std::exception& e) {  // NOSONAR
      RecordFailure(
          FmtTagFail(FmtExpectNoThrowStd(forward<StrT>(fn_code), e.what())));
      RecordFailure(FmtTagFail(forward<BadLogT>(badlog)));
      return false;
    } catch (...) {
      RecordFailure(FmtTagFail(FmtExpectNoThrow(forward<StrT>(fn_code))));
      RecordFailure(FmtTagFail(forward<BadLogT>(badlog)));
      return false;
    }
    RecordMessage(forward<GoodLogT>(goodlog));
    return true;
  }

  template <class CondT, class ValT, class StrT0, class StrT1, class GoodLogT,
            class BadLogT>
  enable_if_t<iIsInvokableV<bool, CondT, ValT>, bool> ExpectThatLog(
      CondT&& condition, ValT&& v, StrT0&& cond_code, StrT1&& val_code,
      BadLogT&& badlog, GoodLogT&& goodlog) {
    if (!condition(forward<ValT>(v))) {
      RecordFailure(FmtTagFail(
          FmtExpectThat(forward<StrT0>(cond_code), forward<StrT1>(val_code))));
      RecordFailure(FmtTagFail(forward<BadLogT>(badlog)));
      return false;
    }
    RecordMessage(forward<GoodLogT>(goodlog));
    return true;
  }

  template <class StrT, class GoodLogT, class BadLogT>
  bool AssertTrueLog(bool v, StrT&& value_code, BadLogT&& badlog,
                     GoodLogT&& goodlog) {
    if (!v) {
      RecordFailure(
          FmtTagFail(FmtTagAssert(FmtExpectTrue(forward<StrT>(value_code)))));
      RecordFailure(FmtTagFail(forward<BadLogT>(badlog)));
      return false;
    }
    RecordMessage(forward<GoodLogT>(goodlog));
    return true;
  }

  template <class StrT, class GoodLogT, class BadLogT>
  bool AssertFalseLog(bool v, StrT&& value_code, BadLogT&& badlog,
                      GoodLogT&& goodlog) {
    if (v) {
      RecordFailure(
          FmtTagFail(FmtTagAssert(FmtExpectFalse(forward<StrT>(value_code)))));
      RecordFailure(FmtTagFail(forward<BadLogT>(badlog)));
      return false;
    }
    RecordMessage(forward<GoodLogT>(goodlog));
    return true;
  }

  template <class LhsT, class RhsT, class StrT0, class StrT1, class GoodLogT,
            class BadLogT>
  bool AssertEqLog(LhsT&& lhs, RhsT&& rhs, StrT0&& lhs_code, StrT1&& rhs_code,
                   BadLogT&& badlog, GoodLogT&& goodlog) {
    return GenericAssertEqualityLog(
        forward<LhsT>(lhs), forward<RhsT>(rhs), forward<StrT0>(lhs_code),
        forward<StrT1>(rhs_code), forward<BadLogT>(badlog),
        forward<GoodLogT>(goodlog), std::equal_to<>{}, FmtExpectEq);
  }

  template <class LhsT, class RhsT, class StrT0, class StrT1, class GoodLogT,
            class BadLogT>
  bool AssertNeLog(LhsT&& lhs, RhsT&& rhs, StrT0&& lhs_code, StrT1&& rhs_code,
                   BadLogT&& badlog, GoodLogT&& goodlog) {
    return GenericAssertEqualityLog(
        forward<LhsT>(lhs), forward<RhsT>(rhs), forward<StrT0>(lhs_code),
        forward<StrT1>(rhs_code), forward<BadLogT>(badlog),
        forward<GoodLogT>(goodlog), std::not_equal_to<>{}, FmtExpectNe);
  }

  template <class LhsT, class RhsT, class StrT0, class StrT1, class GoodLogT,
            class BadLogT>
  bool AssertGtLog(LhsT&& lhs, RhsT&& rhs, StrT0&& lhs_code, StrT1&& rhs_code,
                   BadLogT&& badlog, GoodLogT&& goodlog) {
    return GenericAssertEqualityLog(
        forward<LhsT>(lhs), forward<RhsT>(rhs), forward<StrT0>(lhs_code),
        forward<StrT1>(rhs_code), forward<BadLogT>(badlog),
        forward<GoodLogT>(goodlog), std::greater<>{}, FmtExpectGt);
  }

  template <class LhsT, class RhsT, class StrT0, class StrT1, class GoodLogT,
            class BadLogT>
  bool AssertGeLog(LhsT&& lhs, RhsT&& rhs, StrT0&& lhs_code, StrT1&& rhs_code,
                   BadLogT&& badlog, GoodLogT&& goodlog) {
    return GenericAssertEqualityLog(
        forward<LhsT>(lhs), forward<RhsT>(rhs), forward<StrT0>(lhs_code),
        forward<StrT1>(rhs_code), forward<BadLogT>(badlog),
        forward<GoodLogT>(goodlog), std::greater_equal<>{}, FmtExpectGe);
  }

  template <class LhsT, class RhsT, class StrT0, class StrT1, class GoodLogT,
            class BadLogT>
  bool AssertLtLog(LhsT&& lhs, RhsT&& rhs, StrT0&& lhs_code, StrT1&& rhs_code,
                   BadLogT&& badlog, GoodLogT&& goodlog) {
    return GenericAssertEqualityLog(
        forward<LhsT>(lhs), forward<RhsT>(rhs), forward<StrT0>(lhs_code),
        forward<StrT1>(rhs_code), forward<BadLogT>(badlog),
        forward<GoodLogT>(goodlog), std::less<>{}, FmtExpectLt);
  }

  template <class LhsT, class RhsT, class StrT0, class StrT1, class GoodLogT,
            class BadLogT>
  bool AssertLeLog(LhsT&& lhs, RhsT&& rhs, StrT0&& lhs_code, StrT1&& rhs_code,
                   BadLogT&& badlog, GoodLogT&& goodlog) {
    return GenericAssertEqualityLog(
        forward<LhsT>(lhs), forward<RhsT>(rhs), forward<StrT0>(lhs_code),
        forward<StrT1>(rhs_code), forward<BadLogT>(badlog),
        forward<GoodLogT>(goodlog), std::less_equal<>{}, FmtExpectLe);
  }

  template <class FnT, class StrT, class GoodLogT, class BadLogT>
  bool AssertAnyThrowLog(FnT&& fn, StrT&& fn_code, BadLogT&& badlog,
                         GoodLogT&& goodlog) {
    try {
      fn();
    } catch (...) {  // NOSONAR
      RecordMessage(forward<GoodLogT>(goodlog));
      return true;
    }
    RecordFailure(
        FmtTagFail(FmtTagAssert(FmtExpectAnyThrow(forward<StrT>(fn_code)))));
    RecordFailure(FmtTagFail(forward<BadLogT>(badlog)));
    return false;
  }

  template <class FnT, class StrT, class GoodLogT, class BadLogT>
  bool AssertNoThrowLog(FnT&& fn, StrT&& fn_code, BadLogT&& badlog,
                        GoodLogT&& goodlog) {
    try {
      fn();
    } catch (const std::exception& e) {  // NOSONAR
      RecordFailure(FmtTagFail(
          FmtTagAssert(FmtExpectNoThrowStd(forward<StrT>(fn_code), e.what()))));
      RecordFailure(FmtTagFail(forward<BadLogT>(badlog)));
      return false;
    } catch (...) {
      RecordFailure(
          FmtTagFail(FmtTagAssert(FmtExpectNoThrow(forward<StrT>(fn_code)))));
      RecordFailure(FmtTagFail(forward<BadLogT>(badlog)));
      return false;
    }
    RecordMessage(forward<GoodLogT>(goodlog));
    return true;
  }

  template <class CondT, class ValT, class StrT0, class StrT1, class GoodLogT,
            class BadLogT>
  enable_if_t<iIsInvokableV<bool, CondT, ValT>, bool> AssertThatLog(
      CondT&& condition, ValT&& v, StrT0&& cond_code, StrT1&& val_code,
      BadLogT&& badlog, GoodLogT&& goodlog) {
    if (!condition(forward<ValT>(v))) {
      RecordFailure(FmtTagFail(FmtTagAssert(
          FmtExpectThat(forward<StrT0>(cond_code), forward<StrT1>(val_code)))));
      RecordFailure(FmtTagFail(forward<BadLogT>(badlog)));
      return false;
    }
    RecordMessage(forward<GoodLogT>(goodlog));
    return true;
  }

};  // end MinitestFramework

/// Minitest library's global array storing all unit tests initialized using
/// TEST/TEST_F macros.
static UnitTestArray gTestMap{};  // NOSONAR

/// Minitest library's global test framework.
static MinitestFramework gFramework{gTestMap};  // NOSONAR

/// @} // end of minitest2_framework_impl

}  // namespace minitest

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
