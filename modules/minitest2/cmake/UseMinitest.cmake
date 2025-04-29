#[====================================================================================================================[
Copyright 2025 Anton Yashchenko
Licensed under the Apache License, Version 2.0(the "License");
=======================================================================================================================
@project: Minitest Framework
@author(s): Anton Yashchenko
@website: https://www.acpp.dev
=======================================================================================================================
@file Minitest-CMake integration functions
@ingroup minitest_cmake
@brief CMake functions for detecting and generating CTest test cases using the Minitest library.

Config Variables:
  <MINITEST_LIBRARY [name]>                 : Valid Minitest library target.
  [MINITEST_CONFIG_AUXILLARY_DIR [path]]    : Root path for generated code and files, 
                                              defaults to "${CMAKE_BINARY_DIR}/minitest".
  [MINITEST_CONFIG_CODEGEN_DIR [path]]      : Path where minitest sources are generated. 
                                              Defaults to "${MINITEST_CONFIG_AUXILLARY_DIR}/codegen"
  [MINITEST_CONFIG_SCAN_RESULT_DIR [path]]  : Path where scan results are stored. 
                                              Defaults to "${MINITEST_CONFIG_AUXILLARY_DIR}/scan_results"
Target Properties:
  MinitestTestHeaders

Global Variables:
  MINITEST_CONFIG_AUXILLARY_DIR
  MINITEST_CONFIG_CODEGEN_DIR
  MINITEST_CONFIG_SCAN_RESULT_DIR

Public Commands:
  minitest_add_executable
  minitest_case
  minitest_suite
  minitest_from_list
  minitest_from_headers

Brief quickstart:
  0. Make sure 'enable_testing()' has been called in the CMake project.

  1. Have a copy of the MinitestProject headers and create a CMake target for the minitest library. 
     Set 'MINITEST_LIBRARY' to your CMake minitest library. 
          --OR--
     Use the CMake script provided with Minitest and 'add_subdirectory("minitest_dir")' to install all required 
     targets automatically - which also includes this file.

  2. Create a "minitest" target using 'ADD_MINITEST_EXECUTABLE'. Try to keep the amount of tests per executable
     low, to increase speed. CTest will run executables once per test case to record results. 

  3. Use 'minitest_case','minitest_suite','minitest_from_list' or 'minitest_from_headers' to add tests. 
     Prefer more granular tests to get better diagnostics from gui apps(eg. MSVS Test Explorer).
#]====================================================================================================================]
include("cmake/StdMake.cmake")

if(NOT DEFINED MINITEST_LIBRARY OR "${MINITEST_LIBRARY}" STREQUAL "")
  message(FATAL_ERROR 
    "[Minitest] CMake variable 'MINITEST_LIBRARY' not set.\
    Must be set to valid MinitestLibrary target before including 'UseMinitest.cmake'."
  )
endif()

if(NOT DEFINED MINITEST_CONFIG_AUXILLARY_DIR OR "${MINITEST_CONFIG_AUXILLARY_DIR}" STREQUAL "")
  set(MINITEST_CONFIG_AUXILLARY_DIR ${CMAKE_BINARY_DIR}/minitest PARENT_SCOPE)
  set(MINITEST_CONFIG_AUXILLARY_DIR ${CMAKE_BINARY_DIR}/minitest)
endif()

if(NOT DEFINED MINITEST_CONFIG_CODEGEN_DIR OR "${MINITEST_CONFIG_CODEGEN_DIR}" STREQUAL "")
  set(MINITEST_CONFIG_CODEGEN_DIR ${MINITEST_CONFIG_AUXILLARY_DIR}/codegen PARENT_SCOPE)
  set(MINITEST_CONFIG_CODEGEN_DIR ${MINITEST_CONFIG_AUXILLARY_DIR}/codegen)
endif()

if(NOT DEFINED MINITEST_CONFIG_SCAN_RESULT_DIR OR "${MINITEST_CONFIG_SCAN_RESULT_DIR}" STREQUAL "")
  set(MINITEST_CONFIG_SCAN_RESULT_DIR ${MINITEST_CONFIG_AUXILLARY_DIR}/scan_results PARENT_SCOPE)
  set(MINITEST_CONFIG_SCAN_RESULT_DIR ${MINITEST_CONFIG_AUXILLARY_DIR}/scan_results)
