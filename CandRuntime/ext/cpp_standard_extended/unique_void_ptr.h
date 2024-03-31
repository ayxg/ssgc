#pragma once
#include <functional>
#include <memory>

// Wraps a void* stored as a std::unique_ptr<void>.
// Exposes the unique_ptr's interface.
// get<T>() performs a static_cast from void* to T*.
// No additional functionality is provided.
class UniqueVoidPtr {
 public:
  using raw_void_ptr = void*;
  using void_unique_ptr = std::unique_ptr<void, void (*)(raw_void_ptr)>;
  struct UnknownType {};  // default type for unique_ptr
 private:
  template <class T>
  static constexpr void_unique_ptr MakeStdVoidUptr(T* ptr) {
    return {ptr, [](raw_void_ptr ptr) { delete static_cast<T*>(ptr); }};
  }

  void_unique_ptr ptr_{MakeStdVoidUptr(new UnknownType())};

 public:
  constexpr UniqueVoidPtr() = default;
  constexpr UniqueVoidPtr(const UniqueVoidPtr&) = delete;
  constexpr UniqueVoidPtr& operator=(const UniqueVoidPtr&) = delete;
  constexpr UniqueVoidPtr& operator=(UniqueVoidPtr&&) = default;
  constexpr UniqueVoidPtr(UniqueVoidPtr&& other) = default;

  template <class T>
  constexpr UniqueVoidPtr(T* ptr) : ptr_(MakeStdVoidUptr(ptr)) {}

  template <class T>
  constexpr UniqueVoidPtr(T&& val)
      : ptr_(MakeStdVoidUptr(new T(std::forward<T>(val)))) {}

  template <class T>
  void Reset(T* ptr) {
    ptr_.reset(ptr);
  }

  bool IsNull() { return ptr_.get() == nullptr; }

  void Reset() { ptr_.reset(); }

  raw_void_ptr Ptr() const { return ptr_.get(); }

  template <class T>
  T* GetAs() {
    return static_cast<T*>(ptr_.get());
  }
};