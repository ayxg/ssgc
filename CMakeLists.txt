cmake_minimum_required(VERSION 3.29.5)
include(FetchContent)
#------------------------------------------------------------------------------
# SSG_ROOT Project-Specific Utilities  
#------------------------------------------------------------------------------

# Prevent in-source builds
function(SSG_ASSERT_OUT_OF_SOURCE_BUILDS)
  # make sure the user doesn't play dirty with symlinks
  get_filename_component(SRCDIR "${CMAKE_SOURCE_DIR}" REALPATH)
  get_filename_component(BINDIR "${CMAKE_BINARY_DIR}" REALPATH)

  # disallow in-source builds
  if("${SRCDIR}" STREQUAL "${BINDIR}")
    message(FATAL_ERROR 
        "[SSG][FATAL] In-source builds are disabled.\n")
  endif()
endfunction()

# Check if the project is top-level
macro(SSG_ASSERT_TOP_LEVEL_PROJECT)
  if(NOT PROJECT_IS_TOP_LEVEL)
	  message(FATAL_ERROR 
          "[SSG][FATAL] Project is not top-level CMakeLists file.")
  endif()
endmacro()

# Install sfml-imgui package for this project.
macro(SSG_GITPKG_IMGUI_SFML)
  # SFML
  # Note: SFML builds as shared by default.
  add_compile_definitions(_HAS_AUTO_PTR_ETC=1) # Allows SFML deprecated std::auto_ptr in C++17
  FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML
    GIT_TAG 69ea0cd863aed1d4092b970b676924a716ff718b # 2.6.1
  )
  FetchContent_MakeAvailable(SFML)

  # ImGui
  FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui
    GIT_TAG 8199457a7d9e453f8d3d9cadc14683fb54a858b5 # 1.91
  )
  FetchContent_MakeAvailable(imgui)

  # ImGui-SFML
  FetchContent_Declare(
    imgui-sfml
    GIT_REPOSITORY https://github.com/SFML/imgui-sfml
    GIT_TAG de565ac8f2b795dedc0307b60830cb006afd2ecd # 2.6
  )

  set(IMGUI_DIR ${imgui_SOURCE_DIR}) # pass our imgui source dir to imgui-sfml (mandatory)
  set(IMGUI_SFML_FIND_SFML OFF) # turns off FindSFML impl inside imGui-SFML CMakeLists
  set(IMGUI_SFML_IMGUI_DEMO ON) # enables imgui-sfml demo

  FetchContent_MakeAvailable(imgui-sfml)
endmacro()

macro(SSG_GITPKG_NLOHMANJSON)
  FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
  FetchContent_MakeAvailable(json) # nlohmann_json::nlohmann_json
endmacro()

macro(SSG_GITPKG_GOOGLE_TEST)
# gitbash $ git ls-remote --tags https://github.com/google/googletest.git
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest
  GIT_TAG b514bdc898e2951020cbdca1304b75f5950d1f59 # refs/tags/v1.15.2
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
endmacro()

# check undefined behavior/address sanitizer support
# OUT: SUPPORTS_UBSAN, SUPPORTS_ASAN
macro(SSG_CHECK_SANITIZER_SUPPORT)
  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") 
      AND NOT WIN32)
      set(SUPPORTS_UBSAN ON)
  else()
      set(SUPPORTS_UBSAN OFF)
  endif()

  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") 
      AND WIN32)
      set(SUPPORTS_ASAN OFF)
  else()
      set(SUPPORTS_ASAN ON)
  endif()
endmacro()

