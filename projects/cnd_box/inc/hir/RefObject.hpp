///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2026 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup cnd_hir
/// @brief RefObject is a reference counted object wrapper.
/// 
/// RefObject is a reference counted object wrapper.
///  RefObject owns the data pointer and will handle deletion.
///  RefObject should only be initialized with a non-null pointer
///  using the static New method.
///  Any other operations involving RefObject should be done
///  using the member functions:
///   - IsDeleted : Check if the object is deleted.
///
///    - Self : Returns a C++ reference to the object.
///                 Does NOT increment the ref count.
///
///    - ConstSelf : Returns a C++ const reference to the object.
///                      Does NOT increment the ref count.
///
///    - Copy : Create a copy of the object with a new reference count.
///
///   - Move : Moves the object pointer to a new object.
///                 Does NOT call C++ move constructor.
///                 Marks the current object as deleted.
///
///    - GetRef : Get a new C& reference to the object.
///                      Increments the reference count.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <type_traits>

namespace cnd::hir {

struct InplaceConstructorTag : std::true_type {};
static constexpr inline auto kInplaceConstructor = InplaceConstructorTag{};
template <typename T>
// requires(not std::is_trivial_v<T>)
class RefObject {
 public:
  static constexpr RefObject New() noexcept {
    return RefObject{
        .is_deleted = false,
        .count = new std::size_t(1),
        .ptr = new T{},
    };
  }

  // Pass to New with arguments to construct the object in-place.
  // Create a new C& rvalue with the given data pointer.
  // RefObject owns the data pointer. And will handle deletion.
  static constexpr RefObject New(T* data_ptr) noexcept {
    assert(data_ptr != nullptr && "[C&] Creating null data is forbidden.");
    return RefObject{
        .is_deleted = false,
        .count = new std::size_t(1),
        .ptr = data_ptr,
    };
  }

  // RefObject does not own the data pointer. And will not handle deletion.
  static constexpr RefObject NewWeak(T* data_ptr) noexcept {
    return RefObject{
        .is_deleted = true,
        .count = nullptr,
        .ptr = data_ptr,
    };
  }

  // Create a new C& rvalue given a C++ rvalue.
  // Forwards the rvalue to the constructor of the object.
  static constexpr RefObject New(T&& rvalue_data) noexcept {
    return RefObject{
        .is_deleted = false,
        .count = new std::size_t(1),
        .ptr = new T(std::forward<T>(rvalue_data)),
    };
  }

  // Create a new C& rvalue given a C++ lvalue.
  // Copies the lvalue to the constructor of the object.
  static constexpr RefObject New(const T& lvalue_data) noexcept {
    return RefObject{
        .is_deleted = false,
        .count = new std::size_t(1),
        .ptr = new T(lvalue_data),
    };
  }

  // Create a new C& rvalue given an inplace constructor tag
  // and the arguments to pass to the constructor.
  template <typename... Args>
  static constexpr RefObject New(InplaceConstructorTag, Args&&... args) noexcept {
    return RefObject{
        .is_deleted = false,
        .count = new std::size_t(1),
        .ptr = new T(std::forward<Args>(args)...),
    };
  }

 public:
  // Check if the object is already deleted.
  // Which may occur if the object is moved or manually deleted.
  constexpr inline bool IsDeleted() const noexcept { return is_deleted; }

  // Get a C++ reference to the object.
  // Does not increment the reference count.
  constexpr inline T& Self() noexcept {
    //assert(not is_deleted && "[C&] Cannot refer to a deleted object.");
    return *ptr;
  }

  // Get a C++ reference to the object.
  // Does not increment the reference count.
  constexpr inline const T& Self() const noexcept {
    //assert(not is_deleted && "[C&] Cannot refer to a deleted object.");
    return *ptr;
  }

  // Get a const C++ reference to the object.
  // Does not increment the reference count.
  constexpr inline const T& ConstSelf() const noexcept {
    //assert(not is_deleted && "[C&] Cannot refer to a deleted object.");
    return *ptr;
  }

  // Creates a copy of the object with a new reference count.
  constexpr inline RefObject Copy() const noexcept {
    assert(not is_deleted && "[C&] Cannot copy a deleted object.");
    return RefObject{
        .is_deleted = false,
        .count = new std::size_t(1),
        .ptr = new T(*ptr),
    };
  }

  // The count of this object is deleted.
  // New count is created with same value.
  // Data ptr is assinged to other data. Then
  // this data points to null.
  constexpr inline RefObject Move() noexcept {
    assert(not is_deleted && "[C&] Cannot move a deleted object.");

    auto ret = RefObject{
        .is_deleted = false,
        .count = count,
        .ptr = ptr,
    };

    // !!TODO: ??? is this necessary? if it is then count and data must be
    // non-const pointers.
    count = nullptr;
    ptr = nullptr;

    // Nullify this object
    is_deleted = true;

    return ret;
  }

  // Obtain a C& reference to the object. Increments the reference count.
  constexpr inline RefObject MakeRef() noexcept {
    assert(not is_deleted && "[C&] Cannot reference a deleted object.");
    (*count)++;
    return RefObject{
        .is_deleted = false,
        .count = count,
        .ptr = ptr,
    };
  }

 public:
  //// Copy constructor. Does not increment the reference count.
  // constexpr inline RefObject(const RefObject& other) noexcept
  //     : is_deleted(other.is_deleted),
  //       count(other.count),
  //       ptr(other.ptr) {}

  //// Move constructor. Does not increment the reference count.
  // constexpr inline RefObject(RefObject&& other) noexcept
  //     : is_deleted(other.is_deleted),
  //       count(other.count),
  //       ptr(other.ptr) {
  //   other.is_deleted = true;
  // }

  //// Copy assignment. Does not increment the reference count.
  // constexpr inline RefObject& operator=(const RefObject& other) noexcept {
  //   if (this != &other) {
  //     is_deleted = other.is_deleted;
  //     count = other.count;
  //     ptr = other.ptr;
  //   }
  //   return *this;
  // }

  //// Move assignment. Does not increment the reference count.
  // constexpr inline RefObject& operator=(RefObject&& other) noexcept {
  //   if (this != &other) {
  //     is_deleted = other.is_deleted;
  //     count = other.count;
  //     ptr = other.ptr;
  //     other.is_deleted = true;
  //   }
  //   return *this;
  // }

  //// Default constructor creates a deleted object.
  // constexpr inline RefObject() noexcept = default;

  // Performs 3 checks in order before deleting the object:
  // 1. Was the object moved/already deleted ?
  // 2. Is the object still being referenced? -> Decrement the reference count
  // 3. Are there no references left? -> Delete the object
  constexpr inline bool Release() noexcept {
    if (not is_deleted) {
      if ((*count) > 0) {
        (*count)--;

        if (*count == 0) {
          is_deleted = true;
          delete count;
          delete ptr;
          return false;
        } else
          return true;
      }
    }
    return false;
  }

 public:
  bool is_deleted{true};            // False if the object is still alive.
  std::size_t* count{nullptr};      // C& Reference count of the object.
  T* ptr{nullptr};                  // Pointer to the object data.
};
}  // namespace cnd::hir

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2026 Anton Yashchenko
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