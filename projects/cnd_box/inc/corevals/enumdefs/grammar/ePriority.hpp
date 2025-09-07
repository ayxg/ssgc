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
/// @brief ePriority enum definition.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
// clang-format off
#include "use_ccapi.hpp"
// clang-format on

/// @addtogroup cnd_compiler_corevals
/// @{

namespace cnd::corevals::grammar {

enum class ePriority {
  INVALID = 0,
  NONE = 1,
  Assignment = 20'000,
  LogicalOr = 60'000,
  LogicalAnd = 70'000,
  BitwiseOr = 80'000,
  BitwiseXor = 85'000,
  BitwiseAnd = 90'000,
  Equality = 100'000,
  ThreeWayEquality = 110'000,
  Comparison = 120'000,
  Bitshift = 130'000,
  Term = 140'000,
  Factor = 150'000,
  Prefix = 160'000,
  Postfix = 170'000,
  Functional = 180'000,
  Access = 190'000,
  Max = INT_MAX,
};

constexpr CStr ePriorityToCStr(ePriority e) {
  switch (e) {
    case ePriority::INVALID:
      return "kINVALID";
    case ePriority::NONE:
      return "kNONE";
    case ePriority::Assignment:
      return "kAssignment";
    case ePriority::LogicalOr:
      return "kLogicalOr";
    case ePriority::LogicalAnd:
      return "kLogicalAnd";
    case ePriority::BitwiseOr:
      return "kBitwiseOr";
    case ePriority::BitwiseXor:
      return "kBitwiseXor";
    case ePriority::BitwiseAnd:
      return "kBitwiseAnd";
    case ePriority::Equality:
      return "kEquality";
    case ePriority::ThreeWayEquality:
      return "kThreeWayEquality";
    case ePriority::Comparison:
      return "kComparison";
    case ePriority::Bitshift:
      return "kBitshift";
    case ePriority::Term:
      return "kTerm";
    case ePriority::Factor:
      return "kFactor";
    case ePriority::Prefix:
      return "kPrefix";
    case ePriority::Postfix:
      return "kPostfix";
    case ePriority::Functional:
      return "kFunctional";
    case ePriority::Access:
      return "kAccess";
    case ePriority::Max:
      return "kMax";
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