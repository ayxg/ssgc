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
/// @brief C& source token structure.
///////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup cand_compiler_data
/// @{
#pragma once
// clang-format off
#include <map>
#include <unordered_map>

#include "ccapi/CommonCppApi.hpp"
#include "compiler_utils/CompilerProcessResult.hpp"
// clang-format on

namespace cnd {
using cldev::clmsg::ClMsgBuffer;
using cldev::clmsg::MakeClMsg;

struct SourceFile {
  Size id{};
  Str path{};
  Str data{};
  Vec<Size> line_offsets{};

  StrView slice(Size begin, Size end) const noexcept;
  StrView view() const noexcept;
  Pair<Size, Size> linecol(Size file_id, Size offset) const noexcept;
};

class SourceManager {
  Pair<Size, bool> pushFile(StrView file_path, Str data) noexcept;
  Opt<Ref<const SourceFile>> getFile(Size file_id) const noexcept;
  Opt<Ref<const SourceFile>> getByPath(StrView file_path) const noexcept;

 private:
  Size id_counter_ = std::numeric_limits<Size>::max();
  Vec<Size, SourceFile> files_{};
  std::unordered_map<StrView, Size> lookup_{};
};

Pair<Size, bool> SourceManager::pushFile(StrView file_path, Str data) noexcept {
  if (lookup_.find(file_path) != lookup_.end()) return {lookup_[file_path], false};
  id_counter_++;
  SourceFile new_file{.id = id_counter_, .path = file_path.data(), .data = data};
  Vec<Size> offsets{0};
  for (Size i = 0; i < data.size(); ++i) {
    if (data[i] == '\n') offsets.push_back(i + 1);
  }
  new_file.line_offsets = std::move(offsets);
  files_.emplace(std::move(new_file));
  lookup_.emplace(file_path, id_counter_);
  return {id_counter_, true};
}

Opt<Ref<const SourceFile>> SourceManager::getFile(Size file_id) const noexcept {
  if (file_id >= files_.size()) return std::nullopt;
  return files_.at(file_id);
}

Opt<Ref<const SourceFile>> SourceManager::getByPath(StrView file_path) const noexcept {
  auto it = lookup_.find(file_path);
  if (it != lookup_.end()) return getFile(it->second);
  return std::nullopt;
}

StrView SourceFile::slice(Size begin, Size end) const noexcept {
  assert(begin <= end && "Begin offset must be less than or equal to end offset.");
  assert(end <= data.size() && "End offset must be within the bounds of the source data.");
  assert(begin <= data.size() && "Begin offset must be within the bounds of the source data.");
  return StrView{data.data() + begin, end - begin};
}

StrView SourceFile::view() const noexcept { return StrView{data.data(), data.size()}; }

Pair<Size, Size> SourceFile::linecol(Size file_id, Size offset) const noexcept {
  auto it = std::upper_bound(line_offsets.begin(), line_offsets.end(), offset);
  Size line = std::distance(line_offsets.begin(), it);
  Size line_start = line_offsets[line - 1];
  Size column = offset - line_start + 1;
  return std::make_pair(line, column);
}

}  // namespace cnd

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