endif()

define_property(
  TARGET 
  PROPERTY MINITEST_TARGET_PROP_TEST_HEADERS
  BRIEF_DOCS "Headers containing Minitest test case definitions."
)

#[====================================================================================================================[
  @function minitest_add_executable(
    <NAME [name]>
    [SOURCES [file1] [file2...]]
    [INCLUDE_DIRECTORIES [path1] [path2...]]
    [LINK_LIBS [name1] [name2...]]
    [TEST_HEADERS [file1] [file2...]]
  )
  @brief Creates a unit test executable target with a generated "main.cpp" source file - given a set of headers 
         containing unit tests.
  @sinlge NAME               : New generated target name.
  @multi SOURCES             : Sources(.cpp) to link to the executable. Do NOT include a main method.
  @multi INCLUDE_DIRECTORIES : Target include directories. The include dir containing the test header MUST be included.
  @multi LINK_LIBS           : Additional libraries to link to the executable.
  @multi TEST_HEADERS        : Unit test headers(.hpp) containing unit tests. Relative to one of 'INCLUDE_DIRECTORIES'.
                               ONLY list headers you wish to explicitly "#include" in the generated 'main.cpp'.

  - It is more optimal to have a single header for a given minitest executable which includes all other unit test headers
    to simplify the CMake script. 
 
  - When creating minitest targets prefer to organize each test suite into a seperate target, which can then be added 
    as a CMake test - or multiple.
 
  - For the best test ui feedback, prefer maximal granularily of tests. Use 'minitest_from_headers' or 
    'minitest_from_scan' to automatically detect and seperate tests, or 'minitest_suite' and list the test 
    cases manually per suite.
#]====================================================================================================================]
function(minitest_add_executable)
  set(options)
  set(one_value_args NAME)
  set(multi_value_args SOURCES INCLUDE_DIRECTORIES LINK_LIBS TEST_HEADERS)
  cmake_parse_arguments(PARSE_ARGV 0 arg "${options}" "${one_value_args}" "${multi_value_args}")
  smk_check_unparsed_arguments(minitest_add_executable ARGUMENT arg_UNPARSED_ARGUMENTS)
  smk_assert_arguments(minitest_add_executable ARGUMENTS arg_NAME arg_TEST_HEADERS REQUIRED NOT_EMPTY)

  # Generate a main source file for this unit test using the name of the target.
  set(generated_main_file "${MINITEST_CONFIG_CODEGEN_DIR}/${arg_NAME}.cpp")
  set(include_lines "")
  set(test_target_headers_list "")
  
  foreach(header_file ${arg_TEST_HEADERS})
    string(APPEND include_lines "#include \"${header_file}\"\n")
    list(APPEND test_target_headers_list "${header_file}") 
  endforeach() 

  file(CONFIGURE 
    OUTPUT ${generated_main_file}
    CONTENT "#include \"minitest.hpp\"\n${include_lines}\n\
            int main(int argc, char* argv[]) { return minitest::gFramework.CliMain(argc, argv); }" 
    @ONLY
  )

  # Create test executable, link minitest and forward user sources, headers and libraries.
  add_executable(${arg_NAME} "${generated_main_file}" "${arg_SOURCES}")
  target_link_libraries(${arg_NAME} ${MINITEST_LIBRARY} ${arg_LINK_LIBS})
  target_include_directories(${arg_NAME} PUBLIC ${arg_INCLUDE_DIRECTORIES})
  
  # Set Minitest properties on target.
  set_target_properties(
    ${arg_NAME} PROPERTIES MINITEST_TARGET_PROP_TEST_HEADERS "${test_target_headers_list}"
  ) 
endfunction()


#[====================================================================================================================[
  @function minitest_case(
    <TARGET [name]>
    [SUITE_NAME [name]]
    [CASE_NAME [name]]
    [WORKING_DIRECTORY [path]]  
  )
  @brief Add a CMake test which created a single ctest from a minitest executable.
  @sinlge TARGET             : Target test executable, must be created using 'minitest_add_executable'.
  @sinlge SUITE_NAME         : Name of the test suite ,must contain unit test named 'CASE_NAME'.
  @sinlge CASE_NAME          : Name of the test case to run. 
  @sinlge WORKING_DIRECTORY  : Working directory is only significant if tests must access files.

  - If the 'SUITE_NAME' is empty, all tests in the executable are added as a single CTest
    named '${executable_name}_TEST_ALL'. 'CASE_NAME' is ignored.

  - If the 'CASE_NAME' is empty, the entire test suite is added as a single CMake test.
