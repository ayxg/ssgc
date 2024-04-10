//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
// Copyright 2024 Anton Yashchenko
// Licensed under the Apache License, Version 2.0(the "License");
//---------------------------------------------------------------------------//
// Author(s): Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.acpp.dev
//---------------------------------------------------------------------------//
// Project: C& Programming Language Environment
// Directory: ut-cand-official-compiler
// File: ut_caoco_parser_basic.h
//---------------------------------------------------------------------------//
#ifndef HEADER_GUARD_CALE_UT_CAND_OFFICIAL_COMPILER_UT_CAOCO_PARSER_BASIC_H
#define HEADER_GUARD_CALE_UT_CAND_OFFICIAL_COMPILER_UT_CAOCO_PARSER_BASIC_H
//---------------------------------------------------------------------------//
// Brief: brief
//---------------------------------------------------------------------------//
// Includes:
#include "cppsextended.h"
#include "minitest.h"
// Testing:
#include "caoco_lexer.h"
#include "caoco_parser.h"
//---------------------------------------------------------------------------//

namespace unit_test_parser_basic {
using namespace caoco;
bool CompareAst(const Ast& node1, const Ast& node2) {
  // Compare node types
  if (node1.Type() != node2.Type()) {
    return false;
  }

  // Compare node values
  if (node1.Literal() != node2.Literal()) {
    return false;
  }

  // Compare number of Children
  if (node1.Children().size() != node2.Children().size()) {
    return false;
  }

  // Recursively compare Children
  for (size_t i = 0; i < node1.Children().size(); ++i) {
    if (!CompareAst(node1[i], node2[i])) return false;
  }

  // If all checks pass, the ASTs are equal
  return true;
}

inline void TestAndComparePrimaryExprClosureParser(const std::string& test_name,
                                                   const Ast& expected_ast,
                                                   const std::string& code) {
  auto expected_source = Lexer::Lex(code);
  ASSERT_TRUE(expected_source.Valid());
  auto source = expected_source.Extract();
  auto parse_result = parser::PrimaryExprClosureParser::Perform(
      {source.cbegin(), source.cend()});

  ASSERT_TRUE(parse_result.Valid());
  Ast ast = parse_result.Extract();
  // PrintAst(ast);
  bool compared = CompareAst(ast, expected_ast);
  EXPECT_TRUE_LOG(compared, test_name,
                  test_name + ":Ast is not equal to expected format.");
}

inline void TestInternalParserMethod(
    const std::string& code, std::function<ParseResultWithOffset(TkCursor)> fn,
    const std::string& test_name) {
  auto expected_source = Lexer::Lex(code);
  ASSERT_TRUE(expected_source.Valid());
  auto source = expected_source.Extract();
  auto parse_result = ParseTokens({source.cbegin(), source.cend()});
  ASSERT_TRUE(parse_result.Valid());
}

}  // namespace unit_test_parser_basic

INLINE_MINITEST(Test_ParserBasics, TestCase_SingleOperand) {
  using namespace caoco;
  auto expected_source = Lexer::Lex(
      "1 1.1 'string literal' '\\\'' alnumus 1c 1b 1u none ");  // TODO: #false
                                                                // #true
  ASSERT_TRUE(expected_source.Valid());
  auto source = expected_source.Extract();
  // Operands.
  std::vector<Ast> expected_result = {
      Ast(eAst::kNumberLiteral, "1"),
      Ast(eAst::kDoubleLiteral, "1.1"),
      Ast(eAst::kStringLiteral, "'string literal'"),
      Ast(eAst::kStringLiteral, "'\\\''"),
      Ast(eAst::kIdentifier, "alnumus"),
      Ast(eAst::kByteLiteral, "1c"),
      Ast(eAst::kBoolLiteral, "1b"),
      Ast(eAst::kUnsignedLiteral, "1u"),
      Ast(eAst::kNoneLiteral, "none"),
  };

  auto num_lit = parser::ParseOperand({source.cbegin(), source.cend()});
  ASSERT_TRUE(num_lit.Valid());
  EXPECT_EQ(num_lit.Value().Type(), expected_result[0].Type());
  EXPECT_EQ(num_lit.Value().Literal(), expected_result[0].Literal());

  auto real_lit = parser::ParseOperand(num_lit.Always());
  ASSERT_TRUE(real_lit.Valid());
  EXPECT_EQ(real_lit.Value().Type(), expected_result[1].Type());
  EXPECT_EQ(real_lit.Value().Literal(), expected_result[1].Literal());

  auto str_lit = parser::ParseOperand(real_lit.Always());
  ASSERT_TRUE(str_lit.Valid());
  EXPECT_EQ(str_lit.Value().Type(), expected_result[2].Type());
  EXPECT_EQ(str_lit.Value().Literal(), expected_result[2].Literal());

  auto str_lit2 = parser::ParseOperand(str_lit.Always());
  ASSERT_TRUE(str_lit2.Valid());
  EXPECT_EQ(str_lit2.Value().Type(), expected_result[3].Type());
  EXPECT_EQ(str_lit2.Value().Literal(), expected_result[3].Literal());

  auto alnumus_lit = parser::ParseOperand(str_lit2.Always());
  ASSERT_TRUE(alnumus_lit.Valid());
  EXPECT_EQ(alnumus_lit.Value().Type(), expected_result[4].Type());
  EXPECT_EQ(alnumus_lit.Value().Literal(), expected_result[4].Literal());

  auto byte_lit = parser::ParseOperand(alnumus_lit.Always());
  ASSERT_TRUE(byte_lit.Valid());
  EXPECT_EQ(byte_lit.Value().Type(), expected_result[5].Type());
  EXPECT_EQ(byte_lit.Value().Literal(), expected_result[5].Literal());

  auto bit_lit = parser::ParseOperand(byte_lit.Always());
  ASSERT_TRUE(bit_lit.Valid());
  EXPECT_EQ(bit_lit.Value().Type(), expected_result[6].Type());
  EXPECT_EQ(bit_lit.Value().Literal(), expected_result[6].Literal());

  auto unsigned_lit = parser::ParseOperand(bit_lit.Always());
  ASSERT_TRUE(unsigned_lit.Valid());
  EXPECT_EQ(unsigned_lit.Value().Type(), expected_result[7].Type());
  EXPECT_EQ(unsigned_lit.Value().Literal(), expected_result[7].Literal());

  auto none_lit = parser::ParseOperand(unsigned_lit.Always());
  ASSERT_TRUE(none_lit.Valid());
  EXPECT_EQ(none_lit.Value().Type(), expected_result[8].Type());
  EXPECT_EQ(none_lit.Value().Literal(), expected_result[8].Literal());
}
INLINE_END_MINITEST;
REGISTER_INLINE_TEST_CASE(Test_ParserBasics, TestCase_SingleOperand);

