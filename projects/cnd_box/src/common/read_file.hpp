///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup
/// @brief
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup cnd_compiler
/// @{
#pragma once
#include <expected>
#include <filesystem>
#include <fstream>
#include <vector>

#include "diagnostic.hpp"

namespace ssgc::util {

static std::expected<std::string, Diagnostic> readFile(
    const std::filesystem::path& file_path) {
  if (!std::filesystem::exists(file_path)) {
    return std::unexpected(
        makeErrorPlaceholder(std::format("Does not exist : {}", file_path.string())));
  }

  if (!std::filesystem::is_regular_file(file_path)) {
    return std::unexpected(makeErrorPlaceholder(
        std::format("Not a regular file: {}", file_path.string())));
  }

  std::ifstream source_file_stream(file_path);
  if (!source_file_stream.is_open()) {
    return std::unexpected(makeErrorPlaceholder(
        std::format("Could not open file: {}", file_path.string())));
  }

  std::string temp_file_buffer = std::string((std::istreambuf_iterator<char>(source_file_stream)),
                                             std::istreambuf_iterator<char>());
  source_file_stream.close();

  // Add \0 if not already at end.
  if (temp_file_buffer.back() != '\0') temp_file_buffer.push_back('\0');
  return temp_file_buffer;
}
}  // namespace ssgc::util

/// @} // end of cnd_compiler

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
//
// Licensed under the GNU Affero General Public License, Version 3.
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////