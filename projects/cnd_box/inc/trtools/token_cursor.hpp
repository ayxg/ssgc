///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup
/// @brief
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup cnd_compiler_data
/// @{
#pragma once
// clang-format off
#include "use_ccapi.hpp"
#include "use_corevals.hpp"
#include "use_clmsg.hpp"
#include "cldata/tk.hpp"
#include "cldata/ast.hpp"
// clang-format on

namespace cnd {
template <template <class T> class TokenContainerT>
class TkCursor {
 public:
  using TkVecT = TokenContainerT<const Tk>;
  using TkVecConstIterT = TokenContainerT<const Tk>::const_iterator;
  /*
    // Properties
    constexpr TkVecConstIterT End() const;
    constexpr TkVecConstIterT Begin() const;
    constexpr TkVecConstIterT Iter() const;
    constexpr const Tk& Get() const;
    constexpr Bool AtEnd() const;
    constexpr const Tk& operator->() const;

    // Token Properties
    constexpr eTk Type() const noexcept;
    constexpr Size Length() const;
    constexpr Size Line() const noexcept;
    constexpr Size Col() const noexcept;
    constexpr StrView Literal() const;
    constexpr ePriority Priority() const;
    constexpr eAssoc Assoc() const;
    constexpr eOperation Operation() const;
    constexpr Bool TypeIs(eTk type) const noexcept;
    constexpr Bool TypeIsnt(eTk type) const noexcept;
    constexpr Bool TypeAndLitIs(eTk kind, const Str& literal) const noexcept;
    constexpr Bool IsKeyword() const noexcept;
    constexpr Bool IsModifierKeyword() const noexcept;
    constexpr Bool IsDeclarativeKeyword() const noexcept;
    constexpr Bool IsAnOperand() const noexcept;
    constexpr Bool IsSingularPrefixOperator() const noexcept;
    constexpr Bool IsOpeningScope() const noexcept;
    constexpr Bool IsClosingScope() const noexcept;
    constexpr Bool IsClosingScopeOf(eTk open) const noexcept;
    constexpr Bool IsPrimary() const noexcept;
    constexpr Bool IsPragmatic() const noexcept;
    constexpr eAst NodeType() const noexcept;

    // Iteration
    constexpr TkCursor& Advance();
    constexpr TkCursor& Advance(int n);
    constexpr TkCursor& Advance(TkVecConstIterT new_it);
    constexpr TkCursor& Advance(const TkCursor& to);
    constexpr TkCursor Next(int n) const;
    constexpr TkCursor Next(TkVecConstIterT new_it) const;
    constexpr const Tk& Peek(int n = 1) const;
    constexpr Bool FindForward(const TkVecT& match);
    constexpr Bool FindForwardExact(TkVecConstIterT cursor, const TkVecT& match);

   public:
    constexpr TkCursor(TkVecConstIterT begin, TkVecConstIterT end);
    constexpr TkCursor(TkVecConstIterT begin, TkVecConstIterT end, TkVecConstIterT it);
    constexpr TkCursor(const TkCursor& other);
    constexpr TkCursor& operator=(const TkCursor& other);
  */
  constexpr TkCursor() = default;

 private:
  static constexpr Tk kSentinelEndToken{eTk::kEofile};
  TkVecConstIterT beg_;
  TkVecConstIterT end_;
  TkVecConstIterT it_;

 public:
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /* Properties */
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  constexpr TkVecConstIterT End() const { return end_; }

  constexpr TkVecConstIterT Begin() const { return beg_; }

  constexpr TkVecConstIterT Iter() const { return it_; }

  constexpr const Tk& Get() const {
    if (it_ >= end_) {
      return kSentinelEndToken;
    }
    return *it_;
  }

  constexpr Bool AtEnd() const { return (it_ == end_) || (it_->TypeIs(eTk::kEofile)); }

  constexpr const Tk& operator->() const { return Get(); }

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /* Token Properties */
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  constexpr eTk Type() const noexcept { return Get().Type(); }

  constexpr Size Length() const { return Get().Length(); }

  constexpr StrView Literal() const { return Get().Literal(); }

  constexpr ePriority Priority() const { return Get().Priority(); }

  constexpr eAssoc Assoc() const { return Get().Assoc(); }

  constexpr eOperation Operation() const { return Get().Operation(); }

  constexpr Bool TypeIs(eTk type) const noexcept { return Get().TypeIs(type); }

  constexpr Bool TypeIsnt(eTk type) const noexcept { return !(Get().TypeIs(type)); }

  constexpr Bool TypeAndLitIs(eTk kind, const Str& literal) const noexcept { return Get().TypeAndLitIs(kind, literal); }

