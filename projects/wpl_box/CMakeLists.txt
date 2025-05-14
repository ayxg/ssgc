#[============================================================================[
Copyright 2025 Anton Yashchenko
Licensed under the Apache License, Version 2.0(the "License");
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
include(SsgProjectPackageUtils)
ssg_export_subproject(
	"${PROJECT_NAME}"
	TARGETS wpl_library
	REDIRECT_FIND_PACKAGE
)

#[============================================================================[
Copyright 2025 Anton Yashchenko
Licensed under the Apache License, Version 2.0(the "License");
===============================================================================
@project: [WPL] Windows Process Library
@author(s): Anton Yashchenko
@website: https://www.acpp.dev
#]============================================================================]