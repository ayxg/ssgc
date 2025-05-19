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
#include "token_cursor.hpp"
#include "token_scope.hpp"

#include "token_closure.hpp"
#include "lexer.hpp"

//#include "lr_parser.hpp"
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
CND_CX LLPrsResT ParseWhileDecl(TkCursorT c) CND_NX;
CND_CX LLPrsResT ParseForDecl(TkCursorT c) CND_NX;
CND_CX LLPrsResT ParseUsingDecl(TkCursorT c) CND_NX;
CND_CX LLPrsResT ParseVariableDecl(TkCursorT c) CND_NX;
CND_CX LLPrsResT ParseMethodDecl(TkCursorT c) CND_NX;
CND_CX LLPrsResT ParseClassDecl(TkCursorT c) CND_NX;
CND_CX LLPrsResT ParseEnumDecl(TkCursorT c) CND_NX;
CND_CX LLPrsResT ParseEnumDef(TkCursorT c) CND_NX;
CND_CX LLPrsResT ParseEnumBlock(TkCursorT c) CND_NX;
CND_CX LLPrsResT ParseProgram(TkCursorT c) CND_NX;
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
    return LLParserResult(
        c, Ast(eAst::kNamespaceObjectInclusion, stmt_begin, c.Iter(), nullptr, {primary_result->ast}));

  }

  // Invalid format.
  else
    return DEBUG_FAIL("Using statement expected <@> or <kw-lib> or <kw-namespace> or <type-expression>.");
};

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
CND_CX LLPrsResT ParseProgram(TkCursorT c) CND_NX {
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

class Parser {
 public:
  using ParserOutputT = Ex<Ast, ClMsgBuffer>;
  using ParserFailT = Unex<ClMsgBuffer>;
  using CppSrcLocT = std::source_location;

  struct ParserCursor {
    Ast ast{};
    std::span<const Tk>::const_iterator head{};
  };

  using ParserResultT = Ex<ParserCursor, ClMsgBuffer>;

  static CND_CX ParserResultT ParseOperand(const std::span<const Tk>& tokens,
                                           std::span<const Tk>::const_iterator c) noexcept {
    using cldev::clmsg::MakeClMsg;
    using std::next;
    using std::span;
    if (c->IsAnOperand())
      return ParserCursor{.ast = {c->Type(), c, next(c)}, .head = next(c)};
    else
      return ParserFailT{MakeClMsg<eClErr::kCompilerDevDebugError>(CppSrcLocT{}, "Opening token is not an operand.")};
  };

  static CND_CX ParserResultT ParseReturnStmt(const std::span<const Tk>& tokens,
                                              std::span<const Tk>::const_iterator c) {
    using enum eTk;
    using cldev::clmsg::MakeClMsg;
    using std::move;
    using std::next;
    using std::span;
    span<const Tk>::const_iterator beg_it = c;
    if (c->TypeIs(kKwReturn)) {
      c++;
      if (c->TypeIs(kSemicolon))  // Check for valid empty return statement
        return ParserCursor{.ast = {eAst::kKwReturn, beg_it, next(c)}, .head = next(c)};

      // Skip any preceding whitespace.
      while (c->TypeIs(kNewline) || c->TypeIs(kWhitespace)) c++;

      // Get the primary expr and ending semicolon6 following this return keyword.
      /*   auto value_expr_result = ParsePrimaryStatement(c);*/
      auto expr_res = ParseOperand(tokens, c);
      if (!expr_res) return expr_res;
      return ParserCursor{.ast = {eAst::kKwReturn, beg_it, expr_res.value().head, nullptr, {expr_res.value().ast}},
                          .head = ++(expr_res.value().head)};
    } else {
      return ParserFailT{
          MakeClMsg<eClErr::kCompilerDevDebugError>(CppSrcLocT{}, "Opening token is not a return keyword.")};
    }
  }

  static CND_CX ParserOutputT ParseProgram(std::span<const Tk> tokens) {
    using cldev::clmsg::MakeClMsg;
    using std::advance;
    using std::move;

    auto head = tokens.cbegin();
    Ast program_node{eAst::kProgram};
    while (head != tokens.cend()) {
      if (head->IsDeclarative()) {
        ParserResultT parsed_decl = ParseReturnStmt(tokens, head);
        if (!parsed_decl) return ParserFailT{move(parsed_decl.error())};
        program_node.children.push_back(move(parsed_decl.value().ast));
        head = move(parsed_decl.value().head);
      } else
        return ParserFailT{MakeClMsg<eClErr::kParserExpectedDeclaration>()};
    }
    return program_node;
  }
};

}  // namespace trtools

// We can now make all our unit tests static asserts!! :D
// Its a bit harder because all tokens must relate to a literal! and all asts depend on the direct token literal.
// No string data is actually stored in the tk/ast structs.

CND_CX bool TestInternalParserMethod(std::string_view code,
                                     cnd::trtools::Parser::ParserResultT (*fn)(const std::span<const Tk>&,
                                                                               std::span<const Tk>::const_iterator)) {
  auto expected_source = cnd::trtools::Lexer::Lex(code);
  if (!expected_source) return false;  // Failed to lex
  std::span<const Tk> source = expected_source.value();
  auto parse_result = fn(source, source.cbegin());
  if (!parse_result) return false;  // Failed to parse
  return true;
}

CND_CX bool TestInternalParserMethod(std::span<const Tk> source,
                                     cnd::trtools::Parser::ParserResultT (*fn)(const std::span<const Tk>&,
                                                                               std::span<const Tk>::const_iterator),
                                     const Ast& expected_ast) {
  auto parse_result = fn(source, source.cbegin());
  if (!parse_result) return false;                  // Failed to parse
  return expected_ast == parse_result.value().ast;  // Compare
}

// static_assert([]{
//   using namespace cnd;
//   CND_CX  auto source = R"("1 1.1 "string literal" alnumus 1c 1b 1u none ")"_cndTkLiteral;
//   std::span<const Tk> s = source;
//   auto beg = s.cbegin();
//
//   return TestInternalParserMethod(std::span<const Tk>{beg, beg+1 }, trtools::Parser::ParseOperand,
//                            Ast(eAst::kLitInt, beg, beg + 1));
//
//   //std::vector<Ast> expected_result = {
//   //    Ast(eAst::kLitInt, beg,beg+1),       Ast(eAst::kLitReal, "1.1"),   Ast(eAst::kLitCstr, "'string literal'"),
//   //    Ast(eAst::kLitCstr, "'\\\''"), Ast(eAst::kIdent, "alnumus"), Ast(eAst::kLitByte, "1c"),
//   //    Ast(eAst::kLitBool, "1b"),     Ast(eAst::kLitUint, "1u"),    Ast(eAst::kKwNone, "none"),
//   //};
//   //return true;
//   //ASSERT_TRUE(expected_source.Valid());
////auto source = expected_source.Extract();
////// Operands.
////std::vector<Ast> expected_result = {
////    Ast(eAst::LitInt, "1"),       Ast(eAst::LitReal, "1.1"),   Ast(eAst::LitCstr, "'string literal'"),
////    Ast(eAst::LitCstr, "'\\\''"), Ast(eAst::Ident, "alnumus"), Ast(eAst::LitByte, "1c"),
////    Ast(eAst::LitBool, "1b"),     Ast(eAst::LitUint, "1u"),    Ast(eAst::KwNone, "none"),
////};
////
////auto num_lit = parser::ParseOperand({source.cbegin(), source.cend()});
////ASSERT_TRUE(num_lit.Valid());
////EXPECT_EQ(num_lit.Value().Type(), expected_result[0].Type());
////EXPECT_EQ(num_lit.Value().Literal(), expected_result[0].Literal());
////
////auto real_lit = parser::ParseOperand(num_lit.Always());
////ASSERT_TRUE(real_lit.Valid());
////EXPECT_EQ(real_lit.Value().Type(), expected_result[1].Type());
////EXPECT_EQ(real_lit.Value().Literal(), expected_result[1].Literal());
////
////auto str_lit = parser::ParseOperand(real_lit.Always());
////ASSERT_TRUE(str_lit.Valid());
////EXPECT_EQ(str_lit.Value().Type(), expected_result[2].Type());
////EXPECT_EQ(str_lit.Value().Literal(), expected_result[2].Literal());
////
////auto str_lit2 = parser::ParseOperand(str_lit.Always());
////ASSERT_TRUE(str_lit2.Valid());
////EXPECT_EQ(str_lit2.Value().Type(), expected_result[3].Type());
////EXPECT_EQ(str_lit2.Value().Literal(), expected_result[3].Literal());
////
////auto alnumus_lit = parser::ParseOperand(str_lit2.Always());
////ASSERT_TRUE(alnumus_lit.Valid());
////EXPECT_EQ(alnumus_lit.Value().Type(), expected_result[4].Type());
////EXPECT_EQ(alnumus_lit.Value().Literal(), expected_result[4].Literal());
////
////auto byte_lit = parser::ParseOperand(alnumus_lit.Always());
////ASSERT_TRUE(byte_lit.Valid());
////EXPECT_EQ(byte_lit.Value().Type(), expected_result[5].Type());
////EXPECT_EQ(byte_lit.Value().Literal(), expected_result[5].Literal());
////
////auto bit_lit = parser::ParseOperand(byte_lit.Always());
////ASSERT_TRUE(bit_lit.Valid());
////EXPECT_EQ(bit_lit.Value().Type(), expected_result[6].Type());
////EXPECT_EQ(bit_lit.Value().Literal(), expected_result[6].Literal());
////
////auto unsigned_lit = parser::ParseOperand(bit_lit.Always());
////ASSERT_TRUE(unsigned_lit.Valid());
////EXPECT_EQ(unsigned_lit.Value().Type(), expected_result[7].Type());
////EXPECT_EQ(unsigned_lit.Value().Literal(), expected_result[7].Literal());
////
////auto none_lit = parser::ParseOperand(unsigned_lit.Always());
////ASSERT_TRUE(none_lit.Valid());
////EXPECT_EQ(none_lit.Value().Type(), expected_result[8].Type());
////EXPECT_EQ(none_lit.Value().Literal(), expected_result[8].Literal());
//}());

}  // namespace cnd

