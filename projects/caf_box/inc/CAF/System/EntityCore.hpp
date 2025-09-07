///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2025 Anton Yashchenko
// Licensed under the Apache License, Version 2.0(the "License");
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: [CAF] Core Application Framework
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup coreappfw
/// @brief [HEADER] Entity system.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup coreappfw
/// @{

// clang-format off
#pragma once
#include <concepts>
#include "cxxx.hpp"
#include "CAF/PreConfig.hpp"
// clang-format on

namespace caf::entity_core {
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* External Dependencies*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using TimeType = sf::Time;
using OsEventType = sf::Event;
using EventBufferType = std::vector<OsEventType>;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* fwd decl from "Entity.hpp" header */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace entity_impl {
class Ent;
class EntInterface;
template <class T>
class EntModel;
}  // namespace entity_impl

// Bring in required types from entity_impl namespace.
using Ent = entity_impl::Ent;
using EntInterface = entity_impl::EntInterface;
template <class T>
using EntModel = entity_impl::EntModel<T>;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* fwd decl */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class SelfT>  // Where SelfT must be the public implemtation of Ent from entity_impl.
struct EntCore;         // The private entity class base implementation.

struct EntPtrGuard;
struct EntStrongPtr;
struct EntSignalConnection;

struct EntPtrGuard {
  Ent** gptr;
  EntPtrGuard* next;
};

struct EntStrongPtr {
  Ent* ptr;
  EntPtrGuard* guard;
};

struct EntSignalConnection {
  std::function<void()> callback;
  std::vector<std::function<void()>> recievers;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @class EntImpl
/// @brief
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class SelfT>
struct EntCore {
  // clang-format off
  const SelfT* stem_{nullptr};
  std::vector<SelfT*> brs_{};
  EntPtrGuard* guards_{nullptr};

  constexpr EntCore() noexcept : stem_(nullptr), brs_() {};
  constexpr EntCore(SelfT* stem) noexcept : stem_(stem), brs_() {};
  constexpr EntCore(const SelfT* stem) noexcept : stem_(stem), brs_() {};
  constexpr ~EntCore() noexcept {
    // Delete all branches.
    for (auto& br : brs_) {
      delete br;
    }

    // Nullify all guarded pointers.
    EntPtrGuard* guard = guards_;
    while (guard) {
      if (guard->gptr) *guard->gptr = nullptr;
      EntPtrGuard* next = guard->next;
      delete guard;
      guard = next;
    }

    // Remove self from stem's branch vector.
    if (Stem()) Stem()->brs_.erase(std::remove(Stem()->brs_.begin(), Stem()->brs_.end(), this), Stem()->brs_.end());
  };

  // Guarded Pointer Access
  friend EntStrongPtr; // Allow 'EntStrongPtr' to access 'PushGuard' and 'RemoveGuard'.
  void PushGuard(Ent** pguard) noexcept;
  void RemoveGuard(Ent** pguard);

  // Properties
  constexpr bool IsLeaf() const noexcept;
  constexpr bool IsRoot() const noexcept;
  constexpr bool IsTrunk() const noexcept;
  constexpr unsigned int Level() const noexcept;
  constexpr std::size_t BranchCount() const noexcept { return brs_.size(); }

  // Binary predicates.
   constexpr bool IsDescendantOf(const SelfT* other) const;  ///> Wether T is a descendant of 'other'.
   constexpr bool IsChildOf(const SelfT* other) const;       ///> Wether T is a direct child of 'other'.
   //constexpr bool IsBeforePre(const Ent& other) const;     ///> Wether T precedes 'other' in pre-order traversal.
   //constexpr bool IsBeforePost(const Ent& other) const;    ///> Wether T precedes 'other' in post-order traversal.

  // Traversal
  // constexpr Ent* Find(const std::function<bool(const Ent&)>& pred) const;
  // constexpr Ent* NextPre(const std::function<bool(const Ent&)>& pred) const;
  // constexpr Ent* NextPost(const std::function<bool(const Ent&)>& pred) const;
  // constexpr Ent* NextSibling(const std::function<bool(const Ent&)>& pred) const;

  // Relatives
  constexpr SelfT* Stem() noexcept;
  constexpr SelfT* Root() noexcept;
  constexpr SelfT* Front() noexcept;
  constexpr SelfT* Back() noexcept;
  constexpr std::vector<SelfT*>& Branches() noexcept;

  constexpr const SelfT* Stem() const noexcept;
  constexpr const SelfT* Root() const noexcept;
  constexpr const SelfT* Front() const noexcept;
  constexpr const SelfT* Back() const noexcept;
  constexpr const std::vector<SelfT*>& Branches() const noexcept;

  // Mutation
  // template <class T> constexpr SelfT* Push(T* br) noexcept;
  // template <class T> constexpr SelfT* Push(T&& e);
  // template <class T> constexpr SelfT* Push(const T& e);
  // template <class T> constexpr SelfT* PushFront(T* br) noexcept;
  // template <class T> constexpr SelfT* PushFront(T&& e);
  // template <class T> constexpr SelfT* PushFront(const T& e);

