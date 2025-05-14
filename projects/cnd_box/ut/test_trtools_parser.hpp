///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2025 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file Static Unit Test : LexerLexPunctuator
/// @ingroup cnd_unit_test
/// @brief
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup cnd_unit_test
/// @{

#include "minitest.hpp"
#include "trtools/parser.hpp"

std::ostream& operator<<(std::ostream& os, const cnd::corevals::grammar::eAst& obj) {
  os << static_cast<std::underlying_type<cnd::corevals::grammar::eAst>::type>(obj);
  return os;
}

namespace test_util {

// Compare two produced asts using minitest to log any inequality.
static inline bool CompareAst(const cnd::Ast& node1, const cnd::Ast& node2) noexcept {
  // Compare node types
  EXPECT_EQ_LOG(node1.type, node2.type,
                std::format("Ast type {} not equal {}.", cnd::eAstToCStr(node1.type), cnd::eAstToCStr(node2.type)),
                "Ast Type Comp.");
  if (node1.type != node2.type) {
    return false;
  }

  // Compare node values
  auto lit1 = node1.GetLiteral();
  auto lit2 = node2.GetLiteral();
  EXPECT_EQ_LOG(lit1, lit2, std::format("Ast literal '{}' not equal '{}'.", lit1, lit2), "Ast Literal Comp.");
  if (lit1 != lit2) return false;

  // Compare number of Children
  EXPECT_TRUE_LOG(
      node1.children.size() != node2.children.size(),
      std::format("Ast branch quantity not equal. Left: {} Right: {}.", node1.children.size(), node2.children.size()),
      "Ast Size Comp.");
  if (node1.children.size() != node2.children.size()) {
    return false;
  }

  // Recursively compare Children
  for (size_t i = 0; i < node1.children.size(); ++i) {
    if (!CompareAst(node1.children.at(i), node2.children.at(i))) return false;
  }

  // If all checks pass, the ASTs are equal
  return true;
}

// Compare a produced ast to a synthesized ast using minitest to log any inequality.
static inline bool CompareAst(const cnd::Ast& node1, const cnd::Sast& node2) noexcept {
  // Compare node types
  EXPECT_EQ_LOG(node1.type, node2.type,
                std::format("Ast type {} not equal {}.", cnd::eAstToCStr(node1.type), cnd::eAstToCStr(node2.type)),
                "Ast Type Comp.");
  if (node1.type != node2.type) {
    return false;
  }

  // Compare node values
  auto lit1 = node1.GetLiteral();
  EXPECT_EQ_LOG(lit1, node2.literal, std::format("Ast literal '{}' not equal '{}'.", lit1, node2.literal),
                "Ast Literal Comp.");
  if (lit1 != node2.literal) return false;

  // Compare number of Children
  EXPECT_TRUE_LOG(
      node1.children.size() == node2.children.size(),
      std::format("Ast branch quantity not equal. Left: {} Right: {}.", node1.children.size(), node2.children.size()),
      "Ast Size Comp.");
  if (node1.children.size() != node2.children.size()) {
    return false;
  }

  // Recursively compare Children
  for (size_t i = 0; i < node1.children.size(); ++i) {
    if (!CompareAst(node1.children.at(i), node2.children.at(i))) return false;
  }

  // If all checks pass, the ASTs are equal
  return true;
}

static inline void TestLLParserMethod(std::string_view code,
                                      cnd::trtools::parser::LLPrsResT (*fn)(cnd::trtools::parser::TkCursorT),
                                      const std::string& test_name) {
  auto expected_source = cnd::trtools::Lexer::Lex(code);
  ASSERT_TRUE(expected_source.has_value());
  auto source = cnd::trtools::Lexer::Sanitize(expected_source.value());
  std::span<const cnd::Tk> src_view = source;
  auto parse_result = fn(cnd::trtools::parser::TkCursorT{src_view.cbegin(), src_view.cend()});
  ASSERT_TRUE_LOG(parse_result.has_value(), test_name, test_name + ": Failed to parse Ast.");
}

static inline void TestLLParserMethod(std::string_view code,
                                      cnd::trtools::parser::LLPrsResT (*fn)(cnd::trtools::parser::TkCursorT),
                                      const std::string& test_name, const cnd::Sast& expected) {
  auto expected_source = cnd::trtools::Lexer::Lex(code);
  ASSERT_TRUE(expected_source.has_value());
  auto source = cnd::trtools::Lexer::Sanitize(expected_source.value());
  std::span<const cnd::Tk> src_view = source;
  auto parse_result = fn(cnd::trtools::parser::TkCursorT{src_view.cbegin(), src_view.cend()});
  ASSERT_TRUE_LOG(parse_result.has_value(), test_name, test_name + ": Failed to parse Ast.");
  EXPECT_TRUE_LOG(CompareAst(parse_result.value().ast, expected), test_name,
                  test_name + ":Ast is not equal to expected format.");
}

static inline void TestLLParserMethod(std::string_view code,
                                      cnd::trtools::parser::LLPrsResT (*fn)(cnd::trtools::parser::TkCursorT),
                                      const cnd::Sast& expected) {
  auto expected_source = cnd::trtools::Lexer::Lex(code);
  ASSERT_TRUE(expected_source.has_value());
  auto source = cnd::trtools::Lexer::Sanitize(expected_source.value());
  std::span<const cnd::Tk> src_view = source;
  auto parse_result = fn(cnd::trtools::parser::TkCursorT{src_view.cbegin(), src_view.cend()});
  ASSERT_TRUE(parse_result.has_value());
  EXPECT_TRUE(true);
  EXPECT_TRUE(CompareAst(parse_result.value().ast, expected));
}

static inline void TestLLParse(std::string_view code,
                               cnd::trtools::parser::LLPrsResT (*fn)(cnd::trtools::parser::TkCursorT),
                               const cnd::Sast& expected) {
  using std::format;

  auto expected_source = cnd::trtools::Lexer::Lex(code);
  ASSERT_TRUE_LOG(
      expected_source.has_value(), "[TestLLParse] Tokenization is valid.",
      format("[TestLLParse] Unexpected lexer error: {} \n[INPUT]: {}", expected_source.error().Format(), code));

  auto source = cnd::trtools::Lexer::Sanitize(expected_source.value());
  std::span<const cnd::Tk> src_view = source;
  auto parse_result = fn({src_view.cbegin(), src_view.cend()});

  ASSERT_TRUE_LOG(
      parse_result.has_value(), "[TestLLParse] Parse is valid.",
      format("[TestLLParse] Unexpected parser error: {} \n[INPUT]: {}", parse_result.error().Format(), code));
  ASSERT_TRUE_LOG(CompareAst(parse_result.value().ast, expected), "[TestLLParse] Expected syntax tree is equal.",
                  format("[TestLLParse] Expected syntax tree is not equal:\n[EXPECTED]:\n {}\nPARSED:\n {}\n",
                         expected.Format(), parse_result.value().ast.Format()));
}

static inline void TestLLParse(std::string_view code,
                               cnd::trtools::parser::LLPrsResT (*fn)(cnd::trtools::parser::TkCursorT),
                               std::string_view test_name, const cnd::Sast& expected) {
  using std::format;
  auto expected_source = cnd::trtools::Lexer::Lex(code);
  ASSERT_TRUE_LOG(expected_source.has_value(), "[TestLLParse] Tokenization is valid.",
                  format("[TestLLParse][{}] Unexpected lexer error: {} \n[INPUT]: {}", test_name,
                         expected_source.error().Format(), code));

  auto source = cnd::trtools::Lexer::Sanitize(expected_source.value());
  std::span<const cnd::Tk> src_view = source;
  auto parse_result = fn({src_view.cbegin(), src_view.cend()});
  ASSERT_TRUE_LOG(parse_result.has_value(), "[TestLLParse] Parse is valid.",
                  format("[TestLLParse][{}] Unexpected parser error: {}\n[INPUT]: {}", test_name,
                         parse_result.error().Format(), code));
  ASSERT_TRUE_LOG(CompareAst(parse_result.value().ast, expected), "[TestLLParse] Expected syntax tree is equal.",
                  format("[TestLLParse][{}] Expected syntax tree is not equal:\n[EXPECTED]:\n {}\nPARSED:\n {}\n",
                         test_name, expected.Format(), parse_result.value().ast.Format()));
}

static inline void TestLLParse(std::string_view code,
                               cnd::trtools::parser::LLPrsResT (*fn)(cnd::trtools::parser::TkCursorT)) {
  using namespace cnd::trtools::literals;
  auto expected_source = cnd::trtools::Lexer::Lex(code);
  ASSERT_TRUE_LOG(
      expected_source.has_value(), "[TestLLParse] Tokenization is valid.",
      format("[TestLLParse] Unexpected lexer error: {} \n[INPUT]: {}", expected_source.error().Format(), code));

  auto source = cnd::trtools::Lexer::Sanitize(expected_source.value());
  std::span<const cnd::Tk> src_view = source;
  auto parse_result = fn({src_view.cbegin(), src_view.cend()});
  ASSERT_TRUE_LOG(
      parse_result.has_value(), "[TestLLParse] Parse is valid.",
      format("[TestLLParse] Unexpected parser error: {} \n[INPUT]: {}", parse_result.error().Format(), code));
}

}  // namespace test_util

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Test Parsing Basic Operands. */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(test_llparser, ParseOperand_WholeNumber) {
  using namespace cnd;
  test_util::TestLLParserMethod("1", trtools::parser::ParseOperand, Sast{eAst::kLitInt, "1"});
}


