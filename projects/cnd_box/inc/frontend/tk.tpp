///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup cand_compiler_data
/// @brief C& source token structure definition.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup cand_compiler_data
/// @{
#pragma once
// clang-format off
#include "frontend/tk.hpp"
// clang-format off
#include "ccapi/CommonCppApi.hpp"
#include "use_corevals.hpp"
// clang-format on
// clang-format on

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

constexpr Size Tk::BegCol() const noexcept { return end_col_; }

constexpr Size Tk::EndLine() const noexcept { return beg_line_; }

constexpr Size Tk::EndCol() const noexcept { return end_line_; }

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

//constexpr Tk::Tk(eTk type, const SrcLinesConstIter& beg, const SrcLinesConstIter& end) : type_(type) {
//  file_ = beg.File();
//  beg_line_ = beg.Line();
//  beg_col_ = beg.Col();
//  end_line_ = end.Line();
//  end_col_ = end.Col();
//  literal_ = Str(beg.Ptr(), end.Ptr());
//}

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

constexpr bool Tk::operator==(const Tk& rhs) const { return type_ == rhs.type_ && literal_ == rhs.literal_; }

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
// Licensed under the GNU Affero General Public License, Version 3.
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////