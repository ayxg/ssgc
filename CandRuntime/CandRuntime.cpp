// CandRuntime.cpp : This file contains the 'main' function. Program execution
// begins and ends there.
//
#include "ext/meta_template_archive/mta.h"
#include "ext/cpp_standard_extended/cppsextended.h"

#include <cassert>
#include <functional>
#include <iostream>
#include <memory>
#include <stack>
#include <string>
#include <tuple>
#include <type_traits>
#include <variant>
#include <vector>

using std::shared_ptr;
using std::size_t;
using std::string;
using std::string_view;
using std::variant;
using std::vector;
using std::weak_ptr;
// Template Utilities.

//template <template <class...> class tmpl, typename T>
//struct is_template_for_impl : public std::false_type {};
//
//template <template <class...> class tmpl, class... Args>
//struct is_template_for_impl<tmpl, tmpl<Args...>> : public std::true_type {};
//
//template <template <class...> class tmpl, typename... Ts>
//using is_template_for =
//    std::conjunction<is_template_for_impl<tmpl, std::decay_t<Ts>>...>;
//
//template <template <class...> class tmpl, typename... Ts>
//constexpr bool is_template_for_v = is_template_for<tmpl, Ts...>::value;
//
//template <typename...>
//inline constexpr auto is_unique_type_in_tuple = std::true_type{};
//
//template <typename T, typename... Rest>
//inline constexpr auto is_unique_type_in_tuple<T, Rest...> =
//    std::bool_constant<(!std::is_same_v<T, Rest> && ...) &&
//                       is_unique_type_in_tuple<Rest...>>{};

template <typename T, typename U, typename... Us>
constexpr auto getTupleIndexImpl() {
  if constexpr (std::is_same<T, U>::value) {
    return 0;
  } else {
    static_assert(sizeof...(Us) > 0, "This tuple does not have that type");
    return 1 + getTupleIndexImpl<T, Us...>();
  }
}

template <typename T, typename U, typename... Us>
constexpr auto getTupleIndex(const std::tuple<U, Us...>&) {
  static_assert(is_unique_pack<U, Us...>,
                "getIndex should only be called on tuples with unique types.");
  return getTupleIndexImpl<T, U, Us...>();
}

// Base case
template <typename... Ts>
constexpr std::variant<Ts...> tupleToVariant(const std::tuple<Ts...>& tuple) {
  return std::get<0>(tuple);  // Return the first element of the tuple
}
// Recursive case
template <typename T, typename... Ts>
constexpr std::variant<T, Ts...> tupleToVariant(
    const std::tuple<T, Ts...>& tuple) {
  return std::variant<T, Ts...>(std::get<0>(tuple));
}

// CompileTimeTypeIndex is a utility class that stores a type and an index.
template <class T, size_t Index>
struct CompileTimeTypeIndex {
  using type = T;
  static constexpr size_t idx = Index;
};
// Comparisor operator for CompileTimeTypeIndex
template <class T, size_t Index, class U, size_t Index2>
constexpr static bool operator==(const CompileTimeTypeIndex<T, Index>& lhs,
                                 const CompileTimeTypeIndex<T, Index2>& rhs) {
  return lhs.idx == rhs.idx;
}

template <class... TypeTs>
struct CompileTimeTypeIndexList {
  static constexpr inline auto kNumTypes = sizeof...(TypeTs);
  // Tuple Properties.
  using types_tuple = std::tuple<TypeTs...>;
  static inline types_tuple kTypesTuple = types_tuple{};
  // Variant Properties.
  static inline auto kTypesVariant = tupleToVariant(kTypesTuple);
  using types_variant = decltype(kTypesVariant);
  // Induvidual Type Access
  template <size_t index>
  using type_of = typename std::tuple_element_t<index, types_tuple>::type;

  template <class T>
  struct IndexOf {
    using type = T;
    static constexpr auto idx = getTupleIndex<T>(kTypesTuple);
  };

  template <class T>
  constexpr static size_t index_of() {
    return IndexOf<T>::idx;
  }
};

