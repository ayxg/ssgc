#[============================================================================[
Copyright 2025 Anton Yashchenko
Licensed under the GNU Affero General Public License, Version 3.
===============================================================================
@project: [CXXX] C++ Standard Extended Library Package
@author(s): Anton Yashchenko
@website: https://www.acpp.dev
===============================================================================
@file 
@ingroup
@brief 
#]============================================================================]
cmake_minimum_required (VERSION 3.8)

project(cxxx_box
	VERSION 0.0.0.0
	DESCRIPTION "C++ Standard Extended"
	HOMEPAGE_URL "www.acpp.dev"
	LANGUAGES C CXX	
)

#[============================================================================[
  Find Dependencies
#]============================================================================]
find_package(ssgmono_box REQUIRED CONFIG)
find_package(minitest_box REQUIRED CONFIG)
find_package(mta_box REQUIRED CONFIG)

#[============================================================================[
  Project Init
#]============================================================================]
include(SsgProjectBase)
ssg_setup_standard_project_vars()


#[============================================================================[
  Source Targets
#]============================================================================]
set(CXXX_LIBRARY_HEADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/inc)

add_library(cxxx_library INTERFACE)
target_include_directories(cxxx_library INTERFACE "${CXXX_LIBRARY_HEADERS_DIR}")
target_sources(cxxx_library 
  INTERFACE 
  "${CXXX_LIBRARY_HEADERS_DIR}/cxxx.hpp"
  "${CXXX_LIBRARY_HEADERS_DIR}/cxxx_enumerated_flags.hpp"
  "${CXXX_LIBRARY_HEADERS_DIR}/cxxx_expected.hpp"
  "${CXXX_LIBRARY_HEADERS_DIR}/cxxx_fsys.hpp"
  "${CXXX_LIBRARY_HEADERS_DIR}/cxxx_import_std.hpp"
  "${CXXX_LIBRARY_HEADERS_DIR}/cxxx_macrodef.hpp"
)
target_link_libraries(cxxx_library INTERFACE mta_box::mta_library)
set_target_properties(cxxx_library 
  PROPERTIES  CMAKE_CXX_STANDARD 20
              CMAKE_CXX_STANDARD_REQUIRED ON
)

unset(CXXX_LIBRARY_HEADERS_DIR)
#[============================================================================[
  Test Targets
#]============================================================================]
if(PROJECT_IS_TOP_LEVEL)
  enable_testing()
endif()

minitest_add_executable(
  NAME                CxxxModuleTests
  INCLUDE_DIRECTORIES test
  LINK_LIBS           cxxx_library
  TEST_HEADERS        ut_expected.h
)

minitest_from_headers(
  PREFIX UtCxxx
  TARGET CxxxModuleTests
  SCAN_ALL
)

#[============================================================================[
  Subproject Exports
#]============================================================================]
include(SsgProjectPackageUtils)
ssg_export_subproject(
	"${PROJECT_NAME}"
	TARGETS cxxx_library
	REDIRECT_FIND_PACKAGE
)

#[============================================================================[
Copyright 2025 Anton Yashchenko
Licensed under the GNU Affero General Public License, Version 3.
===============================================================================
@project: [CXXX] C++ Standard Extended Library Package
@author(s): Anton Yashchenko
@website: https://www.acpp.dev
#]============================================================================]