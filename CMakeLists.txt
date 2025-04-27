cmake_minimum_required(VERSION 3.29.5) # 3.29.5
include(FetchContent)

#------------------------------------------------------------------------------
# SSG_ROOT Project-Specific Utilities  
#------------------------------------------------------------------------------
include(bsys/ssg/ssg.cmake)
include(bsys/ssg/gitpkg.cmake)

# Setup project options for internal executables and libraries.
macro(SSG_ROOT_SETUP_PROJECT_OPTIONS)
  include(CMakeDependentOption)
  include(CheckCXXCompilerFlag)
  include(bsys/cmake/LibFuzzer.cmake)

  option(SSG_ROOT_CXX_ENABLE_HARDENING "Enable hardening" ON)
  option(SSG_ROOT_CXX_ENABLE_COVERAGE "Enable coverage reporting" OFF)
  cmake_dependent_option(
      SSG_ROOT_CXX_ENABLE_GLOBAL_HARDENING
      "Attempt to push hardening options to built dependencies"
      ON
      SSG_ROOT_CXX_ENABLE_HARDENING
      OFF)

  # check undefined behavior/address sanitizer support
  ssg_check_cxx_sanitizer_support()

  if(NOT PROJECT_IS_TOP_LEVEL OR SSG_ROOT_CXX_PACKAGING_MAINTAINER_MODE)
      option(SSG_ROOT_CXX_ENABLE_IPO "Enable IPO/LTO" OFF)
      option(SSG_ROOT_CXX_WARNINGS_AS_ERRORS "Treat Warnings As Errors" OFF)
      option(SSG_ROOT_CXX_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
      option(SSG_ROOT_CXX_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" OFF)
      option(SSG_ROOT_CXX_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
      option(SSG_ROOT_CXX_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" OFF)
      option(SSG_ROOT_CXX_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
      option(SSG_ROOT_CXX_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
      option(SSG_ROOT_CXX_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
      option(SSG_ROOT_CXX_ENABLE_CLANG_TIDY "Enable clang-tidy" OFF)
      option(SSG_ROOT_CXX_ENABLE_CPPCHECK "Enable cpp-check analysis" OFF)
      option(SSG_ROOT_CXX_ENABLE_PCH "Enable precompiled headers" OFF)
      option(SSG_ROOT_CXX_ENABLE_CACHE "Enable ccache" OFF)
  else()
      option(SSG_ROOT_CXX_ENABLE_IPO "Enable IPO/LTO" ON)
      option(SSG_ROOT_CXX_WARNINGS_AS_ERRORS "Treat Warnings As Errors" ON)
      option(SSG_ROOT_CXX_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
      option(SSG_ROOT_CXX_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" ${SSG_CXX_COMPILER_HAS_ASAN})
      option(SSG_ROOT_CXX_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
      option(SSG_ROOT_CXX_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" ${SSG_CXX_COMPILER_HAS_UBSAN})
      option(SSG_ROOT_CXX_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
      option(SSG_ROOT_CXX_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
      option(SSG_ROOT_CXX_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
      option(SSG_ROOT_CXX_ENABLE_CLANG_TIDY "Enable clang-tidy" ON)
      option(SSG_ROOT_CXX_ENABLE_CPPCHECK "Enable cpp-check analysis" ON)
      option(SSG_ROOT_CXX_ENABLE_PCH "Enable precompiled headers" OFF)
      option(SSG_ROOT_CXX_ENABLE_CACHE "Enable ccache" ON)
  endif()

  if(NOT PROJECT_IS_TOP_LEVEL)
  mark_as_advanced(
          SSG_ROOT_CXX_ENABLE_IPO
          SSG_ROOT_CXX_WARNINGS_AS_ERRORS
          SSG_ROOT_CXX_ENABLE_USER_LINKER
          SSG_ROOT_CXX_ENABLE_SANITIZER_ADDRESS
          SSG_ROOT_CXX_ENABLE_SANITIZER_LEAK
          SSG_ROOT_CXX_ENABLE_SANITIZER_UNDEFINED
          SSG_ROOT_CXX_ENABLE_SANITIZER_THREAD
          SSG_ROOT_CXX_ENABLE_SANITIZER_MEMORY
          SSG_ROOT_CXX_ENABLE_UNITY_BUILD
          SSG_ROOT_CXX_ENABLE_CLANG_TIDY
          SSG_ROOT_CXX_ENABLE_CPPCHECK
          SSG_ROOT_CXX_ENABLE_COVERAGE
          SSG_ROOT_CXX_ENABLE_PCH
          SSG_ROOT_CXX_ENABLE_CACHE)
  endif()

  ssg_check_cxx_libfuzzer_support(LIBFUZZER_SUPPORTED)
  if(LIBFUZZER_SUPPORTED AND (SSG_ROOT_CXX_ENABLE_SANITIZER_ADDRESS 
      OR SSG_ROOT_CXX_ENABLE_SANITIZER_THREAD 
      OR SSG_ROOT_CXX_ENABLE_SANITIZER_UNDEFINED))
      set(DEFAULT_FUZZER ON)
  else()
      set(DEFAULT_FUZZER OFF)
  endif()

  option(SSG_ROOT_CXX_BUILD_FUZZ_TESTS "Enable fuzz testing executable" ${DEFAULT_FUZZER})
endmacro()


macro(SSG_ROOT_HANDLE_PROJECT_OPTIONS)
  if(SSG_ROOT_CXX_ENABLE_IPO)
    include(CheckIPOSupported)
    check_ipo_supported(RESULT result OUTPUT output)
    if(result)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
    else()
    message(WARNING "IPO is not supported: ${output}")
    endif()
  endif()

  # check undefined behavior/address sanitizer support
  ssg_check_cxx_sanitizer_support()

  if(SSG_ROOT_CXX_ENABLE_HARDENING AND SSG_ROOT_CXX_ENABLE_GLOBAL_HARDENING)
    include(bsys/cmake/Hardening.cmake)
    if(NOT SSG_CXX_COMPILER_HAS_UBSAN 
        OR SSG_ROOT_CXX_ENABLE_SANITIZER_UNDEFINED
        OR SSG_ROOT_CXX_ENABLE_SANITIZER_ADDRESS
        OR SSG_ROOT_CXX_ENABLE_SANITIZER_THREAD
        OR SSG_ROOT_CXX_ENABLE_SANITIZER_LEAK)
        set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
        set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    message("${SSG_ROOT_CXX_ENABLE_HARDENING} ${ENABLE_UBSAN_MINIMAL_RUNTIME} ${SSG_ROOT_CXX_ENABLE_SANITIZER_UNDEFINED}")
    SSG_ROOT_CXX_enable_hardening(SSG_ROOT_CXX_options ON ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()
endmacro()

#------------------------------------------------------------------------------
# SSG_ROOT Project Config  
#------------------------------------------------------------------------------

# C++20 is the default standard for SSG projects
if (NOT DEFINED CMAKE_CXX_STANDARD)
  SET(CMAKE_CXX_STANDARD 23)
endif()

# strongly encouraged to enable this globally to avoid conflicts between
# -Wpedantic being enabled and -std=c++20 and -std=gnu++20 for example
# when compiling with PCH enabled
SET(CMAKE_CXX_EXTENSIONS OFF)

project(SSG_ROOT
	VERSION 0.0.0.0
	DESCRIPTION "SSG Root CMake Project"
	HOMEPAGE_URL "www.ssg.com"
	LANGUAGES CXX	
)

SSG_ASSERT_TOP_LEVEL_PROJECT()
SSG_ASSERT_OUT_OF_SOURCE_BUILD()

# Create root project options.
option(SSG_ROOT_CXX_BUILD_SHARED_DEPS "Build dependencies as shared or static by default." OFF)


#------------------------------------------------------------------------------
# Import and Build Dependencies
#------------------------------------------------------------------------------

# TODO: Setup global options for dependencies and option to make a 'system' dependency
# which may avoid default compiler warnings and settings.

# static or shared libraries ?
if(SSG_ROOT_CXX_BUILD_SHARED_DEPS)
  set(BUILD_SHARED_LIBS ON)
else()
  set(BUILD_SHARED_LIBS OFF) # static default
endif()

# Fetch the required dependencies using FetchContent
SSG_GITPKG_IMGUI_SFML()
SSG_GITPKG_NLOHMANJSON() # Json parsing library
SSG_GITPKG_GOOGLE_TEST()

# SSG Internal Modules
add_subdirectory(modules/mta)
add_subdirectory(modules/cxxx)
add_subdirectory(modules/minitest)
add_subdirectory(modules/caoco)
add_subdirectory(modules/caf)
add_subdirectory(modules/cgui)
add_subdirectory(modules/wpl)

#------------------------------------------------------------------------------
# SSG_ROOT Internal Project Options
#------------------------------------------------------------------------------

# Setup project options for internal executables and libraries.
SSG_ROOT_SETUP_PROJECT_OPTIONS()

#------------------------------------------------------------------------------
# SSG_ROOT Handle and apply internal project options.
#------------------------------------------------------------------------------
SSG_ROOT_HANDLE_PROJECT_OPTIONS()
ssg_generate_cxx_strict_warning_flags()

include(bsys/cmake/UseCandLang.cmake)

#cand_configure()

# cnd compiler target config
add_executable(cnd_compiler
  modules/cnd/src/cnd_main.cpp
  modules/cnd/src/cli_main.cpp
  modules/cnd/src/driver.cpp
  #modules/cnd/src/cnd_cst.cpp
  #modules/cnd/src/cnd_lexer.cpp
  #modules/cnd/src/cnd_scanner.cpp
  #modules/cnd/src/cnd_parser.cpp
  #modules/cnd/src/cnd_tk_closure.cpp
  #modules/cnd/src/cnd_tk_cursor.cpp
)

# Add static unit tests.
target_sources(cnd_compiler PRIVATE
  modules/cnd/ut/sut_LexerBasics.cpp
  modules/cnd/ut/sut_LexerLexEscapedCharSequence.cpp
  modules/cnd/ut/sut_LexerLexIdentifier.cpp
  modules/cnd/ut/sut_LexerLexNumber.cpp
  modules/cnd/ut/sut_LexerLexPunctuator.cpp
)

target_include_directories(cnd_compiler PUBLIC 
    #modules/cnd/imp
    modules/cnd/inc
    #modules/cnd/inc/ccapi
    #modules/cnd/inc/cldata
    #modules/cnd/inc/cldev
    #modules/cnd/inc/corevals
    #modules/cnd/inc/trtools
)

target_link_libraries(cnd_compiler
  PRIVATE
    nlohmann_json::nlohmann_json
    # SSG Modules
    ssgmod::mta::mta_interface
    ssgmod::cxxx::cxxx_interface
    ssgmod::wpl::wpl_interface
)

# Add C& source files to the C++ impl of the C& compiler recursiveley.
#cand_cxx_target_sources(TARGET cnd_compiler SOURCES csl/test.cnd)
#cand_precompile(TARGET cnd_compiler)
#------------------------------------------------------------------------------
# CIDR_EXE Target Config
#------------------------------------------------------------------------------


add_executable(CIDR_EXE 
    modules/cide/src/cide_main.cpp
    modules/cide/src/cide_backend.cpp
    modules/cide/src/cide_ui_text_editor.cpp
)

# Link the required libraries
target_link_libraries(CIDR_EXE
  PRIVATE
	  ImGui-SFML::ImGui-SFML
    nlohmann_json::nlohmann_json
    # SSG Modules
    ssgmod::mta::mta_interface
    ssgmod::cxxx::cxxx_interface
    ssgmod::minitest::minitest_interface
    ssgmod::caoco::caoco_interface
    ssgmod::caf::caf_interface
    ssgmod::cgui::cgui_interface
    ssgmod::wpl::wpl_interface
)


target_include_directories(CIDR_EXE PUBLIC 
    modules/cxxx/ut
    modules/cide/inc
    modules/caoco/ut
)

# Apply standard SSG warnings to the target
target_compile_options(
CIDR_EXE
PRIVATE ${SSG_PROJECT_WARNINGS_CXX})

# Copy DLLs needed for runtime on Windows for imgui-sfml
if(WIN32)
  if (BUILD_SHARED_LIBS)
    add_custom_command(TARGET CIDR_EXE POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
          $<TARGET_FILE:ImGui-SFML::ImGui-SFML>
          $<TARGET_FILE:sfml-graphics>
          $<TARGET_FILE:sfml-window>
          $<TARGET_FILE:sfml-system>
          $<TARGET_FILE_DIR:CIDR_EXE>)
  endif()
endif()

#------------------------------------------------------------------------------
# CIDR_EXE Install Config
#------------------------------------------------------------------------------
include(GNUInstallDirs)

install(TARGETS CIDR_EXE
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# The installed executable should be set as the debug target,
# when using shared libs. Otherswise, missing .dll errors will occur.
if(BUILD_SHARED_LIBS)
  set_target_properties(CIDR_EXE PROPERTIES
    INSTALL_RPATH $ORIGIN/../${CMAKE_INSTALL_LIBDIR}
  )

  set_target_properties(
    ImGui-SFML
    sfml-graphics sfml-system sfml-window
    PROPERTIES
    INSTALL_RPATH $ORIGIN
  )

  install(TARGETS
    ImGui-SFML
    sfml-graphics sfml-system sfml-window
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    NAMELINK_SKIP # don't need versionless .so's
  )
endif()


#------------------------------------------------------------------------------
# Unit Testing
#------------------------------------------------------------------------------
enable_testing()


add_executable(
  hello_test
  bsys/hello_test.cc
)

target_link_libraries(
  hello_test
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(hello_test)

# ut : cxxx module
add_executable(cxxx_minitest modules/cxxx/ut/utmain.cpp)
target_link_libraries(cxxx_minitest
  ssgmod::minitest::minitest_interface
  ssgmod::mta::mta_interface
  ssgmod::cxxx::cxxx_interface
)
target_include_directories(cxxx_minitest INTERFACE modules/cxxx/ut/)
add_test(
  NAME ut_cxxx
  COMMAND $<TARGET_FILE:cxxx_minitest>)

# ut : wpl module
add_executable(wpl_minitest modules/wpl/ut/utmain.cpp)
target_link_libraries(wpl_minitest
  ssgmod::minitest::minitest_interface
  ssgmod::wpl::wpl_interface
)
target_include_directories(wpl_minitest INTERFACE modules/wpl/ut/)
add_test(
  NAME ut_wpl
  COMMAND $<TARGET_FILE:wpl_minitest>)

# Unit Tests : C& Compiler Module
   # ssgmod::minitest::minitest_interface
   # ssgmod::mta::mta_interface
   # ssgmod::cxxx::cxxx_interface
   # ssgmod::caoco::caoco_interface

set(MINITEST_LIBRARY ssgmod::minitest::minitest_interface)

# @brief Creates an executable target with a generated main source file given a header which includes unit tests.
# @param NAME Created target name.
# @param TEST_HEADER The header containing unit tests.
# @param SOURCES Sources to add to the executable. Do not include a main method.
# @param INCLUDE_DIRECTORIES Target include directories. The include dir containing the test header must be included.
# @param LINK_LIBS Libraries to link to the executable.
macro(add_minitest_executable)
  set(options)
  set(oneValueArgs NAME TEST_HEADER)
  set(multiValueArgs SOURCES INCLUDE_DIRECTORIES LINK_LIBS)
  cmake_parse_arguments(arg_add_minitest_executable
      "${options}" "${oneValueArgs}" "${multiValueArgs}"
      ${ARGN}
  )
  # arg_add_minitest_executable_NAME
  # arg_add_minitest_executable_SOURCES
  # arg_add_minitest_executable_LINK_LIBS
  # arg_add_minitest_executable_INCLUDE_DIRECTORIES
  # arg_add_minitest_executable_TEST_HEADER
  
  # Generate a main source file for this unit test using the name of the target.
  file(CONFIGURE 
    OUTPUT minitest/${arg_add_minitest_executable_NAME}.cpp 
    CONTENT "\
  #include \"minitest.hpp\"\n\
  #include \"@arg_add_minitest_executable_TEST_HEADER@\"\n\
  int main(int argc, char* argv[]) { return minitest::CmakeMain(argc, argv); }\
  "   @ONLY)


  add_executable(
    ${arg_add_minitest_executable_NAME}
    ${CMAKE_CURRENT_BINARY_DIR}/minitest/${arg_add_minitest_executable_NAME}.cpp ${arg_add_minitest_executable_SOURCES}
  )

  target_link_libraries(
    ${arg_add_minitest_executable_NAME} 
    ${MINITEST_LIBRARY}
    ${arg_add_minitest_executable_LINK_LIBS}
  )

  target_include_directories(
    ${arg_add_minitest_executable_NAME} 
    PUBLIC ${arg_add_minitest_executable_INCLUDE_DIRECTORIES}
  )
endmacro()


function(minitest_find_header_tests)
  # Read the content of the file into a variable
  file(READ "${ARGV0}" file_content)

  # Define the regex patterns for opening and closing
  # TEST\\(([A-Za-z0-9]+),([A-Za-z0-9]+)\\)
  # "INLINE_MINITEST\\(([^,]+),([^,]+)\\) \{"
  set(opening_regex "INLINE_MINITEST\\(([^,]+),([^)]+)\\)")
  set(closing_regex "INLINE_END_MINITEST_AND_REGISTER\\(([^,]+),([^,]+)\\)")

  # Initialize an empty list to store the pairs
  set(test_case_pairs "")

  # Find all opening TEST_CASE patterns
  string(REGEX MATCHALL "${opening_regex}" opening_matches "${file_content}")

  # Iterate through each opening match
  foreach(opening_match IN LISTS opening_matches)
      # Extract Name and CaseName from the opening match
      string(REGEX REPLACE "${opening_regex}" "\\1" name "${opening_match}")
      string(REGEX REPLACE "${opening_regex}" "\\2" case_name "${opening_match}")

      # Trim any leading/trailing whitespace
      string(STRIP "${name}" name)
      string(STRIP "${case_name}" case_name)


          list(APPEND test_case_pairs "${name}")
                    list(APPEND test_case_pairs "${case_name}")

  endforeach()

  # Output the list of test case pairs
  set(${ARGV1} ${test_case_pairs} PARENT_SCOPE)
endfunction()



macro(add_minitest)
  set(options)
  set(oneValueArgs TARGET MODULE_NAME CASE_NAME WORKING_DIRECTORY)
  set(multiValueArgs)
  cmake_parse_arguments(arg_add_minitest
      "${options}" "${oneValueArgs}" "${multiValueArgs}"
      ${ARGN}
  )
  add_test(
    NAME ${arg_add_minitest_TARGET}${arg_add_minitest_MODULE_NAME}${arg_add_minitest_CASE_NAME}
    COMMAND $<TARGET_FILE:${arg_add_minitest_TARGET}> 
            ${arg_add_minitest_MODULE_NAME} 
            ${arg_add_minitest_CASE_NAME}
    WORKING_DIRECTORY ${arg_add_minitest_WORKING_DIRECTORY}
  )
endmacro()

macro(add_minitest_module)
  set(options)
  set(oneValueArgs TARGET MODULE_NAME WORKING_DIRECTORY)
  set(multiValueArgs CASE_NAMES)
  cmake_parse_arguments(arg_add_minitest_module
      "${options}" "${oneValueArgs}" "${multiValueArgs}"
      ${ARGN}
  )
  foreach(arg_add_minitest_module_CASE_NAME IN ITEMS ${arg_add_minitest_module_CASE_NAMES})
    add_test(
      NAME ${arg_add_minitest_module_TARGET}${arg_add_minitest_module_MODULE_NAME}${arg_add_minitest_module_CASE_NAME}
      COMMAND $<TARGET_FILE:${arg_add_minitest_module_TARGET}> 
              ${arg_add_minitest_module_MODULE_NAME} 
              ${arg_add_minitest_module_CASE_NAME}
      WORKING_DIRECTORY ${arg_add_minitest_module_WORKING_DIRECTORY}
    )
  endforeach()
endmacro()

macro(add_minitest_header)
  set(options)
  set(oneValueArgs TARGET WORKING_DIRECTORY HEADER PREFIX)
  set(multiValueArgs)
  cmake_parse_arguments(arg_add_minitest_header
      "${options}" "${oneValueArgs}" "${multiValueArgs}"
      ${ARGN}
  )

  set(test_case_pairs "")
  minitest_find_header_tests("${arg_add_minitest_header_HEADER}" test_case_pairs)

    # Loop through the list in steps of 2 (Name and CaseName)
  list(LENGTH test_case_pairs num_elements)
  math(EXPR num_pairs "${num_elements} / 2")  # Calculate the number of pairs

  foreach(i RANGE 0 ${num_pairs} 2)
      # Get Name and CaseName from the list
      list(GET test_case_pairs ${i} name)
      math(EXPR next_index "${i} + 1")
      list(GET test_case_pairs ${next_index} case_name)

      # Create a unique test name
      set(test_name "${name}:${case_name}")

      # Use add_test to create the test
        add_test(
    NAME ${arg_add_minitest_header_PREFIX}${test_name}
    COMMAND $<TARGET_FILE:${arg_add_minitest_header_TARGET}> 
            ${name} 
            ${case_name}
    WORKING_DIRECTORY ${arg_add_minitest_header_WORKING_DIRECTORY}
  )

      # Optionally, print the test name for debugging
      message(STATUS "Added test: ${test_name}")
endforeach()
endmacro()

#--------------------------------------------------------------
# Unit Tests : C& Compiler Module
#--------------------------------------------------------------
set(SSG_UT_CND_INCLUDE_DIRS modules/caoco/ut/)
set(SSG_UT_CND_WORKING_DIR ${CMAKE_CURRENT_SOURCE_DIR}/modules/caoco/ut/res/)
set(SSG_UT_CND_LIBS_TO_LINK 
  ssgmod::minitest::minitest_interface
  ssgmod::mta::mta_interface
  ssgmod::cxxx::cxxx_interface 
  ssgmod::caoco::caoco_interface
)

# Executables
if(0)
  add_minitest_executable(
    NAME ut_cnd_Lexer
    SOURCES ${SSG_UT_CND_INCLUDE_DIRS}ut_cnd_Lexer.cpp
    INCLUDE_DIRECTORIES ${SSG_UT_CND_INCLUDE_DIRS}
    LINK_LIBS ${SSG_UT_CND_LIBS_TO_LINK}
  )

  add_minitest_executable(
    NAME ut_cnd_TokenScope
    SOURCES ${SSG_UT_CND_INCLUDE_DIRS}ut_cnd_TokenScope.cpp
    INCLUDE_DIRECTORIES ${SSG_UT_CND_INCLUDE_DIRS}
    LINK_LIBS ${SSG_UT_CND_LIBS_TO_LINK}
  )

  add_minitest_executable(
    NAME ut_cnd_ParserBasics
    SOURCES ${SSG_UT_CND_INCLUDE_DIRS}ut_cnd_ParserBasics.cpp
    INCLUDE_DIRECTORIES ${SSG_UT_CND_INCLUDE_DIRS}
    LINK_LIBS ${SSG_UT_CND_LIBS_TO_LINK}
  )

  add_minitest_executable(
    NAME ut_cnd_Build
    SOURCES ${SSG_UT_CND_INCLUDE_DIRS}ut_cnd_Build.cpp
    INCLUDE_DIRECTORIES ${SSG_UT_CND_INCLUDE_DIRS}
    LINK_LIBS ${SSG_UT_CND_LIBS_TO_LINK}
  )

  # Modules

  add_minitest_module(
    TARGET ut_cnd_Lexer
    MODULE_NAME Test_Lexer
    WORKING_DIRECTORY ${SSG_UT_CND_WORKING_DIR}
    CASE_NAMES TestCase_Keywords
  )

  add_minitest_module(
    TARGET ut_cnd_TokenScope
    MODULE_NAME Test_TokenScope
    WORKING_DIRECTORY ${SSG_UT_CND_WORKING_DIR}
    CASE_NAMES 
    TestCase_ParenScopeFinder
    TestCase_ListScopeFinder
    TestCase_FrameScopeFinder
    TestCase_StatementScopeFinder
  )

  add_minitest_module(
    TARGET ut_cnd_ParserBasics
    MODULE_NAME Test_ParserBasics
    WORKING_DIRECTORY ${SSG_UT_CND_WORKING_DIR}
    CASE_NAMES 
      TestCase_SingleOperand
      TestCase_ValueExpr
      TestCase_PrimaryStatement
      TestCase_VariableDeclarationNoTypeNoAssignNoMod
      TestCase_VariableDeclarationNoTypeNoAssign
      TestCase_VariableDeclarationNoAssign
      TestCase_VariableDefinition
      TestCase_TypeAlias
      TestCase_LibraryNamespaceInclusion
      TestCase_NamespaceInclusion
      TestCase_ObjectInclusion
      TestCase_TypeInclusion
      TestCase_ObjectInclusionFromLibrary
      TestCase_TypeInclusionFromLibrary
      TestCase_TypeImportDeclaration
      TestCase_MethodDeclImplicitVoidArgNoRet
      TestCase_MethodDeclImplicitVoidArgNoRet2
      TestCase_MethodDeclImplicitVoidArgAnyRet
      TestCase_MethodDeclImplicitVoidArgAnyRet2
      TestCase_MethodDeclArgNoRet
      TestCase_MethodDeclArgAnyRet
      TestCase_MethodDeclArgIdentifiedAnyRet
      TestCase_MethodDeclArgsTypedRet
      TestCase_MethodDeclTypedArgsTypedRet
      TestCase_MethodDeclTypedArgsTypedRetWithModifiers
      TestCase_ClassDecl
      TestCase_ClassDeclWithMod
      TestCase_LibWithMod
      TestCase_LibWithModAndDefinition
      TestCase_ClassWithModAndDefinition
      TestCase_MethodDefinition
      TestCase_MainDefinition
      TestCase_PragmaticDeclarations
      TestCase_ParseProgramWithDeclrations
      TestCase_ReturnStatement
      TestCase_IfStatement
      TestCase_IfElseStatement
      TestCase_IfElifStatement
      TestCase_IfElifElseStatement
      TestCase_WhileStatement
      TestCase_ForStatement
      TestCase_AnimalsExampleProgram
      TestCase_BasicEnumDefinition
      TestCase_EnumDefinitionWithValues
  )

  add_minitest_module(
    TARGET ut_cnd_Build
    MODULE_NAME Test_Build
    WORKING_DIRECTORY ${SSG_UT_CND_WORKING_DIR}
    CASE_NAMES TestCase_BasicBuild
  )
endif()

#--------------------------------------------------------------
# @end Unit Tests : C& Compiler Module
#--------------------------------------------------------------
set(SSG_UT_CCC_INCLUDE_DIRS modules/cnd/inc;modules/cnd/ut/)
set(SSG_UT_CCC_WORKING_DIR ${CMAKE_CURRENT_SOURCE_DIR}/modules/cnd/ut/res/)
set(SSG_UT_CCC_LIBS_TO_LINK 
  ssgmod::mta::mta_interface
  ssgmod::cxxx::cxxx_interface 
)

add_minitest_executable(
  NAME test_trtools_parser
  TEST_HEADER test_trtools_parser.hpp
  INCLUDE_DIRECTORIES ${SSG_UT_CCC_INCLUDE_DIRS}
  LINK_LIBS ${SSG_UT_CCC_LIBS_TO_LINK}
)
