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
#ifndef HEADER_GUARD_CALE_CAND_OFFICIAL_COMPILER_CAOCO_RTALLOC_H
#define HEADER_GUARD_CALE_CAND_OFFICIAL_COMPILER_CAOCO_RTALLOC_H
//---------------------------------------------------------------------------//
// Brief: Memory stack for a C& program instance.
//---------------------------------------------------------------------------//
// Includes:
#include "cppsextended.h"
// Caoco
#include "caoco_rtval.h"
//---------------------------------------------------------------------------//

namespace caoco {

//---------------------------------------------------------//
// Class:{RtOperationalAllocator}
// Brief:{Memory stack for a C& program instance.}
// Detail: {
//  Represents the operational stack of a C& program instance.
//  The memory is encapsulated in a vector of RtVal objects.
//  Used to store the intermediate results of a program execution.
//
//  Currently Implemented Operations:
//    - Top
//    - Top with an offset
//    - Push
//    - Pop
//    - PopN
//    - PopGet
//    - PushCopy
//    - Swap
//    - UnaryOperation
//    - BinaryOperation
//    - InPlaceBinaryOperation
//
//  Good reference for the possible action this class must be able to perform:
//    - https://docs.python.org/3/library/dis.html
// }
//---------------------------------------------------------//
class RtOperationalAllocator {
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
  void Push(RtVal&& rtv) { memory_.push_back(std::forward<RtVal>(rtv)); }

  // Push an element onto the stack.
  void Push(const RtVal& rtv) {
    memory_.push_back(std::forward<const RtVal>(rtv));
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
  RtVal&& PopGet() {
    RtVal val{std::move(memory_.back())};
    memory_.pop_back();
    return std::move(val);
  }

  // Push the i-th item  of the stack to the top of the stack
  // without removing it from its original location
  // assert i > 0
  // STACK.append(STACK[-i])
  void PushCopy(size_t n) {
    RtVal copy_rtv{Top(n)};
    Push(std::move(copy_rtv));
  }

  // Swap the top of the stack with the i-th element:
  // STACK[-i], STACK[-1] = STACK[-1], STACK[-i]
  void Swap(size_t n) {
    RtVal& val_to_move_to = Top(n);
    RtVal val_to_move = Top();
    Top() = val_to_move_to;
    val_to_move_to = val_to_move;
  }

  // Unary operations take the top of the stack, apply the operation, and push
  // the result back on the stack.
  // Implements STACK[-1] = <UNARY_OPERATOR>STACK[-1].
  void UnaryOperation(std::function<void(RtVal&)> op) { op(Top()); }

  // Binary and in-place operations
  // Binary operations remove the top two items from the stack(STACK[-1] and
  //                                                          STACK[-2])
  // They perform the operation, then put the result back on the stack.
  void BinaryOperation(std::function<RtVal && (RtVal&&, RtVal&&)> op) {
    RtVal&& rhs = PopGet();
    RtVal&& lhs = PopGet();
    Push(op(std::forward<RtVal>(lhs), std::forward<RtVal>(rhs)));
  }

  // In place operations are like binary operations, but the operation is done
  // in-place when STACK[-2] supports it, and the resulting STACK[-1] may be
  // (but does not have to be) the original STACK[-2].
  //   rhs = STACK.pop()
  //   lhs = STACK.pop()
  //   STACK.append(lhs op rhs)
  void InPlaceBinaryOperation(std::function<void(RtVal&, RtVal&&)> op) {
    RtVal&& rhs = PopGet();
    RtVal& lhs = Top();
    op(lhs, std::forward<RtVal>(rhs));
  }

 private:
  std::vector<RtVal> memory_;
};
//---------------------------------------------------------//
// EndClass:{RtOperationalAllocator}
//---------------------------------------------------------//

//---------------------------------------------------------//
// Class:{RtOperationalAllocator}
// Brief:{Memory stack for a C& program instance.}
// Detail: {
//  Represents the static storage of a C& program instance.
//  Allocates memory for:
//  - Named declarations ( with a static storage specifier)
//    in method definitions.
//  - Global declarations, anything declared in the gloabl scope.
//  - Constants, anything declared with the const specifier.
//  - Variables of namespaces, or static variables of classes.
//  - Class instance variables.
// }
//---------------------------------------------------------//
class RtStaticAllocator {
 public:
  using memory_address = std::list<RtVal>::iterator;
  using rtval_ref = std::reference_wrapper<RtVal>;
  struct memory_descriptor {
    memory_address address;
    size_t size;

    RtVal& Deref() const { return *address; }
  };

 public:
  // Allocate memory for a single new RtVal object
  // which will have static lifetime duration.
  // Memory will be alloc as long as program is running.
  memory_descriptor AllocStatic() {
    memory_.emplace_back();
    return {.address = std::prev(memory_.end()), .size = 1};
  }

  // Alloc Constant
  memory_descriptor AllocConstant(const RtVal& rtv) {
    constants_.emplace(constants_.size(), new RtVal(rtv));
    return {.address = std::prev(memory_.end()), .size = 1};
  }

  // Once allocated memory is only release all-at-once at end of program.
  void ReleaseMemory() {
    // Clear all maps
    static_storage_.clear();
    constants_.clear();

    // Clear all memory
    memory_.clear();
  }

 private:
  std::list<RtVal> memory_;

  // Memory for variables of namespaces, or static variables of classes.
  std::unordered_map<memory_address, RtVal*> static_storage_;

  // Memory for constants
  std::unordered_map<memory_address, RtVal*> constants_;
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
#endif HEADER_GUARD_CALE_CAND_OFFICIAL_COMPILER_CAOCO_RTALLOC_H
//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
