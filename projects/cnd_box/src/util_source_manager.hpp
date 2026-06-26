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

#include "util_read_file.hpp"
#include "util_string_interner.hpp"
namespace ssgc::util {

using SourceFileId = std::size_t;

struct SourceFile {
  SourceFileId id{};
  StringId path{};
  std::string data{};
  std::vector<std::size_t> line_offsets{};

  SourceFile() = default;

  SourceFile(SourceFileId id, StringId file_path, const std::string& data)
      : id(id), path(file_path), data(data), line_offsets() {
    this->recalculateLineOffsets();
  };

  SourceFile(SourceFileId id, StringId file_path, std::string&& data)
      : id(id), path(file_path), data(std::forward<std::string>(data)), line_offsets() {
    this->recalculateLineOffsets();
  };

  void recalculateLineOffsets() {
    if (!this->line_offsets.empty()) {
      this->line_offsets.clear();
    }
    this->line_offsets.push_back(0);
    for (std::size_t i = 0; i < this->data.size(); ++i) {
      if (this->data[i] == '\n') this->line_offsets.push_back(i + 1);
    }
  }

  std::string_view slice(std::size_t begin, std::size_t end) const noexcept;
  std::string_view view() const noexcept;
  std::pair<std::size_t, std::size_t> linecol(std::size_t offset) const noexcept;
};

class SourceManager {
 public:
  std::expected<const SourceFile*, diagnostic::Diagnostic> load(std::string_view file_path,
                                                                bool overwrite = true) noexcept;
  bool unload(SourceFileId file_id) noexcept;
  const SourceFile* get(SourceFileId file_id) const noexcept;
  bool contains(std::string_view file_path) const noexcept;
  std::string_view pathof(SourceFileId file_id) const noexcept;
  std::string_view pathof(const SourceFile* source_file) const noexcept;
 private:
  StringInterner intern_{};
  std::vector<std::unique_ptr<SourceFile>> files_{};
  std::unordered_map<StringId, SourceFileId> lookup_{};
  std::vector<std::size_t> free_slots_{};
};

bool SourceManager::contains(std::string_view file_path) const noexcept {
  std::filesystem::path abs_path = std::filesystem::absolute(file_path);

  // If the file path is not registered in the interner then file was never loaded.
  StringId found_id = this->intern_.find(file_path);
  if (found_id == StringInterner::kInvalidId) {
    return false;
  }

  // Find source file in the lookup map. If not found then it was unloaded.
  auto found_source = this->lookup_.find(found_id);
  if (found_source == this->lookup_.end()) {
    return false;
  }

  return true;
}

std::expected<const SourceFile*, diagnostic::Diagnostic> SourceManager::load(
    std::string_view file_path, bool overwrite) noexcept {
  // Store file by absolute path.
  std::filesystem::path abs_path = std::filesystem::absolute(file_path);
  StringId path_id = this->intern_.push(abs_path.string());

  // Check if file was already loaded.
  auto found = this->lookup_.find(path_id);
  if (found != this->lookup_.end()) {
    // Reload and update entry if overwrite is enabled.
    if (overwrite) {
      std::expected<std::string, diagnostic::Diagnostic> read_file_result = readFile(abs_path);
      if (!read_file_result) {
        return std::unexpected(read_file_result.error());
      }
      this->files_[found->second]->data = std::move(*read_file_result);
      this->files_[found->second]->recalculateLineOffsets();
    }
    return this->files_[found->second].get();
  }

  // Load file data.
  std::expected<std::string, diagnostic::Diagnostic> read_file_result = readFile(abs_path);
  if (!read_file_result) {
    return std::unexpected(read_file_result.error());
  }

  // Attempt to recycle a free slot.
  if (!free_slots_.empty()) {
    std::size_t recycled_idx = free_slots_.back();
    this->free_slots_.pop_back();
    this->files_[recycled_idx]->path = path_id;
    this->files_[recycled_idx]->data = std::move(*read_file_result);
    this->files_[recycled_idx]->recalculateLineOffsets();
    this->lookup_.emplace(path_id, recycled_idx);
    return this->files_[recycled_idx].get();
  }
  // Else create a new entry.
  else {
    this->files_.push_back(
        std::make_unique<SourceFile>(this->files_.size(), path_id, *read_file_result));
    this->lookup_.emplace(path_id, this->files_.size() - 1);
    return this->files_.back().get();
  }
};

const SourceFile* SourceManager::get(SourceFileId file_id) const noexcept {
  if (file_id >= this->files_.size()) {
    return nullptr;
  }
  return this->files_[file_id].get();
}

bool SourceManager::unload(SourceFileId file_id) noexcept {
  // Out of bounds id.
  if (file_id >= this->files_.size()) {
    return false;
  }

  // Already freed.
  if (std::find(this->free_slots_.begin(), this->free_slots_.end(), file_id) !=
      this->free_slots_.end()) {
    return false;
  }

  // Clear file data and push to free slots.
  this->files_[file_id]->data.clear();
  this->files_[file_id]->line_offsets.clear();
  this->free_slots_.push_back(file_id);
  this->lookup_.erase(this->files_[file_id]->path);
  return true;
}

std::string_view SourceManager::pathof(SourceFileId file_id) const noexcept { 
  const SourceFile* f = this->get(file_id);
  if (!f) {
    return "";
  }
  const std::string* fp = this->intern_.get(f->path);
  if (!fp) {
    return "";
  }
  return *fp;
};

std::string_view SourceManager::pathof(const SourceFile* source_file) const noexcept {
  if (!source_file) {
    return "";
  }
  return this->pathof(source_file->id);
};

std::string_view SourceFile::slice(std::size_t begin, std::size_t end) const noexcept {
  assert(begin <= end && "Begin offset must be less than or equal to end offset.");
  assert(end <= data.size() && "End offset must be within the bounds of the source data.");
  assert(begin <= data.size() && "Begin offset must be within the bounds of the source data.");
  return std::string_view{data.data() + begin, end - begin};
}

std::string_view SourceFile::view() const noexcept {
  return std::string_view{data.data(), data.size()};
}

std::pair<std::size_t, std::size_t> SourceFile::linecol(std::size_t offset) const noexcept {
  auto it = std::upper_bound(line_offsets.begin(), line_offsets.end(), offset);
  std::size_t line = std::distance(line_offsets.begin(), it);
  std::size_t line_start = line_offsets[line - 1];
  std::size_t column = offset - line_start + 1;
  return std::make_pair(line, column);
}

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