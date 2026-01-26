///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2026 Anton Yashchenko
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

/// @addtogroup cnd_compiler
/// @{
#pragma once
// clang-format off
#include "ccapi/CommonCppApi.hpp"
#include "compiler_utils/CompilerProcessResult.hpp"
#include "RefObject.hpp"
// clang-format on

namespace cnd::hir {

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Declare predefined high intermediate representation types. Used during the compeval interpretation phase of
// compilation.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Literal Meta Types
struct None;
struct Undefined;
struct Error;

// Literal Signed Integrals
struct I8;
struct I16;
struct I32;
struct I64;

// Literal Unsigned Integrals
struct Bool;
struct U8;
struct U16;
struct U32;
struct U64;

// Literal Floating Points
struct F32;
struct F64;

struct CStr;  // Constant string (C string).
struct Ptr;   // Weak pointer.

struct String;
struct Array;
struct Map;

struct StrIter;
struct ArrayIter;
struct MapIter;
struct UserType;

/// Union of all C& types.
struct AnyValue;
using AV = AnyValue;

enum class eTypeIndex : std::size_t {
  Undefined = 0,
  None,
  Error,
  I8,
  I16,
  I32,
  I64,
  Bool,
  U8,
  U16,
  U32,
  U64,
  F32,
  F64,
  CStr,
  String,
  Array,
  Map,
  StrIter,
  ArrayIter,
  MapIter
};

using AnyVariantBase =
    std::variant<RefObject<Undefined>, RefObject<None>, RefObject<Error>, RefObject<I8>, RefObject<I16>, RefObject<I32>,
                 RefObject<I64>, RefObject<Bool>, RefObject<U8>, RefObject<U16>, RefObject<U32>, RefObject<U64>,
                 RefObject<F32>, RefObject<F64>, RefObject<CStr>, RefObject<String>, RefObject<Array>, RefObject<Map>,
                 RefObject<StrIter>, RefObject<ArrayIter>, RefObject<MapIter>>;

template<class T>
concept iIntegral =
    std::same_as<T, I8> || std::same_as<T, I16> || std::same_as<T, I32> || std::same_as<T, I64> || std::same_as<T, Bool> ||
    std::same_as<T, U8> || std::same_as<T, U16> || std::same_as<T, U32> || std::same_as<T, U64>;

// None object represents a null or nothing.
// Performing an operation with none will result in no change.
// Functions may act diffrently if none is pass. Indicating a lack of argument.
struct None {
  using DataT = void;

  // Always equal to all other none objects.
  constexpr bool operator==(const None&) const { return true; }

  // Always not equal to all other object types.
  template <class T>
  constexpr bool operator==(const T&) const {
    return false;
  }
};

// Represents an uninitialized or destroyed value. Invalid for use.
struct Undefined {
  using DataT = void;

  // Always equal to all other undefined objects.
  constexpr bool operator==(const Undefined&) const { return true; }

  // Always not equal to all other object types.
  template <class T>
  constexpr bool operator==(const T&) const {
    return false;
  }
};

// Literal Error, represents an error. Holds a message as a constant string.
struct Error {
  using DataT = const char*;
  DataT data;
};

struct I8 {
  using DataT = std::int8_t;
  DataT data;

  static constexpr ClRes<I8> FromLiteral(std::string_view sv) noexcept {
    I8 rt{};
    auto res = std::from_chars(sv.data(), sv.data() + sv.size(), rt.data);
    if (res.ec != std::errc{}) {
      return DEBUG_FAIL("Failed to convert number literal to i8 value.");
    }
    return rt;
  }
};

struct I16 {
  using DataT = std::int16_t;
  DataT data;

  static constexpr ClRes<I16> FromLiteral(std::string_view sv) noexcept {
    I16 rt{};
    auto res = std::from_chars(sv.data(), sv.data() + sv.size(), rt.data);
    if (res.ec != std::errc{}) {
      return DEBUG_FAIL("Failed to convert number literal to i16 value.");
    }
    return rt;
  }
};

struct I32 {
  using DataT = std::int32_t;
  DataT data;

  static constexpr ClRes<I32> FromLiteral(std::string_view sv) {
    I32 rt{};
    auto res = std::from_chars(sv.data(), sv.data() + sv.size(), rt.data);
    if (res.ec != std::errc{}) {
      return DEBUG_FAIL("Failed to convert number literal to i32 value.");
    }
    return rt;
  }
};

struct I64 {
  using DataT = std::int64_t;
  DataT data;

  static constexpr ClRes<I64> FromLiteral(std::string_view sv) {
    I64 rt{};
    auto res = std::from_chars(sv.data(), sv.data() + sv.size(), rt.data);
    if (res.ec != std::errc{}) {
      return DEBUG_FAIL("Failed to convert number literal to i64 value.");
    }
    return rt;
  }
};

struct Bool {
  using DataT = bool;
  DataT data;
  static constexpr ClRes<Bool> FromLiteral(std::string_view sv) {
    if (sv == "true")
      return Bool{true};
    else if (sv == "false")
      return Bool{false};
    return DEBUG_FAIL("Failed to convert number literal to bool value.");
  }
};

struct U8 {
  using DataT = std::uint8_t;
  DataT data;
  static constexpr ClRes<U8> FromLiteral(std::string_view sv) {
    U8 rt{};
    auto res = std::from_chars(sv.data(), sv.data() + sv.size(), rt.data);
    if (res.ec != std::errc{}) {
      return DEBUG_FAIL("Failed to convert number literal to u8 value.");
    }
    return rt;
  }
};

struct U16 {
  using DataT = std::uint16_t;
  DataT data;

  static constexpr ClRes<U16> FromLiteral(std::string_view sv) noexcept {
    U16 rt{};
    auto res = std::from_chars(sv.data(), sv.data() + sv.size(), rt.data);
    if (res.ec != std::errc{}) {
      return DEBUG_FAIL("Failed to convert number literal to u16 value.");
    }
    return rt;
  }
};

struct U32 {
  using DataT = std::uint32_t;
  DataT data;

  static constexpr ClRes<U32> FromLiteral(std::string_view sv) noexcept {
    U32 rt{};
    auto res = std::from_chars(sv.data(), sv.data() + sv.size(), rt.data);
    if (res.ec != std::errc{}) {
      return DEBUG_FAIL("Failed to convert number literal to u32 value.");
    }
    return rt;
  }
};

struct U64 {
  using DataT = std::uint64_t;
  DataT data;

  static constexpr ClRes<U64> FromLiteral(std::string_view sv) noexcept {
    U64 rt{};
    auto res = std::from_chars(sv.data(), sv.data() + sv.size(), rt.data);
    if (res.ec != std::errc{}) {
      return DEBUG_FAIL("Failed to convert number literal to u64 value.");
    }
    return rt;
  }
};

struct F32 {
  using DataT = float;
  DataT data;

  static ClRes<F32> FromLiteral(std::string_view sv) noexcept {
    F32 rt{};
    auto res = std::from_chars(sv.data(), sv.data() + sv.size(), rt.data);
    if (res.ec != std::errc{}) {
      return DEBUG_FAIL("Failed to convert number literal to f32 value.");
    }
    return rt;
  }
};

struct F64 {
  using DataT = double;
  DataT data;

  static ClRes<F64> FromLiteral(std::string_view sv) noexcept {
    F64 rt{};
    auto res = std::from_chars(sv.data(), sv.data() + sv.size(), rt.data);
    if (res.ec != std::errc{}) {
      return DEBUG_FAIL("Failed to convert number literal to f64 value.");
    }
    return rt;
  }
};

struct CStr {
  using DataT = const char*;
  DataT data;

  constexpr bool operator==(const CStr& rhs) const noexcept {
    if (std::is_constant_evaluated())
      return std::string_view(data) != rhs.data;
    else
      return std::strcmp(data, rhs.data) != 0;
  }

  constexpr bool operator!=(const CStr& rhs) const noexcept {
    if (std::is_constant_evaluated())
      return std::string_view(data) != rhs.data;
    else
      return std::strcmp(data, rhs.data) != 0;
  }

  static constexpr ClRes<CStr> FromLiteral(std::string_view sv) noexcept { return CStr{.data = sv.data()}; }
};

struct Ptr {
  using DataT = AnyValue*;
  DataT data;
};

struct String {
  using DataT = std::string;
  DataT data;
};

struct Array {
  using DataT = std::vector<AnyValue>;
  DataT data;
};

struct Map {
  using DataT = std::unordered_map<String::DataT, AnyValue>;
  DataT data;
};

struct StrIter {
  using DataT = std::string::iterator;
  DataT data;
};

struct ArrayIter {
  using DataT = std::vector<AnyValue>::iterator;
  DataT data;
};

struct MapIter {
  using DataT = std::unordered_map<String::DataT, AnyValue>::iterator;
  DataT data;
};

struct AnyValue : public AnyVariantBase {
  using variant::variant;

  constexpr eTypeIndex TypeIndex() const { return static_cast<eTypeIndex>(index()); }

  template <class T>
  static constexpr AnyValue Make() noexcept {
    return RefObject<T>{};
  }

  template <class T, class... ArgTs>
  static constexpr AnyValue Make(ArgTs... args) noexcept {
    return RefObject<T>::New(kInplaceConstructor, std::forward<ArgTs>(args)...);
  }

  static constexpr AnyValue MakeInvalidOpError() noexcept {
    return RefObject<Error>::New(kInplaceConstructor, "Invalid Operation");
  }

  template <typename T>
  static constexpr AnyValue MakeCopy(const T& other) noexcept {
    return other.Copy();
  }

  template <typename T>
  static constexpr AnyValue MakeMove(T& other) noexcept {
    return other.Move();
  }

  template <typename T>
  static constexpr AnyValue MakeRef(T& other) noexcept {
    return other.MakeRef();
  }

  template <typename T>
  static constexpr AnyValue WeakRef(T& other) noexcept {
    return other.NewWeak(other.ptr);
  }

  static constexpr AnyValue Copy(const AnyValue& other) noexcept {
    if (Is<Map>(other)) {
      // Visit each key-value pair and copy them
      auto& map = CppRef<Map>(other);
      Map new_map;
      for (auto& [key, value] : map.data) {
        new_map.data[key] = Copy(value);
      }
      return Make<Map>(new_map);
    } else
      return std::visit([&](auto&& arg) -> AnyValue { return arg.Copy(); }, other);
  }

  static constexpr AnyValue Move(AnyValue& other) noexcept {
    return std::visit([&](auto&& arg) -> AnyValue { return MakeMove(arg); }, other);
  }

  static constexpr AnyValue Ref(AnyValue& other) noexcept {
    return std::visit([&](auto&& arg) -> AnyValue { return MakeRef(arg); }, other);
  }

  static constexpr AnyValue WeakRef(AnyValue& other) noexcept {
    return std::visit([&](auto&& arg) -> AnyValue { return WeakRef(arg); }, other);
  }

  template <typename T>
  static constexpr bool Is(const AnyValue& self) noexcept {
    return std::holds_alternative<RefObject<T>>(self);
  }

  template <typename T>
  static constexpr RefObject<T>& Ref(AnyValue& self) {
    return std::get<RefObject<T>>(self);
  }

  template <typename T>
  static constexpr const RefObject<T>& Ref(const AnyValue& self) {
    return std::get<RefObject<T>>(self);
  }

  template <typename T>
  static constexpr T& CppRef(AnyValue& self) {
    return Ref<T>(self).Self();
  }

  template <typename T>
  static constexpr const T& CppRef(const AnyValue& self) {
    return Ref<T>(self).ConstSelf();
  }

  static constexpr AnyValue& SetAs(AnyValue& self, AnyValue&& value) {
    self = std::forward<AnyValue>(value);
    return self;
  }

