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
/// @brief Traits of eClDiagnostic
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
// clang-format off
#include "use_ccapi.hpp"
#include "corevals/enumdefs/diagnostic/eClDiagnostic.hpp"
#include "corevals/enumdefs/diagnostic/eClErr.hpp"
#include "corevals/enumdefs/diagnostic/eClWarning.hpp"
#include "corevals/enumdefs/diagnostic/eClGuide.hpp"
#include "corevals/enumdefs/diagnostic/eClMsg.hpp"
#include "corevals/enumdefs/diagnostic/eClMsgType.hpp"
// clang-format on

/// @addtogroup cnd_compiler_corevals
/// @{

namespace cnd::corevals::diagnostic {
constexpr eClMsgType GetClMsgType(eClErr e) noexcept { return eClMsgType::kError; };
constexpr eClMsgType GetClMsgType(eClWarning e) noexcept { return eClMsgType::kWarning; };
constexpr eClMsgType GetClMsgType(eClGuide e) noexcept { return eClMsgType::kGuideline; };
constexpr eClMsgType GetClMsgType(eClDiagnostic e) noexcept { return eClMsgType::kDiagnostic; };
//constexpr StrView GetClDiagnosticSubCategory(eClDiagnostic flag) noexcept;
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