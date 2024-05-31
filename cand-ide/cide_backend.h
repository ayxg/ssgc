#pragma once
// Standard
#include "cppsextended.h"

// CIDE Common Defs
#include "cide_common.h"

constexpr auto AdvanceIt(auto&& iter, size_t i) {
  std::advance(iter, i);
  return iter;
}

namespace cide::backend {

// Utils
static string LoadFileToStr(stdfs::path file_path) {
  std::filesystem::path filepath(
      std::filesystem::absolute(std::filesystem::path(file_path)));

  std::uintmax_t fsize;

  if (std::filesystem::exists(filepath)) {
    fsize = std::filesystem::file_size(filepath);
  } else {
    throw(std::invalid_argument("File not found: " + filepath.string()));
  }

  std::ifstream infile;
  infile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try {
    infile.open(filepath.c_str(), std::ios::in | std::ifstream::binary);
  } catch (...) {
    std::throw_with_nested(
        std::runtime_error("Can't open input file " + filepath.string()));
  }

  string fileStr;

  try {
    fileStr.resize(fsize);
  } catch (...) {
    std::stringstream err;
    err << "Can't resize to " << fsize << " bytes";
    std::throw_with_nested(std::runtime_error(err.str()));
  }

  infile.read(fileStr.data(), fsize);
  infile.close();

  return fileStr;
}
static inline string LoadFileToStr(const char* const file_path) {
  return LoadFileToStr(std::filesystem::path(file_path));
}
static inline string LoadFileToStr(const string& file_path) {
  return LoadFileToStr(file_path.c_str());
};

static void SaveStrToFile(const string& file_path,
                          const string& file_contents) {
  std::ofstream outfile;
  outfile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try {
    outfile.open(file_path.c_str(), std::ios::out | std::ofstream::binary);
  } catch (...) {
    std::throw_with_nested(
        std::runtime_error("Can't open output file " + file_path));
  }

  outfile.write(file_contents.data(), file_contents.size());
  outfile.close();
}

// #def#
// solution_dir
// dafsldkfjaslkdfjalks
// #end#

// #def#
// cached_files
// file1.cpp
// file2.cpp
// file3.cpp
// #end#

struct CacheFile {
 public:
  using TaggedLinesMap = std::map<string, vector<string>>;
  using TaggedLinesMapIter = TaggedLinesMap::iterator;
  using TaggedLinesMapNode = TaggedLinesMap::node_type;
  using TaggedLineMapNodeVec = std::vector<TaggedLinesMapNode>;

 public:
  bool ContainsTag(const string& tag) { return live_cache.contains(tag); }
  bool ContainsLineByTagAndIndex(const string& tag, std::size_t index = 0) {
    if (live_cache.contains(tag)) {
      return index < live_cache[tag].size();
    }
    return false;
  }

  const vector<string>& GetLinesByTag(const string& tag) {
    if (live_cache.contains(tag)) {
      return live_cache[tag];
    }
    return {};
  }
  vector<stdfs::path> GetLinesByTagAsPath(const string& tag) {
    vector<stdfs::path> ret{};
    if (live_cache.contains(tag)) {
      for (const auto& entry : live_cache[tag]) {
        ret.push_back(stdfs::path(entry));
      };
    }
    return ret;
  }
  const string& GetLineByTagAndIndex(const string& tag, std::size_t index = 0) {
    if (live_cache.contains(tag)) {
      if (index < live_cache[tag].size()) return live_cache[tag][index];
    }
    return "";
  }

  cxx::BoolError ReplaceLinesByTag(const string& tag,
                                   const vector<string>& new_lines) {
    if (live_cache.contains(tag)) {
      live_cache[tag].clear();
      live_cache[tag].insert(live_cache[tag].end(), new_lines.begin(),
                             new_lines.end());
      return true;
    }
    return "No chache lines with this tag exist";
  }
  cxx::BoolError ReplaceLineByTagAndIndex(const string& tag, std::size_t index,
                                          const string& new_line) {
    if (live_cache.contains(tag)) {
      if (index < live_cache[tag].size()) {
        live_cache[tag][index] = new_line;
        return true;
      }
    }
    return "No chache lines with this tag exist";
  }

