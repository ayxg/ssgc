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
/// @brief Traits of eClCliFlag
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
// clang-format off
#include "use_ccapi.hpp"
#include "corevals/enumdefs/driver/eDriverFlag.hpp"
#include "corevals/enumdefs/driver/eDriverFlagInterp.hpp"
// clang-format on

/// @addtogroup cnd_compiler_corevals
/// @{

namespace cnd::corevals::driver {
/// Returns the method of syntax interpretation for a flag enum.
constexpr eDriverFlagInterp GetDriverFlagInterp(eDriverFlag flag) noexcept;
/// Returns short form ident of a flag. Returns empty string if unknown flag.
constexpr StrView GetDriverFlagShortSymbol(eDriverFlag flag) noexcept;
/// Returns short form ident of a flag. Returns empty string if unknown flag.
constexpr StrView GetDriverFlagLongSymbol(eDriverFlag flag) noexcept;
/// Returns eDriverFlag of matching long or short form flag with given string. eDriverFlag::kNONE if no match.
constexpr eDriverFlag GetDriverFlagFromStr(StrView str) noexcept;

constexpr eDriverFlag GetDriverFlagFromChar(Char ch) noexcept;
}  // namespace cnd::corevals::driver

#include "corevals/enumtraits/driver/traitsof_eDriverFlag.tpp"

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