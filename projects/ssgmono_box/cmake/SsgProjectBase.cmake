#[====================================================================================================================[
Copyright 2025 Anton Yashchenko
Licensed under the Apache License, Version 2.0(the "License");
=======================================================================================================================
@project: [SSGMONO] Monorepo Config Scripts Package
@author(s): Anton Yashchenko
@website: https://www.acpp.dev
=======================================================================================================================
@file SsgProjectBase module
@ingroup ssgmono_box
@brief SSG configure standard project variables and options.
#]====================================================================================================================]

include_guard()
include(SsgCMakeUtils)

#[=[
  @dir-property SSG_PROJECT_EXPORT_DIR
  @brief Contains input files for the project export scripts in the source.
  @default "${CMAKE_CURRENT_SOURCE_DIR}/export"
]=]
define_property(DIRECTORY PROPERTY SSG_PROJECT_EXPORT_DIR)

#[=[
  @dir-property SSG_PROJECT_IMPORT_DIR
  @brief Contains files exported by the project for the build tree/install.
  @default "${CMAKE_CURRENT_BINARY_DIR}/import"
]=]
define_property(DIRECTORY PROPERTY SSG_PROJECT_IMPORT_DIR)

#[=[
  @dir-property SSG_PROJECT_CMAKE_MODULES_DIR
  @brief Contains project's cmake scripts and find_package scripts.
  @default "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
]=]
define_property(DIRECTORY PROPERTY SSG_PROJECT_CMAKE_MODULES_DIR)


function(ssg_setup_standard_project_vars)
  set(options)
  set(one_value_args)
  set(multi_value_args)
  cmake_parse_arguments(PARSE_ARGV 0 arg "${options}" "${one_value_args}" "${multi_value_args}")

  if(PROJECT_IS_TOP_LEVEL)  
    message(DEBUG "[SSG] Including project '${PROJECT_NAME}' as top level project.") 
  else()
    message(DEBUG "[SSG] Including project '${PROJECT_NAME}' as subproject.")
  endif()

  set_directory_properties(PROPERTIES 
    SSG_PROJECT_EXPORT_DIR    "${CMAKE_CURRENT_SOURCE_DIR}/export"
    SSG_PROJECT_IMPORT_DIR    "${CMAKE_CURRENT_BINARY_DIR}/import"
    SSG_PROJECT_CMAKE_MODULES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
  )
endfunction()


#[====================================================================================================================[
Copyright 2025 Anton Yashchenko
Licensed under the Apache License, Version 2.0(the "License");
=======================================================================================================================
@project: [SSGMONO] Monorepo Config Scripts Package
@author(s): Anton Yashchenko
@website: https://www.acpp.dev
#]====================================================================================================================]
