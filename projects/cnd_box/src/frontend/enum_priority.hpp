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

/// @addtogroup cnd_compiler_corevals
/// @{

namespace ssgc::frontend {

enum class ePriority {
  kInvalid = 0,
  kNone = 1,
  kAssignment = 20'000,
  kLogicalOr = 60'000,
  kLogicalAnd = 70'000,
  kBitwiseOr = 80'000,
  kBitwiseXor = 85'000,
  kBitwiseAnd = 90'000,
  kEquality = 100'000,
  kThreeWayEquality = 110'000,
  kComparison = 120'000,
  kBitshift = 130'000,
  kTerm = 140'000,
  kFactor = 150'000,
  kPrefix = 160'000,
  kPostfix = 170'000,
  kFunctional = 180'000,
  kAccess = 190'000,
  kMax = 999'999,
  COUNT
};

constexpr const char* ePriorityToCStr(ePriority e) {
  switch (e) {
    case ePriority::kInvalid:
      return "kInvalid";
    case ePriority::kNone:
      return "kNone";
    case ePriority::kAssignment:
      return "kAssignment";
    case ePriority::kLogicalOr:
      return "kLogicalOr";
    case ePriority::kLogicalAnd:
      return "kLogicalAnd";
    case ePriority::kBitwiseOr:
      return "kBitwiseOr";
    case ePriority::kBitwiseXor:
      return "kBitwiseXor";
    case ePriority::kBitwiseAnd:
      return "kBitwiseAnd";
    case ePriority::kEquality:
      return "kEquality";
    case ePriority::kThreeWayEquality:
      return "kThreeWayEquality";
    case ePriority::kComparison:
      return "kComparison";
    case ePriority::kBitshift:
      return "kBitshift";
    case ePriority::kTerm:
      return "kTerm";
    case ePriority::kFactor:
      return "kFactor";
    case ePriority::kPrefix:
      return "kPrefix";
    case ePriority::kPostfix:
      return "kPostfix";
    case ePriority::kFunctional:
      return "kFunctional";
    case ePriority::kAccess:
      return "kAccess";
    case ePriority::kMax:
      return "kMax";
    case ePriority::COUNT:
      return "COUNT";
    default:
      return "<invalid>";
  }
}
}  // namespace ssgc::frontend

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