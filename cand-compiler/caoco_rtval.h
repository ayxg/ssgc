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
// File: caoco_rtval.h
//---------------------------------------------------------------------------//
#ifndef HEADER_GUARD_CALE_CAND_OFFICIAL_COMPILER_CAOCO_RTVAL_H
#define HEADER_GUARD_CALE_CAND_OFFICIAL_COMPILER_CAOCO_RTVAL_H
//---------------------------------------------------------------------------//
// Brief: Runtime Value Class for C&
//---------------------------------------------------------------------------//
// Includes:
#include "cppsextended.h"
//---------------------------------------------------------------------------//

namespace caoco {
using std::forward;
using std::get;
using std::make_unique;
using std::string;
using std::string_view;

static constexpr inline string_view kUndefinedNamespaceName = "#";
static constexpr inline string_view kGlobalNamespaceName = "_global_";
static constexpr inline string_view kClassInitMethodName = "_init_";
static constexpr inline size_t kUndefinedRtti = 0;

// The main 'value' class for the C& runtime. Wraps a variant of all the types
// in the C& runtime. Methods, classes, namespaces and literals...
class RtVal;
class NamedRtVal;
class NamedRtValSet;

// All non-literal types are stored as unique pointers in the NativeVariant.
// NOTE: RtNone,RtUndefined and RtDynPtr are trivially constructible and
//       destructible.So they can be stored as values in the NativeVariant.
template <class T>
using Uptr = std::unique_ptr<T>;

//---------------------------------------------------------------------------//
// Section:{Builtins Forward Decls}
// Brief:{Declarations of classes which are part of the union of the
// NativeVariant.} Detail:{
//  Every object is C& is one of these C++ classes. Including all the literal
//  type alternatives.
//   - RtDynNamespace: class description or static namespace in the runtime.
//   - RtDynObject: Dynamic runtime object instance.
//   - RtDynMethod: Runtime method.
//   - RtCppMethod: C++ method pointer from C&.
//   - RtDynList: Heterogenous list of objects.
//   - RtDynPtr: Pointer to an RtVal object.
//   - RtNone: Represents a null or nothing.
//   - RtUndefined: Represents an uninitialized or destroyed value.
//
//  Undefined is only implicitly created by the runtime.
//  None is a literal object the user may instantiate.
// }
//---------------------------------------------------------------------------//
class RtDynNamespace;
class RtDynObject;
class RtDynMethod;
class RtCppMethod;
class RtDynList;
class RtDynPtr;

struct RtNone;
struct RtUndefined;
//---------------------------------------------------------------------------//
// EndSection:{Builtins Forward Decls}
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
// Section:{Native Union Typedefs}
// Brief:{
//  Typedefs of ALL the classes which the union of the NativeVariant.
// }
// Detail:{
//  Every object is C& is one of these C++ classes.
// }
//---------------------------------------------------------------------------//
using NativeEmptyVariantT = std::monostate;
using NativeIntT = int;
using NativeUnsignedT = unsigned;
using NativeDoubleT = double;
using NativeBoolT = bool;
using NativeCharT = char;
using NativeByteT = unsigned char;
using NativeCStringT = string_view;
using NativeNoneT = RtNone;
using NativeUndefinedT = RtUndefined;
using NativePtrT = RtDynPtr;
using NativeStringT = Uptr<string>;
using NativeNamespaceT = Uptr<RtDynNamespace>;
using NativeObjectT = Uptr<RtDynObject>;
using NativeMethodT = Uptr<RtDynMethod>;
using NativeCppMethodT = Uptr<RtCppMethod>;
using NativeListT = Uptr<RtDynList>;

using NativeTypeIndexList = mta::compile_time_type_index_list<
    NativeEmptyVariantT, NativeIntT, NativeUnsignedT, NativeDoubleT,
    NativeBoolT, NativeCharT, NativeByteT, NativeCStringT, NativeNoneT,
    NativeUndefinedT, NativePtrT, NativeStringT, NativeNamespaceT,
    NativeObjectT, NativeMethodT, NativeCppMethodT, NativeListT>;

// std::variant of all the types in the C& runtime, a member of RtVal.
using NativeVariant = NativeTypeIndexList::types_variant;

// Enumeration of native variant types.
namespace eNativeTypeIndex {
using underlying_type = size_t;
enum enum_type : size_t {
  kEmpty = NativeTypeIndexList::index_of<NativeEmptyVariantT>(),
  kInt = NativeTypeIndexList::index_of<NativeIntT>(),
  kUnsigned = NativeTypeIndexList::index_of<NativeUnsignedT>(),
  kDouble = NativeTypeIndexList::index_of<NativeDoubleT>(),
  kBool = NativeTypeIndexList::index_of<NativeBoolT>(),
  kChar = NativeTypeIndexList::index_of<NativeCharT>(),
  kByte = NativeTypeIndexList::index_of<NativeByteT>(),
  kCString = NativeTypeIndexList::index_of<NativeCStringT>(),
  kNone = NativeTypeIndexList::index_of<NativeNoneT>(),
  kUndefined = NativeTypeIndexList::index_of<NativeUndefinedT>(),
  kDynamicPtr = NativeTypeIndexList::index_of<NativePtrT>(),
  kString = NativeTypeIndexList::index_of<NativeStringT>(),
  kDynamicNamespace = NativeTypeIndexList::index_of<NativeNamespaceT>(),
  kDynamicObject = NativeTypeIndexList::index_of<NativeObjectT>(),
  kDynamicMethod = NativeTypeIndexList::index_of<NativeMethodT>(),
  kCppMethod = NativeTypeIndexList::index_of<NativeCppMethodT>(),
  kDynamicRuntimeArray = NativeTypeIndexList::index_of<NativeListT>(),
};
}  // namespace eNativeTypeIndex
//---------------------------------------------------------------------------//
// EndSection:{Native Union Typedefs}
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
// Section:{Native Type Concepts}
// Brief:{
//  These cconcepts categorize the native types in the C& runtime. For use in
//  NativeVariant visitation and type checking.
// }
// Detail:{
//  iOneOfNativeLiterals: Type is one of the literal types, stored
//                        directly as a value in the NativeVariant.
//  iOneOfNativeArithmetic: Type is one of the literal values which represents a
//                       integral or real numeric value. This includes bool
//                       and RtNone. Any operation appplied to none result in
//                       none. Any operation applied with None results in the
//                       input value.
// }
//---------------------------------------------------------------------------//
template <class T>
concept iOneOfNativeLiterals =
    mta::req::iOneOf<NativeIntT, NativeUnsignedT, NativeDoubleT, NativeBoolT,
                     NativeCharT, NativeByteT, NativeCStringT, NativeNoneT,
                     NativeUndefinedT, NativePtrT>;

template <class T>
concept iOneOfNativeArithmetic =
    mta::req::iOneOf<NativeIntT, NativeUnsignedT, NativeDoubleT, NativeBoolT,
                     NativeCharT, NativeByteT, NativeNoneT>;
template <class T>
concept iOneOfNativeNonLiterals =
    mta::req::iOneOf<NativeStringT, NativeNamespaceT, NativeObjectT,
                     NativeMethodT, NativeCppMethodT, NativeListT>;
//---------------------------------------------------------------------------//
// EndSection:{Native Type Concepts}
//---------------------------------------------------------------------------//

//---------------------------------------------------------//
// Class:{RtVal}
// Brief:{}
//---------------------------------------------------------//
class RtVal {
 public:
  constexpr static inline RtVal NewNone() {
    RtVal new_val;
    new_val.data_ = NativeNoneT();
    return new_val;
  }