# Configure standard C++ compiler warnings.
# OUT: PROJECT_WARNINGS_CXX, PROJECT_WARNINGS_C
macro(SSG_ROOT_CXX_APPLY_STD_COMPILER_FLAGS)
  set(MSVC_WARNINGS
      /W4 # Baseline reasonable warnings
      /w14242 # 'identifier': conversion from 'type1' to 'type2', possible loss of data
      /w14254 # 'operator': conversion from 'type1:field_bits' to 'type2:field_bits', possible loss of data
      /w14263 # 'function': member function does not override any base class virtual member function
      /w14265 # 'classname': class has virtual functions, but destructor is not virtual instances of this class may not
              # be destructed correctly
      /w14287 # 'operator': unsigned/negative constant mismatch
      /we4289 # nonstandard extension used: 'variable': loop control variable declared in the for-loop is used outside
              # the for-loop scope
      /w14296 # 'operator': expression is always 'boolean_value'
      /w14311 # 'variable': pointer truncation from 'type1' to 'type2'
      /w14545 # expression before comma evaluates to a function which is missing an argument list
      /w14546 # function call before comma missing argument list
      /w14547 # 'operator': operator before comma has no effect; expected operator with side-effect
      /w14549 # 'operator': operator before comma has no effect; did you intend 'operator'?
      /w14555 # expression has no effect; expected expression with side- effect
      /w14619 # pragma warning: there is no warning number 'number'
      /w14640 # Enable warning on thread un-safe static member initialization
      /w14826 # Conversion from 'type1' to 'type2' is sign-extended. This may cause unexpected runtime behavior.
      /w14905 # wide string literal cast to 'LPSTR'
      /w14906 # string literal cast to 'LPWSTR'
      /w14928 # illegal copy-initialization; more than one user-defined conversion has been implicitly applied
      /permissive- # standards conformance mode for MSVC compiler.
  )

  set(CLANG_WARNINGS
      -Wall
      -Wextra # reasonable and standard
      -Wshadow # warn the user if a variable declaration shadows one from a parent context
      -Wnon-virtual-dtor # warn the user if a class with virtual functions has a non-virtual destructor. This helps
      # catch hard to track down memory errors
      -Wold-style-cast # warn for c-style casts
      -Wcast-align # warn for potential performance problem casts
      -Wunused # warn on anything being unused
      -Woverloaded-virtual # warn if you overload (not override) a virtual function
      -Wpedantic # warn if non-standard C++ is used
      -Wconversion # warn on type conversions that may lose data
      -Wsign-conversion # warn on sign conversions
      -Wnull-dereference # warn if a null dereference is detected
      -Wdouble-promotion # warn if float is implicit promoted to double
      -Wformat=2 # warn on security issues around functions that format output (ie printf)
      -Wimplicit-fallthrough # warn on statements that fallthrough without an explicit annotation
  )

  set(GCC_WARNINGS
      ${CLANG_WARNINGS}
      -Wmisleading-indentation # warn if indentation implies blocks where blocks do not exist
      -Wduplicated-cond # warn if if / else chain has duplicated conditions
      -Wduplicated-branches # warn if if / else branches have duplicated code
      -Wlogical-op # warn about logical operations being used where bitwise were probably wanted
      -Wuseless-cast # warn if you perform a cast to the same type
      -Wsuggest-override # warn if an overridden member function is not marked 'override' or 'final'
  )

  list(APPEND CLANG_WARNINGS -Werror)
  list(APPEND GCC_WARNINGS -Werror)
  # list(APPEND MSVC_WARNINGS /WX)

  if(MSVC)
  set(PROJECT_WARNINGS_CXX ${MSVC_WARNINGS})
  message("[SSG] MSVC default compiler warnings set.")
  elseif(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
  set(PROJECT_WARNINGS_CXX ${CLANG_WARNINGS})
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set(PROJECT_WARNINGS_CXX ${GCC_WARNINGS})
  else()
  message(AUTHOR_WARNING "No compiler warnings set for CXX compiler: '${CMAKE_CXX_COMPILER_ID}'")
  # TODO support Intel compiler
  endif()
  # use the same warning flags for C
  set(PROJECT_WARNINGS_C "${PROJECT_WARNINGS_CXX}")
endmacro()

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
  SSG_CHECK_SANITIZER_SUPPORT()

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
      option(SSG_ROOT_CXX_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" ${SUPPORTS_ASAN})
      option(SSG_ROOT_CXX_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
      option(SSG_ROOT_CXX_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" ${SUPPORTS_UBSAN})
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

  SSG_ROOT_CXX_check_libfuzzer_support(LIBFUZZER_SUPPORTED)
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
  SSG_CHECK_SANITIZER_SUPPORT()

  if(SSG_ROOT_CXX_ENABLE_HARDENING AND SSG_ROOT_CXX_ENABLE_GLOBAL_HARDENING)
    include(bsys/cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN 
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
SSG_ASSERT_OUT_OF_SOURCE_BUILDS()

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
SSG_ROOT_CXX_APPLY_STD_COMPILER_FLAGS()
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
PRIVATE ${PROJECT_WARNINGS_CXX})

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
# @brief Creates a unit test executable linked to the required libraries
#        of the C& Compiler module.
# @param ARGV0 [PROJECT_NAME]_MINITEST_TARGET_NAME
# @param ARGV1 [PROJECT_NAME]_MINITEST_TARGET_SOURCE
# @out   arg_add_cnd_minitest_executable_NAME
# @out   arg_add_cnd_minitest_executable_INCLUDE_DIRECTORIES
# @out   arg_add_cnd_minitest_executable_SOURCES
# @out   arg_add_cnd_minitest_executable_LINK_LIBS
macro(add_minitest_executable)
  set(options)
  set(oneValueArgs NAME)
  set(multiValueArgs SOURCES INCLUDE_DIRECTORIES LINK_LIBS)
  cmake_parse_arguments(arg_add_minitest_executable
      "${options}" "${oneValueArgs}" "${multiValueArgs}"
      ${ARGN}
  )
  add_executable(
    ${arg_add_minitest_executable_NAME}
    ${arg_add_minitest_executable_SOURCES}
  )

  target_link_libraries(
    ${arg_add_minitest_executable_NAME} 
    ${arg_add_minitest_executable_LINK_LIBS}
  )

  target_include_directories(
    ${arg_add_minitest_executable_NAME} 
    INTERFACE ${arg_add_minitest_executable_INCLUDE_DIRECTORIES}
  )
endmacro()

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

# Minitest CMake Support
#add_minitest(
#  TARGET ut_cnd_Lexer
#  MODULE_NAME Test_Lexer
# CASE_NAME TestCase_Keywords
# WORKING_DIRECTORY ${SSG_UT_CND_WORKING_DIR}
#)

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


#--------------------------------------------------------------
# @end Unit Tests : C& Compiler Module
#--------------------------------------------------------------
