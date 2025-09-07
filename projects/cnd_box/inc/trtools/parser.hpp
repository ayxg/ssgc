///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the Apache License, Version 2.0(the "License");
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
#include "use_ccapi.hpp"
#include "use_corevals.hpp"
#include "use_clmsg.hpp"
#include "cldata/tk.hpp"
#include "cldata/ast.hpp"
#include "cldata/synth_ast.hpp"
#include "TkCursor.hpp"
#include "token_scope.hpp"

#include "Lexer.hpp"
#include "CompilerIO.hpp"
// clang-format on

/// File local macro, returns a debug error for creating and debugging compiler errors on the fly.
#define DEBUG_FAIL(msg) \
  CompilerProcessFailure(MakeClMsg<eClErr::kCompilerDevDebugError>(source_location::current(), msg))
#define DEBUG_MSG(msg) MakeClMsg<eClErr::kCompilerDevDebugError>(source_location::current(), msg)

#ifdef _DEBUG
#define CND_CLDEV_DEBUG_MODE 1
#define CND_DEBUG_ASSERT(expression, msg)                                                                      \
  if ((!!!(expression))) {                                                                                     \
    throw(std::string(#expression "|" msg "\n[") + std::source_location::current().file_name() + "]" + "\n[" + \
          std::source_location::current().function_name() + "]");                                              \
  }
#else
#define CND_CLDEV_DEBUG_MODE
#define CND_DEBUG_ASSERT(expression, msg)
#endif

namespace cnd {
namespace trtools {
namespace parser {
using cldev::clmsg::MakeClMsg;
using std::source_location;
using std::span;

/// Underlying data structure of the token array passed to the parser.
using TkVectorT = Vec<Tk>;
using TkVecConstIterT = Vec<Tk>::const_iterator;
using TkVecIterT = Vec<Tk>::iterator;
using TkContainerT = span<const Tk>;
using TkConstIterT = TkContainerT::const_iterator;
using TkIterT = TkContainerT::iterator;

/// Iterator over a specific const Tk range. Used by the parser when iterating passed data.
using TkCursorT = TkCursor<span>;
using TkScopeT = TkScope<span>;

/// Result of a LL recursive parsing method. A TkCursorT from where to continue reading and the resulting Ast.
struct LLParserResult {
  TkCursorT head{};
  Ast ast{};
};

/// Result of a LL recursive parsing method if it may fail and return an error(most may fail).
using LLPrsResT = CompilerProcessResult<LLParserResult>;
using LRPrsResT = CompilerProcessResult<Ast>;
using ScopePrsResT = CompilerProcessResult<TkScopeT>;
using SepScopePrsResT = CompilerProcessResult<Vec<TkScopeT>>;

/// @defgroup cand_compiler_parser_util Utilities for Common Parser Patterns
/// @ingroup cand_compiler_parser
/// @brief Utilities for Common Parser Patterns.
///
/// Only for use within the parser's internal implementation.
namespace detail {

/// Advance cursor to parse result head. Move and append ast node from parse result to root ast.
CND_CX void ExtractAndAdvance(TkCursorT& cursor, Ast& root, LLPrsResT& parse_res) CND_NX {
  cursor.Advance(parse_res.value().head);
  root.PushBack(move(parse_res.value().ast));
};

// Advance cursor to parse result head. Return forwarded r-value ast node from parse result.
CND_CX Ast&& ExtractAndAdvance(TkCursorT cursor, LLPrsResT& parse_res) CND_NX {
  cursor.Advance(parse_res.value().head);
  return forward<Ast>(move(parse_res.value().ast));
};

/// @brief Parses possibly existing modifiers at the start of a decl and advances cursor if necessary.
/// @param c Token cursor which will be advanced past the modifiers or stay in place if none.
/// @return Error or the resulting ast node.
CND_CX LLPrsResT ParseOptionalModifiers(TkCursorT& c);
CND_CX LLPrsResT ParseGenericBinaryLeftAssociative(TkCursorT c, bool (*next_cond)(const TkCursorT&),
                                                   LLPrsResT (*operand_parser)(TkCursorT)) CND_NX(CND_CLDEV_DEBUG_MODE);
}  // namespace detail

/// @defgroup cand_compiler_parser_parse Internal parsing methods
/// @ingroup cand_compiler_parser
/// @brief LL Recursive Parsing Methods.
///
/// Ordered from the most to least specific. Except ParseTokens which is the main method to call when parsing. The rest
/// are for internal use. Each method is responsible for parsing a specific type of statement or declaration. Each
/// method returns an ParserResultT. The result contains the parsed AST node, and the location of the last token parsed.
/// Begin parsing the next statement from this location. The result also contains an error message if the parsing
/// failed. In this case parsing should stop and the error message should be reported.
/// @{

CND_CX LLPrsResT ParseOperand(TkCursorT c) CND_NX;
CND_CX LLPrsResT ParseArguments(TkCursorT c) CND_NX;
CND_CX LLPrsResT ParseIndexingArguments(TkCursorT c) CND_NX;
CND_CX LLPrsResT ParseListingArguments(TkCursorT c) CND_NX;
CND_CX LLPrsResT ParsePrimaryStatement(TkCursorT c) CND_NX;
CND_CX LLPrsResT ParseConditionalSubExpression(TkCursorT c) CND_NX;
CND_CX LLPrsResT ParsePrimaryPreIdentifier(TkCursorT c) CND_NX;
CND_CX LLPrsResT ParsePrimaryPostIdentifier(TkCursorT c) CND_NX;
CND_CX LLPrsResT ParseModifiers(TkCursorT c) CND_NX;
CND_CX LLPrsResT ParseReturnStmt(TkCursorT c) CND_NX;
CND_CX LLPrsResT ParseMethodParameters(TkCursorT c) CND_NX;
CND_CX LLPrsResT ParseMethodReturnParameters(TkCursorT c) CND_NX;
CND_CX LLPrsResT ParseMethodSignature(TkCursorT c) CND_NX;
CND_CX LLPrsResT ParseMethodDef(TkCursorT c) CND_NX;
CND_CX LLPrsResT ParseMainDef(TkCursorT c) CND_NX;
CND_CX LLPrsResT ParseClassDef(TkCursorT c) CND_NX;
CND_CX LLPrsResT ParseLibDef(TkCursorT c) CND_NX;
CND_CX LLPrsResT ParsePragmaticStmt(TkCursorT c) CND_NX;
CND_CX LLPrsResT ParseFunctionalStmt(TkCursorT c) CND_NX;
CND_CX LLPrsResT ParseConditionalStmt(TkCursorT c) CND_NX;
CND_CX LLPrsResT ParseIfDecl(TkCursorT c) CND_NX;
CND_CX LLPrsResT ParseMainDecl(TkCursorT c) CND_NX;
CND_CX LLPrsResT ParseLibDecl(TkCursorT c) CND_NX;
CND_CX LLPrsResT ParseImportDecl(TkCursorT c) CND_NX;
CND_CX LLPrsResT ParseIncludeStmt(TkCursorT c) CND_NX;
CND_CX LLPrsResT ParsePragmaStmt(TkCursorT c) CND_NX;

CND_CX LLPrsResT ParseWhileDecl(TkCursorT c) CND_NX;
CND_CX LLPrsResT ParseForDecl(TkCursorT c) CND_NX;
CND_CX LLPrsResT ParseUsingDecl(TkCursorT c) CND_NX;
CND_CX LLPrsResT ParseVariableDecl(TkCursorT c) CND_NX;
CND_CX LLPrsResT ParseMethodDecl(TkCursorT c) CND_NX;
CND_CX LLPrsResT ParseClassDecl(TkCursorT c) CND_NX;
CND_CX LLPrsResT ParseEnumDecl(TkCursorT c) CND_NX;
CND_CX LLPrsResT ParseEnumDef(TkCursorT c) CND_NX;
CND_CX LLPrsResT ParseEnumBlock(TkCursorT c) CND_NX;
CND_CX LLPrsResT ParseSyntax(TkCursorT c) CND_NX;
CND_CX LRPrsResT ParseExpr(TkCursorT c) CND_NX;

CND_CX LLPrsResT ParsePrimaryExpr(TkCursorT c) CND_NX;
// CND_CX LLPrsResT ParseSubExpr(TkCursorT c) CND_NX;
CND_CX LLPrsResT ParseListFold(TkCursorT c) CND_NX;
CND_CX LLPrsResT ParseAssignment(TkCursorT c) CND_NX;
CND_CX LLPrsResT ParseLogicalOr(TkCursorT c) CND_NX;
CND_CX LLPrsResT ParseLogicalAnd(TkCursorT c) CND_NX;
CND_CX LLPrsResT ParseBitwiseOr(TkCursorT c) CND_NX;
CND_CX LLPrsResT ParseBitwiseXor(TkCursorT c) CND_NX;
CND_CX LLPrsResT ParseBitwiseAnd(TkCursorT c) CND_NX;
CND_CX LLPrsResT ParseEquality(TkCursorT c) CND_NX;
CND_CX LLPrsResT ParseComparison(TkCursorT c) CND_NX;
CND_CX LLPrsResT ParseThreeWayEquality(TkCursorT c) CND_NX;
CND_CX LLPrsResT ParseBitwiseShift(TkCursorT c) CND_NX;
CND_CX LLPrsResT ParseSummation(TkCursorT c) CND_NX;
CND_CX LLPrsResT ParseProduction(TkCursorT c) CND_NX(CND_CLDEV_DEBUG_MODE);
CND_CX LLPrsResT ParsePrefix(TkCursorT c) CND_NX(CND_CLDEV_DEBUG_MODE);
CND_CX LLPrsResT ParseAccess(TkCursorT c) CND_NX(CND_CLDEV_DEBUG_MODE);
CND_CX LLPrsResT ParsePostfix(TkCursorT c) CND_NX(CND_CLDEV_DEBUG_MODE);
CND_CX LLPrsResT ParseResolution(TkCursorT c) CND_NX(CND_CLDEV_DEBUG_MODE);

CND_CX LLPrsResT ParseScopedArguments(TkCursorT c) CND_NX(CND_CLDEV_DEBUG_MODE);
CND_CX LLPrsResT ParseParenArguments(TkCursorT c) CND_NX(CND_CLDEV_DEBUG_MODE);
CND_CX LLPrsResT ParseSquareArguments(TkCursorT c) CND_NX(CND_CLDEV_DEBUG_MODE);
CND_CX LLPrsResT ParseCurlyArguments(TkCursorT c) CND_NX(CND_CLDEV_DEBUG_MODE);

CND_CX LLPrsResT ParsePrimaryOperand(TkCursorT c) CND_NX(CND_CLDEV_DEBUG_MODE);
CND_CX LLPrsResT ParseOperandSet(TkCursorT c) CND_NX(CND_CLDEV_DEBUG_MODE);
CND_CX LLPrsResT ParseIdentityOperand(TkCursorT c) CND_NX(CND_CLDEV_DEBUG_MODE);
CND_CX LLPrsResT ParseValueOperand(TkCursorT c) CND_NX(CND_CLDEV_DEBUG_MODE);
CND_CX LLPrsResT ParseParenSubexpr(TkCursorT c) CND_NX(CND_CLDEV_DEBUG_MODE);
CND_CX LLPrsResT ParseSquareSubexpr(TkCursorT c) CND_NX(CND_CLDEV_DEBUG_MODE);
CND_CX LLPrsResT ParseCurlySubexpr(TkCursorT c) CND_NX(CND_CLDEV_DEBUG_MODE);

/// @} // end of cand_compiler_parser_parse

/// @defgroup cand_compiler_parser_scope Internal parsing methods
/// @ingroup cand_compiler_parser
/// @brief Methods to determining the beginnings and ends of a scope or set of separated scopes froma tokenized source.
/// @see TkScope in tk_scope.hpp
/// @{

CND_CX ScopePrsResT FindParen(TkConstIterT begin, TkConstIterT end) CND_NX;
CND_CX ScopePrsResT FindParen(TkCursorT crsr) CND_NX;
CND_CX ScopePrsResT FindBrace(TkConstIterT begin, TkConstIterT end) CND_NX;
CND_CX ScopePrsResT FindBrace(TkCursorT crsr) CND_NX;
CND_CX ScopePrsResT FindBracket(TkConstIterT begin, TkConstIterT end) CND_NX;
CND_CX ScopePrsResT FindBracket(TkCursorT crsr) CND_NX;

CND_CX ScopePrsResT FindStatement(eTk open, eTk close, TkConstIterT begin, TkConstIterT end) CND_NX;
CND_CX ScopePrsResT FindOpenStatement(eTk close, TkConstIterT begin, TkConstIterT end) CND_NX;
CND_CX ScopePrsResT FindOpenStatement(Vec<eTk> close, TkConstIterT begin, TkConstIterT end) CND_NX;
CND_CX ScopePrsResT FindProgramStatement(TkConstIterT begin, TkConstIterT end) CND_NX;
CND_CX ScopePrsResT FindProgramStatement(TkCursorT cursor) CND_NX;

CND_CX SepScopePrsResT FindSeperatedParen(TkConstIterT begin, TkConstIterT end, eTk separator) CND_NX;
CND_CX SepScopePrsResT FindSeperatedParen(TkCursorT crsr, eTk separator) CND_NX;
CND_CX SepScopePrsResT FindSeperatedParen(TkScopeT ls, eTk separator) CND_NX;
CND_CX SepScopePrsResT FindSeperatedBrace(TkConstIterT begin, TkConstIterT end, eTk separator) CND_NX;
CND_CX SepScopePrsResT FindSeperatedBrace(TkCursorT crsr, eTk separator) CND_NX;
CND_CX SepScopePrsResT FindSeperatedBrace(TkScopeT ls, eTk separator) CND_NX;
CND_CX SepScopePrsResT FindSeperatedBracket(TkConstIterT begin, TkConstIterT end, eTk separator) CND_NX;
CND_CX SepScopePrsResT FindSeperatedBracket(TkCursorT crsr, eTk separator) CND_NX;
CND_CX SepScopePrsResT FindSeperatedBracket(const TkScopeT& ls, eTk separator) CND_NX;

/// @} // end of cand_compiler_parser_scope

}  // namespace parser

/// @brief Parse pre-loaded or generated C& source file data.
/// @param src_data Source file data. EOF character should be the last element.
static CompilerProcessResult<Ast> ParseSource(const Vec<char>& src_data) {
  StrView src_view = {src_data.begin(), src_data.end()};
  auto lex_res = Lexer::Lex(src_view);
  if (!lex_res) return CompilerProcessFailure(lex_res.error());
  auto sanitized_src = Lexer::Sanitize(*lex_res);
  std::span<const Tk> src_span = std::span{sanitized_src.data(), sanitized_src.size()};
  auto parse_res = parser::ParseSyntax({src_span.cbegin(), src_span.cend()});
  if (!parse_res) return CompilerProcessFailure(parse_res.error());
  return parse_res->ast;
}

/// @brief Parse a C& source file.
/// @param fp Source file path. Absolute or relative to working dir.
static CompilerProcessResult<Ast> ParseFile(const Path& fp) {
  auto loaded_src = LoadSourceFile<char>(fp.string());
  if (!loaded_src) loaded_src.error(); 
  return ParseSource(*loaded_src);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Internal parsing methods impl */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace parser {
CND_CX LLPrsResT ParseOperand(TkCursorT c) CND_NX {
  if (c.IsAnOperand())
    return LLParserResult(c.Next(), Ast{c});
  else  // ImplExpectedToken
    return DEBUG_FAIL("ImplExpectedToken");
};

CND_CX LLPrsResT ParseArguments(TkCursorT c) CND_NX {
  using enum eTk;
  if (c.TypeIsnt(kLParen)) return DEBUG_FAIL("ImplExpectedToken");

  if (c.Next().TypeIs(kRParen))  // Empty Arg.
    return LLParserResult(c.Advance(2), Ast(eAst::kArguments));

  SepScopePrsResT arg_scopes = FindSeperatedParen(c.Iter(), c.End(), kComma);
  if (!arg_scopes) return LLPrsResT::Failure(move(arg_scopes.error()));

  Ast arguments_node = eAst::kArguments;
  for (const auto& arg_scope : arg_scopes.value()) {
    LRPrsResT arg_result = ParseExpr(arg_scope.Contained());
    if (!arg_result) return LLPrsResT::Failure(move(arg_result.error()));
    arguments_node.children.emplace_back(move(arg_result.value()));
  }

  return LLParserResult(c.Advance(arg_scopes.value().back().End()), move(arguments_node));
}

CND_CX LLPrsResT ParseIndexingArguments(TkCursorT c) CND_NX {
  using enum eTk;
  if (c.TypeIsnt(kLBracket)) return DEBUG_FAIL("ImplExpectedToken");
  // Check for empty arguments.
  if (c.Next().TypeIs(kRBracket)) return LLParserResult(c.Advance(2), eAst::kArguments);

  SepScopePrsResT arg_scopes = FindSeperatedBracket(c.Iter(), c.End(), kComma);
  if (!arg_scopes) return LLPrsResT::Failure(arg_scopes.error());
  Ast arguments_node = eAst::kArguments;
  for (const auto& arg_scope : arg_scopes.value()) {
    LRPrsResT arg_result = ParseExpr(arg_scope.Contained());
    if (!arg_result) return LLPrsResT::Failure(move(arg_result.error()));

    arguments_node.children.emplace_back(move(arg_result.value()));
  }

  return LLParserResult(c.Advance(arg_scopes.value().back().End()), move(arguments_node));
}

CND_CX LLPrsResT ParseListingArguments(TkCursorT c) CND_NX {
  using enum eTk;
  if (c.TypeIs(kLBrace)) return DEBUG_FAIL("ImplExpectedToken");

  // Check for empty arguments.
  if (c.Next().TypeIs(kRBrace)) return LLParserResult(c.Advance(2), eAst::kArguments);

  SepScopePrsResT arg_scopes = FindSeperatedBracket(c.Iter(), c.End(), kComma);
  if (!arg_scopes) return LLPrsResT::Failure(arg_scopes.error());
  Ast arguments_node = eAst::kArguments;
  for (const auto& arg_scope : arg_scopes.value()) {
    LRPrsResT arg_result = ParseExpr(arg_scope.Contained());
    if (!arg_result) return LLPrsResT::Failure(move(arg_result.error()));

    arguments_node.children.emplace_back(move(arg_result.value()));
  }

  return LLParserResult(c.Advance(arg_scopes.value().back().End()), move(arguments_node));
}

CND_CX LLPrsResT ParsePrimaryStatement(TkCursorT c) CND_NX {
  if (!c.IsPrimary()) return DEBUG_FAIL("ImplExpectedToken");
  LLPrsResT expr_result = ParsePrimaryExpr(c);
  if (!expr_result) return LLPrsResT::Failure(move(expr_result.error()));
  c.Advance(expr_result->head);
  if (c.TypeIs(eTk::kSemicolon)) {
    c.Advance();
  } else {
    return DEBUG_FAIL("Expected semicolon after <primary-expr>.");
  }
  return LLParserResult(c, move(expr_result->ast));
}

CND_CX LLPrsResT ParseConditionalSubExpression(TkCursorT c) CND_NX {
  ScopePrsResT paren_scope = FindParen(c);
  if (!paren_scope) return LLPrsResT::Failure(move(paren_scope.error()));
  if (!paren_scope.value().ContainedBegin()->IsPrimary())
    return DEBUG_FAIL("Invalid start of conditional sub-expression.");
  LLPrsResT subexpr_result = ParsePrimaryExpr({paren_scope.value().ContainedBegin(), c.End()});
  if (!subexpr_result) return LLPrsResT::Failure(move(subexpr_result.error()));
  return LLParserResult(c.Advance(paren_scope.value().End()), move(subexpr_result->ast));
}

CND_CX LLPrsResT ParsePrimaryPreIdentifier(TkCursorT c) CND_NX {
  using enum eTk;
  // Cursor on <@>. No pre-identifier. Implicitly an any type.
  if (c.TypeIs(kCommercialAt))
    return LLParserResult(c.Advance(), eAst::kKwAny);
  else if (c.IsPrimary()) {
    LLPrsResT expr_result = ParsePrimaryExpr(c);
    if (!expr_result) return LLPrsResT::Failure(move(expr_result.error()));
    c.Advance(expr_result->head);
    if (c.TypeIs(eTk::kCommercialAt))
      c.Advance();
    else
      return DEBUG_FAIL("Expecting a <@> after <primary-type-expression>.");
    return LLParserResult(c, move(expr_result->ast));
  } else
    return DEBUG_FAIL("Expecting a <primary-type-expression> or <@>.");
}

/// ParsePrimaryPostIdentifier <primary_expr>: or <primary_expr>;
///
/// - Handles parsing of primary expressions that are followed by a colon or semicolon.
/// - Used in declarations, method signatures.
CND_CX LLPrsResT ParsePrimaryPostIdentifier(TkCursorT c) CND_NX {
  using enum eTk;
  // Cursor on <:>. No post-identifier. Implicitly an any type.
  if (c.TypeIs(kCommercialAt))
    return LLParserResult(c.Advance(), eAst::kKwAny);
  else if (c.IsPrimary()) {
    LLPrsResT expr_result = ParsePrimaryExpr(c);
    if (!expr_result) return LLPrsResT::Failure(move(expr_result.error()));
    c.Advance(expr_result->head);

    // !SPECIAL CASE: if end of statement is a semicolon, go 1 offset back.
    // This will allow the caller to determine if the statement is terminated or not.
    if (c.TypeIs(eTk::kSemicolon))
      return LLParserResult(c, move(expr_result->ast));
    else if (c.TypeIs(eTk::kColon)) {
      c.Advance();
      return LLParserResult(c, move(expr_result->ast));
    } else
      return DEBUG_FAIL("Expecting a <:> or <;> after <primary-type-expression>.");
  } else
    return DEBUG_FAIL("Expecting a <primary-type-expression> or <:>.");
}

CND_CX LLPrsResT ParseModifiers(TkCursorT c) CND_NX {
  if (!c.IsModifierKeyword()) return DEBUG_FAIL("Expecting a <modifier-keyword>.");
  Ast mod_node(eAst::kModifiers);
  while (c.IsModifierKeyword()) {
    mod_node.children.push_back(c);
    c.Advance();
  }
  return LLParserResult(c, move(mod_node));
}

CND_CX LLPrsResT ParseReturnStmt(TkCursorT c) CND_NX {
  using enum eTk;
  if (c.TypeIsnt(kKwReturn)) return DEBUG_FAIL("Expecting a <kw-return>.");
  c.Advance();

  // Empty return statement.
  if (c.TypeIs(kSemicolon)) {
    Ast return_stmt{eAst::kKwReturn};
    return LLParserResult(c.Advance(), move(return_stmt));
  }

  LLPrsResT expr_result = ParsePrimaryStatement(c);
  if (!expr_result) return expr_result;
  Ast return_stmt(eAst::kKwReturn);
  return_stmt.children.push_back(move(expr_result.value().ast));
  return LLParserResult(expr_result.value().head, move(return_stmt));
}

CND_CX LLPrsResT ParseFunctionalStmt(TkCursorT c) CND_NX {
  using enum eTk;

  if (c.TypeIs(kSemicolon)) return DEBUG_FAIL("Empty statement in code.");

  if (c.IsDeclarativeKeyword()) {
    switch (c.Type()) {
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

  TkCursor<span> decl_begin = c;
  while (c.IsModifierKeyword()) c.Advance();

  if (c.IsDeclarativeKeyword()) {
    switch (c.Type()) {
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

CND_CX LLPrsResT ParseConditionalStmt(TkCursorT c) CND_NX {
  using enum eTk;

  if (c.IsDeclarativeKeyword()) {
    switch (c.Type()) {
      case kKwUse:
        return ParseUsingDecl(c);
      case kKwImport:
        return ParseImportDecl(c);
      default:
        break;
    }
  }

  TkCursor<span> decl_begin = c;
  while (c.IsModifierKeyword()) c.Advance();

  if (c.IsDeclarativeKeyword()) {
    switch (c.Type()) {
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

CND_CX LLPrsResT ParseIfDecl(TkCursorT c) CND_NX {
  using enum eTk;

  LAMBDA xParseIfElifElse = [&c](eAst ast_type) -> LLPrsResT {
    if (c.TypeIs(kKwIf) || c.TypeIs(kKwElif)) {
      auto block_begin = c.Iter();
      c.Advance();
      LLPrsResT cond_res = ParseConditionalSubExpression(c);
      if (!cond_res) return cond_res;

      c.Advance(cond_res.value().head.Iter());
      // Parse the body.
      LLPrsResT body_res = ParseMethodDef(c);
      if (!body_res) return body_res;
      c.Advance(body_res.value().head.Iter());
      Ast ret = Ast{ast_type, block_begin, c.Iter()};
      ret.children.push_back(move(cond_res.value().ast));
      ret.children.push_back(move(body_res.value().ast));
      return LLParserResult(c, move(ret));

    } else if (c.TypeIs(kKwElse)) {
      auto block_begin = c.Iter();
      c.Advance();
      LLPrsResT body_res = ParseMethodDef(c);
      if (!body_res) return body_res;
      c.Advance(body_res.value().head.Iter());
      Ast ret = Ast{ast_type, block_begin, c.Iter()};
      ret.children.push_back(move(body_res.value().ast));
      return LLParserResult(c, move(ret));
    } else
      return DEBUG_FAIL("Expected if or elif or else.");
  };

  if (c.TypeIsnt(kKwIf)) return DEBUG_FAIL("Expected if.");

  Ast ifelifelse_stmt = eAst::kIfStatement;
  auto if_stmt = xParseIfElifElse(eAst::kKwIf);
  if (!if_stmt) return if_stmt;
  ifelifelse_stmt.children.push_back(move(if_stmt.value().ast));

  if (c.TypeIs(kSemicolon)) return LLParserResult(c.Advance(), move(ifelifelse_stmt));

  while (c.TypeIs(kKwElif)) {
    auto elif_stmt = xParseIfElifElse(eAst::kKwElif);
    if (!elif_stmt) return elif_stmt;
    ifelifelse_stmt.children.push_back(move(elif_stmt.value().ast));
  }

  if (c.TypeIs(kKwElse)) {
    auto else_stmt = xParseIfElifElse(eAst::kKwElse);
    if (!else_stmt) return else_stmt;
    ifelifelse_stmt.children.push_back(move(else_stmt.value().ast));
  }

  return LLParserResult(c, move(ifelifelse_stmt));
}

CND_CX LLPrsResT ParseWhileDecl(TkCursorT c) CND_NX {
  using enum eTk;
  // While Keyword, Conditional subexpression, Functional Block, Semicolon.
  if (c.TypeIsnt(kKwWhile)) return DEBUG_FAIL("Expected while.");
  auto block_begin = c.Iter();
  c.Advance();

  LLPrsResT cond_res = ParseConditionalSubExpression(c);
  if (!cond_res) return cond_res;
  c.Advance(cond_res.value().head.Iter());

  LLPrsResT body_res = ParseMethodDef(c);
  if (!body_res) return body_res;
  c.Advance(body_res.value().head.Iter());

  if (c.TypeIs(eTk::kSemicolon)) {
    c.Advance();
    Ast ret = Ast{eAst::kKwWhile, block_begin, c.Iter()};
    ret.Append(cond_res.Extract().ast, body_res.Extract().ast);
    return LLParserResult(c, move(ret));
  } else
    return DEBUG_FAIL("Expected <;> following while statement block.");
}

CND_CX LLPrsResT ParseForDecl(TkCursorT c) CND_NX {
  if (c.TypeIsnt(eTk::kKwFor)) return DEBUG_FAIL("Expected for.");
  auto stmt_begin = c.Iter();
  c.Advance();
  ScopePrsResT cond_scope = FindParen(c);
  if (!cond_scope) return LLPrsResT::Failure(move(cond_scope.error()));

  SepScopePrsResT cond_scopes = FindSeperatedParen(c.Iter(), c.End(), eTk::kSemicolon);
  if (cond_scopes.value().size() > 3)
    return DEBUG_FAIL("InvalidForLoopSyntax. For condition may have a maximum of 3 statements.");
  auto init_var_res = ParseVariableDecl({cond_scopes.value()[0].ContainedBegin(), c.End()});
  if (!init_var_res) return init_var_res;

  auto cond_res = ParsePrimaryStatement({cond_scopes.value()[1].ContainedBegin(), c.End()});
  if (!cond_res) return cond_res;

  auto inc_res = ParsePrimaryExpr({cond_scopes.value()[2].ContainedBegin(), c.End()});
  if (!inc_res) return LLPrsResT::Failure(move(inc_res.error()));
  c.Advance(cond_scope.value().End());

  auto body_res = ParseMethodDef(c);
  if (!body_res) return body_res;
  c.Advance(body_res.value().head.Iter());

  auto stmt_end = body_res.value().head.Iter();
  if (c.TypeIs(eTk::kSemicolon)) {
    c.Advance();
    std::advance(stmt_end, 1);
  }
  Ast ret = {eAst::kKwFor, stmt_begin, stmt_end};
  ret.Append(init_var_res.Extract().ast, cond_res.Extract().ast, inc_res.Extract().ast, body_res.Extract().ast);
  return LLParserResult{c, ret};
}

CND_CX LLPrsResT ParseVariableDecl(TkCursorT c) CND_NX {
  using enum eTk;
  using namespace detail;
  // Format:
  // <modifiers?><def><what*?><commercial_at?><name?><colon?><definition?>
  // <semicolon>
  auto decl_begin = c.Iter();
  LLPrsResT mods = ParseOptionalModifiers(c);
  if (!mods) return mods;
  Ast mod_node = mods.value().ast;

  // Expect a def keyword
  if (c.TypeIsnt(kKwDef)) return DEBUG_FAIL("Expected <kw-def>.");
  c.Advance();

  // Expect a 'what' token(s).
  // Everything following def until kCommericalAt is a type constraint describing 'what' is being created.
  auto what_result = ParsePrimaryPreIdentifier(c);
  if (!what_result) return what_result;
  Ast what_node = move(what_result.value().ast);
  c.Advance(what_result.value().head);

  // Expecting an identifier.
  if (c.TypeIsnt(kIdent)) return DEBUG_FAIL("Expected <identifier>.");
  Ast ident_node = c;
  c.Advance();

  // If there is a colon, this is a Definition.
  // If there is a semicolon, this is a Declaration.
  if (c.TypeIs(kColon)) {
    c.Advance();
    auto def_begin = c.Iter();
    auto def_result = ParsePrimaryStatement(c);
    if (!def_result) return def_result;
    c.Advance(def_result.value().head);
    Ast def_node = Ast(eAst::kVariableDefinition, def_begin, c.Iter(), nullptr, {def_result.Extract().ast});

    return LLParserResult(
        c, Ast(eAst::kVariableDeclaration, decl_begin, c.Iter(), nullptr, {mod_node, what_node, ident_node, def_node}));
  } else if (c.TypeIs(kSemicolon)) {
    c.Advance();
    return LLParserResult(
        c, Ast(eAst::kVariableDeclaration, decl_begin, c.Iter(), nullptr, {mod_node, what_node, ident_node}));
  } else {
    return DEBUG_FAIL("Expected colon or semicolon.");
  }
}

CND_CX LLPrsResT ParseMethodDecl(TkCursorT c) CND_NX {
  using enum eTk;
  // Format:
  // <modifiers?><fn><commercial_at?><name?><signature?><colon?>
  // <definition?> <semicolon>
  Ast mod_node;
  auto decl_begin = c.Iter();
  if (c.IsModifierKeyword()) {
    auto mod_result = ParseModifiers(c);
    if (!mod_result) return mod_result;
    mod_node = move(mod_result.value().ast);
    c.Advance(mod_result.value().head);
  } else {
    // No modifiers found.
    mod_node = Ast(eAst::kModifiers, c.Iter(), c.Iter());
  }

  // Expect a fn keyword
  if (c.TypeIsnt(kKwFn)) return DEBUG_FAIL("Expected <kw-def>.");
  c.Advance();

  // Expecting a commercial @
  if (c.TypeIsnt(kCommercialAt)) return DEBUG_FAIL("Expected <@>.");
  c.Advance();

  // Expecting an identifier.
  if (c.TypeIsnt(kIdent)) return DEBUG_FAIL("Expected <identifier>.");
  Ast ident_node = c;
  c.Advance();

  Ast sig_node;
  // Expecting a signature or colon/semicolon.
  if (c.TypeIsnt(kColon) && c.TypeIsnt(kSemicolon)) {
    auto sig_result = ParseMethodSignature(c);
    if (!sig_result) return sig_result;
    sig_node = move(sig_result.value().ast);
    c.Advance(sig_result.value().head);
    if (c.Peek(-1).TypeIs(kColon)) c.Advance(-1);  // !!SPECIAL CASE:
    // Signature will consume and advance past the colon or semicolon.
    // This is to allow checking if the signature is a decl or
    // definition within the signature parsing method.
    // So after getting signature, advance backwards 1 token
    // but only if that token is a colon!!
  } else {
    sig_node = Ast(eAst::kMethodSignature, c.Iter(), c.Iter());
  }

  // If there is a colon, this is a Definition.
  // If there is a semicolon, this is a Declaration.
  if (c.TypeIs(kColon)) {
    c.Advance();
    auto def_result = ParseMethodDef(c);
    if (!def_result) return def_result;

    Ast def_node = def_result.value().ast;
    c.Advance(def_result.value().head);
    // Expect semicolon
    if (c.TypeIsnt(kSemicolon)) return DEBUG_FAIL("Expected <:>.");
    c.Advance();
    return LLParserResult(
        c, Ast(eAst::kMethodDeclaration, decl_begin, c.Iter(), nullptr, {mod_node, ident_node, sig_node, def_node}));

  } else if (c.TypeIs(kSemicolon)) {
    c.Advance();
    return LLParserResult(
        c, Ast(eAst::kMethodDeclaration, decl_begin, c.Iter(), nullptr, {mod_node, ident_node, sig_node}));

  } else {
    return DEBUG_FAIL("Expected <:> or <;>.");
  }
}

CND_CX LLPrsResT ParseClassDecl(TkCursorT c) CND_NX {
  using enum eTk;
  // Format:
  // <modifiers?><class><commercial_at?><name?><colon?>
  // <class_definition?> <semicolon>
  Ast mod_node;
  auto decl_begin = c.Iter();
  if (c.IsModifierKeyword()) {
    auto mod_result = ParseModifiers(c);
    if (!mod_result) return mod_result;
    mod_node = move(mod_result.value().ast);
    c.Advance(mod_result.value().head);
  } else {
    // No modifiers found.
    mod_node = Ast(eAst::kModifiers, c.Iter(), c.Iter());
  }

  if (c.TypeIsnt(kKwClass)) return DEBUG_FAIL("Expected <kw-class>.");
  c.Advance();

  if (c.TypeIsnt(kCommercialAt)) return DEBUG_FAIL("Expected <@>.");
  c.Advance();

  if (c.TypeIsnt(kIdent)) return DEBUG_FAIL("Expected <identifier>.");
  Ast ident_node = c;
  c.Advance();

  // If there is a colon, this is a Definition.
  // If there is a semicolon, this is a Declaration.
  if (c.TypeIs(kColon)) {
    c.Advance();
    auto def_result = ParseClassDef(c);
    if (!def_result) return def_result;

    c.Advance(def_result.value().head);
    return LLParserResult(c, Ast(eAst::kClassDeclaration, decl_begin, c.Iter(), nullptr,
                                 {mod_node, ident_node, move(def_result.value().ast)}));
  } else if (c.TypeIs(kSemicolon)) {
    c.Advance();
    return LLParserResult(c, Ast(eAst::kClassDeclaration, decl_begin, c.Iter(), nullptr, {mod_node, ident_node}));
  } else
    return DEBUG_FAIL("Expected <:> or <;>.");
}

CND_CX LLPrsResT ParseImportDecl(TkCursorT c) CND_NX {
  using enum eTk;
  auto stmt_begin = c.Iter();
  // Format: <import><name><semicolon>
  if (c.TypeIsnt(kKwImport)) return DEBUG_FAIL("Expected <ident>.");
  c.Advance();

  // Expecting an identifier.
  if (not c.TypeIs(kIdent)) return DEBUG_FAIL("Expected <ident>.");
  Ast ident_node = c;
  c.Advance();

  // Expecting a semicolon.
  if (c.TypeIsnt(kSemicolon)) return DEBUG_FAIL("Expected <:>.");
  c.Advance();
  return LLParserResult(c, Ast(eAst::kImportDeclaration, stmt_begin, c.Iter(), nullptr, {ident_node}));
}

CND_CX LLPrsResT ParseLibDecl(TkCursorT c) CND_NX {
  using enum eTk;
  // Format:
  // <modifiers?><lib><commercial_at?><name?><colon?>
  // <lib_definition?> <semicolon>
  Ast mod_node;  // Check if there are any modifiers.
  auto decl_begin = c.Iter();
  if (c.IsModifierKeyword()) {
    auto mod_result = ParseModifiers(c);
    if (!mod_result) return mod_result;
    mod_node = move(mod_result.value().ast);
    c.Advance(mod_result.value().head);
  } else {
    // No modifiers found.
    mod_node = Ast(eAst::kModifiers, c.Iter(), c.Iter());
  }

  if (c.TypeIsnt(kKwLib)) return DEBUG_FAIL("Expected <kw-lib>.");

  // Expect a lib keyword

  c.Advance();
  // If there is a colon, this is an unnamed library.
  // Expect a definition ending in a semicolon.
  if (c.TypeIs(kColon)) {
    c.Advance();
    auto def_result = ParseLibDef(c);
    if (!def_result) return def_result;

    c.Advance(def_result.value().head);
    return LLParserResult(
        c, Ast(eAst::kLibraryDeclaration, decl_begin, c.Iter(), {move(mod_node), move(def_result.value().ast)}));
  }

  // If there is a @ following the lib keyword, this is a named library.
  // Expecting a commercial @ followed by an identifier.
  if (c.TypeIsnt(kCommercialAt)) DEBUG_FAIL("Expected <@>.");
  c.Advance();

  // Expecting an identifier.
  if (c.TypeIsnt(kIdent)) DEBUG_FAIL("Expected <identifier>.");
  Ast ident_node{c};
  c.Advance();

  if (c.TypeIs(kSemicolon)) {
    c.Advance();
    return LLParserResult(c, Ast(eAst::kLibraryDeclaration, decl_begin, c.Iter(), {mod_node, ident_node}));
  }

  // Expecting a colon and a definition.
  if (c.TypeIsnt(kColon)) DEBUG_FAIL("Expected <:>.");
  c.Advance();

  auto def_result = ParseLibDef(c);
  if (!def_result) return def_result;

  Ast def_node = move(def_result.value().ast);
  c.Advance(def_result.value().head);
  return LLParserResult(c, Ast(eAst::kLibraryDeclaration, decl_begin, c.Iter(), {mod_node, ident_node, def_node}));
}

CND_CX LLPrsResT ParseMainDecl(TkCursorT c) CND_NX {
  using enum eTk;
  // Format:
  // <main><commerical_at?><name?><function_signature><colon?><definition?>
  // <semicolon>

  // Expect a main keyword
  if (c.TypeIsnt(kKwMain)) return DEBUG_FAIL("Expected <kw-main>.");

  auto decl_begin = c.Iter();
  c.Advance();

  // If there is a commercial at following the main keyword, this is a named
  // main. Expecting a commercial @ followed by an identifier.
  if (c.TypeIs(kCommercialAt)) {
    return DEBUG_FAIL("Named main not implemented.");
  } else {
    // This is an unnamed main.
    // Expecting a function signature followed by a colon and a definition.
    // Expecting a colon.
    Ast sig_node;
    if (c.TypeIs(kColon)) {
      sig_node = Ast(eAst::kMethodSignature);
      c.Advance();
    } else {
      auto sig_result = ParseMethodSignature(c);
      if (!sig_result) return sig_result;
      sig_node = sig_result.value().ast;
      c.Advance(sig_result.value().head);
    }

    // Expecting a definition.
    auto def_result = ParseMainDef(c);
    if (!def_result) return def_result;
    Ast def_node = def_result.value().ast;
    c.Advance(def_result.value().head);

    // Expect semicolon
    if (c.TypeIsnt(kSemicolon)) return DEBUG_FAIL("Expected <;>.");
    c.Advance();
    return LLParserResult(c, Ast(eAst::kMainDeclaration, decl_begin, c.Iter(), {sig_node, def_node}));
  }
}

CND_CX LLPrsResT ParseMethodParameters(TkCursorT c) CND_NX {
  using enum eTk;
  auto params_begin = c.Iter();
  CND_CX LAMBDA xFindCommercialAt = [](TkConstIterT beg, TkConstIterT end) -> bool {
    TkCursorT c(beg, end);
    while (c.TypeIsnt(kCommercialAt)) {
      if (!c.AtEnd())
        c.Advance();
      else
        return false;
    }
    return true;
  };

  if (c.TypeIsnt(kLParen)) DEBUG_FAIL("Expected <(>.");

  // Check for empty arguments.
  if (c.Next().TypeIs(kRParen)) {
    c.Advance(2);
    auto params_end = c.Iter();
    return LLParserResult(c, Ast(eAst::kMethodParameterList, params_begin, params_end,
                                 {Ast(eAst::kMethodParameter, Ast(eAst::kMethodVoid))}));
  }

  auto arg_scopes = FindSeperatedParen(c.Iter(), c.End(), kComma);
  if (!arg_scopes) return LLPrsResT::Failure(arg_scopes.error());

  // For each arg:
  // - If there is no @ then it is an identifier of type any -> Expecting an identifier.
  // - If there is an @ then it is a typed argument -> Expect possible mods, followed a primaryPreIdentifier.
  Ast param_list(eAst::kMethodParameterList);
  for (const auto& scope : arg_scopes.value()) {
    Ast param(eAst::kMethodParameter);
    c.Advance(scope.ContainedBegin());
    auto param_begin = c.Iter();
    if (scope.ContainedBegin()->TypeIs(kIdent)) {
      param_list.PushBack(Ast(eAst::kMethodParameter, param_begin, c.Iter(), Ast(eAst::kModifiers, Ast(eAst::kNONE)),
                              Ast(eAst::kKwAny), Ast(scope.ContainedBegin())));
    } else if (xFindCommercialAt(scope.ContainedBegin(), scope.ContainedEnd())) {
      // Modifiers
      if (c.IsModifierKeyword()) {
        auto mods = ParseModifiers(c);
        if (!mods) return mods;
        c.Advance(mods.value().head);
        param.PushBack(move(mods.value().ast));
      } else {
        param.PushBack(Ast(eAst::kModifiers, c.Iter(), c.Iter()));
      }

      // Type
      if (c.IsPrimary()) {
        auto paramtype = ParsePrimaryPreIdentifier(c);
        if (!paramtype) return paramtype;

        c.Advance(paramtype.value().head.Iter());
        param.PushBack(move(paramtype.value().ast));
      } else if (c.TypeIs(kCommercialAt)) {
        c.Advance();
        // Implied any type.
        param.PushBack(eAst::kKwAny);
      } else
        return DEBUG_FAIL("Expected <primary-expr>.");

      // Identity
      if (c.TypeIs(kIdent)) {
        param.PushBack(c);
      } else {
        return DEBUG_FAIL("Expected <identifier>.");
      }

      // Add to list
      param_list.PushBack(move(param));
    } else {
      c.Advance(scope.ContainedBegin());
      return DEBUG_FAIL("Expected <identifier>.");
    }
  }
  return LLParserResult(c.Advance(arg_scopes.value().back().End()), move(param_list));
}

CND_CX LLPrsResT ParseMethodReturnParameters(TkCursorT c) CND_NX {
  Ast method_return_type_node{eAst::kMethodReturnType};
  // Expect any number of modifiers.
  if (c.IsModifierKeyword()) {
    auto method_mods_result = ParseModifiers(c);
    if (not method_mods_result) {
      return method_mods_result;
    }
    c.Advance(method_mods_result.value().head);
    method_return_type_node.PushBack(move(method_mods_result.value().ast));
  }

  // Expecting a primary expression ending in a colon.
  if (c.IsPrimary()) {
    auto ret_type_result = ParsePrimaryPostIdentifier(c);
    if (not ret_type_result) {
      return ret_type_result;
    }
    c.Advance(ret_type_result.value().head);
    method_return_type_node.PushBack(move(ret_type_result.value().ast));
  } else
    return DEBUG_FAIL("Expected <primary-expr>.");

  return LLParserResult(c, move(method_return_type_node));
}

CND_CX LLPrsResT ParseMethodSignature(TkCursorT c) CND_NX {
  using enum eAst;
  using namespace detail;
  CND_CX LAMBDA xMake1ParamSigAst = [](Ast ret, Ast param) {
    return Ast(kMethodSignature, Ast(kMethodParameterList, Ast(kMethodParameter, param)), Ast(kMethodReturnType, ret));
  };

  CND_CX LAMBDA xMakeSigAst = [](Ast ret_type_ast, Ast param_list_ast) {
    return Ast(kMethodSignature, param_list_ast, ret_type_ast);
  };

  // Colon after identifier -> Implicit void arg, no return.
  if (c.TypeIs(eTk::kColon)) return LLParserResult(c, xMake1ParamSigAst(kMethodVoid, kMethodVoid));
  // GreaterThan after identifier -> Some sort of void arg with a return.
  else if (c.TypeIs(eTk::kGt)) {
    c.Advance();
    // Implicit any return void method.
    if (c.TypeIs(eTk::kColon)) return LLParserResult(c.Advance(), xMake1ParamSigAst(kKwAny, kMethodVoid));
    if (c.TypeIs(eTk::kSemicolon)) return LLParserResult(c, xMake1ParamSigAst(kKwAny, kMethodVoid));

    // Parse the return type.
    auto return_type_result = ParseMethodReturnParameters(c);
    if (!return_type_result) return return_type_result;

    // Return type must end in a colon. If it doesn't, error above.
    c.Advance(return_type_result.value().head);
    return LLParserResult(c, xMakeSigAst(return_type_result.Extract().head,
                                         Ast{kMethodParameterList, Ast{kMethodParameter, Ast{kMethodVoid}}}));
  }
  // Open Paren After Identifier -> Method with arguments.
  else if (c.TypeIs(eTk::kLParen)) {
    auto method_params_result = ParseMethodParameters(c);
    if (!method_params_result) return method_params_result;

    c.Advance(method_params_result.value().head);
    if (c.TypeIs(eTk::kSemicolon)) {
      return LLParserResult(c, xMakeSigAst(Ast(kMethodReturnType, Ast(kMethodVoid)), method_params_result.value().ast));
    }

    // Expecting a colon or a greater than.
    if (c.TypeIs(eTk::kColon)) {
      c.Advance();
      return LLParserResult(c, xMakeSigAst(Ast(kMethodReturnType, Ast(kMethodVoid)), method_params_result.value().ast));
    } else if (c.TypeIs(eTk::kGt)) {
      c.Advance();
      // if the next token is a colon, then the return type is any.
      if (c.TypeIs(eTk::kColon)) {
        c.Advance();
        return LLParserResult(c, xMakeSigAst(Ast(kMethodReturnType, Ast(kKwAny)), method_params_result.value().ast));
      }

      if (c.TypeIs(eTk::kSemicolon)) {
        return LLParserResult(c, xMakeSigAst(Ast(kMethodReturnType, Ast(kKwAny)), method_params_result.value().ast));
      }

      auto return_type_result = ParseMethodReturnParameters(c);
      if (!return_type_result) return return_type_result;

      c.Advance(return_type_result.value().head);
      return LLParserResult(c, xMakeSigAst(return_type_result.Extract().ast, method_params_result.Extract().ast));
    } else {
      return DEBUG_FAIL("Expected <:>.");
    }
  } else {
    return DEBUG_FAIL("Expected <:> or <>> or <(>.");
  }
}

CND_CX LLPrsResT ParseMethodDef(TkCursorT c) CND_NX {
  using namespace detail;
  Ast node{eAst::kMethodDefinition};
  auto stmt = FindBrace(c);
  if (!stmt) return LLPrsResT::Failure(stmt.error());
  c.Advance();

  while (c.Iter() != stmt.value().ContainedEnd()) {
    if (c.IsPragmatic()) {
      auto decl = ParseFunctionalStmt(c);
      if (!decl) return decl;
      ExtractAndAdvance(c, node, decl);
    } else if (c.IsPrimary()) {
      auto expr = ParsePrimaryStatement(c);
      if (!expr) return expr;
      ExtractAndAdvance(c, node, expr);
    } else {
      return DEBUG_FAIL("Expected <pragmatic-decl>.");
    }
  }
  c.Advance();

  return LLParserResult(c, move(node));
}

CND_CX LLPrsResT ParseMainDef(TkCursorT c) CND_NX {
  using enum eTk;
  using namespace detail;
  Ast node{eAst::kMainDefinition};
  auto stmt = FindBrace(c);
  if (!stmt) return LLPrsResT::Failure(stmt.error());

  c.Advance();

  while (c.Iter() != stmt.value().ContainedEnd()) {
    if (c.IsPragmatic()) {
      auto decl = ParseFunctionalStmt(c);
      if (!decl) return decl;
      ExtractAndAdvance(c, node, decl);
    } else if (c.IsPrimary()) {
      auto expr = ParsePrimaryStatement(c);
      if (!expr) return expr;
      ExtractAndAdvance(c, node, expr);
    } else {
      return DEBUG_FAIL("Expected <pragmatic-decl> or <primary-expr>.");
    }
  }
  c.Advance();

  return LLParserResult(c, move(node));
}

CND_CX LLPrsResT ParseClassDef(TkCursorT c) CND_NX {
  using enum eTk;
  using namespace detail;
  Ast node{eAst::kClassDefinition};
  auto stmt = FindBrace(c);
  if (!stmt) return LLPrsResT::Failure(stmt.error());

  c.Advance();

  while (c.Iter() != stmt.value().ContainedEnd()) {
    if (c.IsPragmatic()) {
      auto decl = ParsePragmaticStmt(c);
      if (!decl) return decl;
      ExtractAndAdvance(c, node, decl);
    } else {
      return DEBUG_FAIL("Expected <pragmatic-decl>.");
    }
  }
  c.Advance();

  if (c.TypeIs(kSemicolon))
    c.Advance();
  else
    return LLParserResult(c, move(node));

  return LLParserResult(c, move(node));
}

CND_CX LLPrsResT ParseLibDef(TkCursorT c) CND_NX {
  using enum eTk;
  using namespace detail;
  Ast node{eAst::kLibraryDefinition};
  auto stmt = FindBrace(c);
  if (!stmt) return LLPrsResT::Failure(stmt.error());
  c.Advance();

  while (c.Iter() != stmt.value().ContainedEnd()) {
    if (c.IsPragmatic()) {
      auto decl = ParsePragmaticStmt(c);
      if (!decl) return decl;
      ExtractAndAdvance(c, node, decl);
    } else {
      return DEBUG_FAIL("Expected <pragmatic-decl>.");
    }
  }
  c.Advance();

  if (c.TypeIs(kSemicolon))
    c.Advance();
  else
    return LLParserResult(c, move(node));

  return LLParserResult(c, move(node));
}

CND_CX LLPrsResT ParsePragmaticStmt(TkCursorT c) CND_NX {
  using enum eTk;

  // Format: <modifiers?><decl keyword>
  if (c.TypeIs(kSemicolon)) return DEBUG_FAIL("Empty statement in code.");

  // Check for keywords which don't use modifiers.
  if (c.IsDeclarativeKeyword()) {
    switch (c.Type()) {
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
  while (c.IsModifierKeyword()) c.Advance();

  // Check for all keywords which are valid pragmatic declarations.
  if (c.IsDeclarativeKeyword()) {
    switch (c.Type()) {
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

CND_CX LLPrsResT ParseIncludeStmt(TkCursorT c) CND_NX { return DEBUG_FAIL("Not implemented"); }
CND_CX LLPrsResT ParsePragmaStmt(TkCursorT c) CND_NX { return DEBUG_FAIL("Not implemented"); }
CND_CX LLPrsResT ParseSwitchStmt(TkCursorT c) CND_NX { return DEBUG_FAIL("Not implemented"); }

// <directive_desc> ::= <directive_stmt>* | <directive_block>
// <directive_block> ::= <LEFT_CURLY_BRACKET> <IGNORED> <directive_stmt_list> <IGNORED> <RIGHT_CURLY_BRACKET>
// <directive_stmt_list> ::= <directive_stmt>*
// <directive_stmt> ::= <primary_stmt>
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
CND_CX LLPrsResT ParsePragmaticDesc(TkCursorT c) CND_NX {
  using enum eTk;
  // Error on empty statement early
  if (c.TypeIs(kSemicolon)) return DEBUG_FAIL("Empty statement in code. Unnecessary semicolon ';'.");

  // Check if there is an unnamed scope. Override possible left brace in primary expressions,
  // and recursivley call this method to parse contained statements.
  if (c.TypeIs(eTk::kLBrace)) {
    Ast scope_node{eAst::kUnnamedScope};
    while (c.TypeIsnt(eTk::kRBrace) || c.AtEnd()) {
      auto sub_description = ParsePragmaticDesc(c);
      if (!sub_description) return sub_description;
      detail::ExtractAndAdvance(c, scope_node, sub_description);
    }
    if (c.AtEnd()) return DEBUG_FAIL("Unclosed unnamed scope. Closing brace '}' not found.");
    c.Advance();
    if (c.TypeIs(kSemicolon)) return DEBUG_FAIL("Empty statement in code. Unnecessary semicolon ';'.");
    return LLParserResult{c, scope_node};
  }

  /////////////////////////////////////////////////////////
  // From here we are parsing a single statement...
  /////////////////////////////////////////////////////////
  if (c.IsPrimary()) {
    return ParsePrimaryStatement(c);
  }

  // Parse a valid directive keyword statement.
  // Check for keywords which don't allow any modifiers.
  switch (c.Type()) {
    case kKwUse:
      return ParseUsingDecl(c);
    case kKwMain:
      return ParseMainDecl(c);
    case kKwImport:
      return ParseImportDecl(c);
    case kKwEnum:
      return ParseEnumDecl(c);
    default:
      break;
  }

  // Store the initial statement begin location.
  // Skip any modifiers to determine the parsing method to call based on declarative keyword.
  // Parse from decl begin to allow called method to handle modifiers.
  TkCursorT decl_begin = c;
  while (c.IsModifierKeyword()) c.Advance();
  switch (c.Type()) {
    case kKwDef:
      return ParseVariableDecl(decl_begin);
    case kKwFn:
      return ParseMethodDecl(decl_begin);
    case kKwClass:
      return ParseClassDecl(decl_begin);
    case kKwUse:
    case kKwMain:
    case kKwImport:
    case kKwEnum:
    case kKwReturn:
      return DEBUG_FAIL("Declaration keyword cannot be modified.");
    default:
      return DEBUG_FAIL("Declaration keyword not permitted inside pragmatic code block.");
  }
}

// <process_def> ::= <KW_PROC> <IGNORED> <AT_SIGN> <IDENTIFIER> <IGNORED> <COLON> <IGNORED>
//									<LEFT_CURLY_BRACKET> <IGNORED> <process_desc>
//<IGNORED> <RIGHT_CURLY_BRACKET>
// <process_unnamed_def> ::= <KW_PROC> <IGNORED> <COLON> <IGNORED>
//									<LEFT_CURLY_BRACKET> <IGNORED> <process_desc>
//<IGNORED> <RIGHT_CURLY_BRACKET>
// <process_desc> ::= (<process_stmt> | <process_block>)*
// <process_block> ::= <LEFT_CURLY_BRACKET> <IGNORED> <process_stmt>* <IGNORED> <RIGHT_CURLY_BRACKET>
// <process_stmt> ::= <import> | <main> | <pragma> | <primary> | <variable> | <function> | <struct>
//				        | <namespace> | <using> | <enum>
CND_CX LLPrsResT ParseProcDef(TkCursorT c) CND_NX {
  using enum eTk;
  Ast process_node{eAst::kProcessDefinition};

  // Process def will either be a single statement or a block.
  if (c.TypeIs(kSemicolon)) return DEBUG_FAIL("Empty process definition.");

  // Process block
  if (c.TypeIs(kLBrace)) {
    c.Advance();
    while (!c.AtEnd() || c.TypeIs(kRBrace)) {
      if (c.IsPragmaticFirstSet()) {
        auto pragmatic_desc = ParsePragmaticDesc(c);
        if (!pragmatic_desc) return pragmatic_desc;
        detail::ExtractAndAdvance(c, process_node, pragmatic_desc);
      } else
        return DEBUG_FAIL("Expected a pragmatic description at the process level.");
    }
    // eof before closing brace
    if (c.AtEnd()) return DEBUG_FAIL("Expected closing brace.");
    c.Advance();
    return LLParserResult{c, process_node};
  }

  // Single statement process
  if (c.IsPragmaticFirstSet()) {
    auto pragmatic_desc = ParsePragmaticDesc(c);
    if (!pragmatic_desc) return pragmatic_desc;
    detail::ExtractAndAdvance(c, process_node, pragmatic_desc);
  } else
    return DEBUG_FAIL("Expected a pragmatic description at the process level.");

  return LLParserResult(c, process_node);
}

// <process> ::= <process_decl> | <process_def>
// <process_decl> ::= <KW_PROC> <IGNORED> <AT_SIGN> <IDENTIFIER> <IGNORED> <SEMICOLON>
CND_CX LLPrsResT ParseProcDecl(TkCursorT c) CND_NX {
  using enum eTk;
  // Check if there are any modifiers.
  Ast mod_node;
  auto decl_begin = c.Iter();
  if (c.IsModifierKeyword()) {
    auto mod_result = ParseModifiers(c);
    if (!mod_result) return mod_result;
    mod_node = move(mod_result.value().ast);
    c.Advance(mod_result.value().head);
  }
  // No modifiers found. Create empty modifiers node.
  else {
    mod_node = Ast(eAst::kModifiers, c.Iter(), c.Iter());
  }

  if (c.TypeIsnt(kKwProc)) return DEBUG_FAIL("Expected <kw-proc>.");
  c.Advance();

  // If there is a colon, this is an unnamed process. Unamed processes must be defined inline.
  // Expect a definition ending in a semicolon.
  if (c.TypeIs(kColon)) {
    c.Advance();
    auto def_result = ParseProcDef(c);
    if (!def_result) return def_result;
    c.Advance(def_result->head);
    return LLParserResult(c, Ast(eAst::kProcessDeclaration, decl_begin, c.Iter(), {mod_node, def_result->ast}));
  }

  // If there is a @ following the proc keyword, this is a named library.
  if (c.TypeIsnt(kCommercialAt)) DEBUG_FAIL("Expected '@' symbol after 'proc' keyword.");
  c.Advance();
  if (c.TypeIsnt(kIdent)) DEBUG_FAIL("Expected identifier after '@' symbol.");
  Ast ident_node{c};
  c.Advance();

  // Library declaration
  if (c.TypeIs(kSemicolon)) {
    c.Advance();
    return LLParserResult(c, Ast(eAst::kLibraryDeclaration, decl_begin, c.Iter(), {mod_node, ident_node}));
  }

  if (c.TypeIsnt(kColon)) DEBUG_FAIL("Expected ':' symbol followed by process definition.");
  c.Advance();

  auto def_result = ParseProcDef(c);
  if (!def_result) return def_result;
  c.Advance(def_result->head);
  return LLParserResult(c,
                        Ast(eAst::kLibraryDeclaration, decl_begin, c.Iter(), {mod_node, ident_node, def_result->ast}));
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
// <using_library_alias> ::= <KW_USE> <KW_LIB> <AT_SIGN> <IDENTIFIER> <EQUALS_SIGN> <id_expr> <SEMICOLON>
// <using_namespace_alias> ::= <KW_USE> <KW_LIB> <AT_SIGN> <IDENTIFIER> <EQUALS_SIGN> <id_expr> <SEMICOLON>
// <using_function_alias> ::= <KW_USE> <KW_LIB> <AT_SIGN> <IDENTIFIER> <EQUALS_SIGN> <id_expr> <SEMICOLON>
// <using_variable_alias> ::= <KW_USE> <KW_LIB> <AT_SIGN> <IDENTIFIER> <EQUALS_SIGN> <id_expr> <SEMICOLON>
// <using_process_alias> ::= <KW_USE> <KW_LIB> <AT_SIGN> <IDENTIFIER> <EQUALS_SIGN> <id_expr> <SEMICOLON>
// <using_enum_alias> ::= <KW_USE> <KW_LIB> <AT_SIGN> <IDENTIFIER> <EQUALS_SIGN> <id_expr> <SEMICOLON>
//
// <using_library_unscope> ::= <KW_USE> <KW_LIB> <id_expr> <SEMICOLON>
// <using_namespace_unscope> ::= <KW_USE> <KW_NAMESPACE> <id_expr> <SEMICOLON>
// <using_function_unscope> ::= <KW_USE> <KW_FN> <id_expr> <SEMICOLON>
// <using_def_unscope> ::= <KW_USE> <KW_DEF> <id_expr> <SEMICOLON>
// <using_proc_unscope> ::= <KW_USE> <KW_PROC> <id_expr> <SEMICOLON>
// <using_type_unscope> ::= <KW_USE> <id_expr> <SEMICOLON>
// <using_enum_unscope> ::= <KW_USE> <KW_ENUM> <id_expr> <SEMICOLON>
CND_CX LLPrsResT ParseUsingDecl(TkCursorT c) CND_NX {
  using enum eTk;
  if (c.TypeIsnt(kKwUse)) return DEBUG_FAIL("Expected 'use' keyword.");
  auto stmt_begin = c.Iter();
  c.Advance();

  // - @foo -> Type Alias
  // - @foo : lib -> Library Type Alias.
  if (c.TypeIs(kCommercialAt)) {
    c.Advance();
    if (c.TypeIsnt(kIdent)) DEBUG_FAIL("Expected <ident>.");
    Ast alias = Ast(c);
    c.Advance();
    if (c.TypeIsnt(kColon)) DEBUG_FAIL("Expected <:>.");
    c.Advance();

    // If next is lib, then it is a library type alias.
    // Everything following lib must be a value expression closed by a
    // semicolon. Wether it is reduced to a type is determined at a later
    // stage.
    if (c.TypeIs(kKwLib)) {
      c.Advance();
      auto val_expr = ParsePrimaryStatement(c);
      if (!val_expr) return val_expr;
      c.Advance(val_expr.value().head);
      return LLParserResult(c, {eAst::kLibraryTypeAlias, stmt_begin, c.Iter(), nullptr, {alias, val_expr->ast}});
    }

    // else it is a type alias...
    auto val_expr = ParsePrimaryStatement(c);
    if (!val_expr) return val_expr;
    c.Advance(val_expr.value().head);
    return LLParserResult(c, {eAst::kTypeAlias, stmt_begin, c.Iter(), nullptr, {alias, val_expr->ast}});
  }

  // - lib-> Library Namespace Inclusion.
  else if (c.TypeIs(kKwLib)) {
    c.Advance();
    auto val_expr = ParsePrimaryStatement(c);
    if (!val_expr) return val_expr;
    c.Advance(val_expr.value().head);
    return LLParserResult(c,
                          {eAst::kLibraryNamespaceInclusion, stmt_begin, c.Iter(), nullptr, {val_expr.Extract().ast}});

  }

  // - namespace-> Namespace Inclusion.
  else if (c.TypeIs(kKwNamespace)) {
    c.Advance();
    auto val_expr = ParsePrimaryStatement(c);
    if (!val_expr) return val_expr;
    c.Advance(val_expr->head);
    return LLParserResult(c, Ast(eAst::kNamespaceInclusion, stmt_begin, c.Iter(), nullptr, {val_expr->ast}));
  }

  // Namespace object inclusion.
  // The primary expr assumed to resolve to an object or function name. Validate at a later stage.
  else if (c.IsPrimary()) {
    auto primary_result = ParsePrimaryStatement(c);
    if (!primary_result) return primary_result;
    c.Advance(primary_result->head);
    return LLParserResult(c,
                          Ast(eAst::kNamespaceObjectInclusion, stmt_begin, c.Iter(), nullptr, {primary_result->ast}));

  }

  // Invalid format.
  else
    return DEBUG_FAIL("Using statement expected <@> or <kw-lib> or <kw-namespace> or <type-expression>.");
};

// <directive_desc> ::= <directive_stmt>* | <directive_block>
// <directive_block> ::= <LEFT_CURLY_BRACKET> <IGNORED> <directive_stmt_list> <IGNORED> <RIGHT_CURLY_BRACKET>
// <directive_stmt_list> ::= <directive_stmt>*
// <directive_stmt> ::= <primary_stmt>
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
CND_CX LLPrsResT ParseDirectiveDesc(TkCursorT c) CND_NX {
  using enum eTk;
  // Error on empty statement early
  if (c.TypeIs(kSemicolon)) return DEBUG_FAIL("Empty statement in code. Unnecessary semicolon ';'.");

  // Check if there is an unnamed scope. Override possible left brace in primary expressions,
  // and recursivley call this method to parse contained statements.
  if (c.TypeIs(eTk::kLBrace)) {
    Ast scope_node{eAst::kUnnamedScope};
    while (c.TypeIsnt(eTk::kRBrace) || c.AtEnd()) {
      auto sub_description = ParseDirectiveDesc(c);
      if (!sub_description) return sub_description;
      detail::ExtractAndAdvance(c, scope_node, sub_description);
    }
    if (c.AtEnd()) return DEBUG_FAIL("Unclosed unnamed scope. Closing brace '}' not found.");
    c.Advance();
    if (c.TypeIs(kSemicolon)) return DEBUG_FAIL("Empty statement in code. Unnecessary semicolon ';'.");
    return LLParserResult{c, scope_node};
  }

  /////////////////////////////////////////////////////////
  // From here we are parsing a single statement...
  /////////////////////////////////////////////////////////
  if (c.IsPrimary()) {
    return ParsePrimaryStatement(c);
  }

  // Parse a valid directive keyword statement.
  // Check for keywords which don't allow any modifiers.
  switch (c.Type()) {
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

  // Store the initial statement begin location.
  // Skip any modifiers to determine the parsing method to call based on declarative keyword.
  // Parse from decl begin to allow called method to handle modifiers.
  TkCursorT decl_begin = c;
  while (c.IsModifierKeyword()) c.Advance();
  switch (c.Type()) {
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
    case kKwImport:
    case kKwEnum:
    case kKwReturn:
      return DEBUG_FAIL("Declaration keyword cannot be modified.");
    default:
      return DEBUG_FAIL("Declaration keyword not permitted inside directive code block.");
  }
}

// <syntax> ::= <directive_desc>
CND_CX LLPrsResT ParseSyntax(TkCursorT c) CND_NX {
  using namespace detail;
  Ast program_node{eAst::kProgram};
  while (!c.AtEnd()) {
    if (c.IsDirectiveFirstSet()) {
      auto directive_desc = ParseDirectiveDesc(c);
      if (!directive_desc) return directive_desc;
      ExtractAndAdvance(c, program_node, directive_desc);
    } else
      return DEBUG_FAIL("Expected a directive description at the top level.");
  }
  return LLParserResult(c, program_node);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CND_CX LLPrsResT ParseEnumDecl(TkCursorT c) CND_NX {
  using enum eTk;
  using namespace detail;
  // Format:
  // <modifiers?> <enum> <commercial_at?><name?><colon?>
  // <<association_typename><<commercial_at><association_identifier>?><semicolon>?>
  // <<commercial_at><enum_entry_name?><semicolon>
  auto decl_begin = c.Iter();
  LLPrsResT mods = ParseOptionalModifiers(c);
  if (!mods) return mods;
  Ast mod_node = mods.value().ast;

  if (c.TypeIsnt(kKwEnum)) return DEBUG_FAIL("Expected <kw-enum>.");

  c.Advance();
  if (c.TypeIsnt(kCommercialAt)) return DEBUG_FAIL("Expected <identifier>.");
  c.Advance();
  if (c.TypeIsnt(kIdent)) return DEBUG_FAIL("Expected <identifier>.");
  Ast ident_node{c};
  c.Advance();

  // If there is a colon, this is a Definition.
  // If there is a semicolon, this is a Declaration.
  if (c.TypeIs(kColon)) {
    c.Advance();
    auto def = ParseEnumDef(c);
    if (!def) return def;
    Ast def_node = ExtractAndAdvance(c, def);
    c.Advance(def.value().head);
    return LLParserResult(c, Ast(eAst::kEnumDeclaration, decl_begin, c.Iter(), mod_node, ident_node, def_node));
  } else if (c.TypeIs(kSemicolon)) {
    c.Advance();
    return LLParserResult(c, Ast(eAst::kEnumDeclaration, decl_begin, c.Iter(), mod_node, ident_node));
  } else {
    return DEBUG_FAIL("Expected <;> or <:>.");
  }
};

CND_CX LLPrsResT ParseEnumDef(TkCursorT c) CND_NX {
  // If followed by an open brace, positional_enum.
  // If followed by anything else,associative_enum.
  // associative_enum -> <type_expr><commercial_at><enum_entry_name?><colon>
  // After each named association, may be an open brace or another association.

  using enum eTk;
  using namespace detail;
  Ast node{eAst::kEnumDefinition};
  // Parse enum header.
  while (c.TypeIsnt(kLBrace)) {
    Ast assoc_node{eAst::kEnumAssociation};
    auto typeexpr = ParsePrimaryPreIdentifier(c);
    if (!typeexpr) return typeexpr;
    ExtractAndAdvance(c, assoc_node, typeexpr);
    if (c.TypeIsnt(kIdent)) DEBUG_FAIL("Expected <identifier>.");
    assoc_node.PushBack(c);
    c.Advance();
    if (c.TypeIs(kColon)) DEBUG_FAIL("Expected <;> or <:>.");
    c.Advance();
  }

  // Parse enum definition block.
  auto block = ParseEnumBlock(c);
  if (!block) return block;
  ExtractAndAdvance(c, node, block);

  return LLParserResult(c, move(node));
};

CND_CX LLPrsResT ParseEnumBlock(TkCursorT c) CND_NX {
  // Parse inside an enum definition block.
  // Cursor should begin at the open brace of the block.

  using enum eTk;
  using namespace detail;
  // LAMBDA xParseAssociationInitializers = [](TkCursorT& crsr, Ast& entry) -> LLPrsResT {
  //   while (crsr.TypeIsnt(kSemicolon)) {
  //     if (crsr.TypeIsnt(kColon)) return DEBUG_FAIL("Expected <:>.");
  //     crsr.Advance();
  //     auto next_assoc = ParsePrimaryPostIdentifier(crsr);
  //     if (!next_assoc) return next_assoc;
  //     crsr.Advance(next_assoc->head);
  //     entry.PushBack(move(next_assoc->ast));
  //   }
  //   crsr.Advance();
  // };

  Ast node{eAst::kEnumBlock};
  if (c.TypeIsnt(kLBrace))  // Require opening brace.
    return DEBUG_FAIL("Expected <{>.");
  c.Advance();
  while (c.TypeIsnt(kRBrace)) {
    // EnumEntry
    if (c.TypeIs(kCommercialAt)) {
      c.Advance();
      if (c.TypeIsnt(kIdent)) DEBUG_FAIL("Expected <identifier>.");
      node.PushBack({eAst::kEnumEntry, c.Iter(), c.Next().Iter()});
      auto& this_entry = node.children.back();
      c.Advance();
      // Check for association initializers. Expect a colon or semicolon.
      while (c.TypeIsnt(kSemicolon)) {
        if (c.TypeIsnt(kColon)) return DEBUG_FAIL("Expected <:>.");
        c.Advance();
        auto next_assoc = ParsePrimaryPostIdentifier(c);
        if (!next_assoc) return next_assoc;
        c.Advance(next_assoc->head);
        this_entry.PushBack(next_assoc->ast);
      }
      c.Advance();
      // xParseAssociationInitializers(c, this_entry);
    }
    // EnumCategory
    else if (c.TypeIs(kKwUse)) {
      c.Advance();
      Vec<TkConstIterT> this_category{};
      // Consume identifiers until a semicolon is reached.
      while (c.TypeIsnt(kColon)) {
        if (c.TypeIsnt(kCommercialAt)) return DEBUG_FAIL("Expected <@>.");
        c.Advance();
        if (c.TypeIsnt(kIdent)) return DEBUG_FAIL("Expected <identifier>.");
        this_category.push_back(c.Iter());
        c.Advance();
      }
      c.Advance();  // pass colon

      // expect the enum entry for a singular entry or a block for multi entry.
      if (c.TypeIs(kCommercialAt)) {
        c.Advance();
        if (c.TypeIsnt(kIdent)) return DEBUG_FAIL("Expected <identifier>.");
        node.PushBack({eAst::kEnumEntry, c.Iter(), c.Next().Iter()});
        auto& this_entry = node.children.back();

        // Attribute this category to this entry.
        this_entry.PushBack(eAst::kEnumCategory);
        auto& added_category = this_entry.children.back();
        for (auto& tk_iter : this_category) added_category.PushBack(tk_iter);

        // Check for association initializers. Expect a colon or semicolon.
        while (c.TypeIsnt(kSemicolon)) {
          if (c.TypeIsnt(kColon)) return DEBUG_FAIL("Expected <:>.");
          c.Advance();
          auto next_assoc = ParsePrimaryPostIdentifier(c);
          if (!next_assoc) return next_assoc;
          c.Advance(next_assoc->head);
          this_entry.PushBack(move(next_assoc->ast));
        }
        c.Advance();
      }
      // a block
      else if (c.TypeIs(kLBrace)) {
        auto recursed_block_res = ParseEnumBlock(c);
        if (!recursed_block_res) return recursed_block_res;

        // Add this category to all categories of the sub-block
        // as a parent (push to the front), then add the entry
        // to this block.
        auto recursed_block = recursed_block_res.Extract();
        for (auto& entry : recursed_block.ast.children) {
          for (auto& entry_data : entry.children) {
            if (entry_data.TypeIs(eAst::kEnumCategory)) {
              for (auto& name_frag_it : this_category)
                entry_data.children.insert(entry_data.children.begin(), {name_frag_it});
            }
          }
          node.PushBack(entry);
        }
      } else
        return DEBUG_FAIL("Expected <identifier>.");
    }
    // Invalid enum description format.
    else
      return DEBUG_FAIL("Expected <identifier>.");
  }
  c.Advance();

  return LLParserResult(c, move(node));
};

CND_CX ScopePrsResT FindScopeImpl(TkCursorT& crsr) CND_NX {
  auto opening = crsr.Iter();
  int scope_depth = 0;
  Vec<eTk> scope_history;
  // Find the last matching close token that is not within a () [] or {} scope.
  // Start from the token after the open token. All scope depths should be 0
  // at the end of the loop on a valid scope.
  for (auto c = crsr.Next(); c.Iter() != crsr.End(); c.Advance()) {
    if (c.IsOpeningScope()) {
      scope_depth++;
      scope_history.push_back(c.Type());
    } else if (c.IsClosingScope()) {
      if (scope_history.empty())
        if (c.IsClosingScopeOf(crsr.Type()))
          return TkScopeT{true, opening, c.Advance().Iter()};
        else
          return DEBUG_FAIL("Unclosed scope.");

      if (!crsr.IsClosingScopeOf(scope_history.back())) return DEBUG_FAIL("Mismatched scopes.");

      scope_history.pop_back();
      scope_depth--;
    } else
      continue;
  }
  return DEBUG_FAIL("Unclosed scope.");
}

CND_CX ScopePrsResT FindParen(TkCursorT c) CND_NX {
  if (c.AtEnd()) return DEBUG_FAIL("OutOfBounds");  // Out of bounds begin passed to method...
  if (c.TypeIsnt(eTk::kLParen)) return DEBUG_FAIL("Expected opening scope.");  // No open token to start with.
  auto scope_begin = c;
  c.Advance();
  if (c.AtEnd()) return DEBUG_FAIL("kParserOpeningScopeAtEof.");  // End right after open, cannot be closed.
  if (c.TypeIs(eTk::kRParen)) return TkScopeT{true, scope_begin.Iter(), c.Advance().Iter()};  // Empty paren scope '()'
  return FindScopeImpl(scope_begin);
}  // end find_paren

CND_CX ScopePrsResT FindBrace(TkCursorT c) CND_NX {
  if (c.AtEnd()) return DEBUG_FAIL("OutOfBounds");  // Out of bounds begin passed to method...
  if (c.TypeIsnt(eTk::kLBrace)) return DEBUG_FAIL("Expected opening scope.");  // No open token to start with.
  auto scope_begin = c.Iter();
  // c.Advance();
  if (c.AtEnd()) return DEBUG_FAIL("kParserOpeningScopeAtEof.");  // End right after open, cannot be closed.
  if (c.TypeIs(eTk::kRBrace)) return TkScopeT{true, scope_begin, c.Advance().Iter()};  // Empty paren scope '()'
  return FindScopeImpl(c);
}  // end find_paren

CND_CX ScopePrsResT FindBracket(TkCursorT c) CND_NX {
  if (c.AtEnd()) return DEBUG_FAIL("OutOfBounds");  // Out of bounds begin passed to method...
  if (c.TypeIsnt(eTk::kLBracket)) return DEBUG_FAIL("Expected opening scope.");  // No open token to start with.
  auto scope_begin = c.Iter();
  c.Advance();
  if (c.AtEnd()) return DEBUG_FAIL("kParserOpeningScopeAtEof.");  // End right after open, cannot be closed.
  if (c.TypeIs(eTk::kRBracket)) return TkScopeT{true, scope_begin, c.Advance().Iter()};  // Empty paren scope '()'
  return FindScopeImpl(c);
}  // end find_paren

CND_CX LRPrsResT ParseExpr(TkCursorT c) CND_NX { return DEBUG_FAIL("NOT IMPLEMENTED"); }

CND_CX LLPrsResT ParsePrimaryExpr(TkCursorT c) CND_NX {
  if (c.IsAnOperand() || c.IsSingularPrefixOperator() || c.IsOpeningScope())
    return ParseListFold(c);
  else
    DEBUG_FAIL("Unexpected token at start of primary expression.");
}

namespace detail {
CND_CX LLPrsResT ParseGenericBinaryLeftAssociative(TkCursorT c, bool (*next_cond)(const TkCursorT&),
                                                   LLPrsResT (*operand_parser)(TkCursorT))
    CND_NX(CND_CLDEV_DEBUG_MODE) {
  using std::move;

  if (!IsTkPrimarySpecifier(c.Type())) return DEBUG_FAIL("Unexpected token at start of binary access.");

  auto first_op = operand_parser(c);
  if (!first_op) return first_op;
  c.Advance(first_op->head);

  Vec<Ast> accum_rhs{};
  while (next_cond(c)) {
    accum_rhs.emplace_back(c);
    c.Advance();
    auto next_rhs = operand_parser(c);
    if (!next_rhs) return next_rhs;
    c.Advance(next_rhs->head);
    accum_rhs.push_back(next_rhs->ast);
  }

  // Associativity Left -> Right.
  // Note the accum vector holds (operand -> operator) pairs to keep track of the specific operator type.
  Vec<Ast> reduced_binop{};
  reduced_binop.push_back(move(first_op->ast));
  for (auto it = accum_rhs.cbegin(); it != accum_rhs.cend(); it++) {
    auto& prev_op = reduced_binop.back();
    Ast next_binop = *it;
    it++;
    next_binop.src_begin = prev_op.src_begin;
    next_binop.PushBack(move(prev_op));
    next_binop.src_end = it->src_end;
    next_binop.PushBack(*it);
    reduced_binop.pop_back();
    reduced_binop.push_back(next_binop);
  }
  accum_rhs.clear();

  CND_DEBUG_ASSERT(reduced_binop.size() == 1,
                   "[At the of reduction, reduced production vector must contain a single ast node.]");

  return LLParserResult{c.Advance(reduced_binop.back().src_end), reduced_binop.back()};
};
}  // namespace detail

CND_CX LLPrsResT ParseListFold(TkCursorT c) CND_NX {
  return detail::ParseGenericBinaryLeftAssociative(
      c, [](const auto& a) { return a.TypeIs(eTk::kComma); }, ParseAssignment);
};

// [R->L] <assignment> ::= (<logical_or> <EQUALS_SIGN>)* <logical_or>
CND_CX LLPrsResT ParseAssignment(TkCursorT c) CND_NX {
  using std::make_unique;
  using std::move;
  using std::next;

  if (!IsTkPrimarySpecifier(c.Type())) return DEBUG_FAIL("Unexpected token at start of binary access.");

  auto first_op = ParseLogicalOr(c);
  if (!first_op) return first_op;
  c.Advance(first_op->head);

  Vec<Ast> accum_rhs{move(first_op->ast)};
  while (c.TypeIs(eTk::kAssign)) {
    accum_rhs.emplace_back(c);
    c.Advance();
    auto next_rhs = ParseLogicalOr(c);
    if (!next_rhs) return next_rhs;
    c.Advance(next_rhs->head);
    accum_rhs.push_back(move(next_rhs->ast));
  }

  if (accum_rhs.size() == 1) return LLParserResult{c, move(accum_rhs.back())};  // No reduction necessary.

  auto reduced_end = accum_rhs.back().src_end;
  UPtr<Ast> reduced_binop{make_unique<Ast>(move(accum_rhs.back()))};
  UPtr<Ast> rhs_binop{nullptr};
  for (auto it = next(accum_rhs.rbegin()); it != accum_rhs.rend();) {
    Ast& next_binop = *it;
    it++;
    Ast& lhs_operand = *it;
    it++;
    rhs_binop = move(reduced_binop);
    reduced_binop = make_unique<Ast>(move(next_binop));
    reduced_binop->src_begin = lhs_operand.src_begin;
    reduced_binop->PushBack(move(lhs_operand));
    reduced_binop->PushBack(move(*rhs_binop));
    reduced_binop->src_end = reduced_end;
  }
  accum_rhs.clear();

  return LLParserResult{c, *reduced_binop};
};

// [L->R] <logical_or> ::= <logical_and> (<LOGICAL_OR_OPERATOR> <logical_and>)*
CND_CX LLPrsResT ParseLogicalOr(TkCursorT c) CND_NX {
  using std::move;
  return detail::ParseGenericBinaryLeftAssociative(
      c, [](const auto& a) { return a.TypeIs(eTk::kOr); }, ParseLogicalAnd);
};

// [L->R] <logical_and> ::= <bitwise_or> (<LOGICAL_AND_OPERATOR> <bitwise_or>)*
CND_CX LLPrsResT ParseLogicalAnd(TkCursorT c) CND_NX {
  using std::move;
  return detail::ParseGenericBinaryLeftAssociative(
      c, [](const auto& a) { return a.TypeIs(eTk::kAnd); }, ParseBitwiseOr);
};

// [L->R] <bitwise_or> ::= <bitwise_xor> (<VERTICAL_LINE> <bitwise_xor>)*
CND_CX LLPrsResT ParseBitwiseOr(TkCursorT c) CND_NX {
  using std::move;
  return detail::ParseGenericBinaryLeftAssociative(
      c, [](const auto& a) { return a.TypeIs(eTk::kBor); }, ParseBitwiseXor);
};

// [L->R] <bitwise_xor> ::= <bitwise_and> (<CIRCUMFLEX_ACCENT> <bitwise_and>)*
CND_CX LLPrsResT ParseBitwiseXor(TkCursorT c) CND_NX {
  using std::move;
  return detail::ParseGenericBinaryLeftAssociative(
      c, [](const auto& a) { return a.TypeIs(eTk::kXor); }, ParseBitwiseAnd);
};

// [L->R] <bitwise_and> ::= <equality> (<AMPERSAND> <equality>)*
CND_CX LLPrsResT ParseBitwiseAnd(TkCursorT c) CND_NX {
  using std::move;
  return detail::ParseGenericBinaryLeftAssociative(
      c, [](const auto& a) { return a.TypeIs(eTk::kBand); }, ParseEquality);
};

// [L->R] <equality> ::= <comparison> (<EQUALITY_OPERATOR> <comparison>)*
//			  | <comparison> (<NOT_EQUAL_OPERATOR> <comparison>)*
CND_CX LLPrsResT ParseEquality(TkCursorT c) CND_NX {
  using std::move;
  return detail::ParseGenericBinaryLeftAssociative(
      c, [](const auto& a) { return a.TypeIs(eTk::kEq) || a.TypeIs(eTk::kNeq); }, ParseComparison);
};

// [L->R] <comparison> ::= <three_way_equality> (<GREATER_THAN_SIGN> <three_way_equality>)*
//		    | <three_way_equality> (<LESS_THAN_SIGN> <three_way_equality>)*
//		    | <three_way_equality> (<GREATER_THAN_EQUAL_SIGN> <three_way_equality>)*
//		    | <three_way_equality> (<LESS_THAN_EQUAL_SIGN> <three_way_equality>)*
CND_CX LLPrsResT ParseComparison(TkCursorT c) CND_NX {
  using std::move;
  return detail::ParseGenericBinaryLeftAssociative(
      c,
      [](const auto& a) {
        return a.TypeIs(eTk::kGt) || a.TypeIs(eTk::kGte) || a.TypeIs(eTk::kLt) || a.TypeIs(eTk::kLte);
      },
      ParseThreeWayEquality);
};

// [L->R] <three_way_equality> ::= <bitwise_shift> (<SPACESHIP_OPERATOR> <bitwise_shift>)*
CND_CX LLPrsResT ParseThreeWayEquality(TkCursorT c) CND_NX {
  using std::move;
  return detail::ParseGenericBinaryLeftAssociative(
      c, [](const auto& a) { return a.TypeIs(eTk::kSpaceship); }, ParseBitwiseShift);
};

// [L->R] <bitwise_shift> ::= <summation> (<BITWISE_RIGHT_SHIFT> <summation>)*
//			  	  | <summation> (<BITWISE_LEFT_SHIFT> <summation>)*
CND_CX LLPrsResT ParseBitwiseShift(TkCursorT c) CND_NX {
  using std::move;
  return detail::ParseGenericBinaryLeftAssociative(
      c, [](const auto& a) { return a.TypeIs(eTk::kRsh) || a.TypeIs(eTk::kLsh); }, ParseSummation);
};

// [L->R] <summation> ::= <production> (<PLUS_SIGN> <production>)*
//			  | <production> (<HYPHEN_MINUS> <production>)*
CND_CX LLPrsResT ParseSummation(TkCursorT c) CND_NX {
  using std::move;

  if (!IsTkPrimarySpecifier(c.Type())) return DEBUG_FAIL("Unexpected token at start of binary access.");

  auto first_op = ParseProduction(c);
  if (!first_op) return first_op;
  c.Advance(first_op->head);

  Vec<Ast> accum_rhs{};
  while (c.TypeIs(eTk::kAdd) || c.TypeIs(eTk::kSub)) {
    accum_rhs.emplace_back(c);
    c.Advance();
    auto next_rhs = ParseProduction(c);
    if (!next_rhs) return next_rhs;
    c.Advance(next_rhs->head);
    accum_rhs.push_back(next_rhs->ast);
  }

  // Associativity Left -> Right.
  // Note the accum vector holds (operand -> operator) pairs to keep track of the specific operator type.
  Vec<Ast> reduced_binop{};
  reduced_binop.push_back(move(first_op->ast));
  for (auto it = accum_rhs.cbegin(); it != accum_rhs.cend(); it++) {
    auto& prev_op = reduced_binop.back();
    Ast next_binop = *it;
    it++;
    next_binop.src_begin = prev_op.src_begin;
    next_binop.PushBack(move(prev_op));
    next_binop.src_end = it->src_end;
    next_binop.PushBack(*it);
    reduced_binop.pop_back();
    reduced_binop.push_back(next_binop);
  }
  accum_rhs.clear();

  CND_DEBUG_ASSERT(reduced_binop.size() == 1,
                   "[At the of reduction, reduced production vector must contain a single ast node.]");

  return LLParserResult{c.Advance(reduced_binop.back().src_end), reduced_binop.back()};
};

// [L->R] <production> ::= <prefix> (<ASTERISK> <prefix>)*
//				| <prefix> (<SOLIDUS> <prefix>)*
//				| <prefix> (<PERCENT_SIGN> <prefix>)*
CND_CX LLPrsResT ParseProduction(TkCursorT c) CND_NX(CND_CLDEV_DEBUG_MODE) {
  using std::move;

  if (!IsTkPrimarySpecifier(c.Type())) return DEBUG_FAIL("Unexpected token at start of binary access.");

  auto first_op = ParsePrefix(c);
  if (!first_op) return first_op;
  c.Advance(first_op->head);

  Vec<Ast> accum_rhs{};
  while (c.TypeIs(eTk::kMul) || c.TypeIs(eTk::kDiv) || c.TypeIs(eTk::kMod)) {
    accum_rhs.emplace_back(c);
    c.Advance();
    auto next_rhs = ParsePrefix(c);
    if (!next_rhs) return next_rhs;
    c.Advance(next_rhs->head);
    accum_rhs.push_back(next_rhs->ast);
  }

  // Associativity Left -> Right.
  // Note the accum vector holds (operand -> operator) pairs to keep track of the specific operator type.
  Vec<Ast> reduced_binop{};
  reduced_binop.push_back(move(first_op->ast));
  for (auto it = accum_rhs.cbegin(); it != accum_rhs.cend(); it++) {
    auto& prev_op = reduced_binop.back();
    Ast next_binop = *it;
    it++;
    next_binop.src_begin = prev_op.src_begin;
    next_binop.PushBack(move(prev_op));
    next_binop.src_end = it->src_end;
    next_binop.PushBack(*it);
    reduced_binop.pop_back();
    reduced_binop.push_back(next_binop);
  }
  accum_rhs.clear();

  CND_DEBUG_ASSERT(reduced_binop.size() == 1,
                   "[At the of reduction, reduced production vector must contain a single ast node.]");

  return LLParserResult{c.Advance(reduced_binop.back().src_end), reduced_binop.back()};
};

// [R->L] <prefix> ::= <INCREMENT_SIGN> * <access>
//            | <DECREMENT_SIGN> * <access>
//            | <EXCLAMATION_MARK> * <access>
CND_CX LLPrsResT ParsePrefix(TkCursorT c) CND_NX(CND_CLDEV_DEBUG_MODE) {
  using std::move;
  using std::next;
  using std::ranges::subrange;
  if (!IsTkPrimarySpecifier(c.Type())) return DEBUG_FAIL("Unexpected token at start of binary resolution.");
  // Accumulate consecutive prefix operators.
  Vec<Ast> accum_op{};
  while (c.IsPrefixOperator()) {
    accum_op.emplace_back(c);
    c.Advance();
  }
  // Expect a following access expression.
  auto last_op = ParseAccess(c);
  if (!last_op) return last_op;
  c.Advance(last_op->head);
  accum_op.push_back(move(last_op->ast));
  // Reduce. From front to back, all accumulated ops are the parent of the next.
  Ast reduced_prefix{move(accum_op.front())};
  auto reduced_end = accum_op.back().src_end;
  reduced_prefix.src_end = reduced_end;
  Ast* next_prefix = &reduced_prefix;
  for (const auto& next_op : subrange(next(accum_op.cbegin()), accum_op.cend())) {
    next_prefix->PushBack(move(next_op));
    next_prefix = &next_prefix->children.back();
    next_prefix->src_end = reduced_end;
  }
  accum_op.clear();
  return LLParserResult{c, reduced_prefix};
};

// [L->R] <access> ::= <postfix> (<FULL_STOP> <postfix>)*
CND_CX LLPrsResT ParseAccess(TkCursorT c) CND_NX(CND_CLDEV_DEBUG_MODE) {
  using std::move;
  using std::next;
  using std::ranges::subrange;

  if (!IsTkPrimarySpecifier(c.Type())) return DEBUG_FAIL("Unexpected token at start of binary access.");
  CND_DEBUG_ASSERT(!c.IsPrefixOperator(),
                   "[A prefix cannot occur at this stage, it should have been parsed first by precedence.]");

  auto first_op = ParsePostfix(c);
  if (!first_op) return first_op;
  c.Advance(first_op->head);

  Vec<Ast> accum_rhs{};
  while (c.TypeIs(eTk::kPeriod)) {
    accum_rhs.push_back({eAst::kMemberAccess, c.Iter(), c.Advance().Iter()});
    auto next_rhs = ParsePostfix(c);
    if (!next_rhs) return next_rhs;
    c.Advance(next_rhs->head);
    accum_rhs.push_back(next_rhs->ast);
  }

  Vec<Ast> reduced_binop{};
  reduced_binop.push_back(move(first_op->ast));
  for (auto it = accum_rhs.begin(); it != accum_rhs.end();) {
    auto& next_operator = *it;
    auto& prev_operand = reduced_binop.back();
    it++;
    auto& next_operand = *it;
    it++;

    next_operator.src_begin = prev_operand.src_begin;
    next_operator.PushBack(move(prev_operand));
    next_operator.src_end = next_operand.src_end;
    next_operator.PushBack(move(next_operand));
    reduced_binop.pop_back();
    reduced_binop.push_back(next_operator);
  }
  accum_rhs.clear();
  CND_DEBUG_ASSERT(reduced_binop.size() == 1,
                   "[At the of reduction, reduced production vector must contain a single ast node.]");

  return LLParserResult{c, reduced_binop.back()};
};

CND_CX LLPrsResT ParsePostfixAccess(TkCursorT c) CND_NX(CND_CLDEV_DEBUG_MODE) {
  using std::move;
  using std::next;
  using std::ranges::subrange;

  if (!IsTkPrimarySpecifier(c.Type())) return DEBUG_FAIL("Unexpected token at start of binary access.");
  CND_DEBUG_ASSERT(!c.IsPrefixOperator(),
                   "[A prefix cannot occur at this stage, it should have been parsed first by precedence.]");

  auto first_op = ParseResolution(c);
  if (!first_op) return first_op;
  c.Advance(first_op->head);

  Vec<Ast> accum_rhs{};
  while (c.TypeIs(eTk::kPeriod)) {
    accum_rhs.push_back({eAst::kMemberAccess, c.Iter(), c.Advance().Iter()});
    auto next_rhs = ParseResolution(c);
    if (!next_rhs) return next_rhs;
    c.Advance(next_rhs->head);
    accum_rhs.push_back(next_rhs->ast);
  }

  Vec<Ast> reduced_binop{};
  reduced_binop.push_back(move(first_op->ast));
  for (auto it = accum_rhs.begin(); it != accum_rhs.end();) {
    auto& next_operator = *it;
    auto& prev_operand = reduced_binop.back();
    it++;
    auto& next_operand = *it;
    it++;

    next_operator.src_begin = prev_operand.src_begin;
    next_operator.PushBack(move(prev_operand));
    next_operator.src_end = next_operand.src_end;
    next_operator.PushBack(move(next_operand));
    reduced_binop.pop_back();
    reduced_binop.push_back(next_operator);
  }
  accum_rhs.clear();
  CND_DEBUG_ASSERT(reduced_binop.size() == 1,
                   "[At the of reduction, reduced production vector must contain a single ast node.]");

  return LLParserResult{c, reduced_binop.back()};
};

// [L->R] <postfix> ::= <resolution> <INCREMENT_SIGN>*
//             | <resolution> <DECREMENT_SIGN>*
CND_CX LLPrsResT ParsePostfix(TkCursorT c) CND_NX(CND_CLDEV_DEBUG_MODE) {
  using std::move;
  using std::ranges::subrange;
  using std::views::reverse;

  if (!IsTkPrimarySpecifier(c.Type())) return DEBUG_FAIL("Unexpected token at start of binary resolution.");
  CND_DEBUG_ASSERT(!c.IsPrefixOperator(),
                   "[A prefix cannot occur at this stage, it should have been parsed first by precedence.]");

  auto first_op = ParseResolution(c);
  if (!first_op) return first_op;
  c.Advance(first_op->head);

  // Accumulate consecutive postfix operators.
  Vec<Ast> accum_op{move(first_op->ast)};
  while (c.IsPostfixOperator() || c.IsOpeningScope() || c.TypeIs(eTk::kPeriod)) {
    // Singular unary postfix.
    if (c.IsPostfixOperator()) {  // Single op
      accum_op.emplace_back(c);
      c.Advance();
    }
    // Binary call operator.
    else if (c.IsOpeningScope()) {
      // Determine the scope type.
      auto args_type{eAst::kINVALID};
      switch (c.Type()) {
        case eTk::kLParen:
          args_type = eAst::kFunctionCall;
          break;
        case eTk::kLBracket:
          args_type = eAst::kIndexOperator;
          break;
        case eTk::kLBrace:
          args_type = eAst::kListingOperator;
          break;
      }

      // Special early case for empty args. '()' / '[]' / '{}'
      auto lookahead_1 = c.Next();
      if (!lookahead_1.AtEnd() && lookahead_1.IsClosingScopeOf(c.Type())) {
        lookahead_1.Advance();
        accum_op.emplace_back(args_type, c.Iter(), lookahead_1.Iter(),
                              Ast{eAst::kArguments, c.Iter(), lookahead_1.Iter()});
        c.Advance(lookahead_1);
      }
      // Else parse scoped arguments...
      else {
        auto scoped_args = ParseScopedArguments(c);
        if (!scoped_args) return scoped_args;
        accum_op.emplace_back(args_type, c.Iter(), scoped_args->ast.src_end);
        accum_op.back().PushBack(move(scoped_args->ast));
        c.Advance(scoped_args->head);
      }
    }
    // Binary member access.
    else {
      // Store the immediate right hand side of this member access operation which
      // is all less-precedent operations: currently only resolution.
      c.Advance();  // Pass '.'
      auto access_rhs = ParseResolution(c);
      if (!access_rhs) return access_rhs;
      accum_op.emplace_back(eAst::kMemberAccess, c.Iter(), access_rhs->ast.src_end, move(access_rhs->ast));
      c.Advance(access_rhs->head);
    }
  }

  // Reduce. In reverse, from the back, all accumulated ops are the root of the next.
  Ast reduced_postfix{move(accum_op.back())};
  auto reduced_begin = accum_op.front().src_begin;
  reduced_postfix.src_begin = reduced_begin;

  accum_op.pop_back();  // Don't need last element anymore.
  Ast* next_postfix = &reduced_postfix;
  for (const auto& next_op : subrange(accum_op.cbegin(), accum_op.cend()) | reverse) {
    // Binary call operator.
    if (next_postfix->TypeIs(eAst::kFunctionCall) || next_postfix->TypeIs(eAst::kListingOperator) ||
        next_postfix->TypeIs(eAst::kIndexOperator)) {
      // next_postfix->src_begin = next_op.src_begin;
      next_postfix->PushFront(move(next_op));
      next_postfix = &next_postfix->children.front();
      next_postfix->src_begin = reduced_begin;
    }
    // Binary member access.
    else if (next_postfix->TypeIs(eAst::kMemberAccess)) {
      // next_postfix->src_begin = next_op.src_begin;
      next_postfix->PushFront(move(next_op));
      next_postfix = &next_postfix->children.front();
      next_postfix->src_begin = reduced_begin;
    }
    // Singular unary postfix.
    else {
      next_postfix->PushBack(move(next_op));
      next_postfix = &next_postfix->children.back();
      next_postfix->src_begin = reduced_begin;
    }
  }
  accum_op.clear();

  return LLParserResult{c, reduced_postfix};
};

// [L->R] <resolution> ::= <operand> (<DOUBLE_COLON> <operand>) *
//                 | <DOUBLE_COLON> <operand> (<DOUBLE_COLON> <operand>) *
CND_CX LLPrsResT ParseResolution(TkCursorT c) CND_NX(CND_CLDEV_DEBUG_MODE) {
  using std::move;

  if (!IsTkPrimarySpecifier(c.Type())) return DEBUG_FAIL("Unexpected token at start of binary resolution.");
  CND_DEBUG_ASSERT(!c.IsPrefixOperator(),
                   "A prefix cannot occur at this stage, it should have been parsed first by precedence.");

  // Retrieve the first left hand side operand/subexpression.
  // Handle special case where '::' is a prefix, the first operator becomes kRootNamespace.
  Ast first_op{};
  if (c.TypeIs(eTk::kDoubleColon))
    first_op = Ast{eAst::kRootNamespace, c.Iter(), c.Advance().Iter()};
  else {
    auto first_op_res = ParsePrimaryOperand(c);
    if (!first_op_res) return first_op_res;
    c.Advance(first_op_res->head);
    first_op = first_op_res->ast;
  }

  Vec<Ast> accum_rhs{};
  while (c.TypeIs(eTk::kDoubleColon)) {
    accum_rhs.push_back({eAst::kResolutionOp, c.Iter(), c.Advance().Iter()});
    auto next_rhs = ParsePrimaryOperand(c);
    if (!next_rhs) return next_rhs;
    c.Advance(next_rhs->head);
    accum_rhs.push_back(next_rhs->ast);
  }

  Vec<Ast> reduced_binop{};
  reduced_binop.push_back(move(first_op));
  for (auto it = accum_rhs.begin(); it != accum_rhs.end();) {
    auto& next_operator = *it;
    auto& prev_operand = reduced_binop.back();
    it++;
    auto& next_operand = *it;
    it++;

    next_operator.src_begin = prev_operand.src_begin;
    next_operator.PushBack(move(prev_operand));
    next_operator.src_end = next_operand.src_end;
    next_operator.PushBack(move(next_operand));
    reduced_binop.pop_back();
    reduced_binop.push_back(next_operator);
  }
  accum_rhs.clear();
  CND_DEBUG_ASSERT(reduced_binop.size() == 1,
                   "[At the of reduction, reduced production vector must contain a single ast node.]");

  return LLParserResult{c, reduced_binop.back()};
};

CND_CX LLPrsResT ParseScopedArguments(TkCursorT c) CND_NX(CND_CLDEV_DEBUG_MODE) {
  if (c.TypeIs(eTk::kLParen)) return ParseParenArguments(c);
  if (c.TypeIs(eTk::kLBracket)) return ParseSquareArguments(c);
  if (c.TypeIs(eTk::kLBrace)) return ParseCurlyArguments(c);
  return DEBUG_FAIL("Expected an opening scope.");
}
CND_CX LLPrsResT ParseParenArguments(TkCursorT c) CND_NX(CND_CLDEV_DEBUG_MODE) {
  if (!c.TypeIs(eTk::kLParen)) return DEBUG_FAIL("Expected an opening scope.");
  // For now we simply parse a primary expr inside the scope and change the root ast type.
  auto args = ParseParenSubexpr(c);
  if (!args) return args;
  args->ast.type = eAst::kArguments;
  return args;
}
CND_CX LLPrsResT ParseSquareArguments(TkCursorT c) CND_NX(CND_CLDEV_DEBUG_MODE) {
  if (!c.TypeIs(eTk::kLBracket)) return DEBUG_FAIL("Expected an opening bracket.");
  // For now we simply parse a primary expr inside the scope and change the root ast type.
  auto args = ParseSquareSubexpr(c);
  if (!args) return args;
  args->ast.type = eAst::kArguments;
  return args;
}
CND_CX LLPrsResT ParseCurlyArguments(TkCursorT c) CND_NX(CND_CLDEV_DEBUG_MODE) {
  if (!c.TypeIs(eTk::kLBrace)) return DEBUG_FAIL("Expected an opening brace.");
  // For now we simply parse a primary expr inside the scope and change the root ast type.
  auto args = ParseCurlySubexpr(c);
  if (!args) return args;
  args->ast.type = eAst::kArguments;
  return args;
}

CND_CX LLPrsResT ParsePrimaryOperand(TkCursorT c) CND_NX(CND_CLDEV_DEBUG_MODE) {
  if (c.TypeIs(eTk::kIdent)) return ParseIdentityOperand(c);
  if (c.IsAnOperand()) return ParseValueOperand(c);
  if (c.IsOpeningScope()) return ParseOperandSet(c);
  return DEBUG_FAIL("Expected an identity operand, value operand or operand set.");
};
CND_CX LLPrsResT ParseOperandSet(TkCursorT c) CND_NX(CND_CLDEV_DEBUG_MODE) {
  if (c.TypeIs(eTk::kLParen)) return ParseParenSubexpr(c);
  if (c.TypeIs(eTk::kLBracket)) return ParseSquareSubexpr(c);
  if (c.TypeIs(eTk::kLBrace)) return ParseCurlySubexpr(c);
  return DEBUG_FAIL("Expected an opening scope.");
}
CND_CX LLPrsResT ParseIdentityOperand(TkCursorT c) CND_NX(CND_CLDEV_DEBUG_MODE) {
  if (c.TypeIs(eTk::kIdent))
    return LLParserResult(c.Next(), Ast{c});
  else
    return DEBUG_FAIL("Expected an identity-operand.");
};
CND_CX LLPrsResT ParseValueOperand(TkCursorT c) CND_NX(CND_CLDEV_DEBUG_MODE) {
  if (c.TypeIsnt(eTk::kIdent) && c.IsAnOperand())
    return LLParserResult(c.Next(), Ast{c});
  else
    return DEBUG_FAIL("Expected a value-operand.");
};
CND_CX LLPrsResT ParseParenSubexpr(TkCursorT c) CND_NX(CND_CLDEV_DEBUG_MODE) {
  if (c.TypeIsnt(eTk::kLParen)) return DEBUG_FAIL("Expected a left parentheses.");
  auto subexpr_beg = c.Iter();
  c.Advance();
  auto subexpr = ParseListFold(c);
  if (!subexpr) return subexpr;
  c.Advance(subexpr->head);
  if (c.TypeIsnt(eTk::kRParen)) return DEBUG_FAIL("Expected a right parentheses.");
  c.Advance();
  return LLParserResult(c, Ast{eAst::kSubexpression, subexpr_beg, c.Iter(), subexpr->ast});
};
CND_CX LLPrsResT ParseSquareSubexpr(TkCursorT c) CND_NX(CND_CLDEV_DEBUG_MODE) {
  if (c.TypeIsnt(eTk::kLBracket)) return DEBUG_FAIL("Expected a left square bracket.");
  auto subexpr_beg = c.Iter();
  c.Advance();
  auto subexpr = ParseListFold(c);
  if (!subexpr) return subexpr;
  c.Advance(subexpr->head);
  if (c.TypeIsnt(eTk::kRBracket)) return DEBUG_FAIL("Expected a right square bracket.");
  c.Advance();
  return LLParserResult(c, Ast{eAst::kSquareSubexpr, subexpr_beg, c.Iter(), subexpr->ast});
};
CND_CX LLPrsResT ParseCurlySubexpr(TkCursorT c) CND_NX(CND_CLDEV_DEBUG_MODE) {
  if (c.TypeIsnt(eTk::kLBrace)) return DEBUG_FAIL("Expected a left curly bracket.");
  auto subexpr_beg = c.Iter();
  c.Advance();
  auto subexpr = ParseListFold(c);
  if (!subexpr) return subexpr;
  c.Advance(subexpr->head);
  if (c.TypeIsnt(eTk::kRBrace)) return DEBUG_FAIL("Expected a right curly bracket.");
  c.Advance();
  return LLParserResult(c, Ast{eAst::kCurlySubexpr, subexpr_beg, c.Iter(), subexpr->ast});
};

CND_CX SepScopePrsResT FindSeperatedScopeImpl(TkConstIterT begin, TkConstIterT end, eTk separator,
                                              eTk scope_type) CND_NX {
  Vec<TkScopeT> scopes;
  auto closing_scope = [=] {
    switch (scope_type) {
      case eTk::kLParen:
        return eTk::kRParen;
      case eTk::kLBrace:
        return eTk::kRBrace;
      case eTk::kLBracket:
        return eTk::kRBracket;
      default:
        return eTk::kSemicolon;
    }
  }();

  if (begin->Type() != scope_type) {
    scopes.push_back(TkScopeT{false, begin, end});
    return scopes;
  }
  Vec<eTk> scope_type_history;
  TkConstIterT last_closed = begin;
  begin++;  // Skip the open list token
  for (TkConstIterT i = begin; i < end;) {
    if (i->TypeIs(separator) && scope_type_history.empty()) {
      scopes.push_back(TkScopeT{true, last_closed, i + 1});
      last_closed = i;
    } else if (i->IsLScope()) {
      scope_type_history.push_back(i->Type());
    } else if (i->IsRScope() && !scope_type_history.empty()) {
      if (i->IsRScopeOf(scope_type_history.back())) {
        scope_type_history.pop_back();
      } else {
        scopes.push_back(TkScopeT{false, i, end});  // Wrong closing scope
        return scopes;
      }
    } else if (i->TypeIs(closing_scope) && scope_type_history.empty()) {
      // end of list
      scopes.push_back(TkScopeT{true, last_closed, i + 1});
      return scopes;
    } else if (i->TypeIs(eTk::kEofile)) {
      scopes.push_back(TkScopeT{false, i, end});  // End of file
      return scopes;
    }
    std::advance(i, 1);
  }
}

CND_CX SepScopePrsResT FindSeperatedParen(TkConstIterT begin, TkConstIterT end, eTk separator) CND_NX {
  return FindSeperatedScopeImpl(begin, end, separator, eTk::kLParen);
};
CND_CX SepScopePrsResT FindSeperatedBrace(TkConstIterT begin, TkConstIterT end, eTk separator) CND_NX {
  return FindSeperatedScopeImpl(begin, end, separator, eTk::kLBrace);
};
CND_CX SepScopePrsResT FindSeperatedBracket(TkConstIterT begin, TkConstIterT end, eTk separator) CND_NX {
  return FindSeperatedScopeImpl(begin, end, separator, eTk::kLBracket);
};
CND_CX ScopePrsResT FindStatement(eTk open, eTk close, TkConstIterT begin, TkConstIterT end) CND_NX {
  auto opening = begin;
  int scope_depth = 0;
  Vec<eTk> scope_history;
  // Find the last matching close token that is not within a () [] or {} scope.
  // Start from the token after the open token. All scope depths should be 0
  // at the end of the loop on a valid scope.
  for (auto c = begin + 1; c != end; c++) {
    if (c->IsLScope() || c->TypeIs(open)) {
      scope_depth++;
      scope_history.push_back(c->Type());
    } else if (c->IsRScope() || c->TypeIs(close)) {
      if (scope_history.empty())
        if (c->TypeIs(close))
          return TkScopeT{true, opening, c + 1};
        else
          return DEBUG_FAIL("Unclosed scope.");

      if (!c->IsRScopeOf(scope_history.back())) return DEBUG_FAIL("Mismatched scopes.");

      scope_history.pop_back();
      scope_depth--;
    } else
      continue;
  }
  return DEBUG_FAIL("Unclosed scope.");
}
CND_CX ScopePrsResT FindOpenStatement(eTk close, TkConstIterT begin, TkConstIterT end) CND_NX {
  auto opening = begin;
  int scope_depth = 0;
  Vec<eTk> scope_history;
  for (auto c = begin + 1; c != end; c++) {
    if (c->IsLScope()) {
      scope_depth++;
      scope_history.push_back(c->Type());
    } else if (c->IsRScope() || c->TypeIs(close)) {
      if (scope_history.empty())
        if (c->TypeIs(close))
          return TkScopeT{true, opening, c + 1};
        else
          return DEBUG_FAIL("Unclosed scope.");

      if (!c->IsRScopeOf(scope_history.back())) return DEBUG_FAIL("Mismatched scopes.");

      scope_history.pop_back();
      scope_depth--;
    } else
      continue;
  }
  return DEBUG_FAIL("Unclosed scope.");
};
CND_CX ScopePrsResT FindOpenStatement(Vec<eTk> close, TkConstIterT begin, TkConstIterT end) CND_NX {
  auto opening = begin;
  int scope_depth = 0;
  Vec<eTk> scope_history;
  for (auto c = begin + 1; c != end; c++) {
    if (c->IsLScope()) {
      scope_depth++;
      scope_history.push_back(c->Type());
    } else if (c->IsRScope() ||
               std::any_of(close.cbegin(), close.cend(), [=](const auto& v) { return c->TypeIs(v); })) {
      if (scope_history.empty())
        if (std::any_of(close.cbegin(), close.cend(), [=](const auto& v) { return c->TypeIs(v); }))
          return TkScopeT{true, opening, c + 1};
        else
          return DEBUG_FAIL("Unclosed scope.");

      if (!c->IsRScopeOf(scope_history.back())) return DEBUG_FAIL("Mismatched scopes.");

      scope_history.pop_back();
      scope_depth--;
    } else
      continue;
  }
  return DEBUG_FAIL("Unclosed scope.");
};
CND_CX ScopePrsResT FindProgramStatement(TkConstIterT begin, TkConstIterT end) CND_NX {
  return FindOpenStatement(eTk::kSemicolon, begin, end);
};
CND_CX ScopePrsResT FindProgramStatement(TkCursorT cursor) CND_NX {
  return FindOpenStatement(eTk::kSemicolon, cursor.Iter(), cursor.End());
};

namespace detail {

/// @brief Parses possibly existing modifiers at the start of a decl and advances cursor if necessary.
/// @param c Token cursor which will be advanced past the modifiers or stay in place if none.
/// @return Error or the resulting ast node.
CND_CX LLPrsResT ParseOptionalModifiers(TkCursorT& c) {
  Ast mod_node;
  auto decl_begin = c.Iter();
  if (c.IsModifierKeyword()) {
    auto mod_result = ParseModifiers(c);
    if (!mod_result) return mod_result;
    mod_node = move(mod_result.value().ast);
    c.Advance(mod_result.value().head);
  } else {
    // No modifiers found.
    mod_node = Ast(eAst::kModifiers, c.Iter(), c.Iter());
  }
  return LLParserResult(c, mod_node);
}
}  // namespace detail

}  // namespace parser

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* end Internal parsing methods impl */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}  // namespace trtools
}  // namespace cnd

/// @} // end of cnd_compiler_data

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
//
// Licensed under the Apache License, Version 2.0(the "License"); you may not use this file except in compliance with
// the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on
// an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////