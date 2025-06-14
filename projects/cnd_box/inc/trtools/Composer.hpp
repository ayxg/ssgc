///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the Apache License, Version 2.0(the "License");
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
//#include "trtools/Compeval.hpp"

#include "TrInput.hpp"
#include "TrOutput.hpp"
#include "TrUnit.hpp"

//#include "VirtualMachine.hpp"
//#include "IrGenerator.hpp"
// clang-format on

namespace cnd {
namespace trtools {

template <class SourceCharT, class FilePathT>
CompilerProcessResult<Vec<SourceCharT>> LoadSourceFile(FilePathT fp) {
  using cldev::clmsg::MakeClMsg;

  if (!stdfs::exists(fp)) return CompilerProcessFailure(MakeClMsg<eClErr::kFailedToReadFile>(fp, "Does not exist"));

  if (!stdfs::is_regular_file(fp))
    return CompilerProcessFailure(MakeClMsg<eClErr::kFailedToReadFile>(fp, "Not a regular file."));

  std::ifstream source_file_stream(fp);
  if (!source_file_stream.is_open())
    return CompilerProcessFailure(MakeClMsg<eClErr::kFailedToReadFile>(fp, "Could not open file."));

  Vec<SourceCharT> temp_file_buffer = Vec<SourceCharT>((std::istreambuf_iterator<SourceCharT>(source_file_stream)),
                                                       std::istreambuf_iterator<SourceCharT>());
  source_file_stream.close();

  // Add \0 if not already at end.
  if (temp_file_buffer.back() != '\0') temp_file_buffer.push_back('\0');

  return temp_file_buffer;
}

class Composer {
  TrInput tr_input_;
  TrOutput tr_output_;
  TrUnit tr_unit_;

  void GenerateBaseCodeModel();
  void ProcessSourceCode(Ast ast);
  void ProcessSourceFile(StrView fp);

 public:
  //CompilerProcessResult<int> Compose() {
  //  GenerateBaseCodeModel();

  //  // Set the initial exit code and return value to 0(EXIT_SUCCESS) before processing.
  //  tr_output_.exit_code = EXIT_SUCCESS;
  //  tr_output_.return_value = EXIT_SUCCESS;

  //  // Process all input source files in order. 
  //  for (auto src_file_it = tr_input_.src_files.cbegin(); src_file_it != tr_input_.src_files.cend(); src_file_it++) {
  //    // If a file was processed through an 'include' directive inside a previous source file, skip processing.
  //    if (tr_unit_.processed_sources.contains(*src_file_it)) continue;
  //    auto parse_res = ParseFile(*src_file_it);
  //    if (!parse_res) return parse_res.error(); // Parsing failure.

  //    //auto compeval_res = Evaluate(*parse_res,tr_unit_, tr_output_);
  //    //if (!compeval_res) return compeval_res.error();  // Compeval failure.
  //  }
  //
  //  return tr_output_.exit_code;
  //};

  CompilerProcessResult<int> Compose() {
    auto& root_file = tr_input_.src_files.front();

    CND_STDLOG.PrintDiagnostic("[" __FUNCTION__ "][Loading first source file characters.] File: ", root_file.filename(),
                               "\n");
    auto loaded_src = LoadSourceFile<char>(root_file.string());
    if (!loaded_src) return CND_STDLOG.PrintErrForward(loaded_src.error());
    StrView src_view = {loaded_src.value().begin(), loaded_src.value().end()};

    CND_STDLOG.PrintDiagnostic("[" __FUNCTION__ "][Tokenizing first source file.] File: ", root_file.filename(), "\n");

    auto lex_res = trtools::Lexer::Lex(src_view);
    if (!lex_res) return CND_STDLOG.PrintErrForward(lex_res.error());

    CND_STDLOG.PrintDiagnostic("[" __FUNCTION__ "][Dumping lexed tokens.] File: ", root_file.filename(), "\n");
    auto sanitized_src = trtools::Lexer::Sanitize(lex_res.value());
    std::span<const Tk> src_span = std::span{sanitized_src.data(), sanitized_src.size()};
    auto parse_res = cnd::trtools::parser::ParseSyntax({src_span.cbegin(), src_span.cend()});
    if (!parse_res) return CND_STDLOG.PrintErrForward(parse_res.error());

    tr_output_.exit_code = EXIT_SUCCESS;
    return EXIT_SUCCESS;
  };
  void SetInput(TrInput input) { tr_input_ = input; };
  const TrOutput& GetTranslationOutput() const { return tr_output_; };
  CompilerProcessResult<void> Build(Str root_file);

 private:
};

static CompilerProcessResult<TrOutput> Compose(TrInput& input) {
  Composer cmp{};
  cmp.SetInput(input);
  cmp.Compose();
  return cmp.GetTranslationOutput();
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
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
