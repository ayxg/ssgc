//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
// Copyright 2024 Anton Yashchenko
// Licensed under the Apache License, Version 2.0(the "License");
//---------------------------------------------------------------------------//
// Author(s): Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.acpp.dev
//---------------------------------------------------------------------------//
// Project: C& Programming Language Environment
// Directory: cand-official-compiler
// File: caoco_ast.h
//---------------------------------------------------------------------------//
#ifndef HEADER_GUARD_CALE_CAND_OFFICIAL_COMPILER_CAOCO_AST_H
#define HEADER_GUARD_CALE_CAND_OFFICIAL_COMPILER_CAOCO_AST_H
//---------------------------------------------------------------------------//
// Brief: brief
//---------------------------------------------------------------------------//
#include "cppsextended.h"
// Includes:
#include "caoco_grammar.h"
#include "caoco_token.h"
//#include "caoco_token_cursor.h"
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
// Namespace:{caoco}
//---------------------------------------------------------------------------//
namespace caoco {
//---------------------------------------------------------------------------//

//---------------------------------------------------------//
// Class:{Ast}
// Brief:{
//  C& abstract syntax tree node structure.
// }
//---------------------------------------------------------//
class Ast {
 public:
  // Properties
  constexpr eAst Type() const noexcept;
  constexpr const std::string& Literal() const noexcept;
  bool Leaf() const noexcept;
  constexpr bool Root() const noexcept;
  bool Branch() const noexcept;

  // Parent operations.
  Ast& Parent();
  constexpr void SetParent(Ast* parent);
  constexpr void PopParent();

  // Child operations.
  bool Empty() const noexcept;
  std::size_t Size() const;
  Ast& PushBack(const Ast& nd);
  Ast& PushFront(const Ast& nd);
  Ast& PushBack(Ast&& nd);
  Ast& PushFront(Ast&& nd);
  Ast PopBack();
  Ast PopFront();
  Ast& Front();
  Ast& Back();
  Ast& At(std::size_t index);
  const std::list<Ast>& Children() const;
  // Index operator accesses children.
  const Ast& operator[](std::size_t index) const;
  Ast& operator[](std::size_t index);

  //// Fast type queries.
  bool TypeIs(eAst type) const noexcept;
  bool TypeIsnt(eAst type) const noexcept;
  bool IsLiteral() const noexcept;
  bool IsArithmeticBinaryOp() const noexcept;
  bool IsPragmatic() const noexcept;

  // Source location
  constexpr inline size_t SourceLine() const noexcept { return source_line_; }
  constexpr inline size_t SourceColumn() const noexcept {
    return source_column_;
  }
  constexpr inline size_t SetSourceLine(size_t line) {
    return source_line_ = line;
  }
  constexpr inline size_t SetSourceColumn(size_t column) {
    return source_column_ = column;
  }

  // Special PushBack methods for  parser results.
  template <typename AlwaysT>
  Ast& ExtractAndPush(cxx::PartialExpected<Ast, AlwaysT>& nd) {
    assert(nd && "Ast::ExtractAndPushBack() called with an empty node.");
    children_.push_back(std::forward<Ast>(nd.Extract()));
    auto& pushed = children_.back();
    pushed.SetParent(this);
    return pushed;
  }

  Ast& ExtractAndPush(cxx::Expected<Ast>& nd);

 public:
  Ast() : type_(eAst::INVALID) {}
  Ast(const Tk& t);
  Ast(eAst type) : type_(type) {}
  Ast(eAst type, const std::string& literal) : type_(type), literal_(literal) {}
  Ast(eAst type, std::string&& literal)
      : type_(type), literal_(std::move(literal)) {}
  Ast(eAst type, const char* literal) : type_(type), literal_(literal) {}
  Ast(eAst type, std::vector<Tk>::iterator beg, std::vector<Tk>::iterator end);

  template <std::size_t LIT_SIZE>
  Ast(eAst type, const char literal[LIT_SIZE])
      : type_(type), literal_(literal) {}

  template <typename... ChildTs>
    requires(std::is_same_v<Ast, std::decay_t<ChildTs>> && ...)
  Ast(eAst type, const std::string& literal, ChildTs... children)
      : type_(type), literal_(literal) {
    (PushBack(children), ...);
  }

  template <typename... ChildTs>
    requires(std::is_same_v<Ast, std::decay_t<ChildTs>> && ...)
  Ast(eAst type, std::vector<Tk>::iterator beg, std::vector<Tk>::iterator end,
      ChildTs... children)
      : type_(type) {
    literal_ = u8"";
    for (std::vector<Tk>::iterator it = beg; it != end; it++) {
      literal_ += it->Literal();
    }
    (PushBack(children), ...);
  }

