//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
// Copyright 2024 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
//---------------------------------------------------------------------------//
// Author(s): Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.acpp.dev
//---------------------------------------------------------------------------//
// Project: C& Programming Language Environment
// Directory: cand-official-compiler
// File: caoco_token.h
//---------------------------------------------------------------------------//
#ifndef HEADER_GUARD_CALE_CAND_OFFICIAL_COMPILER_CAOCO_TOKEN_H
#define HEADER_GUARD_CALE_CAND_OFFICIAL_COMPILER_CAOCO_TOKEN_H
//---------------------------------------------------------------------------//
// Brief: brief
//---------------------------------------------------------------------------//
#include "cppsextended.h"
// Includes:
#include "caoco_grammar.h"
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
// Namespace:{caoco}
//---------------------------------------------------------------------------//
namespace caoco {
//---------------------------------------------------------------------------//
class Tk;
using TkVector = std::vector<Tk>;
using TkVectorIter = TkVector::iterator;
using TkVectorConstIter = TkVector::const_iterator;

//---------------------------------------------------------//
// Class:{Tk}
// Brief:{
//  C& Source Token.
// }
//---------------------------------------------------------//
class Tk {
 public:
  // Modifiers
  constexpr void SetLine(size_t line) { line_ = line; }
  constexpr void SetCol(size_t col) { col_ = col; }
  constexpr void SetSourceIndexBeg(size_t idx) { source_index_beg_ = idx; }
  constexpr void SetSourceIndexEnd(size_t idx) { source_index_end_ = idx; }
  // Properties
  constexpr eTk Type() const noexcept { return type_; }
  constexpr size_t Size() const noexcept { return literal_.size(); }
  constexpr size_t Line() const noexcept { return line_; }
  constexpr size_t Col() const noexcept { return col_; }
  constexpr size_t SourceIndexBeg() const noexcept { return source_index_beg_; }
  constexpr size_t SourceIndexEnd() const noexcept { return source_index_end_; }
  constexpr const string& Literal() const noexcept { return literal_; }
  constexpr string& LiteralMutable() { return literal_; }
  // Parsing Utilities
  constexpr ePriority Priority() const noexcept { return eTkPriority(type_); };
  constexpr eAssoc Assoc() const noexcept { return eTkAssoc(type_); }
  constexpr eOperation Operation() const noexcept {
    return eTkOperation(type_);
  }
  // Fast type queries.
  constexpr string_view TypeStr() const noexcept { return eTkEnumStr(type_); }
  constexpr bool TypeIs(eTk type) const noexcept { return type_ == type; }
  constexpr bool TypeAndLitIs(eTk kind, const string& literal) const noexcept {
    return type_ == kind && literal_ == literal;
  }
  constexpr bool IsKeyword() const noexcept { return eTkIsKeyword(type_); }
  constexpr bool IsModifier() const noexcept { return eTkIsModifier(type_); }
  constexpr bool IsDeclarative() const noexcept {
    return eTkIsDeclarative(type_);
  };
  constexpr bool IsAnOperand() const noexcept { return eTkIsAnOperand(type_); };
  constexpr bool IsAPrefixOperator() const noexcept {
    return eTkIsAPrefixOperator(type_);
  };
  constexpr bool IsLScope() const noexcept { return eTkIsLScope(type_); };
  constexpr bool IsRScope() const noexcept { return eTkIsRScope(type_); };
  constexpr bool IsRScopeOf(eTk topen) const noexcept {
    return eTkIsRScopeOf(topen, type_);
  };
  constexpr bool IsPrimary() const noexcept { return eTkIsPrimary(type_); };
  constexpr eAst NodeType() const noexcept { return eTkToAstEnum(type_); };

 public:
  constexpr Tk() noexcept : type_(eTk::NONE), line_(0), col_(0), literal_() {}