  constexpr static inline RtVal NewUndefined() {
    RtVal new_val;
    new_val.data_ = NativeUndefinedT();
    return new_val;
  }

 public:
  constexpr size_t Idx() const { return data_.index(); }

  constexpr eNativeTypeIndex::enum_type NativeIdx() const {
    return static_cast<eNativeTypeIndex::enum_type>(data_.index());
  }

  constexpr NativeVariant& Variant() { return data_; }

  constexpr inline void Copy(const NativeVariant& other) {
    const auto xCopyRtVal = [this](const auto& other_val) constexpr {
      using T = std::decay_t<decltype(other_val)>;
      if constexpr (iOneOfNativeLiterals<T>) {
        data_ = other_val;
      } else if constexpr (iOneOfNativeNonLiterals<T>) {
        data_ = std::make_unique<std::decay_t<T>::element_type>(*other_val);
      } else {
        throw std::logic_error("Unknown type in RtVal copy constructor.");
      }
    };
    mta::visit_overloaded(other, xCopyRtVal);
  }

  constexpr inline void Copy(const RtVal& other) { Copy(other.data_); }

  constexpr inline RtVal Clone() const {
    RtVal new_val;
    new_val.Copy(data_);
    return new_val;
  }

  template <class T>
  constexpr T& GetDataRef() {
    return get<T>(data_);
  }

