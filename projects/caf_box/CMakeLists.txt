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
    # /CAF/Demo/
    "${CAF_LIBRARY_SOURCES_DIR}/CAF/Demo/ExWindowHandling.cpp"
    "${CAF_LIBRARY_SOURCES_DIR}/CAF/Demo/ExAnimatedSquare.cpp"
    "${CAF_LIBRARY_SOURCES_DIR}/CAF/Demo/ExBasicUserInterface.cpp"
    "${CAF_LIBRARY_SOURCES_DIR}/CAF/Demo/ExBasicErrorModal.cpp"
    "${CAF_LIBRARY_SOURCES_DIR}/CAF/Demo/ExAdvancedErrorModal.cpp"
    "${CAF_LIBRARY_SOURCES_DIR}/CAF/Demo/ExLoadingBanner.cpp"
    "${CAF_LIBRARY_SOURCES_DIR}/CAF/Demo/ExImGuiTheme.cpp"
    "${CAF_LIBRARY_SOURCES_DIR}/CAF/Demo/ExUsingEntities.cpp"

    # /CAF/Tool/
    "${CAF_LIBRARY_SOURCES_DIR}/CAF/Tool/BconObject.cpp"
    "${CAF_LIBRARY_SOURCES_DIR}/CAF/Tool/ProgressSequence.cpp"
    "${CAF_LIBRARY_SOURCES_DIR}/CAF/Tool/Timeframe.cpp"
    "${CAF_LIBRARY_SOURCES_DIR}/CAF/Tool/Timeline.cpp"
    "${CAF_LIBRARY_SOURCES_DIR}/CAF/Tool/Timer.cpp"
    "${CAF_LIBRARY_SOURCES_DIR}/CAF/Tool/Stopwatch.cpp"

    # /CAF/System/
    "${CAF_LIBRARY_SOURCES_DIR}/CAF/System/Windows.cpp"
    "${CAF_LIBRARY_SOURCES_DIR}/CAF/System/GLWindow.cpp"

    # /CAF/Extension/
    "${CAF_LIBRARY_SOURCES_DIR}/CAF/Extension/ExtImGui.cpp"
    "${CAF_LIBRARY_SOURCES_DIR}/CAF/Extension/ExtImGuiTheme.cpp"
    "${CAF_LIBRARY_SOURCES_DIR}/CAF/Extension/ExtImGuiWidget.cpp"
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
    # /CAF/
    "${CAF_LIBRARY_HEADERS_DIR}/CAF/Demo.hpp"
    "${CAF_LIBRARY_HEADERS_DIR}/CAF/PreConfig.hpp"

    # /CAF/Tool/
    "${CAF_LIBRARY_HEADERS_DIR}/CAF/Tool/BconObject.hpp"
    "${CAF_LIBRARY_HEADERS_DIR}/CAF/Tool/JsonObject.hpp"
    "${CAF_LIBRARY_HEADERS_DIR}/CAF/Tool/ProgressSequence.hpp"
    "${CAF_LIBRARY_HEADERS_DIR}/CAF/Tool/Timeframe.hpp"
    "${CAF_LIBRARY_HEADERS_DIR}/CAF/Tool/Timeline.hpp"
    "${CAF_LIBRARY_HEADERS_DIR}/CAF/Tool/Timer.hpp"
    "${CAF_LIBRARY_HEADERS_DIR}/CAF/Tool/Trigger.hpp"
    "${CAF_LIBRARY_HEADERS_DIR}/CAF/Tool/Stopwatch.hpp"

    # /CAF/System/
    "${CAF_LIBRARY_HEADERS_DIR}/CAF/System/Windows.hpp"
    "${CAF_LIBRARY_HEADERS_DIR}/CAF/System/Entity.hpp"

    # /CAF/Extension/
    "${CAF_LIBRARY_HEADERS_DIR}/CAF/Extension/ExtImGui.hpp"
    "${CAF_LIBRARY_HEADERS_DIR}/CAF/Extension/ExtImGuiTheme.hpp"
    "${CAF_LIBRARY_HEADERS_DIR}/CAF/Extension/ExtImGuiWidget.hpp"
)

set_target_properties(caf_library 
  PROPERTIES  CMAKE_CXX_STANDARD 20
              CMAKE_CXX_STANDARD_REQUIRED ON
)

#[============================================================================[
  Test Targets
#]============================================================================]

#[====================================[
  Test Suite : UtDemoExamples
#]====================================]

minitest_add_executable(
  NAME                UtDemoExamples
  INCLUDE_DIRECTORIES test
  LINK_LIBS           caf_library
  TEST_HEADERS        UtDemoExamples.hpp
)

minitest_from_headers(
  PREFIX UtCaf
  TARGET UtDemoExamples
  HEADERS UtDemoExamples.hpp
)

minitest_add_executable(
  NAME                UtEntitySystem
  INCLUDE_DIRECTORIES test
  LINK_LIBS           caf_library
  TEST_HEADERS        UtEntitySystem.hpp
)

minitest_from_headers(
  PREFIX UtCaf
  TARGET UtEntitySystem
  HEADERS UtEntitySystem.hpp
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