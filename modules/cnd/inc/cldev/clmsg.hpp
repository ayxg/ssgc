///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the Apache License, Version 2.0(the "License");
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
#include "use_ccapi.hpp"
#include "use_corevals.hpp"
#include "compiler_message_base.hpp"
#include "error_messages.hpp"
#include "warning_messages.hpp"
#include "diagnostic_messages.hpp"
#include "guideline_messages.hpp"
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

using cldev::clmsg::ClMsgBuffer;
using cldev::clmsg::MakeClMsg;
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
// Licensed under the Apache License, Version 2.0(the "License"); you may not use this file except in compliance with
// the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on
// an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////