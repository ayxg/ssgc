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
cmake_minimum_required (VERSION 3.8...3.10)

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

# Export
if(PROJECT_IS_TOP_LEVEL)
	include(GNUInstallDirs)
	install(FILES
            "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
						"${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
					DESTINATION "${CMAKE_INSTALL_PREFIX}"
	)
  
	install(FILES
            "${CMAKE_CURRENT_SOURCE_DIR}/cmake/SsgCMakeUtils.cmake"
            "${CMAKE_CURRENT_SOURCE_DIR}/cmake/SsgEmbedFiles.cmake"
            "${CMAKE_CURRENT_SOURCE_DIR}/cmake/SsgGitPkg.cmake"
            "${CMAKE_CURRENT_SOURCE_DIR}/cmake/SsgProjectBase.cmake"
            "${CMAKE_CURRENT_SOURCE_DIR}/cmake/SsgProjectPackageUtils.cmake"
					DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake"
	)

  # Install SsgProjectPackageUtils module "subproject config input files"
	install(FILES
            "${CMAKE_CURRENT_SOURCE_DIR}/cmake/SsgProjectPackageUtils/subproject-config-init.cmake.in"
            "${CMAKE_CURRENT_SOURCE_DIR}/cmake/SsgProjectPackageUtils/alias-targets-init.cmake.in"
					DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake/SsgProjectPackageUtils"
	)    

	include(CMakePackageConfigHelpers)
	configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/config.cmake.in
		"${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
		INSTALL_DESTINATION "${CMAKE_INSTALL_PREFIX}"
	)

	write_basic_package_version_file(
		"${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
		VERSION "${PROJECT_VERSION}"
		COMPATIBILITY AnyNewerVersion
	)
	
endif()


#[============================================================================[
Copyright 2025 Anton Yashchenko
Licensed under the Apache License, Version 2.0(the "License");
===============================================================================
@project: [SSGMONO] Monorepo Config Scripts Package
@author(s): Anton Yashchenko
@website: https://www.acpp.dev
#]============================================================================]