INLINE_MINITEST(Test_ParserBasics, TestCase_ValueExpr) {
  using namespace caoco;
  using namespace unit_test_parser_basic;

  TestAndComparePrimaryExprClosureParser("Single Number",
                                         Ast(eAst::kNumberLiteral, "1"), "1");
  TestAndComparePrimaryExprClosureParser(
      "Single Real", Ast(eAst::kDoubleLiteral, "1.1"), "1.1");
  TestAndComparePrimaryExprClosureParser(
      "Single String", Ast(eAst::kStringLiteral, "'string literal'"),
      "'string literal'");
  TestAndComparePrimaryExprClosureParser(
      "Single String Escaped", Ast(eAst::kStringLiteral, "'\\\''"), "'\\\''");
  TestAndComparePrimaryExprClosureParser(
      "Single Identifier", Ast(eAst::kIdentifier, "alnumus"), "alnumus");
  TestAndComparePrimaryExprClosureParser("Single Byte",
                                         Ast(eAst::kByteLiteral, "1c"), "1c");
  TestAndComparePrimaryExprClosureParser("Single Bit",
                                         Ast(eAst::kBoolLiteral, "1b"), "1b");
  TestAndComparePrimaryExprClosureParser(
      "Single Unsigned", Ast(eAst::kUnsignedLiteral, "1u"), "1u");
  TestAndComparePrimaryExprClosureParser(
      "Single None Literal", Ast(eAst::kNoneLiteral, "none"), "none");

  TestAndComparePrimaryExprClosureParser("Operand in Parenthesis",
                                         Ast(eAst::kNumberLiteral, "1"), "(1)");

  TestAndComparePrimaryExprClosureParser(
      "Binary Addition",
      Ast(eAst::kAddition, "+", Ast(eAst::kNumberLiteral, "1"),
          Ast(eAst::kNumberLiteral, "1")),
      "1 + 1");

  TestAndComparePrimaryExprClosureParser(
      "Binary Diff Priority",
      Ast(eAst::kAddition, "+", Ast(eAst::kNumberLiteral, "1"),
          Ast(eAst::kMultiplication, "*", Ast(eAst::kNumberLiteral, "2"),
              Ast(eAst::kNumberLiteral, "3"))),
      "1 + 2 * 3");

  TestAndComparePrimaryExprClosureParser(
      "Binary Left Associative",
      Ast(eAst::kSubtraction, "-",
          Ast(eAst::kAddition, "+", Ast(eAst::kNumberLiteral, "1"),
              Ast(eAst::kNumberLiteral, "2")),
          Ast(eAst::kNumberLiteral, "3")),
      "1 + 2 - 3");

  TestAndComparePrimaryExprClosureParser(
      "Binary Right Associative . member acces operator.",
      Ast(eAst::kPeriod, ".",
          Ast(eAst::kPeriod, ".",
              Ast(eAst::kPeriod, ".", Ast(eAst::kIdentifier, "a"),
                  Ast(eAst::kIdentifier, "b")),
              Ast(eAst::kIdentifier, "c")),
          Ast(eAst::kIdentifier, "d")),
      "a.b.c.d");

  TestAndComparePrimaryExprClosureParser(
      "Unary Postfix", Ast(eAst::kIncrement, "++", Ast(eAst::kIdentifier, "a")),
      "a++");

  TestAndComparePrimaryExprClosureParser(
      "Parenthesis is resolved first (1+2)*3",
      Ast(eAst::kMultiplication, "*",
          Ast(eAst::kAddition, "+", Ast(eAst::kNumberLiteral, "1"),
              Ast(eAst::kNumberLiteral, "2")),
          Ast(eAst::kNumberLiteral, "3")),
      "(1+2)*3");

  TestAndComparePrimaryExprClosureParser(
      "Unary Prefix", Ast(eAst::kNegation, "!", Ast(eAst::kIdentifier, "a")),
      "!a");

  TestAndComparePrimaryExprClosureParser(
      "Unary Prefix Repeated",
      Ast(eAst::kNegation, "!",
          Ast(eAst::kNegation, "!", Ast(eAst::kIdentifier, "a"))),
      "!!a");

  TestAndComparePrimaryExprClosureParser(
      "Assingment is right associative",
      Ast(eAst::kSimpleAssignment, "=", Ast(eAst::kIdentifier, "a"),
          Ast(eAst::kSimpleAssignment, "=", Ast(eAst::kIdentifier, "b"),
              Ast(eAst::kIdentifier, "c"))),
      "a=b=c");

  TestAndComparePrimaryExprClosureParser(
      "Prefix Unary then Binary",
      Ast(eAst::kAddition, "+",
          Ast(eAst::kNegation, "!", Ast(eAst::kNumberLiteral, "1")),
          Ast(eAst::kNumberLiteral, "2")),
      "!1+2");

  TestAndComparePrimaryExprClosureParser(
      "Prefix Unary then Higher Priority Member Access",
      Ast(eAst::kNegation, "!",
          Ast(eAst::kPeriod, ".", Ast(eAst::kIdentifier, "a"),
              Ast(eAst::kIdentifier, "b"))),
      "!a.b");

  TestAndComparePrimaryExprClosureParser(
      "Prefix Unary After Binary 1+!2",
      Ast(eAst::kAddition, "+", Ast(eAst::kNumberLiteral, "1"),
          Ast(eAst::kNegation, "!", Ast(eAst::kNumberLiteral, "2"))),
      "1+!2");

  TestAndComparePrimaryExprClosureParser(
      "Empty Function Call",
      Ast(eAst::kFunctionCall, "", Ast(eAst::kIdentifier, "a"),
          Ast(eAst::kArguments, "")),
      "a()");

  TestAndComparePrimaryExprClosureParser(
      "Prefix Unary then Function Call",
      Ast(eAst::kNegation, "!",
          Ast(eAst::kFunctionCall, "", Ast(eAst::kIdentifier, "a"),
              Ast(eAst::kArguments, ""))),
      "!a()");

  TestAndComparePrimaryExprClosureParser(
      "Repeated Prefix and Repeated Function Call",
      Ast(eAst::kNegation, "!",
          Ast(eAst::kNegation, "!",
              Ast(eAst::kFunctionCall, "",
                  Ast(eAst::kFunctionCall, "", Ast(eAst::kIdentifier, "a"),
                      Ast(eAst::kArguments, "")),
                  Ast(eAst::kArguments, "")))),
      "!!a()()");

  TestAndComparePrimaryExprClosureParser(
      "Function Call then Binary",
      Ast(eAst::kAddition, "+",
          Ast(eAst::kFunctionCall, "", Ast(eAst::kIdentifier, "a"),
              Ast(eAst::kArguments, "")),
          Ast(eAst::kNumberLiteral, "2")),
      "a()+2");

  TestAndComparePrimaryExprClosureParser(
      "Function Call then Member Access",
      Ast(eAst::kPeriod, ".",
          Ast(eAst::kFunctionCall, "", Ast(eAst::kIdentifier, "a"),
              Ast(eAst::kArguments, "")),
          Ast(eAst::kIdentifier, "b")),
      "a().b");

  TestAndComparePrimaryExprClosureParser(
      "Member Access then Function Call",
      Ast(eAst::kFunctionCall, "",
          Ast(eAst::kPeriod, ".", Ast(eAst::kIdentifier, "a"),
              Ast(eAst::kIdentifier, "b")),
          Ast(eAst::kArguments, "")),
      "a.b()");

  TestAndComparePrimaryExprClosureParser(
      "Binary Then Function Call 1 + a()",
      Ast(eAst::kAddition, "+", Ast(eAst::kNumberLiteral, "1"),
          Ast(eAst::kFunctionCall, "", Ast(eAst::kIdentifier, "a"),
              Ast(eAst::kArguments, ""))),
      "1+a()");

  // a.b().c is parsed as (a.b()).c and not as a.(b().c) because of
  // left-to-right
  // associativity of member access. source: cppreference.com
  TestAndComparePrimaryExprClosureParser(
      "Repeated Member Access with internal Function Call",
      Ast(eAst::kPeriod, ".",
          Ast(eAst::kPeriod, ".",
              Ast(eAst::kFunctionCall, "",
                  Ast(eAst::kPeriod, ".", Ast(eAst::kIdentifier, "a"),
                      Ast(eAst::kIdentifier, "b")),
                  Ast(eAst::kArguments, "")),
              Ast(eAst::kIdentifier, "c")),
          Ast(eAst::kIdentifier, "d")),
      "a.b().c.d");

  TestAndComparePrimaryExprClosureParser(
      "Complex Expression: foo.bar()*1+1",
      Ast(eAst::kAddition, "+",
          Ast(eAst::kMultiplication, "*",
              Ast(eAst::kFunctionCall, "",
                  Ast(eAst::kPeriod, ".", Ast(eAst::kIdentifier, "foo"),
                      Ast(eAst::kIdentifier, "bar")),
                  Ast(eAst::kArguments, "")),
              Ast(eAst::kNumberLiteral, "1")),
          Ast(eAst::kNumberLiteral, "1")),
      "foo.bar()*1+1");

  TestAndComparePrimaryExprClosureParser(
      "Complex Expression: (foo.bar() + 1) * 1",
      Ast(eAst::kMultiplication, "*",
          Ast(eAst::kAddition, "+",
              Ast(eAst::kFunctionCall, "",
                  Ast(eAst::kPeriod, ".", Ast(eAst::kIdentifier, "foo"),
                      Ast(eAst::kIdentifier, "bar")),
                  Ast(eAst::kArguments, "")),
              Ast(eAst::kNumberLiteral, "1")),
          Ast(eAst::kNumberLiteral, "1")),
      "(foo.bar() + 1) * 1");

  TestAndComparePrimaryExprClosureParser(
      "Assingment Experssion : foo = 1 + 2",
      Ast(eAst::kSimpleAssignment, "=", Ast(eAst::kIdentifier, "foo"),
          Ast(eAst::kAddition, "+", Ast(eAst::kNumberLiteral, "1"),
              Ast(eAst::kNumberLiteral, "2"))),
      "foo = 1 + 2");

  TestAndComparePrimaryExprClosureParser(
      "Function Call After Binary Left Associative:  e + d + c + b()",
      Ast(eAst::kAddition, "+",
          Ast(eAst::kAddition, "+",
              Ast(eAst::kAddition, "+", Ast(eAst::kIdentifier, "e"),
                  Ast(eAst::kIdentifier, "d")),
              Ast(eAst::kIdentifier, "c")),
          Ast(eAst::kFunctionCall, "", Ast(eAst::kIdentifier, "b"),
              Ast(eAst::kArguments, ""))),
      "e + d + c + b()");

  TestAndComparePrimaryExprClosureParser(
      "Function Call With Arguments",
      Ast(eAst::kFunctionCall, "", Ast(eAst::kIdentifier, "foo"),
          Ast(eAst::kArguments, "", Ast(eAst::kNumberLiteral, "1"),
              Ast(eAst::kNumberLiteral, "2"), Ast(eAst::kNumberLiteral, "3"))),
      "foo(1, 2, 3)");

  TestAndComparePrimaryExprClosureParser(
      "Indexing",
      Ast(eAst::kIndexOperator, "", Ast(eAst::kIdentifier, "foo"),
          Ast(eAst::kArguments, "", Ast(eAst::kNumberLiteral, "1"),
              Ast(eAst::kNumberLiteral, "2"), Ast(eAst::kNumberLiteral, "3"))),
      "foo[1, 2, 3]");

  TestAndComparePrimaryExprClosureParser(
      "Listing Operator foo{1,2,3}",
      Ast(eAst::kListingOperator, "", Ast(eAst::kIdentifier, "foo"),
          Ast(eAst::kArguments, "", Ast(eAst::kNumberLiteral, "1"),
              Ast(eAst::kNumberLiteral, "2"), Ast(eAst::kNumberLiteral, "3"))),
      "foo{1,2,3}");

  TestAndComparePrimaryExprClosureParser(
      "Access Operator foo::bar",
      Ast(eAst::kDoubleColon, "::", Ast(eAst::kIdentifier, "foo"),
          Ast(eAst::kIdentifier, "bar")),
      "foo::bar");

  TestAndComparePrimaryExprClosureParser("Unary Minus",
                                         Ast(eAst::kNumberLiteral, "-1"), "-1");

  TestAndComparePrimaryExprClosureParser(
      "Unary Minus in expression",
      Ast(eAst::kAddition, "+", Ast(eAst::kNumberLiteral, "-1"),
          Ast(eAst::kNumberLiteral, "2")),
      "-1+2");

  TestAndComparePrimaryExprClosureParser(
      "Unary Minus in expression -1 - -1;",
      Ast(eAst::kSubtraction, "-", Ast(eAst::kNumberLiteral, "-1"),
          Ast(eAst::kNumberLiteral, "-1")),
      "-1 - -1");
}
INLINE_END_MINITEST;
REGISTER_INLINE_TEST_CASE(Test_ParserBasics, TestCase_ValueExpr);

