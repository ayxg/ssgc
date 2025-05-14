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
/// @brief Traits of cCst
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
// clang-format off
#include "use_ccapi.hpp"
#include "corevals/enumdefs/grammar/eTk.hpp"
#include "corevals/enumdefs/grammar/eAssoc.hpp"
#include "corevals/enumdefs/grammar/eOperation.hpp"
#include "corevals/enumdefs/grammar/ePriority.hpp"
#include "corevals/enumdefs/grammar/eAst.hpp"
// clang-format on

/// @addtogroup cnd_compiler_corevals
/// @{

namespace cnd::corevals::grammar {
constexpr eAssoc GetAstAssoc(eAst t) noexcept;
constexpr eOperation GetAstOperation(eAst t) noexcept;
constexpr ePriority GetAstPriority(eAst t) noexcept;
constexpr CStr GetAstSymbol(eAst t) noexcept;
constexpr bool IsAstKeyword(eAst t) noexcept;
constexpr bool IsAstLScope(eAst t) noexcept;
constexpr bool IsAstRScope(eAst t) noexcept;
constexpr bool IsAstRScopeOf(eAst topen, eAst tclose) noexcept;
constexpr bool IsAstModifier(eAst t) noexcept;
constexpr bool IsAstDeclarative(eAst t) noexcept;
constexpr bool IsAstAnOperand(eAst t) noexcept;
constexpr bool IsAstAPrefixOperator(eAst t) noexcept;
constexpr bool IsAstPrimary(eAst t) noexcept;
constexpr bool IsAstPragmatic(eAst t) noexcept;
constexpr eAst GetAstFromTk(eTk t) noexcept;
}  // namespace cnd::corevals::grammar

#include "corevals/enumtraits/grammar/traitsof_eAst.tpp"

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