  template <class T>
  constexpr const T& GetDataView() {
    return get<T>(data_);
  }

  template <class T>
  constexpr T GetDataCopy() {
    return get<T>(value_);
  }

 public:
  constexpr RtVal() = default;
  constexpr RtVal(const RtVal& other) : RtVal(other.data_){};
  constexpr RtVal(RtVal&& other) = default;
  constexpr RtVal(const NativeVariant& other) { Copy(other); }
  constexpr RtVal& operator=(const NativeVariant& other) {
    Copy(other);
    return *this;
  }
  constexpr RtVal& operator=(const RtVal& other) {
    Copy(other);
    return *this;
  }

 private:
  NativeVariant data_;
};
//---------------------------------------------------------//
// EndClass:{RtVal}
//---------------------------------------------------------//

//---------------------------------------------------------//
// Class:{NamedRtVal}
// Brief:{
//  Container for a RtVal with a string_view name.
//  The name should not be modified after construction.
//  The RtVal is stored as a const pointer.
//  NamedRtVal does not own the value it points to.
// }
//---------------------------------------------------------//
class NamedRtVal {
 public:
  using ValPtr = RtVal* const;
  constexpr const string_view& Name() const { return name_; }
  constexpr inline eNativeTypeIndex::enum_type NativeIdx() const {
    return value_->NativeIdx();
  }
  constexpr RtVal& Value() { return *value_; };
  constexpr RtVal* const ValuePtr() const { return value_; };
  constexpr void SetValue(RtVal& new_val) { value_ = &new_val; }
  constexpr bool IsNull() const { return value_ == nullptr; }

 public:
  constexpr NamedRtVal(const string_view& name, RtVal& value)
      : name_(name), value_(&value) {}
  constexpr NamedRtVal(const string_view& name)
      : name_(name), value_(nullptr) {}

  constexpr bool operator==(const NamedRtVal& other) const {
    return name_ == other.name_;
  }

  constexpr bool operator==(const string_view& other) const {
    return name_ == other;
  }

 private:
  string_view name_{""};
  RtVal* value_{nullptr};
};

// C++20's transparent hashing pattern.
// https://en.cppreference.com/w/cpp/container/unordered_set/find
// Use: std::unordered_set<NamedRtVal, NamedRtValHash, std::equal_to<>>
struct NamedRtValHash {
  using hash_type = std::hash<string_view>;
  using is_transparent = void;