  void PushLinesToLiveCache(const TaggedLineMapNodeVec& file_nodes) {
    for (const auto& nd : file_nodes) {
      PushLinesToLiveCache(nd.key(), nd.mapped());
    }
  }
  void PushLinesToLiveCache(const string& tag, const vector<string>& lines) {
    if (live_cache.contains(tag)) {
      live_cache[tag].insert(live_cache[tag].end(), lines.begin(), lines.end());
    } else
      live_cache[tag] = lines;
  }

  void RemoveLiveCacheLinesByTag(const string& tag) { live_cache.erase(tag); }
  void PopLineFromLiveCacheByTag(const string& tag) {
    if (live_cache.contains(tag)) {
      live_cache[tag].pop_back();
    }
  }
  // Loads entire file into the live cache.
  void LoadLinesToLiveCache() {
    std::ifstream in_file_stream{file_path};
    string line;
    while (std::getline(in_file_stream, line)) {
      if (line == "#def#") {
        std::getline(in_file_stream, line);
        string tag = line;
        vector<string> lines;
        while (std::getline(in_file_stream, line)) {
          if (line == "#end#") {
            break;
          }
          lines.push_back(line);
        }

        // Append vectors if the tag already exists.
        if (live_cache.contains(tag)) {
          live_cache[tag].insert(live_cache[tag].end(), lines.begin(),
                                 lines.end());
        } else
          live_cache[tag] = lines;
      }
    }
  }
  void SaveLiveCacheToFile() {
    std::ofstream out_file_stream{file_path};
    for (const auto& [tag, lines] : live_cache) {
      out_file_stream << "#def#\n" << tag << "\n";
      for (const auto& line : lines) {
        out_file_stream << line << "\n";
      }
      out_file_stream << "#end#\n";
    }
  }

  void ClearLiveCache() { live_cache.clear(); }
  void RefreshCacheFile() {
    std::ofstream cache_fstream(file_path);
    cache_fstream.clear();
    cache_fstream.close();
  }

  bool DoesFileExist() const { return stdfs::exists(file_path); }

 public:
  CacheFile() = default;
  CacheFile(const stdfs::path& file_path) : file_path(file_path) {}

 private:
  void PushLinesToFile(const TaggedLineMapNodeVec& file_nodes) {
    for (const auto& nd : file_nodes) {
      PushLinesToFile(nd.key(), nd.mapped());
    }
  }
  void PushLinesToFile(const string& tag, const vector<string>& lines) {
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

 private:
  stdfs::path file_path;
  TaggedLinesMap live_cache;
};

//auto cache_file_test() {
//  CacheFile cache_file{"C:\\candide\\cache\\file1.cpp"};
//  cache_file.LoadLinesToLiveCache();
//  cache_file.PushLinesToLiveCache("binary_path", {"file1.cpp", "file2.cpp"});
//  cache_file.PushLinesToLiveCache("cached_files", {"file3.cpp", "file4.cpp"});
//  cache_file.SaveLiveCacheToFile();
//}

// IDE startup settings, all values must be known at compile time.
// Represent a .caide file
class IdeSettings {
  static constexpr const wchar_t* kSolutionCacheSettingTag = L"#\n";
  static constexpr inline auto kDefaultRepoPath = L"C:\\candide\\repository\\";
  static inline stdfs::path GetDefaultBinaryPath() {
    return stdfs::path{std::filesystem::current_path().native() + L"\\"};
  }
  static inline stdfs::path GetDefaultRepoPath() {
    return stdfs::path(L"C:\\candide\\repository\\");
  }
  static inline stdfs::path GetDefaultSettingsFilePath() {
    return GetDefaultBinaryPath().append(file_extension::wide::kCaIdeSettings);
  }

 public:
  const stdfs::path& ViewRepoPath() const { return repository_path; }
  const stdfs::path& ViewBinaryPath() const { return binary_path; }

