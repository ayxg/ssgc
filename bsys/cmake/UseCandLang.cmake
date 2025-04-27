#----------------------------------------------------------------------------------------------------------------------#
# Copyright 2024 Anton Yashchenko
# Licensed under the GNU Affero General Public License, Version 3.
#----------------------------------------------------------------------------------------------------------------------#
# @project: C& Programming Language
# @author(s): Anton Yashchenko
# @website: https://www.acpp.dev
#----------------------------------------------------------------------------------------------------------------------#
## @file
## @ingroup cnd_cmake
## @brief C& Programming Language CMake Integration Module
##
## Defined Variables:
#[[

]]
##
##
## How to interop C& and C/C++ using CMake:
##
## 1. To call or use C& as part of a C++ executable:
## Pre-compile the C& source at configure time. If no C/C++ headers that the code depends on change then no C& recompile
## is nessecary. To use the C runtime from C& but still compile as a C++ process the C& code MUST define the "Anonymous 
## C& Runtime Process"(ACARP) in the included source files. All C& definitions inside the ACARP will be part of the 
## static context of the C++ program. Definitions outside this scope will only be transpiled if they are dependencies, 
## usually by literal inlining. The ACARP may or may not define a main function. This function must match the given C 
## runtime main method. If the C& main is defined then you cannot define one in your C/C++ code. The name of this 
## process must be passed as an argument to the C& compiler. eg. --acarp_ident:root
##
## @code "C&"
##    proc @root:{
##      main:(int @argc,char^^ @argv)>int:{
##        return 0;
##      }
##    } 
## @endcode
#----------------------------------------------------------------------------------------------------------------------#

## @addtogroup cnd_cmake
## @{

#----------------------------------------------------------------------------------------------------------------------#
# C&CMake Language Properties
# Properties added to cmake and cmake targets when enabling compilation using the C& compiler.
#----------------------------------------------------------------------------------------------------------------------#

## @prop target CAND_PRECOMP_SOURCE_FILES
define_property(TARGET PROPERTY CAND_PRECOMP_SOURCE_FILES
  BRIEF_DOCS "C& source files(.cnd) to be pre-compiled for the target at config time."
)

## @prop target CAND_SOURCE_FILES
define_property(TARGET PROPERTY CAND_SOURCE_FILES
  BRIEF_DOCS "C& source files(.cnd) to be compiled for the target at build time."
)

#----------------------------------------------------------------------------------------------------------------------#
# Implementation Utility Methods, DO NOT use outside of this file.
#----------------------------------------------------------------------------------------------------------------------#

## @function list_append_if_unique
## @brief Append an element to a list if the element would be unique in the list. (List does not contain element.)
## @param 0 TARGET_LIST
## @param 1 ELEM_VALUE 
function(list_append_if_unique TARGET_LIST ELEM_VALUE)
  list(FIND "${TARGET_LIST}" "${ELEM_VALUE}" ELEM_EXISTS)
  if(ELEM_EXISTS EQUAL -1)
    list(APPEND "${TARGET_LIST}" "${ELEM_VALUE}")
    set("${TARGET_LIST}" "${${TARGET_LIST}}" PARENT_SCOPE)
  endif()
endfunction()

## @macro _impl_get_target_dep_tree
macro(_impl_get_target_dep_tree TARGET OUTLIST)
  list_append_if_unique("${OUTLIST}" "${TARGET}") # Target is the root dependency.

  # Append the direct dependencies.
  get_target_property(TARGET_TYPE "${TARGET}" TYPE)
  if("${TARGET_TYPE}" STREQUAL "INTERFACE_LIBRARY")
    get_target_property(DEPENDENCIES "${TARGET}" INTERFACE_LINK_LIBRARIES)
  else()
    get_target_property(DEPENDENCIES "${TARGET}" LINK_LIBRARIES)    
  endif()

  # Recursivley scan dependencies of the direct dependencies.
  foreach(DEP IN LISTS DEPENDENCIES)
    if(NOT TARGET "${DEP}")
      if(NOT INTERFACE "${DEP}")
        continue()
      endif()
    endif()

    list(FIND "${OUTLIST}" "${DEP}" DEP_ALREADY_LISTED)
    if(NOT DEP_ALREADY_LISTED EQUAL -1)
      continue()
    endif()

    _impl_get_target_dep_tree("${DEP}" "${OUTLIST}")
  endforeach()
endmacro()

## @function get_target_dep_tree
function(get_target_dep_tree TARGET OUTLIST)
  set(${OUTLIST} "${TARGET}")
  _impl_get_target_dep_tree(${TARGET} ${OUTLIST})
  set(${OUTLIST} ${${OUTLIST}} PARENT_SCOPE)
endfunction()



