#[====================================================================================================================[
Copyright 2025 Anton Yashchenko
Licensed under the GNU Affero General Public License, Version 3.
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

#[====================================================================================================================[
  @macro ssg_message(<logLevel> <messageContent>)
  @brief [INTERNAL USE ONLY] Called by ssg_message macro.
  @sinlge ARGV0 CMake log level or SSG log level.
  @sinlge ARGV1 message content
]====================================================================================================================]
macro(ssg_message)
  if("${ARGV0}" STREQUAL "FUNC_ERROR"
    OR "${ARGV0}" STREQUAL "FUNC_WARNING"
    OR "${ARGV0}" STREQUAL "FUNC_DEBUG")
    z_ssg_message_func(
      FN_NAME "${CMAKE_CURRENT_FUNCTION}"
      FN_LINE "${CMAKE_CURRENT_FUNCTION_LIST_LINE}"
      FN_FILE "${CMAKE_CURRENT_FUNCTION_LIST_FILE}"
      LOG_LEVEL "${ARGV0}"
      USER_CONTENT "${ARGV1}"
    )
  else() # CMake log level, prefix and forward to message()
    message("${ARGV0}" "[SSG]${ARGV1}")
  endif()
endmacro()


#[====================================================================================================================[
  @macro ssg_assert_arguments(
    <cmakeParseArgsPrefix>
    ARGUMENTS <unprefixedArg1> [unprefixedArg2]...
    [REQUIRED]
    [NOT_EMPTY]
    [FATAL_ERROR]
  )
  @brief Warns with diagnostic message if given function arguments are defined and/or not empty. 
         ONLY call after 'cmake_parse_arguments' inside a function definition.
  Sample Use:
  @code
    # Require non-empty 'arg_FOO', 'arg_BAR' and 'arg_GROK'.
    ssg_assert_arguments(arg
      ARGUMENTS FOO BAR GROK
      REQUIRED
      NOT_EMPTY
    )
  @endcode

  "Added in version 3.15: <one_value_keywords> and <multi_value_keywords> that were given no values at all are 
  collected in a variable <prefix>_KEYWORDS_MISSING_VALUES that will be undefined if all keywords received values. 
  This can be checked to see if there were keywords without any values given."
  - https://cmake.org/cmake/help/latest/command/cmake_parse_arguments.html
]====================================================================================================================]
macro(ssg_assert_arguments)
  # var prefix : [_ssg_assert_arguments]
  set(options REQUIRED NOT_EMPTY FATAL_ERROR)
  set(one_value_args)
  set(multi_value_args ARGUMENTS)
  cmake_parse_arguments(_ssg_assert_arguments
      "${options}" "${one_value_args}" "${multi_value_args}"
      ${ARGN}
  )
  if(_ssg_assert_arguments_FATAL_ERROR)
    foreach(_ssg_assert_arguments_fn_arg IN ITEMS ${_ssg_assert_arguments_ARGUMENTS})
      if(${_ssg_assert_arguments_REQUIRED})
        if(NOT DEFINED "${_ssg_assert_arguments_fn_arg}")
          ssg_message(FUNC_ERROR "Required argument '${_ssg_assert_arguments_fn_arg}' was not provided.")
        endif()
      endif()

      if(${_ssg_assert_arguments_NOT_EMPTY})
        if("${_ssg_assert_arguments_fn_arg}" STREQUAL "")
          ssg_message(FUNC_ERROR "Argument '${_ssg_assert_arguments_fn_arg}' was empty, must have value.")
        endif()
      endif()
    endforeach()
  else()
    foreach(_ssg_assert_arguments_fn_arg IN ITEMS ${_ssg_assert_arguments_ARGUMENTS})
      if(${_ssg_assert_arguments_REQUIRED})
        if(NOT DEFINED "${_ssg_assert_arguments_fn_arg}")
          ssg_message(FUNC_WARNING "Argument '${_ssg_assert_arguments_fn_arg}' was not provided.")
        endif()
      endif()

      if(${_ssg_assert_arguments_NOT_EMPTY})
        if("${_ssg_assert_arguments_fn_arg}" STREQUAL "")
          ssg_message(FUNC_WARNING "Argument '${_ssg_assert_arguments_fn_arg}' was empty.")
        endif()
      endif()
    endforeach()
  endif()
endmacro()