  // Load the settings from a file. Return false if the file is invalid.
  bool Load() {
    // If file does not exist create a new default settings file.
    if (not settings_cache_file.DoesFileExist()) {
      settings_cache_file.RefreshCacheFile();
      settings_cache_file.PushLinesToLiveCache(
          "binary_path", {GetDefaultBinaryPath().string()});
      settings_cache_file.PushLinesToLiveCache("repository_path",
                                               {GetDefaultRepoPath().string()});
      settings_cache_file.SaveLiveCacheToFile();
    }

    settings_cache_file.LoadLinesToLiveCache();

    if (settings_cache_file.ContainsTag("binary_path")) {
      binary_path = settings_cache_file.GetLineByTagAndIndex("binary_path", 0);
    }

    if (settings_cache_file.ContainsTag("repository_path")) {
      repository_path =
          settings_cache_file.GetLineByTagAndIndex("repository_path", 0);
    }

    if (settings_cache_file.ContainsTag("cached_solutions")) {
      cached_solutions =
          settings_cache_file.GetLinesByTagAsPath("cached_solutions");
    }

    settings_cache_file.ClearLiveCache();

    return true;

    // if (!std::filesystem::exists(GetSettingsFilePath())) {
    //   // If no settings exist. Load defaults.
    //   return Default();
    // }

    //// Load.
    // string settings = LoadFileToStr(GetSettingsFilePath().string());
    //// Extract the binary_path
    // auto c = settings.begin();
    // while (c != settings.end()) {
    //   if (*c = '\n') {
    //     break;
    //   }
    //   c++;
    // }
    // binary_path = string(settings.begin(), c);
    // c++;  // advance past newline.
    // string::iterator repo_path_begin = c;
    //// Extract the repository path.
    // while (c != settings.end()) {
    //   if (*c = '\n') {
    //     break;
    //   }
    //   c++;
    // }
    // repository_path = string(settings.begin(), c);
    // c++;
    //// If there is more, it must be a # indicating start of
    //// solution cache.
    // if (c != settings.end() && *c == '#') {
    //   c += 2;
    //   string::iterator last_cache_begin = c;
    //   // consume lines until next '#'
    //   while (c != settings.end() || *c == '#') {
    //     if (*c = '\n') {
    //       cached_solutions.push_back(string(last_cache_begin, c));
    //       c++;
    //       last_cache_begin = c;
    //     } else
    //       c++;
    //   }
    // }
    // return true;
  }

  // Save the settings to a file in the current binary path.
  bool Save() {
    settings_cache_file.RefreshCacheFile();

    settings_cache_file.PushLinesToLiveCache("binary_path",
                                             {binary_path.string()});

    settings_cache_file.PushLinesToLiveCache("repository_path",
                                             {repository_path.string()});

    vector<std::string> cached_sln_lines;
    for (auto& sln : cached_solutions) {
      cached_sln_lines.push_back(sln.string());
    }
    settings_cache_file.PushLinesToLiveCache("cached_solutions",
                                             cached_sln_lines);

    settings_cache_file.SaveLiveCacheToFile();

    return true;
    //// Overwrite previous file. Delete it.
    // stdfs::path file = GetSettingsFilePath();
    // if (std::filesystem::exists(file)) {
    //   // If its empty dont have to clear.
    //   if (!file.empty()) {
    //     std::filesystem::remove(file);
    //   }
    //   // Generate the new file.
    //   GenerateSettingsFile();

    //} else {
    //  GenerateSettingsFile();
    //}
  }

  // Cache a solution folder, if already chached, returns false.
  bool CacheSolution(const stdfs::path& sln_folder) {
    if (cached_solutions.end() != std::find(cached_solutions.begin(),
                                            cached_solutions.end(),
                                            sln_folder)) {
      return false;  // Solution is already cached...
    }
    cached_solutions.push_back(sln_folder);
    return true;
  }

 public:
  IdeSettings() = default;
  IdeSettings(const stdfs::path& bin_path) : binary_path(bin_path) {}
  IdeSettings(const stdfs::path& bin_path, const stdfs::path& repo_path)
      : binary_path(bin_path), repository_path(repo_path) {}

 private:
  CacheFile settings_cache_file{GetDefaultSettingsFilePath()};

