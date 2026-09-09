#pragma once
#include "../common/diagnostic.hpp"
#include "../common/source_manager.hpp"
#include "../common/source_range_raw.hpp"
#include "../common/string_interner.hpp"
#include "../frontend/char_traits.hpp"
#include "../frontend/token.hpp"
#include "token_cursor.hpp"

namespace ssgc {

struct NoneType {
  constexpr bool operator==(const NoneType&) const noexcept { return true; }
};

}

namespace ssgc::frontend {

struct None {
  constexpr bool operator==(const None&) const noexcept { return true; }
};

struct NodeBase;
struct NodeBaseBinaryOperator;
struct NodeBaseUnaryOperator;


// Util
struct NodePoison;

// Operands
struct NodeBool;
struct NodeI8;
struct NodeI16;
struct NodeI32;
struct NodeI64;
struct NodeU8;
struct NodeU16;
struct NodeU32;
struct NodeU64;
struct NodeF32;
struct NodeF64;
struct NodeNone;
struct NodeIdentifier;
struct NodeCStr;

struct NodeGlobalNamespaceIdentifier;

struct NodeTypenameVoid;
struct NodeTypenameAny;


struct NodeParenSubexpr;
struct NodeBracketSubexpr;
struct NodeBraceSubexpr;

// Operators
struct NodeListFold;
struct NodeAssignment;
struct NodeLogicalOr;
struct NodeLogicalAnd;
struct NodeBitwiseOr;
struct NodeBitwiseXor;
struct NodeBitwiseAnd;
struct NodeEquality;
struct NodeInequality;
struct NodeGreater;
struct NodeLess;
struct NodeGreaterEqual;
struct NodeLessEqual;
struct NodeBitwiseShiftRight;
struct NodeBitwiseShiftLeft;
struct NodeAdd;
struct NodeSubtract;
struct NodeMultiply;
struct NodeDivide;
struct NodeModulus;
struct NodePreIncrement;
struct NodePreDecrement;
struct NodeNot;
struct NodeNegative;
struct NodePositive;
struct NodeAccess;
struct NodePostIncrement;
struct NodePostDecrement;
struct NodeResolution;

struct NodeMethod;
struct NodeMethodParameter;
struct NodeFunctionalStatement;
struct NodePrimaryExpr;
struct NodeFile;
struct NodeSyntax;

using NodeUnionOperand =
    std::variant<NodeBool, NodeI8, NodeI16, NodeI32, NodeI64, NodeU8, NodeU16, NodeU32, NodeU64,
                 NodeF32, NodeF64, NodeIdentifier, NodeCStr, NodeNone, NodeParenSubexpr,
                 NodeBracketSubexpr, NodeBraceSubexpr>;
using NodeUnionOperator =
    std::variant<NodeListFold, NodeAssignment, NodeLogicalOr, NodeLogicalAnd, NodeBitwiseOr,
                 NodeBitwiseXor, NodeBitwiseAnd, NodeEquality, NodeInequality, NodeGreater,
                 NodeLess, NodeGreaterEqual, NodeLessEqual, NodeBitwiseShiftRight,
                 NodeBitwiseShiftLeft, NodeAdd, NodeSubtract, NodeMultiply, NodeDivide, NodeModulus,
                 NodePreIncrement, NodePreDecrement, NodeNot, NodeNegative, NodePositive,
                 NodeAccess, NodePostIncrement, NodePostDecrement, NodeResolution>;
using NodeUnionPrimary =
    std::variant<NodePoison, NodeBool, NodeI8, NodeI16, NodeI32, NodeI64, NodeU8, NodeU16, NodeU32,
                 NodeU64, NodeF32, NodeF64, NodeIdentifier, NodeCStr, NodeNone, NodeTypenameVoid,
                 NodeTypenameAny,
                 NodeGlobalNamespaceIdentifier, NodeParenSubexpr, NodeBracketSubexpr,
                 NodeBraceSubexpr, NodeListFold, NodeAssignment, NodeLogicalOr, NodeLogicalAnd,
                 NodeBitwiseOr, NodeBitwiseXor, NodeBitwiseAnd, NodeEquality, NodeInequality,
                 NodeGreater, NodeLess, NodeGreaterEqual, NodeLessEqual, NodeBitwiseShiftRight,
                 NodeBitwiseShiftLeft, NodeAdd, NodeSubtract, NodeMultiply, NodeDivide, NodeModulus,
                 NodePreIncrement, NodePreDecrement, NodeNot, NodeNegative, NodePositive,
                 NodeAccess, NodePostIncrement, NodePostDecrement, NodeResolution>;

struct NodeBase {
  SourceRangeRaw source_range{};
};

template <class T>
struct NodeBaseOperand : NodeBase {
  using ValueType = T;
  ValueType data{};
};

struct NodeBaseBinaryOperator : NodeBase {
  std::unique_ptr<NodePrimaryExpr> lhs;
  std::unique_ptr<NodePrimaryExpr> rhs;

