add_library(caoco_interface INTERFACE)
target_include_directories(caoco_interface INTERFACE inc)
add_library(ssgmod::caoco::caoco_interface ALIAS caoco_interface)