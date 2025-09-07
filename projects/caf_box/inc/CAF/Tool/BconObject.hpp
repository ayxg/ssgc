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
/// @brief [HEADER] B.C.O.N. Basic Cache Object Notation format, file parser and structure object.
/// A basic cache file object to store and retrieve labeled std::string data from and to files.
/// Includes a full parser and generator for the cache file format. The cache file format is newline based with
/// tags using `#def#` and `#end#` to denote groups. It was made up on the spot for the specific use case. The format is
/// not meant to be human-readable, but rather easily parsable and writable by the code.
///
/// @note This class is deprecated, it was made before I decided to add nlohmann/json as a dependency. You should be
/// using the caf included JSON functionality instead.
///
/// Sample output:
/// @code
///   #def#
///   solution_dir
///   dafsldkfjaslkdfjalks
///   #end#
///   #def#
///   cached_files
///   file1.cpp
///   file2.cpp
///   file3.cpp
///   #end#
/// @endcode
///
/// Sample Use:
/// @code
///   auto cache_file_test() {
///     CacheFile cache_file{"C:\\candide\\cache\\file1.cpp"};
///     cache_file.LoadLinesToLiveCache();
///     cache_file.PushLinesToLiveCache("binary_path", {"file1.cpp",
///     "file2.cpp"}); cache_file.PushLinesToLiveCache("cached_files",
///     {"file3.cpp", "file4.cpp"}); cache_file.SaveLiveCacheToFile();
///   }
/// @endcode
///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup coreappfw
/// @{

// clang-format off
#pragma once
#include <filesystem>
#include <map>
#include <vector>
#include <string>

#include "cxxx.hpp"
// clang-format on

namespace caf {
  
class BconObject {
 public:
  using PathType = std::filesystem::path;
  using TaggedLinesMap = std::map<std::string, std::vector<std::string>>;
  using TaggedLinesMapIter = TaggedLinesMap::iterator;
  using TaggedLinesMapNode = TaggedLinesMap::node_type;
  using TaggedLineMapNodeVec = std::vector<TaggedLinesMapNode>;

  bool ContainsTag(const std::string& tag) const;
  bool ContainsLineByTagAndIndex(const std::string& tag, size_t index = 0);
  const std::vector<std::string>& GetLinesByTag(const std::string& tag);
  std::vector<PathType> GetLinesByTagAsPath(const std::string& tag);
  const std::string& GetLineByTagAndIndex(const std::string& tag, size_t index = 0);
  cxx::BoolError ReplaceLinesByTag(const std::string& tag, const std::vector<std::string>& new_lines);
  cxx::BoolError ReplaceLineByTagAndIndex(const std::string& tag, std::size_t index, const std::string& new_line);
  void PushLinesToLiveCache(const TaggedLineMapNodeVec& file_nodes);
  void PushLinesToLiveCache(const std::string& tag, const std::vector<std::string>& lines);
  void RemoveLiveCacheLinesByTag(const std::string& tag);
  void PopLineFromLiveCacheByTag(const std::string& tag);
  // Loads entire file into the live cache.
  void LoadLinesToLiveCache();
  void SaveLiveCacheToFile();
  void ClearLiveCache();
  void RefreshCacheFile();
  bool DoesFileExist() const;

  BconObject() = default;
  explicit BconObject(const PathType& file_path);

 private:
  void PushLinesToFile(const TaggedLineMapNodeVec& file_nodes);
  void PushLinesToFile(const std::string& tag, const std::vector<std::string>& lines);

  PathType file_path;
  TaggedLinesMap live_cache;
};

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
