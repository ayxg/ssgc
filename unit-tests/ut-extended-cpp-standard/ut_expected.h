#pragma once
#include "../../submodules/extended-cpp-standard/cppsextended.h"
#include "../../submodules/mini-test/minitest.h"


INLINE_MINITEST(Test_CxxExpected, TestCase_CxxExpected) {
  using ExpectedString = cxx::Expected<std::string>;
  ExpectedString e_good = ExpectedString::Success("42");
  ExpectedString e_bad = ExpectedString::Failure("42");
  std::string compare = "42";
  EXPECT_TRUE(e_good.Valid());
  EXPECT_FALSE(e_bad.Valid());
  EXPECT_EQ(e_good.Value(), compare);
  EXPECT_EQ(e_bad.Error(), compare);
  std::cout << e_bad.Error() << std::endl;

  // Good should be invalid after extraction of value.
  std::string value = e_good.Extract();
  EXPECT_FALSE(e_good.Valid());
  EXPECT_EQ(e_good.Error(), "Value has been moved out of the expected object.");
  std::cout << e_good.Error() << std::endl;

  // - Chaining Errors properly preserves order of errors.
  // - Chaining errors seperates error messages by a single newline character.
  // - Test Case: e_bad is a recieved error.
  //              e_err is the chain error returned by the reciever.
  ExpectedString e_err = e_bad.ChainFailure("Error 2: 42 was caught!");
  EXPECT_FALSE(e_err.Valid());
  EXPECT_EQ(e_err.Error(), "42\nError 2: 42 was caught!");
  std::cout << e_err.Error() << std::endl;

  // Implicit conversion to boolean to conditional expressions.
  EXPECT_FALSE(e_err);
  EXPECT_FALSE(e_good);
  EXPECT_FALSE(e_bad);
}
INLINE_END_MINITEST;