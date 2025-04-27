///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language Environment
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup cand_compiler_data
/// @brief C& source token structure.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup cand_compiler_data
/// @{
#pragma once
// clang-format off
#include "use_ccapi.hpp"
#include "use_corevals.hpp"
#include "cldata/tk.hpp"
#include "cldata/ast.hpp"
#include "trtools/token_cursor.hpp"
// clang-format on

namespace cnd {

/// @brief C& source token structure.
struct Sast {
  eAst type{};
  const Sast* parent{nullptr};
  Vec<Sast> children{};
  Str literal{};

  constexpr Sast() = default;
  constexpr Sast(Sast&& o) = default;
  constexpr Sast(const Sast& o) = default;
  constexpr Sast& operator=(Sast&& o) = default;
  constexpr Sast& operator=(const Sast& o) = default;

  constexpr Sast(eAst type) : type(type) {}
  constexpr bool TypeIsnt(eAst ast_type) const noexcept { return type != ast_type; }
  constexpr bool TypeIs(eAst ast_type) const noexcept { return type == ast_type; }
  constexpr static bool CompareAst(const Sast& node1, const Sast& node2) {
    // Compare node types
    if (node1.type != node2.type) {
      return false;
    }

    // Compare node values
    if (!(node1.literal == node2.literal)) return false;

    // Compare number of Children
    if (node1.children.size() != node2.children.size()) {
      return false;
    }

    // Recursively compare Children
    for (size_t i = 0; i < node1.children.size(); ++i) {
      if (!CompareAst(node1.children.at(i), node2.children.at(i))) return false;
    }

    // If all checks pass, the ASTs are equal
    return true;
  }
  constexpr static bool CompareAst(const Ast& node1, const Sast& node2) {
    // Compare node types
    if (node1.type != node2.type) {
      return false;
    }

    // Compare node values
    if (!(node1.GetLiteral() == node2.literal)) return false;

    // Compare number of Children
    if (node1.children.size() != node2.children.size()) {
      return false;
    }

    // Recursively compare Children
    for (size_t i = 0; i < node1.children.size(); ++i) {
      if (!CompareAst(node1.children.at(i), node2.children.at(i))) return false;
    }

    // If all checks pass, the ASTs are equal
    return true;
  }
  static Str Format(const Sast& ast, std::size_t depth = 0) {
    Str ret =  std::format("{}[{},{}]\n", Str{"    ", depth}, eAstToCStr(ast.type), ast.literal);
    depth++;
    for (const auto& node : ast.children) {
      ret += Format(node, depth);
    }
    return ret;
  }
  Str Format() const { return Sast::Format(*this); }

  constexpr bool operator==(const Sast& other) const noexcept { return CompareAst(*this, other); }
  constexpr bool operator==(const Ast& other) const noexcept { return CompareAst(other, *this); }

//  constexpr Sast(eAst type) : type{type} {}
  constexpr Sast(eAst type, StrView lit) : type(type), literal(lit) {}

  constexpr Sast(eAst type, StrView lit, const Sast* parent, const Vec<Sast>& children)
      : type{type}, literal(lit), parent(parent), children(children) {}
  constexpr Sast(eAst type, StrView lit, const Sast* parent, Vec<Sast>&& children)
      : type{type}, literal(lit), parent(parent), children(std::forward<Vec<Sast>>(children)) {}
  constexpr Sast(eAst type, StrView lit, Vec<Sast>&& children)
      : type{type}, literal(lit), children(std::forward<Vec<Sast>>(children)) {
    for (auto& child : children) {
      child.parent = this;
    }
  }
  //template <typename... ChildTs>
  //  requires(std::is_same_v<Sast, std::decay_t<ChildTs>> && ...)
  //Sast(eAst type, StrView lit, ChildTs... children) : type(type), literal(lit) {
  //  (PushBack(children), ...);
  //}

  Sast(eAst type, std::same_as<std::decay_t<Sast>> auto... children) : type(type) {
    (PushBack(children), ...);
  }
  Sast(eAst type, StrView lit, std::same_as<std::decay_t<Sast>> auto... children) : type(type), literal(lit) {
    (PushBack(children), ...);
  }
  constexpr bool IsLiteral() const noexcept {
    switch (type) {
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
        return true;
      default:
        return false;
    }
  }

  constexpr void PushBack(Sast&& ast) { children.push_back(ast); }
  constexpr void PushBack(const Sast& ast) { children.push_back(ast); }

  constexpr void Append(std::same_as<Sast> auto&&... ast) {
    children.append_range(Vec<Sast>{std::forward<Sast>(ast)...});
  }
};
}  // namespace cnd

/// @} // end of cand_compiler_data

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