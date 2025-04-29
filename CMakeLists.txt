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
add_subdirectory(modules/minitest)
add_subdirectory(modules/minitest2)
add_subdirectory(modules/mta)
add_subdirectory(modules/cxxx)
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
    modules/cnd/inc
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


# Test Group : CndModuleTests
minitest_add_executable(
  NAME                CndModuleTests
  INCLUDE_DIRECTORIES modules/cnd/inc 
                      modules/cnd/ut
  WORKING_DIRECTORY   modules/cnd/ut/res
  LINK_LIBS           ssgmod::mta::mta_interface
                      ssgmod::cxxx::cxxx_interface
  TEST_HEADERS        test_trtools_parser.hpp
)

minitest_from_headers(
  PREFIX UtCnd
  TARGET CndModuleTests
  TEST_HEADERS test_trtools_parser.hpp
)
