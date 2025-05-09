#[====================================================================================================================[
Copyright 2025 Anton Yashchenko
Licensed under the GNU Affero General Public License, Version 3.
=======================================================================================================================
@project: [SSGMONO] Monorepo Config Scripts Package
@author(s): Anton Yashchenko
@website: https://www.acpp.dev
=======================================================================================================================
@file SsgGitPkg module
@ingroup ssgmono_box
@brief Macros for downloading, compiling and installing dependencies from github at config time.
#]====================================================================================================================]

include_guard()
include(FetchContent)

## @function ssg_gitpkg_imgui_sfml
## @brief Install sfml-imgui package for this project.
macro(ssg_gitpkg_imgui_sfml)
  # Note: SFML builds as shared by default.
  add_compile_definitions(_HAS_AUTO_PTR_ETC=1) # Allows SFML deprecated std::auto_ptr in C++17
  FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML
    GIT_TAG 69ea0cd863aed1d4092b970b676924a716ff718b # 2.6.1
  )

  FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui
    GIT_TAG 8199457a7d9e453f8d3d9cadc14683fb54a858b5 # 1.91
  )

  FetchContent_Declare(
    imgui-sfml 
    GIT_REPOSITORY https://github.com/SFML/imgui-sfml
    GIT_TAG de565ac8f2b795dedc0307b60830cb006afd2ecd # 2.6
    OVERRIDE_FIND_PACKAGE
  )

  FetchContent_MakeAvailable(SFML imgui)
  set(IMGUI_DIR ${imgui_SOURCE_DIR}) # pass our imgui source dir to imgui-sfml (mandatory)
  set(IMGUI_SFML_FIND_SFML OFF) # turns off FindSFML impl inside imGui-SFML CMakeLists
  set(IMGUI_SFML_IMGUI_DEMO ON) # enables imgui-sfml demo
  FetchContent_MakeAvailable(imgui-sfml)
endmacro()


## @function ssg_gitpkg_imgui_sfml
## @brief Install nlohmanjson package for this project.
macro(ssg_gitpkg_nlohmanjson)
  FetchContent_Declare(nlohmanjson URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
    OVERRIDE_FIND_PACKAGE
  )
  FetchContent_MakeAvailable(nlohmanjson) # nlohmann_json::nlohmann_json
endmacro()


## @function ssg_gitpkg_imgui_sfml
## @brief Install google_test package for this project.
macro(ssg_gitpkg_google_test)
  # gitbash $ git ls-remote --tags https://github.com/google/googletest.git
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest
    GIT_TAG b514bdc898e2951020cbdca1304b75f5950d1f59 # refs/tags/v1.15.2
  )
  # For Windows: Prevent overriding the parent project's compiler/linker settings
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)
endmacro()


#[====================================================================================================================[
Copyright 2025 Anton Yashchenko
Licensed under the GNU Affero General Public License, Version 3.
=======================================================================================================================
@project: [SSGMONO] Monorepo Config Scripts Package
@author(s): Anton Yashchenko
@website: https://www.acpp.dev
#]====================================================================================================================]
