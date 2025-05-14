///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup
/// @brief C& Compiler implicit definitions available to all other sources.
///
/// For now we make these big assumptions:
///   - C++23 Standard Library is available.
///   - Compiler is one of MSVC, GCC, or Clang.
///   - Platform is Windows or Linux.
///   - Processor is x32 or x64.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup cnd_compiler_ccapi
/// @{

#pragma once

// Add C++ Standard Library, and extensions and other 'cruft'.
#include "cxxx.hpp"  // C++Extended Standard Library
#include "mta.hpp"   // Meta Template Archive

// Define conventional macros used throughout the code base.
// @see cnd_ccapi_macroconfig.hpp
#include "macroconfig.hpp"

// Configure C& compiler standard library symbols based on C++ triplet parameters.
// Define common macros ,and typedefs in the cnd namespace.
// @note: When std lib does not provide a required type, cxx implementation is used(if available).
#include "cppconfig.hpp"

// Bring the "Common C++ API" into the global cnd namespace.
namespace cnd {
using ccapi::Bool;
using ccapi::Char;
using ccapi::Int;
using ccapi::Long;
using ccapi::LongLong;
using ccapi::SChar;
using ccapi::Short;
using ccapi::Void;

using ccapi::UChar;
using ccapi::UInt;
using ccapi::ULongLong;
using ccapi::UShort;

using ccapi::Char16;
using ccapi::Char32;
using ccapi::Char8;
using ccapi::WChar;

using ccapi::Double;
using ccapi::Float;
using ccapi::LongDouble;

using ccapi::Byte;
using ccapi::MaxAlign;
using ccapi::Nullptr;
using ccapi::Ptrdiff;
using ccapi::Size;

using ccapi::I16;
using ccapi::I32;
using ccapi::I64;
using ccapi::I8;

using ccapi::UI16;
using ccapi::UI32;
using ccapi::UI64;
using ccapi::UI8;

using ccapi::CStr;
using ccapi::CWStr;
using ccapi::VoidPtr;

using ccapi::CStr;
using ccapi::CWStr;
using ccapi::VoidPtr;

namespace stdfs = ccapi::stdfs;

using ccapi::Path;
using ccapi::Str;
using ccapi::StrView;
using ccapi::WStr;
using ccapi::WStrView;

using ccapi::Ex;
using ccapi::List;
using ccapi::Opt;
using ccapi::Pair;
using ccapi::Unex;
using ccapi::Vec;
using ccapi::Span;
using ccapi::Arr;

using ccapi::SPtr;
using ccapi::UPtr;
using ccapi::WPtr;

using ccapi::SameAs;

using ccapi::cref;
using ccapi::forward;
using ccapi::get;
using ccapi::make_pair;
using ccapi::make_tuple;
using ccapi::make_unique;
using ccapi::move;
using ccapi::ref;

}  // namespace cnd

// Alias common C& compiler development types which are C++ fundamentals or standard library template specializations.
namespace cnd {

/// Source character codepoint integral. Fundamental alias.
/// 
/// Used to represent a single character codepoint in a loaded source code file buffer. 
/// Aliases the unsigned optimal system integral. 
/// Usually an 'unsigned long long' or 'unsigned long' equivalent'. 
/// Size is guaranteed to have at least 32 bits unsigned storage capacity. 
using SrcChar = I32;

}  // namespace cnd

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
