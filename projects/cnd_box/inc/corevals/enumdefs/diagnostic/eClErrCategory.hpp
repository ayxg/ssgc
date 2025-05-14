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
/// @brief eClErrCategory enum definition.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
// clang-format off
#include "use_ccapi.hpp"
#include "corevals/reflected_meta_enum.hpp"
// clang-format on

/// @addtogroup cnd_compiler_corevals
/// @{


namespace cnd::corevals::diagnostic {
// Define the enum.
enum class eClErrCategory {
  kNone,
  kWarning,
  kCompilerImpl,
  kCommandLine,
  kGeneral,
  kScanner,
  kLexer,
  kParser,
  kPreprocessor,
  kConstEval,
  COUNT
};

// Define the enum to cstr conversion.
constexpr CStr eClErrCategoryToCStr(eClErrCategory e) {
  switch (e) {
    case eClErrCategory::kNone:
      return "kNone";
    case eClErrCategory::kWarning:
      return "kWarning";
    case eClErrCategory::kCompilerImpl:
      return "kCompilerImpl";
    case eClErrCategory::kCommandLine:
      return "kCommandLine";
    case eClErrCategory::kGeneral:
      return "kGeneral";
    case eClErrCategory::kScanner:
      return "kScanner";
    case eClErrCategory::kLexer:
      return "kLexer";
    case eClErrCategory::kParser:
      return "kParser";
    case eClErrCategory::kPreprocessor:
      return "kPreprocessor";
    case eClErrCategory::kConstEval:
      return "kConstEval";
    case eClErrCategory::COUNT:
      return "COUNT";
    default:
      return "<invalid>";
  }
}

}  // namespace cnd::corevals::diagnostic

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