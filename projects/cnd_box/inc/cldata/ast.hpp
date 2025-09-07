///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the Apache License, Version 2.0(the "License");
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
#include "trtools/TkCursor.hpp"
// clang-format on

namespace cnd {

/// @brief C& source token structure.
struct Ast {
  eAst type{};
  const Ast* parent{nullptr};
  Vec<Ast> children{};

  std::span<const Tk>::const_iterator src_begin{};
  std::span<const Tk>::const_iterator src_end{};

  constexpr Ast() = default;
  constexpr Ast(Ast&& o) = default;
  constexpr Ast(const Ast& o) = default;
  constexpr Ast& operator=(Ast&& o) = default;
  constexpr Ast& operator=(const Ast& o) = default;

  constexpr Ast(eAst type) : type(type) {}
  constexpr bool TypeIsnt(eAst ast_type) const noexcept { return type != ast_type; }
  constexpr bool TypeIs(eAst ast_type) const noexcept { return type == ast_type; }

  constexpr static bool CompareAst(const Ast& node1, const Ast& node2) {
    // Compare node types
    if (node1.type != node2.type) {
      return false;
    }

    // Compare node values
    if (!std::equal(node1.src_begin, node1.src_end, node2.src_begin, node2.src_end,
                    [](const Tk& a, const Tk& b) { return a.literal_ == b.literal_; }))
      return false;

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
  static Str Format(const Ast& ast, std::size_t depth = 0) {
    Str ret = std::format("{}[{},{}]\n", Str{"    ", depth}, eAstToCStr(ast.type), ast.GetLiteral());
    depth++;
    for (const auto& node : ast.children) {
      ret += Format(node, depth);
    }
    return ret;
  }
  Str Format() const { return Ast::Format(*this); }

  constexpr bool operator==(const Ast& other) const noexcept { return CompareAst(*this, other); }

  constexpr Ast(const TkCursor<std::span>& c)
      : type(GetAstFromTk(c.Get().Type())), src_begin(c.Iter()), src_end{c.Next().Iter()} {}
  constexpr Ast(const std::span<const Tk>::const_iterator& c)
      : type(GetAstFromTk(c->Type())), src_begin(c), src_end{c + 1} {}

  constexpr Ast(eTk operand_token, std::span<const Tk>::const_iterator src_beg,
                std::span<const Tk>::const_iterator src_end)
      : type{GetAstFromTk(operand_token)}, src_begin{src_beg}, src_end{src_end} {}

  constexpr Ast(eAst type, std::span<const Tk>::const_iterator src_beg, std::span<const Tk>::const_iterator src_end)
      : type{type}, src_begin{src_beg}, src_end{src_end} {}

  constexpr Ast(eAst type, std::span<const Tk>::const_iterator src_beg, std::span<const Tk>::const_iterator src_end,
                const Ast* parent, const Vec<Ast>& children)
      : type{type}, src_begin{src_beg}, src_end{src_end}, parent(parent), children(children) {}
  constexpr Ast(eAst type, std::span<const Tk>::const_iterator src_beg, std::span<const Tk>::const_iterator src_end,
                const Ast* parent, Vec<Ast>&& children)
      : type{type}, src_begin{src_beg}, src_end{src_end}, parent(parent), children(std::forward<Vec<Ast>>(children)) {}
  constexpr Ast(eAst type, std::span<const Tk>::const_iterator src_beg, std::span<const Tk>::const_iterator src_end,
                Vec<Ast>&& children)
      : type{type}, src_begin{src_beg}, src_end{src_end}, children(std::forward<Vec<Ast>>(children)) {
    for (auto& child : children) {
      child.parent = this;
    }
  }
  template <typename... ChildTs>
    requires(std::is_same_v<Ast, std::decay_t<ChildTs>> && ...)
  Ast(eAst type, std::span<const Tk>::const_iterator beg, std::span<const Tk>::const_iterator end, ChildTs... children)
      : src_begin{beg}, src_end{end}, type(type) {
    (PushBack(children), ...);
  }

  template <typename... ChildTs>
    requires(std::is_same_v<Ast, std::decay_t<ChildTs>> && ...)
  Ast(eAst type, ChildTs... children) : type(type) {
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
  constexpr bool IsLiteralSignificant() const noexcept {
    switch (type) {
      using enum eAst;
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
  constexpr Str GetLiteral() const noexcept {
    if (src_begin != src_end) {
      Str ret;
      for (const Tk& tk : std::ranges::subrange{src_begin, src_end}) {
        ret += tk.literal_;
      }
      return ret;
    }
    return "";
  }

  constexpr void PushBack(Ast&& ast) { children.push_back(ast); }
  constexpr void PushBack(const Ast& ast) { children.push_back(ast); }
  constexpr void PushFront(Ast&& ast) { children.insert(children.begin(), ast); }
  constexpr void PushFront(const Ast& ast) { children.insert(children.begin(), ast); }
  constexpr Ast& At(std::size_t i) { return children[i]; }
  constexpr const Ast& At(std::size_t i) const { return children[i]; }

  constexpr void Append(std::same_as<Ast> auto&&... ast) { children.append_range(Vec<Ast>{std::forward<Ast>(ast)...}); }
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
// Licensed under the Apache License, Version 2.0(the "License"); you may not use this file except in compliance with
// the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on
// an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////