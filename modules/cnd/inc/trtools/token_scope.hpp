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
#include "trtools/token_cursor.hpp"
// clang-format on

namespace cnd {
template <template <class T> class TokenContainerT>
class TkScope {
  using TkVecT = TokenContainerT<const Tk>;
  using TkVecConstIterT = TokenContainerT<const Tk>::const_iterator;
  using TkCursorT = TkCursor<TokenContainerT>;

 public:
  //constexpr TkVecConstIterT ContainedEnd() const;
  //constexpr TkVecConstIterT ContainedBegin() const;
  //constexpr bool IsEmpty() const;
  //constexpr TkVecConstIterT End() const;
  //constexpr TkVecConstIterT Begin() const;
  //constexpr TkCursorT Contained() const;
  //constexpr operator Bool() const;
  constexpr TkScope() = default;
  constexpr TkScope(TkScope&&) = default;
  constexpr TkScope(const TkScope &) = default;
  constexpr TkScope& operator = (TkScope &&) = default;
  constexpr TkScope& operator = (const TkScope &) = default;
  //constexpr TkScope(bool valid, TkVecConstIterT begin, TkVecConstIterT end);

 private:
  TkVecConstIterT begin_{};
  TkVecConstIterT end_{};

 public:
  /// Returns the end of the scope, not including the close token.
  constexpr TkVecConstIterT ContainedEnd() const { return end_ - 1; }

  /// Returns the beginning of the scope, not including the open token.
  constexpr TkVecConstIterT ContainedBegin() const { return begin_ + 1; }

  /// Returns true if the scope is empty.
  constexpr bool IsEmpty() const { return ContainedBegin() == ContainedEnd(); }

  /// Returns the end of the scope, including the close token.
  constexpr TkVecConstIterT End() const { return end_; }

  /// Returns the beginning of the scope, including the open token.
  constexpr TkVecConstIterT Begin() const { return begin_; }

  constexpr TkCursorT Contained() const { return TkCursorT{ContainedBegin(), ContainedEnd()}; }

  constexpr TkScope(bool valid, TkVecConstIterT begin, TkVecConstIterT end) : begin_(begin), end_(end) {}
};

}  // namespace cnd

// #include "cnd_tk_scope.tpp"

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
