# Windows Process Library
add_library(wpl_interface INTERFACE)
target_sources(wpl_interface INTERFACE
    src/wpl_core.cpp
)
target_include_directories(wpl_interface INTERFACE inc)

add_library(ssgmod::wpl::wpl_interface ALIAS wpl_interface)

# Test Group : WplLibraryTests
minitest_add_executable(
  NAME                WplModuleTests
  INCLUDE_DIRECTORIES ut
  LINK_LIBS           ssgmod::wpl::wpl_interface
  TEST_HEADERS        ut_wpl.hpp
)

minitest_from_headers(
  PREFIX UtWpl
  TARGET WplModuleTests
  SCAN_ALL
)