TEST(test_llparser, ParseOperand_RealNumber) {
  using namespace cnd;
  test_util::TestLLParserMethod("1.1", trtools::parser::ParseOperand, Sast{eAst::kLitReal, "1.1"});
}

TEST(test_llparser, ParseOperand_CString) {
  using namespace cnd;
  test_util::TestLLParserMethod("\"string literal\"", trtools::parser::ParseOperand,
                                Sast{eAst::kLitCstr, "\"string literal\""});
  test_util::TestLLParserMethod("\"\\\\\"", trtools::parser::ParseOperand, Sast{eAst::kLitCstr, "\"\\\\\""});
}

TEST(test_llparser, ParseOperand_Alnumus) {
  using namespace cnd;
  test_util::TestLLParserMethod("alnumus", trtools::parser::ParseOperand, Sast{eAst::kIdent, "alnumus"});
}

TEST(test_llparser, ParseOperand_ByteLiteral) {
  using namespace cnd;
  test_util::TestLLParserMethod("1c", trtools::parser::ParseOperand, Sast{eAst::kLitByte, "1c"});
}

TEST(test_llparser, ParseOperand_BoolLiteral) {
  using namespace cnd;
  test_util::TestLLParserMethod("1b", trtools::parser::ParseOperand, Sast{eAst::kLitBool, "1b"});
}

TEST(test_llparser, ParseOperand_UnsignedLiteral) {
  using namespace cnd;
  test_util::TestLLParserMethod("1u", trtools::parser::ParseOperand, Sast{eAst::kLitUint, "1u"});
}

TEST(test_llparser, ParseOperand_NoneKeyword) {
  using namespace cnd;
  test_util::TestLLParserMethod("none", trtools::parser::ParseOperand, Sast{eAst::kKwNone, "none"});
}

// This test confirms proper advancement of the iterator after parsing an operand.
TEST(test_llparser, ParseOperand_AllSingularOperands) {
  using namespace cnd::trtools::literals;
  using namespace cnd;
  using namespace cnd::trtools::parser;
  CND_CX auto src = "1 1.1 \"string literal\" \"\\\\\" alnumus 1c 1b 1u none "_cndTkLiteral;
  auto src_span = std::span{src.data(), src.size()};

  // Operands.
  std::vector<std::pair<eAst, std::string>> expected_result = {
      std::make_pair(eAst::kLitInt, "1"),
      std::make_pair(eAst::kLitReal, "1.1"),
      std::make_pair(eAst::kLitCstr, "\"string literal\""),
      std::make_pair(eAst::kLitCstr, "\"\\\\\""),
      std::make_pair(eAst::kIdent, "alnumus"),
      std::make_pair(eAst::kLitByte, "1c"),
      std::make_pair(eAst::kLitBool, "1b"),
      std::make_pair(eAst::kLitUint, "1u"),
      std::make_pair(eAst::kKwNone, "none"),
  };

  auto num_lit = ParseOperand({src_span.cbegin(), src_span.cend()});
  ASSERT_TRUE(num_lit.has_value());
  EXPECT_EQ(num_lit.Value().ast.type, expected_result[0].first);
  EXPECT_EQ(num_lit.Value().ast.GetLiteral(), expected_result[0].second);

  auto real_lit = ParseOperand(num_lit.Value().head.Advance());
  ASSERT_TRUE(real_lit.has_value());
  EXPECT_EQ(real_lit.Value().ast.type, expected_result[1].first);
  EXPECT_EQ(real_lit.Value().ast.GetLiteral(), expected_result[1].second);

  auto str_lit = ParseOperand(real_lit.Value().head.Advance());
  ASSERT_TRUE(str_lit.has_value());
  EXPECT_EQ(str_lit.Value().ast.type, expected_result[2].first);
  EXPECT_EQ(str_lit.Value().ast.GetLiteral(), expected_result[2].second);

  auto str_lit2 = ParseOperand(str_lit.Value().head.Advance());
  ASSERT_TRUE(str_lit2.has_value());
  EXPECT_EQ(str_lit2.Value().ast.type, expected_result[3].first);
  EXPECT_EQ(str_lit2.Value().ast.GetLiteral(), expected_result[3].second);

  auto alnumus_lit = ParseOperand(str_lit2.Value().head.Advance());
  ASSERT_TRUE(alnumus_lit.has_value());
  EXPECT_EQ(alnumus_lit.Value().ast.type, expected_result[4].first);
  EXPECT_EQ(alnumus_lit.Value().ast.GetLiteral(), expected_result[4].second);

  auto byte_lit = ParseOperand(alnumus_lit.Value().head.Advance());
  ASSERT_TRUE(byte_lit.has_value());
  EXPECT_EQ(byte_lit.Value().ast.type, expected_result[5].first);
  EXPECT_EQ(byte_lit.Value().ast.GetLiteral(), expected_result[5].second);

  auto bit_lit = ParseOperand(byte_lit.Value().head.Advance());
  ASSERT_TRUE(bit_lit.has_value());
  EXPECT_EQ(bit_lit.Value().ast.type, expected_result[6].first);
  EXPECT_EQ(bit_lit.Value().ast.GetLiteral(), expected_result[6].second);

  auto unsigned_lit = ParseOperand(bit_lit.Value().head.Advance());
  ASSERT_TRUE(unsigned_lit.has_value());
  EXPECT_EQ(unsigned_lit.Value().ast.type, expected_result[7].first);
  EXPECT_EQ(unsigned_lit.Value().ast.GetLiteral(), expected_result[7].second);

  auto none_lit = ParseOperand(unsigned_lit.Value().head.Advance());
  ASSERT_TRUE(none_lit.has_value());
  EXPECT_EQ(none_lit.Value().ast.type, expected_result[8].first);
  EXPECT_EQ(none_lit.Value().ast.GetLiteral(), expected_result[8].second);
}

TEST(test_llparser, ParseResolution) {
  using namespace cnd;
  using namespace test_util;
  using enum eAst;
  using cnd::trtools::parser::ParseResolution;

  TestLLParse("a::b", ParseResolution, Sast{kResolutionOp, "a::b", Sast{kIdent, "a"}, Sast{kIdent, "b"}});
  TestLLParse("1::b", ParseResolution, Sast(kResolutionOp, "1::b", Sast{kLitInt, "1"}, Sast{kIdent, "b"}));
  TestLLParse("\"a\"::b", ParseResolution, Sast(kResolutionOp, "\"a\"::b", Sast{kLitCstr, "\"a\""}, Sast{kIdent, "b"}));
  TestLLParse("a::b", ParseResolution, Sast(kResolutionOp, "a::b", Sast{kIdent, "a"}, Sast{kIdent, "b"}));
  TestLLParse("a::1", ParseResolution, Sast(kResolutionOp, "a::1", Sast{kIdent, "a"}, Sast{kLitInt, "1"}));
  TestLLParse("a::\"a\"", ParseResolution, Sast(kResolutionOp, "a::\"a\"", Sast{kIdent, "a"}, Sast{kLitCstr, "\"a\""}));

  TestLLParse("a::b::c", ParseResolution,
              Sast(kResolutionOp, "a::b::c", Sast(kResolutionOp, "a::b", Sast{kIdent, "a"}, Sast{kIdent, "b"}),
                   Sast{kIdent, "c"}));
  TestLLParse("a::b::c::d", ParseResolution,
              Sast(kResolutionOp, "a::b::c::d",
                   Sast(kResolutionOp, "a::b::c", Sast(kResolutionOp, "a::b", Sast{kIdent, "a"}, Sast{kIdent, "b"}),
                        Sast{kIdent, "c"}),
                   Sast{kIdent, "d"}));

  // RunAndCompareParserMethod(ParseResolution, Sast(kDoubleColon, "::", Sast{kIdent, "a"}, Sast{kIdent, "b"}),
  // "(a::b)");

  // RunAndCompareParserMethod(
  //     ParseResolution,
  //     Sast(kDoubleColon, "::",
  //          Sast(kDoubleColon, "::", Sast(kDoubleColon, "::", Sast{kIdent, "a"}, Sast{kIdent, "b"}), Sast{kIdent,
  //          "c"}), Sast{kIdent, "d"}),
  //     "(a::b)::c::d");

  // RunAndCompareParserMethod(
  //     ParseResolution,
  //     Sast(kDoubleColon, "::",
  //          Sast(kDoubleColon, "::", Sast{kIdent, "a"}, Sast(kDoubleColon, "::", Sast{kIdent, "b"}, Sast{kIdent,
  //          "c"})), Sast{kIdent, "d"}),
  //     "a::(b::c)::d");
}

