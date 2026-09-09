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
#include "source_manager.hpp"

#include <cassert>
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

SourceFile::SourceFile() = default;

SourceFile::SourceFile(std::size_t id, std::size_t file_path, const std::string& data)
    : id(id), path(file_path), data(data), line_offsets() {
  recalculateLineOffsets();
};

SourceFile::SourceFile(std::size_t id, std::size_t file_path, std::string&& data)
    : id(id), path(file_path), data(std::forward<std::string>(data)), line_offsets() {
  recalculateLineOffsets();
};

void SourceFile::recalculateLineOffsets() {
  line_offsets.clear();

  line_offsets.push_back(0);
  for (std::size_t i = 0; i < data.size(); ++i) {
    if (data[i] == '\r') {
      if (i + 1 < data.size() && data[i + 1] == '\n') {
        ++i;  // consume LF too
      }
      line_offsets.push_back(i + 1);
    } else if (data[i] == '\n') {
      line_offsets.push_back(i + 1);
    }
  }
}

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

bool SourceManager::contains(std::string_view file_path) const noexcept {
  std::filesystem::path abs_path = std::filesystem::absolute(file_path);

  // If the file path is not registered in the interner then file was never loaded.
  std::size_t found_id = intern_.find(file_path);
  if (found_id == StringInterner::kInvalidId) {
    return false;
  }

  // Find source file in the lookup map. If not found then it was unloaded.
  auto found_source = lookup_.find(found_id);
  if (found_source == lookup_.end()) {
    return false;
  }

  return true;
}

std::expected<const SourceFile*, Diagnostic> SourceManager::load(
    std::string_view file_path, bool overwrite) noexcept {
  // Store file by absolute path.
  std::filesystem::path abs_path = std::filesystem::absolute(file_path);
  std::size_t path_id = intern_.push(abs_path.string());

  // Check if file was already loaded.
  auto found = lookup_.find(path_id);
  if (found != lookup_.end()) {
    // Reload and update entry if overwrite is enabled.
    if (overwrite) {
      std::expected<std::string, Diagnostic> read_file_result = util::readFile(abs_path);
      if (!read_file_result) {
        return std::unexpected(read_file_result.error());
      }
      files_[found->second]->data = std::move(*read_file_result);
      files_[found->second]->recalculateLineOffsets();
    }
    return files_[found->second].get();
  }

  // Load file data.
  std::expected<std::string, Diagnostic> read_file_result = util::readFile(abs_path);
  if (!read_file_result) {
    return std::unexpected(read_file_result.error());
  }

  // Attempt to recycle a free slot.
  if (!free_slots_.empty()) {
    std::size_t recycled_idx = free_slots_.back();
    free_slots_.pop_back();
    files_[recycled_idx]->path = path_id;
    files_[recycled_idx]->data = std::move(*read_file_result);
    files_[recycled_idx]->recalculateLineOffsets();
    lookup_.emplace(path_id, recycled_idx);
    return files_[recycled_idx].get();
  }
  // Else create a new entry.
  else {
    files_.push_back(std::make_unique<SourceFile>(files_.size(), path_id, *read_file_result));
    lookup_.emplace(path_id, files_.size() - 1);
    return files_.back().get();
  }
};


std::expected<const SourceFile*, Diagnostic> SourceManager::generate(std::string_view file_path,
                                                                     std::string_view data,
                                                                     bool overwrite) noexcept {
  std::size_t path_id = intern_.push(file_path);

  // Check if file was already loaded.
  auto found = lookup_.find(path_id);
  if (found != lookup_.end()) {
    // Reload and update entry if overwrite is enabled.
    if (overwrite) {
      files_[found->second]->data = data;
      files_[found->second]->recalculateLineOffsets();
    }
    return files_[found->second].get();
  }

  // Attempt to recycle a free slot.
  if (!free_slots_.empty()) {
    std::size_t recycled_idx = free_slots_.back();
    free_slots_.pop_back();
    files_[recycled_idx]->path = path_id;
    files_[recycled_idx]->data = data;
    files_[recycled_idx]->recalculateLineOffsets();
    lookup_.emplace(path_id, recycled_idx);
    return files_[recycled_idx].get();
  }
  // Else create a new entry.
  else {
    files_.push_back(
        std::make_unique<SourceFile>(files_.size(), path_id, std::string{data.data(),data.size()}));
    lookup_.emplace(path_id, files_.size() - 1);
    return files_.back().get();
  }
};

const SourceFile* SourceManager::get(std::size_t file_id) const noexcept {
  if (file_id >= files_.size()) {
    return nullptr;
  }
  return files_[file_id].get();
}

bool SourceManager::unload(std::size_t file_id) noexcept {
  // Out of bounds id.
  if (file_id >= files_.size()) {
    return false;
  }

  // Already freed.
  if (std::find(free_slots_.begin(), free_slots_.end(), file_id) != free_slots_.end()) {
    return false;
  }

  // Clear file data and push to free slots.
  files_[file_id]->data.clear();
  files_[file_id]->line_offsets.clear();
  free_slots_.push_back(file_id);
  lookup_.erase(files_[file_id]->path);
  return true;
}

std::string_view SourceManager::pathof(std::size_t file_id) const noexcept {
  const SourceFile* f = get(file_id);
  if (!f) {
    return "";
  }
  const std::string* fp = intern_.get(f->path);
  if (!fp) {
    return "";
  }
  return *fp;
};

std::string_view SourceManager::pathof(const SourceFile* source_file) const noexcept {
  if (!source_file) {
    return "";
  }
  return pathof(source_file->id);
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