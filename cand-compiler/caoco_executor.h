//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
// Copyright 2024 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
//---------------------------------------------------------------------------//
// Author(s): Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.acpp.dev
//---------------------------------------------------------------------------//
// Project: C& Programming Language Environment
// Directory: cand-official-compiler
// File: caoco_executor.h
//---------------------------------------------------------------------------//
#ifndef HEADER_GUARD_CALE_CAND_OFFICIAL_COMPILER_CAOCO_EXECUTOR_H
#define HEADER_GUARD_CALE_CAND_OFFICIAL_COMPILER_CAOCO_EXECUTOR_H
//---------------------------------------------------------------------------//
// Brief: Runs IR lines on a program instance.
//---------------------------------------------------------------------------//
// Includes:
#include "cppsextended.h"
// Caoco
//
#include "caoco_rtval.h"
//
#include "caoco_rtalloc.h"
//
#include "caoco_program.h"

//---------------------------------------------------------------------------//
namespace caoco {

// A list of IrActions to be applied by the Executor to an RtProgram.
class ProgramActionBlock;

// Handles execution of action blocks.
class Executor {
  RtProgram& executable_;  // The program instance to execute on.

 public:
  Executor(RtProgram& prog) : executable_(prog) {}

  // For executing method blocks from within actions.
  // Should the passed namespace not be within the program, behaviour is
  // undefined. Implemented in caoco_runtime.h as the implementation must be
  // aware of the definition of all IrActions.
  RtVal Execute(const ProgramActionBlock& action_block,
                RtDynNamespace& target_ns);

  // For executing the program.
  // uses _global_ namespace of the program when none is passed.
  // Implemented in caoco_runtime.h as the implementation
  // must be aware of the definition of all IrActions.
  RtVal Execute(const ProgramActionBlock& action_block);
};
}  // namespace caoco

//---------------------------------------------------------------------------//
// Copyright 2024 Anton Yashchenko
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
//---------------------------------------------------------------------------//
// Author(s): Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.acpp.dev
//---------------------------------------------------------------------------//
// Project: C& Programming Language Environment
// Directory: cand-official-compiler
// File: caoco_executor.h
//---------------------------------------------------------------------------//
#endif HEADER_GUARD_CALE_CAND_OFFICIAL_COMPILER_CAOCO_EXECUTOR_H
//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
