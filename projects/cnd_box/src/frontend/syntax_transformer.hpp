#pragma once
#include "../cli/parser.hpp"
#include "../common/diagnostic.hpp"
#include "../common/logger.hpp"
#include "../common/source_manager.hpp"
#include "../common/string_interner.hpp"
#include "ast.hpp"
#include "enum_ast.hpp"
#include "lexer.hpp"
#include "node.hpp"
#include "parser.hpp"

namespace ssgc::frontend {

#define SSGC_PARSER_INVALID_SYNTAX(ast_var, msg)                                                        \
  makeErrorParserInvalidSyntax(std::source_location::current(), ctx.getAstSourceRangeRaw(ast_var).file, \
                               ctx.getAstSourceRangeRaw(ast_var).begin, msg)

template <typename T>
struct LowerResult {
  using NodeType = T;
  T node{};
  Diagnostics diagnostics{};

  bool failed() { return !diagnostics->empty(); }
};

struct LoweringContext {
  StringInterner& id_table;
  StringInterner& string_table;
  const SourceFile& source_file;
  const std::vector<Token>& tokens;

  std::string_view getAstLiteral(const Ast& ast) const {
    if (ast.sourceBegin() == ast.sourceEnd()) {
      return "";
    }

    // Trim insignificant tokens
    const Token* end_token = &tokens.at(ast.sourceEnd() - 1);

    while (isTokenInsignificant(end_token->kind)) {
      end_token--;
    }
    return source_file.slice(tokens.at(ast.sourceBegin()).source_range.begin, end_token->source_range.end);
  }