  template <typename T>
  static constexpr AnyValue& SetAs(AnyValue& self, T&& value) {
    self = AnyValue::Make<T>(std::forward<T>(value));
    return self;
  }

  template <class T>
    requires std::same_as<T, I32>
  static constexpr AnyValue GetAs(const AnyValue& self) {
    if (Is<I32>(self))
      return Copy(self);
    else if (Is<Bool>(self))
      return Make<I32>(CppRef<Bool>(self));
    else if (Is<U64>(self))
      return Make<I32>(CppRef<U64>(self));
    else if (Is<F32>(self))
      return Make<I32>(CppRef<F32>(self));
    else if (Is<I8>(self))
      return Make<I32>(CppRef<I8>(self));
    else if (Is<U8>(self))
      return Make<I32>(CppRef<U8>(self));
    else if (Is<String>(self))
      return Make<I32>(std::stoi(CppRef<String>(self).data));
    else
      return MakeInvalidOpError();
  }

  template <class T>
    requires std::same_as<T, Bool>
  static constexpr AnyValue GetAs(const AnyValue& self) {
    if (Is<I32>(self)) {
      return Make<Bool>(CppRef<I32>(self));
    } else if (Is<Bool>(self)) {
      return Copy(self);
    } else if (Is<U64>(self)) {
      return Make<Bool>(CppRef<U64>(self));
    } else if (Is<F32>(self)) {
      return Make<Bool>(CppRef<F32>(self));
    } else if (Is<I8>(self)) {
      return Make<Bool>(CppRef<I8>(self));
    } else if (Is<U8>(self)) {
      return Make<Bool>(CppRef<U8>(self));
    } else if (Is<String>(self)) {
      return Make<Bool>(!CppRef<String>(self).data.empty());
    } else if (Is<Array>(self)) {
      return Make<Bool>(!CppRef<Array>(self).data.empty());
    } /*else if (Is<Map>(self)) {
      return Make<Bool>(!CppRef<Map>(self).empty());
    }*/
    else {
      return MakeInvalidOpError();
    }
  }

  template <class T>
    requires std::same_as<T, U64>
  static constexpr AnyValue GetAs(const AnyValue& self) {
    if (Is<I32>(self)) {
      return Make<U64>(CppRef<I32>(self));
    } else if (Is<Bool>(self)) {
      return Make<U64>(CppRef<Bool>(self));
    } else if (Is<U64>(self)) {
      return Copy(self);
    } else if (Is<F32>(self)) {
      return Make<U64>(CppRef<F32>(self));
    } else if (Is<I8>(self)) {
      return Make<U64>(CppRef<I8>(self));
    } else if (Is<U8>(self)) {
      return Make<U64>(CppRef<U8>(self));
    } else if (Is<String>(self)) {
      return Make<U64>(std::stoull(CppRef<String>(self)));
    } else {
      return MakeInvalidOpError();
    }
  }

  template <class T>
    requires std::same_as<T, F32>
  static constexpr AnyValue GetAs(const AnyValue& self) {
    if (Is<I32>(self)) {
      return Make<F32>(CppRef<I32>());
    } else if (Is<Bool>(self)) {
      return Make<F32>(CppRef<Bool>());
    } else if (Is<U64>(self)) {
      return Make<F32>(CppRef<U64>());
    } else if (Is<F32>(self)) {
      return Copy(self);
    } else if (Is<I8>(self)) {
      return Make<F32>(CppRef<I8>());
    } else if (Is<U8>(self)) {
      return Make<F32>(CppRef<U8>());
    } else if (Is<String>(self)) {
      return Make<F32>(std::stod(CppRef<String>(self)));
    } else {
      return MakeInvalidOpError();
    }
  }

  template <class T>
    requires std::same_as<T, I8>
  static constexpr AnyValue GetAs(const AnyValue& self) {
    if (Is<I32>(self)) {
      return Make<I8>(CppRef<I32>());
    } else if (Is<Bool>(self)) {
      return Make<I8>(CppRef<Bool>());
    } else if (Is<U64>(self)) {
      return Make<I8>(CppRef<U64>());
    } else if (Is<F32>(self)) {
      return Make<I8>(CppRef<F32>());
    } else if (Is<I8>(self)) {
      return Copy(self);
    } else if (Is<U8>(self)) {
      return Make<I8>(CppRef<U8>());
    } else {
      return MakeInvalidOpError();
    }
  }

  template <class T>
    requires std::same_as<T, String>
  static constexpr AnyValue GetAs(const AnyValue& self) {
    if (Is<String>(self)) {
      return Copy(self);
    } else if (Is<I32>(self)) {
      return Make<String>(std::to_string(CppRef<I32>(self)));
    } else if (Is<Bool>(self)) {
      return Make<String>(CppRef<Bool>(self) ? "true" : "false");
    } else if (Is<U64>(self)) {
      return Make<String>(std::to_string(CppRef<U64>(self)));
    } else if (Is<F32>(self)) {
      return Make<String>(std::to_string(CppRef<F32>(self)));
    } else if (Is<I8>(self)) {
      return Make<String>(std::string(1, CppRef<I8>(self)));
    } else if (Is<U8>(self)) {
      return Make<String>(std::string(1, CppRef<U8>(self)));
    } else if (Is<Array>(self)) {
      std::string ret;
      ret += "[";
      for (auto& elem : CppRef<Array>(self)) {
        ret += CppRef<String>(elem.GetAs<String>(self));
        ret += ", ";
      }
      ret.pop_back();
      ret.pop_back();
      ret += "]";
      return Make<String>(std::move(ret));
    } else if (Is<Map>(self)) {
      std::string ret;
      ret += "{";
      for (auto& [key, value] : CppRef<Map>(self)) {
        ret += key;
        ret += " : ";
        ret += CppRef<String>(value.GetAs<String>(self));
        ret += ", ";
      }
      ret.pop_back();
      ret.pop_back();
      ret += "}";
      return Make<String>(std::move(ret));
    } else {
      return MakeInvalidOpError();
    }
  }

  // template <class T>
  //   requires std::same_as<T, I32>
  // static constexpr I32 CppGetAs(const AnyValue& self) {
  //   if (Is<I32>(self))
  //     return Copy(self);
  //   else if (Is<Bool>(self))
  //     return CppRef<Bool>(self);
  //   else if (Is<U64>(self))
  //     return CppRef<U64>(self);
  //   else if (Is<F32>(self))
  //     return CppRef<F32>(self);
  //   else if (Is<I8>(self))
  //     return CppRef<I8>(self);
  //   else if (Is<U8>(self))
  //     return CppRef<U8>(self);
  //   else if (Is<String>(self))
  //     return std::stoi(CppRef<String>(self));
  //   else
  //     throw "InconvertibleTypes";
  // }

  // template <class T>
  //   requires std::same_as<T, Bool>
  // static constexpr Bool CppGetAs(const AnyValue& self) {
  //   if (Is<I32>(self)) {
  //     return CppRef<I32>(self);
  //   } else if (Is<Bool>(self)) {
  //     return Copy(self);
  //   } else if (Is<U64>(self)) {
  //     return CppRef<U64>(self);
  //   } else if (Is<F32>(self)) {
  //     return CppRef<F32>(self);
  //   } else if (Is<I8>(self)) {
  //     return CppRef<I8>(self);
  //   } else if (Is<U8>(self)) {
  //     return CppRef<U8>(self);
  //   } else if (Is<String>(self)) {
  //     return !CppRef<String>(self).data.empty();
  //   } else if (Is<Array>(self)) {
  //     return !CppRef<Array>(self).data.empty();
  //   } else if (Is<Map>(self)) {
  //     return !CppRef<Map>(self).data.empty();
  //   } else {
  //     throw "InconvertibleTypes";
  //   }
  // }

  // template <class T>
  //   requires std::same_as<T, U64>
  // static constexpr U64 CppGetAs(const AnyValue& self) {
  //   if (Is<I32>(self)) {
  //     return CppRef<I32>(self);
  //   } else if (Is<Bool>(self)) {
  //     return CppRef<Bool>(self);
  //   } else if (Is<U64>(self)) {
  //     return CppRef<Bool>(self);
  //   } else if (Is<F32>(self)) {
  //     return CppRef<F32>(self);
  //   } else if (Is<I8>(self)) {
  //     return CppRef<I8>(self);
  //   } else if (Is<U8>(self)) {
  //     return CppRef<U8>(self);
  //   } else if (Is<String>(self)) {
  //     return std::stoull(CppRef<String>(self));
  //   } else {
  //     throw "InconvertibleTypes";
  //   }
  // }

  // template <class T>
  //   requires std::same_as<T, F32>
  // static constexpr F32 CppGetAs(const AnyValue& self) {
  //   if (Is<I32>(self)) {
  //     return (CppRef<I32>(self));
  //   } else if (Is<Bool>(self)) {
  //     return (CppRef<Bool>(self));
  //   } else if (Is<U64>(self)) {
  //     return (CppRef<U64>(self));
  //   } else if (Is<F32>(self)) {
  //     return CppRef<F32>(self);
  //   } else if (Is<I8>(self)) {
  //     return (CppRef<I8>(self));
  //   } else if (Is<U8>(self)) {
  //     return (CppRef<U8>(self));
  //   } else if (Is<String>(self)) {
  //     return (std::stod(CppRef<String>(self)));
  //   } else {
  //     throw "InconvertibleTypes";
  //   }
  // }

  // template <class T>
  //   requires std::same_as<T, I8>
  // static constexpr I8 CppGetAs(const AnyValue& self) {
  //   if (Is<I32>(self)) {
  //     return CppRef<I32>(self);
  //   } else if (Is<Bool>(self)) {
  //     return CppRef<Bool>(self);
  //   } else if (Is<U64>(self)) {
  //     return CppRef<U64>(self);
  //   } else if (Is<F32>(self)) {
  //     return CppRef<F32>();
  //   } else if (Is<I8>(self)) {
  //     return CppRef<I8>(self);
  //   } else if (Is<U8>(self)) {
  //     return CppRef<U8>(self);
  //   } else {
  //     throw "InconvertibleTypes";
  //   }
  // }

  // template <class T>
  //   requires std::same_as<T, String>
  // static constexpr String CppGetAs(const AnyValue& self) {
  //   if (Is<String>(self)) {
  //     return CppRef<String>(self);
  //   } else if (Is<I32>(self)) {
  //     return (std::to_string(CppRef<I32>(self)));
  //   } else if (Is<Bool>(self)) {
  //     return (CppRef<Bool>(self) ? "true" : "false");
  //   } else if (Is<U64>(self)) {
  //     return (std::to_string(CppRef<U64>(self)));
  //   } else if (Is<F32>(self)) {
  //     return (std::to_string(CppRef<F32>(self)));
  //   } else if (Is<I8>(self)) {
  //     return (std::string(1, CppRef<I8>(self)));
  //   } else if (Is<U8>(self)) {
  //     return (std::string(1, CppRef<U8>(self)));
  //   } else if (Is<Array>(self)) {
  //     std::string ret;
  //     ret += "[";
  //     for (auto& elem : CppRef<Array>(self)) {
  //       ret += elem.CppGetAs<String>(self);
  //       ret += ", ";
  //     }
  //     ret.pop_back();
  //     ret.pop_back();
  //     ret += "]";
  //     return (std::move(ret));
  //   } else if (Is<Map>(self)) {
  //     std::string ret;
  //     ret += "{";
  //     for (auto& [key, value] : CppRef<Map>(self)) {
  //       ret += key;
  //       ret += " : ";
  //       ret += CppGetAs<String>(self).data;
  //       ret += ", ";
  //     }
  //     ret.pop_back();
  //     ret.pop_back();
  //     ret += "}";
  //     return (std::move(ret));
  //   } else {
  //     throw "InconvertibleTypes";
  //   }
  // }

