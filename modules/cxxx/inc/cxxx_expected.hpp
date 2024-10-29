///////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the Apache License, Version 2.0(the "License");
///////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language Environment
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup cppmodule2_cppextended
/// @brief Structures for handling expected values and errors.
///////////////////////////////////////////////////////////////////////////////

/// @addtogroup cppmodule2_cppextended
/// @{
#ifndef HEADER_GUARD_EXTENDED_CPP_STANDARD_EXPECTED_H
#define HEADER_GUARD_EXTENDED_CPP_STANDARD_EXPECTED_H
// Cxx C++ Standard Lib
#include "cxxx_import_std.hpp"
// Cxx Macro Definitions
#include "cxxx_macrodef.hpp"

namespace cxx {

/// Represents a boolean true value or an error message.
class BoolError {
 public:
  constexpr bool Valid() const { return valid_; }
  constexpr const std::string& Error() const { return error_; }

  /// Creates a runtime_error exception out of the Error() string.
  inline std::runtime_error Exception() const {
    return std::runtime_error(error_.c_str());
  }

  /// True if object is in a valid state.
  constexpr operator bool() const { return valid_; }

 public:
  BoolError(std::string error_message) : valid_(false), error_(error_message) {}
  BoolError(const char* error_message) : valid_(false), error_(error_message) {}
  BoolError() : valid_(true), error_("") {}
  BoolError(bool value) : valid_(value) {
    if (value)
      error_ = "";
    else
      error_ = "BoolError: Unspecified false error.";
  }

 private:
  /// Is this object in a valid state?
  bool valid_;         
  /// Error message if in error state.
  std::string error_;  
};

/// @brief Represents an expected value or an error message.
/// @tparam T The value type expected to be held in this object.
template <typename T>
class Expected {
 public:
  constexpr bool Valid() const { return expected_.has_value(); }
  constexpr T Extract() {
    T val = std::move(expected_.value());
    expected_.reset();
    error_ = "Value has been moved out of the expected object.";
    return val;
  }
  constexpr const T& Value() const { return expected_.value(); }
  constexpr const auto& Error() const { return error_; }
  constexpr operator bool() const { return expected_.has_value(); }
  static constexpr Expected<T> Success(const T& expected) {
    return Expected<T>(std::forward<const T>(expected));
  }
  static constexpr Expected<T> Success(T&& expected) {
    return Expected<T>(std::forward<T>(expected));
  }
  static constexpr Expected<T> Failure(const std::string& error_message) {
    return Expected<T>(std::nullopt,
                       std::forward<const std::string>(error_message));
  }
  static constexpr Expected<T> Failure(std::string&& error_message) {
    return Expected<T>(std::nullopt, std::forward<std::string>(error_message));
  }

  template <typename U>
  static constexpr Expected<T> NewChainFailure(const Expected<U>& other) {
    auto ret = Expected<T>::Failure(other.Error());
    return ret;
  }

  template <typename U>
  static constexpr Expected<T> NewChainFailure(
      const Expected<U>& other, const std::string& error_message) {
    auto ret = Expected<T>::Failure(other.Error() + "\n" + error_message);
    return ret;
  }

  constexpr Expected<T>&& ChainFailure(std::string error_message) {
    return std::forward<Expected<T>>(
        Expected<T>::NewChainFailure(*this, error_message));
  }

  template <typename U>
  constexpr Expected<T>&& ChainFailure(const Expected<U>& other) {
    return std::forward<Expected<T>>(Expected<T>::NewChainFailure<U>(other));
  }

 private:
  std::optional<T> expected_{std::nullopt};
  std::string error_{""};

  constexpr Expected(T expected) : expected_(expected) {}
  template <typename T>
  constexpr Expected(const T& expected) : expected_(expected) {}

#pragma warning( \
    disable : 4100)  // Disable unused parameter warning for std::nullopt_t
                     // Cannot be instantiated directly from type.
  constexpr Expected(const std::nullopt_t& none, std::string error_message)
      : expected_(std::nullopt), error_(error_message) {}
#pragma warning(default : 4100)
};

/// @brief PartialExpected returns a value or an error message, but also allows
///        for an additional value which will always be passed along with the
///        expected.
/// @tparam T Expected value type.
/// @tparam AlwaysT Value type that is always returned.
template <typename T, typename AlwaysT>
class PartialExpected {
  AlwaysT always_;
  std::optional<T> value_{std::nullopt};
  std::string error_{""};

  template <typename T, typename AlwaysT>
  constexpr PartialExpected(const AlwaysT& always, T expected)
      : always_(always), value_(expected) {}

  template <typename AlwaysT>
  constexpr PartialExpected(const AlwaysT& always) : always_(always) {}

 public:
  constexpr bool Valid() const { return value_.has_value(); }

  constexpr const T& Value() const { return value_.value(); }

  constexpr const AlwaysT& Always() const { return always_; }

  constexpr const std::string& Error() const { return error_; }
  constexpr operator bool() const { return value_.has_value(); }
  constexpr T Extract() {
    T val = std::move(value_.value());
    value_.reset();
    error_ = "Value has been moved out of the expected object.";
    return val;
  }

  constexpr PartialExpected<T, AlwaysT> ChainFailure(
      std::string error_message) {
    return PartialExpected::NewChainFailure(*this, error_message);
  }

  constexpr PartialExpected<T, AlwaysT> ChainFailure() {
    return PartialExpected::NewChainFailure(*this);
  }

  static constexpr PartialExpected<T, AlwaysT> Success(const AlwaysT& always,
                                                       T expected) {
    return PartialExpected(always, expected);
  }

  static constexpr PartialExpected<T, AlwaysT> Failure(
      const AlwaysT& always, const std::string& error_message) {
    auto ret = PartialExpected(always);
    ret.error_ = error_message;
    return ret;
  }

  static constexpr PartialExpected<T, AlwaysT> NewChainFailure(
      const PartialExpected& other, const AlwaysT& new_always,
      const std::string& error_message) {
    auto ret = PartialExpected::Failure(new_always,
                                        other.Error() + "\n" + error_message);
    return ret;
  }

  static constexpr PartialExpected<T, AlwaysT> NewChainFailure(
      const PartialExpected& other, const std::string& error_message) {
    auto ret = PartialExpected::Failure(other.Always(),
                                        other.Error() + "\n" + error_message);
    return ret;
  }

  template <typename U>
  static constexpr PartialExpected<T, AlwaysT> NewChainFailure(
      const Expected<U>& other, const std::string& error_message) {
    auto ret = PartialExpected::Failure(other.Always(),
                                        other.Error() + "\n" + error_message);
    return ret;
  }
};

};  // namespace cxx

#endif HEADER_GUARD_EXTENDED_CPP_STANDARD_EXPECTED_H
/// @} // end of cppmodule2_cppextended
///////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language Environment
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
//
// Licensed under the Apache License, Version 2.0(the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
///////////////////////////////////////////////////////////////////////////////