template <class T>
concept iCompileTimeTypeIndexList = requires(T v) {
  requires is_template_for_v<std::tuple, typename T::types_tuple>;
  requires is_template_for_v<std::variant, typename T::types_variant>;
  requires T::kNumTypes > 0;
  requires T::kTypesTuple == tupleToVariant(T::kTypesTuple);
  requires T::kTypesVariant == tupleToVariant(T::kTypesTuple);
};
/// Counter Reference Object Template
/// countet_ref_object is a class that manages a shared pointer to an object.
/// </summary>
template <class T>
struct counted_ref {
  const size_t& m_count;
  shared_ptr<T> m_ptr{nullptr};

  constexpr counted_ref(shared_ptr<T> ptr, size_t& count)
      : m_ptr(ptr), m_count(count) {}
  constexpr size_t use_count() { return m_count; }
  constexpr void reset() { m_ptr.reset(); }
};

template <class T>
struct weak_ref {
  bool is_locked_{false};
  size_t& m_count;
  weak_ptr<T> m_ptr{};

  constexpr weak_ref(weak_ptr<T> ptr, size_t& count)
      : m_ptr(ptr), m_count(count) {}
  constexpr size_t use_count() { return m_count; }
  constexpr counted_ref<T> lock() {
    is_locked_ = true;
    auto&& locked = m_ptr.lock();
    m_count++;
    return counted_ref<T>(locked, m_count);
  }
  constexpr void unlock(counted_ref<T>&& ref) {
    is_locked_ = false;
    ref.reset();
    m_count--;
  }
  constexpr bool is_locked() const { return is_locked; }
};

template <class T>
class counted_ref_object {
  shared_ptr<T> m_ptr{nullptr};
  size_t m_count{0};

 public:
  constexpr counted_ref_object() = default;  // Default ctor 0 count and nullptr

  // Void arg ctor
  constexpr counted_ref_object& ctor() {
    m_ptr = std::shared_ptr<T>(new T());
    m_count = 1;
    return *this;
  }

  // Var arg ctor
  template <class... U>
  constexpr counted_ref_object& ctor(U&&... args) {
    m_ptr = std::shared_ptr<T>(new T(std::forward<U>(args)...));
    m_count = 1;
    return *this;
  }

  // Get a direct mutable reference to the stored object.
  // Does not increase the ref count.
  constexpr T& get() { return *m_ptr; }

  // Get a direct const reference to the stored object.
  // Does not increase the ref count.
  constexpr const T& getc() const { return *m_ptr; }

  // Create a new counted reference to the object.Increases the ref count.
  constexpr counted_ref<T> ref() {
    m_count++;
    return counted_ref(m_ptr, m_count);
  }

  // Create a new weak reference to the object. Does not increase the ref count.
  // Use lock to get a counted reference.
  constexpr weak_ref<T> weak() { return weak_ref<T>(m_ptr, m_count); }

  // Check if the object is unique, i.e. only one reference to the object.
  constexpr bool is_unique() { return m_count == 1; }

  // Check if the object is expired, i.e. no references to the object.
  // Indicated the object is ready to be destroyed - or is not initialized.
  // Should never occur as the object should be destroyed when the ref count
  // reaches 0. This is a safety check.
  constexpr bool is_expired() { return m_count == 0; }

  // Check if the object is empty, i.e. no object is stored.
  constexpr bool is_empty() { return m_ptr == nullptr; }

  // Unref the object, decrease the ref count.
  // Consumes the counted reference.
  // Asserts both objects point to the same object.
  constexpr void unref(counted_ref<T>&& unref_this) {
    assert(m_ptr.get() == unref_this.m_ptr.get() &&
           "Unref called with a different object.");
    unref_this.reset();
    m_count--;
    if (m_count == 0) {
      m_ptr.reset();
    }
  }

  // Completley unref the object, no references left.
  // Destroys the object.
  // Use is_unique to check if the object is the last remaining reference before
  // calling this.
  constexpr void unref_last() {
    assert(
        is_unique() &&
        "Pointer to object must be unique before unrefing the last instance.");
    m_count--;
    m_ptr.reset();
  }

  // Current ref count.
  constexpr size_t use_count() { return m_count; }
};

// Builtin Abstract object types.

