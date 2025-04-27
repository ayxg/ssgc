///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the Apache License, Version 2.0(the "License");
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
// Licensed under the Apache License, Version 2.0(the "License"); you may not use this file except in compliance with
// the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on
// an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////