#[============================================================================[
Copyright 2025 Anton Yashchenko
Licensed under the Apache License, Version 2.0(the "License");
===============================================================================
@project: [CIDE] C& Integrated Developer Environment
@author(s): Anton Yashchenko
@website: https://www.acpp.dev
===============================================================================
@file 
@ingroup
@brief 
#]============================================================================]
cmake_minimum_required (VERSION 3.8...3.10)

project(cide_box
	VERSION 0.0.0.0
	DESCRIPTION "[CIDE] C& Integrated Developer Repository"
	HOMEPAGE_URL "www.acpp.dev"
	LANGUAGES C CXX	
)

#[============================================================================[
  Find Dependencies
#]============================================================================]
find_package(ssgmono_box REQUIRED CONFIG)

find_package(imgui-sfml REQUIRED)
find_package(nlohmanjson REQUIRED)

find_package(minitest_box REQUIRED CONFIG)
find_package(mta_box REQUIRED CONFIG)
find_package(cxxx_box REQUIRED CONFIG)
find_package(wpl_box REQUIRED CONFIG)
find_package(caf_box REQUIRED CONFIG)
find_package(cgui_box REQUIRED CONFIG)


#[============================================================================[
  Project Init
#]============================================================================]
include(SsgProjectBase)
ssg_setup_standard_project_vars()


#[============================================================================[
  Source Targets
#]============================================================================]

#[====================================[
  @target cidr_library
#]====================================]
set(cidr_library_BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cidr_library)
set(cidr_library_HEADERS_DIR ${cidr_library_BASE_DIR}/inc)
set(cidr_library_SOURCES_DIR ${cidr_library_BASE_DIR}/src)

add_library(cidr_library STATIC
  "${cidr_library_SOURCES_DIR}/Main.cpp"
  "${cidr_library_SOURCES_DIR}/Backend.cpp"
  "${cidr_library_SOURCES_DIR}/UserInterface/TextEditor.cpp"
)
target_include_directories(cidr_library PUBLIC "${cidr_library_HEADERS_DIR}")
target_sources(cidr_library 
  INTERFACE 
  "${cidr_library_HEADERS_DIR}/CIDR.hpp"
  "${cidr_library_HEADERS_DIR}/CIDR/Common.hpp"
  "${cidr_library_HEADERS_DIR}/CIDR/Backend.hpp"
  "${cidr_library_HEADERS_DIR}/CIDR/Main.hpp"
  "${cidr_library_HEADERS_DIR}/CIDR/UserInterface.hpp"
  "${cidr_library_HEADERS_DIR}/CIDR/UserInterface/AstExplorer.hpp"
  "${cidr_library_HEADERS_DIR}/CIDR/UserInterface/CppTestExplorer.hpp"
  "${cidr_library_HEADERS_DIR}/CIDR/UserInterface/Launcher.hpp"
  "${cidr_library_HEADERS_DIR}/CIDR/UserInterface/TextEditor.hpp"
)
target_link_libraries(cidr_library    
  PUBLIC
	  ImGui-SFML::ImGui-SFML
    nlohmann_json::nlohmann_json
    # SSG Modules
    mta_box::mta_library
    cxxx_box::cxxx_library
    minitest_box::minitest_library
    caf_box::caf_library
    cgui_box::cgui_library
    wpl_box::wpl_library
    cnd_box::cnd_compiler_interface
)

set_target_properties(cidr_library 
  PROPERTIES  CMAKE_CXX_STANDARD 23
              CMAKE_CXX_STANDARD_REQUIRED ON
)

unset(cidr_library_HEADERS_DIR)
unset(cidr_library_SOURCES_DIR)

#[====================================[
  @target cidr_executable
#]====================================]
set(cidr_executable_BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cidr_executable)
set(cidr_executable_HEADERS_DIR ${cidr_executable_BASE_DIR}/inc)
set(cidr_executable_SOURCES_DIR ${cidr_executable_BASE_DIR}/src)

add_executable(cidr_executable "${cidr_executable_SOURCES_DIR}/MainGui.cpp")
target_link_libraries(cidr_executable cidr_library)

unset(cidr_executable_BASE_DIR)
unset(cidr_executable_HEADERS_DIR)
unset(cidr_executable_SOURCES_DIR)


#[============================================================================[
  Subproject Exports
#]============================================================================]
include(SsgProjectPackageUtils)
ssg_export_subproject(
	"${PROJECT_NAME}"
	TARGETS cidr_executable
	REDIRECT_FIND_PACKAGE
)

#[============================================================================[
  Install
#]============================================================================]

# Handle install of sfml for windows when building shared libs.
#
#
# Copy DLLs needed for runtime on Windows for imgui-sfml.
if(WIN32)
  if (BUILD_SHARED_LIBS)
    add_custom_command(TARGET cidr_executable POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
          $<TARGET_FILE:ImGui-SFML::ImGui-SFML>
          $<TARGET_FILE:sfml-graphics>
          $<TARGET_FILE:sfml-window>
          $<TARGET_FILE:sfml-system>
          $<TARGET_FILE_DIR:cidr_executable>)
  endif()
endif()

# The installed executable should be set as the debug target,
# when using shared libs. Otherswise, missing .dll errors will occur.
include(GNUInstallDirs)
install(TARGETS cidr_executable
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
if(BUILD_SHARED_LIBS)
  set_target_properties(cidr_executable PROPERTIES
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


#[============================================================================[
Copyright 2025 Anton Yashchenko
Licensed under the Apache License, Version 2.0(the "License");
===============================================================================
@project: [CIDE] C& Integrated Developer Environment
@author(s): Anton Yashchenko
@website: https://www.acpp.dev
#]============================================================================]
