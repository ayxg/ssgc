#[============================================================================[
Copyright 2025 Anton Yashchenko
Licensed under the GNU Affero General Public License, Version 3.
===============================================================================
@project: [MTA] Meta Template Archive
@author(s): Anton Yashchenko
@website: https://www.acpp.dev
===============================================================================
@file 
@ingroup
@brief 
#]============================================================================]
cmake_minimum_required (VERSION 3.8)

project(mta_box
	VERSION 0.0.0.0
	DESCRIPTION "C++ Templates Meta-Programming Archive"
	HOMEPAGE_URL "www.acpp.dev"
	LANGUAGES C CXX	
)

find_package(ssgmono_box REQUIRED CONFIG)
include(SsgProjectBase)
ssg_setup_standard_project_vars()


#[============================================================================[
  Source Targets
#]============================================================================]
set(MTA_LIBRARY_HEADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/inc)
set(MTA_LIBRARY_SOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_library(mta_library INTERFACE)
target_include_directories(mta_library INTERFACE "${MTA_LIBRARY_HEADERS_DIR}")
target_sources(mta_library 
  INTERFACE "${MTA_LIBRARY_HEADERS_DIR}/mta.hpp"
            "${MTA_LIBRARY_HEADERS_DIR}/mta_require.hpp"
)
set_target_properties(mta_library 
  PROPERTIES  CMAKE_CXX_STANDARD 20
              CMAKE_CXX_STANDARD_REQUIRED ON
)

#[============================================================================[
  Subproject Exports
#]============================================================================]
if (NOT PROJECT_IS_TOP_LEVEL)
  get_directory_property(MTA_PKG_EXPORT_DIR SSG_PROJECT_EXPORT_DIR)
  get_directory_property(MTA_PKG_IMPORT_DIR SSG_PROJECT_IMPORT_DIR)

  ssg_configure_alias_targets_file(
    TARGETS mta_library
    PREFIX ${PROJECT_NAME}
    OUTPUT_FILE_PATH "${MTA_PKG_IMPORT_DIR}/${PROJECT_NAME}-subproject-targets.cmake"
    INPUT_FILE_PATH "${MTA_PKG_EXPORT_DIR}/subproject-targets.cmake.in"
  )

  ssg_export_subproject_config(${PROJECT_NAME} 
    CONFIG_SUBPROJECT "${MTA_PKG_EXPORT_DIR}/subproject-config.cmake.in"
    OUTPUT_DIR "${MTA_PKG_IMPORT_DIR}"
  ) 
endif()


#[============================================================================[
Copyright 2025 Anton Yashchenko
Licensed under the GNU Affero General Public License, Version 3.
===============================================================================
@project: [MTA] Meta Template Archive
@author(s): Anton Yashchenko
@website: https://www.acpp.dev
#]============================================================================]