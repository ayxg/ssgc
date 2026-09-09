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
struct Ast {
  eAst type{};
  const Ast* root{nullptr};
  std::vector<Ast> branches{};
  std::size_t begin_source_token{0};
  std::size_t end_source_token{0};

  constexpr Ast() = default;
  constexpr Ast(Ast&& o) = default;
  constexpr Ast(const Ast& o) = default;
  constexpr Ast& operator=(Ast&& o) = default;
  constexpr Ast& operator=(const Ast& o) = default;

  constexpr bool TypeIsnt(eAst ast_type) const noexcept { return type != ast_type; }
  constexpr bool TypeIs(eAst ast_type) const noexcept { return type == ast_type; }
  constexpr std::size_t sourceEnd() const noexcept { return end_source_token; }
  constexpr std::size_t sourceBegin() const noexcept { return begin_source_token; }
  constexpr void setSourceEnd(std::size_t end_token) noexcept { end_source_token = end_token;
  }
  constexpr void setSourceBegin(std::size_t begin_token) noexcept { begin_source_token = begin_token;
  }
  template <typename... ChildTs>
    requires(std::is_same_v<Ast, std::decay_t<ChildTs>> && ...)
  Ast(eAst type, std::size_t src_beg, std::size_t src_end, ChildTs... children)
      : begin_source_token{src_beg},
        end_source_token{src_end},
        type(type),
        branches({children...}) {
    //(branches.push_back(children), ...);
  }
  constexpr Ast(const TokenCursor& c)
      : type(getAstFromToken(c.kind())),
        begin_source_token(c.index()),
        end_source_token(c.index() + 1),
        root(nullptr),
        branches({}) {}

  static std::string Format(const SourceFile* src, const std::vector<Token>* tokens, const Ast& ast,
                            std::size_t depth = 0) {
    std::string_view lit{};
    if (ast.begin_source_token == ast.end_source_token) {
      lit = "";
    }else {
      const Token* end_token = &tokens->at(ast.sourceEnd() - 1);

      while (ssgc::frontend::isTokenInsignificant(end_token->kind)) {
        end_token--;
      }
      lit = src->slice(tokens->at(ast.begin_source_token).source_range.begin,
                     end_token->source_range.end);
    }
    std::string ret =
        std::format("{}[{},{}]\n", std::string(depth * 4, ' '), eAstToCStr(ast.type), lit);
    depth++;
    for (const auto& node : ast.branches) {
      ret += Ast::Format(src, tokens,node, depth);
    }
    return ret;
  }

  std::string Format(const SourceFile* src, const std::vector<Token>* tokens) const {
    return Ast::Format(src,tokens, *this);
  }

  constexpr Ast(eAst type) : type{type} {}
  constexpr Ast(eAst type, std::size_t src_beg, std::size_t src_end)
      : type{type},
        begin_source_token{src_beg},
        end_source_token{src_end},
        root(nullptr),
        branches({}) {}
  constexpr Ast(eAst type, std::size_t src_beg, std::size_t src_end, const Ast* parent,
                const std::vector<Ast>& branches)
      : type{type},
        begin_source_token{src_beg},
        end_source_token{src_end},
        root(parent),
        branches(branches) {}
  constexpr Ast(eAst type, std::size_t src_beg, std::size_t src_end,
                const std::vector<Ast>& branches)
      : type{type},
        begin_source_token{src_beg},
        end_source_token{src_end},
        root(nullptr),
        branches(branches) {}


  constexpr const Ast& operator[](std::size_t idx) const noexcept { 
    return branches.at(idx);
  }

  const std::size_t size() const noexcept { return branches.size();
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