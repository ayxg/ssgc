#[============================================================================[
Copyright 2025 Anton Yashchenko
Licensed under the Apache License, Version 2.0(the "License");
===============================================================================
@project: [CAF] Core Application Framework
@author(s): Anton Yashchenko
@website: https://www.acpp.dev
===============================================================================
@file 
@ingroup
@brief 
#]============================================================================]
cmake_minimum_required (VERSION 3.8...3.10)

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
find_package(nlohmanjson REQUIRED)

find_package(mta_box REQUIRED CONFIG)
find_package(cxxx_box REQUIRED CONFIG)
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

add_library(caf_library STATIC
  "${CAF_LIBRARY_SOURCES_DIR}/caf_cache_file.cpp"
  "${CAF_LIBRARY_SOURCES_DIR}/caf_context.cpp"
  "${CAF_LIBRARY_SOURCES_DIR}/caf_imgui_ext.cpp"
  "${CAF_LIBRARY_SOURCES_DIR}/caf_time.cpp"
)
target_include_directories(caf_library PUBLIC "${CAF_LIBRARY_HEADERS_DIR}")
target_link_libraries(caf_library 
  PUBLIC    mta_box::mta_library
            cxxx_box::cxxx_library
            ImGui-SFML::ImGui-SFML
            nlohmann_json::nlohmann_json
            #wpl_box::wpl_library
)
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
include(SsgProjectPackageUtils)
ssg_export_subproject(
	"${PROJECT_NAME}"
	TARGETS caf_library
	REDIRECT_FIND_PACKAGE
)

#[============================================================================[
Copyright 2025 Anton Yashchenko
Licensed under the Apache License, Version 2.0(the "License");
===============================================================================
@project: [CAF] Core Application Framework
@author(s): Anton Yashchenko
@website: https://www.acpp.dev
#]============================================================================]