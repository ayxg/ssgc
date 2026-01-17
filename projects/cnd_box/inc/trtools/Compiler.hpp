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

namespace cnd {
namespace trtools {

class Compiler {
 public:
  Compiler(const TrInput& tr_input) : tr_input(tr_input){}
  ClRes<TrOutput> Translate(const TrInput& trin) noexcept;

 private:
  // Loads, lexes, sanitizes and parses a C& source file. Stores result of operations into associated maps at file path
  // key. Assert a file has not been already loaded for this compiler instance before calling this method on a given
  // path.
  ClRes<void> LoadSourceFile(const std::string& fp) noexcept;

  // Loads source file at given path and stores in 'sources' at file path key. Currently only used  internally by
  // 'LoadSourceFile' method.
  ClRes<void> ReadSourceFile(const std::string& fp) noexcept;

 private:
  const TrInput& tr_input;
  TrOutput tr_output;

  std::unordered_map<std::string, std::vector<char>> sources{};
  std::unordered_map<std::string_view, std::vector<Tk>> tokens{};
  std::unordered_map<std::string_view, std::span<const Tk>> sanitized_tokens{};
  std::unordered_map<std::string_view, Ast> trees{};
};

ClRes<TrOutput> Compiler::Translate(const TrInput& trin) noexcept {
  //
  return TrOutput{};
}

ClRes<void> Compiler::LoadSourceFile(const std::string& fp) noexcept {
  // Load file data.
  auto src_read = ReadSourceFile(fp);
  if (!src_read) return src_read;

  // Lex and store tokens.
  StrView src_view = {sources[fp].cbegin(), sources[fp].cend()};
  auto lex_res = Lexer::Lex(src_view);
  if (!lex_res) return ClFail(lex_res.error());
  tokens[fp] = lex_res.value();

  // Sanitize and store sanitized tokens.
  auto sanitized_src = Lexer::Sanitize(tokens[fp]);
  sanitized_tokens[fp] = std::span{sanitized_src.data(), sanitized_src.size()};

  // Parse and store abstract syntax tree.
  auto parse_res = parser::ParseSyntax({sanitized_tokens[fp].cbegin(), sanitized_tokens[fp].cend()});
  if (!parse_res) return ClFail(parse_res.error());
  trees[fp] = parse_res.Extract().ast;

  return ClRes<void>{};
}

ClRes<void> Compiler::ReadSourceFile(const std::string& fp) noexcept {
  if (!stdfs::exists(fp)) return ClFail(MakeClMsg<eClErr::kFailedToReadFile>(fp, "Does not exist"));

  if (!stdfs::is_regular_file(fp)) return ClFail(MakeClMsg<eClErr::kFailedToReadFile>(fp, "Not a regular file."));

  std::ifstream source_file_stream(fp);
  if (!source_file_stream.is_open()) return ClFail(MakeClMsg<eClErr::kFailedToReadFile>(fp, "Could not open file."));

  std::vector<char> temp_file_buffer =
      std::vector<char>((std::istreambuf_iterator<char>(source_file_stream)), std::istreambuf_iterator<char>());
  source_file_stream.close();

  // Add \0 if not already at end.
  if (temp_file_buffer.back() != '\0') temp_file_buffer.push_back('\0');

  sources[fp] = temp_file_buffer;

  return ClRes<void>{};
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
