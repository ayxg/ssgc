# C++ Standard Extended Library
add_library(cxxx_interface INTERFACE)
target_include_directories(cxxx_interface INTERFACE inc)

add_library(ssgmod::cxxx::cxxx_interface ALIAS cxxx_interface)

# Test Group : CxxxModuleTests
minitest_add_executable(
  NAME                CxxxModuleTests
  INCLUDE_DIRECTORIES ut
  LINK_LIBS           ssgmod::mta::mta_interface 
                      ssgmod::cxxx::cxxx_interface
  TEST_HEADERS        ut_expected.h
)

minitest_from_headers(
  PREFIX UtCxxx
  TARGET CxxxModuleTests
  SCAN_ALL
)