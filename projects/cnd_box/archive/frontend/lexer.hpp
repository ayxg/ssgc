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
// clang-format off
#include <string_view>
#include <expected>
#include <vector>

#include "ccapi/CommonCppApi.hpp"
//#include "compiler_utils/CompilerProcessResult.hpp"
#include "grammar/eTk.hpp"
#include "frontend/tk.hpp"
#include "diagnostic/error_messages.hpp"
// clang-format on

/// Set true to enable inline static unit tests during compiler development.
// #define CND_ENABLE_STATIC_TEST_MODULE_Lexer true

namespace cnd::frontend {
using cldev::clmsg::ClMsgBuffer;  //> Result of an error in an intermediate or final lex steps.

std::expected<std::vector<Tk>, ClMsgBuffer> tokenizeSourceCode(
    std::string_view source_code) noexcept;
std::vector<Tk> sanitizeTokens(const std::vector<Tk>& output_tokens) noexcept;

class Lexer {
  Lexer() noexcept;
  std::expected<std::vector<Tk>, ClMsgBuffer> tokenize(std::string_view source_code) noexcept;
  static std::vector<Tk> sanitize(const std::vector<Tk>& output_tokens) noexcept;

 private:
  std::unique_ptr<void, void (*)(void*)> pimpl_;
};

}  // namespace cnd::frontend

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