#]====================================================================================================================]
function(minitest_case)
  set(options)
  set(one_value_args TARGET SUITE_NAME CASE_NAME WORKING_DIRECTORY)
  set(multi_value_args)
  cmake_parse_arguments(PARSE_ARGV 0 arg "${options}" "${one_value_args}" "${multi_value_args}")
  smk_check_unparsed_arguments(minitest_case ARGUMENT arg_UNPARSED_ARGUMENTS)
  smk_assert_arguments(minitest_case ARGUMENTS arg_TARGET REQUIRED NOT_EMPTY)
  if(DEFINED arg_CASE_NAME)
    smk_assert_arguments(minitest_add_executable ARGUMENTS arg_SUITE_NAME REQUIRED NOT_EMPTY)   
  endif()

  if(NOT DEFINED arg_SUITE_NAME OR "${arg_SUITE_NAME}" STREQUAL "")
    add_test(
      NAME ${arg_TARGET}_TEST_ALL
      COMMAND $<TARGET_FILE:${arg_TARGET}>
      WORKING_DIRECTORY ${arg_WORKING_DIRECTORY}
    )
  elseif(NOT DEFINED arg_CASE_NAME OR "${arg_CASE_NAME}" STREQUAL "")
    add_test(
      NAME ${arg_TARGET}${arg_SUITE_NAME}
      COMMAND $<TARGET_FILE:${arg_TARGET}> 
            ${arg_SUITE_NAME}
      WORKING_DIRECTORY ${arg_WORKING_DIRECTORY}
    )
  else()
    add_test(
      NAME ${arg_TARGET}${arg_SUITE_NAME}${arg_CASE_NAME}
      COMMAND $<TARGET_FILE:${arg_TARGET}> 
              ${arg_SUITE_NAME} 
              ${arg_CASE_NAME}
      WORKING_DIRECTORY ${arg_WORKING_DIRECTORY}
    )
  endif()
endfunction()


#[====================================================================================================================[
  @function minitest_suite(
    <TARGET [name]>
    <PREFIX [name]>
    [SUITE_NAME [name]]
    [WORKING_DIRECTORY [path]]
    [CASE_NAMES [name1][name2...]]
    [FIND_TESTS]
  )
  @brief Add a CMake test for each listed case in a given test suite from a minitest executable.
  @sinlge TARGET             : Target test executable, must be created using 'MINITEST_ADD_EXECUTABLE'.
  @single PREFIX             : Prefix for test name and generated file names.
  @sinlge SUITE_NAME         : Name of the test suite which should contain test cases named 'CASE_NAMES'. 
  @sinlge WORKING_DIRECTORY  : Working directory is only significant if tests must access files.
  @multi CASE_NAMES          : List of names of the test cases to run.
  @option FIND_TESTS         : Ignored if 'CASE_NAMES' is passed, otherwise scans tests post build, reconfig to add.

  - When no 'CASE_NAMES' are provided, the target will be ran in "Scan Mode" to detect all tests
    belonging to 'SUITE_NAME' and add matching tests. 

  - If no 'SUITE_NAME' is provided, all tests are scanned. Otherwise the tests are filtered by the suite name.
    To filter by multiple suites use 'minitest_from_list' or 'minitest_from_headers'.
