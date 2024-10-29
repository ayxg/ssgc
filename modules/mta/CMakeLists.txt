# MTA
add_library(mta_interface INTERFACE)
target_include_directories(mta_interface INTERFACE inc)

add_library(ssgmod::mta::mta_interface ALIAS mta_interface)