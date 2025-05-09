#[====================================================================================================================[
Copyright 2025 Anton Yashchenko
Licensed under the Apache License, Version 2.0(the "License");
=======================================================================================================================
@project: [SSGMONO] Monorepo Config Scripts Package
@author(s): Anton Yashchenko
@website: https://www.acpp.dev
=======================================================================================================================
@file SsgCMakeUtils.cmake
@ingroup ssgmono_box
@brief CMake utility functions prefixed with [ssg].
#]====================================================================================================================]
include_guard()

function(ssg_assert_arguments)
  set(options REQUIRED NOT_EMPTY)
  set(one_value_args)
  set(multi_value_args ARGUMENTS)
  cmake_parse_arguments(PARSE_ARGV 1 arg "${options}" "${one_value_args}" "${multi_value_args}")  
  
  foreach(fn_argument IN LISTS arg_ARGUMENTS)
    if(${arg_REQUIRED})
      if(NOT DEFINED ${fn_argument})
        message(FATAL_ERROR "Required argument '${fn_argument}' was not provided to ${ARGV0}.")
      endif()
    endif()

    if(${arg_NOT_EMPTY})
        if("${fn_argument}" STREQUAL "")
        message(FATAL_ERROR "Arg '${fn_argument}' for command ${ARGV0} was empty, must have value.")
      endif()
    endif()
  endforeach()
endfunction()

function(ssg_check_unparsed_arguments)
  set(options REQUIRE)
  set(one_value_args)
  set(multi_value_args UNPARSED_ARG)
  cmake_parse_arguments(PARSE_ARGV 1 arg "${options}" "${one_value_args}" "${multi_value_args}")  
  if(DEFINED ${arg_UNPARSED_ARG})
    if(${arg_REQUIRE})
      message(FATAL_ERROR "[${ARGV0}] Unparsed arguments detected in function or macro. Values: ${${arg_UNPARSED_ARG}}")    
    endif()
      message(WARNING "[${ARGV0}] Unparsed arguments detected in function or macro. Values: ${${arg_UNPARSED_ARG}}")
  endif()
endfunction()

#[====================================================================================================================[
Copyright 2025 Anton Yashchenko
Licensed under the Apache License, Version 2.0(the "License");
=======================================================================================================================
@project: [SSGMONO] Monorepo Config Scripts Package
@author(s): Anton Yashchenko
@website: https://www.acpp.dev
#]====================================================================================================================]
