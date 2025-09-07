///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2025 Anton Yashchenko
// Licensed under the Apache License, Version 2.0(the "License");
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup ssgc_trtools
/// @brief
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup cnd_unit_test
/// @{
#pragma once
// !!Keep clang format OFF for this file ,or else expected ast constructors will be unreadable.
// clang-format off
#include "use_ccapi.hpp"
#include "use_corevals.hpp"
#include "corevals/enumdefs/grammar/eIrInstr.hpp"

// clang-format on

namespace cnd::trtools {

using VmType = std::variant<bool,                // 1
                            int,                 // 2
                            long long int,       // 3
                            unsigned,            // 4
                            long long unsigned,  // 5
                            float,               // 6
                            double               // 7
                            >;

// Listed along their webassembly counterpart.
enum eOpCode {
  kPush,
  kPushStatic,

  // Control
  kCreateBlock,   // block : Creates a label that can later be branched out of with a branch.
  kCreateIf,      // if : Executes a statement if the last item on the stack is true (non-zero).
  kCreateElif,    // elif
  kCreateElse,    // else
  kCreateLoop,    // loop : Creates a label that can later be branched to with a br.
  kBranchTo,      // br : Branches to a loop, block, or if.
  kCallFunction,  // Calls a function.
  kDropTop,       // drop - Pops value from the stack and discards it.
  kEscapeBlock,   // end - Can be used to end a block loop, if or else.
  kDoNothing,
  kReturn,  // return - returns from a function.
  kSelect,  // selects one of first two operands based on a boolean condition.
  kUnreachable,

  // Memory
  kMemoryAllocate,  // Increase the size of the memory instance.
  kMemorySize,      // Get the size of the memory instance.
  kMemoryLoad,      // Load a number from memory.
  kMemoryStore,     // Store a number in memory.
  kMemoryCopy,      // Copy data from one region in memory to another
  kMemoryFill,      // Set all values in a region to a specific byte

  // Variables
  kDeclareLocal,
  kGetLocal,
  kSetLocal,
  kTransformLocal,  // Set the value of a local variable and keep the value on the stack.

  kDeclareGlobal,
  kGetGlobal,
  kSetGlobal,

  // Numeric operations
  kI32PushConstant,  // const
  kI64PushConstant,  // const
  kF32PushConstant,  // const
  kF64PushConstant,  // const

  // I32 numeric operations
  kI32CountLeadingZeroes,   // clz
  kI32CountTrailingZeroes,  // ctz
  kI32PopulationCount,      // popcnt
  kI32Add,                  // add
  kI32Subtract,             // sub
  kI32Muliply,              // mul
  kI32DivideSigned,         // div_s
  kI32DivideUnsigned,       // div_u
  kI32RemainderSigned,      // rem_s
  kI32RemainderUnsigned,    // rem_u
  kI32And,                  // and
  kI32Or,                   // or
  kI32Xor,                  // xor
  kI32ShiftLeft,            // shl
  kI32ShiftRightSigned,     // shr_s
  kI32ShiftRightUnsigned,   // shr_s
  kI32RotateLeft,           // rotl
  kI32RotateRight,          // rotr

  // I64 numeric operations
  kI64CountLeadingZeroes,   // clz
  kI64CountTrailingZeroes,  // ctz
  kI64PopulationCount,      // popcnt
  kI64Add,                  // add
  kI64Subtract,             // sub
  kI64Muliply,              // mul
  kI64DivideSigned,         // div_s
  kI64DivideUnsigned,       // div_u
  kI64RemainderSigned,      // rem_s
  kI64RemainderUnsigned,    // rem_u
  kI64And,                  // and
  kI64Or,                   // or
  kI64Xor,                  // xor
  kI64ShiftLeft,            // shl
  kI64ShiftRightSigned,     // shr_s
  kI64ShiftRightUnsigned,   // shr_s
  kI64RotateLeft,           // rotl
  kI64RotateRight,          // rotr

