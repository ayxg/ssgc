# Windows Process Library
add_library(wpl_interface INTERFACE)
target_sources(wpl_interface INTERFACE
    src/wpl_core.cpp
)
target_include_directories(wpl_interface INTERFACE inc)

add_library(ssgmod::wpl::wpl_interface ALIAS wpl_interface)