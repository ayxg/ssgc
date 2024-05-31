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
// File: caoco_token_closure.h
//---------------------------------------------------------------------------//
#ifndef HEADER_GUARD_CALE_CAND_OFFICIAL_COMPILER_CAOCO_TOKEN_CURSOR_H
#define HEADER_GUARD_CALE_CAND_OFFICIAL_COMPILER_CAOCO_TOKEN_CURSOR_H
//---------------------------------------------------------------------------//
// Brief: Iterator-like class with special methods for tokens/ast nodes.
//---------------------------------------------------------------------------//
#include "cppsextended.h"
// Includes:
#include "caoco_grammar.h"
#include "caoco_token.h"
#include "caoco_ast.h"
//---------------------------------------------------------------------------//

namespace caoco {

class TkCursor {
 public:
  // Properties
  constexpr TkVectorConstIter End() const { return end_; }
  constexpr TkVectorConstIter Begin() const { return beg_; }
  constexpr TkVectorConstIter Iter() const { return it_; }
  constexpr const Tk& Get() const;
  constexpr bool AtEnd() const;
  constexpr const Tk& operator->() const { return Get(); }

  // Token Properties
  constexpr inline eTk Type() const noexcept { return Get().Type(); }
  constexpr inline size_t Size() const { return Get().Size(); }
  constexpr inline size_t Line() const noexcept { return Get().Line(); }
  constexpr inline size_t Col() const noexcept { return Get().Col(); }
  constexpr inline const std::string& Literal() const;
  constexpr inline ePriority Priority() const { return Get().Priority(); }
  constexpr inline eAssoc Assoc() const { return Get().Assoc(); }
  constexpr inline eOperation Operation() const { return Get().Operation(); }
  constexpr inline bool TypeIs(eTk type) const noexcept;
  constexpr inline bool TypeIsnt(eTk type) const noexcept;
  constexpr inline bool TypeAndLitIs(eTk kind,
                                     const std::string& literal) const noexcept;
  constexpr inline bool IsKeyword() const noexcept { return Get().IsKeyword(); }
  constexpr inline bool IsModifierKeyword() const noexcept;
  constexpr inline bool IsDeclarativeKeyword() const noexcept;
  constexpr inline bool IsAnOperand() const noexcept;
  constexpr bool IsSingularPrefixOperator() const noexcept;
  constexpr inline bool IsOpeningScope() const noexcept;
  constexpr inline bool IsClosingScope() const noexcept;
  constexpr inline bool IsClosingScopeOf(eTk open) const noexcept;
  constexpr inline bool IsPrimary() const noexcept;
  constexpr inline bool IsPragmatic() const noexcept {
    auto& c = Get();
    return c.IsModifier() || c.IsDeclarative();
  }
  constexpr inline eAst NodeType() const noexcept { return Get().NodeType(); }

  // Iteration
  // advances the cursor by n.
  constexpr TkCursor& Advance(int n = 1);

  // advances the cursor to the new_cursor. Checks that
  // cursor is within beg and end.
  constexpr TkCursor& Advance(TkVectorConstIter new_it);

// Specialized Advance for use in the lark_parser.h.
  // Shortcut for accessing .Always().Iter() from an InternalParseResult.
  // before. c.Advance(result.Always().Iter());
  // use. c.Advance(result);
  constexpr TkCursor& Advance(
      const cxx::PartialExpected<Ast, TkCursor>& result) {
    return Advance(result.Always().Iter());
  }

  // returns cursor advanced by N. N may be negative.
  TkCursor Next(int n = 1) const;
  TkCursor Next(TkVectorConstIter new_it) const;
  // returns the token at the cursor + n.
  constexpr const Tk& Peek(int n = 0) const { return Next(n).Get(); }

  // True there is a match in the iterator's range.
  // Starting from and including the current token.
  constexpr bool FindForward(TkVector match);

  // Returns true if the next n tokens match the
  // match vector. Starting from and including the current token.
  constexpr bool FindForwardExact(TkVectorConstIter cursor, TkVector match);

 public:
  TkCursor() = default;
  TkCursor(TkVectorConstIter begin, TkVectorConstIter end)
      : beg_(begin), end_(end), it_(begin) {}
  TkCursor(TkVectorConstIter begin, TkVectorConstIter end, TkVectorConstIter it)
      : beg_(begin), end_(end), it_(it) {}
  TkCursor(const TkCursor& other)
      : beg_(other.beg_), end_(other.end_), it_(other.it_) {}
  TkCursor& operator=(const TkCursor& other) {
    beg_ = other.beg_;
    end_ = other.end_;
    it_ = other.it_;
    return *this;
  }