  // F32 numeric operations
  kF32Absolute,    // abs
  kF32Negate,      // neg
  kF32Cieling,     // ciel
  kF32Floor,       // floor
  kF32Truncate,    // trunc
  kF32SquareRoot,  // sqrt
  kF32Add,         // add
  kF32Subtract,    // sub
  kF32Muliply,     // mul
  kF32Divide,      // div
  kF32Minimum,     // mul
  kF32Maximum,     // div
  kF32CopySign,    // copysign

  // F32 numeric operations
  kF64Absolute,    // abs
  kF64Negate,      // neg
  kF64Cieling,     // ciel
  kF64Floor,       // floor
  kF64Truncate,    // trunc
  kF64SquareRoot,  // sqrt
  kF64Add,         // add
  kF64Subtract,    // sub
  kF64Muliply,     // mul
  kF64Divide,      // div
  kF64Minimum,     // mul
  kF64Maximum,     // div
  kF64CopySign,    // copysign

  kInstructionCount
};

enum eRegister {
  kAccumulator,       // RAX
  kBase,              // RBX
  kCounter,           // RCX
  kStackPointer,      // RSP
  kStackBasePointer,  // RBP
  kDestination,       // RDI
  kSource,            // RSI
  kData,              // RDX
};

using VmI32 = int;
using VmI64 = long long int;
using VmIU32 = unsigned int;
using VmIU64 = unsigned long long int;
using VmF32 = float;
using VmF64 = double;
struct VmAddress {
  VmIU32 offset;  ///< Offset in bytes from the base address.
};
struct VmSlot {
  VmIU32 slot;
  VmIU32 store;
};
using VmOpArg = std::variant<VmI32,   // 1
                             VmI64,   // 2
                             VmIU32,  // 3
                             VmIU64,  // 4
                             VmF32,   // 5
                             VmF64,   // 6
                             eRegister,VmAddress,VmSlot>;

namespace eVmOpArg {
enum eVmOpArg {
  kI32,   // 1
  kI64,   // 2
  kIU32,  // 3
  kIU64,  // 4
  kF32,   // 5
  kF64,   // 6
  kRegister,
};
}

struct VmInstruction {
  corevals::grammar::eIrInstr code;
  std::vector<VmOpArg> args;
};

struct VmModule {
  Vec<VmType>::iterator base_stack_ptr;
  Vec<VmInstruction> instructions;
  std::map<std::string, std::size_t> labels;
};

enum class eMachineState {
  kReading,
  kExecuting,
  kInit,
  kOff
};

class VirtualMachine {
  using opcode = corevals::grammar::eIrInstr;
  ////////////////////////////////////////////
  // Kernel
  ////////////////////////////////////////////
  Arr<VmType,64> mainframe;
  Vec<VmType> stack;
  // Machine State Registers 
  eMachineState machine_state_{eMachineState::kOff};  
  
  VmIU64 current_line;
  VmModule& current_module; 
  Vec<VmModule> modules;
  Vec<std::pair<UI32, UI32>> module_indices;  ///< Module name and its index in the modules vector.

  void NewModule(std::string_view id) { 
    modules.push_back(VmModule{});
    module_indices.push_back({std::hash<std::string_view>{}(id), modules.size() - 1});
  }

  void SetCurrentModule(std::string_view id) {
    auto it = std::find_if(module_indices.begin(), module_indices.end(),
                           [&id](const std::pair<UI32, UI32>& pair) { return pair.first == std::hash<std::string_view>{}(id); });
    if (it != module_indices.end()) {
      current_module = modules[it->second];
      current_module.base_stack_ptr = stack.end();
    } else {
      throw std::runtime_error("Module not found: " + std::string(id));
    }
  }

