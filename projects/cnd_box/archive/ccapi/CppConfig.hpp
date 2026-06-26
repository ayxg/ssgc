///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the Apache License, Version 2.0(the "License")
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup
/// @brief C& Compiler implicit definitions available to all other sources.
///
/// For now we make these big fat assumptions:
///   1. C++23 Standard Library is available.
///   2. Compiler is one of MSVC, GCC, or Clang.
///   3. Platform is Windows or Linux.
///   4. Processor is x32 or x64.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup cnd_compiler_ccapi
/// @{
#pragma once
// clang-format off
#include "mta.hpp"   // Meta Template Archive
#include "cxxx.hpp"  // C++Extended Standard Library
#include "ccapi/MacroConfig.hpp"
// clang-format on

namespace cnd::ccapi {
///////////////////////////////////////////////////////////
// Fundamental types.
///////////////////////////////////////////////////////////
using Void = void;
using Bool = bool;
using Char = char;
using SChar = signed char;
using Short = short;
using Int = int;
using Long = long;
using LongLong = long long;

using UChar = unsigned char;
using UShort = unsigned short;
using UInt = unsigned int;
using ULongLong = unsigned long;

using WChar = wchar_t;
using Char8 = char8_t;
using Char16 = char16_t;
using Char32 = char32_t;

using Float = float;
using Double = double;
using LongDouble = long double;

// C Std Defs
using Size = std::size_t;
using Ptrdiff = std::ptrdiff_t;
using MaxAlign = std::max_align_t;
using Nullptr = std::nullptr_t;
using Byte = std::byte;

// Exact size integrals
using I8 = std::int8_t;
using I16 = std::int16_t;
using I32 = std::int32_t;
using I64 = std::int64_t;

using UI8 = std::uint8_t;
using UI16 = std::uint16_t;
using UI32 = std::uint32_t;
using UI64 = std::uint64_t;

// Syntatic Sugar
using CStr = const char*;
using CWStr = const wchar_t*;
using VoidPtr = void*;

///////////////////////////////////////////////////////////
// Common c++ std lib type aliases.
///////////////////////////////////////////////////////////

// filesystem library namespace
namespace stdfs = std::filesystem;

// Strings
using Str = std::string;
using WStr = std::wstring;
using StrView = std::string_view;
using WStrView = std::wstring_view;
using Path = stdfs::path;

// Containers
template <class T, class AllocT = std::allocator<T>>
using Vec = std::vector<T, AllocT>;

template <class T, std::size_t Extent = std::dynamic_extent>
using Span = std::span<T, Extent>;

template <class T, std::size_t N>
using Arr = std::array<T, N>;

template <class T, class AllocT = std::allocator<T>>
using List = std::list<T, AllocT>;

template <class T, class U>
using Ex = std::expected<T, U>;

template <class T>
using Unex = std::unexpected<T>;

template <class T>
using Opt = std::optional<T>;

template <class... Ts>
using Var = std::variant<Ts...>;

template <class T1, class T2>
using Pair = std::pair<T1, T2>;

template <class... Ts>
using Tup = std::tuple<Ts...>;

// Smart pointers
template <class T, class D = std::default_delete<T>>
using UPtr = std::unique_ptr<T, D>;

template <class T>
using SPtr = std::shared_ptr<T>;

template <class T>
using WPtr = std::weak_ptr<T>;

template <class T>
using Ref = std::reference_wrapper<T>;

template <class T>
using CRef = std::reference_wrapper<std::add_const<T>>;

// Concepts
template <class T, class U>
concept SameAs = std::same_as<T, U>;

// Methods
using std::cref;
using std::forward;
using std::get;
using std::make_pair;
using std::make_tuple;
using std::make_unique;
using std::move;
using std::ref;
}  // namespace cnd::ccapi

/// @} // end of cnd_compiler_ccapi

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