TEST(test_llparser, ParsePostfix) {
  using namespace cnd;
  using namespace test_util;
  using enum eAst;
  using cnd::trtools::parser::ParsePostfix;
  TestLLParse("a++", ParsePostfix, Sast(kInc, "a++", Sast(kIdent, "a")));
  TestLLParse("a--", ParsePostfix, Sast(kDec, "a--", Sast(kIdent, "a")));

  TestLLParse("a++++", ParsePostfix, Sast(kInc, "a++++", Sast(kInc, "a++", Sast(kIdent, "a"))));
  TestLLParse("a----", ParsePostfix, Sast(kDec, "a----", Sast(kDec, "a--", Sast(kIdent, "a"))));
  TestLLParse(
      "a++--++--", ParsePostfix,
      Sast(kDec, "a++--++--", Sast(kInc, "a++--++", Sast(kDec, "a++--", Sast(kInc, "a++", Sast(kIdent, "a"))))));

  TestLLParse("a::b::c++--", ParsePostfix);
  TestLLParse("a::b++--::c", ParsePostfix);
  TestLLParse("a++--::b::c", ParsePostfix);
}

TEST(test_llparser, ParseMemberAccess) {
  using namespace cnd;
  using namespace test_util;
  using enum eAst;
  using cnd::trtools::parser::ParseAccess;
  TestLLParse(
      "a.b.c.d", ParseAccess,
      Sast(kPeriod, "a.b.c.d",
           Sast(kPeriod, "a.b.c", Sast(kPeriod, "a.b", Sast(kIdent, "a"), Sast(kIdent, "b")), Sast(kIdent, "c")),
           Sast(kIdent, "d")));

  TestLLParse("a::b.d::c", ParseAccess);
  TestLLParse("a::b.d::c.e::f", ParseAccess);
  TestLLParse("a--.b++.c--.d++", ParseAccess);
}

TEST(test_llparser, ParsePrefix) {
  using namespace cnd;
  using namespace test_util;
  using enum eAst;
  using cnd::trtools::parser::ParsePrefix;
  TestLLParse("++a", ParsePrefix, Sast(kInc, "++a", Sast(kIdent, "a")));
  TestLLParse("--a", ParsePrefix, Sast(kDec, "--a", Sast(kIdent, "a")));
  TestLLParse("!a", ParsePrefix, Sast(kNot, "!a", Sast(kIdent, "a")));

  TestLLParse("----a", ParsePrefix, Sast(kDec, "----a", Sast(kDec, "--a", Sast(kIdent, "a"))));
  TestLLParse("++++a", ParsePrefix, Sast(kInc, "++++a", Sast(kInc, "++a", Sast(kIdent, "a"))));
  TestLLParse("!!a", ParsePrefix, Sast(kNot, "!!a", Sast(kNot, "!a", Sast(kIdent, "a"))));

  // Prefix -> Resolution
  TestLLParse("++a::b", ParsePrefix,
              Sast(kInc, "++a::b", Sast{kResolutionOp, "a::b", Sast{kIdent, "a"}, Sast{kIdent, "b"}}));
  TestLLParse("--a::b", ParsePrefix,
              Sast(kDec, "--a::b", Sast{kResolutionOp, "a::b", Sast{kIdent, "a"}, Sast{kIdent, "b"}}));
  TestLLParse("!a::b", ParsePrefix,
              Sast(kNot, "!a::b", Sast{kResolutionOp, "a::b", Sast{kIdent, "a"}, Sast{kIdent, "b"}}));

  // Prefix -> Postfix
  TestLLParse("++a++", ParsePrefix, Sast(kInc, "++a++", Sast(kInc, "a++", Sast(kIdent, "a"))));
  TestLLParse("--a++", ParsePrefix, Sast(kDec, "--a++", Sast(kInc, "a++", Sast(kIdent, "a"))));
  TestLLParse("!a++", ParsePrefix, Sast(kNot, "!a++", Sast(kInc, "a++", Sast(kIdent, "a"))));

  // Prefix -> MemberAccess
  TestLLParse("++a.b", ParsePrefix,
              Sast(kInc, "++a.b", Sast(kMemberAccess, "a.b", Sast(kIdent, "a"), Sast(kIdent, "b"))));
  TestLLParse("--a.b", ParsePrefix,
              Sast(kDec, "--a.b", Sast(kMemberAccess, "a.b", Sast(kIdent, "a"), Sast(kIdent, "b"))));
  TestLLParse("!a.b", ParsePrefix,
              Sast(kNot, "!a.b", Sast(kMemberAccess, "a.b", Sast(kIdent, "a"), Sast(kIdent, "b"))));
}

TEST(test_llparser, ParseProduction) {
  using namespace cnd;
  using namespace test_util;
  using enum eAst;
  using cnd::trtools::parser::ParseProduction;
  // '*' -> kMul
  TestLLParse("a * b", ParseProduction, Sast{kMul, "a*b", Sast{kIdent, "a"}, Sast{kIdent, "b"}});
  TestLLParse("a * b * c", ParseProduction,
              Sast(kMul, "a*b*c", Sast(kMul, "a*b", Sast{kIdent, "a"}, Sast{kIdent, "b"}), Sast{kIdent, "c"}));
  TestLLParse("a * b * c * d", ParseProduction,
              Sast(kMul, "a*b*c*d",
                   Sast(kMul, "a*b*c", Sast(kMul, "a*b", Sast{kIdent, "a"}, Sast{kIdent, "b"}), Sast{kIdent, "c"}),
                   Sast{kIdent, "d"}));
  // '/' -> kDiv
  TestLLParse("a / b", ParseProduction, Sast{kDiv, "a/b", Sast{kIdent, "a"}, Sast{kIdent, "b"}});
  TestLLParse("a / b / c", ParseProduction,
              Sast(kDiv, "a/b/c", Sast(kDiv, "a/b", Sast{kIdent, "a"}, Sast{kIdent, "b"}), Sast{kIdent, "c"}));
  TestLLParse("a / b / c / d", ParseProduction,
              Sast(kDiv, "a/b/c/d",
                   Sast(kDiv, "a/b/c", Sast(kDiv, "a/b", Sast{kIdent, "a"}, Sast{kIdent, "b"}), Sast{kIdent, "c"}),
                   Sast{kIdent, "d"}));
  // '%' -> kMod
  TestLLParse("a % b", ParseProduction, Sast{kMod, "a%b", Sast{kIdent, "a"}, Sast{kIdent, "b"}});
  TestLLParse("a % b % c", ParseProduction,
              Sast(kMod, "a%b%c", Sast(kMod, "a%b", Sast{kIdent, "a"}, Sast{kIdent, "b"}), Sast{kIdent, "c"}));
  TestLLParse("a % b % c % d", ParseProduction,
              Sast(kMod, "a%b%c%d",
                   Sast(kMod, "a%b%c", Sast(kMod, "a%b", Sast{kIdent, "a"}, Sast{kIdent, "b"}), Sast{kIdent, "c"}),
                   Sast{kIdent, "d"}));

  //// Production -> Resolution
  // TestLLParse("a::b * c::d", ParseProduction, Sast{kMul, "a::b*c::d", Sast{kIdent, "a"}, Sast{kIdent, "b"}});

  //// Production -> Postfix
  // TestLLParse("a::b * c::d", ParseProduction, Sast{kMul, "a::b*c::d", Sast{kIdent, "a"}, Sast{kIdent, "b"}});

  //// Production -> MemberAccess
  // TestLLParse("a::b * c::d", ParseProduction, Sast{kMul, "a::b*c::d", Sast{kIdent, "a"}, Sast{kIdent, "b"}});

  //// Production -> Prefix
  // TestLLParse("a::b * c::d", ParseProduction, Sast{kMul, "a::b*c::d", Sast{kIdent, "a"}, Sast{kIdent, "b"}});
}

