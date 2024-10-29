///////////////////////////////////////////////////////////////////////////////
// @project: Sophia Silicon Gold Solutions
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup cppmodule5_coreappfw
/// @brief A basic cache file object to store and retrieve labeled std::string
/// data from and to files. Includes a full parser and generator for the cache
/// file format.
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
///////////////////////////////////////////////////////////////////////////////

/// @addtogroup cppmodule5_coreappfw
/// @{

#ifndef HEADER_GUARD_SSG_CAF_CACHE_FILE_H
#define HEADER_GUARD_SSG_CAF_CACHE_FILE_H

// External Includes:
#include <cxxx.hpp>

namespace caf {
using std::ifstream;
using std::map;
using std::ofstream;
using std::size_t;
using std::string;
using std::vector;

class CacheFile {
 public:
  using TaggedLinesMap = map<string, vector<string>>;
  using TaggedLinesMapIter = TaggedLinesMap::iterator;
  using TaggedLinesMapNode = TaggedLinesMap::node_type;
  using TaggedLineMapNodeVec = vector<TaggedLinesMapNode>;

  bool ContainsTag(const string& tag) const;
  bool ContainsLineByTagAndIndex(const string& tag, size_t index = 0);
  const vector<string>& GetLinesByTag(const string& tag);
  vector<cxx::stdfs::path> GetLinesByTagAsPath(const string& tag);
  const string& GetLineByTagAndIndex(const string& tag, size_t index = 0);
  cxx::BoolError ReplaceLinesByTag(const string& tag,
                                   const vector<string>& new_lines);
  cxx::BoolError ReplaceLineByTagAndIndex(const string& tag, size_t index,
                                          const string& new_line);
  void PushLinesToLiveCache(const TaggedLineMapNodeVec& file_nodes);
  void PushLinesToLiveCache(const string& tag, const vector<string>& lines);
  void RemoveLiveCacheLinesByTag(const string& tag);
  void PopLineFromLiveCacheByTag(const string& tag);
  // Loads entire file into the live cache.
  void LoadLinesToLiveCache();
  void SaveLiveCacheToFile();
  void ClearLiveCache();
  void RefreshCacheFile();
  bool DoesFileExist() const;

  CacheFile() = default;
  CacheFile(const cxx::stdfs::path& file_path);

 private:
  void PushLinesToFile(const TaggedLineMapNodeVec& file_nodes);
  void PushLinesToFile(const string& tag, const vector<string>& lines);

  cxx::stdfs::path file_path;
  TaggedLinesMap live_cache;
};
}  // namespace caf

#endif HEADER_GUARD_SSG_CAF_CACHE_FILE_H
/// @} // end of cppmodule5_coreappfw

///////////////////////////////////////////////////////////////////////////////
// @project: Sophia Silicon Gold Solutions
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////////////////