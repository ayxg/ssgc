///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup cnd_compiler_ccapi
/// @brief Convenience and conventional macros for the C& compiler codebase.
/// 
/// Defined macros:
///   CND_CODEBASE_COMPILED_WITH_DEBUG : 1 if compiled with debug mode enabled, 0 otherwise.
///   CND_DEBUG_ASSERT(x) : Calls standard assert if compiled with debug enabled.
///   CND_LAMBDA : Use instead of 'auto' for lambda definitions.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup cnd_compiler_ccapi
/// @{
#pragma once
// clang-format off
#include "cxxx.hpp"  // C++Extended Standard Library
#include "mta.hpp"   // Meta Template Archive

#define CND_CX constexpr
#define CND_NX noexcept

#ifdef _DEBUG
  // Based on the current C++ toolchain's macro indicators: true if the codebase is compiled with debug mode enabled.
  #define CND_CODEBASE_COMPILED_WITH_DEBUG 1
#else
  // Based on the current C++ toolchain's macro indicators: true if the codebase is compiled with debug mode enabled.
  #define CND_CODEBASE_COMPILED_WITH_DEBUG 0
#endif  // _DEBUG

#if CND_CODEBASE_COMPILED_WITH_DEBUG
  // Calls standard assert if compiled with debug enabled.
  #define CND_DEBUG_ASSERT(x) assert(x)
#else
  // Calls standard assert if compiled with debug enabled.
  #define CND_DEBUG_ASSERT
#endif  // CND_DEBUG

// !!convention-0000!! 
// Lambda definitions should be explitly typed as 'CND_LAMBDA' in place of 'auto'.
#define CND_LAMBDA auto


// clang-format on

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