  std::size_t operator()(const char* str) const { return hash_type{}(str); }
  std::size_t operator()(std::string_view str) const {
    return hash_type{}(str);
  }
  std::size_t operator()(std::string const& str) const {
    return hash_type{}(str);
  }
};
//---------------------------------------------------------//
// EndClass:{NamedRtVal}
//---------------------------------------------------------//

//---------------------------------------------------------//
// Class:{NamedRtValSet}
// Brief:{
//  Container for a RtVal with a string_view name.
//  The name should not be modified after construction.
// }
//---------------------------------------------------------//

class NamedRtValSet {
 public:
  using Set = std::unordered_set<NamedRtVal, NamedRtValHash, std::equal_to<>>;
  using SetIter = Set::iterator;
  using SetConstIter = Set::const_iterator;
  using ExpectedRtValPtr = cxx::Expected<NamedRtVal::ValPtr>;

  ExpectedRtValPtr GetRtValPtr(const string_view& name) {
    if (data_.contains(name)) {
      return ExpectedRtValPtr::Success(data_.find(name)->ValuePtr());
    } else
      return ExpectedRtValPtr::Failure(
          string("[NamedRtValSet][GetRtVal] the name '") + name.data() +
          "' does not exist in this set.");
  }

  ExpectedRtValPtr GetRtValPtr(size_t idx) {
    if (idx < data_.size()) {
      auto it = data_.begin();
      std::advance(it, idx);
      return ExpectedRtValPtr::Success(it->ValuePtr());
    } else
      return ExpectedRtValPtr::Failure(
          string("[NamedRtValSet][GetRtVal] the index '") +
          std::to_string(idx) + "' is out of bounds.");
  }

  // Returns a reference to the RtVal with the given name.
  // !!Unchecked access!! If not found, undefined behavior.
  RtVal& GetRtVal(const string_view& name) {
    return *data_.find(name)->ValuePtr();
  }

  // Returns a reference to the RtVal with the given index.
  // !!Unchecked access!! If not found, undefined behavior.
  RtVal& GetRtVal(size_t idx) {
    auto it = data_.begin();
    std::advance(it, idx);
    return *it->ValuePtr();
  }

  bool Contains(const string_view& name) const { return data_.contains(name); }

  bool Contains(size_t idx) const { return idx < data_.size(); }

  cxx::BoolError Emplace(NamedRtVal&& named_val) {
    auto insertion_result = data_.emplace(forward<NamedRtVal>(named_val));
    return insertion_result.second;
  }

  cxx::BoolError Emplace(const string_view& name, RtVal& value) {
    return Emplace(NamedRtVal(name, value));
  }

  cxx::BoolError Erase(const string_view& name) {
    if (Contains(name)) {
      data_.erase(data_.find(name));
      return true;
    } else
      return string("[NamedRtValSet][Erase] Name '") + name.data() +
             "' does not exist in this set.";
  }

  cxx::BoolError Erase(size_t idx) {
    if (Contains(idx)) {
      auto it = data_.begin();
      std::advance(it, idx);
      data_.erase(it);
      return true;
    } else
      return string("[NamedRtValSet][Erase] Index '") + std::to_string(idx) +
             "' is out of bounds.";
  }

  cxx::BoolError PopBack() {
    if (data_.empty()) {
      return "[NamedRtValSet][PopBack] Set is empty.";
    } else {
      data_.erase(std::prev(data_.end()));
      return true;
    }
  }

  // !!Unchecked access!! If set is empty, undefined behavior.
  const NamedRtVal& Back() const { return *std::prev(data_.end()); }

  void Clear() { data_.clear(); }

 public:
  NamedRtValSet() = default;
  NamedRtValSet(const NamedRtValSet&) = default;
  NamedRtValSet(NamedRtValSet&&) = default;
  NamedRtValSet& operator=(const NamedRtValSet&) = default;
  NamedRtValSet& operator=(NamedRtValSet&&) = default;

  // Utility constructor for creating a null set from a list of names.
  NamedRtValSet(mta::req::iConvertibleTo<string_view> auto... names)
      : data_(NamedRtVal{names}...) {
    assert(mta::are_unique(names...) &&
           "[NamedRtValSet][NamedRtValSet] List of names passed to constructor "
           "must be unique.");
  }

