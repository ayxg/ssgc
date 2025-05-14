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
#include "cldata/translation_input.hpp"
#include "cldata/translation_output.hpp"
#include "trtools/lexer.hpp"
#include "trtools/parser.hpp"
#include "trtools/abstractor.hpp"
// clang-format on

namespace cnd {
// using clerr::ClRes;

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
  TranslationInput tr_input_;
  TranslationOutput tr_output_;
  // TranslationUnit tr_unit_;

  // CompilerProcessResult<cnd::SrcVec> ReadSourcFile() {

  //}

 public:
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
    CND_STDLOG.PrintDiagnostic(cldev_util::DumpTokens(lex_res.value()));
    auto sanitized_src = trtools::Lexer::Sanitize(lex_res.value());
    std::span<const Tk> src_span = std::span{sanitized_src.data(), sanitized_src.size()};
    auto parse_res = cnd::trtools::parser::ParseProgram({src_span.cbegin(), src_span.cend()});
    if (!parse_res) return CND_STDLOG.PrintErrForward(parse_res.error());

    tr_output_.exit_code = EXIT_SUCCESS;
    return EXIT_SUCCESS;
  };
  void SetInput(TranslationInput input) { tr_input_ = input; };
  const TranslationOutput& GetTranslationOutput() const { return tr_output_; };
  CompilerProcessResult<void> Build(Str root_file);

 private:
};

static CompilerProcessResult<TranslationOutput> Compose(TranslationInput& input) {
  Composer cmp{};
  cmp.SetInput(input);
  cmp.Compose();
  return cmp.GetTranslationOutput();
}

};  // namespace trtools
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
