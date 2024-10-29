add_library(caf_interface INTERFACE)

target_include_directories(caf_interface INTERFACE 
    inc
)

target_sources(caf_interface INTERFACE
  src/caf_imgui_ext.cpp
  src/caf_cache_file.cpp
  src/caf_context.cpp
  src/caf_time.cpp
)
# External alias
add_library(ssgmod::caf::caf_interface ALIAS caf_interface)
