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
#include <optional>
#include <string>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <vector>

namespace ssgc::util {

using StringId = std::size_t;

class StringInterner {
  std::vector<std::unique_ptr<std::string>> strings_{};
  std::unordered_map<std::string_view, StringId> lookup_{};

 public:
  static constexpr StringId kInvalidId = std::numeric_limits<StringId>::max();
  bool contains(std::string_view data) const { return lookup_.find(data) != lookup_.end(); }

  StringId push(std::string_view data) {
    auto found = lookup_.find(data);
    if (found != lookup_.end()) {
      return found->second;
    }
    strings_.push_back(std::make_unique<std::string>(data.data()));
    return strings_.size() - 1;
  }

  const std::string* get(StringId id) const {
    if (id >= strings_.size()) {
      return nullptr;
    }
    return strings_[id].get();
  }

  StringId find(std::string_view data) const noexcept {
    auto found = lookup_.find(data);
    if (found != lookup_.end()) {
      return found->second;
    }
    return kInvalidId;
  }

  void clear() {
    strings_.clear();
    lookup_.clear();
  }
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