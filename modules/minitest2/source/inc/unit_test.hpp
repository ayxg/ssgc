#pragma once
#include "common.hpp"

namespace minitest {
// Aliasing these for now in-case a different impl is provided in the future.
using UnitTestFunction = function<void()>;
using UnitTestLog = vector<string>;

/// Struct modeling a single registered instance of a unit test.
struct UnitTest {
  size_t id{0};
  string suite{""};
  string name{""};
  UnitTestFunction fn{[]() { return; }};
  bool result{true};
  UnitTestLog log{};
};

/// Used to quickly access unit test by suite and name from an analogous
/// unit test reference map.
struct UnitTestSignature {
  string suite{};
  string name{};
  friend bool operator<(const UnitTestSignature& lhs,
                        const UnitTestSignature& rhs) {
    // This seems to be the easiest way to implement a piecewise comparator in C++14.
    return std::make_pair(lhs.suite, lhs.name) <
           std::make_pair(rhs.suite, rhs.name);
  }
};

}  // namespace minitest
