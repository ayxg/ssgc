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
#include "ParserTestUtils.hpp"
// clang-format on

namespace cnd_unit_test::frontend::parser {
// Alias synthesized ast and ast enum to reduce noise for 'expected' ast construction.
using A = ssgc::frontend::SynthesizedAst;
using enum ssgc::frontend::eAst;

// Method used for unit testing in this header
using namespace cnd_unit_test::frontend::test_util;

// The namespace being tested from "trtools/Parser.hpp" header.
using namespace ssgc::frontend;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* UtParserPrimaryExprOperands : Validate parsing singular terminals. */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(UtParserPrimaryExprOperands, LiteralBool) {
  TestParsingMethod("1b", ParseOperand, A{kLitU1, "1b"});
  TestParsingMethod("0b", ParseOperand, A{kLitU1, "0b"});
  TestParsingMethod("true", ParseOperand, A{kKwTrue, "true"});
  TestParsingMethod("false", ParseOperand, A{kKwFalse, "false"});
}

TEST(UtParserPrimaryExprOperands, LiteralU8) {
  for (int i = 0; i <= 127; i++) {
    TestParsingMethod(std::to_string(i) + "B", ParseOperand, A{kLitU8, std::to_string(i) + "B"});
    TestParsingMethod(std::to_string(i) + "u8", ParseOperand, A{kLitU8, std::to_string(i) + "u8"});
  }
}

TEST(UtParserPrimaryExprOperands, LiteralU16) {
  TestParsingMethod("123u16", ParseOperand, A{kLitU16, "123u16"});
}

TEST(UtParserPrimaryExprOperands, LiteralU32) {
  TestParsingMethod("123u", ParseOperand, A{kLitU32, "123u"});
  TestParsingMethod("123u32", ParseOperand, A{kLitU32, "123u32"});
}

TEST(UtParserPrimaryExprOperands, LiteralU64) {
  TestParsingMethod("123U", ParseOperand, A{kLitU64, "123U"});
  TestParsingMethod("123u64", ParseOperand, A{kLitU64, "123u64"});
}

TEST(UtParserPrimaryExprOperands, LiteralI8) {
  for (int i = 0; i <= 127; i++) {
    TestParsingMethod(std::to_string(i) + "c", ParseOperand, A{kLitI8, std::to_string(i) + "c"});
    TestParsingMethod(std::to_string(i) + "i8", ParseOperand, A{kLitI8, std::to_string(i) + "i8"});
  }
}

TEST(UtParserPrimaryExprOperands, LiteralI16) {
  TestParsingMethod("123i16", ParseOperand, A{kLitI16, "123i16"});
}

TEST(UtParserPrimaryExprOperands, LiteralI32) {
  TestParsingMethod("123", ParseOperand, A{kLitI32, "123"});
  TestParsingMethod("123i32", ParseOperand, A{kLitI32, "123i32"});
}

TEST(UtParserPrimaryExprOperands, LiteralI64) {
  TestParsingMethod("123L", ParseOperand, A{kLitI64, "123L"});
  TestParsingMethod("123i64", ParseOperand, A{kLitI64, "123i64"});
}

TEST(UtParserPrimaryExprOperands, LiteralF32) {
  TestParsingMethod("123.f", ParseOperand, A{kLitF32, "123.f"});
  TestParsingMethod("123.456f", ParseOperand, A{kLitF32, "123.456f"});
}

TEST(UtParserPrimaryExprOperands, LiteralF64) {
  TestParsingMethod("123.", ParseOperand, A{kLitF64, "123."});
  TestParsingMethod("123.456", ParseOperand, A{kLitF64, "123.456"});
}

TEST(UtParserPrimaryExprOperands, LiteralCStr) {
  TestParsingMethod("\"string literal\"", ParseOperand, A{kLitCstr, "\"string literal\""});
  TestParsingMethod("\"\\\\\"", ParseOperand, A{kLitCstr, "\"\\\\\""});
}

TEST(UtParserPrimaryExprOperands, NoneKeyword) {
  TestParsingMethod("none", ParseOperand, A{kKwNone, "none"});
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* UtParserPrimaryExprTopDown : Validate primary expressions by calling the top-most
 * ParsePrimaryExpr method.        */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(UtParserPrimaryExprTopDown, WholeNumber) {
  TestParsingMethod("1", ParsePrimaryExpr, A(kLitI32, "1"));
}

TEST(UtParserPrimaryExprTopDown, RealNumber) {
  TestParsingMethod("1.1", ParsePrimaryExpr, A(kLitF64, "1.1"));
}

TEST(UtParserPrimaryExprTopDown, CString) {
  TestParsingMethod("\"string literal\"", ParsePrimaryExpr, A(kLitCstr, "\"string literal\""));
}

TEST(UtParserPrimaryExprTopDown, CStringEscaped) {
  TestParsingMethod("\"\\\\\"", ParsePrimaryExpr, A(kLitCstr, "\"\\\\\""));
}

TEST(UtParserPrimaryExprTopDown, Alnumus) {
  TestParsingMethod("alnumus", ParsePrimaryExpr, A(kIdent, "alnumus"));
}

TEST(UtParserPrimaryExprTopDown, ByteLiteral) {
  TestParsingMethod("1c", ParsePrimaryExpr, A(kLitI8, "1c"));
}

TEST(UtParserPrimaryExprTopDown, BoolLiteral) {
  TestParsingMethod("1b", ParsePrimaryExpr, A(kLitU1, "1b"));
}

TEST(UtParserPrimaryExprTopDown, UnsignedLiteral) {
  TestParsingMethod("1u", ParsePrimaryExpr, A(kLitU32, "1u"));
}

TEST(UtParserPrimaryExprTopDown, NoneKeyword) {
  TestParsingMethod("none", ParsePrimaryExpr, A(kKwNone, "none"));
}

TEST(UtParserPrimaryExprTopDown, OperandInParenthesis) {
  TestParsingMethod("(1)", ParsePrimaryExpr, A(kSubexpression, "(1)", A(kLitI32, "1")));
}

TEST(UtParserPrimaryExprTopDown, OperandInSquareBrackets) {
  TestParsingMethod("[1]", ParsePrimaryExpr, A(kSquareSubexpr, "[1]", A(kLitI32, "1")));
}

TEST(UtParserPrimaryExprTopDown, OperandInCurlyBraces) {
  TestParsingMethod("{1}", ParsePrimaryExpr, A(kCurlySubexpr, "{1}", A{kLitI32, "1"}));
}

TEST(UtParserPrimaryExprTopDown, BinarySum) {
  TestParsingMethod("1 + 1", ParsePrimaryExpr, A(kAdd, "1 + 1", A(kLitI32, "1"), A(kLitI32, "1")));
}

TEST(UtParserPrimaryExprTopDown, BinarySumThenTerm) {
  TestParsingMethod(
      "1 + 2 * 3", ParsePrimaryExpr,
      A(kAdd, "1 + 2 * 3", A(kLitI32, "1"), A(kMul, "2 * 3", A(kLitI32, "2"), A(kLitI32, "3"))));
}

TEST(UtParserPrimaryExprTopDown, BinaryTermThenSum) {
  TestParsingMethod(
      "1 * 2 + 3", ParsePrimaryExpr,
      A(kAdd, "1 * 2 + 3", A(kMul, "1 * 2", A(kLitI32, "1"), A(kLitI32, "2")), A(kLitI32, "3")));
}

TEST(UtParserPrimaryExprTopDown, BinarySumIsLeftAssociative) {
  TestParsingMethod(
      "1 + 2 - 3", ParsePrimaryExpr,
      A(kSub, "1 + 2 - 3", A(kAdd, "1 + 2", A(kLitI32, "1"), A(kLitI32, "2")), A(kLitI32, "3")));
}

TEST(UtParserPrimaryExprTopDown, BinaryMemberAccessIsLeftAssociative) {
  TestParsingMethod("a.b.c.d", ParsePrimaryExpr,
                    A(kMemberAccess, "a.b.c.d",
                      A(kMemberAccess, "a.b.c",
                        A(kMemberAccess, "a.b", A(kIdent, "a"), A(kIdent, "b")), A(kIdent, "c")),
                      A(kIdent, "d")));
}

TEST(UtParserPrimaryExprTopDown, ParenIsResolvedFirst) {
  TestParsingMethod("(1+2)*3", ParsePrimaryExpr,
                    A(kMul, "(1+2)*3",
                      A(kSubexpression, "(1+2)", A(kAdd, "1+2", A(kLitI32, "1"), A(kLitI32, "2"))),
                      A(kLitI32, "3")));
}

TEST(UtParserPrimaryExprTopDown, UnaryPrefix) {
  TestParsingMethod("!a", ParsePrimaryExpr, A(kNot, "!a", A(kIdent, "a")));
}

TEST(UtParserPrimaryExprTopDown, RepeatedUnaryPrefix) {
  TestParsingMethod("!!a", ParsePrimaryExpr, A(kNot, "!!a", A(kNot, "!a", A(kIdent, "a"))));
}

TEST(UtParserPrimaryExprTopDown, AssignmentIsRightAssociative) {
  // (a=(b=c))
  TestParsingMethod(
      "a=b=c", ParseAssignment,
      A(kAssign, "a=b=c", A(kIdent, "a"), A(kAssign, "b=c", A(kIdent, "b"), A(kIdent, "c"))));
  // (a=(b=c))
  TestParsingMethod(
      "x=a=b=c", ParseAssignment,
      A(kAssign, "x=a=b=c", A(kIdent, "x"),
        A(kAssign, "a=b=c", A(kIdent, "a"), A(kAssign, "b=c", A(kIdent, "b"), A(kIdent, "c")))));
}

TEST(UtParserPrimaryExprTopDown, PrefixUnaryThenBinary) {
  TestParsingMethod("!1+2", ParsePrimaryExpr,
                    A(kAdd, "!1+2", A(kNot, "!1", A(kLitI32, "1")), A(kLitI32, "2")));
}

TEST(UtParserPrimaryExprTopDown, PrefixUnaryThenMemberAccess) {
  TestParsingMethod("!a.b", ParsePrimaryExpr,
                    A(kNot, "!a.b", A(kMemberAccess, "a.b", A(kIdent, "a"), A(kIdent, "b"))));
}

TEST(UtParserPrimaryExprTopDown, PrefixUnaryAfterBinary) {
  TestParsingMethod("1+!2", ParsePrimaryExpr,
                    A(kAdd, "1+!2", A(kLitI32, "1"), A(kNot, "!2", A(kLitI32, "2"))));
}

TEST(UtParserPrimaryExprTopDown, EmptyFunctionCall) {
  TestParsingMethod("a()", ParsePrimaryExpr,
                    A(kFunctionCall, "a()", A(kIdent, "a"), A(kArguments, "()")));
}

TEST(UtParserPrimaryExprTopDown, PrefixUnaryThenFunctionCall) {
  TestParsingMethod("!a()", ParsePrimaryExpr,
                    A(kNot, "!a()", A(kFunctionCall, "a()", A(kIdent, "a"), A(kArguments, "()"))));
}

TEST(UtParserPrimaryExprTopDown, RepeatedUnaryThenRepeatedFunctionCall) {
  TestParsingMethod(
      "!!a()()", ParsePrimaryExpr,
      A(kNot, "!!a()()",
        A(kNot, "!a()()",
          A(kFunctionCall, "a()()", A(kFunctionCall, "a()", A(kIdent, "a"), A(kArguments, "()")),
            A(kArguments, "()")))));
}

TEST(UtParserPrimaryExprTopDown, FunctionCallThenBinary) {
  TestParsingMethod("a()+2", ParsePrimaryExpr,
                    A(kAdd, "a()+2", A(kFunctionCall, "a()", A(kIdent, "a"), A(kArguments, "()")),
                      A(kLitI32, "2")));
}

TEST(UtParserPrimaryExprTopDown, FunctionCallThenMemberAccess) {
  TestParsingMethod(
      "a().b", ParsePrimaryExpr,
      A(kMemberAccess, "a().b", A(kFunctionCall, "a()", A(kIdent, "a"), A(kArguments, "()")),
        A(kIdent, "b")));
}

TEST(UtParserPrimaryExprTopDown, MemberAccessThenFunctionCall) {
  TestParsingMethod(
      "a.b()", ParsePrimaryExpr,
      A(kFunctionCall, "a.b()", A(kMemberAccess, "a.b", A(kIdent, "a"), A(kIdent, "b")),
        A(kArguments, "()")));
}

TEST(UtParserPrimaryExprTopDown, BinaryThenFunctionCall) {
  TestParsingMethod("1+a()", ParsePrimaryExpr,
                    A(kAdd, "1+a()", A(kLitI32, "1"),
                      A(kFunctionCall, "a()", A(kIdent, "a"), A(kArguments, "()"))));
}

TEST(UtParserPrimaryExprTopDown, InteleavedMemberAccessAndFunctionCall) {
  // a.b().c is parsed as (a.b()).c and not as a.(b().c) because of left-to-right associativity of
  // member access. Source: cppreference.com
  TestParsingMethod("a.b().c.d", ParsePrimaryExpr,
                    A(kMemberAccess, "a.b().c.d",
                      A(kMemberAccess, "a.b().c",
                        A(kFunctionCall, "a.b()", A(kMemberAccess, "a.b", A(kIdent, "a"), A(kIdent, "b")),
                          A(kArguments, "()")),
                        A(kIdent, "c")),
                      A(kIdent, "d")));
}

TEST(UtParserPrimaryExprTopDown, ComplexExpr) {
  TestParsingMethod(
      "foo.bar()*1+1", ParsePrimaryExpr,
      A(kAdd, "foo.bar()*1+1",
        A(kMul, "foo.bar()*1",
          A(kFunctionCall, "foo.bar()",
            A(kMemberAccess, "foo.bar", A(kIdent, "foo"), A(kIdent, "bar")), A(kArguments, "()")),
          A(kLitI32, "1")),
        A(kLitI32, "1")));
}

TEST(UtParserPrimaryExprTopDown, ComplexExpr2) {
  TestParsingMethod(
      "(foo.bar()+1)*1", ParsePrimaryExpr,
      A(kMul, "(foo.bar()+1)*1",
        A(kSubexpression, "(foo.bar()+1)",
          A(kAdd, "foo.bar()+1",
            A(kFunctionCall, "foo.bar()",
              A(kMemberAccess, "foo.bar", 
                A(kIdent, "foo"), 
                A(kIdent, "bar")), 
              A(kArguments, "()")),
            A(kLitI32, "1"))),
        A(kLitI32, "1")));
}

TEST(UtParserPrimaryExprTopDown, AssignmentExpression) {
  TestParsingMethod(
      "foo=1+2", ParsePrimaryExpr,
      A(kAssign, "foo=1+2", A(kIdent, "foo"), A(kAdd, "1+2", A(kLitI32, "1"), A(kLitI32, "2"))));
}

TEST(UtParserPrimaryExprTopDown, FunctionCallAfterBinaryLeftAssociative) {
  TestParsingMethod(
      "a+b+c+d()", ParsePrimaryExpr,
      A(kAdd, "a+b+c+d()",
        A(kAdd, "a+b+c", A(kAdd, "a+b", A{kIdent, "a"}, A{kIdent, "b"}), A{kIdent, "c"}),
        A(kFunctionCall, "d()", A(kIdent, "d"), A(kArguments, "()"))));
}

TEST(UtParserPrimaryExprTopDown, FunctionCallWithArguments) {
  TestParsingMethod(
      "foo(a,b,c)", ParsePrimaryExpr,
      A(kFunctionCall, "foo(a,b,c)", A(kIdent, "foo"),
        A(kArguments, "(a,b,c)",
          A(kComma, "a,b,c", A(kComma, "a,b", A{kIdent, "a"}, A{kIdent, "b"}), A{kIdent, "c"}))));
}

TEST(UtParserPrimaryExprTopDown, IndexingCallWithArguments) {
  TestParsingMethod(
      "foo[a,b,c]", ParsePrimaryExpr,
      A(kIndexOperator, "foo[a,b,c]", A(kIdent, "foo"),
        A(kArguments, "[a,b,c]",
          A(kComma, "a,b,c", A(kComma, "a,b", A{kIdent, "a"}, A{kIdent, "b"}), A{kIdent, "c"}))));
}

TEST(UtParserPrimaryExprTopDown, ListingCallWithArguments) {
  TestParsingMethod(
      "foo{a,b,c}", ParsePrimaryExpr,
      A(kListingOperator, "foo{a,b,c}", A(kIdent, "foo"),
        A(kArguments, "{a,b,c}",
          A(kComma, "a,b,c", A(kComma, "a,b", A{kIdent, "a"}, A{kIdent, "b"}), A{kIdent, "c"}))));
}

TEST(UtParserPrimaryExprTopDown, ResolutionOperator) {
  TestParsingMethod("foo::bar", ParsePrimaryExpr,
                    A(kResolutionOp, "foo::bar", A(kIdent, "foo"), A(kIdent, "bar")));
}

TEST(UtParserPrimaryExprTopDown, UnaryMinusSingleOperand) {
  TestParsingMethod("-1", ParsePrimaryExpr, A(kUnaryMinus, "-1", A(kLitI32, "1")));
}

TEST(UtParserPrimaryExprTopDown, UnaryMinusInExpr) {
  TestParsingMethod("-1+2", ParsePrimaryExpr,
                    A(kAdd, "-1+2", A(kUnaryMinus, "-1", A(kLitI32, "1")), A(kLitI32, "2")));
}

TEST(UtParserPrimaryExprTopDown, UnaryMinusInExprWithSubtraction) {
  TestParsingMethod("-1 - -1", ParsePrimaryExpr,
                    A(kSub, "-1 - -1", A(kUnaryMinus, "-1", A(kLitI32, "1")),
                      A(kUnaryMinus, "-1", A(kLitI32, "1"))));
}

TEST(UtParserPrimaryExprTopDown, UnaryPlusSingleOperand) {
  TestParsingMethod("+1", ParsePrimaryExpr, A(kUnaryPlus, "+1", A(kLitI32, "1")));
}

TEST(UtParserPrimaryExprTopDown, UnaryPlusInExpr) {
  TestParsingMethod("+1+2", ParsePrimaryExpr,
                    A(kAdd, "+1+2", A(kUnaryPlus, "+1", A(kLitI32, "1")), A(kLitI32, "2")));
}

TEST(UtParserPrimaryExprTopDown, UnaryPlusInExprWithSubtraction) {
  TestParsingMethod("+1 + +1", ParsePrimaryExpr,
                    A(kAdd, "+1 + +1", A(kUnaryPlus, "+1", A(kLitI32, "1")),
                      A(kUnaryPlus, "+1", A(kLitI32, "1"))));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* UtParserPrimaryExprGrammar : */
/* Validate primary expression associativity and precedence by extensively testing each primary
 * expression grammar   */
/* rule(precedence level). Each precedence level should be tested with all possible orthogonal
 * following operations. */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(UtParserPrimaryExprGrammar, ParseResolution) {
  TestParsingMethod("a::b", ParseResolution,
                    A{kResolutionOp, "a::b", A{kIdent, "a"}, A{kIdent, "b"}});
  TestParsingMethod("1::b", ParseResolution,
                    A(kResolutionOp, "1::b", A{kLitI32, "1"}, A{kIdent, "b"}));
  TestParsingMethod("\"a\"::b", ParseResolution,
                    A(kResolutionOp, "\"a\"::b", A{kLitCstr, "\"a\""}, A{kIdent, "b"}));
  TestParsingMethod("a::b", ParseResolution,
                    A(kResolutionOp, "a::b", A{kIdent, "a"}, A{kIdent, "b"}));
  TestParsingMethod("a::1", ParseResolution,
                    A(kResolutionOp, "a::1", A{kIdent, "a"}, A{kLitI32, "1"}));
  TestParsingMethod("a::\"a\"", ParseResolution,
                    A(kResolutionOp, "a::\"a\"", A{kIdent, "a"}, A{kLitCstr, "\"a\""}));

  TestParsingMethod("a::b::c", ParseResolution,
                    A(kResolutionOp, "a::b::c",
                      A(kResolutionOp, "a::b", A{kIdent, "a"}, A{kIdent, "b"}), A{kIdent, "c"}));
  TestParsingMethod("a::b::c::d", ParseResolution,
                    A(kResolutionOp, "a::b::c::d",
                      A(kResolutionOp, "a::b::c",
                        A(kResolutionOp, "a::b", A{kIdent, "a"}, A{kIdent, "b"}), A{kIdent, "c"}),
                      A{kIdent, "d"}));

  // RunAndCompareParserMethod(ParseResolution, A(kDoubleColon, "::", A{kIdent, "a"},
  // A{kIdent, "b"}),
  // "(a::b)");

  // RunAndCompareParserMethod(
  //     ParseResolution,
  //     A(kDoubleColon, "::",
  //          A(kDoubleColon, "::", A(kDoubleColon, "::", A{kIdent, "a"}, A{kIdent,
  //          "b"}), A{kIdent, "c"}), A{kIdent, "d"}),
  //     "(a::b)::c::d");

  // RunAndCompareParserMethod(
  //     ParseResolution,
  //     A(kDoubleColon, "::",
  //          A(kDoubleColon, "::", A{kIdent, "a"}, A(kDoubleColon, "::", A{kIdent,
  //          "b"}, A{kIdent, "c"})), A{kIdent, "d"}),
  //     "a::(b::c)::d");
}

TEST(UtParserPrimaryExprGrammar, ParsePostfix) {
  TestParsingMethod("a++", ParsePostfix, A(kInc, "a++", A(kIdent, "a")));
  TestParsingMethod("a--", ParsePostfix, A(kDec, "a--", A(kIdent, "a")));
  TestParsingMethod("a.b", ParsePostfix, A(kMemberAccess, "a.b", A(kIdent, "a"), A(kIdent, "b")));
  TestParsingMethod("a->b", ParsePostfix, A(kPointerMemberAccess, "a->b", A(kIdent, "a"), A(kIdent, "b")));

  TestParsingMethod("a++++", ParsePostfix, A(kInc, "a++++", A(kInc, "a++", A(kIdent, "a"))));
  TestParsingMethod("a----", ParsePostfix, A(kDec, "a----", A(kDec, "a--", A(kIdent, "a"))));
  TestParsingMethod(
      "a++--++--", ParsePostfix,
      A(kDec, "a++--++--", A(kInc, "a++--++", A(kDec, "a++--", A(kInc, "a++", A(kIdent, "a"))))));

  TestParsingMethod("a::b::c++--", ParsePostfix);
  TestParsingMethod("a::b++--::c", ParsePostfix);
  TestParsingMethod("a++--::b::c", ParsePostfix);
}

TEST(UtParserPrimaryExprGrammar, ParseMemberAccess) {
  TestParsingMethod("a.b.c.d", ParsePostfix,
      A(kMemberAccess, "a.b.c.d",
                      A(kMemberAccess, "a.b.c",
                        A(kMemberAccess, "a.b", A(kIdent, "a"), A(kIdent, "b")),
                        A(kIdent, "c")),
        A(kIdent, "d")));

  TestParsingMethod("a::b.d::c", ParsePostfix);
  TestParsingMethod("a::b.d::c.e::f", ParsePostfix);
  TestParsingMethod("a--.b++.c--.d++", ParsePostfix);
}

TEST(UtParserPrimaryExprGrammar, ParsePrefix) {
  TestParsingMethod("++a", ParsePrefix, A(kInc, "++a", A(kIdent, "a")));
  TestParsingMethod("--a", ParsePrefix, A(kDec, "--a", A(kIdent, "a")));
  TestParsingMethod("!a", ParsePrefix, A(kNot, "!a", A(kIdent, "a")));
  TestParsingMethod("*a", ParsePrefix, A(kDereference, "*a", A(kIdent, "a")));
  TestParsingMethod("&a", ParsePrefix, A(kAddressOf, "&a", A(kIdent, "a")));

  TestParsingMethod("----a", ParsePrefix, A(kDec, "----a", A(kDec, "--a", A(kIdent, "a"))));
  TestParsingMethod("++++a", ParsePrefix, A(kInc, "++++a", A(kInc, "++a", A(kIdent, "a"))));
  TestParsingMethod("!!a", ParsePrefix, A(kNot, "!!a", A(kNot, "!a", A(kIdent, "a"))));

  // Prefix -> Resolution
  TestParsingMethod("++a::b", ParsePrefix,
                    A(kInc, "++a::b", A{kResolutionOp, "a::b", A{kIdent, "a"}, A{kIdent, "b"}}));
  TestParsingMethod("--a::b", ParsePrefix,
                    A(kDec, "--a::b", A{kResolutionOp, "a::b", A{kIdent, "a"}, A{kIdent, "b"}}));
  TestParsingMethod("!a::b", ParsePrefix,
                    A(kNot, "!a::b", A{kResolutionOp, "a::b", A{kIdent, "a"}, A{kIdent, "b"}}));

  // Prefix -> Postfix
  TestParsingMethod("++a++", ParsePrefix, A(kInc, "++a++", A(kInc, "a++", A(kIdent, "a"))));
  TestParsingMethod("--a++", ParsePrefix, A(kDec, "--a++", A(kInc, "a++", A(kIdent, "a"))));
  TestParsingMethod("!a++", ParsePrefix, A(kNot, "!a++", A(kInc, "a++", A(kIdent, "a"))));

  // Prefix -> MemberAccess
  TestParsingMethod("++a.b", ParsePrefix,
                    A(kInc, "++a.b", A(kMemberAccess, "a.b", A(kIdent, "a"), A(kIdent, "b"))));
  TestParsingMethod("--a.b", ParsePrefix,
                    A(kDec, "--a.b", A(kMemberAccess, "a.b", A(kIdent, "a"), A(kIdent, "b"))));
  TestParsingMethod("!a.b", ParsePrefix,
                    A(kNot, "!a.b", A(kMemberAccess, "a.b", A(kIdent, "a"), A(kIdent, "b"))));
}

TEST(UtParserPrimaryExprGrammar, ParseProduction) {
  // '*' -> kMul
  TestParsingMethod("a*b", ParseProduction, A{kMul, "a*b", A{kIdent, "a"}, A{kIdent, "b"}});
  TestParsingMethod(
      "a*b*c", ParseProduction,
      A(kMul, "a*b*c", A(kMul, "a*b", A{kIdent, "a"}, A{kIdent, "b"}), A{kIdent, "c"}));
  TestParsingMethod(
      "a*b*c*d", ParseProduction,
      A(kMul, "a*b*c*d",
        A(kMul, "a*b*c", A(kMul, "a*b", A{kIdent, "a"}, A{kIdent, "b"}), A{kIdent, "c"}),
        A{kIdent, "d"}));
  // '/' -> kDiv
  TestParsingMethod("a/b", ParseProduction, A{kDiv, "a/b", A{kIdent, "a"}, A{kIdent, "b"}});
  TestParsingMethod(
      "a/b/c", ParseProduction,
      A(kDiv, "a/b/c", A(kDiv, "a/b", A{kIdent, "a"}, A{kIdent, "b"}), A{kIdent, "c"}));
  TestParsingMethod(
      "a/b/c/d", ParseProduction,
      A(kDiv, "a/b/c/d",
        A(kDiv, "a/b/c", A(kDiv, "a/b", A{kIdent, "a"}, A{kIdent, "b"}), A{kIdent, "c"}),
        A{kIdent, "d"}));
  // '%' -> kMod
  TestParsingMethod("a%b", ParseProduction, A{kMod, "a%b", A{kIdent, "a"}, A{kIdent, "b"}});
  TestParsingMethod(
      "a%b%c", ParseProduction,
      A(kMod, "a%b%c", A(kMod, "a%b", A{kIdent, "a"}, A{kIdent, "b"}), A{kIdent, "c"}));
  TestParsingMethod(
      "a%b%c%d", ParseProduction,
      A(kMod, "a%b%c%d",
        A(kMod, "a%b%c", A(kMod, "a%b", A{kIdent, "a"}, A{kIdent, "b"}), A{kIdent, "c"}),
        A{kIdent, "d"}));

  //// Production -> Resolution
  // TestLLParse("a::b * c::d", ParseProduction, A{kMul, "a::b*c::d", A{kIdent, "a"},
  // A{kIdent, "b"}});

  //// Production -> Postfix
  // TestLLParse("a::b * c::d", ParseProduction, A{kMul, "a::b*c::d", A{kIdent, "a"},
  // A{kIdent, "b"}});

  //// Production -> MemberAccess
  // TestLLParse("a::b * c::d", ParseProduction, A{kMul, "a::b*c::d", A{kIdent, "a"},
  // A{kIdent, "b"}});

  //// Production -> Prefix
  // TestLLParse("a::b * c::d", ParseProduction, A{kMul, "a::b*c::d", A{kIdent, "a"},
  // A{kIdent, "b"}});
}

TEST(UtParserPrimaryExprGrammar, ParseSummation) {
  // '+' -> kAdd
  TestParsingMethod("a+b", ParseSummation, A{kAdd, "a+b", A{kIdent, "a"}, A{kIdent, "b"}});
  TestParsingMethod(
      "a+b+c", ParseSummation,
      A(kAdd, "a+b+c", A(kAdd, "a+b", A{kIdent, "a"}, A{kIdent, "b"}), A{kIdent, "c"}));
  TestParsingMethod(
      "a+b+c+d", ParseSummation,
      A(kAdd, "a+b+c+d",
        A(kAdd, "a+b+c", A(kAdd, "a+b", A{kIdent, "a"}, A{kIdent, "b"}), A{kIdent, "c"}),
        A{kIdent, "d"}));
  // '-' -> kSub
  TestParsingMethod("a-b", ParseSummation, A{kSub, "a-b", A{kIdent, "a"}, A{kIdent, "b"}});
  TestParsingMethod(
      "a-b-c", ParseSummation,
      A(kSub, "a-b-c", A(kSub, "a-b", A{kIdent, "a"}, A{kIdent, "b"}), A{kIdent, "c"}));
  TestParsingMethod(
      "a-b-c-d", ParseSummation,
      A(kSub, "a-b-c-d",
        A(kSub, "a-b-c", A(kSub, "a-b", A{kIdent, "a"}, A{kIdent, "b"}), A{kIdent, "c"}),
        A{kIdent, "d"}));

  //// Production -> Resolution
  // TestLLParse("a::b * c::d", ParseProduction, A{kMul, "a::b*c::d", A{kIdent, "a"},
  // A{kIdent, "b"}});

  //// Production -> Postfix
  // TestLLParse("a::b * c::d", ParseProduction, A{kMul, "a::b*c::d", A{kIdent, "a"},
  // A{kIdent, "b"}});

  //// Production -> MemberAccess
  // TestLLParse("a::b * c::d", ParseProduction, A{kMul, "a::b*c::d", A{kIdent, "a"},
  // A{kIdent, "b"}});

  //// Production -> Prefix
  // TestLLParse("a::b * c::d", ParseProduction, A{kMul, "a::b*c::d", A{kIdent, "a"},
  // A{kIdent, "b"}});

  /// Summation -> Production
}

TEST(UtParserPrimaryExprGrammar, ParseListFold) {
  TestParsingMethod("a,b", ParseListFold, A{kComma, "a,b", A{kIdent, "a"}, A{kIdent, "b"}});
  TestParsingMethod(
      "a,b,c", ParseListFold,
      A(kComma, "a,b,c", A(kComma, "a,b", A{kIdent, "a"}, A{kIdent, "b"}), A{kIdent, "c"}));
  TestParsingMethod(
      "a,b,c,d", ParseListFold,
      A(kComma, "a,b,c,d",
        A(kComma, "a,b,c", A(kComma, "a,b", A{kIdent, "a"}, A{kIdent, "b"}), A{kIdent, "c"}),
        A{kIdent, "d"}));

  //// Production -> Resolution
  // TestLLParse("a::b * c::d", ParseProduction, A{kMul, "a::b*c::d", A{kIdent, "a"},
  // A{kIdent, "b"}});

  //// Production -> Postfix
  // TestLLParse("a::b * c::d", ParseProduction, A{kMul, "a::b*c::d", A{kIdent, "a"},
  // A{kIdent, "b"}});

  //// Production -> MemberAccess
  // TestLLParse("a::b * c::d", ParseProduction, A{kMul, "a::b*c::d", A{kIdent, "a"},
  // A{kIdent, "b"}});

  //// Production -> Prefix
  // TestLLParse("a::b * c::d", ParseProduction, A{kMul, "a::b*c::d", A{kIdent, "a"},
  // A{kIdent, "b"}});

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