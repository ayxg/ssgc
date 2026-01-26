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
/// @brief Traits of cCst
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
// clang-format off
#include "ccapi/CommonCppApi.hpp"
#include "grammar/eTk.hpp"
#include "grammar/eAssoc.hpp"
#include "grammar/eOperation.hpp"
#include "grammar/ePriority.hpp"
#include "grammar/eAst.hpp"
// clang-format on

/// @addtogroup cnd_compiler_corevals
/// @{

namespace cnd::corevals::grammar {
constexpr eAssoc GetAstAssoc(eAst t) noexcept;
constexpr eOperation GetAstOperation(eAst t) noexcept;
constexpr ePriority GetAstPriority(eAst t) noexcept;
constexpr CStr GetAstSymbol(eAst t) noexcept;
constexpr bool IsAstKeyword(eAst t) noexcept;
constexpr bool IsAstLScope(eAst t) noexcept;
constexpr bool IsAstRScope(eAst t) noexcept;
constexpr bool IsAstRScopeOf(eAst topen, eAst tclose) noexcept;
constexpr bool IsAstModifier(eAst t) noexcept;
constexpr bool IsAstDeclarative(eAst t) noexcept;
constexpr bool IsAstAnOperand(eAst t) noexcept;
constexpr bool IsAstAPrefixOperator(eAst t) noexcept;
constexpr bool IsAstPrimary(eAst t) noexcept;
constexpr bool IsAstPragmatic(eAst t) noexcept;
constexpr eAst GetAstFromTk(eTk t) noexcept;
}  // namespace cnd::corevals::grammar

#include "grammar/traitsof_eAst.tpp"

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