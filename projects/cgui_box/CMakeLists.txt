#[============================================================================[
Copyright 2025 Anton Yashchenko
Licensed under the GNU Affero General Public License, Version 3.
===============================================================================
@project: [CGUI] ImGui Object-Oriented Interface Library
@author(s): Anton Yashchenko
@website: https://www.acpp.dev
===============================================================================
@file 
@ingroup
@brief 
#]============================================================================]
cmake_minimum_required (VERSION 3.8...3.10)

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

find_package(imgui-sfml REQUIRED)
find_package(nlohmanjson REQUIRED)

find_package(mta_box REQUIRED CONFIG)
find_package(cxxx_box REQUIRED CONFIG)
find_package(caf_box REQUIRED CONFIG)
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

add_library(cgui_library STATIC
  "${CGUI_LIBRARY_SOURCES_DIR}/cgui.cpp"
)
target_include_directories(cgui_library PUBLIC "${CGUI_LIBRARY_HEADERS_DIR}")
target_sources(cgui_library 
  INTERFACE 
  "${CGUI_LIBRARY_HEADERS_DIR}/cgui.hpp"
)
target_link_libraries(cgui_library PUBLIC     
            mta_box::mta_library
            cxxx_box::cxxx_library
            ImGui-SFML::ImGui-SFML
            nlohmann_json::nlohmann_json
            caf_box::caf_library
)

set_target_properties(cgui_library 
  PROPERTIES  CMAKE_CXX_STANDARD 20
              CMAKE_CXX_STANDARD_REQUIRED ON
)

#[============================================================================[
  Subproject Exports
#]============================================================================]
include(SsgProjectPackageUtils)
ssg_export_subproject(
	"${PROJECT_NAME}"
	TARGETS cgui_library
	REDIRECT_FIND_PACKAGE
)

#[============================================================================[
Copyright 2025 Anton Yashchenko
Licensed under the GNU Affero General Public License, Version 3.
===============================================================================
@project: [CGUI] ImGui Object-Oriented Interface Library
@author(s): Anton Yashchenko
@website: https://www.acpp.dev
#]============================================================================]