TEST(test_llparser, ParseSummation) {
  using namespace cnd;
  using namespace test_util;
  using enum eAst;
  using cnd::trtools::parser::ParseSummation;
  // '+' -> kAdd
  TestLLParse("a + b", ParseSummation, Sast{kAdd, "a+b", Sast{kIdent, "a"}, Sast{kIdent, "b"}});
  TestLLParse("a + b + c", ParseSummation,
              Sast(kAdd, "a+b+c", Sast(kAdd, "a+b", Sast{kIdent, "a"}, Sast{kIdent, "b"}), Sast{kIdent, "c"}));
  TestLLParse("a + b + c + d", ParseSummation,
              Sast(kAdd, "a+b+c+d",
                   Sast(kAdd, "a+b+c", Sast(kAdd, "a+b", Sast{kIdent, "a"}, Sast{kIdent, "b"}), Sast{kIdent, "c"}),
                   Sast{kIdent, "d"}));
  // '-' -> kSub
  TestLLParse("a - b", ParseSummation, Sast{kSub, "a-b", Sast{kIdent, "a"}, Sast{kIdent, "b"}});
  TestLLParse("a - b - c", ParseSummation,
              Sast(kSub, "a-b-c", Sast(kSub, "a-b", Sast{kIdent, "a"}, Sast{kIdent, "b"}), Sast{kIdent, "c"}));
  TestLLParse("a - b - c - d", ParseSummation,
              Sast(kSub, "a-b-c-d",
                   Sast(kSub, "a-b-c", Sast(kSub, "a-b", Sast{kIdent, "a"}, Sast{kIdent, "b"}), Sast{kIdent, "c"}),
                   Sast{kIdent, "d"}));

  //// Production -> Resolution
  // TestLLParse("a::b * c::d", ParseProduction, Sast{kMul, "a::b*c::d", Sast{kIdent, "a"}, Sast{kIdent, "b"}});

  //// Production -> Postfix
  // TestLLParse("a::b * c::d", ParseProduction, Sast{kMul, "a::b*c::d", Sast{kIdent, "a"}, Sast{kIdent, "b"}});

  //// Production -> MemberAccess
  // TestLLParse("a::b * c::d", ParseProduction, Sast{kMul, "a::b*c::d", Sast{kIdent, "a"}, Sast{kIdent, "b"}});

  //// Production -> Prefix
  // TestLLParse("a::b * c::d", ParseProduction, Sast{kMul, "a::b*c::d", Sast{kIdent, "a"}, Sast{kIdent, "b"}});

  /// Summation -> Production
}

