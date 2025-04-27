#pragma once
#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

namespace minitest {
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
static enable_if_t<iStreamableV<T>, string> OverloadToString(T v) {
  stringstream ss{};
  ss << v;
  return ss.str();
}

template <class T>
static enable_if_t<!iStreamableV<T>, string> OverloadToString(T v) {
  stringstream ss{};
  ss << "[Address][" << &v << "]";
  return ss.str();
}

}  // namespace minitest
