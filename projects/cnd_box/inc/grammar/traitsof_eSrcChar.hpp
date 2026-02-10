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
/// @brief Traits of eSrcChar
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
// clang-format off
#include "ccapi/CommonCppApi.hpp"
#include "grammar/eSrcChar.hpp"
// clang-format on

/// @addtogroup cnd_compiler_corevals
/// @{

namespace cnd::corevals::grammar {

/// The eSrcChar enum or an integral type which can fit within a source code character representation.
/// corevals::grammar::eSrcChar enum, or an integral type sized 32 bits or less.
template <class T>
concept iConvertibleToSrcChar = std::same_as<T, eSrcChar> || (std::integral<T> && sizeof(T) <= 4);

constexpr bool IsSrcCharAlpha(iConvertibleToSrcChar auto c) noexcept;
constexpr bool IsSrcCharNumeric(iConvertibleToSrcChar auto c) noexcept;
constexpr bool IsSrcCharUnderscore(iConvertibleToSrcChar auto c) noexcept;
constexpr bool IsSrcCharAlphanumeric(iConvertibleToSrcChar auto c) noexcept;
constexpr bool IsSrcCharAlphaUnderscore(iConvertibleToSrcChar auto c) noexcept;
constexpr bool IsSrcCharAlnumus(iConvertibleToSrcChar auto c) noexcept;
constexpr bool IsSrcCharPunctuator(iConvertibleToSrcChar auto c) noexcept;
constexpr bool IsSrcCharSpace(iConvertibleToSrcChar auto c) noexcept;
constexpr bool IsSrcCharNewline(iConvertibleToSrcChar auto c) noexcept;
constexpr bool IsSrcCharWhitespace(iConvertibleToSrcChar auto c) noexcept;
constexpr bool IsSrcCharControl(iConvertibleToSrcChar auto c) noexcept;
constexpr bool IsSrcCharValid(iConvertibleToSrcChar auto c) noexcept;
constexpr bool IsSrcCharUpper(iConvertibleToSrcChar auto c) noexcept;
}  // namespace cnd::corevals::grammar

#define CND_FILE_LOCK_COREVALS_TRAITSOF_ESRCCHAR_HPP
#include "traitsof_esrcchar.tpp"
#undef CND_FILE_LOCK_COREVALS_TRAITSOF_ESRCCHAR_HPP

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