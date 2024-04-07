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