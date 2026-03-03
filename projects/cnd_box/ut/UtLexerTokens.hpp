///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2026 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup cnd_unit_test
/// @brief [UtLexerTokens]
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup cnd_unit_test
/// @{
#pragma once
#include "frontend/lexer.hpp"
#include "minitest.hpp"

namespace cnd_unit_test::frontend::lexer {
using namespace cnd::frontend;
using cnd::eTk;
using cnd::eTkToCStr;
using cnd::Size;

void TestSingleToken(SrcView source, eTk expected_type, SrcView expected_literal, Size begcol = 1, Size endcol = 1,
                     Size begline = 1, Size endline = 1) {
  auto lex_result = Lexer::Lex(source);
  if (!lex_result) std::cout << lex_result.error().Format() << std::endl;
  ASSERT_TRUE(lex_result);

  // Check type.
  if (expected_type != eTk::kNONE)
    EXPECT_EQ_LOG(lex_result->front().Type(), expected_type,
                  std::format("Expected token type '{}' but got '{}'.", eTkToCStr(expected_type),
                              eTkToCStr(lex_result->front().Type())));

  // Check literal.
  if (expected_literal == "") expected_literal = source;
  EXPECT_EQ_LOG(
      lex_result->front().Literal(), expected_literal,
      std::format("Expected token literal '{}' but got '{}'.", expected_literal, lex_result->front().Literal()));

  // Check start column.
  EXPECT_EQ_LOG(lex_result->front().BegCol(), begcol,
                std::format("Expected token begin column '{}' but got '{}'.", begcol, lex_result->front().BegCol()));

  // Check end column.
  EXPECT_EQ_LOG(lex_result->front().EndCol(), endcol,
                std::format("Expected token end column '{}' but got '{}'.", endcol, lex_result->front().EndCol()));

  // Check begin line.
  EXPECT_EQ_LOG(lex_result->front().BegLine(), begline,
                std::format("Expected token begin line '{}' but got '{}'.", begline, lex_result->front().BegLine()));

  // Check end column.
  EXPECT_EQ_LOG(lex_result->front().EndLine(), endline,
                std::format("Expected token end line '{}' but got '{}'.", endline, lex_result->front().EndLine()));
}

// Expected literal is defaulted to input source.
void TestSingleToken(SrcView source, eTk expected_type, Size begcol, Size endcol = 1, Size begline = 1,
                     Size endline = 1) {
  auto lex_result = Lexer::Lex(source);
  if (!lex_result) std::cout << lex_result.error().Format() << std::endl;
  ASSERT_TRUE(lex_result);

  // Check type.
  if (expected_type != eTk::kNONE)
    EXPECT_EQ_LOG(lex_result->front().Type(), expected_type,
                  std::format("Expected token type '{}' but got '{}'.", eTkToCStr(expected_type),
                              eTkToCStr(lex_result->front().Type())));

  // Check literal.
  EXPECT_EQ_LOG(lex_result->front().Literal(), source,
                std::format("Expected token literal '{}' but got '{}'.", source, lex_result->front().Literal()));

  // Check start column.
  EXPECT_EQ_LOG(lex_result->front().BegCol(), begcol,
                std::format("Expected token begin column '{}' but got '{}'.", begcol, lex_result->front().BegCol()));

  // Check end column.
  EXPECT_EQ_LOG(lex_result->front().EndCol(), endcol,
                std::format("Expected token end column '{}' but got '{}'.", endcol, lex_result->front().EndCol()));

  // Check begin line.
  EXPECT_EQ_LOG(lex_result->front().BegLine(), begline,
                std::format("Expected token begin line '{}' but got '{}'.", begline, lex_result->front().BegLine()));

  // Check end column.
  EXPECT_EQ_LOG(lex_result->front().EndLine(), endline,
                std::format("Expected token end line '{}' but got '{}'.", endline, lex_result->front().EndLine()));
}

// Expect lines to be 1 and cols to be 1 and size of source + 1 by default. Expected literal is defaulted to source.
void TestSingleToken(SrcView source, eTk expected_type = eTk::kNONE) {
  auto lex_result = Lexer::Lex(source);
  if (!lex_result) std::cout << lex_result.error().Format() << std::endl;
  ASSERT_TRUE(lex_result);

  // Check type.
  if (expected_type != eTk::kNONE)
    EXPECT_EQ_LOG(lex_result->front().Type(), expected_type,
                  std::format("Expected token type '{}' but got '{}'.", eTkToCStr(expected_type),
                              eTkToCStr(lex_result->front().Type())));

  // Check literal.
  EXPECT_EQ_LOG(lex_result->front().Literal(), source,
                std::format("Expected token literal '{}' but got '{}'.", source, lex_result->front().Literal()));

  // Check start column.
  EXPECT_EQ_LOG(lex_result->front().BegCol(), 1,
                std::format("Expected token begin column '{}' but got '{}'.", 1, lex_result->front().BegCol()));

  // Check end column.
  EXPECT_EQ_LOG(
      lex_result->front().EndCol(), 1 + source.size(),
      std::format("Expected token end column '{}' but got '{}'.", 1 + source.size(), lex_result->front().EndCol()));

  // Check begin line.
  EXPECT_EQ_LOG(lex_result->front().BegLine(), 1,
                std::format("Expected token begin line '{}' but got '{}'.", 1, lex_result->front().BegLine()));

  // Check end column.
  EXPECT_EQ_LOG(lex_result->front().EndLine(), 1,
                std::format("Expected token end line '{}' but got '{}'.", 1, lex_result->front().EndLine()));
}

void TestCheckLexerErrorCode(SrcView source, cnd::cldev::clmsg::eClErr expected_code) {
  auto lex_result = Lexer::Lex(source);
  ASSERT_FALSE(lex_result);
  EXPECT_EQ_LOG(
      lex_result.error().GetLastMessageId().code, cnd::cldev::clmsg::GetClMsgIdOf(expected_code).code,
      std::format("Expected error code '{}' but got '{}'.", cnd::cldev::clmsg::GetClMsgIdOf(expected_code).code,
                  lex_result.error().GetLastMessageId().code));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Numeric literals.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(LexerTokens, LiteralU1) {
  TestSingleToken("1b", eTk::kLitU1);
  TestSingleToken("0b", eTk::kLitU1);
}

TEST(LexerTokens, LiteralU8) {
  for (int i = 0; i <= 9; i++) {
    TestSingleToken(std::to_string(i) + "B", eTk::kLitU8);
    TestSingleToken(std::to_string(i) + "u8", eTk::kLitU8);
  }
  for (int i = 10; i <= 99; i++) {
    TestSingleToken(std::to_string(i) + "B", eTk::kLitU8);
    TestSingleToken(std::to_string(i) + "u8", eTk::kLitU8);
  }
  for (int i = 100; i <= 127; i++) {
    TestSingleToken(std::to_string(i) + "B", eTk::kLitU8);
    TestSingleToken(std::to_string(i) + "u8", eTk::kLitU8);
  }
}

TEST(LexerTokens, LiteralU16) { TestSingleToken("123u16", eTk::kLitU16); }

TEST(LexerTokens, LiteralU32) {
  TestSingleToken("123u", eTk::kLitU32);
  TestSingleToken("123u32", eTk::kLitU32);
}

TEST(LexerTokens, LiteralU64) {
  TestSingleToken("123U", eTk::kLitU64);
  TestSingleToken("123u64", eTk::kLitU64);
}

TEST(LexerTokens, LiteralI8) {
  for (int i = 0; i <= 9; i++) {
    TestSingleToken(std::to_string(i) + "c", eTk::kLitI8);
    TestSingleToken(std::to_string(i) + "i8", eTk::kLitI8);
  }
  for (int i = 10; i <= 99; i++) {
    TestSingleToken(std::to_string(i) + "c", eTk::kLitI8);
    TestSingleToken(std::to_string(i) + "i8", eTk::kLitI8);
  }
  for (int i = 100; i <= 127; i++) {
    TestSingleToken(std::to_string(i) + "c", eTk::kLitI8);
    TestSingleToken(std::to_string(i) + "i8", eTk::kLitI8);
  }
}

TEST(LexerTokens, LiteralI16) { TestSingleToken("123i16", eTk::kLitI16); }

TEST(LexerTokens, LiteralI32) {
  TestSingleToken("123", eTk::kLitI32);
  TestSingleToken("123i32", eTk::kLitI32);
  // Make sure i32 doesn't get mistaken for an f32 if followed by ellipsis.
  TestSingleToken("123...", eTk::kLitI32, "123", 1, 4);
}

TEST(LexerTokens, LiteralI64) {
  TestSingleToken("123L", eTk::kLitI64);
  TestSingleToken("123i64", eTk::kLitI64);
}

TEST(LexerTokens, LiteralF32) {
  TestSingleToken("123.f", eTk::kLitF32);
  TestSingleToken("123.456f", eTk::kLitF32);
}

TEST(LexerTokens, LiteralF64) {
  TestSingleToken("123.", eTk::kLitF64);
  TestSingleToken("123.456", eTk::kLitF64);
}

TEST(LexerTokens, LiteralReal) {
  TestSingleToken("123.r", eTk::kLitReal);
  TestSingleToken("123.456r", eTk::kLitReal);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Punctuators.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(LexerTokens, Hash) { TestSingleToken("#", eTk::kHash); }

TEST(LexerTokens, Add) { TestSingleToken("+", eTk::kAdd); }

TEST(LexerTokens, Sub) { TestSingleToken("-", eTk::kSub); }

TEST(LexerTokens, Mul) { TestSingleToken("*", eTk::kMul); }

TEST(LexerTokens, Div) { TestSingleToken("/", eTk::kDiv); }

TEST(LexerTokens, Mod) { TestSingleToken("%", eTk::kMod); }

TEST(LexerTokens, And) { TestSingleToken("&&", eTk::kAnd); }

TEST(LexerTokens, Or) { TestSingleToken("||", eTk::kOr); }

TEST(LexerTokens, Xor) { TestSingleToken("^", eTk::kXor); }

TEST(LexerTokens, Not) { TestSingleToken("!", eTk::kNot); }

TEST(LexerTokens, Lsh) { TestSingleToken("<<", eTk::kLsh); }

TEST(LexerTokens, Rsh) { TestSingleToken(">>", eTk::kRsh); }

TEST(LexerTokens, Eq) { TestSingleToken("==", eTk::kEq); }

TEST(LexerTokens, Neq) { TestSingleToken("!=", eTk::kNeq); }

TEST(LexerTokens, Lt) { TestSingleToken("<", eTk::kLt); }

TEST(LexerTokens, Gt) { TestSingleToken(">", eTk::kGt); }

TEST(LexerTokens, Lte) { TestSingleToken("<=", eTk::kLte); }

TEST(LexerTokens, Gte) { TestSingleToken(">=", eTk::kGte); }

TEST(LexerTokens, Spaceship) { TestSingleToken("<=>", eTk::kSpaceship); }

TEST(LexerTokens, Assign) { TestSingleToken("=", eTk::kAssign); }

TEST(LexerTokens, NewAssign) { TestSingleToken(":=", eTk::kNewAssign); }

TEST(LexerTokens, AddAssign) { TestSingleToken("+=", eTk::kAddAssign); }

TEST(LexerTokens, SubAssign) { TestSingleToken("-=", eTk::kSubAssign); }

TEST(LexerTokens, MulAssign) { TestSingleToken("*=", eTk::kMulAssign); }

TEST(LexerTokens, DivAssign) { TestSingleToken("/=", eTk::kDivAssign); }

TEST(LexerTokens, ModAssign) { TestSingleToken("%=", eTk::kModAssign); }

TEST(LexerTokens, AndAssign) { TestSingleToken("&=", eTk::kAndAssign); }

TEST(LexerTokens, OrAssign) { TestSingleToken("|=", eTk::kOrAssign); }

TEST(LexerTokens, XorAssign) { TestSingleToken("^=", eTk::kXorAssign); }

TEST(LexerTokens, LshAssign) { TestSingleToken("<<=", eTk::kLshAssign); }

TEST(LexerTokens, RshAssign) { TestSingleToken(">>=", eTk::kRshAssign); }

TEST(LexerTokens, Inc) { TestSingleToken("++", eTk::kInc); }

TEST(LexerTokens, Dec) { TestSingleToken("--", eTk::kDec); }

TEST(LexerTokens, Bnot) { TestSingleToken("~", eTk::kBnot); }

TEST(LexerTokens, Band) { TestSingleToken("&", eTk::kBand); }

TEST(LexerTokens, Bor) { TestSingleToken("|", eTk::kBor); }

TEST(LexerTokens, LParen) { TestSingleToken("(", eTk::kLParen); }

TEST(LexerTokens, RParen) { TestSingleToken(")", eTk::kRParen); }

TEST(LexerTokens, LBrace) { TestSingleToken("{", eTk::kLBrace); }

TEST(LexerTokens, RBrace) { TestSingleToken("}", eTk::kRBrace); }

TEST(LexerTokens, LBracket) { TestSingleToken("[", eTk::kLBracket); }

TEST(LexerTokens, RBracket) { TestSingleToken("]", eTk::kRBracket); }

TEST(LexerTokens, Semicolon) { TestSingleToken(";", eTk::kSemicolon); }

TEST(LexerTokens, Colon) { TestSingleToken(":", eTk::kColon); }

TEST(LexerTokens, Comma) { TestSingleToken(",", eTk::kComma); }

TEST(LexerTokens, Period) { TestSingleToken(".", eTk::kPeriod); }

TEST(LexerTokens, DoubleColon) { TestSingleToken("::", eTk::kDoubleColon); }

TEST(LexerTokens, Ellipsis) { TestSingleToken("...", eTk::kEllipsis); }

TEST(LexerTokens, CommercialAt) { TestSingleToken("@", eTk::kCommercialAt); }

TEST(LexerTokens, Dollar) { TestSingleToken("$", eTk::kDollar); }

TEST(LexerTokens, Backlash) { TestSingleToken("\\", eTk::kBacklash); }

TEST(LexerTokens, Question) { TestSingleToken("?", eTk::kQuestion); }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Whitespace/comments.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(LexerTokens, Whitespace) {
  TestSingleToken(" ", eTk::kWhitespace);
  TestSingleToken("\t", eTk::kWhitespace);
}

TEST(LexerTokens, Newline) {
  // Unix newline style.
  TestSingleToken("\n", eTk::kNewline, 1, 1, 1, 2);
  TestSingleToken("\n\n", eTk::kNewline, 1, 1, 1, 3);

  // Windows newline style.
  TestSingleToken("\r\n", eTk::kNewline, 1, 1, 1, 2);
  TestSingleToken("\r\n\r\n", eTk::kNewline, 1, 1, 1, 3);

  // Old mac newline style.
  TestSingleToken("\r", eTk::kNewline, 1, 1, 1, 2);
  TestSingleToken("\r\r", eTk::kNewline, 1, 1, 1, 3);
}

TEST(LexerTokens, LineComment) { TestSingleToken("`comment", eTk::kLineComment); }

TEST(LexerTokens, BlockComment) {
  TestSingleToken("/` comment `/", eTk::kBlockComment);
  TestSingleToken("/` comment ``/", eTk::kBlockComment);
  TestSingleToken("/` comment \n`/", eTk::kBlockComment, 1, 3, 1, 2);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Character sequences.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(LexerTokens, EscapedCharSequence) {
  TestSingleToken("\"Hello World\"", eTk::kLitCstr);
  // Allow escaped quotes & escaped backlash before quote.
  TestSingleToken("\"\\\"Hello World\\\"\"", eTk::kLitCstr);
  // String ends exactly at the closing quote.
  TestSingleToken("\"Hello\"\"World\"", eTk::kLitCstr, "\"Hello\"", 1, 8, 1, 1);
  // Line continuation with escaped newline. ( Correct line tracking? )
  TestSingleToken("\"Hello \\\nWorld\"", eTk::kLitCstr, 1, 8, 1, 2);
}

TEST(LexerTokens, EscapedCharSequenceError) {
  // Unclosed string.
  TestCheckLexerErrorCode("\"Hello World\\\"", cnd::eClErr::kLexerUnclosedStringLiteral);

  // String ending in a lone backlash.
  TestCheckLexerErrorCode("\"Hello World\\", cnd::eClErr::kLexerUnclosedStringLiteral);
}

TEST(LexerTokens, CharacterLiteral) {
  TestSingleToken("'a'", eTk::kLitI8);
  TestSingleToken("'\\n'", eTk::kLitI8);
  TestSingleToken("'\\''", eTk::kLitI8);
  TestSingleToken("'\\\\'", eTk::kLitI8);
  TestSingleToken("'\\t'", eTk::kLitI8);
}

TEST(LexerTokens, CharacterLiteralError) {
  // Eof before closing quote.
  TestCheckLexerErrorCode("'", cnd::eClErr::kLexerUnclosedCharacterLiteral);
  // Newline in char literal.
  TestCheckLexerErrorCode("'\n'", cnd::eClErr::kLexerUnclosedCharacterLiteral);
  // Ending in a lone backlash.
  TestCheckLexerErrorCode("'\\", cnd::eClErr::kLexerUnclosedCharacterLiteral);
  // Unclosed char literal.
  TestCheckLexerErrorCode("'a + unclosed", cnd::eClErr::kLexerUnclosedCharacterLiteral);
  // Empty char literal.
  TestCheckLexerErrorCode("''", cnd::eClErr::kLexerEmptyCharacterLiteral);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Identifier and keywords.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(LexerTokens, Ident) {
  TestSingleToken("foo", eTk::kIdent);
  TestSingleToken("foo_", eTk::kIdent);
  TestSingleToken("foo_123", eTk::kIdent);
  TestSingleToken("_foo_123", eTk::kIdent);
}

TEST(LexerTokens, KeywordDef) { TestSingleToken("def", eTk::kKwDef); }
TEST(LexerTokens, KeywordFn) { TestSingleToken("fn", eTk::kKwFn); }
TEST(LexerTokens, KeywordClass) { TestSingleToken("class", eTk::kKwClass); }
TEST(LexerTokens, KeywordMain) { TestSingleToken("main", eTk::kKwMain); }
TEST(LexerTokens, KeywordImport) { TestSingleToken("import", eTk::kKwImport); }
TEST(LexerTokens, KeywordNamespace) { TestSingleToken("namespace", eTk::kKwNamespace); }
TEST(LexerTokens, KeywordUse) { TestSingleToken("using", eTk::kKwUse); }
TEST(LexerTokens, KeywordLib) { TestSingleToken("lib", eTk::kKwLib); }
TEST(LexerTokens, KeywordDll) { TestSingleToken("dll", eTk::kKwDll); }
TEST(LexerTokens, KeywordEnum) { TestSingleToken("enum", eTk::kKwEnum); }

TEST(LexerTokens, KeywordIf) { TestSingleToken("if", eTk::kKwIf); }
TEST(LexerTokens, KeywordElif) { TestSingleToken("elif", eTk::kKwElif); }
TEST(LexerTokens, KeywordElse) { TestSingleToken("else", eTk::kKwElse); }
TEST(LexerTokens, KeywordCxif) { TestSingleToken("cxif", eTk::kKwCxif); }
TEST(LexerTokens, KeywordCxelif) { TestSingleToken("cxelif", eTk::kKwCxelif); }
TEST(LexerTokens, KeywordCxelse) { TestSingleToken("cxelse", eTk::kKwCxelse); }

TEST(LexerTokens, KeywordSwitch) { TestSingleToken("switch", eTk::kKwSwitch); }
TEST(LexerTokens, KeywordCase) { TestSingleToken("case", eTk::kKwCase); }
TEST(LexerTokens, KeywordDefault) { TestSingleToken("default", eTk::kKwDefault); }

TEST(LexerTokens, KeywordWhile) { TestSingleToken("while", eTk::kKwWhile); }
TEST(LexerTokens, KeywordFor) { TestSingleToken("for", eTk::kKwFor); }

TEST(LexerTokens, KeywordReturn) { TestSingleToken("return", eTk::kKwReturn); }
TEST(LexerTokens, KeywordBreak) { TestSingleToken("break", eTk::kKwBreak); }
TEST(LexerTokens, KeywordContinue) { TestSingleToken("continue", eTk::kKwContinue); }

TEST(LexerTokens, KeywordInt) { TestSingleToken("int", eTk::kKwInt); }
TEST(LexerTokens, KeywordUint) { TestSingleToken("uint", eTk::kKwUint); }
TEST(LexerTokens, KeywordReal) { TestSingleToken("real", eTk::kKwReal); }
TEST(LexerTokens, KeywordBool) { TestSingleToken("bool", eTk::kKwBool); }
TEST(LexerTokens, KeywordChar) { TestSingleToken("char", eTk::kKwChar); }
TEST(LexerTokens, KeywordByte) { TestSingleToken("byte", eTk::kKwByte); }
TEST(LexerTokens, KeywordCstr) { TestSingleToken("cstr", eTk::kKwCstr); }
TEST(LexerTokens, KeywordStr) { TestSingleToken("str", eTk::kKwStr); }
TEST(LexerTokens, KeywordPtr) { TestSingleToken("ptr", eTk::kKwPtr); }
TEST(LexerTokens, KeywordList) { TestSingleToken("list", eTk::kKwList); }
TEST(LexerTokens, KeywordArray) { TestSingleToken("array", eTk::kKwArray); }

TEST(LexerTokens, KeywordTrue) { TestSingleToken("true", eTk::kKwTrue); }
TEST(LexerTokens, KeywordFalse) { TestSingleToken("false", eTk::kKwFalse); }
TEST(LexerTokens, KeywordNone) { TestSingleToken("none", eTk::kKwNone); }
TEST(LexerTokens, KeywordVoid) { TestSingleToken("void", eTk::kKwVoid); }

TEST(LexerTokens, KeywordIn) { TestSingleToken("in", eTk::kKwIn); }
TEST(LexerTokens, KeywordAs) { TestSingleToken("as", eTk::kKwAs); }

TEST(LexerTokens, KeywordCin) { TestSingleToken("cin", eTk::kKwCin); }
TEST(LexerTokens, KeywordCout) { TestSingleToken("cout", eTk::kKwCout); }

TEST(LexerTokens, KeywordNative) { TestSingleToken("native", eTk::kKwNative); }
TEST(LexerTokens, KeywordConst) { TestSingleToken("const", eTk::kKwConst); }
TEST(LexerTokens, KeywordRef) { TestSingleToken("ref", eTk::kKwRef); }

TEST(LexerTokens, KeywordPrivate) { TestSingleToken("private", eTk::kKwPrivate); }
TEST(LexerTokens, KeywordPublic) { TestSingleToken("public", eTk::kKwPublic); }
TEST(LexerTokens, KeywordStatic) { TestSingleToken("static", eTk::kKwStatic); }

TEST(LexerTokens, KeywordAny) { TestSingleToken("any", eTk::kKwAny); }
TEST(LexerTokens, KeywordAuto) { TestSingleToken("auto", eTk::kKwAuto); }
TEST(LexerTokens, KeywordType) { TestSingleToken("type", eTk::kKwType); }
TEST(LexerTokens, KeywordValue) { TestSingleToken("value", eTk::kKwValue); }

TEST(LexerTokens, KeywordTemplate) { TestSingleToken("template", eTk::kKwTemplate); }
TEST(LexerTokens, KeywordProc) { TestSingleToken("proc", eTk::kKwProc); }

}  // namespace cnd_unit_test::frontend::lexer
/// @} // end of cnd_unit_test

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2026 Anton Yashchenko
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