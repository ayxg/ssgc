///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
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
#include "use_ccapi.hpp"
#include "use_corevals.hpp"
#include "use_clmsg.hpp"

#include "cldata/tk.hpp"
#include "trtools/Lexer.hpp"
#include "trtools/Parser.hpp"

#include "TrInput.hpp"
#include "TrOutput.hpp"
#include "TrUnit.hpp"
// clang-format on

namespace cnd::trtools {
struct Evaluator {
  using CtValue = int;
  const Ast& syntax;
  TrUnit& unit;
  TrOutput& out;

 public:
  void Run();
 private:
  int EvalPrimary(const Ast& expr) {
    
  }

  void EvalAddition(const Ast& expr) {
    // Get the lhs op (assume lit int for now_
    CtValue lhs = EvalPrimary(expr.children[0]);
    CtValue rhs = EvalPrimary(expr.children[1]);
    // Choose the instruction to use.
 
  }

  int OpAdd_I32(int l,int r);
};

CompilerProcessResult<int> Evaluate(const Ast& syntax, TrUnit& unit, TrOutput& out) { 
  Evaluator ev{syntax,unit,out};
  ev.Run();
  return 0;
}
}  // namespace cnd

/// @} // end of cnd_compiler

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
