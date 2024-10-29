# C++ Standard Extended Library
add_library(cxxx_interface INTERFACE)
target_include_directories(cxxx_interface INTERFACE inc)

add_library(ssgmod::cxxx::cxxx_interface ALIAS cxxx_interface)

