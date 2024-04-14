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
// File: caoco_ir.h
//---------------------------------------------------------------------------//
#ifndef HEADER_GUARD_CALE_CAND_OFFICIAL_COMPILER_CAOCO_IR_H
#define HEADER_GUARD_CALE_CAND_OFFICIAL_COMPILER_CAOCO_IR_H
//---------------------------------------------------------------------------//
// Brief: Runtime Value Class for C&
//---------------------------------------------------------------------------//
// Includes:
#include "cppsextended.h"
// Caoco
//
#include "caoco_rtval.h"
//
#include "caoco_rtalloc.h"
//---------------------------------------------------------------------------//

namespace caoco {

//---------------------------------------------------------------------------//
// Namespace:{iraction}
// Brief:{}
// Detail:{
//
// }
//---------------------------------------------------------------------------//

// Types of arguments which can be passed to actions.
using ProgramActionArgVariant =
    std::variant<NativeIntT, NativeUnsignedT, NativeDoubleT, NativeBoolT,
                 NativeCharT, NativeByteT, NativeNoneT, NativeUndefinedT,
                 NativeStringT>;

namespace iraction {
//---------------------------------------------------------------------------//

struct IrActionBase;  // Contains methods/members common to all actions.
// Compiler Utils
struct DebugPrint;
// Variables
struct DeclareLocalVar;
struct AssignLocalVar;
struct ResolveLocalVar;

struct DeclareGlobalVar;
struct ResolveGlobalVar;
struct AssignGlobalVar;

struct DeclareConstVar;
struct ResolveConstVar;
struct AssignConstVar;

struct DeclareStaticVar;
struct ResolveStaticVar;
struct AssignStaticVar;

struct DeclareInstanceVar;
struct ResolveInstanceVar;
struct AssignInstanceVar;

struct DeclareConstStaticVar;
struct ResolveConstStaticVar;
struct AssignConstStaticVar;

// Memory
struct AllocateMemory;
struct AllocateLiteral;
struct AllocatoTo;
struct AllocateHot;

// Classes
struct DefineClass;
struct AddClassStaticMember;
struct AddClassInstanceMember;
struct ConstructClassInstance;

// Operators
struct ActionBinaryOp;
struct ActionUnaryOp;
struct ActionFuncCall;


#define USING_DECAYED_T(v) using T = std::decay_t<decltype(v)>
#define CXIF if constexpr
enum class eRtErrorCode : size_t {
  kNoError = 0,
  kUndefinedValue,
  kNullPointerOperation,
  kVarAlreadyDeclared,
  kVarNotDeclared,
  kNotImplemented,
};

class IrActionBase {
 public:
  virtual ~IrActionBase() = default;
  virtual void Perform() = 0;

 public:
  IrActionBase(RtStaticAllocator &static_memory,
               RtOperationalAllocator &active_memory,
               RtDynNamespace &anamespace)
      : static_memory_(static_memory),
        active_memory_(active_memory),
        namespace_(anamespace) {}

  IrActionBase(RtStaticAllocator &static_memory,
               RtOperationalAllocator &active_memory)
      : static_memory_(static_memory), active_memory_(active_memory) {}

 protected:
  eRtErrorCode error_code_{eRtErrorCode::kNoError};
  RtStaticAllocator &static_memory_;
  RtOperationalAllocator &active_memory_;
  RtDynNamespace &namespace_{RtDynNamespace::gStaticUndefinedNamespace};
};

// Create an undefined named local variable in a given namespace.
class DeclareLocalVar : public IrActionBase {
  string_view name{""};

  DeclareLocalVar(RtStaticAllocator &static_memory,
                  RtOperationalAllocator &active_memory,
                  const string_view &name)
      : IrActionBase(static_memory, active_memory), name(name) {}

  void Perform() override {
    if (namespace_.ContainsLocal(name)) {
      error_code_ = eRtErrorCode::kVarAlreadyDeclared;
      return;
    }
    // Cant add directly, memory is owned by static mem.
    // Must create a new val, add to memory then add to ns.
    RtStaticAllocator::memory_descriptor new_val = static_memory_.AllocStatic();
    RtVal &val_ref = new_val.Deref();
    val_ref = RtVal::NewUndefined();
    namespace_.AddDecl(name, val_ref);
    error_code_ = eRtErrorCode::kNoError;
  }
};

// Resolve a local value and push a ref to it onto the hot stack.
class ResolveLocalVar : public IrActionBase {
  ResolveLocalVar(RtStaticAllocator &static_memory,
                  RtOperationalAllocator &active_memory,
                  const string_view &name)
      : IrActionBase(static_memory, active_memory), name(name) {}

  void Perform() override {
    // Check if the variable is declared.
    auto var = namespace_.Resolve(name);
    if (var) {
      // Push a reference to the variable onto the hot stack.
      active_memory_.Push(RtDynPtr(var.Extract()).Get());
    } else {
      error_code_ = eRtErrorCode::kVarNotDeclared;
    }
  }

 private:
  const string_view name;
};

// Assing an RtVal from the top of the working stack to a variable.
class AssignLocalVar : public IrActionBase {
  AssignLocalVar(RtStaticAllocator &static_memory,
                 RtOperationalAllocator &active_memory, const string_view &name)
      : IrActionBase(static_memory, active_memory), name(name) {}

  void Perform() override {
    // Check if the variable is declared.
    auto var = namespace_.Resolve(name);
    if (var) {
      // Assign the value to the variable.
      *var.Extract() = active_memory_.PopGet();
    } else {
      error_code_ = eRtErrorCode::kVarNotDeclared;
    }
  }