/// @} // end of cnd_compiler_data
//
// static_assert([] {
//  using namespace cnd::trtools::literals;
//  CND_CX auto src = "42;"_cndTkLiteral;
//  auto src_span = std::span{src.data(), src.size()};
//  return cnd::trtools::parser::ParseOperand({src_span.cbegin(), src_span.cend()});
//}()
//                  .value()
//                  .ast.type == cnd::eAst::kLitInt);
//
// static_assert([] {
//  using namespace cnd::trtools::literals;
//  CND_CX auto src = "42;"_cndTkLiteral;
//  auto src_span = std::span{src.data(), src.size()};
//  return cnd::trtools::parser::ParseOperand({src_span.cbegin(), src_span.cend()});
//}()
//                  .value()
//                  .ast.type == cnd::eAst::kLitInt);

static_assert([] {
  using namespace cnd::trtools::literals;
  CND_CX auto src = "42;"_cndTkLiteral;
  auto src_span = std::span{src.data(), src.size()};
  return cnd::trtools::Parser::ParseOperand(src_span, src_span.cbegin());
}()
                  .value()
                  .ast.type == cnd::eAst::kLitInt);

//
// static_assert(cnd::trtools::Parser::ParseProgram("return 1;"_cndTkLiteral).value() == cnd::Ast{cnd::eAst::kProgram});

static_assert([] {
  using namespace cnd::trtools::literals;
  CND_CX auto src = "return;"_cndTkLiteral;
  auto src_span = std::span{src.data(), src.size()};
  return cnd::trtools::parser::ParseReturnStmt({src_span.cbegin(), src_span.cend()});
}()
                  .value()
                  .ast.type == cnd::eAst::kKwReturn);

// static_assert([] {
//   using namespace cnd::trtools::literals;
//   CND_CX auto src = "return 1;"_cndTkLiteral;
//   auto src_span = std::span{src.data(), src.size()};
//   return cnd::trtools::parser::ParseReturnStmt({src_span.cbegin(), src_span.cend()});
// }()
//                   .value()
//                   .ast.type == cnd::eAst::kKwReturn);

// static_assert([] {
//   using namespace cnd::trtools::literals;
//   CND_CX auto src = "return 1;"_cndTkLiteral;
//   auto src_span = std::span{src.data(), src.size()};
//   return cnd::trtools::Parser::ParseProgram(src_span);
// }()
//                   .value_or(cnd::Ast{})
//                   .type == cnd::eAst::kProgram);

//// Assert an minimal 'return 0' program to test CND_CX  evaluation.
//// The compiler should be able to recognize that the defined process is CND_CX  - only.
//// When the entire program is evaluable at C++ compile time. It is considered a 'pure' C& program.
//// The translation unit should have a property such as 'is_pure_source' which will be used to conditionally
//// enable runtime code in the C& implementation.
////
//// In fact, the entire CND_CX  implementation isn't that useful due to not allowing recursive compilation
//// compared to the C& comptime evaluation which allows calling and generating shared objects and executables.
//// C++ compiler does not allow IO operations or access to the filesystem in CND_CX  evaluation..
//// C& Compile time allows this, but can only evaluate it at the C& Compiler's C++ runtime.

// static_assert(cnd::ParseTranslationPartition((R"(
//   proc @root: ` define a process called root
//       main: return 0; ` define the main method
//
//   return root(); ` call the root process
//)"_cndTkLiteral))
//                   .empty());

namespace cnd {

//// Receives a translation partition which is the concrete syntax tree of the parsed source file.
// CND_CX  Ex<int, cldev::clmsg::ClMsgBuffer> Abstract(ConstevalContext& ctx, const Cst& part) {
//   using cldev::clmsg::ClMsgBuffer;
//   using cldev::clmsg::MakeClMsg;
//   // Go through all pragmatic statements checking if they must be interpreted before the full
//   // construction of the ast.
//   for (const Cst& stmt : part.children) {
//     if (IsCstDeclarative(stmt.type)) {
//       // Pragmatic return -> evaluate expression, halt composition and return exit code.
//       if (stmt.type == eAst::kKwReturn) {
//         auto return_expr = AbstractPrimaryExpr(ctx, stmt.children.front());
//         if (!return_expr.children.empty())
//           return Unex<ClMsgBuffer>{MakeClMsg<eClErr::kCompilerDevDebugError>(
//               std::source_location{}, "Pragmatic return statement must evaluate to a literal integral.")};
//         ctx.is_halted = true;
//         return cxx::cxstoi(return_expr.literal.c_str());
//       }
//       // Variable Decl ->
//       else if (stmt.type == eAst::kVariableDeclaration) {
//
//       }
//     }
//   }
//
//
// }

}  // namespace cnd

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

