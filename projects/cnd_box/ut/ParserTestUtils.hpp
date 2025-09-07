///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2025 Anton Yashchenko
// Licensed under the Apache License, Version 2.0(the "License");
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
#include "cxxx_enumerated_flags.hpp"
#include "minitest.hpp"
#include "trtools/CompilerIO.hpp"
#include "trtools/Parser.hpp"

// Overload ostream >> for eAst enum for minitest library.
std::ostream& operator<<(std::ostream& os, const cnd::corevals::grammar::eAst& obj) {
  os << static_cast<std::underlying_type<cnd::corevals::grammar::eAst>::type>(obj);
  return os;
}

namespace cnd_unit_test::frontend::test_util {

using std::format;
using std::span;
using std::string;
using std::string_view;

using cnd::Ast;
using cnd::eAstToCStr;
using cnd::Sast;
using cnd::Tk;
using cnd::trtools::Lexer;
using cnd::trtools::LoadSourceFile;
using cnd::trtools::parser::LLPrsResT;
using cnd::trtools::parser::TkCursorT;

using ParsingMethod = LLPrsResT (*)(TkCursorT);

enum class eTestParsingMethod : int {
  kNone,
  kLoadFromFile,
  kCompareTypeOnly,
  kCompareSignificantOnly,
};
using TestParsingMethodFlags = cxx::EnumeratedFlags<eTestParsingMethod>;

static string GenerateSynthesizedAstCode(const Ast& node, std::string::size_type indent = 0) {
  string synth_branches{};
  string this_indent(indent * 2, ' ');

  for (auto it = node.children.cbegin(); it < node.children.cend(); it++) {
    if (it == node.children.cend() - 1)
      synth_branches += GenerateSynthesizedAstCode(*it, indent + 1);
    else
      synth_branches += GenerateSynthesizedAstCode(*it, indent + 1) + ",\n";
  }

  return format("{0}Sast{{{1},\"{2}\"{3}", this_indent, eAstToCStr(node.type),
                node.IsLiteralSignificant() ? node.GetLiteral() : "",
                synth_branches.empty() ? format("}}", synth_branches, this_indent)
                                       : format(",\n{}\n{}}}", synth_branches, this_indent));
}

static void PrintSynthesizedAstCode(const Ast& node, std::string file_path) {
  std::ofstream file;
  file.open(file_path, std::ios::out | std::ios::trunc);
  if (file.is_open()) {
    file << GenerateSynthesizedAstCode(node);
  }
  file.close();
}

// Compare two produced asts using minitest to log any inequality.
// @see `TestParsingMethod`
static bool TestCompareAst(const Ast& node1, const Ast& node2, TestParsingMethodFlags flags = {}) {
  // Compare node types
  EXPECT_EQ_LOG(node1.type, node2.type,
                format("Ast type {} not equal {}.", eAstToCStr(node1.type), eAstToCStr(node2.type)), "Ast Type Comp.");
  if (node1.type != node2.type) return false;

  // Compare node values
  bool is_compare_literals = true;
  if (flags.Check(eTestParsingMethod::kCompareSignificantOnly) && node1.IsLiteralSignificant())
    is_compare_literals = false;
  // Overrides kCompareSignificantOnly flag.
  if (flags.Check(eTestParsingMethod::kCompareTypeOnly)) is_compare_literals = false;

  if (is_compare_literals) {
    auto lit1 = node1.GetLiteral();
    auto lit2 = node2.GetLiteral();
    EXPECT_EQ_LOG(lit1, lit2, format("Ast literal '{}' not equal '{}'.", lit1, lit2), "Ast Literal Comp.");
    if (lit1 != lit2) return false;
  }

  // Compare number of Children
  EXPECT_TRUE_LOG(
      node1.children.size() != node2.children.size(),
      format("Ast branch quantity not equal. Left: {} Right: {}.", node1.children.size(), node2.children.size()),
      "Ast Size Comp.");
  if (node1.children.size() != node2.children.size()) return false;

  // Recursively compare Children
  for (size_t i = 0; i < node1.children.size(); ++i)
    if (!TestCompareAst(node1.children.at(i), node2.children.at(i), flags)) return false;

  // If all checks pass, the ASTs are equal
  return true;
}

// Compare a produced ast to a synthesized ast using minitest to log any inequality.
// @see `TestParsingMethod`
static bool TestCompareAst(const Ast& node1, const Sast& node2, TestParsingMethodFlags flags = {}) {
  EXPECT_EQ_LOG(node1.type, node2.type,
                format("Ast type {} not equal {}.", eAstToCStr(node1.type), eAstToCStr(node2.type)), "Ast Type Comp.");
  if (node1.type != node2.type) return false;
  bool is_compare_literals = true;
  if (flags.Check(eTestParsingMethod::kCompareSignificantOnly) && node1.IsLiteralSignificant())
    is_compare_literals = false;
  if (flags.Check(eTestParsingMethod::kCompareTypeOnly)) is_compare_literals = false;
  if (is_compare_literals) {
    auto lit1 = node1.GetLiteral();
    auto& lit2 = node2.literal;
    EXPECT_EQ_LOG(lit1, lit2, format("Ast literal '{}' not equal '{}'.", lit1, lit2), "Ast Literal Comp.");
    if (lit1 != lit2) return false;
  }
  EXPECT_TRUE_LOG(
      node1.children.size() == node2.children.size(),
      std::format("Ast branch quantity not equal. Left: {} Right: {}.", node1.children.size(), node2.children.size()),
      "Ast Size Comp.");
  if (node1.children.size() != node2.children.size()) return false;
  for (size_t i = 0; i < node1.children.size(); ++i)
    if (!TestCompareAst(node1.children.at(i), node2.children.at(i), flags)) return false;
  return true;
}

// Lexes, sanitizes given code, then parses using the given method. If errors occur at any stage
// test fails.
//
//
// eTestParsingMethod::kLoadFromFile :
//    `code` argument must be a path to a file instead of source code. Tests parsing method from
//    a given code source file path. Code may be a snippet, not necessarily top-down valid C& syntax.
static void TestParsingMethod(string_view code, ParsingMethod fn, TestParsingMethodFlags flags = {}) {
  string err_msg_buffer{};
  string loaded_source{};

  if (flags.Check(eTestParsingMethod::kLoadFromFile)) {
    auto load_res = LoadSourceFile<char>(code);
    err_msg_buffer = load_res ? "" : load_res.error().Format();
    ASSERT_TRUE_LOG(load_res.has_value(), "Failed load source file. Error:" + err_msg_buffer, "Loaded source file.");
    loaded_source = load_res->data();
  }

  auto expected_source = Lexer::Lex(flags.Check(eTestParsingMethod::kLoadFromFile) ? loaded_source : code);
  err_msg_buffer = expected_source ? "" : expected_source.error().Format();
  ASSERT_TRUE_LOG(expected_source.has_value(), "Failed to lex code. Error:" + err_msg_buffer, "Lex is valid.");
  auto source = Lexer::Sanitize(*expected_source);
  span<const Tk> src_view = source;
  auto parse_result = fn(TkCursorT{src_view.cbegin(), src_view.cend()});
  err_msg_buffer = parse_result ? "" : parse_result.error().Format();
  ASSERT_TRUE_LOG(parse_result.has_value(), "Failed to parse code. Error:" + err_msg_buffer, "Parse is valid.");

  static string last_test_suite_name{""};
  static string last_test_case_name{""};
  static int last_test_counter{0};
  std::filesystem::create_directory("_ut_generated_code");
  if (last_test_suite_name == CURRENT_TEST_SUITE_NAME && last_test_case_name == CURRENT_TEST_CASE_NAME) {
    last_test_counter++;
    PrintSynthesizedAstCode(parse_result->ast, format("_ut_generated_code/{}{}{}.txt", CURRENT_TEST_SUITE_NAME,
                                                      CURRENT_TEST_CASE_NAME, last_test_counter));
  } else {
    PrintSynthesizedAstCode(parse_result->ast,
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
static void TestParsingMethod(string_view code, ParsingMethod fn, const cnd::Sast& expected,
                              TestParsingMethodFlags flags = {}) {
  string err_msg_buffer{};
  string loaded_source{""};

  if (flags.Check(eTestParsingMethod::kLoadFromFile)) {
    auto load_res = LoadSourceFile<char>(code);
    err_msg_buffer = load_res ? "" : load_res.error().Format();
    ASSERT_TRUE_LOG(load_res.has_value(), "Failed load source file. Error:" + err_msg_buffer, "Loaded source file.");
    loaded_source = load_res->data();
  }

  auto expected_source = Lexer::Lex(flags.Check(eTestParsingMethod::kLoadFromFile) ? loaded_source : code);
  err_msg_buffer = expected_source ? "" : expected_source.error().Format();
  ASSERT_TRUE_LOG(expected_source.has_value(), "Failed to lex code. Error:" + err_msg_buffer, "Lex is valid.");
  auto source = Lexer::Sanitize(*expected_source);
  span<const Tk> src_view = source;
  auto parse_result = fn(TkCursorT{src_view.cbegin(), src_view.cend()});
  err_msg_buffer = parse_result ? "" : parse_result.error().Format();
  ASSERT_TRUE_LOG(parse_result.has_value(), "Failed to parse code. Error:" + err_msg_buffer, "Parse is valid.");
  ASSERT_TRUE_LOG(TestCompareAst(parse_result->ast, expected, flags),
                  format("Expected syntax tree is not equal:\n[Expected]:\n{}\n[Parsed]:\n{}\n", expected.Format(),
                         parse_result->ast.Format()),
                  "Expected syntax tree is equal.");
}

}  // namespace cnd::unit_tests

/// @} // end of cnd_unit_test

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