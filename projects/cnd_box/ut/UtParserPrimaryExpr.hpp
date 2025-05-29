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
/// @brief Tests parsing primary expressions in isolation. 
/// 
/// This is arguably the hardest part of the parser to develop. These tests provide some sanity and prevent regression.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup cnd_unit_test
/// @{
#pragma once
// clang-format off
#include "minitest.hpp"
#include "trtools/parser.hpp"
#include "ParserTestUtils.hpp"
// clang-format on

namespace cnd_unit_test::frontend::parser {
// Alias synthesized ast and ast enum to reduce noise for 'expected' ast construction.
using Sast = cnd::Sast;
using enum cnd::eAst;

// Method used for unit testing in this header
using cnd_unit_test::frontend::test_util::TestParsingMethod;
using enum cnd_unit_test::frontend::test_util::eTestParsingMethod;

// The namespace being tested from "trtools/parser.hpp" header.
using namespace cnd::trtools::parser;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* UtParserPrimaryExprOperands : Validate parsing singular terminals.                                                */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(UtParserPrimaryExprOperands, WholeNumber) {
  TestParsingMethod("1", ParseOperand, Sast{kLitInt, "1"});
}

TEST(UtParserPrimaryExprOperands, RealNumber) {
  
  TestParsingMethod("1.1", ParseOperand, Sast{kLitReal, "1.1"});
}

TEST(UtParserPrimaryExprOperands, CString) {
  
  TestParsingMethod("\"string literal\"", ParseOperand,
                               Sast{kLitCstr, "\"string literal\""});
  TestParsingMethod("\"\\\\\"", ParseOperand, Sast{kLitCstr, "\"\\\\\""});
}

TEST(UtParserPrimaryExprOperands, Alnumus) {
  
  TestParsingMethod("alnumus", ParseOperand, Sast{kIdent, "alnumus"});
}

TEST(UtParserPrimaryExprOperands, ByteLiteral) {
  
  TestParsingMethod("1c", ParseOperand, Sast{kLitByte, "1c"});
}

TEST(UtParserPrimaryExprOperands, BoolLiteral) {
  
  TestParsingMethod("1b", ParseOperand, Sast{kLitBool, "1b"});
}

TEST(UtParserPrimaryExprOperands, UnsignedLiteral) {
  
  TestParsingMethod("1u", ParseOperand, Sast{kLitUint, "1u"});
}

TEST(UtParserPrimaryExprOperands, NoneKeyword) {
  
  TestParsingMethod("none", ParseOperand, Sast{kKwNone, "none"});
}

TEST(UtParserPrimaryExprOperands, AllSingularOperands) {
  // This test confirms proper advancement of the iterator after parsing an operand.
  using namespace cnd::trtools::literals;
  
  using namespace cnd::trtools::parser;
  CND_CX auto src = "1 1.1 \"string literal\" \"\\\\\" alnumus 1c 1b 1u none "_cndTkLiteral;
  auto src_span = std::span{src.data(), src.size()};

  // Operands.
  std::vector<std::pair<cnd::eAst, std::string>> expected_result = {
      std::make_pair(kLitInt, "1"),
      std::make_pair(kLitReal, "1.1"),
      std::make_pair(kLitCstr, "\"string literal\""),
      std::make_pair(kLitCstr, "\"\\\\\""),
      std::make_pair(kIdent, "alnumus"),
      std::make_pair(kLitByte, "1c"),
      std::make_pair(kLitBool, "1b"),
      std::make_pair(kLitUint, "1u"),
      std::make_pair(kKwNone, "none"),
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* UtParserPrimaryExprTopDown : Validate primary expressions by calling the top-most ParsePrimaryExpr method.        */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(UtParserPrimaryExprTopDown, WholeNumber) {
  
  
  
  

  TestParsingMethod("1", ParsePrimaryExpr, Sast(kLitInt, "1"));
}

TEST(UtParserPrimaryExprTopDown, RealNumber) {
  
  
  
  
  TestParsingMethod("1.1", ParsePrimaryExpr, Sast(kLitReal, "1.1"));
}

TEST(UtParserPrimaryExprTopDown, CString) {
  
  
  
  
  TestParsingMethod("\"string literal\"", ParsePrimaryExpr, Sast(kLitCstr, "\"string literal\""));
}

TEST(UtParserPrimaryExprTopDown, CStringEscaped) {
  
  
  
  
  TestParsingMethod("\"\\\\\"", ParsePrimaryExpr, Sast(kLitCstr, "\"\\\\\""));
}

TEST(UtParserPrimaryExprTopDown, Alnumus) {
  
  
  
  
  TestParsingMethod("alnumus", ParsePrimaryExpr, Sast(kIdent, "alnumus"));
}

TEST(UtParserPrimaryExprTopDown, ByteLiteral) {
  
  
  
  
  TestParsingMethod("1c", ParsePrimaryExpr, Sast(kLitByte, "1c"));
}

TEST(UtParserPrimaryExprTopDown, BoolLiteral) {
  
  
  
  
  TestParsingMethod("1b", ParsePrimaryExpr, Sast(kLitBool, "1b"));
}

TEST(UtParserPrimaryExprTopDown, UnsignedLiteral) {
  
  
  
  
  TestParsingMethod("1u", ParsePrimaryExpr, Sast(kLitUint, "1u"));
}

TEST(UtParserPrimaryExprTopDown, NoneKeyword) {
  
  
  
  
  TestParsingMethod("none", ParsePrimaryExpr, Sast(kKwNone, "none"));
}

TEST(UtParserPrimaryExprTopDown, OperandInParenthesis) {
  
  
  
  
  TestParsingMethod("(1)", ParsePrimaryExpr, Sast(kSubexpression, "(1)", Sast(kLitInt, "1")));
}

TEST(UtParserPrimaryExprTopDown, OperandInSquareBrackets) {
  
  
  
  
  TestParsingMethod("[1]", ParsePrimaryExpr, Sast(kSquareSubexpr, "[1]", Sast(kLitInt, "1")));
}

TEST(UtParserPrimaryExprTopDown, OperandInCurlyBraces) {
  
  
  
  
  TestParsingMethod("{1}", ParsePrimaryExpr, Sast(kCurlySubexpr, "{1}", Sast{kLitInt, "1"}));
}

TEST(UtParserPrimaryExprTopDown, BinarySum) {
  
  
  
  
  TestParsingMethod("1 + 1", ParsePrimaryExpr, Sast(kAdd, "1+1", Sast(kLitInt, "1"), Sast(kLitInt, "1")));
}

TEST(UtParserPrimaryExprTopDown, BinarySumThenTerm) {
  
  
  
  
  TestParsingMethod("1 + 2 * 3", ParsePrimaryExpr,
                    Sast(kAdd, "1+2*3", Sast(kLitInt, "1"), Sast(kMul, "2*3", Sast(kLitInt, "2"), Sast(kLitInt, "3"))));
}

TEST(UtParserPrimaryExprTopDown, BinaryTermThenSum) {
  
  
  
  
  TestParsingMethod("1 * 2 + 3", ParsePrimaryExpr,
                    Sast(kAdd, "1*2+3", Sast(kMul, "1*2", Sast(kLitInt, "1"), Sast(kLitInt, "2")), Sast(kLitInt, "3")));
}

TEST(UtParserPrimaryExprTopDown, BinarySumIsLeftAssociative) {
  
  
  TestParsingMethod(
      "1 + 2 - 3", ParsePrimaryExpr,
      Sast(kSub, "1+2-3", Sast(kAdd, "1+2", Sast(kLitInt, "1"), Sast(kLitInt, "2")), Sast(kLitInt, "3")));
}

TEST(UtParserPrimaryExprTopDown, BinaryMemberAccessIsLeftAssociative) {
  
  
  TestParsingMethod(
      "a.b.c.d", ParsePrimaryExpr,
      Sast(kMemberAccess, "a.b.c.d",
           Sast(kMemberAccess, "a.b.c", Sast(kMemberAccess, "a.b", Sast(kIdent, "a"), Sast(kIdent, "b")),
                Sast(kIdent, "c")),
           Sast(kIdent, "d")));
}

TEST(UtParserPrimaryExprTopDown, ParenIsResolvedFirst) {
  
  
  
  TestParsingMethod(
      "(1+2)*3", ParsePrimaryExpr,
      Sast(kMul, "(1+2)*3", Sast(kSubexpression, "(1+2)", Sast(kAdd, "1+2", Sast(kLitInt, "1"), Sast(kLitInt, "2"))),
           Sast(kLitInt, "3")));
}

TEST(UtParserPrimaryExprTopDown, UnaryPrefix) {
  
  
  
  TestParsingMethod("!a", ParsePrimaryExpr, Sast(kNot, "!a", Sast(kIdent, "a")));
}

TEST(UtParserPrimaryExprTopDown, RepeatedUnaryPrefix) {
  
  
  
  TestParsingMethod("!!a", ParsePrimaryExpr,
                               Sast(kNot, "!!a", Sast(kNot, "!a", Sast(kIdent, "a"))));
}

TEST(UtParserPrimaryExprTopDown, AssignmentIsRightAssociative) {
  // (a=(b=c))
  TestParsingMethod(
      "a=b=c", ParseAssignment,
      Sast(kAssign, "a=b=c", Sast(kIdent, "a"), Sast(kAssign, "b=c", Sast(kIdent, "b"), Sast(kIdent, "c"))));
  // (a=(b=c))
  TestParsingMethod(
      "x=a=b=c", ParseAssignment,
      Sast(kAssign, "x=a=b=c", Sast(kIdent, "x"),
           Sast(kAssign, "a=b=c", Sast(kIdent, "a"), Sast(kAssign, "b=c", Sast(kIdent, "b"), Sast(kIdent, "c")))));
}

TEST(UtParserPrimaryExprTopDown, PrefixUnaryThenBinary) {
  
  
  
  TestParsingMethod("!1+2", ParsePrimaryExpr,
                               Sast(kAdd, "!1+2", Sast(kNot, "!1", Sast(kLitInt, "1")), Sast(kLitInt, "2")));
}

TEST(UtParserPrimaryExprTopDown, PrefixUnaryThenMemberAccess) {
  
  
  
  TestParsingMethod("!a.b", ParsePrimaryExpr,
                               Sast(kNot, "!a.b", Sast(kMemberAccess, "a.b", Sast(kIdent, "a"), Sast(kIdent, "b"))));
}

TEST(UtParserPrimaryExprTopDown, PrefixUnaryAfterBinary) {
  
  
  
  TestParsingMethod("1+!2", ParsePrimaryExpr,
                               Sast(kAdd, "1+!2", Sast(kLitInt, "1"), Sast(kNot, "!2", Sast(kLitInt, "2"))));
}

TEST(UtParserPrimaryExprTopDown, EmptyFunctionCall) {
  
  
  
  TestParsingMethod("a()", ParsePrimaryExpr,
                               Sast(kFunctionCall, "a()", Sast(kIdent, "a"), Sast(kArguments, "()")));
}

TEST(UtParserPrimaryExprTopDown, PrefixUnaryThenFunctionCall) {
  
  
  
  TestParsingMethod(
      "!a()", ParsePrimaryExpr,
      Sast(kNot, "!a()", Sast(kFunctionCall, "a()", Sast(kIdent, "a"), Sast(kArguments, "()"))));
}

TEST(UtParserPrimaryExprTopDown, RepeatedUnaryThenRepeatedFunctionCall) {
  
  
  
  TestParsingMethod(
      "!!a()()", ParsePrimaryExpr,
      Sast(kNot, "!!a()()",
           Sast(kNot, "!a()()",
                Sast(kFunctionCall, "a()()", Sast(kFunctionCall, "a()", Sast(kIdent, "a"), Sast(kArguments, "()")),
                     Sast(kArguments, "()")))));
}

TEST(UtParserPrimaryExprTopDown, FunctionCallThenBinary) {
  
  
  
  TestParsingMethod(
      "a()+2", ParsePrimaryExpr,
      Sast(kAdd, "a()+2", Sast(kFunctionCall, "a()", Sast(kIdent, "a"), Sast(kArguments, "()")), Sast(kLitInt, "2")));
}

TEST(UtParserPrimaryExprTopDown, FunctionCallThenMemberAccess) {
  
  
  
  TestParsingMethod(
      "a().b", ParsePrimaryExpr,
      Sast(kMemberAccess, "a().b", Sast(kFunctionCall, "a()", Sast(kIdent, "a"), Sast(kArguments, "()")),
           Sast(kIdent, "b")));
}

TEST(UtParserPrimaryExprTopDown, MemberAccessThenFunctionCall) {
  
  
  
  TestParsingMethod(
      "a.b()", ParsePrimaryExpr,
      Sast(kFunctionCall, "a.b()", Sast(kMemberAccess, "a.b", Sast(kIdent, "a"), Sast(kIdent, "b")),
           Sast(kArguments, "()")));
}

TEST(UtParserPrimaryExprTopDown, BinaryThenFunctionCall) {
  
  
  
  TestParsingMethod(
      "1+a()", ParsePrimaryExpr,
      Sast(kAdd, "1+a()", Sast(kLitInt, "1"), Sast(kFunctionCall, "a()", Sast(kIdent, "a"), Sast(kArguments, "()"))));
}

TEST(UtParserPrimaryExprTopDown, InteleavedMemberAccessAndFunctionCall) {
  
  
  
  // a.b().c is parsed as (a.b()).c and not as a.(b().c) because of left-to-right associativity of member access.
  // Source: cppreference.com
  TestParsingMethod(
      "a.b().c.d", ParsePrimaryExpr,
      Sast(kMemberAccess, ".",
           Sast(kMemberAccess, ".",
                Sast(kFunctionCall, "", Sast(kMemberAccess, ".", Sast(kIdent, "a"), Sast(kIdent, "b")),
                     Sast(kArguments, "")),
                Sast(kIdent, "c")),
           Sast(kIdent, "d")));
}

TEST(UtParserPrimaryExprTopDown, ComplexExpr) {
  
  
  
  TestParsingMethod(
      "foo.bar()*1+1", ParsePrimaryExpr,
      Sast(kAdd, "foo.bar()*1+1",
           Sast(kMul, "foo.bar()*1",
                Sast(kFunctionCall, "foo.bar()",
                     Sast(kMemberAccess, "foo.bar", Sast(kIdent, "foo"), Sast(kIdent, "bar")), Sast(kArguments, "()")),
                Sast(kLitInt, "1")),
           Sast(kLitInt, "1")));
}

TEST(UtParserPrimaryExprTopDown, ComplexExpr2) {
  
  
  
  TestParsingMethod(
      "(foo.bar() + 1) * 1", ParsePrimaryExpr,
      Sast(kMul, "(foo.bar()+1)*1",
           Sast(kAdd, "foo.bar()+1",
                Sast(kFunctionCall, "foo.bar()",
                     Sast(kMemberAccess, "foo.bar", Sast(kIdent, "foo"), Sast(kIdent, "bar")), Sast(kArguments, "()")),
                Sast(kLitInt, "1")),
           Sast(kLitInt, "1")));
}

TEST(UtParserPrimaryExprTopDown, AssignmentExpression) {
  
  
  
  TestParsingMethod(
      "foo = 1 + 2", ParsePrimaryExpr,
      Sast(kAssign, "foo=1+2", Sast(kIdent, "foo"), Sast(kAdd, "1+2", Sast(kLitInt, "1"), Sast(kLitInt, "2"))));
}

TEST(UtParserPrimaryExprTopDown, FunctionCallAfterBinaryLeftAssociative) {
  
  
  
  TestParsingMethod(
      "a+b+c+d()", ParsePrimaryExpr,
      Sast(kAdd, "a+b+c+d()",
           Sast(kAdd, "a+b+c", Sast(kAdd, "a+b", Sast{kIdent, "a"}, Sast{kIdent, "b"}), Sast{kIdent, "c"}),
           Sast(kFunctionCall, "d()", Sast(kIdent, "d"), Sast(kArguments, "()"))));
}

TEST(UtParserPrimaryExprTopDown, FunctionCallWithArguments) {
  
  
  
  TestParsingMethod(
      "foo(a, b, c)", ParsePrimaryExpr,
      Sast(kFunctionCall, "foo(a,b,c)", Sast(kIdent, "foo"),
           Sast(kArguments, "(a,b,c)",
                Sast(kComma, "a,b,c", Sast(kComma, "a,b", Sast{kIdent, "a"}, Sast{kIdent, "b"}), Sast{kIdent, "c"}))));
}

TEST(UtParserPrimaryExprTopDown, IndexingCallWithArguments) {
  
  
  
  TestParsingMethod(
      "foo[a, b, c]", ParsePrimaryExpr,
      Sast(kIndexOperator, "foo[a,b,c]", Sast(kIdent, "foo"),
           Sast(kArguments, "[a,b,c]",
                Sast(kComma, "a,b,c", Sast(kComma, "a,b", Sast{kIdent, "a"}, Sast{kIdent, "b"}), Sast{kIdent, "c"}))));
}

TEST(UtParserPrimaryExprTopDown, ListingCallWithArguments) {
  
  
  
  TestParsingMethod(
      "foo{a,b,c}", ParsePrimaryExpr,
      Sast(kListingOperator, "foo{a,b,c}", Sast(kIdent, "foo"),
           Sast(kArguments, "{a,b,c}",
                Sast(kComma, "a,b,c", Sast(kComma, "a,b", Sast{kIdent, "a"}, Sast{kIdent, "b"}), Sast{kIdent, "c"}))));
}

TEST(UtParserPrimaryExprTopDown, ResolutionOperator) {
  
  
  
  TestParsingMethod("foo::bar", ParsePrimaryExpr,
                               Sast(kResolutionOp, "foo::bar", Sast(kIdent, "foo"), Sast(kIdent, "bar")));
}

TEST(UtParserPrimaryExprTopDown, UnaryMinusSingleOperand) {
  
  
  
  TestParsingMethod("-1", ParsePrimaryExpr, Sast(kUnaryMinus, "-1", Sast(kLitInt, "1")));
}

TEST(UtParserPrimaryExprTopDown, UnaryMinusInExpr) {
  
  
  
  TestParsingMethod("-1+2", ParsePrimaryExpr,
                               Sast(kAdd, "-1+2", Sast(kUnaryMinus, "-1", Sast(kLitInt, "1")), Sast(kLitInt, "2")));
}

TEST(UtParserPrimaryExprTopDown, UnaryMinusInExprWithSubtraction) {
  
  
  
  TestParsingMethod(
      "-1 - -1", ParsePrimaryExpr,
      Sast(kSub, "-1--1", Sast(kUnaryMinus, "-1", Sast(kLitInt, "1")), Sast(kUnaryMinus, "-1", Sast(kLitInt, "1"))));
}

TEST(UtParserPrimaryExprTopDown, UnaryPlusSingleOperand) {
  
  
  
  TestParsingMethod("+1", ParsePrimaryExpr, Sast(kUnaryPlus, "+1", Sast(kLitInt, "1")));
}

TEST(UtParserPrimaryExprTopDown, UnaryPlusInExpr) {
  
  
  
  TestParsingMethod("+1+2", ParsePrimaryExpr,
                               Sast(kAdd, "+1+2", Sast(kUnaryPlus, "+1", Sast(kLitInt, "1")), Sast(kLitInt, "2")));
}

TEST(UtParserPrimaryExprTopDown, UnaryPlusInExprWithSubtraction) {
  
  
  
  TestParsingMethod(
      "+1 + +1", ParsePrimaryExpr,
      Sast(kSub, "+1++1", Sast(kUnaryPlus, "+1", Sast(kLitInt, "1")), Sast(kUnaryPlus, "+1", Sast(kLitInt, "1"))));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* UtParserPrimaryExprGrammar :                                                                                      */
/* Validate primary expression associativity and precedence by extensively testing each primary expression grammar   */
/* rule(precedence level). Each precedence level should be tested with all possible orthogonal following operations. */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(UtParserPrimaryExprGrammar, ParseResolution) {
  TestParsingMethod("a::b", ParseResolution, Sast{kResolutionOp, "a::b", Sast{kIdent, "a"}, Sast{kIdent, "b"}});
  TestParsingMethod("1::b", ParseResolution, Sast(kResolutionOp, "1::b", Sast{kLitInt, "1"}, Sast{kIdent, "b"}));
  TestParsingMethod("\"a\"::b", ParseResolution,
                    Sast(kResolutionOp, "\"a\"::b", Sast{kLitCstr, "\"a\""}, Sast{kIdent, "b"}));
  TestParsingMethod("a::b", ParseResolution, Sast(kResolutionOp, "a::b", Sast{kIdent, "a"}, Sast{kIdent, "b"}));
  TestParsingMethod("a::1", ParseResolution, Sast(kResolutionOp, "a::1", Sast{kIdent, "a"}, Sast{kLitInt, "1"}));
  TestParsingMethod("a::\"a\"", ParseResolution,
                    Sast(kResolutionOp, "a::\"a\"", Sast{kIdent, "a"}, Sast{kLitCstr, "\"a\""}));

  TestParsingMethod("a::b::c", ParseResolution,
                    Sast(kResolutionOp, "a::b::c", Sast(kResolutionOp, "a::b", Sast{kIdent, "a"}, Sast{kIdent, "b"}),
                         Sast{kIdent, "c"}));
  TestParsingMethod("a::b::c::d", ParseResolution,
                    Sast(kResolutionOp, "a::b::c::d",
                         Sast(kResolutionOp, "a::b::c",
                              Sast(kResolutionOp, "a::b", Sast{kIdent, "a"}, Sast{kIdent, "b"}), Sast{kIdent, "c"}),
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

TEST(UtParserPrimaryExprGrammar, ParsePostfix) {
  TestParsingMethod("a++", ParsePostfix, Sast(kInc, "a++", Sast(kIdent, "a")));
  TestParsingMethod("a--", ParsePostfix, Sast(kDec, "a--", Sast(kIdent, "a")));

  TestParsingMethod("a++++", ParsePostfix, Sast(kInc, "a++++", Sast(kInc, "a++", Sast(kIdent, "a"))));
  TestParsingMethod("a----", ParsePostfix, Sast(kDec, "a----", Sast(kDec, "a--", Sast(kIdent, "a"))));
  TestParsingMethod(
      "a++--++--", ParsePostfix,
      Sast(kDec, "a++--++--", Sast(kInc, "a++--++", Sast(kDec, "a++--", Sast(kInc, "a++", Sast(kIdent, "a"))))));

  TestParsingMethod("a::b::c++--", ParsePostfix);
  TestParsingMethod("a::b++--::c", ParsePostfix);
  TestParsingMethod("a++--::b::c", ParsePostfix);
}

TEST(UtParserPrimaryExprGrammar, ParseMemberAccess) {
  TestParsingMethod(
      "a.b.c.d", ParseAccess,
      Sast(kPeriod, "a.b.c.d",
           Sast(kPeriod, "a.b.c", Sast(kPeriod, "a.b", Sast(kIdent, "a"), Sast(kIdent, "b")), Sast(kIdent, "c")),
           Sast(kIdent, "d")));

  TestParsingMethod("a::b.d::c", ParseAccess);
  TestParsingMethod("a::b.d::c.e::f", ParseAccess);
  TestParsingMethod("a--.b++.c--.d++", ParseAccess);
}

TEST(UtParserPrimaryExprGrammar, ParsePrefix) {
  TestParsingMethod("++a", ParsePrefix, Sast(kInc, "++a", Sast(kIdent, "a")));
  TestParsingMethod("--a", ParsePrefix, Sast(kDec, "--a", Sast(kIdent, "a")));
  TestParsingMethod("!a", ParsePrefix, Sast(kNot, "!a", Sast(kIdent, "a")));

  TestParsingMethod("----a", ParsePrefix, Sast(kDec, "----a", Sast(kDec, "--a", Sast(kIdent, "a"))));
  TestParsingMethod("++++a", ParsePrefix, Sast(kInc, "++++a", Sast(kInc, "++a", Sast(kIdent, "a"))));
  TestParsingMethod("!!a", ParsePrefix, Sast(kNot, "!!a", Sast(kNot, "!a", Sast(kIdent, "a"))));

  // Prefix -> Resolution
  TestParsingMethod("++a::b", ParsePrefix,
                    Sast(kInc, "++a::b", Sast{kResolutionOp, "a::b", Sast{kIdent, "a"}, Sast{kIdent, "b"}}));
  TestParsingMethod("--a::b", ParsePrefix,
                    Sast(kDec, "--a::b", Sast{kResolutionOp, "a::b", Sast{kIdent, "a"}, Sast{kIdent, "b"}}));
  TestParsingMethod("!a::b", ParsePrefix,
                    Sast(kNot, "!a::b", Sast{kResolutionOp, "a::b", Sast{kIdent, "a"}, Sast{kIdent, "b"}}));

  // Prefix -> Postfix
  TestParsingMethod("++a++", ParsePrefix, Sast(kInc, "++a++", Sast(kInc, "a++", Sast(kIdent, "a"))));
  TestParsingMethod("--a++", ParsePrefix, Sast(kDec, "--a++", Sast(kInc, "a++", Sast(kIdent, "a"))));
  TestParsingMethod("!a++", ParsePrefix, Sast(kNot, "!a++", Sast(kInc, "a++", Sast(kIdent, "a"))));

  // Prefix -> MemberAccess
  TestParsingMethod("++a.b", ParsePrefix,
                    Sast(kInc, "++a.b", Sast(kMemberAccess, "a.b", Sast(kIdent, "a"), Sast(kIdent, "b"))));
  TestParsingMethod("--a.b", ParsePrefix,
                    Sast(kDec, "--a.b", Sast(kMemberAccess, "a.b", Sast(kIdent, "a"), Sast(kIdent, "b"))));
  TestParsingMethod("!a.b", ParsePrefix,
                    Sast(kNot, "!a.b", Sast(kMemberAccess, "a.b", Sast(kIdent, "a"), Sast(kIdent, "b"))));
}

TEST(UtParserPrimaryExprGrammar, ParseProduction) {
  // '*' -> kMul
  TestParsingMethod("a * b", ParseProduction, Sast{kMul, "a*b", Sast{kIdent, "a"}, Sast{kIdent, "b"}});
  TestParsingMethod("a * b * c", ParseProduction,
                    Sast(kMul, "a*b*c", Sast(kMul, "a*b", Sast{kIdent, "a"}, Sast{kIdent, "b"}), Sast{kIdent, "c"}));
  TestParsingMethod(
      "a * b * c * d", ParseProduction,
      Sast(kMul, "a*b*c*d",
           Sast(kMul, "a*b*c", Sast(kMul, "a*b", Sast{kIdent, "a"}, Sast{kIdent, "b"}), Sast{kIdent, "c"}),
           Sast{kIdent, "d"}));
  // '/' -> kDiv
  TestParsingMethod("a / b", ParseProduction, Sast{kDiv, "a/b", Sast{kIdent, "a"}, Sast{kIdent, "b"}});
  TestParsingMethod("a / b / c", ParseProduction,
                    Sast(kDiv, "a/b/c", Sast(kDiv, "a/b", Sast{kIdent, "a"}, Sast{kIdent, "b"}), Sast{kIdent, "c"}));
  TestParsingMethod(
      "a / b / c / d", ParseProduction,
      Sast(kDiv, "a/b/c/d",
           Sast(kDiv, "a/b/c", Sast(kDiv, "a/b", Sast{kIdent, "a"}, Sast{kIdent, "b"}), Sast{kIdent, "c"}),
           Sast{kIdent, "d"}));
  // '%' -> kMod
  TestParsingMethod("a % b", ParseProduction, Sast{kMod, "a%b", Sast{kIdent, "a"}, Sast{kIdent, "b"}});
  TestParsingMethod("a % b % c", ParseProduction,
                    Sast(kMod, "a%b%c", Sast(kMod, "a%b", Sast{kIdent, "a"}, Sast{kIdent, "b"}), Sast{kIdent, "c"}));
  TestParsingMethod(
      "a % b % c % d", ParseProduction,
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

TEST(UtParserPrimaryExprGrammar, ParseSummation) {
  // '+' -> kAdd
  TestParsingMethod("a + b", ParseSummation, Sast{kAdd, "a+b", Sast{kIdent, "a"}, Sast{kIdent, "b"}});
  TestParsingMethod("a + b + c", ParseSummation,
                    Sast(kAdd, "a+b+c", Sast(kAdd, "a+b", Sast{kIdent, "a"}, Sast{kIdent, "b"}), Sast{kIdent, "c"}));
  TestParsingMethod(
      "a + b + c + d", ParseSummation,
      Sast(kAdd, "a+b+c+d",
           Sast(kAdd, "a+b+c", Sast(kAdd, "a+b", Sast{kIdent, "a"}, Sast{kIdent, "b"}), Sast{kIdent, "c"}),
           Sast{kIdent, "d"}));
  // '-' -> kSub
  TestParsingMethod("a - b", ParseSummation, Sast{kSub, "a-b", Sast{kIdent, "a"}, Sast{kIdent, "b"}});
  TestParsingMethod("a - b - c", ParseSummation,
                    Sast(kSub, "a-b-c", Sast(kSub, "a-b", Sast{kIdent, "a"}, Sast{kIdent, "b"}), Sast{kIdent, "c"}));
  TestParsingMethod(
      "a - b - c - d", ParseSummation,
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

TEST(UtParserPrimaryExprGrammar, ParseListFold) {
  TestParsingMethod("a,b", ParseListFold, Sast{kComma, "a,b", Sast{kIdent, "a"}, Sast{kIdent, "b"}});
  TestParsingMethod(
      "a,b,c", ParseListFold,
      Sast(kComma, "a,b,c", Sast(kComma, "a,b", Sast{kIdent, "a"}, Sast{kIdent, "b"}), Sast{kIdent, "c"}));
  TestParsingMethod(
      "a , b , c , d", ParseListFold,
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

}  // namespace cnd_unit_test::frontend::parser
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