  void setLeftOperand(NodePrimaryExpr&& expr) {
    lhs = std::make_unique<NodePrimaryExpr>(std::forward<NodePrimaryExpr>(expr));
  }

  void setRightOperand(NodePrimaryExpr&& expr) {
    rhs = std::make_unique<NodePrimaryExpr>(std::forward<NodePrimaryExpr>(expr));
  }
};

struct NodeBaseUnaryOperator : NodeBase {
  std::unique_ptr<NodePrimaryExpr> operand;
};

struct NodePoison : public NodeBase {};

struct NodeBool : NodeBaseOperand<bool> {};
struct NodeI8 : NodeBaseOperand<std::int8_t> {};
struct NodeI16 : NodeBaseOperand<std::int16_t> {};
struct NodeI32 : NodeBaseOperand<std::int32_t> {};
struct NodeI64 : NodeBaseOperand<std::int64_t> {};
struct NodeU8 : NodeBaseOperand<std::uint8_t> {};
struct NodeU16 : NodeBaseOperand<std::uint16_t> {};
struct NodeU32 : NodeBaseOperand<std::uint32_t> {};
struct NodeU64 : NodeBaseOperand<std::uint64_t> {};
struct NodeF32 : NodeBaseOperand<float> {};
struct NodeF64 : NodeBaseOperand<double> {};
struct NodeNone : NodeBaseOperand<None> {};
struct NodeCStr : NodeBaseOperand<std::size_t> {};
struct NodeIdentifier : NodeBaseOperand<std::size_t> {};
struct NodeGlobalNamespaceIdentifier : NodeBase {};
struct NodeTypenameVoid : NodeBase {};
struct NodeTypenameAny : NodeBase {};
struct NodeParenSubexpr : NodeBaseUnaryOperator {};
struct NodeBracketSubexpr : NodeBaseUnaryOperator {};
struct NodeBraceSubexpr : NodeBaseUnaryOperator {};

struct NodeListFold : NodeBaseBinaryOperator {};
struct NodeAssignment : NodeBaseBinaryOperator {};
struct NodeLogicalOr : NodeBaseBinaryOperator {};
struct NodeLogicalAnd : NodeBaseBinaryOperator {};
struct NodeBitwiseOr : NodeBaseBinaryOperator {};
struct NodeBitwiseXor : NodeBaseBinaryOperator {};
struct NodeBitwiseAnd : NodeBaseBinaryOperator {};
struct NodeEquality : NodeBaseBinaryOperator {};
struct NodeInequality : NodeBaseBinaryOperator {};
struct NodeGreater : NodeBaseBinaryOperator {};
struct NodeLess : NodeBaseBinaryOperator {};
struct NodeGreaterEqual : NodeBaseBinaryOperator {};
struct NodeLessEqual : NodeBaseBinaryOperator {};
struct NodeBitwiseShiftRight : NodeBaseBinaryOperator {};
struct NodeBitwiseShiftLeft : NodeBaseBinaryOperator {};
struct NodeAdd : NodeBaseBinaryOperator {};
struct NodeSubtract : NodeBaseBinaryOperator {};
struct NodeMultiply : NodeBaseBinaryOperator {};
struct NodeDivide : NodeBaseBinaryOperator {};
struct NodeModulus : NodeBaseBinaryOperator {};
struct NodePreIncrement : NodeBaseUnaryOperator {};
struct NodePreDecrement : NodeBaseUnaryOperator {};
struct NodeNot : NodeBaseUnaryOperator {};
struct NodeNegative : NodeBaseUnaryOperator {};
struct NodePositive : NodeBaseUnaryOperator {};
struct NodeAccess : NodeBaseBinaryOperator {};
struct NodePostIncrement : NodeBaseUnaryOperator {};
struct NodePostDecrement : NodeBaseUnaryOperator {};
struct NodeResolution : NodeBaseBinaryOperator {};

struct NodePrimaryExpr : NodeUnionPrimary {
  using variant::variant;
  SourceRangeRaw* sourceRange() {
    return std::visit([](auto& self) { return &self.source_range; }, *this);
  }

