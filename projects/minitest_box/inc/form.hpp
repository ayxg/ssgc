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
/// @brief Formatting methods for minitest console output/logging.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "common.hpp"

// clang-format off
namespace minitest {

/// @addtogroup minitest2_framework_impl
/// @{

static string FmtRunTest(const string& suite, const string& test);
static string FmtPassTest(const string& suite, const string& test);

static const char* FmtUnknownExceptionFail();
static string FmtStdExceptionFail(const string& msg);

static string FmtExpectTrue(const string& value_code);
static string FmtExpectFalse(const string& value_code);
static string FmtExpectEq(const string& lhs_code, const string& rhs_code,const string& lhs_val, const string& rhs_val);
static string FmtExpectNe(const string& lhs_code, const string& rhs_code,const string& lhs_val, const string& rhs_val);
static string FmtExpectGt(const string& lhs_code, const string& rhs_code,const string& lhs_val, const string& rhs_val);
static string FmtExpectGe(const string& lhs_code, const string& rhs_code,const string& lhs_val, const string& rhs_val);
static string FmtExpectLt(const string& lhs_code, const string& rhs_code,const string& lhs_val, const string& rhs_val);
static string FmtExpectLe(const string& lhs_code, const string& rhs_code,const string& lhs_val, const string& rhs_val);
static string FmtExpectAnyThrow(const string& fn_code);
static string FmtExpectNoThrow(const string& fn_code);
static string FmtExpectThat(const string& cond_code, const string& val_code);

/// @} // end of minitest2_framework_impl

}  // namespace minitest
// clang-format on

namespace minitest {

static string FmtRunTest(const string& suite, const string& test) {
  return "[Run| " + suite + ":" + test + "]";
}

static string FmtPassTest(const string& suite, const string& test) {
  return "-----[Pass| " + suite + ":" + test + "]";
}

static string FmtTagFail(const string& s) { 
  return (s.front() == '[') ? "[Fail]" + s : "[Fail] " + s;
}

static string FmtTagFail(string&& s) { 
  return (s.front() == '[') ? "[Fail]" + move(s) : "[Fail] " + move(s);
}

static string FmtTagAssert(const string& s) {
  return (s.front() == '[') ? "[Assert]" + s : "[Assert] " + s;
}

static string FmtTagAssert(string&& s) {
  return (s.front() == '[') ? "[Assert]" + move(s) : "[Assert] " + move(s);
}

static const char* FmtUnknownExceptionFail() {
  return "An unknown exception was caught.";
}

static string FmtStdExceptionFail(const string& msg) {
  return "An unexpected standard exception was caught, with message: "
         "'" +
         msg + "'";
}

static string FmtExpectTrue(const string& value_code) {
  return "Expected TRUE boolean value.\n\t--[Condition]: " + value_code;
}

static string FmtExpectTrue(string&& value_code) {
  return "Expected TRUE boolean value.\n\t--[Condition]: " + move(value_code);
}

static string FmtExpectFalse(const string& value_code) {
  return "Expected FALSE boolean value.\n\t--[Condition]: " + value_code;
}

static string FmtExpectEq(const string& lhs_code, const string& rhs_code,
                            const string& lhs_val, const string& rhs_val) {
  return "Expected equality between values.\n\t--[Left]: " + lhs_code +
         "\n\t--[Right]: " + rhs_code + "\n\t--[Left-Value]: " + lhs_val +
         "\n\t--[Right-Value]: " + rhs_val;
}

static string FmtExpectNe(const string& lhs_code, const string& rhs_code,
                            const string& lhs_val, const string& rhs_val) {
  return "Expected inequality between values.\n\t--[Left]: " + lhs_code +
         "\n\t--[Right]: " + rhs_code + "\n\t--[Left-Value]: " + lhs_val +
         "\n\t--[Right-Value]: " + rhs_val;
}

static string FmtExpectGt(const string& lhs_code, const string& rhs_code,
                            const string& lhs_val, const string& rhs_val) {
  return "Expected left to be greater than right.\n\t--[Left]: " +
         lhs_code + "\n\t--[Right]: " + rhs_code +
         "\n\t--[Left-Value]: " + lhs_val + "\n\t--[Right-Value]: " + rhs_val;
}

static string FmtExpectGe(const string& lhs_code, const string& rhs_code,
                            const string& lhs_val, const string& rhs_val) {
  return "Expected left to be greater than, or equal, to "
         "right.\n\t--[Left]: " +
         lhs_code + "\n\t--[Right]: " + rhs_code +
         "\n\t--[Left-Value]: " + lhs_val + "\n\t--[Right-Value]: " + rhs_val;
}

static string FmtExpectLt(const string& lhs_code, const string& rhs_code,
                            const string& lhs_val, const string& rhs_val) {
  return "Expected left to be less than right.\n\t--[Left]: " +
         lhs_code + "\n\t--[Right]: " + rhs_code +
         "\n\t--[Left-Value]: " + lhs_val + "\n\t--[Right-Value]: " + rhs_val;
}

static string FmtExpectLe(const string& lhs_code, const string& rhs_code,
                            const string& lhs_val, const string& rhs_val) {
  return "Expected left to be less than, or equal, to "
         "right.\n\t--[Left]: " +
         lhs_code + "\n\t--[Right]: " + rhs_code +
         "\n\t--[Left-Value]: " + lhs_val + "\n\t--[Right-Value]: " + rhs_val;
}

static string FmtExpectAnyThrow(const string& fn_code) {
  return "Expected any throw (exception). Expression: " + fn_code;
}

static string FmtExpectNoThrow(const string& fn_code) {
  return "Expected no exceptions to be thrown. Got unknown exception. Expression: " + fn_code;
}

static string FmtExpectNoThrowStd(const string& fn_code,const string & exception_msg) {
  return "Expected no exceptions to be thrown. Got standard exception with "
         "message: '" +
         exception_msg + "'\nExpression: " + fn_code;
}

static string FmtExpectThat(const string& cond_code, const string& val_code) {
  return "Expected value to fulfill condition. \n\t[Value]: " +
         val_code + "\n\t[Condition]: " + cond_code;
}

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
