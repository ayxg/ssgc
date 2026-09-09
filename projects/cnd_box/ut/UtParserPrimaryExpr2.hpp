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
/// @brief Tests parsing primary expressions in isolation.
///
/// This is arguably the hardest part of the parser to develop. These tests provide some sanity and
/// prevent regression.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup cnd_unit_test
/// @{
#pragma once
// clang-format off
#include "minitest.hpp"
#include "compiler/compiler.hpp"
//#include "frontend/parser.hpp"
//#include "ParserTestUtils.hpp"
// clang-format on

namespace cnd_unit_test::frontend::parser {
// Alias synthesized ast and ast enum to reduce noise for 'expected' ast construction.
// using Sast = cnd::Sast;
// using enum cnd::eAst;
//
//// Method used for unit testing in this header
// using cnd_unit_test::frontend::test_util::TestParsingMethod;
// using enum cnd_unit_test::frontend::test_util::eTestParsingMethod;
//
//// The namespace being tested from "trtools/Parser.hpp" header.
using namespace ssgc::frontend;

template <typename ResultT>
void TestOperandParsingMethod(std::string_view data,
                              ParseResult<ResultT> (*method)(const Token&,
                                                             const ssgc::SourceManager&),
                              typename ResultT::ValueType expected_value) {
  ssgc::Compiler comp{};
  const ssgc::SourceFile* src = comp.generateSource("_ut_parser_", data);
  const std::vector<Token>* tokens = comp.tokenize(src->id);
  auto result = method(tokens->at(0), comp.context.sources);
  EXPECT_EQ(result.node.data, expected_value);
}


bool CompareNode(const NodePrimaryExpr& lhs, const NodePrimaryExpr& rhs) {
  auto xGetTypename = [](const NodePrimaryExpr& node) {
    return std::visit(
        [](const auto& node_alternative) {
          return typeid(std::decay_t<decltype(node_alternative)>).name();
        },
        node);
  };

  bool type_equal = EXPECT_EQ_LOG(
      lhs.index(), rhs.index(),
      std::format("Node type {} not equal {}.", xGetTypename(lhs), xGetTypename(rhs)));

  if (!type_equal) {
    return false;
  }

  bool is_equal = std::visit(
      [&rhs](const auto& lhsv) {
        using LhsT = std::decay_t<decltype(lhsv)>;
        auto& rhsv = std::get<LhsT>(rhs);

        if constexpr (requires { lhsv.data; }) {
          return EXPECT_EQ(lhsv.data, rhsv.data);
        } else if constexpr (std::is_base_of<NodeBaseBinaryOperator, LhsT>::value) {
          bool lhs_equal = CompareNode(*lhsv.lhs, *rhsv.lhs);
          bool rhs_equal = CompareNode(*lhsv.rhs, *rhsv.rhs);

          if (!lhs_equal || !rhs_equal) {
            return false;
          } else {
            return true;
          }
        } else if constexpr (std::is_base_of<NodeBaseUnaryOperator, LhsT>::value) {
          return CompareNode(*lhsv.operand, *rhsv.operand);
        } else {
          return true;
        }
      },
      lhs);

  return is_equal;
}

void TestOperatorParsingMethod(std::string_view data,
                               LLParseResult<NodePrimaryExpr> (*method)(TokenCursor,
                                                                      ParsingContext&),
                               NodePrimaryExpr expected_node) {
  ssgc::Compiler comp{};
  const ssgc::SourceFile* src = comp.generateSource("_ut_parser_", data);
  const std::vector<Token>* tokens = comp.tokenize(src->id);
  ParsingContext parsing_context = comp.getParsingContext();
  auto result = method(TokenCursor{0, tokens->data(), tokens->data() + tokens->size()}, parsing_context);
  CompareNode(result.node, expected_node);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* UtParserPrimaryExprOperands : Validate parsing singular terminals. */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(UtParserPrimaryExprOperands, LiteralBool) {
  TestOperandParsingMethod("1b", parseBool, true);
  TestOperandParsingMethod("0b", parseBool, false);
  TestOperandParsingMethod("true", parseBool, true);
  TestOperandParsingMethod("false", parseBool, false);
}

TEST(UtParserPrimaryExprOperands, LiteralU8) {
  for (int i = 0; i <= 127; i++) {
    TestOperandParsingMethod(std::to_string(i) + "B", parseU8, i);
    TestOperandParsingMethod(std::to_string(i) + "u8", parseU8, i);
  }
}

TEST(UtParserPrimaryExprOperands, LiteralU16) { TestOperandParsingMethod("123u16", parseU16, 123); }

TEST(UtParserPrimaryExprOperands, LiteralU32) {
  TestOperandParsingMethod("123u", parseU32, 123);
  TestOperandParsingMethod("123u32", parseU32, 123);
}

TEST(UtParserPrimaryExprOperands, LiteralU64) {
  TestOperandParsingMethod("123U", parseU64, 123);
  TestOperandParsingMethod("123u64", parseU64, 123);
}

TEST(UtParserPrimaryExprOperands, LiteralI8) {
  for (int i = 0; i <= 127; i++) {
    TestOperandParsingMethod(std::to_string(i) + "c", parseI8, i);
    TestOperandParsingMethod(std::to_string(i) + "i8", parseI8, i);
  }
}

TEST(UtParserPrimaryExprOperands, LiteralI16) { TestOperandParsingMethod("123i16", parseI16, 123); }

TEST(UtParserPrimaryExprOperands, LiteralI32) {
  TestOperandParsingMethod("123", parseI32, 123);
  TestOperandParsingMethod("123i32", parseI32, 123);
}

TEST(UtParserPrimaryExprOperands, LiteralI64) {
  TestOperandParsingMethod("123L", parseI64, 123);
  TestOperandParsingMethod("123i64", parseI64, 123);
}

TEST(UtParserPrimaryExprOperands, LiteralF32) {
  TestOperandParsingMethod("123.f", parseF32, 123);
  TestOperandParsingMethod("123.456f", parseF32, 123.456f);
}

TEST(UtParserPrimaryExprOperands, LiteralF64) {
  TestOperandParsingMethod("123.", parseF64, 123);
  TestOperandParsingMethod("123.456", parseF64, 123.456);
}

TEST(UtParserPrimaryExprOperands, LiteralCStr) {
  ssgc::Compiler comp{};
  const ssgc::SourceFile* src = comp.generateSource("_ut_parser_", "\"string literal\"");
  const std::vector<Token>* tokens = comp.tokenize(src->id);
  auto result = parseCStr(tokens->at(0), comp.context.sources, comp.context.interner);
  EXPECT_EQ(*comp.context.interner.get(result.node.data), std::string_view("string literal"));

  src = comp.generateSource("_ut_parser_", "\"\\\\\"");
  tokens = comp.tokenize(src->id);
  auto result2 = parseCStr(tokens->at(0), comp.context.sources, comp.context.interner);
  EXPECT_EQ(*comp.context.interner.get(result2.node.data), std::string_view("\\"));
}

// TEST(UtParserPrimaryExprOperands, NoneKeyword) {
//    TestParsingMethod("none", ParseOperand, Sast{kKwNone, "none"});
//  }

// TEST(UtParserPrimaryExprOperands, LiteralReal) {
//   TestOperandParsingMethod("123.r", eToken::kLitReal, 1, 6);
//   TestOperandParsingMethod("123.456r", eToken::kLitReal, 1, 9);
// }

// TEST(UtParserPrimaryExprOperands, RealNumber) {
//   TestParsingMethod("1.1r", ParseOperand, Sast{kLitReal, "1.1r"});
// }
//
// TEST(UtParserPrimaryExprOperands, CString) {
//   TestParsingMethod("\"string literal\"", ParseOperand, Sast{kLitCstr, "\"string literal\""});
//   TestParsingMethod("\"\\\\\"", ParseOperand, Sast{kLitCstr, "\"\\\\\""});
// }
//
// TEST(UtParserPrimaryExprOperands, Alnumus) {
//   TestParsingMethod("alnumus", ParseOperand, Sast{kIdent, "alnumus"});
// }
//
// TEST(UtParserPrimaryExprOperands, ByteLiteral) {
//   TestParsingMethod("1c", ParseOperand, Sast{kLitI8, "1c"});
// }
//
// TEST(UtParserPrimaryExprOperands, BoolLiteral) {
//   TestParsingMethod("1b", ParseOperand, Sast{kLitU1, "1b"});
// }
//
// TEST(UtParserPrimaryExprOperands, UnsignedLiteral) {
//   TestParsingMethod("1u", ParseOperand, Sast{kLitU32, "1u"});
// }
//
// TEST(UtParserPrimaryExprOperands, NoneKeyword) {
//   TestParsingMethod("none", ParseOperand, Sast{kKwNone, "none"});
// }
//
// TEST(UtParserPrimaryExprOperands, AllSingularOperands) {
//   // This test confirms proper advancement of the iterator after parsing an operand.
//   using namespace cnd::frontend::parser;
//   using namespace cnd::frontend;
//   cnd::Vec<cnd::Tk> src =
//       sanitizeTokens(tokenizeSourceCode("1 1.1 \"string literal\" \"\\\\\" alnumus 1c 1b 1u none
//       ")
//                          .value_or(cnd::Vec<cnd::Tk>{}));
//   std::span<const cnd::Tk> src_span = std::span{src.data(), src.size()};
//
//   // Operands.
//   std::vector<std::pair<cnd::eAst, std::string>> expected_result = {
//       std::make_pair(kLitI32, "1"),
//       std::make_pair(kLitF64, "1.1"),
//       std::make_pair(kLitCstr, "\"string literal\""),
//       std::make_pair(kLitCstr, "\"\\\\\""),
//       std::make_pair(kIdent, "alnumus"),
//       std::make_pair(kLitI8, "1c"),
//       std::make_pair(kLitU1, "1b"),
//       std::make_pair(kLitU32, "1u"),
//       std::make_pair(kKwNone, "none"),
//   };
//
//   auto num_lit = ParseOperand({src_span.begin(), src_span.end()});
//   ASSERT_TRUE(num_lit.has_value());
//   EXPECT_EQ(num_lit.value().ast.type, expected_result[0].first);
//   EXPECT_EQ(num_lit.value().ast.GetLiteral(), expected_result[0].second);
//
//   auto real_lit = ParseOperand(num_lit.value().head);
//   ASSERT_TRUE(real_lit.has_value());
//   EXPECT_EQ(real_lit.value().ast.type, expected_result[1].first);
//   EXPECT_EQ(real_lit.value().ast.GetLiteral(), expected_result[1].second);
//
//   auto str_lit = ParseOperand(real_lit.value().head);
//   ASSERT_TRUE(str_lit.has_value());
//   EXPECT_EQ(str_lit.value().ast.type, expected_result[2].first);
//   EXPECT_EQ(str_lit.value().ast.GetLiteral(), expected_result[2].second);
//
//   auto str_lit2 = ParseOperand(str_lit.value().head);
//   ASSERT_TRUE(str_lit2.has_value());
//   EXPECT_EQ(str_lit2.value().ast.type, expected_result[3].first);
//   EXPECT_EQ(str_lit2.value().ast.GetLiteral(), expected_result[3].second);
//
//   auto alnumus_lit = ParseOperand(str_lit2.value().head);
//   ASSERT_TRUE(alnumus_lit.has_value());
//   EXPECT_EQ(alnumus_lit.value().ast.type, expected_result[4].first);
//   EXPECT_EQ(alnumus_lit.value().ast.GetLiteral(), expected_result[4].second);
//
//   auto byte_lit = ParseOperand(alnumus_lit.value().head);
//   ASSERT_TRUE(byte_lit.has_value());
//   EXPECT_EQ(byte_lit.value().ast.type, expected_result[5].first);
//   EXPECT_EQ(byte_lit.value().ast.GetLiteral(), expected_result[5].second);
//
//   auto bit_lit = ParseOperand(byte_lit.value().head);
//   ASSERT_TRUE(bit_lit.has_value());
//   EXPECT_EQ(bit_lit.value().ast.type, expected_result[6].first);
//   EXPECT_EQ(bit_lit.value().ast.GetLiteral(), expected_result[6].second);
//
//   auto unsigned_lit = ParseOperand(bit_lit.value().head);
//   ASSERT_TRUE(unsigned_lit.has_value());
//   EXPECT_EQ(unsigned_lit.value().ast.type, expected_result[7].first);
//   EXPECT_EQ(unsigned_lit.value().ast.GetLiteral(), expected_result[7].second);
//
//   auto none_lit = ParseOperand(unsigned_lit.value().head);
//   ASSERT_TRUE(none_lit.has_value());
//   EXPECT_EQ(none_lit.value().ast.type, expected_result[8].first);
//   EXPECT_EQ(none_lit.value().ast.GetLiteral(), expected_result[8].second);
// }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* UtParserPrimaryExprTopDown : Validate primary expressions by calling the top-most
 * ParsePrimaryExpr method.        */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// TEST(UtParserPrimaryExprTopDown, WholeNumber) {
//   TestParsingMethod("1", ParsePrimaryExpr, Sast(kLitI32, "1"));
// }
//
// TEST(UtParserPrimaryExprTopDown, RealNumber) {
//   TestParsingMethod("1.1", ParsePrimaryExpr, Sast(kLitF64, "1.1"));
// }
//
// TEST(UtParserPrimaryExprTopDown, CString) {
//   TestParsingMethod("\"string literal\"", ParsePrimaryExpr, Sast(kLitCstr, "\"string
//   literal\""));
// }
//
// TEST(UtParserPrimaryExprTopDown, CStringEscaped) {
//   TestParsingMethod("\"\\\\\"", ParsePrimaryExpr, Sast(kLitCstr, "\"\\\\\""));
// }
//
// TEST(UtParserPrimaryExprTopDown, Alnumus) {
//   TestParsingMethod("alnumus", ParsePrimaryExpr, Sast(kIdent, "alnumus"));
// }
//
// TEST(UtParserPrimaryExprTopDown, ByteLiteral) {
//   TestParsingMethod("1c", ParsePrimaryExpr, Sast(kLitI8, "1c"));
// }
//
// TEST(UtParserPrimaryExprTopDown, BoolLiteral) {
//   TestParsingMethod("1b", ParsePrimaryExpr, Sast(kLitU1, "1b"));
// }
//
// TEST(UtParserPrimaryExprTopDown, UnsignedLiteral) {
//   TestParsingMethod("1u", ParsePrimaryExpr, Sast(kLitU32, "1u"));
// }
//
// TEST(UtParserPrimaryExprTopDown, NoneKeyword) {
//   TestParsingMethod("none", ParsePrimaryExpr, Sast(kKwNone, "none"));
// }
//
// TEST(UtParserPrimaryExprTopDown, OperandInParenthesis) {
//   TestParsingMethod("(1)", ParsePrimaryExpr, Sast(kSubexpression, "(1)", Sast(kLitI32, "1")));
// }
//
// TEST(UtParserPrimaryExprTopDown, OperandInSquareBrackets) {
//   TestParsingMethod("[1]", ParsePrimaryExpr, Sast(kSquareSubexpr, "[1]", Sast(kLitI32, "1")));
// }
//
// TEST(UtParserPrimaryExprTopDown, OperandInCurlyBraces) {
//   TestParsingMethod("{1}", ParsePrimaryExpr, Sast(kCurlySubexpr, "{1}", Sast{kLitI32, "1"}));
// }
//
// TEST(UtParserPrimaryExprTopDown, BinarySum) {
//   TestParsingMethod("1 + 1", ParsePrimaryExpr,
//                     Sast(kAdd, "1+1", Sast(kLitI32, "1"), Sast(kLitI32, "1")));
// }
//
// TEST(UtParserPrimaryExprTopDown, BinarySumThenTerm) {
//   TestParsingMethod("1 + 2 * 3", ParsePrimaryExpr,
//                     Sast(kAdd, "1+2*3", Sast(kLitI32, "1"),
//                          Sast(kMul, "2*3", Sast(kLitI32, "2"), Sast(kLitI32, "3"))));
// }
//
// TEST(UtParserPrimaryExprTopDown, BinaryTermThenSum) {
//   TestParsingMethod("1 * 2 + 3", ParsePrimaryExpr,
//                     Sast(kAdd, "1*2+3", Sast(kMul, "1*2", Sast(kLitI32, "1"), Sast(kLitI32,
//                     "2")),
//                          Sast(kLitI32, "3")));
// }
//
// TEST(UtParserPrimaryExprTopDown, BinarySumIsLeftAssociative) {
//   TestParsingMethod("1 + 2 - 3", ParsePrimaryExpr,
//                     Sast(kSub, "1+2-3", Sast(kAdd, "1+2", Sast(kLitI32, "1"), Sast(kLitI32,
//                     "2")),
//                          Sast(kLitI32, "3")));
// }
//
// TEST(UtParserPrimaryExprTopDown, BinaryMemberAccessIsLeftAssociative) {
//   TestParsingMethod("a.b.c.d", ParsePrimaryExpr,
//                     Sast(kMemberAccess, "a.b.c.d",
//                          Sast(kMemberAccess, "a.b.c",
//                               Sast(kMemberAccess, "a.b", Sast(kIdent, "a"), Sast(kIdent, "b")),
//                               Sast(kIdent, "c")),
//                          Sast(kIdent, "d")));
// }
//
// TEST(UtParserPrimaryExprTopDown, ParenIsResolvedFirst) {
//   TestParsingMethod(
//       "(1+2)*3", ParsePrimaryExpr,
//       Sast(kMul, "(1+2)*3",
//            Sast(kSubexpression, "(1+2)", Sast(kAdd, "1+2", Sast(kLitI32, "1"), Sast(kLitI32,
//            "2"))), Sast(kLitI32, "3")));
// }
//
// TEST(UtParserPrimaryExprTopDown, UnaryPrefix) {
//   TestParsingMethod("!a", ParsePrimaryExpr, Sast(kNot, "!a", Sast(kIdent, "a")));
// }
//
// TEST(UtParserPrimaryExprTopDown, RepeatedUnaryPrefix) {
//   TestParsingMethod("!!a", ParsePrimaryExpr,
//                     Sast(kNot, "!!a", Sast(kNot, "!a", Sast(kIdent, "a"))));
// }
//
// TEST(UtParserPrimaryExprTopDown, AssignmentIsRightAssociative) {
//   // (a=(b=c))
//   TestParsingMethod("a=b=c", ParseAssignment,
//                     Sast(kAssign, "a=b=c", Sast(kIdent, "a"),
//                          Sast(kAssign, "b=c", Sast(kIdent, "b"), Sast(kIdent, "c"))));
//   // (a=(b=c))
//   TestParsingMethod("x=a=b=c", ParseAssignment,
//                     Sast(kAssign, "x=a=b=c", Sast(kIdent, "x"),
//                          Sast(kAssign, "a=b=c", Sast(kIdent, "a"),
//                               Sast(kAssign, "b=c", Sast(kIdent, "b"), Sast(kIdent, "c")))));
// }
//
// TEST(UtParserPrimaryExprTopDown, PrefixUnaryThenBinary) {
//   TestParsingMethod("!1+2", ParsePrimaryExpr,
//                     Sast(kAdd, "!1+2", Sast(kNot, "!1", Sast(kLitI32, "1")), Sast(kLitI32,
//                     "2")));
// }
//
// TEST(UtParserPrimaryExprTopDown, PrefixUnaryThenMemberAccess) {
//   TestParsingMethod(
//       "!a.b", ParsePrimaryExpr,
//       Sast(kNot, "!a.b", Sast(kMemberAccess, "a.b", Sast(kIdent, "a"), Sast(kIdent, "b"))));
// }
//
// TEST(UtParserPrimaryExprTopDown, PrefixUnaryAfterBinary) {
//   TestParsingMethod("1+!2", ParsePrimaryExpr,
//                     Sast(kAdd, "1+!2", Sast(kLitI32, "1"), Sast(kNot, "!2", Sast(kLitI32,
//                     "2"))));
// }
//
// TEST(UtParserPrimaryExprTopDown, EmptyFunctionCall) {
//   TestParsingMethod("a()", ParsePrimaryExpr,
//                     Sast(kFunctionCall, "a()", Sast(kIdent, "a"), Sast(kArguments, "()")));
// }
//
// TEST(UtParserPrimaryExprTopDown, PrefixUnaryThenFunctionCall) {
//   TestParsingMethod(
//       "!a()", ParsePrimaryExpr,
//       Sast(kNot, "!a()", Sast(kFunctionCall, "a()", Sast(kIdent, "a"), Sast(kArguments, "()"))));
// }
//
// TEST(UtParserPrimaryExprTopDown, RepeatedUnaryThenRepeatedFunctionCall) {
//   TestParsingMethod(
//       "!!a()()", ParsePrimaryExpr,
//       Sast(kNot, "!!a()()",
//            Sast(kNot, "!a()()",
//                 Sast(kFunctionCall, "a()()",
//                      Sast(kFunctionCall, "a()", Sast(kIdent, "a"), Sast(kArguments, "()")),
//                      Sast(kArguments, "()")))));
// }
//
// TEST(UtParserPrimaryExprTopDown, FunctionCallThenBinary) {
//   TestParsingMethod(
//       "a()+2", ParsePrimaryExpr,
//       Sast(kAdd, "a()+2", Sast(kFunctionCall, "a()", Sast(kIdent, "a"), Sast(kArguments, "()")),
//            Sast(kLitI32, "2")));
// }
//
// TEST(UtParserPrimaryExprTopDown, FunctionCallThenMemberAccess) {
//   TestParsingMethod("a().b", ParsePrimaryExpr,
//                     Sast(kMemberAccess, "a().b",
//                          Sast(kFunctionCall, "a()", Sast(kIdent, "a"), Sast(kArguments, "()")),
//                          Sast(kIdent, "b")));
// }
//
// TEST(UtParserPrimaryExprTopDown, MemberAccessThenFunctionCall) {
//   TestParsingMethod(
//       "a.b()", ParsePrimaryExpr,
//       Sast(kFunctionCall, "a.b()", Sast(kMemberAccess, "a.b", Sast(kIdent, "a"), Sast(kIdent,
//       "b")),
//            Sast(kArguments, "()")));
// }
//
// TEST(UtParserPrimaryExprTopDown, BinaryThenFunctionCall) {
//   TestParsingMethod("1+a()", ParsePrimaryExpr,
//                     Sast(kAdd, "1+a()", Sast(kLitI32, "1"),
//                          Sast(kFunctionCall, "a()", Sast(kIdent, "a"), Sast(kArguments, "()"))));
// }
//
// TEST(UtParserPrimaryExprTopDown, InteleavedMemberAccessAndFunctionCall) {
//   // a.b().c is parsed as (a.b()).c and not as a.(b().c) because of left-to-right associativity
//   of
//   // member access. Source: cppreference.com
//   TestParsingMethod("a.b().c.d", ParsePrimaryExpr,
//                     Sast(kMemberAccess, "a.b().c.d",
//                          Sast(kMemberAccess, "a.b().c",
//                               Sast(kFunctionCall, "a.b()",
//                                    Sast(kMemberAccess, "a.b", Sast(kIdent, "a"), Sast(kIdent,
//                                    "b")), Sast(kArguments, "()")),
//                               Sast(kIdent, "c")),
//                          Sast(kIdent, "d")));
// }
//
// TEST(UtParserPrimaryExprTopDown, ComplexExpr) {
//   TestParsingMethod(
//       "foo.bar()*1+1", ParsePrimaryExpr,
//       Sast(kAdd, "foo.bar()*1+1",
//            Sast(kMul, "foo.bar()*1",
//                 Sast(kFunctionCall, "foo.bar()",
//                      Sast(kMemberAccess, "foo.bar", Sast(kIdent, "foo"), Sast(kIdent, "bar")),
//                      Sast(kArguments, "()")),
//                 Sast(kLitI32, "1")),
//            Sast(kLitI32, "1")));
// }
//
// TEST(UtParserPrimaryExprTopDown, ComplexExpr2) {
//   TestParsingMethod(
//       "(foo.bar() + 1) * 1", ParsePrimaryExpr,
//       Sast(kMul, "(foo.bar()+1)*1",
//         Sast(kSubexpression, "(foo.bar()+1)",
//            Sast(kAdd, "foo.bar()+1",
//                 Sast(kFunctionCall, "foo.bar()",
//                      Sast(kMemberAccess, "foo.bar", Sast(kIdent, "foo"), Sast(kIdent, "bar")),
//                      Sast(kArguments, "()")),
//                 Sast(kLitI32, "1"))),
//            Sast(kLitI32, "1")));
// }
//
// TEST(UtParserPrimaryExprTopDown, AssignmentExpression) {
//   TestParsingMethod("foo = 1 + 2", ParsePrimaryExpr,
//                     Sast(kAssign, "foo=1+2", Sast(kIdent, "foo"),
//                          Sast(kAdd, "1+2", Sast(kLitI32, "1"), Sast(kLitI32, "2"))));
// }
//
// TEST(UtParserPrimaryExprTopDown, FunctionCallAfterBinaryLeftAssociative) {
//   TestParsingMethod(
//       "a+b+c+d()", ParsePrimaryExpr,
//       Sast(kAdd, "a+b+c+d()",
//            Sast(kAdd, "a+b+c", Sast(kAdd, "a+b", Sast{kIdent, "a"}, Sast{kIdent, "b"}),
//                 Sast{kIdent, "c"}),
//            Sast(kFunctionCall, "d()", Sast(kIdent, "d"), Sast(kArguments, "()"))));
// }
//
// TEST(UtParserPrimaryExprTopDown, FunctionCallWithArguments) {
//   TestParsingMethod(
//       "foo(a, b, c)", ParsePrimaryExpr,
//       Sast(kFunctionCall, "foo(a,b,c)", Sast(kIdent, "foo"),
//            Sast(kArguments, "(a,b,c)",
//                 Sast(kComma, "a,b,c", Sast(kComma, "a,b", Sast{kIdent, "a"}, Sast{kIdent, "b"}),
//                      Sast{kIdent, "c"}))));
// }
//
// TEST(UtParserPrimaryExprTopDown, IndexingCallWithArguments) {
//   TestParsingMethod(
//       "foo[a, b, c]", ParsePrimaryExpr,
//       Sast(kIndexOperator, "foo[a,b,c]", Sast(kIdent, "foo"),
//            Sast(kArguments, "[a,b,c]",
//                 Sast(kComma, "a,b,c", Sast(kComma, "a,b", Sast{kIdent, "a"}, Sast{kIdent, "b"}),
//                      Sast{kIdent, "c"}))));
// }
//
// TEST(UtParserPrimaryExprTopDown, ListingCallWithArguments) {
//   TestParsingMethod(
//       "foo{a,b,c}", ParsePrimaryExpr,
//       Sast(kListingOperator, "foo{a,b,c}", Sast(kIdent, "foo"),
//            Sast(kArguments, "{a,b,c}",
//                 Sast(kComma, "a,b,c", Sast(kComma, "a,b", Sast{kIdent, "a"}, Sast{kIdent, "b"}),
//                      Sast{kIdent, "c"}))));
// }
//
// TEST(UtParserPrimaryExprTopDown, ResolutionOperator) {
//   TestParsingMethod("foo::bar", ParsePrimaryExpr,
//                     Sast(kResolutionOp, "foo::bar", Sast(kIdent, "foo"), Sast(kIdent, "bar")));
// }
//
// TEST(UtParserPrimaryExprTopDown, UnaryMinusSingleOperand) {
//   TestParsingMethod("-1", ParsePrimaryExpr, Sast(kUnaryMinus, "-1", Sast(kLitI32, "1")));
// }
//
// TEST(UtParserPrimaryExprTopDown, UnaryMinusInExpr) {
//   TestParsingMethod(
//       "-1+2", ParsePrimaryExpr,
//       Sast(kAdd, "-1+2", Sast(kUnaryMinus, "-1", Sast(kLitI32, "1")), Sast(kLitI32, "2")));
// }
//
// TEST(UtParserPrimaryExprTopDown, UnaryMinusInExprWithSubtraction) {
//   TestParsingMethod("-1 - -1", ParsePrimaryExpr,
//                     Sast(kSub, "-1--1", Sast(kUnaryMinus, "-1", Sast(kLitI32, "1")),
//                          Sast(kUnaryMinus, "-1", Sast(kLitI32, "1"))));
// }
//
// TEST(UtParserPrimaryExprTopDown, UnaryPlusSingleOperand) {
//   TestParsingMethod("+1", ParsePrimaryExpr, Sast(kUnaryPlus, "+1", Sast(kLitI32, "1")));
// }
//
// TEST(UtParserPrimaryExprTopDown, UnaryPlusInExpr) {
//   TestParsingMethod(
//       "+1+2", ParsePrimaryExpr,
//       Sast(kAdd, "+1+2", Sast(kUnaryPlus, "+1", Sast(kLitI32, "1")), Sast(kLitI32, "2")));
// }
//
// TEST(UtParserPrimaryExprTopDown, UnaryPlusInExprWithAddition) {
//   TestParsingMethod("+1 + +1", ParsePrimaryExpr,
//                     Sast(kAdd, "+1++1", Sast(kUnaryPlus, "+1", Sast(kLitI32, "1")),
//                          Sast(kUnaryPlus, "+1", Sast(kLitI32, "1"))));
// }
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///* UtParserPrimaryExprGrammar : */
///* Validate primary expression associativity and precedence by extensively testing each primary
// * expression grammar   */
///* rule(precedence level). Each precedence level should be tested with all possible orthogonal
// * following operations. */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
TEST(UtParserPrimaryExprGrammar, ParseResolution) {
   TestOperatorParsingMethod("a::b", parseResolution,
                     NodeResolution{kResolutionOp, "a::b", Sast{kIdent, "a"}, Sast{kIdent, "b"}});
  // TestParsingMethod("1::b", ParseResolution,
  //                   Sast(kResolutionOp, "1::b", Sast{kLitI32, "1"}, Sast{kIdent, "b"}));
  // TestParsingMethod("\"a\"::b", ParseResolution,
  //                   Sast(kResolutionOp, "\"a\"::b", Sast{kLitCstr, "\"a\""}, Sast{kIdent, "b"}));
  // TestParsingMethod("a::b", ParseResolution,
  //                   Sast(kResolutionOp, "a::b", Sast{kIdent, "a"}, Sast{kIdent, "b"}));
  // TestParsingMethod("a::1", ParseResolution,
  //                   Sast(kResolutionOp, "a::1", Sast{kIdent, "a"}, Sast{kLitI32, "1"}));
  // TestParsingMethod("a::\"a\"", ParseResolution,
  //                   Sast(kResolutionOp, "a::\"a\"", Sast{kIdent, "a"}, Sast{kLitCstr, "\"a\""}));
}
//  TestParsingMethod(
//      "a::b::c", ParseResolution,
//      Sast(kResolutionOp, "a::b::c",
//           Sast(kResolutionOp, "a::b", Sast{kIdent, "a"}, Sast{kIdent, "b"}), Sast{kIdent, "c"}));
//  TestParsingMethod("a::b::c::d", ParseResolution,
//                    Sast(kResolutionOp, "a::b::c::d",
//                         Sast(kResolutionOp, "a::b::c",
//                              Sast(kResolutionOp, "a::b", Sast{kIdent, "a"}, Sast{kIdent, "b"}),
//                              Sast{kIdent, "c"}),
//                         Sast{kIdent, "d"}));
//
//  // RunAndCompareParserMethod(ParseResolution, Sast(kDoubleColon, "::", Sast{kIdent, "a"},
//  // Sast{kIdent, "b"}),
//  // "(a::b)");
//
//  // RunAndCompareParserMethod(
//  //     ParseResolution,
//  //     Sast(kDoubleColon, "::",
//  //          Sast(kDoubleColon, "::", Sast(kDoubleColon, "::", Sast{kIdent, "a"}, Sast{kIdent,
//  //          "b"}), Sast{kIdent, "c"}), Sast{kIdent, "d"}),
//  //     "(a::b)::c::d");
//
//  // RunAndCompareParserMethod(
//  //     ParseResolution,
//  //     Sast(kDoubleColon, "::",
//  //          Sast(kDoubleColon, "::", Sast{kIdent, "a"}, Sast(kDoubleColon, "::", Sast{kIdent,
//  //          "b"}, Sast{kIdent, "c"})), Sast{kIdent, "d"}),
//  //     "a::(b::c)::d");
//}
//
// TEST(UtParserPrimaryExprGrammar, ParsePostfix) {
//  TestParsingMethod("a++", ParsePostfix, Sast(kInc, "a++", Sast(kIdent, "a")));
//  TestParsingMethod("a--", ParsePostfix, Sast(kDec, "a--", Sast(kIdent, "a")));
//
//  TestParsingMethod("a++++", ParsePostfix,
//                    Sast(kInc, "a++++", Sast(kInc, "a++", Sast(kIdent, "a"))));
//  TestParsingMethod("a----", ParsePostfix,
//                    Sast(kDec, "a----", Sast(kDec, "a--", Sast(kIdent, "a"))));
//  TestParsingMethod(
//      "a++--++--", ParsePostfix,
//      Sast(kDec, "a++--++--",
//           Sast(kInc, "a++--++", Sast(kDec, "a++--", Sast(kInc, "a++", Sast(kIdent, "a"))))));
//
//  TestParsingMethod("a::b::c++--", ParsePostfix);
//  TestParsingMethod("a::b++--::c", ParsePostfix);
//  TestParsingMethod("a++--::b::c", ParsePostfix);
//}
//
// TEST(UtParserPrimaryExprGrammar, ParseMemberAccess) {
//  TestParsingMethod(
//      "a.b.c.d", ParseAccess,
//      Sast(kMemberAccess, "a.b.c.d",
//           Sast(kMemberAccess, "a.b.c", Sast(kMemberAccess, "a.b", Sast(kIdent, "a"), Sast(kIdent,
//           "b")),
//                Sast(kIdent, "c")),
//           Sast(kIdent, "d")));
//
//  TestParsingMethod("a::b.d::c", ParseAccess);
//  TestParsingMethod("a::b.d::c.e::f", ParseAccess);
//  TestParsingMethod("a--.b++.c--.d++", ParseAccess);
//}
//
// TEST(UtParserPrimaryExprGrammar, ParsePrefix) {
//  TestParsingMethod("++a", ParsePrefix, Sast(kInc, "++a", Sast(kIdent, "a")));
//  TestParsingMethod("--a", ParsePrefix, Sast(kDec, "--a", Sast(kIdent, "a")));
//  TestParsingMethod("!a", ParsePrefix, Sast(kNot, "!a", Sast(kIdent, "a")));
//
//  TestParsingMethod("----a", ParsePrefix,
//                    Sast(kDec, "----a", Sast(kDec, "--a", Sast(kIdent, "a"))));
//  TestParsingMethod("++++a", ParsePrefix,
//                    Sast(kInc, "++++a", Sast(kInc, "++a", Sast(kIdent, "a"))));
//  TestParsingMethod("!!a", ParsePrefix, Sast(kNot, "!!a", Sast(kNot, "!a", Sast(kIdent, "a"))));
//
//  // Prefix -> Resolution
//  TestParsingMethod(
//      "++a::b", ParsePrefix,
//      Sast(kInc, "++a::b", Sast{kResolutionOp, "a::b", Sast{kIdent, "a"}, Sast{kIdent, "b"}}));
//  TestParsingMethod(
//      "--a::b", ParsePrefix,
//      Sast(kDec, "--a::b", Sast{kResolutionOp, "a::b", Sast{kIdent, "a"}, Sast{kIdent, "b"}}));
//  TestParsingMethod(
//      "!a::b", ParsePrefix,
//      Sast(kNot, "!a::b", Sast{kResolutionOp, "a::b", Sast{kIdent, "a"}, Sast{kIdent, "b"}}));
//
//  // Prefix -> Postfix
//  TestParsingMethod("++a++", ParsePrefix,
//                    Sast(kInc, "++a++", Sast(kInc, "a++", Sast(kIdent, "a"))));
//  TestParsingMethod("--a++", ParsePrefix,
//                    Sast(kDec, "--a++", Sast(kInc, "a++", Sast(kIdent, "a"))));
//  TestParsingMethod("!a++", ParsePrefix, Sast(kNot, "!a++", Sast(kInc, "a++", Sast(kIdent,
//  "a"))));
//
//  // Prefix -> MemberAccess
//  TestParsingMethod(
//      "++a.b", ParsePrefix,
//      Sast(kInc, "++a.b", Sast(kMemberAccess, "a.b", Sast(kIdent, "a"), Sast(kIdent, "b"))));
//  TestParsingMethod(
//      "--a.b", ParsePrefix,
//      Sast(kDec, "--a.b", Sast(kMemberAccess, "a.b", Sast(kIdent, "a"), Sast(kIdent, "b"))));
//  TestParsingMethod(
//      "!a.b", ParsePrefix,
//      Sast(kNot, "!a.b", Sast(kMemberAccess, "a.b", Sast(kIdent, "a"), Sast(kIdent, "b"))));
//}
//
// TEST(UtParserPrimaryExprGrammar, ParseProduction) {
//  // '*' -> kMul
//  TestParsingMethod("a * b", ParseProduction,
//                    Sast{kMul, "a*b", Sast{kIdent, "a"}, Sast{kIdent, "b"}});
//  TestParsingMethod("a * b * c", ParseProduction,
//                    Sast(kMul, "a*b*c", Sast(kMul, "a*b", Sast{kIdent, "a"}, Sast{kIdent, "b"}),
//                         Sast{kIdent, "c"}));
//  TestParsingMethod(
//      "a * b * c * d", ParseProduction,
//      Sast(kMul, "a*b*c*d",
//           Sast(kMul, "a*b*c", Sast(kMul, "a*b", Sast{kIdent, "a"}, Sast{kIdent, "b"}),
//                Sast{kIdent, "c"}),
//           Sast{kIdent, "d"}));
//  // '/' -> kDiv
//  TestParsingMethod("a / b", ParseProduction,
//                    Sast{kDiv, "a/b", Sast{kIdent, "a"}, Sast{kIdent, "b"}});
//  TestParsingMethod("a / b / c", ParseProduction,
//                    Sast(kDiv, "a/b/c", Sast(kDiv, "a/b", Sast{kIdent, "a"}, Sast{kIdent, "b"}),
//                         Sast{kIdent, "c"}));
//  TestParsingMethod(
//      "a / b / c / d", ParseProduction,
//      Sast(kDiv, "a/b/c/d",
//           Sast(kDiv, "a/b/c", Sast(kDiv, "a/b", Sast{kIdent, "a"}, Sast{kIdent, "b"}),
//                Sast{kIdent, "c"}),
//           Sast{kIdent, "d"}));
//  // '%' -> kMod
//  TestParsingMethod("a % b", ParseProduction,
//                    Sast{kMod, "a%b", Sast{kIdent, "a"}, Sast{kIdent, "b"}});
//  TestParsingMethod("a % b % c", ParseProduction,
//                    Sast(kMod, "a%b%c", Sast(kMod, "a%b", Sast{kIdent, "a"}, Sast{kIdent, "b"}),
//                         Sast{kIdent, "c"}));
//  TestParsingMethod(
//      "a % b % c % d", ParseProduction,
//      Sast(kMod, "a%b%c%d",
//           Sast(kMod, "a%b%c", Sast(kMod, "a%b", Sast{kIdent, "a"}, Sast{kIdent, "b"}),
//                Sast{kIdent, "c"}),
//           Sast{kIdent, "d"}));
//
//  //// Production -> Resolution
//  // TestLLParse("a::b * c::d", ParseProduction, Sast{kMul, "a::b*c::d", Sast{kIdent, "a"},
//  // Sast{kIdent, "b"}});
//
//  //// Production -> Postfix
//  // TestLLParse("a::b * c::d", ParseProduction, Sast{kMul, "a::b*c::d", Sast{kIdent, "a"},
//  // Sast{kIdent, "b"}});
//
//  //// Production -> MemberAccess
//  // TestLLParse("a::b * c::d", ParseProduction, Sast{kMul, "a::b*c::d", Sast{kIdent, "a"},
//  // Sast{kIdent, "b"}});
//
//  //// Production -> Prefix
//  // TestLLParse("a::b * c::d", ParseProduction, Sast{kMul, "a::b*c::d", Sast{kIdent, "a"},
//  // Sast{kIdent, "b"}});
//}
//
// TEST(UtParserPrimaryExprGrammar, ParseSummation) {
//  // '+' -> kAdd
//  TestParsingMethod("a + b", ParseSummation,
//                    Sast{kAdd, "a+b", Sast{kIdent, "a"}, Sast{kIdent, "b"}});
//  TestParsingMethod("a + b + c", ParseSummation,
//                    Sast(kAdd, "a+b+c", Sast(kAdd, "a+b", Sast{kIdent, "a"}, Sast{kIdent, "b"}),
//                         Sast{kIdent, "c"}));
//  TestParsingMethod(
//      "a + b + c + d", ParseSummation,
//      Sast(kAdd, "a+b+c+d",
//           Sast(kAdd, "a+b+c", Sast(kAdd, "a+b", Sast{kIdent, "a"}, Sast{kIdent, "b"}),
//                Sast{kIdent, "c"}),
//           Sast{kIdent, "d"}));
//  // '-' -> kSub
//  TestParsingMethod("a - b", ParseSummation,
//                    Sast{kSub, "a-b", Sast{kIdent, "a"}, Sast{kIdent, "b"}});
//  TestParsingMethod("a - b - c", ParseSummation,
//                    Sast(kSub, "a-b-c", Sast(kSub, "a-b", Sast{kIdent, "a"}, Sast{kIdent, "b"}),
//                         Sast{kIdent, "c"}));
//  TestParsingMethod(
//      "a - b - c - d", ParseSummation,
//      Sast(kSub, "a-b-c-d",
//           Sast(kSub, "a-b-c", Sast(kSub, "a-b", Sast{kIdent, "a"}, Sast{kIdent, "b"}),
//                Sast{kIdent, "c"}),
//           Sast{kIdent, "d"}));
//
//  //// Production -> Resolution
//  // TestLLParse("a::b * c::d", ParseProduction, Sast{kMul, "a::b*c::d", Sast{kIdent, "a"},
//  // Sast{kIdent, "b"}});
//
//  //// Production -> Postfix
//  // TestLLParse("a::b * c::d", ParseProduction, Sast{kMul, "a::b*c::d", Sast{kIdent, "a"},
//  // Sast{kIdent, "b"}});
//
//  //// Production -> MemberAccess
//  // TestLLParse("a::b * c::d", ParseProduction, Sast{kMul, "a::b*c::d", Sast{kIdent, "a"},
//  // Sast{kIdent, "b"}});
//
//  //// Production -> Prefix
//  // TestLLParse("a::b * c::d", ParseProduction, Sast{kMul, "a::b*c::d", Sast{kIdent, "a"},
//  // Sast{kIdent, "b"}});
//
//  /// Summation -> Production
//}
//
// TEST(UtParserPrimaryExprGrammar, ParseListFold) {
//  TestParsingMethod("a,b", ParseListFold,
//                    Sast{kComma, "a,b", Sast{kIdent, "a"}, Sast{kIdent, "b"}});
//  TestParsingMethod("a,b,c", ParseListFold,
//                    Sast(kComma, "a,b,c", Sast(kComma, "a,b", Sast{kIdent, "a"}, Sast{kIdent,
//                    "b"}),
//                         Sast{kIdent, "c"}));
//  TestParsingMethod(
//      "a , b , c , d", ParseListFold,
//      Sast(kComma, "a,b,c,d",
//           Sast(kComma, "a,b,c", Sast(kComma, "a,b", Sast{kIdent, "a"}, Sast{kIdent, "b"}),
//                Sast{kIdent, "c"}),
//           Sast{kIdent, "d"}));
//
//  //// Production -> Resolution
//  // TestLLParse("a::b * c::d", ParseProduction, Sast{kMul, "a::b*c::d", Sast{kIdent, "a"},
//  // Sast{kIdent, "b"}});
//
//  //// Production -> Postfix
//  // TestLLParse("a::b * c::d", ParseProduction, Sast{kMul, "a::b*c::d", Sast{kIdent, "a"},
//  // Sast{kIdent, "b"}});
//
//  //// Production -> MemberAccess
//  // TestLLParse("a::b * c::d", ParseProduction, Sast{kMul, "a::b*c::d", Sast{kIdent, "a"},
//  // Sast{kIdent, "b"}});
//
//  //// Production -> Prefix
//  // TestLLParse("a::b * c::d", ParseProduction, Sast{kMul, "a::b*c::d", Sast{kIdent, "a"},
//  // Sast{kIdent, "b"}});
//
//  /// Summation -> Production
//}

}  // namespace cnd_unit_test::frontend::parser
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