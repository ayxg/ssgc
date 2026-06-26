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
#include <string>
#include <string_view>

#include "frontend_enum_token.hpp"

namespace ssgc::frontend {

/// @brief C& source token structure.
struct Token {
  // public:
  //  // Modifiers
  //  constexpr void SetFile(std::size_t file);
  //  constexpr void SetBegLine(std::size_t line);
  //  constexpr void SetEndLine(std::size_t line);
  //  constexpr void SetBegCol(std::size_t col);
  //  constexpr void SetEndCol(std::size_t col);

  // // Data Properties
  // constexpr eToken Type() const noexcept;
  // constexpr std::size_t Length() const noexcept;
  // constexpr std::size_t File() const noexcept;
  // constexpr std::size_t BegLine() const noexcept;
  // constexpr std::size_t BegCol() const noexcept;
  // constexpr std::size_t EndLine() const noexcept;
  // constexpr std::size_t EndCol() const noexcept;
  // constexpr const StrView& Literal() const noexcept;
  // constexpr StrView& LiteralMutable();

  // // Parsing Utilities
  // constexpr ePriority Priority() const noexcept;
  // constexpr eAssoc Assoc() const noexcept;
  // constexpr eOperation Operation() const noexcept;
  // constexpr StrView TypeStr() const noexcept;
  // constexpr bool TypeIs(eToken type) const noexcept;
  // constexpr bool TypeAndLitIs(eToken kind, StrView literal) const noexcept;
  // constexpr bool IsKeyword() const noexcept;
  // constexpr bool IsModifier() const noexcept;
  // constexpr bool IsDeclarative() const noexcept;
  // constexpr bool IsAnOperand() const noexcept;
  // constexpr bool IsAPrefixOperator() const noexcept;
  // constexpr bool IsLScope() const noexcept;
  // constexpr bool IsRScope() const noexcept;
  // constexpr bool IsRScopeOf(eToken topen) const noexcept;
  // constexpr bool IsPrimary() const noexcept;
  // constexpr eAst NodeType() const noexcept;

  //constexpr Token() noexcept;
  //// constexpr Token(eToken type, const SrcLinesConstIter& beg, const SrcLinesConstIter& end);
  //constexpr Token(eToken type) noexcept;
  //constexpr Token(const Token& other) noexcept;
  //constexpr Token(Token&& other) noexcept;
  //constexpr Token& operator=(const Token& other) noexcept;
  //constexpr Token& operator=(Token&& other) noexcept;

  //constexpr Token(eToken type, std::string_view src, std::size_t bline, std::size_t bcol,
  //                std::size_t eline, std::size_t ecol) noexcept
  //    : type{type}, file{0}, offset{0}, size{0} {};
  //      //beg_line_{bline},
  //      //end_line_{eline},
  //      //beg_col_{bcol},
  //      //end_col_{ecol},
  //      //literal_{src} {};

  //constexpr Token(eToken type, std::string_view literal);
  //// CND_CX Token(eToken type, Str literal, std::size_t line, std::size_t col);
  //// CND_CX Token(const Token& other) noexcept;
  //// CND_CX Token(Token&& other) noexcept;
  //constexpr bool operator==(const Token& rhs) const;
  //constexpr bool operator!=(const Token& rhs) const;

  // public:
  eToken kind{eToken::kNone};
  std::size_t file{0};
  std::size_t offset{0};
  std::size_t size{0};
  //std::size_t beg_line_{0};
  //std::size_t end_line_{0};
  //std::size_t beg_col_{0};
  //std::size_t end_col_{0};
  //std::string literal_{""};
};

}  // namespace ssgc::frontend

