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
// File: caoco_program.h
//---------------------------------------------------------------------------//
#ifndef HEADER_GUARD_CALE_CAND_OFFICIAL_COMPILER_CAOCO_PROGRAM_H
#define HEADER_GUARD_CALE_CAND_OFFICIAL_COMPILER_CAOCO_PROGRAM_H
//---------------------------------------------------------------------------//
// Brief: An instance of a C& program.
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

// Executor will be passed an RtProgram reference
// and apply the given IrActionBlocks to the RtProgram.
class Executor;

// Represents a C& program instance.
// 1. Owns the program's memory.
// 2. Owns an instance of an Executor which refers to this program.
class RtProgram {
  friend class Executor;  // Executor needs access to the program's internals.
 private:
  RtStaticAllocator static_allocator_;
  RtOperationalAllocator operational_allocator_;
  RtDynNamespace global_namespace_{kGlobalNamespaceName};
  RtVal program_return_value_{RtVal::NewUndefined()};
  eNativeTypeIndex::enum_type program_return_type_{
      eNativeTypeIndex::kUndefined};
  Executor* executor_; // Storing pointer allows executor to be destroyed 
                       // before the program.
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
// File: caoco_program.h
//---------------------------------------------------------------------------//
#endif HEADER_GUARD_CALE_CAND_OFFICIAL_COMPILER_CAOCO_PROGRAM_H
//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
