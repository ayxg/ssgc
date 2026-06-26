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

/// @addtogroup cnd_compiler_tools
/// @{
#pragma once
#include "TkCursor.hpp"

// clang-format off
#include "ccapi/CommonCppApi.hpp"
#include "use_corevals.hpp"
#include "use_clmsg.hpp"
#include "frontend/tk.hpp"
#include "frontend/ast.hpp"
// clang-format on

namespace cnd {
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Properties */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

constexpr TkCursor::TkVecConstIterT TkCursor::End() const { return end_; }

constexpr TkCursor::TkVecConstIterT TkCursor::Begin() const { return beg_; }

constexpr TkCursor::TkVecConstIterT TkCursor::Iter() const { return it_; }

constexpr const Tk& TkCursor::Get() const {
  if (it_ >= end_) {
    return kSentinelEndToken;
  }
  return *it_;
}

constexpr Bool TkCursor::AtEnd() const { return (it_ == end_) || (it_->TypeIs(eTk::kEofile)); }

constexpr const Tk& TkCursor::operator->() const { return Get(); }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Token Properties */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

constexpr eTk TkCursor::Type() const noexcept { return Get().Type(); }

constexpr Size TkCursor::Length() const { return Get().Length(); }

// constexpr Size TkCursor::Line() const noexcept { return Get().Line(); }
// constexpr Size TkCursor::Col() const noexcept { return Get().Col(); }
constexpr StrView TkCursor::Literal() const { return Get().Literal(); }

constexpr ePriority TkCursor::Priority() const { return Get().Priority(); }

constexpr eAssoc TkCursor::Assoc() const { return Get().Assoc(); }

constexpr eOperation TkCursor::Operation() const { return Get().Operation(); }

constexpr Bool TkCursor::TypeIs(eTk type) const noexcept { return Get().TypeIs(type); }

constexpr Bool TkCursor::TypeIsnt(eTk type) const noexcept { return !(Get().TypeIs(type)); }

constexpr Bool TkCursor::TypeAndLitIs(eTk kind, const Str& literal) const noexcept {
  return Get().TypeAndLitIs(kind, literal);
}

constexpr Bool TkCursor::IsKeyword() const noexcept { return Get().IsKeyword(); }

constexpr Bool TkCursor::IsModifierKeyword() const noexcept { return Get().IsModifier(); }

constexpr Bool TkCursor::IsDeclarativeKeyword() const noexcept { return Get().IsDeclarative(); }

constexpr Bool TkCursor::IsAnOperand() const noexcept { return Get().IsAnOperand(); }

constexpr Bool TkCursor::IsSingularPrefixOperator() const noexcept { return Get().IsAPrefixOperator(); }

constexpr Bool TkCursor::IsOpeningScope() const noexcept { return Get().IsLScope(); }

constexpr Bool TkCursor::IsClosingScope() const noexcept { return Get().IsRScope(); }

constexpr Bool TkCursor::IsClosingScopeOf(eTk open) const noexcept { return Get().IsRScopeOf(open); }

constexpr Bool TkCursor::IsPrimary() const noexcept { return Get().IsPrimary(); }

constexpr Bool TkCursor::IsPragmatic() const noexcept {
  auto& c = Get();
  return c.IsModifier() || c.IsDeclarative();
}

constexpr eCst TkCursor::NodeType() const noexcept { return Get().NodeType(); }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Iteration */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// Advances the cursor by n.
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

/// Advances the cursor to the new_cursor. Checks that
/// cursor is within beg and end.
constexpr TkCursor& TkCursor::Advance(TkVecConstIterT new_it) {
  if (new_it < beg_) {
    throw std::out_of_range("tk_cursor passed advance_to outside of begin.");
  } else if (new_it > end_) {
    throw std::out_of_range("tk_cursor passed advance_to outside of end.");
  } else
    it_ = new_it;
  return *this;
}

// constexpr TkCursor& Advance(const ParseRes& result);

/// Returns cursor advanced by N. N may be negative.
constexpr TkCursor TkCursor::Next(int n) const {
  TkCursor next_cursor = *this;
  next_cursor.Advance(n);
  return next_cursor;
}

constexpr TkCursor TkCursor::Next(TkVecConstIterT new_it) const {
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

constexpr const Tk& TkCursor::Peek(int n) const { return Next(n).Get(); }

constexpr Bool TkCursor::FindForward(const TkVecT& match) {
  auto end = std::next(it_, match.size());
  auto found =
      std::search(it_, end, match.begin(), match.end(), [](const Tk& a, const Tk& b) { return a.Type() == b.Type(); });

  if (found != end) {
    return true;
  }
  return false;
}

constexpr Bool TkCursor::FindForwardExact(TkVecConstIterT cursor, const TkVecT& match) {
  // @note If you ever get compilation error:
  // [Error C3889 call to object of class type 'std::equal_to<void>' : no matching call operator found]
  // It means that Tk class does not have operator== defined.
  auto end = std::next(cursor, static_cast<std::ptrdiff_t>(match.size()));
  if (std::equal(match.begin(), match.end(), cursor, end)) {
    return true;
  }
  return false;
}

constexpr TkCursor::TkCursor(TkVecConstIterT begin, TkVecConstIterT end) : beg_(begin), end_(end), it_(begin) {}

constexpr TkCursor::TkCursor(TkVecConstIterT begin, TkVecConstIterT end, TkVecConstIterT it)
    : beg_(begin), end_(end), it_(it) {}

constexpr TkCursor::TkCursor(const TkCursor& other) : beg_(other.beg_), end_(other.end_), it_(other.it_) {}

constexpr TkCursor& TkCursor::operator=(const TkCursor& other) {
  beg_ = other.beg_;
  end_ = other.end_;
  it_ = other.it_;
  return *this;
}

}  // namespace cnd

/// @} // end of cnd_compiler_tools

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