TEST(test_llparser, ParseListFold) {
  using namespace cnd;
  using namespace test_util;
  using enum eAst;
  using cnd::trtools::parser::ParseListFold;

  TestLLParse("a,b", ParseListFold, Sast{kComma, "a,b", Sast{kIdent, "a"}, Sast{kIdent, "b"}});
  TestLLParse("a,b,c", ParseListFold,
              Sast(kComma, "a,b,c", Sast(kComma, "a,b", Sast{kIdent, "a"}, Sast{kIdent, "b"}), Sast{kIdent, "c"}));
  TestLLParse("a , b , c , d", ParseListFold,
              Sast(kComma, "a,b,c,d",
                   Sast(kComma, "a,b,c", Sast(kComma, "a,b", Sast{kIdent, "a"}, Sast{kIdent, "b"}), Sast{kIdent, "c"}),
                   Sast{kIdent, "d"}));

  //// Production -> Resolution
  // TestLLParse("a::b * c::d", ParseProduction, Sast{kMul, "a::b*c::d", Sast{kIdent, "a"}, Sast{kIdent, "b"}});

  //// Production -> Postfix
  // TestLLParse("a::b * c::d", ParseProduction, Sast{kMul, "a::b*c::d", Sast{kIdent, "a"}, Sast{kIdent, "b"}});

  //// Production -> MemberAccess
  // TestLLParse("a::b * c::d", ParseProduction, Sast{kMul, "a::b*c::d", Sast{kIdent, "a"}, Sast{kIdent, "b"}});

  //// Production -> Prefix
  // TestLLParse("a::b * c::d", ParseProduction, Sast{kMul, "a::b*c::d", Sast{kIdent, "a"}, Sast{kIdent, "b"}});

  /// Summation -> Production
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Test LR Primary Expr Parser. */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(LLParser_PrimaryExpressions, WholeNumber) {
  using namespace cnd;
  using namespace test_util;
  using enum eAst;
  using cnd::trtools::parser::ParsePrimaryExpr;

  TestLLParse("1", ParsePrimaryExpr, Sast(kLitInt, "1"));
}

TEST(LLParser_PrimaryExpressions, RealNumber) {
  using namespace cnd;
  using namespace test_util;
  using enum eAst;
  using cnd::trtools::parser::ParsePrimaryExpr;
  TestLLParse("1.1", ParsePrimaryExpr, Sast(kLitReal, "1.1"));
}

TEST(LLParser_PrimaryExpressions, CString) {
  using namespace cnd;
  using namespace test_util;
  using enum eAst;
  using cnd::trtools::parser::ParsePrimaryExpr;
  TestLLParse("\"string literal\"", ParsePrimaryExpr, Sast(kLitCstr, "\"string literal\""));
}

TEST(LLParser_PrimaryExpressions, CStringEscaped) {
  using namespace cnd;
  using namespace test_util;
  using enum eAst;
  using cnd::trtools::parser::ParsePrimaryExpr;
  TestLLParse("\"\\\\\"", ParsePrimaryExpr, Sast(kLitCstr, "\"\\\\\""));
}

TEST(LLParser_PrimaryExpressions, Alnumus) {
  using namespace cnd;
  using namespace test_util;
  using enum eAst;
  using cnd::trtools::parser::ParsePrimaryExpr;
  TestLLParse("alnumus", ParsePrimaryExpr, Sast(kIdent, "alnumus"));
}

TEST(LLParser_PrimaryExpressions, ByteLiteral) {
  using namespace cnd;
  using namespace test_util;
  using enum eAst;
  using cnd::trtools::parser::ParsePrimaryExpr;
  TestLLParse("1c", ParsePrimaryExpr, Sast(kLitByte, "1c"));
}

TEST(LLParser_PrimaryExpressions, BoolLiteral) {
  using namespace cnd;
  using namespace test_util;
  using enum eAst;
  using cnd::trtools::parser::ParsePrimaryExpr;
  TestLLParse("1b", ParsePrimaryExpr, Sast(kLitBool, "1b"));
}

TEST(LLParser_PrimaryExpressions, UnsignedLiteral) {
  using namespace cnd;
  using namespace test_util;
  using enum eAst;
  using cnd::trtools::parser::ParsePrimaryExpr;
  TestLLParse("1u", ParsePrimaryExpr, Sast(kLitUint, "1u"));
}

TEST(LLParser_PrimaryExpressions, NoneKeyword) {
  using namespace cnd;
  using namespace test_util;
  using enum eAst;
  using cnd::trtools::parser::ParsePrimaryExpr;
  TestLLParse("none", ParsePrimaryExpr, Sast(kKwNone, "none"));
}

TEST(LLParser_PrimaryExpressions, OperandInParenthesis) {
  using namespace cnd;
  using namespace test_util;
  using enum eAst;
  using cnd::trtools::parser::ParsePrimaryExpr;
  TestLLParse("(1)", ParsePrimaryExpr, Sast(kSubexpression, "(1)", Sast(kLitInt, "1")));
}

TEST(LLParser_PrimaryExpressions, OperandInSquareBrackets) {
  using namespace cnd;
  using namespace test_util;
  using enum eAst;
  using cnd::trtools::parser::ParsePrimaryExpr;
  TestLLParse("[1]", ParsePrimaryExpr, Sast(kSquareSubexpr, "[1]", Sast(kLitInt, "1")));
}

TEST(LLParser_PrimaryExpressions, OperandInCurlyBraces) {
  using namespace cnd;
  using namespace test_util;
  using enum eAst;
  using cnd::trtools::parser::ParsePrimaryExpr;
  TestLLParse("{1}", ParsePrimaryExpr, Sast(kCurlySubexpr, "{1}", Sast(kLitInt, "1")));
}

TEST(LLParser_PrimaryExpressions, BinarySum) {
  using namespace cnd;
  using namespace test_util;
  using enum eAst;
  using cnd::trtools::parser::ParsePrimaryExpr;
  TestLLParse("1 + 1", ParsePrimaryExpr, Sast(kAdd, "1+1", Sast(kLitInt, "1"), Sast(kLitInt, "1")));
}

TEST(LLParser_PrimaryExpressions, BinarySumThenTerm) {
  using namespace cnd;
  using namespace test_util;
  using enum eAst;
  using cnd::trtools::parser::ParsePrimaryExpr;
  TestLLParse("1 + 2 * 3", ParsePrimaryExpr,
              Sast(kAdd, "1+2*3", Sast(kLitInt, "1"), Sast(kMul, "2*3", Sast(kLitInt, "2"), Sast(kLitInt, "3"))));
}

TEST(LLParser_PrimaryExpressions, BinaryTermThenSum) {
  using namespace cnd;
  using namespace test_util;
  using enum eAst;
  using cnd::trtools::parser::ParsePrimaryExpr;
  TestLLParse("1 * 2 + 3", ParsePrimaryExpr,
              Sast(kAdd, "1*2+3", Sast(kMul, "1*2", Sast(kLitInt, "1"), Sast(kLitInt, "2")), Sast(kLitInt, "3")));
}

TEST(LLParser_PrimaryExpressions, BinarySumIsLeftAssociative) {
  using namespace cnd;
  using enum eAst;
  test_util::TestLLParse("1 + 2 - 3", trtools::parser::ParsePrimaryExpr,
                         Sast(kSub, "1+2-3", Sast(kAdd, "1+2", Sast(kLitInt, "1"), Sast(kLitInt, "2")), Sast(kLitInt, "3")));
}

TEST(LLParser_PrimaryExpressions, BinaryMemberAccessIsLeftAssociative) {
  using namespace cnd;
  using enum eAst;
  test_util::TestLLParse(
      "a.b.c.d", trtools::parser::ParsePrimaryExpr,
      Sast(kMemberAccess, "a.b.c.d",
                              Sast(kMemberAccess, "a.b.c",
                                   Sast(kMemberAccess, "a.b", Sast(kIdent, "a"), Sast(kIdent, "b")), Sast(kIdent, "c")),
           Sast(kIdent, "d")));
}

TEST(LLParser_PrimaryExpressions, ParenIsResolvedFirst) {
  using namespace cnd;
  using namespace test_util;
  using enum eAst;
  test_util::TestLLParse("(1+2)*3", trtools::parser::ParsePrimaryExpr,
                         Sast(kMul, "(1+2)*3",
                              Sast(kSubexpression, "(1+2)", Sast(kAdd, "1+2", Sast(kLitInt, "1"), Sast(kLitInt, "2"))),
                                   Sast(kLitInt, "3")));
}

TEST(LLParser_PrimaryExpressions, UnaryPrefix) {
  using namespace cnd;
  using namespace test_util;
  using enum eAst;
  test_util::TestLLParse("!a", trtools::parser::ParsePrimaryExpr, Sast(kNot, "!a", Sast(kIdent, "a")));
}

TEST(LLParser_PrimaryExpressions, RepeatedUnaryPrefix) {
  using namespace cnd;
  using namespace test_util;
  using enum eAst;
  test_util::TestLLParse("!!a", trtools::parser::ParsePrimaryExpr, "Unary Prefix Repeated",
                         Sast(kNot, "!!a", Sast(kNot, "!a", Sast(kIdent, "a"))));
}

TEST(LLParser_PrimaryExpressions, AssignmentIsRightAssociative) {
  using namespace cnd;
  using namespace test_util;
  using enum eAst;
  using cnd::trtools::parser::ParseAssignment;
  // (a=(b=c))
  TestLLParse("a=b=c", ParseAssignment,
              Sast(kAssign, "a=b=c", Sast(kIdent, "a"), Sast(kAssign, "b=c", Sast(kIdent, "b"), Sast(kIdent, "c"))));
  // (a=(b=c))
  TestLLParse(
      "x=a=b=c", ParseAssignment,
      Sast(kAssign, "x=a=b=c", Sast(kIdent, "x"),
           Sast(kAssign, "a=b=c", Sast(kIdent, "a"), Sast(kAssign, "b=c", Sast(kIdent, "b"), Sast(kIdent, "c")))));
}

TEST(LLParser_PrimaryExpressions, PrefixUnaryThenBinary) {
  using namespace cnd;
  using namespace test_util;
  using enum eAst;
  test_util::TestLLParse("!1+2", trtools::parser::ParsePrimaryExpr, "Prefix Unary then Binary",
                         Sast(kAdd, "!1+2", Sast(kNot, "!1", Sast(kLitInt, "1")), Sast(kLitInt, "2")));
}

TEST(LLParser_PrimaryExpressions, PrefixUnaryThenMemberAccess) {
  using namespace cnd;
  using namespace test_util;
  using enum eAst;
  test_util::TestLLParse("!a.b", trtools::parser::ParsePrimaryExpr, "Prefix Unary then Higher Priority Member Access",
                         Sast(kNot, "!a.b", Sast(kMemberAccess, "a.b", Sast(kIdent, "a"), Sast(kIdent, "b"))));
}

TEST(LLParser_PrimaryExpressions, PrefixUnaryAfterBinary) {
  using namespace cnd;
  using namespace test_util;
  using enum eAst;
  test_util::TestLLParse("1+!2", trtools::parser::ParsePrimaryExpr, "Prefix Unary After Binary 1+!2",
                         Sast(kAdd, "1+!2", Sast(kLitInt, "1"), Sast(kNot, "!2", Sast(kLitInt, "2"))));
}

TEST(LLParser_PrimaryExpressions, EmptyFunctionCall) {
  using namespace cnd;
  using namespace test_util;
  using enum eAst;
  test_util::TestLLParse("a()", trtools::parser::ParsePrimaryExpr, "Empty Function Call",
                         Sast(kFunctionCall, "a()", Sast(kIdent, "a"), Sast(kArguments, "()")));
}

TEST(LLParser_PrimaryExpressions, PrefixUnaryThenFunctionCall) {
  using namespace cnd;
  using namespace test_util;
  using enum eAst;
  test_util::TestLLParse("!a()", trtools::parser::ParsePrimaryExpr, "Prefix Unary then Function Call",
                         Sast(kNot, "!a()", Sast(kFunctionCall, "a()", Sast(kIdent, "a"), Sast(kArguments, "()"))));
}

TEST(LLParser_PrimaryExpressions, RepeatedUnaryThenRepeatedFunctionCall) {
  using namespace cnd;
  using namespace test_util;
  using enum eAst;
  test_util::TestLLParse(
      "!!a()()", trtools::parser::ParsePrimaryExpr, "Repeated Prefix and Repeated Function Call",
      Sast(kNot, "!!a()()",
           Sast(kNot, "!a()()",
                Sast(kFunctionCall, "a()()", Sast(kFunctionCall, "a()", Sast(kIdent, "a"), Sast(kArguments, "()")),
                     Sast(kArguments, "()")))));
}

TEST(LLParser_PrimaryExpressions, FunctionCallThenBinary) {
  using namespace cnd;
  using namespace test_util;
  using enum eAst;
  test_util::TestLLParse(
      "a()+2", trtools::parser::ParsePrimaryExpr, "Function Call then Binary",
      Sast(kAdd, "a()+2", Sast(kFunctionCall, "a()", Sast(kIdent, "a"), Sast(kArguments, "()")), Sast(kLitInt, "2")));
}

TEST(LLParser_PrimaryExpressions, FunctionCallThenMemberAccess) {
  using namespace cnd;
  using namespace test_util;
  using enum eAst;
  test_util::TestLLParse(
      "a().b", trtools::parser::ParsePrimaryExpr, "Function Call then Member Access",
      Sast(kMemberAccess, "a().b", Sast(kFunctionCall, "a()", Sast(kIdent, "a"), Sast(kArguments, "()")), Sast(kIdent, "b")));
}

TEST(LLParser_PrimaryExpressions, MemberAccessThenFunctionCall) {
  using namespace cnd;
  using namespace test_util;
  using enum eAst;
  test_util::TestLLParse(
      "a.b()", trtools::parser::ParsePrimaryExpr, "Member Access then Function Call",
      Sast(kFunctionCall, "a.b()", Sast(kMemberAccess, "a.b", Sast(kIdent, "a"), Sast(kIdent, "b")), Sast(kArguments, "()")));
}

TEST(LLParser_PrimaryExpressions, BinaryThenFunctionCall) {
  using namespace cnd;
  using namespace test_util;
  using enum eAst;
  test_util::TestLLParse(
      "1+a()", trtools::parser::ParsePrimaryExpr, "Binary Then Function Call 1 + a()",
      Sast(kAdd, "1+a()", Sast(kLitInt, "1"), Sast(kFunctionCall, "a()", Sast(kIdent, "a"), Sast(kArguments, "()"))));
}

TEST(LLParser_PrimaryExpressions, InteleavedMemberAccessAndFunctionCall) {
  using namespace cnd;
  using namespace test_util;
  using enum eAst;
  // a.b().c is parsed as (a.b()).c and not as a.(b().c) because of left-to-right associativity of member access.
  // Source: cppreference.com
  test_util::TestLLParse(
      "a.b().c.d", trtools::parser::ParsePrimaryExpr, "Repeated Member Access with internal Function Call",
      Sast(kMemberAccess, ".",
           Sast(kMemberAccess, ".",
                Sast(kFunctionCall, "", Sast(kMemberAccess, ".", Sast(kIdent, "a"), Sast(kIdent, "b")),
                     Sast(kArguments, "")),
                Sast(kIdent, "c")),
           Sast(kIdent, "d")));
}

TEST(LLParser_PrimaryExpressions, ComplexExpr) {
  using namespace cnd;
  using namespace test_util;
  using enum eAst;
  test_util::TestLLParse("foo.bar()*1+1", trtools::parser::ParsePrimaryExpr, "Complex Expression: foo.bar()*1+1",
                         Sast(kAdd, "foo.bar()*1+1",
                              Sast(kMul, "foo.bar()*1",
                                   Sast(kFunctionCall, "foo.bar()", Sast(kMemberAccess, "foo.bar", Sast(kIdent, "foo"), Sast(kIdent, "bar")),
                                        Sast(kArguments, "()")),
                                   Sast(kLitInt, "1")),
                              Sast(kLitInt, "1")));
}

TEST(LLParser_PrimaryExpressions, ComplexExpr2) {
  using namespace cnd;
  using namespace test_util;
  using enum eAst;
  test_util::TestLLParse("(foo.bar() + 1) * 1", trtools::parser::ParsePrimaryExpr,
                         "Complex Expression: (foo.bar() + 1) * 1",
                         Sast(kMul, "(foo.bar()+1)*1",
                              Sast(kAdd, "foo.bar()+1",
                                   Sast(kFunctionCall, "foo.bar()", Sast(kMemberAccess, "foo.bar", Sast(kIdent, "foo"), Sast(kIdent, "bar")),
                                        Sast(kArguments, "()")),
                                   Sast(kLitInt, "1")),
                              Sast(kLitInt, "1")));
}

TEST(LLParser_PrimaryExpressions, AssignmentExpression) {
  using namespace cnd;
  using namespace test_util;
  using enum eAst;
  test_util::TestLLParse(
      "foo = 1 + 2", trtools::parser::ParsePrimaryExpr, "Assignment Expression : foo = 1 + 2",
      Sast(kAssign, "foo=1+2", Sast(kIdent, "foo"), Sast(kAdd, "1+2", Sast(kLitInt, "1"), Sast(kLitInt, "2"))));
}

TEST(LLParser_PrimaryExpressions, FunctionCallAfterBinaryLeftAssociative) {
  using namespace cnd;
  using namespace test_util;
  using enum eAst;
  test_util::TestLLParse(
      "a+b+c+d()", trtools::parser::ParsePrimaryExpr,
      "Function Call After Binary Left Associative:  a+b+c+d()",
              Sast(kAdd, "a+b+c+d()",
                   Sast(kAdd, "a+b+c", Sast(kAdd, "a+b", Sast{kIdent, "a"}, Sast{kIdent, "b"}), Sast{kIdent, "c"}),
           Sast(kFunctionCall, "d()", Sast(kIdent, "d"), Sast(kArguments, "()"))));
}

TEST(LLParser_PrimaryExpressions, FunctionCallWithArguments) {
  using namespace cnd;
  using namespace test_util;
  using enum eAst;
  test_util::TestLLParse(
      "foo(a, b, c)", trtools::parser::ParsePrimaryExpr, "Function Call With Arguments",
      Sast(kFunctionCall, "foo(a,b,c)", Sast(kIdent, "foo"),
           Sast(kArguments, "(a,b,c)",
                Sast(kComma, "a,b,c", Sast(kComma, "a,b", Sast{kIdent, "a"}, Sast{kIdent, "b"}), Sast{kIdent, "c"}))));
}

TEST(LLParser_PrimaryExpressions, IndexingCallWithArguments) {
  using namespace cnd;
  using namespace test_util;
  using enum eAst;
  test_util::TestLLParse(
      "foo[a, b, c]", trtools::parser::ParsePrimaryExpr, "Indexing",
      Sast(kIndexOperator, "foo[a,b,c]", Sast(kIdent, "foo"),
           Sast(kArguments, "[a,b,c]",
                Sast(kComma, "a,b,c", Sast(kComma, "a,b", Sast{kIdent, "a"}, Sast{kIdent, "b"}), Sast{kIdent, "c"}))));
}

TEST(LLParser_PrimaryExpressions, ListingCallWithArguments) {
  using namespace cnd;
  using namespace test_util;
  using enum eAst;
  test_util::TestLLParse(
      "foo{a,b,c}", trtools::parser::ParsePrimaryExpr, "Listing Operator foo{1,2,3}",
      Sast(kListingOperator, "foo{a,b,c}", Sast(kIdent, "foo"),
           Sast(kArguments, "{a,b,c}",
                Sast(kComma, "a,b,c", Sast(kComma, "a,b", Sast{kIdent, "a"}, Sast{kIdent, "b"}), Sast{kIdent, "c"}))));
}

TEST(LLParser_PrimaryExpressions, ResolutionOperator) {
  using namespace cnd;
  using namespace test_util;
  using enum eAst;
  test_util::TestLLParse("foo::bar", trtools::parser::ParsePrimaryExpr, "Access Operator foo::bar",
                         Sast(kResolutionOp, "foo::bar", Sast(kIdent, "foo"), Sast(kIdent, "bar")));
}

//TES(LLParser_PrimaryExpressions, UnaryMinusSingleOperand) {
//  using namespace cnd;
//  using namespace test_util;
//  using enum eAst;
//  test_util::TestLLParse("-1", trtools::parser::ParsePrimaryExpr, "Unary Minus",
//                         Sast(kUnaryMinus, "-1", Sast(kLitInt, "1")));
//}
//END_MTEST(LLParser_PrimaryExpressions, UnaryMinusSingleOperand);
//
//TES(LLParser_PrimaryExpressions, UnaryMinusInExpr) {
//  using namespace cnd;
//  using namespace test_util;
//  using enum eAst;
//  test_util::TestLLParse("-1+2", trtools::parser::ParsePrimaryExpr, "Unary Minus in expression",
//                         Sast(kAdd, "-1+2", Sast(kUnaryMinus, "-1", Sast(kLitInt, "1")), Sast(kLitInt, "2")));
//}
//END_MTEST(LLParser_PrimaryExpressions, UnaryMinusInExpr);
//
//TES(LLParser_PrimaryExpressions, UnaryMinusInExprWithSubtraction) {
//  using namespace cnd;
//  using namespace test_util;
//  using enum eAst;
//  test_util::TestLLParse(
//      "-1 - -1", trtools::parser::ParsePrimaryExpr, "Unary Minus in expression -1 - -1;",
//      Sast(kSub, "-1--1", Sast(kUnaryMinus, "-1", Sast(kLitInt, "1")), Sast(kUnaryMinus, "-1", Sast(kLitInt, "1"))));
//}
//END_MTEST(LLParser_PrimaryExpressions, UnaryMinusInExprWithSubtraction);
//
//TES(LLParser_PrimaryExpressions, UnaryPlusSingleOperand) {
//  using namespace cnd;
//  using namespace test_util;
//  using enum eAst;
//  test_util::TestLLParse("+1", trtools::parser::ParsePrimaryExpr, "Unary Plus",
//                         Sast(kUnaryPlus, "+1", Sast(kLitInt, "1")));
//}
//END_MTEST(LLParser_PrimaryExpressions, UnaryPlusSingleOperand);
//
//TES(LLParser_PrimaryExpressions, UnaryPlusInExpr) {
//  using namespace cnd;
//  using namespace test_util;
//  using enum eAst;
//  test_util::TestLLParse("+1+2", trtools::parser::ParsePrimaryExpr, "Unary Plus in expression",
//                         Sast(kAdd, "+1+2", Sast(kUnaryPlus, "+1", Sast(kLitInt, "1")), Sast(kLitInt, "2")));
//}
//END_MTEST(LLParser_PrimaryExpressions, UnaryPlusInExpr);
//
//TES(LLParser_PrimaryExpressions, UnaryPlusInExprWithSubtraction) {
//  using namespace cnd;
//  using namespace test_util;
//  using enum eAst;
//  test_util::TestLLParse(
//      "+1 + +1", trtools::parser::ParsePrimaryExpr, "Unary Plus in expression +1 + +1;",
//      Sast(kSub, "+1++1", Sast(kUnaryPlus, "+1", Sast(kLitInt, "1")), Sast(kUnaryPlus, "+1", Sast(kLitInt, "1"))));
//}
//END_MTEST(LLParser_PrimaryExpressions, UnaryPlusInExprWithSubtraction);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Test LL Parser Methods. */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(Test_ParserBasics, TestCase_PrimaryStatement) {
  using namespace cnd;
  using namespace cnd::trtools;
  using namespace test_util;
  using enum cnd::eAst;
  // test a primary expr statement starting with a subexpression.
  TestLLParserMethod("(1+2)*a;", parser::ParsePrimaryStatement, "Primary Statement");
}