namespace ssgc::frontend {
//
// constexpr void Token::SetFile(std::size_t file) { file_ = file; }
//
// constexpr void Token::SetBegLine(std::size_t line) { beg_line_ = line; }
//
// constexpr void Token::SetEndLine(std::size_t line) { beg_line_ = line; }
//
// constexpr void Token::SetBegCol(std::size_t col) { beg_col_ = col; }
//
// constexpr void Token::SetEndCol(std::size_t col) { beg_col_ = col; }
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///* Data Properties */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// constexpr eToken Token::Type() const noexcept { return type; }
//
// constexpr std::size_t Token::Length() const noexcept { return literal_.size(); }
//
// constexpr std::size_t Token::File() const noexcept { return file_; };
//
// constexpr std::size_t Token::BegLine() const noexcept { return beg_col_; }
//
// constexpr std::size_t Token::BegCol() const noexcept { return beg_col_; }
//
// constexpr std::size_t Token::EndLine() const noexcept { return end_line_; }
//
// constexpr std::size_t Token::EndCol() const noexcept { return end_col_; }
//
// constexpr const StrView& Token::Literal() const noexcept { return literal_; }
//
// constexpr StrView& Token::LiteralMutable() { return literal_; }
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///* Parsing Utilities */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// constexpr ePriority Token::Priority() const noexcept { return GetTkPriority(type); }
//
// constexpr eAssoc Token::Assoc() const noexcept { return GetTkAssoc(type); }
//
// constexpr eOperation Token::Operation() const noexcept { return GetTkOperation(type); };
//
// constexpr StrView Token::TypeStr() const noexcept { return eTkToCStr(type); }
//
// constexpr bool Token::TypeIs(eToken type) const noexcept { return type == type; };
//
// constexpr bool Token::TypeAndLitIs(eToken kind, StrView literal) const noexcept {
//  return type == kind && literal_ == literal;
//}
//
// constexpr bool Token::IsKeyword() const noexcept { return IsTkKeyword(type); }
//
// constexpr bool Token::IsModifier() const noexcept { return IsTkModifier(type); }
//
// constexpr bool Token::IsDeclarative() const noexcept { return IsTkDeclarative(type); };
//
// constexpr bool Token::IsAnOperand() const noexcept { return IsTkAnOperand(type); };
//
// constexpr bool Token::IsAPrefixOperator() const noexcept { return IsTkAPrefixOperator(type); };
//
// constexpr bool Token::IsLScope() const noexcept { return IsTkLScope(type); };
//
// constexpr bool Token::IsRScope() const noexcept { return IsTkRScope(type); };
//
// constexpr bool Token::IsRScopeOf(eToken topen) const noexcept { return IsTkRScopeOf(topen,
// type); };
//
// constexpr bool Token::IsPrimary() const noexcept { return IsTkPrimary(type); };
//
// constexpr eAst Token::NodeType() const noexcept { return GetAstFromTk(type); };
//
//constexpr Token::Token() noexcept : type(eToken::kNone) {}

// constexpr Token::Token(eToken type, const std::string_view::const_iterator& beg, const
// SrcLinesConstIter& end)
//    :
// type(type) {
//   file_ = beg.File();
//   beg_line_ = beg.Line();
//   beg_col_ = beg.Col();
//   end_line_ = end.Line();
//   end_col_ = end.Col();
//   literal_ = Str(beg.Ptr(), end.Ptr());
// }

//constexpr Token::Token(eToken type) noexcept : type(type) {}
//
//constexpr Token::Token(eToken type, std::string_view literal) : type(type), literal_{literal} {}
//
//constexpr Token::Token(const Token& other) noexcept
//    : type(other.type),
//      file_(other.file_),
//      beg_line_(other.beg_line_),
//      end_line_(other.end_line_),
//      beg_col_(other.beg_col_),
//      end_col_(other.end_col_),
//      literal_(other.literal_) {}
//
//constexpr Token::Token(Token&& other) noexcept
//    : type(other.type),
//      file_(other.file_),
//      beg_line_(other.beg_line_),
//      end_line_(other.end_line_),
//      beg_col_(other.beg_col_),
//      end_col_(other.end_col_),
//      literal_(std::move(other.literal_)) {}
//
//constexpr Token& Token::operator=(const Token& other) noexcept {
//  type = other.type;
//  file_ = other.file_;
//  beg_line_ = other.beg_line_;
//  end_line_ = other.end_line_;
//  beg_col_ = other.beg_col_;
//  end_col_ = other.end_col_;
//  literal_ = other.literal_;
//  return *this;
//}
//
//constexpr Token& Token::operator=(Token&& other) noexcept {
//  type = other.type;
//  file_ = other.file_;
//  beg_line_ = other.beg_line_;
//  end_line_ = other.end_line_;
//  beg_col_ = other.beg_col_;
//  end_col_ = other.end_col_;
//  literal_ = std::move(other.literal_);
//  return *this;
//}

//constexpr bool Token::operator==(const Token& rhs) const {
//  return type == rhs.type && literal_ == rhs.literal_;
//}

//constexpr bool Token::operator!=(const Token& rhs) const { return !(*this == rhs); }

}  // namespace ssgc::frontend
// #include "tk.tpp"

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