  constexpr Bool IsKeyword() const noexcept { return Get().IsKeyword(); }

  constexpr Bool IsModifierKeyword() const noexcept { return Get().IsModifier(); }

  constexpr Bool IsDeclarativeKeyword() const noexcept { return Get().IsDeclarative(); }

  constexpr Bool IsAnOperand() const noexcept { return Get().IsAnOperand(); }

  constexpr Bool IsSingularPrefixOperator() const noexcept { return Get().IsAPrefixOperator(); }
  constexpr Bool IsPrefixOperator() const noexcept { return Get().IsAPrefixOperator(); }
  constexpr Bool IsPostfixOperator() const noexcept { return Get().TypeIs(eTk::kInc) || Get().TypeIs(eTk::kDec); }

  constexpr Bool IsOpeningScope() const noexcept { return Get().IsLScope(); }

  constexpr Bool IsClosingScope() const noexcept { return Get().IsRScope(); }

  constexpr Bool IsClosingScopeOf(eTk open) const noexcept { return Get().IsRScopeOf(open); }

  constexpr Bool IsPrimary() const noexcept { return Get().IsPrimary(); }

  constexpr Bool IsPragmatic() const noexcept {
    auto& c = Get();
    return c.IsModifier() || c.IsDeclarative();
  }

  constexpr eAst NodeType() const noexcept { return Get().NodeType(); }

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /* Iteration */
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  /// Advances the cursor by 1.
  constexpr TkCursor& Advance() {
    if (it_ + 1 >= end_) {
      it_ = end_;
    } else if (it_ + 1 < beg_) {
      it_ = beg_;
    } else {
      it_ += 1;
    }
    return *this;
  }

  /// Advances the cursor by n.
  constexpr TkCursor& Advance(int n) {
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

  /// Advances the cursor to the new_cursor. Checks that
  /// cursor is within beg and end.
  constexpr TkCursor& Advance(TkVecConstIterT new_it) {
    if (new_it < beg_) {
      throw std::out_of_range("tk_cursor passed advance_to outside of begin.");
    } else if (new_it > end_) {
      throw std::out_of_range("tk_cursor passed advance_to outside of end.");
    } else
      it_ = new_it;
    return *this;
  }

  constexpr TkCursor& Advance(const TkCursor& to) { return this->Advance(to.Iter()); }

  /// Returns cursor advanced by N. N may be negative.
  constexpr TkCursor Next(int n) const {
    TkCursor next_cursor = *this;
    next_cursor.Advance(n);
    return next_cursor;
  }

  /// Returns cursor advanced by N. N may be negative.
  constexpr TkCursor Next() const {
    TkCursor next_cursor = *this;
    next_cursor.Advance(1);
    return next_cursor;
  }

  constexpr TkCursor Next(TkVecConstIterT new_it) const {
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

  constexpr const Tk& Peek(int n) const { return Next(n).Get(); }

  constexpr Bool FindForward(const TkVecT& match) {
    auto end = std::next(it_, match.size());
    auto found = std::search(it_, end, match.begin(), match.end(),
                             [](const Tk& a, const Tk& b) { return a.Type() == b.Type(); });

    if (found != end) {
      return true;
    }
    return false;
  }

  constexpr Bool FindForwardExact(TkVecConstIterT cursor, const TkVecT& match) {
    // @note If you ever get compilation error:
    // [Error C3889 call to object of class type 'std::equal_to<void>' : no matching call operator found]
    // It means that Tk class does not have operator== defined.
    auto end = std::next(cursor, static_cast<std::ptrdiff_t>(match.size()));
    if (std::equal(match.begin(), match.end(), cursor, end)) {
      return true;
    }
    return false;
  }

  constexpr TkCursor(TkVecConstIterT begin, TkVecConstIterT end) : beg_(begin), end_(end), it_(begin) {}

  constexpr TkCursor(TkVecConstIterT begin, TkVecConstIterT end, TkVecConstIterT it)
      : beg_(begin), end_(end), it_(it) {}

  constexpr TkCursor(const TkCursor& other) : beg_(other.beg_), end_(other.end_), it_(other.it_) {}

  constexpr TkCursor& operator=(const TkCursor& other) {
    beg_ = other.beg_;
    end_ = other.end_;
    it_ = other.it_;
    return *this;
  }
};

using TkVecCursor = TkCursor<Vec>;
using TkSpanCursor = TkCursor<std::span>;
}  // namespace cnd

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* impl */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// #include "token_cursor.tpp"

/// @} // end of cnd_compiler_data

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
