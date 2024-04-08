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
// File: enumerated_flags.h
//---------------------------------------------------------------------------//
#ifndef HEADER_GUARD_CALE_EXTENDED_CPP_STANDARD_ENUMERATED_FLAGS_H
#define HEADER_GUARD_CALE_EXTENDED_CPP_STANDARD_ENUMERATED_FLAGS_H
//---------------------------------------------------------------------------//
// Brief: Enumerated Flags Structure
//---------------------------------------------------------------------------//
// Includes:
#include "mta.h"
//---------------------------------------------------------------------------//

namespace cxx {
//---------------------------------------------------------//
// Template:{EnumeratedFlags}
// Brief:{
//      Represents a set of flags that can be combined using bitwise OR.
//      Flags may be appended or removed. Retrieve the flags using Get().
// }
// Template Parameters:{
//		1. EnumT : An enumeration type which will be used as flags.
//		2. UnderlyingT : Defaults to the underlying type of EnumT.
// }
// Detail:{
//  !!TODO: add example here.
// }
//---------------------------------------------------------//
template <mta::req::iEnumeration EnumT,
          mta::req::iIntegral UnderlyingT = std::underlying_type_t<EnumT>>
class EnumeratedFlags {
 public:
  // Retrieve the flags by value.
  UnderlyingT Get() const { return flags_; }

  // Append a single flag.
  constexpr void Append(EnumT flags) { flags_ |= flags; }

  // Append a list of flags, applied using bitwise OR.
  constexpr void Append(EnumT flags, std::same_as<EnumT> auto... other_flags) {
    (..., (flags_ |= other_flags));
  }

  // Remove a single flag.
  constexpr void Remove(EnumT flags) { flags_ &= ~(flags); }

  // Remove a list of flags, applied using bitwise AND with bitwise NOT.
  constexpr void Remove(EnumT flags, std::same_as<EnumT> auto... other_flags) {
    (..., (flags_ &= ~(flags)));
  }

  // Bitwise OR assignment operator.
  constexpr EnumeratedFlags& operator|=(EnumT flag) {
    flags_ |= flag;
    return *this;
  }

  // Default value is 0.
  EnumeratedFlags() = default;

  // Construct from a single flag.
  EnumeratedFlags(EnumT flags) : flags_(0) { flags_ |= flags; }

  // Construct from a list of flags which will be combined using bitwise OR.
  EnumeratedFlags(EnumT flags, std::same_as<EnumT> auto... other_flags)
      : flags_(flags) {
    (..., (flags_ |= other_flags));
  }

  // Implicit conversion to the enumeration type.
  constexpr operator EnumT() const { return static_cast<EnumT>(flags_); }

  // Implicit conversion to the underlying type.
  constexpr operator UnderlyingT() const { return flags_; }

 private:
  UnderlyingT flags_{0};
};
//---------------------------------------------------------//
// EndTemplate:{EnumeratedFlags}
//---------------------------------------------------------//
};  // namespace cxx

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
// File: enumerated_flags.h
//---------------------------------------------------------------------------//
#endif HEADER_GUARD_CALE_EXTENDED_CPP_STANDARD_ENUMERATED_FLAGS_H
//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
