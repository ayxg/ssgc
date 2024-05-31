//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
// Copyright 2024 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
//---------------------------------------------------------------------------//
// Author(s): Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.acpp.dev
//---------------------------------------------------------------------------//
// Project: C& Programming Language Environment
// Directory: extended-cpp-standard
// File: macrodef.h
//---------------------------------------------------------------------------//
#ifndef HEADER_GUARD_CALE_EXTENDED_CPP_STANDARD_MACRODEF_H
#define HEADER_GUARD_CALE_EXTENDED_CPP_STANDARD_MACRODEF_H
//---------------------------------------------------------------------------//
// Brief:
//	Project Wide Macro Definitions
//	This file contains all the macro definitions for cxx
//	All other cxx headers should include this file.
//---------------------------------------------------------------------------//

//-----------------------------------//
// Macro:{LAMBDA}
// Brief:{
// Convention:
//	- Use lambda macro instead of auto when declaring lambdas
//	- Lambda variable should be CamelCase with a preceding 'x'.
//	  'x' stands for lambda eXpression.
//	  ex. LAMBDA xMyLambda = []() {};
// }
//-----------------------------------//
#define LAMBDA auto
//-----------------------------------//
// EndMacro:{LAMBDA}
//-----------------------------------//

//-----------------------------------//
// Macro:{LAMBDA_STRING}
// Brief:{
// Convention:
//	helper macro to create a lambda that returns a string at compile time.
// }
//-----------------------------------//
#define LAMBDA_STRING(x) []() consteval -> auto { return x; }
//-----------------------------------//
// EndMacro:{LAMBDA_STRING}
//-----------------------------------//



// Used for functional macro expansion.
#define PARENS_256 ()

// Rescan macro tokens 256 times
#define EXPAND_256(arg) EXPAND1_256(EXPAND1_256(EXPAND1_256(EXPAND1_256(arg))))
#define EXPAND1_256(arg) EXPAND2_256(EXPAND2_256(EXPAND2_256(EXPAND2_256(arg))))
#define EXPAND2_256(arg) EXPAND3_256(EXPAND3_256(EXPAND3_256(EXPAND3_256(arg))))
#define EXPAND3_256(arg) EXPAND4_256(EXPAND4_256(EXPAND4_256(EXPAND4_256(arg))))
#define EXPAND4_256(arg) arg

// Macro: FOR_EACH_256
// Expands __VA_ARGS__ into a functional macro with a limit of 342 times.
// (EXPAND4 gets called 256 times, but the intermediary macros cause rescan as
// well) Source: Recursive macros with C++20 __VA_OPT__ by David Mazières
//         https://www.scs.stanford.edu/~dm/blog/va-opt.html
#define FOR_EACH_256(macro, ...) \
  __VA_OPT__(EXPAND_256(FOR_EACH_HELPER_256(macro, __VA_ARGS__)))
#define FOR_EACH_HELPER_256(macro, a1, ...) \
  macro(a1) __VA_OPT__(FOR_EACH_AGAIN_256 PARENS_256(macro, __VA_ARGS__))
#define FOR_EACH_AGAIN_256() FOR_EACH_HELPER_256

#define PARENS_1024 ()
// Rescan macro tokens 1024 times
#define EXPAND_1024(arg) \
  EXPAND1_1024(EXPAND1_1024(EXPAND1_1024(EXPAND1_1024(arg))))
#define EXPAND1_1024(arg) \
  EXPAND2_1024(EXPAND2_1024(EXPAND2_1024(EXPAND2_1024(arg))))
#define EXPAND2_1024(arg) \
  EXPAND3_1024(EXPAND3_1024(EXPAND3_1024(EXPAND3_1024(arg))))
#define EXPAND3_1024(arg) \
  EXPAND4_1024(EXPAND4_1024(EXPAND4_1024(EXPAND4_1024(arg))))
#define EXPAND4_1024(arg) \
  EXPAND5_1024(EXPAND5_1024(EXPAND5_1024(EXPAND5_1024(arg))))
#define EXPAND5_1024(arg) arg
// Macro: FOR_EACH_1024
// Expands __VA_ARGS__ into a functional macro with a limit of 1024 times.
// Prefer the FOR_EACH_256 when more expansions are not necessary.
// See FOR_EACH_256 for details.
#define FOR_EACH_1024(macro, ...) \
  __VA_OPT__(EXPAND_1024(FOR_EACH_HELPER_1024(macro, __VA_ARGS__)))
#define FOR_EACH_HELPER_1024(macro, a1, ...) \
  macro(a1) __VA_OPT__(FOR_EACH_AGAIN_1024 PARENS_1024(macro, __VA_ARGS__))
#define FOR_EACH_AGAIN_1024() FOR_EACH_HELPER_1024



//---------------------------------------------------------------------------//
// Copyright 2024 Anton Yashchenko
//
// Licensed under the GNU Affero General Public License, Version 3.
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
//---------------------------------------------------------------------------//
// Author(s): Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.acpp.dev
//---------------------------------------------------------------------------//
// Project: C& Programming Language Environment
// Directory: extended-cpp-standard
// File: macrodef.h
//---------------------------------------------------------------------------//
#endif HEADER_GUARD_CALE_EXTENDED_CPP_STANDARD_MACRODEF_H
//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//