#[====================================================================================================================[
  @macro ssg_check_unparsed_args(<cmakeParseArgsPrefix> [FATAL_ERROR])
  @brief Call after cmake_parse_arguments to check for unparsed arguments.
  @sinlge ARGV0 Prefix passed to the last call of cmake_parse_arguments() function.
  @option FATAL_ERROR Causes a fatal error if unparsed arguments are detected.

  "All remaining arguments are collected in a variable <prefix>_UNPARSED_ARGUMENTS that will be undefined if all 
  arguments were recognized. This can be checked afterwards to see whether your macro or function was called with 
  unrecognized parameters." - https://cmake.org/cmake/help/latest/command/cmake_parse_arguments.html
#]====================================================================================================================]
macro(ssg_check_unparsed_arguments)
  if(DEFINED "${ARGV0}_UNPARSED_ARGUMENTS")
    if(${ARGC} GREATER "1" AND "${ARGV1}" STREQUAL "FATAL_ERROR")
      ssg_message(FUNC_ERROR 
        "Unparsed arguments detected in function or macro. Values: ${${ARGV0}_UNPARSED_ARGUMENTS}"
      )    
    endif()

    ssg_message(FUNC_WARNING 
      "Unparsed arguments detected in function or macro. Values: ${${ARGV0}_UNPARSED_ARGUMENTS}"
    )
  endif()
endmacro()


#[========================================================[
  Internal Use Only
#]========================================================]

#[====================================================================================================================[
  @function z_ssg_message(<logLevel> <message>)
  @brief [INTERNAL USE ONLY] Called by ssg_message macro.
  @sinlge FN_NAME message function name tag
  @sinlge FN_LINE message function line tag
  @sinlge FN_FILE message function list file tag
  @sinlge LOG_LEVEL ssg log level, must be one of FUNC_ERROR, FUNC_WARNING, FUNC_DEBUG
  @sinlge USER_CONTENT message content
]====================================================================================================================]
function(z_ssg_message_func)
  set(options)
  set(one_value_args FN_NAME FN_LINE FN_FILE LOG_LEVEL USER_CONTENT)
  set(multi_value_args)
  cmake_parse_arguments(PARSE_ARGV 0 arg "${options}" "${one_value_args}" "${multi_value_args}")

  # Figure out the forward CMake log level.
  set(this_cmake_log_level "")
  if("${arg_LOG_LEVEL}" STREQUAL "FUNC_ERROR")
    set(this_cmake_log_level "FATAL_ERROR")
  elseif("${arg_LOG_LEVEL}" STREQUAL "FUNC_WARNING")
    set(this_cmake_log_level "WARNING")
  elseif("${arg_LOG_LEVEL}" STREQUAL "FUNC_DEBUG")
    set(this_cmake_log_level "DEBUG")
  else() # This function only handles [FUNC_] log levels .
    message(FATAL_ERROR 
      "${arg_indent}[SSG][FUNC_ERROR][Fn:${CMAKE_CURRENT_FUNCTION}][Line:${CMAKE_CURRENT_FUNCTION_LIST_LINE}][File]:\n"
      "${arg_indent}${CMAKE_CURRENT_FUNCTION_LIST_FILE}\n"
      "${arg_indent}[Message]: Invalid log level: ${arg_LOG_LEVEL}."
    )
  endif()

  #
  # Setup the formatted output.
  #
  # Must indent lines to get proper 'formatted' message.
  if("${arg_LOG_LEVEL}" STREQUAL "FUNC_WARNING" OR "${arg_LOG_LEVEL}" STREQUAL "FUNC_ERROR") 
    set(arg_indent " ")
  else()
    set(arg_indent "") 
  endif()

  # TODO: improve indentation detection for multi line error messages ?
  message("${this_cmake_log_level}"
    "${arg_indent}[SSG][${this_cmake_log_level}][Fn:${arg_FN_NAME}][Line:${arg_FN_LINE}][File]:\n"
    "${arg_indent}${arg_FN_FILE}\n"
    "${arg_indent}[Message]: ${arg_USER_CONTENT}"
  )
endfunction()

#[========================================================[
  end - Internal Use Only
#]========================================================]

#[====================================================================================================================[
Copyright 2025 Anton Yashchenko
Licensed under the GNU Affero General Public License, Version 3.
=======================================================================================================================
@project: [SSGMONO] Monorepo Config Scripts Package
@author(s): Anton Yashchenko
@website: https://www.acpp.dev
#]====================================================================================================================]
