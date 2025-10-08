#[====================================================================================================================[
Copyright 2025 Anton Yashchenko
Licensed under the Apache License, Version 2.0(the "License");
=======================================================================================================================
@project: [MINITEST] Minimal Test Framework
@author(s): Anton Yashchenko
@website: https://www.acpp.dev
=======================================================================================================================
@file 
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
cmake_minimum_required (VERSION 3.8...3.10)

project(minitest_box
	VERSION 0.0.0.0
	DESCRIPTION "[MINITEST] Minimal Test Framework"
	HOMEPAGE_URL "www.acpp.dev"
	LANGUAGES C CXX	
)

find_package(ssgmono_box REQUIRED CONFIG)
include(SsgProjectBase)
ssg_setup_standard_project_vars()

#[============================================================================[
  Package Options
#]============================================================================]
option(MINITEST_BOX_ENABLE_TESTS 
  "Enable test targets. Requires 'MINITEST_CONFIG_ENABLE_CMAKE_INTEGRATION'." 
  ON
)
option(MINITEST_BOX_ENABLE_DOCS 
  "Enable building documentation." 
  OFF
)
option(MINITEST_BOX_ENABLE_CTEST_INTEGRATION 
  "Enables and includes unit Minitest-CTest integration functions." 
  ON
)


#[============================================================================[
  Package Targets
#]============================================================================]
include(GNUInstallDirs)

# minitest_box::minitest_library
set(MINITEST_LIBRARY_HEADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/inc)
set(MINITEST_LIBRARY_SOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
add_library(minitest_library INTERFACE)
target_include_directories(minitest_library 
  INTERFACE 
    $<BUILD_INTERFACE:${MINITEST_LIBRARY_HEADERS_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_sources(minitest_library 
  INTERFACE 
  "${MINITEST_LIBRARY_HEADERS_DIR}/common.hpp"
  "${MINITEST_LIBRARY_HEADERS_DIR}/fixture.hpp"
  "${MINITEST_LIBRARY_HEADERS_DIR}/form.hpp"
  "${MINITEST_LIBRARY_HEADERS_DIR}/minitest.hpp"
  "${MINITEST_LIBRARY_HEADERS_DIR}/test_framework.hpp"
  "${MINITEST_LIBRARY_HEADERS_DIR}/unit_test.hpp"
)
set_target_properties(minitest_library 
  PROPERTIES  CMAKE_CXX_STANDARD 14
              CMAKE_CXX_STANDARD_REQUIRED ON
)

#[============================================================================[
  Package Modules
#]============================================================================]

# MINITEST_LIBRARY_MODULES_DIR : minitest ctest integration scripts dir
# @note this must be exported in the subproject config file.
set(MINITEST_LIBRARY_CMAKE_MODULES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
list(APPEND CMAKE_MODULE_PATH "${MINITEST_LIBRARY_CMAKE_MODULES_DIR}")

# enable_tests() must also be called by the consuming project.
if(${MINITEST_BOX_ENABLE_CTEST_INTEGRATION})
  include(UseMinitest)
endif()


#[============================================================================[
  Test Targets
#]============================================================================]
if(${MINITEST_BOX_ENABLE_CTEST_INTEGRATION} AND ${MINITEST_BOX_ENABLE_TESTS})
  if(PROJECT_IS_TOP_LEVEL)
    enable_testing()
  endif()

  # ut_checks_asserts
  minitest_add_executable(
    NAME ut_checks_asserts
    INCLUDE_DIRECTORIES test
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
if(${MINITEST_BOX_ENABLE_DOCS})
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
endif(${MINITEST_BOX_ENABLE_DOCS})


#[============================================================================[
  Subproject Exports
#]============================================================================]
include(SsgProjectPackageUtils)
ssg_export_subproject(
	"${PROJECT_NAME}"
	TARGETS minitest_library
	MODULE_PATHS "${MINITEST_LIBRARY_CMAKE_MODULES_DIR}"
	REDIRECT_FIND_PACKAGE
)

#[============================================================================[
  Install Package
#]============================================================================]
if(PROJECT_IS_TOP_LEVEL)
	install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
    DESTINATION "${CMAKE_INSTALL_PREFIX}"
	)
  
	install(FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/StdMake.cmake"
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/UseMinitest.cmake"
    DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake"
	)

  install(TARGETS minitest_library
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  )

  include(CMakePackageConfigHelpers)
  configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/config.cmake.in
	  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
	  INSTALL_DESTINATION "${CMAKE_INSTALL_PREFIX}"
  )

  write_basic_package_version_file(
	  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
	  VERSION "${PROJECT_VERSION}"
	  COMPATIBILITY AnyNewerVersion
  )
	
endif()

#[====================================================================================================================[
Copyright 2025 Anton Yashchenko
Licensed under the Apache License, Version 2.0(the "License");
=======================================================================================================================
@project: [MINITEST] Minimal Test Framework
@author(s): Anton Yashchenko
@website: https://www.acpp.dev
#]====================================================================================================================]