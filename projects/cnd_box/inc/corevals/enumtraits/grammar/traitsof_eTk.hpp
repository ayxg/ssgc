///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the Apache License, Version 2.0(the "License");
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup cnd_compiler_corevals
/// @brief Traits of eTk.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
// clang-format off
#include "use_ccapi.hpp"
#include "corevals/enumdefs/grammar/eTk.hpp"
#include "corevals/enumdefs/grammar/eAssoc.hpp"
#include "corevals/enumdefs/grammar/eOperation.hpp"
#include "corevals/enumdefs/grammar/ePriority.hpp"
// clang-format on

/// @addtogroup cnd_compiler_corevals
/// @{

namespace cnd::corevals::grammar {
constexpr eAssoc GetTkAssoc(eTk t) noexcept;
constexpr eOperation GetTkOperation(eTk t) noexcept;
constexpr ePriority GetTkPriority(eTk t) noexcept;
constexpr CStr GetTkSymbol(eTk t) noexcept;
constexpr bool IsTkKeyword(eTk t) noexcept;
constexpr bool IsTkLScope(eTk t) noexcept;
constexpr bool IsTkRScope(eTk t) noexcept;
constexpr bool IsTkRScopeOf(eTk topen, eTk tclose) noexcept;
constexpr bool IsTkModifier(eTk t) noexcept;
constexpr bool IsTkDeclarative(eTk t) noexcept;
constexpr bool IsTkAnOperand(eTk t) noexcept;
constexpr bool IsTkAPrefixOperator(eTk t) noexcept;
constexpr bool IsTkPrimary(eTk t) noexcept;
constexpr bool IsTkPragmatic(eTk t) noexcept;
constexpr eTk GetTkFromKeyword(StrView kw) noexcept;
}  // namespace cnd::corevals::grammar

#define CND_FILE_LOCK_COREVALS_TRAITSOF_ETK_HPP
#include "corevals/enumtraits/grammar/traitsof_eTk.tpp"
#undef CND_FILE_LOCK_COREVALS_TRAITSOF_ETK_HPP

/// @} // end of cnd_compiler_corevals

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