///////////////////////////////////////////////////////////
/* Declarations. */
///////////////////////////////////////////////////////////

TEST(Test_ParserBasics, TestCase_VariableDeclarationNoTypeNoAssignNoMod) {
  using namespace cnd;
  using namespace cnd::trtools;
  using namespace test_util;
  using enum cnd::eAst;
  TestLLParserMethod("def@Foo;", parser::ParseVariableDecl, "VariableDeclarationNoTypeNoAssignNoMod");
}

TEST(Test_ParserBasics, TestCase_VariableDeclarationNoTypeNoAssign) {
  using namespace cnd;
  using namespace cnd::trtools;
  using namespace test_util;
  using enum cnd::eAst;

  TestLLParserMethod("const def@Foo;", parser::ParseVariableDecl, "VariableDeclarationNoTypeNoAssign");
}

TEST(Test_ParserBasics, TestCase_VariableDeclarationNoAssign) {
  using namespace cnd;
  using namespace cnd::trtools;
  using namespace test_util;
  using enum cnd::eAst;
  TestLLParserMethod("const def str@Foo;", parser::ParseVariableDecl, "VariableDeclarationNoAssign");
}

TEST(Test_ParserBasics, TestCase_VariableDefinition) {
  using namespace cnd;
  using namespace cnd::trtools::parser;
  using namespace test_util;
  using enum cnd::eAst;

  TestLLParserMethod("const def str@Foo: 42;", ParseVariableDecl, "VariableDefinition");
}

