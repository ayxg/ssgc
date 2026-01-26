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
/// @brief eClGuideCategory enum definition.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
// clang-format off
#include "ccapi/CommonCppApi.hpp"
#include "compiler_utils/ReflectedMetaEnum.hpp"
// clang-format on

/// @addtogroup cnd_compiler_corevals
/// @{

namespace cnd::corevals::diagnostic {
// Define the enum.
enum class eClGuideCategory {
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
constexpr CStr eClGuideCategoryToCStr(eClGuideCategory e) {
  using enum eClGuideCategory;
  switch (e) {
    case kNone:
      return "kNone";
    case kWarning:
      return "kWarning";
    case kCompilerImpl:
      return "kCompilerImpl";
    case kCommandLine:
      return "kCommandLine";
    case kGeneral:
      return "kGeneral";
    case kScanner:
      return "kScanner";
    case kLexer:
      return "kLexer";
    case kParser:
      return "kParser";
    case kPreprocessor:
      return "kPreprocessor";
    case kConstEval:
      return "kConstEval";
    case COUNT:
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