  template <class T>
    requires(!std::is_same_v<T, AnyValue>)
  static constexpr AnyValue Add(const AnyValue& lhs, const T& rhs) {
    switch (lhs.TypeIndex()) {
      case eTypeIndex::I8:
        return Make<I32>(CppRef<I8>(lhs).data + rhs.data);
      case eTypeIndex::I16:
        return Make<I32>(CppRef<I16>(lhs).data + rhs.data);
      case eTypeIndex::I32:
        return Make<I32>(CppRef<I32>(lhs).data + rhs.data);
      case eTypeIndex::I64:
        return Make<I64>(CppRef<I64>(lhs).data + rhs.data);
      case eTypeIndex::Bool:
        return Make<U32>(CppRef<Bool>(lhs).data + rhs.data);
      case eTypeIndex::U8:
        return Make<U32>(CppRef<U8>(lhs).data + rhs.data);
      case eTypeIndex::U16:
        return Make<U32>(CppRef<U16>(lhs).data + rhs.data);
      case eTypeIndex::U32:
        return Make<U32>(CppRef<U32>(lhs).data + rhs.data);
      case eTypeIndex::U64:
        return Make<U64>(CppRef<U64>(lhs).data + rhs.data);
      case eTypeIndex::F32:
        return Make<F32>(CppRef<F32>(lhs).data + rhs.data);
      case eTypeIndex::F64:
        return Make<F64>(CppRef<F64>(lhs).data + rhs.data);
      default:
        return MakeInvalidOpError();
    }
  }

  static constexpr AnyValue Add(const AnyValue& self, const AnyValue& rhs) {
    switch (rhs.TypeIndex()) {
      case eTypeIndex::I8:
        return Add(self, CppRef<I32>(rhs));
      case eTypeIndex::I16:
        return Add(self, CppRef<I16>(rhs));
      case eTypeIndex::I32:
        return Add(self, CppRef<I32>(rhs));
      case eTypeIndex::I64:
        return Add(self, CppRef<I64>(rhs));
      case eTypeIndex::Bool:
        return Add(self, CppRef<Bool>(rhs));
      case eTypeIndex::U8:
        return Add(self, CppRef<U8>(rhs));
      case eTypeIndex::U16:
        return Add(self, CppRef<U16>(rhs));
      case eTypeIndex::U32:
        return Add(self, CppRef<U32>(rhs));
      case eTypeIndex::U64:
        return Add(self, CppRef<U64>(rhs));
      case eTypeIndex::F32:
        return Add(self, CppRef<F32>(rhs));
      case eTypeIndex::F64:
        return Add(self, CppRef<F64>(rhs));
      default:
        return MakeInvalidOpError();
    }
  }

  template <class T>
    requires(!std::is_same_v<T, AnyValue>)
  static constexpr AnyValue Sub(const AnyValue& lhs, const T& rhs) {
    switch (lhs.TypeIndex()) {
      case eTypeIndex::I8:
        return Make<I32>(CppRef<I8>(lhs).data - rhs.data);
      case eTypeIndex::I16:
        return Make<I32>(CppRef<I16>(lhs).data - rhs.data);
      case eTypeIndex::I32:
        return Make<I32>(CppRef<I32>(lhs).data - rhs.data);
      case eTypeIndex::I64:
        return Make<I64>(CppRef<I64>(lhs).data - rhs.data);
      case eTypeIndex::Bool:
        return Make<U32>(CppRef<Bool>(lhs).data - rhs.data);
      case eTypeIndex::U8:
        return Make<U32>(CppRef<U8>(lhs).data - rhs.data);
      case eTypeIndex::U16:
        return Make<U32>(CppRef<U16>(lhs).data - rhs.data);
      case eTypeIndex::U32:
        return Make<U32>(CppRef<U32>(lhs).data - rhs.data);
      case eTypeIndex::U64:
        return Make<U64>(CppRef<U64>(lhs).data - rhs.data);
      case eTypeIndex::F32:
        return Make<F32>(CppRef<F32>(lhs).data - rhs.data);
      case eTypeIndex::F64:
        return Make<F64>(CppRef<F64>(lhs).data - rhs.data);
      default:
        return MakeInvalidOpError();
    }
  }

  static constexpr AnyValue Sub(const AnyValue& self, const AnyValue& rhs) {
    switch (rhs.TypeIndex()) {
      case eTypeIndex::I8:
        return Sub(self, CppRef<I32>(rhs));
      case eTypeIndex::I16:
        return Sub(self, CppRef<I16>(rhs));
      case eTypeIndex::I32:
        return Sub(self, CppRef<I32>(rhs));
      case eTypeIndex::I64:
        return Sub(self, CppRef<I64>(rhs));
      case eTypeIndex::Bool:
        return Sub(self, CppRef<Bool>(rhs));
      case eTypeIndex::U8:
        return Sub(self, CppRef<U8>(rhs));
      case eTypeIndex::U16:
        return Sub(self, CppRef<U16>(rhs));
      case eTypeIndex::U32:
        return Sub(self, CppRef<U32>(rhs));
      case eTypeIndex::U64:
        return Sub(self, CppRef<U64>(rhs));
      case eTypeIndex::F32:
        return Sub(self, CppRef<F32>(rhs));
      case eTypeIndex::F64:
        return Sub(self, CppRef<F64>(rhs));
      default:
        return MakeInvalidOpError();
    }
  }

  template <class T>
    requires(!std::is_same_v<T, AnyValue>)
  static constexpr AnyValue Mul(const AnyValue& lhs, const T& rhs) {
    switch (lhs.TypeIndex()) {
      case eTypeIndex::I8:
        return Make<I32>(CppRef<I8>(lhs).data * rhs.data);
      case eTypeIndex::I16:
        return Make<I32>(CppRef<I16>(lhs).data * rhs.data);
      case eTypeIndex::I32:
        return Make<I32>(CppRef<I32>(lhs).data * rhs.data);
      case eTypeIndex::I64:
        return Make<I64>(CppRef<I64>(lhs).data * rhs.data);
      case eTypeIndex::Bool:
        return Make<U32>(CppRef<Bool>(lhs).data * rhs.data);
      case eTypeIndex::U8:
        return Make<U32>(CppRef<U8>(lhs).data * rhs.data);
      case eTypeIndex::U16:
        return Make<U32>(CppRef<U16>(lhs).data * rhs.data);
      case eTypeIndex::U32:
        return Make<U32>(CppRef<U32>(lhs).data * rhs.data);
      case eTypeIndex::U64:
        return Make<U64>(CppRef<U64>(lhs).data * rhs.data);
      case eTypeIndex::F32:
        return Make<F32>(CppRef<F32>(lhs).data * rhs.data);
      case eTypeIndex::F64:
        return Make<F64>(CppRef<F64>(lhs).data * rhs.data);
      default:
        return MakeInvalidOpError();
    }
  }

  static constexpr AnyValue Mul(const AnyValue& self, const AnyValue& rhs) {
    switch (rhs.TypeIndex()) {
      case eTypeIndex::I8:
        return Mul(self, CppRef<I32>(rhs));
      case eTypeIndex::I16:
        return Mul(self, CppRef<I16>(rhs));
      case eTypeIndex::I32:
        return Mul(self, CppRef<I32>(rhs));
      case eTypeIndex::I64:
        return Mul(self, CppRef<I64>(rhs));
      case eTypeIndex::Bool:
        return Mul(self, CppRef<Bool>(rhs));
      case eTypeIndex::U8:
        return Mul(self, CppRef<U8>(rhs));
      case eTypeIndex::U16:
        return Mul(self, CppRef<U16>(rhs));
      case eTypeIndex::U32:
        return Mul(self, CppRef<U32>(rhs));
      case eTypeIndex::U64:
        return Mul(self, CppRef<U64>(rhs));
      case eTypeIndex::F32:
        return Mul(self, CppRef<F32>(rhs));
      case eTypeIndex::F64:
        return Mul(self, CppRef<F64>(rhs));
      default:
        return MakeInvalidOpError();
    }
  }

  template <class T>
    requires(!std::is_same_v<T, AnyValue>)
  static constexpr AnyValue Div(const AnyValue& lhs, const T& rhs) {
    switch (lhs.TypeIndex()) {
      case eTypeIndex::I8:
        return Make<I32>(CppRef<I8>(lhs).data / rhs.data);
      case eTypeIndex::I16:
        return Make<I32>(CppRef<I16>(lhs).data / rhs.data);
      case eTypeIndex::I32:
        return Make<I32>(CppRef<I32>(lhs).data / rhs.data);
      case eTypeIndex::I64:
        return Make<I64>(CppRef<I64>(lhs).data / rhs.data);
      case eTypeIndex::Bool:
        return Make<U32>(CppRef<Bool>(lhs).data / rhs.data);
      case eTypeIndex::U8:
        return Make<U32>(CppRef<U8>(lhs).data / rhs.data);
      case eTypeIndex::U16:
        return Make<U32>(CppRef<U16>(lhs).data / rhs.data);
      case eTypeIndex::U32:
        return Make<U32>(CppRef<U32>(lhs).data / rhs.data);
      case eTypeIndex::U64:
        return Make<U64>(CppRef<U64>(lhs).data / rhs.data);
      case eTypeIndex::F32:
        return Make<F32>(CppRef<F32>(lhs).data / rhs.data);
      case eTypeIndex::F64:
        return Make<F64>(CppRef<F64>(lhs).data / rhs.data);
      default:
        return MakeInvalidOpError();
    }
  }

  static constexpr AnyValue Div(const AnyValue& self, const AnyValue& rhs) {
    switch (rhs.TypeIndex()) {
      case eTypeIndex::I8:
        return Div(self, CppRef<I32>(rhs));
      case eTypeIndex::I16:
        return Div(self, CppRef<I16>(rhs));
      case eTypeIndex::I32:
        return Div(self, CppRef<I32>(rhs));
      case eTypeIndex::I64:
        return Div(self, CppRef<I64>(rhs));
      case eTypeIndex::Bool:
        return Div(self, CppRef<Bool>(rhs));
      case eTypeIndex::U8:
        return Div(self, CppRef<U8>(rhs));
      case eTypeIndex::U16:
        return Div(self, CppRef<U16>(rhs));
      case eTypeIndex::U32:
        return Div(self, CppRef<U32>(rhs));
      case eTypeIndex::U64:
        return Div(self, CppRef<U64>(rhs));
      case eTypeIndex::F32:
        return Div(self, CppRef<F32>(rhs));
      case eTypeIndex::F64:
        return Div(self, CppRef<F64>(rhs));
      default:
        return MakeInvalidOpError();
    }
  }

    template <iIntegral T>
  static constexpr AnyValue Mod(const AnyValue& lhs, const T& rhs) {
    switch (lhs.TypeIndex()) {
      case eTypeIndex::I8:
        return Make<I32>(CppRef<I8>(lhs).data % rhs.data);
      case eTypeIndex::I16:
        return Make<I32>(CppRef<I16>(lhs).data % rhs.data);
      case eTypeIndex::I32:
        return Make<I32>(CppRef<I32>(lhs).data % rhs.data);
      case eTypeIndex::I64:
        return Make<I64>(CppRef<I64>(lhs).data % rhs.data);
      case eTypeIndex::Bool:
        return Make<U32>(CppRef<Bool>(lhs).data % rhs.data);
      case eTypeIndex::U8:
        return Make<U32>(CppRef<U8>(lhs).data % rhs.data);
      case eTypeIndex::U16:
        return Make<U32>(CppRef<U16>(lhs).data % rhs.data);
      case eTypeIndex::U32:
        return Make<U32>(CppRef<U32>(lhs).data % rhs.data);
      case eTypeIndex::U64:
        return Make<U64>(CppRef<U64>(lhs).data % rhs.data);
      case eTypeIndex::F32:
        return Make<F32>(std::fmodf(CppRef<F32>(lhs).data, rhs.data));
      case eTypeIndex::F64:
        return Make<F64>(std::fmod(CppRef<F64>(lhs).data, rhs.data));
      default:
        return MakeInvalidOpError();
    }
  }

