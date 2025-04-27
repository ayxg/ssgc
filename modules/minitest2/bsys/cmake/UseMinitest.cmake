# Minitest CMake Module
set(MINITEST_LIBRARY) # Library target of the minitest library being used.

## @function ADD_MINITEST_EXECUTABLE
## @brief Creates a unit test executable target with a generated "main.cpp"
##        source file - given a set of headers containing unit tests.
## @sinlge NAME                : New generated target name.
## @multi TEST_HEADERS        : Unit test headers(.hpp) containing unit tests. Relative to one of 'INCLUDE_DIRECTORIES'.
##                              ONLY list headers you wish to explicitly "#include" in the generated 'main.cpp'.
## @multi SOURCES             : Sources(.cpp) to link to the executable. Do NOT include a main method.
## @multi INCLUDE_DIRECTORIES : Target include directories. The include dir containing the test header MUST be included.
## @multi LINK_LIBS           : Additional libraries to link to the executable.
##
## - It is more optimal to have a single header for a given minitest executable
##   which includes all other unit test headers to simplify the CMake script. 
## 
## - When creating minitest targets prefer to organize each test suite into a 
##   seperate target, which can then be added as a CMake test - or multiple.
## 
## - For the best test ui feedback, prefer maximal granularily of tests. 
##   Use 'ADD_MINITEST_AUTO' to automatically detect and seperate tests,
##   or 'ADD_MINITEST_SUITE' and list the test cases manually.
## 
## - Each minitest executable gets a MinitestTestHeaders property listing the 'TEST_HEADERS'.
function(MINITEST_ADD_EXECUTABLE)
  set(options)
  set(oneValueArgs NAME)
  set(multiValueArgs SOURCES INCLUDE_DIRECTORIES LINK_LIBS TEST_HEADERS)
  # _arg_NAME
  # _arg_SOURCES
  # _arg_LINK_LIBS
  # _arg_INCLUDE_DIRECTORIES
  # _arg_TEST_HEADERS
  cmake_parse_arguments(_arg "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  # Generate a main source file for this unit test using the name of the target.
  set(_GENERATED_MAIN_FILE minitest/generated/"${_arg_NAME}".cpp)
  set(_INCLUDE_LINES "")
  foreach(header ${_arg_TEST_HEADERS})
    set(_INCLUDE_LINES "${_INCLUDE_LINES}#include \"${header}\"\n")
  endforeach()
  file(CONFIGURE 
    OUTPUT "${_GENERATED_MAIN_FILE}" 
    CONTENT "\
    #include \"minitest.hpp\"\n\
    ${_INCLUDES}\n\
    int main(int argc, char* argv[]) { return minitest::CmakeMain(argc, argv); }"
    @ONLY)
  
  # Create test executable and append user sources.
  add_executable(
    "${_arg_NAME}"
    "${CMAKE_CURRENT_BINARY_DIR}"/"${_GENERATED_MAIN_FILE}" "${_arg_SOURCES}"
  )
  # Link minitest and user libraries.
  target_link_libraries(
    "${_arg_NAME}" 
    "${MINITEST_LIBRARY}"
    "${_arg_LINK_LIBS}"
  )
  # Add minitest and user include dirs.
  target_include_directories(
    "${_arg_NAME}" 
    PUBLIC "${_arg_INCLUDE_DIRECTORIES}"
  )

  # Add MinitestTestHeaders property
  set_target_properties(
    "${_arg_NAME}" PROPERTIES MinitestTestHeaders "${_arg_TEST_HEADERS}"
  )
endfunction()

## @function MINITEST_CASE
## @brief Add a CMake test which runs a unit test from a minitest executable.
## @sinlge TARGET             : Target test executable, must be created using 'MINITEST_ADD_EXECUTABLE'.
## @sinlge SUITE_NAME         : Name of the test suite which should contain unit test named 'CASE_NAME'.
## @sinlge CASE_NAME          : Name of the test case to run. 
## @sinlge WORKING_DIRECTORY  : Working directory is only significant if tests must access files.
##
## - If the 'SUITE_NAME' is empty, all tests in the executable are added as a single CMake test
##   named '${executable_name}_TEST_ALL'.
## - If the 'CASE_NAME' is empty, the entire test suite is added as a single CMake test.
function(MINITEST_CASE)
  set(options)
  set(oneValueArgs TARGET SUITE_NAME CASE_NAME WORKING_DIRECTORY)
  set(multiValueArgs)
  # _arg_TARGET
  # _arg_SUITE_NAME
  # _arg_CASE_NAME
  # _arg_WORKING_DIRECTORY
  cmake_parse_arguments(_arg_ "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  if(NOT DEFINED _arg_SUITE_NAME OR "${_arg_SUITE_NAME}" STREQUAL "")
    add_test(
      NAME ${_arg_TARGET}_TEST_ALL
      COMMAND $<TARGET_FILE:${_arg_TARGET}>
      WORKING_DIRECTORY ${_arg_WORKING_DIRECTORY}
    )
  elseif(NOT DEFINED _arg_CASE_NAME OR "${_arg_CASE_NAME}" STREQUAL "")
    add_test(
      NAME ${_arg_TARGET}${_arg_SUITE_NAME}
      COMMAND $<TARGET_FILE:${_arg_TARGET}> 
            ${_arg_SUITE_NAME}
      WORKING_DIRECTORY ${_arg_WORKING_DIRECTORY}
    )
  else()
    add_test(
      NAME ${_arg_TARGET}${_arg_SUITE_NAME}${_arg_CASE_NAME}
      COMMAND $<TARGET_FILE:${_arg_TARGET}> 
              ${_arg_SUITE_NAME} 
              ${_arg_CASE_NAME}
      WORKING_DIRECTORY ${_arg_WORKING_DIRECTORY}
    )
  endif()
endfunction()

## @function MINITEST_SUITE
## @brief Add a CMake test for each listed case in a given test suite from a minitest executable.
## @sinlge TARGET             : Target test executable, must be created using 'MINITEST_ADD_EXECUTABLE'.
## @sinlge SUITE_NAME         : Name of the test suite which should contain test cases named 'CASE_NAMES'. 
## @multi CASE_NAMES          : Name of the test cases to run.
## @sinlge WORKING_DIRECTORY  : Working directory is only significant if tests must access files.
## @opt FIND_TESTS : Scans test headers listed in the ::MinitestTestHeaders property of a minitest executable
##                   for tests belong to 'SUITE_NAME'. When enabled, 'CASE_NAMES' is ignored. 
##                   If 'SUITE_NAME' is empty then all detected tests are added.
function(MINITEST_SUITE)
  set(options FIND_TESTS)
  set(oneValueArgs TARGET SUITE_NAME WORKING_DIRECTORY)
  set(multiValueArgs CASE_NAMES)
  # _arg_TARGET
  # _arg_SUITE_NAME
  # _arg_CASE_NAMES
  # _arg_WORKING_DIRECTORY
  cmake_parse_arguments(_arg_ "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  foreach(_CASE_NAME IN ITEMS ${_arg_CASE_NAMES})
    add_test(
      NAME ${_arg_TARGET}${_arg_SUITE_NAME}${_CASE_NAME}
      COMMAND $<TARGET_FILE:${_arg_TARGET}> 
              ${_arg_SUITE_NAME} 
              ${_CASE_NAME}
      WORKING_DIRECTORY ${_arg_WORKING_DIRECTORY}
    )
  endforeach()
endfunction()

## @function MINITEST_INTERNAL_FIND_HEADER_TESTS
## @brief Internal use only. Scans a C++ header file for unit test macro definitions and
##        extracts the file names. Currently does NOT account for the preprocessor, so
##        if you use this method, the header cannot conditionally exclude unit test
##        definitions - or they will be interpreted as active.
## @sinlge ARGV0        : Path to the header file to scan.
## @sinlge @inout ARGV1 : Name of the variable to store the list of detected "suite:case" name pairs.
## @single ARGV2        : Name of a test suite. If passed, only test belonging to this suite are
##                        appended to the list.
function(MINITEST_INTERNAL_FIND_HEADER_TESTS)
  file(READ "${ARGV0}" file_content)

  # Define the regex patterns for opening and closing
  # TEST\\(([A-Za-z0-9]+),([A-Za-z0-9]+)\\)
  # "INLINE_MINITEST\\(([^,]+),([^,]+)\\) \{"
  set(opening_regex "INLINE_MINITEST\\(([^,]+),([^)]+)\\)")
  set(closing_regex "INLINE_END_MINITEST_AND_REGISTER\\(([^,]+),([^,]+)\\)")
  set(test_case_pairs "")

  # Find all opening TEST_CASE patterns
  string(REGEX MATCHALL "${opening_regex}" opening_matches "${file_content}")

  foreach(opening_match IN LISTS opening_matches)
    # Extract suite and case name.
    string(REGEX REPLACE "${opening_regex}" "\\1" name "${opening_match}")
    string(REGEX REPLACE "${opening_regex}" "\\2" case_name "${opening_match}")
    # Trim whitespace
    string(STRIP "${name}" name)
    string(STRIP "${case_name}" case_name)
    # If ARGV2 is provided, only append matching suite cases.

    list(APPEND test_case_pairs "${name}")
    list(APPEND test_case_pairs "${case_name}")
  endforeach()

  # Output the list of test case pairs
  set(${ARGV1} ${test_case_pairs} PARENT_SCOPE)
endfunction()

## @function MINITEST_SUITE
## @brief Add a CMake test for each listed case in a given test suite from a minitest executable.
function(MINITEST_HEADERS)
  set(options)
  set(oneValueArgs TARGET WORKING_DIRECTORY PREFIX SUITE_NAME)
  set(multiValueArgs HEADERS)
  # _arg_TARGET
  # _arg_WORKING_DIRECTORY
  # _arg_PREFIX
  # _arg_SUITE_NAME
  # _arg_HEADERS
  cmake_parse_arguments(_arg_ "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  # Use the target's property MinitestTestHeaders if '_arg_HEADERS' is empty.
  set(_TEST_CASE_SIGNATURES "")
  set(_TARGET_TEST_HEADERS "")
  if(NOT DEFINED _arg_HEADERS OR "${_arg_HEADERS}" STREQUAL "")
    get_target_property(_TARGET_TEST_HEADERS "${_arg_TARGET}" MinitestTestHeaders)  
  else()
    set(_TARGET_TEST_HEADERS "${_arg_HEADERS}")
  endif()

  # Scan every given header for test cases.
  foreach(_HEADER_FILE IN ITEMS ${_TARGET_TEST_HEADERS})
    set(_THIS_FILE_SIGNATURES "")
    MINITEST_INTERNAL_FIND_HEADER_TESTS("${_HEADER_FILE}" _THIS_FILE_SIGNATURES)
    list(APPEND _TEST_CASE_SIGNATURES "${_THIS_FILE_SIGNATURES}")
  endforeach()  

  # Loop through the list in steps of 2 (Name and CaseName)
  list(LENGTH _TEST_CASE_SIGNATURES _N_ELEMENTS)
  math(EXPR _N_TESTS "${_N_ELEMENTS} / 2")  # Calculate the number of pairs

  foreach(_IDX RANGE 0 ${_N_TESTS} 2)
    # Get Name and CaseName from the list
    list(GET _TEST_CASE_SIGNATURES ${_IDX} _SUITE_NAME)
    math(EXPR _NEXT_IDX "${_IDX} + 1") # advance idx
    list(GET _TEST_CASE_SIGNATURES ${_NEXT_IDX} _CASE_NAME)

    # Create a unique test name
    set(_TEST_NAME "${_SUITE_NAME}:${_CASE_NAME}")

    # Use add_test to create the test
    add_test(
      NAME ${_arg_PREFIX}${_TEST_NAME}
      COMMAND $<TARGET_FILE:${_arg_TARGET}> 
            ${_SUITE_NAME} 
            ${_CASE_NAME}
      WORKING_DIRECTORY ${_arg_WORKING_DIRECTORY}
    )

    # Optionally, print the test name for debugging
    message(STATUS "Added test: ${_TEST_NAME}")
  endforeach()
endfunction()