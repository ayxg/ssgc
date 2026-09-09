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
#include <vector>

#include "enum_ast.hpp"
#include "token.hpp"
#include "token_cursor.hpp"
#include "token_traits.hpp"

namespace ssgc::frontend {

/// @brief C& source token structure.
struct SynthesizedAst {
  eAst type{};
  const SynthesizedAst* root{nullptr};
  std::vector<SynthesizedAst> branches{};
  std::string literal{};

  constexpr SynthesizedAst() = default;
  constexpr SynthesizedAst(SynthesizedAst&& o) = default;
  constexpr SynthesizedAst(const SynthesizedAst& o) = default;
  constexpr SynthesizedAst& operator=(SynthesizedAst&& o) = default;
  constexpr SynthesizedAst& operator=(const SynthesizedAst& o) = default;

  constexpr bool TypeIsnt(eAst ast_type) const noexcept { return type != ast_type; }
  constexpr bool TypeIs(eAst ast_type) const noexcept { return type == ast_type; }
  // constexpr static bool CompareAst(const SynthesizedAst& node1, const SynthesizedAst& node2) {
  //   // Compare node types
  //   if (node1.type != node2.type) {
  //     return false;
  //   }

  //  // Compare node values
  //  if (!std::equal(node1.src_begin, node1.src_end, node2.src_begin, node2.src_end,
  //                  [](const Token& a, const Token& b) { return a.literal_ == b.literal_; }))
  //    return false;

  //  // Compare number of Children
  //  if (node1.children.size() != node2.children.size()) {
  //    return false;
  //  }

  //  // Recursively compare Children
  //  for (size_t i = 0; i < node1.children.size(); ++i) {
  //    if (!CompareAst(node1.children.at(i), node2.children.at(i))) return false;
  //  }

  //  // If all checks pass, the ASTs are equal
  //  return true;
  //}
   static std::string Format(const SynthesizedAst& ast, std::size_t depth = 0) {
    std::string ret =
        std::format("{}[{},{}]\n", std::string(depth * 4, ' '), eAstToCStr(ast.type),
    ast.literal); depth++; for (const auto& node : ast.branches) {
      ret += Format(node, depth);
    }
    return ret;
  }
   std::string Format() const { return SynthesizedAst::Format(*this); }

  // constexpr bool operator==(const SynthesizedAst& other) const noexcept { return CompareAst(*this, other); }

  // constexpr SynthesizedAst(const std::span<const Token>::iterator& c)
  //     : type(GetAstFromTk(c->Type())), src_begin(c), src_end{c + 1} {}

  // constexpr SynthesizedAst(eTk operand_token, std::span<const Token>::iterator src_beg,
  //               std::span<const Token>::iterator src_end)
  //     : type{GetAstFromTk(operand_token)}, src_begin{src_beg}, src_end{src_end} {}

  constexpr SynthesizedAst(eAst type) : type{type} {}
  constexpr SynthesizedAst(eAst type,const std::string & literal)
      : type{type}, literal{literal}, root(nullptr), branches({}) {}

  template <typename... ChildTs>
    requires(std::is_same_v<SynthesizedAst, std::decay_t<ChildTs>> && ...)
  SynthesizedAst(eAst type, std::string literal, ChildTs... children)
      : literal{literal}, type(type), branches({children...}) {
    //(branches.push_back(children), ...);
  }

};
}  // namespace ssgc::frontend

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