    template <std::same_as<F32> T>
  static constexpr AnyValue Mod(const AnyValue& lhs, const T& rhs) {
    switch (lhs.TypeIndex()) {
      case eTypeIndex::I8:
        return Make<F64>(std::fmodf(CppRef<I8>(lhs).data, rhs.data));
      case eTypeIndex::I16:
        return Make<F64>(std::fmodf(CppRef<I16>(lhs).data, rhs.data));
      case eTypeIndex::I32:
        return Make<F64>(std::fmodf(CppRef<I32>(lhs).data, rhs.data));
      case eTypeIndex::I64:
        return Make<F64>(std::fmodf(CppRef<I64>(lhs).data, rhs.data));
      case eTypeIndex::Bool:
        return Make<F64>(std::fmodf(CppRef<Bool>(lhs).data, rhs.data));
      case eTypeIndex::U8:
        return Make<F64>(std::fmodf(CppRef<U8>(lhs).data, rhs.data));
      case eTypeIndex::U16:
        return Make<F64>(std::fmodf(CppRef<U16>(lhs).data, rhs.data));
      case eTypeIndex::U32:
        return Make<F64>(std::fmodf(CppRef<U32>(lhs).data, rhs.data));
      case eTypeIndex::U64:
        return Make<F64>(std::fmodf(CppRef<U64>(lhs).data, rhs.data));
      case eTypeIndex::F32:
        return Make<F32>(std::fmodf(CppRef<F32>(lhs).data, rhs.data));
      case eTypeIndex::F64:
        return Make<F64>(std::fmod(CppRef<F64>(lhs).data, rhs.data));
      default:
        return MakeInvalidOpError();
    }
  }

    template <std::same_as<F64> T>
  static constexpr AnyValue Mod(const AnyValue& lhs, const T& rhs) {
    switch (lhs.TypeIndex()) {
      case eTypeIndex::I8:
        return Make<F64>(std::fmod(CppRef<I8>(lhs).data, rhs.data));
      case eTypeIndex::I16:
        return Make<F64>(std::fmod(CppRef<I16>(lhs).data, rhs.data));
      case eTypeIndex::I32:
        return Make<F64>(std::fmod(CppRef<I32>(lhs).data, rhs.data));
      case eTypeIndex::I64:
        return Make<F64>(std::fmod(CppRef<I64>(lhs).data, rhs.data));
      case eTypeIndex::Bool:
        return Make<F64>(std::fmod(CppRef<Bool>(lhs).data, rhs.data));
      case eTypeIndex::U8:
        return Make<F64>(std::fmod(CppRef<U8>(lhs).data, rhs.data));
      case eTypeIndex::U16:
        return Make<F64>(std::fmod(CppRef<U16>(lhs).data, rhs.data));
      case eTypeIndex::U32:
        return Make<F64>(std::fmod(CppRef<U32>(lhs).data, rhs.data));
      case eTypeIndex::U64:
        return Make<F64>(std::fmod(CppRef<U64>(lhs).data, rhs.data));
      case eTypeIndex::F32:
        return Make<F32>(std::fmod(CppRef<F32>(lhs).data, rhs.data));
      case eTypeIndex::F64:
        return Make<F64>(std::fmod(CppRef<F64>(lhs).data, rhs.data));
      default:
        return MakeInvalidOpError();
    }
  }

  static constexpr AnyValue Mod(const AnyValue& self, const AnyValue& rhs) {
    switch (rhs.TypeIndex()) {
      case eTypeIndex::I8:
        return Mod(self, CppRef<I32>(rhs));
      case eTypeIndex::I16:
        return Mod(self, CppRef<I16>(rhs));
      case eTypeIndex::I32:
        return Mod(self, CppRef<I32>(rhs));
      case eTypeIndex::I64:
        return Mod(self, CppRef<I64>(rhs));
      case eTypeIndex::Bool:
        return Mod(self, CppRef<Bool>(rhs));
      case eTypeIndex::U8:
        return Mod(self, CppRef<U8>(rhs));
      case eTypeIndex::U16:
        return Mod(self, CppRef<U16>(rhs));
      case eTypeIndex::U32:
        return Mod(self, CppRef<U32>(rhs));
      case eTypeIndex::U64:
        return Mod(self, CppRef<U64>(rhs));
      case eTypeIndex::F32:
        return Mod(self, CppRef<F32>(rhs));
      case eTypeIndex::F64:
        return Mod(self, CppRef<F64>(rhs));
      default:
        return MakeInvalidOpError();
    }
  }

  template <class T>
    requires(!std::is_same_v<T, AnyValue>)
  static constexpr AnyValue And(const AnyValue& lhs, const T& rhs) {
    switch (lhs.TypeIndex()) {
      case eTypeIndex::I8:
        return Make<I32>(CppRef<I8>(lhs).data / rhs.data);
      case eTypeIndex::I16:
        return Make<I32>(CppRef<I16>(lhs).data / rhs.data);
      case eTypeIndex::I32:
        return Make<I32>(CppRef<I32>(lhs).data / rhs.data);
      case eTypeIndex::I64:
        return Make<I64>(CppRef<I64>(lhs).data / rhs.data);
      case eTypeIndex::Bool:
        return Make<U32>(CppRef<Bool>(lhs).data / rhs.data);
      case eTypeIndex::U8:
        return Make<U32>(CppRef<U8>(lhs).data / rhs.data);
      case eTypeIndex::U16:
        return Make<U32>(CppRef<U16>(lhs).data / rhs.data);
      case eTypeIndex::U32:
        return Make<U32>(CppRef<U32>(lhs).data / rhs.data);
      case eTypeIndex::U64:
        return Make<U64>(CppRef<U64>(lhs).data / rhs.data);
      case eTypeIndex::F32:
        return Make<F32>(CppRef<F32>(lhs).data / rhs.data);
      case eTypeIndex::F64:
        return Make<F64>(CppRef<F64>(lhs).data / rhs.data);
      default:
        return MakeInvalidOpError();
    }
  }

  static constexpr AnyValue And(const AnyValue& self, const AnyValue& rhs) {
    switch (rhs.TypeIndex()) {
      case eTypeIndex::I8:
        return Div(self, CppRef<I32>(rhs));
      case eTypeIndex::I16:
        return Div(self, CppRef<I16>(rhs));
      case eTypeIndex::I32:
        return Div(self, CppRef<I32>(rhs));
      case eTypeIndex::I64:
        return Div(self, CppRef<I64>(rhs));
      case eTypeIndex::Bool:
        return Div(self, CppRef<Bool>(rhs));
      case eTypeIndex::U8:
        return Div(self, CppRef<U8>(rhs));
      case eTypeIndex::U16:
        return Div(self, CppRef<U16>(rhs));
      case eTypeIndex::U32:
        return Div(self, CppRef<U32>(rhs));
      case eTypeIndex::U64:
        return Div(self, CppRef<U64>(rhs));
      case eTypeIndex::F32:
        return Div(self, CppRef<F32>(rhs));
      case eTypeIndex::F64:
        return Div(self, CppRef<F64>(rhs));
      default:
        return MakeInvalidOpError();
    }
  }

  template <class T>
    requires(!std::is_same_v<T, AnyValue>)
  static constexpr AnyValue Or(const AnyValue& lhs, const T& rhs) {
    switch (lhs.TypeIndex()) {
      case eTypeIndex::I8:
        return Make<I32>(CppRef<I8>(lhs).data / rhs.data);
      case eTypeIndex::I16:
        return Make<I32>(CppRef<I16>(lhs).data / rhs.data);
      case eTypeIndex::I32:
        return Make<I32>(CppRef<I32>(lhs).data / rhs.data);
      case eTypeIndex::I64:
        return Make<I64>(CppRef<I64>(lhs).data / rhs.data);
      case eTypeIndex::Bool:
        return Make<U32>(CppRef<Bool>(lhs).data / rhs.data);
      case eTypeIndex::U8:
        return Make<U32>(CppRef<U8>(lhs).data / rhs.data);
      case eTypeIndex::U16:
        return Make<U32>(CppRef<U16>(lhs).data / rhs.data);
      case eTypeIndex::U32:
        return Make<U32>(CppRef<U32>(lhs).data / rhs.data);
      case eTypeIndex::U64:
        return Make<U64>(CppRef<U64>(lhs).data / rhs.data);
      case eTypeIndex::F32:
        return Make<F32>(CppRef<F32>(lhs).data / rhs.data);
      case eTypeIndex::F64:
        return Make<F64>(CppRef<F64>(lhs).data / rhs.data);
      default:
        return MakeInvalidOpError();
    }
  }

  static constexpr AnyValue Or(const AnyValue& self, const AnyValue& rhs) {
    switch (rhs.TypeIndex()) {
      case eTypeIndex::I8:
        return Div(self, CppRef<I32>(rhs));
      case eTypeIndex::I16:
        return Div(self, CppRef<I16>(rhs));
      case eTypeIndex::I32:
        return Div(self, CppRef<I32>(rhs));
      case eTypeIndex::I64:
        return Div(self, CppRef<I64>(rhs));
      case eTypeIndex::Bool:
        return Div(self, CppRef<Bool>(rhs));
      case eTypeIndex::U8:
        return Div(self, CppRef<U8>(rhs));
      case eTypeIndex::U16:
        return Div(self, CppRef<U16>(rhs));
      case eTypeIndex::U32:
        return Div(self, CppRef<U32>(rhs));
      case eTypeIndex::U64:
        return Div(self, CppRef<U64>(rhs));
      case eTypeIndex::F32:
        return Div(self, CppRef<F32>(rhs));
      case eTypeIndex::F64:
        return Div(self, CppRef<F64>(rhs));
      default:
        return MakeInvalidOpError();
    }
  }

  template <class T>
    requires(!std::is_same_v<T, AnyValue>)
  static constexpr AnyValue Xor(const AnyValue& lhs, const T& rhs) {
    switch (lhs.TypeIndex()) {
      case eTypeIndex::I8:
        return Make<I32>(CppRef<I8>(lhs).data / rhs.data);
      case eTypeIndex::I16:
        return Make<I32>(CppRef<I16>(lhs).data / rhs.data);
      case eTypeIndex::I32:
        return Make<I32>(CppRef<I32>(lhs).data / rhs.data);
      case eTypeIndex::I64:
        return Make<I64>(CppRef<I64>(lhs).data / rhs.data);
      case eTypeIndex::Bool:
        return Make<U32>(CppRef<Bool>(lhs).data / rhs.data);
      case eTypeIndex::U8:
        return Make<U32>(CppRef<U8>(lhs).data / rhs.data);
      case eTypeIndex::U16:
        return Make<U32>(CppRef<U16>(lhs).data / rhs.data);
      case eTypeIndex::U32:
        return Make<U32>(CppRef<U32>(lhs).data / rhs.data);
      case eTypeIndex::U64:
        return Make<U64>(CppRef<U64>(lhs).data / rhs.data);
      case eTypeIndex::F32:
        return Make<F32>(CppRef<F32>(lhs).data / rhs.data);
      case eTypeIndex::F64:
        return Make<F64>(CppRef<F64>(lhs).data / rhs.data);
      default:
        return MakeInvalidOpError();
    }
  }

