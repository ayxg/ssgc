cmake_minimum_required(VERSION 3.29.5) # 3.29.5
include(FetchContent)

#------------------------------------------------------------------------------
# SSG_ROOT Project-Specific Utilities  
#------------------------------------------------------------------------------
include(bsys/ssg/ssg.cmake)
include(bsys/ssg/gitpkg.cmake)

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

# SSG Internal Modules
add_subdirectory(modules/mta)
add_subdirectory(modules/cxxx)
add_subdirectory(modules/minitest)
add_subdirectory(modules/minitest2)
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

# include(bsys/cmake/UseCandLang.cmake)

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


# Test Suite : CxxxModuleTests
minitest_add_executable(
  NAME                CxxxModuleTests
  INCLUDE_DIRECTORIES modules/cxxx/ut
  LINK_LIBS           ssgmod::mta::mta_interface 
                      ssgmod::cxxx::cxxx_interface
  TEST_HEADERS        ut_expected.h
)

minitest_from_headers(
  PREFIX UtCxxx
  TARGET CxxxModuleTests
  SCAN_ALL
)

# Test Suite : WplLibraryTests
minitest_add_executable(
  NAME                WplModuleTests
  INCLUDE_DIRECTORIES modules/wpl/ut
  LINK_LIBS           ssgmod::wpl::wpl_interface
  TEST_HEADERS        ut_wpl.hpp
)

minitest_from_headers(
  PREFIX UtWpl
  TARGET WplModuleTests
  SCAN_ALL
)

#[=[
  # Unit Tests : C& Compiler Module
     # ssgmod::minitest::minitest_interface
     # ssgmod::mta::mta_interface
     # ssgmod::cxxx::cxxx_interface
     # ssgmod::caoco::caoco_interface

  set(MINITEST_LIBRARY ssgmod::minitest::minitest_interface)

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
]=]