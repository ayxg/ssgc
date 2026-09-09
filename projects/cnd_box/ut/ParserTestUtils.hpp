///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2025 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup cnd_unit_test
/// @brief Utilities for unit testing the parser.
///
/// These methods should ONLY be used within unit tests.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup cnd_unit_test
/// @{
#pragma once
// #include "compiler_utils/LoadSourceFile.hpp"
#include "compiler/compiler.hpp"
#include "cxxx_enumerated_flags.hpp"
#include "frontend/ast_traits.hpp"
#include "frontend/syntax_transformer.hpp"
#include "frontend/synthesized_ast.hpp"
#include "minitest.hpp"

// Overload ostream >> for eAst enum for minitest library.
std::ostream& operator<<(std::ostream& os, const ssgc::frontend::eAst& obj) {
  os << static_cast<std::underlying_type<ssgc::frontend::eAst>::type>(obj);
  return os;
}

namespace cnd_unit_test::frontend::test_util {

using std::format;
using std::span;
using std::string;
using std::string_view;

using ssgc::frontend::Ast;
using ssgc::frontend::Token;
using namespace ssgc::frontend::parser;
using ParsingMethod = LLPrsResT (*)(TkCursorT);
template <class NodeT>
using TransformMethod = ssgc::frontend::TransformResult<NodeT> (*)(
    const Ast& ast, ssgc::frontend::TransformationContext& ctx);
using Sast = ssgc::frontend::SynthesizedAst;

enum class eTestParsingMethod : int {
  kNone = 0,
  kLoadFromFile = 1 << 0,           // 0001
  kCompareTypeOnly = 1 << 1,        // 0010
  kCompareSignificantOnly = 1 << 2  // 0100
};
using TestParsingMethodFlags = cxx::EnumeratedFlags<eTestParsingMethod>;

static std::string_view getAstLiteral(const ssgc::SourceFile* src, const std::vector<Token>* tokens,
                                      const Ast& node) {
  if (node.sourceBegin() == node.sourceEnd()) {
    return "";
  }

  // Trim insignificant tokens
  const Token* end_token = &tokens->at(node.sourceEnd() - 1);

  while (ssgc::frontend::isTokenInsignificant(end_token->kind)) {
    end_token--;
  }
  return src->slice(tokens->at(node.sourceBegin()).source_range.begin, end_token->source_range.end);
}
static string GenerateSynthesizedAstCode(const ssgc::SourceFile* src,
                                         const std::vector<Token>* tokens, const Ast& node,
                                         std::string::size_type indent = 0) {
  string synth_branches{};
  string this_indent(indent * 2, ' ');

  for (auto it = node.branches.cbegin(); it < node.branches.cend(); it++) {
    if (it == node.branches.cend() - 1)
      synth_branches += GenerateSynthesizedAstCode(src, tokens, *it, indent + 1);
    else
      synth_branches += GenerateSynthesizedAstCode(src, tokens, *it, indent + 1) + ",\n";
  }

  return std::format(
      "{0}Sast{{{1},\"{2}\"{3}", this_indent, eAstToCStr(node.type),
      ssgc::isAstLiteralSignificant(node.type) ? getAstLiteral(src, tokens, node) : "",
      synth_branches.empty() ? format("}}", synth_branches, this_indent)
                             : format(",\n{}\n{}}}", synth_branches, this_indent));
}

static void PrintSynthesizedAstCode(const ssgc::SourceFile* src, const std::vector<Token>* tokens,
                                    const Ast& node, std::string file_path) {
  std::ofstream file;
  file.open(file_path, std::ios::out | std::ios::trunc);
  if (file.is_open()) {
    file << GenerateSynthesizedAstCode(src, tokens, node);
  }
  file.close();
}

// Compare two produced asts using minitest to log any inequality.
// @see `TestParsingMethod`
static bool TestCompareAst(const ssgc::SourceFile* src, const std::vector<Token>* tokens,
                           const Ast& node1, const Ast& node2, TestParsingMethodFlags flags = {}) {
  // Compare node types
  EXPECT_EQ_LOG(node1.type, node2.type,
                format("Ast type {} not equal {}.", eAstToCStr(node1.type), eAstToCStr(node2.type)),
                "Ast Type Comp.");
  if (node1.type != node2.type) return false;

  // Compare node values
  bool is_compare_literals = true;
  if (flags.Check(eTestParsingMethod::kCompareSignificantOnly) &&
      !ssgc::isAstLiteralSignificant(node1.type)) {
    is_compare_literals = false;
  }

  // Overrides kCompareSignificantOnly flag.
  if (flags.Check(eTestParsingMethod::kCompareTypeOnly)) is_compare_literals = false;

  if (is_compare_literals) {
    auto lit1 = getAstLiteral(src, tokens, node1);
    auto lit2 = getAstLiteral(src, tokens, node2);
    EXPECT_EQ_LOG(lit1, lit2, format("Ast literal '{}' not equal '{}'.", lit1, lit2),
                  "Ast Literal Comp.");
    if (lit1 != lit2) return false;
  }

  // Compare number of Children
  EXPECT_TRUE_LOG(node1.branches.size() != node2.branches.size(),
                  std::format("Ast branch quantity not equal. Left: {} Right: {}.",
                              node1.branches.size(), node2.branches.size()),
                  "Ast Size Comp.");
  if (node1.branches.size() != node2.branches.size()) return false;

  // Recursively compare Children
  for (size_t i = 0; i < node1.branches.size(); ++i)
    if (!TestCompareAst(src, tokens, node1.branches.at(i), node2.branches.at(i), flags))
      return false;

  // If all checks pass, the ASTs are equal
  return true;
}

// Compare a produced ast to a synthesized ast using minitest to log any inequality.
// @see `TestParsingMethod`
static bool TestCompareAst(const ssgc::SourceFile* src, const std::vector<Token>* tokens,
                           const Ast& node1, const Sast& node2, TestParsingMethodFlags flags = {}) {
  EXPECT_EQ_LOG(
      node1.type, node2.type,
      format("Ast type {} not equal {}.", eAstToCStr(node1.type), eAstToCStr(node2.type)));
  if (node1.type != node2.type) return false;
  bool is_compare_literals = true;
  if (flags.Check(eTestParsingMethod::kCompareSignificantOnly) &&
      !ssgc::isAstLiteralSignificant(node1.type))
    is_compare_literals = false;
  if (flags.Check(eTestParsingMethod::kCompareTypeOnly)) is_compare_literals = false;
  if (is_compare_literals) {
    auto lit1 = getAstLiteral(src, tokens, node1);
    auto& lit2 = node2.literal;
    EXPECT_EQ_LOG(lit1, lit2, format("Ast literal '{}' not equal '{}'.", lit1, lit2));
    if (lit1 != lit2) return false;
  }
  EXPECT_TRUE_LOG(node1.branches.size() == node2.branches.size(),
                  std::format("Ast branch quantity not equal. Left: {} Right: {}.",
                              node1.branches.size(), node2.branches.size()));
  if (node1.branches.size() != node2.branches.size()) return false;
  for (size_t i = 0; i < node1.branches.size(); ++i)
    if (!TestCompareAst(src, tokens, node1.branches.at(i), node2.branches.at(i), flags))
      return false;
  return true;
}

// Lexes, sanitizes given code, then parses using the given method. If errors occur at any stage
// test fails.
//
//
// eTestParsingMethod::kLoadFromFile :
//    `code` argument must be a path to a file instead of source code. Tests parsing method from
//    a given code source file path. Code may be a snippet, not necessarily top-down valid C&
//    syntax.
static void TestParsingMethod(string_view code, ParsingMethod fn,
                              TestParsingMethodFlags flags = {}) {
  ssgc::Compiler comp{};

  // Load/generate source file data.
  const ssgc::SourceFile* src = nullptr;
  if (flags.Check(eTestParsingMethod::kLoadFromFile)) {
    src = comp.loadSource(code);

    ASSERT_TRUE_LOG(!comp.errorOccured(),
                    comp.errorOccured() ? formatDiagnostic(comp.getLastError(), &comp.context) : "",
                    "Loaded source file.");

  } else {
    src = comp.generateSource("_ut_parser_", code);
  }

  // Tokenize.
  const std::vector<ssgc::frontend::Token>* tokens = comp.tokenize(src->id);
  std::string err_msg_buffer{""};
  if (comp.errorOccured()) {
    auto [err_begin, err_end] = comp.getErrorRange();
    for (const auto& diag : std::ranges::subrange(err_begin, err_end)) {
      err_msg_buffer += formatDiagnostic(diag, &comp.context);
      err_msg_buffer += '\n';
    }
  }
  ASSERT_TRUE_LOG(!comp.errorOccured(), err_msg_buffer, "Loaded source file.");

  auto parse_result = fn({0, tokens->data(), tokens->data() + tokens->size()});
  err_msg_buffer = parse_result ? "" : formatDiagnostic(parse_result.error(), &comp.context);
  ASSERT_TRUE_LOG(parse_result.has_value(), err_msg_buffer, "Parse is valid.");

  static string last_test_suite_name{""};
  static string last_test_case_name{""};
  static int last_test_counter{0};
  std::filesystem::create_directory("_ut_generated_code");
  if (last_test_suite_name == CURRENT_TEST_SUITE_NAME &&
      last_test_case_name == CURRENT_TEST_CASE_NAME) {
    last_test_counter++;
    PrintSynthesizedAstCode(src, tokens, parse_result->ast,
                            format("_ut_generated_code/{}{}{}.txt", CURRENT_TEST_SUITE_NAME,
                                   CURRENT_TEST_CASE_NAME, last_test_counter));
  } else {
    PrintSynthesizedAstCode(
        src, tokens, parse_result->ast,
        format("_ut_generated_code/{}{}.txt", CURRENT_TEST_SUITE_NAME, CURRENT_TEST_CASE_NAME));
    last_test_suite_name = CURRENT_TEST_SUITE_NAME;
    last_test_case_name = CURRENT_TEST_CASE_NAME;
  }
}

// Lexes, sanitizes given code, then parses using the given method and compares to expected output.
// Fail if compiler errors occur at any stage.
// Fail if ast is not equal to expected ast in both type and literal value.
//
//
// eTestParsingMethod::kCompareTypeOnly :
//    Only compare the Ast types. Not their literal value.
//
// eTestParsingMethod::kCompareSignificantOnly :
//    Only compare literal Ast value if its significant. @see `cnd::Tk::IsLiteralSignificant`
//
// Note the expected ast should be passed as a synthesized ast, unlike regular ast it stores the
// literal value internally - avoiding the lexing stage.
static void TestParsingMethod(string_view code, ParsingMethod fn, const Sast& expected,
                              TestParsingMethodFlags flags = {}) {
  ssgc::Compiler comp{};

  // Load/generate source file data.
  const ssgc::SourceFile* src = nullptr;
  if (flags.Check(eTestParsingMethod::kLoadFromFile)) {
    src = comp.loadSource(code);

    ASSERT_TRUE_LOG(!comp.errorOccured(),
                    comp.errorOccured() ? formatDiagnostic(comp.getLastError(), &comp.context) : "",
                    "Loaded source file.");

  } else {
    src = comp.generateSource("_ut_parser_", code);
  }

  // Tokenize.
  const std::vector<ssgc::frontend::Token>* tokens = comp.tokenize(src->id);
  std::string err_msg_buffer{""};
  if (comp.errorOccured()) {
    auto [err_begin, err_end] = comp.getErrorRange();
    for (const auto& diag : std::ranges::subrange(err_begin, err_end)) {
      err_msg_buffer += formatDiagnostic(diag, &comp.context);
      err_msg_buffer += '\n';
    }
  }
  ASSERT_TRUE_LOG(!comp.errorOccured(), err_msg_buffer, "Loaded source file.");

  auto parse_result = fn({0, tokens->data(), tokens->data() + tokens->size()});
  err_msg_buffer = parse_result ? "" : formatDiagnostic(parse_result.error(), &comp.context);
  ASSERT_TRUE_LOG(parse_result.has_value(), err_msg_buffer, "Parse is valid.");

  ASSERT_TRUE_LOG(
      TestCompareAst(src, tokens, parse_result->ast, expected, flags),
      std::format("Expected syntax tree is not equal:\n[Expected]:\n{}\n[Parsed]:\n{}\n",
                  expected.Format(), parse_result->ast.Format(src, tokens)),
      "Expected syntax tree is equal.");
}

template <class NodeT>
static void TestTransformMethod(string_view code, ParsingMethod fn,
                                TransformMethod<NodeT> transform_fn,
                                TestParsingMethodFlags flags = {}) {
  ssgc::Compiler comp{};

  // Load/generate source file data.
  const ssgc::SourceFile* src = nullptr;
  if (flags.Check(eTestParsingMethod::kLoadFromFile)) {
    src = comp.loadSource(code);

    ASSERT_TRUE_LOG(!comp.errorOccured(),
                    comp.errorOccured() ? formatDiagnostic(comp.getLastError(), &comp.context) : "",
                    "Loaded source file.");

  } else {
    src = comp.generateSource("_ut_parser_", code);
  }

  // Tokenize.
  const std::vector<ssgc::frontend::Token>* tokens = comp.tokenize(src->id);
  std::string err_msg_buffer{""};
  if (comp.errorOccured()) {
    auto [err_begin, err_end] = comp.getErrorRange();
    for (const auto& diag : std::ranges::subrange(err_begin, err_end)) {
      err_msg_buffer += formatDiagnostic(diag, &comp.context);
      err_msg_buffer += '\n';
    }
  }
  ASSERT_TRUE_LOG(!comp.errorOccured(), err_msg_buffer, "Loaded source file.");

  // Parse
  auto parse_result = fn({0, tokens->data(), tokens->data() + tokens->size()});
  err_msg_buffer = parse_result ? "" : formatDiagnostic(parse_result.error(), &comp.context);
  ASSERT_TRUE_LOG(parse_result.has_value(), err_msg_buffer, "Parse is valid.");

  ssgc::frontend::TransformationContext transform_context{.id_table = comp.context.id_table,
                                                          .string_table = comp.context.string_table,
                                                          .source_file = *src,
                                                          .tokens = *tokens};
  ssgc::frontend::TransformResult<NodeT> transform_result = transform_fn(parse_result->ast, transform_context);
  ASSERT_TRUE_LOG(transform_result.diagnostics->empty(),
                  formatDiagnostic(transform_result.diagnostics, &comp.context),
                  "Transform is valid.");


}
}  // namespace cnd_unit_test::frontend::test_util

/// @} // end of cnd_unit_test

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2025 Anton Yashchenko
//
// This program is free software : you can redistribute it and / or modify it
// under the terms of the GNU Affero General Public License as published by the
// Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////