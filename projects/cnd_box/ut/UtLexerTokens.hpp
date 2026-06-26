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
#include "frontend/source.hpp"
#include "minitest.hpp"

namespace cnd_unit_test::frontend::lexer {
using namespace cnd::frontend;
using cnd::eTk;
using cnd::eTkToCStr;
using cnd::Size;

void testSingleToken(std::string_view source, eTk expected_type, std::string_view expected_literal,
                     Size begcol = 1, Size endcol = 1, Size begline = 1, Size endline = 1) {
  auto lex_result = tokenizeSourceCode(source);
  if (!lex_result) std::cout << lex_result.error().Format() << std::endl;
  ASSERT_TRUE(lex_result);

  // Check type.
  if (expected_type != eTk::kNONE)
    EXPECT_EQ_LOG(lex_result->front().Type(), expected_type,
                  std::format("Expected token type '{}' but got '{}'.", eTkToCStr(expected_type),
                              eTkToCStr(lex_result->front().Type())));

  // Check literal.
  if (expected_literal == "") expected_literal = source;
  EXPECT_EQ_LOG(lex_result->front().Literal(), expected_literal,
                std::format("Expected token literal '{}' but got '{}'.", expected_literal,
                            lex_result->front().Literal()));

  // Check start column.
  EXPECT_EQ_LOG(lex_result->front().BegCol(), begcol,
                std::format("Expected token begin column '{}' but got '{}'.", begcol,
                            lex_result->front().BegCol()));

  // Check end column.
  EXPECT_EQ_LOG(lex_result->front().EndCol(), endcol,
                std::format("Expected token end column '{}' but got '{}'.", endcol,
                            lex_result->front().EndCol()));

  // Check begin line.
  EXPECT_EQ_LOG(lex_result->front().BegLine(), begline,
                std::format("Expected token begin line '{}' but got '{}'.", begline,
                            lex_result->front().BegLine()));

  // Check end column.
  EXPECT_EQ_LOG(lex_result->front().EndLine(), endline,
                std::format("Expected token end line '{}' but got '{}'.", endline,
                            lex_result->front().EndLine()));
}

// Expected literal is defaulted to input source.
void testSingleToken(std::string_view source, eTk expected_type, Size begcol, Size endcol = 1,
                     Size begline = 1, Size endline = 1) {
  auto lex_result = tokenizeSourceCode(source);
  if (!lex_result) std::cout << lex_result.error().Format() << std::endl;
  ASSERT_TRUE(lex_result);

  // Check type.
  if (expected_type != eTk::kNONE)
    EXPECT_EQ_LOG(lex_result->front().Type(), expected_type,
                  std::format("Expected token type '{}' but got '{}'.", eTkToCStr(expected_type),
                              eTkToCStr(lex_result->front().Type())));

  // Check literal.
  EXPECT_EQ_LOG(lex_result->front().Literal(), source,
                std::format("Expected token literal '{}' but got '{}'.", source,
                            lex_result->front().Literal()));

  // Check start column.
  EXPECT_EQ_LOG(lex_result->front().BegCol(), begcol,
                std::format("Expected token begin column '{}' but got '{}'.", begcol,
                            lex_result->front().BegCol()));

  // Check end column.
  EXPECT_EQ_LOG(lex_result->front().EndCol(), endcol,
                std::format("Expected token end column '{}' but got '{}'.", endcol,
                            lex_result->front().EndCol()));

  // Check begin line.
  EXPECT_EQ_LOG(lex_result->front().BegLine(), begline,
                std::format("Expected token begin line '{}' but got '{}'.", begline,
                            lex_result->front().BegLine()));

  // Check end column.
  EXPECT_EQ_LOG(lex_result->front().EndLine(), endline,
                std::format("Expected token end line '{}' but got '{}'.", endline,
                            lex_result->front().EndLine()));
}

// Expect lines to be 1 and cols to be 1 and size of source + 1 by default. Expected literal is
// defaulted to source.
void testSingleToken(std::string_view source, eTk expected_type = eTk::kNONE) {
  auto lex_result = tokenizeSourceCode(source);
  if (!lex_result) std::cout << lex_result.error().Format() << std::endl;
  ASSERT_TRUE(lex_result);

  // Check type.
  if (expected_type != eTk::kNONE)
    EXPECT_EQ_LOG(lex_result->front().Type(), expected_type,
                  std::format("Expected token type '{}' but got '{}'.", eTkToCStr(expected_type),
                              eTkToCStr(lex_result->front().Type())));

  // Check literal.
  EXPECT_EQ_LOG(lex_result->front().Literal(), source,
                std::format("Expected token literal '{}' but got '{}'.", source,
                            lex_result->front().Literal()));

  // Check start column.
  EXPECT_EQ_LOG(lex_result->front().BegCol(), 1,
                std::format("Expected token begin column '{}' but got '{}'.", 1,
                            lex_result->front().BegCol()));

  // Check end column.
  EXPECT_EQ_LOG(lex_result->front().EndCol(), 1 + source.size(),
                std::format("Expected token end column '{}' but got '{}'.", 1 + source.size(),
                            lex_result->front().EndCol()));

  // Check begin line.
  EXPECT_EQ_LOG(lex_result->front().BegLine(), 1,
                std::format("Expected token begin line '{}' but got '{}'.", 1,
                            lex_result->front().BegLine()));

  // Check end column.
  EXPECT_EQ_LOG(
      lex_result->front().EndLine(), 1,
      std::format("Expected token end line '{}' but got '{}'.", 1, lex_result->front().EndLine()));
}

void testMultipleTokens(std::string_view source,
                        std::vector<std::pair<eTk, std::string>> expected_tokens,
                        bool sanitize = false) {
  auto lex_result = tokenizeSourceCode(source);
  if (!lex_result) std::cout << lex_result.error().Format() << std::endl;
  ASSERT_TRUE(lex_result);

  if (sanitize) lex_result.value() = sanitizeTokens(*lex_result);

  for (auto i = 0; i < expected_tokens.size(); i++) {
    const auto& [expected_type, expected_literal] = expected_tokens.at(i);
    if (!EXPECT_TRUE_LOG(lex_result->size() > i,
                         std::format("Expected at least {} tokens but got {}.",
                                     expected_tokens.size(), lex_result->size())))
      return;

    // Check type.
    if (expected_type != eTk::kNONE)
      if (!EXPECT_EQ_LOG(
              lex_result->at(i).Type(), expected_type,
              std::format("Expected token type '{}' but got '{}'.", eTkToCStr(expected_type),
                          eTkToCStr(lex_result->at(i).Type()))))
        return;

    // Check literal.
    if (!EXPECT_EQ_LOG(lex_result->at(i).Literal(), expected_literal,
                       std::format("Expected token literal '{}' but got '{}'.", expected_literal,
                                   lex_result->at(i).Literal())))
      return;
  }
}

void testCheckLexerErrorCode(std::string_view source, cnd::cldev::clmsg::eClErr expected_code) {
  auto lex_result = tokenizeSourceCode(source);
  ASSERT_FALSE(lex_result);
  EXPECT_EQ_LOG(lex_result.error().GetLastMessageId().code,
                cnd::cldev::clmsg::GetClMsgIdOf(expected_code).code,
                std::format("Expected error code '{}' but got '{}'.",
                            cnd::cldev::clmsg::GetClMsgIdOf(expected_code).code,
                            lex_result.error().GetLastMessageId().code));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Numeric literals.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(LexerTokens, LiteralU1) {
  testSingleToken("1b", eTk::kLitU1, 1, 3);
  testSingleToken("0b", eTk::kLitU1, 1, 3);
}

TEST(LexerTokens, LiteralU8) {
  for (int i = 0; i <= 9; i++) {
    testSingleToken(std::to_string(i) + "B", eTk::kLitU8, 1, 3);
    testSingleToken(std::to_string(i) + "u8", eTk::kLitU8, 1, 4);
  }
  for (int i = 10; i <= 99; i++) {
    testSingleToken(std::to_string(i) + "B", eTk::kLitU8, 1, 4);
    testSingleToken(std::to_string(i) + "u8", eTk::kLitU8, 1, 5);
  }
  for (int i = 100; i <= 127; i++) {
    testSingleToken(std::to_string(i) + "B", eTk::kLitU8, 1, 5);
    testSingleToken(std::to_string(i) + "u8", eTk::kLitU8, 1, 6);
  }
}

TEST(LexerTokens, LiteralU16) { testSingleToken("123u16", eTk::kLitU16); }

TEST(LexerTokens, LiteralU32) {
  testSingleToken("123u", eTk::kLitU32, 1, 5);
  testSingleToken("123u32", eTk::kLitU32, 1, 7);
}

TEST(LexerTokens, LiteralU64) {
  testSingleToken("123U", eTk::kLitU64, 1, 5);
  testSingleToken("123u64", eTk::kLitU64, 1, 7);
}

TEST(LexerTokens, LiteralI8) {
  for (int i = 0; i <= 9; i++) {
    testSingleToken(std::to_string(i) + "c", eTk::kLitI8, 1, 3);
    testSingleToken(std::to_string(i) + "i8", eTk::kLitI8, 1, 4);
  }
  for (int i = 10; i <= 99; i++) {
    testSingleToken(std::to_string(i) + "c", eTk::kLitI8, 1, 4);
    testSingleToken(std::to_string(i) + "i8", eTk::kLitI8, 1, 5);
  }
  for (int i = 100; i <= 127; i++) {
    testSingleToken(std::to_string(i) + "c", eTk::kLitI8, 1, 5);
    testSingleToken(std::to_string(i) + "i8", eTk::kLitI8, 1, 6);
  }
}

TEST(LexerTokens, LiteralI16) { testSingleToken("123i16", eTk::kLitI16); }

TEST(LexerTokens, LiteralI32) {
  testSingleToken("123", eTk::kLitI32, 1, 4);
  testSingleToken("123i32", eTk::kLitI32, 1, 7);
  // Make sure i32 doesn't get mistaken for an f32 if followed by ellipsis.
  testSingleToken("123...", eTk::kLitI32, "123", 1, 4);
}

TEST(LexerTokens, LiteralI64) {
  testSingleToken("123L", eTk::kLitI64, 1, 5);
  testSingleToken("123i64", eTk::kLitI64, 1, 7);
}

TEST(LexerTokens, LiteralF32) {
  testSingleToken("123.f", eTk::kLitF32, 1, 6);
  testSingleToken("123.456f", eTk::kLitF32, 1, 9);
}

TEST(LexerTokens, LiteralF64) {
  testSingleToken("123.", eTk::kLitF64, 1, 5);
  testSingleToken("123.456", eTk::kLitF64, 1, 8);
}

TEST(LexerTokens, LiteralReal) {
  testSingleToken("123.r", eTk::kLitReal, 1, 6);
  testSingleToken("123.456r", eTk::kLitReal, 1, 9);
}

TEST(LexerTokens, UnknownNumericSuffixError) {
  testCheckLexerErrorCode("123foo", cnd::eClErr::kLexerUnknownNumericSuffix);
  testCheckLexerErrorCode("123.foo", cnd::eClErr::kLexerUnknownNumericSuffix);
  testCheckLexerErrorCode("123.0foo", cnd::eClErr::kLexerUnknownNumericSuffix);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Punctuators.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(LexerTokens, Hash) { testSingleToken("#", eTk::kHash, 1, 2); }

TEST(LexerTokens, Add) { testSingleToken("+", eTk::kAdd, 1, 2); }

TEST(LexerTokens, Sub) { testSingleToken("-", eTk::kSub, 1, 2); }

TEST(LexerTokens, Mul) { testSingleToken("*", eTk::kMul, 1, 2); }

TEST(LexerTokens, Div) { testSingleToken("/", eTk::kDiv, 1, 2); }

TEST(LexerTokens, Mod) { testSingleToken("%", eTk::kMod, 1, 2); }

TEST(LexerTokens, And) { testSingleToken("&&", eTk::kAnd, 1, 3); }

TEST(LexerTokens, Or) { testSingleToken("||", eTk::kOr, 1, 3); }

TEST(LexerTokens, Xor) { testSingleToken("^", eTk::kXor, 1, 2); }

TEST(LexerTokens, Not) { testSingleToken("!", eTk::kNot, 1, 2); }

TEST(LexerTokens, Lsh) { testSingleToken("<<", eTk::kLsh, 1, 3); }

TEST(LexerTokens, Rsh) { testSingleToken(">>", eTk::kRsh, 1, 3); }

TEST(LexerTokens, Eq) { testSingleToken("==", eTk::kEq, 1, 3); }

TEST(LexerTokens, Neq) { testSingleToken("!=", eTk::kNeq, 1, 3); }

TEST(LexerTokens, Lt) { testSingleToken("<", eTk::kLt, 1, 2); }

TEST(LexerTokens, Gt) { testSingleToken(">", eTk::kGt, 1, 2); }

TEST(LexerTokens, Lte) { testSingleToken("<=", eTk::kLte, 1, 3); }

TEST(LexerTokens, Gte) { testSingleToken(">=", eTk::kGte, 1, 3); }

TEST(LexerTokens, Spaceship) { testSingleToken("<=>", eTk::kSpaceship, 1, 4); }

TEST(LexerTokens, Assign) { testSingleToken("=", eTk::kAssign, 1, 2); }

TEST(LexerTokens, NewAssign) { testSingleToken(":=", eTk::kNewAssign, 1, 3); }

TEST(LexerTokens, AddAssign) { testSingleToken("+=", eTk::kAddAssign, 1, 3); }

TEST(LexerTokens, SubAssign) { testSingleToken("-=", eTk::kSubAssign, 1, 3); }

TEST(LexerTokens, MulAssign) { testSingleToken("*=", eTk::kMulAssign, 1, 3); }

TEST(LexerTokens, DivAssign) { testSingleToken("/=", eTk::kDivAssign, 1, 3); }

TEST(LexerTokens, ModAssign) { testSingleToken("%=", eTk::kModAssign, 1, 3); }

TEST(LexerTokens, AndAssign) { testSingleToken("&=", eTk::kAndAssign, 1, 3); }

TEST(LexerTokens, OrAssign) { testSingleToken("|=", eTk::kOrAssign, 1, 3); }

TEST(LexerTokens, XorAssign) { testSingleToken("^=", eTk::kXorAssign, 1, 3); }

TEST(LexerTokens, LshAssign) { testSingleToken("<<=", eTk::kLshAssign, 1, 4); }

TEST(LexerTokens, RshAssign) { testSingleToken(">>=", eTk::kRshAssign, 1, 4); }

TEST(LexerTokens, Inc) { testSingleToken("++", eTk::kInc, 1, 3); }

TEST(LexerTokens, Dec) { testSingleToken("--", eTk::kDec, 1, 3); }

TEST(LexerTokens, Bnot) { testSingleToken("~", eTk::kBnot, 1, 2); }

TEST(LexerTokens, Band) { testSingleToken("&", eTk::kBand, 1, 2); }

TEST(LexerTokens, Bor) { testSingleToken("|", eTk::kBor, 1, 2); }

TEST(LexerTokens, LParen) { testSingleToken("(", eTk::kLParen, 1, 2); }

TEST(LexerTokens, RParen) { testSingleToken(")", eTk::kRParen, 1, 2); }

TEST(LexerTokens, LBrace) { testSingleToken("{", eTk::kLBrace, 1, 2); }

TEST(LexerTokens, RBrace) { testSingleToken("}", eTk::kRBrace, 1, 2); }

TEST(LexerTokens, LBracket) { testSingleToken("[", eTk::kLBracket, 1, 2); }

TEST(LexerTokens, RBracket) { testSingleToken("]", eTk::kRBracket, 1, 2); }

TEST(LexerTokens, Semicolon) { testSingleToken(";", eTk::kSemicolon, 1, 2); }

TEST(LexerTokens, Colon) { testSingleToken(":", eTk::kColon, 1, 2); }

TEST(LexerTokens, Comma) { testSingleToken(",", eTk::kComma, 1, 2); }

TEST(LexerTokens, Period) { testSingleToken(".", eTk::kPeriod, 1, 2); }

TEST(LexerTokens, DoubleColon) { testSingleToken("::", eTk::kDoubleColon, 1, 3); }

TEST(LexerTokens, Ellipsis) { testSingleToken("...", eTk::kEllipsis, 1, 4); }

TEST(LexerTokens, CommercialAt) { testSingleToken("@", eTk::kCommercialAt, 1, 2); }

TEST(LexerTokens, Dollar) { testSingleToken("$", eTk::kDollar, 1, 2); }

TEST(LexerTokens, Backlash) { testSingleToken("\\", eTk::kBacklash, 1, 2); }

TEST(LexerTokens, Question) { testSingleToken("?", eTk::kQuestion, 1, 2); }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Whitespace/comments.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(LexerTokens, Whitespace) {
  testSingleToken(" ", eTk::kWhitespace, 1, 2);
  testSingleToken("\t", eTk::kWhitespace, 1, 2);
}

TEST(LexerTokens, Newline) {
  // Unix newline style.
  testSingleToken("\n", eTk::kNewline, 1, 1, 1, 2);
  testSingleToken("\n\n", eTk::kNewline, 1, 1, 1, 3);

  // Windows newline style.
  testSingleToken("\r\n", eTk::kNewline, 1, 1, 1, 2);
  testSingleToken("\r\n\r\n", eTk::kNewline, 1, 1, 1, 3);

  // Old mac newline style.
  testSingleToken("\r", eTk::kNewline, 1, 1, 1, 2);
  testSingleToken("\r\r", eTk::kNewline, 1, 1, 1, 3);
}

TEST(LexerTokens, LineComment) { testSingleToken("`comment", eTk::kLineComment, 1, 9); }

TEST(LexerTokens, BlockComment) {
  testSingleToken("/` comment `/", eTk::kBlockComment, 1, 14);
  testSingleToken("/` comment ``/", eTk::kBlockComment, 1, 15);
  testSingleToken("/` comment \n`/", eTk::kBlockComment, 1, 3, 1, 2);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Character sequences.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(LexerTokens, EscapedCharSequence) {
  testSingleToken("\"Hello World\"", eTk::kLitCstr, 1, 14);
  // Allow escaped quotes & escaped backlash before quote.
  testSingleToken("\"\\\"Hello World\\\"\"", eTk::kLitCstr, 1, 18);
  // String ends exactly at the closing quote.
  testSingleToken("\"Hello\"\"World\"", eTk::kLitCstr, "\"Hello\"", 1, 8, 1, 1);
  // Line continuation with escaped newline. ( Correct line tracking? )
  testSingleToken("\"Hello \\\nWorld\"", eTk::kLitCstr, 1, 8, 1, 2);
}

TEST(LexerTokens, EscapedCharSequenceError) {
  // Unclosed string.
  testCheckLexerErrorCode("\"Hello World\\\"", cnd::eClErr::kLexerUnclosedStringLiteral);

  // String ending in a lone backlash.
  testCheckLexerErrorCode("\"Hello World\\", cnd::eClErr::kLexerUnclosedStringLiteral);
}

TEST(LexerTokens, CharacterLiteral) {
  testSingleToken("'a'", eTk::kLitI8, 1, 4);
  testSingleToken("'\\n'", eTk::kLitI8, 1, 5);
  testSingleToken("'\\''", eTk::kLitI8, 1, 5);
  testSingleToken("'\\\\'", eTk::kLitI8, 1, 5);
  testSingleToken("'\\t'", eTk::kLitI8, 1, 5);
}

TEST(LexerTokens, CharacterLiteralError) {
  // Eof before closing quote.
  testCheckLexerErrorCode("'", cnd::eClErr::kLexerUnclosedCharacterLiteral);
  // Newline in char literal.
  testCheckLexerErrorCode("'\n'", cnd::eClErr::kLexerUnclosedCharacterLiteral);
  // Ending in a lone backlash.
  testCheckLexerErrorCode("'\\", cnd::eClErr::kLexerUnclosedCharacterLiteral);
  // Unclosed char literal.
  testCheckLexerErrorCode("'a + unclosed", cnd::eClErr::kLexerUnclosedCharacterLiteral);
  // Empty char literal.
  testCheckLexerErrorCode("''", cnd::eClErr::kLexerEmptyCharacterLiteral);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Identifier and keywords.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(LexerTokens, Ident) {
  testSingleToken("foo", eTk::kIdent, 1, 4);
  testSingleToken("foo_", eTk::kIdent, 1, 5);
  testSingleToken("foo_123", eTk::kIdent, 1, 8);
  testSingleToken("_foo_123", eTk::kIdent, 1, 9);
}

TEST(LexerTokens, KeywordInclude) { testSingleToken("include", eTk::kKwInclude, 1, 8); }
TEST(LexerTokens, KeywordPragma) { testSingleToken("pragma", eTk::kKwPragma, 1, 7); }

TEST(LexerTokens, KeywordDef) { testSingleToken("def", eTk::kKwDef, 1, 4); }
TEST(LexerTokens, KeywordFn) { testSingleToken("fn", eTk::kKwFn, 1, 3); }
TEST(LexerTokens, KeywordClass) { testSingleToken("class", eTk::kKwClass, 1, 6); }
TEST(LexerTokens, KeywordMain) { testSingleToken("main", eTk::kKwMain, 1, 5); }
TEST(LexerTokens, KeywordImport) { testSingleToken("import", eTk::kKwImport, 1, 7); }
TEST(LexerTokens, KeywordNamespace) { testSingleToken("namespace", eTk::kKwNamespace, 1, 10); }
TEST(LexerTokens, KeywordUse) { testSingleToken("using", eTk::kKwUse, 1, 6); }
TEST(LexerTokens, KeywordLib) { testSingleToken("lib", eTk::kKwLib, 1, 4); }
TEST(LexerTokens, KeywordDll) { testSingleToken("dll", eTk::kKwDll, 1, 4); }
TEST(LexerTokens, KeywordEnum) { testSingleToken("enum", eTk::kKwEnum, 1, 5); }

TEST(LexerTokens, KeywordIf) { testSingleToken("if", eTk::kKwIf, 1, 3); }
TEST(LexerTokens, KeywordElif) { testSingleToken("elif", eTk::kKwElif, 1, 5); }
TEST(LexerTokens, KeywordElse) { testSingleToken("else", eTk::kKwElse, 1, 5); }
TEST(LexerTokens, KeywordCxif) { testSingleToken("cxif", eTk::kKwCxif, 1, 5); }
TEST(LexerTokens, KeywordCxelif) { testSingleToken("cxelif", eTk::kKwCxelif, 1, 7); }
TEST(LexerTokens, KeywordCxelse) { testSingleToken("cxelse", eTk::kKwCxelse, 1, 7); }

TEST(LexerTokens, KeywordSwitch) { testSingleToken("switch", eTk::kKwSwitch, 1, 7); }
TEST(LexerTokens, KeywordCase) { testSingleToken("case", eTk::kKwCase, 1, 5); }
TEST(LexerTokens, KeywordDefault) { testSingleToken("default", eTk::kKwDefault, 1, 8); }

TEST(LexerTokens, KeywordWhile) { testSingleToken("while", eTk::kKwWhile, 1, 6); }
TEST(LexerTokens, KeywordFor) { testSingleToken("for", eTk::kKwFor, 1, 4); }

TEST(LexerTokens, KeywordReturn) { testSingleToken("return", eTk::kKwReturn, 1, 7); }
TEST(LexerTokens, KeywordBreak) { testSingleToken("break", eTk::kKwBreak, 1, 6); }
TEST(LexerTokens, KeywordContinue) { testSingleToken("continue", eTk::kKwContinue, 1, 9); }

TEST(LexerTokens, KeywordInt) { testSingleToken("int", eTk::kKwInt, 1, 4); }
TEST(LexerTokens, KeywordUint) { testSingleToken("uint", eTk::kKwUint, 1, 5); }
TEST(LexerTokens, KeywordReal) { testSingleToken("real", eTk::kKwReal, 1, 5); }
TEST(LexerTokens, KeywordBool) { testSingleToken("bool", eTk::kKwBool, 1, 5); }
TEST(LexerTokens, KeywordChar) { testSingleToken("char", eTk::kKwChar, 1, 5); }
TEST(LexerTokens, KeywordByte) { testSingleToken("byte", eTk::kKwByte, 1, 5); }
TEST(LexerTokens, KeywordCstr) { testSingleToken("cstr", eTk::kKwCstr, 1, 5); }
TEST(LexerTokens, KeywordStr) { testSingleToken("str", eTk::kKwStr, 1, 4); }
TEST(LexerTokens, KeywordPtr) { testSingleToken("ptr", eTk::kKwPtr, 1, 4); }
TEST(LexerTokens, KeywordList) { testSingleToken("list", eTk::kKwList, 1, 5); }
TEST(LexerTokens, KeywordArray) { testSingleToken("array", eTk::kKwArray, 1, 6); }

TEST(LexerTokens, KeywordTrue) { testSingleToken("true", eTk::kKwTrue, 1, 5); }
TEST(LexerTokens, KeywordFalse) { testSingleToken("false", eTk::kKwFalse, 1, 6); }
TEST(LexerTokens, KeywordNone) { testSingleToken("none", eTk::kKwNone, 1, 5); }
TEST(LexerTokens, KeywordVoid) { testSingleToken("void", eTk::kKwVoid, 1, 5); }

TEST(LexerTokens, KeywordIn) { testSingleToken("in", eTk::kKwIn, 1, 3); }
TEST(LexerTokens, KeywordAs) { testSingleToken("as", eTk::kKwAs, 1, 3); }

TEST(LexerTokens, KeywordCin) { testSingleToken("cin", eTk::kKwCin, 1, 4); }
TEST(LexerTokens, KeywordCout) { testSingleToken("cout", eTk::kKwCout, 1, 5); }

TEST(LexerTokens, KeywordNative) { testSingleToken("native", eTk::kKwNative, 1, 7); }
TEST(LexerTokens, KeywordConst) { testSingleToken("const", eTk::kKwConst, 1, 6); }
TEST(LexerTokens, KeywordRef) { testSingleToken("ref", eTk::kKwRef, 1, 4); }

TEST(LexerTokens, KeywordPrivate) { testSingleToken("private", eTk::kKwPrivate, 1, 8); }
TEST(LexerTokens, KeywordPublic) { testSingleToken("public", eTk::kKwPublic, 1, 7); }
TEST(LexerTokens, KeywordStatic) { testSingleToken("static", eTk::kKwStatic, 1, 7); }

TEST(LexerTokens, KeywordAny) { testSingleToken("any", eTk::kKwAny, 1, 4); }
TEST(LexerTokens, KeywordAuto) { testSingleToken("auto", eTk::kKwAuto, 1, 5); }
TEST(LexerTokens, KeywordType) { testSingleToken("type", eTk::kKwType, 1, 5); }
TEST(LexerTokens, KeywordValue) { testSingleToken("value", eTk::kKwValue, 1, 6); }

TEST(LexerTokens, KeywordTemplate) { testSingleToken("template", eTk::kKwTemplate, 1, 9); }
TEST(LexerTokens, KeywordProc) { testSingleToken("proc", eTk::kKwProc, 1, 5); }

TEST(LexerTokens, AllOperands) {
  testMultipleTokens(
      "1b 0b 42B 42u8 123u16 123u 123u32 123U "
      "123u64 123c 123i8 123i16 123 123i32 123L 123i64 123.f "
      "123.456f 123. 123.456 123.r 123.456r "
      "\"string literal\" \"\\\\\" alnumus true false none ",
      {{eTk::kLitU1, "1b"},
       {eTk::kLitU1, "0b"},
       {eTk::kLitU8, "42B"},
       {eTk::kLitU8, "42u8"},
       {eTk::kLitU16, "123u16"},
       {eTk::kLitU32, "123u"},
       {eTk::kLitU32, "123u32"},
       {eTk::kLitU64, "123U"},
       {eTk::kLitU64, "123u64"},
       {eTk::kLitI8, "123c"},
       {eTk::kLitI8, "123i8"},
       {eTk::kLitI16, "123i16"},
       {eTk::kLitI32, "123"},
       {eTk::kLitI32, "123i32"},
       {eTk::kLitI64, "123L"},
       {eTk::kLitI64, "123i64"},
       {eTk::kLitF32, "123.f"},
       {eTk::kLitF32, "123.456f"},
       {eTk::kLitF64, "123."},
       {eTk::kLitF64, "123.456"},
       {eTk::kLitReal, "123.r"},
       {eTk::kLitReal, "123.456r"},
       {eTk::kLitCstr, "\"string literal\""},
       {eTk::kLitCstr, "\"\\\\\""},
       {eTk::kIdent, "alnumus"},
       {eTk::kKwTrue, "true"},
       {eTk::kKwFalse, "false"},
       {eTk::kKwNone, "none"}},
      true);
}

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