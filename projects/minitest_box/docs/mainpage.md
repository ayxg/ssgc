Mini-Test : C++ Unit Testing Framework {#mainpage}
===================================
## Minimal unit testing framework for personal and small scale C++ projects.

- Mini-Test has a familiar interface emulating GoogleTest framework. When it's time to move on to a more
  robust testing framework such as GoogleTest, only minimal refactoring is required. 
- Minitest is easy to use. There are no dependencies except the C++ standard library. 
  The library can be consumed as a CMake project or header only.
- Initially created as a quick replacement for GoogleTest. Suprisingly, it has served me well in the pass 2
  years developing and testing my personal projects. So, I have decided to continue improving this library
  rather than abandoning it for GoogleTest.

### Topics
- @ref minitest
- @ref minitest0_macro_api0_case_definitions
- @ref minitest0_macro_api1_checks_and_asserts
- @ref minitest2_framework_impl
- @ref minitest3_common
- @ref minitest4_unittest

### Guides
- @ref minitest_guide_cmake_integration

## **Example of a unit test:**
```C
auto my_method() { return true; }
TEST(MyTest,MyTestCase){
  EXPECT_TRUE(my_method());
  EXPECT_TRUE_LOG(false,"error!"+ my_method()); // All checks and asserts have a log
                                                // version recieving a string.
  EXPECT_FALSE(false);
  EXPECT_EQ(1,1);
  EXPECT_NE(1,2);
  EXPECT_ANY_THROW([](){ throw std::runtime_error("error"); });
  EXPECT_NO_THROW([](){ throw std::runtime_error("error"); });
  throw "oops"!; // Unexpected exceptions will be caught and reported.
}
```

## **Example of a fixture unit test:**
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

MINITEST_F(MyFixtureTest, MyFixtureTestCase, MyFixture) {
  EXPECT_TRUE(my_foo == 0);
}
```

## **Example of retrieving the test result/accessing result later at runtime:**
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