macro(cand_configure)
set(CMAKE_CAND_COMPILER ${CMAKE_SOURCE_DIR}/cnd/cnd.exe)
set(CAND_PRECOMP_BINARY_DIR "${CMAKE_SOURCE_DIR}")     # Default pre-compile source dir.
set(CAND_PRECOMP_SOURCE_DIR "${CMAKE_SOURCE_DIR}") # Default pre-compile output dir.
set(CAND_PRECOMP_AUXILLARY_DIR ${CMAKE_SOURCE_DIR}/cand/aux) # Default pre-compile auxilary dir.
set(CAND_PRECOMP_USE_EXTERNAL_LINKER_DRIVER FALSE)    # Frontend layer which calls the linker if the cli is external.
set(CAND_PRECOMP_USE_EXTERNAL_COMPILER_DRIVER FALSE)  # Frontend layer which calls the compiler if the cli is external.
set(CAND_PRECOMP_EXTERNAL_LINKER_DRIVER)    # Frontend layer which calls the linker if the cli is external.
set(CAND_PRECOMP_EXTERNAL_COMPILER_DRIVER)  # Frontend layer which calls the compiler if the cli is external.
set(CAND_HOST_IMPLICIT_LIBRARIES ${CMAKE_CXX_STANDARD_LIBRARIES_INIT}) # OS-provided libraries which are implicitly linked to a target by the host linker.
set(CAND_PRECOMP_FINAL_OUTPUT_DUMP_FILE_PATH ${CAND_PRECOMP_AUXILLARY_DIR}/final_output_paths.txt)
endmacro()

