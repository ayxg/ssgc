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
/// @brief [SOURCE] BconObject class
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup coreappfw
/// @{

// clang-format off
#include "CAF/Tool/BconObject.hpp"

#include <filesystem>
#include <map>
#include <vector>
#include <string>

#include "cxxx.hpp"
// clang-format on

namespace caf {
bool BconObject::ContainsTag(const std::string& tag) const { return live_cache.contains(tag); }

bool BconObject::ContainsLineByTagAndIndex(const std::string& tag, std::size_t index) {
  if (live_cache.contains(tag)) {
    return index < live_cache[tag].size();
  }
  return false;
}

const std::vector<std::string>& BconObject::GetLinesByTag(const std::string& tag) {
  if (live_cache.contains(tag)) {
    return live_cache[tag];
  }
  return {};
}

std::vector<cxx::stdfs::path> BconObject::GetLinesByTagAsPath(const std::string& tag) {
  std::vector<cxx::stdfs::path> ret{};
  if (live_cache.contains(tag)) {
    for (const auto& entry : live_cache[tag]) {
      ret.push_back(cxx::stdfs::path(entry));
    };
  }
  return ret;
}

const std::string& BconObject::GetLineByTagAndIndex(const std::string& tag, std::size_t index) {
  if (live_cache.contains(tag)) {
    if (index < live_cache[tag].size()) return live_cache[tag][index];
  }
  return "";
}

cxx::BoolError BconObject::ReplaceLinesByTag(const std::string& tag, const std::vector<std::string>& new_lines) {
  if (live_cache.contains(tag)) {
    live_cache[tag].clear();
    live_cache[tag].insert(live_cache[tag].end(), new_lines.begin(), new_lines.end());
    return true;
  }
  return "No cache lines with this tag exist";
}

cxx::BoolError BconObject::ReplaceLineByTagAndIndex(const std::string& tag, std::size_t index,
                                                    const std::string& new_line) {
  if (live_cache.contains(tag)) {
    if (index < live_cache[tag].size()) {
      live_cache[tag][index] = new_line;
      return true;
    }
  }
  return "No cache lines with this tag exist";
}

void BconObject::PushLinesToLiveCache(const TaggedLineMapNodeVec& file_nodes) {
  for (const auto& nd : file_nodes) {
    PushLinesToLiveCache(nd.key(), nd.mapped());
  }
}

void BconObject::PushLinesToLiveCache(const std::string& tag, const std::vector<std::string>& lines) {
  if (live_cache.contains(tag)) {
    live_cache[tag].insert(live_cache[tag].end(), lines.begin(), lines.end());
  } else
    live_cache[tag] = lines;
}

void BconObject::RemoveLiveCacheLinesByTag(const std::string& tag) { live_cache.erase(tag); }

void BconObject::PopLineFromLiveCacheByTag(const std::string& tag) {
  if (live_cache.contains(tag)) {
    live_cache[tag].pop_back();
  }
}

// Loads entire file into the live cache.
void BconObject::LoadLinesToLiveCache() {
  std::ifstream in_file_stream{file_path};
  std::string line;
  while (std::getline(in_file_stream, line)) {
    if (line == "#def#") {
      std::getline(in_file_stream, line);
      std::string tag = line;
      std::vector<std::string> lines;
      while (std::getline(in_file_stream, line)) {
        if (line == "#end#") {
          break;
        }
        lines.push_back(line);
      }

      // Append std::vectors if the tag already exists.
      if (live_cache.contains(tag)) {
        live_cache[tag].insert(live_cache[tag].end(), lines.begin(), lines.end());
      } else
        live_cache[tag] = lines;
    }
  }
}

void BconObject::SaveLiveCacheToFile() {
  std::ofstream out_file_stream{file_path};
  for (const auto& [tag, lines] : live_cache) {
    out_file_stream << "#def#\n" << tag << "\n";
    for (const auto& line : lines) {
      out_file_stream << line << "\n";
    }
    out_file_stream << "#end#\n";
  }
}

void BconObject::ClearLiveCache() { live_cache.clear(); }

void BconObject::RefreshCacheFile() {
  std::ofstream cache_fstream(file_path);
  cache_fstream.clear();
  cache_fstream.close();
}

bool BconObject::DoesFileExist() const { return cxx::stdfs::exists(file_path); }

BconObject::BconObject(const cxx::stdfs::path& file_path) : file_path(file_path) {}

void BconObject::PushLinesToFile(const TaggedLineMapNodeVec& file_nodes) {
  for (const auto& nd : file_nodes) {
    PushLinesToFile(nd.key(), nd.mapped());
  }
}

void BconObject::PushLinesToFile(const std::string& tag, const std::vector<std::string>& lines) {
  // 1 line for #def# directive
  // 1 line for #end# directive
  // 1 line for the tag.
  // 1 line for each line in the cache.
  std::ofstream out_file_stream{file_path, std::ios::app};
  out_file_stream << "#def#\n" << tag << "\n";
  for (auto& line : lines) {
    out_file_stream << line << "\n";
  }
  out_file_stream << "#end#\n";
}

}  // namespace caf

/// @} // end of coreappfw

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: [CAF] Core Application Framework
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
// @created: 2025/05/09
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