// None object represents a null or nothing.
// Performing an operation with none will result in no change.
// Functions may act diffrently if none is pass. Indicating a lack of argument.
struct BuiltinNone {
  // Always equal to all other none objects.
  constexpr bool operator==(const BuiltinNone&) const { return true; }
  // Always not equal to all other object types.
  template <class T>
  constexpr bool operator==(const T&) const {
    return false;
  }
};

// Represents an uninitialized or destroyed value. Invalid for use.
struct BuiltinUndefined {
  // Always equal to all other undefined objects.
  constexpr bool operator==(const BuiltinNone&) const { return true; }
  // Always not equal to all other object types.
  template <class T>
  constexpr bool operator==(const T&) const {
    return false;
  }
};

// fwd decl
class DynamicRuntimeNamespace;
// Forward declaration for the dynamic runtime object class, stored in the
// runtime variant class.
class DynamicRuntimeObject;
// Forward declaration for the runtime method class, stored in the runtime
// variant and dynamic runtime object.
class DynamicRuntimeMethod;
// Forward declaration for the native_runtime_method class, stored in the
// runtime variant.
class CppRuntimeMethod;
// Forward declaration for the runtime_vector class contained in the runtime
// variant.
class DynamicRuntimeArray;
// Fwd decl for dynamic address class contained in the runtime variant.
class DynamicAddress;

// Typedefs for native variant types
using NativeIntT = int;
using NativeUnsignedT = unsigned;
using NativeDoubleT = double;
using NativeBoolT = bool;
using NativeCharT = char;
using NativeByteT = unsigned char;
using NativeNoneT = BuiltinNone;
using NativeUndefinedT = BuiltinUndefined;
using NativeDynamicNamespaceT = counted_ref_object<DynamicRuntimeNamespace>;
using NativeDynamicObjectT = counted_ref_object<DynamicRuntimeObject>;
using NativeDynamicMethodT = counted_ref_object<DynamicRuntimeMethod>;
using NativeDynamicAddressT = counted_ref_object<DynamicAddress>;
using NativeStringT = counted_ref_object<string>;
using NativeCppMethodT = counted_ref_object<CppRuntimeMethod>;
using NativeDynamicRuntimeArrayT = counted_ref_object<DynamicRuntimeArray>;

// Native type index list
using NativeTypes = CompileTimeTypeIndexList<
    // Literals
    NativeIntT, NativeUnsignedT, NativeDoubleT, NativeBoolT, NativeCharT,
    NativeByteT, NativeNoneT, NativeUndefinedT,

    // (Ref Counted) Dynamic Objects
    NativeDynamicNamespaceT, NativeDynamicObjectT, NativeDynamicMethodT,
    NativeDynamicAddressT,

    // (Ref Counted) Builtins
    NativeStringT, NativeCppMethodT, NativeDynamicRuntimeArrayT>;

using NativeVariant = NativeTypes::types_variant;

// Enumeration of native variant types.
enum class eNativeTypeIndex : size_t {
  kInt = NativeTypes::IndexOf<NativeIntT>::idx,
  kUnsigned = NativeTypes::IndexOf<NativeUnsignedT>::idx,
  kDouble = NativeTypes::IndexOf<NativeDoubleT>::idx,
  kBool = NativeTypes::IndexOf<NativeBoolT>::idx,
  kChar = NativeTypes::IndexOf<NativeCharT>::idx,
  kByte = NativeTypes::IndexOf<NativeByteT>::idx,
  kNone = NativeTypes::IndexOf<NativeNoneT>::idx,
  kUndefined = NativeTypes::IndexOf<NativeUndefinedT>::idx,
  kDynamicNamespace = NativeTypes::IndexOf<NativeDynamicNamespaceT>::idx,
  kDynamicObject = NativeTypes::IndexOf<NativeDynamicObjectT>::idx,
  kDynamicMethod = NativeTypes::IndexOf<NativeDynamicMethodT>::idx,
  kDynamicAddress = NativeTypes::IndexOf<NativeDynamicAddressT>::idx,
  kString = NativeTypes::IndexOf<NativeStringT>::idx,
  kCppMethod = NativeTypes::IndexOf<NativeCppMethodT>::idx,
  kDynamicRuntimeArray = NativeTypes::IndexOf<NativeDynamicRuntimeArrayT>::idx,
};

