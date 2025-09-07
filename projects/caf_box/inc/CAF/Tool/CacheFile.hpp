///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2025 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: [CAF] Core Application Framework
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup coreappfw
/// @brief [HEADER]
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup coreappfw
/// @{

// clang-format off
#pragma once
#include <filesystem>
#include "cxxx.hpp"

// clang-format on

namespace caf {
namespace stdfs = std::filesystem;
template <class T>
struct CacheFile {
  using PathType = stdfs::path;
  using DataType = T;
  PathType data{};
  DataType path{};

  bool Load() {
    if (!stdfs::exists(path)) return false;
    std::ifstream cache_file(path);
    if (!cache_file.is_open()) return false;
    try {
      data = DataType::FromJson(nlohmann::json::parse(cache_file));
    } catch (nlohmann::json::parse_error& err) {
      return false;
    }
  };

  bool Save() const {
    std::ofstream cache_file(path);
    if (!cache_file.is_open()) return false;
    cache_file << DataType::ToJson(data).dump();
    cache_file.close();
    return true;
  };
};

}  // namespace caf

/// @} // end of coreappfw

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: [CAF] Core Application Framework
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
// @created: 2025/05/16
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2025 Anton Yashchenko
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
