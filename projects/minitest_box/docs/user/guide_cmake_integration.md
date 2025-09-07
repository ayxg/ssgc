# Using Minitest with CMake {#minitest_guide_cmake_integration}

- Make sure 'enable_testing()' has been called in the consuming CMake project.

- Have a copy of the MinitestProject headers and create a CMake target for the minitest library. 
  Set 'MINITEST_LIBRARY' to your CMake minitest library. 
        **--OR--**
  Use the CMake script provided with Minitest and 'add_subdirectory("minitest_dir")' to install all required 
  targets automatically - which also includes this file.

## Create a "minitest" target using `minitest_add_executable`.
  ```cmake
  minitest_add_executable(
    <NAME [name]>                            : New generated target name.
    [SOURCES [file1] [file2...]]             : Sources(.cpp) to link to the executable.
    [INCLUDE_DIRECTORIES [path1] [path2...]] : Target include directories.
    [LINK_LIBS [name1] [name2...]]           : Additional libraries to link to the executable.
    [TEST_HEADERS [file1] [file2...]]        : Unit test headers(.hpp) containing unit tests.
  )
  ```

  `minitest_add_executable` adds a target with a generated 'main.cpp' source file - given a set of headers 
  containing unit tests. A source file containing a `main()` definition must NOT be included or passed to
  `SOURCES`.

  The `TEST_HEADERS` argument must be a list of headers(.hpp) containing unit tests. Relative to one of 
  provided `INCLUDE_DIRECTORIES`. ONLY list headers you wish to explicitly `#include` in the generated 
  'main.cpp'. The 'MINITEST_TARGET_PROP_TEST_HEADERS' target property can be used to later access or
  change these test headers.
  
  When creating minitest targets, prefer to organize each test suite into a seperate target, which can 
  then be added as a CMake test - or multiple. For the best test ui feedback, prefer maximal granularily
  of tests. Use `minitest_from_headers` or `minitest_from_scan` to automatically detect and seperate tests, 
  or `minitest_suite` and list the test cases manually per suite.
  
  Try to keep the amount of tests per executable low(~1000 'TEST' defs), to increase speed. CTest will run executables once 
  per test case to record results. If a big object error occurs at build time, the target contains 
  execessive test definitions, consider splitting into multiple targets.


## Use 'minitest_case','minitest_suite','minitest_from_list' or 'minitest_from_headers' to add tests. 

Minitest provides many ways of adding tests to allow you to structure your project your way. There are 4
functions for adding tests:
  1. `minitest_case` - Creates a single ctest from a minitest executable. Filters by suite or case. 
  The most basic way to add tests is to use `minitest_case`.
  ```cmake
  minitest_case(
    TARGET FooTestTarget
    SUITE_NAME FooTestSuite
    CASE_NAME FooTestCase
  )

  # If the 'CASE_NAME' is empty, the entire test suite
  # is added as a single CMake test.
  minitest_case(
    TARGET FooTestTarget
    SUITE_NAME FooTestSuite
  )

  # If the 'SUITE_NAME' is empty, all tests in the executable
  # are added as a single CTest named '${executable_name}_TEST_ALL'.
  minitest_case(
    TARGET FooTestTarget
  )
  ```
  2. `minitest_suite` - Creates a test for each listed case in a suite, or scan for cases matching a suite.
  ```cmake
  # Example using 'minitest_suite' in manual mode.
  minitest_suite(
    PREFIX FooPrefix
    TARGET FooTestTarget
    SUITE_NAME FooTestSuite
    CASE_NAMES FooTestCase BarTestCase
  )

  # Example using 'minitest_suite' in scan mode. Tests are filtered by the suite name.
  # To filter by multiple suites use 'minitest_from_list' or 'minitest_from_headers'.
  minitest_suite(
    PREFIX FooPrefix
    TARGET FooTestTarget
    SUITE_NAME FooTestSuite
    FIND_TESTS
  )

  # If no 'SUITE_NAME' is provided, all tests are scanned.
  minitest_suite(
    PREFIX FooPrefix
    TARGET FooTestTarget
    FIND_TESTS
  )
  ```
  3. `minitest_from_list` - Creates a test for each 'suite;case;' pair in a list. Filters by suites or regex.
  ```cmake
  minitest_case(
    TARGET FooTestTarget
    SUITE_NAME FooTestSuite
    CASE_NAME FooTestCase
  )

  # If the 'CASE_NAME' is empty, the entire test suite
  # is added as a single CMake test.
  minitest_case(
    TARGET FooTestTarget
    SUITE_NAME FooTestSuite
  )

  # If the 'SUITE_NAME' is empty, all tests in the executable
  # are added as a single CTest named '${executable_name}_TEST_ALL'.
  minitest_case(
    TARGET FooTestTarget
  )
  ```
  4. `minitest_from_headers` - Creates a test for each test case found in given header files. 
                               Filters by suites or regex.
  ```cmake
  minitest_from_headers(
    PREFIX FooPrefix
    TARGET FooTestTarget
    SCAN_ALL
  )

  minitest_from_headers(
    PREFIX FooPrefix
    TARGET FooTestTarget
    HEADERS foo_test_header.hpp
  )

  minitest_from_headers(
    PREFIX FooPrefix
    TARGET FooTestTarget
    SUITE_NAMES FooTestSuite
    SCAN_ALL
  )

  minitest_from_headers(
    PREFIX FooPrefix
    TARGET FooTestTarget
    REGEX_PATTERN "([^,\(]+)"
    REGEX
    SCAN_ALL
  )
  ```
