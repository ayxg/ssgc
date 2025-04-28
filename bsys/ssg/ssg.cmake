##--------------------------------------------------------------------------------------------------------------------#
## Copyright 2024 Anton Yashchenko
## Licensed under the GNU Affero General Public License, Version 3.
##--------------------------------------------------------------------------------------------------------------------#
## @project: Sophia Silicon Gold Monorepo
## @author(s): Anton Yashchenko
## @website: https://www.acpp.dev
##--------------------------------------------------------------------------------------------------------------------#
## @file SSG CMake Extensions Header
## @ingroup ssg_cmake_extensions
## @brief Common patterns and utilities for cmake projects within the ssg monorepo ecosystem.
##--------------------------------------------------------------------------------------------------------------------#
include_guard()

## @addtogroup ssg_cmake_extensions
## @{

## @function ssg_assert_out_of_source_build
## @brief Asserts source directory is not the same as binary directory.
function(ssg_assert_out_of_source_build)
  # make sure the user doesn't play dirty with symlinks
  get_filename_component(SRCDIR "${CMAKE_SOURCE_DIR}" REALPATH)
  get_filename_component(BINDIR "${CMAKE_BINARY_DIR}" REALPATH)

  if("${SRCDIR}" STREQUAL "${BINDIR}")
    message(FATAL_ERROR "[SSG][FATAL_ERROR] In-source builds are disabled.\n")
  endif()
endfunction()


## @macro ssg_assert_out_of_source_build
## @brief Check if the current project is top-level.
macro(ssg_assert_top_level_project)
  if(NOT PROJECT_IS_TOP_LEVEL)
	  message(FATAL_ERROR 
          "[SSG][FATAL_ERROR] Project is not top-level CMakeLists file.")
  endif()
endmacro()


## @macro ssg_check_sanitizer_support
## @brief Check if undefined behavior/address sanitizer is supported by the C++ compiler.
## @out SSG_CXX_COMPILER_HAS_UBSAN True if undefined behavour sanitizer is supported.
## @out SSG_CXX_COMPILER_HAS_ASAN True if address sanitizer sanitizer is supported.
macro(ssg_check_cxx_sanitizer_support)
  # Currently these are only enabled for clang or gnu, cant msvc do it too ? 
  # TODO: check if msvc has an option.
  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") 
      AND NOT WIN32)
      set(SSG_CXX_COMPILER_HAS_UBSAN ON)
      message(VERBOSE "[SSG][VERBOSE] C++ compiler undefined behavour sanitizer support detected.")
  else()
      set(SSG_CXX_COMPILER_HAS_UBSAN OFF)
  endif()

  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") 
      AND WIN32)
      set(SSG_CXX_COMPILER_HAS_ASAN OFF)
  else()
      set(SSG_CXX_COMPILER_HAS_ASAN ON)
      message(VERBOSE "[SSG][VERBOSE] C++ compiler address sanitizer support detected.")
  endif()
endmacro()


## @macro ssg_check_libfuzzer_support
## @brief Check if fuzz testing is supported by the C++ compiler.
## @inout var_name Stores true in var name if libfuzzer is supported.
function(ssg_check_cxx_libfuzzer_support var_name)
  set(LibFuzzerTestSource
  "
  #include <cstdint>

  extern \"C\" int LLVMFuzzerTestOneInput(const std::uint8_t *data, std::size_t size) {
    return 0;
  }
  ")

  include(CheckCXXSourceCompiles)
  set(CMAKE_REQUIRED_FLAGS "-fsanitize=fuzzer")
  set(CMAKE_REQUIRED_LINK_OPTIONS "-fsanitize=fuzzer")
  check_cxx_source_compiles("${LibFuzzerTestSource}" ${var_name})
endfunction()


