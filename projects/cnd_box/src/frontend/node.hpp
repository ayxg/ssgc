#pragma once
#include <charconv>
#include <memory>
#include <variant>

#include "../common/diagnostic.hpp"
#include "../compiler/translation_context.hpp"
#include "ast.hpp"
#include "enum_ast.hpp"
#include "token.hpp"
#include "char_traits.hpp"

//namespace ssgc::frontend {
//
//std::string_view getTokenLiteral(const TrContext& ctx, const Token& tk) {
//  const util::SourceFile* src_file = ctx.sources.get(tk.file);
//  if (!src_file) {
//    return "";
//  }
//  return std::string_view{&src_file->data[tk.offset], tk.size};
//}
//
//std::string_view getAstLiteral(const TrContext& ctx, const Ast& ast) {
//  const Token& tk = ctx.tokens[ast.index];
//  return getTokenLiteral(ctx, tk);
//}
//
//template <typename T>
//struct ProcessResult {
//  T node;
//  Diagnostics diagnostics;
//};
//
//struct SourceLocation {
//  std::size_t file;
//  std::size_t index;
//  std::size_t size;
//};
//
//struct BaseNode {
//  SourceLocation source_location;
//};
//
//struct BaseNodeBinaryOperator;
//
//struct UnionNodePragmaticStatement;
//struct UnionNodeDeclarativeStatement;
//struct UnionNodeFunctionalStatement;
//struct UnionNodeExpression;
//
//struct NodeBool;
//struct NodeI8;
//struct NodeI16;
//struct NodeI32;
//struct NodeI64;
//struct NodeU8;
//struct NodeU16;
//struct NodeU32;
//struct NodeU64;
//struct NodeF32;
//struct NodeF64;
//
//struct NodeString;
//struct NodeIdentifier;
//struct NodeTypename;
//
//struct NodeAssignment;
//struct NodeLogicalOr;
//struct NodeLogicalAnd;
//struct NodeBitwiseOr;
//struct NodeBitwiseXor;
//struct NodeBitwiseAnd;
//struct NodeEquality;
//struct NodeInequality;
//struct NodeGreater;
//struct NodeLess;
//struct NodeGreaterEqual;
//struct NodeLessEqual;
//struct NodeBitwiseShiftRight;
//struct NodeBitwiseShiftLeft;
//struct NodeAdd;
//struct NodeSubtract;
//struct NodeMultiply;
//struct NodeDivide;
//struct NodeModulus;
//struct NodePreIncrement;
//struct NodePreDecrement;
//struct NodeNot;
//struct NodeNegative;
//struct NodePositive;
//struct NodeAccess;
//struct NodePostIncrement;
//struct NodePostDecrement;
//struct NodeResolution;
//
//struct NodeFile;
//struct NodeInclude;
//struct NodeNamespace;
//struct NodeVariableDefintion;
//struct NodeVariableModifiers;
//struct NodeFunctionDeclaration;
//struct NodeFunctionDefinition;
//struct NodeFunctionParameter;
//struct NodeStructDeclaration;
//struct NodeStructDefinition;
//struct NodeMember;
//struct NodeClass;
//
//struct NodeIfStatement;
//struct NodeReturnStatement;
//struct NodeWhileStatement;
//
//using UnionNodeExpressionUnderlyingT = std::variant<
//    std::unique_ptr<NodeBool>, std::unique_ptr<NodeI8>, std::unique_ptr<NodeI16>,
//    std::unique_ptr<NodeI32>, std::unique_ptr<NodeI64>, std::unique_ptr<NodeU8>,
//    std::unique_ptr<NodeU16>, std::unique_ptr<NodeU32>, std::unique_ptr<NodeU64>,
//    std::unique_ptr<NodeF32>, std::unique_ptr<NodeF64>, std::unique_ptr<NodeString>,
//    std::unique_ptr<NodeTypename>, std::unique_ptr<NodeIdentifier>,
//
//    std::unique_ptr<NodeAssignment>, std::unique_ptr<NodeLogicalOr>,
//    std::unique_ptr<NodeLogicalAnd>, std::unique_ptr<NodeBitwiseOr>,
//    std::unique_ptr<NodeBitwiseXor>, std::unique_ptr<NodeBitwiseAnd>, std::unique_ptr<NodeEquality>,
//    std::unique_ptr<NodeInequality>, std::unique_ptr<NodeGreater>, std::unique_ptr<NodeLess>,
//    std::unique_ptr<NodeGreaterEqual>, std::unique_ptr<NodeLessEqual>,
//    std::unique_ptr<NodeBitwiseShiftRight>, std::unique_ptr<NodeBitwiseShiftLeft>,
//    std::unique_ptr<NodeAdd>, std::unique_ptr<NodeSubtract>, std::unique_ptr<NodeMultiply>,
//    std::unique_ptr<NodeDivide>, std::unique_ptr<NodeModulus>, std::unique_ptr<NodePreIncrement>,
//    std::unique_ptr<NodePreDecrement>, std::unique_ptr<NodeNot>, std::unique_ptr<NodeNegative>,
//    std::unique_ptr<NodePositive>, std::unique_ptr<NodeAccess>, std::unique_ptr<NodePostIncrement>,
//    std::unique_ptr<NodePostDecrement>, std::unique_ptr<NodeResolution>>;
//
//using UnionNodeFunctionalStatementUnderlyingT =
//    std::variant<UnionNodeExpression, std::unique_ptr<NodeVariableDefintion>,
//                 std::unique_ptr<NodeIfStatement>, std::unique_ptr<NodeReturnStatement>,
//                 std::unique_ptr<NodeWhileStatement>>;
//
//struct UnionNodeExpression : UnionNodeExpressionUnderlyingT {
//  using variant::variant;
//
//  static ProcessResult<UnionNodeExpression> fromAst(const Ast& ast) {
//    return ProcessResult<UnionNodeExpression>();
//  }
//};
//
//struct UnionNodeFunctionalStatement : UnionNodeFunctionalStatementUnderlyingT {
//  using variant::variant;
//};
//
//struct BaseNodeBinaryOperator : BaseNode {
//  UnionNodeExpression lhs_operand{};
//  UnionNodeExpression rhs_operand{};
//};
//
//struct BaseNodeUnaryOperator : BaseNode {
//  UnionNodeExpression operand{};
//};
//
//struct NodeBool : BaseNode {
//  using ValueType = bool;
//  ValueType data{false};
//  static ProcessResult<NodeBool> fromAst(const TrContext& ctx, const Ast& ast) {
//    return ProcessResult<NodeBool>();
//  }
//};
//
//struct NodeI8 : BaseNode {
//  using ValueType = std::int8_t;
//  ValueType data{0};
//  static ProcessResult<NodeI8> fromAst(const TrContext& ctx, const Ast& ast) {
//    assert(ast.TypeIs(eAst::kLitI8) && "Expected kLitI8 ast type.");
//    ProcessResult<NodeI8> ret{};
//    const Token& tk = ctx.tokens[ast.index];
//    ret.node.source_location.file = tk.file;
//    ret.node.source_location.index = tk.offset;
//    ret.node.source_location.size = tk.size;
//
//    std::string_view literal = getTokenLiteral(ctx, tk);
//
//    if (literal.ends_with("i8")) {
//      literal.remove_suffix(2);
//    } else if (literal.ends_with("c")) {
//      literal.remove_suffix(1);
//    } else if (!isCharNumeric(literal.back())) {
//      ret.diagnostics->push_back(makeErrorPlaceholder("Invalid i8 literal suffix."));
//    }
//
//    auto [error_loc, error_code] =
//        std::from_chars(literal.data(), literal.data() + literal.size(), ret.node.data);
//
//    if (error_code == std::errc::invalid_argument) {
//      ret.diagnostics->push_back(makeErrorPlaceholder("Invalid i8 literal format."));
//    } else if (error_code == std::errc::result_out_of_range) {
//      ret.diagnostics->push_back(makeErrorPlaceholder("i8 literal value out of range."));
//    }
//
//    return ret;
//  }
//};
//
//struct NodeI16 : BaseNode {
//  using ValueType = std::int16_t;
//  ValueType data{0};
//  static ProcessResult<NodeI16> fromAst(const TrContext& ctx, const Ast& ast) {
//    assert(ast.TypeIs(eAst::kLitI16) && "Expected kLitI16 ast type.");
//    ProcessResult<NodeI16> ret{};
//    const Token& tk = ctx.tokens[ast.index];
//    ret.node.source_location.file = tk.file;
//    ret.node.source_location.index = tk.offset;
//    ret.node.source_location.size = tk.size;
//
//    std::string_view literal = getTokenLiteral(ctx, tk);
//
//    if (literal.ends_with("i16")) {
//      literal.remove_suffix(3);
//    } else if (!isCharNumeric(literal.back())) {
//      ret.diagnostics->push_back(makeErrorPlaceholder("Invalid i16 literal suffix."));
//    }
//
//    auto [error_loc, error_code] =
//        std::from_chars(literal.data(), literal.data() + literal.size(), ret.node.data);
//
//    if (error_code == std::errc::invalid_argument) {
//      ret.diagnostics->push_back(makeErrorPlaceholder("Invalid i16 literal format."));
//    } else if (error_code == std::errc::result_out_of_range) {
//      ret.diagnostics->push_back(makeErrorPlaceholder("i16 literal value out of range."));
//    }
//
//    return ret;
//  }
//};
//
//struct NodeI32 : BaseNode {
//  using ValueType = std::int32_t;
//  ValueType data{0};
//
//  static ProcessResult<NodeI32> fromAst(const TrContext& ctx, const Ast& ast) {
//    assert(ast.TypeIs(eAst::kLitI32) && "Expected kLitI32 ast type.");
//    ProcessResult<NodeI32> ret{};
//    const Token& tk = ctx.tokens[ast.index];
//    ret.node.source_location.file = tk.file;
//    ret.node.source_location.index = tk.offset;
//    ret.node.source_location.size = tk.size;
//
//    std::string_view literal = getTokenLiteral(ctx, tk);
//
//    if (literal.ends_with("i32")) {
//      literal.remove_suffix(3);
//    } else if (!isCharNumeric(literal.back())) {
//      ret.diagnostics->push_back(makeErrorPlaceholder("Invalid i32 literal suffix."));   
//    }
//
//    auto [error_loc, error_code] =
//        std::from_chars(literal.data(), literal.data() + literal.size(), ret.node.data);
//
//    if (error_code == std::errc::invalid_argument) {
//      ret.diagnostics->push_back(makeErrorPlaceholder("Invalid i32 literal format."));        
//    } else if (error_code == std::errc::result_out_of_range) {
//      ret.diagnostics->push_back(makeErrorPlaceholder("i32 literal value out of range."));
//    }
//
//    return ret;
//  }
//};
//
//struct NodeI64 : BaseNode {
//  using ValueType = std::int64_t;
//  ValueType data{0};
//  static ProcessResult<NodeI64> fromAst(const TrContext& ctx, const Ast& ast) {
//    assert(ast.TypeIs(eAst::kLitI64) && "Expected kLitI64 ast type.");
//    ProcessResult<NodeI64> ret{};
//    const Token& tk = ctx.tokens[ast.index];
//    ret.node.source_location.file = tk.file;
//    ret.node.source_location.index = tk.offset;
//    ret.node.source_location.size = tk.size;
//
//    std::string_view literal = getTokenLiteral(ctx, tk);
//
//    if (literal.ends_with("i64")) {
//      literal.remove_suffix(3);
//    } else if (literal.ends_with("L")) {
//      literal.remove_suffix(1);
//    }
//    else if (!isCharNumeric(literal.back())) {
//      ret.diagnostics->push_back(makeErrorPlaceholder("Invalid i64 literal suffix."));
//    }
//
//    auto [error_loc, error_code] =
//        std::from_chars(literal.data(), literal.data() + literal.size(), ret.node.data);
//
//    if (error_code == std::errc::invalid_argument) {
//      ret.diagnostics->push_back(makeErrorPlaceholder("Invalid i64 literal format."));
//    } else if (error_code == std::errc::result_out_of_range) {
//      ret.diagnostics->push_back(makeErrorPlaceholder("i64 literal value out of range."));
//    }
//
//    return ret;
//  }
//};
//
//struct NodeU8 : BaseNode {
//  using ValueType = std::uint8_t;
//  ValueType data{0u};
//  static ProcessResult<NodeU8> fromAst(const TrContext& ctx, const Ast& ast) {
//    assert(ast.TypeIs(eAst::kLitU8) && "Expected kLitU8 ast type.");
//    ProcessResult<NodeU8> ret{};
//    const Token& tk = ctx.tokens[ast.index];
//    ret.node.source_location.file = tk.file;
//    ret.node.source_location.index = tk.offset;
//    ret.node.source_location.size = tk.size;
//
//    std::string_view literal = getTokenLiteral(ctx, tk);
//
//    if (literal.ends_with("u8")) {
//      literal.remove_suffix(3);
//    } else if (literal.ends_with("B")) {
//      literal.remove_suffix(1);
//    } else if (!isCharNumeric(literal.back())) {
//      ret.diagnostics->push_back(makeErrorPlaceholder("Invalid u8 literal suffix."));
//    }
//
//    auto [error_loc, error_code] =
//        std::from_chars(literal.data(), literal.data() + literal.size(), ret.node.data);
//
//    if (error_code == std::errc::invalid_argument) {
//      ret.diagnostics->push_back(makeErrorPlaceholder("Invalid u8 literal format."));
//    } else if (error_code == std::errc::result_out_of_range) {
//      ret.diagnostics->push_back(makeErrorPlaceholder("u8 literal value out of range."));
//    }
//
//    return ret;
//  }
//};
//
//struct NodeU16 : BaseNode {
//  using ValueType = std::uint16_t;
//  ValueType data{0u};
//  static ProcessResult<NodeU16> fromAst(const TrContext& ctx, const Ast& ast) {
//    assert(ast.TypeIs(eAst::kLitI64) && "Expected kLitI64 ast type.");
//    ProcessResult<NodeU16> ret{};
//    const Token& tk = ctx.tokens[ast.index];
//    ret.node.source_location.file = tk.file;
//    ret.node.source_location.index = tk.offset;
//    ret.node.source_location.size = tk.size;
//
//    std::string_view literal = getTokenLiteral(ctx, tk);
//
//    if (literal.ends_with("i64")) {
//      literal.remove_suffix(3);
//    } else if (literal.ends_with("L")) {
//      literal.remove_suffix(1);
//    } else if (!isCharNumeric(literal.back())) {
//      ret.diagnostics->push_back(makeErrorPlaceholder("Invalid i64 literal suffix."));
//    }
//
//    auto [error_loc, error_code] =
//        std::from_chars(literal.data(), literal.data() + literal.size(), ret.node.data);
//
//    if (error_code == std::errc::invalid_argument) {
//      ret.diagnostics->push_back(makeErrorPlaceholder("Invalid i64 literal format."));
//    } else if (error_code == std::errc::result_out_of_range) {
//      ret.diagnostics->push_back(makeErrorPlaceholder("i64 literal value out of range."));
//    }
//
//    return ret;
//  }
//};
//
//struct NodeU32 : BaseNode {
//  using ValueType = std::uint32_t;
//  ValueType data{0u};
//  static ProcessResult<NodeU32> fromAst(const TrContext& ctx, const Ast& ast) {
//    assert(ast.TypeIs(eAst::kLitI64) && "Expected kLitI64 ast type.");
//    ProcessResult<NodeI64> ret{};
//    const Token& tk = ctx.tokens[ast.index];
//    ret.node.source_location.file = tk.file;
//    ret.node.source_location.index = tk.offset;
//    ret.node.source_location.size = tk.size;
//
//    std::string_view literal = getTokenLiteral(ctx, tk);
//
//    if (literal.ends_with("i64")) {
//      literal.remove_suffix(3);
//    } else if (literal.ends_with("L")) {
//      literal.remove_suffix(1);
//    } else if (!isCharNumeric(literal.back())) {
//      ret.diagnostics->push_back(makeErrorPlaceholder("Invalid i64 literal suffix."));
//    }
//
//    auto [error_loc, error_code] =
//        std::from_chars(literal.data(), literal.data() + literal.size(), ret.node.data);
//
//    if (error_code == std::errc::invalid_argument) {
//      ret.diagnostics->push_back(makeErrorPlaceholder("Invalid i64 literal format."));
//    } else if (error_code == std::errc::result_out_of_range) {
//      ret.diagnostics->push_back(makeErrorPlaceholder("i64 literal value out of range."));
//    }
//
//    return ret;
//  }
//};
//
//struct NodeU64 : BaseNode {
//  using ValueType = std::uint64_t;
//  ValueType data{0u};
//  static ProcessResult<NodeU64> fromAst(const TrContext& ctx, const Ast& ast) {
//    assert(ast.TypeIs(eAst::kLitI64) && "Expected kLitI64 ast type.");
//    ProcessResult<NodeI64> ret{};
//    const Token& tk = ctx.tokens[ast.index];
//    ret.node.source_location.file = tk.file;
//    ret.node.source_location.index = tk.offset;
//    ret.node.source_location.size = tk.size;
//
//    std::string_view literal = getTokenLiteral(ctx, tk);
//
//    if (literal.ends_with("i64")) {
//      literal.remove_suffix(3);
//    } else if (literal.ends_with("L")) {
//      literal.remove_suffix(1);
//    } else if (!isCharNumeric(literal.back())) {
//      ret.diagnostics->push_back(makeErrorPlaceholder("Invalid i64 literal suffix."));
//    }
//
//    auto [error_loc, error_code] =
//        std::from_chars(literal.data(), literal.data() + literal.size(), ret.node.data);
//
//    if (error_code == std::errc::invalid_argument) {
//      ret.diagnostics->push_back(makeErrorPlaceholder("Invalid i64 literal format."));
//    } else if (error_code == std::errc::result_out_of_range) {
//      ret.diagnostics->push_back(makeErrorPlaceholder("i64 literal value out of range."));
//    }
//
//    return ret;
//  }
//};
//
//struct NodeF32 : BaseNode {
//  using ValueType = float;
//  ValueType data{0.f};
//  static ProcessResult<NodeF32> fromAst(const Ast& ast) {
//    assert(ast.TypeIs(eAst::kLitI64) && "Expected kLitI64 ast type.");
//    ProcessResult<NodeI64> ret{};
//    const Token& tk = ctx.tokens[ast.index];
//    ret.node.source_location.file = tk.file;
//    ret.node.source_location.index = tk.offset;
//    ret.node.source_location.size = tk.size;
//
//    std::string_view literal = getTokenLiteral(ctx, tk);
//
//    if (literal.ends_with("i64")) {
//      literal.remove_suffix(3);
//    } else if (literal.ends_with("L")) {
//      literal.remove_suffix(1);
//    } else if (!isCharNumeric(literal.back())) {
//      ret.diagnostics->push_back(makeErrorPlaceholder("Invalid i64 literal suffix."));
//    }
//
//    auto [error_loc, error_code] =
//        std::from_chars(literal.data(), literal.data() + literal.size(), ret.node.data);
//
//    if (error_code == std::errc::invalid_argument) {
//      ret.diagnostics->push_back(makeErrorPlaceholder("Invalid i64 literal format."));
//    } else if (error_code == std::errc::result_out_of_range) {
//      ret.diagnostics->push_back(makeErrorPlaceholder("i64 literal value out of range."));
//    }
//
//    return ret;
//  }
//};
//
//struct NodeF64 : BaseNode {
//  using ValueType = double;
//  ValueType data{0.0};
//  static ProcessResult<NodeF64> fromAst(const Ast& ast) {
//    assert(ast.TypeIs(eAst::kLitI64) && "Expected kLitI64 ast type.");
//    ProcessResult<NodeI64> ret{};
//    const Token& tk = ctx.tokens[ast.index];
//    ret.node.source_location.file = tk.file;
//    ret.node.source_location.index = tk.offset;
//    ret.node.source_location.size = tk.size;
//
//    std::string_view literal = getTokenLiteral(ctx, tk);
//
//    if (literal.ends_with("i64")) {
//      literal.remove_suffix(3);
//    } else if (literal.ends_with("L")) {
//      literal.remove_suffix(1);
//    } else if (!isCharNumeric(literal.back())) {
//      ret.diagnostics->push_back(makeErrorPlaceholder("Invalid i64 literal suffix."));
//    }
//
//    auto [error_loc, error_code] =
//        std::from_chars(literal.data(), literal.data() + literal.size(), ret.node.data);
//
//    if (error_code == std::errc::invalid_argument) {
//      ret.diagnostics->push_back(makeErrorPlaceholder("Invalid i64 literal format."));
//    } else if (error_code == std::errc::result_out_of_range) {
//      ret.diagnostics->push_back(makeErrorPlaceholder("i64 literal value out of range."));
//    }
//
//    return ret;
//  }
//};
//
//struct NodeString : BaseNode {
//  std::size_t id{};
//  static ProcessResult<NodeString> fromAst(const Ast& ast) { 
//    assert(ast.TypeIs(eAst::kLitCstr) && "Expected eAst::kLitCstr.");
//    ProcessResult<NodeString> ret{};
//
//
//
//    return ProcessResult<NodeString>(); 
//  }
//};
//
//struct NodeTypename : BaseNode {
//  std::size_t id{};
//  static ProcessResult<NodeTypename> fromAst(const Ast& ast) {
//    return ProcessResult<NodeTypename>();
//  }
//};
//
//struct NodeIdentifier : BaseNode {
//  std::size_t id{};
//  static ProcessResult<NodeIdentifier> fromAst(const Ast& ast) {
//    return ProcessResult<NodeIdentifier>();
//  }
//};
//
//struct NodeAssignment : BaseNodeBinaryOperator {};
//
//struct NodeLogicalOr : BaseNodeBinaryOperator {};
//
//struct NodeLogicalAnd : BaseNodeBinaryOperator {};
//
//struct NodeBitwiseOr : BaseNodeBinaryOperator {};
//
//struct NodeBitwiseXor : BaseNodeBinaryOperator {};
//
//struct NodeBitwiseAnd : BaseNodeBinaryOperator {};
//
//struct NodeEquality : BaseNodeBinaryOperator {};
//
//struct NodeInequality : BaseNodeBinaryOperator {};
//
//struct NodeGreater : BaseNodeBinaryOperator {};
//
//struct NodeLess : BaseNodeBinaryOperator {};
//
//struct NodeGreaterEqual : BaseNodeBinaryOperator {};
//
//struct NodeLessEqual : BaseNodeBinaryOperator {};
//
//struct NodeBitwiseShiftRight : BaseNodeBinaryOperator {};
//
//struct NodeBitwiseShiftLeft : BaseNodeBinaryOperator {};
//
//struct NodeAdd : BaseNodeBinaryOperator {};
//
//struct NodeSubtract : BaseNodeBinaryOperator {};
//
//struct NodeMultiply : BaseNodeBinaryOperator {};
//
//struct NodeDivide : BaseNodeBinaryOperator {};
//
//struct NodeModulus : BaseNodeBinaryOperator {};
//
//struct NodePreIncrement : BaseNodeUnaryOperator {};
//
//struct NodePreDecrement : BaseNodeUnaryOperator {};
//
//struct NodeNot : BaseNodeUnaryOperator {};
//
//struct NodeNegative : BaseNodeUnaryOperator {};
//
//struct NodePositive : BaseNodeUnaryOperator {};
//
//struct NodeAccess : BaseNodeUnaryOperator {};
//
//struct NodePostIncrement : BaseNodeUnaryOperator {};
//
//struct NodePostDecrement : BaseNodeUnaryOperator {};
//
//struct NodeResolution : BaseNodeBinaryOperator {};
//
//struct NodeFile : BaseNode {
//  std::size_t id{};
//  std::vector<UnionNodePragmaticStatement> statements{};
//  static std::pair<NodeFile, Diagnostics> fromAst(const Ast& ast) {
//    assert(ast.type == eAst::kProgram && "Expected eAst::kProgram node.");
//    for (const Ast& branch : ast.branches) {
//      switch (ast.type) {
//        case eAst::kVariableDeclaration: {
//          auto var_node = NodeVariableDefintion::fromAst(branch);
//        }
//        default:
//          assert(false && "Unexpected directive statement type.");
//      }
//    }
//  }
//};
//
//struct NodeInclude : BaseNode {
//  NodeString path{};
// 
//};
//
//struct NodeNamespace : BaseNode {
//  NodeIdentifier ident{};
//  std::vector<UnionNodePragmaticStatement> statements{};
//};
//
//struct NodeVariableModifiers : BaseNode {
//  static ProcessResult<NodeVariableModifiers> fromAst(const Ast& ast) {
//    return ProcessResult<NodeVariableModifiers>();
//  }
//};
//
//struct NodeVariableDefintion : BaseNode {
//  NodeVariableModifiers mods{};
//  NodeIdentifier ident{};
//  NodeTypename type{};
//  UnionNodeExpression init{};
//  static ProcessResult<NodeVariableDefintion> fromAst(const Ast& ast) {
//    ProcessResult<NodeVariableDefintion> result{};
//
//    // modifiers
//    auto mods_result = NodeVariableModifiers::fromAst(ast.branches.at(0));
//    if (!mods_result.diagnostics->empty()) {
//      result.diagnostics.append(mods_result.diagnostics);
//    }
//    result.node.mods = mods_result.node;
//
//    // type constraints
//    auto type_result = NodeTypename::fromAst(ast.branches.at(1));
//    if (!type_result.diagnostics->empty()) {
//      result.diagnostics.append(type_result.diagnostics);
//    }
//    result.node.type = type_result.node;
//
//    // identifier
//    auto id_node = NodeIdentifier::fromAst(ast.branches.at(2));
//    if (!id_node.diagnostics->empty()) {
//      result.diagnostics.append(id_node.diagnostics);
//    }
//    result.node.ident = id_node.node;
//
//    // definition
//    if (ast.branches.size() > 3) {
//      auto init_node = UnionNodeExpression::fromAst(ast.branches.at(3));
//      if (!init_node.diagnostics->empty()) {
//        result.diagnostics.append(init_node.diagnostics);
//      }
//      result.node.init = init_node.node;
//    }
//
//    return result;
//  }
//};
//
//struct NodeFunctionDeclaration : BaseNode {
//  NodeIdentifier ident{};
//  NodeTypename return_type{};
//  std::vector<NodeFunctionParameter> parameters{};
//};
//
//struct NodeFunctionDefinition : BaseNode {
//  std::vector<UnionNodeFunctionalStatement> body{};
//};
//
//struct NodeFunctionParameter : BaseNode {
//  NodeIdentifier ident{};
//  NodeTypename type{};
//  UnionNodeExpression default_value{};
//};
//
//struct NodeMember : BaseNode {
//  NodeIdentifier ident{};
//  NodeTypename type{};
//};
//
//struct NodeStructDeclaration : BaseNode {
//  NodeIdentifier ident{};
//};
//
//struct NodeStructDefinition : BaseNode {
//  std::vector<NodeMember> members{};
//};
//
//struct NodeIfStatement : BaseNode {
//  std::vector<UnionNodeExpression> conditions{};
//  std::vector<std::vector<UnionNodeFunctionalStatement>> blocks{};
//  std::vector<UnionNodeFunctionalStatement> else_block{};
//};
//
//struct NodeReturnStatement : BaseNode {
//  UnionNodeExpression return_value{};
//};
//
//struct NodeWhileStatement {
//  UnionNodeExpression condition{};
//  std::vector<std::vector<UnionNodeFunctionalStatement>> block{};
//};
//
//}  // namespace ssgc::frontend