INLINE_MINITEST(Test_ParserBasics, TestCase_PrimaryStatement) {
  using namespace caoco;
  using namespace unit_test_parser_basic;
  // test a primary expr statement starting with a subexpression.
  TestInternalParserMethod("(1+2)*a;", parser::ParsePrimaryStatement,
                           "Primary Statement");
}
INLINE_END_MINITEST;
REGISTER_INLINE_TEST_CASE(Test_ParserBasics, TestCase_PrimaryStatement);
// Declarations

INLINE_MINITEST(Test_ParserBasics,
                TestCase_VariableDeclarationNoTypeNoAssignNoMod) {
  using namespace caoco;
  using namespace unit_test_parser_basic;
  TestInternalParserMethod("def@Foo;", parser::ParseVariableDecl,
                           "VariableDeclarationNoTypeNoAssignNoMod");
}
INLINE_END_MINITEST;
REGISTER_INLINE_TEST_CASE(Test_ParserBasics,
                          TestCase_VariableDeclarationNoTypeNoAssignNoMod);

INLINE_MINITEST(Test_ParserBasics, TestCase_VariableDeclarationNoTypeNoAssign) {
  using namespace caoco;
  using namespace unit_test_parser_basic;
  TestInternalParserMethod("const def@Foo;", parser::ParseVariableDecl,
                           "VariableDeclarationNoTypeNoAssign");
}
INLINE_END_MINITEST;
REGISTER_INLINE_TEST_CASE(Test_ParserBasics,
                          TestCase_VariableDeclarationNoTypeNoAssign);