 private:
  Set data_;
};
//---------------------------------------------------------//
// EndClass:{NamedRtValSet}
//---------------------------------------------------------//

//---------------------------------------------------------//
// Class:{RtNone}
// Brief:{}
//---------------------------------------------------------//
struct RtNone {
  constexpr bool operator==(const RtNone&) const { return true; }
  template <class T>
  constexpr bool operator==(const T&) const {
    return false;
  }
};
//---------------------------------------------------------//
// EndClass:{RtNone}
//---------------------------------------------------------//

//---------------------------------------------------------//
// Class:{RtUndefined}
// Brief:{}
//---------------------------------------------------------//
struct RtUndefined {
  constexpr bool operator==(const RtUndefined&) const { return true; }
  template <class T>
  constexpr bool operator==(const T&) const {
    return false;
  }
};
//---------------------------------------------------------//
// EndClass:{RtUndefined}
//---------------------------------------------------------//

//---------------------------------------------------------//
// Class:{RtDynNamespace}
// Brief:{ Represents a static namespace, object definition or
//         the global namespace in the C& runtime.}
// Detail:{
//  NOTE: Dyn namespace type enumation is named 'eCategories'
//        to avoid confusion with types.
//
//  A namespace in C& can be one of four types:
//    - Undefined namespace, always named '#',
//      an undefined or invalid namespace/class def.
//      '#' can never be a namespace name, here its used to
//      signify the null namespace.
//
//    - A static namespace, containing static members only.
//      Any re-definition of this namespace will add to
//      the existing namespace.
//
//    - A class definition which is a static namespace,
//      containing a default generated _init_ method
//      which describes the contruction of the non-static
//      members of a RtDynObject copied from the class def's
//      instance members.
//      Contains an _rtti_ value which is the unique runtime
//      type index of this class.
//      For now 0 will represent the null type, or lack-of-type.
//      Any re-definition of this class namespace is an error.
//
//    - The global namespace of this program instance. Same
//      as a static namespace with special optimizations.
//      The name of the global namespace should be '_global_'.
//
//  A namespace may have a parent namespace, which is called
//  to when resolving a name. The global namespace has no parent.
//  All other namespaces have a parent or are children of the
//  global namespace.
//  Upon querying the pare of global or undefined namespaces,
//  the same namespace is returned.
// }
//---------------------------------------------------------//
class RtDynNamespace {
 public:
  using ExpectedRtValPtr = NamedRtValSet::ExpectedRtValPtr;

  enum class eCategory { kUndefined, kNamespace, kClass, kGlobal };

  // !!DO NOT MODIFY or ACCESS!!
  // Static null namespace used by default constructor of RtDynObject
  static RtDynNamespace gStaticUndefinedNamespace;

 public:
  constexpr eCategory Category() const { return category_; }
  constexpr size_t Rtti() const { return rtti_; }
  constexpr string_view Name() const { return name_; }

  // !!Warning!!
  // If namespace or class def has a nullptr parent, undefined behavior.
  constexpr RtDynNamespace& Parent() {
    if (IsUndefined() || IsGlobal()) return *this;
    return *parent_;
  }
  constexpr const RtDynNamespace& Parent() const {
    if (IsUndefined() || IsGlobal()) return *this;
    return *parent_;
  }
  constexpr bool IsUndefined() const {
    return category_ == eCategory::kUndefined;
  }
  constexpr bool IsNamespace() const {
    return category_ == eCategory::kNamespace;
  }
  constexpr bool IsClass() const { return category_ == eCategory::kClass; }
  constexpr bool IsGlobal() const { return category_ == eCategory::kGlobal; }

  constexpr bool AddInstanceDecl(const NamedRtVal& named_val) {
    return instance_decls_.Emplace(NamedRtVal(named_val));
  }

  constexpr bool AddInstanceDecl(NamedRtVal&& named_val) {
    return instance_decls_.Emplace(forward<NamedRtVal>(named_val));
  }