class RuntimeValue {
  eNativeTypeIndex native_index_{eNativeTypeIndex::kUndefined};
  NativeVariant value_{
      NativeUndefinedT()};  // Todo: factory function for creating native type
                            // from native type indexes
 public:
  template <class T>  // todo: require type to be one of the native variant
                      // types.
  constexpr T& GetRef() {
    return std::get<T>(value_);
  }

  template <class T>
  constexpr T GetCopy() {
    return std::get<T>(value_);
  }

  // For visitng the variant from C++ methods.
  constexpr NativeVariant& GetUnderlyingVariant() { return value_; }

 public:
  constexpr RuntimeValue() = default;
  constexpr RuntimeValue(eNativeTypeIndex idx, NativeVariant native_var)
      : native_index_(idx), value_(native_var) {}
  template <class T>
    requires(not std::is_same_v<T, RuntimeValue>)
  static constexpr RuntimeValue New(T&& val) {
    RuntimeValue rtval;
    rtval.value_ = std::forward<T>(val);
    rtval.native_index_ = static_cast<eNativeTypeIndex>(rtval.value_.index());
    return rtval;
  }
  constexpr eNativeTypeIndex NativeIndex() const { return native_index_; }
  static constexpr inline RuntimeValue NewUndefined() { return RuntimeValue{}; }
  static constexpr inline RuntimeValue NewNone() {
    return RuntimeValue{eNativeTypeIndex::kNone, NativeVariant(NativeNoneT{})};
  }
};

class NamedRuntimeValue {
  string_view name_{""};
  RuntimeValue value_;

 public:
  constexpr string_view& Name() { return name_; }
  constexpr RuntimeValue& Value() { return value_; };
  constexpr inline eNativeTypeIndex NativeIndex() {
    return value_.NativeIndex();
  }
  constexpr NamedRuntimeValue(string_view name, RuntimeValue value)
      : name_(name), value_(value) {}
};

class NamedRuntimeValueSet {
  vector<NamedRuntimeValue> values_;

 public:
  constexpr NamedRuntimeValueSet() {}
  constexpr auto Begin() { return values_.begin(); }
  constexpr auto End() { return values_.end(); }

  constexpr vector<NamedRuntimeValue>::iterator Find(const string_view& name) {
    return std::find_if(Begin(), End(), [&name](NamedRuntimeValue& v) {
      return v.Name() == name;
    });
  }

  constexpr bool Contains(const string_view& name) {
    auto it = Find(name);
    if (it != values_.end()) {
      return true;
    }
    return false;
  }

  constexpr bool Push(NamedRuntimeValue named_rtval) {
    if (Contains(named_rtval.Name())) {
      return false;
    } else {
      values_.push_back(named_rtval);
      return true;
    }
  };
};

class CppRuntimeMethod {
  RuntimeValue (*method_)(NamedRuntimeValueSet);

 public:
  constexpr CppRuntimeMethod() : method_(nullptr) {}
  constexpr CppRuntimeMethod(RuntimeValue (*method)(NamedRuntimeValueSet))
      : method_(method) {}
  RuntimeValue Call() { return method_(NamedRuntimeValueSet()); }
  RuntimeValue Call(NamedRuntimeValueSet args) { return method_(args); }
};

enum class eDynNamespaceCategory {
  kUndefined,  // Null namespace, always named '#', an invalid namespace name.
  kNamespace,  // A regular namespace, containing static members only.
  kClassDef,   // A class definition, the ctor_ and rtti_ runtime values contain
               // the class metadata.
};

class DynamicRuntimeNamespace {
  eDynNamespaceCategory category_{eDynNamespaceCategory::kUndefined};
  string_view name_{"#"};  // # can never be a namespace name, here its used to
                           // signify the null namespace
  NamedRuntimeValueSet members_{};

  // Object namespace only
  // For now 0 will represent the null type. or lack-of-type.
  unsigned rtti_{
      0};  // Unique type index of the object type (if this is a type)

  // Object constructor containing definition of non-static members.
  NativeDynamicMethodT ctor_{NativeDynamicMethodT().ctor()};