#]====================================================================================================================]
function(minitest_suite)
  set(options FIND_TESTS)
  set(one_value_args PREFIX TARGET SUITE_NAME WORKING_DIRECTORY)
  set(multi_value_args CASE_NAMES)
  cmake_parse_arguments(PARSE_ARGV 0 arg "${options}" "${one_value_args}" "${multi_value_args}")
  smk_check_unparsed_arguments(minitest_suite ARGUMENT arg_UNPARSED_ARGUMENTS)
  smk_assert_arguments(minitest_suite ARGUMENTS arg_TARGET arg_PREFIX REQUIRED NOT_EMPTY)
  if(DEFINED arg_CASE_NAMES)
    smk_assert_arguments(minitest_add_executable ARGUMENTS arg_SUITE_NAME REQUIRED NOT_EMPTY)   
  endif()

  if(NOT DEFINED arg_SUITE_NAME OR "${arg_SUITE_NAME}" STREQUAL "") 
    if(${arg_FIND_TESTS})
      z_minitest_suite_run_scan(
        PREFIX ${arg_PREFIX}
        TARGET ${arg_TARGET}
        WORKING_DIRECTORY "${arg_WORKING_DIRECTORY}"
      )
    endif()
  elseif(NOT DEFINED arg_CASE_NAMES OR "${arg_CASE_NAMES}" STREQUAL "")
    if(${arg_FIND_TESTS})
      z_minitest_suite_run_scan(
        PREFIX ${arg_PREFIX}
        TARGET ${arg_TARGET}
        WORKING_DIRECTORY "${arg_WORKING_DIRECTORY}"
        SUITE_NAMES ${arg_SUITE_NAME}
      )
    endif()
  else()
    foreach(_CASE_NAME IN ITEMS ${arg_CASE_NAMES})
      add_test(
        NAME ${arg_TARGET}${arg_SUITE_NAME}${_CASE_NAME}
        COMMAND $<TARGET_FILE:${arg_TARGET}> 
                ${arg_SUITE_NAME} 
                ${_CASE_NAME}
        WORKING_DIRECTORY ${arg_WORKING_DIRECTORY}
      )
    endforeach()
  endif()
endfunction()

#[====================================================================================================================[
  @function minitest_from_list(
    <TARGET [name]> 
    <PREFIX [name]>
    [WORKING_DIRECTORY [path]]
    [REGEX_PATTERN ["regex pattern string"]]
    [SUITE_NAMES name1 [name2...]]
    [TESTS [[suite;case]1] [[[suite;case]2]...]] 
    [FIND_TESTS]
  )
  @brief Add a test for each 'suite:case' pair in a list. Optionally, filter based on suite names or a regex pattern.
         Use FIND_TESTS to automatically detect tests. Build and re-configure to add scanned test targets.
  @sinlge TARGET            : Minitest executable target, must be created using 'minitest_add_executable'.
  @single PREFIX            : Prefix for test name and generated file names. Should be unique across minitests.
  @sinlge WORKING_DIRECTORY : Working dir for this set of tests.
  @sinlge REGEX_PATTERN     : Regex pattern string. Required when 'REGEX' option is enabled.
                              Filter "suite;case;" pairs based on regex pattern. Ignore 'SUITE_NAMES' argument.
  @multi  SUITE_NAMES       : List of suite names to filter the list. Only matching tests are added.
  @multi  TESTS             : List of test cases to add. Must be even sized, containing "suite;case;" pairs.
  @option FIND_TESTS        : Scan for tests post-build. 
#]====================================================================================================================]
function(minitest_from_list)
  set(options FIND_TESTS)
  set(one_value_args PREFIX TARGET WORKING_DIRECTORY REGEX_PATTERN)
  set(multi_value_args TESTS SUITE_NAMES)
  cmake_parse_arguments(PARSE_ARGV 0 arg "${options}" "${one_value_args}" "${multi_value_args}")
  smk_check_unparsed_arguments(minitest_from_list ARGUMENT arg_UNPARSED_ARGUMENTS)
  smk_assert_arguments(minitest_from_list ARGUMENTS arg_TARGET arg_PREFIX REQUIRED NOT_EMPTY)

  if(${arg_FIND_TESTS})
    if(DEFINED arg_TESTS) 
      message(WARNING 
        "[Minitest] 'TESTS' arg provided to 'minitest_from_list' ignored when using 'FIND_TESTS'.\
        Minitest : ${arg_PREFIX}${arg_TARGET}.")
    endif()
    z_minitest_suite_run_scan(
      PREFIX ${arg_PREFIX}
      TARGET ${arg_TARGET}
      WORKING_DIRECTORY "${arg_WORKING_DIRECTORY}"
      SUITE_NAMES ${arg_SUITE_NAMES}
      REGEX_PATTERN ${arg_REGEX_PATTERN}
    )
  else()
    if(NOT DEFINED arg_TESTS OR "${arg_TESTS}" STREQUAL "") 
      message(WARNING 
        "[Minitest] No 'TESTS' arg provided to 'minitest_from_list'.\
        Did you mean to 'FIND_TESTS'? Minitest : ${arg_PREFIX}${arg_TARGET}.")
    else()
      z_minitest_filter_test_list(
        filtered_test_case_signatures
        TESTS         ${arg_TESTS}
        SUITE_NAMES   ${arg_SUITE_NAMES}
        REGEX_PATTERN "${arg_REGEX_PATTERN}"
      )

      z_minitest_add_target_tests_from_list(
        TARGET            ${arg_TARGET}
        WORKING_DIRECTORY ${arg_WORKING_DIRECTORY}
        PREFIX            ${arg_PREFIX}
        TESTS             ${filtered_test_case_signatures}
      )
    endif()
  endif()