INLINE_MINITEST(Test_ParserBasics, TestCase_VariableDeclarationNoAssign) {
  using namespace caoco;
  using namespace unit_test_parser_basic;
  TestInternalParserMethod("const def str@Foo;", parser::ParseVariableDecl,
                           "VariableDeclarationNoAssign");
}
INLINE_END_MINITEST;
REGISTER_INLINE_TEST_CASE(Test_ParserBasics, TestCase_VariableDeclarationNoAssign);

INLINE_MINITEST(Test_ParserBasics, TestCase_VariableDefinition) {
  using namespace caoco::parser;
  using namespace unit_test_parser_basic;
  TestInternalParserMethod("const def str@Foo: 42;", ParseVariableDecl,
                           "VariableDefinition");
}
INLINE_END_MINITEST;
REGISTER_INLINE_TEST_CASE(
  Test_ParserBasics, TestCase_VariableDefinition);

INLINE_MINITEST(Test_ParserBasics, TestCase_TypeAlias) {
  using namespace caoco::parser;
  using namespace unit_test_parser_basic;
  TestInternalParserMethod("use @MyInteger: int;", ParseUsingDecl, "TypeAlias");
}
INLINE_END_MINITEST;
REGISTER_INLINE_TEST_CASE(
  Test_ParserBasics, TestCase_TypeAlias);