 public:
  DynamicRuntimeNamespace() {}
  DynamicRuntimeNamespace(const string_view& name) : name_(name) {}
  constexpr bool IsNullNamespace() { return name_ == "#"; }

  constexpr bool ContainsMember(const string_view& name) {
    return members_.Contains(name);
  }

  constexpr void AddMember(RuntimeValue val, string_view name) {
    members_.Push(NamedRuntimeValue(name, val));
  }

  constexpr RuntimeValue& AccessMember(const string_view& name) {
    auto it = members_.Find(name);
    if (it != members_.End()) {
      return it->Value();
    } else {
      throw std::runtime_error(
          std::string(
              "DynamicRuntimeNamespace:AccessMember: Member with name '") +
          name.data() + "' does not exist in dynamic object named '" +
          name_.data() + "'.");
    }
  }
};
// !!DO NOT MODIFY!! Static null namespace used by default constructor of
// DynamicRuntimeObject.
static DynamicRuntimeNamespace kNullNamespace = DynamicRuntimeNamespace{};

// A dynamic object is the same as a namespace- but can also contain 'static'
// members. These belong to all instances of this object- stored in a reference
// to a dynamic namespace.
class DynamicRuntimeObject : public DynamicRuntimeNamespace {
  DynamicRuntimeNamespace& static_object_namespace_{kNullNamespace};

 public:
  DynamicRuntimeObject() : DynamicRuntimeNamespace() {}
  DynamicRuntimeObject(const string_view& name)
      : DynamicRuntimeNamespace(name) {}

  void SetStaticObjectNamespace(DynamicRuntimeNamespace& dyn_ns) {
    static_object_namespace_ = dyn_ns;
  }

  constexpr bool ContainsStaticMember(const string_view& name) {
    return static_object_namespace_.ContainsMember(name);
  }

  constexpr void AddStaticMember(RuntimeValue val, const string_view& name) {
    static_object_namespace_.AddMember(std::move(val), name);
  }

  constexpr RuntimeValue& AccessStaticMember(const string_view& name) {
    return static_object_namespace_.AccessMember(name);
  }
};
class ProgramActionBlock;

class DynamicRuntimeMethod {
  string_view name_;
  NamedRuntimeValueSet arguments_;
  ProgramActionBlock*
      instructions_;  // codes for executing the method at runtime.
};

class DynamicRuntimeArray {
  vector<RuntimeValue> data_;
};

// Types of arguments which can be passed to actions.
using ProgramActionArgVariant =
    std::variant<NativeIntT, NativeUnsignedT, NativeDoubleT, NativeBoolT,
                 NativeCharT, NativeByteT, NativeNoneT, NativeUndefinedT,
                 NativeStringT>;

struct ActionBase;  // Contains methods/members common to all actions.
// Compiler Utils
struct ActionDebugPrint;
// Variables
struct ActionDeclareVariable;  // Args: name, line index of definition or
                               // nothing.
struct ActionAssignVariable;

// Memory
struct ActionAllocateMemory {};   // allocate a RuntimeValue on the cold stack,
                                  // return index.
struct ActionAllocateLiteral {};  // Allocate a literal value onto the cold
                                  // stack.return index.
struct ActionAllocatoTo {};   // Alllocate value to an index on the cold stack.
struct ActionAllocateHot {};  // Push a named value onto the hot heap.Continue
                              // reading.

// Classes
struct ActionDefineClass {};  // Create class namespace, register rtti_ ,
                              // generate
                              // ctor_ method.
struct ActionAddClassStaticMember {};  // Add member to class namespace
struct ActionAddClassMember {};  // Add an action line to the ctor_ method of
                                 // the
                                 // given class namespace.
struct ActionConstructClassInstance {
};  // Search for class namespace. Call ctor_
    // method. Must be followed by a
    // ActionAllocatoTo or ActionAllocateHot.

// Operators
struct ActionBinaryOp {};  // Perform binary operation on the bottom of the hot
                           // heap.
struct ActionUnaryOp {
};  // Perform unary operation on the bottom of the hot heap.
struct ActionFuncCall {};  // Execute a method given a name with arguments.

// Variant of all types of actions. An action is synonymous with an opcode or
// intermediate representation line.
using ProgramActionVariant =
    std::variant<ActionDebugPrint, ActionDeclareVariable, ActionAssignVariable>;

