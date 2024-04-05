#pragma once
#include "../../submodules/extended-cpp-standard/cppsextended.h"
#include "../../submodules/mini-test/minitest.h"

MINITEST(TestName, TesCasename) {
  // Test code here.
  int i;
}
END_MINITEST;

INLINE_MINITEST(A, B) {
  // Test code here.
  int i;
}
INLINE_END_MINITEST;
void ut_expected() {
  INLINE_MINITEST(A, C) {
    // Test code here.
    int i;
  }
  INLINE_END_MINITEST;

  RUN_INLINE_MINITEST(A, B)

  RUN_INLINE_MINITEST(A, C)
}
