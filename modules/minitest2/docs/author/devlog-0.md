# Devlog 0 : First major update, part A.
Initially created as a quick replacement for GoogleTest. Suprisingly, it has served me well in the past 2
years developing and testing my personal projects. So, I have decided to continue improving this library
rather than abandoning it for GoogleTest.

## Improvements:
  - Added support for C\+\+17 and C\+\+14.
  - Added recursive unit tests which validate the Minitest library itself.
  - `TEST_F` fixture test macro can now optionally forward arguments to an initialization of the fixture.
  - `TEST` and `TEST_F` case definition macros no longer require to be followed by `END_MINITEST`.
  - Improved readme.md to sound less noobish, and more straight to the point.

## New Features:
  - Transition to CMake-based project. Added initial basic 'CMakeLists.txt'.
  - Minitest-CTest integration functions script : 'UseMinitest.cmake'.

## API Changes:
  - `MINITEST` and `MINITEST_F` macros changed to `TEST` and `TEST_F`. Familiarity, more similar to GoogleTest.