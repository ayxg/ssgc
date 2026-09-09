///////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
///////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language Environment
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup cand_compiler_data
/// @brief
///////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup cand_compiler_data
/// @{
#pragma once
#include <cstdlib>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "read_file.hpp"
#include "string_interner.hpp"
namespace ssgc {

using SourceFileId = std::size_t;

struct SourceFile {
  std::size_t id{};
  std::size_t path{};
  std::string data{};
  std::vector<std::size_t> line_offsets{};

  SourceFile();

  SourceFile(std::size_t id, std::size_t file_path, const std::string& data);

  SourceFile(std::size_t id, std::size_t file_path, std::string&& data);

  void recalculateLineOffsets();

  std::string_view slice(std::size_t begin, std::size_t end) const noexcept;
  std::string_view view() const noexcept;
  std::pair<std::size_t, std::size_t> linecol(std::size_t offset) const noexcept;
};

class SourceManager {
 public:
  std::expected<const SourceFile*, Diagnostic> load(std::string_view file_path,
                                                                bool overwrite = true) noexcept;
  std::expected<const SourceFile*, Diagnostic> generate(std::string_view file_path,std::string_view data,
                                                    bool overwrite = true) noexcept;


  bool unload(std::size_t file_id) noexcept;
  const SourceFile* get(std::size_t file_id) const noexcept;
  bool contains(std::string_view file_path) const noexcept;
  std::string_view pathof(std::size_t file_id) const noexcept;
  std::string_view pathof(const SourceFile* source_file) const noexcept;

 private:
  StringInterner intern_{};
  std::vector<std::unique_ptr<SourceFile>> files_{};
  std::unordered_map<std::size_t, std::size_t> lookup_{};
  std::vector<std::size_t> free_slots_{};
};

}  // namespace ssgc::util

/// @} // end of cand_compiler_data

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