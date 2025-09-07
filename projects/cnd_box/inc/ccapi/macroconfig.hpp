///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the Apache License, Version 2.0(the "License");
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
// Licensed under the Apache License, Version 2.0(the "License"); you may not use this file except in compliance with
// the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on
// an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
