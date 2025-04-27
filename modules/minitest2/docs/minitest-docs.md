Mini-Test : C++20 Unit Testing Framework {#caoco_docs_minitest}
===================================

## Lightweight single header unit testing framework for personal and small C++ 20 and above projects.

Mini-Test has a familiar interface emulating the Google Testing Framework. When it's time to move on to a more robust testing framework such as Google Test, only minimal refactoring is required. Mini-Test has no dependencies except the C++ standard library, and nothing to build - simply copy minitest.h into your project and include it.

 Requires:
   - Compiler flag minimum /std:c++20
   - P0315R4 : Lambdas in unevaluated contexts
     https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0315r4.pdf
 
 This is a single header file unit testing framework. Tests are defined as
 lambdas in a template parameter, which is executed upon initialization of
 static variables. As a result the order of tests is not guaranteed but the
 usual order of static initialization is followed. New: now supports fixtures
 and inline tests which can be run at a later time and defined inside
 methods.
 
 Configuration Macros
 These macros are used to set up how the minitest library should behave:
 
   - MINITEST_CONFIG_RECORD_ALL: If true, all test results are recorded and
       can be viewed using MINITESTS_RECORDED macro.
 
   - MINITEST_CONFIG_NO_CONSOLE_PRINT: If true, no output is printed to the
       console during the test run.
 
   - MINITEST_CONFIG_CUSTOM_SEPARATOR: If defined, a custom separator is used
       between test sections which should be the macro def. Default is a 
       dashed line. Definition must be a string literal with a newline.

---

| Macro Definitions | Use |
| :-- | :-- |
| `MINITEST(TestName, TestCaseName)` | Defines a static test case. |
| `END_MINITEST` | Completes the definition and runs test case. |
| `MINITEST_F(TestName, TestCaseName, TestFixtureClass)` | Defines a fixture test case. |
| `END_MINITEST_F(TestFixtureClass)` | Completes the definition and runs fixture test case. |
| `INLINE_MINITEST(TestName, TestCaseName)`| Defines an inline test case to be executed at a later time.|
| `END_INLINE_MINITEST`| Completes the definition of an inline test case.|
| `MINITEST_REGISTER_CASE(TestName, TestCaseName)`| Registers an inline test case to be executed at a later time as part of a test module.|
| `MINITEST_RUN_REGISTERED_MODULE(TestName)`| Runs all registered inline test cases for a test module.|
| `MINITEST_FUNCTOR_RUN_INLINE(TestName)` | Provides a functor to run all registered inline test cases for a test module.|
| `MINITEST_RUN_INLINE(TestName,TestCaseName)` | Runs a single inline test case returning boolean result.|
| `FINISH_MINITEST` | Stores result of entire test suite in a bool, and optionally prints the recorded results of individual test checks.|


---

| `MINITEST(TestName, TestCaseName)` |
| :-- |
|Defines a static test case. Always close with 'END_MINITEST' macro.| 

| Arg | Description |
|:--|:--|
|`TestName`|Name of the test. Must be unique per program.|
|`TestCaseName`|Name of the test case, must be unique per test.|


Details:
    Defines a static test case. Which will be executed once per program. The order of static test case execution is the C++ static order of evaluation, based on the order the tests were defined. Always close with the `END_MINITEST` macro.

*example of a static unit test:*
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
---

| `END_MINITEST` |
| :-- |
|Completes and runs a test case. Must be called after your test case definition.| 

See `MINITEST(TestName, TestCaseName)` details.

---

| `MINITEST_F(TestName, TestCaseName, TestFixtureClass)` |
| :-- |
|Defines a fixture test case. Always close with `END_MINITEST_F(TestFixtureClass)`.| 

| Arg | Description |
|:--|:--|
|`TestName`|Name of the test. Must be unique per program.|
|`TestCaseName`|Name of the test case, must be unique per test.|
|`TestFixtureClass`| Name of the fixture class. Must inherit from virtual base class `minitest::Fixture`. See `minitest::Fixture` for more details.

Always close with `END_MINITEST_F(TestFixtureClass)`.

*example of a static fixture unit test:*
```C
// Example of a fixture.
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
  EXPECT_TRUE(my_foo != 1);
  EXPECT_FALSE(my_foo == 1);
  EXPECT_FALSE(my_foo != 0);
  EXPECT_EQ(my_foo,0);
  EXPECT_EQ(my_foo,1);
  EXPECT_NE(my_foo,1);
  EXPECT_NE(my_foo,0);
  EXPECT_ANY_THROW([](){ throw std::runtime_error("error"); });
  EXPECT_ANY_THROW([](){});
  EXPECT_NO_THROW([](){});
  EXPECT_NO_THROW([](){ throw std::runtime_error("error"); });
}
END_MINITEST_F(MyFixtureTestCase);
```
---

| `END_MINITEST_F(TestFixtureClass)` |
| :-- |
|Completes and runs a test case. Must be called after your test case definition.| 

| Arg | Description |
|:--|:--|
|`TestFixtureClass`|Name of the fixture class used in the last matching `BEGIN_MINITEST_F` call.

See `MINITEST_F(TestName, TestCaseName, TestFixtureClass)` details.

---

## ex1. Sample Use:
```
   auto my_method() { return true; }
 
   MINITEST(MyTest,MyTestCase){
     EXPECT_TRUE(my_method());
     EXPECT_TRUE(false);
     EXPECT_FALSE(false);
     EXPECT_FALSE(true);
     EXPECT_EQ(1,1);
     EXPECT_EQ(1,2);
     EXPECT_NE(1,2);
     EXPECT_NE(1,1);
     EXPECT_ANY_THROW([](){ throw std::runtime_error("error"); });
     EXPECT_ANY_THROW([](){});
     EXPECT_NO_THROW([](){});
     EXPECT_NO_THROW([](){ throw std::runtime_error("error"); });
   }
   END_MINITEST;
 
   FINISH_MINITESTS; // returns a boolean true if all tests passed
                     // call this right before your main function
                     // Macro MINITESTS_RESULT will contain result.
   int main() {
     return MINITESTS_RESULT ? 0 : 1;
   }
```
 
## ex2. Using inline test cases.
```
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
```