TEST(Test_ParserBasics, TestCase_TypeAlias) {
  using namespace cnd;
  using namespace cnd::trtools::parser;
  using namespace test_util;
  using enum cnd::eAst;

  TestLLParserMethod("using @MyInteger: int;", ParseUsingDecl, "TypeAlias");
}

TEST(Test_ParserBasics, TestCase_LibraryNamespaceInclusion) {
  using namespace cnd;
  using namespace cnd::trtools::parser;
  using namespace test_util;
  using enum cnd::eAst;

  TestLLParserMethod("using lib my_math_lib;", ParseUsingDecl, "LibraryNamespaceInclusion");
}

TEST(Test_ParserBasics, TestCase_NamespaceInclusion) {
  using namespace cnd;
  using namespace cnd::trtools::parser;
  using namespace test_util;
  using enum cnd::eAst;

  TestLLParserMethod("using namespace my_ns;", ParseUsingDecl, "NamespaceInclusion");
}

TEST(Test_ParserBasics, TestCase_ObjectInclusion) {
  using namespace cnd;
  using namespace cnd::trtools::parser;
  using namespace test_util;
  using enum cnd::eAst;

  TestLLParserMethod("using my_ns::Foo;", ParseUsingDecl, "ObjectInclusion");
}

TEST(Test_ParserBasics, TestCase_TypeInclusion) {
  using namespace cnd;
  using namespace cnd::trtools::parser;
  using namespace test_util;
  using enum cnd::eAst;

  TestLLParserMethod("using@MyFooType: my_ns::Foo;", ParseUsingDecl, "TypeInclusion");
}

TEST(Test_ParserBasics, TestCase_ObjectInclusionFromLibrary) {
  using namespace cnd;
  using namespace cnd::trtools::parser;
  using namespace test_util;
  using enum cnd::eAst;

  TestLLParserMethod("using lib my_math_lib::add;", ParseUsingDecl, "ObjectInclusionFromLibrary");
}

TEST(Test_ParserBasics, TestCase_TypeInclusionFromLibrary) {
  using namespace cnd;
  using namespace cnd::trtools::parser;
  using namespace test_util;
  using enum cnd::eAst;

  TestLLParserMethod("using @MyAddMethodImpl: lib my_math_lib::add;", ParseUsingDecl, "TypeInclusionFromLibrary");
}

TEST(Test_ParserBasics, TestCase_TypeImportDeclaration) {
  using namespace cnd;
  using namespace cnd::trtools::parser;
  using namespace test_util;
  using enum cnd::eAst;

  TestLLParserMethod("import foo;", ParseImportDecl, "ImportDeclaration");
}

TEST(Test_ParserBasics, TestCase_MethodDeclImplicitVoidArgNoRet) {
  using namespace cnd;
  using namespace cnd::trtools::parser;
  using namespace test_util;
  using enum cnd::eAst;

  TestLLParserMethod("fn@add;", ParseMethodDecl, "MethodDeclImplicitVoidArgNoRet");
}

TEST(Test_ParserBasics, TestCase_MethodDeclImplicitVoidArgNoRet2) {
  using namespace cnd;
  using namespace cnd::trtools::parser;
  using namespace test_util;
  using enum cnd::eAst;

  TestLLParserMethod("fn@add();", ParseMethodDecl, "MethodDeclImplicitVoidArgNoRet");
}

TEST(Test_ParserBasics, TestCase_MethodDeclImplicitVoidArgAnyRet) {
  using namespace cnd;
  using namespace cnd::trtools::parser;
  using namespace test_util;
  using enum cnd::eAst;

  TestLLParserMethod("fn@add>;", ParseMethodDecl, "TestCaseMethodDeclImplicitVoidArgAnyRet");
}

TEST(Test_ParserBasics, TestCase_MethodDeclImplicitVoidArgAnyRet2) {
  using namespace cnd;
  using namespace cnd::trtools::parser;
  using namespace test_util;
  using enum cnd::eAst;

  TestLLParserMethod("fn@add()>;", ParseMethodDecl, "TestCaseMethodDeclImplicitVoidArgAnyRet2");
}

TEST(Test_ParserBasics, TestCase_MethodDeclArgNoRet) {
  using namespace cnd;
  using namespace cnd::trtools::parser;
  using namespace test_util;
  using enum cnd::eAst;

  TestLLParserMethod("fn@add(a,b);", ParseMethodDecl, "TestCaseMethodDeclArgNoRet");
}

TEST(Test_ParserBasics, TestCase_MethodDeclArgAnyRet) {
  using namespace cnd;
  using namespace cnd::trtools::parser;
  using namespace test_util;
  using enum cnd::eAst;

  TestLLParserMethod("fn@add(a,b)>;", ParseMethodDecl, "TestCaseMethodDeclArgAnyRet");
}

TEST(Test_ParserBasics, TestCase_MethodDeclArgIdentifiedAnyRet) {
  using namespace cnd;
  using namespace cnd::trtools::parser;
  using namespace test_util;
  using enum cnd::eAst;

  TestLLParserMethod("fn@add(@a,@b)>;", ParseMethodDecl, "TestCaseMethodDeclArgAnyRet");
}

TEST(Test_ParserBasics, TestCase_MethodDeclArgsTypedRet) {
  using namespace cnd;
  using namespace cnd::trtools::parser;
  using namespace test_util;
  using enum cnd::eAst;

  TestLLParserMethod("fn@add(@a,@b)>int;", ParseMethodDecl, "TestCaseMethodDeclArgsTypedRet");
}

TEST(Test_ParserBasics, TestCase_MethodDeclTypedArgsTypedRet) {
  using namespace cnd;
  using namespace cnd::trtools::parser;
  using namespace test_util;
  using enum cnd::eAst;

  TestLLParserMethod("fn@add(int @a,int @b)>int;", ParseMethodDecl, "TestCaseMethodDeclTypedArgsTypedRet");
}

TEST(Test_ParserBasics, TestCase_MethodDeclTypedArgsTypedRetWithModifiers) {
  using namespace cnd;
  using namespace cnd::trtools::parser;
  using namespace test_util;
  using enum cnd::eAst;

  TestLLParserMethod("fn@add(const int @a,const int @b)>const int;", ParseMethodDecl,
                     "TestCaseMethodDeclTypedArgsTypedRet");
}

TEST(Test_ParserBasics, TestCase_ClassDecl) {
  using namespace cnd;
  using namespace cnd::trtools::parser;
  using namespace test_util;
  using enum cnd::eAst;

  TestLLParserMethod("class@Husky;", ParseClassDecl, "TestCaseMethodDeclTypedArgsTypedRet");
}

TEST(Test_ParserBasics, TestCase_ClassDeclWithMod) {
  using namespace cnd;
  using namespace cnd::trtools::parser;
  using namespace test_util;
  using enum cnd::eAst;

  TestLLParserMethod("const static class@Husky;", ParseClassDecl, "TestCaseClassDeclWithMod");
}

TEST(Test_ParserBasics, TestCase_LibWithMod) {
  using namespace cnd;
  using namespace cnd::trtools::parser;
  using namespace test_util;
  using enum cnd::eAst;

  TestLLParserMethod("const static lib@MathLib;", ParseLibDecl, "TestCaseCLibDeclWithMod");
}

TEST(Test_ParserBasics, TestCase_LibWithModAndDefinition) {
  using namespace cnd;
  using namespace cnd::trtools::parser;
  using namespace test_util;
  using enum cnd::eAst;

  TestLLParserMethod("const static lib@MathLib:{const def str@Foo: 42;using @MyInteger: int;};", ParseLibDecl,
                     "TestCaseCLibDeclWithMod");
}

TEST(Test_ParserBasics, TestCase_ClassWithModAndDefinition) {
  using namespace cnd;
  using namespace cnd::trtools::parser;
  using namespace test_util;
  using enum cnd::eAst;

  TestLLParserMethod("const static class@Husky:{const def str@Foo: 42;using @MyInteger: int;};", ParseClassDecl,
                     "TestCaseClassDeclWithMod");
}

TEST(Test_ParserBasics, TestCase_MethodDefinition) {
  using namespace cnd;
  using namespace cnd::trtools::parser;
  using namespace test_util;
  using enum cnd::eAst;

  TestLLParserMethod("fn@add(const int @a,const int @b)>const int:{a+b;};", ParseMethodDecl,
                     "TestCaseMethodDefinition");
}

TEST(Test_ParserBasics, TestCase_MainDefinition) {
  using namespace cnd;
  using namespace cnd::trtools::parser;
  using namespace test_util;
  using enum cnd::eAst;

  TestLLParserMethod("main(a,b):{a+b;};", ParseMainDecl, "TestCaseMainDefinition");
}