  constexpr SelfT* PopBack() noexcept;
  constexpr SelfT* PopFront() noexcept;
  constexpr SelfT* Pop(const SelfT* br) noexcept;
  constexpr SelfT* Pop(SelfT* br) noexcept;

  constexpr void DeleteBack();
  constexpr void DeleteFront();
  constexpr void Delete(const SelfT* br);

  // template <class T> constexpr void Apply(this T& self, const std::function<void(SelfT&)>& func);
  // template <class T> constexpr void ApplyBranches(this SelfT& self, const std::function<void(SelfT&)>& func) noexcept;
  // template <class T> constexpr void Apply(this const SelfT& self, const std::function<void(const SelfT&)>& func) noexcept;
  // template <class T> constexpr void ApplyBranches(this const SelfT& self, const std::function<void(const SelfT&)>& func) noexcept;
  // clang-format on

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /* Inline Definitions */
  // Templated methods will have to be defined inline.
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // Mutation
  template <class T>
  constexpr SelfT* Push(T* e) noexcept {
    if (!e) return nullptr;
    SelfT* new_br = brs_.emplace_back(e);
    new_br->stem_ = const_cast<const SelfT*>(this);
    return new_br;
  }

  template <class T>
  constexpr SelfT* Push(T&& e) {
    brs_.push_back(new SelfT{std::forward<T>(e)});
    brs_.back()->stem_ = static_cast<SelfT*>(this);
    return brs_.back();
  }

  template <class T>
  constexpr SelfT* Push(const T& e) {
    brs_.push_back(new SelfT{std::forward<T>(e)});
    brs_.back()->stem_ = static_cast<SelfT*>(this);
    return brs_.back();
  }

  template <class T>
  constexpr SelfT* PushFront(T* br) noexcept {
    if (!br) return nullptr;
    SelfT* new_br = brs_.emplace(brs_.begin(), br);
    new_br->stem_ = static_cast<SelfT*>(this);
    return new_br;
  }

  template <class T>
  constexpr SelfT* PushFront(T&& e) {
    brs_.insert(brs_.begin(), new SelfT{std::forward<T>(e)});
    brs_.front()->stem_ = static_cast<SelfT*>(this);
    return brs_.front();
  }

  template <class T>
  constexpr SelfT* PushFront(const T& e) {
    brs_.insert(brs_.begin(), new SelfT{std::forward<T>(e)});
    brs_.front()->stem_ = static_cast<SelfT*>(this);
    return brs_.front();
  }

  template <class T>
  constexpr void Apply(this T& self, const std::function<void(SelfT&)>& func) noexcept {
    func(self);
    if (self.IsLeaf()) return;
    for (SelfT* br : self.brs_) {
      br->Apply(func);
    }
  }

  template <class T>
  constexpr void Apply(this const T& self, const std::function<void(const SelfT&)>& func) noexcept {
    func(self);
    if (self.IsLeaf()) return;
    for (const SelfT* br : self.brs_) {
      br->Apply(func);
    }
  }

  template <class T>
  constexpr void ApplyBranches(this T& self, const std::function<void(SelfT&)>& func) noexcept {
    if (self.IsLeaf()) return;
    for (SelfT* br : self.brs_) {
      br->Apply(func);
    }
  }