  // Literal is implicitly set to the pre-defined literal if it exists.
  constexpr Tk(eTk type) noexcept
      : type_(type), line_(0), col_(0), literal_(eTkLiteral(type)) {
  }
  constexpr Tk(eTk type, vector<char>::const_iterator beg,
               vector<char>::const_iterator end) noexcept;
  constexpr Tk(eTk type, vector<char>::const_iterator beg,
               vector<char>::const_iterator end, tuple<size_t, size_t> src_idx);
  constexpr Tk(eTk type, vector<char>::const_iterator beg,
               vector<char>::const_iterator end, size_t line,
               size_t col) noexcept;
  constexpr Tk(eTk type, string literal);
  constexpr Tk(eTk type, string literal, size_t line, size_t col);
  constexpr Tk(eTk type, string literal, tuple<size_t, size_t> src_idx);
  constexpr Tk(const Tk& other) noexcept;
  constexpr Tk(Tk&& other) noexcept;
  constexpr auto operator=(const Tk& other) noexcept;

  constexpr auto operator=(Tk&& other) noexcept;
  constexpr bool operator==(const Tk& rhs) const {
    return type_ == rhs.type_ && literal_ == rhs.literal_;
  };
  constexpr bool operator!=(const Tk& rhs) const { return !(*this == rhs); };

 private:
  eTk type_{eTk::NONE};
  string literal_{""};
  size_t line_{0};
  size_t col_{0};
  size_t source_index_beg_{0};  // Index of start char in the source code.
  size_t source_index_end_{0};  // 1 past the end char in the source code.
};
//---------------------------------------------------------//
// EndClass:{Tk}
//---------------------------------------------------------//

constexpr Tk::Tk(eTk type, vector<char>::const_iterator beg,
                 vector<char>::const_iterator end) noexcept
    : type_(type), line_(0), col_(0) {
  literal_ = string(beg, end);
}
constexpr Tk::Tk(eTk type, vector<char>::const_iterator beg,
                 vector<char>::const_iterator end,
                 tuple<size_t, size_t> src_idx)
    : type_(type),
      line_(),
      col_(),
      source_index_beg_(get<0>(src_idx)),
      source_index_end_(get<1>(src_idx)) {
  literal_ = string(beg, end);
}
constexpr Tk::Tk(eTk type, vector<char>::const_iterator beg,
                 vector<char>::const_iterator end, size_t line,
                 size_t col) noexcept
    : type_(type), line_(line), col_(col) {
  literal_ = string(beg, end);
}

constexpr Tk::Tk(eTk type, string literal)
    : type_(type), line_(0), col_(0), literal_(literal) {}

constexpr Tk::Tk(eTk type, string literal, size_t line, size_t col)
    : type_(type), line_(line), col_(col), literal_(literal) {}

constexpr Tk::Tk(eTk type, string literal, tuple<size_t, size_t> src_idx)
    : type_(type),
      line_(),
      col_(),
      literal_(literal),
      source_index_beg_(get<0>(src_idx)),
      source_index_end_(get<1>(src_idx)) {}

constexpr Tk::Tk(const Tk& other) noexcept
    : type_(other.type_),
      line_(other.line_),
      col_(other.col_),
      literal_(other.literal_),
      source_index_beg_(other.source_index_beg_),
      source_index_end_(other.source_index_end_) {}

constexpr Tk::Tk(Tk&& other) noexcept
    : type_(other.type_),
      line_(other.line_),
      col_(other.col_),
      literal_(std::move(other.literal_)),
      source_index_beg_(other.source_index_beg_),
      source_index_end_(other.source_index_end_) {}

constexpr auto Tk::operator=(const Tk& other) noexcept {
  type_ = other.type_;
  line_ = other.line_;
  col_ = other.col_;
  literal_ = other.literal_;
  source_index_beg_ = other.source_index_beg_;
  source_index_end_ = other.source_index_end_;
  return *this;
}

constexpr auto Tk::operator=(Tk&& other) noexcept {
  type_ = other.type_;
  line_ = other.line_;
  col_ = other.col_;
  literal_ = std::move(other.literal_);
  source_index_beg_ = other.source_index_beg_;
  source_index_end_ = other.source_index_end_;
  return *this;
}
//---------------------------------------------------------------------------//
}  // namespace caoco
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
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
//---------------------------------------------------------------------------//
// Author(s): Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.acpp.dev
//---------------------------------------------------------------------------//
// Project: C& Programming Language Environment
// Directory: cand-official-compiler
// File: caoco_token.h
//---------------------------------------------------------------------------//
#endif HEADER_GUARD_CALE_CAND_OFFICIAL_COMPILER_CAOCO_TOKEN_H
//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