INLINE_MINITEST(Test_ParserBasics, TestCase_LibraryNamespaceInclusion) {
  using namespace caoco::parser;
  using namespace unit_test_parser_basic;
  TestInternalParserMethod("use lib my_math_lib;", ParseUsingDecl,
                           "LibraryNamespaceInclusion");
}
INLINE_END_MINITEST;
REGISTER_INLINE_TEST_CASE(
  Test_ParserBasics, TestCase_LibraryNamespaceInclusion);

INLINE_MINITEST(Test_ParserBasics, TestCase_NamespaceInclusion) {
  using namespace caoco::parser;
  using namespace unit_test_parser_basic;
  TestInternalParserMethod("use namespace my_ns;", ParseUsingDecl,
                           "NamespaceInclusion");
}
INLINE_END_MINITEST;
REGISTER_INLINE_TEST_CASE(
  Test_ParserBasics, TestCase_NamespaceInclusion);

INLINE_MINITEST(Test_ParserBasics, TestCase_ObjectInclusion) {
  using namespace caoco::parser;
  using namespace unit_test_parser_basic;
  TestInternalParserMethod("use my_ns::Foo;", ParseUsingDecl,
                           "ObjectInclusion");
}
INLINE_END_MINITEST;
REGISTER_INLINE_TEST_CASE(
  Test_ParserBasics, TestCase_ObjectInclusion);

INLINE_MINITEST(Test_ParserBasics, TestCase_TypeInclusion) {
  using namespace caoco::parser;
  using namespace unit_test_parser_basic;
  TestInternalParserMethod("use@MyFooType: my_ns::Foo;", ParseUsingDecl,
                           "TypeInclusion");
}
INLINE_END_MINITEST;
REGISTER_INLINE_TEST_CASE(
  Test_ParserBasics, TestCase_TypeInclusion);

INLINE_MINITEST(Test_ParserBasics, TestCase_ObjectInclusionFromLibrary) {
  using namespace caoco::parser;
  using namespace unit_test_parser_basic;
  TestInternalParserMethod("use lib my_math_lib::add;", ParseUsingDecl,
                           "ObjectInclusionFromLibrary");
}
INLINE_END_MINITEST;
REGISTER_INLINE_TEST_CASE(
  Test_ParserBasics, TestCase_ObjectInclusionFromLibrary);

INLINE_MINITEST(Test_ParserBasics, TestCase_TypeInclusionFromLibrary) {
  using namespace caoco::parser;
  using namespace unit_test_parser_basic;
  TestInternalParserMethod("use @MyAddMethodImpl: lib my_math_lib::add;",
                           ParseUsingDecl, "TypeInclusionFromLibrary");
}
INLINE_END_MINITEST;
REGISTER_INLINE_TEST_CASE(
  Test_ParserBasics, TestCase_TypeInclusionFromLibrary);

INLINE_MINITEST(Test_ParserBasics, TestCase_TypeImportDeclaration) {
  using namespace caoco::parser;
  using namespace unit_test_parser_basic;
  TestInternalParserMethod("import foo;", ParseImportDecl, "ImportDeclaration");
}
INLINE_END_MINITEST;
REGISTER_INLINE_TEST_CASE(
  Test_ParserBasics, TestCase_TypeImportDeclaration);

INLINE_MINITEST(Test_ParserBasics, TestCase_MethodDeclImplicitVoidArgNoRet) {
  using namespace caoco::parser;
  using namespace unit_test_parser_basic;
  TestInternalParserMethod("fn@add;", ParseMethodDecl,
                           "MethodDeclImplicitVoidArgNoRet");
}
INLINE_END_MINITEST;
REGISTER_INLINE_TEST_CASE(
  Test_ParserBasics, TestCase_MethodDeclImplicitVoidArgNoRet);

