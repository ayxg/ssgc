///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the Apache License, Version 2.0(the "License");
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup cnd_corevals
/// @brief Compiler Diagnostic Message Base Implementation Header
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup cnd_compiler_cldev
/// @{
#pragma once
#include "compiler_message_base.hpp"
// clang-format off
#include "use_ccapi.hpp"
#include "use_corevals.hpp"
// clang-format on

namespace cnd {
namespace cldev {
namespace clmsg {

using std::get;                // For ClMsgDataUnionT accessing std::variant.
using std::holds_alternative;  // For ClMsgDataUnionT accessing std::variant.
using std::to_underlying;      // To get the diagnostic enum's underlying value.

/////////////////////////////////////////////////////////
/* Implement ClMsgChain, ClMsgUnion, and ClMsg classes */
/////////////////////////////////////////////////////////


}  // namespace clmsg
}  // namespace cldev

// template <class T>
// using CompilerProcessResult = Ex<T, CompilerMessageBuffer>;
// using CompilerProcessFailure = CompilerProcessResult<CompilerMessageBuffer>;

}  // namespace cnd

/// @} // end of cnd_compiler_cldev

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