endfunction()


#[====================================================================================================================[
  @function minitest_from_headers(
    <TARGET [name]> 
    <PREFIX [name]>
    <HEADERS [file1] [file2...]>
    [WORKING_DIRECTORY [path]]
    [REGEX_PATTERN ["regex pattern string"]]
    [SUITE_NAMES [name1] [name2...]]
    [SCAN_ALL]
  )
  @brief 
  @sinlge TARGET            : Minitest executable target, must be created using 'minitest_add_executable'.
  @single PREFIX            : Prefix added to the test name, forwarded to add_test() for each case.
  @multi  HEADERS           : List of test cases to add. Must be even sized, containing "suite;case;" pairs.
  @sinlge WORKING_DIRECTORY : Working dir for this set of tests.
  @single REGEX_PATTERN     : Requires 'REGEX' option. Pattern to filter test cases by.
                              Filter test cases by applying 'REGEX_PATTERN' to "suite_name;case_name;".
  @multi  SUITE_NAMES       : List of suite names to filter the list. Only matching tests are added.
  @option SCAN_ALL          : Scan all test headers provided to the 'add_minitest_executable' call of this 'TARGET'.
#]====================================================================================================================]
function(minitest_from_headers)
  set(options SCAN_ALL)
  set(one_value_args TARGET WORKING_DIRECTORY PREFIX REGEX_PATTERN)
  set(multi_value_args HEADERS SUITE_NAMES)
  cmake_parse_arguments(PARSE_ARGV 0 arg "${options}" "${one_value_args}" "${multi_value_args}")
  smk_check_unparsed_arguments(z_minitest_suite_run_scan ARGUMENT arg_UNPARSED_ARGUMENTS)
  smk_assert_arguments(z_minitest_suite_run_scan ARGUMENTS arg_TARGET arg_PREFIX REQUIRED NOT_EMPTY)

  if(${arg_SCAN_ALL}) # Get header list from target properties and overwrite 'arg_HEADERS'.
    if(DEFINED arg_HEADERS AND NOT "${arg_HEADERS}" STREQUAL "")
      message(WARNING 
        "[Minitest][minitest_from_headers] 'HEADERS' argument ignored when using 'SCAN_ALL' option.")
    endif()
    get_target_property(arg_HEADERS ${arg_TARGET} MINITEST_TARGET_PROP_TEST_HEADERS)
  endif()

  set(test_case_signatures "")
  foreach(header_file IN ITEMS "${arg_HEADERS}") # Scan all given headers for test cases.
    get_target_property(tgt_includes ${arg_TARGET} INCLUDE_DIRECTORIES)
    find_file(header_file_path 
        NAMES "${header_file}"
        PATHS ${tgt_includes}
        NO_DEFAULT_PATH
    )
    if(header_file_path)
      get_filename_component(abs_header_file_path ${header_file_path} ABSOLUTE)
    else()
      message(FATAL_ERROR "[Minitest] Test header file not found in target include directories.")
    endif()

    set(this_file_signatures "")
    z_minitest_from_headers_find_header_tests("${header_file_path}" this_file_signatures)
    list(APPEND test_case_signatures "${this_file_signatures}")
  endforeach()  

  z_minitest_filter_test_list(
    filtered_test_case_signatures
    TESTS         ${test_case_signatures}
    SUITE_NAMES   ${arg_SUITE_NAMES}
    REGEX_PATTERN "${arg_REGEX_PATTERN}"
  )

  z_minitest_add_target_tests_from_list(
    TARGET            ${arg_TARGET}
    WORKING_DIRECTORY ${arg_WORKING_DIRECTORY}
    PREFIX            ${arg_PREFIX}
    TESTS             ${filtered_test_case_signatures}
  )