  // Get the binary path of the IDE. Current working directory of the C++
  // context. NOTE: this a copy not reference. current_path() is a dangerous
  // global.
  // In the context of development it will be the cand-ide project folder.
  stdfs::path binary_path{
      GetDefaultBinaryPath()};  // fs::path to the IDE executable/binaries.
                                // This is the current working path of
                                // the C++ backend. fs::path to the
                                // cand-ide.exe folder.
  stdfs::path repository_path{
      GetDefaultRepoPath()};  // fs::path to the folder containing all solutions
                              // Which belong to this IDE. Only these will be
                              // Scanned and populated in the solution list.
                              // Each folder which ccontains a '.casln' file
  // is a solution. Only one .casln is loaded per folder.
  // The rest are ignored.

  std::vector<stdfs::path> cached_solutions{};

  // inline void GenerateSettingsFile() const {
  //   std::ofstream(GetSettingsFilePath())
  //       // 1. Binary fs::path
  //       << binary_path.string()
  //       << '\n'
  //       // 2. Repository fs::path
  //       << repository_path.string()
  //       << '\n'
  //       // 3. Cached Solutions
  //       << [this]() -> string {
  //     std::wstring sln_folders = kSolutionCacheSettingTag;
  //     for (auto& cached_sln : this->cached_solutions) {
  //       sln_folders += cached_sln.native() + L'\n';
  //     }
  //     sln_folders += kSolutionCacheSettingTag;
  //     return string(sln_folders.cbegin(), sln_folders.cend());
  //   }();
  // }

  //// Create folder default repository path of C://candide/repository/
  //// If folder does not exist. Adds repo path.
  //// Generate a default settings file and save it in the current binary path.
  // bool Default() {
  //   repository_path = "C:\\candide\\repository\\";
  //   try {
  //     std::filesystem::create_directories(repository_path);
  //     GenerateSettingsFile();
  //   } catch (...) {
  //     // Failed to load/create file.
  //     return false;
  //   }
  //   return true;
  // };
};

// Represent a .casln file which holds metadata about the solution.
// Can be saved and reloaded.
struct SolutionSettings {
  inline stdfs::path GetSettingsFilePath() const;
  bool Default();
  bool Save();
  bool Load();
  // Directory of the solution.[RELATIVE: Relative to the repository path.]
  // This is the working directory of the Solution's filesystem.
  // If you load a file from IDE code. This is the root directory.
  // For this solution's executable.
  stdfs::path solution_path;

  // Solution file [RELATIVE: Relative to the repository path]
  // .cansln file where solution metadata is stored. Only 1 per solution.
  // It is auto-inferred to be the file named '.casln' inside the solution path.
  stdfs::path solution_file;

  // .camake files describing a build process of this solution.Autogenerated,
  // user-editable. unnamed file '.camake' is automatically included as the
  // first and default build file.
  std::vector<stdfs::path> build_files;

  // Working files which belong to the solution. Not including .casln / .camake
  // files.
  std::vector<stdfs::path> working_files;
};

// Model of an instance of a file tab in the editor.
struct IdeFileTab {
  bool is_active = false;
  string* const text_buffer{new string("")};
  const string tab_name{};
  const stdfs::path file_path{};

  ~IdeFileTab() { delete text_buffer; }
};

struct IdeFileEditor {
 public:
  using FileTab = IdeFileTab;
  using ExpectedFileTabRef = cxx::Expected<std::reference_wrapper<IdeFileTab>>;
  using FileTabVec = vector<IdeFileTab>;
  using FileTabVecIter = vector<IdeFileTab>::iterator;

 public:
  // Add a new file tab to the editor given a file path.Also make it active.
  FileTab& AddFileTab(const stdfs::path& file_path) {
    file_tabs.emplace_back(FileTab{
        .is_active = false,
        .text_buffer = new string(""),
        .tab_name = file_path.filename().string(),
        .file_path = file_path,
    });
    return SetFileTabActiveUnchecked(file_tabs.size() - 1);
  }

