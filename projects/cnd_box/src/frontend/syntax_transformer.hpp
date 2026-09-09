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

  #define SSGC_PARSER_INVALID_SYNTAX(ast_var,msg)\
  makeErrorParserInvalidSyntax(std::source_location::current(),\
                                   ctx.getAstSourceRangeRaw(ast_var).file,\
                                   ctx.getAstSourceRangeRaw(ast_var).begin, msg)

template <typename T>
struct TransformResult {
  T node{};
  Diagnostics diagnostics{};

  bool failed() { return !diagnostics->empty();
  }
};

struct TransformationContext {
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
    return source_file.slice(tokens.at(ast.sourceBegin()).source_range.begin,
                             end_token->source_range.end);
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


TransformResult<NodeBool> transformBool(const Ast& ast, TransformationContext& ctx) noexcept;
TransformResult<NodeI8> transformI8(const Ast& ast, TransformationContext& ctx) noexcept;
TransformResult<NodeI16> transformI16(const Ast& ast, TransformationContext& ctx) noexcept;
TransformResult<NodeI32> transformI32(const Ast& ast, TransformationContext& ctx) noexcept;
TransformResult<NodeI64> transformI64(const Ast& ast, TransformationContext& ctx) noexcept;
TransformResult<NodeU8> transformU8(const Ast& ast, TransformationContext& ctx) noexcept;
TransformResult<NodeU16> transformU16(const Ast& ast, TransformationContext& ctx) noexcept;
TransformResult<NodeU32> transformU32(const Ast& ast, TransformationContext& ctx) noexcept;
TransformResult<NodeU64> transformU64(const Ast& ast, TransformationContext& ctx) noexcept;
TransformResult<NodeF32> transformF32(const Ast& ast, TransformationContext& ctx) noexcept;
TransformResult<NodeF64> transformF64(const Ast& ast, TransformationContext& ctx) noexcept;
TransformResult<NodeNone> transformNone(const Ast& ast, TransformationContext& ctx) noexcept;
TransformResult<NodeIdentifier> transformIdentifier(const Ast& ast, TransformationContext& ctx) noexcept;
TransformResult<NodeCStr> transformStringLiteral(const Ast& ast, TransformationContext& ctx) noexcept;

TransformResult<NodeParenSubexpr> transformParenSubexpr(const Ast& ast, TransformationContext& ctx) noexcept;
TransformResult<NodeBraceSubexpr> transformBraceSubexpr(const Ast& ast, TransformationContext& ctx) noexcept;
TransformResult<NodeBracketSubexpr> transformBracketSubexpr(const Ast& ast, TransformationContext& ctx) noexcept;

TransformResult<NodePrimaryExpr> transformListFold(const Ast& ast, TransformationContext& ctx) noexcept;
TransformResult<NodePrimaryExpr> transformAssignment(const Ast& ast, TransformationContext& ctx) noexcept;
TransformResult<NodePrimaryExpr> transformLogicalOr(const Ast& ast, TransformationContext& ctx) noexcept;
TransformResult<NodePrimaryExpr> transformLogicalAnd(const Ast& ast, TransformationContext& ctx) noexcept;
TransformResult<NodePrimaryExpr> transformBitwiseOr(const Ast& ast, TransformationContext& ctx) noexcept;
TransformResult<NodePrimaryExpr> transformBitwiseXor(const Ast& ast, TransformationContext& ctx) noexcept;
TransformResult<NodePrimaryExpr> transformBitwiseAnd(const Ast& ast, TransformationContext& ctx) noexcept;
TransformResult<NodePrimaryExpr> transformEquality(const Ast& ast, TransformationContext& ctx) noexcept;
TransformResult<NodePrimaryExpr> transformComparison(const Ast& ast, TransformationContext& ctx) noexcept;
TransformResult<NodePrimaryExpr> transformThreeWayEquality(const Ast& ast, TransformationContext& ctx) noexcept;
TransformResult<NodePrimaryExpr> transformBitwiseShift(const Ast& ast, TransformationContext& ctx) noexcept;
TransformResult<NodePrimaryExpr> transformSummation(const Ast& ast, TransformationContext& ctx) noexcept;
TransformResult<NodePrimaryExpr> transformProduction(const Ast& ast, TransformationContext& ctx) noexcept;
TransformResult<NodePrimaryExpr> transformPrefix(const Ast& ast, TransformationContext& ctx) noexcept;
TransformResult<NodePrimaryExpr> transformPostfix(const Ast& ast, TransformationContext& ctx) noexcept;
TransformResult<NodePrimaryExpr> transformResolution(const Ast& ast, TransformationContext& ctx) noexcept;

TransformResult<NodePrimaryExpr> transformPrimaryExpr(const Ast& ast,
                                                      TransformationContext& ctx) noexcept;

TransformResult<NodeModifiers> transformModifiers(const Ast& ast,
                                                  TransformationContext& ctx) noexcept;

TransformResult<NodeFunctionalStatement> transformFunctionalStatement(
    const Ast& ast, TransformationContext& ctx) noexcept;

TransformResult<NodeMethod> transformMethod(const Ast& ast, TransformationContext& ctx) noexcept;
TransformResult<NodePrimaryExpr> transformOperand(const Ast& ast, TransformationContext& ctx) noexcept;
TransformResult<NodePrimaryExpr> transformArguments(const Ast& ast, TransformationContext& ctx) noexcept;
TransformResult<NodePrimaryExpr> transformIndexingArguments(const Ast& ast, TransformationContext& ctx) noexcept;
TransformResult<NodePrimaryExpr> transformListingArguments(const Ast& ast, TransformationContext& ctx) noexcept;
TransformResult<NodePrimaryExpr> transformPrimaryStatement(const Ast& ast, TransformationContext& ctx) noexcept;
TransformResult<NodePrimaryExpr> transformConditionalSubExpression(const Ast& ast, TransformationContext& ctx) noexcept;
TransformResult<NodePrimaryExpr> transformPrimaryPreIdentifier(const Ast& ast, TransformationContext& ctx) noexcept;
TransformResult<NodePrimaryExpr> transformPrimaryPostIdentifier(const Ast& ast, TransformationContext& ctx) noexcept;

TransformResult<NodePrimaryExpr> transformReturnStmt(const Ast& ast, TransformationContext& ctx) noexcept;
TransformResult<NodePrimaryExpr> transformMethodParameters(const Ast& ast, TransformationContext& ctx) noexcept;
TransformResult<NodePrimaryExpr> transformMethodReturnParameters(const Ast& ast, TransformationContext& ctx) noexcept;
TransformResult<NodePrimaryExpr> transformMethodSignature(const Ast& ast, TransformationContext& ctx) noexcept;
TransformResult<NodePrimaryExpr> transformMethodDef(const Ast& ast, TransformationContext& ctx) noexcept;
TransformResult<NodePrimaryExpr> transformMainDef(const Ast& ast, TransformationContext& ctx) noexcept;
TransformResult<NodePrimaryExpr> transformClassDef(const Ast& ast, TransformationContext& ctx) noexcept;
TransformResult<NodePrimaryExpr> transformLibDef(const Ast& ast, TransformationContext& ctx) noexcept;
TransformResult<NodePrimaryExpr> transformPragmaticStmt(const Ast& ast, TransformationContext& ctx) noexcept;

TransformResult<NodePrimaryExpr> transformConditionalStmt(const Ast& ast, TransformationContext& ctx) noexcept;
TransformResult<NodePrimaryExpr> transformIfDecl(const Ast& ast, TransformationContext& ctx) noexcept;
TransformResult<NodePrimaryExpr> transformMainDecl(const Ast& ast, TransformationContext& ctx) noexcept;
TransformResult<NodePrimaryExpr> transformLibDecl(const Ast& ast, TransformationContext& ctx) noexcept;
TransformResult<NodePrimaryExpr> transformImportDecl(const Ast& ast, TransformationContext& ctx) noexcept;
TransformResult<NodePrimaryExpr> transformIncludeStmt(const Ast& ast, TransformationContext& ctx) noexcept;
TransformResult<NodePrimaryExpr> transformPragmaStmt(const Ast& ast, TransformationContext& ctx) noexcept;

TransformResult<NodePrimaryExpr> transformWhileDecl(const Ast& ast, TransformationContext& ctx) noexcept;
TransformResult<NodePrimaryExpr> transformForDecl(const Ast& ast, TransformationContext& ctx) noexcept;
TransformResult<NodePrimaryExpr> transformUsingDecl(const Ast& ast, TransformationContext& ctx) noexcept;
TransformResult<NodePrimaryExpr> transformVariableDecl(const Ast& ast, TransformationContext& ctx) noexcept;
TransformResult<NodePrimaryExpr> transformMethodDecl(const Ast& ast, TransformationContext& ctx) noexcept;
TransformResult<NodePrimaryExpr> transformClassDecl(const Ast& ast, TransformationContext& ctx) noexcept;
TransformResult<NodePrimaryExpr> transformEnumDecl(const Ast& ast, TransformationContext& ctx) noexcept;
TransformResult<NodePrimaryExpr> transformEnumDef(const Ast& ast, TransformationContext& ctx) noexcept;
TransformResult<NodePrimaryExpr> transformEnumBlock(const Ast& ast, TransformationContext& ctx) noexcept;
TransformResult<NodePrimaryExpr> transformSyntax(const Ast& ast, TransformationContext& ctx) noexcept;
TransformResult<NodePrimaryExpr> transformNamespaceDecl(const Ast& ast, TransformationContext& ctx) noexcept;

TransformResult<NodeFile> transformFile(const Ast& ast, TransformationContext& ctx) noexcept;

TransformResult<NodeFile> transformFile(const Ast& ast, TransformationContext& ctx) noexcept {
  assert(ast.type == eAst::kSourceFile);

  return {NodeFile{}, Diagnostics{}};
}

TransformResult<NodeBool> transformBool(const Ast& ast, TransformationContext& ctx) noexcept {
  assert(ast.type == eAst::kLitU1 || ast.type == eAst::kKwTrue ||
         ast.type == eAst::kKwFalse && "Expected kLitU1,kKwTrue or kKwFalse token type.");
  TransformResult<NodeBool> ret{};
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

TransformResult<NodeI8> transformI8(const Ast& ast, TransformationContext& ctx) noexcept {
  assert(ast.type == eAst::kLitI8 && "Expected kLitI8 ast type.");
  TransformResult<NodeI8> ret{};
  ret.node.source_range = ctx.getAstSourceRangeRaw(ast);
  std::string_view literal = ctx.getAstLiteral(ast);

  if (literal.ends_with("i8")) {
    literal.remove_suffix(2);
  } else if (!isCharNumeric(literal.back())) {
    ret.diagnostics->push_back(makeErrorPlaceholder("Invalid i8 literal suffix."));
  }

  auto [error_loc, error_code] =
      std::from_chars(literal.data(), literal.data() + literal.size(), ret.node.data);

  if (error_code == std::errc::invalid_argument) {
    ret.diagnostics->push_back(makeErrorPlaceholder("Invalid i8 literal format."));
  } else if (error_code == std::errc::result_out_of_range) {
    ret.diagnostics->push_back(makeErrorPlaceholder("i8 literal value out of range."));
  }

  return ret;
}

TransformResult<NodeI16> transformI16(const Ast& ast, TransformationContext& ctx) noexcept {
  assert(ast.type == eAst::kLitI16 && "Expected kLitI16 token type.");
  TransformResult<NodeI16> ret{};
  ret.node.source_range = ctx.getAstSourceRangeRaw(ast);
  std::string_view literal = ctx.getAstLiteral(ast);

  if (literal.ends_with("i16")) {
    literal.remove_suffix(3);
  } else if (!isCharNumeric(literal.back())) {
    ret.diagnostics->push_back(makeErrorPlaceholder("Invalid i16 literal suffix."));
  }

  auto [error_loc, error_code] =
      std::from_chars(literal.data(), literal.data() + literal.size(), ret.node.data);

  if (error_code == std::errc::invalid_argument) {
    ret.diagnostics->push_back(makeErrorPlaceholder("Invalid i16 literal format."));
  } else if (error_code == std::errc::result_out_of_range) {
    ret.diagnostics->push_back(makeErrorPlaceholder("i16 literal value out of range."));
  }

  return ret;
}

TransformResult<NodeI32> transformI32(const Ast& ast, TransformationContext& ctx) noexcept {
  assert(ast.type == eAst::kLitI32 && "Expected kLitI32 ast type.");
  TransformResult<NodeI32> ret{};
  ret.node.source_range = ctx.getAstSourceRangeRaw(ast);
  std::string_view literal = ctx.getAstLiteral(ast);

  if (literal.ends_with("i32")) {
    literal.remove_suffix(3);
  } else if (!isCharNumeric(literal.back())) {
    ret.diagnostics->push_back(makeErrorPlaceholder("Invalid i32 literal suffix."));
  }

  auto [error_loc, error_code] =
      std::from_chars(literal.data(), literal.data() + literal.size(), ret.node.data);

  if (error_code == std::errc::invalid_argument) {
    ret.diagnostics->push_back(makeErrorPlaceholder("Invalid i32 literal format."));
  } else if (error_code == std::errc::result_out_of_range) {
    ret.diagnostics->push_back(makeErrorPlaceholder("i32 literal value out of range."));
  }

  return ret;
}

TransformResult<NodeI64> transformI64(const Ast& ast, TransformationContext& ctx) noexcept {
  assert(ast.type == eAst::kLitI64 && "Expected kLitI16 token type.");
  TransformResult<NodeI64> ret{};
  ret.node.source_range = ctx.getAstSourceRangeRaw(ast);
  std::string_view literal = ctx.getAstLiteral(ast);

  if (literal.ends_with("i64")) {
    literal.remove_suffix(3);
  } else if (!isCharNumeric(literal.back())) {
    ret.diagnostics->push_back(makeErrorPlaceholder("Invalid i64 literal suffix."));
  }

  auto [error_loc, error_code] =
      std::from_chars(literal.data(), literal.data() + literal.size(), ret.node.data);

  if (error_code == std::errc::invalid_argument) {
    ret.diagnostics->push_back(makeErrorPlaceholder("Invalid i64 literal format."));
  } else if (error_code == std::errc::result_out_of_range) {
    ret.diagnostics->push_back(makeErrorPlaceholder("i64 literal value out of range."));
  }

  return ret;
}

TransformResult<NodeU8> transformU8(const Ast& ast, TransformationContext& ctx) noexcept {
  assert(ast.type == eAst::kLitU8 && "Expected kLitU8 token type.");
  TransformResult<NodeU8> ret{};
  ret.node.source_range = ctx.getAstSourceRangeRaw(ast);
  std::string_view literal = ctx.getAstLiteral(ast);

  if (literal.ends_with("u8")) {
    literal.remove_suffix(2);
  } else if (!isCharNumeric(literal.back())) {
    ret.diagnostics->push_back(makeErrorPlaceholder("Invalid u8 literal suffix."));
  }

  auto [error_loc, error_code] =
      std::from_chars(literal.data(), literal.data() + literal.size(), ret.node.data);

  if (error_code == std::errc::invalid_argument) {
    ret.diagnostics->push_back(makeErrorPlaceholder("Invalid u8 literal format."));
  } else if (error_code == std::errc::result_out_of_range) {
    ret.diagnostics->push_back(makeErrorPlaceholder("u8 literal value out of range."));
  }

  return ret;
}

TransformResult<NodeU16> transformU16(const Ast& ast, TransformationContext& ctx) noexcept {
  assert(ast.type == eAst::kLitU16 && "Expected kLitU16 token type.");
  TransformResult<NodeU16> ret{};
  ret.node.source_range = ctx.getAstSourceRangeRaw(ast);
  std::string_view literal = ctx.getAstLiteral(ast);


  if (literal.ends_with("u16")) {
    literal.remove_suffix(3);
  } else if (!isCharNumeric(literal.back())) {
    ret.diagnostics->push_back(makeErrorPlaceholder("Invalid u16 literal suffix."));
  }

  auto [error_loc, error_code] =
      std::from_chars(literal.data(), literal.data() + literal.size(), ret.node.data);

  if (error_code == std::errc::invalid_argument) {
    ret.diagnostics->push_back(makeErrorPlaceholder("Invalid u16 literal format."));
  } else if (error_code == std::errc::result_out_of_range) {
    ret.diagnostics->push_back(makeErrorPlaceholder("u16 literal value out of range."));
  }

  return ret;
}

TransformResult<NodeU32> transformU32(const Ast& ast, TransformationContext& ctx) noexcept {
  assert(ast.type == eAst::kLitU32 && "Expected kLitU32 token type.");
  TransformResult<NodeU32> ret{};
  ret.node.source_range = ctx.getAstSourceRangeRaw(ast);
  std::string_view literal = ctx.getAstLiteral(ast);


  if (literal.ends_with("u32")) {
    literal.remove_suffix(3);
  } else if (!isCharNumeric(literal.back())) {
    ret.diagnostics->push_back(makeErrorPlaceholder("Invalid u32 literal suffix."));
  }

  auto [error_loc, error_code] =
      std::from_chars(literal.data(), literal.data() + literal.size(), ret.node.data);

  if (error_code == std::errc::invalid_argument) {
    ret.diagnostics->push_back(makeErrorPlaceholder("Invalid u32 literal format."));
  } else if (error_code == std::errc::result_out_of_range) {
    ret.diagnostics->push_back(makeErrorPlaceholder("u32 literal value out of range."));
  }

  return ret;
}

TransformResult<NodeU64> transformU64(const Ast& ast, TransformationContext& ctx) noexcept {
  assert(ast.type == eAst::kLitU64 && "Expected kLitU64 token type.");
  TransformResult<NodeU64> ret{};
  ret.node.source_range = ctx.getAstSourceRangeRaw(ast);
  std::string_view literal = ctx.getAstLiteral(ast);


  if (literal.ends_with("u64")) {
    literal.remove_suffix(3);
  } else if (!isCharNumeric(literal.back())) {
    ret.diagnostics->push_back(makeErrorPlaceholder("Invalid u64 literal suffix."));
  }

  auto [error_loc, error_code] =
      std::from_chars(literal.data(), literal.data() + literal.size(), ret.node.data);

  if (error_code == std::errc::invalid_argument) {
    ret.diagnostics->push_back(makeErrorPlaceholder("Invalid u64 literal format."));
  } else if (error_code == std::errc::result_out_of_range) {
    ret.diagnostics->push_back(makeErrorPlaceholder("u64 literal value out of range."));
  }

  return ret;
}

TransformResult<NodeF32> transformF32(const Ast& ast, TransformationContext& ctx) noexcept {
  assert(ast.type == eAst::kLitF32 && "Expected kLitF32 token type.");
  TransformResult<NodeF32> ret{};
  ret.node.source_range = ctx.getAstSourceRangeRaw(ast);
  std::string_view literal = ctx.getAstLiteral(ast);

  if (literal.ends_with("f32")) {
    literal.remove_suffix(3);
  } else if (literal.ends_with("f")) {
    literal.remove_suffix(1);
  } else if (!isCharNumeric(literal.back())) {
    ret.diagnostics->push_back(makeErrorPlaceholder("Invalid f32 literal suffix."));
  }

  auto [error_loc, error_code] =
      std::from_chars(literal.data(), literal.data() + literal.size(), ret.node.data);

  if (error_code == std::errc::invalid_argument) {
    ret.diagnostics->push_back(makeErrorPlaceholder("Invalid f32 literal format."));
  } else if (error_code == std::errc::result_out_of_range) {
    ret.diagnostics->push_back(makeErrorPlaceholder("f32 literal value out of range."));
  }

  return ret;
}

TransformResult<NodeF64> transformF64(const Ast& ast, TransformationContext& ctx) noexcept {
  assert(ast.type == eAst::kLitF64 && "Expected kLitF64 token type.");
  TransformResult<NodeF64> ret{};
  ret.node.source_range = ctx.getAstSourceRangeRaw(ast);
  std::string_view literal = ctx.getAstLiteral(ast);

  if (literal.ends_with("f64")) {
    literal.remove_suffix(3);
  } else if (!isCharNumeric(literal.back())) {
    ret.diagnostics->push_back(makeErrorPlaceholder("Invalid f64 literal suffix."));
  }

  auto [error_loc, error_code] =
      std::from_chars(literal.data(), literal.data() + literal.size(), ret.node.data);

  if (error_code == std::errc::invalid_argument) {
    ret.diagnostics->push_back(makeErrorPlaceholder("Invalid f64 literal format."));
  } else if (error_code == std::errc::result_out_of_range) {
    ret.diagnostics->push_back(makeErrorPlaceholder("f64 literal value out of range."));
  }

  return ret;
}

TransformResult<NodeNone> transformNone(const Ast& ast, TransformationContext& ctx) noexcept {
  assert(ast.type == eAst::kKwNone && "Expected kKwNone token type.");
  TransformResult<NodeNone> ret{};
  ret.node.source_range = ctx.getAstSourceRangeRaw(ast);

  return ret;
}

TransformResult<NodeIdentifier> transformIdentifier(const Ast& ast,
                                                    TransformationContext& ctx) noexcept {
  assert(ast.type == eAst::kIdent && "Expected kIdent token type.");
  TransformResult<NodeIdentifier> ret{};
  ret.node.source_range = ctx.getAstSourceRangeRaw(ast);
  std::string_view literal = ctx.getAstLiteral(ast);
  ret.node.data = ctx.id_table.push(literal);
  return ret;
}

TransformResult<NodeCStr> transformStringLiteral(const Ast& ast,
                                                 TransformationContext& ctx) noexcept {
  assert(ast.type == eAst::kLitCstr && "Expected kLitCstr ast type.");
  TransformResult<NodeCStr> ret{};
  ret.node.source_range = ctx.getAstSourceRangeRaw(ast);
  std::string_view literal = ctx.getAstLiteral(ast);

  assert(!literal.empty() && literal.front() == '\"' &&
         "Invalid string literal value. Expected '\"' character at begin.");
  assert(!literal.empty() && literal.back() == '\"' &&
         "Invalid string literal value. Expected '\"' character at end.");
  assert(literal.size() >= 2 &&
         "Invalid string literal value. Must be atleast 2 chars of '\"'.");

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


TransformResult<NodeParenSubexpr> transformParenSubexpr(const Ast& ast,
                                                        TransformationContext& ctx) noexcept {
  return {};
}

TransformResult<NodeBraceSubexpr> transformBraceSubexpr(const Ast& ast,
                                                        TransformationContext& ctx) noexcept {
  return {};
}

TransformResult<NodeBracketSubexpr> transformBracketSubexpr(const Ast& ast,
                                                            TransformationContext& ctx) noexcept {
  return {};
}

template<class NodeT,class ResultNodeT = NodeT>
  auto xApplyPrimaryTransformGeneric(const Ast& ast, TransformationContext& ctx) {
  TransformResult<ResultNodeT> ret{};
  ret.node = NodeT{};
  *ret.node.sourceRange() = ctx.getAstSourceRangeRaw(ast);
  return ret;
};

TransformResult<NodePrimaryExpr> transformPrimaryExpr(const Ast& ast,
                                                      TransformationContext& ctx) noexcept {
  auto xApplyPrimaryTransform = [](auto&& fn, const Ast& ast, TransformationContext& ctx) {
    TransformResult<NodePrimaryExpr> ret{};
    auto transform_result = fn(ast, ctx);
    ret.node = std::move(transform_result.node);
    ret.diagnostics.append(std::move(transform_result.diagnostics));
    return ret;
  };



  switch (ast.type) {
    case eAst::kLitU1:
      return xApplyPrimaryTransform(transformBool, ast, ctx);
    case eAst::kLitU8:
      return xApplyPrimaryTransform(transformU8, ast, ctx);
    case eAst::kLitU16:
      return xApplyPrimaryTransform(transformU16, ast, ctx);
    case eAst::kLitU32:
      return xApplyPrimaryTransform(transformU32, ast, ctx);
    case eAst::kLitU64:
      return xApplyPrimaryTransform(transformU64, ast, ctx);
    case eAst::kLitI8:
      return xApplyPrimaryTransform(transformI8, ast, ctx);
    case eAst::kLitI16:
      return xApplyPrimaryTransform(transformI16, ast, ctx);
    case eAst::kLitI32:
      return xApplyPrimaryTransform(transformI32, ast, ctx);
    case eAst::kLitI64:
      return xApplyPrimaryTransform(transformI64, ast, ctx);
    case eAst::kLitF32:
      return xApplyPrimaryTransform(transformF32, ast, ctx);
    case eAst::kLitF64:
      return xApplyPrimaryTransform(transformF64, ast, ctx);
    case eAst::kIdent:
      return xApplyPrimaryTransform(transformIdentifier, ast, ctx);
    case eAst::kLitCstr:
      return xApplyPrimaryTransform(transformStringLiteral, ast, ctx);
    case eAst::kKwNone:
      return xApplyPrimaryTransform(transformNone, ast, ctx);
    case eAst::kKwVoid:
      return xApplyPrimaryTransformGeneric<NodeTypenameVoid, NodePrimaryExpr>(ast, ctx);
    case eAst::kKwAny:
      return xApplyPrimaryTransformGeneric<NodeTypenameAny, NodePrimaryExpr>(ast, ctx);
    default:
      Diagnostic error = SSGC_PARSER_INVALID_SYNTAX(ast,
        std::format("Invalid primary expression. Node: \n{}",ast.Format(&ctx.source_file,&ctx.tokens))
        
      );
      return TransformResult<NodePrimaryExpr>{NodePoison{}, Diagnostics{error}};
  }
}

TransformResult<NodeModifiers> transformModifiers(const Ast& ast,
                                                  TransformationContext& ctx) noexcept {
  return {};
}

TransformResult<NodeFunctionalStatement> transformFunctionalStatement(
    const Ast& ast, TransformationContext& ctx) noexcept {
  return {};
}

TransformResult<NodeMethod> transformMethod(const Ast& ast, TransformationContext& ctx) noexcept {
  assert(ast.type == eAst::kMethodDeclaration);
  TransformResult<NodeMethod> ret{};
  ret.node.source_range = ctx.getAstSourceRangeRaw(ast);

  assert(ast.branches.size() > 0);
  const Ast& modifiers = ast[0];
  TransformResult<NodeModifiers> modifiers_node_res = transformModifiers(ast[0], ctx);
  ret.node.mods = modifiers_node_res.node;
  ret.diagnostics.append(modifiers_node_res.diagnostics);

  assert(ast.branches.size() > 1);
  const Ast& name = ast[1];
  TransformResult<NodeIdentifier> name_node_res = transformIdentifier(name, ctx);
  ret.node.name = name_node_res.node;
  ret.diagnostics.append(name_node_res.diagnostics);

  assert(ast.branches.size() > 2);
  assert(ast[2].type == eAst::kMethodSignature);
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
    const Ast& param_mods = param[0];
    const Ast& param_type = param[1];
    const Ast& param_name = param[2];

    TransformResult<NodeModifiers> param_mods_res = transformModifiers(param_mods, ctx);
    TransformResult<NodePrimaryExpr> param_type_res = transformPrimaryExpr(param_type, ctx);
    TransformResult<NodeIdentifier> param_name_res = transformIdentifier(param_name, ctx);

    ret.diagnostics.append(std::move(param_mods_res.diagnostics));
    ret.diagnostics.append(std::move(param_type_res.diagnostics));
    ret.diagnostics.append(std::move(param_name_res.diagnostics));

    NodeMethodParameter param_node{};
    param_node.source_range = ctx.getAstSourceRangeRaw(param);
    param_node.mods = std::move(param_mods_res.node);
    param_node.type = std::move(param_type_res.node);
    param_node.name = std::move(param_name_res.node);

    ret.node.params.push_back(std::move(param_node));
  }

  assert(signature[1].type == eAst::kMethodReturnType);
  const Ast& return_type = signature[1];

  assert(return_type.branches.size() > 0);
  assert(return_type[0].type == eAst::kModifiers);
  const Ast& return_type_mods = return_type[0];
  auto return_type_mods_res = transformModifiers(return_type_mods, ctx);
  ret.node.return_mods = return_type_mods_res.node;
  ret.diagnostics.append(return_type_mods_res.diagnostics);

  assert(return_type.branches.size() > 1);
  const Ast& return_type_expr = return_type[1];
  assert(isAstPrimaryExpr(return_type_expr.type));
  auto return_type_expr_res = transformPrimaryExpr(return_type_expr, ctx);
  ret.node.return_mods = std::move(return_type_mods_res.node);
  ret.diagnostics.append(return_type_mods_res.diagnostics);

  if (ast.branches.size() > 3) {
    const Ast& definition = ast[3];
    for (const Ast& statement : definition.branches) {
      auto stmt_res = transformFunctionalStatement(statement, ctx);
      ret.node.definition.push_back(stmt_res.node);
    }
  }

  return ret;
}

template<class BinopNodeT>
TransformResult<NodePrimaryExpr> transformBinaryOperation(const Ast& ast,
                                                          TransformationContext& ctx) noexcept {
  TransformResult<NodePrimaryExpr> result{};

  BinopNodeT node{};
  const Ast& lhs = ast[0];
  TransformResult<NodePrimaryExpr> lhs_node_result = transformPrimaryExpr(lhs, ctx);
  node.setLeftOperand(std::move(lhs_node_result.node));
  result.diagnostics.append(lhs_node_result.diagnostics);

  const Ast& rhs = ast[0];
  TransformResult<NodePrimaryExpr> rhs_node_result = transformPrimaryExpr(rhs, ctx);
  node.setRightOperand(std::move(rhs_node_result.node));
  result.diagnostics.append(rhs_node_result.diagnostics);

  auto& result_node = result.node.emplace<BinopNodeT>();
  result_node = std::move(node);
  return result;
}


TransformResult<NodePrimaryExpr> transformResolution(const Ast& ast,
  TransformationContext& ctx) noexcept {
  assert(ast.TypeIs(eAst::kResolutionOp));
  assert(ast.size() == 2);
  return transformBinaryOperation<NodeResolution>(ast, ctx);
}

//TransformResult<NodePrimaryExpr> transformListFold(const Ast& ast,
//                                                   TransformationContext& ctx) noexcept;
//TransformResult<NodePrimaryExpr> transformAssignment(const Ast& ast,
//                                                     TransformationContext& ctx) noexcept;
//TransformResult<NodePrimaryExpr> transformLogicalOr(const Ast& ast,
//                                                    TransformationContext& ctx) noexcept;
//TransformResult<NodePrimaryExpr> transformLogicalAnd(const Ast& ast,
//                                                     TransformationContext& ctx) noexcept;
//TransformResult<NodePrimaryExpr> transformBitwiseOr(const Ast& ast,
//                                                    TransformationContext& ctx) noexcept;
//TransformResult<NodePrimaryExpr> transformBitwiseXor(const Ast& ast,
//                                                     TransformationContext& ctx) noexcept;
//TransformResult<NodePrimaryExpr> transformBitwiseAnd(const Ast& ast,
//                                                     TransformationContext& ctx) noexcept;
//TransformResult<NodePrimaryExpr> transformEquality(const Ast& ast,
//                                                   TransformationContext& ctx) noexcept;
//TransformResult<NodePrimaryExpr> transformComparison(const Ast& ast,
//                                                     TransformationContext& ctx) noexcept;
//TransformResult<NodePrimaryExpr> transformThreeWayEquality(const Ast& ast,
//                                                           TransformationContext& ctx) noexcept;
//TransformResult<NodePrimaryExpr> transformBitwiseShift(const Ast& ast,
//                                                       TransformationContext& ctx) noexcept;
//TransformResult<NodePrimaryExpr> transformSummation(const Ast& ast,
//                                                    TransformationContext& ctx) noexcept;
//TransformResult<NodePrimaryExpr> transformProduction(const Ast& ast,
//                                                     TransformationContext& ctx) noexcept;
//TransformResult<NodePrimaryExpr> transformPrefix(const Ast& ast,
//                                                 TransformationContext& ctx) noexcept;
//TransformResult<NodePrimaryExpr> transformPostfix(const Ast& ast,
//                                                  TransformationContext& ctx) noexcept;


//TransformResult<NodeCStr> transformCStr(const Token& tk, const SourceManager& source_manager,
//                                StringInterner& string_table) {
//  assert(ast.type == eToken::kLitCstr && "Expected kLitCstr token type.");
//  TransformResult<NodeCStr> ret{};
//  ret.node.source_range = tk.source_range;
//  const SourceFile* src_file = source_manager.get(tk.source_range.file);
//  assert(src_file && "Invalid source file id. Source file must be loaded.");
//  std::string_view literal = src_file->slice(tk.source_range.begin, tk.source_range.end);
//  assert(literal.front() == '\"' &&
//         "Invalid literal const string value. Expected '\"' character at begin.");
//  assert(literal.back() == '\"' &&
//         "Invalid literal const string value. Expected '\"' character at end.");
//  literal = literal.substr(1, literal.size() - 2);
//
//  // https://en.cppreference.com/c/language/escape
//  bool is_escape = false;
//  std::string escaped_str = "";
//  for (auto it = literal.begin(); it < literal.end(); it++) {
//    if (is_escape) {
//      switch (*it) {
//        case '\'':
//          escaped_str += *it;
//          is_escape = false;
//          continue;
//        case '"':
//          escaped_str += *it;
//          is_escape = false;
//          continue;
//        case '?':
//          escaped_str += '\?';
//          is_escape = false;
//          continue;
//        case '\\':
//          escaped_str += *it;
//          is_escape = false;
//          continue;
//        case 'a':
//          escaped_str += '\a';
//          is_escape = false;
//          continue;
//        case 'b':
//          escaped_str += '\b';
//          is_escape = false;
//          continue;
//        case 'f':
//          escaped_str += '\f';
//          is_escape = false;
//          continue;
//        case 'n':
//          escaped_str += '\n';
//          is_escape = false;
//          continue;
//        case 'r':
//          escaped_str += '\r';
//          is_escape = false;
//          continue;
//        case 't':
//          escaped_str += '\t';
//          is_escape = false;
//          continue;
//        case 'v':
//          escaped_str += '\t';
//          is_escape = false;
//          continue;
//        default:
//          ret.diagnostics->push_back("Invalid escape sequence.");
//          escaped_str += *it;
//          is_escape = false;
//          continue;
//      }
//    } else {
//      if (*it == '\\') {
//        is_escape = true;
//      } else {
//        escaped_str += *it;
//      }
//    }
//  }
//
//  ret.node.data = string_table.push(escaped_str);
//  return ret;
//}
//
//TransformResult<NodeBool> transformBool(const Token& tk, const SourceManager& source_manager) {
//  assert(ast.type == eToken::kLitU1 || ast.type == eToken::kKwTrue ||
//         ast.type == eToken::kKwFalse && "Expected kLitU1,kKwTrue or kKwFalse token type.");
//  TransformResult<NodeBool> ret{};
//  ret.node.source_range = tk.source_range;
//  const SourceFile* src_file = source_manager.get(tk.source_range.file);
//  assert(src_file && "Invalid source file id. Source file must be loaded.");
//  std::string_view literal = src_file->slice(tk.source_range.begin, tk.source_range.end);
//  if (ast.type == eToken::kKwTrue) {
//    ret.node.data = true;
//  } else if (ast.type == eToken::kKwFalse) {
//    ret.node.data = false;
//  } else {
//    if (literal == "0b") {
//      ret.node.data = false;
//    } else if (literal == "1b") {
//      ret.node.data = true;
//    } else {
//      ret.diagnostics->push_back(makeErrorPlaceholder("Invalid i32 literal format."));
//      ret.node.data = false;
//    }
//  }
//  return ret;
//}
//
//TransformResult<NodeI8> transformI8(const Token& tk, const SourceManager& source_manager) {
//  assert(ast.type == eToken::kLitI8 && "Expected kLitI8 token type.");
//  TransformResult<NodeI8> ret{};
//  ret.node.source_range = tk.source_range;
//  const SourceFile* src_file = source_manager.get(tk.source_range.file);
//  assert(src_file && "Invalid source file id. Source file must be loaded.");
//  std::string_view literal = src_file->slice(tk.source_range.begin, tk.source_range.end);
//  if (literal.ends_with("i8")) {
//    literal.remove_suffix(2);
//  } else if (!isCharNumeric(literal.back())) {
//    ret.diagnostics->push_back(makeErrorPlaceholder("Invalid i8 literal suffix."));
//  }
//
//  auto [error_loc, error_code] =
//      std::from_chars(literal.data(), literal.data() + literal.size(), ret.node.data);
//
//  if (error_code == std::errc::invalid_argument) {
//    ret.diagnostics->push_back(makeErrorPlaceholder("Invalid i8 literal format."));
//  } else if (error_code == std::errc::result_out_of_range) {
//    ret.diagnostics->push_back(makeErrorPlaceholder("i8 literal value out of range."));
//  }
//
//  return ret;
//}
//
//TransformResult<NodeI16> transformI16(const Token& tk, const SourceManager& source_manager) {
//  assert(ast.type == eToken::kLitI16 && "Expected kLitI16 token type.");
//  TransformResult<NodeI16> ret{};
//  ret.node.source_range = tk.source_range;
//  const SourceFile* src_file = source_manager.get(tk.source_range.file);
//  assert(src_file && "Invalid source file id. Source file must be loaded.");
//  std::string_view literal = src_file->slice(tk.source_range.begin, tk.source_range.end);
//  if (literal.ends_with("i16")) {
//    literal.remove_suffix(3);
//  } else if (!isCharNumeric(literal.back())) {
//    ret.diagnostics->push_back(makeErrorPlaceholder("Invalid i16 literal suffix."));
//  }
//
//  auto [error_loc, error_code] =
//      std::from_chars(literal.data(), literal.data() + literal.size(), ret.node.data);
//
//  if (error_code == std::errc::invalid_argument) {
//    ret.diagnostics->push_back(makeErrorPlaceholder("Invalid i16 literal format."));
//  } else if (error_code == std::errc::result_out_of_range) {
//    ret.diagnostics->push_back(makeErrorPlaceholder("i16 literal value out of range."));
//  }
//
//  return ret;
//}
//
//TransformResult<NodeI32> transformI32(const Token& tk, const SourceManager& source_manager) {
//  assert(ast.type == eToken::kLitI32 && "Expected kLitI32 token type.");
//  TransformResult<NodeI32> ret{};
//  ret.node.source_range = tk.source_range;
//  const SourceFile* src_file = source_manager.get(tk.source_range.file);
//  assert(src_file && "Invalid source file id. Source file must be loaded.");
//  std::string_view literal = src_file->slice(tk.source_range.begin, tk.source_range.end);
//
//  if (literal.ends_with("i32")) {
//    literal.remove_suffix(3);
//  } else if (!isCharNumeric(literal.back())) {
//    ret.diagnostics->push_back(makeErrorPlaceholder("Invalid i32 literal suffix."));
//  }
//
//  auto [error_loc, error_code] =
//      std::from_chars(literal.data(), literal.data() + literal.size(), ret.node.data);
//
//  if (error_code == std::errc::invalid_argument) {
//    ret.diagnostics->push_back(makeErrorPlaceholder("Invalid i32 literal format."));
//  } else if (error_code == std::errc::result_out_of_range) {
//    ret.diagnostics->push_back(makeErrorPlaceholder("i32 literal value out of range."));
//  }
//
//  return ret;
//}
//
//TransformResult<NodeI64> transformI64(const Token& tk, const SourceManager& source_manager) {
//  assert(ast.type == eToken::kLitI64 && "Expected kLitI16 token type.");
//  TransformResult<NodeI64> ret{};
//  ret.node.source_range = tk.source_range;
//  const SourceFile* src_file = source_manager.get(tk.source_range.file);
//  assert(src_file && "Invalid source file id. Source file must be loaded.");
//  std::string_view literal = src_file->slice(tk.source_range.begin, tk.source_range.end);
//  if (literal.ends_with("i64")) {
//    literal.remove_suffix(3);
//  } else if (!isCharNumeric(literal.back())) {
//    ret.diagnostics->push_back(makeErrorPlaceholder("Invalid i64 literal suffix."));
//  }
//
//  auto [error_loc, error_code] =
//      std::from_chars(literal.data(), literal.data() + literal.size(), ret.node.data);
//
//  if (error_code == std::errc::invalid_argument) {
//    ret.diagnostics->push_back(makeErrorPlaceholder("Invalid i64 literal format."));
//  } else if (error_code == std::errc::result_out_of_range) {
//    ret.diagnostics->push_back(makeErrorPlaceholder("i64 literal value out of range."));
//  }
//
//  return ret;
//}
//
//TransformResult<NodeU8> transformU8(const Token& tk, const SourceManager& source_manager) {
//  assert(ast.type == eToken::kLitU8 && "Expected kLitU8 token type.");
//  TransformResult<NodeU8> ret{};
//  ret.node.source_range = tk.source_range;
//  const SourceFile* src_file = source_manager.get(tk.source_range.file);
//  assert(src_file && "Invalid source file id. Source file must be loaded.");
//  std::string_view literal = src_file->slice(tk.source_range.begin, tk.source_range.end);
//  if (literal.ends_with("u8")) {
//    literal.remove_suffix(2);
//  } else if (!isCharNumeric(literal.back())) {
//    ret.diagnostics->push_back(makeErrorPlaceholder("Invalid u8 literal suffix."));
//  }
//
//  auto [error_loc, error_code] =
//      std::from_chars(literal.data(), literal.data() + literal.size(), ret.node.data);
//
//  if (error_code == std::errc::invalid_argument) {
//    ret.diagnostics->push_back(makeErrorPlaceholder("Invalid u8 literal format."));
//  } else if (error_code == std::errc::result_out_of_range) {
//    ret.diagnostics->push_back(makeErrorPlaceholder("u8 literal value out of range."));
//  }
//
//  return ret;
//}
//
//TransformResult<NodeU16> transformU16(const Token& tk, const SourceManager& source_manager) {
//  assert(ast.type == eToken::kLitU16 && "Expected kLitU16 token type.");
//  TransformResult<NodeU16> ret{};
//  ret.node.source_range = tk.source_range;
//  const SourceFile* src_file = source_manager.get(tk.source_range.file);
//  assert(src_file && "Invalid source file id. Source file must be loaded.");
//  std::string_view literal = src_file->slice(tk.source_range.begin, tk.source_range.end);
//
//  if (literal.ends_with("u16")) {
//    literal.remove_suffix(3);
//  } else if (!isCharNumeric(literal.back())) {
//    ret.diagnostics->push_back(makeErrorPlaceholder("Invalid u16 literal suffix."));
//  }
//
//  auto [error_loc, error_code] =
//      std::from_chars(literal.data(), literal.data() + literal.size(), ret.node.data);
//
//  if (error_code == std::errc::invalid_argument) {
//    ret.diagnostics->push_back(makeErrorPlaceholder("Invalid u16 literal format."));
//  } else if (error_code == std::errc::result_out_of_range) {
//    ret.diagnostics->push_back(makeErrorPlaceholder("u16 literal value out of range."));
//  }
//
//  return ret;
//}
//
//TransformResult<NodeU32> transformU32(const Token& tk, const SourceManager& source_manager) {
//  assert(ast.type == eToken::kLitU32 && "Expected kLitU32 token type.");
//  TransformResult<NodeU32> ret{};
//  ret.node.source_range = tk.source_range;
//  const SourceFile* src_file = source_manager.get(tk.source_range.file);
//  assert(src_file && "Invalid source file id. Source file must be loaded.");
//  std::string_view literal = src_file->slice(tk.source_range.begin, tk.source_range.end);
//
//  if (literal.ends_with("u32")) {
//    literal.remove_suffix(3);
//  } else if (!isCharNumeric(literal.back())) {
//    ret.diagnostics->push_back(makeErrorPlaceholder("Invalid u32 literal suffix."));
//  }
//
//  auto [error_loc, error_code] =
//      std::from_chars(literal.data(), literal.data() + literal.size(), ret.node.data);
//
//  if (error_code == std::errc::invalid_argument) {
//    ret.diagnostics->push_back(makeErrorPlaceholder("Invalid u32 literal format."));
//  } else if (error_code == std::errc::result_out_of_range) {
//    ret.diagnostics->push_back(makeErrorPlaceholder("u32 literal value out of range."));
//  }
//
//  return ret;
//}
//
//TransformResult<NodeU64> transformU64(const Token& tk, const SourceManager& source_manager) {
//  assert(ast.type == eToken::kLitU64 && "Expected kLitU64 token type.");
//  TransformResult<NodeU64> ret{};
//  ret.node.source_range = tk.source_range;
//  const SourceFile* src_file = source_manager.get(tk.source_range.file);
//  assert(src_file && "Invalid source file id. Source file must be loaded.");
//  std::string_view literal = src_file->slice(tk.source_range.begin, tk.source_range.end);
//
//  if (literal.ends_with("u64")) {
//    literal.remove_suffix(3);
//  } else if (!isCharNumeric(literal.back())) {
//    ret.diagnostics->push_back(makeErrorPlaceholder("Invalid u64 literal suffix."));
//  }
//
//  auto [error_loc, error_code] =
//      std::from_chars(literal.data(), literal.data() + literal.size(), ret.node.data);
//
//  if (error_code == std::errc::invalid_argument) {
//    ret.diagnostics->push_back(makeErrorPlaceholder("Invalid u64 literal format."));
//  } else if (error_code == std::errc::result_out_of_range) {
//    ret.diagnostics->push_back(makeErrorPlaceholder("u64 literal value out of range."));
//  }
//
//  return ret;
//}
//
//TransformResult<NodeF32> transformF32(const Token& tk, const SourceManager& source_manager) {
//  assert(ast.type == eToken::kLitF32 && "Expected kLitF32 token type.");
//  TransformResult<NodeF32> ret{};
//  ret.node.source_range = tk.source_range;
//  const SourceFile* src_file = source_manager.get(tk.source_range.file);
//  assert(src_file && "Invalid source file id. Source file must be loaded.");
//  std::string_view literal = src_file->slice(tk.source_range.begin, tk.source_range.end);
//
//  if (literal.ends_with("f32")) {
//    literal.remove_suffix(3);
//  } else if (!isCharNumeric(literal.back())) {
//    ret.diagnostics->push_back(makeErrorPlaceholder("Invalid f32 literal suffix."));
//  }
//
//  auto [error_loc, error_code] =
//      std::from_chars(literal.data(), literal.data() + literal.size(), ret.node.data);
//
//  if (error_code == std::errc::invalid_argument) {
//    ret.diagnostics->push_back(makeErrorPlaceholder("Invalid f32 literal format."));
//  } else if (error_code == std::errc::result_out_of_range) {
//    ret.diagnostics->push_back(makeErrorPlaceholder("f32 literal value out of range."));
//  }
//
//  return ret;
//}
//
//TransformResult<NodeF64> transformF64(const Token& tk, const SourceManager& source_manager) {
//  assert(ast.type == eToken::kLitF64 && "Expected kLitF64 token type.");
//  TransformResult<NodeF64> ret{};
//  ret.node.source_range = tk.source_range;
//  const SourceFile* src_file = source_manager.get(tk.source_range.file);
//  assert(src_file && "Invalid source file id. Source file must be loaded.");
//  std::string_view literal = src_file->slice(tk.source_range.begin, tk.source_range.end);
//  if (literal.ends_with("f64")) {
//    literal.remove_suffix(3);
//  } else if (!isCharNumeric(literal.back())) {
//    ret.diagnostics->push_back(makeErrorPlaceholder("Invalid f64 literal suffix."));
//  }
//
//  auto [error_loc, error_code] =
//      std::from_chars(literal.data(), literal.data() + literal.size(), ret.node.data);
//
//  if (error_code == std::errc::invalid_argument) {
//    ret.diagnostics->push_back(makeErrorPlaceholder("Invalid f64 literal format."));
//  } else if (error_code == std::errc::result_out_of_range) {
//    ret.diagnostics->push_back(makeErrorPlaceholder("f64 literal value out of range."));
//  }
//
//  return ret;
//}
//
//TransformResult<NodeNone> transformNone(const Token& tk, const SourceManager& source_manager) {
//  assert(ast.type == eToken::kKwNone && "Expected kKwNone token type.");
//  TransformResult<NodeNone> ret{};
//  ret.node.source_range = tk.source_range;
//  return ret;
//}

}  // namespace ssgc::frontend