endfunction()


#[====================================================================================================================[
  @function z_minitest_suite_run_scan(
    [TARGET [name]]
    [PREFIX [name]]
    [WORKING_DIRECTORY [path]]
    [REGEX_PATTERN ["regex pattern string"]]
    [SUITE_NAMES [name1] [name2...]]
  )
  @brief Internal use only. Run test target in scan mode post build, using --scan command. Upon next cmake config, 
         read detected tests from file and add a CTest for each. Filter by 'SUITE_NAMES'.
  @sinlge TARGET
  @single PREFIX
  @sinlge WORKING_DIRECTORY
  @multi SUITE_NAMES : List of suite names to filter the list. Only matching tests are added.
#]====================================================================================================================]
function(z_minitest_suite_run_scan)
  set(options)
  set(one_value_args PREFIX TARGET WORKING_DIRECTORY REGEX_PATTERN)
  set(multi_value_args SUITE_NAMES)
  cmake_parse_arguments(PARSE_ARGV 0 arg "${options}" "${one_value_args}" "${multi_value_args}")
  smk_check_unparsed_arguments(z_minitest_suite_run_scan ARGUMENT arg_UNPARSED_ARGUMENTS)
  smk_assert_arguments(z_minitest_suite_run_scan ARGUMENTS arg_TARGET arg_PREFIX REQUIRED NOT_EMPTY)

  # Run test target in scan mode post build, using --scan command.
  set(temp_scan_result_file 
    "${MINITEST_CONFIG_SCAN_RESULT_DIR}/minitest_scan_result_for_${arg_PREFIX}_${arg_TARGET}.txt")
  set(this_test_scan_result_file "${MINITEST_CONFIG_SCAN_RESULT_DIR}/${arg_PREFIX}_${arg_TARGET}.txt")
  if(NOT DEFINED arg_SUITE_NAMES OR "${arg_SUITE_NAMES}" STREQUAL "")
    set(arg_SUITE_NAMES "")
  else()
    list(JOIN arg_SUITE_NAMES " " arg_SUITE_NAMES) 
  endif()

  add_custom_command(
    TARGET ${arg_TARGET} POST_BUILD
    COMMAND $<TARGET_FILE:${arg_TARGET}> --scan ${arg_SUITE_NAMES} > "${temp_scan_result_file}"
    COMMAND echo "[Minitest] Scanning tests... Prefix: ${arg_PREFIX} Target : ${arg_TARGET}"
    BYPRODUCTS "${temp_scan_result_file}"
    WORKING_DIRECTORY ${MINITEST_CONFIG_SCAN_RESULT_DIR}
  )
  add_custom_command(
    TARGET ${arg_TARGET} POST_BUILD
    COMMAND 
      ${CMAKE_COMMAND} -E copy "${temp_scan_result_file}" "${this_test_scan_result_file}"
    COMMAND 
      ${CMAKE_COMMAND} -E rm "${temp_scan_result_file}"
    COMMAND echo "[Minitest] Tests detected. Reconfigure to add targets."
    BYPRODUCTS "${this_test_scan_result_file}"
    DEPENDS "${temp_scan_result_file}"
    WORKING_DIRECTORY ${MINITEST_CONFIG_SCAN_RESULT_DIR}
  )  

  # This section will run if a matching scan results file is found. Referesh cache to delete.
  # Check if a scan was performed on previous build and add the tests.
  if(EXISTS "${this_test_scan_result_file}")
    file(READ "${this_test_scan_result_file}" run_scan_output)

    # SUITE_NAMES filter is redundant here but filtering again will allow the filter to be applied
    # on re-config, not only re-build.
    z_minitest_filter_test_list(
      filtered_scan_output
      TESTS         ${run_scan_output}
      SUITE_NAMES   ${arg_SUITE_NAMES} 
      REGEX_PATTERN "${arg_REGEX_PATTERN}"
    )

    z_minitest_add_target_tests_from_list(
      TARGET            ${arg_TARGET}
      WORKING_DIRECTORY ${arg_WORKING_DIRECTORY}
      PREFIX            ${arg_PREFIX}
      TESTS             ${filtered_scan_output}
    )  
  else()
    message(STATUS 
      "[Minitest] Build tests and reconfigure to detect. Prefix: ${arg_PREFIX} Target : ${arg_TARGET}")
  endif()