INLINE_MINITEST(Test_ParserBasics, TestCase_MethodDeclImplicitVoidArgNoRet2) {
  using namespace caoco::parser;
  using namespace unit_test_parser_basic;
  TestInternalParserMethod("fn@add();", ParseMethodDecl,
                           "MethodDeclImplicitVoidArgNoRet");
}
INLINE_END_MINITEST;
  REGISTER_INLINE_TEST_CASE(
  Test_ParserBasics, TestCase_MethodDeclImplicitVoidArgNoRet2);

INLINE_MINITEST(Test_ParserBasics, TestCase_MethodDeclImplicitVoidArgAnyRet) {
  using namespace caoco::parser;
  using namespace unit_test_parser_basic;
  TestInternalParserMethod("fn@add>;", ParseMethodDecl,
                           "TestCaseMethodDeclImplicitVoidArgAnyRet");
}
INLINE_END_MINITEST;
REGISTER_INLINE_TEST_CASE(
  Test_ParserBasics, TestCase_MethodDeclImplicitVoidArgAnyRet);

INLINE_MINITEST(Test_ParserBasics, TestCase_MethodDeclImplicitVoidArgAnyRet2) {
  using namespace caoco::parser;
  using namespace unit_test_parser_basic;
  TestInternalParserMethod("fn@add()>;", ParseMethodDecl,
                           "TestCaseMethodDeclImplicitVoidArgAnyRet2");
}
INLINE_END_MINITEST;
REGISTER_INLINE_TEST_CASE(
  Test_ParserBasics, TestCase_MethodDeclImplicitVoidArgAnyRet2);

INLINE_MINITEST(Test_ParserBasics, TestCase_MethodDeclArgNoRet) {
  using namespace caoco::parser;
  using namespace unit_test_parser_basic;
  TestInternalParserMethod("fn@add(a,b);", ParseMethodDecl,
                           "TestCaseMethodDeclArgNoRet");
}
INLINE_END_MINITEST;
REGISTER_INLINE_TEST_CASE(
  Test_ParserBasics, TestCase_MethodDeclArgNoRet);

INLINE_MINITEST(Test_ParserBasics, TestCase_MethodDeclArgAnyRet) {
  using namespace caoco::parser;
  using namespace unit_test_parser_basic;
  TestInternalParserMethod("fn@add(a,b)>;", ParseMethodDecl,
                           "TestCaseMethodDeclArgAnyRet");
}
INLINE_END_MINITEST;
REGISTER_INLINE_TEST_CASE(
  Test_ParserBasics, TestCase_MethodDeclArgAnyRet);

INLINE_MINITEST(Test_ParserBasics, TestCase_MethodDeclArgIdentifiedAnyRet) {
  using namespace caoco::parser;
  using namespace unit_test_parser_basic;
  TestInternalParserMethod("fn@add(@a,@b)>;", ParseMethodDecl,
                           "TestCaseMethodDeclArgAnyRet");
}
INLINE_END_MINITEST;
REGISTER_INLINE_TEST_CASE(
  Test_ParserBasics, TestCase_MethodDeclArgIdentifiedAnyRet);

INLINE_MINITEST(Test_ParserBasics, TestCase_MethodDeclArgsTypedRet) {
  using namespace caoco::parser;
  using namespace unit_test_parser_basic;
  TestInternalParserMethod("fn@add(@a,@b)>int;", ParseMethodDecl,
                           "TestCaseMethodDeclArgsTypedRet");
}
INLINE_END_MINITEST;
REGISTER_INLINE_TEST_CASE(
  Test_ParserBasics, TestCase_MethodDeclArgsTypedRet);

INLINE_MINITEST(Test_ParserBasics, TestCase_MethodDeclTypedArgsTypedRet) {
  using namespace caoco::parser;
  using namespace unit_test_parser_basic;
  TestInternalParserMethod("fn@add(int @a,int @b)>int;", ParseMethodDecl,
                           "TestCaseMethodDeclTypedArgsTypedRet");
}
INLINE_END_MINITEST;
REGISTER_INLINE_TEST_CASE(
  Test_ParserBasics, TestCase_MethodDeclTypedArgsTypedRet);

INLINE_MINITEST(Test_ParserBasics,
                TestCase_MethodDeclTypedArgsTypedRetWithModifiers) {
  using namespace caoco::parser;
  using namespace unit_test_parser_basic;
  TestInternalParserMethod("fn@add(const int @a,const int @b)>const int;",
                           ParseMethodDecl,
                           "TestCaseMethodDeclTypedArgsTypedRet");
}
INLINE_END_MINITEST;
REGISTER_INLINE_TEST_CASE(
  Test_ParserBasics, TestCase_MethodDeclTypedArgsTypedRetWithModifiers);

INLINE_MINITEST(Test_ParserBasics, TestCase_ClassDecl) {
  using namespace caoco::parser;
  using namespace unit_test_parser_basic;
  TestInternalParserMethod("class@Husky;", ParseClassDecl,
                           "TestCaseMethodDeclTypedArgsTypedRet");
}
INLINE_END_MINITEST;
REGISTER_INLINE_TEST_CASE(
  Test_ParserBasics, TestCase_ClassDecl);

INLINE_MINITEST(Test_ParserBasics, TestCase_ClassDeclWithMod) {
  using namespace caoco::parser;
  using namespace unit_test_parser_basic;
  TestInternalParserMethod("const static class@Husky;", ParseClassDecl,
                           "TestCaseClassDeclWithMod");
}
INLINE_END_MINITEST;
REGISTER_INLINE_TEST_CASE(
  Test_ParserBasics, TestCase_ClassDeclWithMod);