// Collection of program actions, synonymous with a collection of OPCODES in
// Python, or llvm IR actions from C++.
class ProgramActionBlock {
  std::vector<ProgramActionVariant> actions_;

 public:
  const std::vector<ProgramActionVariant>& Actions() const { return actions_; }
  ProgramActionBlock() = default;

  template <typename... T>
  ProgramActionBlock(T&&... args)
      : actions_{std::move(ProgramActionVariant(std::forward<T>(args)))...} {}
};

class Program;
// Handles execution of action blocks.
class ProgramActionExecutor {
  Program& program_;

 public:
  ProgramActionExecutor(Program& prog) : program_(prog) {}
  RuntimeValue Execute(const ProgramActionBlock& action_block,
                       DynamicRuntimeNamespace& ns);

  // uses global program namespace when none is passed.
  RuntimeValue Execute(const ProgramActionBlock& action_block);
};

using RtVal = RuntimeValue;
class CompilerStackMemory {
  vector<RtVal> memory_;

 public:
  // Get a ref to the top element of the stack.
  RtVal& Top() { return memory_.back(); }
  const RtVal& Top() const { return memory_.back(); }

  // Top with an offset. Offset must be greater than 0.
  RtVal& Top(size_t offset) {
    assert(offset > 0);
    return memory_[memory_.size() - offset--];
  }
  const RtVal& Top(size_t offset) const {
    assert(offset > 0);
    return memory_[memory_.size() - offset--];
  }

  // Push an element onto the stack.
  void Push(RuntimeValue&& rtv) {
    memory_.push_back(std::forward<RuntimeValue>(rtv));
  }
  // Push an element onto the stack.
  void Push(const RuntimeValue& rtv) {
    memory_.push_back(std::forward<const RuntimeValue>(rtv));
  }
  // Removes the top-of-stack item
  void Pop() { memory_.pop_back(); }

  // Pop a number of items from the top of the stack.
  void PopN(size_t n) {
    while (n != 0) {
      Pop();
      n--;
    }
  }

  // Removes the top-of-stack item and returns an rvalue reference
  RuntimeValue&& PopGet() {
    RuntimeValue val{std::move(memory_.back())};
    memory_.pop_back();
    return std::move(val);
  }

  // Push the i-th item  of the stack to the top of the stack
  // without removing it from its original location
  // assert i > 0
  // STACK.append(STACK[-i])
  void PushCopy(size_t n) {
    RuntimeValue copy_rtv{Top(n)};
    Push(std::move(copy_rtv));
  }

  // Swap the top of the stack with the i-th element:
  // STACK[-i], STACK[-1] = STACK[-1], STACK[-i]
  void Swap(size_t n) {
    RuntimeValue& val_to_move_to = Top(n);
    RuntimeValue val_to_move = Top();
    Top() = val_to_move_to;
    val_to_move_to = val_to_move;
  }

  // Unary operations take the top of the stack, apply the operation, and push
  // the result back on the stack.
  // Implements STACK[-1] = <UNARY_OPERATOR>STACK[-1].
  void UnaryOperation(std::function<void(RuntimeValue&)> op) {
    op(Top());
  }

  // Binary and in-place operations
  // Binary operations remove the top two items from the stack(STACK[-1] and
  //                                                          STACK[-2])
  //    .They perform the operation,
  //   then put the result back on the stack.
  //       In
  //       - place operations are like binary operations,
  //   but the operation is done in - place when STACK[-2] supports it,
  //   and the resulting STACK[-1] may
  //      be(but does not have to be) the original STACK[-2]
  //    rhs = STACK.pop()
  //     lhs = STACK.pop()
  //     STACK.append(lhs op rhs)
  void BinaryOperation(std::function<RtVal && (RtVal&&, RtVal&&)> op) {
    RtVal&& rhs = PopGet();
    RtVal&& lhs = PopGet();
    Push(op(std::forward<RtVal>(lhs), std::forward<RtVal>(rhs)));
  }

  void InPlaceBinaryOperation(std::function<void(RtVal&, RtVal&&)> op) {
    RtVal&& rhs = PopGet();
    RtVal& lhs = Top();
    op(lhs, std::forward<RtVal>(rhs));
  }
};