 private:
  static const Tk kSentinelEndToken;
  TkVectorConstIter beg_;
  TkVectorConstIter end_;
  TkVectorConstIter it_;
};
const Tk TkCursor::kSentinelEndToken = Tk(eTk::Eofile);

constexpr const Tk& TkCursor::Get() const {
  if (it_ >= end_) {
    return kSentinelEndToken;
  }
  return *it_;
}

constexpr bool TkCursor::AtEnd() const {
  return (it_ == end_) || (it_->TypeIs(eTk::Eofile));
}

// Iteration
// advances the cursor by n.
constexpr TkCursor& TkCursor::Advance(int n) {
  if (n != 0) {
    if (it_ + n >= end_) {
      it_ = end_;
    } else if (it_ + n < beg_) {
      it_ = beg_;
    } else {
      it_ += n;
    }
  }
  return *this;
}

// advances the cursor to the new_cursor. Checks that
// cursor is within beg and end.
constexpr TkCursor& TkCursor::Advance(TkVectorConstIter new_it) {
  if (new_it < beg_) {
    throw std::out_of_range("tk_cursor passed advance_to outside of begin.");
  } else if (new_it > end_) {
    throw std::out_of_range("tk_cursor passed advance_to outside of end.");
  } else
    it_ = new_it;
  return *this;
}



// returns cursor advanced by N. N may be negative.
TkCursor TkCursor::Next(int n) const {
  TkCursor next_cursor = *this;
  next_cursor.Advance(n);
  return next_cursor;
}

TkCursor TkCursor::Next(TkVectorConstIter new_it) const {
  if (new_it < beg_) {
    throw std::out_of_range("tk_cursor passed advance_to outside of begin.");
  } else if (new_it > end_) {
    throw std::out_of_range("tk_cursor passed advance_to outside of end.");
  } else {
    TkCursor rt = *this;
    rt.it_ = new_it;
    return rt;
  }
}

// True there is a match in the iterator's range.
// Starting from and including the current token.
constexpr bool TkCursor::FindForward(TkVector match) {
  auto end = std::next(it_, match.size());
  auto found = std::search(
      it_, end, match.begin(), match.end(),
      [](const Tk& a, const Tk& b) { return a.Type() == b.Type(); });

  if (found != end) {
    return true;
  }
  return false;
}

// Returns true if the next n tokens match the
// match vector. Starting from and including the current token.
constexpr bool TkCursor::FindForwardExact(TkVectorConstIter cursor,
                                          TkVector match) {
  auto end = std::next(cursor, static_cast<std::ptrdiff_t>(match.size()));
  if (std::equal(match.begin(), match.end(), cursor, end)) {
    return true;
  }
  return false;
}

// Token Properties
constexpr inline const std::string& TkCursor::Literal() const {
  return Get().Literal();
}
constexpr inline bool TkCursor::TypeIs(eTk type) const noexcept {
  return Get().TypeIs(type);
}
constexpr inline bool TkCursor::TypeIsnt(eTk type) const noexcept {
  return not(Get().TypeIs(type));
}
constexpr inline bool TkCursor::TypeAndLitIs(eTk kind, const std::string& literal) const noexcept {
  return Get().TypeAndLitIs(kind, literal);
}

constexpr inline bool TkCursor::IsModifierKeyword() const noexcept {
  return Get().IsModifier();
}
constexpr inline bool TkCursor::IsDeclarativeKeyword() const noexcept {
  return Get().IsDeclarative();
}
constexpr inline bool TkCursor::IsAnOperand() const noexcept {
  return Get().IsAnOperand();
}
constexpr bool TkCursor::IsSingularPrefixOperator() const noexcept {
  return Get().IsAPrefixOperator();
}
constexpr inline bool TkCursor::IsOpeningScope() const noexcept {
  return Get().IsLScope();
}
constexpr inline bool TkCursor::IsClosingScope() const noexcept {
  return Get().IsRScope();
}
constexpr inline bool TkCursor::IsClosingScopeOf(eTk open) const noexcept {
  return Get().IsRScopeOf(open);
}

constexpr inline bool TkCursor::IsPrimary() const noexcept {
  return Get().IsPrimary();
}



}  // namespace caoco

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
// File: caoco_token_cursor.h
//---------------------------------------------------------------------------//
#endif HEADER_GUARD_CALE_CAND_OFFICIAL_COMPILER_CAOCO_TOKEN_CURSOR_H
//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//