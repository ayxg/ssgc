#[============================================================================[
Copyright 2025 Anton Yashchenko
Licensed under the Apache License, Version 2.0(the "License");
===============================================================================
@project: [CND] C& Programming Language Official Compiler
@author(s): Anton Yashchenko
@website: https://www.acpp.dev
===============================================================================
@file 
@ingroup
@brief 
#]============================================================================]
cmake_minimum_required (VERSION 3.8)

project(cnd_box
	VERSION 0.0.0.0
	DESCRIPTION "[CND] C& Programming Language Official Compiler"
	HOMEPAGE_URL "www.acpp.dev"
	LANGUAGES C CXX	
)

#[============================================================================[
  Find Dependencies
#]============================================================================]
find_package(ssgmono_box REQUIRED CONFIG)

find_package(minitest_box REQUIRED CONFIG)
find_package(mta_box REQUIRED CONFIG)
find_package(cxxx_box REQUIRED CONFIG)
find_package(wpl_box REQUIRED CONFIG)

#[============================================================================[
  Project Init
#]============================================================================]
include(SsgProjectBase)
ssg_setup_standard_project_vars()

#[============================================================================[
  Source Targets
#]============================================================================]
set(cnd_compiler_interface_HEADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/inc)
set(cnd_compiler_interface_SOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Compiler Library : for use as an embedded api.
add_library(cnd_compiler_interface INTERFACE)
target_include_directories(cnd_compiler_interface INTERFACE "${cnd_compiler_interface_HEADERS_DIR}")
target_link_libraries(cnd_compiler_interface 
  INTERFACE mta_box::mta_library
            cxxx_box::cxxx_library
            wpl_box::wpl_library
)
set_target_properties(
  cnd_compiler_interface 
  PROPERTIES  CMAKE_CXX_STANDARD 23
              CMAKE_CXX_STANDARD_REQUIRED ON
              CMAKE_CXX_EXTENSIONS OFF
)

#target_sources(cnd_compiler_library 
#  PUBLIC 
#  "${CND_LIBRARY_HEADERS_DIR}/cgui.hpp"
#)

# Compiler CLI executable
#add_executable(cnd_compiler_executable
#  "${CND_LIBRARY_SOURCES_DIR}/cnd_main.cpp"
#  "${CND_LIBRARY_SOURCES_DIR}/cli_main.cpp"
#  "${CND_LIBRARY_SOURCES_DIR}/driver.cpp"
#)
#target_include_directories(cnd_compiler_executable PRIVATE "${CND_LIBRARY_HEADERS_DIR}")
#target_sources(cnd_compiler_executable 
#  PUBLIC 
#  "${CND_LIBRARY_HEADERS_DIR}/cgui.hpp"
#)
#target_link_libraries(cnd_compiler_executable
#  PRIVATE
#    nlohmann_json::nlohmann_json
#    # SSG Modules
#    mta_box::mta_library
#    cxxx_box::cxxx_library
#    wpl_box::wpl_library
#)
#set_target_properties(
#  cnd_compiler_executable 
#  PROPERTIES  CMAKE_CXX_STANDARD 23
#              CMAKE_CXX_STANDARD_REQUIRED ON
#             CMAKE_CXX_EXTENSIONS OFF
#)

#[============================================================================[
  Test Targets
#]============================================================================]

# Add static unit tests.
#target_sources(cnd_compiler_library PRIVATE
#  ut/sut_LexerBasics.cpp
#  ut/sut_LexerLexEscapedCharSequence.cpp
#  ut/sut_LexerLexIdentifier.cpp
#  ut/sut_LexerLexNumber.cpp
#  ut/sut_LexerLexPunctuator.cpp
#)
#target_sources(cnd_compiler_executable PRIVATE
#  ut/sut_LexerBasics.cpp
#  ut/sut_LexerLexEscapedCharSequence.cpp
# ut/sut_LexerLexIdentifier.cpp
#  ut/sut_LexerLexNumber.cpp
#  ut/sut_LexerLexPunctuator.cpp
#)

# Test Group : CndModuleTests
minitest_add_executable(
  NAME                CndModuleTests
  INCLUDE_DIRECTORIES ut
  WORKING_DIRECTORY   ut/res
  LINK_LIBS           cnd_compiler_interface
  TEST_HEADERS        test_trtools_parser.hpp
)

minitest_from_headers(
  PREFIX UtCnd
  TARGET CndModuleTests
  HEADERS test_trtools_parser.hpp
)

#[============================================================================[
  Subproject Exports
#]============================================================================]
include(SsgProjectPackageUtils)
ssg_export_subproject(
	"${PROJECT_NAME}"
	TARGETS cnd_compiler_interface
	REDIRECT_FIND_PACKAGE
)


#[============================================================================[
Copyright 2025 Anton Yashchenko
Licensed under the Apache License, Version 2.0(the "License");
===============================================================================
@project: [CND] C& Programming Language Official Compiler
@author(s): Anton Yashchenko
@website: https://www.acpp.dev
#]============================================================================]
