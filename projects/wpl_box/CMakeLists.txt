#[============================================================================[
Copyright 2025 Anton Yashchenko
Licensed under the GNU Affero General Public License, Version 3.
===============================================================================
@project: [WPL] Windows Process Library
@author(s): Anton Yashchenko
@website: https://www.acpp.dev
===============================================================================
@file 
@ingroup
@brief 
#]============================================================================]
cmake_minimum_required (VERSION 3.8)

project(wpl_box
	VERSION 0.0.0.0
	DESCRIPTION "Windows Process Library"
	HOMEPAGE_URL "www.acpp.dev"
	LANGUAGES C CXX	
)

#[============================================================================[
  Find Dependencies
#]============================================================================]
find_package(ssgmono_box REQUIRED CONFIG)
find_package(minitest_box REQUIRED CONFIG)


#[============================================================================[
  Project Init
#]============================================================================]
include(SsgProjectBase)
ssg_setup_standard_project_vars()


#[============================================================================[
  Source Targets
#]============================================================================]
set(WPL_LIBRARY_HEADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/inc)
set(WPL_LIBRARY_SOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_library(wpl_library "${WPL_LIBRARY_SOURCES_DIR}/wpl_core.cpp")
target_include_directories(wpl_library PUBLIC "${WPL_LIBRARY_HEADERS_DIR}")
target_sources(wpl_library 
  INTERFACE 
  "${WPL_LIBRARY_HEADERS_DIR}/wpl_core.hpp"
)

set_target_properties(wpl_library 
  PROPERTIES  CMAKE_CXX_STANDARD 20
              CMAKE_CXX_STANDARD_REQUIRED ON
)


#[============================================================================[
  Test Targets
#]============================================================================]
if(PROJECT_IS_TOP_LEVEL)
  enable_testing()
endif()

minitest_add_executable(
  NAME                WplModuleTests
  INCLUDE_DIRECTORIES test
  LINK_LIBS           wpl_library
  TEST_HEADERS        ut_wpl.hpp
)

minitest_from_headers(
  PREFIX UtWpl
  TARGET WplModuleTests
  SCAN_ALL
)


#[============================================================================[
  Subproject Exports
#]============================================================================]
if (NOT PROJECT_IS_TOP_LEVEL)
  get_directory_property(WPL_BOX_EXPORT_DIR SSG_PROJECT_EXPORT_DIR)
  get_directory_property(WPL_BOX_IMPORT_DIR SSG_PROJECT_IMPORT_DIR)

  ssg_configure_alias_targets_file(
    TARGETS wpl_library
    PREFIX ${PROJECT_NAME}
    OUTPUT_FILE_PATH "${WPL_BOX_IMPORT_DIR}/${PROJECT_NAME}-subproject-targets.cmake"
    INPUT_FILE_PATH "${WPL_BOX_EXPORT_DIR}/subproject-targets.cmake.in"
  )

  ssg_export_subproject_config(${PROJECT_NAME} 
    CONFIG_SUBPROJECT "${WPL_BOX_EXPORT_DIR}/subproject-config.cmake.in"
    OUTPUT_DIR "${WPL_BOX_IMPORT_DIR}"
  ) 
endif()


#[============================================================================[
Copyright 2025 Anton Yashchenko
Licensed under the GNU Affero General Public License, Version 3.
===============================================================================
@project: [WPL] Windows Process Library
@author(s): Anton Yashchenko
@website: https://www.acpp.dev
#]============================================================================]