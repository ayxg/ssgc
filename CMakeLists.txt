#[============================================================================[
Copyright 2025 Anton Yashchenko
Licensed under the GNU Affero General Public License, Version 3.
===============================================================================
@project: SSG Compiler Monorepo Package
@author(s): Anton Yashchenko
@website: https://www.acpp.dev
===============================================================================
@file 
@ingroup ssgc_box
@brief Root CMake project for the Sophia Silicon Gold Compiler.
@prefix [ssgc_box][SSGC_BOX]
#]============================================================================]

cmake_minimum_required(VERSION 3.29.5) # 3.29.5 ... 3.8
project(ssgc_box
	VERSION 0.0.0.0
	DESCRIPTION "SSG company monorepo : root cmake project"
	HOMEPAGE_URL "www.ssg.com"
	LANGUAGES C CXX	
)

# C++23 is the default standard for SSG projects
if (NOT DEFINED CMAKE_CXX_STANDARD)
  SET(CMAKE_CXX_STANDARD 23)
endif()

# strongly encouraged to enable this globally to avoid conflicts between
# -Wpedantic being enabled and -std=c++20 and -std=gnu++20 for example
# when compiling with PCH enabled
SET(CMAKE_CXX_EXTENSIONS OFF)

# Testing enabled?
enable_testing()

#[============================================================================[
  Internal Projects
#]============================================================================]

# SSGC_BOX_LOCAL_PROJECTS_SOURCE_DIR : local projects source directory.
# @warning subprojects should never depend on this variable.
set(SSGC_BOX_LOCAL_PROJECTS_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/projects")

#======================================
# Declare local projects.
#
# Add subprojects using FetchContent so other subprojects in this project can
# use them as 'find_package(<projectName>)' dependencies.
include(FetchContent)

# ssgmono_box - ssg monorepo config scripts
# Manually add AND find 'ssgmono_box'.
# Afterwards use the 'SsgProjectPackageUtils' module function from ssgmono :
#   'SsgProjectBase_DeclareSubproject(<projectName> SOURCE_DIR [path])'
# to add subprojects in this project.
#
# Other top-level projects should use 'find_package(ssgmono_box)' instead,
# when using the 'ssgmono_box' project a dependency.
FetchContent_Declare(ssgmono_box 
  SOURCE_DIR "${SSGC_BOX_LOCAL_PROJECTS_SOURCE_DIR}/ssgmono_box" 
  OVERRIDE_FIND_PACKAGE
)
find_package(ssgmono_box REQUIRED CONFIG)

#
# Declare all other subprojects here...
#
ssg_declare_subprojects(
	NAMES 
		minitest_box
		mta_box
		cxxx_box
		wpl_box
		caf_box
		cgui_box
		cnd_box
		cide_box
	SOURCE_DIRS
		"${SSGC_BOX_LOCAL_PROJECTS_SOURCE_DIR}/minitest_box"
		"${SSGC_BOX_LOCAL_PROJECTS_SOURCE_DIR}/mta_box"
		"${SSGC_BOX_LOCAL_PROJECTS_SOURCE_DIR}/cxxx_box"
		"${SSGC_BOX_LOCAL_PROJECTS_SOURCE_DIR}/wpl_box"
		"${SSGC_BOX_LOCAL_PROJECTS_SOURCE_DIR}/caf_box"
		"${SSGC_BOX_LOCAL_PROJECTS_SOURCE_DIR}/cgui_box"
		"${SSGC_BOX_LOCAL_PROJECTS_SOURCE_DIR}/cnd_box"
		"${SSGC_BOX_LOCAL_PROJECTS_SOURCE_DIR}/cide_box"
)

#======================================
# Aquire external subprojects.
include(SsgGitPkg)
ssg_gitpkg_imgui_sfml()
ssg_gitpkg_nlohmanjson()
#======================================

#[============================================================================[
  Find dependencies
#]============================================================================]

#find_package(minitest_box REQUIRED CONFIG)
#find_package(mta_box REQUIRED CONFIG)
#find_package(cxxx_box REQUIRED CONFIG)
#find_package(wpl_box REQUIRED CONFIG)
find_package(cnd_box REQUIRED CONFIG)
#find_package(cide_box REQUIRED CONFIG)

#[============================================================================[
Copyright 2025 Anton Yashchenko
Licensed under the GNU Affero General Public License, Version 3.
===============================================================================
@project: SSG Compiler Monorepo Package
@author(s): Anton Yashchenko
@website: https://www.acpp.dev
#]============================================================================]