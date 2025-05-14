#[============================================================================[
Copyright 2025 Anton Yashchenko
Licensed under the Apache License, Version 2.0(the "License");
===============================================================================
@project: [SSGMONO] Monorepo Config Scripts Package
@author(s): Anton Yashchenko
@website: https://www.acpp.dev
===============================================================================
@file Project CMakeLists.txt
@ingroup ssgmono_box
@brief CMake scripts and configs for internal boxes(packages).
#]============================================================================]
cmake_minimum_required (VERSION 3.8)

project(ssgmono_box
	VERSION 0.0.0.0
	DESCRIPTION "SSG Monorepo Configuration Project"
	HOMEPAGE_URL "www.acpp.dev"
	LANGUAGES C CXX	
)

# Add ssgmono scripts to cmake module path. Other subprojects should use:
#		find_package(ssgmono_box REQUIRED CONFIG)
#		include(SsgProjectBase)
#		ssg_setup_standard_project_vars()
#
# @note this must be exported in the subproject config file.
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(SsgProjectBase)
ssg_setup_standard_project_vars()

include(SsgProjectPackageUtils)
ssg_export_subproject(
	"${PROJECT_NAME}"
	MODULE_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
	REDIRECT_FIND_PACKAGE
)

#[============================================================================[
Copyright 2025 Anton Yashchenko
Licensed under the Apache License, Version 2.0(the "License");
===============================================================================
@project: [SSGMONO] Monorepo Config Scripts Package
@author(s): Anton Yashchenko
@website: https://www.acpp.dev
#]============================================================================]

