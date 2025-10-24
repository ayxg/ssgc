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
/// @brief Traits of eClCliFlag
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
// clang-format off
#include "use_ccapi.hpp"
#include "corevals/enumdefs/driver/eDriverFlag.hpp"
#include "corevals/enumdefs/driver/eDriverFlagInterp.hpp"
// clang-format on

/// @addtogroup cnd_compiler_corevals
/// @{

namespace cnd::corevals::driver {
/// Returns the method of syntax interpretation for a flag enum.
constexpr eDriverFlagInterp GetDriverFlagInterp(eDriverFlag flag) noexcept;
/// Returns short form ident of a flag. Returns empty string if unknown flag.
constexpr StrView GetDriverFlagShortSymbol(eDriverFlag flag) noexcept;
/// Returns short form ident of a flag. Returns empty string if unknown flag.
constexpr StrView GetDriverFlagLongSymbol(eDriverFlag flag) noexcept;
/// Returns eDriverFlag of matching long or short form flag with given string. eDriverFlag::kNONE if no match.
constexpr eDriverFlag GetDriverFlagFromStr(StrView str) noexcept;

constexpr eDriverFlag GetDriverFlagFromChar(Char ch) noexcept;
}  // namespace cnd::corevals::driver

#include "corevals/enumtraits/driver/traitsof_eDriverFlag.tpp"

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