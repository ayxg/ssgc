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

//#include "cnd_implicit.hpp" // Toolset abstraction layer
//
//// Data
//#include "cnd_constdef.hpp"           // Compiler Constants
//#include "cnd_clerr.hpp"              // Compiler Error Handling
//#include "cnd_translation_output.hpp" // Translation Output
//#include "cnd_translation_input.hpp"  // Translation Input
//#include "cnd_translation_unit.hpp"   // Translation Unit
//
//#include "cnd_src_traits.hpp" // Source Character Traits
//#include "cnd_src_lines.hpp"  // Source Code Data Structure
//#include "cnd_tk.hpp"         // Token Structure
//#include "cnd_cst.hpp"        // Concrete Syntax Tree
//#include "cnd_tk_cursor.hpp" 
//#include "cnd_tk_scope.hpp"
//#include "cnd_tk_closure.hpp"

//
//// Tools
//#include "cnd_cli.hpp" // Command Line Interface
//#include "cnd_scanner.hpp"
#include "lexer.hpp"
//#include "cnd_preprocessor.hpp"
#include "parser.hpp"
//#include "cnd_abstractor.hpp"
//#include "cnd_cail.hpp"
//#include "cnd_ceval.hpp"
//#include "cnd_optimizer.hpp"
//#include "cnd_codegen.hpp"
//#include "cnd_linker.hpp"
//#include "cnd_builder.hpp"
// clang-format on
namespace cnd {
using clerr::ClRes;

namespace cldev_util {
constexpr Str DumpTokens(const Vec<Tk>& tokens) {
  Str ss;
  Size line{0};
  for (const auto& tk : tokens) {
    if (tk.TypeIs(eTk::kNewline)) {
      ss += '\n';
      ss += std::format("[{}|{}|{}|{}|{}|{}|{}]", "\\n", tk.TypeStr(), tk.File(), tk.BegLine(), tk.EndLine(),
                        tk.BegCol(), tk.EndCol());
    } else
      ss += std::format("[{}|{}|{}|{}|{}|{}|{}]", tk.Literal(), tk.TypeStr(), tk.File(), tk.BegLine(), tk.EndLine(),
                        tk.BegCol(), tk.EndCol());
  }
  ss += '\n';
  return ss;
}
}  // namespace cldev_util

namespace compiler {

class Compiler {
  TranslationInput tr_input_;
  TranslationOutput tr_output_;
  TranslationUnit tr_unit_;

 public:
  ClRes<int> Compose() {
    auto& root_file = tr_input_.src_files.front();

    CND_STDLOG.PrintDiagnostic("[" __FUNCTION__ "][Loading first source file characters.] File: ", root_file.filename(),
                               "\n");
    ClRes<cnd::SrcVec> loaded_src = scanner::ReadSourceFile(root_file);
    if (!loaded_src) return CND_STDLOG.PrintErrForward(loaded_src.error());

    CND_STDLOG.PrintDiagnostic(
        "[" __FUNCTION__ "][Splitting first source file characters.] File: ", root_file.filename(), "\n");
    cnd::SrcLines src_lines = scanner::SplitSourceLines(loaded_src.value());

    CND_STDLOG.PrintDiagnostic("[" __FUNCTION__ "][Tokenizing first source file.] File: ", root_file.filename(), "\n");
    auto lex_res = lexer::Lex(src_lines);
    if (!lex_res) return CND_STDLOG.PrintErrForward(lex_res.error());
    CND_STDLOG.PrintDiagnostic("[" __FUNCTION__ "][Dumping lexed tokens.] File: ", root_file.filename(), "\n");
    CND_STDLOG.PrintDiagnostic(cldev_util::DumpTokens(lex_res.value()));

    auto parse_res =
        parser::ParseTranslationUnit(tr_unit_.Fragment(), TkCursor{lex_res.value().begin(), lex_res.value().end()});
    if (!parse_res) return CND_STDLOG.PrintErrForward(parse_res.error());

    tr_output_.exit_code = EXIT_SUCCESS;
    return EXIT_SUCCESS;
  };
  void SetInput(TranslationInput input) { tr_input_ = input; };
  const TranslationOutput& GetTranslationOutput() const { return tr_output_; };
  clerr::ClRes<void> Build(Str root_file);

 private:
};

static ClRes<TranslationOutput> Compose(compiler::TranslationInput& input) {
  Compiler cmp{};
  cmp.SetInput(input);
  cmp.Compose();
  return cmp.GetTranslationOutput();
}

};  // namespace compiler
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