  static constexpr AnyValue Xor(const AnyValue& self, const AnyValue& rhs) {
    switch (rhs.TypeIndex()) {
      case eTypeIndex::I8:
        return Div(self, CppRef<I32>(rhs));
      case eTypeIndex::I16:
        return Div(self, CppRef<I16>(rhs));
      case eTypeIndex::I32:
        return Div(self, CppRef<I32>(rhs));
      case eTypeIndex::I64:
        return Div(self, CppRef<I64>(rhs));
      case eTypeIndex::Bool:
        return Div(self, CppRef<Bool>(rhs));
      case eTypeIndex::U8:
        return Div(self, CppRef<U8>(rhs));
      case eTypeIndex::U16:
        return Div(self, CppRef<U16>(rhs));
      case eTypeIndex::U32:
        return Div(self, CppRef<U32>(rhs));
      case eTypeIndex::U64:
        return Div(self, CppRef<U64>(rhs));
      case eTypeIndex::F32:
        return Div(self, CppRef<F32>(rhs));
      case eTypeIndex::F64:
        return Div(self, CppRef<F64>(rhs));
      default:
        return MakeInvalidOpError();
    }
  }

  template <class T>
    requires(!std::is_same_v<T, AnyValue>)
  static constexpr AnyValue Lsh(const AnyValue& lhs, const T& rhs) {
    switch (lhs.TypeIndex()) {
      case eTypeIndex::I8:
        return Make<I32>(CppRef<I8>(lhs).data / rhs.data);
      case eTypeIndex::I16:
        return Make<I32>(CppRef<I16>(lhs).data / rhs.data);
      case eTypeIndex::I32:
        return Make<I32>(CppRef<I32>(lhs).data / rhs.data);
      case eTypeIndex::I64:
        return Make<I64>(CppRef<I64>(lhs).data / rhs.data);
      case eTypeIndex::Bool:
        return Make<U32>(CppRef<Bool>(lhs).data / rhs.data);
      case eTypeIndex::U8:
        return Make<U32>(CppRef<U8>(lhs).data / rhs.data);
      case eTypeIndex::U16:
        return Make<U32>(CppRef<U16>(lhs).data / rhs.data);
      case eTypeIndex::U32:
        return Make<U32>(CppRef<U32>(lhs).data / rhs.data);
      case eTypeIndex::U64:
        return Make<U64>(CppRef<U64>(lhs).data / rhs.data);
      case eTypeIndex::F32:
        return Make<F32>(CppRef<F32>(lhs).data / rhs.data);
      case eTypeIndex::F64:
        return Make<F64>(CppRef<F64>(lhs).data / rhs.data);
      default:
        return MakeInvalidOpError();
    }
  }

  static constexpr AnyValue Lsh(const AnyValue& self, const AnyValue& rhs) {
    switch (rhs.TypeIndex()) {
      case eTypeIndex::I8:
        return Div(self, CppRef<I32>(rhs));
      case eTypeIndex::I16:
        return Div(self, CppRef<I16>(rhs));
      case eTypeIndex::I32:
        return Div(self, CppRef<I32>(rhs));
      case eTypeIndex::I64:
        return Div(self, CppRef<I64>(rhs));
      case eTypeIndex::Bool:
        return Div(self, CppRef<Bool>(rhs));
      case eTypeIndex::U8:
        return Div(self, CppRef<U8>(rhs));
      case eTypeIndex::U16:
        return Div(self, CppRef<U16>(rhs));
      case eTypeIndex::U32:
        return Div(self, CppRef<U32>(rhs));
      case eTypeIndex::U64:
        return Div(self, CppRef<U64>(rhs));
      case eTypeIndex::F32:
        return Div(self, CppRef<F32>(rhs));
      case eTypeIndex::F64:
        return Div(self, CppRef<F64>(rhs));
      default:
        return MakeInvalidOpError();
    }
  }

  template <class T>
    requires(!std::is_same_v<T, AnyValue>)
  static constexpr AnyValue Rsh(const AnyValue& lhs, const T& rhs) {
    switch (lhs.TypeIndex()) {
      case eTypeIndex::I8:
        return Make<I32>(CppRef<I8>(lhs).data / rhs.data);
      case eTypeIndex::I16:
        return Make<I32>(CppRef<I16>(lhs).data / rhs.data);
      case eTypeIndex::I32:
        return Make<I32>(CppRef<I32>(lhs).data / rhs.data);
      case eTypeIndex::I64:
        return Make<I64>(CppRef<I64>(lhs).data / rhs.data);
      case eTypeIndex::Bool:
        return Make<U32>(CppRef<Bool>(lhs).data / rhs.data);
      case eTypeIndex::U8:
        return Make<U32>(CppRef<U8>(lhs).data / rhs.data);
      case eTypeIndex::U16:
        return Make<U32>(CppRef<U16>(lhs).data / rhs.data);
      case eTypeIndex::U32:
        return Make<U32>(CppRef<U32>(lhs).data / rhs.data);
      case eTypeIndex::U64:
        return Make<U64>(CppRef<U64>(lhs).data / rhs.data);
      case eTypeIndex::F32:
        return Make<F32>(CppRef<F32>(lhs).data / rhs.data);
      case eTypeIndex::F64:
        return Make<F64>(CppRef<F64>(lhs).data / rhs.data);
      default:
        return MakeInvalidOpError();
    }
  }

  static constexpr AnyValue Rsh(const AnyValue& self, const AnyValue& rhs) {
    switch (rhs.TypeIndex()) {
      case eTypeIndex::I8:
        return Div(self, CppRef<I32>(rhs));
      case eTypeIndex::I16:
        return Div(self, CppRef<I16>(rhs));
      case eTypeIndex::I32:
        return Div(self, CppRef<I32>(rhs));
      case eTypeIndex::I64:
        return Div(self, CppRef<I64>(rhs));
      case eTypeIndex::Bool:
        return Div(self, CppRef<Bool>(rhs));
      case eTypeIndex::U8:
        return Div(self, CppRef<U8>(rhs));
      case eTypeIndex::U16:
        return Div(self, CppRef<U16>(rhs));
      case eTypeIndex::U32:
        return Div(self, CppRef<U32>(rhs));
      case eTypeIndex::U64:
        return Div(self, CppRef<U64>(rhs));
      case eTypeIndex::F32:
        return Div(self, CppRef<F32>(rhs));
      case eTypeIndex::F64:
        return Div(self, CppRef<F64>(rhs));
      default:
        return MakeInvalidOpError();
    }
  }

  template <class T>
    requires(!std::is_same_v<T, AnyValue>)
  static constexpr AnyValue& IAdd(AnyValue& self, const T& rhs) {
    switch (self.TypeIndex()) {
      case eTypeIndex::I8:
        CppRef<I8>(self).data += rhs.data;
      case eTypeIndex::I16:
        CppRef<I16>(self).data += rhs.data;
      case eTypeIndex::I32:
        CppRef<I32>(self).data += rhs.data;
      case eTypeIndex::I64:
        CppRef<I64>(self).data += rhs.data;
      case eTypeIndex::Bool:
        CppRef<Bool>(self).data += rhs.data;
      case eTypeIndex::U8:
        CppRef<U8>(self).data += rhs.data;
      case eTypeIndex::U16:
        CppRef<U16>(self).data += rhs.data;
      case eTypeIndex::U32:
        CppRef<U32>(self).data += rhs.data;
      case eTypeIndex::U64:
        CppRef<U64>(self).data += rhs.data;
      case eTypeIndex::F32:
        CppRef<F32>(self).data += rhs.data;
      case eTypeIndex::F64:
        CppRef<F64>(self).data += rhs.data;
      default:
        SetAs(self, MakeInvalidOpError());
    }
    return self;
  }

  static constexpr AnyValue& IAdd(AnyValue& self, const AnyValue& rhs) {
    switch (rhs.TypeIndex()) {
      case eTypeIndex::I8:
        return IAdd(self, CppRef<I8>(rhs));
      case eTypeIndex::I16:
        return IAdd(self, CppRef<I16>(rhs));
      case eTypeIndex::I32:
        return IAdd(self, CppRef<I32>(rhs));
      case eTypeIndex::I64:
        return IAdd(self, CppRef<I64>(rhs));
      case eTypeIndex::Bool:
        return IAdd(self, CppRef<Bool>(rhs));
      case eTypeIndex::U8:
        return IAdd(self, CppRef<U8>(rhs));
      case eTypeIndex::U16:
        return IAdd(self, CppRef<U16>(rhs));
      case eTypeIndex::U32:
        return IAdd(self, CppRef<U32>(rhs));
      case eTypeIndex::U64:
        return IAdd(self, CppRef<U64>(rhs));
      case eTypeIndex::F32:
        return IAdd(self, CppRef<F32>(rhs));
      case eTypeIndex::F64:
        return IAdd(self, CppRef<F64>(rhs));
      default:
        SetAs(self, MakeInvalidOpError());
    }
    return self;
  }

  static AnyValue LessThan(const AnyValue& self, const I32& rhs) {
    if (Is<I32>(self))
      return Make<Bool>(CppRef<I32>(self).data < rhs.data);
    else if (Is<Bool>(self))
      return Make<Bool>(CppRef<Bool>(self).data < rhs.data);
    else if (Is<U64>(self))
      return Make<Bool>(CppRef<U64>(self).data < rhs.data);
    else if (Is<F32>(self))
      return Make<Bool>(CppRef<F32>(self).data < rhs.data);
    else if (Is<I8>(self))
      return Make<Bool>(CppRef<I8>(self).data < rhs.data);
    else if (Is<U8>(self))
      return Make<Bool>(CppRef<U8>(self).data < rhs.data);
    else if (Is<String>(self))
      return Make<Bool>(std::stoi(CppRef<String>(self).data) < rhs.data);
    else
      return MakeInvalidOpError();
  }

  static AnyValue LessThan(const AnyValue& self, const Bool& rhs) {
    if (Is<I32>(self))
      return Make<Bool>(CppRef<I32>(self).data < rhs.data);
    else if (Is<Bool>(self))
      return Make<Bool>(CppRef<Bool>(self).data < rhs.data);
    else if (Is<U64>(self))
      return Make<Bool>(CppRef<U64>(self).data < rhs.data);
    else if (Is<F32>(self))
      return Make<Bool>(CppRef<F32>(self).data < rhs.data);
    else if (Is<I8>(self))
      return Make<Bool>(CppRef<I8>(self).data < rhs.data);
    else if (Is<U8>(self))
      return Make<Bool>(CppRef<U8>(self).data < rhs.data);
    else
      return MakeInvalidOpError();
  }

  static AnyValue LessThan(const AnyValue& self, const U64& rhs) {
    if (Is<I32>(self))
      return Make<Bool>(CppRef<I32>(self).data < rhs.data);
    else if (Is<Bool>(self))
      return Make<Bool>(CppRef<Bool>(self).data < rhs.data);
    else if (Is<U64>(self))
      return Make<Bool>(CppRef<U64>(self).data < rhs.data);
    else if (Is<F32>(self))
      return Make<Bool>(CppRef<F32>(self).data < rhs.data);
    else if (Is<I8>(self))
      return Make<Bool>(CppRef<I8>(self).data < rhs.data);
    else if (Is<U8>(self))
      return Make<Bool>(CppRef<U8>(self).data < rhs.data);
    else
      return MakeInvalidOpError();
  }

  static AnyValue LessThan(const AnyValue& self, const F32& rhs) {
    if (Is<I32>(self))
      return Make<Bool>(CppRef<I32>(self).data < rhs.data);
    else if (Is<Bool>(self))
      return Make<Bool>(CppRef<Bool>(self).data < rhs.data);
    else if (Is<U64>(self))
      return Make<Bool>(CppRef<U64>(self).data < rhs.data);
    else if (Is<F32>(self))
      return Make<Bool>(CppRef<F32>(self).data < rhs.data);
    else if (Is<I8>(self))
      return Make<Bool>(CppRef<I8>(self).data < rhs.data);
    else if (Is<U8>(self))
      return Make<Bool>(CppRef<U8>(self).data < rhs.data);
    else
      return MakeInvalidOpError();
  }

