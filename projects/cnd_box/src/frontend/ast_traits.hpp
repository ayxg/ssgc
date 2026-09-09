#pragma once
#include "ast.hpp"
#include "enum_ast.hpp"

namespace ssgc {
constexpr bool isAstLiteralSignificant(frontend::eAst kind) noexcept {
  switch (kind) {
    using enum frontend::eAst;
    // Literal terminals
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
    // Intermediates which store a literal value
    case kEnumEntry:
      return true;
    default:
      return false;
  }
}
}  // namespace ssgc