///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
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
#include "corevals/grammar/eTk.hpp"
#include "corevals/grammar/eAssoc.hpp"
#include "corevals/grammar/eOperation.hpp"
#include "corevals/grammar/ePriority.hpp"
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
#include "corevals/grammar/traitsof_eTk.tpp"
#undef CND_FILE_LOCK_COREVALS_TRAITSOF_ETK_HPP

/// @} // end of cnd_compiler_corevals

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