  static AnyValue LessThan(const AnyValue& self, const I8& rhs) {
    if (Is<I32>(self))
      return Make<Bool>(CppRef<I32>(self).data < rhs.data);
    else if (Is<Bool>(self))
      return Make<Bool>(CppRef<Bool>(self).data < rhs.data);
    else if (Is<U64>(self))
      return Make<Bool>(CppRef<U64>(self).data < rhs.data);
    else if (Is<F32>(self))
      return Make<Bool>(CppRef<F32>(self).data < rhs.data);
    else if (Is<I8>(self))
      return Make<Bool>(CppRef<I8>(self).data < rhs.data);
    else if (Is<U8>(self))
      return Make<Bool>(CppRef<U8>(self).data < rhs.data);
    else
      return MakeInvalidOpError();
  }

  static AnyValue LessThan(const AnyValue& self, const U8& rhs) {
    if (Is<I32>(self))
      return Make<Bool>(CppRef<I32>(self).data < rhs.data);
    else if (Is<Bool>(self))
      return Make<Bool>(CppRef<Bool>(self).data < rhs.data);
    else if (Is<U64>(self))
      return Make<Bool>(CppRef<U64>(self).data < rhs.data);
    else if (Is<F32>(self))
      return Make<Bool>(CppRef<F32>(self).data < rhs.data);
    else if (Is<I8>(self))
      return Make<Bool>(CppRef<I8>(self).data < rhs.data);
    else if (Is<U8>(self))
      return Make<Bool>(CppRef<U8>(self).data < rhs.data);
    else
      return MakeInvalidOpError();
  }

  static AnyValue LessThan(const AnyValue& self, const StrIter& rhs) {
    if (Is<StrIter>(self))
      return Make<Bool>(CppRef<StrIter>(self).data < rhs.data);
    else
      return MakeInvalidOpError();
  }

  static AnyValue LessThan(const AnyValue& self, const ArrayIter& rhs) {
    if (Is<ArrayIter>(self))
      return Make<Bool>(CppRef<ArrayIter>(self).data < rhs.data);
    else
      return MakeInvalidOpError();
  }

  static AnyValue Lt(const AnyValue& self, const AnyValue& rhs) {
    if (Is<I32>(self))
      return LessThan(self, CppRef<I32>(rhs));
    else if (Is<Bool>(self))
      return LessThan(self, CppRef<Bool>(rhs));
    else if (Is<U64>(self))
      return LessThan(self, CppRef<U64>(rhs));
    else if (Is<F32>(self))
      return LessThan(self, CppRef<F32>(rhs));
    else if (Is<I8>(self))
      return LessThan(self, CppRef<I8>(rhs));
    else if (Is<U8>(self))
      return LessThan(self, CppRef<U8>(rhs));
    else if (Is<StrIter>(self))
      return LessThan(self, CppRef<StrIter>(rhs));
    else if (Is<ArrayIter>(self))
      return LessThan(self, CppRef<ArrayIter>(rhs));
    else
      return MakeInvalidOpError();
  }

  static AnyValue GreaterThan(const AnyValue& self, const I32& rhs) {
    if (Is<I32>(self))
      return Make<Bool>(CppRef<I32>(self).data > rhs.data);
    else if (Is<Bool>(self))
      return Make<Bool>(CppRef<Bool>(self).data > rhs.data);
    else if (Is<U64>(self))
      return Make<Bool>(CppRef<U64>(self).data > rhs.data);
    else if (Is<F32>(self))
      return Make<Bool>(CppRef<F32>(self).data > rhs.data);
    else if (Is<I8>(self))
      return Make<Bool>(CppRef<I8>(self).data > rhs.data);
    else if (Is<U8>(self))
      return Make<Bool>(CppRef<U8>(self).data > rhs.data);
    else if (Is<String>(self))
      return Make<Bool>(std::stoi(CppRef<String>(self).data) > rhs.data);
    else
      return MakeInvalidOpError();
  }

  static AnyValue GreaterThan(const AnyValue& self, const Bool& rhs) {
    if (Is<I32>(self))
      return Make<Bool>(CppRef<I32>(self).data > rhs.data);
    else if (Is<Bool>(self))
      return Make<Bool>(CppRef<Bool>(self).data > rhs.data);
    else if (Is<U64>(self))
      return Make<Bool>(CppRef<U64>(self).data > rhs.data);
    else if (Is<F32>(self))
      return Make<Bool>(CppRef<F32>(self).data > rhs.data);
    else if (Is<I8>(self))
      return Make<Bool>(CppRef<I8>(self).data > rhs.data);
    else if (Is<U8>(self))
      return Make<Bool>(CppRef<U8>(self).data > rhs.data);
    else
      return MakeInvalidOpError();
  }

  static AnyValue GreaterThan(const AnyValue& self, const U64& rhs) {
    if (Is<I32>(self))
      return Make<Bool>(CppRef<I32>(self).data > rhs.data);
    else if (Is<Bool>(self))
      return Make<Bool>(CppRef<Bool>(self).data > rhs.data);
    else if (Is<U64>(self))
      return Make<Bool>(CppRef<U64>(self).data > rhs.data);
    else if (Is<F32>(self))
      return Make<Bool>(CppRef<F32>(self).data > rhs.data);
    else if (Is<I8>(self))
      return Make<Bool>(CppRef<I8>(self).data > rhs.data);
    else if (Is<U8>(self))
      return Make<Bool>(CppRef<U8>(self).data > rhs.data);
    else
      return MakeInvalidOpError();
  }

  static AnyValue GreaterThan(const AnyValue& self, const F32& rhs) {
    if (Is<I32>(self))
      return Make<Bool>(CppRef<I32>(self).data > rhs.data);
    else if (Is<Bool>(self))
      return Make<Bool>(CppRef<Bool>(self).data > rhs.data);
    else if (Is<U64>(self))
      return Make<Bool>(CppRef<U64>(self).data > rhs.data);
    else if (Is<F32>(self))
      return Make<Bool>(CppRef<F32>(self).data > rhs.data);
    else if (Is<I8>(self))
      return Make<Bool>(CppRef<I8>(self).data > rhs.data);
    else if (Is<U8>(self))
      return Make<Bool>(CppRef<U8>(self).data > rhs.data);
    else
      return MakeInvalidOpError();
  }

  static AnyValue GreaterThan(const AnyValue& self, const I8& rhs) {
    if (Is<I32>(self))
      return Make<Bool>(CppRef<I32>(self).data > rhs.data);
    else if (Is<Bool>(self))
      return Make<Bool>(CppRef<Bool>(self).data > rhs.data);
    else if (Is<U64>(self))
      return Make<Bool>(CppRef<U64>(self).data > rhs.data);
    else if (Is<F32>(self))
      return Make<Bool>(CppRef<F32>(self).data > rhs.data);
    else if (Is<I8>(self))
      return Make<Bool>(CppRef<I8>(self).data > rhs.data);
    else if (Is<U8>(self))
      return Make<Bool>(CppRef<U8>(self).data > rhs.data);
    else
      return MakeInvalidOpError();
  }

  static AnyValue GreaterThan(const AnyValue& self, const U8& rhs) {
    if (Is<I32>(self))
      return Make<Bool>(CppRef<I32>(self).data > rhs.data);
    else if (Is<Bool>(self))
      return Make<Bool>(CppRef<Bool>(self).data > rhs.data);
    else if (Is<U64>(self))
      return Make<Bool>(CppRef<U64>(self).data > rhs.data);
    else if (Is<F32>(self))
      return Make<Bool>(CppRef<F32>(self).data > rhs.data);
    else if (Is<I8>(self))
      return Make<Bool>(CppRef<I8>(self).data > rhs.data);
    else if (Is<U8>(self))
      return Make<Bool>(CppRef<U8>(self).data > rhs.data);
    else
      return MakeInvalidOpError();
  }

  static AnyValue GreaterThan(const AnyValue& self, const StrIter& rhs) {
    if (Is<StrIter>(self))
      return Make<Bool>(CppRef<StrIter>(self).data > rhs.data);
    else
      return MakeInvalidOpError();
  }

  static AnyValue GreaterThan(const AnyValue& self, const ArrayIter& rhs) {
    if (Is<ArrayIter>(self))
      return Make<Bool>(CppRef<ArrayIter>(self).data > rhs.data);
    else
      return MakeInvalidOpError();
  }

  static AnyValue Gt(const AnyValue& self, const AnyValue& rhs) {
    if (Is<I32>(self))
      return GreaterThan(self, CppRef<I32>(rhs));
    else if (Is<Bool>(self))
      return GreaterThan(self, CppRef<Bool>(rhs));
    else if (Is<U64>(self))
      return GreaterThan(self, CppRef<U64>(rhs));
    else if (Is<F32>(self))
      return GreaterThan(self, CppRef<F32>(rhs));
    else if (Is<I8>(self))
      return GreaterThan(self, CppRef<I8>(rhs));
    else if (Is<U8>(self))
      return GreaterThan(self, CppRef<U8>(rhs));
    else if (Is<StrIter>(self))
      return GreaterThan(self, CppRef<StrIter>(rhs));
    else if (Is<ArrayIter>(self))
      return GreaterThan(self, CppRef<ArrayIter>(rhs));
    else
      return MakeInvalidOpError();
  }

  static AnyValue LessThanOrEqual(const AnyValue& self, const I32& rhs) {
    if (Is<I32>(self))
      return Make<Bool>(CppRef<I32>(self).data <= rhs.data);
    else if (Is<Bool>(self))
      return Make<Bool>(CppRef<Bool>(self).data <= rhs.data);
    else if (Is<U64>(self))
      return Make<Bool>(CppRef<U64>(self).data <= rhs.data);
    else if (Is<F32>(self))
      return Make<Bool>(CppRef<F32>(self).data <= rhs.data);
    else if (Is<I8>(self))
      return Make<Bool>(CppRef<I8>(self).data <= rhs.data);
    else if (Is<U8>(self))
      return Make<Bool>(CppRef<U8>(self).data <= rhs.data);
    else if (Is<String>(self))
      return Make<Bool>(std::stoi(CppRef<String>(self).data) <= rhs.data);
    else
      return MakeInvalidOpError();
  }

  static AnyValue LessThanOrEqual(const AnyValue& self, const Bool& rhs) {
    if (Is<I32>(self))
      return Make<Bool>(CppRef<I32>(self).data <= rhs.data);
    else if (Is<Bool>(self))
      return Make<Bool>(CppRef<Bool>(self).data <= rhs.data);
    else if (Is<U64>(self))
      return Make<Bool>(CppRef<U64>(self).data <= rhs.data);
    else if (Is<F32>(self))
      return Make<Bool>(CppRef<F32>(self).data <= rhs.data);
    else if (Is<I8>(self))
      return Make<Bool>(CppRef<I8>(self).data <= rhs.data);
    else if (Is<U8>(self))
      return Make<Bool>(CppRef<U8>(self).data <= rhs.data);
    else
      return MakeInvalidOpError();
  }

  static AnyValue LessThanOrEqual(const AnyValue& self, const U64& rhs) {
    if (Is<I32>(self))
      return Make<Bool>(CppRef<I32>(self).data <= rhs.data);
    else if (Is<Bool>(self))
      return Make<Bool>(CppRef<Bool>(self).data <= rhs.data);
    else if (Is<U64>(self))
      return Make<Bool>(CppRef<U64>(self).data <= rhs.data);
    else if (Is<F32>(self))
      return Make<Bool>(CppRef<F32>(self).data <= rhs.data);
    else if (Is<I8>(self))
      return Make<Bool>(CppRef<I8>(self).data <= rhs.data);
    else if (Is<U8>(self))
      return Make<Bool>(CppRef<U8>(self).data <= rhs.data);
    else
      return MakeInvalidOpError();
  }

