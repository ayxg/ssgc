///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2025 Anton Yashchenko
// Licensed under the Apache License, Version 2.0(the "License");
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: Minitest Framework
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup minitest
/// @brief UnitTest and UnitTestSignature structures.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "common.hpp"

namespace minitest {

/// @addtogroup minitest2_framework_impl
/// @{

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

/// @} // end of minitest2_framework_impl

}  // namespace minitest


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: Minitest Framework
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2025 Anton Yashchenko
//
// Licensed under the Apache License, Version 2.0(the "License"); you may not
// use this file except in compliance with the License. You may obtain a copy of
// the License at http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
// WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
// License for the specific language governing permissions and limitations under
// the License.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////