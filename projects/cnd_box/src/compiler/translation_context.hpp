///////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
///////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language Environment
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup ssgc_util
/// @brief
///////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup ssgc_util
/// @{
#pragma once
#include <cstdlib>
#include <functional>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "../frontend/token.hpp"
#include "../cli/parser.hpp"
#include "../common/diagnostic.hpp"
#include "../common/logger.hpp"
#include "../common/source_manager.hpp"
#include "../common/string_interner.hpp"
#include "../frontend/ast.hpp"

#include "../frontend/syntax.hpp"
namespace ssgc {

struct TrContext {
  StringInterner string_table{};
  StringInterner id_table{};
  SourceManager sources{};
  util::Logger log{};
  cli::CliFlagMap args{};
  Diagnostics diagnostics{};
  std::unordered_map<std::size_t, std::vector<frontend::Token>> tokens{};
  std::unordered_map < std::size_t, frontend::Ast> syntax_trees{};
  std::unordered_map<std::size_t, frontend::NodeFile> syntax_nodes{};


};

}  // namespace ssgc

/// @} // end of ssgc_util

///////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////////////////////////////////////