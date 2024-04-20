//=-------------------------------------------------------------------------=//
//---------------------------------------------------------------------------//
// All Rights Reserved | Copyright 2024 NTONE INC.
// Author: Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.bigteeny.com
//---------------------------------------------------------------------------//
// Project: caoco
// Directory: unit_tests
// File: ut0_expected.h
//---------------------------------------------------------------------------//
#ifndef HEADER_GUARD_CAOCO_UNIT_TESTS_UT0_EXPECTED_H
#define HEADER_GUARD_CAOCO_UNIT_TESTS_UT0_EXPECTED_H
// Includes:
#include "cppsextended.h"
#include "minitest.h"

//---------------------------------------------------------------------------//
//=-------------------------------------------------------------------------=//

namespace ut {
INLINE_MINITEST(Test_CxxExpected, TestCase_CxxExpected)
//using ExpectedString = cxx::Expected<std::string>;
//ExpectedString e_good = ExpectedString::Success("42");
//ExpectedString e_bad = ExpectedString::Failure("42");
//std::string compare = "42";
//EXPECT_TRUE(e_good.Valid());
//EXPECT_FALSE(e_bad.Valid());
//EXPECT_EQ(e_good.Value(), compare);
//EXPECT_EQ(e_bad.Error(), compare);
//
//// Good should be invalid after extraction of value.
//std::string value = e_good.Extract();
//EXPECT_FALSE(e_good.Valid());
//EXPECT_EQ(e_good.Error(), "Value has been moved out of the expected object.");
//
//// - Chaining Errors properly preserves order of errors.
//// - Chaining errors seperates error messages by a single newline character.
//// - Test Case: e_bad is a recieved error.
////              e_err is the chain error returned by the reciever.
//ExpectedString e_err = e_bad.ChainFailure("Error 2: 42 was caught!");
//EXPECT_FALSE(e_err.Valid());
//EXPECT_EQ(e_err.Error(), "42\nError 2: 42 was caught!");
//
//// Implicit conversion to boolean to conditional expressions.
//EXPECT_FALSE(e_err);
//EXPECT_FALSE(e_good);
//EXPECT_FALSE(e_bad);
INLINE_END_MINITEST;
MINITEST_REGISTER_CASE(Test_CxxExpected, TestCase_CxxExpected);

INLINE_MINITEST(Test_CxxExpected, TestCase_PartialExpectedConstrtuct)
//using PartialExpectedString = cxx::PartialExpected<std::string, std::string>;
//PartialExpectedString e_good = PartialExpectedString::Success("Always", "Good");
//PartialExpectedString e_bad = PartialExpectedString::Failure("Always", "Bad");
//EXPECT_TRUE(e_good.Valid());
//EXPECT_FALSE(e_bad.Valid());
//EXPECT_EQ(e_good.Always(), "Always");
//EXPECT_EQ(e_bad.Always(), "Always");
//EXPECT_EQ(e_good.Value(), "Good");
//EXPECT_EQ(e_bad.Error(), "Bad");
//
//// Good should be invalid after extraction of value.
//std::string value = e_good.Extract();
//EXPECT_FALSE(e_good.Valid());
//EXPECT_EQ(e_good.Error(), "Value has been moved out of the expected object.");

// - Chaining Errors properly preserves order of errors.
// - Chaining errors seperates error messages by a single newline character.
// - Test Case: e_bad is a recieved error.
//              e_err is the chain error returned by the reciever.
// ChainFailure is NOT provided with a new always value.
//      -> Old value is passed.
//PartialExpectedString e_err = e_bad.ChainFailure("Error 2: Bad was caught!");
//EXPECT_FALSE(e_err.Valid());
//EXPECT_EQ(e_err.Error(), "Bad\nError 2: Bad was caught!");
//EXPECT_EQ(e_err.Always(), e_bad.Always());

//// ChainFailure is NOT provided with a new always value.
////      -> Old value is passed.
////PartialExpectedString e_err_new_always = PartialExpectedString::ChainFailure(
////    e_bad, "VeryBad", "Error 2: Bad was caught!");
////EXPECT_FALSE(e_err_new_always.Valid());
////EXPECT_EQ(e_err_new_always.Error(), "Bad\nError 2: Bad was caught!");
////EXPECT_NE(e_err_new_always.Always(), e_bad.Always());
////EXPECT_EQ(e_err_new_always.Always(), "VeryBad");
//// Implicit conversion to boolean to conditional expressions.
//EXPECT_FALSE(e_err);
//EXPECT_FALSE(e_good);
//EXPECT_FALSE(e_bad);
INLINE_END_MINITEST;
MINITEST_REGISTER_CASE(Test_CxxExpected, TestCase_PartialExpectedConstrtuct);

INLINE_MINITEST(Test_CxxExpected, TestCase_BoolError)
//using BoolError = cxx::BoolError;
//BoolError bool_err = BoolError("Error");
//BoolError bool_good = BoolError(true);
//BoolError bool_bad = BoolError(false);
//// bool_err is not valid.
//// bool_good is valid.
//// bool_bad is not valid.
//EXPECT_FALSE(bool_err.Valid());
//EXPECT_TRUE(bool_good.Valid());
//EXPECT_FALSE(bool_bad.Valid());
//
//EXPECT_EQ(bool_err.Error(), "Error");
//EXPECT_EQ(bool_bad.Error(), "BoolError: Unspecified false error.");
//
//// Implicit conversion to boolean to conditional expressions.
//EXPECT_FALSE(bool_err);
//EXPECT_TRUE(bool_good);
//EXPECT_FALSE(bool_bad);
INLINE_END_MINITEST
MINITEST_REGISTER_CASE(Test_CxxExpected, TestCase_BoolError);
}  // namespace ut

//namespace ut::modules {
//static bool RUN_INLINE_MINITESTS_UT_EXPECTED_H() {
//  bool is_passed = true;
//  is_passed = MINITEST_RUN_INLINE(Test_CxxExpected, TestCase_CxxExpected);
//  is_passed =
//      MINITEST_RUN_INLINE(Test_CxxExpected, TestCase_PartialExpectedConstrtuct);
//  is_passed = MINITEST_RUN_INLINE(Test_CxxExpected, TestCase_BoolError);
//  return is_passed;
//}
//}  // namespace ut::modules

//=-------------------------------------------------------------------------=//
//---------------------------------------------------------------------------//
// All Rights Reserved | Copyright 2024 NTONE INC.
// Author: Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.bigteeny.com
//---------------------------------------------------------------------------//
// Project: caoco
// Directory: unit_tests
// File: ut0_expected.h
//---------------------------------------------------------------------------//
#endif HEADER_GUARD_CAOCO_UNIT_TESTS_UT0_EXPECTED_H
//---------------------------------------------------------------------------//
//=-------------------------------------------------------------------------=//