// Program:
// 1. Owns the memory.
//    - Hot Heap : unnamed values, current operation's operands.
//    - Global Namespace: defs, funcs ,vars.
// 2. Owns an instance of an ActionExecutor which refers to this program.
class Program {
  friend class ProgramActionExecutor;
  std::list<RuntimeValue> hot_heap_{};
  std::vector<RuntimeValue> working_stack{};
  DynamicRuntimeNamespace global_namespace_{"_global_"};
  ProgramActionExecutor executor_{*this};

 public:
  RuntimeValue Run(const ProgramActionBlock& program_code) {
    return executor_.Execute(program_code);
  }
};

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// Actions
//////////////////////////////////////////////////////////////////////////////////////////

// Debug print for compiler programmer debugging purposes.
struct ActionDebugPrint {
  string_view name{""};
  ActionDebugPrint() = default;
  ActionDebugPrint(const string_view& name) : name(name) {}

  bool Perform(DynamicRuntimeNamespace& ns) const {
    // Declare an empty(undefined) variable.
    if (!ns.ContainsMember(name)) {
      std::cout << ">[VARIABLE DOES NOT EXIST]" << std::endl;
      return true;  // Variable does not exist.
    }
    auto& val = ns.AccessMember(name);
    std::visit(
        [&](auto& v) {
          using T = std::decay_t<decltype(v)>;  // NOTE: !! Important to decay
                                                // t!! or else ref is inffered
          std::cout << ">[VARIABLE][NAME:" << name.data() << "][VALUE: ";
          if constexpr (std::is_same_v<T, NativeStringT>) {
            std::cout << v.get();
          } else {
            std::cout << "##NOT IMPLEMENTED##";
          }
          std::cout << "]" << std::endl;
        },
        val.GetUnderlyingVariant());
    return true;
  }
};

// Create an undefined named variable
struct ActionDeclareVariable {
  string_view name{""};

  ActionDeclareVariable() = default;
  ActionDeclareVariable(const string_view& name) : name(name) {}

  bool Perform(DynamicRuntimeNamespace& ns) const {
    // Declare an empty(undefined) variable.
    if (ns.ContainsMember(name)) {
      return false;  // Variable is already declared.
    }
    ns.AddMember(RuntimeValue::NewUndefined(), name);
    return true;
  }
};

// Assign the given variable name with a RuntimeValue
struct ActionAssignVariable {
  string_view name{""};
  RuntimeValue val;
  ActionAssignVariable(const string_view& name, RuntimeValue val)
      : name(name), val(val) {}

  bool Perform(DynamicRuntimeNamespace& ns) const {
    if (!ns.ContainsMember(name)) {
      return false;
    }
    // Move value into variable
    ns.AccessMember(name) = std::move(val);
    return true;
  }
};

/////////////////////////////////////////////////////////////////////////////////////////////
// Executor Out of line definitions.

// Out of line definitions or else cannot use program_ variable. Program must be
// defined.
RuntimeValue ProgramActionExecutor::Execute(
    const ProgramActionBlock& action_block, DynamicRuntimeNamespace& ns) {
  for (auto& action_variant : action_block.Actions()) {
    if (not std::visit(
            [&ns](auto& v) {
              using ActionT = decltype(v);
              return v.Perform(ns);
            },
            action_variant)) {
      throw std::runtime_error("Error Executing Action Line.");
    };
  }

  return RuntimeValue::NewUndefined();
}

// Out of line definitions or else cannot use program_ variable. Program must be
// defined.
// // uses global program namespace when none is passed.
RuntimeValue ProgramActionExecutor::Execute(
    const ProgramActionBlock& action_block) {
  return Execute(action_block, program_.global_namespace_);
}

