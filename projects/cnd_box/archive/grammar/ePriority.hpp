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
/// @brief ePriority enum definition.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
// clang-format off
#include "ccapi/CommonCppApi.hpp"
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