#[====================================================================================================================[
Copyright 2025 Anton Yashchenko
Licensed under the GNU Affero General Public License, Version 3.
=======================================================================================================================
@project: [SSGMONO] Monorepo Config Scripts Package
@author(s): Anton Yashchenko
@website: https://www.acpp.dev
=======================================================================================================================
@file SsgProjectBase module
@ingroup ssgmono_box
@brief SSG project packaging utilities.
#]====================================================================================================================]
include_guard()
include(SsgCMakeUtils)


#[============================================================================[
  @function ssg_declare_subproject(

  )
  @brief 
#]============================================================================]
function(ssg_declare_subproject projectName)
  set(options)
  set(one_value_args SOURCE_DIR)
  set(multi_value_args)
  cmake_parse_arguments(PARSE_ARGV 1 arg "${options}" "${one_value_args}" "${multi_value_args}")

  # Include as sub-project. Use OVERRIDE_FIND_PACKAGE to force subsequent find_package calls
  # to use the local project configuration files. It's up to the submodule maintainer to
  # ensure local configuration exports match the exported targets.
  include(FetchContent)
  FetchContent_Declare("${ARGV0}" 
    SOURCE_DIR "${arg_SOURCE_DIR}" 
    OVERRIDE_FIND_PACKAGE
  )
endfunction()

#[============================================================================[
  @function ssg_configure_alias_targets_file(
    TARGETS <target1> <target2> ...
    PREFIX <prefix>
    OUTPUT_FILE_PATH <path>
    [INPUT_FILE_PATH <path>]
  )
  @brief Creates an alias targets file to be included by the subproject config file.

  Appends given prefix to all targets in list. For each target, adds a generated cmake line base on target type:
    add_[library|executable](${arg_PREFIX}${target} ALIAS ${target})

  When INPUT_FILE_PATH is provided, the input file must be a valid cmake script configure file.
  The file MUST contain @INIT_SUBPROJECT_TARGETS@ placeholder at the top of the file.
  Additional target properties may be exported in this script.
  It is reccommended to name the input file subproject-targets.cmake.in.
#]============================================================================]
function(ssg_configure_alias_targets_file)
  set(options)
  set(one_value_args PREFIX OUTPUT_FILE_PATH INPUT_FILE_PATH)
  set(multi_value_args TARGETS)
  cmake_parse_arguments(PARSE_ARGV 0 arg "${options}" "${one_value_args}" "${multi_value_args}")
  ssg_check_unparsed_arguments(ssg_configure_alias_targets_file ARGUMENT arg_UNPARSED_ARGUMENTS)
  ssg_assert_arguments(ssg_configure_alias_targets_file ARGUMENTS arg_PREFIX arg_TARGETS arg_OUTPUT_FILE_PATH REQUIRED NOT_EMPTY)

  # Declare config variables for SSG_THIS_PACKAGE_LOCAL_TARGETS_INIT generated header.
  set(SSG_THIS_PACKAGE_EXPORT_NAME ${arg_PREFIX})
  set(SSG_THIS_PACKAGE_EXPORT_PREFIX "${arg_PREFIX}::")
  
  # Append alias prefix to all targets in list.
  set(SSG_THIS_PACKAGE_PREFIXED_TARGETS_LIST "")
  foreach(exported_target ${arg_TARGETS})
    list(APPEND 
      SSG_THIS_PACKAGE_PREFIXED_TARGETS_LIST 
      "${SSG_THIS_PACKAGE_EXPORT_PREFIX}${exported_target}"
    )
  endforeach()

  # For each target. Add a generated cmake line: 
  #   add_[library|executable](${arg_PREFIX}${target} ALIAS ${target}) 
  set(SSG_THIS_PACKAGE_LOCAL_TARGETS_EXPORT "")
  foreach(alias_target ${arg_TARGETS})
    get_target_property(target_type ${alias_target} TYPE)
    if(${target_type} STREQUAL "EXECUTABLE")
      string(APPEND SSG_THIS_PACKAGE_LOCAL_TARGETS_EXPORT 
        "add_executable(${SSG_THIS_PACKAGE_EXPORT_PREFIX}${alias_target} ALIAS ${alias_target})\n"
      )
    else()
      string(APPEND SSG_THIS_PACKAGE_LOCAL_TARGETS_EXPORT 
        "add_library(${SSG_THIS_PACKAGE_EXPORT_PREFIX}${alias_target} ALIAS ${alias_target})\n"
      )
    endif()
  endforeach()

  # Generate the init header for the targets file.
  configure_file(
    "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/SsgProjectPackageUtils/subproject-targets-init.cmake.in"
    "${CMAKE_FILES_DIRECTORY}/SsgProjectPackageUtils/${SSG_THIS_PACKAGE_EXPORT_NAME}-subproject-targets-init.cmake"
    @ONLY
  )

  # Read generated header into 'SSG_THIS_PACKAGE_LOCAL_TARGETS_INIT' for use in the
  # config call.
  file(READ 
    "${CMAKE_FILES_DIRECTORY}/SsgProjectPackageUtils/${SSG_THIS_PACKAGE_EXPORT_NAME}-subproject-targets-init.cmake"
    SSG_THIS_PACKAGE_LOCAL_TARGETS_INIT
  )

  # Append user config if 'INPUT_FILE_PATH' is provided, else generate the targets file automatically.
  if(NOT DEFINED arg_INPUT_FILE_PATH OR "${arg_INPUT_FILE_PATH}" STREQUAL "")
    file(WRITE 
      "${CMAKE_FILES_DIRECTORY}/SsgProjectPackageUtils/${SSG_THIS_PACKAGE_EXPORT_NAME}-subproject-targets.cmake.in"
      "@SSG_THIS_PACKAGE_LOCAL_TARGETS_INIT@\n"
    )
    configure_file(
      "${CMAKE_FILES_DIRECTORY}/SsgProjectPackageUtils/${SSG_THIS_PACKAGE_EXPORT_NAME}-subproject-targets.cmake.in"
      "${arg_OUTPUT_FILE_PATH}"
      @ONLY
    )
  else()
    configure_file(
      "${arg_INPUT_FILE_PATH}"
      "${arg_OUTPUT_FILE_PATH}"
      @ONLY
    )
  endif()
