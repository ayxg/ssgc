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
#include "compiler/compiler.hpp"
// #include "frontend/source.hpp"
#include "minitest.hpp"

namespace cnd_unit_test::frontend::lexer {
using ssgc::frontend::eToken;
using ssgc::frontend::eTokenToCStr;

void testSingleToken(std::string_view source, eToken expected_type,
                     std::string_view expected_literal, std::size_t begcol = 1,
                     std::size_t endcol = 1, std::size_t begline = 1, std::size_t endline = 1,
                     std::size_t size = 0) {
  std::size_t expected_offset = 0;
  std::size_t expected_size = size;
  if (expected_size == 0) {
    expected_size = (endcol - begcol) + (endline - begline);
  }

  ssgc::Compiler comp{};
  const ssgc::SourceFile* src = comp.generateSource("ut_lexer", source, true);
  const std::vector<ssgc::frontend::Token>* lex_result = comp.tokenize(src->id);
  bool error_occured = comp.errorOccured();
  if (error_occured) {
    auto [err_begin, err_end] = comp.getErrorRange();
    for (const ssgc::Diagnostic& err : std::ranges::subrange(err_begin, err_end)) {
      std::cout << comp.formatDiagnostic(err) << std::endl;
    }
  }
  ASSERT_TRUE(!error_occured);
  const ssgc::frontend::Token& test_token = lex_result->front();

  // Check type.
  if (expected_type != eToken::kNone) {
    EXPECT_EQ_LOG(test_token.kind, expected_type,
                  std::format("Expected token type '{}' but got '{}'.", eTokenToCStr(expected_type),
                              eTokenToCStr(test_token.kind)));
  }

  // Check literal.
  if (expected_literal == "") expected_literal = source;
  std::string_view literal = src->slice(test_token.source_range.begin, test_token.source_range.end);
  EXPECT_EQ_LOG(
      literal, expected_literal,
      std::format("Expected token literal '{}' but got '{}'.", expected_literal, literal));

  // Check offset.
  EXPECT_EQ_LOG(
      test_token.source_range.begin, expected_offset,
                std::format("Expected token offset '{}' but got '{}'.", expected_offset,
                            test_token.source_range.begin));

  // Check size.
  EXPECT_EQ_LOG(
      test_token.source_range.size(), expected_size,
                std::format("Expected token size '{}' but got '{}'.", expected_size,
                            test_token.source_range.size()));

  ssgc::SourceRange src_range = comp.getSourceRange(test_token.source_range);

  // Check begin col.
  EXPECT_EQ_LOG(
      src_range.begin.col, begcol,
      std::format("Expected token begin col '{}' but got '{}'.", begcol, src_range.begin.col));

  // Check end col.
  EXPECT_EQ_LOG(
      src_range.end.col, endcol,
      std::format("Expected token end col '{}' but got '{}'.", endcol, src_range.end.col));

  // Check begin line.
  EXPECT_EQ_LOG(
      src_range.begin.line, begline,
      std::format("Expected token begin line '{}' but got '{}'.", begline, src_range.begin.line));

  // Check end line.
  EXPECT_EQ_LOG(
      src_range.end.line, endline,
      std::format("Expected token end line '{}' but got '{}'.", endline, src_range.end.line));
}

// Expected literal is defaulted to input source.
void testSingleToken(std::string_view source, eToken expected_type, std::size_t begcol,
                     std::size_t endcol = 1, std::size_t begline = 1, std::size_t endline = 1,
                     std::size_t size = 0) {
  return testSingleToken(source, expected_type, "", begcol, endcol, begline, endline, size);
}

// Expect lines to be 1 and cols to be 1 and size of source + 1 by default. Expected literal is
// defaulted to source.
void testSingleToken(std::string_view source, eToken expected_type = eToken::kNone) {
  return testSingleToken(source, expected_type, 1, source.size() + 1, 1, 1);
}

void testMultipleTokens(std::string_view source,
                        std::vector<std::pair<eToken, std::string>> expected_tokens,
                        bool sanitize = false) {
  ssgc::Compiler comp{};
  const ssgc::SourceFile* src = comp.generateSource("ut_lexer", source, true);
  const std::vector<ssgc::frontend::Token>* lex_result = comp.tokenize(src->id);
  bool error_occured = comp.errorOccured();
  if (error_occured) {
    auto [err_begin, err_end] = comp.getErrorRange();
    for (const ssgc::Diagnostic& err : std::ranges::subrange(err_begin, err_end)) {
      std::cout << comp.formatDiagnostic(err) << std::endl;
    }
  }
  ASSERT_TRUE(!error_occured);

  std::size_t test_token_idx = 0;
  for (auto i = 0; i < expected_tokens.size(); i++) {
    if (sanitize) {
      while (lex_result->at(test_token_idx).kind == eToken::kWhitespace ||
             lex_result->at(test_token_idx).kind == eToken::kNewline) {
        test_token_idx++;
      }
      if (!EXPECT_TRUE_LOG(lex_result->size() > test_token_idx,
                           std::format("Expected at least {} significant tokens but got {}.",
                                       expected_tokens.size(), i))) {
        return;
      }
    } else {
      if (!EXPECT_TRUE_LOG(lex_result->size() > i,
                           std::format("Expected at least {} tokens but got {}.",
                                       expected_tokens.size(), lex_result->size()))) {
        return;
      }
    }

    const auto& [expected_type, expected_literal] = expected_tokens.at(i);
    const ssgc::frontend::Token& test_token = lex_result->at(test_token_idx);
    test_token_idx++;

    // Check type.
    if (expected_type != eToken::kNone) {
      if (!EXPECT_EQ_LOG(test_token.kind, expected_type,
                         std::format("Expected token type '{}' but got '{}'.",
                                     eTokenToCStr(expected_type), eTokenToCStr(test_token.kind)))) {
        return;
      }
    }

    // Check literal.
    std::string_view lit = src->slice(test_token.source_range.begin, test_token.source_range.end);
    if (!EXPECT_EQ_LOG(
            lit, expected_literal,
            std::format("Expected token literal '{}' but got '{}'.", expected_literal, lit))) {
      return;
    }
  }
}

void testCheckLexerErrorCode(std::string_view source, ssgc::eError expected_code) {
  ssgc::Compiler comp{};
  const ssgc::SourceFile* src = comp.generateSource("ut_lexer", source, true);
  const std::vector<ssgc::frontend::Token>* lex_result = comp.tokenize(src->id);
  bool error_occured = comp.errorOccured();
  ASSERT_TRUE(error_occured);
  EXPECT_EQ_LOG(comp.getLastError().id.code, getDiagnosticId(expected_code).code,
                std::format("Expected error code '{}' but got '{}'.",
                            getDiagnosticId(expected_code).code, comp.getLastError().id.code));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Numeric literals.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(LexerTokens, LiteralU1) {
  testSingleToken("1b", eToken::kLitU1, 1, 3);
  testSingleToken("0b", eToken::kLitU1, 1, 3);
}

TEST(LexerTokens, LiteralU8) {
  for (int i = 0; i <= 9; i++) {
    testSingleToken(std::to_string(i) + "B", eToken::kLitU8, 1, 3);
    testSingleToken(std::to_string(i) + "u8", eToken::kLitU8, 1, 4);
  }
  for (int i = 10; i <= 99; i++) {
    testSingleToken(std::to_string(i) + "B", eToken::kLitU8, 1, 4);
    testSingleToken(std::to_string(i) + "u8", eToken::kLitU8, 1, 5);
  }
  for (int i = 100; i <= 127; i++) {
    testSingleToken(std::to_string(i) + "B", eToken::kLitU8, 1, 5);
    testSingleToken(std::to_string(i) + "u8", eToken::kLitU8, 1, 6);
  }
}

TEST(LexerTokens, LiteralU16) { testSingleToken("123u16", eToken::kLitU16); }

TEST(LexerTokens, LiteralU32) {
  testSingleToken("123u", eToken::kLitU32, 1, 5);
  testSingleToken("123u32", eToken::kLitU32, 1, 7);
}

TEST(LexerTokens, LiteralU64) {
  testSingleToken("123U", eToken::kLitU64, 1, 5);
  testSingleToken("123u64", eToken::kLitU64, 1, 7);
}

TEST(LexerTokens, LiteralI8) {
  for (int i = 0; i <= 9; i++) {
    testSingleToken(std::to_string(i) + "c", eToken::kLitI8, 1, 3);
    testSingleToken(std::to_string(i) + "i8", eToken::kLitI8, 1, 4);
  }
  for (int i = 10; i <= 99; i++) {
    testSingleToken(std::to_string(i) + "c", eToken::kLitI8, 1, 4);
    testSingleToken(std::to_string(i) + "i8", eToken::kLitI8, 1, 5);
  }
  for (int i = 100; i <= 127; i++) {
    testSingleToken(std::to_string(i) + "c", eToken::kLitI8, 1, 5);
    testSingleToken(std::to_string(i) + "i8", eToken::kLitI8, 1, 6);
  }
}

TEST(LexerTokens, LiteralI16) { testSingleToken("123i16", eToken::kLitI16); }

TEST(LexerTokens, LiteralI32) {
  testSingleToken("123", eToken::kLitI32, 1, 4);
  testSingleToken("123i32", eToken::kLitI32, 1, 7);
  // Make sure i32 doesn't get mistaken for an f32 if followed by ellipsis.
  testSingleToken("123...", eToken::kLitI32, "123", 1, 4);
}

TEST(LexerTokens, LiteralI64) {
  testSingleToken("123L", eToken::kLitI64, 1, 5);
  testSingleToken("123i64", eToken::kLitI64, 1, 7);
}

TEST(LexerTokens, LiteralF32) {
  testSingleToken("123.f", eToken::kLitF32, 1, 6);
  testSingleToken("123.456f", eToken::kLitF32, 1, 9);
}

TEST(LexerTokens, LiteralF64) {
  testSingleToken("123.", eToken::kLitF64, 1, 5);
  testSingleToken("123.456", eToken::kLitF64, 1, 8);
}

TEST(LexerTokens, LiteralReal) {
  testSingleToken("123.r", eToken::kLitReal, 1, 6);
  testSingleToken("123.456r", eToken::kLitReal, 1, 9);
}

TEST(LexerTokens, UnknownNumericSuffixError) {
  testCheckLexerErrorCode("123foo", ssgc::eError::kLexerUnknownNumericSuffix);
  testCheckLexerErrorCode("123.foo", ssgc::eError::kLexerUnknownNumericSuffix);
  testCheckLexerErrorCode("123.0foo", ssgc::eError::kLexerUnknownNumericSuffix);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Punctuators.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(LexerTokens, Hash) { testSingleToken("#", eToken::kHash, 1, 2); }

TEST(LexerTokens, Add) { testSingleToken("+", eToken::kAdd, 1, 2); }

TEST(LexerTokens, Sub) { testSingleToken("-", eToken::kSub, 1, 2); }

TEST(LexerTokens, Mul) { testSingleToken("*", eToken::kMul, 1, 2); }

TEST(LexerTokens, Div) { testSingleToken("/", eToken::kDiv, 1, 2); }

TEST(LexerTokens, Mod) { testSingleToken("%", eToken::kMod, 1, 2); }

TEST(LexerTokens, And) { testSingleToken("&&", eToken::kAnd, 1, 3); }

TEST(LexerTokens, Or) { testSingleToken("||", eToken::kOr, 1, 3); }

TEST(LexerTokens, Xor) { testSingleToken("^", eToken::kXor, 1, 2); }

TEST(LexerTokens, Not) { testSingleToken("!", eToken::kNot, 1, 2); }

TEST(LexerTokens, Lsh) { testSingleToken("<<", eToken::kLsh, 1, 3); }

TEST(LexerTokens, Rsh) { testSingleToken(">>", eToken::kRsh, 1, 3); }

TEST(LexerTokens, Eq) { testSingleToken("==", eToken::kEq, 1, 3); }

TEST(LexerTokens, Neq) { testSingleToken("!=", eToken::kNeq, 1, 3); }

TEST(LexerTokens, Lt) { testSingleToken("<", eToken::kLt, 1, 2); }

TEST(LexerTokens, Gt) { testSingleToken(">", eToken::kGt, 1, 2); }

TEST(LexerTokens, Lte) { testSingleToken("<=", eToken::kLte, 1, 3); }

TEST(LexerTokens, Gte) { testSingleToken(">=", eToken::kGte, 1, 3); }

TEST(LexerTokens, Spaceship) { testSingleToken("<=>", eToken::kSpaceship, 1, 4); }

TEST(LexerTokens, Assign) { testSingleToken("=", eToken::kAssign, 1, 2); }

TEST(LexerTokens, NewAssign) { testSingleToken(":=", eToken::kNewAssign, 1, 3); }

TEST(LexerTokens, AddAssign) { testSingleToken("+=", eToken::kAddAssign, 1, 3); }

TEST(LexerTokens, SubAssign) { testSingleToken("-=", eToken::kSubAssign, 1, 3); }

TEST(LexerTokens, MulAssign) { testSingleToken("*=", eToken::kMulAssign, 1, 3); }

TEST(LexerTokens, DivAssign) { testSingleToken("/=", eToken::kDivAssign, 1, 3); }

TEST(LexerTokens, ModAssign) { testSingleToken("%=", eToken::kModAssign, 1, 3); }

TEST(LexerTokens, AndAssign) { testSingleToken("&=", eToken::kAndAssign, 1, 3); }

TEST(LexerTokens, OrAssign) { testSingleToken("|=", eToken::kOrAssign, 1, 3); }

TEST(LexerTokens, XorAssign) { testSingleToken("^=", eToken::kXorAssign, 1, 3); }

TEST(LexerTokens, LshAssign) { testSingleToken("<<=", eToken::kLshAssign, 1, 4); }

TEST(LexerTokens, RshAssign) { testSingleToken(">>=", eToken::kRshAssign, 1, 4); }

TEST(LexerTokens, Inc) { testSingleToken("++", eToken::kInc, 1, 3); }

TEST(LexerTokens, Dec) { testSingleToken("--", eToken::kDec, 1, 3); }

TEST(LexerTokens, Bnot) { testSingleToken("~", eToken::kBnot, 1, 2); }

TEST(LexerTokens, Band) { testSingleToken("&", eToken::kBand, 1, 2); }

TEST(LexerTokens, Bor) { testSingleToken("|", eToken::kBor, 1, 2); }

TEST(LexerTokens, LParen) { testSingleToken("(", eToken::kLParen, 1, 2); }

TEST(LexerTokens, RParen) { testSingleToken(")", eToken::kRParen, 1, 2); }

TEST(LexerTokens, LBrace) { testSingleToken("{", eToken::kLBrace, 1, 2); }

TEST(LexerTokens, RBrace) { testSingleToken("}", eToken::kRBrace, 1, 2); }

TEST(LexerTokens, LBracket) { testSingleToken("[", eToken::kLBracket, 1, 2); }

TEST(LexerTokens, RBracket) { testSingleToken("]", eToken::kRBracket, 1, 2); }

TEST(LexerTokens, Semicolon) { testSingleToken(";", eToken::kSemicolon, 1, 2); }

TEST(LexerTokens, Colon) { testSingleToken(":", eToken::kColon, 1, 2); }

TEST(LexerTokens, Comma) { testSingleToken(",", eToken::kComma, 1, 2); }

TEST(LexerTokens, Period) { testSingleToken(".", eToken::kPeriod, 1, 2); }

TEST(LexerTokens, DoubleColon) { testSingleToken("::", eToken::kDoubleColon, 1, 3); }

TEST(LexerTokens, Ellipsis) { testSingleToken("...", eToken::kEllipsis, 1, 4); }

TEST(LexerTokens, CommercialAt) { testSingleToken("@", eToken::kCommercialAt, 1, 2); }

TEST(LexerTokens, Dollar) { testSingleToken("$", eToken::kDollar, 1, 2); }

TEST(LexerTokens, Backlash) { testSingleToken("\\", eToken::kBacklash, 1, 2); }

TEST(LexerTokens, Question) { testSingleToken("?", eToken::kQuestion, 1, 2); }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Whitespace/comments.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(LexerTokens, Whitespace) {
  testSingleToken(" ", eToken::kWhitespace, 1, 2);
  testSingleToken("\t", eToken::kWhitespace, 1, 2);
}

TEST(LexerTokens, Newline) {
  // Unix newline style.
  testSingleToken("\n", eToken::kNewline, 1, 1, 1, 2);
  testSingleToken("\n\n", eToken::kNewline, 1, 1, 1, 3);

  // Windows newline style.
  testSingleToken("\r\n", eToken::kNewline, 1, 1, 1, 2, 2);
  testSingleToken("\r\n\r\n", eToken::kNewline, 1, 1, 1, 3, 4);

  // Old mac newline style.
  testSingleToken("\r", eToken::kNewline, 1, 1, 1, 2);
  testSingleToken("\r\r", eToken::kNewline, 1, 1, 1, 3);
}

TEST(LexerTokens, LineComment) { testSingleToken("`comment", eToken::kLineComment, 1, 9); }

TEST(LexerTokens, BlockComment) {
  testSingleToken("/` comment `/", eToken::kBlockComment, 1, 14);
  testSingleToken("/` comment ``/", eToken::kBlockComment, 1, 15);
  testSingleToken("/` comment \n`/", eToken::kBlockComment, 1, 3, 1, 2, 14);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Character sequences.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(LexerTokens, EscapedCharSequence) {
  testSingleToken("\"Hello World\"", eToken::kLitCstr, 1, 14);
  // Allow escaped quotes & escaped backlash before quote.
  testSingleToken("\"\\\"Hello World\\\"\"", eToken::kLitCstr, 1, 18);
  // String ends exactly at the closing quote.
  testSingleToken("\"Hello\"\"World\"", eToken::kLitCstr, "\"Hello\"", 1, 8, 1, 1);
  // Line continuation with escaped newline. ( Correct line tracking? )
  testSingleToken("\"Hello \\\nWorld\"", eToken::kLitCstr, 1, 7, 1, 2, 15);
}

TEST(LexerTokens, EscapedCharSequenceError) {
  // Unclosed string.
  testCheckLexerErrorCode("\"Hello World\\\"", ssgc::eError::kLexerUnclosedStringLiteral);

  // String ending in a lone backlash.
  testCheckLexerErrorCode("\"Hello World\\", ssgc::eError::kLexerUnclosedStringLiteral);
}

TEST(LexerTokens, CharacterLiteral) {
  testSingleToken("'a'", eToken::kLitI8, 1, 4);
  testSingleToken("'\\n'", eToken::kLitI8, 1, 5);
  testSingleToken("'\\''", eToken::kLitI8, 1, 5);
  testSingleToken("'\\\\'", eToken::kLitI8, 1, 5);
  testSingleToken("'\\t'", eToken::kLitI8, 1, 5);
}

TEST(LexerTokens, CharacterLiteralError) {
  // Eof before closing quote.
  testCheckLexerErrorCode("'", ssgc::eError::kLexerUnclosedCharacterLiteral);
  // Newline in char literal.
  testCheckLexerErrorCode("'\n'", ssgc::eError::kLexerUnclosedCharacterLiteral);
  // Ending in a lone backlash.
  testCheckLexerErrorCode("'\\", ssgc::eError::kLexerUnclosedCharacterLiteral);
  // Unclosed char literal.
  testCheckLexerErrorCode("'a + unclosed", ssgc::eError::kLexerUnclosedCharacterLiteral);
  // Empty char literal.
  testCheckLexerErrorCode("''", ssgc::eError::kLexerEmptyCharacterLiteral);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Identifier and keywords.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(LexerTokens, Ident) {
  testSingleToken("foo", eToken::kIdent, 1, 4);
  testSingleToken("foo_", eToken::kIdent, 1, 5);
  testSingleToken("foo_123", eToken::kIdent, 1, 8);
  testSingleToken("_foo_123", eToken::kIdent, 1, 9);
}

// TEST(LexerTokens, KeywordInclude) { testSingleToken("include", eToken::kKwInclude, 1, 8); }
// TEST(LexerTokens, KeywordPragma) { testSingleToken("pragma", eToken::kKwPragma, 1, 7); }

TEST(LexerTokens, KeywordDef) { testSingleToken("def", eToken::kKwDef, 1, 4); }
TEST(LexerTokens, KeywordFn) { testSingleToken("fn", eToken::kKwFn, 1, 3); }
TEST(LexerTokens, KeywordClass) { testSingleToken("class", eToken::kKwClass, 1, 6); }
TEST(LexerTokens, KeywordMain) { testSingleToken("main", eToken::kKwMain, 1, 5); }
TEST(LexerTokens, KeywordImport) { testSingleToken("import", eToken::kKwImport, 1, 7); }
TEST(LexerTokens, KeywordNamespace) { testSingleToken("namespace", eToken::kKwNamespace, 1, 10); }
TEST(LexerTokens, KeywordUse) { testSingleToken("using", eToken::kKwUse, 1, 6); }
TEST(LexerTokens, KeywordLib) { testSingleToken("lib", eToken::kKwLib, 1, 4); }
TEST(LexerTokens, KeywordDll) { testSingleToken("dll", eToken::kKwDll, 1, 4); }
TEST(LexerTokens, KeywordEnum) { testSingleToken("enum", eToken::kKwEnum, 1, 5); }

TEST(LexerTokens, KeywordIf) { testSingleToken("if", eToken::kKwIf, 1, 3); }
TEST(LexerTokens, KeywordElif) { testSingleToken("elif", eToken::kKwElif, 1, 5); }
TEST(LexerTokens, KeywordElse) { testSingleToken("else", eToken::kKwElse, 1, 5); }
TEST(LexerTokens, KeywordCxif) { testSingleToken("cxif", eToken::kKwCxif, 1, 5); }
TEST(LexerTokens, KeywordCxelif) { testSingleToken("cxelif", eToken::kKwCxelif, 1, 7); }
TEST(LexerTokens, KeywordCxelse) { testSingleToken("cxelse", eToken::kKwCxelse, 1, 7); }

TEST(LexerTokens, KeywordSwitch) { testSingleToken("switch", eToken::kKwSwitch, 1, 7); }
TEST(LexerTokens, KeywordCase) { testSingleToken("case", eToken::kKwCase, 1, 5); }
TEST(LexerTokens, KeywordDefault) { testSingleToken("default", eToken::kKwDefault, 1, 8); }

TEST(LexerTokens, KeywordWhile) { testSingleToken("while", eToken::kKwWhile, 1, 6); }
TEST(LexerTokens, KeywordFor) { testSingleToken("for", eToken::kKwFor, 1, 4); }

TEST(LexerTokens, KeywordReturn) { testSingleToken("return", eToken::kKwReturn, 1, 7); }
TEST(LexerTokens, KeywordBreak) { testSingleToken("break", eToken::kKwBreak, 1, 6); }
TEST(LexerTokens, KeywordContinue) { testSingleToken("continue", eToken::kKwContinue, 1, 9); }

TEST(LexerTokens, KeywordInt) { testSingleToken("int", eToken::kKwInt, 1, 4); }
TEST(LexerTokens, KeywordUint) { testSingleToken("uint", eToken::kKwUint, 1, 5); }
TEST(LexerTokens, KeywordReal) { testSingleToken("real", eToken::kKwReal, 1, 5); }
TEST(LexerTokens, KeywordBool) { testSingleToken("bool", eToken::kKwBool, 1, 5); }
TEST(LexerTokens, KeywordChar) { testSingleToken("char", eToken::kKwChar, 1, 5); }
TEST(LexerTokens, KeywordByte) { testSingleToken("byte", eToken::kKwByte, 1, 5); }
TEST(LexerTokens, KeywordCstr) { testSingleToken("cstr", eToken::kKwCstr, 1, 5); }
TEST(LexerTokens, KeywordStr) { testSingleToken("str", eToken::kKwStr, 1, 4); }
TEST(LexerTokens, KeywordPtr) { testSingleToken("ptr", eToken::kKwPtr, 1, 4); }
TEST(LexerTokens, KeywordList) { testSingleToken("list", eToken::kKwList, 1, 5); }
TEST(LexerTokens, KeywordArray) { testSingleToken("array", eToken::kKwArray, 1, 6); }

TEST(LexerTokens, KeywordTrue) { testSingleToken("true", eToken::kKwTrue, 1, 5); }
TEST(LexerTokens, KeywordFalse) { testSingleToken("false", eToken::kKwFalse, 1, 6); }
TEST(LexerTokens, KeywordNone) { testSingleToken("none", eToken::kKwNone, 1, 5); }
TEST(LexerTokens, KeywordVoid) { testSingleToken("void", eToken::kKwVoid, 1, 5); }

TEST(LexerTokens, KeywordIn) { testSingleToken("in", eToken::kKwIn, 1, 3); }
TEST(LexerTokens, KeywordAs) { testSingleToken("as", eToken::kKwAs, 1, 3); }

TEST(LexerTokens, KeywordCin) { testSingleToken("cin", eToken::kKwCin, 1, 4); }
TEST(LexerTokens, KeywordCout) { testSingleToken("cout", eToken::kKwCout, 1, 5); }

TEST(LexerTokens, KeywordNative) { testSingleToken("native", eToken::kKwNative, 1, 7); }
TEST(LexerTokens, KeywordConst) { testSingleToken("const", eToken::kKwConst, 1, 6); }
TEST(LexerTokens, KeywordRef) { testSingleToken("ref", eToken::kKwRef, 1, 4); }

TEST(LexerTokens, KeywordPrivate) { testSingleToken("private", eToken::kKwPrivate, 1, 8); }
TEST(LexerTokens, KeywordPublic) { testSingleToken("public", eToken::kKwPublic, 1, 7); }
TEST(LexerTokens, KeywordStatic) { testSingleToken("static", eToken::kKwStatic, 1, 7); }

TEST(LexerTokens, KeywordAny) { testSingleToken("any", eToken::kKwAny, 1, 4); }
TEST(LexerTokens, KeywordAuto) { testSingleToken("auto", eToken::kKwAuto, 1, 5); }
TEST(LexerTokens, KeywordType) { testSingleToken("type", eToken::kKwType, 1, 5); }
TEST(LexerTokens, KeywordValue) { testSingleToken("value", eToken::kKwValue, 1, 6); }

TEST(LexerTokens, KeywordTemplate) { testSingleToken("template", eToken::kKwTemplate, 1, 9); }
TEST(LexerTokens, KeywordProc) { testSingleToken("proc", eToken::kKwProc, 1, 5); }

TEST(LexerTokens, AllOperands) {
  testMultipleTokens(
      "1b 0b 42B 42u8 123u16 123u 123u32 123U "
      "123u64 123c 123i8 123i16 123 123i32 123L 123i64 123.f "
      "123.456f 123. 123.456 123.r 123.456r "
      "\"string literal\" \"\\\\\" alnumus true false none ",
      {{eToken::kLitU1, "1b"},
       {eToken::kLitU1, "0b"},
       {eToken::kLitU8, "42B"},
       {eToken::kLitU8, "42u8"},
       {eToken::kLitU16, "123u16"},
       {eToken::kLitU32, "123u"},
       {eToken::kLitU32, "123u32"},
       {eToken::kLitU64, "123U"},
       {eToken::kLitU64, "123u64"},
       {eToken::kLitI8, "123c"},
       {eToken::kLitI8, "123i8"},
       {eToken::kLitI16, "123i16"},
       {eToken::kLitI32, "123"},
       {eToken::kLitI32, "123i32"},
       {eToken::kLitI64, "123L"},
       {eToken::kLitI64, "123i64"},
       {eToken::kLitF32, "123.f"},
       {eToken::kLitF32, "123.456f"},
       {eToken::kLitF64, "123."},
       {eToken::kLitF64, "123.456"},
       {eToken::kLitReal, "123.r"},
       {eToken::kLitReal, "123.456r"},
       {eToken::kLitCstr, "\"string literal\""},
       {eToken::kLitCstr, "\"\\\\\""},
       {eToken::kIdent, "alnumus"},
       {eToken::kKwTrue, "true"},
       {eToken::kKwFalse, "false"},
       {eToken::kKwNone, "none"}},
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