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
#include "cnd_cst.hpp"

// clang-format off
#include "cnd_implicit.hpp"
#include "cnd_constdef.hpp"
#include "cnd_clerr.hpp"
#include "cnd_tk.hpp"
// clang-format on

namespace cnd {

Cst::Cst() : type_(eCst::kNONE) {}

Cst::Cst(eCst type) : type_(type) {}

Cst::Cst(eCst type, const Str& literal) : type_(type), literal_(literal) {}

Cst::Cst(eCst type, Str&& literal) : type_(type), literal_(forward<Str>(literal)) {}

Cst::Cst(eCst type, const char* literal) : type_(type), literal_(literal) {}

Cst::Cst(const Tk& t) : type_(eTkToCstEnum(t.Type())), literal_(t.Literal()) {}

Cst::Cst(eCst type, std::vector<Tk>::iterator beg, std::vector<Tk>::iterator end) : type_(type) {
  literal_ = "";
  for (std::vector<Tk>::iterator it = beg; it != end; it++) {
    literal_ += it->Literal();
  }
}

bool Cst::Leaf() const noexcept { return children_.empty(); }

bool Cst::Branch() const noexcept { return !children_.empty(); }

Cst& Cst::Parent() {
  assert(parent_ != nullptr &&
         "[Cst& Cst::Parent()] Called on node with no parent. (this->parent_ "
         "is nullptr).");
  return *parent_;
}

bool Cst::Empty() const noexcept { return children_.empty(); }

std::size_t Cst::Size() const { return children_.size(); }

Cst& Cst::PushBack(const Cst& nd) {
  children_.push_back(nd);
  auto& pushed = children_.back();
  pushed.SetParent(this);
  return pushed;
}

Cst& Cst::PushFront(const Cst& nd) {
  children_.push_front(nd);
  auto& pushed = children_.front();
  pushed.SetParent(this);
  return pushed;
}

Cst& Cst::PushBack(Cst&& nd) {
  children_.push_back(std::move(nd));
  auto& pushed = children_.back();
  pushed.SetParent(this);
  return pushed;
}

Cst& Cst::ExtractAndPush(cxx::Expected<Cst>& nd) {
  assert(nd && "Cst::ExtractAndPushBack() called with an empty node.");
  children_.push_back(std::forward<Cst>(nd.Extract()));
  auto& pushed = children_.back();
  pushed.SetParent(this);
  return pushed;
}

Cst& Cst::PushFront(Cst&& nd) {
  children_.push_front(std::move(nd));
  auto& pushed = children_.front();
  pushed.SetParent(this);
  return pushed;
}

Cst Cst::PopBack() {
  if (!children_.empty()) {
    Cst popped = children_.back();
    children_.pop_back();
    popped.PopParent();
    return popped;
  } else
    throw std::out_of_range("Cst node pop_back() called on node with no children.");
}

Cst Cst::PopFront() {
  if (!children_.empty()) {
    Cst popped = children_.front();
    children_.pop_front();
    popped.PopParent();
    return popped;
  } else
    throw std::out_of_range("Cst node pop_front() called on node with no children.");
}

Cst& Cst::Front() {
  if (!children_.empty())
    return children_.front();
  else
    throw std::out_of_range("Cst node front() called on node with no children.");
}

Cst& Cst::Back() {
  if (!children_.empty())
    return children_.back();
  else
    throw std::out_of_range("Cst node front() called on node with no children.");
}

Cst& Cst::At(std::size_t index) {
  if (index >= children_.size()) throw std::out_of_range("Cst node at() called with index out of range.");
  if (index == 0) return children_.front();

  return [this, &index]() -> Cst& {
    auto it = children_.begin();
    for (std::size_t i = 0; i < index; i++) {
      it++;
    }
    return *it;
  }();
}

const std::list<Cst>& Cst::Children() const { return children_; }

std::list<Cst>& Cst::ChildrenUnsafe() { return children_; }

const Cst& Cst::operator[](std::size_t index) const {
  if (index >= children_.size()) throw std::out_of_range("ast node [] index operator called with index out of range.");
  if (index == 0) return children_.front();

  return [this, &index]() -> const Cst& {
    auto it = children_.begin();
    for (std::size_t i = 0; i < index; i++) {
      it++;
    }
    return *it;
  }();
}

Cst& Cst::operator[](std::size_t index) { return At(index); }

bool Cst::TypeIs(eCst type) const noexcept { return type_ == type; }

bool Cst::TypeIsnt(eCst type) const noexcept { return type_ != type; }

bool Cst::IsLiteral() const noexcept {
  switch (type_) {
    using enum eCst;
    case kLitCstr:
    case kLitInt:
    case kLitUint:
    case kLitBool:
    case kLitReal:
    case kLitChar:
    case kLitByte:
    case kKwNone:
    case kKwTrue:
    case kKwFalse:
      return true;
    default:
      return false;
  }
}

bool Cst::IsArithmeticBinaryOp() const noexcept {
  switch (type_) {
    using enum eCst;
    case kAdd:
    case kSub:
    case kMul:
    case kDiv:
    case kMod:
      return true;
    default:
      return false;
  }
}

// bool Cst::IsPragmatic() const noexcept { return eAstIsPragmatic(type_); }

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