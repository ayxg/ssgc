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
/// @brief eOperation enum definition.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
// clang-format off
#include "use_ccapi.hpp"
// clang-format on

/// @addtogroup cnd_compiler_corevals
/// @{

namespace cnd::corevals::grammar {

enum class eOperation { INVALID, NONE, Binary, Prefix, Postfix, COUNT };

constexpr CStr eOperationToCStr(eOperation e) {
  switch (e) {
    case eOperation::INVALID:
      return "kINVALID";
    case eOperation::NONE:
      return "kNONE";
    case eOperation::Binary:
      return "kBinary";
    case eOperation::Prefix:
      return "kPrefix";
    case eOperation::Postfix:
      return "kPostfix";
    case eOperation::COUNT:
      return "kCOUNT";
    default:
      return "<invalid>";
  }
}
}  // namespace cnd::corevals::grammar

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