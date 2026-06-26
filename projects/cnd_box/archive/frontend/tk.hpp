///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
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
#include "ccapi/CommonCppApi.hpp"
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

namespace cnd {

constexpr void Tk::SetFile(Size file) { file_ = file; }

constexpr void Tk::SetBegLine(Size line) { beg_line_ = line; }

constexpr void Tk::SetEndLine(Size line) { beg_line_ = line; }

constexpr void Tk::SetBegCol(Size col) { beg_col_ = col; }

constexpr void Tk::SetEndCol(Size col) { beg_col_ = col; }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Data Properties */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

constexpr eTk Tk::Type() const noexcept { return type_; }

constexpr Size Tk::Length() const noexcept { return literal_.size(); }

constexpr Size Tk::File() const noexcept { return file_; };

constexpr Size Tk::BegLine() const noexcept { return beg_col_; }

constexpr Size Tk::BegCol() const noexcept { return beg_col_; }

constexpr Size Tk::EndLine() const noexcept { return end_line_; }

constexpr Size Tk::EndCol() const noexcept { return end_col_; }

constexpr const StrView& Tk::Literal() const noexcept { return literal_; }

constexpr StrView& Tk::LiteralMutable() { return literal_; }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Parsing Utilities */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

constexpr ePriority Tk::Priority() const noexcept { return GetTkPriority(type_); }

constexpr eAssoc Tk::Assoc() const noexcept { return GetTkAssoc(type_); }

constexpr eOperation Tk::Operation() const noexcept { return GetTkOperation(type_); };

constexpr StrView Tk::TypeStr() const noexcept { return eTkToCStr(type_); }

constexpr bool Tk::TypeIs(eTk type) const noexcept { return type_ == type; };

constexpr bool Tk::TypeAndLitIs(eTk kind, StrView literal) const noexcept {
  return type_ == kind && literal_ == literal;
}

constexpr bool Tk::IsKeyword() const noexcept { return IsTkKeyword(type_); }

constexpr bool Tk::IsModifier() const noexcept { return IsTkModifier(type_); }

constexpr bool Tk::IsDeclarative() const noexcept { return IsTkDeclarative(type_); };

constexpr bool Tk::IsAnOperand() const noexcept { return IsTkAnOperand(type_); };

constexpr bool Tk::IsAPrefixOperator() const noexcept { return IsTkAPrefixOperator(type_); };

constexpr bool Tk::IsLScope() const noexcept { return IsTkLScope(type_); };

constexpr bool Tk::IsRScope() const noexcept { return IsTkRScope(type_); };

constexpr bool Tk::IsRScopeOf(eTk topen) const noexcept { return IsTkRScopeOf(topen, type_); };

constexpr bool Tk::IsPrimary() const noexcept { return IsTkPrimary(type_); };

constexpr eAst Tk::NodeType() const noexcept { return GetAstFromTk(type_); };

constexpr Tk::Tk() noexcept : type_(eTk::kNONE) {}

// constexpr Tk::Tk(eTk type, const SrcLinesConstIter& beg, const SrcLinesConstIter& end) :
// type_(type) {
//   file_ = beg.File();
//   beg_line_ = beg.Line();
//   beg_col_ = beg.Col();
//   end_line_ = end.Line();
//   end_col_ = end.Col();
//   literal_ = Str(beg.Ptr(), end.Ptr());
// }

constexpr Tk::Tk(eTk type) noexcept : type_(type) {}

constexpr Tk::Tk(eTk type, StrView literal) : type_(type), literal_{literal} {}

constexpr Tk::Tk(const Tk& other) noexcept
    : type_(other.type_),
      file_(other.file_),
      beg_line_(other.beg_line_),
      end_line_(other.end_line_),
      beg_col_(other.beg_col_),
      end_col_(other.end_col_),
      literal_(other.literal_) {}

constexpr Tk::Tk(Tk&& other) noexcept
    : type_(other.type_),
      file_(other.file_),
      beg_line_(other.beg_line_),
      end_line_(other.end_line_),
      beg_col_(other.beg_col_),
      end_col_(other.end_col_),
      literal_(std::move(other.literal_)) {}

constexpr Tk& Tk::operator=(const Tk& other) noexcept {
  type_ = other.type_;
  file_ = other.file_;
  beg_line_ = other.beg_line_;
  end_line_ = other.end_line_;
  beg_col_ = other.beg_col_;
  end_col_ = other.end_col_;
  literal_ = other.literal_;
  return *this;
}

constexpr Tk& Tk::operator=(Tk&& other) noexcept {
  type_ = other.type_;
  file_ = other.file_;
  beg_line_ = other.beg_line_;
  end_line_ = other.end_line_;
  beg_col_ = other.beg_col_;
  end_col_ = other.end_col_;
  literal_ = std::move(other.literal_);
  return *this;
}

constexpr bool Tk::operator==(const Tk& rhs) const {
  return type_ == rhs.type_ && literal_ == rhs.literal_;
}

constexpr bool Tk::operator!=(const Tk& rhs) const { return !(*this == rhs); }

}  // namespace cnd
//#include "tk.tpp"

/// @} // end of cand_compiler_data

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