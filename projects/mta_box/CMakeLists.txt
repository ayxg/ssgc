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
cmake_minimum_required (VERSION 3.8...3.10)

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
include(SsgProjectPackageUtils)
ssg_export_subproject(
	"${PROJECT_NAME}"
	TARGETS mta_library
	REDIRECT_FIND_PACKAGE
)

#[============================================================================[
Copyright 2025 Anton Yashchenko
Licensed under the GNU Affero General Public License, Version 3.
===============================================================================
@project: [MTA] Meta Template Archive
@author(s): Anton Yashchenko
@website: https://www.acpp.dev
#]============================================================================]