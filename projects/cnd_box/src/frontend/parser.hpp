///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup
/// @brief
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup cnd_compiler_data
/// @{
#pragma once
// clang-format off
#include <vector>
#include <span>
#include <expected>

#include "../common/diagnostic.hpp"
#include "token.hpp"
#include "ast.hpp"
#include "token_cursor.hpp"

// clang-format on

/// File local macro, returns a debug error for creating and debugging compiler errors on the fly.
#define DEBUG_FAIL(msg)                                                         \
  std::unexpected(makeErrorParserExpectedToken(std::source_location::current(), \
                                               c.get().source_range.file,       \
                                               c.get().source_range.end - 1, c.kind(), msg))
// std::unexpected(makeErrorDeveloperBug(std::source_location::current(), msg))
#define DEBUG_MSG(msg) \
  MakeClMsg<eClErr::kCompilerDevDebugError>(std::source_location::current(), msg)

#define SSGC_PARSER_EXPECTED_TOKEN(expected_tokens)                                            \
  std::unexpected(                                                                             \
      makeErrorParserExpectedToken(std::source_location::current(), c.get().source_range.file, \
                                   c.get().source_range.end - 1, c.kind(), expected_tokens))
#ifdef _DEBUG
#define CND_CLDEV_DEBUG_MODE 1
#define CND_DEBUG_ASSERT(expression, msg)                                                        \
  if ((!!!(expression))) {                                                                       \
    throw(std::string(#expression "|" msg "\n[") + std::source_location::current().file_name() + \
          "]" + "\n[" + std::source_location::current().function_name() + "]");                  \
  }
#else
#define CND_CLDEV_DEBUG_MODE
#define CND_DEBUG_ASSERT(expression, msg)
#endif

namespace ssgc {
namespace frontend {
namespace parser {
// using cldev::clmsg::MakeClMsg;
// using std::source_location;
// using std::span;

/// Underlying data structure of the token array passed to the parser.
using TkVectorT = std::vector<Token>;
using TkVecConstIterT = std::vector<Token>::const_iterator;
using TkVecIterT = std::vector<Token>::iterator;
using TkContainerT = std::span<const Token>;
using TkConstIterT = TkContainerT::iterator;
using TkIterT = TkContainerT::iterator;

/// Iterator over a specific const Token range. Used by the parser when iterating passed data.
using TkCursorT = TokenCursor;
// using TkScopeT = TkScope<std::span>;

/// Result of a LL recursive parsing method. A TkCursorT from where to continue reading and the
/// resulting Ast.
struct LLParserResult {
  std::size_t head{};
  Ast ast{};
};

/// Result of a LL recursive parsing method if it may fail and return an error(most may fail).
using LLPrsResT = std::expected<LLParserResult, Diagnostic>;
using LRPrsResT = std::expected<Ast, Diagnostic>;
// using ScopePrsResT = std::expected<TkScopeT, Diagnostic>;
// using SepScopePrsResT = std::expected<std::vector<TkScopeT>, Diagnostic>;

/// @defgroup cand_compiler_parser_util Utilities for Common Parser Patterns
/// @ingroup cand_compiler_parser
/// @brief Utilities for Common Parser Patterns.
///
/// Only for use within the parser's internal implementation.
namespace detail {

/// Advance cursor to parse result head. Move and append ast node from parse result to root ast.
constexpr void AppendAndAdvance(TkCursorT& cursor, Ast& root, LLPrsResT& parse_res) noexcept {
  cursor.advanceTo(parse_res.value().head);
  root.branches.push_back(std::move(parse_res.value().ast));
};

// Advance cursor to parse result head. Return forwarded r-value ast node from parse result.
constexpr Ast&& ExtractAndAdvance(TkCursorT cursor, LLPrsResT& parse_res) noexcept {
  cursor.advanceTo(parse_res.value().head);
  return std::forward<Ast>(std::move(parse_res.value().ast));
};

/// @brief Parses possibly existing modifiers at the start of a decl and advances cursor if
/// necessary.
/// @param c Token cursor which will be advanced past the modifiers or stay in place if none.
/// @return Error or the resulting ast node.
constexpr LLPrsResT ParseOptionalModifiers(TkCursorT& c);
// constexpr LLPrsResT ParseGenericBinaryLeftAssociative(
//     TkCursorT c, bool (*next_cond)(const TkCursorT&),
//     LLPrsResT (*operand_parser)(TkCursorT)) noexcept;

constexpr LLPrsResT reduceBinaryLeftAssociative(TkCursorT c,
                                                bool (*match_binary_operation)(const TkCursorT&),
                                                LLPrsResT (*operand_parser)(TkCursorT)) noexcept;

/// @brief Template for parsing declarations with common structure: modifiers? keyword @ identifier
/// [: definition]
/// @tparam KeywordCheck Function to check if current token is the expected keyword
/// @tparam ParseSignature Optional function to parse signature/type information between @ and
/// identifier
/// @tparam ParseDef Function to parse the definition body (when followed by :)
/// @tparam AstType The AST node type for the resulting declaration
struct DeclParser {
  /// Parse: <modifiers?> <keyword> <@> <identifier> [: <definition>] | ;
  /// Returns the complete declaration AST node
  static constexpr LLPrsResT ParseNamedDeclaration(TkCursorT c, eToken keyword, eAst decl_ast_type,
                                                   eAst def_ast_type,
                                                   LLPrsResT (*parse_def)(TkCursorT)) noexcept {
    using enum eToken;
    using namespace detail;

    auto decl_begin = c.index();

    // Parse optional modifiers
    LLPrsResT mods = ParseOptionalModifiers(c);
    if (!mods) return mods;
    Ast mod_node = mods.value().ast;
    c.advanceTo(mods->head);

    // Expect keyword
    if (!c.is(keyword)) return DEBUG_FAIL("Expected declaration keyword.");
    c.advance();

    // Expect commercial @
    if (!c.is(kCommercialAt)) return SSGC_PARSER_EXPECTED_TOKEN(kCommercialAt);
    c.advance();

    // Expect identifier
    if (!c.is(kIdent)) return DEBUG_FAIL("Expected <identifier>.");
    Ast ident_node = c;
    c.advance();

    // Check for definition (colon) or declaration (semicolon)
    if (c.is(eToken::kColon)) {
      c.advance();
      auto def_result = parse_def(c);
      if (!def_result) return def_result;

      Ast def_node = def_result.value().ast;
      c.advanceTo(def_result.value().head);

      return LLParserResult(c.index(), Ast(decl_ast_type, decl_begin, c.index(), nullptr,
                                           {mod_node, ident_node, def_node}));
    } else if (c.is(eToken::kSemicolon)) {
      c.advance();
      return LLParserResult(
          c.index(), Ast(decl_ast_type, decl_begin, c.index(), nullptr, {mod_node, ident_node}));
    } else {
      return DEBUG_FAIL("Expected <:> or <;>.");
    }
  }

  /// Parse: <modifiers?> <keyword> [: <definition>] | ;
  /// For unnamed declarations (like unnamed lib or unnamed proc)
  static constexpr LLPrsResT ParseUnnamedDeclaration(TkCursorT c, eToken keyword,
                                                     eAst decl_ast_type,
                                                     LLPrsResT (*parse_def)(TkCursorT)) noexcept {
    using enum eToken;
    using namespace detail;

    auto decl_begin = c.index();

    // Parse optional modifiers
    LLPrsResT mods = ParseOptionalModifiers(c);
    if (!mods) return mods;
    Ast mod_node = mods.value().ast;

    // Expect keyword
    if (!c.is(keyword)) return DEBUG_FAIL("Expected declaration keyword.");
    c.advance();

    // Check for definition (colon)
    if (c.is(eToken::kColon)) {
      c.advance();
      auto def_result = parse_def(c);
      if (!def_result) return def_result;

      c.advanceTo(def_result.value().head);
      return LLParserResult(
          c.index(), Ast(decl_ast_type, decl_begin, c.index(), {mod_node, def_result.value().ast}));
    } else {
      return DEBUG_FAIL("Expected <:> with definition.");
    }
  }
};

}  // namespace detail

/// @defgroup cand_compiler_parser_parse Internal parsing methods
/// @ingroup cand_compiler_parser
/// @brief LL Recursive Parsing Methods.
///
/// Ordered from the most to least specific. Except ParseTokens which is the main method to call
/// when parsing. The rest are for internal use. Each method is responsible for parsing a specific
/// type of statement or declaration. Each method returns an ParserResultT. The result contains the
/// parsed AST node, and the location of the last token parsed. Begin parsing the next statement
/// from this location. The result also contains an error message if the parsing failed. In this
/// case parsing should stop and the error message should be reported.
/// @{

constexpr LLPrsResT ParseOperand(TkCursorT c) noexcept;
constexpr LLPrsResT ParseArguments(TkCursorT c) noexcept;
constexpr LLPrsResT ParseIndexingArguments(TkCursorT c) noexcept;
constexpr LLPrsResT ParseListingArguments(TkCursorT c) noexcept;
constexpr LLPrsResT ParsePrimaryStatement(TkCursorT c) noexcept;
constexpr LLPrsResT ParseConditionalSubExpression(TkCursorT c) noexcept;
constexpr LLPrsResT ParsePrimaryPreIdentifier(TkCursorT c) noexcept;
constexpr LLPrsResT ParsePrimaryPostIdentifier(TkCursorT c) noexcept;
constexpr LLPrsResT ParseModifiers(TkCursorT c) noexcept;
constexpr LLPrsResT ParseReturnStmt(TkCursorT c) noexcept;
constexpr LLPrsResT ParseMethodParameters(TkCursorT c) noexcept;
constexpr LLPrsResT ParseMethodReturnParameters(TkCursorT c) noexcept;
constexpr LLPrsResT ParseMethodSignature(TkCursorT c) noexcept;
constexpr LLPrsResT ParseMethodDef(TkCursorT c) noexcept;
constexpr LLPrsResT ParseMainDef(TkCursorT c) noexcept;
constexpr LLPrsResT ParseClassDef(TkCursorT c) noexcept;
constexpr LLPrsResT ParseLibDef(TkCursorT c) noexcept;
constexpr LLPrsResT ParsePragmaticStmt(TkCursorT c) noexcept;
constexpr LLPrsResT ParseFunctionalStmt(TkCursorT c) noexcept;
constexpr LLPrsResT ParseConditionalStmt(TkCursorT c) noexcept;
constexpr LLPrsResT ParseIfDecl(TkCursorT c) noexcept;
constexpr LLPrsResT ParseMainDecl(TkCursorT c) noexcept;
constexpr LLPrsResT ParseLibDecl(TkCursorT c) noexcept;
constexpr LLPrsResT ParseImportDecl(TkCursorT c) noexcept;
constexpr LLPrsResT ParseIncludeStmt(TkCursorT c) noexcept;
constexpr LLPrsResT ParsePragmaStmt(TkCursorT c) noexcept;

constexpr LLPrsResT ParseWhileDecl(TkCursorT c) noexcept;
constexpr LLPrsResT ParseForDecl(TkCursorT c) noexcept;
constexpr LLPrsResT ParseUsingDecl(TkCursorT c) noexcept;
constexpr LLPrsResT ParseVariableDecl(TkCursorT c) noexcept;
constexpr LLPrsResT ParseMethodDecl(TkCursorT c) noexcept;
constexpr LLPrsResT ParseClassDecl(TkCursorT c) noexcept;
constexpr LLPrsResT ParseEnumDecl(TkCursorT c) noexcept;
constexpr LLPrsResT ParseEnumDef(TkCursorT c) noexcept;
constexpr LLPrsResT ParseEnumBlock(TkCursorT c) noexcept;
constexpr LLPrsResT ParseSyntax(TkCursorT c) noexcept;
constexpr LRPrsResT ParseExpr(TkCursorT c) noexcept;
constexpr LLPrsResT ParseNamespaceDecl(TkCursorT c) noexcept;
constexpr LLPrsResT ParsePrimaryExpr(TkCursorT c) noexcept;

// constexpr LLPrsResT ParseSubExpr(TkCursorT c) noexcept;
constexpr LLPrsResT ParseListFold(TkCursorT c) noexcept;
constexpr LLPrsResT ParseAssignment(TkCursorT c) noexcept;
constexpr LLPrsResT ParseLogicalOr(TkCursorT c) noexcept;
constexpr LLPrsResT ParseLogicalAnd(TkCursorT c) noexcept;
constexpr LLPrsResT ParseBitwiseOr(TkCursorT c) noexcept;
constexpr LLPrsResT ParseBitwiseXor(TkCursorT c) noexcept;
constexpr LLPrsResT ParseBitwiseAnd(TkCursorT c) noexcept;
constexpr LLPrsResT ParseEquality(TkCursorT c) noexcept;
constexpr LLPrsResT ParseComparison(TkCursorT c) noexcept;
constexpr LLPrsResT ParseThreeWayEquality(TkCursorT c) noexcept;
constexpr LLPrsResT ParseBitwiseShift(TkCursorT c) noexcept;
constexpr LLPrsResT ParseSummation(TkCursorT c) noexcept;
constexpr LLPrsResT ParseProduction(TkCursorT c) noexcept;
constexpr LLPrsResT ParsePrefix(TkCursorT c) noexcept;
constexpr LLPrsResT ParsePostfix(TkCursorT c) noexcept;
constexpr LLPrsResT ParseResolution(TkCursorT c) noexcept;

constexpr LLPrsResT ParseScopedArguments(TkCursorT c) noexcept;
constexpr LLPrsResT ParseParenArguments(TkCursorT c) noexcept;
constexpr LLPrsResT ParseSquareArguments(TkCursorT c) noexcept;
constexpr LLPrsResT ParseCurlyArguments(TkCursorT c) noexcept;

constexpr LLPrsResT ParsePrimaryOperand(TkCursorT c) noexcept;
constexpr LLPrsResT ParseOperandSet(TkCursorT c) noexcept;
constexpr LLPrsResT ParseIdentityOperand(TkCursorT c) noexcept;
constexpr LLPrsResT ParseValueOperand(TkCursorT c) noexcept;
constexpr LLPrsResT ParseParenSubexpr(TkCursorT c) noexcept;
constexpr LLPrsResT ParseSquareSubexpr(TkCursorT c) noexcept;
constexpr LLPrsResT ParseCurlySubexpr(TkCursorT c) noexcept;

/// @} // end of cand_compiler_parser_parse

/// @defgroup cand_compiler_parser_scope Internal parsing methods
/// @ingroup cand_compiler_parser
/// @brief Methods to determining the beginnings and ends of a scope or set of separated scopes
/// froma tokenized source.
/// @see TkScope in tk_scope.hpp
/// @{

// constexpr ScopePrsResT FindParen(TkConstIterT begin, TkConstIterT end) noexcept;
// constexpr ScopePrsResT FindParen(TkCursorT crsr) noexcept;
// constexpr ScopePrsResT FindBrace(TkConstIterT begin, TkConstIterT end) noexcept;
// constexpr ScopePrsResT FindBrace(TkCursorT crsr) noexcept;
// constexpr ScopePrsResT FindBracket(TkConstIterT begin, TkConstIterT end) noexcept;
// constexpr ScopePrsResT FindBracket(TkCursorT crsr) noexcept;
//
// constexpr ScopePrsResT FindStatement(eToken open, eToken close, TkConstIterT begin,
//                                      TkConstIterT end) noexcept;
// constexpr ScopePrsResT FindOpenStatement(eToken close, TkConstIterT begin,
//                                          TkConstIterT end) noexcept;
// constexpr ScopePrsResT FindOpenStatement(std::vector<eToken> close, TkConstIterT begin,
//                                          TkConstIterT end) noexcept;
// constexpr ScopePrsResT FindProgramStatement(TkConstIterT begin, TkConstIterT end) noexcept;
// constexpr ScopePrsResT FindProgramStatement(TkCursorT cursor) noexcept;
//
// constexpr SepScopePrsResT FindSeperatedParen(TkConstIterT begin, TkConstIterT end,
//                                              eToken separator) noexcept;
// constexpr SepScopePrsResT FindSeperatedParen(TkCursorT crsr, eToken separator) noexcept;
// constexpr SepScopePrsResT FindSeperatedParen(TkScopeT ls, eToken separator) noexcept;
// constexpr SepScopePrsResT FindSeperatedBrace(TkConstIterT begin, TkConstIterT end,
//                                              eToken separator) noexcept;
// constexpr SepScopePrsResT FindSeperatedBrace(TkCursorT crsr, eToken separator) noexcept;
// constexpr SepScopePrsResT FindSeperatedBrace(TkScopeT ls, eToken separator) noexcept;
// constexpr SepScopePrsResT FindSeperatedBracket(TkConstIterT begin, TkConstIterT end,
//                                                eToken separator) noexcept;
// constexpr SepScopePrsResT FindSeperatedBracket(TkCursorT crsr, eToken separator) noexcept;
// constexpr SepScopePrsResT FindSeperatedBracket(const TkScopeT& ls, eToken separator) noexcept;

/// @} // end of cand_compiler_parser_scope

}  // namespace parser

///// @brief Parse pre-loaded or generated C& source file data.
///// @param src_data Source file data. EOF character should be the last element.
// static std::expected<Ast,Diagnostics> ParseSource(const std::vector<char>& src_data) {
//   StrView src_view = {src_data.begin(), src_data.end()};
//   auto lex_res = tokenizeSourceCode(src_view);
//   if (!lex_res) return CompilerProcessFailure(lex_res.error());
//   auto sanitized_src = sanitizeTokens(*lex_res);
//   std::span<const Token> src_span = std::span{sanitized_src.data(), sanitized_src.size()};
//   auto parse_res = parser::ParseSyntax({src_span.begin(), src_span.end()});
//   if (!parse_res) return CompilerProcessFailure(parse_res.error());
//   return parse_res->ast;
// }
//
///// @brief Parse a C& source file.
///// @param fp Source file path. Absolute or relative to working dir.
// static std::expected<Ast, Diagnostics> ParseFile(const Path& fp) {
//   auto loaded_src = LoadSourceFile<char>(fp.string());
//   if (!loaded_src) return CompilerProcessFailure(loaded_src.error());
//   return ParseSource(*loaded_src);
// }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Internal parsing methods impl */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace parser {
constexpr LLPrsResT ParseOperand(TkCursorT c) noexcept {
  if (isTokenAnOperand(c.kind())) {
    Ast node{c};
    c.advance();
    return LLParserResult(c.index(), node);
  } else {
    return DEBUG_FAIL("ImplExpectedToken");
  }
};

constexpr LLPrsResT ParseArguments(TkCursorT c) noexcept {
  std::size_t args_begin = c.index();

  if (!c.is(eToken::kLParen)) {
    return DEBUG_FAIL("Expected opening parenthesis.");
  }
  c.advance();

  if (c.is(eToken::kRParen)) {
    c.advance();
    return LLParserResult(c.index(), Ast(eAst::kArguments, args_begin, c.index()));
  }

  auto args = ParsePrimaryExpr(c);
  if (!args) return args;
  c.advanceTo(args->head);

  if (!c.is(eToken::kRParen)) {
    return DEBUG_FAIL("Expected closing parenthesis.");
  }

  return LLParserResult(c.advance().index(),
                        Ast(eAst::kArguments, args_begin, c.index(), {args->ast}));
}

constexpr LLPrsResT ParseIndexingArguments(TkCursorT c) noexcept {
  std::size_t args_begin = c.index();

  if (!c.is(eToken::kLBracket)) {
    return DEBUG_FAIL("Expected opening bracket.");
  }
  c.advance();

  if (c.is(eToken::kRBracket)) {
    c.advance();
    return LLParserResult(c.index(), Ast(eAst::kIndexArguments, args_begin, c.index()));
  }

  auto args = ParsePrimaryExpr(c);
  if (!args) return args;
  c.advanceTo(args->head);

  if (!c.is(eToken::kRBracket)) {
    return DEBUG_FAIL("Expected closing bracket.");
  }

  return LLParserResult(c.advance().index(),
                        Ast(eAst::kIndexArguments, args_begin, c.index(), {args->ast}));
}

constexpr LLPrsResT ParseListingArguments(TkCursorT c) noexcept {
  std::size_t args_begin = c.index();

  if (!c.is(eToken::kLBrace)) {
    return DEBUG_FAIL("Expected opening bracket.");
  }
  c.advance();

  if (c.is(eToken::kRBrace)) {
    c.advance();
    return LLParserResult(c.index(), Ast(eAst::kArguments, args_begin, c.index()));
  }

  auto args = ParsePrimaryExpr(c);
  if (!args) return args;
  c.advanceTo(args->head);

  if (!c.is(eToken::kRBrace)) {
    return DEBUG_FAIL("Expected closing bracket.");
  }

  return LLParserResult(c.advance().index(),
                        Ast(eAst::kArguments, args_begin, c.index(), {args->ast}));
}

constexpr LLPrsResT ParsePrimaryStatement(TkCursorT c) noexcept {
  if (!isTokenPrimary(c.kind())) {
    return DEBUG_FAIL("Expected primary expression specifier.");
  }
  LLPrsResT expr_result = ParsePrimaryExpr(c);
  if (!expr_result) {
    return expr_result;
  }
  c.advanceTo(expr_result->head);
  if (!c.is(eToken::kSemicolon)) {
    return DEBUG_FAIL("Expected semicolon after primary expression.");
  }
  c.advance();
  return LLParserResult(c.index(), expr_result->ast);
}

constexpr LLPrsResT ParseConditionalSubExpression(TkCursorT c) noexcept {
  std::size_t args_begin = c.index();

  if (!c.is(eToken::kLParen)) {
    return DEBUG_FAIL("Expected opening parenthesis.");
  }
  c.advance();

  if (c.is(eToken::kRParen)) {
    return DEBUG_FAIL("Expected primary expression.");
  }

  auto expr = ParsePrimaryExpr(c);
  if (!expr) return expr;
  c.advanceTo(expr->head);

  if (!c.is(eToken::kRParen)) {
    return DEBUG_FAIL("Expected closing parenthesis.");
  }

  return LLParserResult(c.advance().index(), expr->ast);
}

/// <pre-identifier> ::= <primary-type-expr>? <@>
constexpr LLPrsResT ParsePrimaryPreIdentifier(TkCursorT c) noexcept {
  // Cursor on <@>. No pre-identifier. Implicitly an any type.
  if (c.is(eToken::kCommercialAt)) {
    c.advance();
    return LLParserResult(c.index(), Ast(eAst::kKwAny, c.index(), c.index()));
  } else if (isTokenPrimary(c.kind())) {
    LLPrsResT expr_result = ParsePrimaryExpr(c);
    if (!expr_result) return LLPrsResT::unexpected_type(std::move(expr_result.error()));
    c.advanceTo(expr_result->head);
    if (c.is(eToken::kCommercialAt))
      c.advance();
    else
      return DEBUG_FAIL("Expecting a <@> after <primary-type-expression>.");
    return LLParserResult(c.index(), std::move(expr_result->ast));
  } else {
    return DEBUG_FAIL("Expecting a <primary-type-expression> or <@>.");
  }
}

/// ParsePrimaryPostIdentifier <primary_expr>: or <primary_expr>;
///
/// - Handles parsing of primary expressions that are followed by a colon or semicolon.
/// - Used in declarations, method signatures.
constexpr LLPrsResT ParsePrimaryPostIdentifier(TkCursorT c) noexcept {
  using enum eToken;
  // Cursor on <:>. No post-identifier. Implicitly an any type.
  if (c.is(eToken::kColon))
    return LLParserResult(c.advance().index(), Ast(eAst::kKwAny, c.index(), c.index()));
  else if (isTokenPrimary(c.kind())) {
    LLPrsResT expr_result = ParsePrimaryExpr(c);
    if (!expr_result) return LLPrsResT::unexpected_type(std::move(expr_result.error()));
    c.advanceTo(expr_result->head);

    // !SPECIAL CASE: if end of statement is a semicolon, go 1 offset back.
    // This will allow the caller to determine if the statement is terminated or not.
    if (c.is(eToken::kSemicolon))
      return LLParserResult(c.index(), std::move(expr_result->ast));
    else if (c.is(eToken::kColon)) {
      c.advance();
      return LLParserResult(c.index(), std::move(expr_result->ast));
    } else
      return DEBUG_FAIL("Expecting a <:> or <;> after <primary-type-expression>.");
  } else
    return DEBUG_FAIL("Expecting a <primary-type-expression> or <:>.");
}

constexpr LLPrsResT ParseModifiers(TkCursorT c) noexcept {
  if (!isTokenModifier(c.kind())) {
    return LLParserResult(c.index(), {eAst::kModifiers, c.index(), c.index()});
  }

  Ast modifiers_node(eAst::kModifiers, c.index(), c.index());
  while (isTokenModifier(c.kind())) {
    modifiers_node.branches.push_back(c);
    c.advance();
  }
  modifiers_node.setSourceEnd(c.index());
  return LLParserResult(modifiers_node.sourceEnd(), modifiers_node);
}

constexpr LLPrsResT ParseReturnStmt(TkCursorT c) noexcept {
  std::size_t return_begin = c.index();
  if (!c.is(eToken::kKwReturn)) {
    return DEBUG_FAIL("Expecting a <kw-return>.");
  }
  c.advance();

  // Empty return statement.
  if (c.is(eToken::kSemicolon)) {
    c.advance();
    Ast return_stmt{eAst::kKwReturn, return_begin, c.index()};
    return LLParserResult(return_stmt.sourceEnd(), return_stmt);
  }

  LLPrsResT expr_result = ParsePrimaryStatement(c);
  if (!expr_result) {
    return expr_result;
  }
  Ast return_stmt(eAst::kKwReturn, return_begin, expr_result->head, expr_result->ast);
  return LLParserResult(return_stmt.sourceEnd(), return_stmt);
}

constexpr LLPrsResT ParseFunctionalStmt(TkCursorT c) noexcept {
  using enum eToken;

  if (c.is(eToken::kSemicolon)) {
    return DEBUG_FAIL("Empty statement in code.");
  }

  if (isTokenDeclarative(c.kind())) {
    switch (c.kind()) {
      case kKwIf:
        return ParseIfDecl(c);
      case kKwWhile:
        return ParseWhileDecl(c);
      case kKwFor:
        return ParseForDecl(c);
      case kKwReturn:
        return ParseReturnStmt(c);
      case kKwUse:
        return ParseUsingDecl(c);
      case kKwImport:
        return ParseImportDecl(c);
      default:
        break;
    }
  }

  auto decl_begin = c;
  while (isTokenModifier(c.kind())) c.advance();

  if (isTokenDeclarative(c.kind())) {
    switch (c.kind()) {
      case kKwDef:
        return ParseVariableDecl(decl_begin);
      case kKwFn:
        return ParseMethodDecl(decl_begin);
      case kKwClass:
        return ParseClassDecl(decl_begin);
      case kKwUse:
        return DEBUG_FAIL("Declaration keyword cannot be modified.");
      default:
        return DEBUG_FAIL("Declaration keyword not permitted inside functional code block.");
    }
  } else
    return DEBUG_FAIL("Expected <functional-decl-keyword>.");
}

constexpr LLPrsResT ParseConditionalStmt(TkCursorT c) noexcept {
  using enum eToken;

  if (isTokenDeclarative(c.kind())) {
    switch (c.kind()) {
      case kKwUse:
        return ParseUsingDecl(c);
      case kKwImport:
        return ParseImportDecl(c);
      default:
        break;
    }
  }

  auto decl_begin = c;
  while (isTokenModifier(c.kind())) c.advance();

  if (isTokenDeclarative(c.kind())) {
    switch (c.kind()) {
      case kKwDef:
        return ParseVariableDecl(c);
      case kKwFn:
        return ParseMethodDecl(c);
      case kKwClass:
        return ParseClassDecl(c);
      case kKwUse:
        return DEBUG_FAIL("Declaration keyword cannot be modified.");
      default:
        return DEBUG_FAIL("Declaration keyword not permitted inside conditional code block.");
    }
  } else
    return DEBUG_FAIL("Expected <conditional-decl-keyword>.");
}

constexpr LLPrsResT ParseIfDecl(TkCursorT c) noexcept {
  using enum eToken;

  auto xParseIfElifElse = [&c](eAst ast_type) -> LLPrsResT {
    if (c.is(eToken::kKwIf) || c.is(eToken::kKwElif)) {
      std::size_t block_begin = c.index();
      c.advance();
      LLPrsResT cond_res = ParseConditionalSubExpression(c);
      if (!cond_res) return cond_res;

      c.advanceTo(cond_res->head);
      // Parse the body.
      LLPrsResT body_res = ParseMethodDef(c);
      if (!body_res) return body_res;
      c.advanceTo(body_res->head);
      Ast ret = Ast(ast_type, block_begin, c.index());
      ret.branches.push_back(std::move(cond_res.value().ast));
      ret.branches.push_back(std::move(body_res.value().ast));
      return LLParserResult(c.index(), ret);

    } else if (c.is(eToken::kKwElse)) {
      std::size_t block_begin = c.index();
      c.advance();
      LLPrsResT body_res = ParseMethodDef(c);
      if (!body_res) return body_res;
      c.advanceTo(body_res->head);
      Ast ret = Ast(ast_type, block_begin, c.index());
      ret.branches.push_back(std::move(body_res.value().ast));
      return LLParserResult(c.index(), std::move(ret));
    } else
      return DEBUG_FAIL("Expected if or elif or else.");
  };

  if (!c.is(kKwIf)) return DEBUG_FAIL("Expected if.");

  Ast ifelifelse_stmt{eAst::kIfStatement, c.index(), c.index()};
  auto if_stmt = xParseIfElifElse(eAst::kKwIf);
  if (!if_stmt) return if_stmt;
  ifelifelse_stmt.branches.push_back(std::move(if_stmt.value().ast));

  if (c.is(eToken::kSemicolon)) {
    c.advance();
    return LLParserResult(c.index(), std::move(ifelifelse_stmt));
  }

  while (c.is(eToken::kKwElif)) {
    auto elif_stmt = xParseIfElifElse(eAst::kKwElif);
    if (!elif_stmt) return elif_stmt;
    ifelifelse_stmt.branches.push_back(std::move(elif_stmt.value().ast));
  }

  if (c.is(eToken::kKwElse)) {
    auto else_stmt = xParseIfElifElse(eAst::kKwElse);
    if (!else_stmt) return else_stmt;
    ifelifelse_stmt.branches.push_back(std::move(else_stmt.value().ast));
  }

  return LLParserResult(c.index(), std::move(ifelifelse_stmt));
}

constexpr LLPrsResT ParseWhileDecl(TkCursorT c) noexcept {
  // While Keyword, Conditional subexpression, Functional Block, Semicolon.
  if (!c.is(eToken::kKwWhile)) return DEBUG_FAIL("Expected while.");
  auto block_begin = c.index();
  c.advance();

  LLPrsResT cond_res = ParseConditionalSubExpression(c);
  if (!cond_res) return cond_res;
  c.advanceTo(cond_res->head);

  LLPrsResT body_res = ParseMethodDef(c);
  if (!body_res) return body_res;
  c.advanceTo(body_res->head);

  if (c.is(eToken::kSemicolon)) {
    c.advance();
    Ast ret = Ast(eAst::kKwWhile, block_begin, c.index());
    ret.branches.push_back(cond_res.value().ast);
    ret.branches.push_back(body_res.value().ast);
    return LLParserResult(c.index(), std::move(ret));
  } else
    return DEBUG_FAIL("Expected <;> following while statement block.");
}

constexpr LLPrsResT ParseForDecl(TkCursorT c) noexcept {
  using namespace detail;
  if (!c.is(eToken::kKwFor)) return DEBUG_FAIL("Expected for.");
  std::size_t stmt_begin = c.index();
  c.advance();

  if (!c.is(eToken::kLParen))
    return DEBUG_FAIL("Expected opening parenthesis '(' after 'for' keyword.");
  c.advance();

  Ast node{};
  node.setSourceBegin(stmt_begin);

  // Optional loop local variable definition.
  if (c.is(eToken::kKwDef)) {
    auto init_var_res = ParseVariableDecl(c);
    if (!init_var_res) return init_var_res;
    detail::AppendAndAdvance(c, node, init_var_res);
  }

  // Required conditional expression.
  auto cond_res = ParsePrimaryExpr(c);
  if (!cond_res.has_value()) return cond_res;
  detail::AppendAndAdvance(c, node, cond_res);

  // Optional increment expression.
  if (c.is(eToken::kSemicolon)) {
    c.advance();
    auto inc_res = ParsePrimaryExpr(c);
    if (!inc_res) return inc_res;
    detail::AppendAndAdvance(c, node, inc_res);
  }

  if (!c.is(eToken::kRParen))
    return DEBUG_FAIL("Expected closing parenthesis ')' after 'for' condition.");
  c.advance();

  auto body_res = ParseMethodDef(c);
  if (!body_res) return body_res;
  detail::AppendAndAdvance(c, node, body_res);

  node.setSourceEnd(c.index());
  return LLParserResult{c.index(), node};
}

constexpr LLPrsResT ParseVariableDecl(TkCursorT c) noexcept {
  using enum eToken;
  using namespace detail;
  // Format:
  // <modifiers?><def><what*?><commercial_at?><name?><colon?><definition?>
  // <semicolon>
  auto decl_begin = c.index();
  LLPrsResT mods = ParseOptionalModifiers(c);
  if (!mods) return mods;
  Ast mod_node = mods.value().ast;

  // Expect a def keyword
  if (!c.is(kKwDef)) return DEBUG_FAIL("Expected <kw-def>.");
  c.advance();

  // Expect a 'what' token(s).
  // Everything following def until kCommericalAt is a type constraint describing 'what' is being
  // created.
  auto what_result = ParsePrimaryPreIdentifier(c);
  if (!what_result) return what_result;
  Ast what_node = std::move(what_result.value().ast);
  c.advanceTo(what_result->head);

  // Expecting an identifier.
  if (!c.is(kIdent)) return DEBUG_FAIL("Expected <identifier>.");
  Ast ident_node = c;
  c.advance();

  // If there is a colon, this is a Definition.
  // If there is a semicolon, this is a Declaration.
  if (c.is(eToken::kColon)) {
    c.advance();
    auto def_begin = c.index();
    auto def_result = ParsePrimaryStatement(c);
    if (!def_result) return def_result;
    c.advanceTo(def_result->head);
    Ast def_node =
        Ast(eAst::kVariableDefinition, def_begin, c.index(), nullptr, {def_result.value().ast});

    return LLParserResult(c.index(), Ast(eAst::kVariableDeclaration, decl_begin, c.index(), nullptr,
                                         {mod_node, what_node, ident_node, def_node}));
  } else if (c.is(eToken::kSemicolon)) {
    c.advance();
    return LLParserResult(c.index(), Ast(eAst::kVariableDeclaration, decl_begin, c.index(), nullptr,
                                         {mod_node, what_node, ident_node}));
  } else {
    return DEBUG_FAIL("Expected colon or semicolon.");
  }
}

constexpr LLPrsResT ParseMethodDecl(TkCursorT c) noexcept {
  using enum eToken;
  // Format:
  // <modifiers?><fn><commercial_at?><name?><signature?><colon?>
  // <definition?> <semicolon>
  Ast mod_node;
  auto decl_begin = c.index();

  auto mod_result = ParseModifiers(c);
  if (!mod_result) return mod_result;
  mod_node = std::move(mod_result.value().ast);
  c.advanceTo(mod_result.value().head);


  if (!c.is(eToken::kKwFn)) return DEBUG_FAIL("Expected <kw-def>.");
  c.advance();

  if (!c.is(eToken::kCommercialAt)) return DEBUG_FAIL("Expected <@>.");
  c.advance();

  if (!c.is(eToken::kIdent)) return DEBUG_FAIL("Expected <identifier>.");
  Ast ident_node = c;
  c.advance();

  Ast sig_node;
  auto sig_result = ParseMethodSignature(c);
  if (!sig_result) return sig_result;
  sig_node = std::move(sig_result.value().ast);
  c.advanceTo(sig_result.value().head);


  // If there is a colon, this is a Definition.
  // If there is a semicolon, this is a Declaration.
  if (c.is(eToken::kColon)) {
    c.advance();
    auto def_result = ParseMethodDef(c);
    if (!def_result) return def_result;

    Ast def_node = def_result.value().ast;
    c.advanceTo(def_result.value().head);
    // Expect semicolon
    if (!c.is(kSemicolon)) return DEBUG_FAIL("Expected <;>.");
    c.advance();
    return LLParserResult(c.index(), Ast(eAst::kMethodDeclaration, decl_begin, c.index(), nullptr,
                                         {mod_node, ident_node, sig_node, def_node}));

  } else if (c.is(eToken::kSemicolon)) {
    c.advance();
    return LLParserResult(c.index(), Ast(eAst::kMethodDeclaration, decl_begin, c.index(), nullptr,
                                         {mod_node, ident_node, sig_node}));

  } else {
    return DEBUG_FAIL("Expected <:> or <;>.");
  }
}

constexpr LLPrsResT ParseClassDecl(TkCursorT c) noexcept {
  using enum eToken;
  // Format:
  // <modifiers?><class><commercial_at?><name?><colon?>
  // <class_definition?> <semicolon>
  return detail::DeclParser::ParseNamedDeclaration(c, kKwClass, eAst::kClassDeclaration,
                                                   eAst::kClassDefinition, ParseClassDef);
}

constexpr LLPrsResT ParseImportDecl(TkCursorT c) noexcept {
  using enum eToken;
  auto stmt_begin = c.index();
  // Format: <import><name><semicolon>
  if (!c.is(kKwImport)) return DEBUG_FAIL("Expected <ident>.");
  c.advance();

  // Expecting an identifier.
  if (not c.is(eToken::kIdent)) return DEBUG_FAIL("Expected <ident>.");
  Ast ident_node = c;
  c.advance();

  // Expecting a semicolon.
  if (!c.is(kSemicolon)) return DEBUG_FAIL("Expected <:>.");
  c.advance();
  return LLParserResult(
      c.index(), Ast(eAst::kImportDeclaration, stmt_begin, c.index(), nullptr, {ident_node}));
}

constexpr LLPrsResT ParseLibDecl(TkCursorT c) noexcept {
  // Format:
  // <modifiers?><lib><commercial_at?><name?><colon?>
  // <lib_definition?> <semicolon>

  // Parse optional modifiers first
  auto decl_begin = c.index();
  LLPrsResT mods = detail::ParseOptionalModifiers(c);
  if (!mods) return mods;
  Ast mod_node = mods.value().ast;

  if (!c.is(eToken::kKwLib)) return DEBUG_FAIL("Expected <kw-lib>.");
  c.advance();

  // Check if this is an unnamed library (colon immediately follows lib keyword)
  if (c.is(eToken::kColon)) {
    c.advance();
    auto def_result = ParseLibDef(c);
    if (!def_result) return def_result;

    c.advanceTo(def_result->head);
    return LLParserResult(c.index(), Ast(eAst::kLibraryDeclaration, decl_begin, c.index(),
                                         {std::move(mod_node), std::move(def_result.value().ast)}));
  }

  // Otherwise, this is a named library
  if (!c.is(eToken::kCommercialAt)) return DEBUG_FAIL("Expected <@>.");
  c.advance();

  if (!c.is(eToken::kIdent)) return DEBUG_FAIL("Expected <identifier>.");
  Ast ident_node{c};
  c.advance();

  // Declaration or definition
  if (c.is(eToken::kSemicolon)) {
    c.advance();
    return LLParserResult(
        c.index(), Ast(eAst::kLibraryDeclaration, decl_begin, c.index(), {mod_node, ident_node}));
  }

  if (!c.is(eToken::kColon)) return DEBUG_FAIL("Expected <:>.");
  c.advance();

  auto def_result = ParseLibDef(c);
  if (!def_result) return def_result;

  Ast def_node = std::move(def_result.value().ast);
  c.advanceTo(def_result.value().head);
  return LLParserResult(c.index(), Ast(eAst::kLibraryDeclaration, decl_begin, c.index(),
                                       {mod_node, ident_node, def_node}));
}

constexpr LLPrsResT ParseMainDecl(TkCursorT c) noexcept {
  // Format:
  // <main><commerical_at?><name?><function_signature><colon?><definition?>
  // <semicolon>

  // Expect a main keyword
  if (!c.is(eToken::kKwMain)) return DEBUG_FAIL("Expected <kw-main>.");

  auto decl_begin = c.index();
  c.advance();

  // If there is a commercial at following the main keyword, this is a named
  // main. Expecting a commercial @ followed by an identifier.
  if (c.is(eToken::kCommercialAt)) {
    return DEBUG_FAIL("Named main not implemented.");
  } else {
    // This is an unnamed main.
    // Expecting a function signature followed by a colon and a definition.
    // Expecting a colon.
    Ast sig_node;
    if (c.is(eToken::kColon)) {
      c.advance();
      sig_node = Ast(eAst::kMethodSignature, decl_begin, c.index());
    } else {
      auto sig_result = ParseMethodSignature(c);
      if (!sig_result) return sig_result;
      sig_node = sig_result->ast;
      c.advanceTo(sig_result->head);
    }

    // <colon>
    if (!c.is(eToken::kColon)) {
      return DEBUG_FAIL(eToken::kColon);
    }
    c.advance();

    // Expecting a definition.
    auto def_result = ParseMainDef(c);
    if (!def_result) return def_result;
    Ast def_node = def_result->ast;
    c.advanceTo(def_result->head);

    // Expect semicolon
    if (!c.is(eToken::kSemicolon)) return DEBUG_FAIL("Expected <;>.");
    c.advance();
    return LLParserResult(c.index(),
                          Ast(eAst::kMainDeclaration, decl_begin, c.index(), {sig_node, def_node}));
  }
}

// <kMethodParameters> ::= <kLParen> (<kPrimaryExpr> <kCommercialAt> <Identifier>) <kRParen>
constexpr LLPrsResT ParseMethodParameters(TkCursorT c) noexcept {
  auto params_begin = c.index();
  if (!c.is(eToken::kLParen)) {
    return DEBUG_FAIL("Expected <(>.");
  }
  c.advance();

  if (c.is(eToken::kRParen)) {
    std::size_t method_param_begin{c.index()};
    c.advance();
    Ast ret_node{eAst::kMethodParameterList, params_begin, c.index()};
    ret_node.branches.push_back(
        Ast{eAst::kMethodParameter, method_param_begin, method_param_begin});
    ret_node.branches.back().branches.push_back(
        Ast{eAst::kKwVoid, method_param_begin, method_param_begin});
    ret_node.branches.back().branches.push_back(
        Ast{eAst::kIdent, method_param_begin, method_param_begin});
    return LLParserResult(c.index(), ret_node);
  }

  Ast ret_node{eAst::kMethodParameterList, params_begin, c.index()};

  while (!c.atEnd()) {
    bool has_modifier_node{false};
    Ast modifier_node{};
    std::size_t param_begin = c.index();

    if (isTokenModifier(c.kind())) {
      auto mod_result{ParseModifiers(c)};
      if (!mod_result) return mod_result;
      has_modifier_node = true;
      modifier_node = std::move(mod_result.value().ast);
      c.advanceTo(mod_result.value().head);
    }

    if (c.is(eToken::kCommercialAt)) {
      ret_node.branches.push_back({eAst::kMethodParameter, param_begin, c.index()});
      Ast& param_node{ret_node.branches.back()};

      if (has_modifier_node) {
        param_node.branches.push_back(modifier_node);
      }

      Ast type_expr_node{eAst::kKwAny, param_begin, param_begin};
      param_node.branches.push_back(std::move(type_expr_node));
      c.advance();

      if (!c.is(eToken::kIdent)) {
        return DEBUG_FAIL("Expected <identifier>.");
      }
      std::size_t identifier_begin = c.index();
      c.advance();
      Ast identifier_node{eAst::kIdent, identifier_begin, c.index()};
      param_node.branches.push_back(std::move(identifier_node));

      if (c.is(eToken::kComma)) {
        param_node.setSourceEnd(c.index());
        c.advance();
        continue;
      } else if (c.is(eToken::kRParen)) {
        param_node.setSourceEnd(c.index());
        c.advance();
        ret_node.setSourceEnd(c.index());
        return LLParserResult(c.index(), ret_node);
      } else {
        return DEBUG_FAIL("Expected <comma> or <right-parenthesis>.");
      }
    } else if (isTokenPrimarySpecifier(c.kind())) {
      ret_node.branches.push_back({eAst::kMethodParameter, param_begin, param_begin});
      Ast& param_node{ret_node.branches.back()};

      if (has_modifier_node) {
        param_node.branches.push_back(modifier_node);
      }

      auto param_type_res = ParsePrimaryExpr(c);
      if (!param_type_res) {
        return param_type_res;
      }
      c.advanceTo(param_type_res->head);
      param_node.branches.push_back(param_type_res->ast);

      if (!c.is(eToken::kCommercialAt)) {
        return DEBUG_FAIL("Expected <commercial-at>.");
      }
      c.advance();

      if (!c.is(eToken::kIdent)) {
        return DEBUG_FAIL("Expected <identifier>.");
      }
      std::size_t identifier_begin = c.index();
      c.advance();
      Ast identifier_node{eAst::kIdent, identifier_begin, c.index()};
      param_node.branches.push_back(std::move(identifier_node));

      if (c.is(eToken::kComma)) {
        param_node.setSourceEnd(c.index());
        c.advance();
        continue;
      } else if (c.is(eToken::kRParen)) {
        param_node.setSourceEnd(c.index());
        c.advance();
        ret_node.setSourceEnd(c.index());
        return LLParserResult(c.index(), ret_node);
      } else {
        return DEBUG_FAIL("Expected <comma> or <right-parenthesis>.");
      }
    } else {
      return DEBUG_FAIL("Expected <right-parenthesis>.");
    }
  }
  return DEBUG_FAIL("Expected <right-parenthesis>.");
}

constexpr LLPrsResT ParseMethodReturnParameters(TkCursorT c) noexcept {
  Ast method_return_type_node{eAst::kMethodReturnType, c.index(), c.index()};
  // Expect any number of modifiers.
  if (isTokenModifier(c.kind())) {
    auto method_mods_result = ParseModifiers(c);
    if (not method_mods_result) {
      return method_mods_result;
    }
    c.advanceTo(method_mods_result->head);
    method_return_type_node.branches.push_back(std::move(method_mods_result->ast));
  }

  // Expecting a primary expression ending in a colon.
  if (isTokenPrimary(c.kind())) {
    auto ret_type_result = ParsePrimaryExpr(c);
    if (not ret_type_result) {
      return ret_type_result;
    }
    c.advanceTo(ret_type_result->head);
    method_return_type_node.branches.push_back(std::move(ret_type_result->ast));
  } else
    return DEBUG_FAIL("Expected <primary-expr>.");

  return LLParserResult(c.index(), std::move(method_return_type_node));
}

constexpr LLPrsResT ParseMethodSignature(TkCursorT c) noexcept {
  using namespace detail;

  std::size_t signature_begin = c.index();
  // Colon after identifier -> Implicit void arg, no return.
  if (c.is(eToken::kColon) || c.is(eToken::kSemicolon)) {
    Ast ret_node = {eAst::kMethodSignature, c.index(), c.index()};
    ret_node.branches.push_back({eAst::kMethodParameterList, c.index(), c.index()});

    Ast& parm_list_node = ret_node.branches.back();
    parm_list_node.branches.push_back({eAst::kMethodParameter, c.index(), c.index()});

    Ast& param_node = parm_list_node.branches.back();
    param_node.branches.push_back({eAst::kModifiers, c.index(), c.index()});
    param_node.branches.push_back({eAst::kKwVoid, c.index(), c.index()});
    param_node.branches.push_back({eAst::kIdent, c.index(), c.index()});

    ret_node.branches.push_back({eAst::kMethodReturnType, c.index(), c.index()});

    Ast& return_type_node = ret_node.branches.back();
    return_type_node.branches.push_back({eAst::kModifiers, c.index(), c.index()});
    return_type_node.branches.push_back({eAst::kKwVoid, c.index(), c.index()});

    return LLParserResult(c.index(), ret_node);
  }
  // GreaterThan after identifier -> Some sort of void arg with a return.
  else if (c.is(eToken::kGt)) {
    c.advance();
    // Implicit any return void method.
    if (c.is(eToken::kColon) || c.is(eToken::kSemicolon)) {
      Ast ret_node = {eAst::kMethodSignature, c.index(), c.index()};
      ret_node.branches.push_back({eAst::kMethodParameterList, c.index(), c.index()});
      Ast& parm_list_node = ret_node.branches.back();

      parm_list_node.branches.push_back({eAst::kMethodParameter, c.index(), c.index()});
      Ast& param_node = parm_list_node.branches.back();
      param_node.branches.push_back({eAst::kModifiers, c.index(), c.index()});
      param_node.branches.push_back({eAst::kKwVoid, c.index(), c.index()});
      param_node.branches.push_back({eAst::kIdent, c.index(), c.index()});

      ret_node.branches.push_back({eAst::kMethodReturnType, c.index(), c.index()});
      Ast& return_type_node = ret_node.branches.back();
      return_type_node.branches.push_back({eAst::kModifiers, c.index(), c.index()});
      return_type_node.branches.push_back({eAst::kKwAny, c.index(), c.index()});

      return LLParserResult(c.index(), ret_node);
    }

    // Parse the return type.
    auto return_type_result = ParseMethodReturnParameters(c);
    if (!return_type_result) return return_type_result;

    // Return type must end in a colon. If it doesn't, error above.
    c.advanceTo(return_type_result.value().head);
    Ast ret_node = {eAst::kMethodSignature, signature_begin, c.index()};
    ret_node.branches.push_back({eAst::kMethodParameterList, signature_begin, signature_begin});
    Ast& parm_list_node = ret_node.branches.back();

    parm_list_node.branches.push_back({eAst::kMethodParameter, signature_begin, signature_begin});
    Ast& param_node = parm_list_node.branches.back();
    param_node.branches.push_back({eAst::kKwVoid, c.index(), c.index()});

    ret_node.branches.push_back(std::move(return_type_result->ast));

    return LLParserResult(c.index(), ret_node);
  }
  // Open Paren After Identifier -> Method with arguments.
  else if (c.is(eToken::kLParen)) {
    auto method_params_result = ParseMethodParameters(c);
    if (!method_params_result) return method_params_result;

    c.advanceTo(method_params_result.value().head);
    Ast ret_node = {
        eAst::kMethodSignature,
        signature_begin,
        c.index(),
        {{method_params_result->ast},
         {eAst::kMethodReturnType, c.index(), c.index(), {{eAst::kKwVoid, c.index(), c.index()}}}}};
    if (c.is(eToken::kSemicolon)) {
      return LLParserResult(c.index(), ret_node);
    }

    // Expecting a colon or a greater than.
    if (c.is(eToken::kColon)) {
      // c.advance();
      return LLParserResult(c.index(), ret_node);
    } else if (c.is(eToken::kGt)) {
      c.advance();
      // if the next token is a colon, then the return type is any.
      if (c.is(eToken::kColon) || c.is(eToken::kSemicolon)) {
        ret_node = {eAst::kMethodSignature,
                    signature_begin,
                    c.index(),
                    {{method_params_result->ast},
                     {eAst::kMethodReturnType,
                      c.index(),
                      c.index(),
                      {{eAst::kKwAny, c.index(), c.index()}}}}};
        return LLParserResult(c.index(), ret_node);
      }

      auto return_type_result = ParseMethodReturnParameters(c);
      if (!return_type_result) return return_type_result;

      c.advanceTo(return_type_result->head);
      ret_node = {eAst::kMethodSignature,
                  signature_begin,
                  c.index(),
                  {{method_params_result->ast}, {return_type_result->ast}}};
      return LLParserResult(c.index(), ret_node);
    } else {
      return DEBUG_FAIL("Expected <:>.");
    }
  } else {
    return DEBUG_FAIL("Expected <:> or <>> or <(>.");
  }
}

constexpr LLPrsResT ParseMethodDef(TkCursorT c) noexcept {
  Ast node{eAst::kMethodDefinition, c.index(), c.index()};
  if (!c.is(eToken::kLBrace)) return DEBUG_FAIL("Expected opening brace '{'.");
  c.advance();

  while (!c.is(eToken::kRBrace)) {
    if (isTokenPragmatic(c.kind())) {
      auto decl = ParseFunctionalStmt(c);
      if (!decl) return decl;
      detail::AppendAndAdvance(c, node, decl);
    } else if (isTokenPrimary(c.kind())) {
      auto expr = ParsePrimaryStatement(c);
      if (!expr) return expr;
      detail::AppendAndAdvance(c, node, expr);
    } else {
      return DEBUG_FAIL("Expected <pragmatic-decl>.");
    }
  }
  c.advance();

  return LLParserResult(c.index(), std::move(node));
}

constexpr LLPrsResT ParseMainDef(TkCursorT c) noexcept {
  Ast node{eAst::kMethodDefinition, c.index(), c.index()};
  if (!c.is(eToken::kLBrace)) return SSGC_PARSER_EXPECTED_TOKEN(eToken::kLBrace);
  c.advance();

  while (!c.is(eToken::kRBrace)) {
    if (isTokenPragmatic(c.kind())) {
      auto decl = ParseFunctionalStmt(c);
      if (!decl) return decl;
      detail::AppendAndAdvance(c, node, decl);
    } else if (isTokenPrimary(c.kind())) {
      auto expr = ParsePrimaryStatement(c);
      if (!expr) return expr;
      detail::AppendAndAdvance(c, node, expr);
    } else {
      return SSGC_PARSER_EXPECTED_TOKEN("Expected pragmatic declaration.");
    }
  }
  c.advance();

  return LLParserResult(c.index(), std::move(node));
}

constexpr LLPrsResT ParseClassDef(TkCursorT c) noexcept {
  Ast node{eAst::kClassDefinition, c.index(), c.index()};
  if (!c.is(eToken::kLBrace)) {
    return DEBUG_FAIL("Expected opening brace.");
  }
  c.advance();

  while (!c.is(eToken::kRBrace)) {
    if (isTokenPragmatic(c.kind())) {
      auto decl = ParsePragmaticStmt(c);
      if (!decl) return decl;
      detail::AppendAndAdvance(c, node, decl);
    } else {
      return DEBUG_FAIL("Expected pragmatic declaration.");
    }
  }
  c.advance();

  if (!c.is(eToken::kSemicolon)) {
    return DEBUG_FAIL("Expected semicolon.");
  }
  c.advance();
  return LLParserResult(c.index(), std::move(node));
}

constexpr LLPrsResT ParseLibDef(TkCursorT c) noexcept {
  Ast node{eAst::kLibraryDefinition, c.index(), c.index()};
  if (!c.is(eToken::kLBrace)) {
    return DEBUG_FAIL("Expected opening brace.");
  }
  c.advance();

  while (!c.is(eToken::kRBrace)) {
    if (isTokenPragmatic(c.kind())) {
      auto decl = ParsePragmaticStmt(c);
      if (!decl) return decl;
      detail::AppendAndAdvance(c, node, decl);
    } else {
      return DEBUG_FAIL("Expected pragmatic declaration.");
    }
  }
  c.advance();

  if (!c.is(eToken::kSemicolon)) {
    return DEBUG_FAIL("Expected semicolon.");
  }
  c.advance();
  return LLParserResult(c.index(), std::move(node));
}

constexpr LLPrsResT ParsePragmaticStmt(TkCursorT c) noexcept {
  using enum eToken;

  // Format: <modifiers?><decl keyword>
  if (c.is(eToken::kSemicolon)) return DEBUG_FAIL("Empty statement in code.");

  // Check for keywords which don't use modifiers.
  if (isTokenDeclarative(c.kind())) {
    switch (c.kind()) {
      case kKwUse:
        return ParseUsingDecl(c);
      case kKwMain:
        return ParseMainDecl(c);
      case kKwImport:
        return ParseImportDecl(c);
      case kKwEnum:
        return ParseEnumDecl(c);
      case kKwReturn:
        return ParseReturnStmt(c);
      default:
        break;
    }
  }

  TkCursorT decl_begin = c;  // Store the initial modifier location.
  while (isTokenModifier(c.kind())) c.advance();

  // Check for all keywords which are valid pragmatic declarations.
  if (isTokenDeclarative(c.kind())) {
    switch (c.kind()) {
      case kKwDef:
        return ParseVariableDecl(decl_begin);
      case kKwFn:
        return ParseMethodDecl(decl_begin);
      case kKwClass:
        return ParseClassDecl(decl_begin);
      case kKwLib:
        return ParseLibDecl(decl_begin);
      case kKwUse:
      case kKwMain:
        return DEBUG_FAIL("Declaration keyword cannot be modified.");
      default:
        return DEBUG_FAIL("Declaration keyword not permitted inside pragmatic code block.");
    }
  } else
    return DEBUG_FAIL("Expected <pragmatic-decl-keyword>.");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

constexpr LLPrsResT ParseIncludeStmt(TkCursorT c) noexcept {
  auto stmt_begin = c.index();
  // Format: <import><string-literal><semicolon>
  if (!c.is(eToken::kKwInclude)) return DEBUG_FAIL("Expected <keyword-include>.");
  c.advance();

  // String literal -> local include.
  if (c.is(eToken::kLitCstr)) {
    Ast ident_node = c;
    ident_node.type = eAst::kIQCharSeuquence;
    c.advance();
    if (!c.is(eToken::kSemicolon)) return DEBUG_FAIL("Expected <semicolon>.");
    c.advance();
    return LLParserResult(
        c.index(), Ast(eAst::kIncludeLocalStmt, stmt_begin, c.index(), nullptr, {ident_node}));
  }
  // Less than -> system include.
  else if (c.is(eToken::kLt)) {
    auto ia_char_sequence_begin = c.index();
    c.advance();
    while (!c.is(eToken::kGt)) {
      c.advance();
    }

    if (!c.is(eToken::kGt)) return DEBUG_FAIL("Unclosed system inclusion.");
    c.advance();

    Ast ia_char_sequence = Ast(eAst::kIACharSeuquence, ia_char_sequence_begin, c.index());

    if (!c.is(eToken::kSemicolon)) return DEBUG_FAIL("Expected <semicolon>.");
    c.advance();
    return LLParserResult(c.index(), Ast(eAst::kIncludeSystemStmt, stmt_begin, c.index(), nullptr,
                                         {ia_char_sequence}));
  }

  return DEBUG_FAIL("Expected <quoted-include-char-sequence> or <angled-include-char-sequence>.");
}
constexpr LLPrsResT ParsePragmaStmt(TkCursorT c) noexcept { return DEBUG_FAIL("Not implemented"); }
constexpr LLPrsResT ParseSwitchStmt(TkCursorT c) noexcept { return DEBUG_FAIL("Not implemented"); }

// <directive_desc> ::= <directive_stmt>* | <directive_block>
// <directive_block> ::= <LEFT_CURLY_BRACKET> <IGNORED> <directive_stmt_list> <IGNORED>
// <RIGHT_CURLY_BRACKET> <directive_stmt_list> ::= <directive_stmt>* <directive_stmt> ::=
// <primary_stmt>
//				   | <include>
//				   | <pragma>
//				   | <process>
//				   | <library>
//				   | <variable>
//				   | <function>
//				   | <struct>
//				   | <using>
//				   | <enum>
//				   | <directive_namespace>
//				   | <directive_if>
//				   | <directive_switch>
//				   | <directive_while>
//				   | <directive_for>
//				   | <directive_return>
constexpr LLPrsResT ParsePragmaticDesc(TkCursorT c) noexcept {
  // Error on empty statement early
  if (c.is(eToken::kSemicolon))
    return DEBUG_FAIL("Empty statement in code. Unnecessary semicolon ';'.");

  // Check if there is an unnamed scope. Override possible left brace in primary expressions,
  // and recursivley call this method to parse contained statements.
  if (c.is(eToken::kLBrace)) {
    Ast scope_node{eAst::kUnnamedScope, c.index(), c.index()};
    while (!c.is(eToken::kRBrace) || c.atEnd()) {
      auto sub_description = ParsePragmaticDesc(c);
      if (!sub_description) return sub_description;
      detail::AppendAndAdvance(c, scope_node, sub_description);
    }
    if (c.atEnd()) return DEBUG_FAIL("Unclosed unnamed scope. Closing brace '}' not found.");
    c.advance();
    if (c.is(eToken::kSemicolon))
      return DEBUG_FAIL("Empty statement in code. Unnecessary semicolon ';'.");
    return LLParserResult{c.index(), scope_node};
  }

  /////////////////////////////////////////////////////////
  // From here we are parsing a single statement...
  /////////////////////////////////////////////////////////
  if (isTokenPrimary(c.kind())) {
    return ParsePrimaryStatement(c);
  }

  // Parse a valid directive keyword statement.
  // Check for keywords which don't allow any modifiers.
  switch (c.kind()) {
    // case eToken::kKwInclude:
    //   return ParseIncludeStmt(c);
    case eToken::kKwUse:
      return ParseUsingDecl(c);
    case eToken::kKwMain:
      return ParseMainDecl(c);
    case eToken::kKwImport:
      return ParseImportDecl(c);
    case eToken::kKwEnum:
      return ParseEnumDecl(c);
    default:
      break;
  }

  // Store the initial statement begin location.
  // Skip any modifiers to determine the parsing method to call based on declarative keyword.
  // Parse from decl begin to allow called method to handle modifiers.
  TkCursorT decl_begin = c;
  while (isTokenModifier(c.kind())) c.advance();
  switch (c.kind()) {
    case eToken::kKwDef:
      return ParseVariableDecl(decl_begin);
    case eToken::kKwFn:
      return ParseMethodDecl(decl_begin);
    case eToken::kKwClass:
      return ParseClassDecl(decl_begin);
    case eToken::kKwUse:
    case eToken::kKwMain:
    case eToken::kKwImport:
    case eToken::kKwEnum:
    case eToken::kKwReturn:
      return DEBUG_FAIL("Declaration keyword cannot be modified.");
    default:
      return DEBUG_FAIL("Declaration keyword not permitted inside pragmatic code block.");
  }
}

// <process_def> ::= <KW_PROC> <IGNORED> <AT_SIGN> <IDENTIFIER> <IGNORED> <COLON> <IGNORED>
//									<LEFT_CURLY_BRACKET>
//<IGNORED> <process_desc> <IGNORED> <RIGHT_CURLY_BRACKET>
// <process_unnamed_def> ::= <KW_PROC> <IGNORED> <COLON> <IGNORED>
//									<LEFT_CURLY_BRACKET>
//<IGNORED> <process_desc> <IGNORED> <RIGHT_CURLY_BRACKET>
// <process_desc> ::= (<process_stmt> | <process_block>)*
// <process_block> ::= <LEFT_CURLY_BRACKET> <IGNORED> <process_stmt>* <IGNORED>
// <RIGHT_CURLY_BRACKET> <process_stmt> ::= <import> | <main> | <pragma> | <primary> | <variable> |
// <function> | <struct>
//				        | <namespace> | <using> | <enum>
constexpr LLPrsResT ParseProcDef(TkCursorT c) noexcept {
  using enum eToken;
  Ast process_node{eAst::kProcessDefinition, c.index(), c.index()};

  // Process def will either be a single statement or a block.
  if (c.is(eToken::kSemicolon)) return DEBUG_FAIL("Empty process definition.");

  // Process block
  if (c.is(eToken::kLBrace)) {
    c.advance();

    if (c.is(eToken::kRBrace))  // Empty Proc def
    {
      c.advance();
      return LLParserResult{c.index(), process_node};
    } else {
      while (!c.atEnd() && !c.is(kRBrace)) {
        if (isTokenPragmatic(c.kind())) {
          auto pragmatic_desc = ParsePragmaticDesc(c);
          if (!pragmatic_desc) return pragmatic_desc;
          detail::AppendAndAdvance(c, process_node, pragmatic_desc);
        } else
          return DEBUG_FAIL("Expected a pragmatic description at the process level.");
      }
    }
    // eof before closing brace
    if (c.atEnd()) return DEBUG_FAIL("Expected closing brace.");
    c.advance();
    return LLParserResult{c.index(), process_node};
  }

  // Single statement process
  if (isTokenPragmatic(c.kind())) {
    auto pragmatic_desc = ParsePragmaticDesc(c);
    if (!pragmatic_desc) return pragmatic_desc;
    detail::AppendAndAdvance(c, process_node, pragmatic_desc);
  } else
    return DEBUG_FAIL("Expected a pragmatic description at the process level.");

  return LLParserResult(c.index(), process_node);
}

// <process> ::= <process_decl> | <process_def>
// <process_decl> ::= <KW_PROC> <IGNORED> <AT_SIGN> <IDENTIFIER> <IGNORED> <SEMICOLON>
constexpr LLPrsResT ParseProcDecl(TkCursorT c) noexcept {
  using enum eToken;
  // Check if there are any modifiers.
  auto decl_begin = c.index();
  LLPrsResT mods = detail::ParseOptionalModifiers(c);
  if (!mods) return mods;
  Ast mod_node = mods.value().ast;

  if (!c.is(kKwProc)) return DEBUG_FAIL("Expected <kw-proc>.");
  c.advance();

  // If there is a colon, this is an unnamed process. Unnamed processes must be defined inline.
  // Expect a definition ending in a semicolon.
  if (c.is(eToken::kColon)) {
    c.advance();
    auto def_result = ParseProcDef(c);
    if (!def_result) return def_result;
    c.advanceTo(def_result->head);
    return LLParserResult(c.index(), Ast(eAst::kProcessDeclaration, decl_begin, c.index(),
                                         {mod_node, def_result->ast}));
  }

  // If there is a @ following the proc keyword, this is a named process.
  if (!c.is(kCommercialAt)) return DEBUG_FAIL("Expected '@' symbol after 'proc' keyword.");
  c.advance();
  if (!c.is(kIdent)) return DEBUG_FAIL("Expected identifier after '@' symbol.");
  Ast ident_node{c};
  c.advance();

  // Process declaration
  if (c.is(eToken::kSemicolon)) {
    c.advance();
    return LLParserResult(
        c.index(), Ast(eAst::kProcessDeclaration, decl_begin, c.index(), {mod_node, ident_node}));
  }

  if (!c.is(kColon)) return DEBUG_FAIL("Expected ':' symbol followed by process definition.");
  c.advance();

  auto def_result = ParseProcDef(c);
  if (!def_result) return def_result;
  c.advanceTo(def_result->head);
  return LLParserResult(c.index(), Ast(eAst::kProcessDeclaration, decl_begin, c.index(),
                                       {mod_node, ident_node, def_result->ast}));
}

// <using> ::= <using_type_alias>
//		  | <using_library_alias>
//		  | <using_namespace_alias>
//		  | <using_function_alias>
//		  | <using_variable_alias>
//		  | <using_process_alias>
//		  | <using_enum_alias>
//		  | <using_library_unscope>
//		  | <using_namespace_unscope>
//		  | <using_function_unscope>
//		  | <using_def_unscope>
//		  | <using_proc_unscope>
//		  | <using_type_unscope>
//		  | <using_enum_unscope>
//
// <using_type_alias> ::= <KW_USE> <AT_SIGN> <IDENTIFIER> <EQUALS_SIGN>  <type_expr>  <SEMICOLON>
// <using_library_alias> ::= <KW_USE> <KW_LIB> <AT_SIGN> <IDENTIFIER> <EQUALS_SIGN> <id_expr>
// <SEMICOLON> <using_namespace_alias> ::= <KW_USE> <KW_LIB> <AT_SIGN> <IDENTIFIER> <EQUALS_SIGN>
// <id_expr> <SEMICOLON> <using_function_alias> ::= <KW_USE> <KW_LIB> <AT_SIGN> <IDENTIFIER>
// <EQUALS_SIGN> <id_expr> <SEMICOLON> <using_variable_alias> ::= <KW_USE> <KW_LIB> <AT_SIGN>
// <IDENTIFIER> <EQUALS_SIGN> <id_expr> <SEMICOLON> <using_process_alias> ::= <KW_USE> <KW_LIB>
// <AT_SIGN> <IDENTIFIER> <EQUALS_SIGN> <id_expr> <SEMICOLON> <using_enum_alias> ::= <KW_USE>
// <KW_LIB> <AT_SIGN> <IDENTIFIER> <EQUALS_SIGN> <id_expr> <SEMICOLON>
//
// <using_library_unscope> ::= <KW_USE> <KW_LIB> <id_expr> <SEMICOLON>
// <using_namespace_unscope> ::= <KW_USE> <KW_NAMESPACE> <id_expr> <SEMICOLON>
// <using_function_unscope> ::= <KW_USE> <KW_FN> <id_expr> <SEMICOLON>
// <using_def_unscope> ::= <KW_USE> <KW_DEF> <id_expr> <SEMICOLON>
// <using_proc_unscope> ::= <KW_USE> <KW_PROC> <id_expr> <SEMICOLON>
// <using_type_unscope> ::= <KW_USE> <id_expr> <SEMICOLON>
// <using_enum_unscope> ::= <KW_USE> <KW_ENUM> <id_expr> <SEMICOLON>
constexpr LLPrsResT ParseUsingDecl(TkCursorT c) noexcept {
  if (!c.is(eToken::kKwUse)) return DEBUG_FAIL("Expected 'use' keyword.");
  auto stmt_begin = c.index();
  c.advance();

  // - @foo -> Type Alias
  // - @foo : lib -> Library Type Alias.
  if (c.is(eToken::kCommercialAt)) {
    c.advance();
    if (!c.is(eToken::kIdent)) return DEBUG_FAIL("Expected <ident>.");
    Ast alias = Ast(c);
    c.advance();
    if (!c.is(eToken::kColon)) return DEBUG_FAIL("Expected <:>.");
    c.advance();

    // If next is lib, then it is a library type alias.
    // Everything following lib must be a value expression closed by a
    // semicolon. Wether it is reduced to a type is determined at a later
    // stage.
    if (c.is(eToken::kKwLib)) {
      c.advance();
      auto val_expr = ParsePrimaryStatement(c);
      if (!val_expr) return val_expr;
      c.advanceTo(val_expr.value().head);
      return LLParserResult(
          c.index(),
          {eAst::kLibraryTypeAlias, stmt_begin, c.index(), nullptr, {alias, val_expr->ast}});
    }

    // else it is a type alias...
    auto val_expr = ParsePrimaryStatement(c);
    if (!val_expr) return val_expr;
    c.advanceTo(val_expr.value().head);
    return LLParserResult(
        c.index(), {eAst::kTypeAlias, stmt_begin, c.index(), nullptr, {alias, val_expr->ast}});
  }

  // - lib-> Library Namespace Inclusion.
  else if (c.is(eToken::kKwLib)) {
    c.advance();
    auto val_expr = ParsePrimaryStatement(c);
    if (!val_expr) return val_expr;
    c.advanceTo(val_expr.value().head);
    return LLParserResult(
        c.index(),
        {eAst::kLibraryNamespaceInclusion, stmt_begin, c.index(), nullptr, {val_expr.value().ast}});

  }

  // - namespace-> Namespace Inclusion.
  else if (c.is(eToken::kKwNamespace)) {
    c.advance();
    auto val_expr = ParsePrimaryStatement(c);
    if (!val_expr) return val_expr;
    c.advanceTo(val_expr->head);
    return LLParserResult(
        c.index(), Ast(eAst::kNamespaceInclusion, stmt_begin, c.index(), nullptr, {val_expr->ast}));
  }

  // Namespace object inclusion.
  // The primary expr assumed to resolve to an object or function name. Validate at a later stage.
  else if (isTokenPrimary(c.kind())) {
    auto primary_result = ParsePrimaryStatement(c);
    if (!primary_result) return primary_result;
    c.advanceTo(primary_result->head);
    return LLParserResult(c.index(), Ast(eAst::kNamespaceObjectInclusion, stmt_begin, c.index(),
                                         nullptr, {primary_result->ast}));

  }

  // Invalid format.
  else
    return DEBUG_FAIL(
        "Using statement expected <@> or <kw-lib> or <kw-namespace> or <type-expression>.");
};

// <directive_desc> ::= <directive_stmt>* | <directive_block>
// <directive_block> ::= <LEFT_CURLY_BRACKET> <IGNORED> <directive_stmt_list> <IGNORED>
// <RIGHT_CURLY_BRACKET> <directive_stmt_list> ::= <directive_stmt>* <directive_stmt> ::=
// <primary_stmt>
//				   | <include>
//				   | <pragma>
//				   | <process>
//				   | <library>
//				   | <variable>
//				   | <function>
//				   | <struct>
//				   | <using>
//				   | <enum>
//				   | <directive_namespace>
//				   | <directive_if>
//				   | <directive_switch>
//				   | <directive_while>
//				   | <directive_for>
//				   | <directive_return>
constexpr LLPrsResT ParseDirectiveDesc(TkCursorT c) noexcept {
  using enum eToken;
  // Error on empty statement early
  if (c.is(eToken::kSemicolon))
    return DEBUG_FAIL("Empty statement in code. Unnecessary semicolon ';'.");

  // Check if there is an unnamed scope. Override possible left brace in primary expressions,
  // and recursivley call this method to parse contained statements.
  if (c.is(eToken::kLBrace)) {
    Ast scope_node{eAst::kUnnamedScope, c.index(), c.index()};
    while (!c.is(eToken::kRBrace) || c.atEnd()) {
      auto sub_description = ParseDirectiveDesc(c);
      if (!sub_description) return sub_description;
      detail::AppendAndAdvance(c, scope_node, sub_description);
    }
    if (c.atEnd()) return DEBUG_FAIL("Unclosed unnamed scope. Closing brace '}' not found.");
    c.advance();
    if (c.is(eToken::kSemicolon))
      return DEBUG_FAIL("Empty statement in code. Unnecessary semicolon ';'.");
    return LLParserResult{c.index(), scope_node};
  }

  /////////////////////////////////////////////////////////
  // From here we are parsing a single statement...
  /////////////////////////////////////////////////////////
  if (isTokenPrimary(c.kind())) {
    return ParsePrimaryStatement(c);
  }

  // Parse a valid directive keyword statement.
  // Check for keywords which don't allow any modifiers.
  switch (c.kind()) {
    case kKwUse:
      return ParseUsingDecl(c);
    case kKwMain:
      return ParseMainDecl(c);
    case kKwImport:
      return ParseImportDecl(c);
    case kKwEnum:
      return ParseEnumDecl(c);
    case kKwReturn:
      return ParseReturnStmt(c);
    case kKwNamespace:
      return ParseNamespaceDecl(c);
    default:
      break;
  }

  // Store the initial statement begin location.
  // Skip any modifiers to determine the parsing method to call based on declarative keyword.
  // Parse from decl begin to allow called method to handle modifiers.
  TkCursorT decl_begin = c;
  while (isTokenModifier(c.kind())) c.advance();
  switch (c.kind()) {
    case kKwDef:
      return ParseVariableDecl(decl_begin);
    case kKwFn:
      return ParseMethodDecl(decl_begin);
    case kKwClass:
      return ParseClassDecl(decl_begin);
    case kKwLib:
      return ParseLibDecl(decl_begin);
    case kKwProc:
      return ParseProcDecl(decl_begin);
    case kKwUse:
    case kKwMain:
    case kKwImport:
    case kKwEnum:
    case kKwReturn:
      return DEBUG_FAIL("Declaration keyword cannot be modified.");
    default:
      return DEBUG_FAIL("Declaration keyword not permitted inside directive code block.");
  }
}

constexpr LLPrsResT ParseNamespaceDecl(TkCursorT c) noexcept {
  if (!c.is(eToken::kKwNamespace)) return DEBUG_FAIL("Expected 'namespace' keyword.");

  c.advance();
  if (!c.is(eToken::kCommercialAt))
    return DEBUG_FAIL("Expected '@' symbol after 'namespace' keyword.");
  c.advance();
  if (!c.is(eToken::kIdent)) return DEBUG_FAIL("Expected <identifier>.");
  Ast node{eAst::kNamespaceDecl, c.index(), c.index()};
  node.branches.push_back(Ast(c));

  c.advance();
  if (c.is(eToken::kSemicolon)) return LLParserResult(c.advance().index(), node);
  if (!c.is(eToken::kLBrace)) return DEBUG_FAIL("Expected <{>.");

  while (!c.atEnd() || !c.is(eToken::kRBrace)) {
    if (isTokenDirectiveSpecifier(c.kind())) {
      auto directive_desc = ParseDirectiveDesc(c);
      if (!directive_desc) return directive_desc;
      c.advanceTo(directive_desc.value().head);
      node.branches.push_back(std::move(directive_desc.value().ast));
    } else
      return DEBUG_FAIL("Expected a directive description at the top level.");
  }

  if (c.atEnd()) return DEBUG_FAIL("Expected closing brace '}' for namespace declaration.");
  c.advance();

  return LLParserResult(c.index(), node);
}

// <syntax> ::= <directive_desc>
constexpr LLPrsResT ParseSyntax(TkCursorT c) noexcept {
  using namespace detail;
  Ast program_node{eAst::kSourceFile, c.index(), c.index()};
  while (!c.atEnd()) {
    if (isTokenDirectiveSpecifier(c.kind())) {
      auto directive_desc = ParseDirectiveDesc(c);
      if (!directive_desc) return directive_desc;
      c.advanceTo(directive_desc.value().head);
      program_node.branches.push_back(std::move(directive_desc.value().ast));
    } else
      return DEBUG_FAIL("Expected a directive description at the top level.");
  }
  return LLParserResult(c.index(), program_node);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

constexpr LLPrsResT ParseEnumDecl(TkCursorT c) noexcept {
  using enum eToken;
  using namespace detail;
  // Format:
  // <modifiers?> <enum> <commercial_at?><name?><colon?>
  // <<association_typename><<commercial_at><association_identifier>?><semicolon>?>
  // <<commercial_at><enum_entry_name?><semicolon>
  auto decl_begin = c.index();
  LLPrsResT mods = ParseOptionalModifiers(c);
  if (!mods) return mods;
  Ast mod_node = mods.value().ast;

  if (!c.is(kKwEnum)) return DEBUG_FAIL("Expected <kw-enum>.");

  c.advance();
  if (!c.is(kCommercialAt)) return DEBUG_FAIL("Expected <identifier>.");
  c.advance();
  if (!c.is(kIdent)) return DEBUG_FAIL("Expected <identifier>.");
  Ast ident_node{c};
  c.advance();

  // If there is a colon, this is a Definition.
  // If there is a semicolon, this is a Declaration.
  if (c.is(eToken::kColon)) {
    c.advance();
    auto def = ParseEnumDef(c);
    if (!def) return def;
    Ast def_node = detail::ExtractAndAdvance(c, def);
    c.advanceTo(def.value().head);
    return LLParserResult(c.index(), Ast(eAst::kEnumDeclaration, decl_begin, c.index(),
                                         {mod_node, ident_node, def_node}));
  } else if (c.is(eToken::kSemicolon)) {
    c.advance();
    return LLParserResult(
        c.index(), Ast(eAst::kEnumDeclaration, decl_begin, c.index(), {mod_node, ident_node}));
  } else {
    return DEBUG_FAIL("Expected <;> or <:>.");
  }
};

constexpr LLPrsResT ParseEnumDef(TkCursorT c) noexcept {
  // If followed by an open brace, positional_enum.
  // If followed by anything else,associative_enum.
  // associative_enum -> <type_expr><commercial_at><enum_entry_name?><colon>
  // After each named association, may be an open brace or another association.

  using enum eToken;
  using namespace detail;
  Ast node{eAst::kEnumDefinition, c.index(), c.index()};

  // (<type_expr> <commercial_at> <enum_entry_name?> <colon>) | <semicolon>
  bool is_unnamed_associative_enum = false;
  bool is_associative_enum = false;
  while (!c.is(kLBrace)) {
    Ast assoc_node{eAst::kEnumAssociation, c.index(), c.index()};
    // <type_expr>
    if (isTokenPrimary(c.kind())) {
      LLPrsResT expr_result = ParsePrimaryExpr(c);
      if (!expr_result) return LLPrsResT::unexpected_type(expr_result.error());
      detail::AppendAndAdvance(c, assoc_node, expr_result);
    } else {
      assoc_node.branches.push_back(Ast(eAst::kKwAny, c.index(), c.index()));
    }

    // <commercial_at> <identifier>?
    if (c.is(eToken::kCommercialAt) && !is_unnamed_associative_enum) {
      c.advance();
      auto ident_node = ParseIdentityOperand(c);
      if (!ident_node) return LLPrsResT::unexpected_type(ident_node.error());
      detail::AppendAndAdvance(c, assoc_node, ident_node);
      if (is_unnamed_associative_enum)
        return DEBUG_FAIL("Cannot mix named and unnamed enum associations.");
      is_associative_enum = true;
    } else {
      if (is_associative_enum) return DEBUG_FAIL("Cannot mix named and unnamed enum associations.");
      is_unnamed_associative_enum = true;
    }

    // <colon> | <semicolon>
    node.branches.push_back(assoc_node);
    if (c.is(eToken::kColon)) {
      // Continue to next association or start of enum block.
      c.advance();
      continue;
    } else if (c.is(eToken::kSemicolon)) {
      // No definition provided.
      return LLParserResult(c.advance().index(), node);
    } else {
      return DEBUG_FAIL("Expected <;> or <:>.");
    }
  }

  // Parse enum definition block.
  auto block = ParseEnumBlock(c);
  if (!block) return block;
  detail::AppendAndAdvance(c, node, block);

  return LLParserResult(c.index(), std::move(node));
};

constexpr LLPrsResT ParseEnumBlock(TkCursorT c) noexcept {
  // Parse inside an enum definition block.
  // Cursor should begin at the open brace of the block.
  using enum eToken;
  using namespace detail;

  Ast node{eAst::kEnumBlock, c.index(), c.index()};
  if (!c.is(eToken::kLBrace))  // Require opening brace.
    return DEBUG_FAIL("Expected <{>.");
  c.advance();

  while (!c.is(eToken::kRBrace)) {
    // EnumEntry
    if (c.is(eToken::kCommercialAt)) {
      c.advance();
      if (!c.is(eToken::kIdent)) return DEBUG_FAIL("Expected <identifier>.");
      node.branches.push_back(Ast(eAst::kEnumEntry, c.index(), c.next().index()));
      auto& this_entry = node.branches.back();
      c.advance();
      // Check for association initializers. Expect a colon or semicolon.
      while (!c.is(eToken::kSemicolon)) {
        if (!c.is(eToken::kColon)) return DEBUG_FAIL("Expected <:>.");
        c.advance();

        if (!isTokenPrimary(c.kind())) return DEBUG_FAIL("Expected <primary-expr>.");
        LLPrsResT expr_result = ParsePrimaryExpr(c);
        if (!expr_result) return LLPrsResT::unexpected_type(expr_result.error());
        detail::AppendAndAdvance(c, this_entry, expr_result);
      }
      c.advance();
      // xParseAssociationInitializers(c, this_entry);
    }
    // EnumCategory
    else if (c.is(eToken::kKwNamespace)) {
      c.advance();
      std::vector<Ast> this_category{};
      // Consume identifiers until a semicolon is reached.
      while (!c.is(eToken::kColon)) {
        if (!c.is(eToken::kCommercialAt)) return DEBUG_FAIL("Expected <@>.");
        c.advance();
        if (!c.is(eToken::kIdent)) return DEBUG_FAIL("Expected <identifier>.");
        this_category.push_back(Ast(getAstFromToken(c.kind()), c.index(), c.next().index()));
        c.advance();
      }
      c.advance();  // pass colon

      // expect the enum entry for a singular entry or a block for multi entry.
      if (c.is(eToken::kCommercialAt)) {
        c.advance();
        if (!c.is(eToken::kIdent)) return DEBUG_FAIL("Expected <identifier>.");
        node.branches.push_back(Ast(eAst::kEnumEntry, c.index(), c.next().index()));
        auto& this_entry = node.branches.back();
        c.advance();

        // Attribute this category to this entry.
        this_entry.branches.push_back(Ast{eAst::kEnumCategory, c.index(), c.index()});
        auto& added_category = this_entry.branches.back();
        for (auto& tk_iter : this_category) added_category.branches.push_back(tk_iter);

        // Check for association initializers. Expect a colon or semicolon.
        while (!c.is(eToken::kSemicolon)) {
          if (!c.is(eToken::kColon)) return DEBUG_FAIL("Expected <:>.");
          c.advance();

          if (!isTokenPrimary(c.kind())) return DEBUG_FAIL("Expected <primary-expr>.");
          LLPrsResT expr_result = ParsePrimaryExpr(c);
          if (!expr_result) return LLPrsResT::unexpected_type(expr_result.error());
          detail::AppendAndAdvance(c, this_entry, expr_result);
        }
        c.advance();
      }
      // a block
      else if (c.is(eToken::kLBrace)) {
        auto recursed_block_res = ParseEnumBlock(c);
        if (!recursed_block_res) return recursed_block_res;

        // Add this category to all categories of the sub-block
        // as a parent (push to the front), then add the entry
        // to this block.
        auto& recursed_block = recursed_block_res.value();
        for (auto& entry : recursed_block.ast.branches) {
          for (auto& entry_data : entry.branches) {
            if (entry_data.type == eAst::kEnumCategory) {
              for (auto& name_frag_it : this_category)
                entry_data.branches.insert(entry_data.branches.begin(), {name_frag_it});
            }
          }
          node.branches.push_back(entry);
        }
      } else
        return DEBUG_FAIL("Expected <identifier>.");
    }
    // Invalid enum description format.
    else
      return DEBUG_FAIL("Expected <identifier>.");
  }

  return LLParserResult(c.index(), node);
};

// constexpr ScopePrsResT FindScopeImpl(TkCursorT& crsr) noexcept {
//   auto opening = crsr.Iter();
//   int scope_depth = 0;
//   std::vector<eToken> scope_history;
//   // Find the last matching close token that is not within a () [] or {} scope.
//   // Start from the token after the open token. All scope depths should be 0
//   // at the end of the loop on a valid scope.
//   for (auto c = crsr.Next(); c.index() != crsr.End(); c.advance()) {
//     if (c.IsOpeningScope()) {
//       scope_depth++;
//       scope_history.push_back(c.kind());
//     } else if (c.IsClosingScope()) {
//       if (scope_history.empty())
//         if (c.IsClosingScopeOf(crsr.Type()))
//           return TkScopeT{true, opening, c.advance().Iter()};
//         else
//           return DEBUG_FAIL("Unclosed scope.");
//
//       if (!crsr.IsClosingScopeOf(scope_history.back())) return DEBUG_FAIL("Mismatched scopes.");
//
//       scope_history.pop_back();
//       scope_depth--;
//     } else
//       continue;
//   }
//   return DEBUG_FAIL("Unclosed scope.");
// }
//
// constexpr ScopePrsResT FindParen(TkCursorT c) noexcept {
//   if (c.atEnd()) return DEBUG_FAIL("OutOfBounds");  // Out of bounds begin passed to method...
//   if (!c.is(eToken::kLParen))
//     return DEBUG_FAIL("Expected opening scope.");  // No open token to start with.
//   auto scope_begin = c;
//   c.advance();
//   if (c.atEnd())
//     return DEBUG_FAIL("kParserOpeningScopeAtEof.");  // End right after open, cannot be closed.
//   if (c.is(eToken::kRParen))
//     return TkScopeT{true, scope_begin.Iter(), c.advance().Iter()};  // Empty paren scope '()'
//   return FindScopeImpl(scope_begin);
// }  // end find_paren
//
// constexpr ScopePrsResT FindBrace(TkCursorT c) noexcept {
//   if (c.atEnd()) return DEBUG_FAIL("OutOfBounds");  // Out of bounds begin passed to method...
//   if (!c.is(eToken::kLBrace))
//     return DEBUG_FAIL("Expected opening scope.");  // No open token to start with.
//   auto scope_begin = c.index();
//   // c.advance();
//   if (c.atEnd())
//     return DEBUG_FAIL("kParserOpeningScopeAtEof.");  // End right after open, cannot be closed.
//   if (c.is(eToken::kRBrace))
//     return TkScopeT{true, scope_begin, c.advance().Iter()};  // Empty paren scope '()'
//   return FindScopeImpl(c);
// }  // end find_paren
//
// constexpr ScopePrsResT FindBracket(TkCursorT c) noexcept {
//   if (c.atEnd()) return DEBUG_FAIL("OutOfBounds");  // Out of bounds begin passed to method...
//   if (!c.is(eToken::kLBracket))
//     return DEBUG_FAIL("Expected opening scope.");  // No open token to start with.
//   auto scope_begin = c.index();
//   c.advance();
//   if (c.atEnd())
//     return DEBUG_FAIL("kParserOpeningScopeAtEof.");  // End right after open, cannot be closed.
//   if (c.is(eToken::kRBracket))
//     return TkScopeT{true, scope_begin, c.advance().Iter()};  // Empty paren scope '()'
//   return FindScopeImpl(c);
// }  // end find_paren

constexpr LRPrsResT ParseExpr(TkCursorT c) noexcept { return DEBUG_FAIL("NOT IMPLEMENTED"); }

constexpr LLPrsResT ParsePrimaryExpr(TkCursorT c) noexcept {
  if (isTokenPrimarySpecifier(c.kind()))
    return ParseListFold(c);
  else
    return DEBUG_FAIL("Unexpected token at start of primary expression.");
}

namespace detail {
constexpr LLPrsResT reduceBinaryLeftAssociative(TkCursorT c,
                                                bool (*match_binary_operation)(const TkCursorT&),
                                                LLPrsResT (*operand_parser)(TkCursorT)) noexcept {
  LLPrsResT lhs_operand = operand_parser(c);
  if (!lhs_operand) return lhs_operand;
  c.advanceTo(lhs_operand->head);

  Ast& accumulated_lhs_operand = lhs_operand->ast;
  while (match_binary_operation(c)) {
    Ast binary_operation{c};
    c.advance();

    auto rhs_operand = operand_parser(c);
    if (!rhs_operand) return rhs_operand;
    c.advanceTo(rhs_operand->head);

    binary_operation.setSourceBegin(accumulated_lhs_operand.sourceBegin());
    binary_operation.setSourceEnd(rhs_operand->ast.sourceEnd());

    binary_operation.branches.push_back(std::move(accumulated_lhs_operand));
    binary_operation.branches.push_back(std::move(rhs_operand->ast));

    accumulated_lhs_operand = std::move(binary_operation);
    lhs_operand->head = rhs_operand->head;
  }

  return lhs_operand;
};
}  // namespace detail

constexpr LLPrsResT ParseListFold(TkCursorT c) noexcept {
  return detail::reduceBinaryLeftAssociative(
      c, [](const auto& a) { return a.is(eToken::kComma); }, ParseAssignment);
};

// [R->L] <assignment> ::= (<logical_or> <EQUALS_SIGN>)* <logical_or>
constexpr LLPrsResT ParseAssignment(TkCursorT c) noexcept {
  if (!isTokenPrimarySpecifier(c.kind()))
    return DEBUG_FAIL("Unexpected token at start of binary access.");

  auto first_operand = ParseLogicalOr(c);
  if (!first_operand) return first_operand;
  c.advanceTo(first_operand->head);

  // No assignment operator: <assignment> ::= <logical_or>
  if (!isTokenAssignmentOperator(c.kind())) {
    return first_operand;
  }

  // Parse the complete assignment chain:
  // eg.
  //     a = b = c = d
  //
  //     [a] [=] [b] [=] [c] [=] [d]
  //
  std::vector<Ast> expression_chain{std::move(first_operand->ast)};
  while (isTokenAssignmentOperator(c.kind())) {
    expression_chain.emplace_back(c);
    c.advance();

    auto rhs_operand = ParseLogicalOr(c);
    if (!rhs_operand) return rhs_operand;

    c.advanceTo(rhs_operand->head);
    expression_chain.push_back(std::move(rhs_operand->ast));
  }

  // Reduce right-to-left:
  // eg.
  //     a = b = c = d
  //
  //     a = b = (c = d)
  //
  //     a = (b = (c = d))
  //
  const auto reduced_end = expression_chain.back().sourceEnd();
  std::unique_ptr<Ast> reduced_binop{std::make_unique<Ast>(std::move(expression_chain.back()))};
  std::unique_ptr<Ast> rhs_binop{nullptr};
  for (auto it = next(expression_chain.rbegin()); it != expression_chain.rend();) {
    Ast& operation = *it++;
    Ast& lhs_operand = *it++;

    rhs_binop = std::move(reduced_binop);
    reduced_binop = std::make_unique<Ast>(std::move(operation));

    reduced_binop->setSourceBegin(lhs_operand.sourceBegin());
    reduced_binop->branches.push_back(std::move(lhs_operand));
    reduced_binop->branches.push_back(std::move(*rhs_binop));
    reduced_binop->setSourceEnd(reduced_end);
  }

  return LLParserResult{c.index(), *reduced_binop};
}

// [L->R] <logical_or> ::= <logical_and> (<LOGICAL_OR_OPERATOR> <logical_and>)*
constexpr LLPrsResT ParseLogicalOr(TkCursorT c) noexcept {
  return detail::reduceBinaryLeftAssociative(
      c, [](const auto& a) { return a.is(eToken::kOr); }, ParseLogicalAnd);
};

// [L->R] <logical_and> ::= <bitwise_or> (<LOGICAL_AND_OPERATOR> <bitwise_or>)*
constexpr LLPrsResT ParseLogicalAnd(TkCursorT c) noexcept {
  return detail::reduceBinaryLeftAssociative(
      c, [](const auto& a) { return a.is(eToken::kAnd); }, ParseBitwiseOr);
};

// [L->R] <bitwise_or> ::= <bitwise_xor> (<VERTICAL_LINE> <bitwise_xor>)*
constexpr LLPrsResT ParseBitwiseOr(TkCursorT c) noexcept {
  return detail::reduceBinaryLeftAssociative(
      c, [](const auto& a) { return a.is(eToken::kBor); }, ParseBitwiseXor);
};

// [L->R] <bitwise_xor> ::= <bitwise_and> (<CIRCUMFLEX_ACCENT> <bitwise_and>)*
constexpr LLPrsResT ParseBitwiseXor(TkCursorT c) noexcept {
  return detail::reduceBinaryLeftAssociative(
      c, [](const auto& a) { return a.is(eToken::kXor); }, ParseBitwiseAnd);
};

// [L->R] <bitwise_and> ::= <equality> (<AMPERSAND> <equality>)*
constexpr LLPrsResT ParseBitwiseAnd(TkCursorT c) noexcept {
  return detail::reduceBinaryLeftAssociative(
      c, [](const auto& a) { return a.is(eToken::kBand); }, ParseEquality);
};

// [L->R] <equality> ::= <comparison> (<EQUALITY_OPERATOR> <comparison>)*
//			  | <comparison> (<NOT_EQUAL_OPERATOR> <comparison>)*
constexpr LLPrsResT ParseEquality(TkCursorT c) noexcept {
  return detail::reduceBinaryLeftAssociative(
      c, [](const auto& a) { return a.is(eToken::kEq) || a.is(eToken::kNeq); }, ParseComparison);
};

// [L->R] <comparison> ::= <three_way_equality> (<GREATER_THAN_SIGN> <three_way_equality>)*
//		    | <three_way_equality> (<LESS_THAN_SIGN> <three_way_equality>)*
//		    | <three_way_equality> (<GREATER_THAN_EQUAL_SIGN> <three_way_equality>)*
//		    | <three_way_equality> (<LESS_THAN_EQUAL_SIGN> <three_way_equality>)*
constexpr LLPrsResT ParseComparison(TkCursorT c) noexcept {
  return detail::reduceBinaryLeftAssociative(
      c,
      [](const auto& a) {
        return a.is(eToken::kGt) || a.is(eToken::kGte) || a.is(eToken::kLt) || a.is(eToken::kLte);
      },
      ParseThreeWayEquality);
};

// [L->R] <three_way_equality> ::= <bitwise_shift> (<SPACESHIP_OPERATOR> <bitwise_shift>)*
constexpr LLPrsResT ParseThreeWayEquality(TkCursorT c) noexcept {
  return detail::reduceBinaryLeftAssociative(
      c, [](const auto& a) { return a.is(eToken::kSpaceship); }, ParseBitwiseShift);
};

// [L->R] <bitwise_shift> ::= <summation> (<BITWISE_RIGHT_SHIFT> <summation>)*
//			  	  | <summation> (<BITWISE_LEFT_SHIFT> <summation>)*
constexpr LLPrsResT ParseBitwiseShift(TkCursorT c) noexcept {
  return detail::reduceBinaryLeftAssociative(
      c, [](const auto& a) { return a.is(eToken::kRsh) || a.is(eToken::kLsh); }, ParseSummation);
};

// [L->R] <summation> ::= <production> (<PLUS_SIGN> <production>)*
//			  | <production> (<HYPHEN_MINUS> <production>)*
constexpr LLPrsResT ParseSummation(TkCursorT c) noexcept {
  if (!isTokenPrimarySpecifier(c.kind())) {
    return DEBUG_FAIL("Unexpected token at start of summation.");
  }

  return detail::reduceBinaryLeftAssociative(
      c, [](const TkCursorT& cursor) { return cursor.is(eToken::kAdd) || cursor.is(eToken::kSub); },
      ParseProduction);
};

// [L->R] <production> ::= <prefix> (<ASTERISK> <prefix>)*
//				| <prefix> (<SOLIDUS> <prefix>)*
//				| <prefix> (<PERCENT_SIGN> <prefix>)*
constexpr LLPrsResT ParseProduction(TkCursorT c) noexcept {
  if (!isTokenPrimarySpecifier(c.kind())) {
    return DEBUG_FAIL("Unexpected token at start of production.");
  }

  return detail::reduceBinaryLeftAssociative(
      c,
      [](const TkCursorT& cursor) {
        return cursor.is(eToken::kMul) || cursor.is(eToken::kDiv) || cursor.is(eToken::kMod);
      },
      ParsePrefix);
};

// [R->L] <prefix> ::= <PLUS_SIGN> * <access>
//            | <HYPHEN_MINUS> * <access>
//            | <INCREMENT_SIGN> * <access>
//            | <DECREMENT_SIGN> * <access>
//            | <EXCLAMATION_MARK> * <access>
//            | <AMPERSAND> * <access>
//            | <ASTERISK> * <access>
constexpr LLPrsResT ParsePrefix(TkCursorT c) noexcept {
  if (!isTokenPrimarySpecifier(c.kind()))
    return DEBUG_FAIL("Unexpected token at start of prefix expression.");

  // No prefix operator: this is just an access expression.
  if (!isTokenAPrefixOperator(c.kind())) {
    return ParsePostfix(c);
  }

  // Accumulate consecutive prefix operators.
  std::vector<Ast> prefix_chain{};
  while (isTokenAPrefixOperator(c.kind())) {
    prefix_chain.emplace_back(c);

    if (c.is(eToken::kAdd)) {
      prefix_chain.back().type = eAst::kUnaryPlus;
    } else if (c.is(eToken::kSub)) {
      prefix_chain.back().type = eAst::kUnaryMinus;
    } else if (c.is(eToken::kBand)) {
      prefix_chain.back().type = eAst::kAddressOf;
    } else if (c.is(eToken::kMul)) {
      prefix_chain.back().type = eAst::kDereference;
    }
    c.advance();
  }

  // Parse the expression following the prefix operators.
  auto operand = ParsePostfix(c);
  if (!operand) return operand;
  c.advanceTo(operand->head);
  prefix_chain.push_back(std::move(operand->ast));

  // Build the unary chain from the outermost operator inward.
  //
  //     - + ~ a
  //
  //     - -> + -> ~ -> a
  //
  // Each prefix operator becomes the parent of the next element.
  Ast reduced_prefix{std::move(prefix_chain.front())};
  const auto source_end = prefix_chain.back().sourceEnd();
  reduced_prefix.setSourceEnd(source_end);
  Ast* parent = &reduced_prefix;
  for (auto it = std::next(prefix_chain.begin()); it != prefix_chain.end(); ++it) {
    parent->branches.push_back(std::move(*it));
    parent = &parent->branches.back();
    parent->setSourceEnd(source_end);
  }

  return LLParserResult{c.index(), reduced_prefix};
}

// [L->R] <postfix> ::= <resolution> <INCREMENT_SIGN>*
//             | <resolution> <DECREMENT_SIGN>*
// [L->R] <postfix> ::= <resolution> <postfix_operator>*
//                   | <resolution> <call_operator>*
//                   | <resolution> <member_access>*
// Postfix operations are left-associative:
constexpr LLPrsResT ParsePostfix(TkCursorT c) noexcept {
  if (!isTokenPrimarySpecifier(c.kind()))
    return DEBUG_FAIL("Unexpected token at start of postfix expression.");

  auto lhs = ParseResolution(c);
  if (!lhs) {
    return lhs;
  }
  c.advanceTo(lhs->head);

  while (isTokenPostfixOperator(c.kind()) || isTokenOpeningScope(c.kind()) ||
         c.is(eToken::kPeriod) || c.is(eToken::kRightArrow)) {
    // Unary postfix operator
    if (isTokenPostfixOperator(c.kind())) {
      Ast operation{c};
      const auto source_begin = lhs->ast.sourceBegin();

      c.advance();
      operation.setSourceBegin(source_begin);
      operation.setSourceEnd(c.index());
      operation.branches.push_back(std::move(lhs->ast));
      lhs->ast = std::move(operation);
      lhs->head = c.index();
      continue;
    }

    // Scoped postfix operator
    if (isTokenOpeningScope(c.kind())) {
      eAst operation_type = eAst::kInvalid;
      switch (c.kind()) {
        case eToken::kLParen:
          operation_type = eAst::kFunctionCall;
          break;
        case eToken::kLBracket:
          operation_type = eAst::kIndexOperator;
          break;
        case eToken::kLBrace:
          operation_type = eAst::kListingOperator;
          break;
      }

      // Empty scope
      auto closing_scope = c.next();
      if (!closing_scope.atEnd() && isTokenClosingScopeOf(c.kind(), closing_scope.kind())) {
        closing_scope.advance();
        Ast operation{operation_type, c.index(), closing_scope.index(),
                      Ast{eAst::kArguments, c.index(), closing_scope.index()}};
        const auto source_begin = lhs->ast.sourceBegin();

        operation.setSourceBegin(source_begin);
        operation.setSourceEnd(closing_scope.index());

        // The lhs is the first branch; arguments are already present.
        operation.branches.insert(operation.branches.begin(), std::move(lhs->ast));
        lhs->ast = std::move(operation);
        lhs->head = closing_scope.index();
        c.advanceTo(closing_scope.index());
        continue;
      }

      // Non-empty scope.
      auto arguments = ParseScopedArguments(c);
      if (!arguments) {
        return arguments;
      }
      const auto source_begin = lhs->ast.sourceBegin();
      const auto source_end = arguments->ast.sourceEnd();
      Ast operation{operation_type, c.index(), source_end};
      operation.setSourceBegin(source_begin);
      operation.setSourceEnd(source_end);

      // LHS comes before the arguments.
      operation.branches.insert(operation.branches.begin(), std::move(lhs->ast));
      operation.branches.push_back(std::move(arguments->ast));
      lhs->ast = std::move(operation);
      lhs->head = arguments->head;
      c.advanceTo(arguments->head);
      continue;
    }

    // Member access or pointer member access
    // The RHS is parsed as a resolution expression.
    eToken access_token_kind = c.kind();
    c.advance();  // consume '.' or '->'

    auto rhs = ParseResolution(c);
    if (!rhs) return rhs;
    const auto source_begin = lhs->ast.sourceBegin();
    const auto source_end = rhs->head;
    Ast operation{};
    if (access_token_kind == eToken::kPeriod) {
      operation = {eAst::kMemberAccess, source_begin, source_end, std::move(rhs->ast)};
    } else {  // eToken::kRightArrow
      operation = {eAst::kPointerMemberAccess, source_begin, source_end, std::move(rhs->ast)};
    }

    // LHS is the first branch, RHS was supplied by the constructor.
    operation.branches.insert(operation.branches.begin(), std::move(lhs->ast));
    operation.setSourceBegin(source_begin);
    operation.setSourceEnd(source_end);
    lhs->ast = std::move(operation);
    lhs->head = rhs->head;
    c.advanceTo(rhs->head);
  }

  return lhs;
}
// [L->R] <resolution> ::= <operand> (<DOUBLE_COLON> <operand>) *
//                 | <DOUBLE_COLON> <operand> (<DOUBLE_COLON> <operand>) *
constexpr LLPrsResT ParseResolution(TkCursorT c) noexcept {
  if (!isTokenPrimarySpecifier(c.kind()))
    return DEBUG_FAIL("Unexpected token at start of binary resolution.");

  // Handle special case where '::' is a prefix, the first operator becomes kRootNamespace.
  Ast lhs_operand{};
  if (c.is(eToken::kDoubleColon)) {
    lhs_operand = Ast{eAst::kRootNamespace, c.index(), c.advance().index()};
  }
  // Retrieve the first left hand side operand/subexpression.
  else {
    auto lhs_operand_res = ParsePrimaryOperand(c);
    if (!lhs_operand_res) return lhs_operand_res;
    c.advanceTo(lhs_operand_res->head);
    lhs_operand = lhs_operand_res->ast;
  }

  Ast& accumulated_lhs_operand = lhs_operand;
  while (c.is(eToken::kDoubleColon)) {
    Ast binary_operation{eAst::kResolutionOp, c.index(), c.advance().index()};

    auto rhs_operand = ParsePrimaryOperand(c);
    if (!rhs_operand) return rhs_operand;
    c.advanceTo(rhs_operand->head);

    binary_operation.setSourceBegin(accumulated_lhs_operand.sourceBegin());
    binary_operation.setSourceEnd(rhs_operand->ast.sourceEnd());

    binary_operation.branches.push_back(std::move(accumulated_lhs_operand));
    binary_operation.branches.push_back(std::move(rhs_operand->ast));

    accumulated_lhs_operand = std::move(binary_operation);
  }

  return LLParserResult{c.index(), lhs_operand};
};

constexpr LLPrsResT ParseScopedArguments(TkCursorT c) noexcept {
  if (c.is(eToken::kLParen)) return ParseParenArguments(c);
  if (c.is(eToken::kLBracket)) return ParseSquareArguments(c);
  if (c.is(eToken::kLBrace)) return ParseCurlyArguments(c);
  return DEBUG_FAIL("Expected an opening scope.");
}
constexpr LLPrsResT ParseParenArguments(TkCursorT c) noexcept {
  if (!c.is(eToken::kLParen)) return DEBUG_FAIL("Expected an opening scope.");
  // For now we simply parse a primary expr inside the scope and change the root ast type.
  auto args = ParseParenSubexpr(c);
  if (!args) return args;
  args->ast.type = eAst::kArguments;
  return args;
}
constexpr LLPrsResT ParseSquareArguments(TkCursorT c) noexcept {
  if (!c.is(eToken::kLBracket)) return DEBUG_FAIL("Expected an opening bracket.");
  // For now we simply parse a primary expr inside the scope and change the root ast type.
  auto args = ParseSquareSubexpr(c);
  if (!args) return args;
  args->ast.type = eAst::kArguments;
  return args;
}
constexpr LLPrsResT ParseCurlyArguments(TkCursorT c) noexcept {
  if (!c.is(eToken::kLBrace)) return DEBUG_FAIL("Expected an opening brace.");
  // For now we simply parse a primary expr inside the scope and change the root ast type.
  auto args = ParseCurlySubexpr(c);
  if (!args) return args;
  args->ast.type = eAst::kArguments;
  return args;
}

constexpr LLPrsResT ParsePrimaryOperand(TkCursorT c) noexcept {
  if (c.is(eToken::kIdent)) return ParseIdentityOperand(c);
  if (isTokenAnOperand(c.kind())) return ParseValueOperand(c);
  if (isTokenOpeningScope(c.kind())) return ParseOperandSet(c);
  return DEBUG_FAIL("Expected an identity operand, value operand or operand set.");
};
constexpr LLPrsResT ParseOperandSet(TkCursorT c) noexcept {
  if (c.is(eToken::kLParen)) return ParseParenSubexpr(c);
  if (c.is(eToken::kLBracket)) return ParseSquareSubexpr(c);
  if (c.is(eToken::kLBrace)) return ParseCurlySubexpr(c);
  return DEBUG_FAIL("Expected an opening scope.");
}
constexpr LLPrsResT ParseIdentityOperand(TkCursorT c) noexcept {
  if (c.is(eToken::kIdent)) {
    Ast ident_node{eAst::kIdent, c.index(), c.index() + 1};
    c.advance();
    return LLParserResult(c.index(), ident_node);
  } else {
    return DEBUG_FAIL("Expected an identifier.");
  }
};
constexpr LLPrsResT ParseValueOperand(TkCursorT c) noexcept {
  if (!c.is(eToken::kIdent) && isTokenAnOperand(c.kind())) {
    Ast ast{c};
    return LLParserResult(c.next().index(), ast);
  } else
    return DEBUG_FAIL("Expected a value-operand.");
};
constexpr LLPrsResT ParseParenSubexpr(TkCursorT c) noexcept {
  if (!c.is(eToken::kLParen)) return DEBUG_FAIL("Expected a left parentheses.");
  auto subexpr_beg = c.index();
  c.advance();
  auto subexpr = ParseListFold(c);
  if (!subexpr) return subexpr;
  c.advanceTo(subexpr->head);
  if (!c.is(eToken::kRParen)) return DEBUG_FAIL("Expected a right parentheses.");
  c.advance();
  return LLParserResult(c.index(),
                        Ast(eAst::kSubexpression, subexpr_beg, c.index(), {subexpr->ast}));
};
constexpr LLPrsResT ParseSquareSubexpr(TkCursorT c) noexcept {
  if (!c.is(eToken::kLBracket)) return DEBUG_FAIL("Expected a left square bracket.");
  auto subexpr_beg = c.index();
  c.advance();
  auto subexpr = ParseListFold(c);
  if (!subexpr) return subexpr;
  c.advanceTo(subexpr->head);
  if (!c.is(eToken::kRBracket)) return DEBUG_FAIL("Expected a right square bracket.");
  c.advance();
  return LLParserResult(c.index(),
                        Ast(eAst::kSquareSubexpr, subexpr_beg, c.index(), {subexpr->ast}));
};
constexpr LLPrsResT ParseCurlySubexpr(TkCursorT c) noexcept {
  if (!c.is(eToken::kLBrace)) return DEBUG_FAIL("Expected a left curly bracket.");
  auto subexpr_beg = c.index();
  c.advance();
  auto subexpr = ParseListFold(c);
  if (!subexpr) return subexpr;
  c.advanceTo(subexpr->head);
  if (!c.is(eToken::kRBrace)) return DEBUG_FAIL("Expected a right curly bracket.");
  c.advance();
  return LLParserResult(c.index(),
                        Ast(eAst::kCurlySubexpr, subexpr_beg, c.index(), {subexpr->ast}));
};

// constexpr SepScopePrsResT FindSeperatedScopeImpl(TkConstIterT begin, TkConstIterT end,
//                                                  eToken separator, eToken scope_type) noexcept {
//   std::vector<TkScopeT> scopes;
//   auto closing_scope = [=] {
//     switch (scope_type) {
//       case eToken::kLParen:
//         return eToken::kRParen;
//       case eToken::kLBrace:
//         return eToken::kRBrace;
//       case eToken::kLBracket:
//         return eToken::kRBracket;
//       default:
//         return eToken::kSemicolon;
//     }
//   }();
//
//   if (begin->Type() != scope_type) {
//     scopes.push_back(TkScopeT{false, begin, end});
//     return scopes;
//   }
//   std::vector<eToken> scope_type_history;
//   TkConstIterT last_closed = begin;
//   begin++;  // Skip the open list token
//   for (TkConstIterT i = begin; i < end;) {
//     if (i->TypeIs(separator) && scope_type_history.empty()) {
//       scopes.push_back(TkScopeT{true, last_closed, i + 1});
//       last_closed = i;
//     } else if (i->IsLScope()) {
//       scope_type_history.push_back(i->Type());
//     } else if (i->IsRScope() && !scope_type_history.empty()) {
//       if (i->IsRScopeOf(scope_type_history.back())) {
//         scope_type_history.pop_back();
//       } else {
//         scopes.push_back(TkScopeT{false, i, end});  // Wrong closing scope
//         return scopes;
//       }
//     } else if (i->TypeIs(closing_scope) && scope_type_history.empty()) {
//       // end of list
//       scopes.push_back(TkScopeT{true, last_closed, i + 1});
//       return scopes;
//     } else if (i->TypeIs(eToken::kEofile)) {
//       scopes.push_back(TkScopeT{false, i, end});  // End of file
//       return scopes;
//     }
//     std::advance(i, 1);
//   }
// }
//
// constexpr SepScopePrsResT FindSeperatedParen(TkConstIterT begin, TkConstIterT end,
//                                              eToken separator) noexcept {
//   return FindSeperatedScopeImpl(begin, end, separator, eToken::kLParen);
// };
// constexpr SepScopePrsResT FindSeperatedBrace(TkConstIterT begin, TkConstIterT end,
//                                              eToken separator) noexcept {
//   return FindSeperatedScopeImpl(begin, end, separator, eToken::kLBrace);
// };
// constexpr SepScopePrsResT FindSeperatedBracket(TkConstIterT begin, TkConstIterT end,
//                                                eToken separator) noexcept {
//   return FindSeperatedScopeImpl(begin, end, separator, eToken::kLBracket);
// };
// constexpr ScopePrsResT FindStatement(eToken open, eToken close, TkConstIterT begin,
//                                      TkConstIterT end) noexcept {
//   auto opening = begin;
//   int scope_depth = 0;
//   std::vector<eToken> scope_history;
//   // Find the last matching close token that is not within a () [] or {} scope.
//   // Start from the token after the open token. All scope depths should be 0
//   // at the end of the loop on a valid scope.
//   for (auto c = begin + 1; c != end; c++) {
//     if (c->IsLScope() || c->TypeIs(open)) {
//       scope_depth++;
//       scope_history.push_back(c->Type());
//     } else if (c->IsRScope() || c->TypeIs(close)) {
//       if (scope_history.empty())
//         if (c->TypeIs(close))
//           return TkScopeT{true, opening, c + 1};
//         else
//           return DEBUG_FAIL("Unclosed scope.");
//
//       if (!c->IsRScopeOf(scope_history.back())) return DEBUG_FAIL("Mismatched scopes.");
//
//       scope_history.pop_back();
//       scope_depth--;
//     } else
//       continue;
//   }
//   return DEBUG_FAIL("Unclosed scope.");
// }
// constexpr ScopePrsResT FindOpenStatement(eToken close, TkConstIterT begin,
//                                          TkConstIterT end) noexcept {
//   auto opening = begin;
//   int scope_depth = 0;
//   std::vector<eToken> scope_history;
//   for (auto c = begin + 1; c != end; c++) {
//     if (c->IsLScope()) {
//       scope_depth++;
//       scope_history.push_back(c->Type());
//     } else if (c->IsRScope() || c->TypeIs(close)) {
//       if (scope_history.empty())
//         if (c->TypeIs(close))
//           return TkScopeT{true, opening, c + 1};
//         else
//           return DEBUG_FAIL("Unclosed scope.");
//
//       if (!c->IsRScopeOf(scope_history.back())) return DEBUG_FAIL("Mismatched scopes.");
//
//       scope_history.pop_back();
//       scope_depth--;
//     } else
//       continue;
//   }
//   return DEBUG_FAIL("Unclosed scope.");
// };
// constexpr ScopePrsResT FindOpenStatement(std::vector<eToken> close, TkConstIterT begin,
//                                          TkConstIterT end) noexcept {
//   auto opening = begin;
//   int scope_depth = 0;
//   std::vector<eToken> scope_history;
//   for (auto c = begin + 1; c != end; c++) {
//     if (c->IsLScope()) {
//       scope_depth++;
//       scope_history.push_back(c->Type());
//     } else if (c->IsRScope() || std::any_of(close.cbegin(), close.cend(),
//                                             [=](const auto& v) { return c->TypeIs(v); })) {
//       if (scope_history.empty())
//         if (std::any_of(close.cbegin(), close.cend(), [=](const auto& v) { return c->TypeIs(v);
//         }))
//           return TkScopeT{true, opening, c + 1};
//         else
//           return DEBUG_FAIL("Unclosed scope.");
//
//       if (!c->IsRScopeOf(scope_history.back())) return DEBUG_FAIL("Mismatched scopes.");
//
//       scope_history.pop_back();
//       scope_depth--;
//     } else
//       continue;
//   }
//   return DEBUG_FAIL("Unclosed scope.");
// };
// constexpr ScopePrsResT FindProgramStatement(TkConstIterT begin, TkConstIterT end) noexcept {
//   return FindOpenStatement(eToken::kSemicolon, begin, end);
// };
// constexpr ScopePrsResT FindProgramStatement(TkCursorT cursor) noexcept {
//   return FindOpenStatement(eToken::kSemicolon, cursor.Iter(), cursor.End());
// };

constexpr std::pair<Ast, Diagnostics> parse(std::size_t offset, const Token* beg,
                                            const Token* end) noexcept {
  Diagnostics diagnostics{};
  LLPrsResT parse_res = ParseSyntax(TkCursorT{offset, beg, end});
  if (!parse_res) {
    diagnostics->push_back(parse_res.error());
    return {Ast{eAst::kPoison}, std::move(diagnostics)};
  }
  return {std::move(parse_res->ast), std::move(diagnostics)};
}

namespace detail {

/// @brief Parses possibly existing modifiers at the start of a decl and advances cursor if
/// necessary.
/// @param c Token cursor which will be advanced past the modifiers or stay in place if none.
/// @return Error or the resulting ast node.
constexpr LLPrsResT ParseOptionalModifiers(TkCursorT& c) {
  Ast mod_node;
  auto decl_begin = c.index();
  if (isTokenModifier(c.kind())) {
    auto mod_result = ParseModifiers(c);
    if (!mod_result) return mod_result;
    mod_node = std::move(mod_result.value().ast);
    c.advanceTo(mod_result->head);
  } else {
    // No modifiers found.
    mod_node = Ast(eAst::kModifiers, c.index(), c.index());
  }
  return LLParserResult(c.index(), mod_node);
}
}  // namespace detail

}  // namespace parser

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* end Internal parsing methods impl */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}  // namespace frontend
}  // namespace ssgc

/// @} // end of cnd_compiler_data

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
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