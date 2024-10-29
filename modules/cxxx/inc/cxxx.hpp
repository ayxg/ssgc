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

#ifndef HEADER_GUARD_SSG_CXXX_CXXX_H
#define HEADER_GUARD_SSG_CXXX_CXXX_H

// External Includes:
#include "mta.hpp"

// Local includes.
// Cxx C++ Standard Lib
#include "cxxx_import_std.hpp"
// Cxx Macro Definitions
#include "cxxx_macrodef.hpp"
// cxx::BoolError, cxx::Expected, cxx::PartialExpected
#include "cxxx_expected.hpp"
/// cxx::EnumeratedFlags
#include "cxxx_enumerated_flags.hpp"
// Filesystem utilities
#include "cxxx_fsys.hpp"

namespace cxx {
constexpr auto AdvanceIt(auto&& iter, size_t i) {
  std::advance(iter, i);
  return iter;
}

constexpr bool StrEq(char const* a, char const* b) {
  return *a == *b && (*a == '\0' || StrEq(a + 1, b + 1));
}

template <class EnumT>
  requires std::is_enum_v<EnumT>
struct UnknownEnumEntry : std::exception {
  using enum_type = EnumT;
  constexpr cstring what() const override { return "Unkown Enum Entry."; }
};

}  // namespace cxx

#endif HEADER_GUARD_SSG_CXXX_CXXX_H
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
