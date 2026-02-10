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
#include "ccapi/CommonCppApi.hpp"

#include "compiler_utils/CompilerProcessResult.hpp"

#include "compiler/TranslationInput.hpp"
#include "compiler/TranslationOutput.hpp"
#include "hir/Compeval.hpp"
// clang-format on

namespace cnd {
namespace trtools {

class Compiler {
 public:
  Compiler(const TrInput& input_) : input_(input_) {}
  ClRes<TrOutput> Translate() noexcept;

 private:
  const TrInput& input_;
  TrOutput output_;
  hir::TrUnit unit_{input_,output_};

};

ClRes<TrOutput> Compiler::Translate() noexcept {
  auto eval_res = unit_.Evaluate();
  if (!eval_res) return ClFail(eval_res.error());
  return output_;
}

}  // namespace trtools
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
