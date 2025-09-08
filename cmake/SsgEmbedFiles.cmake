#[====================================================================================================================[
Copyright 2025 Anton Yashchenko
Licensed under the GNU Affero General Public License, Version 3.
=======================================================================================================================
@project: [SSGMONO] Monorepo Config Scripts Package
@author(s): Anton Yashchenko
@website: https://www.acpp.dev
=======================================================================================================================
@file SsgEmbedFiles module
@ingroup ssgmono_box
@brief Helper for generating C++ headers/sources that embed binary files into a target.
#]====================================================================================================================]

include_guard()
include(SsgCMakeUtils)

#[====================================================================================================================[
  @function ssg_embed_files(
    FILES [file_path1 [file_path2...]]
    NAMES [var_name1 [var_name2...]]
    INPUT [file_path]
    OUTPUT [file_path]
  )

  @brief Helper for generating C++ headers/sources that embed binary files into a target.

  @multi FILES : List of paths to binary input files. 

  @multi NAMES : List of variable names for each binary input file to pass to the input config script.
                 For each name, the name and SIZE_OF_${name} will be set as config input vars.

  @single INPUT : Path to a cmake config.in file that will be processed to generate the output file.

  @single OUTPUT : Path where to write the generated output file.
  
  For each provided path and name pair:
  - Loads the file and converts it to a comma separated byte list. 
  - Sets the byte list to the matching variable name.
  - Sets the byte list element count to "SIZE_OF_${variable-name}"
  - Calls configure_file with the given 'CONFIG' file and 'OUTPUT' path args, '@ONLY' option is used.

  Sample use case - embed a font or image into a GUI application:
    @code cmake
      ssg_embed_files(
        FILES font/RobotSans/RobotSans.ttf  img/app-logo-128x128.png
        NAMES kDefaultFont kAppLogo128x128
        INPUT EmbeddedData.hpp.in
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Codegen/EmbeddedData.hpp
      )
    @endcode

    The 'CONFIG' input file:
    @code cmake.in
      #pragma once
      namespace cidr::data::embedded {
        static constexpr std::array<unsigned char,@kDefaultFont@> kDefaultFont = {
          @SIZE_OF_kDefaultFont@
        };
        static constexpr std::array<unsigned char,@kAppLogo128x128@> kAppLogo128x128 = {
          @SIZE_OF_kAppLogo128x128@
        };
      }
    @endcode
#]====================================================================================================================]
function(ssg_embed_files)
  set(options)
  set(one_value_args INPUT OUTPUT)
  set(multi_value_args FILES NAMES)
  cmake_parse_arguments(PARSE_ARGV 0 arg "${options}" "${one_value_args}" "${multi_value_args}")
  ssg_check_unparsed_arguments(arg FATAL_ERROR)
  ssg_assert_arguments(
    ARGUMENTS arg_FILES arg_NAMES arg_INPUT arg_OUTPUT
    REQUIRED 
    NOT_EMPTY
    FATAL_ERROR
  )

  list(LENGTH arg_FILES files_count)
  list(LENGTH arg_NAMES names_count)
  if(NOT files_count EQUAL names_count)
    message(FATAL_ERROR "[ssg][ssg_embed_file] File count (${files_count}) must equal name count (${names_count}).")
  endif()

  math(EXPR files_count "${files_count} - 1" OUTPUT_FORMAT DECIMAL)
  foreach(idx RANGE 0 ${files_count})
    list(GET arg_FILES ${idx} this_file)
    list(GET arg_NAMES ${idx} this_binary_name)
    get_filename_component(this_file "${this_file}" ABSOLUTE)   

    if(NOT EXISTS "${this_file}")
      message(FATAL_ERROR "[ssg][ssg_embed_file] File '${this_file}' does not exist.")
    endif()

    file(READ "${this_file}" hex_list HEX)
    file(SIZE "${this_file}" hex_list_size)

    string(REGEX REPLACE "(..)" "0x\\1," hex_list "${hex_list}")
    string(REGEX REPLACE ",$" "" hex_list "${hex_list}")

    set(DATA_OF_${this_binary_name} "${hex_list}")
    set(SIZE_OF_${this_binary_name} "${hex_list_size}")
  endforeach()

  configure_file("${arg_INPUT}" "${arg_OUTPUT}" @ONLY)
endfunction()

#[====================================================================================================================[
Copyright 2025 Anton Yashchenko
Licensed under the GNU Affero General Public License, Version 3.
=======================================================================================================================
@project: [SSGMONO] Monorepo Config Scripts Package
@author(s): Anton Yashchenko
@website: https://www.acpp.dev
#]====================================================================================================================]