  static AnyValue LessThanOrEqual(const AnyValue& self, const F32& rhs) {
    if (Is<I32>(self))
      return Make<Bool>(CppRef<I32>(self).data <= rhs.data);
    else if (Is<Bool>(self))
      return Make<Bool>(CppRef<Bool>(self).data <= rhs.data);
    else if (Is<U64>(self))
      return Make<Bool>(CppRef<U64>(self).data <= rhs.data);
    else if (Is<F32>(self))
      return Make<Bool>(CppRef<F32>(self).data <= rhs.data);
    else if (Is<I8>(self))
      return Make<Bool>(CppRef<I8>(self).data <= rhs.data);
    else if (Is<U8>(self))
      return Make<Bool>(CppRef<U8>(self).data <= rhs.data);
    else
      return MakeInvalidOpError();
  }

  static AnyValue LessThanOrEqual(const AnyValue& self, const I8& rhs) {
    if (Is<I32>(self))
      return Make<Bool>(CppRef<I32>(self).data <= rhs.data);
    else if (Is<Bool>(self))
      return Make<Bool>(CppRef<Bool>(self).data <= rhs.data);
    else if (Is<U64>(self))
      return Make<Bool>(CppRef<U64>(self).data <= rhs.data);
    else if (Is<F32>(self))
      return Make<Bool>(CppRef<F32>(self).data <= rhs.data);
    else if (Is<I8>(self))
      return Make<Bool>(CppRef<I8>(self).data <= rhs.data);
    else if (Is<U8>(self))
      return Make<Bool>(CppRef<U8>(self).data <= rhs.data);
    else
      return MakeInvalidOpError();
  }

  static AnyValue LessThanOrEqual(const AnyValue& self, const U8& rhs) {
    if (Is<I32>(self))
      return Make<Bool>(CppRef<I32>(self).data <= rhs.data);
    else if (Is<Bool>(self))
      return Make<Bool>(CppRef<Bool>(self).data <= rhs.data);
    else if (Is<U64>(self))
      return Make<Bool>(CppRef<U64>(self).data <= rhs.data);
    else if (Is<F32>(self))
      return Make<Bool>(CppRef<F32>(self).data <= rhs.data);
    else if (Is<I8>(self))
      return Make<Bool>(CppRef<I8>(self).data <= rhs.data);
    else if (Is<U8>(self))
      return Make<Bool>(CppRef<U8>(self).data <= rhs.data);
    else
      return MakeInvalidOpError();
  }

  static AnyValue LessThanOrEqual(const AnyValue& self, const StrIter& rhs) {
    if (Is<StrIter>(self))
      return Make<Bool>(CppRef<StrIter>(self).data <= rhs.data);
    else
      return MakeInvalidOpError();
  }

  static AnyValue LessThanOrEqual(const AnyValue& self, const ArrayIter& rhs) {
    if (Is<ArrayIter>(self))
      return Make<Bool>(CppRef<ArrayIter>(self).data <= rhs.data);
    else
      return MakeInvalidOpError();
  }

  static AnyValue Lte(const AnyValue& self, const AnyValue& rhs) {
    if (Is<I32>(self))
      return LessThanOrEqual(self, CppRef<I32>(rhs));
    else if (Is<Bool>(self))
      return LessThanOrEqual(self, CppRef<Bool>(rhs));
    else if (Is<U64>(self))
      return LessThanOrEqual(self, CppRef<U64>(rhs));
    else if (Is<F32>(self))
      return LessThanOrEqual(self, CppRef<F32>(rhs));
    else if (Is<I8>(self))
      return LessThanOrEqual(self, CppRef<I8>(rhs));
    else if (Is<U8>(self))
      return LessThanOrEqual(self, CppRef<U8>(rhs));
    else if (Is<StrIter>(self))
      return LessThanOrEqual(self, CppRef<StrIter>(rhs));
    else if (Is<ArrayIter>(self))
      return LessThanOrEqual(self, CppRef<ArrayIter>(rhs));
    else
      return MakeInvalidOpError();
  }

  static AnyValue GreaterThanOrEqual(const AnyValue& self, const I32& rhs) {
    if (Is<I32>(self))
      return Make<Bool>(CppRef<I32>(self).data >= rhs.data);
    else if (Is<Bool>(self))
      return Make<Bool>(CppRef<Bool>(self).data >= rhs.data);
    else if (Is<U64>(self))
      return Make<Bool>(CppRef<U64>(self).data >= rhs.data);
    else if (Is<F32>(self))
      return Make<Bool>(CppRef<F32>(self).data >= rhs.data);
    else if (Is<I8>(self))
      return Make<Bool>(CppRef<I8>(self).data >= rhs.data);
    else if (Is<U8>(self))
      return Make<Bool>(CppRef<U8>(self).data >= rhs.data);
    else if (Is<String>(self))
      return Make<Bool>(std::stoi(CppRef<String>(self).data) >= rhs.data);
    else
      return MakeInvalidOpError();
  }

  static AnyValue GreaterThanOrEqual(const AnyValue& self, const Bool& rhs) {
    if (Is<I32>(self))
      return Make<Bool>(CppRef<I32>(self).data >= rhs.data);
    else if (Is<Bool>(self))
      return Make<Bool>(CppRef<Bool>(self).data >= rhs.data);
    else if (Is<U64>(self))
      return Make<Bool>(CppRef<U64>(self).data >= rhs.data);
    else if (Is<F32>(self))
      return Make<Bool>(CppRef<F32>(self).data >= rhs.data);
    else if (Is<I8>(self))
      return Make<Bool>(CppRef<I8>(self).data >= rhs.data);
    else if (Is<U8>(self))
      return Make<Bool>(CppRef<U8>(self).data >= rhs.data);
    else
      return MakeInvalidOpError();
  }

  static AnyValue GreaterThanOrEqual(const AnyValue& self, const U64& rhs) {
    if (Is<I32>(self))
      return Make<Bool>(CppRef<I32>(self).data >= rhs.data);
    else if (Is<Bool>(self))
      return Make<Bool>(CppRef<Bool>(self).data >= rhs.data);
    else if (Is<U64>(self))
      return Make<Bool>(CppRef<U64>(self).data >= rhs.data);
    else if (Is<F32>(self))
      return Make<Bool>(CppRef<F32>(self).data >= rhs.data);
    else if (Is<I8>(self))
      return Make<Bool>(CppRef<I8>(self).data >= rhs.data);
    else if (Is<U8>(self))
      return Make<Bool>(CppRef<U8>(self).data >= rhs.data);
    else
      return MakeInvalidOpError();
  }

  static AnyValue GreaterThanOrEqual(const AnyValue& self, const F32& rhs) {
    if (Is<I32>(self))
      return Make<Bool>(CppRef<I32>(self).data >= rhs.data);
    else if (Is<Bool>(self))
      return Make<Bool>(CppRef<Bool>(self).data >= rhs.data);
    else if (Is<U64>(self))
      return Make<Bool>(CppRef<U64>(self).data >= rhs.data);
    else if (Is<F32>(self))
      return Make<Bool>(CppRef<F32>(self).data >= rhs.data);
    else if (Is<I8>(self))
      return Make<Bool>(CppRef<I8>(self).data >= rhs.data);
    else if (Is<U8>(self))
      return Make<Bool>(CppRef<U8>(self).data >= rhs.data);
    else
      return MakeInvalidOpError();
  }

  static AnyValue GreaterThanOrEqual(const AnyValue& self, const I8& rhs) {
    if (Is<I32>(self))
      return Make<Bool>(CppRef<I32>(self).data >= rhs.data);
    else if (Is<Bool>(self))
      return Make<Bool>(CppRef<Bool>(self).data >= rhs.data);
    else if (Is<U64>(self))
      return Make<Bool>(CppRef<U64>(self).data >= rhs.data);
    else if (Is<F32>(self))
      return Make<Bool>(CppRef<F32>(self).data >= rhs.data);
    else if (Is<I8>(self))
      return Make<Bool>(CppRef<I8>(self).data >= rhs.data);
    else if (Is<U8>(self))
      return Make<Bool>(CppRef<U8>(self).data >= rhs.data);
    else
      return MakeInvalidOpError();
  }

  static AnyValue GreaterThanOrEqual(const AnyValue& self, const U8& rhs) {
    if (Is<I32>(self))
      return Make<Bool>(CppRef<I32>(self).data >= rhs.data);
    else if (Is<Bool>(self))
      return Make<Bool>(CppRef<Bool>(self).data >= rhs.data);
    else if (Is<U64>(self))
      return Make<Bool>(CppRef<U64>(self).data >= rhs.data);
    else if (Is<F32>(self))
      return Make<Bool>(CppRef<F32>(self).data >= rhs.data);
    else if (Is<I8>(self))
      return Make<Bool>(CppRef<I8>(self).data >= rhs.data);
    else if (Is<U8>(self))
      return Make<Bool>(CppRef<U8>(self).data >= rhs.data);
    else
      return MakeInvalidOpError();
  }

  static AnyValue GreaterThanOrEqual(const AnyValue& self, const StrIter& rhs) {
    if (Is<StrIter>(self))
      return Make<Bool>(CppRef<StrIter>(self).data >= rhs.data);
    else
      return MakeInvalidOpError();
  }

  static AnyValue GreaterThanOrEqual(const AnyValue& self, const ArrayIter& rhs) {
    if (Is<ArrayIter>(self))
      return Make<Bool>(CppRef<ArrayIter>(self).data >= rhs.data);
    else
      return MakeInvalidOpError();
  }

  static AnyValue Gte(const AnyValue& self, const AnyValue& rhs) {
    if (Is<I32>(self))
      return GreaterThanOrEqual(self, CppRef<I32>(rhs));
    else if (Is<Bool>(self))
      return GreaterThanOrEqual(self, CppRef<Bool>(rhs));
    else if (Is<U64>(self))
      return GreaterThanOrEqual(self, CppRef<U64>(rhs));
    else if (Is<F32>(self))
      return GreaterThanOrEqual(self, CppRef<F32>(rhs));
    else if (Is<I8>(self))
      return GreaterThanOrEqual(self, CppRef<I8>(rhs));
    else if (Is<U8>(self))
      return GreaterThanOrEqual(self, CppRef<U8>(rhs));
    else if (Is<StrIter>(self))
      return GreaterThanOrEqual(self, CppRef<StrIter>(rhs));
    else if (Is<ArrayIter>(self))
      return GreaterThanOrEqual(self, CppRef<ArrayIter>(rhs));
    else
      return MakeInvalidOpError();
  }

  static AnyValue EqualTo(const AnyValue& self, I32 rhs) {
    if (Is<I32>(self))
      return Make<Bool>(CppRef<I32>(self).data == rhs.data);
    else if (Is<Bool>(self))
      return Make<Bool>(CppRef<Bool>(self).data == rhs.data);
    else if (Is<U64>(self))
      return Make<Bool>(CppRef<U64>(self).data == rhs.data);
    else if (Is<F32>(self))
      return Make<Bool>(CppRef<F32>(self).data == rhs.data);
    else if (Is<I8>(self))
      return Make<Bool>(CppRef<I8>(self).data == rhs.data);
    else if (Is<U8>(self))
      return Make<Bool>(CppRef<U8>(self).data == rhs.data);
    else
      return MakeInvalidOpError();
  }

