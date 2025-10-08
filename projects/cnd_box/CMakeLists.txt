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
cmake_minimum_required (VERSION 3.8...3.10)

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
#find_package(wpl_box REQUIRED CONFIG)

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
target_include_directories(cnd_compiler_interface 
  INTERFACE "${cnd_compiler_interface_HEADERS_DIR}"
)
target_sources(cnd_compiler_interface INTERFACE
  "${cnd_compiler_interface_SOURCES_DIR}/CliMain.cpp" # SsgcCliMain.hpp
  "${cnd_compiler_interface_SOURCES_DIR}/CliDriver.cpp" # SsgcDriver.hpp
)
target_link_libraries(cnd_compiler_interface 
  INTERFACE mta_box::mta_library
            cxxx_box::cxxx_library
            #wpl_box::wpl_library
)
set_target_properties(
  cnd_compiler_interface 
  PROPERTIES  CMAKE_CXX_STANDARD 23
              CMAKE_CXX_STANDARD_REQUIRED ON
              CMAKE_CXX_EXTENSIONS OFF
)

# SSG Compiler CLI: command line interface executable.
#
# The implementation is contained inside 'cnd_compiler_interface'.
# This target only calls 'ssgc::CliMain' method defined the interface. 
add_executable(ssgc_compiler_executable
  "${cnd_compiler_interface_SOURCES_DIR}/Main.cpp"
)
target_link_libraries(ssgc_compiler_executable PRIVATE cnd_compiler_interface)

#[============================================================================[
  Test Targets
#]============================================================================]

#[====================================[
  Test Suite : UtCompeval
#]====================================]
# This suite requires test scripts defined in the 'ut/res' directory. But we dont wan't to perform tests in the source
# tree. So we copy the test scripts folder to the build directory.
set(UT_COMPEVAL_TEST_CODE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ut/res/test-code/compeval")
set(UT_COMPEVAL_TEST_CODE_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/test-code/compeval")
execute_process(
  COMMAND 
    ${CMAKE_COMMAND} -E copy_directory_if_different "${UT_COMPEVAL_TEST_CODE_SOURCE_DIR}" "${UT_COMPEVAL_TEST_CODE_BINARY_DIR}"
)

minitest_add_executable(
  NAME                UtCompeval
  INCLUDE_DIRECTORIES ut
  LINK_LIBS           cnd_compiler_interface
  TEST_HEADERS        UtCompeval.hpp
)

minitest_from_headers(
  PREFIX UtCndFrontEnd
  TARGET UtCompeval
  HEADERS UtCompeval.hpp
  WORKING_DIRECTORY "${UT_COMPEVAL_TEST_CODE_BINARY_DIR}"
)

minitest_add_executable(
  NAME                UtCompilerCli
  INCLUDE_DIRECTORIES ut
  LINK_LIBS           cnd_compiler_interface
  TEST_HEADERS        UtCompilerCli.hpp
)

minitest_from_headers(
  PREFIX UtCndFrontEnd
  TARGET UtCompilerCli
  HEADERS UtCompilerCli.hpp
  WORKING_DIRECTORY "${UT_COMPEVAL_TEST_CODE_BINARY_DIR}"
)

unset(UT_COMPEVAL_TEST_CODE_SOURCE_DIR)
unset(UT_COMPEVAL_TEST_CODE_BINARY_DIR)

#[====================================[
  Test Suite : UtCndParserPrimaryExpr 
             & UtParserGrammarRules
#]====================================]

minitest_add_executable(
  NAME                UtCndParserPrimaryExpr
  INCLUDE_DIRECTORIES ut
  LINK_LIBS           cnd_compiler_interface
  TEST_HEADERS        UtParserPrimaryExpr.hpp
)

minitest_from_headers(
  PREFIX UtCndFrontEnd
  TARGET UtCndParserPrimaryExpr
  HEADERS UtParserPrimaryExpr.hpp
)

minitest_add_executable(
  NAME                UtParserGrammarRules
  INCLUDE_DIRECTORIES ut
  WORKING_DIRECTORY   ut/res
  LINK_LIBS           cnd_compiler_interface
  TEST_HEADERS        UtParserGrammarRules.hpp
)

minitest_from_headers(
  PREFIX UtCndFrontEnd
  TARGET UtParserGrammarRules
  HEADERS UtParserGrammarRules.hpp
)

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