// Pragmatic statements appears at program top level, or in a library.
// This tests that the ParsePragmaticStmt can handle all statement types (none were missed.).
TEST(Test_ParserBasics, TestCase_PragmaticDeclarations) {
  using namespace cnd;
  using namespace cnd::trtools::parser;
  using namespace test_util;
  using enum cnd::eAst;
  TestLLParserMethod("const def str@Foo: 42;", ParsePragmaticStmt, "VariableDefinition");
  TestLLParserMethod("using @MyInteger: int;", ParsePragmaticStmt, "TypeAlias");
  TestLLParserMethod("using lib my_math_lib;", ParsePragmaticStmt, "LibraryNamespaceInclusion");
  TestLLParserMethod("using namespace my_ns;", ParsePragmaticStmt, "NamespaceInclusion");
  TestLLParserMethod("using my_ns::Foo;", ParsePragmaticStmt, "ObjectInclusion");
  TestLLParserMethod("using@MyFooType: my_ns::Foo;", ParsePragmaticStmt, "TypeInclusion");
  TestLLParserMethod("using lib my_math_lib::add;", ParsePragmaticStmt, "ObjectInclusionFromLibrary");
  TestLLParserMethod("using @MyAddMethodImpl: lib my_math_lib::add;", ParsePragmaticStmt, "TypeInclusionFromLibrary");
  TestLLParserMethod("const static lib@MathLib;", ParsePragmaticStmt, "PragmaticDeclLibrary");
  TestLLParserMethod("import foo;", ParsePragmaticStmt, "ImportDeclaration");
  TestLLParserMethod("fn@add;", ParsePragmaticStmt, "MethodDeclImplicitVoidArgNoRet");
  TestLLParserMethod("fn@add();", ParsePragmaticStmt, "MethodDeclImplicitVoidArgNoRet");
  TestLLParserMethod("fn@add>;", ParsePragmaticStmt, "TestCaseMethodDeclImplicitVoidArgAnyRet");
  TestLLParserMethod("fn@add()>;", ParsePragmaticStmt, "TestCaseMethodDeclImplicitVoidArgAnyRet2");
  TestLLParserMethod("fn@add(a,b)>;", ParsePragmaticStmt, "TestCaseMethodDeclArgAnyRet");
  TestLLParserMethod("fn@add(@a,@b)>int;", ParsePragmaticStmt, "TestCaseMethodDeclArgsTypedRet");
  TestLLParserMethod("fn@add(int @a,int @b)>int;", ParsePragmaticStmt, "TestCaseMethodDeclTypedArgsTypedRet");
  TestLLParserMethod("fn@add(const int @a,const int @b)>const int;", ParsePragmaticStmt,
                     "TestCaseMethodDeclTypedArgsTypedRet");
  TestLLParserMethod("class@Husky;", ParsePragmaticStmt, "TestCaseMethodDeclTypedArgsTypedRet");
  TestLLParserMethod("const static class@Husky;", ParsePragmaticStmt, "TestCaseClassDeclWithMod");
  TestLLParserMethod("const static lib@MathLib;", ParsePragmaticStmt, "TestCaseCLibDeclWithMod");
}

TEST(Test_ParserBasics, TestCase_ParseProgramWithDeclarations) {
  using namespace cnd;
  using namespace cnd::trtools::parser;
  using namespace test_util;
  using enum cnd::eAst;
  TestLLParserMethod(
      "import foo;"
      "const static lib@MathLib;"
      "using @MyAddMethodImpl: lib MathLib::add;"
      "const def str@Foo: 42;"
      "fn@add(const int @a,const int @b)>const int;"
      "const static class @Husky;",
      ParseProgram, "TestCaseParseProgramWithDeclrations");
}

///////////////////////////////////////////////////////////
/* Control Flow Statements. */
///////////////////////////////////////////////////////////

TEST(Test_ParserBasics, TestCase_ReturnStatement) {
  using namespace cnd;
  using namespace cnd::trtools::parser;
  using namespace test_util;
  using enum cnd::eAst;
  TestLLParserMethod("return a + b;", ParseReturnStmt, "TestCaseReturnStatement");
}

TEST(Test_ParserBasics, TestCase_IfStatement) {
  using namespace cnd;
  using namespace cnd::trtools::parser;
  using namespace test_util;
  using enum cnd::eAst;
  TestLLParserMethod("if(a){ a + b; };", ParseIfDecl, "TestCaseIfStatement");
}

TEST(Test_ParserBasics, TestCase_IfElseStatement) {
  using namespace cnd;
  using namespace cnd::trtools::parser;
  using namespace test_util;
  using enum cnd::eAst;
  TestLLParserMethod("if(a){ a + b; }else{a;};", ParseIfDecl, "TestCaseIfElseStatement");
}

TEST(Test_ParserBasics, TestCase_IfElifStatement) {
  using namespace cnd;
  using namespace cnd::trtools::parser;
  using namespace test_util;
  using enum cnd::eAst;
  TestLLParserMethod("if(a){ a + b; }elif(b){a;};", ParseIfDecl, "TestCaseIfElifStatement");
}

TEST(Test_ParserBasics, TestCase_IfElifElseStatement) {
  using namespace cnd;
  using namespace cnd::trtools::parser;
  using namespace test_util;
  using enum cnd::eAst;
  TestLLParserMethod("if(a){ a + b; }elif(b){a;}else{b;};", ParseIfDecl, "TestCaseIfElifElseStatement");
}

///////////////////////////////////////////////////////////
/* Loop Statements. */
///////////////////////////////////////////////////////////

TEST(Test_ParserBasics, TestCase_WhileStatement) {
  using namespace cnd;
  using namespace cnd::trtools::parser;
  using namespace test_util;
  using enum cnd::eAst;
  TestLLParserMethod("while(a){ a + b; };", ParseWhileDecl, "TestCaseWhileStatement");
}

TEST(Test_ParserBasics, TestCase_ForStatement) {
  using namespace cnd;
  using namespace cnd::trtools::parser;
  using namespace test_util;
  using enum cnd::eAst;
  TestLLParserMethod("for(def@a:0;a!=end;a++){ a + b; };", ParseForDecl, "TestCaseForStatement");
}

///////////////////////////////////////////////////////////
/* Uncategorized. */
///////////////////////////////////////////////////////////

// Animals Example Program.
TEST(Test_ParserBasics, TestCase_AnimalsExampleProgram) {
  using namespace cnd;
  using namespace cnd::trtools::parser;
  using namespace test_util;
  using enum cnd::eAst;
  TestLLParserMethod(
      "main(): {class @Horse : {fn @makeSound() : {return 'Neigh!';};};class "
      "@Cow "
      ": {  fn @makeSound() : { return 'Moo!'; };}"
      ";class @Wolf : {fn @makeSound() : { return 'Oooo!'; };};class @Cricket "
      ": "
      "{fn @makeSound() : { return 'Chirp!'; };};"
      "def @farm_animals : list{Horse(), Cow()};def @all_animals : "
      "farm_animals + list{Wolf(), Cricket()};"
      "fn @makeAnimalSounds(list @animal_list) : {def str @sounds;for (def "
      "@idx : 0; idx < animal_list.Size();"
      "idx++) {sounds += animal_list[idx].makeSound();};return sounds;};return "
      "makeAnimalSounds(all_animals);};",
      ParseProgram, "TestCaseAnimalsExampleProgram");
}

TEST(Test_ParserBasics, TestCase_BasicEnumDefinition) {
  using namespace cnd;
  using namespace cnd::trtools::parser;
  using namespace test_util;
  using enum cnd::eAst;
  TestLLParserMethod("enum @eResult:{@Good;@Bad;}", ParseEnumDecl, "TestCaseEnumDecl");
}

TEST(Test_ParserBasics, TestCase_EnumDefinitionWithValues) {
  using namespace cnd;
  using namespace cnd::trtools::parser;
  using namespace test_util;
  using enum cnd::eAst;
  TestLLParserMethod("enum @eResult:{@Good:0;@Bad:1;}", ParseEnumDecl, "TestCaseEnumDecl");
}

TEST(Test_ParserBasics, TestCase_EnumDefinitionWithPositionalAssociatedValue) {
  using namespace cnd;
  using namespace cnd::trtools::parser;
  using namespace test_util;
  using enum cnd::eAst;
  TestLLParserMethod("enum @eResult:str:{@Good:1:\"Good\":;@Bad:\"Bad\";}", ParseEnumDecl, "TestCaseEnumDecl");
}

TEST(Test_ParserBasics, TestCase_EnumDefinitionWithNamedAssociatedValue) {
  using namespace cnd;
  using namespace cnd::trtools::parser;
  using namespace test_util;
  using enum cnd::eAst;
  TestLLParserMethod("enum @eResult:str @EnumStr:{@Good:1:\"Good\":;@Bad:\"Bad\";}", ParseEnumDecl, "TestCaseEnumDecl");
}

TEST(Test_ParserBasics, TestCase_EnumDefinitionWithTaggedEntries) {
  using namespace cnd;
  using namespace cnd::trtools::parser;
  using namespace test_util;
  using enum cnd::eAst;
  TestLLParserMethod("enum @eResult:str @EnumStr:{@Good:1:\"Good\";tag@ErrorState:@Bad:1:\"Bad\";}", ParseEnumDecl,
                     "TestCaseEnumDecl");
}

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
