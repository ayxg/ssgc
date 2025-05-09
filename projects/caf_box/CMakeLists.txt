#[============================================================================[
Copyright 2025 Anton Yashchenko
Licensed under the GNU Affero General Public License, Version 3.
===============================================================================
@project: [CAF] Core Application Framework
@author(s): Anton Yashchenko
@website: https://www.acpp.dev
===============================================================================
@file 
@ingroup
@brief 
#]============================================================================]
cmake_minimum_required (VERSION 3.8)

project(caf_box
	VERSION 0.0.0.0
	DESCRIPTION "[CAF] Core Application Framework"
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
set(CAF_LIBRARY_HEADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/inc)
set(CAF_LIBRARY_SOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_library(caf_library 
  "${CAF_LIBRARY_SOURCES_DIR}/caf_cache_file.cpp"
  "${CAF_LIBRARY_SOURCES_DIR}/caf_context.cpp"
  "${CAF_LIBRARY_SOURCES_DIR}/caf_imgui_ext.cpp"
  "${CAF_LIBRARY_SOURCES_DIR}/caf_time.cpp"
)
target_include_directories(caf_library PUBLIC "${CAF_LIBRARY_HEADERS_DIR}")
target_sources(caf_library 
  INTERFACE 
  "${CAF_LIBRARY_HEADERS_DIR}/caf.hpp"
  "${CAF_LIBRARY_HEADERS_DIR}/caf_cache_file.hpp"
  "${CAF_LIBRARY_HEADERS_DIR}/caf_config.hpp"
  "${CAF_LIBRARY_HEADERS_DIR}/caf_context.hpp"
  "${CAF_LIBRARY_HEADERS_DIR}/caf_imgui_ext.hpp"
  "${CAF_LIBRARY_HEADERS_DIR}/caf_predef.hpp"
  "${CAF_LIBRARY_HEADERS_DIR}/caf_time.hpp"
)

set_target_properties(caf_library 
  PROPERTIES  CMAKE_CXX_STANDARD 20
              CMAKE_CXX_STANDARD_REQUIRED ON
)

#[============================================================================[
  Subproject Exports
#]============================================================================]
if (NOT PROJECT_IS_TOP_LEVEL)
  get_directory_property(CAF_BOX_EXPORT_DIR SSG_PROJECT_EXPORT_DIR)
  get_directory_property(CAF_BOX_IMPORT_DIR SSG_PROJECT_IMPORT_DIR)

  ssg_configure_alias_targets_file(
    TARGETS caf_library
    PREFIX ${PROJECT_NAME}
    OUTPUT_FILE_PATH "${CAF_BOX_IMPORT_DIR}/${PROJECT_NAME}-subproject-targets.cmake"
    INPUT_FILE_PATH "${CAF_BOX_EXPORT_DIR}/subproject-targets.cmake.in"
  )

  ssg_export_subproject_config(${PROJECT_NAME} 
    CONFIG_SUBPROJECT "${CAF_BOX_EXPORT_DIR}/subproject-config.cmake.in"
    OUTPUT_DIR "${CAF_BOX_IMPORT_DIR}"
  ) 
endif()


#[============================================================================[
Copyright 2025 Anton Yashchenko
Licensed under the GNU Affero General Public License, Version 3.
===============================================================================
@project: [CAF] Core Application Framework
@author(s): Anton Yashchenko
@website: https://www.acpp.dev
#]============================================================================]