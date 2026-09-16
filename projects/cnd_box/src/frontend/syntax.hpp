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

}  // namespace ssgc

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
struct NodeIncrement;
struct NodeDecrement;
struct NodeNot;
struct NodeNegative;
struct NodePositive;
struct NodeAccess;
struct NodeResolution;

struct NodeIncludeStatement;
struct NodeMethod;
struct NodeMethodParameter;
struct NodeClass;

struct NodeFunctionalStatement;
struct NodePrimaryExpr;
struct NodeFile;
struct NodeSyntax;

using NodeUnionOperand =
    std::variant<NodeBool, NodeI8, NodeI16, NodeI32, NodeI64, NodeU8, NodeU16, NodeU32, NodeU64, NodeF32, NodeF64,
                 NodeIdentifier, NodeCStr, NodeNone, NodeParenSubexpr, NodeBracketSubexpr, NodeBraceSubexpr>;
using NodeUnionOperator =
    std::variant<NodeListFold, NodeAssignment, NodeLogicalOr, NodeLogicalAnd, NodeBitwiseOr, NodeBitwiseXor,
                 NodeBitwiseAnd, NodeEquality, NodeInequality, NodeGreater, NodeLess, NodeGreaterEqual, NodeLessEqual,
                 NodeBitwiseShiftRight, NodeBitwiseShiftLeft, NodeAdd, NodeSubtract, NodeMultiply, NodeDivide,
                 NodeModulus, NodeIncrement, NodeDecrement, NodeNot, NodeNegative, NodePositive, NodeAccess,
                 NodeResolution>;
using NodeUnionPrimary =
    std::variant<NodePoison, NodeBool, NodeI8, NodeI16, NodeI32, NodeI64, NodeU8, NodeU16, NodeU32, NodeU64, NodeF32,
                 NodeF64, NodeIdentifier, NodeCStr, NodeNone, NodeTypenameVoid, NodeTypenameAny,
                 NodeGlobalNamespaceIdentifier, NodeParenSubexpr, NodeBracketSubexpr, NodeBraceSubexpr, NodeListFold,
                 NodeAssignment, NodeLogicalOr, NodeLogicalAnd, NodeBitwiseOr, NodeBitwiseXor, NodeBitwiseAnd,
                 NodeEquality, NodeInequality, NodeGreater, NodeLess, NodeGreaterEqual, NodeLessEqual,
                 NodeBitwiseShiftRight, NodeBitwiseShiftLeft, NodeAdd, NodeSubtract, NodeMultiply, NodeDivide,
                 NodeModulus, NodeIncrement, NodeDecrement, NodeNot, NodeNegative, NodePositive, NodeAccess,
                 NodeResolution>;

using NodeUnionDirectiveStatement = std::variant<NodeMethod, NodeUnionPrimary>;

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
struct NodeNot : NodeBaseUnaryOperator {};
struct NodeNegative : NodeBaseUnaryOperator {};
struct NodePositive : NodeBaseUnaryOperator {};
struct NodeAccess : NodeBaseBinaryOperator {};
struct NodeIncrement : NodeBaseUnaryOperator {};
struct NodeDecrement : NodeBaseUnaryOperator {};
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

struct NodeModifiers : NodeBase {};

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

struct NodeVariableDeclaration : NodeBase {
  NodeModifiers mods{};
  NodePrimaryExpr type{};
  NodeIdentifier name{};
  NodePrimaryExpr init{};
};

struct NodeClass : NodeBase {
  NodeModifiers mods{};
  NodeIdentifier name{};
  std::vector<NodeMethod> methods{};
  std::vector<NodeVariableDeclaration> members{};
};

struct NodeFile : NodeBase {
  SourceFileId id{};
  std::vector<NodeUnionDirectiveStatement> statements{};
};

struct NodeIncludeStatement : NodeBase {
  StringId path{};
  bool is_local{};
};

struct NodeSyntax {
  std::list<NodeFile> files{};
};

}  // namespace ssgc::frontend