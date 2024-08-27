///////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language Environment
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup cppmodule2_cppextended
/// @brief C++ Extended Standard Library Includes
///////////////////////////////////////////////////////////////////////////////

/// @addtogroup cppmodule2_cppextended
/// @{

#ifndef HEADER_GUARD_CALE_EXTENDED_CPP_STANDARD_CPPSEXTENDED_H
#define HEADER_GUARD_CALE_EXTENDED_CPP_STANDARD_CPPSEXTENDED_H

// External Includes:
#include "mta.h"

// Local includes.
// Cxx C++ Standard Lib
#include "import_std.h"
// Cxx Macro Definitions
#include "macrodef.h"
// cxx::BoolError, cxx::Expected, cxx::PartialExpected
#include "expected.h"
/// cxx::EnumeratedFlags
#include "enumerated_flags.h"

// Maybe Unused...
//#include "cx_ftos.h"
//#include "cx_itos.h"
//#include "sysio.h"
//#include "unique_void_ptr.h"
//#include "hetero_method_map.h"

// Common typedefs from std to bring into global namespace.
using std::size_t;
using std::array;
using std::string;
using std::string_view;
using std::vector;
using std::tuple;

using std::unique_ptr;
using std::shared_ptr;

// Using for common std functions.
using std::make_unique;
using std::make_shared;
using std::make_tuple;
using std::to_string;

// Typedefs for some literal types/pointers.
using cstring = const char*;
using cwstring = const wchar_t*;
using void_ptr = void*;
using const_void_ptr = const void*;

namespace cxx {
template <class EnumT>
  requires std::is_enum_v<EnumT>
struct UnknownEnumEntry : std::exception {
  using enum_type = EnumT;
  constexpr const char* what() const override { return "Unkown Enum Entry."; }
};

}  // namespace cxx


#endif HEADER_GUARD_CALE_EXTENDED_CPP_STANDARD_CPPSEXTENDED_H
/// @} // end of cppmodule2_cppextended

///////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language Environment
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////////////////