endfunction()


#[====================================================================================================================[
  @function z_minitest_from_headers_find_header_tests(<header-file> <output-var>)
  @brief Internal use only. Scans a C++ header file for unit test macro definitions and extracts the file names. 
         Currently does NOT account for the preprocessor, so if you use this method, the header cannot conditionally 
         exclude unit test definitions - or they will be interpreted as active.
  @sinlge ARGV0        : Path to the header file to scan.
  @sinlge ARGV1        : Name of the variable to store the list of detected "suite:case" name pairs.
#]====================================================================================================================]
function(z_minitest_from_headers_find_header_tests)
  file(READ "${ARGV0}" file_content)
  set(fixture_case_regex "TEST_FA\\(([^,\(]+),([^,)]+)")
  set(test_case_regex "TEST\\(([^,\(]+),([^,)]+)")
  set(test_case_pairs "")
  # Find TEST_FA pattern.
  string(REGEX MATCHALL "${fixture_case_regex}" fixture_matches "${file_content}")
  foreach(regex_match IN LISTS fixture_matches)
    # Extract suite and case name.
    string(REGEX REPLACE "${fixture_case_regex}" "\\1" name "${regex_match}")
    string(REGEX REPLACE "${fixture_case_regex}" "\\2" case_name "${regex_match}")   
    
    string(STRIP "${name}" name)
    string(STRIP "${case_name}" case_name)
    
    list(APPEND test_case_pairs "${name}")
    list(APPEND test_case_pairs "${case_name}")
  endforeach()
  # Repeat for TEST pattern.
  string(REGEX MATCHALL "${test_case_regex}" test_matches "${file_content}")
  foreach(regex_match IN LISTS test_matches)
    string(REGEX REPLACE "${test_case_regex}" "\\1" name "${regex_match}")
    string(REGEX REPLACE "${test_case_regex}" "\\2" case_name "${regex_match}")
    string(STRIP "${name}" name)
    string(STRIP "${case_name}" case_name)
    list(APPEND test_case_pairs "${name}")
    list(APPEND test_case_pairs "${case_name}")
  endforeach()

  set(${ARGV1} ${test_case_pairs} PARENT_SCOPE)
endfunction()


#[====================================================================================================================[
  @function z_minitest_add_target_tests_from_list(
    <TARGET [name]>
    <PREFIX [name]>
    [WORKING_DIRECTORY [path]]
    [TESTS [[suite;case]1] [[[suite;case]2...]]]
  )
  @brief Internal use only. Given a list of test cases. Calls add_test() forwarding arguments and generating a test 
         named "${arg_PREFIX}:${suite_name}:${case_name}" for each case.
  @sinlge TARGET             : Target test executable, must be created using 'minitest_add_executable'.
  @sinlge PREFIX             : Prefix added to the test name forwarded to add_test() for each case.
  @sinlge WORKING_DIRECTORY  : Test dir. Forwarded to add_test() for each case.
  @multi  TESTS              : List of test cases to add. Must be even sized, containing "suite;case;" pairs.
#]====================================================================================================================]
function(z_minitest_add_target_tests_from_list)
  set(options)
  set(one_value_args TARGET WORKING_DIRECTORY PREFIX)
  set(multi_value_args TESTS)
  cmake_parse_arguments(PARSE_ARGV 0 arg "${options}" "${one_value_args}" "${multi_value_args}")
  smk_check_unparsed_arguments(z_minitest_add_target_tests_from_list ARGUMENT arg_UNPARSED_ARGUMENTS)
  smk_assert_arguments(z_minitest_add_target_tests_from_list ARGUMENTS arg_TARGET arg_PREFIX REQUIRED NOT_EMPTY)

  list(LENGTH arg_TESTS n_elements)
  math(EXPR n_tests "(${n_elements} / 2) - 1")
  if(${n_elements} GREATER 0) # Add a ctest for each "suite;case;" pair.
    foreach(idx RANGE 0 ${n_tests})
      math(EXPR suite_idx "${idx} * 2")
      math(EXPR case_idx "${suite_idx} + 1")

      list(GET arg_TESTS ${suite_idx} suite_name)
      list(GET arg_TESTS ${case_idx} case_name)
    
      string(STRIP ${suite_name} suite_name)
      string(STRIP ${case_name} case_name)

      set(_TEST_NAME "${arg_PREFIX}:${suite_name}:${case_name}")
      add_test(
        NAME ${_TEST_NAME}
        COMMAND $<TARGET_FILE:${arg_TARGET}> 
              ${suite_name} 
              ${case_name}
        WORKING_DIRECTORY ${arg_WORKING_DIRECTORY}
      )
      message(VERBOSE "[Minitest] Added test: ${idx} - ${_TEST_NAME}")
    endforeach()
  endif()
