{
  // CMakePresets.json defines a set of presets for configuring and building the project with CMake.
  // To use a preset from the command line, pass the –prefix=prefix-name argument to the cmake command.
  // Any other arguments passed will override conflicting variables in the preset.
  // eg.
  //    $ cmake . --preset=preset-name
  //
  // General format for a preset entry is as follows:
  //  {
  //    "name": "preset-name",
  //    "displayName": "Pretty name for editor GUI",
  //    "description": "Tooltip for editor",
  //    "generator": "Name of build system to generate for, argument to -G flag of cmake command",
  //    "binaryDir": "build directory, can use ${sourceDir} to specify it as relative to project root directory",
  //    "cacheVariables": {
  //      "CACHE_VAR1": "equivalent to -DCACHE_VAR1=",
  //      "CACHE_VAR2": {
  //        "type": "STRING",
  //        "value": "equivalent to -DCACHE_VAR2:STRING="
  //      }
  //    },
  //    "environment": {
  //      "ENV_VAR1": "same format as the cache variable entries"
  //    }
  //  }
  //
  // Presets can also inherit from other presets using the "inherits" field. This is most useful for
  // mapping out a matrix of configurations. The configurations in this preset file are mapped as follows:
  //    - "base" : root preset common for to all configurations.
  // The matrix is then split into 4 dimensions:
  //    - "[arch]-arch" : preset for each supported architecture.
  //    - "[os]" : preset for each supported operating system.
  //    - "[compiler]" : preset for each supported compiler.
  //    - "[build-type]" : preset for each supported build type (Debug, Release)
  //
  // For example: "x64-windows-msvc-debug" will inherit from x64,windows, msvc and debug presets.
  //
  // References:
  // - https://github.com/Chemiseblanc/chemiseblanc.github.io/blob/fc83c22dbac2c2f9a2dedf95440dcfa1bd2ad4d4/content/posts/simplify-your-c-development-environment-using-cmakepresets-json/index.md#L92-L96
  // - https://cmake.org/cmake/help/latest/manual/cmake-presets.7.html
  //
  /////////////////////////////////////////////////////////////////////////////////////////////////
  // Setting up the environment for each compiler on Windows:
  /////////////////////////////////////////////////////////////////////////////////////////////////
  // Im still figuring out the best way, these are the methods I am currently using.
  //
  // Clang-CL:
  //    - Install Visual Studio with the "Desktop development with C++" workload and make sure to include the "Clang tools for windows" component.
  //
  // GCC (MSYS2/UCRT64):
  //    - Install MSYS2 from the official website: https://www.msys2.org
  //    - After installation, open "MSYS2 UCRT64" from the Start menu and install the C and C++ compiler: $ pacman -S mingw-w64-ucrt-x86_64-gcc
  //    - Add the MSYS2 UCRT64 bin directory to your system PATH environment variable(above the visual studio one). The path is typically: C:\msys64\ucrt64\bin
  //
  // Clang (MSYS2/CLANG64):
  //    - Install MSYS2 from the official website: https://www.msys2.org
  //    - After installation, open "MSYS2 CLANG64" from the Start menu and install the C and C++ compiler: $ pacman -S mingw-w64-clang-x86_64-clang
  //    - Add the MSYS2 CLANG64 bin directory to your system PATH environment variable(above the visual studio one). The path is typically: C:\msys64\mingw64\bin
  //    - Make sure you have the clang toolchain file at cmake/toolchains/clang.cmake
  //
  //

  "version": 3,
  "cmakeMinimumRequired": {
    "major": 3,
    "minor": 20,
    "patch": 0
  },
  "configurePresets": [
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Base preset.
    // - Using Ninja as the default generator for all platforms.
    // - Outputting build files to out/build/{presetName} and install files to out/install/{presetName}.
    ///////////////////////////////////////////////////////////////////////////////////////////////
    {
      "name": "base",
      "hidden": true,
      "generator": "Ninja",
      "binaryDir": "${sourceDir}/out/build/${presetName}",
      "installDir": "${sourceDir}/out/install/${presetName}"
    },

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Architechture
    ///////////////////////////////////////////////////////////////////////////////////////////////
    {
      "name": "x86",
      "hidden": true,
      "architecture": {
        "value": "x86",
        "strategy": "external"
      }
    },
    {
      "name": "x64",
      "hidden": true,
      "architecture": {
        "value": "x64",
        "strategy": "external"
      }
    },

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Operating system.
    ///////////////////////////////////////////////////////////////////////////////////////////////
    {
      "name": "windows",
      "hidden": true,
      "condition": {
        "type": "equals",
        "lhs": "${hostSystemName}",
        "rhs": "Windows"
      }
    },
    {
      "name": "linux",
      "hidden": true,
      "condition": {
        "type": "equals",
        "lhs": "${hostSystemName}",
        "rhs": "Linux"
      }
    },
    {
      "name": "macos",
      "hidden": true,
      "condition": {
        "type": "equals",
        "lhs": "${hostSystemName}",
        "rhs": "Darwin"
      }
    },

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Build type.
    ///////////////////////////////////////////////////////////////////////////////////////////////
    {
      "name": "debug",
      "hidden": true,
      "cacheVariables": {
        "CMAKE_BUILD_TYPE": "Debug"
      }
    },
    {
      "name": "release",
      "hidden": true,
      "cacheVariables": {
        "CMAKE_BUILD_TYPE": "Release"
      }
    },


    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Compiler type.
    ///////////////////////////////////////////////////////////////////////////////////////////////
    {
      "name": "msvc",
      "hidden": true,
      "cacheVariables": {
        "CMAKE_C_COMPILER": "cl",
        "CMAKE_CXX_COMPILER": "cl"
      }
    },
    {
      "name": "clangcl",
      "hidden": true,
      "cacheVariables": {
        "CMAKE_C_COMPILER": "clang-cl",
        "CMAKE_CXX_COMPILER": "clang-cl"
      }
    },
    { // Clang for windows edge case.
      "name": "winclang",
      "hidden": true,
      "toolchainFile": "${sourceDir}/cmake/toolchains/clang.cmake"
      // !!Note: can't seem to make this work without using a toolchain file. :(
      // Content of clang.cmake:
      //    set(CMAKE_SYSTEM_NAME Windows)
      //    set(CMAKE_C_COMPILER "clang.exe")
      //    set(CMAKE_CXX_COMPILER "clang++.exe")
      //    set(CMAKE_C_COMPILER_TARGET x86_64-w64-windows-gnu)
      //    set(CMAKE_CXX_COMPILER_TARGET x86_64-w64-windows-gnu)
      //    set(CMAKE_EXE_LINKER_FLAGS "-fuse-ld=lld")
      //    set(CMAKE_SHARED_LINKER_FLAGS "-fuse-ld=lld")
    },
    {
      "name": "clang",
      "hidden": true,
      "cacheVariables": {
        "CMAKE_C_COMPILER": "clang",
        "CMAKE_CXX_COMPILER": "clang++"
      }
    },
    {
      "name": "gcc",
      "hidden": true,
      "cacheVariables": {
        "CMAKE_C_COMPILER": "gcc",
        "CMAKE_CXX_COMPILER": "g++"
      }
    },

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Presets vector.
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Windows
    // MSVC
    {
      "name": "x86-windows-msvc-debug",
      "displayName": "x86-windows-msvc-debug",
      "inherits": [
        "base",
        "x86",
        "windows",
        "msvc",
        "debug"
      ]
    },
    {
      "name": "x86-windows-msvc-release",
      "displayName": "x86-windows-msvc-release",
      "inherits": [
        "base",
        "x86",
        "windows",
        "msvc",
        "release"
      ]
    },
    {
      "name": "x64-windows-msvc-debug",
      "displayName": "x64-windows-msvc-debug",
      "inherits": [
        "base",
        "x64",
        "windows",
        "msvc",
        "debug"
      ]
    },
    {
      "name": "x64-windows-msvc-release",
      "displayName": "x64-windows-msvc-release",
      "inherits": [
        "base",
        "x64",
        "windows",
        "msvc",
        "release"
      ]
    },

    // clang-cl
    {
      "name": "x86-windows-clangcl-debug",
      "displayName": "x86-windows-clangcl-debug",
      "inherits": [
        "base",
        "x86",
        "windows",
        "clangcl",
        "debug"
      ]
    },
    {
      "name": "x86-windows-clangcl-release",
      "displayName": "x86-windows-clangcl-release",
      "inherits": [
        "base",
        "x86",
        "windows",
        "clangcl",
        "release"
      ]
    },
    {
      "name": "x64-windows-clangcl-debug",
      "displayName": "x64-windows-clangcl-debug",
      "inherits": [
        "base",
        "x64",
        "windows",
        "clangcl",
        "debug"
      ]
    },
    {
      "name": "x64-windows-clangcl-release",
      "displayName": "x64-windows-clangcl-release",
      "inherits": [
        "base",
        "x64",
        "windows",
        "clangcl",
        "release"
      ]
    },

    // gcc (MSYS2/UCRT64)
    {
      "name": "x86-windows-gcc-debug",
      "displayName": "x86-windows-gcc-debug",
      "inherits": [
        "base",
        "x86",
        "windows",
        "gcc",
        "debug"
      ]
    },
    {
      "name": "x86-windows-gcc-release",
      "displayName": "x86-windows-gcc-release",
      "inherits": [
        "base",
        "x86",
        "windows",
        "gcc",
        "release"
      ]
    },
    {
      "name": "x64-windows-gcc-debug",
      "displayName": "x64-windows-gcc-debug",
      "inherits": [
        "base",
        "x64",
        "windows",
        "gcc",
        "debug"
      ]
    },
    {
      "name": "x64-windows-gcc-release",
      "displayName": "x64-windows-gcc-release",
      "inherits": [
        "base",
        "x64",
        "windows",
        "gcc",
        "release"
      ]
    },

    // clang (MSYS2/CLANG64)
    {
      "name": "x86-windows-clang-debug",
      "displayName": "x86-windows-clang-debug",
      "inherits": [
        "base",
        "x86",
        "windows",
        "winclang",
        "debug"
      ]
    },
    {
      "name": "x86-windows-clang-release",
      "displayName": "x86-windows-clang-release",
      "inherits": [
        "base",
        "x86",
        "windows",
        "winclang",
        "release"
      ]
    },
    {
      "name": "x64-windows-clang-debug",
      "displayName": "x64-windows-clang-debug",
      "inherits": [
        "base",
        "x64",
        "windows",
        "winclang",
        "debug"
      ]
    },
    {
      "name": "x64-windows-clang-release",
      "displayName": "x64-windows-clang-release",
      "inherits": [
        "base",
        "x64",
        "windows",
        "winclang",
        "release"
      ]
    },

    // Linux
    // gcc (MSYS2/UCRT64)
    {
      "name": "x86-linux-gcc-debug",
      "displayName": "x86-linux-gcc-debug",
      "inherits": [
        "base",
        "x86",
        "linux",
        "gcc",
        "debug"
      ]
    },
    {
      "name": "x86-linux-gcc-release",
      "displayName": "x86-linux-gcc-release",
      "inherits": [
        "base",
        "x86",
        "linux",
        "gcc",
        "release"
      ]
    },
    {
      "name": "x64-linux-gcc-debug",
      "displayName": "x64-linux-gcc-debug",
      "inherits": [
        "base",
        "x64",
        "linux",
        "gcc",
        "debug"
      ]
    },
    {
      "name": "x64-linux-gcc-release",
      "displayName": "x64-linux-gcc-release",
      "inherits": [
        "base",
        "x64",
        "linux",
        "gcc",
        "release"
      ]
    },

    // clang (MSYS2/CLANG64)
    {
      "name": "x86-linux-clang-debug",
      "displayName": "x86-linux-clang-debug",
      "inherits": [
        "base",
        "x86",
        "linux",
        "clang",
        "debug"
      ]
    },
    {
      "name": "x86-linux-clang-release",
      "displayName": "x86-linux-clang-release",
      "inherits": [
        "base",
        "x86",
        "linux",
        "clang",
        "release"
      ]
    },
    {
      "name": "x64-linux-clang-debug",
      "displayName": "x64-linux-clang-debug",
      "inherits": [
        "base",
        "x64",
        "linux",
        "clang",
        "debug"
      ]
    },
    {
      "name": "x64-linux-clang-release",
      "displayName": "x64-linux-clang-release",
      "inherits": [
        "base",
        "x64",
        "linux",
        "clang",
        "release"
      ]
    }
  ]
}