void test_counted_ref_object() {
  counted_ref_object<int> referenced_int;
  std::cout << "\nIs Expired on construction:" << referenced_int.is_expired();
  std::cout << "\nIs Empty on construction:" << referenced_int.is_empty();

  referenced_int.ctor(5);
  std::cout << "\nIs Expired after ctor:" << referenced_int.is_expired();
  std::cout << "\nIs Empty after ctor:" << referenced_int.is_empty();
  std::cout << "\nIs Unique after ctor:" << referenced_int.is_unique();

  counted_ref<int> int_ref = referenced_int.ref();
  std::cout << "\nUse count increase after ref, observable from the ref: "
            << int_ref.use_count() << std::endl;
  std::cout << "\nuse count increase after ref, observable from the object: "
            << referenced_int.use_count() << std::endl;
  std::cout << "\nIsnt unique after ref:" << referenced_int.is_unique()
            << std::endl;

  // Get a weak reference, ref count shouldnt increase.
  weak_ref<int> int_weak = referenced_int.weak();
  std::cout << "Count After weak ref: " << int_ref.use_count() << std::endl;

  // Lock the weak reference, ref count should increase.
  counted_ref<int> int_ref2 = int_weak.lock();
  std::cout << "Count After locking weak ref: " << int_ref.use_count()
            << std::endl;

  // Unlock the weak reference, ref count should decrease.
  int_weak.unlock(std::move(int_ref2));

  std::cout << "Count After unlocking weak ref: " << int_ref.use_count()
            << std::endl;

  // Unref the reference, ref count should decrease.
  referenced_int.unref(std::move(int_ref));
  std::cout << "Count after unrefing: " << referenced_int.use_count()
            << std::endl;

  // Object should have only the last remaining reference.
  if (referenced_int.is_unique()) {
    referenced_int.unref_last();
  }
  // Object should be empty and expired.
  std::cout << "\nIs Expired after unref:" << referenced_int.is_expired();
  std::cout << "\nIs Empty after unref:" << referenced_int.is_empty();
}

RuntimeValue builtin_print(NamedRuntimeValueSet) {
  std::cout << "Hello from C++!" << std::endl;
  return RuntimeValue::NewNone();
}
void test_runtime_value() {
  // Make literals
  RuntimeValue int_val = RuntimeValue::New(1);
  RuntimeValue unsigned_val = RuntimeValue::New(1u);
  RuntimeValue double_val = RuntimeValue::New(1.0);
  RuntimeValue bool_val = RuntimeValue::New(true);
  RuntimeValue none_val = RuntimeValue::NewNone();
  RuntimeValue undefined_val = RuntimeValue::NewUndefined();

  // Make Ref Counted Builtins
  RuntimeValue builtin_string =
      RuntimeValue::New(NativeStringT().ctor("Hello from C&!"));

  RuntimeValue builtin_cpp_method =
      RuntimeValue::New(NativeCppMethodT().ctor(&builtin_print));

  // Create a dynamic object.
  // 1. Each dynamic object must have an associated dynamic namespace, this
  // namespace owns all static elements of the object.
  RuntimeValue builtin_dynamic_namespace =
      RuntimeValue::New(NativeDynamicNamespaceT().ctor("FooClass"));

  RuntimeValue builtin_dynamic_object =
      RuntimeValue::New(NativeDynamicObjectT().ctor());
  // Retrieve the object !!note ref count is not increased here.
  // GetRef-> retrieves from RuntimeValue
  // get()-> retrieves from RefCountedObj
  auto& obj = builtin_dynamic_object.GetRef<NativeDynamicObjectT>().get();
  obj.SetStaticObjectNamespace(
      builtin_dynamic_namespace.GetRef<NativeDynamicNamespaceT>().get());
  obj.AddMember(builtin_string, "foo_str");
  obj.AddMember(builtin_cpp_method, "cpp_method");

  std::cout << "\n Printing member foo_str from object FooClass:"
            << obj.AccessMember("foo_str").GetRef<NativeStringT>().getc()
            << "\n Calling Method cpp_method From Object FooClass:";
  obj.AccessMember("cpp_method").GetRef<NativeCppMethodT>().get().Call();
  std::cout << "\n";
}

int main() {
  test_counted_ref_object();
  test_runtime_value();

  // Test the executor
  Program program;
  ProgramActionBlock code(
      ActionDeclareVariable("myString"),
      ActionAssignVariable("myString", RuntimeValue::New(NativeStringT().ctor(
                                           "Hello from C&!"))),
      ActionDebugPrint("myString"));
  program.Run(code);
}
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started:
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add
//   Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project
//   and select the .sln file
