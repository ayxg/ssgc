# Mini-Test : C++20 Unit Testing Framework
## Lightweight and elegant single header unit testing framework for personal and small C++ 20 and above projects.

Mini-Test has a familiar interface emulating the Google Testing Framework. When it's time to move on to a more robust testing framework such as Google Test, only minimal refactoring is required. Mini-Test has no dependencies except the C++ standard library, and nothing to build - simply copy minitest.h into your project and include it. The entire testing framework is only about 200~ lines of code, the result is a mimic of a robust framework with none of the hassle.

**Example of a unit test:**
```C
// NOTE: the braces{} are optional but recommended for better readability.
auto my_method() { return true; }
MINITEST(MyTest,MyTestCase){
  EXPECT_TRUE(my_method());
  EXPECT_TRUE_LOG(false,"error!"+ my_method()); // All checks and asserts have a log
                                                // version recieving a string.
  EXPECT_FALSE(false);
  EXPECT_EQ(1,1);
  EXPECT_NE(1,2);
  EXPECT_ANY_THROW([](){ throw std::runtime_error("error"); });
  EXPECT_NO_THROW([](){ throw std::runtime_error("error"); });
}
END_MINITEST;
```

**Example of a fixture unit test:**
```C
struct MyFixture : minitest::Fixture {
  int my_foo = 0;
  void SetUp() override {
    std::cout << "Setting up the fixture\n";
  }

  void TearDown() override {
    std::cout << "Tearing down the fixture\n";
  }
};

// Example of a fixture unit test.
MINITEST_F(MyFixtureTest, MyFixtureTestCase, MyFixture) {
  EXPECT_TRUE(my_foo == 0);
}
END_MINITEST_F(MyFixtureTestCase);
```

**Example of retrieving the test result/accessing result later at runtime:**
```C
FINISH_MINITESTS;  // Macro to finish the test suite, call before main function.

// Main Function is unaffected by the unit tests.
int main() {
  std::cout << "Hello World!\n";

  if (MINITESTS_RESULT)
    std::cout << "Testing was successful.\n";
  else
    std::cout << "A test case failed.\n";

  return MINITESTS_RESULT ? 0 : 1; // example of return result based on tests.
}

```

**Example Output:**
```
---------------------------------------------------------------------------------------------------------------------
[Begin Mini Test] MyTest [Case]MyTestCase
---------------------------------------------------------------------------------------------------------------------
[FAIL] Expected TRUE.
[FAIL] Expected FALSE.
[FAIL] Expected Equality with: >|1 Got: 2
[FAIL] Expected Inequality with>|1
[FAIL] Expected exception but got none.
[FAIL] Expected no exception but one was raised.
---------------------------------------------------------------------------------------------------------------------
[End Mini Test] MyTest [Case]MyTestCase
---------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------
[Begin Mini Test] MyFixtureTest [Case]MyFixtureTestCase
---------------------------------------------------------------------------------------------------------------------
Setting up the fixture
[FAIL] Expected Equality with: >|0 Got: 1
[FAIL] Expected Inequality with>|0
[FAIL] Expected exception but got none.
[FAIL] Expected no exception but one was raised.
Tearing down the fixture
---------------------------------------------------------------------------------------------------------------------
[End Mini Test] MyFixtureTest [Case]MyFixtureTestCase
---------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------
Failed Tests:
---------------------------------------------------------------------------------------------------------------------
[FAILURE DETECTED] Test: MyTest Case: MyTestCase On Check:[EXPECTATION FAILED]: EXPECT_TRUE
[FAILURE DETECTED] Test: MyTest Case: MyTestCase On Check:[EXPECTATION FAILED]: EXPECT_FALSE
[FAILURE DETECTED] Test: MyTest Case: MyTestCase On Check:[EXPECTATION FAILED]: EXPECT_EQ
[FAILURE DETECTED] Test: MyTest Case: MyTestCase On Check:[EXPECTATION FAILED]: EXPECT_NE
[FAILURE DETECTED] Test: MyTest Case: MyTestCase On Check:[EXPECTATION FAILED]: EXPECT_ANY_THROW
[FAILURE DETECTED] Test: MyTest Case: MyTestCase On Check:[EXPECTATION FAILED]: EXPECT_NO_THROW
[FAILURE DETECTED] Test: MyFixtureTestCase Case: MyFixtureTest On Check:[EXPECTATION FAILED]: EXPECT_EQ
[FAILURE DETECTED] Test: MyFixtureTestCase Case: MyFixtureTest On Check:[EXPECTATION FAILED]: EXPECT_NE
[FAILURE DETECTED] Test: MyFixtureTestCase Case: MyFixtureTest On Check:[EXPECTATION FAILED]: EXPECT_ANY_THROW
[FAILURE DETECTED] Test: MyFixtureTestCase Case: MyFixtureTest On Check:[EXPECTATION FAILED]: EXPECT_NO_THROW
---------------------------------------------------------------------------------------------------------------------
End of Failed Tests:
---------------------------------------------------------------------------------------------------------------------
Hello World!
A test case failed.
```

