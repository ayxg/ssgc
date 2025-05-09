#[====================================================================================================================[
Copyright 2025 Anton Yashchenko
Licensed under the Apache License, Version 2.0(the "License");
=======================================================================================================================
@project: Minitest Framework
@author(s): Anton Yashchenko
@website: https://www.acpp.dev
=======================================================================================================================
@file 
@ingroup minitest_cmake
@brief CMake utility functions prefixed with [smk].
#]====================================================================================================================]
include_guard()

function(smk_assert_arguments)
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

function(smk_check_unparsed_arguments)
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
@project: Minitest Library
@author(s): Anton Yashchenko
@website: https://www.acpp.dev
=======================================================================================================================
@notes Minitest CMake Integration

=======================================================================================================================
@rationale Minitest CMake Integration
 
- Wouldnt it be useful to have a cli arg to only detect test cases? Yes. The difficulty is running this target is not
  trivial at config-time. When running the test target its dependencies may not be available yet. try_run() and
  try_compile() methods seems like an option at first. Using these function on every test would be expensive,
  the dependencies must be scanned manually then passed to this function, building a project would require some
  custom project format which will not work for a library. Furthermore, it complicates cross compilation.
  Currently the next best solution is to run the scans at build time then cache the results, upon next build,
  the cached tests will be added. Use add_custom_command.

#]====================================================================================================================]