endfunction()


#[============================================================================[
  @function SsgProjectBase_ConfigureLocalConfigFile(
    CONFIG_LOCAL
    CONFIG
  )
  @brief Export a subproject config file which emulates exported targets when 
         the project is brought in with FetchContent.

  Allows overriding find_package to trigger this extra configuration file when 
  called for the subproject.
  
  When FetchContent is called  with 'OVERRIDE_FIND_PACKAGE', cmake automatically 
  creates a config and version file for the subproject inside the CMAKE_PACKAGE_REDIRECTS_DIR.
  The generated config file does only one thing : include an optional file called 
  [pkg-name]-extras.cmake or [pkg-name]Extras.cmake. 
  
  This function adds an 'extras' file for this subproject which includes the
  inside CMAKE_PACKAGE_REDIRECTS_DIR. The generated 'extras' file includes
  the following optional files:
    - PACKAGE_CONFIG_FILE       [pkg-name]-config.cmake 
      : The external config file (can be used to implement local and export at once)

    - PACKAGE_VERSION_FILE      [pkg-name]-config-version.cmake 

    - PACKAGE_CONFIG_LOCAL_FILE [pkg-name]-config-local.cmake 
      : Additional local config. Loads before the external config file.
  
  The files should define any additional commands or variables that the
  dependency would normally provide but which won't be available globally
  if the dependency is brought into the build via FetchContent instead.
#]============================================================================]
function(ssg_export_subproject_config)
  set(options)
  set(one_value_args CONFIG_SUBPROJECT CONFIG OUTPUT_DIR)
  set(multi_value_args)
  cmake_parse_arguments(PARSE_ARGV 1 arg "${options}" "${one_value_args}" "${multi_value_args}")
  ssg_check_unparsed_arguments(ssg_configure_alias_targets_file ARGUMENT arg_UNPARSED_ARGUMENTS)
  ssg_assert_arguments(ssg_configure_alias_targets_file ARGUMENTS arg_OUTPUT_DIR REQUIRED NOT_EMPTY)

  if(DEFINED arg_CONFIG_SUBPROJECT AND NOT "${arg_CONFIG_SUBPROJECT}" STREQUAL "")
    if(NOT EXISTS "${arg_CONFIG_SUBPROJECT}")
      message(FATAL_ERROR "[ssg_export_subproject_config] 'CONFIG_SUBPROJECT' argument file does not exist.\n    Path : ${arg_CONFIG_SUBPROJECT}")
    endif()
    set(enable_subproject_config_file ON)
  else()
    set(enable_subproject_config_file OFF)
  endif()

  if(DEFINED arg_CONFIG AND NOT "${arg_CONFIG}" STREQUAL "")
    if(NOT EXISTS "${arg_CONFIG}")
      message(FATAL_ERROR "[ssg_export_subproject_config] 'CONFIG' argument file does not exist.\n    Path : ${arg_CONFIG}")
    endif()
    set(enable_config_file ON)
  else()
    set(enable_config_file OFF)
  endif()

  set(this_import_dir "${arg_OUTPUT_DIR}")
  set(this_export_name "${ARGV0}")

  set(extras_file_content
    "# Generated by SsgProjectBase_ExportLocalConfigFile.\n"
  )

  if(enable_subproject_config_file)
    set(this_config_file_path "${this_import_dir}/${this_export_name}-subproject-config.cmake")
    configure_file(
      "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/SsgProjectPackageUtils/subproject-config-init.cmake.in" 
      "${CMAKE_FILES_DIRECTORY}/SsgProjectPackageUtils/${this_export_name}-subproject-config-init.cmake"
      @ONLY
    )
    file(READ 
      "${CMAKE_FILES_DIRECTORY}/SsgProjectPackageUtils/${this_export_name}-subproject-config-init.cmake"
      SSG_THIS_PACKAGE_SUBPROJECT_CONFIG_INIT
    )

    set(SSG_THIS_PACKAGE_IMPORT_DIR ${this_import_dir})
    configure_file("${arg_CONFIG_SUBPROJECT}" "${this_config_file_path}" @ONLY)
    string(APPEND extras_file_content "include(\"${this_config_file_path}\")\n")
  endif()

  if(enable_config_file)
    include(CMakePackageConfigHelpers)
    configure_package_config_file(
      "${arg_CONFIG}"
      "${this_import_dir}/${this_export_name}-config.cmake"
      INSTALL_DESTINATION "${this_import_dir}"
    )
    string(APPEND extras_file_content "include(\"${this_import_dir}/${this_export_name}-config.cmake\")\n")
  endif()

  file(WRITE 
    "${CMAKE_FIND_PACKAGE_REDIRECTS_DIR}/${this_export_name}-extra.cmake"
    "${extras_file_content}"
  )
endfunction()


#[====================================================================================================================[
Copyright 2025 Anton Yashchenko
Licensed under the GNU Affero General Public License, Version 3.
=======================================================================================================================
@project: [SSGMONO] Monorepo Config Scripts Package
@author(s): Anton Yashchenko
@website: https://www.acpp.dev
#]====================================================================================================================]

