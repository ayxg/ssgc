# Minitest
add_library(minitest_interface INTERFACE)
target_include_directories(minitest_interface INTERFACE inc)

add_library(ssgmod::minitest::minitest_interface ALIAS minitest_interface)