 private:
  size_t source_line_{0};
  size_t source_column_{0};
  eAst type_{eAst::INVALID};
  std::string literal_{""};
  Ast* parent_{nullptr};
  std::list<Ast> children_;
};
//---------------------------------------------------------//
// EndClass:{Ast}
//---------------------------------------------------------//

Ast::Ast(const Tk& t) : type_(eTkToAstEnum(t.Type())), literal_(t.Literal()) {}

Ast::Ast(eAst type, std::vector<Tk>::iterator beg,
         std::vector<Tk>::iterator end)
    : type_(type) {
  literal_ = "";
  for (std::vector<Tk>::iterator it = beg; it != end; it++) {
    literal_ += it->Literal();
  }
}

constexpr eAst Ast::Type() const noexcept { return type_; }

constexpr const std::string& Ast::Literal() const noexcept { return literal_; }

bool Ast::Leaf() const noexcept { return children_.empty(); }

constexpr bool Ast::Root() const noexcept { return parent_ == nullptr; }

bool Ast::Branch() const noexcept { return !children_.empty(); }

Ast& Ast::Parent() {
  assert(parent_ != nullptr &&
         "[Ast& Ast::Parent()] Called on node with no parent. (this->parent_ "
         "is nullptr).");
  return *parent_;
}

constexpr void Ast::SetParent(Ast* parent) { parent_ = parent; }

constexpr void Ast::PopParent() {
  assert(
      parent_ != nullptr &&
      "[Ast& Ast::PopParent()] Called on node with no parent. (this->parent_ "
      "is nullptr).");
  parent_ = nullptr;
}

bool Ast::Empty() const noexcept { return children_.empty(); }
std::size_t Ast::Size() const { return children_.size(); }

Ast& Ast::PushBack(const Ast& nd) {
  children_.push_back(nd);
  auto& pushed = children_.back();
  pushed.SetParent(this);
  return pushed;
}

Ast& Ast::PushFront(const Ast& nd) {
  children_.push_front(nd);
  auto& pushed = children_.front();
  pushed.SetParent(this);
  return pushed;
}

Ast& Ast::PushBack(Ast&& nd) {
  children_.push_back(std::move(nd));
  auto& pushed = children_.back();
  pushed.SetParent(this);
  return pushed;
}


Ast& Ast::ExtractAndPush(cxx::Expected<Ast>& nd) {
  assert(nd && "Ast::ExtractAndPushBack() called with an empty node.");
  children_.push_back(std::forward<Ast>(nd.Extract()));
  auto& pushed = children_.back();
  pushed.SetParent(this);
  return pushed;
}

Ast& Ast::PushFront(Ast&& nd) {
  children_.push_front(std::move(nd));
  auto& pushed = children_.front();
  pushed.SetParent(this);
  return pushed;
}

Ast Ast::PopBack() {
  if (!children_.empty()) {
    Ast popped = children_.back();
    children_.pop_back();
    popped.PopParent();
    return popped;
  } else
    throw std::out_of_range(
        "Ast node pop_back() called on node with no children.");
}

Ast Ast::PopFront() {
  if (!children_.empty()) {
    Ast popped = children_.front();
    children_.pop_front();
    popped.PopParent();
    return popped;
  } else
    throw std::out_of_range(
        "Ast node pop_front() called on node with no children.");
}

Ast& Ast::Front() {
  if (!children_.empty())
    return children_.front();
  else
    throw std::out_of_range(
        "Ast node front() called on node with no children.");
}

Ast& Ast::Back() {
  if (!children_.empty())
    return children_.back();
  else
    throw std::out_of_range(
        "Ast node front() called on node with no children.");
}

Ast& Ast::At(std::size_t index) {
  if (index >= children_.size())
    throw std::out_of_range("Ast node at() called with index out of range.");
  if (index == 0) return children_.front();

  return [this, &index]() -> Ast& {
    auto it = children_.begin();
    for (std::size_t i = 0; i < index; i++) {
      it++;
    }
    return *it;
  }();
}

const std::list<Ast>& Ast::Children() const { return children_; }

const Ast& Ast::operator[](std::size_t index) const {
  if (index >= children_.size())
    throw std::out_of_range(
        "ast node [] index operator called with index out of range.");
  if (index == 0) return children_.front();

  return [this, &index]() -> const Ast& {
    auto it = children_.begin();
    for (std::size_t i = 0; i < index; i++) {
      it++;
    }
    return *it;
  }();
}

Ast& Ast::operator[](std::size_t index) { return At(index); }

bool Ast::TypeIs(eAst type) const noexcept { return type_ == type; }

bool Ast::TypeIsnt(eAst type) const noexcept { return type_ != type; }

bool Ast::IsLiteral() const noexcept {
  switch (type_) {
    using enum eAst;
    case LitCstr:
    case LitInt:
    case LitUint:
    case LitBool:
    case LitReal:
    case LitChar:
    case LitByte:
    case KwNone:
    case KwTrue:
    case KwFalse:
      return true;
    default:
      return false;
  }
}

bool Ast::IsArithmeticBinaryOp() const noexcept {
  switch (type_) {
    using enum eAst;
    case Add:
    case Sub:
    case Mul:
    case Div:
    case Mod:
      return true;
    default:
      return false;
  }
}

bool Ast::IsPragmatic() const noexcept { return eAstIsPragmatic(type_); }

//---------------------------------------------------------------------------//
}  // namespace caoco
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
// Copyright 2024 Anton Yashchenko
//
// Licensed under the Apache License, Version 2.0(the "License");
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
// File: cand_ast.h
//---------------------------------------------------------------------------//
#endif HEADER_GUARD_CALE_CAND_OFFICIAL_COMPILER_CAOCO_AST_H
//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//