INLINE_MINITEST(Test_ParserBasics, TestCase_LibWithMod) {
  using namespace caoco::parser;
  using namespace unit_test_parser_basic;
  TestInternalParserMethod("const static lib@MathLib;", ParseLibDecl,
                           "TestCaseCLibDeclWithMod");
}
INLINE_END_MINITEST;
REGISTER_INLINE_TEST_CASE(
  Test_ParserBasics, TestCase_LibWithMod);

INLINE_MINITEST(Test_ParserBasics, TestCase_LibWithModAndDefinition) {
  using namespace caoco::parser;
  using namespace unit_test_parser_basic;
  TestInternalParserMethod(
      "const static lib@MathLib:{const def str@Foo: 42;use @MyInteger: int;};",
      ParseLibDecl, "TestCaseCLibDeclWithMod");
}
INLINE_END_MINITEST;
REGISTER_INLINE_TEST_CASE(
  Test_ParserBasics, TestCase_LibWithModAndDefinition);

INLINE_MINITEST(Test_ParserBasics, TestCase_ClassWithModAndDefinition) {
  using namespace caoco::parser;
  using namespace unit_test_parser_basic;
  TestInternalParserMethod(
      "const static class@Husky:{const def str@Foo: 42;use @MyInteger: int;};",
      ParseClassDecl, "TestCaseClassDeclWithMod");
}
INLINE_END_MINITEST;
REGISTER_INLINE_TEST_CASE(
  Test_ParserBasics, TestCase_ClassWithModAndDefinition);

INLINE_MINITEST(Test_ParserBasics, TestCase_MethodDefinition) {
  using namespace caoco::parser;
  using namespace unit_test_parser_basic;
  TestInternalParserMethod(
      "fn@add(const int @a,const int @b)>const int:{a+b;};", ParseMethodDecl,
      "TestCaseMethodDefinition");
}
INLINE_END_MINITEST;
REGISTER_INLINE_TEST_CASE(
  Test_ParserBasics, TestCase_MethodDefinition);

INLINE_MINITEST(Test_ParserBasics, TestCase_MainDefinition) {
  using namespace caoco::parser;
  using namespace unit_test_parser_basic;
  TestInternalParserMethod("main(a,b):{a+b;};", ParseMainDecl,
                           "TestCaseMainDefinition");
}
INLINE_END_MINITEST;
REGISTER_INLINE_TEST_CASE(
  Test_ParserBasics, TestCase_MainDefinition);

// Pragmatic Statements: appears at top level, or in a library.
INLINE_MINITEST(Test_ParserBasics, TestCase_PragmaticDeclarations) {
  using namespace caoco::parser;
  using namespace unit_test_parser_basic;
  TestInternalParserMethod("const def str@Foo: 42;", ParsePragmaticStmt,
                           "VariableDefinition");
  TestInternalParserMethod("use @MyInteger: int;", ParsePragmaticStmt,
                           "TypeAlias");
  TestInternalParserMethod("use lib my_math_lib;", ParsePragmaticStmt,
                           "LibraryNamespaceInclusion");
  TestInternalParserMethod("use namespace my_ns;", ParsePragmaticStmt,
                           "NamespaceInclusion");
  TestInternalParserMethod("use my_ns::Foo;", ParsePragmaticStmt,
                           "ObjectInclusion");
  TestInternalParserMethod("use@MyFooType: my_ns::Foo;", ParsePragmaticStmt,
                           "TypeInclusion");
  TestInternalParserMethod("use lib my_math_lib::add;", ParsePragmaticStmt,
                           "ObjectInclusionFromLibrary");
  TestInternalParserMethod("use @MyAddMethodImpl: lib my_math_lib::add;",
                           ParsePragmaticStmt, "TypeInclusionFromLibrary");

  TestInternalParserMethod("const static lib@MathLib;", ParsePragmaticStmt,
                           "PragmaticDeclLibrary");
  TestInternalParserMethod("import foo;", ParsePragmaticStmt,
                           "ImportDeclaration");
  TestInternalParserMethod("fn@add;", ParsePragmaticStmt,
                           "MethodDeclImplicitVoidArgNoRet");
  TestInternalParserMethod("fn@add();", ParsePragmaticStmt,
                           "MethodDeclImplicitVoidArgNoRet");
  TestInternalParserMethod("fn@add>;", ParsePragmaticStmt,
                           "TestCaseMethodDeclImplicitVoidArgAnyRet");
  TestInternalParserMethod("fn@add()>;", ParsePragmaticStmt,
                           "TestCaseMethodDeclImplicitVoidArgAnyRet2");
  TestInternalParserMethod("fn@add(a,b)>;", ParsePragmaticStmt,
                           "TestCaseMethodDeclArgAnyRet");
  TestInternalParserMethod("fn@add(@a,@b)>int;", ParsePragmaticStmt,
                           "TestCaseMethodDeclArgsTypedRet");
  TestInternalParserMethod("fn@add(int @a,int @b)>int;", ParsePragmaticStmt,
                           "TestCaseMethodDeclTypedArgsTypedRet");
  TestInternalParserMethod("fn@add(const int @a,const int @b)>const int;",
                           ParsePragmaticStmt,
                           "TestCaseMethodDeclTypedArgsTypedRet");
  TestInternalParserMethod("class@Husky;", ParsePragmaticStmt,
                           "TestCaseMethodDeclTypedArgsTypedRet");

  TestInternalParserMethod("const static class@Husky;", ParsePragmaticStmt,
                           "TestCaseClassDeclWithMod");
  TestInternalParserMethod("const static lib@MathLib;", ParsePragmaticStmt,
                           "TestCaseCLibDeclWithMod");
}
INLINE_END_MINITEST;
REGISTER_INLINE_TEST_CASE(Test_ParserBasics, TestCase_PragmaticDeclarations);

