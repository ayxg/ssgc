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
#include "cnd_implicit.hpp"
#include "cnd_constdef.hpp"
#include "cnd_clerr.hpp"
#include "cnd_tk.hpp"
// clang-format on

namespace cnd {

// C& abstract syntax tree node structure.
class Cst {
 public:
  // Properties
  constexpr eCst Type() const noexcept;
  constexpr const std::string& Literal() const noexcept;
  bool Leaf() const noexcept;
  constexpr bool Root() const noexcept;
  bool Branch() const noexcept;

  // Parent operations.
  Cst& Parent();
  constexpr void SetParent(Cst* parent);
  constexpr void PopParent();

  // Child operations.
  bool Empty() const noexcept;
  std::size_t Size() const;
  Cst& PushBack(const Cst& nd);
  Cst& PushFront(const Cst& nd);
  Cst& PushBack(Cst&& nd);
  Cst& PushFront(Cst&& nd);
  Cst PopBack();
  Cst PopFront();
  Cst& Front();
  Cst& Back();
  Cst& At(std::size_t index);
  const std::list<Cst>& Children() const;
  std::list<Cst>& ChildrenUnsafe();

  /// Index operator accesses children.
  const Cst& operator[](std::size_t index) const;
  Cst& operator[](std::size_t index);

  // Fast type queries.
  bool TypeIs(eCst type) const noexcept;
  bool TypeIsnt(eCst type) const noexcept;
  bool IsLiteral() const noexcept;
  bool IsArithmeticBinaryOp() const noexcept;
  bool IsPragmatic() const noexcept;

  // Source location
  constexpr size_t SourceLine() const noexcept;
  constexpr size_t SourceColumn() const noexcept;
  constexpr size_t SetSourceLine(size_t line);
  constexpr size_t SetSourceColumn(size_t column);

  /// Special PushBack methods for  parser results.
  template <typename AlwaysT>
  Cst& ExtractAndPush(cxx::PartialExpected<Cst, AlwaysT>& nd);
  Cst& ExtractAndPush(cxx::Expected<Cst>& nd);

 public:
  Cst();
  Cst(const Tk& t);
  Cst(eCst type);
  Cst(eCst type, const std::string& literal);
  Cst(eCst type, std::string&& literal);
  Cst(eCst type, const char* literal);
  Cst(eCst type, std::vector<Tk>::iterator beg, std::vector<Tk>::iterator end);

  template <std::size_t LIT_SIZE>
  Cst(eCst type, const char literal[LIT_SIZE]);

  template <typename... ChildTs>
    requires(std::is_same_v<Cst, std::decay_t<ChildTs>> && ...)
  Cst(eCst type, const std::string& literal, ChildTs... children);

  template <typename... ChildTs>
    requires(std::is_same_v<Cst, std::decay_t<ChildTs>> && ...)
  Cst(eCst type, std::vector<Tk>::iterator beg, std::vector<Tk>::iterator end, ChildTs... children);

 private:
  size_t source_line_{0};
  size_t source_column_{0};
  eCst type_{eCst::kNONE};
  std::string literal_{""};
  Cst* parent_{nullptr};
  std::list<Cst> children_;
};

template <std::size_t LIT_SIZE>
Cst::Cst(eCst type, const char literal[LIT_SIZE]) : type_(type), literal_(literal) {}

template <typename... ChildTs>
  requires(std::is_same_v<Cst, std::decay_t<ChildTs>> && ...)
Cst::Cst(eCst type, const std::string& literal, ChildTs... children) : type_(type), literal_(literal) {
  (PushBack(children), ...);
}

template <typename... ChildTs>
  requires(std::is_same_v<Cst, std::decay_t<ChildTs>> && ...)
Cst::Cst(eCst type, std::vector<Tk>::iterator beg, std::vector<Tk>::iterator end, ChildTs... children) : type_(type) {
  literal_ = u8"";
  for (std::vector<Tk>::iterator it = beg; it != end; it++) {
    literal_ += it->Literal();
  }
  (PushBack(children), ...);
}

constexpr size_t Cst::SourceLine() const noexcept { return source_line_; }

constexpr size_t Cst::SourceColumn() const noexcept { return source_column_; }

constexpr size_t Cst::SetSourceLine(size_t line) { return source_line_ = line; }

constexpr size_t Cst::SetSourceColumn(size_t column) { return source_column_ = column; }

constexpr eCst Cst::Type() const noexcept { return type_; }

constexpr const std::string& Cst::Literal() const noexcept { return literal_; }

constexpr bool Cst::Root() const noexcept { return parent_ == nullptr; }

constexpr void Cst::SetParent(Cst* parent) { parent_ = parent; }

constexpr void Cst::PopParent() {
  assert(parent_ != nullptr &&
         "[Cst& Cst::PopParent()] Called on node with no parent. (this->parent_ "
         "is nullptr).");
  parent_ = nullptr;
}

template <typename AlwaysT>
Cst& Cst::ExtractAndPush(cxx::PartialExpected<Cst, AlwaysT>& nd) {
  assert(nd && "Cst::ExtractAndPushBack() called with an empty node.");
  children_.push_back(std::forward<Cst>(nd.Extract()));
  auto& pushed = children_.back();
  pushed.SetParent(this);
  return pushed;
}

}  // namespace cnd

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