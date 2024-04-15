# Mini-Test : C++20 Unit Testing Framework
## Lightweight single header unit testing framework for personal and small C++ 20 and above projects.

Mini-Test has a familiar interface emulating the Google Testing Framework. When it's time to move on to a more robust testing framework such as Google Test, only minimal refactoring is required. Mini-Test has no dependencies except the C++ standard library, and nothing to build - simply copy minitest.h into your project and include it.

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


| Details |
| :-- |

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

|Details/Example |
|:--|

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

|Details/Example |
|:--|

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

|Details/Example |
|:--|

See `MINITEST_F(TestName, TestCaseName, TestFixtureClass)` details.

---