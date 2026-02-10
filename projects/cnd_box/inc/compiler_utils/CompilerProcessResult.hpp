///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup cnd_corevals
/// @brief
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup cnd_compiler_cldev
/// @{
#pragma once
// clang-format off
#include "ccapi/CommonCppApi.hpp"
#include "compiler_utils/CompilerMessageBase.hpp"
#include "diagnostic/error_messages.hpp"
#include "diagnostic/warning_messages.hpp"
#include "diagnostic/diagnostic_messages.hpp"
#include "diagnostic/guideline_messages.hpp"
// clang-format on

namespace cnd {

// Type used by the compiler upon a translation failure. A buffer of dynamically allocated error messages.
using CompilerProcessError = cldev::clmsg::ClMsgBuffer;

// Purpose of this class is to provide an indirection layer from std::expected. Since the old implementation was
// pre-C++23, the C++Extended Library [cxx::Expected] type was used. This allows easier migration of the code to C++23.
template <class T>
struct ExpectedTypeInterface : public Ex<T, CompilerProcessError> {
  using ValueT = T;
  using ErrorT = CompilerProcessError;
  using ImplT = Ex<ValueT, ErrorT>;
  using ImplT::expected;  // use the constructor provided by std::expected.
  using ImplT::operator=;
  using Failure = Unex<ErrorT>;
  using Success = ExpectedTypeInterface;
  constexpr operator ImplT() const { return static_cast<ImplT>(*this); }

  constexpr T&& Extract() { return std::forward<T>(move(this->value())); }
  constexpr CompilerProcessError& Error() noexcept { return this->error(); }
  constexpr T& Value() noexcept { return this->value(); }
  constexpr const CompilerProcessError& Error() const noexcept { return this->error(); }
  constexpr const T& Value() const noexcept { return this->value(); }

};

template<>
struct ExpectedTypeInterface<void> : public Ex<void, CompilerProcessError> {
  using ValueT = void;
  using ErrorT = CompilerProcessError;
  using ImplT = Ex<ValueT, ErrorT>;
  using ImplT::expected;  // use the constructor provided by std::expected.
  using ImplT::operator=;
  using Failure = Unex<ErrorT>;
  using Success = ExpectedTypeInterface;
  constexpr operator ImplT() const { return static_cast<ImplT>(*this); }

  constexpr CompilerProcessError& Error() noexcept { return this->error(); }
  constexpr const CompilerProcessError& Error() const noexcept { return this->error(); }
};

using cldev::clmsg::ClMsgBuffer;
using cldev::clmsg::MakeClMsg;

template <class T>
using ClRes = ExpectedTypeInterface<T>;
using ClFail = Unex<ClMsgBuffer>;

template <class T>
using CompilerProcessResult = ExpectedTypeInterface<T>;
using CompilerProcessFailure = Unex<ClMsgBuffer>;
}  // namespace cnd

/// @} // end of cnd_compiler_cldev

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
//
// This program is free software : you can redistribute it and / or modify it
// under the terms of the GNU Affero General Public License as published by the
// Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////