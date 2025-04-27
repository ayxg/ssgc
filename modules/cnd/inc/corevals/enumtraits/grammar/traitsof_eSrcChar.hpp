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
/// @brief Traits of eSrcChar
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
// clang-format off
#include "use_ccapi.hpp"
#include "corevals/enumdefs/grammar/eSrcChar.hpp"
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
// Licensed under the Apache License, Version 2.0(the "License"); you may not use this file except in compliance with
// the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on
// an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////