  SourceRangeRaw getAstSourceRangeRaw(const Ast& ast) const {
    SourceRangeRaw src_range{};
    src_range.file = source_file.id;

    if (ast.sourceBegin() == ast.sourceEnd()) {
      // temp, handle this better later
      src_range.begin = 0;
      src_range.end = 0;
      return src_range;
    }

    // Trim insignificant tokens
    const Token* end_token = &tokens.at(ast.sourceEnd() - 1);

    while (isTokenInsignificant(end_token->kind)) {
      end_token--;
    }

    src_range.begin = tokens.at(ast.sourceBegin()).source_range.begin;
    src_range.end = end_token->source_range.end;
    return src_range;
  }
};

constexpr bool isAstPrimaryExpr(eAst kind) noexcept {
  switch (kind) {
    using enum eAst;
    case kLitCstr:
    case kLitInt:
    case kLitUint:
    case kLitBool:
    case kLitReal:
    case kLitChar:
    case kLitByte:
    case kKwNone:
    case kKwTrue:
    case kKwFalse:
    case kIdent:
      return true;
    case kKwVoid:
    case kKwAny:
      return true;

    default:
      return false;
  }
}

LowerResult<NodeBool> lowerBool(const Ast& ast, LoweringContext& ctx) noexcept;
LowerResult<NodeI8> lowerI8(const Ast& ast, LoweringContext& ctx) noexcept;
LowerResult<NodeI16> lowerI16(const Ast& ast, LoweringContext& ctx) noexcept;
LowerResult<NodeI32> lowerI32(const Ast& ast, LoweringContext& ctx) noexcept;
LowerResult<NodeI64> lowerI64(const Ast& ast, LoweringContext& ctx) noexcept;
LowerResult<NodeU8> lowerU8(const Ast& ast, LoweringContext& ctx) noexcept;
LowerResult<NodeU16> lowerU16(const Ast& ast, LoweringContext& ctx) noexcept;
LowerResult<NodeU32> lowerU32(const Ast& ast, LoweringContext& ctx) noexcept;
LowerResult<NodeU64> lowerU64(const Ast& ast, LoweringContext& ctx) noexcept;
LowerResult<NodeF32> lowerF32(const Ast& ast, LoweringContext& ctx) noexcept;
LowerResult<NodeF64> lowerF64(const Ast& ast, LoweringContext& ctx) noexcept;
LowerResult<NodeNone> lowerNone(const Ast& ast, LoweringContext& ctx) noexcept;
LowerResult<NodeIdentifier> lowerIdentifier(const Ast& ast, LoweringContext& ctx) noexcept;
LowerResult<NodeCStr> lowerStringLiteral(const Ast& ast, LoweringContext& ctx) noexcept;
LowerResult<NodeParenSubexpr> lowerParenSubexpr(const Ast& ast, LoweringContext& ctx) noexcept;
LowerResult<NodeBraceSubexpr> lowerBraceSubexpr(const Ast& ast, LoweringContext& ctx) noexcept;
LowerResult<NodeBracketSubexpr> lowerBracketSubexpr(const Ast& ast, LoweringContext& ctx) noexcept;
LowerResult<NodePrimaryExpr> lowerPrimaryExpr(const Ast& ast, LoweringContext& ctx) noexcept;
LowerResult<NodeModifiers> lowerModifiers(const Ast& ast, LoweringContext& ctx) noexcept;
LowerResult<NodeFunctionalStatement> lowerFunctionalStatement(const Ast& ast, LoweringContext& ctx) noexcept;

LowerResult<NodeMethod> lowerMethod(const Ast& ast, LoweringContext& ctx) noexcept;

LowerResult<NodePrimaryExpr> lowerNamespaceDecl(const Ast& ast, LoweringContext& ctx) noexcept;
LowerResult<NodePrimaryExpr> lowerIncludeStatement(const Ast& ast, LoweringContext& ctx) noexcept;
LowerResult<NodePrimaryExpr> lowerClass(const Ast& ast, LoweringContext& ctx) noexcept;
LowerResult<NodePrimaryExpr> lowerVariable(const Ast& ast, LoweringContext& ctx) noexcept;

template <class LoweringFunctionT,
          class ResultNodeT = std::invoke_result_t<LoweringFunctionT, const Ast&, LoweringContext&>>
void applyLower(LoweringFunctionT&& lowering_func, ResultNodeT& assign_to, Diagnostics& diagnostics, const Ast& ast,
                LoweringContext& ctx) noexcept {
  auto lower_result = lowering_func(ast, ctx);
  assign_to = std::move(lower_result.node);
  diagnostics.append(std::move(lower_result.diagnostics));
}

template <class NodeT, class ResultNodeT = NodeT>
auto xApplyPrimaryLowerGeneric(const Ast& ast, LoweringContext& ctx) {
  LowerResult<ResultNodeT> ret{};
  ret.node = NodeT{};
  *ret.node.sourceRange() = ctx.getAstSourceRangeRaw(ast);
  return ret;
};

template <class BinopNodeT>
LowerResult<NodePrimaryExpr> lowerBinaryOperation(const Ast& ast, LoweringContext& ctx) noexcept {
  assert(ast.size() == 2);

  LowerResult<NodePrimaryExpr> result{};

  BinopNodeT node{};
  const Ast& lhs = ast[0];
  LowerResult<NodePrimaryExpr> lhs_node_result = lowerPrimaryExpr(lhs, ctx);
  node.setLeftOperand(std::move(lhs_node_result.node));
  result.diagnostics.append(lhs_node_result.diagnostics);

  const Ast& rhs = ast[0];
  LowerResult<NodePrimaryExpr> rhs_node_result = lowerPrimaryExpr(rhs, ctx);
  node.setRightOperand(std::move(rhs_node_result.node));
  result.diagnostics.append(rhs_node_result.diagnostics);

  auto& result_node = result.node.emplace<BinopNodeT>();
  result_node = std::move(node);
  return result;
}

template <class UnaryOperandNodeT>
LowerResult<NodePrimaryExpr> lowerUnaryOperation(const Ast& ast, LoweringContext& ctx) noexcept {
  assert(ast.size() == 1);
  LowerResult<NodePrimaryExpr> result{};
  auto& result_node = result.node.emplace<UnaryOperandNodeT>();
  const Ast& operand = ast[0];
  LowerResult<NodePrimaryExpr> lhs_node_result = lowerPrimaryExpr(operand, ctx);
  result_node.operand = std::make_unique<NodePrimaryExpr>(std::move(lhs_node_result.node));
  result.diagnostics.append(lhs_node_result.diagnostics);
  return result;
}

// LowerResult<NodePrimaryExpr> lowerOperand(const Ast& ast,
//                                                       LoweringContext& ctx) noexcept;
//     LowerResult<NodePrimaryExpr> lowerArguments(const Ast& ast,
//                                                         LoweringContext& ctx) noexcept;
//     LowerResult<NodePrimaryExpr> lowerIndexingArguments(
//         const Ast& ast, LoweringContext& ctx) noexcept;
//     LowerResult<NodePrimaryExpr> lowerListingArguments(const Ast& ast,
//                                                                LoweringContext& ctx)
//                                                                noexcept;
//     LowerResult<NodePrimaryExpr> lowerPrimaryStatement(const Ast& ast,
//                                                                LoweringContext& ctx)
//                                                                noexcept;
//     LowerResult<NodePrimaryExpr> lowerConditionalSubExpression(
//         const Ast& ast, LoweringContext& ctx) noexcept;
//     LowerResult<NodePrimaryExpr> lowerPrimaryPreIdentifier(
//         const Ast& ast, LoweringContext& ctx) noexcept;
//     LowerResult<NodePrimaryExpr> lowerPrimaryPostIdentifier(
//         const Ast& ast, LoweringContext& ctx) noexcept;
//
//     LowerResult<NodePrimaryExpr> lowerReturnStmt(const Ast& ast,
//                                                          LoweringContext& ctx) noexcept;
//     LowerResult<NodePrimaryExpr> lowerLibDef(const Ast& ast,
//                                                      LoweringContext& ctx) noexcept;
//     LowerResult<NodePrimaryExpr> lowerPragmaticStmt(const Ast& ast,
//                                                             LoweringContext& ctx) noexcept;
//
//     LowerResult<NodePrimaryExpr> lowerConditionalStmt(const Ast& ast,
//                                                               LoweringContext& ctx)
//                                                               noexcept;
//     LowerResult<NodePrimaryExpr> lowerIfDecl(const Ast& ast,
//                                                      LoweringContext& ctx) noexcept;
//     LowerResult<NodePrimaryExpr> lowerMainDecl(const Ast& ast,
//                                                        LoweringContext& ctx) noexcept;
//     LowerResult<NodePrimaryExpr> lowerLibDecl(const Ast& ast,
//                                                       LoweringContext& ctx) noexcept;
//     LowerResult<NodePrimaryExpr> lowerImportDecl(const Ast& ast,
//                                                          LoweringContext& ctx) noexcept;
//     LowerResult<NodePrimaryExpr> lowerIncludeStmt(const Ast& ast,
//                                                           LoweringContext& ctx) noexcept;
//     LowerResult<NodePrimaryExpr> lowerPragmaStmt(const Ast& ast,
//                                                          LoweringContext& ctx) noexcept;
//
//     LowerResult<NodePrimaryExpr> lowerWhileDecl(const Ast& ast,
//                                                         LoweringContext& ctx) noexcept;
//     LowerResult<NodePrimaryExpr> lowerForDecl(const Ast& ast,
//                                                       LoweringContext& ctx) noexcept;
//     LowerResult<NodePrimaryExpr> lowerUsingDecl(const Ast& ast,
//                                                         LoweringContext& ctx) noexcept;
//     LowerResult<NodePrimaryExpr> lowerVariableDecl(const Ast& ast,
//                                                            LoweringContext& ctx) noexcept;
//     LowerResult<NodePrimaryExpr> lowerEnumDecl(const Ast& ast,
//                                                        LoweringContext& ctx) noexcept;
//     LowerResult<NodePrimaryExpr> lowerEnumDef(const Ast& ast,
//                                                       LoweringContext& ctx) noexcept;
//     LowerResult<NodePrimaryExpr> lowerEnumBlock(const Ast& ast,
//                                                         LoweringContext& ctx) noexcept;
//     LowerResult<NodePrimaryExpr> lowerSyntax(const Ast& ast,
//                                                      LoweringContext& ctx) noexcept;

LowerResult<NodeFile> lowerFile(const Ast& ast, LoweringContext& ctx) noexcept;

LowerResult<NodeFile> lowerFile(const Ast& ast, LoweringContext& ctx) noexcept {
  assert(ast.type == eAst::kSourceFile);

  LowerResult<NodeFile> ret{};
  auto xApplyLowerAndPushStatement = [&ret](auto&& fn, const Ast& ast, LoweringContext& ctx) {
    auto lower_result = fn(ast, ctx);
    ret.node.statements.push_back(std::move(lower_result.node));
    ret.diagnostics.append(std::move(lower_result.diagnostics));
  };

  for (const Ast& directive_statement : ast.branches) {
    if (isAstPrimaryExpr(ast.type)) {
      xApplyLowerAndPushStatement(lowerPrimaryExpr, directive_statement, ctx);
      continue;
    }

    switch (directive_statement.type) {
      case eAst::kMethodDeclaration:
        xApplyLowerAndPushStatement(lowerMethod, directive_statement, ctx);
        break;
      case eAst::kClassDeclaration:
        xApplyLowerAndPushStatement(lowerClass, directive_statement, ctx);
        break;
      case eAst::kIncludeLocalStmt:
        xApplyLowerAndPushStatement(lowerIncludeStatement, directive_statement, ctx);
        break;
      default:
        ret.diagnostics->push_back("Expected directive statement.");
    }
  }

  return ret;
}

LowerResult<NodeBool> lowerBool(const Ast& ast, LoweringContext& ctx) noexcept {
  assert(ast.type == eAst::kLitU1 || ast.type == eAst::kKwTrue ||
         ast.type == eAst::kKwFalse && "Expected kLitU1,kKwTrue or kKwFalse token type.");
  LowerResult<NodeBool> ret{};
  ret.node.source_range = ctx.getAstSourceRangeRaw(ast);
  std::string_view literal = ctx.getAstLiteral(ast);

  if (ast.type == eAst::kKwTrue) {
    ret.node.data = true;
  } else if (ast.type == eAst::kKwFalse) {
    ret.node.data = false;
  } else {
    if (literal == "0b") {
      ret.node.data = false;
    } else if (literal == "1b") {
      ret.node.data = true;
    } else {
      ret.diagnostics->push_back("Invalid boolean literal format.");
      ret.node.data = false;
    }
  }
  return ret;
}

LowerResult<NodeI8> lowerI8(const Ast& ast, LoweringContext& ctx) noexcept {
  assert(ast.type == eAst::kLitI8 && "Expected kLitI8 ast type.");
  LowerResult<NodeI8> ret{};
  ret.node.source_range = ctx.getAstSourceRangeRaw(ast);
  std::string_view literal = ctx.getAstLiteral(ast);

  if (literal.ends_with("i8")) {
    literal.remove_suffix(2);
  } else if (!isCharNumeric(literal.back())) {
    ret.diagnostics->push_back(makeErrorPlaceholder("Invalid i8 literal suffix."));
  }

  auto [error_loc, error_code] = std::from_chars(literal.data(), literal.data() + literal.size(), ret.node.data);

  if (error_code == std::errc::invalid_argument) {
    ret.diagnostics->push_back(makeErrorPlaceholder("Invalid i8 literal format."));
  } else if (error_code == std::errc::result_out_of_range) {
    ret.diagnostics->push_back(makeErrorPlaceholder("i8 literal value out of range."));
  }

  return ret;
}

LowerResult<NodeI16> lowerI16(const Ast& ast, LoweringContext& ctx) noexcept {
  assert(ast.type == eAst::kLitI16 && "Expected kLitI16 token type.");
  LowerResult<NodeI16> ret{};
  ret.node.source_range = ctx.getAstSourceRangeRaw(ast);
  std::string_view literal = ctx.getAstLiteral(ast);

  if (literal.ends_with("i16")) {
    literal.remove_suffix(3);
  } else if (!isCharNumeric(literal.back())) {
    ret.diagnostics->push_back(makeErrorPlaceholder("Invalid i16 literal suffix."));
  }

  auto [error_loc, error_code] = std::from_chars(literal.data(), literal.data() + literal.size(), ret.node.data);

  if (error_code == std::errc::invalid_argument) {
    ret.diagnostics->push_back(makeErrorPlaceholder("Invalid i16 literal format."));
  } else if (error_code == std::errc::result_out_of_range) {
    ret.diagnostics->push_back(makeErrorPlaceholder("i16 literal value out of range."));
  }

  return ret;
}

LowerResult<NodeI32> lowerI32(const Ast& ast, LoweringContext& ctx) noexcept {
  assert(ast.type == eAst::kLitI32 && "Expected kLitI32 ast type.");
  LowerResult<NodeI32> ret{};
  ret.node.source_range = ctx.getAstSourceRangeRaw(ast);
  std::string_view literal = ctx.getAstLiteral(ast);

  if (literal.ends_with("i32")) {
    literal.remove_suffix(3);
  } else if (!isCharNumeric(literal.back())) {
    ret.diagnostics->push_back(makeErrorPlaceholder("Invalid i32 literal suffix."));
  }

  auto [error_loc, error_code] = std::from_chars(literal.data(), literal.data() + literal.size(), ret.node.data);

  if (error_code == std::errc::invalid_argument) {
    ret.diagnostics->push_back(makeErrorPlaceholder("Invalid i32 literal format."));
  } else if (error_code == std::errc::result_out_of_range) {
    ret.diagnostics->push_back(makeErrorPlaceholder("i32 literal value out of range."));
  }

  return ret;
}

LowerResult<NodeI64> lowerI64(const Ast& ast, LoweringContext& ctx) noexcept {
  assert(ast.type == eAst::kLitI64 && "Expected kLitI16 token type.");
  LowerResult<NodeI64> ret{};
  ret.node.source_range = ctx.getAstSourceRangeRaw(ast);
  std::string_view literal = ctx.getAstLiteral(ast);

  if (literal.ends_with("i64")) {
    literal.remove_suffix(3);
  } else if (!isCharNumeric(literal.back())) {
    ret.diagnostics->push_back(makeErrorPlaceholder("Invalid i64 literal suffix."));
  }

  auto [error_loc, error_code] = std::from_chars(literal.data(), literal.data() + literal.size(), ret.node.data);

  if (error_code == std::errc::invalid_argument) {
    ret.diagnostics->push_back(makeErrorPlaceholder("Invalid i64 literal format."));
  } else if (error_code == std::errc::result_out_of_range) {
    ret.diagnostics->push_back(makeErrorPlaceholder("i64 literal value out of range."));
  }

  return ret;
}

LowerResult<NodeU8> lowerU8(const Ast& ast, LoweringContext& ctx) noexcept {
  assert(ast.type == eAst::kLitU8 && "Expected kLitU8 token type.");
  LowerResult<NodeU8> ret{};
  ret.node.source_range = ctx.getAstSourceRangeRaw(ast);
  std::string_view literal = ctx.getAstLiteral(ast);

  if (literal.ends_with("u8")) {
    literal.remove_suffix(2);
  } else if (!isCharNumeric(literal.back())) {
    ret.diagnostics->push_back(makeErrorPlaceholder("Invalid u8 literal suffix."));
  }

  auto [error_loc, error_code] = std::from_chars(literal.data(), literal.data() + literal.size(), ret.node.data);

  if (error_code == std::errc::invalid_argument) {
    ret.diagnostics->push_back(makeErrorPlaceholder("Invalid u8 literal format."));
  } else if (error_code == std::errc::result_out_of_range) {
    ret.diagnostics->push_back(makeErrorPlaceholder("u8 literal value out of range."));
  }

  return ret;
}

LowerResult<NodeU16> lowerU16(const Ast& ast, LoweringContext& ctx) noexcept {
  assert(ast.type == eAst::kLitU16 && "Expected kLitU16 token type.");
  LowerResult<NodeU16> ret{};
  ret.node.source_range = ctx.getAstSourceRangeRaw(ast);
  std::string_view literal = ctx.getAstLiteral(ast);

  if (literal.ends_with("u16")) {
    literal.remove_suffix(3);
  } else if (!isCharNumeric(literal.back())) {
    ret.diagnostics->push_back(makeErrorPlaceholder("Invalid u16 literal suffix."));
  }

  auto [error_loc, error_code] = std::from_chars(literal.data(), literal.data() + literal.size(), ret.node.data);

  if (error_code == std::errc::invalid_argument) {
    ret.diagnostics->push_back(makeErrorPlaceholder("Invalid u16 literal format."));
  } else if (error_code == std::errc::result_out_of_range) {
    ret.diagnostics->push_back(makeErrorPlaceholder("u16 literal value out of range."));
  }

  return ret;
}

LowerResult<NodeU32> lowerU32(const Ast& ast, LoweringContext& ctx) noexcept {
  assert(ast.type == eAst::kLitU32 && "Expected kLitU32 token type.");
  LowerResult<NodeU32> ret{};
  ret.node.source_range = ctx.getAstSourceRangeRaw(ast);
  std::string_view literal = ctx.getAstLiteral(ast);

  if (literal.ends_with("u32")) {
    literal.remove_suffix(3);
  } else if (!isCharNumeric(literal.back())) {
    ret.diagnostics->push_back(makeErrorPlaceholder("Invalid u32 literal suffix."));
  }

  auto [error_loc, error_code] = std::from_chars(literal.data(), literal.data() + literal.size(), ret.node.data);

  if (error_code == std::errc::invalid_argument) {
    ret.diagnostics->push_back(makeErrorPlaceholder("Invalid u32 literal format."));
  } else if (error_code == std::errc::result_out_of_range) {
    ret.diagnostics->push_back(makeErrorPlaceholder("u32 literal value out of range."));
  }

  return ret;
}

LowerResult<NodeU64> lowerU64(const Ast& ast, LoweringContext& ctx) noexcept {
  assert(ast.type == eAst::kLitU64 && "Expected kLitU64 token type.");
  LowerResult<NodeU64> ret{};
  ret.node.source_range = ctx.getAstSourceRangeRaw(ast);
  std::string_view literal = ctx.getAstLiteral(ast);

  if (literal.ends_with("u64")) {
    literal.remove_suffix(3);
  } else if (!isCharNumeric(literal.back())) {
    ret.diagnostics->push_back(makeErrorPlaceholder("Invalid u64 literal suffix."));
  }

  auto [error_loc, error_code] = std::from_chars(literal.data(), literal.data() + literal.size(), ret.node.data);

  if (error_code == std::errc::invalid_argument) {
    ret.diagnostics->push_back(makeErrorPlaceholder("Invalid u64 literal format."));
  } else if (error_code == std::errc::result_out_of_range) {
    ret.diagnostics->push_back(makeErrorPlaceholder("u64 literal value out of range."));
  }

  return ret;
}

LowerResult<NodeF32> lowerF32(const Ast& ast, LoweringContext& ctx) noexcept {
  assert(ast.type == eAst::kLitF32 && "Expected kLitF32 token type.");
  LowerResult<NodeF32> ret{};
  ret.node.source_range = ctx.getAstSourceRangeRaw(ast);
  std::string_view literal = ctx.getAstLiteral(ast);

  if (literal.ends_with("f32")) {
    literal.remove_suffix(3);
  } else if (literal.ends_with("f")) {
    literal.remove_suffix(1);
  } else if (!isCharNumeric(literal.back())) {
    ret.diagnostics->push_back(makeErrorPlaceholder("Invalid f32 literal suffix."));
  }

  auto [error_loc, error_code] = std::from_chars(literal.data(), literal.data() + literal.size(), ret.node.data);

  if (error_code == std::errc::invalid_argument) {
    ret.diagnostics->push_back(makeErrorPlaceholder("Invalid f32 literal format."));
  } else if (error_code == std::errc::result_out_of_range) {
    ret.diagnostics->push_back(makeErrorPlaceholder("f32 literal value out of range."));
  }

  return ret;
}

LowerResult<NodeF64> lowerF64(const Ast& ast, LoweringContext& ctx) noexcept {
  assert(ast.type == eAst::kLitF64 && "Expected kLitF64 token type.");
  LowerResult<NodeF64> ret{};
  ret.node.source_range = ctx.getAstSourceRangeRaw(ast);
  std::string_view literal = ctx.getAstLiteral(ast);

  if (literal.ends_with("f64")) {
    literal.remove_suffix(3);
  } else if (!isCharNumeric(literal.back())) {
    ret.diagnostics->push_back(makeErrorPlaceholder("Invalid f64 literal suffix."));
  }

  auto [error_loc, error_code] = std::from_chars(literal.data(), literal.data() + literal.size(), ret.node.data);

  if (error_code == std::errc::invalid_argument) {
    ret.diagnostics->push_back(makeErrorPlaceholder("Invalid f64 literal format."));
  } else if (error_code == std::errc::result_out_of_range) {
    ret.diagnostics->push_back(makeErrorPlaceholder("f64 literal value out of range."));
  }

  return ret;
}

LowerResult<NodeNone> lowerNone(const Ast& ast, LoweringContext& ctx) noexcept {
  assert(ast.type == eAst::kKwNone && "Expected kKwNone token type.");
  LowerResult<NodeNone> ret{};
  ret.node.source_range = ctx.getAstSourceRangeRaw(ast);

  return ret;
}

LowerResult<NodeIdentifier> lowerIdentifier(const Ast& ast, LoweringContext& ctx) noexcept {
  assert(ast.type == eAst::kIdent && "Expected kIdent token type.");
  LowerResult<NodeIdentifier> ret{};
  ret.node.source_range = ctx.getAstSourceRangeRaw(ast);
  std::string_view literal = ctx.getAstLiteral(ast);
  ret.node.data = ctx.id_table.push(literal);
  return ret;
}

LowerResult<NodeCStr> lowerStringLiteral(const Ast& ast, LoweringContext& ctx) noexcept {
  assert(ast.type == eAst::kLitCstr && "Expected kLitCstr ast type.");
  LowerResult<NodeCStr> ret{};
  ret.node.source_range = ctx.getAstSourceRangeRaw(ast);
  std::string_view literal = ctx.getAstLiteral(ast);

  assert(!literal.empty() && literal.front() == '\"' &&
         "Invalid string literal value. Expected '\"' character at begin.");
  assert(!literal.empty() && literal.back() == '\"' && "Invalid string literal value. Expected '\"' character at end.");
  assert(literal.size() >= 2 && "Invalid string literal value. Must be atleast 2 chars of '\"'.");

  literal = literal.substr(1, literal.size() - 2);

  // https://en.cppreference.com/c/language/escape
  bool is_escape = false;
  std::string escaped_str = "";
  for (auto it = literal.begin(); it < literal.end(); it++) {
    if (is_escape) {
      switch (*it) {
        case '\'':
          escaped_str += *it;
          is_escape = false;
          continue;
        case '"':
          escaped_str += *it;
          is_escape = false;
          continue;
        case '?':
          escaped_str += '\?';
          is_escape = false;
          continue;
        case '\\':
          escaped_str += *it;
          is_escape = false;
          continue;
        case 'a':
          escaped_str += '\a';
          is_escape = false;
          continue;
        case 'b':
          escaped_str += '\b';
          is_escape = false;
          continue;
        case 'f':
          escaped_str += '\f';
          is_escape = false;
          continue;
        case 'n':
          escaped_str += '\n';
          is_escape = false;
          continue;
        case 'r':
          escaped_str += '\r';
          is_escape = false;
          continue;
        case 't':
          escaped_str += '\t';
          is_escape = false;
          continue;
        case 'v':
          escaped_str += '\t';
          is_escape = false;
          continue;
        default:
          ret.diagnostics->push_back("Invalid escape sequence.");
          escaped_str += *it;
          is_escape = false;
          continue;
      }
    } else {
      if (*it == '\\') {
        is_escape = true;
      } else {
        escaped_str += *it;
      }
    }
  }

  ret.node.data = ctx.string_table.push(escaped_str);
  return ret;
}

LowerResult<NodeParenSubexpr> lowerParenSubexpr(const Ast& ast, LoweringContext& ctx) noexcept { return {}; }

LowerResult<NodeBraceSubexpr> lowerBraceSubexpr(const Ast& ast, LoweringContext& ctx) noexcept { return {}; }

LowerResult<NodeBracketSubexpr> lowerBracketSubexpr(const Ast& ast, LoweringContext& ctx) noexcept { return {}; }

LowerResult<NodePrimaryExpr> lowerPrimaryExpr(const Ast& ast, LoweringContext& ctx) noexcept {
  auto xApplyPrimaryLower = [](auto&& fn, const Ast& ast, LoweringContext& ctx) {
    LowerResult<NodePrimaryExpr> ret{};
    auto lower_result = fn(ast, ctx);
    ret.node = std::move(lower_result.node);
    ret.diagnostics.append(std::move(lower_result.diagnostics));
    return ret;
  };

  switch (ast.type) {
    case eAst::kLitU1:
      return xApplyPrimaryLower(lowerBool, ast, ctx);
    case eAst::kLitU8:
      return xApplyPrimaryLower(lowerU8, ast, ctx);
    case eAst::kLitU16:
      return xApplyPrimaryLower(lowerU16, ast, ctx);
    case eAst::kLitU32:
      return xApplyPrimaryLower(lowerU32, ast, ctx);
    case eAst::kLitU64:
      return xApplyPrimaryLower(lowerU64, ast, ctx);
    case eAst::kLitI8:
      return xApplyPrimaryLower(lowerI8, ast, ctx);
    case eAst::kLitI16:
      return xApplyPrimaryLower(lowerI16, ast, ctx);
    case eAst::kLitI32:
      return xApplyPrimaryLower(lowerI32, ast, ctx);
    case eAst::kLitI64:
      return xApplyPrimaryLower(lowerI64, ast, ctx);
    case eAst::kLitF32:
      return xApplyPrimaryLower(lowerF32, ast, ctx);
    case eAst::kLitF64:
      return xApplyPrimaryLower(lowerF64, ast, ctx);
    case eAst::kIdent:
      return xApplyPrimaryLower(lowerIdentifier, ast, ctx);
    case eAst::kLitCstr:
      return xApplyPrimaryLower(lowerStringLiteral, ast, ctx);
    case eAst::kKwNone:
      return xApplyPrimaryLower(lowerNone, ast, ctx);
    case eAst::kKwVoid:
      return xApplyPrimaryLowerGeneric<NodeTypenameVoid, NodePrimaryExpr>(ast, ctx);
    case eAst::kKwAny:
      return xApplyPrimaryLowerGeneric<NodeTypenameAny, NodePrimaryExpr>(ast, ctx);

    // Binary Operators
    case eAst::kListingOperator:
      return lowerBinaryOperation<NodeListFold>(ast, ctx);
    case eAst::kAssign:
      return lowerBinaryOperation<NodeAssignment>(ast, ctx);
    case eAst::kOr:
      return lowerBinaryOperation<NodeLogicalOr>(ast, ctx);
    case eAst::kAnd:
      return lowerBinaryOperation<NodeLogicalAnd>(ast, ctx);
    case eAst::kBor:
      return lowerBinaryOperation<NodeBitwiseOr>(ast, ctx);
    case eAst::kXor:
      return lowerBinaryOperation<NodeBitwiseXor>(ast, ctx);
    case eAst::kBand:
      return lowerBinaryOperation<NodeBitwiseAnd>(ast, ctx);
    case eAst::kEq:
      return lowerBinaryOperation<NodeEquality>(ast, ctx);
    case eAst::kNeq:
      return lowerBinaryOperation<NodeInequality>(ast, ctx);
    case eAst::kGt:
      return lowerBinaryOperation<NodeGreater>(ast, ctx);
    case eAst::kLt:
      return lowerBinaryOperation<NodeLess>(ast, ctx);
    case eAst::kGte:
      return lowerBinaryOperation<NodeGreaterEqual>(ast, ctx);
    case eAst::kLte:
      return lowerBinaryOperation<NodeLessEqual>(ast, ctx);
    case eAst::kRsh:
      return lowerBinaryOperation<NodeBitwiseShiftRight>(ast, ctx);
    case eAst::kLsh:
      return lowerBinaryOperation<NodeBitwiseShiftLeft>(ast, ctx);
    case eAst::kAdd:
      return lowerBinaryOperation<NodeAdd>(ast, ctx);
    case eAst::kSub:
      return lowerBinaryOperation<NodeSubtract>(ast, ctx);
    case eAst::kMul:
      return lowerBinaryOperation<NodeMultiply>(ast, ctx);
    case eAst::kDiv:
      return lowerBinaryOperation<NodeDivide>(ast, ctx);
    case eAst::kMod:
      return lowerBinaryOperation<NodeModulus>(ast, ctx);
    case eAst::kMemberAccess:
      return lowerBinaryOperation<NodeAccess>(ast, ctx);
    case eAst::kResolutionOp:
      return lowerBinaryOperation<NodeResolution>(ast, ctx);

    // Unary Operations
    case eAst::kInc:
      return lowerUnaryOperation<NodeIncrement>(ast, ctx);
    case eAst::kDec:
      return lowerUnaryOperation<NodeDecrement>(ast, ctx);
    case eAst::kNot:
      return lowerUnaryOperation<NodeNot>(ast, ctx);
      // return lowerBinaryOperation<NodeNegative>(ast, ctx);
      // return lowerBinaryOperation<NodePositive>(ast, ctx);

    default:
      Diagnostic error = SSGC_PARSER_INVALID_SYNTAX(
          ast, std::format("Invalid primary expression. Node: \n{}", ast.Format(&ctx.source_file, &ctx.tokens))

      );
      return LowerResult<NodePrimaryExpr>{NodePoison{}, Diagnostics{error}};
  }
}

LowerResult<NodeModifiers> lowerModifiers(const Ast& ast, LoweringContext& ctx) noexcept { return {}; }

LowerResult<NodeFunctionalStatement> lowerFunctionalStatement(const Ast& ast, LoweringContext& ctx) noexcept {
  return {};
}

LowerResult<NodeMethod> lowerMethod(const Ast& ast, LoweringContext& ctx) noexcept {
  assert(ast.type == eAst::kMethodDeclaration);
  assert(ast.branches.size() > 2);
  assert(ast[0].type == eAst::kModifiers);
  assert(ast[1].type == eAst::kIdent);
  assert(ast[2].type == eAst::kMethodSignature);
  assert(ast[2].branches.size() > 1);
  assert(ast[2][0].type == eAst::kMethodParameterList);

  LowerResult<NodeMethod> ret{};
  ret.node.source_range = ctx.getAstSourceRangeRaw(ast);
  applyLower(lowerModifiers, ret.node.mods, ret.diagnostics, ast[0], ctx);
  applyLower(lowerIdentifier, ret.node.name, ret.diagnostics, ast[1], ctx);

  const Ast& signature = ast[2];

  assert(signature.branches.size() > 1);
  assert(signature[0].type == eAst::kMethodParameterList);
  const Ast& parameter_list = signature[0];

  for (const Ast& param : parameter_list.branches) {
    assert(param.type == eAst::kMethodParameter);
    assert(param.branches.size() > 2);
    assert(param[0].type == eAst::kModifiers);
    assert(isAstPrimaryExpr(param[1].type));
    assert(param[2].type == eAst::kIdent);

    NodeMethodParameter param_node{};
    param_node.source_range = ctx.getAstSourceRangeRaw(param);
    applyLower(lowerModifiers, param_node.mods, ret.diagnostics, param[0], ctx);
    applyLower(lowerPrimaryExpr, param_node.type, ret.diagnostics, param[1], ctx);
    applyLower(lowerIdentifier, param_node.name, ret.diagnostics, param[2], ctx);
    ret.node.params.push_back(std::move(param_node));
  }

  assert(signature[1].type == eAst::kMethodReturnType);
  const Ast& return_type = signature[1];

  assert(return_type.branches.size() > 0);
  assert(return_type[0].type == eAst::kModifiers);
  const Ast& return_type_mods = return_type[0];
  auto return_type_mods_res = lowerModifiers(return_type_mods, ctx);
  ret.node.return_mods = return_type_mods_res.node;
  ret.diagnostics.append(return_type_mods_res.diagnostics);

  assert(return_type.branches.size() > 1);
  const Ast& return_type_expr = return_type[1];
  assert(isAstPrimaryExpr(return_type_expr.type));
  auto return_type_expr_res = lowerPrimaryExpr(return_type_expr, ctx);
  ret.node.return_mods = std::move(return_type_mods_res.node);
  ret.diagnostics.append(return_type_mods_res.diagnostics);

  if (ast.branches.size() > 3) {
    const Ast& definition = ast[3];
    for (const Ast& statement : definition.branches) {
      auto stmt_res = lowerFunctionalStatement(statement, ctx);
      ret.node.definition.push_back(stmt_res.node);
    }
  }

  return ret;
}

LowerResult<NodePrimaryExpr> lowerIncludeStatement(const Ast& ast, LoweringContext& ctx) noexcept {
  assert(ast.type == eAst::kIncludeLocalStmt || ast.type == eAst::kIncludeSystemStmt);

  return {};
}

LowerResult<NodePrimaryExpr> lowerClass(const Ast& ast, LoweringContext& ctx) noexcept { return {}; };

LowerResult<NodePrimaryExpr> lowerVariable(const Ast& ast, LoweringContext& ctx) noexcept { return {}; };

}  // namespace ssgc::frontend