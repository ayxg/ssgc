#!/usr/bin/env bash
set -e

run_msvc_build () {
  local preset=$1
  cmd.exe //c call vsdevcmd-build.bat "$preset"
}

run_normal_build () {
  local preset=$1
  cmake --preset "$preset"
  cmake --build out/build/"$preset" --parallel
}

echo "Building 'x64-windows-msvc-debug'"
run_msvc_build x64-windows-msvc-debug

echo "Building 'x64-windows-clangcl-debug'"
run_msvc_build x64-windows-clangcl-debug

echo "Building 'x64-windows-gcc-debug'"
run_normal_build x64-windows-gcc-debug

echo "Building 'x64-windows-clang-debug'"
run_normal_build x64-windows-clang-debug