## @macro ssg_generate_strict_cxx_compiler_warning_flags
## @brief Generate pendantic warning enabling flags to pass to the C& compiler. 
## @out SSG_PROJECT_WARNINGS_CXX
## @out SSG_PROJECT_WARNINGS_C
## @warning Projects within the ssg ecosystem should be build-able with these flags enabled. 
##          Only diable a specific flag if necessary ,or when building system dependencies.
macro(ssg_generate_cxx_strict_warning_flags)
  set(MSVC_WARNINGS
      /W4 # Baseline reasonable warnings
      /w14242 # 'identifier': conversion from 'type1' to 'type2', possible loss of data
      /w14254 # 'operator': conversion from 'type1:field_bits' to 'type2:field_bits', possible loss of data
      /w14263 # 'function': member function does not override any base class virtual member function
      /w14265 # 'classname': class has virtual functions, but destructor is not virtual instances of this class may not
              # be destructed correctly
      /w14287 # 'operator': unsigned/negative constant mismatch
      /we4289 # nonstandard extension used: 'variable': loop control variable declared in the for-loop is used outside
              # the for-loop scope
      /w14296 # 'operator': expression is always 'boolean_value'
      /w14311 # 'variable': pointer truncation from 'type1' to 'type2'
      /w14545 # expression before comma evaluates to a function which is missing an argument list
      /w14546 # function call before comma missing argument list
      /w14547 # 'operator': operator before comma has no effect; expected operator with side-effect
      /w14549 # 'operator': operator before comma has no effect; did you intend 'operator'?
      /w14555 # expression has no effect; expected expression with side- effect
      /w14619 # pragma warning: there is no warning number 'number'
      /w14640 # Enable warning on thread un-safe static member initialization
      /w14826 # Conversion from 'type1' to 'type2' is sign-extended. This may cause unexpected runtime behavior.
      /w14905 # wide string literal cast to 'LPSTR'
      /w14906 # string literal cast to 'LPWSTR'
      /w14928 # illegal copy-initialization; more than one user-defined conversion has been implicitly applied
      /permissive- # standards conformance mode for MSVC compiler.
  )

  set(CLANG_WARNINGS
      -Wall
      -Wextra # reasonable and standard
      -Wshadow # warn the user if a variable declaration shadows one from a parent context
      -Wnon-virtual-dtor # warn the user if a class with virtual functions has a non-virtual destructor. This helps
      # catch hard to track down memory errors
      -Wold-style-cast # warn for c-style casts
      -Wcast-align # warn for potential performance problem casts
      -Wunused # warn on anything being unused
      -Woverloaded-virtual # warn if you overload (not override) a virtual function
      -Wpedantic # warn if non-standard C++ is used
      -Wconversion # warn on type conversions that may lose data
      -Wsign-conversion # warn on sign conversions
      -Wnull-dereference # warn if a null dereference is detected
      -Wdouble-promotion # warn if float is implicit promoted to double
      -Wformat=2 # warn on security issues around functions that format output (ie printf)
      -Wimplicit-fallthrough # warn on statements that fallthrough without an explicit annotation
  )

  set(GCC_WARNINGS
      ${CLANG_WARNINGS}
      -Wmisleading-indentation # warn if indentation implies blocks where blocks do not exist
      -Wduplicated-cond # warn if if / else chain has duplicated conditions
      -Wduplicated-branches # warn if if / else branches have duplicated code
      -Wlogical-op # warn about logical operations being used where bitwise were probably wanted
      -Wuseless-cast # warn if you perform a cast to the same type
      -Wsuggest-override # warn if an overridden member function is not marked 'override' or 'final'
  )

  list(APPEND CLANG_WARNINGS -Werror)
  list(APPEND GCC_WARNINGS -Werror)
  # list(APPEND MSVC_WARNINGS /WX)

  if(MSVC)
    set(SSG_PROJECT_WARNINGS_CXX ${MSVC_WARNINGS})
    message("[SSG] MSVC default compiler warnings set.")
  elseif(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
    set(SSG_PROJECT_WARNINGS_CXX ${CLANG_WARNINGS})
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(SSG_PROJECT_WARNINGS_CXX ${GCC_WARNINGS})
  else()
    message(AUTHOR_WARNING "No compiler warnings set for CXX compiler: '${CMAKE_CXX_COMPILER_ID}'")
    # TODO support Intel compiler
  endif()

  # use the same warning flags for C
  set(SSG_PROJECT_WARNINGS_C "${SSG_PROJECT_WARNINGS_CXX}")
endmacro()


# Setup project options for internal executables and libraries.
macro(SSG_ROOT_SETUP_PROJECT_OPTIONS)
  include(CMakeDependentOption)
  include(CheckCXXCompilerFlag)
  include(bsys/cmake/LibFuzzer.cmake)

  option(SSG_ROOT_CXX_ENABLE_HARDENING "Enable hardening" ON)
  option(SSG_ROOT_CXX_ENABLE_COVERAGE "Enable coverage reporting" OFF)
  cmake_dependent_option(
      SSG_ROOT_CXX_ENABLE_GLOBAL_HARDENING
      "Attempt to push hardening options to built dependencies"
      ON
      SSG_ROOT_CXX_ENABLE_HARDENING
      OFF)

  # check undefined behavior/address sanitizer support
  ssg_check_cxx_sanitizer_support()

  if(NOT PROJECT_IS_TOP_LEVEL OR SSG_ROOT_CXX_PACKAGING_MAINTAINER_MODE)
      option(SSG_ROOT_CXX_ENABLE_IPO "Enable IPO/LTO" OFF)
      option(SSG_ROOT_CXX_WARNINGS_AS_ERRORS "Treat Warnings As Errors" OFF)
      option(SSG_ROOT_CXX_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
      option(SSG_ROOT_CXX_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" OFF)
      option(SSG_ROOT_CXX_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
      option(SSG_ROOT_CXX_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" OFF)
      option(SSG_ROOT_CXX_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
      option(SSG_ROOT_CXX_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
      option(SSG_ROOT_CXX_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
      option(SSG_ROOT_CXX_ENABLE_CLANG_TIDY "Enable clang-tidy" OFF)
      option(SSG_ROOT_CXX_ENABLE_CPPCHECK "Enable cpp-check analysis" OFF)
      option(SSG_ROOT_CXX_ENABLE_PCH "Enable precompiled headers" OFF)
      option(SSG_ROOT_CXX_ENABLE_CACHE "Enable ccache" OFF)
  else()
      option(SSG_ROOT_CXX_ENABLE_IPO "Enable IPO/LTO" ON)
      option(SSG_ROOT_CXX_WARNINGS_AS_ERRORS "Treat Warnings As Errors" ON)
      option(SSG_ROOT_CXX_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
      option(SSG_ROOT_CXX_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" ${SSG_CXX_COMPILER_HAS_ASAN})
      option(SSG_ROOT_CXX_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
      option(SSG_ROOT_CXX_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" ${SSG_CXX_COMPILER_HAS_UBSAN})
      option(SSG_ROOT_CXX_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
      option(SSG_ROOT_CXX_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
      option(SSG_ROOT_CXX_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
      option(SSG_ROOT_CXX_ENABLE_CLANG_TIDY "Enable clang-tidy" ON)
      option(SSG_ROOT_CXX_ENABLE_CPPCHECK "Enable cpp-check analysis" ON)
      option(SSG_ROOT_CXX_ENABLE_PCH "Enable precompiled headers" OFF)
      option(SSG_ROOT_CXX_ENABLE_CACHE "Enable ccache" ON)
  endif()

  if(NOT PROJECT_IS_TOP_LEVEL)
  mark_as_advanced(
          SSG_ROOT_CXX_ENABLE_IPO
          SSG_ROOT_CXX_WARNINGS_AS_ERRORS
          SSG_ROOT_CXX_ENABLE_USER_LINKER
          SSG_ROOT_CXX_ENABLE_SANITIZER_ADDRESS
          SSG_ROOT_CXX_ENABLE_SANITIZER_LEAK
          SSG_ROOT_CXX_ENABLE_SANITIZER_UNDEFINED
          SSG_ROOT_CXX_ENABLE_SANITIZER_THREAD
          SSG_ROOT_CXX_ENABLE_SANITIZER_MEMORY
          SSG_ROOT_CXX_ENABLE_UNITY_BUILD
          SSG_ROOT_CXX_ENABLE_CLANG_TIDY
          SSG_ROOT_CXX_ENABLE_CPPCHECK
          SSG_ROOT_CXX_ENABLE_COVERAGE
          SSG_ROOT_CXX_ENABLE_PCH
          SSG_ROOT_CXX_ENABLE_CACHE)
  endif()

  ssg_check_cxx_libfuzzer_support(LIBFUZZER_SUPPORTED)
  if(LIBFUZZER_SUPPORTED AND (SSG_ROOT_CXX_ENABLE_SANITIZER_ADDRESS 
      OR SSG_ROOT_CXX_ENABLE_SANITIZER_THREAD 
      OR SSG_ROOT_CXX_ENABLE_SANITIZER_UNDEFINED))
      set(DEFAULT_FUZZER ON)
  else()
      set(DEFAULT_FUZZER OFF)
  endif()

  option(SSG_ROOT_CXX_BUILD_FUZZ_TESTS "Enable fuzz testing executable" ${DEFAULT_FUZZER})
endmacro()


macro(SSG_ROOT_HANDLE_PROJECT_OPTIONS)
  if(SSG_ROOT_CXX_ENABLE_IPO)
    include(CheckIPOSupported)
    check_ipo_supported(RESULT result OUTPUT output)
    if(result)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
    else()
    message(WARNING "IPO is not supported: ${output}")
    endif()
  endif()

  # check undefined behavior/address sanitizer support
  ssg_check_cxx_sanitizer_support()

  if(SSG_ROOT_CXX_ENABLE_HARDENING AND SSG_ROOT_CXX_ENABLE_GLOBAL_HARDENING)
    include(bsys/cmake/Hardening.cmake)
    if(NOT SSG_CXX_COMPILER_HAS_UBSAN 
        OR SSG_ROOT_CXX_ENABLE_SANITIZER_UNDEFINED
        OR SSG_ROOT_CXX_ENABLE_SANITIZER_ADDRESS
        OR SSG_ROOT_CXX_ENABLE_SANITIZER_THREAD
        OR SSG_ROOT_CXX_ENABLE_SANITIZER_LEAK)
        set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
        set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    message("${SSG_ROOT_CXX_ENABLE_HARDENING} ${ENABLE_UBSAN_MINIMAL_RUNTIME} ${SSG_ROOT_CXX_ENABLE_SANITIZER_UNDEFINED}")
    SSG_ROOT_CXX_enable_hardening(SSG_ROOT_CXX_options ON ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()
endmacro()

## @} // end of namespace ssg_cmake_extensions
#---------------------------------------------------------------------------------------------------------------------#
## @project: C& Programming Language
## @author(s): Anton Yashchenko
## @website: https://www.acpp.dev
#---------------------------------------------------------------------------------------------------------------------#
## Copyright 2024 Anton Yashchenko
##
## Licensed under the GNU Affero General Public License, Version 3. you may not use this file except in compliance with
## the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
##
## Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on
## an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the
## specific language governing permissions and limitations under the License.
#---------------------------------------------------------------------------------------------------------------------#