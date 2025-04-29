#[====================================================================================================================[
Copyright 2025 Anton Yashchenko
Licensed under the Apache License, Version 2.0(the "License");
=======================================================================================================================
@project: Minitest Framework
@author(s): Anton Yashchenko
@website: https://www.acpp.dev
=======================================================================================================================
@file Minitest Framework 'CMakeLists.txt'
@ingroup minitest_cmake
@brief C++14 minimal unit testing framework with CMake integration.

Options:
  [MINITEST_CONFIG_ENABLE_CMAKE_INTEGRATION]: Enables and includes unit Minitest-CTest integration functions.
  [MINITEST_CONFIG_ENABLE_TESTS]            : Add minitest library unit test targets.
                                              Requires 'MINITEST_CONFIG_ENABLE_CMAKE_INTEGRATION'.
  [MINITEST_CONFIG_AUXILLARY_DIR [path]]    : Root path for minitest generated code and files, 
                                              defaults to "${CMAKE_BINARY_DIR}/minitest".
  [MINITEST_CONFIG_CODEGEN_DIR [path]]      : Path where minitest sources are generated. 
                                              Defaults to "${MINITEST_CONFIG_AUXILLARY_DIR}/codegen"
  [MINITEST_CONFIG_SCAN_RESULT_DIR [path]]  : Path where minitest scan results are stored. 
                                              Defaults to "${MINITEST_CONFIG_AUXILLARY_DIR}/scan_results"
#]====================================================================================================================]
cmake_minimum_required (VERSION 3.8)
project(MinitestFramework LANGUAGES C CXX)

if(CMAKE_CXX_STANDARD LESS 14)
  message(FATAL_ERROR "[Minitest] C++14 is required to build Minitest library.")
endif()

option(MINITEST_CONFIG_ENABLE_TESTS "[Minitest] Enable test targets. Requires 'MINITEST_CONFIG_ENABLE_CMAKE_INTEGRATION'." ON)
option(MINITEST_CONFIG_BUILD_DOCS "[Minitest] Enable building documentation." OFF)
option(MINITEST_CONFIG_ENABLE_CMAKE_INTEGRATION "[Minitest] Enables and includes unit Minitest-CTest integration functions." ON)

#[============================================================================[
  Source Targets
#]============================================================================]
add_library(MinitestLibrary INTERFACE)
target_include_directories(MinitestLibrary INTERFACE "source/inc")
add_library(MinitestProject::MinitestLibrary ALIAS MinitestLibrary)

# Must be set for 'UseMinitest.cmake'.
set(MINITEST_LIBRARY MinitestLibrary) 

# enable_tests() must also be called by the consuming project.
if(${MINITEST_CONFIG_ENABLE_CMAKE_INTEGRATION})
  include("cmake/UseMinitest.cmake")
endif()

#[============================================================================[
  Test Targets
#]============================================================================]
if(${MINITEST_CONFIG_ENABLE_CMAKE_INTEGRATION} AND ${MINITEST_CONFIG_ENABLE_TESTS})
  # ut_checks_asserts
  minitest_add_executable(
    NAME ut_checks_asserts
    INCLUDE_DIRECTORIES source/test
    TEST_HEADERS ut_checks_asserts.hpp
  )

  minitest_from_headers(
    PREFIX UtMinitest
    TARGET ut_checks_asserts
    SCAN_ALL
  )
endif() 

#[============================================================================[
  Documentation
#]============================================================================]
if(${MINITEST_CONFIG_BUILD_DOCS})
  find_package(Doxygen)
  if (DOXYGEN_FOUND)
    set(minitest_doxygen_in ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
    set(minitest_doxygen_out ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    configure_file(${minitest_doxygen_in} ${minitest_doxygen_out} @ONLY)
    message(VERBOSE "[Minitest] Documentation enabled and configured.")

    add_custom_target(doc_doxygen ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${minitest_doxygen_out}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation for [Minitest]"
        VERBATIM )

    unset(minitest_doxygen_in)
    unset(minitest_doxygen_out)
  endif (DOXYGEN_FOUND)
endif(${MINITEST_CONFIG_BUILD_DOCS})


#[====================================================================================================================[
Copyright 2025 Anton Yashchenko
Licensed under the Apache License, Version 2.0(the "License");
=======================================================================================================================
@project: Minitest Framework
@author(s): Anton Yashchenko
@website: https://www.acpp.dev
#]====================================================================================================================]