  constexpr bool AddInstanceDecl(const string_view& name, RtVal& value) {
    return instance_decls_.Emplace(NamedRtVal(name, value));
  }

  constexpr bool AddDecl(const NamedRtVal& named_val) {
    return decls_.Emplace(NamedRtVal(named_val));
  }

  constexpr bool AddDecl(NamedRtVal&& named_val) {
    return decls_.Emplace(forward<NamedRtVal>(named_val));
  }

  constexpr bool AddDecl(const string_view& name, RtVal& value) {
    return decls_.Emplace(NamedRtVal(name, value));
  }

 public:
  constexpr const NamedRtValSet& InstanceDecls() const {
    return instance_decls_;
  }

  constexpr bool ContainsLocal(const string_view& name) const {
    return decls_.Contains(name);
  }

  constexpr ExpectedRtValPtr ResolveLocal(const string_view& name) {
    return decls_.GetRtValPtr(name);
  }

  // Faster than regular resolve but undefined name does not exist.
  constexpr RtVal& ResolveLocalUnchecked(const string_view& name) {
    return decls_.GetRtVal(name);
  }

  constexpr bool Contains(const string_view& name) const {
    if (ContainsLocal(name)) return true;
    if (IsGlobal()) return false;
    return Parent().Contains(name);
  }

  // It is assumed that this is never called on an undefined namespace.
  // It is assumed that all static namespace and class defs have a parent.
  constexpr ExpectedRtValPtr Resolve(const string_view& name) {
    if (ContainsLocal(name)) {
      return ResolveLocal(name);
    } else {
      if (IsGlobal())
        return ExpectedRtValPtr::Failure(
            string("Global namespace does not contain the variable named '") +
            name.data() + "'.");
      return Parent().Resolve(name);
    }
  }

  // !!Unchecked access!! If not found, undefined behavior.
  // Use Contains() to check if the name exists in this hierarchy.
  // Checks all namespaces in the hierarchy.
  constexpr RtVal& ResolveUnchecked(const string_view& name) {
    if (ContainsLocal(name)) {
      return ResolveLocalUnchecked(name);
    } else {
      return Parent().ResolveUnchecked(name);
    }
  }

 public:
  RtDynNamespace() = default;
  RtDynNamespace(const RtDynNamespace&) = default;
  RtDynNamespace(RtDynNamespace&&) = default;
  RtDynNamespace& operator=(const RtDynNamespace&) = default;
  RtDynNamespace& operator=(RtDynNamespace&&) = default;

  RtDynNamespace(const string_view& name)
      : category_(eCategory::kNamespace), name_(name) {}

 private:
  eCategory category_{eCategory::kUndefined};
  size_t rtti_{kUndefinedRtti};
  RtDynNamespace* parent_{nullptr};
  string_view name_{kUndefinedNamespaceName};
  NamedRtValSet decls_;           // Declarations in this namespace.
  NamedRtValSet instance_decls_;  // non-static members.
                                  // copied over for each instance of a class.
};
// !!DO NOT MODIFY or ACCESS!!
// Static null namespace used by default constructor of RtDynObject
RtDynNamespace RtDynNamespace::gStaticUndefinedNamespace{};

//---------------------------------------------------------//
// EndClass:{RtDynNamespace}
//---------------------------------------------------------//

//---------------------------------------------------------//
// Class:{RtDynObject}
// Brief:{
// A dynamic object is the same as a namespace- but can also contain 'static'
// members. These belong to all instances of this object- stored in a reference
// to a dynamic namespace.
// }
//---------------------------------------------------------//
class RtDynObject {
 public:
  constexpr size_t Rtti() const { return rtti_; }
  constexpr const string_view& Name() const { return class_def_.Name(); }

 public:
  // Creates a new object from the currently linked class def.
  // NOTE: Resolving and calling the _init_ method of the class
  // def is done by the runtime ir-code executor.
  RtDynObject InitFromNamespace() {
    member_decls_ = class_def_.InstanceDecls();
  }

