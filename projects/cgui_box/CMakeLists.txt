#[============================================================================[
Copyright 2025 Anton Yashchenko
Licensed under the Apache License, Version 2.0(the "License");
===============================================================================
@project: [CGUI] ImGui Object-Oriented Interface Library
@author(s): Anton Yashchenko
@website: https://www.acpp.dev
===============================================================================
@file 
@ingroup
@brief 
#]============================================================================]
cmake_minimum_required (VERSION 3.8)

project(cgui_box
	VERSION 0.0.0.0
	DESCRIPTION "ImGui Object-Oriented Interface Library"
	HOMEPAGE_URL "www.acpp.dev"
	LANGUAGES C CXX	
)

#[============================================================================[
  Find Dependencies
#]============================================================================]
find_package(ssgmono_box REQUIRED CONFIG)
find_package(imgui-sfml REQUIRED)

#[============================================================================[
  Project Init
#]============================================================================]
include(SsgProjectBase)
ssg_setup_standard_project_vars()


#[============================================================================[
  Source Targets
#]============================================================================]
set(CGUI_LIBRARY_HEADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/inc)
set(CGUI_LIBRARY_SOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_library(cgui_library 
  "${CGUI_LIBRARY_SOURCES_DIR}/cgui.cpp"
)
target_include_directories(cgui_library PUBLIC "${CGUI_LIBRARY_HEADERS_DIR}")
target_sources(cgui_library 
  INTERFACE 
  "${CGUI_LIBRARY_HEADERS_DIR}/cgui.hpp"
)
target_link_libraries(cgui_library PUBLIC ImGui-SFML::ImGui-SFML)

set_target_properties(cgui_library 
  PROPERTIES  CMAKE_CXX_STANDARD 20
              CMAKE_CXX_STANDARD_REQUIRED ON
)

#[============================================================================[
  Subproject Exports
#]============================================================================]
if (NOT PROJECT_IS_TOP_LEVEL)
  get_directory_property(CGUI_BOX_EXPORT_DIR SSG_PROJECT_EXPORT_DIR)
  get_directory_property(CGUI_BOX_IMPORT_DIR SSG_PROJECT_IMPORT_DIR)

  ssg_configure_alias_targets_file(
    TARGETS cgui_library
    PREFIX ${PROJECT_NAME}
    OUTPUT_FILE_PATH "${CGUI_BOX_IMPORT_DIR}/${PROJECT_NAME}-subproject-targets.cmake"
    INPUT_FILE_PATH "${CGUI_BOX_EXPORT_DIR}/subproject-targets.cmake.in"
  )

  ssg_export_subproject_config(${PROJECT_NAME} 
    CONFIG_SUBPROJECT "${CGUI_BOX_EXPORT_DIR}/subproject-config.cmake.in"
    OUTPUT_DIR "${CGUI_BOX_IMPORT_DIR}"
  ) 
endif()


#[============================================================================[
Copyright 2025 Anton Yashchenko
Licensed under the Apache License, Version 2.0(the "License");
===============================================================================
@project: [CGUI] ImGui Object-Oriented Interface Library
@author(s): Anton Yashchenko
@website: https://www.acpp.dev
#]============================================================================]
