///////////////////////////////////////////////////////////////////////////////
// @project: Sophia Silicon Gold Solutions
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup cppmodule5_coreappfw
/// @brief caf_cache_file.hpp source
///////////////////////////////////////////////////////////////////////////////

/// @addtogroup cppmodule5_coreappfw
/// @{

#include "caf_cache_file.hpp"

#include <cxxx.hpp>

namespace caf {
bool CacheFile::ContainsTag(const std::string& tag) const {
  return live_cache.contains(tag);
}

bool CacheFile::ContainsLineByTagAndIndex(const std::string& tag,
                                          std::size_t index) {
  if (live_cache.contains(tag)) {
    return index < live_cache[tag].size();
  }
  return false;
}

const std::vector<std::string>& CacheFile::GetLinesByTag(
    const std::string& tag) {
  if (live_cache.contains(tag)) {
    return live_cache[tag];
  }
  return {};
}

std::vector<cxx::stdfs::path> CacheFile::GetLinesByTagAsPath(
    const std::string& tag) {
  std::vector<cxx::stdfs::path> ret{};
  if (live_cache.contains(tag)) {
    for (const auto& entry : live_cache[tag]) {
      ret.push_back(cxx::stdfs::path(entry));
    };
  }
  return ret;
}

const std::string& CacheFile::GetLineByTagAndIndex(const std::string& tag,
                                                   std::size_t index) {
  if (live_cache.contains(tag)) {
    if (index < live_cache[tag].size()) return live_cache[tag][index];
  }
  return "";
}

cxx::BoolError CacheFile::ReplaceLinesByTag(
    const std::string& tag, const std::vector<std::string>& new_lines) {
  if (live_cache.contains(tag)) {
    live_cache[tag].clear();
    live_cache[tag].insert(live_cache[tag].end(), new_lines.begin(),
                           new_lines.end());
    return true;
  }
  return "No chache lines with this tag exist";
}

cxx::BoolError CacheFile::ReplaceLineByTagAndIndex(
    const std::string& tag, std::size_t index, const std::string& new_line) {
  if (live_cache.contains(tag)) {
    if (index < live_cache[tag].size()) {
      live_cache[tag][index] = new_line;
      return true;
    }
  }
  return "No chache lines with this tag exist";
}

void CacheFile::PushLinesToLiveCache(const TaggedLineMapNodeVec& file_nodes) {
  for (const auto& nd : file_nodes) {
    PushLinesToLiveCache(nd.key(), nd.mapped());
  }
}

void CacheFile::PushLinesToLiveCache(const std::string& tag,
                                     const std::vector<std::string>& lines) {
  if (live_cache.contains(tag)) {
    live_cache[tag].insert(live_cache[tag].end(), lines.begin(), lines.end());
  } else
    live_cache[tag] = lines;
}

void CacheFile::RemoveLiveCacheLinesByTag(const std::string& tag) {
  live_cache.erase(tag);
}

void CacheFile::PopLineFromLiveCacheByTag(const std::string& tag) {
  if (live_cache.contains(tag)) {
    live_cache[tag].pop_back();
  }
}

// Loads entire file into the live cache.
void CacheFile::LoadLinesToLiveCache() {
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
        live_cache[tag].insert(live_cache[tag].end(), lines.begin(),
                               lines.end());
      } else
        live_cache[tag] = lines;
    }
  }
}

void CacheFile::SaveLiveCacheToFile() {
  std::ofstream out_file_stream{file_path};
  for (const auto& [tag, lines] : live_cache) {
    out_file_stream << "#def#\n" << tag << "\n";
    for (const auto& line : lines) {
      out_file_stream << line << "\n";
    }
    out_file_stream << "#end#\n";
  }
}

void CacheFile::ClearLiveCache() { live_cache.clear(); }

void CacheFile::RefreshCacheFile() {
  std::ofstream cache_fstream(file_path);
  cache_fstream.clear();
  cache_fstream.close();
}

bool CacheFile::DoesFileExist() const { return cxx::stdfs::exists(file_path); }

CacheFile::CacheFile(const cxx::stdfs::path& file_path)
    : file_path(file_path) {}

void CacheFile::PushLinesToFile(const TaggedLineMapNodeVec& file_nodes) {
  for (const auto& nd : file_nodes) {
    PushLinesToFile(nd.key(), nd.mapped());
  }
}

void CacheFile::PushLinesToFile(const std::string& tag,
                                const std::vector<std::string>& lines) {
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

/// @} // end of cppmodule5_coreappfw

///////////////////////////////////////////////////////////////////////////////
// @project: Sophia Silicon Gold Solutions
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
//
// Licensed under the Apache License, Version 2.0(the "License");
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
///////////////////////////////////////////////////////////////////////////////