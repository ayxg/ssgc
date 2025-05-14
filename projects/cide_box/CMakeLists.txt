#[============================================================================[
Copyright 2025 Anton Yashchenko
Licensed under the GNU Affero General Public License, Version 3.
===============================================================================
@project: [CIDE] C& Integrated Developer Environment
@author(s): Anton Yashchenko
@website: https://www.acpp.dev
===============================================================================
@file 
@ingroup
@brief 
#]============================================================================]
cmake_minimum_required (VERSION 3.8)

project(cide_box
	VERSION 0.0.0.0
	DESCRIPTION "[CIDE] C& Integrated Developer Environment"
	HOMEPAGE_URL "www.acpp.dev"
	LANGUAGES C CXX	
)

#[============================================================================[
  Find Dependencies
#]============================================================================]
find_package(ssgmono_box REQUIRED CONFIG)

find_package(imgui-sfml REQUIRED)
find_package(nlohmann_json REQUIRED)

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
set(CIDE_EXECUTABLE_HEADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/inc)
set(CIDE_EXECUTABLE_SOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_library(cide_executable 
  "${CIDE_LIBRARY_SOURCES_DIR}/cide_main.cpp"
  "${CIDE_LIBRARY_SOURCES_DIR}/cide_backend.cpp"
  "${CIDE_LIBRARY_SOURCES_DIR}/cide_ui_text_editor.cpp"
)
target_include_directories(cide_executable PUBLIC "${CIDE_LIBRARY_HEADERS_DIR}")
target_sources(cide_executable 
  INTERFACE 
  "${CIDE_LIBRARY_HEADERS_DIR}/cide_backend.h"
  "${CIDE_LIBRARY_HEADERS_DIR}/cide_common.h"
  "${CIDE_LIBRARY_HEADERS_DIR}/cide_ui.h"
  "${CIDE_LIBRARY_HEADERS_DIR}/cide_ui_ast_explorer.h"
  "${CIDE_LIBRARY_HEADERS_DIR}/cide_ui_cpp_test_explorer.h"
  "${CIDE_LIBRARY_HEADERS_DIR}/cide_ui_launcher.h"
  "${CIDE_LIBRARY_HEADERS_DIR}/cide_ui_text_editor.h"
)
target_link_libraries(cide_executable   
  PRIVATE
	  ImGui-SFML::ImGui-SFML
    nlohmann_json::nlohmann_json
    # SSG Modules
    mta_box::mta_library
    cxxx_box::cxx_library
    minitest_box::minitest_library
    caf_box::caf_library
    cgui_box::cgui_library
    wpl_box::wpl_library
    cnd_box::cnd_library
)

set_target_properties(cide_executable 
  PROPERTIES  CMAKE_CXX_STANDARD 20
              CMAKE_CXX_STANDARD_REQUIRED ON
)

#[============================================================================[
  Subproject Exports
#]============================================================================]
include(SsgProjectPackageUtils)
ssg_export_subproject(
	"${PROJECT_NAME}"
	TARGETS cide_executable
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
    add_custom_command(TARGET cide_executable POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
          $<TARGET_FILE:ImGui-SFML::ImGui-SFML>
          $<TARGET_FILE:sfml-graphics>
          $<TARGET_FILE:sfml-window>
          $<TARGET_FILE:sfml-system>
          $<TARGET_FILE_DIR:cide_executable>)
  endif()
endif()

# The installed executable should be set as the debug target,
# when using shared libs. Otherswise, missing .dll errors will occur.
include(GNUInstallDirs)
install(TARGETS cide_executable
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
if(BUILD_SHARED_LIBS)
  set_target_properties(cide_executable PROPERTIES
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
Licensed under the GNU Affero General Public License, Version 3.
===============================================================================
@project: [CIDE] C& Integrated Developer Environment
@author(s): Anton Yashchenko
@website: https://www.acpp.dev
#]============================================================================]