  std::size_t sourceBegin() const {
    return std::visit([](const auto& self) { return self.source_range.begin; }, *this);
  }

  std::size_t sourceEnd() const {
    return std::visit([](const auto& self) { return self.source_range.end; }, *this);
  }

  void setSourceBegin(std::size_t begin) {
    std::visit([&begin](auto& self) { self.source_range.begin = begin; }, *this);
  }

  void setSourceEnd(std::size_t end) {
    std::visit([&end](auto& self) { self.source_range.end = end; }, *this);
  }

  NodeBaseBinaryOperator* asBinaryOperator() {
    return std::visit(
        [](auto& self) {
          if constexpr (std::is_base_of<NodeBaseBinaryOperator, decltype(self)>::value) {
            return static_cast<NodeBaseBinaryOperator*>(&self);
          } else {
            return nullptr;
          }
        },
        *this);
  }
};

struct NodeFunctionalStatement : NodeBase {};

struct NodeModifiers : NodeBase {
  
};

struct NodeMethodParameter : NodeBase {
  NodeModifiers mods{};
  NodePrimaryExpr type{};
  NodePrimaryExpr name{};
};

struct NodeMethod : NodeBase {
  NodeModifiers mods{};
  NodeIdentifier name{};
  std::vector<NodeMethodParameter> params{};
  NodeModifiers return_mods{};
  NodePrimaryExpr return_type{};