endfunction()


#[====================================================================================================================[
  @function z_minitest_filter_test_list(
    <OutVar>
    <TESTS [[suite;case]1] [[[suite;case]2...]]>
    [SUITE_NAMES [name1] [name2...]]
    [REGEX_PATTERN ["regex pattern string"]]
  )
  @brief Filter a list of tests by matching suite names and/or a regex pattern.
#]====================================================================================================================]
function(z_minitest_filter_test_list)
  set(options)
  set(one_value_args REGEX_PATTERN)
  set(multi_value_args TESTS SUITE_NAMES)
  cmake_parse_arguments(PARSE_ARGV 1 arg "${options}" "${one_value_args}" "${multi_value_args}")
  smk_check_unparsed_arguments(z_minitest_filter_test_list ARGUMENT arg_UNPARSED_ARGUMENTS)
  smk_assert_arguments(z_minitest_filter_test_list ARGUMENTS arg_TESTS arg_PREFIX REQUIRED NOT_EMPTY)

  set(filtered_list "")
  list(LENGTH arg_TESTS n_elements)
  math(EXPR n_tests "(${n_elements} / 2) - 1")

  foreach(idx RANGE 0 ${n_tests})
    math(EXPR suite_idx "${idx} * 2")
    math(EXPR case_idx "${suite_idx} + 1")
    
    list(GET arg_TESTS ${suite_idx} suite_name)
    list(GET arg_TESTS ${case_idx} case_name)

    string(STRIP ${suite_name} suite_name)
    string(STRIP ${case_name} case_name)

    set(this_test_signature "${suite_name};${case_name};")  
    set(add_this_test TRUE)

    # Filter by suite name.
    if(DEFINED arg_SUITE_NAMES AND NOT "${arg_SUITE_NAMES}" STREQUAL "")
      set(add_this_test FALSE)
      foreach(filter_suite IN LISTS arg_SUITE_NAMES)
        if("${filter_suite}" STREQUAL "${suite_name}")
            set(add_this_test TRUE)        
        endif()
      endforeach()
    endif()
    
    # Filter by regex pattern
    if(${add_this_test})
      if(DEFINED arg_REGEX_PATTERN AND NOT "${arg_REGEX_PATTERN}" STREQUAL "")
        set(add_this_test FALSE)
        string(REGEX MATCH "${arg_REGEX_PATTERN}" this_regex_matched ${this_test_signature})
        if(DEFINED this_regex_matched AND NOT "${this_regex_matched}" STREQUAL "")
          set(add_this_test TRUE)
        endif()
      endif()
    endif()

    if(${add_this_test})
      list(APPEND filtered_list ${suite_name})
      list(APPEND filtered_list ${case_name})
    endif()
  endforeach()

  set(${ARGV0} ${filtered_list} PARENT_SCOPE)
endfunction()

#[====================================================================================================================[
Copyright 2025 Anton Yashchenko
Licensed under the Apache License, Version 2.0(the "License");
=======================================================================================================================
@project: Minitest Library
@author(s): Anton Yashchenko
@website: https://www.acpp.dev
=======================================================================================================================
@notes Minitest CMake Integration

=======================================================================================================================
@rationale Minitest CMake Integration
 
- Wouldnt it be useful to have a cli arg to only detect test cases? Yes. The difficulty is running this target is not
  trivial at config-time. When running the test target its dependencies may not be available yet. try_run() and
  try_compile() methods seems like an option at first. Using these function on every test would be expensive,
  the dependencies must be scanned manually then passed to this function, building a project would require some
  custom project format which will not work for a library. Furthermore, it complicates cross compilation.
  Currently the next best solution is to run the scans at build time then cache the results, upon next build,
  the cached tests will be added. Use add_custom_command.

#]====================================================================================================================]