  template <class T>
  constexpr void ApplyBranches(this const T& self, const std::function<void(const SelfT&)>& func) noexcept {
    if (self.IsLeaf()) return;
    for (const SelfT* br : self.brs_) {
      br->Apply(func);
    }
  }
};

///////////////////////////////////////////////////////////////////////////////
// EntCore : Relatives
///////////////////////////////////////////////////////////////////////////////

/// Wether T has no branches.
template <class SelfT>
constexpr bool EntCore<SelfT>::IsLeaf() const noexcept {
  return brs_.empty();
};

/// Wether T is a root-most node.
template <class SelfT>
constexpr bool EntCore<SelfT>::IsRoot() const noexcept {
  return !stem_;
};

/// Wether T is a trunk node, i.e. has branches and a parent.
template <class SelfT>
constexpr bool EntCore<SelfT>::IsTrunk() const noexcept {
  return !IsRoot() && !IsLeaf();
};

///> Number of edges on the path from root node to T.
template <class SelfT>
constexpr unsigned int EntCore<SelfT>::Level() const noexcept {
  return stem_ ? stem_->Level() + 1 : 0;
}

///> Wether T is a descendant of 'other'.
template <class SelfT>
constexpr bool EntCore<SelfT>::IsDescendantOf(const SelfT* other) const {
  if (stem_ == other) return true;
  SelfT* current = stem_;
  while (current) {
    if (current == other) return true;
    current = current->stem_;
  }
  return false;
}

///> Wether T is a child of 'other'.
template <class SelfT>
constexpr bool EntCore<SelfT>::IsChildOf(const SelfT* other) const {
  return stem_ == other;
}

template <class SelfT>
constexpr SelfT* EntCore<SelfT>::Stem() noexcept {
  return const_cast<SelfT*>(stem_);
};

template <class SelfT>
constexpr const SelfT* EntCore<SelfT>::Stem() const noexcept {
  return stem_;
};

template <class SelfT>
constexpr SelfT* EntCore<SelfT>::Root() noexcept {
  if (!stem_) return this;
  return const_cast<SelfT*>(stem_->Root());
};

template <class SelfT>
constexpr const SelfT* EntCore<SelfT>::Root() const noexcept {
  if (!stem_) return this;
  return stem_->Root();
};

template <class SelfT>
constexpr const std::vector<SelfT*>& EntCore<SelfT>::Branches() const noexcept {
  return brs_;
};

template <class SelfT>
constexpr std::vector<SelfT*>& EntCore<SelfT>::Branches() noexcept {
  return brs_;
};

template <class SelfT>
constexpr const SelfT* EntCore<SelfT>::Front() const noexcept {
  if (brs_.empty()) return nullptr;
  return brs_.front();
};

template <class SelfT>
constexpr const SelfT* EntCore<SelfT>::Back() const noexcept {
  if (brs_.empty()) return nullptr;
  return brs_.back();
};

template <class SelfT>
constexpr SelfT* EntCore<SelfT>::Front() noexcept {
  if (brs_.empty()) return nullptr;
  return brs_.front();
};

template <class SelfT>
constexpr SelfT* EntCore<SelfT>::Back() noexcept {
  if (brs_.empty()) return nullptr;
  return brs_.back();
};

template <class SelfT>
void EntCore<SelfT>::PushGuard(Ent** pguard) noexcept {
  if (!guards_) {
    guards_ = new EntPtrGuard{pguard, nullptr};
  } else {
    EntPtrGuard* newgr = new EntPtrGuard{pguard, guards_};
    guards_ = newgr;
  }
}

template <class SelfT>
void EntCore<SelfT>::RemoveGuard(Ent** pguard) {
  if (!guards_) return;  // No guards to remove.
  EntPtrGuard* prev = nullptr;
  EntPtrGuard* curr = guards_;
  while (curr) {
    if (curr->pguard == pguard) {
      if (prev)
        prev->next = curr->next;
      else
        guards_ = curr->next;  // Remove head guard.

      delete curr;
      return;
    }
    prev = curr;
    curr = curr->next;
  }
}

template <class SelfT>
constexpr SelfT* EntCore<SelfT>::PopBack() noexcept {
  if (brs_.empty()) return nullptr;
  SelfT* back = brs_.back();
  brs_.pop_back();
  if (back->stem_ == this) back->stem_ = nullptr;  // Remove stem reference.
  return back;
}

template <class SelfT>
constexpr SelfT* EntCore<SelfT>::PopFront() noexcept {
  if (brs_.empty()) return nullptr;
  SelfT* front = brs_.front();
  brs_.erase(brs_.begin());
  if (front->stem_ == this) front->stem_ = nullptr;  // Remove stem reference.
  return front;
}

template <class SelfT>
constexpr SelfT* EntCore<SelfT>::Pop(const SelfT* br) noexcept {
  if (!br) return nullptr;
  auto it = std::find(brs_.begin(), brs_.end(), br);
  if (it == brs_.end()) return nullptr;  // Not found.
  SelfT* found_br = *it;
  brs_.erase(it);
  if (found_br->stem_ == this) found_br->stem_ = nullptr;  // Remove stem reference.
  return found_br;
}

template <class SelfT>
constexpr SelfT* EntCore<SelfT>::Pop(SelfT* br) noexcept {
  if (!br) return nullptr;
  auto it = std::find(brs_.begin(), brs_.end(), br);
  if (it == brs_.end()) return nullptr;  // Not found.
  SelfT* found_br = *it;
  brs_.erase(it);
  if (found_br->stem_ == this) found_br->stem_ = nullptr;  // Remove stem reference.
  return found_br;
}

template <class SelfT>
constexpr void EntCore<SelfT>::DeleteBack() {
  if (brs_.empty()) return nullptr;
  SelfT* back = brs_.back();
  brs_.pop_back();
  if (back->stem_ == this) back->stem_ = nullptr;  // Remove stem reference.
  delete back;
}

template <class SelfT>
constexpr void EntCore<SelfT>::DeleteFront() {
  if (brs_.empty()) return nullptr;
  SelfT* front = brs_.front();
  brs_.erase(brs_.begin());
  if (front->stem_ == this) front->stem_ = nullptr;  // Remove stem reference.
  delete front;
}

template <class SelfT>
constexpr void EntCore<SelfT>::Delete(const SelfT* br) {
  if (!br) return;
  auto it = std::find(brs_.begin(), brs_.end(), br);
  if (it == brs_.end()) return;
  SelfT* found_br = *it;
  brs_.erase(it);
  if (found_br->stem_ == this) found_br->stem_ = nullptr;
  delete found_br;
}

}  // namespace caf::entity_core

/// @} // end of coreappfw

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: [CAF] Core Application Framework
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
// @created: 2025/07/23
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2025 Anton Yashchenko
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
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////