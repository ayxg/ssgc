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
/// @brief eOperation enum definition.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

/// @addtogroup cnd_compiler_corevals
/// @{

namespace ssgc::frontend {

enum class eOperation { kInvalid, kNone, kBinary, kPrefix, kPostfix, COUNT };

constexpr const char* eOperationToCStr(eOperation e) {
  switch (e) {
    case eOperation::kInvalid:
      return "kINVALID";
    case eOperation::kNone:
      return "kNONE";
    case eOperation::kBinary:
      return "kBinary";
    case eOperation::kPrefix:
      return "kPrefix";
    case eOperation::kPostfix:
      return "kPostfix";
    case eOperation::COUNT:
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