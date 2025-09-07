///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the Apache License, Version 2.0(the "License");
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language Environment
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup cand_compiler_data
/// @brief C& source token structure.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup cand_compiler_data
/// @{
#pragma once
// clang-format off
#include "use_ccapi.hpp"
#include "use_corevals.hpp"
// clang-format on

namespace cnd {

/// @brief C& source token structure.
struct Tk {
 public:
  // Modifiers
  constexpr void SetFile(Size file);
  constexpr void SetBegLine(Size line);
  constexpr void SetEndLine(Size line);
  constexpr void SetBegCol(Size col);
  constexpr void SetEndCol(Size col);

  // Data Properties
  constexpr eTk Type() const noexcept;
  constexpr Size Length() const noexcept;
  constexpr Size File() const noexcept;
  constexpr Size BegLine() const noexcept;
  constexpr Size BegCol() const noexcept;
  constexpr Size EndLine() const noexcept;
  constexpr Size EndCol() const noexcept;
  constexpr const StrView& Literal() const noexcept;
  constexpr StrView& LiteralMutable();

  // Parsing Utilities
  constexpr ePriority Priority() const noexcept;
  constexpr eAssoc Assoc() const noexcept;
  constexpr eOperation Operation() const noexcept;
  constexpr StrView TypeStr() const noexcept;
  constexpr Bool TypeIs(eTk type) const noexcept;
  constexpr bool TypeAndLitIs(eTk kind, StrView literal) const noexcept;
  constexpr bool IsKeyword() const noexcept;
  constexpr bool IsModifier() const noexcept;
  constexpr bool IsDeclarative() const noexcept;
  constexpr bool IsAnOperand() const noexcept;
  constexpr bool IsAPrefixOperator() const noexcept;
  constexpr bool IsLScope() const noexcept;
  constexpr bool IsRScope() const noexcept;
  constexpr bool IsRScopeOf(eTk topen) const noexcept;
  constexpr bool IsPrimary() const noexcept;
  constexpr eAst NodeType() const noexcept;

  constexpr Tk() noexcept;
  // constexpr Tk(eTk type, const SrcLinesConstIter& beg, const SrcLinesConstIter& end);
  constexpr Tk(eTk type) noexcept;
  constexpr Tk(const Tk& other) noexcept;
  constexpr Tk(Tk&& other) noexcept;
  constexpr Tk& operator=(const Tk& other) noexcept;
  constexpr Tk& operator=(Tk&& other) noexcept;

  constexpr Tk(eTk type, const StrView& src, Size bline, Size bcol, Size eline, Size ecol) noexcept
      : type_{type},
        file_{0},
        beg_line_{bline},
        end_line_{eline},
        beg_col_{bcol},
        end_col_{ecol},
        literal_{src} {};

  constexpr Tk(eTk type, StrView literal);
  // CND_CX Tk(eTk type, Str literal, Size line, Size col);
  // CND_CX Tk(const Tk& other) noexcept;
  // CND_CX Tk(Tk&& other) noexcept;
  constexpr Bool operator==(const Tk& rhs) const;
  constexpr Bool operator!=(const Tk& rhs) const;

  // public:
  eTk type_{eTk::kNONE};
  Size file_{0};
  Size beg_line_{0};
  Size end_line_{0};
  Size beg_col_{0};
  Size end_col_{0};
  StrView literal_{""};
};

}  // namespace cnd

#include "tk.tpp"

/// @} // end of cand_compiler_data

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