# Add C& source files to be pre-compiled at CMake config time. 
function(cand_cxx_target_sources)
  set(options)
  set(oneValueArgs TARGET)
  set(multiValueArgs SOURCES)
  cmake_parse_arguments(arg_cand_cxx_target_sources "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
  # Append source file path to CAND_SOURCE_FILES property of the given executable.
  set_target_properties(${arg_cand_cxx_target_sources_TARGET} PROPERTIES CAND_SOURCE_FILES "${arg_cand_cxx_target_sources_SOURCES}")
endfunction()

# Calls the C& compiler executable at CMake config time.
function(cand_precompile)
  set(options)
  set(oneValueArgs TARGET)
  set(multiValueArgs SOURCES)
  cmake_parse_arguments(arg_cand_precompile "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
  
  # Generate target C++ dependency tree to pass to C&.
  #get_target_dep_tree(${arg_cand_precompile_TARGET} TARGET_DEP_TREE)

  # Get the CMake depenant arguments to pass the C& compiler.
  set(CAND_PRECOMP_PARAMS --comp)                                           # Run in composition mode. 
  # When precompiling the resulting output should be injected into the C target's source directory before the build stage.
  list(APPEND CAND_PRECOMP_PARAMS --out_dir=${CAND_PRECOMP_BINARY_DIR})   
  list(APPEND CAND_PRECOMP_PARAMS --work_dir=${CAND_PRECOMP_SOURCE_DIR})   
  list(APPEND CAND_PRECOMP_PARAMS --aux_dir=${CAND_PRECOMP_AUXILLARY_DIR})
  
  # Make sure no C source files are harmed.
  list(APPEND CAND_PRECOMP_PARAMS --no_overwrite)            

  # Dump the final paths so they may be added to the target's source files for the cmake build step.
  list(APPEND CAND_PRECOMP_PARAMS --dump_final_output_paths=${CAND_PRECOMP_FINAL_OUTPUT_DUMP_FILE_PATH})
  
  # For now we are only supporting C++ for embedded pre-compilation.

  # 1. Get host linker properties.
  list(APPEND CAND_PRECOMP_PARAMS --host_cxx_linker=${CMAKE_CXX_COMPILER_LINKER}) 
  list(APPEND CAND_PRECOMP_PARAMS --host_cxx_linker_type=${CMAKE_CXX_COMPILER_LINKER_ID})
  list(APPEND CAND_PRECOMP_PARAMS --host_cxx_linker_driver_type=${CMAKE_CXX_COMPILER_LINKER_FRONTEND_VARIANT})
  list(APPEND CAND_PRECOMP_PARAMS --host_cxx_linker_version=${CMAKE_CXX_COMPILER_LINKER_VERSION})


  if(CAND_PRECOMP_USE_EXTERNAL_LINKER_DRIVER) # Is the linker using an external cli ?
    list(APPEND CAND_PRECOMP_PARAMS --host_cxx_linker_driver_external=TRUE)
    list(APPEND CAND_PRECOMP_PARAMS --host_cxx_linker_driver=${CAND_PRECOMP_EXTERNAL_LINKER_DRIVER})
  else()
    list(APPEND CAND_PRECOMP_PARAMS --host_cxx_linker_driver_external=FALSE) # linker cli is internal.
  endif()

  # 2. Get host C++ compiler properties
  list(APPEND CAND_PRECOMP_PARAMS --host_cxx_cl=${CMAKE_CXX_COMPILER}) 
  list(APPEND CAND_PRECOMP_PARAMS --host_cxx_cl_type=${CMAKE_CXX_COMPILER_ID})
  list(APPEND CAND_PRECOMP_PARAMS --host_cxx_cl_driver_type=${CMAKE_CXX_COMPILER_FRONTEND_VARIANT})
  list(APPEND CAND_PRECOMP_PARAMS --host_cxx_cl_version=${CMAKE_CXX_COMPILER_VERSION})

  if(CAND_PRECOMP_USE_EXTERNAL_COMPILER_DRIVER) # Is the compiler using an external cli ?
    list(APPEND CAND_PRECOMP_PARAMS --host_cxx_cl_driver_external=TRUE)
    list(APPEND CAND_PRECOMP_PARAMS --host_cxx_cl_driver=${CAND_PRECOMP_EXTERNAL_COMPILER_DRIVER})
  else()
    list(APPEND CAND_PRECOMP_PARAMS --host_cxx_cl_driver_external=FALSE) # compiler cli is internal.
  endif()

  # 3. Get the implicitly linked host operating system standard libraries.
  list(APPEND CAND_PRECOMP_PARAMS --host_cxx_implicit_linked_libs=${CAND_HOST_IMPLICIT_LIBRARIES})

  # 4. Get the current C/C++ target's properties.
  get_target_property(CAND_PRECOMP_TARGET_COMPILE_DEFINITIONS ${arg_cand_precompile_TARGET} LINK_LIBRARIES)
  get_target_property(CAND_PRECOMP_TARGET_SOURCES ${arg_cand_precompile_TARGET} SOURCES)
  get_target_property(CAND_PRECOMP_TARGET_INCLUDE_DIRECTORIES ${arg_cand_precompile_TARGET} INCLUDE_DIRECTORIES)

  list(APPEND CAND_PRECOMP_PARAMS --host_cxx_predefs=${CAND_PRECOMP_TARGET_COMPILE_DEFINITIONS})
  list(APPEND CAND_PRECOMP_PARAMS --host_cxx_sources=${CAND_PRECOMP_TARGET_SOURCES})
  list(APPEND CAND_PRECOMP_PARAMS --host_cxx_include_dirs=${CAND_PRECOMP_TARGET_INCLUDE_DIRECTORIES})

  # Join the argument list separated by whitespace.
  #list(JOIN CAND_PRECOMP_PARAMS " " EXPANDED_CAND_PRECOMP_PARAMS)

  # Run the compilation.
  set(CAND_PRECOMP_EXIT_CODE_RESULT) # exit code returned by the c& compiler or a string error if it failed to run.
  set(CAND_PRECOMP_STDOUT_RESULT) # stdout returned by the c& compiler.
  set(CAND_PRECOMP_STDERR_RESULT) # stderr returned by the c& compiler.
  set(CAND_PRECOMP_FINAL_OUTPUT_SOURCES) # sources produced by the C& compiler

  execute_process(COMMAND ${CMAKE_CAND_COMPILER} ${CAND_PRECOMP_PARAMS}
               WORKING_DIRECTORY ${CAND_PRECOMP_SOURCE_DIR}
               RESULT_VARIABLE CAND_PRECOMP_EXIT_CODE_RESULT
               OUTPUT_VARIABLE CAND_PRECOMP_STDOUT_RESULT
               ERROR_VARIABLE CAND_PRECOMP_STDERR_RESULT
                # [INPUT_FILE <file>] 
                # [OUTPUT_FILE <file>]
                # [ERROR_FILE <file>]
                # [OUTPUT_QUIET]
                # [ERROR_QUIET]
                COMMAND_ECHO STDOUT
                # [OUTPUT_STRIP_TRAILING_WHITESPACE]
                # [ERROR_STRIP_TRAILING_WHITESPACE]
                ENCODING AUTO
                ECHO_OUTPUT_VARIABLE
                ECHO_ERROR_VARIABLE
                #COMMAND_ERROR_IS_FATAL ANY
  )

  # Get the resulting output paths from 'CAND_PRECOMP_FINAL_OUTPUT_DUMP_FILE_PATH' add them to the target.
  #file(READ ${CAND_PRECOMP_FINAL_OUTPUT_DUMP_FILE_PATH} CAND_PRECOMP_FINAL_OUTPUT_SOURCES)
  #target_sources(${arg_cand_precompile_TARGET} ${CAND_PRECOMP_FINAL_OUTPUT_SOURCES})
endfunction()