  // Sets a file tab as active given a file path. If the file is not open, it
  // will be opened and set as active.
  FileTab& SetFileTabActive(const stdfs::path& file_path) {
    for (auto& tab : file_tabs) {
      if (tab.file_path == file_path) {
        tab.is_active = true;
      } else {
        tab.is_active = false;
      }
    }

    auto active_tab =
        std::find_if(file_tabs.begin(), file_tabs.end(),
                     [](const FileTab& tab) { return tab.is_active; });

    if (active_tab == file_tabs.end()) {
      AddFileTab(file_path);
      return SetFileTabActive(file_path);
    } else {
      active_tab_ = active_tab;
      return *active_tab;
    }
  }

  // Sets active file tab given an index. if the index is out of bounds
  // method will return error.
  ExpectedFileTabRef SetFileTabActive(std::size_t index) {
    if (index >= file_tabs.size()) {
      return ExpectedFileTabRef::Failure("Index out of bounds.");
    }

    for (std::size_t i = 0; i < file_tabs.size(); i++) {
      if (i == index) {
        active_tab_ = AdvanceIt(file_tabs.begin(), i);
        active_tab_->is_active = true;
      } else {
        file_tabs[i].is_active = false;
      }
    }

    return ExpectedFileTabRef::Success(file_tabs[index]);
  }

  // Sets active file tab without checking out of bounds.
  FileTab& SetFileTabActiveUnchecked(std::size_t index) {
    for (auto& t : file_tabs) {
      t.is_active = false;
    }
    file_tabs[index].is_active = true;
  }

  string& const GetActiveTextBuffer() const {
    return *active_tab_->text_buffer;
  }

 private:
  FileTabVec file_tabs{};
  FileTabVecIter active_tab_{};
};

struct IdeModel {
  IdeSettings settings{};
  SolutionSettings
      active_sln_settings;  // Settings of the currently active solution.
  // Initialize the IDE.
  // 1. Load settings from the IDE settings file. If the settings file does not
  // exist, create it.
  //    Default settings file:
  //
  bool startup() { return settings.Load(); }

  // Creates a new solution.
  cxx::BoolError NewSolution(const string& name) {
    // Check if the path exists. If it already does cancel.
    // Query for a diffrent solution name.
    stdfs::path sln_folder_path = settings.ViewRepoPath();
    sln_folder_path += name;

    if (stdfs::exists(sln_folder_path)) {
      return "Could not create solution dirs: Solution folder is already in "
             "use.";
    }

    if (!stdfs::create_directories(sln_folder_path)) {
      return "Could not create solution dirs: OS write access may not be "
             "available.";
    };

    // Add this solution to the solution cache.
    settings.CacheSolution(sln_folder_path);

    // Create the solution settings file.
    // Arg to constructor is the root solution dir.
    SolutionSettings sln_settings{sln_folder_path};

    // Generate default solution settings.
    sln_settings.Default();

    // Set as current active solution.
    active_sln_settings = sln_settings;

    return true;
  }

  // Open an existing solution as the active solution.
  // Impl Detail:
  //  1. Find the directory with the same name as solution in the repo folder.
  //  2. Load the .casln settings file. If file is invalid, solution is
  //  corrupted.
  //  3. Load all the cached working files from the .casln settings into the
  //  file editor.
  //  4. Load last-active cached file.
  bool OpenSolution(const string& name) {
    stdfs::path sln_folder_path = settings.ViewRepoPath();
    sln_folder_path += name;

    if (!stdfs::exists(sln_folder_path)) {
      return "Could not open solution: solution folder does not exist.";
    }

    // Load the solution settings file.
    SolutionSettings sln_settings{sln_folder_path};
    if (!sln_settings.Load()) {
      return "Could not open solution: .casln' solution settings file possibly "
             "corrupted.";
    }

    // Load the solution settings into the active solution.
    active_sln_settings = sln_settings;

    //// Load all the cached working files from the .casln settings into the
    /// file / editor.
    // for (auto& file : active_sln_settings.working_files) {
    //   // Load the file into the editor.
    //   // If the file is not found, skip it.
    //   if (!stdfs::exists(file)) {
    //     continue;
    //   }
  }
};

}  // namespace cide::backend

// Implementation
namespace cide::backend {



}