INLINE_MINITEST(Test_ParserBasics, TestCase_ParseProgramWithDeclrations) {
  using namespace caoco::parser;
  using namespace unit_test_parser_basic;
  TestInternalParserMethod(
      "import foo;"
      "const static lib@MathLib;"
      "use @MyAddMethodImpl: lib MathLib::add;"
      "const def str@Foo: 42;"
      "fn@add(const int @a,const int @b)>const int;"
      "const static class @Husky;",
      ParseProgram, "TestCaseParseProgramWithDeclrations");
}
INLINE_END_MINITEST;
REGISTER_INLINE_TEST_CASE(Test_ParserBasics,
                          TestCase_ParseProgramWithDeclrations);

// Control Flow Statements
INLINE_MINITEST(Test_ParserBasics, TestCase_ReturnStatement) {
  using namespace caoco::parser;
  using namespace unit_test_parser_basic;
  TestInternalParserMethod("return a + b;", ParseReturnStmt,
                           "TestCaseReturnStatement");
}
INLINE_END_MINITEST;
REGISTER_INLINE_TEST_CASE(Test_ParserBasics, TestCase_ReturnStatement);

INLINE_MINITEST(Test_ParserBasics, TestCase_IfStatement) {
  using namespace caoco::parser;
  using namespace unit_test_parser_basic;
  TestInternalParserMethod("if(a){ a + b; };", ParseIfDecl,
                           "TestCaseIfStatement");
}
INLINE_END_MINITEST;
REGISTER_INLINE_TEST_CASE(Test_ParserBasics, TestCase_IfStatement);

INLINE_MINITEST(Test_ParserBasics, TestCase_IfElseStatement) {
  using namespace caoco::parser;
  using namespace unit_test_parser_basic;
  TestInternalParserMethod("if(a){ a + b; }else{a;};", ParseIfDecl,
                           "TestCaseIfElseStatement");
}
INLINE_END_MINITEST;
REGISTER_INLINE_TEST_CASE(
  Test_ParserBasics, TestCase_IfElseStatement);

INLINE_MINITEST(Test_ParserBasics, TestCase_IfElifStatement) {
  using namespace caoco::parser;
  using namespace unit_test_parser_basic;
  TestInternalParserMethod("if(a){ a + b; }elif(b){a;};", ParseIfDecl,
                           "TestCaseIfElifStatement");
}
INLINE_END_MINITEST;
REGISTER_INLINE_TEST_CASE(
  Test_ParserBasics, TestCase_IfElifStatement);

INLINE_MINITEST(Test_ParserBasics, TestCase_IfElifElseStatement) {
  using namespace caoco::parser;
  using namespace unit_test_parser_basic;
  TestInternalParserMethod("if(a){ a + b; }elif(b){a;}else{b;};", ParseIfDecl,
                           "TestCaseIfElifElseStatement");
}
INLINE_END_MINITEST;
REGISTER_INLINE_TEST_CASE(
  Test_ParserBasics, TestCase_IfElifElseStatement);

// Loop Statements
INLINE_MINITEST(Test_ParserBasics, TestCase_WhileStatement) {
  using namespace caoco::parser;
  using namespace unit_test_parser_basic;
  TestInternalParserMethod("while(a){ a + b; };", ParseWhileDecl,
                           "TestCaseWhileStatement");
}
INLINE_END_MINITEST;
REGISTER_INLINE_TEST_CASE(Test_ParserBasics, TestCase_WhileStatement);

INLINE_MINITEST(Test_ParserBasics, TestCase_ForStatement) {
  using namespace caoco::parser;
  using namespace unit_test_parser_basic;
  TestInternalParserMethod("for(def@a:0;a!=end;a++){ a + b; };", ParseForDecl,
                           "TestCaseForStatement");
}
INLINE_END_MINITEST;
REGISTER_INLINE_TEST_CASE(Test_ParserBasics, TestCase_ForStatement);

// Animals Example Program.
INLINE_MINITEST(Test_ParserBasics, TestCase_AnimalsExampleProgram) {
  using namespace caoco;
  using namespace unit_test_parser_basic;
  TestInternalParserMethod(
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
      parser::ParseProgram, "TestCaseAnimalsExampleProgram");
}
INLINE_END_MINITEST;
REGISTER_INLINE_TEST_CASE(Test_ParserBasics, TestCase_AnimalsExampleProgram);

//---------------------------------------------------------------------------//
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
//---------------------------------------------------------------------------//
// Author(s): Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.acpp.dev
//---------------------------------------------------------------------------//
// Project: C& Programming Language Environment
// Directory: ut-cand-official-compiler
// File: ut_caoco_parser_basic.h
//---------------------------------------------------------------------------//
#endif HEADER_GUARD_CALE_UT_CAND_OFFICIAL_COMPILER_UT_CAOCO_PARSER_BASIC_H
//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//