  void ReadInstruction(VmInstruction& instruction) {
    current_module.instructions.push_back(instruction);
    current_line = current_module.instructions.size() - 1;
    auto args_count = instruction.args.size();
    switch (args_count) {
      case 0:
        ApplyZeroArgOp(instruction.code);
        break;
      case 1:
        ApplyOneArgOp(instruction.code,instruction.args[0]);
        break;
      case 2:
        ApplyTwoArgOp(instruction.code,instruction.args[0], instruction.args[1]);
        break;
      case 3:
        ApplyThreeArgOp(instruction.code,instruction.args[0], instruction.args[1], instruction.args[2]);
        break;
      case 4:
        ApplyFourArgOp(instruction.code,instruction.args[0], instruction.args[1], instruction.args[2],
                       instruction.args[3]);
        break;
      default:
        ApplyVaArgOp(instruction.code,instruction.args);
    }
  } 
  void ApplyZeroArgOp(opcode code) {

  }
  void ApplyOneArgOp(opcode code, VmOpArg& arg1) {
    switch (code) {
      case opcode::kLabel:

        break;
      case opcode::kAbsolute:
        Absolute(arg1);
        break;
      case opcode::kCieling:
        Cieling(arg1);
        break;
      case opcode::kFloor:
        Floor(arg1);
        break;
      case opcode::kTruncate:
        Truncate(arg1);
        break;
      case opcode::kSquareRoot:
        SquareRoot(arg1);
        break;
      case opcode::kCopySign:
        CopySign(arg1);
        break;
      case opcode::kDecrement:
        Decrement(arg1);
        break;
    }
  }
  void ApplyTwoArgOp(opcode code, VmOpArg& arg1, VmOpArg& arg2) {}
  void ApplyThreeArgOp(opcode code, VmOpArg& arg1, VmOpArg& arg2, VmOpArg& arg3) {}
  void ApplyFourArgOp(opcode code, VmOpArg& arg1, VmOpArg& arg2, VmOpArg& arg3, VmOpArg& arg4) {}
  void ApplyVaArgOp(opcode code, Vec<VmOpArg> varargs) {}




  Vec<std::pair<UI32, UI32>> label_record;
  Vec<VmInstruction> line_record;
  Vec<Vec<VmInstruction>> fn_record; 
  Vec<Vec<Vec<VmInstruction>>::const_iterator> call_stack;      


  Vec<VmType> op_stack;         
  





  using VmPtr = Vec<VmType>::iterator;



  static constexpr std::size_t kCodes1Offset{};
  static constexpr std::size_t kCodes2Offset{256};
  static constexpr std::size_t kCodes3Offset{512};
  static constexpr std::size_t kCodesVaOffset{1024};
  static constexpr std::size_t GetOpCodeArgCount(eOpCode code) {}

  std::size_t current_line_{0};  /// Current instruction index being executed.
  std::array<VmType, 8> registers_;
  std::array<std::function<void(std::vector<VmOpArg>)>, eOpCode::kInstructionCount> instructions_;

  std::array<std::function<void()>, 256> codes_;
  std::array<std::function<void(VmOpArg)>, 256> codes1_;
  std::array<std::function<void(VmOpArg, VmOpArg)>, 256> codes2_;
  std::array<std::function<void(VmOpArg, VmOpArg, VmOpArg)>, 256> codes3_;
  std::array<std::function<void(std::vector<VmOpArg>)>, 256> codesva_;

  Vec<VmInstruction> lines_;
  Vec<VmType> op_stack_{};
  Vec<VmType> static_data_{};

  void Evaluate(const Vec<VmInstruction>& lines_in) {
    for (auto line_it = lines_in.cbegin(); line_it != lines_in.cend(); line_it++) {
      lines_.push_back(*line_it);  // Store instruction in read lines store.
      current_line_++;             // Set InstructionLine register.
      // Choose op code set based on instruction argument count.
      switch (GetOpCodeArgCount(line_it->code)) {
        case 0:
          codes_[line_it->code]();
        case 1:
          codes1_[line_it->code - kCodes1Offset](line_it->args[0]);
        case 2:
          codes2_[line_it->code - kCodes2Offset](line_it->args[0], line_it->args[1]);
        case 3:
          codes3_[line_it->code - kCodes3Offset](line_it->args[0], line_it->args[1],line_it->args[2]);
        default:
          codesva_[line_it->code - kCodesVaOffset](line_it->args);
      }
    }
  }