/// Primary Expression Shift-Reduction Parser
///
/// Cursor begin and end must be the start and end of the expression.
/// Note this parser does not take the entire source as an argument.
/// Only pass the current scope to be parser.
/// The end of TkCursor may or may not be end of source.
/// This parser does not advance the cursor or return a new start point.
// class ExprParser {
//  public:
//   using ClosureListIter = ClosureBuffer::ClosureListIterT;
//   CND_CX static LRPrsResT Perform(TkCursorT c) { return ExprParser().Parse(c); }
//   CND_CX LRPrsResT Parse(TkCursorT c);
//
//  private:
//   /// Set is_first_operator_ to false if currently true, else do nothing.
//   CND_CX void FirstOperatorSwitch();
//   CND_CX void ResolvePrefix();
//   CND_CX void ResolvePostfix();
//   CND_CX void ResolveBinaryLeftAssoc();
//   CND_CX void ResolveBinaryRightAssoc();
//   CND_CX void ResolveBinary();
//   /// Resolves the last closure based on type,unchecked.
//   CND_CX void ResolveLast();
//
//   /// [Action Skip] Push OPERAND to closure output,
//   ///    advance head by the number of tokens consumed.
//   CND_CX CompilerProcessResult<bool> ActionSkip(TkCursorT& c);
//
//   /// [Action Store] Push OPERATOR to closure output,
//   ///                Push back a new closure from OPERATOR,
//   ///                advance head by the number of tokens consumed.
//   CND_CX CompilerProcessResult<bool> ActionStore(TkCursorT& c);
//
//   /// [Action Check] Compares priority of head and last closure.
//   ///    If head >= last closure, store head.
//   ///    If head < last closure,
//   ///        -> resolve all consecutive closures of lower priority.
//   ///           Then store.
//   ///    If head is at end, resolve all closures. Return.
//   CND_CX CompilerProcessResult<bool> ActionCheck(TkCursorT& c);
//
//   /// [Choose Action] Decides which action to take.
//   ///                 Main loop of the closure resolver.
//   /// Action will be skip , store, or check.
//   /// - On first occurence of an operator- a store will be performed.
//   ///    All following operators will be checks.
//   /// - A skip is performed when an operand is encountered.
//   ///    Multi-token operands trigger repeated skips.
//   /// - A check is performed when an operator is encountered.
//   /// - At the end, always do a check.
//   CND_CX CompilerProcessResult<bool> ChooseAction(TkCursorT& c);
//
//   CND_CX CompilerProcessResult<Vec<Tk>> CreateAndResolveClosures(TkCursorT c);
//
//   /// After the closures are resolved, perform an LL recursive descent parse.
//   /// Expect the expression to be fully parenthesized.
//   CND_CX LRPrsResT ParseImpl(TkCursor<Tk, Span> c);
//
//  private:
//   enum class eNextExpectedHeadToken { kOperative, kOperator, kNone };
//   static CND_CX Tk kOpenParenTk{eTk::kLParen, "("};
//   static CND_CX Tk kCloseParenTk{eTk::kRParen, ")"};
//   /// Expected token type on next iteration.
//   eNextExpectedHeadToken next_expected_ = {eNextExpectedHeadToken::kOperative};
//   Vec<Pair<TkIterT, TkIterT>> closures_;
//   Vec<TkIterT> tkbuf_;
//   Ast parsed_{eTk::kNONE};
//   bool is_first_operator_ = {true};
//   bool is_resolved_ = {false};
// };
// struct LRParser3 {
//  using ClResBool = CompilerProcessResult<bool>;
//  enum class ParseAction { Shift, Reduce };
//  enum class eClosureState {
//    kUnknown,
//    kZeroProduct,
//    kEndProduct,
//    kOperand,
//    kSubexprBegin,
//    kSubexpr,
//    kPrefix,
//    kExpr,
//    kEndScope,
//    kOpenScope,
//    kUnaryPrefixOp,
//    kUnaryPostfixOp,
//    kLeftBinaryOp,
//    kRightBinaryOp,
//  };
//  struct Closure {
//    eClosureState state{eClosureState::kUnknown};
//    Ast ast{eAst::kNONE};
//  };
//  Vec<Closure> stack_;
//  int dbg_counter{0};
//  int GetStepNum() { return dbg_counter++; }
//  void PrintStep(const TkCursorT& lookahead, int this_state, std::string rule, std::string action) {
//    std::cout << std::setw(5) << "Step " << std::setw(3) << GetStepNum() << std::setw(7) << "| State " << std::setw(3)
//              << this_state << std::setw(8) << " | Rule :" << std::setw(20) << rule << " | L0 = \""
//              << lookahead.Literal() << "\" S = [" <<
//        [&]() {
//          std::stringstream ss;
//          for (auto it = stack_.begin(); it != stack_.end(); it++) {
//            ss << eAstToCStr(it->ast.type) << ',';
//          }
//          return ss.str();
//        }() << "]"
//              << "| Action : " << action << std::endl;
//  }
//  using StateResult = CompilerProcessResult<Vec<Closure>::iterator>;
//
//  CND_CX Closure PopStack() {
//    auto back = stack_.back();
//    stack_.pop_back();
//    return back;
//  }
//
//  CND_CX Vec<Closure>::iterator LastClosure() { return stack_.end() - 1; }
//
//  CND_CX Vec<Closure>::iterator Shift(TkCursorT& lookahead, eClosureState state) {
//    stack_.push_back({state, lookahead});
//    lookahead.Advance();
//    return stack_.end() - 1;
//  }
//
//  CompilerProcessResult<Ast> Parse(TkCursorT c) {
//    std::cout << "Parsing:" << std::endl;
//    auto result = State0(c);
//    if (!result) return CompilerProcessResult<Ast>::Failure(result.error());
//    if (result.value()->state != eClosureState::kEndProduct)
//      return DEBUG_FAIL("LR Parser Logic Failure.");
//    else
//      return result.value()->ast;
//  }
//
//  /* 0
//   * 0 $accept: . expr $end
//   *
//   * '(' ? shift, and go to state 1
//   * operand ?  shift, and go to state 2
//   *
//   * kOperand ? go to state 3
//   * kExpr ? go to state 4
//   */
//
//  StateResult State0(TkCursorT& c) {
//    if (c.AtEnd()) {
//      PrintStep(c, 0, "iPrimaryExpression : * iExpression $EOF", "[Error] Begin is at end.");
//      return DEBUG_FAIL("Begin is at end.");
//    }
//
//    if (c.IsAnOperand()) {
//      PrintStep(c, 0, "iPrimaryExpression : * iExpression $EOF", "L[0] ? kInt | kIdent -> [Shift] [Goto 2]");
//      Shift(c, eClosureState::kOperand);
//      auto tmp = State2(c);
//      if (!tmp) return ClResBool::Failure{tmp.error()};
//      return Reduce0(c, tmp);
//    } else if (c.TypeIs(eTk::kLParen)) {
//      PrintStep(c, 0, "iPrimaryExpression : * iExpression $EOF", "L[0] ? kLParen -> [Shift] [Goto 1]");
//      Shift(c, eClosureState::kOpenScope);
//      auto tmp = State1(c);
//      if (!tmp) return ClResBool::Failure{tmp.error()};
//      return Reduce0(c, tmp);
//    } else {
//      return DEBUG_FAIL("Primary expression must begin with an operand, prefix or subexpr.");
//    }
//  }
//  StateResult Reduce0(TkCursorT& c, const StateResult& last_prod) {
//    switch (last_prod.value()->state) {
//      case eClosureState::kExpr:
//        PrintStep(c, 0, "iPrimaryExpression : * iExpression $EOF", "S[N] ? iExpr -> [Goto 4]");
//        return Reduce0(c, State4(c, last_prod));
//      case eClosureState::kOperand:
//        PrintStep(c, 0, "iPrimaryExpression : * iExpression $EOF", "S[N] ? iOperand -> [Goto 3]");
//        return Reduce0(c, State3(c, last_prod));
//      case eClosureState::kEndProduct:
//        PrintStep(c, 0, "iPrimaryExpression : * iExpression $EOF", "S[N] ? iEndProduct -> [Accept]");
//        return LastClosure();
//      default:
//        return DEBUG_FAIL("Unexpected production.");
//    }
//  }
//
//  /* 1
//   * 4 kOperand: '(' . iExpression ')'
//   *
//   * '('  shift, and go to state 1
//   * literal ?  shift, and go to state 2
//   *
//   * kOperand ? go to state 3
//   * kExpr ? go to state 5
//   */
//  StateResult State1(TkCursorT& c) {
//    if (c.IsAnOperand()) {
//      PrintStep(c, 0, "iOperand: kLParen * iExpression kRParen", "L[0] ? kInt | kIdent -> [Shift] [Goto 2]");
//      Shift(c, eClosureState::kOperand);
//      auto tmp = State2(c);
//      if (!tmp) return ClResBool::Failure{tmp.error()};
//      return Reduce1(c, tmp.value());
//    } else if (c.TypeIs(eTk::kLParen)) {
//      PrintStep(c, 0, "iOperand: kLParen * iExpression kRParen", "L[0] ? kLParen -> [Shift] [Goto 1]");
//      Shift(c, eClosureState::kOpenScope);
//      auto tmp = State1(c);
//      if (!tmp) return ClResBool::Failure{tmp.error()};
//      return Reduce1(c, tmp.value());
//    } else {
//      return DEBUG_FAIL("Primary expression must begin with an operand, prefix or subexpr.");
//    }
//  }
//  StateResult Reduce1(TkCursorT& c, const StateResult& last_prod) {
//    switch (last_prod.value()->state) {
//      case eClosureState::kExpr:
//        PrintStep(c, 1, "iOperand: kLParen * iExpression kRParen", "S[N] ? iExpression -> [Goto 5]");
//        return Reduce1(c, State5(c, last_prod));
//      case eClosureState::kOperand:
//        PrintStep(c, 1, "iOperand: kLParen * iExpression kRParen", "S[N] ? iOperand -> [Goto 3]");
//        return Reduce1(c, State3(c, last_prod));
//      case eClosureState::kSubexpr:
//        PrintStep(c, 1, "iOperand: kLParen * iExpression kRParen", "S[N] ? iSubexpr -> [Goto 4]");
//        return Reduce1(c, State4(c, last_prod));
//      case eClosureState::kEndProduct:
//        PrintStep(c, 1, "iOperand: kLParen * iExpression kRParen", "S[N] ? kEndProduct -> [Accept]");
//        return LastClosure();
//      default:
//        return DEBUG_FAIL("Unexpected production.");
//    }
//  }
//
//  // 2 kOperand : literal .
//  StateResult State2(TkCursorT& in) {
//    PrintStep(in, 2, "iOperand: : kInt | kIdent *", "[Reduce] S[N]-> iOperand [Accept]");
//    stack_.push_back({eClosureState::kOperand, PopStack().ast});
//    return LastClosure();
//  }
//
//  // 3 kExpr: kOperand .
//  //   $default  reduce using rule 3 (expr)
//  StateResult State3(TkCursorT& in, const StateResult& last_prod) {
//    PrintStep(in, 3, "iExpression: : iOperand *", "[Reduce] S[N]-> iExpression [Accept]");
//    stack_.push_back({eClosureState::kExpr, PopStack().ast});
//    return LastClosure();
//  }
//
//  // 4
//  /*
//   * 0 $accept: expr . $end
//   *
//   * $end  shift, and go to state 6
//   */
//  StateResult State4(TkCursorT& in, const StateResult& last_prod) {
//    if (in.AtEnd()) {
//      PrintStep(in, 4, "iPrimaryExpression: iExpression * $EOF", "L[0] ? $EOF -> [Shift] [Goto 6]");
//      return State6(in);
//    }
//    return DEBUG_FAIL("Not Implemented.");
//  }
//
//  // 5
//  /*
//   * 4 kSubexpr : '(' expr . ')'
//   *
//   * ')'  shift, and go to state 7
//   */
//  StateResult State5(TkCursorT& in, const StateResult& last_prod) {
//    if (in.TypeIs(eTk::kRParen)) {
//      PrintStep(in, 4, "iPrimaryExpression: kLParen iExpression * kRParen", "L[0] ? kLParen -> [Shift] [Goto 7]");
//      Shift(in, eClosureState::kEndScope);
//      return State7(in);
//    }
//    return DEBUG_FAIL("Not Implemented.");
//  }
//
//  // 6
//  /*
//   * 0 $accept: expr $end .
//   *
//   * $default  accept
//   */
//  StateResult State6(TkCursorT& in) {
//    PrintStep(in, 4, "iPrimaryExpression: iExpression $EOF *", "[Reduce] S[N]-> iPrimaryExpression [Accept]");
//    stack_.push_back({eClosureState::kEndProduct, PopStack().ast});
//    return LastClosure();
//  }
//
//  // 7
//  /*
//   * 4 kSubexpr : '(' expr ')' .
//   *
//   * $default  reduce using rule 4 (term)
//   */
//  StateResult State7(TkCursorT& in) {
//    PrintStep(in, 4, "iSubexpression: kLParen iExpression kRParen *",
//              "[Reduce] --S[N] , S[N]-> iPrimaryExpression , --S[N] [Accept]");
//    stack_.pop_back();          // pop ')'
//    Ast expr = PopStack().ast;  // Pop expr
//    stack_.pop_back();          // '('
//    stack_.push_back({eClosureState::kSubexpr, expr});
//    return LastClosure();
//  }
//};
// struct LRParser2 {
//  using ClResBool = CompilerProcessResult<bool>;
//  enum class eClosureState {
//    kUnknown,
//    kZeroProduct,
//    kEndProduct,
//    // Terms
//    kOperand,
//
//    // Nonterms
//    kEndScope,
//    kOpenScope,
//    kUnaryPrefixOp,
//    kUnaryPostfixOp,
//    kLeftBinaryOp,
//    kRightBinaryOp,
//  };
//
//  static constexpr bool IsTerminal(eClosureState state) {
//    using enum eClosureState;
//    switch (state) {
//      case kOperand:
//        return true;
//      case kEndProduct:
//      case kEndScope:
//      case kOpenScope:
//      case kUnaryPostfixOp:
//      case kUnaryPrefixOp:
//      case kLeftBinaryOp:
//      case kRightBinaryOp:
//      case kZeroProduct:
//      case kUnknown:
//      default:
//        return false;
//    }
//  }
//  struct Closure {
//    eClosureState state{eClosureState::kUnknown};
//    Ast ast{eAst::kNONE};
//  };
//  Vec<Closure> rdstack_;
//  Vec<Closure>::iterator rdback_;
//  bool done_;
//
// private:
//  // Scans the stack from top to bottom returning an iterator to the first non-terminal.
//  CND_CX Vec<Closure>::iterator FindLastNonterminal(Vec<Closure>& v) {
//    return std::find_if(v.rbegin(), v.rend(), [](auto& a) -> bool { return !IsTerminal(a.state); }).base() - 1;
//  }
//
//  // Scans the stack from top to bottom returning an iterator to the first production matching the predicate.
//  CND_CX Vec<Closure>::iterator FindLast(Vec<Closure> v, bool(pred)(Closure&)) {
//    return std::find_if(v.rbegin(), v.rend(), pred).base();
//  }
//
//  // Scans the stack from top to bottom returning an iterator to the consecutive productions matching the predicate.
//  CND_CX Vec<Vec<Closure>::iterator> FindLastConsecutive(Vec<Closure> v, bool(pred)(Closure&)) {
//    Vec<Vec<Closure>::iterator> acc;
//    for (auto it = v.rbegin(); it != v.rend(); it++) {
//      if (pred(*it))
//        acc.push_back(std::prev(it.base()));
//      else
//        break;
//    }
//    return acc;
//  }
//
// public:
//  CND_CX LRPrsResT Parse(TkCursorT lookahead) {
//    Ast node;
//    rdstack_.clear();
//    rdstack_.push_back(Closure{eClosureState::kZeroProduct});
//    done_ = false;
//
//    while (!lookahead.AtEnd()) {
//      auto last_nonterm = *FindLastNonterminal(rdstack_);
//      if (rdstack_.back().state == eClosureState::kZeroProduct || rdstack_.back().state == eClosureState::kOpenScope
//      ||
//          rdstack_.back().state == eClosureState::kLeftBinaryOp) {
//        if (lookahead.IsAnOperand()) {
//          ActionShift(eClosureState::kOperand, lookahead);
//        } else if (lookahead.Operation() == eOperation::Prefix) {
//          ActionShift(eClosureState::kUnaryPrefixOp, lookahead);
//        } else if (lookahead.TypeIs(eTk::kLParen)) {
//          ActionShift(eClosureState::kOpenScope, lookahead);
//        } else {
//          return DEBUG_FAIL("Primary expression must begin with an operand, prefix or subexpr.");
//        }
//      }
//      //
//      else if (rdstack_.back().state == eClosureState::kOperand || rdstack_.back().state == eClosureState::kEndScope)
//      {
//        if (lookahead.Operation() == eOperation::Binary) {
//          if (lookahead.Assoc() == eAssoc::Left)
//            ActionShift(eClosureState::kLeftBinaryOp, lookahead);
//          else
//            ActionShift(eClosureState::kRightBinaryOp, lookahead);
//        } else if (lookahead.TypeIs(eTk::kRParen)) {
//          ActionShift(eClosureState::kEndScope, lookahead);
//        } else if (lookahead.Operation() == eOperation::Postfix) {
//          ActionShift(eClosureState::kUnaryPrefixOp, lookahead);
//        } else {
//          return DEBUG_FAIL("Invalid token following operand.");
//        }
//      } else
//        return DEBUG_FAIL("Not implemented.");
//
//      while (FindLastNonterminal(rdstack_) != rdstack_.end() && rdstack_.size() > 1 &&
//             (GetAstPriority(last_nonterm.ast.type) > GetAstPriority(rdstack_.back().ast.type) ||
//              (GetAstPriority(last_nonterm.ast.type) == GetAstPriority(rdstack_.back().ast.type) &&
//               GetAstAssoc(last_nonterm.ast.type) == eAssoc::Left) ||
//              last_nonterm.state == eClosureState::kEndScope))
//        ActionReduce(lookahead);
//    }
//
//    rdstack_.push_back({eClosureState::kEndProduct});
//    while (rdstack_.size() > 1) ActionReduce(lookahead);
//    node = rdstack_.back().ast;
//    return node;
//  }
//
//  CND_CX void ActionShift(eClosureState state, TkCursorT& lookahead) {
//    rdstack_.push_back({state, lookahead});
//    lookahead.Advance();
//  }
//
//  CND_CX ClResBool ActionReduce(TkCursorT& c) {
//    if (rdstack_.back().state == eClosureState::kEndProduct) {
//      auto end_product = rdstack_.back();
//      rdstack_.pop_back();
//      auto last_nonterm = FindLastNonterminal(rdstack_);
//
//      // End of parsing. Perform final reduction and return early.
//      if (last_nonterm->state == eClosureState::kZeroProduct) {
//        Ast tmp = rdstack_.back().ast;
//        rdstack_.pop_back();
//        rdstack_.pop_back();
//        rdstack_.push_back({eClosureState::kOperand, tmp});
//        return ClResBool{};
//      }
//
//      // Else reduce next and push the end product back onto stack.
//      if (last_nonterm->state == eClosureState::kLeftBinaryOp) {
//        ReduceBinaryLeftOp();
//      } else if (rdstack_.back().state == eClosureState::kEndScope) {
//        ActionReduce(c);  // Reduce the scope first.
//      } else {
//        return DEBUG_FAIL("Not Implemented");
//      }
//      rdstack_.push_back(end_product);
//    } else if (rdstack_.back().state == eClosureState::kEndScope) {
//      auto end_scope = rdstack_.back();
//      rdstack_.pop_back();
//      auto last_nonterm = FindLastNonterminal(rdstack_);
//
//      if (last_nonterm->state == eClosureState::kOpenScope) {
//        Ast tmp = rdstack_.back().ast;
//        rdstack_.pop_back();
//        rdstack_.pop_back();
//        rdstack_.push_back({eClosureState::kOperand, tmp});
//        return ClResBool{};
//      }
//
//      if (last_nonterm->state == eClosureState::kLeftBinaryOp) {
//        ReduceBinaryLeftOp();
//      } else {
//        return DEBUG_FAIL("Not Implemented");
//      }
//      rdstack_.push_back(end_scope);
//    } else if (rdstack_.back().state == eClosureState::kLeftBinaryOp) {
//      auto end_product = rdstack_.back();
//      rdstack_.pop_back();
//      auto last_nonterm = FindLastNonterminal(rdstack_);
//      if (last_nonterm->state == eClosureState::kLeftBinaryOp) {
//        ReduceBinaryLeftOp();
//      } else if (last_nonterm->state == eClosureState::kEndScope) {
//        ActionReduce(c);
//      } else {
//        return DEBUG_FAIL("Not Implemented");
//      }
//      rdstack_.push_back(end_product);
//    }
//    return ClResBool{true};
//    //// Pop and store all closures ahead of the nonterm in the frame stack.
//    // Vec<Closure> frame_stack{};
//    // std::copy(nonterm + 1, rdstack_.end(), frame_stack.begin());
//    // for (auto i = std::distance(nonterm + 1, rdstack_.end()); i != 0; i--) rdstack_.pop_back();
//
//    //// Pop the nonterm off the stack.
//    // Ast construct = nonterm->ast;
//    // rdstack_.pop_back();
//
//    //// Reduce based on closure automaton logic.
//    // if (GetAstOperation(construct.type) == eOperation::Binary) {
//    //   if (GetAstAssoc(construct.type) == eAssoc::Left) {
//    //     // Get RHS term from the frame stack.
//    //     construct.PushBack(frame_stack.back().ast);
//    //     frame_stack.pop_back();
//
//    //    // Get LHS term from the read stack.
//    //    construct.PushBack(rdstack_.back().ast);
//    //    rdstack_.pop_back();
//    //  }  // Right associative
//    //  else {
//    //    // Get RHS term from the frame stack.
//    //    auto rhs = frame_stack.back().ast;
//    //    frame_stack.pop_back();
//
//    //    // If the next closure is of equal priority and associativity then reduce it first.
//    //    auto next_nonterm = FindLastNonterminal(rdstack_);
//    //    if (!rdstack_.empty() && GetAstPriority(next_nonterm->ast.type) == GetAstPriority(construct.type) &&
//    //        GetAstAssoc(next_nonterm->ast.type) == GetAstAssoc(construct.type)) {
//    //      ActionReduce(next_nonterm,c);
//    //    }
//
//    //    // Get LHS term from the read stack.
//    //    construct.PushBack(rdstack_.back().ast);
//    //    rdstack_.pop_back();
//
//    //    // Store the RHS operand.
//    //    construct.PushBack(rhs);
//    //  }
//    //} else if (GetAstOperation(construct.type) == eOperation::Prefix) {
//    //  // Get RHS term from the frame stack.
//    //  construct.PushBack(frame_stack.back().ast);
//    //  frame_stack.pop_back();
//    //} else if (GetAstOperation(construct.type) == eOperation::Postfix) {
//    //  // Get LHS term from the read stack.
//    //  construct.PushBack(rdstack_.back().ast);
//    //  rdstack_.pop_back();
//    //}
//    //// Push the generated closure.
//    // rdstack_.push_back({eClosureState::kOperand,construct});
//    //// Push back the remaining frame stack.
//    // rdstack_.append_range(std::ranges::subrange(frame_stack.rbegin(), frame_stack.rend()));
//  }
//  CND_CX void ReduceBinaryLeftOp() {
//    Ast rhs = rdstack_.back().ast;
//    rdstack_.pop_back();
//    Ast op = rdstack_.back().ast;
//    rdstack_.pop_back();
//    Ast lhs = rdstack_.back().ast;
//    rdstack_.pop_back();
//    op.PushBack(lhs);
//    op.PushBack(rhs);
//    rdstack_.push_back({eClosureState::kOperand, op});
//  }
//};
// class LRParser {
//  using ClResVoid = CompilerProcessResult<bool>;
//  enum class eTkBinding { kOperative, kOperator, kNone };
//  eTkBinding next_binding_ = {eTkBinding::kOperative};
//  bool is_first_operator_ = {true};
//  bool is_done_ = {false};
//  Vec<Ast> buffer_;
//  Vec<Pair<TkIterT, TkIterT>> closures_;
//  Vec<std::variant<TkIterT, Ast>> tkbuf_;
//  Ast parsed_{eAst::kNONE};
//
// public:
//  CND_CX LRPrsResT Parse(TkCursorT c) {
//    while (!is_done_) {
//      ClResVoid action_result = ChooseAction(c);
//      if (!action_result) return LRPrsResT::Failure(action_result.Error());
//    }
//    return parsed_;
//  }
//
//  CND_CX ClResVoid ActionSkip(TkCursorT& c) {
//    // Check for an open paren-> Subexpression
//    // Resolve the subexpr first.
//    // Push resolved result to output.
//    if (c.TypeIs(eTk::kLParen)) {
//      auto scope = FindParen(c);
//      if (!scope) {
//        return ClResVoid::Failure(DEBUG_MSG("Invalid Scope."));
//      }
//      // Resolve the inside of the parentheses.
//      auto subexpr = LRParser{}.Parse(scope.Value().Contained());
//      if (!subexpr) {
//        subexpr.Error().Data().push_back(DEBUG_MSG("Could not resolve subexpression."));
//        return ClResVoid::Failure(subexpr.Error());
//      }
//      tkbuf_.push_back(subexpr.value());
//      for (auto i = c.Iter(); i != scope.Value().End(); i++) {
//        c.Advance();
//      }
//      return ClResVoid{};
//    }
//
//    tkbuf_.push_back(std::variant<TkIterT, Ast>{}.emplace<0>(c.Iter()));
//    c.Advance();
//    return ClResVoid{};
//  }
//
//  CND_CX ClResVoid ActionStore(TkCursorT& c) {
//    auto op = c.Iter();
//    closures_.push_back(std::make_pair(op, op + 1));
//    c.Advance();
//    return ClResVoid{};
//  }
//
//  CND_CX ClResVoid ActionCheck(TkCursorT& c) {
//    // Resolve all closures at the end.
//    if (c.AtEnd()) {
//      while (closures_.size() > 0) ResolveLastClosure();
//      return ClResVoid{};
//    }
//    // If: (Head >= Last Closure) => Store
//    if (c.Priority() >= closures_.back().first->Priority()) ActionStore(c);
//    // Else While:( Head < Last Closure ) => Resolve Next
//    else {
//      while (!closures_.empty() && c.Priority() < closures_.back().first->Priority()) ResolveLastClosure();
//      ActionStore(c);
//    }
//    return ClResVoid{};
//  }
//
//  CND_CX ClResVoid ResolveLastClosure() {
//    auto& op = *(closures_.back().first);
//    if (op.Operation() == eOperation::Prefix)
//      return ResolvePrefix();
//    else if (op.IsRScope())
//      return ResolveScope();
//    else if (op.Operation() == eOperation::Postfix)
//      return ResolvePostfix();
//    else if (op.Operation() == eOperation::Binary && op.Assoc() == eAssoc::Left)
//      return ResolveBinaryLeftAssoc();
//    else if (closures_.back().first->Operation() == eOperation::Binary &&
//             closures_.back().first->Assoc() == eAssoc::Right)
//      return ResolveBinaryRightAssoc();
//    else
//      return DEBUG_FAIL("ActionResolveLast: Invalid token type in closure.Must be an operator.");
//  }
//
//  CND_CX ClResVoid ResolveBinaryLeftAssoc() {
//    Ast construct = closures_.back().first;
//
//    std::variant<TkIterT, Ast> rhs{tkbuf_.back()};
//    tkbuf_.pop_back();
//    std::variant<TkIterT, Ast> lhs{tkbuf_.back()};
//    tkbuf_.pop_back();
//
//    if (std::holds_alternative<Ast>(lhs))
//      construct.PushBack(std::get<Ast>(lhs));
//    else
//      construct.PushBack(std::get<TkIterT>(lhs));
//
//    if (std::holds_alternative<Ast>(rhs))
//      construct.PushBack(std::get<Ast>(rhs));
//    else
//      construct.PushBack(std::get<TkIterT>(rhs));
//
//    closures_.pop_back();
//    tkbuf_.push_back(construct);
//    return ClResVoid{};
//  }
//
//  CND_CX ClResVoid ResolveBinaryRightAssoc() {
//    Ast construct = closures_.back().first;
//    Tk last_closure_type = *closures_.back().first;
//
//    // Store the RHS operand.
//    std::variant<TkIterT, Ast> rhs{tkbuf_.back()};
//    tkbuf_.pop_back();
//    closures_.pop_back();
//
//    // If the next closure is of equal priority and associativity then bind it first.
//    if (!closures_.empty() && closures_.back().first->Priority() == last_closure_type.Priority() &&
//        closures_.back().first->Assoc() == last_closure_type.Assoc()) {
//      ResolveBinaryRightAssoc();
//    }
//
//    // Get the LHS operand.
//    std::variant<TkIterT, Ast> lhs{tkbuf_.back()};
//    tkbuf_.pop_back();
//    if (std::holds_alternative<Ast>(lhs))
//      construct.PushBack(std::get<Ast>(lhs));
//    else
//      construct.PushBack(std::get<TkIterT>(lhs));
//
//    // Store the RHS operand.
//    if (std::holds_alternative<Ast>(rhs))
//      construct.PushBack(std::get<Ast>(rhs));
//    else
//      construct.PushBack(std::get<TkIterT>(rhs));
//
//    tkbuf_.push_back(construct);
//    return ClResVoid{};
//  }
//
//  CND_CX ClResVoid ResolvePrefix() {
//    Ast construct = closures_.back().first;
//    if (std::holds_alternative<Ast>(tkbuf_.back()))
//      construct.PushBack(std::get<Ast>(tkbuf_.back()));
//    else
//      construct.PushBack(std::get<TkIterT>(tkbuf_.back()));
//    closures_.pop_back();
//    tkbuf_.pop_back();
//    tkbuf_.push_back(construct);
//    return ClResVoid{};
//  }
//
//  CND_CX ClResVoid ResolvePostfix() {
//    Ast construct = closures_.back().first;
//    if (std::holds_alternative<Ast>(tkbuf_.back()))
//      construct.PushBack(std::get<Ast>(tkbuf_.back()));
//    else
//      construct.PushBack(std::get<TkIterT>(tkbuf_.back()));
//    closures_.pop_back();
//    tkbuf_.pop_back();
//    tkbuf_.push_back(construct);
//    return ClResVoid{};
//  }
//
//  CND_CX ClResVoid ResolveScope() {
//    Ast construct;
//    auto scope_type = closures_.back().first->Type();
//    if (scope_type == eTk::kRParen)
//      construct = Ast{eAst::kFunctionCall, closures_.back().first, closures_.back().first + 1};
//    else if (scope_type == eTk::kRBrace)
//      construct = Ast{eAst::kListingOperator, closures_.back().first, closures_.back().first + 1};
//    else if (scope_type == eTk::kRBracket)
//      construct = Ast{eAst::kIndexOperator, closures_.back().first, closures_.back().first + 1};
//    else
//      return DEBUG_FAIL("Unknown Scope Type");
//    closures_.pop_back();
//
//    bool empty_scope = true;
//    while (!closures_.empty() && !IsTkRScopeOf(closures_.back().first->Type(), scope_type)) {
//      ResolveLastClosure();
//      empty_scope = false;
//    }
//    construct.src_begin = closures_.back().first;  // Updated the src beg in ast to the beginning of scope.
//    closures_.pop_back();                          // Pop the R scope.
//
//    // If scope was empty no ast was produced so we only pop a single arg, the caller.
//    if (empty_scope) {
//      // Add the last resolved ast as the branch of this scope.
//      if (std::holds_alternative<Ast>(tkbuf_.back()))
//        construct.PushBack(std::get<Ast>(tkbuf_.back()));
//      else
//        construct.PushBack(std::get<TkIterT>(tkbuf_.back()));
//      tkbuf_.pop_back();
//      tkbuf_.push_back(construct);
//      return ClResVoid{};
//    }
//
//    // Get the resolved ast contained in the scope.
//    if (std::holds_alternative<Ast>(tkbuf_.back()))
//      construct.PushBack(std::get<Ast>(tkbuf_.back()));
//    else
//      construct.PushBack(std::get<TkIterT>(tkbuf_.back()));
//    tkbuf_.pop_back();
//    tkbuf_.push_back(construct);
//
//    // Get the caller operand of the scope.
//    if (std::holds_alternative<Ast>(tkbuf_.back()))
//      construct.PushBack(std::get<Ast>(tkbuf_.back()));
//    else
//      construct.PushBack(std::get<TkIterT>(tkbuf_.back()));
//    tkbuf_.pop_back();
//    tkbuf_.push_back(construct);
//
//    return ClResVoid{};
//  }
//
//  CND_CX Vec<Vec<Pair<TkIterT, TkIterT>>::iterator> FindClosureReverseConsecutive(
//      std::function<bool(Vec<Pair<TkIterT, TkIterT>>::iterator, Vec<Pair<TkIterT, TkIterT>>::iterator)>&& condition) {
//    auto it = closures_.rbegin();
//    auto end = closures_.rend();
//    std::vector<Vec<Pair<TkIterT, TkIterT>>::iterator> accumulated_iterators;
//    while (it != end) {
//      auto next = std::next(it);
//      if (condition(std::prev(it.base()), std::prev(next.base()))) {
//        accumulated_iterators.push_back(std::prev(it.base()));
//      } else {
//        accumulated_iterators.push_back(std::prev(it.base()));
//        break;
//      }
//      ++it;
//    }
//    return accumulated_iterators;
//  }
//
//  /// @brief Decides which action to take. Main loop of the closure resolver. Action will be skip , store, or check.
//  ///
//  /// At the end a check is performed. Else...
//  /// Action is chosen based on expected head token which will be one of:
//  ///   Operative :operand,prefix operator,open scope(subexpr).
//  ///   Operator : binary operator, postfix operator, or open paren(function call), or open brace(index operator)
//  /// Based on the token type of operand or operator:
//  ///   OPERATOR-> First? -> Store. All following operators will be checks.
//  ///   OPERAND-> Skip. Multi-token operands trigger repeated skips.
//  ///
//  /// SPECIAL CASE[Unary Minus] -> If binary '-' is encountered.
//  ///                           -> Bind tightly to the following operand.
//  ///                           -> Assert Numeric Literal. Emplace - into
//  ///                           operand.
//  ///                           -> Skip Both.
//  ///                         // If the following is non number, error.
//  CND_CX ClResVoid ChooseAction(TkCursorT& c) {
//    if (c.AtEnd()) {
//      ClResVoid check_result = ActionCheck(c);
//      if (!check_result) {
//        check_result.Error().Data().push_back(DEBUG_MSG("Could not resolve closures."));
//        return CompilerProcessResult<bool>::Failure(check_result.Error());
//      }
//      if (std::holds_alternative<Ast>(tkbuf_.back()))
//        parsed_ = std::get<Ast>(tkbuf_.back());
//      else
//        parsed_ = std::get<TkIterT>(tkbuf_.back());
//      is_done_ = true;
//    } else {
//      // Operative
//      if (next_binding_ == eTkBinding::kOperative) {
//        // Operand.
//        if (c.Operation() == eOperation::NONE || c.TypeIs(eTk::kLParen)) {
//          auto skip_result = ActionSkip(c);
//          if (!skip_result) {
//            skip_result.Error().Data().push_back(DEBUG_MSG("Could not skip operand."));
//            return CompilerProcessResult<bool>::Failure(skip_result.Error());
//          }
//          next_binding_ = eTkBinding::kOperator;
//        }
//        // Prefix Operator.
//        else if (c.Operation() == eOperation::Prefix) {
//          if (is_first_operator_) {
//            auto store_result = ActionStore(c);
//            if (!store_result) {
//              store_result.Error().Data().push_back(DEBUG_MSG("Could not store prefix operator."));
//              return CompilerProcessResult<bool>::Failure(store_result.Error());
//            }
//          } else {
//            auto check_result = ActionCheck(c);
//            if (!check_result) {
//              check_result.Error().Data().push_back(DEBUG_MSG("Could not check prefix operator."));
//              return CompilerProcessResult<bool>::Failure(check_result.Error());
//            }
//          }
//          if (is_first_operator_) is_first_operator_ = false;
//          next_binding_ = eTkBinding::kOperative;
//        }
//        // Expected operative got operator -> User Error.
//        else if (c.Operation() == eOperation::Binary || c.Operation() == eOperation::Postfix) {
//          return DEBUG_FAIL("Invalid token type encountered, operator following operator.");
//        }
//        // Else user error -> this token does not belong in a value expression.
//        else {
//          return DEBUG_FAIL("Token type not valid for a primary expression.");
//        }
//      }
//      // Operator
//      else if (next_binding_ == eTkBinding::kOperator) {
//        // Open Paren, Open Bracket, Postfix Operator
//        //    -> Check, next is Operator.
//        if (c.TypeIs(eTk::kLParen) || c.TypeIs(eTk::kLBracket) || c.TypeIs(eTk::kLBrace) ||
//            c.Operation() == eOperation::Postfix) {
//          UPtr<CompilerProcessResult<bool>> action_result;
//          if (is_first_operator_) {
//            action_result = std::make_unique<CompilerProcessResult<bool>>(ActionStore(c));
//            if (is_first_operator_) is_first_operator_ = false;
//            next_binding_ = eTkBinding::kOperator;
//          } else {
//            action_result = std::make_unique<CompilerProcessResult<bool>>(ActionCheck(c));
//            if (is_first_operator_) is_first_operator_ = false;
//            next_binding_ = eTkBinding::kOperator;
//          }
//        }
//        // Binary Operator -> Check, next is Operative.
//        else if (c.Operation() == eOperation::Binary) {
//          UPtr<CompilerProcessResult<bool>> action_result;
//          if (is_first_operator_) {
//            action_result = std::make_unique<CompilerProcessResult<bool>>(ActionStore(c));
//          } else {
//            action_result = std::make_unique<CompilerProcessResult<bool>>(ActionCheck(c));
//          }
//          if (is_first_operator_) is_first_operator_ = false;
//          next_binding_ = eTkBinding::kOperative;
//        }
//        // Prefix -> user Error, prefix following operand.
//        else if (c.Operation() == eOperation::Prefix) {
//          return DEBUG_FAIL("Invalid token type encountered, prefix following operand.");
//        }
//        // Operand -> user Error, operand following operand.
//        else if (c.Operation() == eOperation::NONE) {
//          return DEBUG_FAIL("Invalid token type encountered, operand following operand.");
//        }
//        // Else user error -> this token does not belong in a value
//        // expression.
//        else {
//          return DEBUG_FAIL("Token type not valid for a primary expression.");
//        }
//      }
//      // This should never happen -> critical logic error. next_binding_ enum set to invalid value.
//      else {
//        return DEBUG_FAIL(
//            "[CRITICAL LOGIC ERROR] Invalid next expected head token enumeration type. eNextExpectedHeadToken enum set
//            " "to invalid value.");
//      }
//    }
//    return ClResVoid{true};
//  }
//};

