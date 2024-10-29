add_library(cgui_interface INTERFACE)

target_include_directories(cgui_interface INTERFACE 
    inc
)

target_sources(cgui_interface INTERFACE
    src/cgui.cpp
    #src/cgui_imgui_stdlib.cpp
)
# External alias
add_library(ssgmod::cgui::cgui_interface ALIAS cgui_interface)
