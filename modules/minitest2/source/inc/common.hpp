///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2025 Anton Yashchenko
// Licensed under the Apache License, Version 2.0(the "License");
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: Minitest Framework
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup minitest3_common
/// @brief Minitest Framework common definitions and type aliases.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
// clang-format off
#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>
// clang-format on

namespace minitest {

/// @addtogroup minitest3_common
/// @{

using std::enable_if_t;
using std::forward;
using std::function;
using std::map;
using std::ostream;
using std::pair;
using std::reference_wrapper;
using std::size_t;
using std::string;
using std::stringstream;
using std::vector;

/// Concept to check if a type is invokable/callable. template <ret,fn,args...>
/// For now temporarily using std::_Is_invocable_r from MSVC.
/// @see iIsInvokableV
template <class R, class F, class... Args>
using iIsInvokable = std::_Is_invocable_r<R, F, Args...>;

template <class R, class F, class... Args>
static constexpr bool iIsInvokableV =
    std::_Is_invocable_r<R, F, Args...>::value;

/// Concept to check if a type is streamable.
/// Non-streamable objects are output as their pointer address.
/// @see iStreamableV
template <typename T>
struct iStreamable {
  static constexpr bool value = std::is_same<
      ostream&,
      // Here we are removing const and reference then re-adding lv-ref to make
      // sure result of stream is always decayed to a reference.
      std::add_lvalue_reference_t<std::remove_const_t<std::remove_reference_t<
          decltype(std::declval<ostream>()
                   << std::declval<std::decay_t<T>>())>>>>::value;
};

template <typename T>
static constexpr bool iStreamableV = iStreamable<int>::value;

// Some basic static validations for iStreamable.
static_assert(iStreamable<int>::value,
              "minitest::iStreamable implementation failure.");

static_assert(iStreamable<string>::value,
              "minitest::iStreamable implementation failure.");

/// Allows user to provide an overload for Minitest's internal to string
/// functionality. Define a stringstream input operator for your type and
/// minitest will apply it when logging errors from that value type.
template <class T>
static enable_if_t<iStreamableV<T>, string> OverloadToString(T&& v) {
  stringstream ss{};
  ss << std::forward<T>(v);
  return ss.str();
} 


template <class T>
static enable_if_t<!iStreamableV<T>, string> OverloadToString(T&& v) {
  stringstream ss{};
  ss << "[Address][" << &v << "]";
  return ss.str();
}

/// @} // end of minitest3_common

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