// CND_CX LRPrsResT ExprParser::Parse(TkCursorT c) {
//   auto closures_res = CreateAndResolveClosures(c);
//   if (!closures_res) return LLPrsResT::Failure(closures_res.Error());
//   Span<const Tk> closures = closures_res.Value();
//
//   TkCursorT intermediate_expr(closures.cbegin(), closures.cend());
//   return ParseImpl({closures.begin(), closures.end()});
// }
//
// CND_CX void ExprParser::FirstOperatorSwitch() {
//   if (is_first_operator_) is_first_operator_ = false;
// }
//
// CND_CX void ExprParser::ResolvePrefix() {
//   // Find consecutive prefix closures.
//   auto nclosures = closure_buffer_
//                        .FindClosureReverseConsecutive([](ClosureListIter a, ClosureListIter b) -> bool {
//                          return (a->Priority() == b->Priority()) && (a->Operation() == b->Operation());
//                        })
//                        .size();
//   auto closure_count = nclosures;
//   while (nclosures > 0) {
//     closure_buffer_.StreamInsertBeforeClosure(std::prev(closure_buffer_.LastClosure(), nclosures - 1), kOpenParenTk);
//     closure_buffer_.StreamPushBack(kCloseParenTk);
//     nclosures--;
//   }
//
//   for (std::size_t i = 0; i < closure_count; i++) {
//     closure_buffer_.PopClosure();
//   }
// }
//
// CND_CX void ExprParser::ResolvePostfix() {
//   // Find consecutive postfix closures.
//   auto nclosures = closure_buffer_
//                        .FindClosureReverseConsecutive([](ClosureListIter a, ClosureListIter b) -> bool {
//                          return (a->Priority() == b->Priority()) && (a->Operation() == b->Operation());
//                        })
//                        .size();
//   auto closure_count = nclosures;
//
//   while (nclosures > 0) {
//     closure_buffer_.StreamInsertAfterClosure(std::prev(closure_buffer_.LastClosure(), closure_count), kOpenParenTk);
//
//     closure_buffer_.StreamInsertAfterClosure(std::prev(closure_buffer_.LastClosure(), nclosures - 1), kCloseParenTk);
//
//     nclosures--;
//   }
//
//   for (std::size_t i = 0; i < closure_count; i++) {
//     closure_buffer_.PopClosure();
//   }
// };
//
// CND_CX void ExprParser::ResolveBinaryLeftAssoc() {
//   Vec<ClosureListIter> binary_closures = closure_buffer_.FindClosureReverseConsecutiveAndIgnore(
//       [](ClosureListIter a, ClosureListIter b) -> bool {
//         return (a->Priority() == b->Priority()) && (a->Operation() == b->Operation()) && (a->Assoc() == b->Assoc());
//       },
//       [](ClosureListIter a) -> bool {
//         return ((a->Priority() == ePriority::Postfix) || (a->Priority() == ePriority::Prefix));
//       });
//
//   // Resolve first closure.(last in the list)
//   ClosureListIter open_paren_location = std::prev(binary_closures.back());
//
//   // skip any postfix/prefix closures when looking for the open paren loc.
//   while (open_paren_location->Priority() == ePriority::Postfix ||
//          open_paren_location->Priority() == ePriority::Prefix) {
//     open_paren_location = std::prev(open_paren_location);
//   }
//
//   closure_buffer_.StreamInsertAfterClosure(open_paren_location, {eTk::kLParen, "("});
//   closure_buffer_.StreamPushBack({eTk::kRParen, ")"});
//
//   // Resolve the rest,skip last in loop.
//   Vec<ClosureListIter>::iterator close_paren_location_iter = binary_closures.begin();
//   while (close_paren_location_iter != std::prev(binary_closures.end())) {
//     closure_buffer_.StreamInsertAfterClosure(open_paren_location, {eTk::kLParen, "("});
//
//     closure_buffer_.StreamInsertBeforeClosure(*close_paren_location_iter, {eTk::kRParen, ")"});
//     close_paren_location_iter = std::next(close_paren_location_iter);
//   }
//   Ast testr = {eAst::kNONE, open_paren_location->Front(), (**close_paren_location_iter)};
//   // Pop all collected closures.
//   for (ClosureListIter closure_iter : binary_closures) {
//     closure_buffer_.PopClosure(closure_iter);
//   }
// };
//
// CND_CX void ExprParser::ResolveBinaryRightAssoc() {
//   Vec<ClosureListIter> binary_closures = closure_buffer_.FindClosureReverseConsecutiveAndIgnore(
//       [](ClosureListIter a, ClosureListIter b) -> bool {
//         return (a->Priority() == b->Priority()) && (a->Operation() == b->Operation()) && (a->Assoc() == b->Assoc());
//       },
//       [](ClosureListIter a) -> bool {
//         return ((a->Priority() == ePriority::Postfix) || (a->Priority() == ePriority::Prefix));
//       });
//
//   // Resolve first closure.(last in the list)
//   closure_buffer_.StreamInsertAfterClosure(std::prev(binary_closures.back()), {eTk::kLParen, "("});
//   closure_buffer_.StreamPushBack({eTk::kRParen, ")"});
//   // Resolve the rest.
//   Vec<ClosureListIter>::iterator open_paren_location_iter = std::next(binary_closures.begin());
//   while (open_paren_location_iter != binary_closures.end()) {
//     closure_buffer_.StreamInsertAfterClosure(*open_paren_location_iter, {eTk::kLParen, "("});
//     closure_buffer_.StreamPushBack({eTk::kRParen, ")"});
//     open_paren_location_iter = std::next(open_paren_location_iter);
//   }
//   // Pop all collected closures.
//   for (ClosureListIter closure_iter : binary_closures) {
//     closure_buffer_.PopClosure(closure_iter);
//   }
// };
//
// CND_CX void ExprParser::ResolveBinary() {
//   // Resolve associative binary if there are repeated equivalent priority
//   // closures before the last.
//   if (closure_buffer_.LastClosure()->Priority() == std::prev(closure_buffer_.LastClosure())->Priority()) {
//     if (closure_buffer_.LastClosure()->Assoc() == eAssoc::Left) {
//       ResolveBinaryLeftAssoc();
//     } else if (closure_buffer_.LastClosure()->Assoc() == eAssoc::Right) {
//       ResolveBinaryRightAssoc();
//     } else {
//       throw "ResolveBinary: Invalid binary token in closure, association "
//             "not left or right.\n";
//     }
//   } else {  // Resolve Singular Binary Closure
//     // Special Case: If the binary closure is higher priority than
//     // prefix/postfix
//     //               Then don't only skip the postfix.
//
//     ClosureListIter open_paren_insertion_loc_iter = std::prev(closure_buffer_.LastClosure());
//
//     // Lower:
//     if (closure_buffer_.LastClosure()->Priority() < ePriority::Postfix) {
//       // If there are postfix/prefix closures-> skip them when
//       // looking for the open paren insertion location.
//       ClosureListIter postfix_it = std::prev(closure_buffer_.LastClosure());
//       while (postfix_it->Priority() == ePriority::Postfix) {
//         postfix_it = std::prev(postfix_it);
//       }
//
//       ClosureListIter prefix_it = postfix_it;
//       if (prefix_it->Priority() == ePriority::Prefix) {
//         while (prefix_it->Priority() == ePriority::Prefix) {
//           prefix_it = std::prev(prefix_it);
//         }
//       }
//
//       open_paren_insertion_loc_iter = prefix_it;
//     }
//     // Higher(will never be equal):
//     else {
//       // If there are postfix/prefix closures-> skip them when
//       // looking for the open paren insertion location.
//       ClosureListIter postfix_it = std::prev(closure_buffer_.LastClosure());
//       while (postfix_it->Priority() == ePriority::Postfix) {
//         postfix_it = std::prev(postfix_it);
//       }
//
//       open_paren_insertion_loc_iter = postfix_it;
//     }
//
//     closure_buffer_.StreamInsertAfterClosure(open_paren_insertion_loc_iter, {eTk::kLParen, "("});
//     closure_buffer_.StreamPushBack({eTk::kRParen, ")"});
//     closure_buffer_.PopClosure();
//   }
// };
//
// CND_CX void ExprParser::ResolveLast() {
//   if (closure_buffer_.LastClosure()->Operation() == eOperation::Prefix) {
//     ResolvePrefix();
//   } else if (closure_buffer_.LastClosure()->Operation() == eOperation::Postfix) {
//     ResolvePostfix();
//   } else if (closure_buffer_.LastClosure()->Operation() == eOperation::Binary) {
//     ResolveBinary();
//   } else {
//     throw "ActionResolveLast: Invalid token type in closure.Must be an "
//                "operator.\n";
//   }
// }
//
// CND_CX CompilerProcessResult<bool> ExprParser::ActionSkip(TkCursorT& c) {
//   // Check for an open paren-> Subexpression
//   // Resolve the subexpr first.
//   // Push resolved result to output.
//   if (c.TypeIs(eTk::kLParen)) {
//     auto scope = FindParen(c);
//     if (!scope) {
//       return "Mismatched parentheses in operand.";
//     } else {
//       // Resolve but do not parse!the inside of the parentheses.
//       auto subexpr = ExprParser().CreateAndResolveClosures(scope.Value().Contained());
//       if (!subexpr) {
//         subexpr.Error().Data().push_back(DEBUG_MSG("Could not resolve subexpression."));
//         return CompilerProcessResult<bool>::Failure(subexpr.Error());
//       }
//       for (const Tk& tk : subexpr.Value()) {
//         closure_buffer_.StreamPushBack(tk);
//       }
//       for (auto i = c.Iter(); i != scope.Value().End(); i++) {
//         c.Advance();
//       }
//       return true;
//     }
//   }
//   // Check if this is a singular token OPERAND.
//   if (c.Operation() == eOperation::NONE || c.TypeIs(eTk::kSub)) {
//     // If unary minus - Emplace unary minus into operand.
//     if (c.TypeIs(eTk::kSub)) {
//       c.Advance();
//       if (c.TypeIs(eTk::kLitInt) || c.TypeIs(eTk::kLitReal)) {
//         Tk negative_number = c.Get();
//         // Here we get the pointer to the operand's data and assume the unary negation sign is from the same source
//         // string. temporary fix, need to find a better solution.
//         negative_number.literal_ = StrView{negative_number.literal_.data() - 1, negative_number.literal_.size() + 1};
//         // negative_number.LiteralMutable() = "-" + negative_number.Literal();
//         closure_buffer_.StreamPushBack(negative_number);
//         c.Advance();
//         return true;
//       } else {
//         return DEBUG_FAIL("Unary minus must be followed by a numeric literal.");
//       }
//     }
//     // Parse single token operand.
//     closure_buffer_.StreamPushBack(c.Get());
//     c.Advance();
//     return true;
//   }
//   return DEBUG_FAIL("Could not read operand token,invalid or unimplemented.");
// }
//
// CND_CX CompilerProcessResult<bool> ExprParser::ActionStore(TkCursorT& c) {
//   // Check for an open paren-> Function Call
//   // Arguments are parsed and resolved in ParseImpl.
//   // Push a closure for the function call.
//   if (c.TypeIs(eTk::kLParen)) {
//     auto scope = FindParen(c);
//     if (not scope) {
//       return "Mismatched parentheses in function call.";
//     } else {
//       closure_buffer_.StreamPushBack(c.Get());
//       auto fn_args_closure_begin = closure_buffer_.LastStreamed();
//       c.Advance();
//       // Store the rest of the function call.
//       for (auto i = c.Iter(); i != scope.Value().End(); i++) {
//         closure_buffer_.StreamPushBack(c.Get());
//         c.Advance();
//       }
//       // Make the closure.
//       closure_buffer_.PushBackClosure(fn_args_closure_begin, closure_buffer_.LastStreamed());
//       return true;
//     }
//   }
//   // Check for an open bracket-> Indexing Operator
//   if (c.TypeIs(eTk::kLBracket)) {
//     auto scope = FindBracket(c);
//     if (not scope) {
//       return "Mismatched bracket in indexing call.";
//     } else {
//       closure_buffer_.StreamPushBack(c.Get());
//       auto fn_args_closure_begin = closure_buffer_.LastStreamed();
//       c.Advance();
//       for (auto i = c.Iter(); i != scope.Value().End(); i++) {
//         closure_buffer_.StreamPushBack(c.Get());
//         c.Advance();
//       }
//       closure_buffer_.PushBackClosure(fn_args_closure_begin, closure_buffer_.LastStreamed());
//       return true;
//     }
//   }
//   // Listing Operator
//   if (c.TypeIs(eTk::kLBrace)) {
//     auto scope = FindBrace(c);
//     if (not scope) {
//       return "Mismatched brace in listing call.";
//     } else {
//       closure_buffer_.StreamPushBack(c.Get());
//       auto fn_args_closure_begin = closure_buffer_.LastStreamed();
//       c.Advance();
//       for (auto i = c.Iter(); i != scope.Value().End(); i++) {
//         closure_buffer_.StreamPushBack(c.Get());
//         c.Advance();
//       }
//       closure_buffer_.PushBackClosure(fn_args_closure_begin, closure_buffer_.LastStreamed());
//       return true;
//     }
//   }
//   // Single Token Operator.
//   if (c.Operation() == eOperation::Binary || c.Operation() == eOperation::Prefix ||
//       c.Operation() == eOperation::Postfix) {
//     closure_buffer_.StreamPushBack(c.Iter());
//     closure_buffer_.PushBackClosure(closure_buffer_.LastStreamed());
//     c.Advance();
//     return true;
//   }
//
//   return "ExprParser::ActionStore: Could not read operator "
//          "token, "
//          "invalid or unimplemented.";
// }
//
// CND_CX CompilerProcessResult<bool> ExprParser::ActionCheck(TkCursorT& c) {
//   if (c.AtEnd()) {
//     while (closure_buffer_.ClosureCount() > 0) {
//       ResolveLast();
//     }
//     return true;
//   } else {
//     if (c.Priority() >= closure_buffer_.LastClosure()->Priority()) {
//       // If: (Head >= Last Closure) => Store
//       ActionStore(c);
//     } else {
//       // While:( Head < Last Closure ) => Resolve Next
//       while (c.Priority() < closure_buffer_.LastClosure()->Priority()) {
//         ResolveLast();
//       }
//       ActionStore(c);
//     }
//     return true;
//   }
// }
//
// CND_CX CompilerProcessResult<bool> ExprParser::ChooseAction(TkCursorT& c) {
//   if (c.AtEnd()) {
//     auto check_result = ActionCheck(c);
//     if (check_result) {
//       is_resolved_ = true;
//       return true;
//     } else {
//       check_result.Error().Data().push_back(DEBUG_MSG("Could not resolve closures."));
//       return CompilerProcessResult<bool>::Failure(check_result.Error());
//     }
//   }
//   // Choose action based on expected head token.
//   else {
//     // Operative-> Looking for an operand,prefix operator,open scope(subexpr)
//     // SPECIAL CASE[Unary Minus] -> If binary '-' is encountered.
//     //                           -> Bind tightly to the following operand.
//     //                           -> Assert Numeric Literal. Emplace - into
//     //                           operand.
//     //                           -> Skip Both.
//     //                         // If the following is non number, error.
//     if (next_expected_head_token_ == eNextExpectedHeadToken::kOperative) {
//       if (c.Operation() == eOperation::NONE || c.TypeIs(eTk::kLParen) || c.TypeIs(eTk::kSub)) {
//         auto skip_result = ActionSkip(c);
//         if (skip_result) {
//           next_expected_head_token_ = eNextExpectedHeadToken::kOperator;
//           return true;
//         } else {
//           skip_result.Error().Data().push_back(DEBUG_MSG("Could not skip operand."));
//           return CompilerProcessResult<bool>::Failure(skip_result.Error());
//         }
//       }
//       // Prefix Operator -> Check Single Prefix Operator.
//       else if (c.Operation() == eOperation::Prefix) {
//         if (is_first_operator_) {
//           auto store_result = ActionStore(c);
//           if (!store_result) {
//             store_result.Error().Data().push_back(DEBUG_MSG("Could not store prefix operator."));
//             return CompilerProcessResult<bool>::Failure(store_result.Error());
//           }
//         } else {
//           auto check_result = ActionCheck(c);
//           if (!check_result) {
//             check_result.Error().Data().push_back(DEBUG_MSG("Could not check prefix operator."));
//             return CompilerProcessResult<bool>::Failure(check_result.Error());
//           }
//         }
//         FirstOperatorSwitch();
//         next_expected_head_token_ = eNextExpectedHeadToken::kOperative;
//         return true;
//       }
//       // expected operand got operator -> user error
//       else if (c.Operation() == eOperation::Binary || c.Operation() == eOperation::Postfix) {
//         return DEBUG_FAIL("Invalid token type encountered, operator following operator.");
//       }
//       // Else user error -> this token does not belong in a value
//       // expression.
//       else {
//         return DEBUG_FAIL("Token type not valid for a primary expression.");
//       }
//     }
//     // Operator
//     // Looking for a binary operator, postfix operator, or open
//     // paren(function call), or open brace(index operator)
//     else if (next_expected_head_token_ == eNextExpectedHeadToken::kOperator) {
//       // Open Paren, Open Bracket, Postfix Operator
//       //    -> Check, next is Operator.
//       if (c.TypeIs(eTk::kLParen) || c.TypeIs(eTk::kLBracket) || c.TypeIs(eTk::kLBrace) ||
//           c.Operation() == eOperation::Postfix) {
//         UPtr<CompilerProcessResult<bool>> action_result;
//         if (is_first_operator_) {
//           action_result = std::make_unique<CompilerProcessResult<bool>>(ActionStore(c));
//           FirstOperatorSwitch();
//           next_expected_head_token_ = eNextExpectedHeadToken::kOperator;
//         } else {
//           action_result = std::make_unique<CompilerProcessResult<bool>>(ActionCheck(c));
//           FirstOperatorSwitch();
//           next_expected_head_token_ = eNextExpectedHeadToken::kOperator;
//         }
//       }
//       // Binary Operator -> Check, next is Operative.
//       else if (c.Operation() == eOperation::Binary) {
//         UPtr<CompilerProcessResult<bool>> action_result;
//         if (is_first_operator_) {
//           action_result = std::make_unique<CompilerProcessResult<bool>>(ActionStore(c));
//         } else {
//           action_result = std::make_unique<CompilerProcessResult<bool>>(ActionCheck(c));
//         }
//         FirstOperatorSwitch();
//         next_expected_head_token_ = eNextExpectedHeadToken::kOperative;
//       }
//       // Prefix -> user Error, prefix following operand.
//       else if (c.Operation() == eOperation::Prefix) {
//         return DEBUG_FAIL("Invalid token type encountered, prefix following operand.");
//       }
//       // Operand -> user Error, operand following operand.
//       else if (c.Operation() == eOperation::NONE) {
//         return DEBUG_FAIL("Invalid token type encountered, operand following operand.");
//       }
//       // Else user error -> this token does not belong in a value
//       // expression.
//       else {
//         return DEBUG_FAIL("Token type not valid for a primary expression.");
//       }
//     }
//     // This should never happen -> critical logic error.
//     // eNextExpectedHeadToken enum set to invalid value.
//     else {
//       return DEBUG_FAIL(
//           "[CRITICAL LOGIC ERROR] Invalid next expected head token enumeration type. eNextExpectedHeadToken enum set
//           " "to invalid value.");
//     }
//   }
// }
//
// CND_CX CompilerProcessResult<Vec<Tk>> ExprParser::CreateAndResolveClosures(TkCursorT c) {
//   while (not is_resolved_) {
//     auto action_result = ChooseAction(c);
//     if (!action_result) {
//       return CompilerProcessResult<Vec<Tk>>::Failure(action_result.Error());
//     }
//   }
//   return CompilerProcessResult<Vec<Tk>>::Success(closure_buffer_.StreamToVector());
// }
//
// CND_CX LRPrsResT ExprParser::ParseImpl(TkCursor<Tk, Span> c) {
//   using enum eTk;
//   Ast out_node;
//
//   if (c.TypeIs(kLParen)) {
//     auto scope = FindParen(c);
//     if (!scope) return LLPrsResT::Failure(scope.Error());
//
//     // 1. Check for redundant paren-> Parse the inside of the paren instead.
//     if (scope->End() == c.End()) return ParseImpl(scope->Contained());
//
//     // 2. Else scope is an operand contained in a subexpression.
//     auto lhs_result = ParseImpl(scope->Contained());
//     if (!lhs_result) return lhs_result;
//     c.Advance(scope->End());
//
//     // 2.1. Postfix
//     if (c.Operation() == eOperation::Postfix) {
//       // 2.1.1. Postfix () -> Function Call
//       if (c.TypeIs(kLParen)) {
//         auto scope = FindParen(c);
//         if (!scope) return LRPrsResT::Failure(scope.Error());
//         auto args = ParseArguments(c);
//         if (!args) return LRPrsResT::Failure(args.Error());
//         out_node = Ast(eAst::kFunctionCall, scope->Begin(), scope->End(), lhs_result.Extract(), args->ast);
//         c.Advance(scope->End());
//       }
//       // 2.1.2. Postfix [] -> Index Operator
//       else if (c.TypeIs(kLBracket)) {
//         auto scope = FindBracket(c);
//         if (!scope) return LRPrsResT::Failure(scope.Error());
//         auto args = ParseIndexingArguments(c);
//         if (!args) return LRPrsResT::Failure(args.Error());
//         out_node = Ast(eAst::kIndexOperator, scope->Begin(), scope->End(), lhs_result.Extract(), args->ast);
//         c.Advance(scope->End());
//       }
//       // 2.1.3. Postfix {} -> Listing Operator
//       else if (c.TypeIs(kLBrace)) {
//         auto scope = FindBrace(c);
//         if (!scope) return LRPrsResT::Failure(scope.Error());
//         auto args = ParseListingArguments(c);
//         if (!args) return LRPrsResT::Failure(args.Error());
//         out_node = Ast(eAst::kListingOperator, scope->Begin(), scope->End(), lhs_result.Extract(), args->ast);
//         c.Advance(scope->End());
//       }
//       // 2.1.4. Postfix Single Token Operator.
//       else {
//         out_node = Ast(c);
//         out_node.PushBack(lhs_result.Extract());
//         c.Advance();
//       }
//     }
//     // 2.2. Binary Operator
//     else if (c.Operation() == eOperation::Binary) {
//       out_node = Ast(c);
//       c.Advance();
//       // Expecting an operand after a binary operator.
//       // 2.2.1. Open Paren -> Subexpr
//       if (c.TypeIs(kLParen)) {
//         auto scope = FindParen(c);
//         if (!scope) return LLPrsResT::Failure(scope.Error());
//         LRPrsResT rhs_result = ParseImpl(scope->Contained());
//         if (!rhs_result) return rhs_result;
//         out_node.PushBack(lhs_result.Extract());
//         out_node.PushBack(rhs_result.Extract());
//         c.Advance(scope->End());
//       }
//       // 2.2.2. Else it has to be a singular token operand.
//       else {
//         auto rhs_result = ParseOperand(c);
//         if (!rhs_result) return LRPrsResT::Failure(rhs_result.Error());
//         out_node.PushBack(lhs_result.Extract());
//         out_node.PushBack(rhs_result->ast);
//         c.Advance(rhs_result->head);
//       }
//     }
//     // 2.3 End of expr
//     else {
//       if (c.AtEnd())
//         out_node = lhs_result.Extract();
//       else
//         return DEBUG_FAIL("Invalid token type encountered following binary operator.");
//     }
//
//   }
//   // - Operand
//   else if (c.Operation() == eOperation::NONE) {
//     auto lhs_result = ParseOperand(c);
//     if (!lhs_result) return LLPrsResT::Failure(lhs_result.Error());
//     c.Advance(lhs_result->head);
//
//     // 1. Postfix
//     if (c.Operation() == eOperation::Postfix) {
//       // 1.1. Postfix () -> Function Call
//       if (c.TypeIs(kLParen)) {
//         auto scope = FindParen(c);
//         if (!scope) return LRPrsResT::Failure(scope.Error());
//         auto args = ParseArguments(c);
//         if (!args) return LRPrsResT::Failure(args.Error());
//         out_node = Ast(eAst::kFunctionCall, scope->Begin(), scope->End(), lhs_result->ast, args->ast);
//         c.Advance(scope->End());
//       }
//       // 1.2. Postfix [] -> Index Operator
//       else if (c.TypeIs(kLBracket)) {
//         auto scope = FindBracket(c);
//         if (!scope) return LRPrsResT::Failure(scope.Error());
//         auto args = ParseIndexingArguments(c);
//         if (!args) return LRPrsResT::Failure(args.Error());
//         out_node = Ast(eAst::kIndexOperator, scope->Begin(), scope->End(), lhs_result->ast, args->ast);
//         c.Advance(scope->End());
//       }
//       // 1.3. Postfix {} -> Listing Operator
//       else if (c.TypeIs(kLBrace)) {
//         auto scope = FindBrace(c);
//         if (!scope) return LRPrsResT::Failure(scope.Error());
//         auto args = ParseListingArguments(c);
//         if (!args) return LRPrsResT::Failure(args.Error());
//         out_node = Ast(eAst::kListingOperator, scope->Begin(), scope->End(), lhs_result->ast, args->ast);
//         c.Advance(scope->End());
//       }
//       // 1.3. Postfix Single Token Operator.
//       else {
//         out_node = Ast(c);
//         out_node.PushBack(lhs_result->ast);
//         c.Advance();
//       }
//     }
//     // 2. Binary Operator after operand.
//     else if (c.Operation() == eOperation::Binary) {
//       out_node = Ast(c);
//       c.Advance();
//       // Expecting an operand after a binary operator.
//       // 2.1. Open Paren -> Subexpr
//       if (c.TypeIs(kLParen)) {
//         auto scope = FindParen(c);
//         if (!scope) return LLPrsResT::Failure(scope.Error());
//         LRPrsResT rhs_result = ParseImpl(scope->Contained());
//         if (!rhs_result) return rhs_result;
//         out_node.PushBack(lhs_result->ast);
//         out_node.PushBack(rhs_result.Value());
//         c.Advance(scope->End());
//       }
//       // 2.2. Else it has to be a singular token operand.
//       else {
//         auto rhs_result = ParseOperand(c);
//         if (!rhs_result) return LLPrsResT::Failure(rhs_result.Error());
//         out_node.PushBack(lhs_result->ast);
//         out_node.PushBack(rhs_result->ast);
//         c.Advance(rhs_result->head);
//       }
//     }
//     // 3. End of the expression.
//     else {
//       if (c.AtEnd())
//         out_node = lhs_result->ast;
//       else
//         return DEBUG_FAIL("Invalid token type encountered following binary operator.");
//     }
//   }
//   // - Prefix
//   else if (c.Operation() == eOperation::Prefix) {
//     // Prefix Operator, must be followed by an operand.
//     out_node = Ast(c);
//     c.Advance();
//     if (c.AtEnd()) return DEBUG_FAIL("Prefix operator at end of expression.");
//
//     // 1. Open Paren -> Subexpr
//     if (c.TypeIs(kLParen)) {
//       auto scope = FindParen(c);
//       if (!scope) return LRPrsResT::Failure(scope.Error());
//       LRPrsResT lhs_result = ParseImpl(scope->Contained());
//       if (!lhs_result) return lhs_result;
//       out_node.PushBack(lhs_result.Value());
//       c.Advance(scope->End());
//     }
//     // 2. Else it has to be a singular token operand.
//     else if (c.Operation() == eOperation::NONE) {
//       auto lhs_result = ParseOperand(c);
//       if (!lhs_result) return LRPrsResT::Failure(lhs_result.Error());
//       out_node.PushBack(lhs_result->ast);
//       c.Advance(lhs_result->head);
//     }
//     // 3. Error -> Invalid token type encountered following prefix operator.
//     else
//       return DEBUG_FAIL("Invalid token type encountered following prefix operator.");
//
//   }
//   // Error -> Invalid token type at beginning of expression.
//   else
//     return DEBUG_FAIL("Invalid token type at beginning of primary expression.");
//
//   return LRPrsResT::Success(move(out_node));
// }
