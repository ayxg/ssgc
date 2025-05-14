///////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the Apache License, Version 2.0(the "License");
///////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language Environment
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup cppmodule2_cppextended
/// @brief Project Wide Macro Definitions
/// 
/// This file contains all the macro definitions for cxx.
/// All other cxx headers should include this file.
///////////////////////////////////////////////////////////////////////////////

/// @addtogroup cppmodule2_cppextended
/// @{
#ifndef HEADER_GUARD_CALE_EXTENDED_CPP_STANDARD_MACRODEF_H
#define HEADER_GUARD_CALE_EXTENDED_CPP_STANDARD_MACRODEF_H

/// @def LAMBDA
/// 
/// Convention:
///	- Use lambda macro instead of auto when declaring lambdas
///	- Lambda variable should be CamelCase with a preceding 'x'.
///	  'x' stands for lambda eXpression.
///	  ex. LAMBDA xMyLambda = []() {};
#define LAMBDA auto

/// @def LAMBDA_STRING
/// @brief helper macro to create a lambda that returns a string at compile time.
#define LAMBDA_STRING(x) []() consteval -> auto { return x; }

///// Used for functional macro expansion.
//#define PARENS_256 ()
//
///// Rescan macro tokens 256 times
//#define EXPAND_256(arg) EXPAND1_256(EXPAND1_256(EXPAND1_256(EXPAND1_256(arg))))
//#define EXPAND1_256(arg) EXPAND2_256(EXPAND2_256(EXPAND2_256(EXPAND2_256(arg))))
//#define EXPAND2_256(arg) EXPAND3_256(EXPAND3_256(EXPAND3_256(EXPAND3_256(arg))))
//#define EXPAND3_256(arg) EXPAND4_256(EXPAND4_256(EXPAND4_256(EXPAND4_256(arg))))
//#define EXPAND4_256(arg) arg
//
///// @def FOR_EACH_256(macro, ...)
///// 
///// Expands __VA_ARGS__ into a functional macro with a limit of 342 times.
///// (EXPAND4 gets called 256 times, but the intermediary macros cause rescan as
///// well) Source: Recursive macros with C++20 __VA_OPT__ by David Mazières
/////         https://www.scs.stanford.edu/~dm/blog/va-opt.html
//#define FOR_EACH_256(macro, ...) \
//  __VA_OPT__(EXPAND_256(FOR_EACH_HELPER_256(macro, __VA_ARGS__)))
//#define FOR_EACH_HELPER_256(macro, a1, ...) \
//  macro(a1) __VA_OPT__(FOR_EACH_AGAIN_256 PARENS_256(macro, __VA_ARGS__))
//#define FOR_EACH_AGAIN_256() FOR_EACH_HELPER_256
//
//#define PARENS_1024 ()
//// Rescan macro tokens 1024 times
//#define EXPAND_1024(arg) \
//  EXPAND1_1024(EXPAND1_1024(EXPAND1_1024(EXPAND1_1024(arg))))
//#define EXPAND1_1024(arg) \
//  EXPAND2_1024(EXPAND2_1024(EXPAND2_1024(EXPAND2_1024(arg))))
//#define EXPAND2_1024(arg) \
//  EXPAND3_1024(EXPAND3_1024(EXPAND3_1024(EXPAND3_1024(arg))))
//#define EXPAND3_1024(arg) \
//  EXPAND4_1024(EXPAND4_1024(EXPAND4_1024(EXPAND4_1024(arg))))
//#define EXPAND4_1024(arg) \
//  EXPAND5_1024(EXPAND5_1024(EXPAND5_1024(EXPAND5_1024(arg))))
//#define EXPAND5_1024(arg) arg
//
///// @def FOR_EACH_1024(macro, ...)
///// @see FOR_EACH_256(macro, ...) for details.
///// 
///// Expands __VA_ARGS__ into a functional macro with a limit of 1024 times.
///// Prefer the FOR_EACH_256 when more expansions are not necessary.
//#define FOR_EACH_1024(macro, ...) \
//  __VA_OPT__(EXPAND_1024(FOR_EACH_HELPER_1024(macro, __VA_ARGS__)))
//#define FOR_EACH_HELPER_1024(macro, a1, ...) \
//  macro(a1) __VA_OPT__(FOR_EACH_AGAIN_1024 PARENS_1024(macro, __VA_ARGS__))
//#define FOR_EACH_AGAIN_1024() FOR_EACH_HELPER_1024

#endif HEADER_GUARD_CALE_EXTENDED_CPP_STANDARD_MACRODEF_H
/// @} // end of cppmodule2_cppextended
///////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language Environment
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
//
// Licensed under the Apache License, Version 2.0(the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
///////////////////////////////////////////////////////////////////////////////
