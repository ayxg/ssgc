///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup cnd_compiler_corevals
/// @brief eClCliFlagInterp enum definition.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
// clang-format off
#include "use_ccapi.hpp"
#include "corevals/reflected_meta_enum.hpp"
// clang-format on

/// @addtogroup cnd_compiler_corevals
/// @{

namespace cnd::corevals::driver {
enum class eDriverFlagInterp {kNONE, kOnOff, kModeSwitch, kVarDef, kSingleArg, kTwoArg, kMultiArg,kBoolean, COUNT };

constexpr CStr eDriverFlagInterpToCStr(eDriverFlagInterp e) noexcept {
  using enum eDriverFlagInterp;
  switch (e) {
    case kNONE:
      return "kNONE";
    case kOnOff:
      return "kOnOff";
    case kModeSwitch:
      return "kModeSwitch";
    case kVarDef:
      return "kVarDef";
    case kSingleArg:
      return "kSingleArg";
    case kTwoArg:
      return "kTwoArg";
    case kMultiArg:
      return "kMultiArg";
    case kBoolean:
      return "kBoolean";
    case COUNT:
      return "COUNT";
    default:
      return "<invalid>";
  }
}

}  // namespace cnd::corevals::driver

/// @} // end of cnd_compiler_corevals

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