  std::vector<NodeFunctionalStatement> definition{};
};

struct NodeFile : NodeBase {
  SourceFileId id{};

};

struct NodeSyntax {
  std::list<NodeFile> files{};
};

//ParseResult<SyntaxBool> parseBool(const Token& tk, const SourceManager& source_manager);
//ParseResult<SyntaxI8> parseI8(const Token& tk, const SourceManager& source_manager);
//ParseResult<SyntaxI16> parseI16(const Token& tk, const SourceManager& source_manager);
//ParseResult<SyntaxI32> parseI32(const Token& tk, const SourceManager& source_manager);
//ParseResult<SyntaxI64> parseI64(const Token& tk, const SourceManager& source_manager);
//ParseResult<SyntaxU8> parseU8(const Token& tk, const SourceManager& source_manager);
//ParseResult<SyntaxU16> parseU16(const Token& tk, const SourceManager& source_manager);
//ParseResult<SyntaxU32> parseU32(const Token& tk, const SourceManager& source_manager);
//ParseResult<SyntaxU64> parseU64(const Token& tk, const SourceManager& source_manager);
//ParseResult<SyntaxF32> parseF32(const Token& tk, const SourceManager& source_manager);
//ParseResult<SyntaxF64> parseF64(const Token& tk, const SourceManager& source_manager);
//ParseResult<SyntaxNone> parseNone(const Token& tk, const SourceManager& source_manager);
//
//ParseResult<SyntaxIdentifier> parseIdentifier(const Token& tk, const SourceManager& source_manager,
//                                            StringInterner& id_table);
//ParseResult<SyntaxCStr> parseCStr(const Token& tk, const SourceManager& source_manager,
//                                StringInterner& string_table);
//LLParseResult<SyntaxPrimaryExpr> parseValueOperand(TokenCursor c, ParsingContext& ctx);
//LLParseResult<SyntaxParenSubexpr> parseParenSubexpr(TokenCursor c, ParsingContext& ctx) noexcept;
//LLParseResult<SyntaxBraceSubexpr> parseBraceSubexpr(TokenCursor c, ParsingContext& ctx) noexcept;
//LLParseResult<SyntaxBracketSubexpr> parseBracketSubexpr(TokenCursor c, ParsingContext& ctx) noexcept;
//LLParseResult<SyntaxPrimaryExpr> parseOperand(TokenCursor c, ParsingContext& ctx) noexcept;
//
//// namespace detail {
//// LLParseResult<SyntaxPrimaryExpr> parseGenericBinaryLeftAssociative(
////     TokenCursor c, bool (*next_cond)(const TokenCursor&),
////     LLParseResult<SyntaxPrimaryExpr> (*operand_parser)(TokenCursor)) noexcept {
////   assert(isTokenPrimarySpecifier(c.kind()) && "Unexpected token at start of binary operation.");
////
////   LLParseResult<SyntaxPrimaryExpr> first_op = operand_parser(c);
////   if (first_op.failed()) {
////     return first_op;
////   }
////   c.advanceTo(first_op.head);
////
////   std::vector<SyntaxPrimaryExpr> accum_rhs{};
////   while (next_cond(c)) {
////     accum_rhs.emplace_back(c);
////     c.advance();
////     auto next_rhs = operand_parser(c);
////     if (next_rhs.failed()) {
////       return next_rhs;
////     }
////     c.advanceTo(next_rhs.head);
////     accum_rhs.push_back(next_rhs.node);
////   }
////
////   // Associativity Left -> Right.
////   // Note the accum vector holds (operand -> operator) pairs to keep track of the specific
////   operator
////   // type.
////   std::vector<SyntaxPrimaryExpr> reduced_binop{};
////   reduced_binop.push_back(std::move(first_op.node));
////   for (auto it = accum_rhs.begin(); it != accum_rhs.end(); it++) {
////     auto& prev_op = reduced_binop.back();
////     SyntaxPrimaryExpr next_binop = std::move(*it);
////     it++;
////     next_binop.setSourceBegin(prev_op.sourceBegin());
////     next_binop.lhs.push_back(std::move(prev_op));
////     //next_binop.size = it->sourceEnd() - next_binop.index;
////     next_binop.rhs.push_back(*it);
////     reduced_binop.pop_back();
////     reduced_binop.push_back(next_binop);
////   }
////   accum_rhs.clear();
////
////   CND_DEBUG_ASSERT(
////       reduced_binop.size() == 1,
////       "[At the of reduction, reduced production vector must contain a single ast node.]");
////
////   c.advanceTo(reduced_binop.back().sourceEnd());
////   return LLParserResult{c.index(), reduced_binop.back()};
//// };
//// }  // namespace detail
//
//LLParseResult<SyntaxPrimaryExpr> parseListFold(TokenCursor c, ParsingContext& ctx) noexcept {
//  return {};
//};
//
//// [L->R] <resolution> ::= <operand> (<DOUBLE_COLON> <operand>) *
////                 | <DOUBLE_COLON> <operand> (<DOUBLE_COLON> <operand>) *
//LLParseResult<SyntaxPrimaryExpr> parseResolution(TokenCursor c, ParsingContext& ctx) noexcept {
//  assert(!isTokenPrimarySpecifier(c.kind()) && "Unexpected token at start of binary resolution.");
//  // CND_DEBUG_ASSERT(
//  //     !c.IsPrefixOperator(),
//  //     "A prefix cannot occur at this stage, it should have been parsed first by precedence.");
//
//  // Retrieve the first left hand side operand/subexpression.
//  // Handle special case where '::' is a prefix, the first operator becomes kRootNamespace.
//  SyntaxPrimaryExpr first_op{};
//  if (c.is(eToken::kDoubleColon)) {
//    first_op = SyntaxGlobalNamespaceIdentifier{c.get().source_range};
//  } else {
//    LLParseResult<SyntaxPrimaryExpr> first_op_res = parseOperand(c, ctx);
//    if (first_op_res.failed()) {
//      return first_op_res;
//    }
//    c.advance(first_op_res.head);
//    first_op = std::move(first_op_res.node);
//  }
//
//  std::vector<SyntaxPrimaryExpr> accum_rhs{};
//  while (c.is(eToken::kDoubleColon)) {
//    SyntaxResolution node{};
//    node.source_range.file = c.get().source_range.file;
//    node.source_range.begin = c.get().source_range.begin;
//    c.advance();
//    node.source_range.end = c.get().source_range.begin;
//    accum_rhs.push_back(std::move(node));
//
//    LLParseResult<SyntaxPrimaryExpr> next_rhs = parseOperand(c, ctx);
//    if (next_rhs.failed()) {
//      return next_rhs;
//    }
//    c.advanceTo(next_rhs.head);
//    accum_rhs.push_back(std::move(next_rhs.node));
//  }
//
//  std::vector<SyntaxPrimaryExpr> reduced_binop{};
//  reduced_binop.push_back(std::move(first_op));
//  for (auto it = accum_rhs.begin(); it != accum_rhs.end();) {
//    SyntaxPrimaryExpr& next_operator = *it;
//    SyntaxPrimaryExpr& prev_operand = reduced_binop.back();
//    it++;
//    SyntaxPrimaryExpr& next_operand = *it;
//    it++;
//
//    SyntaxBaseBinaryOperator* next_binop = next_operator.asBinaryOperator();
//    assert(next_binop != nullptr && "Expected binary operator node.");
//    next_binop->source_range.begin = prev_operand.sourceBegin();
//    next_binop->lhs = std::make_unique<SyntaxPrimaryExpr>(std::move(prev_operand));
//    next_binop->source_range.end = next_operand.sourceEnd();
//    next_binop->rhs = std::make_unique<SyntaxPrimaryExpr>(std::move(next_operand));
//    reduced_binop.pop_back();
//    reduced_binop.push_back(std::move(next_operator));
//  }
//  accum_rhs.clear();
//  CND_DEBUG_ASSERT(
//      reduced_binop.size() == 1,
//      "[At the of reduction, reduced production vector must contain a single ast node.]");
//
//  LLParseResult<SyntaxPrimaryExpr> ret{};
//  ret.head = c.index();
//  ret.node = std::move(reduced_binop.back());
//  return ret;
//};
//
//LLParseResult<SyntaxPrimaryExpr> parseOperand(TokenCursor c, ParsingContext& ctx) noexcept {
//  if (isTokenAnOperand(c.kind())) {
//    return parseValueOperand(c, ctx);
//  } else if (c.is(eToken::kLParen)) {
//    return LLParseResult<SyntaxPrimaryExpr>::from_result(parseParenSubexpr(c, ctx));
//  } else if (c.is(eToken::kLBracket)) {
//    return LLParseResult<SyntaxPrimaryExpr>::from_result(parseBracketSubexpr(c, ctx));
//  } else if (c.is(eToken::kLBrace)) {
//    return LLParseResult<SyntaxPrimaryExpr>::from_result(parseBraceSubexpr(c, ctx));
//  } else {
//    LLParseResult<SyntaxPrimaryExpr> ret{};
//    ret.diagnostics->push_back("Expected an singular operand or operand set.");
//    ret.head = c.index();
//    ret.node = SyntaxPoison{c.get().source_range};
//    return ret;
//  }
//}
//
//LLParseResult<SyntaxParenSubexpr> parseParenSubexpr(TokenCursor c, ParsingContext& ctx) noexcept {
//  assert(c.is(eToken::kLParen) && "Expected a left parentheses.");
//  LLParseResult<SyntaxParenSubexpr> ret{};
//  ret.node.source_range = c.get().source_range;
//  c.advance();
//
//  auto subexpr_result = parseListFold(c, ctx);
//  ret.diagnostics = std::move(subexpr_result.diagnostics);
//  ret.head = subexpr_result.head;
//  ret.node.operand = std::make_unique<SyntaxPrimaryExpr>(std::move(subexpr_result.node));
//  if (subexpr_result.failed()) {
//    ret.node.source_range.end = subexpr_result.node.sourceEnd();
//    return ret;
//  } else {
//    c.advanceTo(subexpr_result.head);
//  }
//
//  if (!c.is(eToken::kRParen)) {
//    ret.diagnostics->push_back(makeErrorPlaceholder("Expected a right parentheses."));
//    ret.node.source_range.end = subexpr_result.node.sourceEnd();
//    return ret;
//  }
//  c.advance();
//  ret.node.source_range.end = c.get().source_range.end;
//  return ret;
//}
//
//LLParseResult<SyntaxBracketSubexpr> parseBracketSubexpr(TokenCursor c, ParsingContext& ctx) noexcept {
//  assert(c.is(eToken::kLBracket) && "Expected a left bracket.");
//  LLParseResult<SyntaxBracketSubexpr> ret{};
//  ret.node.source_range = c.get().source_range;
//  c.advance();
//
//  auto subexpr_result = parseListFold(c, ctx);
//  ret.diagnostics = std::move(subexpr_result.diagnostics);
//  ret.head = subexpr_result.head;
//  ret.node.operand = std::make_unique<SyntaxPrimaryExpr>(std::move(subexpr_result.node));
//  if (subexpr_result.failed()) {
//    ret.node.source_range.end = subexpr_result.node.sourceEnd();
//    return ret;
//  } else {
//    c.advanceTo(subexpr_result.head);
//  }
//
//  if (!c.is(eToken::kRBracket)) {
//    ret.diagnostics->push_back(makeErrorPlaceholder("Expected a right bracket."));
//    ret.node.source_range.end = subexpr_result.node.sourceEnd();
//    return ret;
//  }
//  c.advance();
//  ret.node.source_range.end = c.get().source_range.end;
//  return ret;
//}
//
//LLParseResult<SyntaxBraceSubexpr> parseBraceSubexpr(TokenCursor c, ParsingContext& ctx) noexcept {
//  assert(c.is(eToken::kLBrace) && "Expected a left brace.");
//  LLParseResult<SyntaxBraceSubexpr> ret{};
//  ret.node.source_range = c.get().source_range;
//  c.advance();
//
//  auto subexpr_result = parseListFold(c, ctx);
//  ret.diagnostics = std::move(subexpr_result.diagnostics);
//  ret.head = subexpr_result.head;
//  ret.node.operand = std::make_unique<SyntaxPrimaryExpr>(std::move(subexpr_result.node));
//  if (subexpr_result.failed()) {
//    ret.node.source_range.end = subexpr_result.node.sourceEnd();
//    return ret;
//  } else {
//    c.advanceTo(subexpr_result.head);
//  }
//
//  if (!c.is(eToken::kRBrace)) {
//    ret.diagnostics->push_back(makeErrorPlaceholder("Expected a right brace."));
//    ret.node.source_range.end = subexpr_result.node.sourceEnd();
//    return ret;
//  }
//  c.advance();
//  ret.node.source_range.end = c.get().source_range.end;
//  return ret;
//}
//
//ParseResult<SyntaxIdentifier> parseIdentifier(const Token& tk, const SourceManager& source_manager,
//                                            StringInterner& id_table) {
//  assert(tk.kind == eToken::kIdent && "Expected kIdent token type.");
//  ParseResult<SyntaxIdentifier> ret{};
//  ret.node.source_range = tk.source_range;
//  const SourceFile* src_file = source_manager.get(tk.source_range.file);
//  assert(src_file && "Invalid source file id. Source file must be loaded.");
//  std::string_view literal = src_file->slice(tk.source_range.begin, tk.source_range.end);
//  ret.node.data = id_table.push(literal);
//  return ret;
//}
//
//ParseResult<SyntaxCStr> parseCStr(const Token& tk, const SourceManager& source_manager,
//                                StringInterner& string_table) {
//  assert(tk.kind == eToken::kLitCstr && "Expected kLitCstr token type.");
//  ParseResult<SyntaxCStr> ret{};
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
//ParseResult<SyntaxBool> parseBool(const Token& tk, const SourceManager& source_manager) {
//  assert(tk.kind == eToken::kLitU1 || tk.kind == eToken::kKwTrue ||
//         tk.kind == eToken::kKwFalse && "Expected kLitU1,kKwTrue or kKwFalse token type.");
//  ParseResult<SyntaxBool> ret{};
//  ret.node.source_range = tk.source_range;
//  const SourceFile* src_file = source_manager.get(tk.source_range.file);
//  assert(src_file && "Invalid source file id. Source file must be loaded.");
//  std::string_view literal = src_file->slice(tk.source_range.begin, tk.source_range.end);
//  if (tk.kind == eToken::kKwTrue) {
//    ret.node.data = true;
//  } else if (tk.kind == eToken::kKwFalse) {
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
//ParseResult<SyntaxI8> parseI8(const Token& tk, const SourceManager& source_manager) {
//  assert(tk.kind == eToken::kLitI8 && "Expected kLitI8 token type.");
//  ParseResult<SyntaxI8> ret{};
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
//ParseResult<SyntaxI16> parseI16(const Token& tk, const SourceManager& source_manager) {
//  assert(tk.kind == eToken::kLitI16 && "Expected kLitI16 token type.");
//  ParseResult<SyntaxI16> ret{};
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
//ParseResult<SyntaxI32> parseI32(const Token& tk, const SourceManager& source_manager) {
//  assert(tk.kind == eToken::kLitI32 && "Expected kLitI32 token type.");
//  ParseResult<SyntaxI32> ret{};
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
//ParseResult<SyntaxI64> parseI64(const Token& tk, const SourceManager& source_manager) {
//  assert(tk.kind == eToken::kLitI64 && "Expected kLitI16 token type.");
//  ParseResult<SyntaxI64> ret{};
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
//ParseResult<SyntaxU8> parseU8(const Token& tk, const SourceManager& source_manager) {
//  assert(tk.kind == eToken::kLitU8 && "Expected kLitU8 token type.");
//  ParseResult<SyntaxU8> ret{};
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
//ParseResult<SyntaxU16> parseU16(const Token& tk, const SourceManager& source_manager) {
//  assert(tk.kind == eToken::kLitU16 && "Expected kLitU16 token type.");
//  ParseResult<SyntaxU16> ret{};
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
//ParseResult<SyntaxU32> parseU32(const Token& tk, const SourceManager& source_manager) {
//  assert(tk.kind == eToken::kLitU32 && "Expected kLitU32 token type.");
//  ParseResult<SyntaxU32> ret{};
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
//ParseResult<SyntaxU64> parseU64(const Token& tk, const SourceManager& source_manager) {
//  assert(tk.kind == eToken::kLitU64 && "Expected kLitU64 token type.");
//  ParseResult<SyntaxU64> ret{};
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
//ParseResult<SyntaxF32> parseF32(const Token& tk, const SourceManager& source_manager) {
//  assert(tk.kind == eToken::kLitF32 && "Expected kLitF32 token type.");
//  ParseResult<SyntaxF32> ret{};
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
//ParseResult<SyntaxF64> parseF64(const Token& tk, const SourceManager& source_manager) {
//  assert(tk.kind == eToken::kLitF64 && "Expected kLitF64 token type.");
//  ParseResult<SyntaxF64> ret{};
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
//ParseResult<SyntaxNone> parseNone(const Token& tk, const SourceManager& source_manager) {
//  assert(tk.kind == eToken::kKwNone && "Expected kKwNone token type.");
//  ParseResult<SyntaxNone> ret{};
//  ret.node.source_range = tk.source_range;
//  return ret;
//}
//
//LLParseResult<SyntaxPrimaryExpr> parseValueOperand(TokenCursor c, ParsingContext& ctx) {
//  assert(isTokenAnOperand(c.kind()) && "Expected operand token.");
//  switch (c.kind()) {
//    case eToken::kKwTrue:
//    case eToken::kKwFalse:
//    case eToken::kLitU1: {
//      auto res = parseBool(c.get(), ctx.source_manager);
//      return {.node = res.node, .diagnostics = res.diagnostics, .head = c.advance().index()};
//    } break;
//    case eToken::kIdent: {
//      auto res = parseIdentifier(c.get(), ctx.source_manager, ctx.id_table);
//      return {.node = res.node, .diagnostics = res.diagnostics, .head = c.advance().index()};
//    } break;
//    case eToken::kLitCstr: {
//      auto res = parseCStr(c.get(), ctx.source_manager, ctx.string_table);
//      return {.node = res.node, .diagnostics = res.diagnostics, .head = c.advance().index()};
//    } break;
//    case eToken::kLitI8: {
//      auto res = parseI8(c.get(), ctx.source_manager);
//      return {.node = res.node, .diagnostics = res.diagnostics, .head = c.advance().index()};
//    } break;
//    case eToken::kLitI16: {
//      auto res = parseI16(c.get(), ctx.source_manager);
//      return {.node = res.node, .diagnostics = res.diagnostics, .head = c.advance().index()};
//    } break;
//    case eToken::kLitI32: {
//      auto res = parseI32(c.get(), ctx.source_manager);
//      return {.node = res.node, .diagnostics = res.diagnostics, .head = c.advance().index()};
//    } break;
//    case eToken::kLitI64: {
//      auto res = parseI64(c.get(), ctx.source_manager);
//      return {.node = res.node, .diagnostics = res.diagnostics, .head = c.advance().index()};
//    } break;
//    case eToken::kLitU8: {
//      auto res = parseI8(c.get(), ctx.source_manager);
//      return {.node = res.node, .diagnostics = res.diagnostics, .head = c.advance().index()};
//    } break;
//    case eToken::kLitU16: {
//      auto res = parseI16(c.get(), ctx.source_manager);
//      return {.node = res.node, .diagnostics = res.diagnostics, .head = c.advance().index()};
//    } break;
//    case eToken::kLitU32: {
//      auto res = parseI32(c.get(), ctx.source_manager);
//      return {.node = res.node, .diagnostics = res.diagnostics, .head = c.advance().index()};
//    } break;
//    case eToken::kLitU64: {
//      auto res = parseI64(c.get(), ctx.source_manager);
//      return {.node = res.node, .diagnostics = res.diagnostics, .head = c.advance().index()};
//    } break;
//    case eToken::kLitF32: {
//      auto res = parseF64(c.get(), ctx.source_manager);
//      return {.node = res.node, .diagnostics = res.diagnostics, .head = c.advance().index()};
//    } break;
//    case eToken::kLitF64: {
//      auto res = parseF64(c.get(), ctx.source_manager);
//      return {.node = res.node, .diagnostics = res.diagnostics, .head = c.advance().index()};
//    } break;
//    case eToken::kKwNone: {
//      auto res = parseNone(c.get(), ctx.source_manager);
//      return {.node = res.node, .diagnostics = res.diagnostics, .head = c.advance().index()};
//    } break;
//    default:
//      assert(false && "Unknown or unimplemented operand kind.");
//      return {.node = SyntaxNone{}, .diagnostics = Diagnostics{}, .head = c.index()};
//  }
//}
}  // namespace ssgc::frontend