 private:
  const string_view name;
};

// Unary operation is always in-place
class UnaryNegation : public IrActionBase {
  UnaryNegation(RtStaticAllocator &static_memory,
                RtOperationalAllocator &active_memory)
      : IrActionBase(static_memory, active_memory) {}

  void Perform() override {
    active_memory_.UnaryOperation([this](RtVal &v) { return Visit(v); });
  }

  inline void Visit(RtVal &val) {
    error_code_ = eRtErrorCode::kNoError;
    std::visit(
        [this](auto &v) {
          USING_DECAYED_T(v);
          CXIF(iOneOfNativeLiterals<T>) {
            CXIF(iOneOfNativeArithmetics<T>) {
              CXIF(mta::req::iSameAs<T, NativeBoolT>) {
                // Boolean negation uses the ! operator.
                v = !v;
              }
              else if (mta::req::iSameAs<T, NativeNoneT>) {
                // Do nothing for a None object.
              }
              else {
                // Its some numeric literal, use - operator.
                v = -v;
              }
            }
            else if (mta::req::iSameAs<T, NativePtrT>) {
              // Visit the pointed to val if not a nullptr.
              if (v.IsNull())
                error_code_ = eRtErrorCode::kNullPointerOperation;
              else
                Visit(v.Get());
            }
            else {
              // Undefined or deleted val. Cannot perform operation.
              error_code_ = eRtErrorCode::kUndefinedValue;
            }
          }
          else {
            // Objects should search for the defined member method '_negate_'

            error_code_ = eRtErrorCode::kNotImplemented;
          }
        },
        val.Variant());
  }
};

class BinaryAddition : public IrActionBase {
  BinaryAddition(RtStaticAllocator &static_memory,
                 RtOperationalAllocator &active_memory)
      : IrActionBase(static_memory, active_memory) {}

  void Perform() override {
    active_memory_.BinaryOperation([this](RtVal &&lhs, RtVal &&rhs) {
      return Visit(forward<RtVal>(lhs), forward<RtVal>(rhs));
    });
  }

  template <typename LhsT>
    requires iOneOfNativeArithmetic<LhsT>
  inline RtVal &&VisitRhs(LhsT &&lhsv, RtVal &&rhsv) {
    return std::visit(
        [&lhsv](auto &&v) {
          using RhsT = std::decay_t<decltype(rhsv)>;
          // RHS is a native literal.
          CXIF(iOneOfNativeLiterals<RhsT>) {
            // RHS is an arithmetic literal.
            CXIF(iOneOfNativeArithmetics<RhsT>) {
              CXIF(mta::req::iSameAs<RhsT, NativeNoneT>) {
                // Adding none to anything equals anything.
                return lhsv;
              }
              // Both values are native arithmetics.
              else return lhsv + rhsv;
            }
            // RHS is a pointer.
            else CXIF(mta::req::iSameAs<T, NativePtrT>) {
              if (v.IsNull()) {
                error_code_ = eRtErrorCode::kNullPointerOperation;
                return RtVal::NewUndefined();
              } else
                return VisitRhs(forward<LhsT>(lhsv), v.Get());
            }
            // RHS is undefined
            else {
              error_code_ = eRtErrorCode::kUndefinedValue;
              return RtVal::NewUndefined();
            }
          }
        },
        rhs_val.Variant());
  }

  // Return type is decided based on the type of the lhs
  // operand.
  inline RtVal &&Visit(RtVal &&lhs_val, RtVal &&rhs_val) {
    error_code_ = eRtErrorCode::kNoError;
    return std::visit(
        [this, &rhs_val](auto &&v) {
          using LhsT = std::decay_t<decltype(v)>;
          // LHS is a native literal.
          CXIF(iOneOfNativeLiterals<LhsT>) {
            // LHS is a None object, do nothing.
            CXIF(mta::req::iSameAs<LhsT, NativeNoneT>) { return v; }
            // LHS is a pointer to some RtVal.
            // If non-null pointer, visit the pointed to val.
            else CXIF(mta::req::iSameAs<LhsT, NativePtrT>) {
              if (v.IsNull()) {
                error_code_ = eRtErrorCode::kNullPointerOperation;
                return RtVal::NewUndefined();
              } else
                return Visit(v.Get());
            }
            // LHS is an Undefined val.
            // Cannot perform operation.
            else CXIF(mta::req::iSameAs<LhsT, NativeUndefinedT>) {
              error_code_ = eRtErrorCode::kUndefinedValue;
              return RtVal::NewUndefined();
            }
            //  Arithmetic literal, visit RHS.
            else {
              return VisitRhs<LhsT>(v, rhs_val);
            }
          }
          // LHS is a non-literal.
          else {
            // LHS is a string.

            // LHS is a dynamic object

            // LHS is a dynamic list of RtVals

            error_code_ = eRtErrorCode::kNotImplemented;
            return RtVal::NewUndefined();
          }
        },
        lhs_val.Variant());
  }
};

//---------------------------------------------------------------------------//
}  // namespace iraction
//---------------------------------------------------------------------------//


// Variant of all types of actions. An action is synonymous with an opcode or
// intermediate representation line.
using IrActionVariant = std::variant<iraction::DebugPrint>;

// Collection of program actions, synonymous with a collection of OPCODES in
// Python, or llvm IR actions from C++.
class IrActionBlock {
 public:
  const std::vector<IrActionVariant> &Actions() const { return actions_; }

  IrActionBlock() = default;

  template <typename... T>
  IrActionBlock(T &&...actions)
      : actions_{std::move(ProgramActionVariant(std::forward<T>(actions)))...} {
  }

 private:
  std::vector<IrActionVariant> actions_;
};

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
// File: caoco_ir.h
//---------------------------------------------------------------------------//
#endif HEADER_GUARD_CALE_CAND_OFFICIAL_COMPILER_CAOCO_IR_H
//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