  static AnyValue EqualTo(const AnyValue& self, Bool rhs) {
    if (Is<I32>(self))
      return Make<Bool>(CppRef<I32>(self).data == rhs.data);
    else if (Is<Bool>(self))
      return Make<Bool>(CppRef<Bool>(self).data == rhs.data);
    else if (Is<U64>(self))
      return Make<Bool>(CppRef<U64>(self).data == rhs.data);
    else if (Is<F32>(self))
      return Make<Bool>(CppRef<F32>(self).data == rhs.data);
    else if (Is<I8>(self))
      return Make<Bool>(CppRef<I8>(self).data == rhs.data);
    else if (Is<U8>(self))
      return Make<Bool>(CppRef<U8>(self).data == rhs.data);
    else
      return MakeInvalidOpError();
  }

  static AnyValue EqualTo(const AnyValue& self, U64 rhs) {
    if (Is<I32>(self))
      return Make<Bool>(CppRef<I32>(self).data == rhs.data);
    else if (Is<Bool>(self))
      return Make<Bool>(CppRef<Bool>(self).data == rhs.data);
    else if (Is<U64>(self))
      return Make<Bool>(CppRef<U64>(self).data == rhs.data);
    else if (Is<F32>(self))
      return Make<Bool>(CppRef<F32>(self).data == rhs.data);
    else if (Is<I8>(self))
      return Make<Bool>(CppRef<I8>(self).data == rhs.data);
    else if (Is<U8>(self))
      return Make<Bool>(CppRef<U8>(self).data == rhs.data);
    else
      return MakeInvalidOpError();
  }

  static AnyValue EqualTo(const AnyValue& self, F32 rhs) {
    if (Is<I32>(self))
      return Make<Bool>(CppRef<I32>(self).data == rhs.data);
    else if (Is<Bool>(self))
      return Make<Bool>(CppRef<Bool>(self).data == rhs.data);
    else if (Is<U64>(self))
      return Make<Bool>(CppRef<U64>(self).data == rhs.data);
    else if (Is<F32>(self))
      return Make<Bool>(CppRef<F32>(self).data == rhs.data);
    else if (Is<I8>(self))
      return Make<Bool>(CppRef<I8>(self).data == rhs.data);
    else if (Is<U8>(self))
      return Make<Bool>(CppRef<U8>(self).data == rhs.data);
    else
      return MakeInvalidOpError();
  }

  static AnyValue EqualTo(const AnyValue& self, I8 rhs) {
    if (Is<I32>(self))
      return Make<Bool>(CppRef<I32>(self).data == rhs.data);
    else if (Is<Bool>(self))
      return Make<Bool>(CppRef<Bool>(self).data == rhs.data);
    else if (Is<U64>(self))
      return Make<Bool>(CppRef<U64>(self).data == rhs.data);
    else if (Is<F32>(self))
      return Make<Bool>(CppRef<F32>(self).data == rhs.data);
    else if (Is<I8>(self))
      return Make<Bool>(CppRef<I8>(self).data == rhs.data);
    else if (Is<U8>(self))
      return Make<Bool>(CppRef<U8>(self).data == rhs.data);
    else
      return MakeInvalidOpError();
  }

  static AnyValue EqualTo(const AnyValue& self, U8 rhs) {
    if (Is<I32>(self))
      return Make<Bool>(CppRef<I32>(self).data == rhs.data);
    else if (Is<Bool>(self))
      return Make<Bool>(CppRef<Bool>(self).data == rhs.data);
    else if (Is<U64>(self))
      return Make<Bool>(CppRef<U64>(self).data == rhs.data);
    else if (Is<F32>(self))
      return Make<Bool>(CppRef<F32>(self).data == rhs.data);
    else if (Is<I8>(self))
      return Make<Bool>(CppRef<I8>(self).data == rhs.data);
    else if (Is<U8>(self))
      return Make<Bool>(CppRef<U8>(self).data == rhs.data);
    else
      return MakeInvalidOpError();
  }

  static AnyValue EqualTo(const AnyValue& self, String rhs) {
    if (Is<String>(self))
      return Make<Bool>(CppRef<String>(self).data == rhs.data);
    else
      return MakeInvalidOpError();
  }

  static AnyValue EqualTo(const AnyValue& self, CStr rhs) {
    if (Is<String>(self))
      return Make<Bool>(CppRef<String>(self).data == rhs.data);
    else
      return MakeInvalidOpError();
  }

  static AnyValue EqualTo(const AnyValue& self, StrIter rhs) {
    if (Is<StrIter>(self))
      return Make<Bool>(CppRef<StrIter>(self).data == rhs.data);
    else
      return MakeInvalidOpError();
  }

  static AnyValue EqualTo(const AnyValue& self, ArrayIter rhs) {
    if (Is<ArrayIter>(self))
      return Make<Bool>(CppRef<ArrayIter>(self).data == rhs.data);
    else
      return MakeInvalidOpError();
  }

  static AnyValue EqualTo(const AnyValue& self, MapIter rhs) {
    if (Is<MapIter>(self))
      return Make<Bool>(CppRef<MapIter>(self).data == rhs.data);
    else
      return MakeInvalidOpError();
  }

  static AnyValue Eq(const AnyValue& self, const AnyValue& rhs) {
    if (Is<I32>(self))
      return EqualTo(self, CppRef<I32>(rhs));
    else if (Is<Bool>(self))
      return EqualTo(self, CppRef<Bool>(rhs));
    else if (Is<U64>(self))
      return EqualTo(self, CppRef<U64>(rhs));
    else if (Is<F32>(self))
      return EqualTo(self, CppRef<F32>(rhs));
    else if (Is<I8>(self))
      return EqualTo(self, CppRef<I8>(rhs));
    else if (Is<U8>(self))
      return EqualTo(self, CppRef<U8>(rhs));
    else if (Is<StrIter>(self))
      return EqualTo(self, CppRef<StrIter>(rhs));
    else if (Is<ArrayIter>(self))
      return EqualTo(self, CppRef<ArrayIter>(rhs));
    else if (Is<MapIter>(self))
      return EqualTo(self, CppRef<MapIter>(rhs));
    else
      return MakeInvalidOpError();
  }

  static AnyValue Neq(const AnyValue& self, const AnyValue& rhs) { return AV::Make<Bool>(!EqualToDirect(self, rhs)); }

  bool operator==(AnyValue& rhs) { return EqualToDirect(*this, rhs); }

  bool operator!=(AnyValue& rhs) { return !EqualToDirect(*this, rhs); }

  bool operator<(AnyValue& rhs) { return LessThanDirect(*this, rhs); }

  bool operator>(AnyValue& rhs) { return LessThanDirect(rhs, *this); }

  static bool Release(AnyValue& self) {
    if (Is<Array>(self)) {
      for (auto& elem : CppRef<Array>(self).data) {
        Release(elem);
      }
    } else if (Is<Map>(self)) {
      for (auto& [key, value] : CppRef<Map>(self).data) {
        Release(value);
      }
    }

    return std::visit([](auto&& arg) -> bool { return arg.Release(); }, self);
  }

  static AnyValue Begin(AnyValue& self) {
    if (Is<String>(self))
      return Make<StrIter>(CppRef<String>(self).data.begin());
    else if (Is<Array>(self))
      return Make<ArrayIter>(CppRef<Array>(self).data.begin());
    else if (Is<Map>(self))
      return Make<MapIter>(CppRef<Map>(self).data.begin());
    else
      return MakeInvalidOpError();
  }

  static AnyValue End(AnyValue& self) {
    if (Is<String>(self))
      return Make<StrIter>(CppRef<String>(self).data.end());
    else if (Is<Array>(self))
      return Make<ArrayIter>(CppRef<Array>(self).data.end());
    else if (Is<Map>(self))
      return Make<MapIter>(CppRef<Map>(self).data.end());
    else
      return MakeInvalidOpError();
  }

  static AnyValue& Advance(AnyValue& self) {
    if (Is<StrIter>(self)) {
      std::advance(CppRef<StrIter>(self).data, 1);
      return self;
    } else if (Is<ArrayIter>(self)) {
      std::advance(CppRef<ArrayIter>(self).data, 1);
      return self;
    } else if (Is<MapIter>(self)) {
      std::advance(CppRef<MapIter>(self).data, 1);
      return self;
    } else
      return SetAs(self, MakeInvalidOpError());
  }

  static AnyValue& Dereference(AnyValue& self) {
    if (Is<ArrayIter>(self))
      return *CppRef<ArrayIter>(self).data;
    else if (Is<MapIter>(self))
      return CppRef<MapIter>(self).data->second;
    else
      return SetAs(self, MakeInvalidOpError());
  }

  static AnyValue IndexOf(AnyValue& self) {
    if (Is<MapIter>(self))
      return Make<String>(CppRef<MapIter>(self).data->first);
    else
      return SetAs(self, MakeInvalidOpError());
  }

  static AnyValue Length(const AnyValue& self) {
    if (Is<String>(self))
      return Make<U64>(CppRef<String>(self).data.length());
    else if (Is<Array>(self))
      return Make<U64>(CppRef<Array>(self).data.size());
    else if (Is<Map>(self))
      return Make<U64>(CppRef<Map>(self).data.size());
    else
      return MakeInvalidOpError();
  }

  template <typename T>
  static constexpr bool LessThanDirect(AnyValue& self, const T& rhs) {
    auto ret = Lt(self, rhs);
    if (Is<Bool>(ret)) {
      return CppRef<Bool>(ret).data;
    } else {
      throw "Invalid operation";
    }
  }

  template <typename T>
  static constexpr bool EqualToDirect(const AnyValue& self, const T& rhs) {
    auto ret = Eq(self, rhs);
    if (Is<Bool>(ret)) {
      return CppRef<Bool>(ret).data;
    } else {
      throw "Invalid operation";
    }
  }

  template <typename T>
  static constexpr bool GreaterThanDirect(AnyValue& self, const T& rhs) {
    auto ret = GreaterThan(self, rhs);
    if (Is<Bool>(ret)) {
      return CppRef<Bool>(ret).data;
    } else {
      throw "Invalid operation";
    }
  }

  template <typename T>
  static constexpr bool LessThanOrEqualDirect(AnyValue& self, const T& rhs) {
    auto ret = LessThanOrEqual(self, rhs);
    if (Is<Bool>(ret)) {
      return CppRef<Bool>(ret).data;
    } else {
      throw "Invalid operation";
    }
  }

  template <typename T>
  static constexpr bool GreaterThanOrEqualDirect(AnyValue& self, const T& rhs) {
    auto ret = GreaterThanOrEqual(self, rhs);
    if (Is<Bool>(ret)) {
      return CppRef<Bool>(ret).data;
    } else {
      throw "Invalid operation";
    }
  }

  static void EraseAt(AnyValue& self, const AnyValue& key) {
    if (Is<Map>(self)) {
      if (Is<String>(key))
        CppRef<Map>(self).data.erase(CppRef<String>(key).data);
      else
        throw "InvalidOperation";
    } else {
      throw "InvalidOperation";
    }
  }

  static void Erase(AnyValue& self, const AnyValue& val) {
    if (Is<Map>(self)) {
      auto& map = CppRef<Map>(self);
      auto is_found =
          std::find_if(map.data.begin(), map.data.end(), [&](auto& elem) { return EqualToDirect(elem.second, val); });
      if (is_found != map.data.end()) map.data.erase(is_found);
    } else if (Is<Array>(self)) {
      auto& arr = CppRef<Array>(self);
      auto is_found =
          std::find_if(arr.data.begin(), arr.data.end(), [&](auto& elem) { return EqualToDirect(elem, val); });
      if (is_found != arr.data.end()) arr.data.erase(is_found);
    } else {
      SetAs(self, MakeInvalidOpError());
    }
  }
};

}  // namespace cnd::hir

/// @} // end of cnd_compiler

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2026 Anton Yashchenko
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
