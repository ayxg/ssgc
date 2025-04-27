#----------------------------------------------------------------------------------------------------------------------#
# Copyright 2024 Anton Yashchenko
# Licensed under the Apache License, Version 2.0(the "License");
#----------------------------------------------------------------------------------------------------------------------#
# @project: C& Programming Language
# @author(s): Anton Yashchenko
# @website: https://www.acpp.dev
#----------------------------------------------------------------------------------------------------------------------#
## @file Minitest library module root CMakeLists.txt
## @ingroup ssg_cmake_modules
## @brief C++14 minimal unit testing framework with CMake integration.
#----------------------------------------------------------------------------------------------------------------------#

## @addtogroup ssg_cmake_modules
## @{
cmake_minimum_required(3.8)
project(MinitestLibraryModule)
if(CMAKE_CXX_STANDARD LESS 14)
  message(FATAL_ERROR 
    "[MinitestLibraryModule] C++14 is required to build Minitest library.")
endif()

include("bsys/cmake/UseMinitest.cmake") # CMake unit testing integration macros

#-----------------------------------------------------------------------------#
# Source Targets
#-----------------------------------------------------------------------------#
add_library(Minitest_Interface INTERFACE)
target_include_directories(Minitest_Interface INTERFACE "source/inc")

#-----------------------------------------------------------------------------#
# Test Targets
#-----------------------------------------------------------------------------#
ADD_MINITEST_EXECUTABLE(
  NAME Minitest_UtChecksAsserts
  INCLUDE "test/ut_checks_asserts" 
)

ADD_MINITEST_SUITE(minitest_ut_checks_asserts "FailedCheckIsDetected")

#-----------------------------------------------------------------------------#
# Exported Targets
#-----------------------------------------------------------------------------#
add_library(MinitestLibrary::MinitestInterface ALIAS MinitestInterface)
add_library(MinitestLibrary::UtChecksAsserts ALIAS minitest_ut_checks_asserts)


## @} end of ssg_cmake_modules