  // Instructions
  void I32Constant(VmI32 arg1) { op_stack_.push_back(arg1); }
  void I64Constant(VmI64 arg1) { op_stack_.push_back(arg1); }
  void F32Constant(VmF32 arg1) { op_stack_.push_back(arg1); }
  void F64Constant(VmF64 arg1) { op_stack_.push_back(arg1); }

  // I32 numeric operations
  void I32CountLeadingZeroes(VmOpArg arg1, VmOpArg arg2) {
    ValueAt(arg2) = std::countl_zero(std::bit_cast<VmIU32>(get<VmI32>(arg1)));
  }  // clz
  void I32CountTrailingZeroes(VmOpArg arg1, VmOpArg arg2) {
    ValueAt(arg2) = std::countr_zero(std::bit_cast<VmIU32>(get<VmI32>(arg1)));
  }  // ctz
  void I32PopulationCount(VmOpArg arg1, VmOpArg arg2) {
    ValueAt(arg2) = std::popcount(std::bit_cast<VmIU32>(get<VmI32>(arg1)));
  }  // popcnt
  void I32Add(VmOpArg arg1, VmOpArg arg2, VmOpArg arg3) {
    ValueAt(arg3) = get<VmI32>(ValueAt(arg1)) + get<VmI32>(ValueAt(arg2));
  }  // add
  void I32Subtract(VmOpArg arg1, VmOpArg arg2, VmOpArg arg3) {
    ValueAt(arg3) = get<VmI32>(ValueAt(arg1)) - get<VmI32>(ValueAt(arg2));
  }  // sub
  void I32Muliply(VmOpArg arg1, VmOpArg arg2, VmOpArg arg3) {
    ValueAt(arg3) = get<VmI32>(ValueAt(arg1)) * get<VmI32>(ValueAt(arg2));
  }  // mul
  void I32DivideSigned(VmOpArg arg1, VmOpArg arg2, VmOpArg arg3) {
    ValueAt(arg3) = get<VmI32>(ValueAt(arg1)) / get<VmI32>(ValueAt(arg2));
  }  // div_s
  void I32DivideUnsigned(VmOpArg arg1, VmOpArg arg2, VmOpArg arg3) {
    ValueAt(arg3) = static_cast<VmIU32>(get<VmI32>(ValueAt(arg1))) + static_cast<VmIU32>(get<VmI32>(ValueAt(arg2)));
  }  // div_u
  void I32RemainderSigned(VmOpArg arg1, VmOpArg arg2, VmOpArg arg3) {
    ValueAt(arg3) = get<VmI32>(ValueAt(arg1)) % get<VmI32>(ValueAt(arg2));
  }  // rem_s
  void I32RemainderUnsigned(VmOpArg arg1, VmOpArg arg2, VmOpArg arg3) {
    ValueAt(arg3) = static_cast<VmIU32>(get<VmI32>(ValueAt(arg1))) % static_cast<VmIU32>(get<VmI32>(ValueAt(arg2)));
  }  // rem_u
  void I32And(VmOpArg arg1, VmOpArg arg2, VmOpArg arg3) {
    ValueAt(arg3) = get<VmI32>(ValueAt(arg1)) && get<VmI32>(ValueAt(arg2));
  }  // and
  void I32Or(VmOpArg arg1, VmOpArg arg2, VmOpArg arg3) {
    ValueAt(arg3) = get<VmI32>(ValueAt(arg1)) || get<VmI32>(ValueAt(arg2));
  }  // or
  void I32Xor(VmOpArg arg1, VmOpArg arg2, VmOpArg arg3) {
    ValueAt(arg3) = get<VmI32>(ValueAt(arg1)) | get<VmI32>(ValueAt(arg2));
  }  // xor
  void I32ShiftLeft(VmOpArg arg1, VmOpArg arg2, VmOpArg arg3) {
    ValueAt(arg3) = get<VmI32>(ValueAt(arg1)) << get<VmI32>(ValueAt(arg2));
  }  // shl
  void I32ShiftRightSigned(VmOpArg arg1, VmOpArg arg2, VmOpArg arg3) {
    ValueAt(arg3) = get<VmI32>(ValueAt(arg1)) >> get<VmI32>(ValueAt(arg2));
  }  // shr_s
  void I32ShiftRightUnsigned(VmOpArg arg1, VmOpArg arg2, VmOpArg arg3) {
    ValueAt(arg3) = static_cast<VmI32>(static_cast<VmIU32>(get<VmI32>(ValueAt(arg1))) >>
                                       static_cast<VmIU32>(get<VmI32>(ValueAt(arg2))));
  }  // shr_s
  void I32RotateLeft(VmOpArg arg1, VmOpArg arg2, VmOpArg arg3) {
    ValueAt(arg3) = static_cast<VmI32>(std::rotl(std::bit_cast<VmIU32>(get<VmI32>(arg1)), get<VmI32>(arg2)));
  }  // rotl
  void I32RotateRight(VmOpArg arg1, VmOpArg arg2, VmOpArg arg3) {
    ValueAt(arg3) = static_cast<VmI32>(std::rotr(std::bit_cast<VmIU32>(get<VmI32>(arg1)), get<VmI32>(arg2)));
  }  // rotr

