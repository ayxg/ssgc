///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file C& Lexer
/// @ingroup
/// @brief
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup cnd_compiler_data
/// @{
#pragma once

#include <expected>
#include <string_view>
#include <vector>

// #include "compiler_utils/CompilerProcessResult.hpp"
#include "diagnostics.hpp"
#include "frontend_enum_token.hpp"
#include "frontend_token.hpp"

/// Set true to enable inline static unit tests during compiler development.
// #define CND_ENABLE_STATIC_TEST_MODULE_Lexer true

namespace ssgc::frontend {

std::expected<std::vector<Token>, diagnostic::Diagnostics> tokenizeSourceCode(
    std::string_view source_code) noexcept;
std::vector<Token> sanitizeTokens(const std::vector<Token>& output_tokens) noexcept;

class Lexer {
 public:
  Lexer() noexcept;
  std::expected<std::vector<Token>, diagnostic::Diagnostics> tokenize(
      std::string_view source_code) noexcept;
  static std::vector<Token> sanitize(const std::vector<Token>& output_tokens) noexcept;

 private:
  std::unique_ptr<void, void (*)(void*)> pimpl_;
};

}  // namespace ssgc::frontend

/// @} // end of cnd_compiler_data

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