 public:
  // When default constructing an object, the class def is the global
  // gStaticUndefinedNamespace. This is the default state of an object.
  RtDynObject() = default;

  // Initialization of the member_decls_ is offset to
  // a separate method to allow initializing undefined object.
  RtDynObject(RtDynNamespace& class_def)
      : class_def_(class_def), rtti_(class_def.Rtti()), member_decls_() {}

 private:
  RtDynNamespace& class_def_{RtDynNamespace::gStaticUndefinedNamespace};
  size_t rtti_{kUndefinedRtti};
  NamedRtValSet member_decls_{};
};
//---------------------------------------------------------//
// EndClass:{RtDynObject}
//---------------------------------------------------------//

//---------------------------------------------------------//
// Class:{RtDynMethod}
// Brief:{
//  Represents a dynamic method in the C& runtime.
//  A method is a set of instructions which can be executed.
//  The instructions may be any C++ void method which
//  takes a RtDynNamespace& argument - but it is
//  Expected that the method is a bound  IrCodeAction which will
//  execute an action on the runtime memory stack. Resolving
//  the variable values from the currently bound context.
// }
//---------------------------------------------------------//
class RtDynMethod {
 public:
  constexpr const string_view& Name() const { return name_; }
  constexpr const NamedRtValSet& Arguments() const { return arguments_; }
  constexpr const RtDynNamespace& Context() const { return context_; }
  void Call() {
    for (auto& instruction : instructions_) {
      instruction(context_);
    }
  }

 private:
  string_view name_{""};
  NamedRtValSet arguments_{};
  RtDynNamespace& context_{RtDynNamespace::gStaticUndefinedNamespace};
  std::vector<std::function<void(RtDynNamespace&)>> instructions_{
      [](auto&) {}};  // codes for executing the method at runtime.
};
//---------------------------------------------------------//
// EndClass:{RtDynMethod}
//---------------------------------------------------------//

//---------------------------------------------------------//
// Class:{RtCppMethod}
// Brief:{}
//---------------------------------------------------------//
class RtCppMethod {
  using CppMethodT = RtVal (*)(const NamedRtValSet&);

 public:
  static const NamedRtValSet kEmptyArgs;

 public:
  RtVal Call() const { return method_(NamedRtValSet()); }
  RtVal Call(const NamedRtValSet& args) const { return method_(args); }
  constexpr RtCppMethod() : method_(nullptr) {}
  constexpr RtCppMethod(CppMethodT method) : method_(method) {}

 private:
  CppMethodT method_;
};
//---------------------------------------------------------//
// EndClass:{RtCppMethod}
//---------------------------------------------------------//

class RtDynPtr {
 public:
  constexpr inline eNativeTypeIndex::underlying_type TypeIndex() {
    if (ptr_ == nullptr) return eNativeTypeIndex::kEmpty;
    return ptr_->Idx();
  };
  RtVal& Get() { return *ptr_; }
  bool IsNull() { return ptr_ == nullptr; }

 public:
  RtDynPtr() = default;
  RtDynPtr(RtVal* ptr) : ptr_(ptr) {}
  RtDynPtr(const RtDynPtr& other) : ptr_(other.ptr_) {}
  RtDynPtr(RtDynPtr&& other) : ptr_(other.ptr_) {}
  RtDynPtr(RtVal& val) : ptr_(&val) {}

 private:
  RtVal* ptr_{nullptr};
};

//---------------------------------------------------------//
// Class:{RtDynList}
// Brief:{}
//---------------------------------------------------------//
class RtDynList {
 public:
  using List = std::vector<RtVal>;
  List data_;
};
//---------------------------------------------------------//
// EndClass:{RtDynList}
//---------------------------------------------------------//

}  // namespace caoco

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
// File: caoco_rtval.h
//---------------------------------------------------------------------------//
#endif HEADER_GUARD_CALE_CAND_OFFICIAL_COMPILER_CAOCO_RTVAL_H
//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