  void GoTo(VmOpArg arg1) {
    current_line_ =
        GetArgAs<VmIU64>(arg1) -
        1;  // Set as one line before because the Evaluate method will advance after executing this instruction.
  }

  /// @brief This instruction decrements the stack pointer and stores the data specified as the argument into the
  /// location pointed to by the stack pointer.
  /// @param memory location of data to copy
  void Push(VmOpArg arg1) {
    op_stack_.push_back(ValueAt(arg1));
    registers_[eRegister::kStackPointer] = op_stack_.size();
  };

  /// @brief This instruction loads the data stored in the location pointed to by the stack pointer into the argument
  /// specified and then increments the stack pointer.
  /// @param memory location to copy data to
  void Pop(VmOpArg arg1) {
    ValueAt(arg1) = op_stack_.back();
    op_stack_.pop_back();
    registers_[eRegister::kStackPointer] = op_stack_.size();
  };

  // Copies the src operand into the dest operand. After the operation both operands contain the same contents.
  void Copy(VmOpArg src, VmOpArg dest) { ValueAt(dest) = ValueAt(src); }

 private:
  /// @brief Returns a reference to the value located at register or stack index.
  constexpr VmType& ValueAt(const VmOpArg& arg1) {
    if (std::holds_alternative<eRegister>(arg1))
      return registers_[GetRegisterIndexAs<std::size_t>(arg1)];
    else
      return op_stack_[GetArgAs<std::size_t>(arg1)];
  }

  /// @brief Get a vm argument value and cast it to T.
  /// @param vm argument, cannot be a register.
  template <class T>
  constexpr T GetArgAs(const VmOpArg& arg1) {
    switch (arg1.index()) {
      case eVmOpArg::kI32:
        return static_cast<T>(std::get<eVmOpArg::kI32>(arg1));
      case eVmOpArg::kI64:
        return static_cast<T>(std::get<eVmOpArg::kI64>(arg1));
      case eVmOpArg::kIU32:
        return static_cast<T>(std::get<eVmOpArg::kIU32>(arg1));
      case eVmOpArg::kIU64:
        return static_cast<T>(std::get<eVmOpArg::kIU64>(arg1));
      case eVmOpArg::kF32:
        return static_cast<T>(std::get<eVmOpArg::kF32>(arg1));
      case eVmOpArg::kF64:
        return static_cast<T>(std::get<eVmOpArg::kF64>(arg1));
      default:
        throw "Can't get argument value from register type.";
    }
  }

  template <class T>
  constexpr T GetRegisterIndexAs(const VmOpArg& arg1) {
    if (std::holds_alternative<eRegister>(arg1))
      return static_cast<T>(std::to_underlying(std::get<eRegister>(arg1)));
    else
      throw "Can't determine register index from value type.";
  }
};

}  // namespace cnd::trtools

/// @} // end of ssgc_trtools

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2025 Anton Yashchenko
//
// Licensed under the Apache License, Version 2.0(the "License"); you may not use this file except in compliance with
// the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on
// an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
