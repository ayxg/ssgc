///////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
///////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language Environment
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup cand_cide
/// @brief CIDE Backend Implementation.
///////////////////////////////////////////////////////////////////////////////

#pragma once
// Standard
#include "caf.hpp"
#include "cxxx.hpp"
// CIDE Common Defs
#include "cide_common.h"
#include "wpl_core.hpp"

/// @addtogroup cand_cide_backend
/// @{

namespace cide::backend {

using caf::CacheFile;
using cxx::AdvanceIt;
using cxx::LoadFileToStr;
using cxx::SaveStrToFile;

// JSON Serialization
namespace jsonlib = nlohmann;
using JsonObj = jsonlib::json;

template <class T>
concept JsonConvertible = requires(const T& t, const JsonObj& o) {
  { T::ToJson(t) } -> std::same_as<JsonObj>;
  { T::FromJson(o) } -> std::same_as<T>;
};

JsonObj ToJson(const JsonConvertible auto& obj) {
  return decltype(obj)::ToJson(obj);
}

auto FromJson(const JsonConvertible auto& obj, const JsonObj& json)
    -> std::decay_t<decltype(obj)> {
  return std::decay_t<decltype(obj)>::FromJson(json);
}

// Cide Backend Error
enum class eApiErr : unsigned int {
  kUnknown,
  kDirDepNotFound,        // A directory was expected to exist on the system.
  kExeDepNotFound,        // an executable required could not be run
  kSubprocessCannotRun,   // failed to run an executable
  kSubprocessFailedExit,  // executable run by this program returned non 0
                          // result.
  kFileNotFound,          // file was expected to exist on the system.
  kJsonParseError,        // failed to parse a json file.
};

struct ApiErr {
  eApiErr num{eApiErr::kUnknown};
  cstring data{""};
};

template <class T>
using ApiRes = std::expected<T, ApiErr>;
using ApiFail = std::unexpected<ApiErr>;

static constexpr ApiFail MakeApiFail(eApiErr err, cstring data = "");
static constexpr ApiFail MakeApiFail(ApiErr fail);

enum eParamInterpType : size_t {
  Line,
  List,
  Choice,
};

/// Ide settings detected based on the host operating system and IDE binary.
/// These settings must be set before first launch of the IDE. Once set, a
/// reconfiguration is only required if the IDE binary location changes.
enum class eToolchainParam : size_t {
  kCmakePath,
  kCppCompilerPath,
  kCppLinkerPath,

  // Os General
  kOsTempPath,
  kOsProgramDataPath,
  kOsCliPath,

  // Windows Toolset
  kWin32ProgramFilesDir,
  kWin32VswherePath,  /// Detects Visual Studio installations.
  kWin32MsvsPath,     /// Used to find VsDevCmd.bat(vs dev cli) and other MSVC
                      /// C++ Toolchain components.
  kWin32MsvsInstallerPath,  /// %ProgramFiles(x86)%\Microsoft Visual
                            /// Studio\Installer (use %ProgramFiles%
                            /// in a 32-bit program prior to Windows 10). This
                            /// is a fixed location that will be maintained.
                            /// Details:
                            /// https://github.com/microsoft/vswhere/wiki/Installing
  kWin32VsdevcmdPath,  /// Path to the MSVS DevCmd.bat.
  kCount
};

static constexpr string_view eToolchainParamToSysStr(eToolchainParam v);
static constexpr string_view eToolchainParamToOptStr(eToolchainParam v);

static constexpr cstring kDefaultToolchainCacheFilePath =
    "cache\\CideToolchainParams.json";
static constexpr cstring kOsAppdataToolchainCacheFilePath =
    "cide\\cache\\CideToolchainParams.json";

/// Volatile host env vars which must be loaded at runtime.
/// Cached as "CideToolchainParams.json"
class ToolchainParams {
  static constexpr cstring kDefaultMsvsInstallerPath =
      "\\Microsoft Visual Studio\\Installer";
  static constexpr cstring kDefaultVswherePath =
      "\\Microsoft Visual "
      "Studio\\Installer\\vswhere.exe";

 public:
  using DataT =
      std::array<string, static_cast<size_t>(eToolchainParam::kCount)>;
  static JsonObj ToJson(const ToolchainParams& obj);
  static ToolchainParams FromJson(const JsonObj& obj);
  constexpr size_t Size() const;
  constexpr const DataT& ViewData() const;
  constexpr const string& ViewParam(eToolchainParam pr) const;
  ApiRes<void> Load();
  ApiRes<void> LoadCached(const string& from);
  ApiRes<void> Save(const string& to);
  ApiRes<void> SaveToDefault();

 private:
  constexpr string& GetParam(eToolchainParam pr);
  constexpr void SetParam(eToolchainParam pr, std::string_view val);
  string GetOsTempPath();
  ApiRes<string> FindVswhere(const string& program_files_dir);
  ApiRes<void> DownloadVswhere();

  DataT data;
  string last_cache_path_{};
};
static_assert(JsonConvertible<ToolchainParams>);

enum class eIdeParam : std::size_t {
  kAppdataDir,
  kBinaryDir,
  kCacheDir,
  kRepoDir,
  kRecentSolutionDirs,
};

static constexpr cstring kIdeParamToStr(eIdeParam value) {
  switch (value) {
    case eIdeParam::kAppdataDir:
      return "kAppdataDir";
    case eIdeParam::kBinaryDir:
      return "kBinaryDir";
    case eIdeParam::kCacheDir:
      return "kCacheDir";
    case eIdeParam::kRepoDir:
      return "kRepoDir";
    case eIdeParam::kRecentSolutionDirs:
      return "kRecentSolutionDirs";
    default:
      return "";
  }
}
static constexpr cstring kIdeParamToSysStr(eIdeParam value) {
  switch (value) {
    case eIdeParam::kAppdataDir:
      return "CIDE_APPDATA_DIR";
    case eIdeParam::kBinaryDir:
      return "CIDE_BINARY_DIR";
    case eIdeParam::kCacheDir:
      return "CIDE_CACHE_DIR";
    case eIdeParam::kRepoDir:
      return "CIDE_REPO_DIR";
    case eIdeParam::kRecentSolutionDirs:
      return "CIDE_RECENT_SOLUTION_DIRS";
    default:
      return "";
  }
}
static constexpr cstring kIdeParamToOptStr(eIdeParam value) {
  switch (value) {
    case eIdeParam::kAppdataDir:
      return "IDE Appdata Directory";
    case eIdeParam::kBinaryDir:
      return "IDE Binary Directory";
    case eIdeParam::kCacheDir:
      return "IDE Cache Directory";
    case eIdeParam::kRepoDir:
      return "IDE Repository Directory";
    case eIdeParam::kRecentSolutionDirs:
      return "IDE Recent Solution Directories";
    default:
      return "";
  }
}
static constexpr eParamInterpType kIdeParamInterpType(eIdeParam value) {
  switch (value) {
    case eIdeParam::kAppdataDir:
    case eIdeParam::kBinaryDir:
    case eIdeParam::kCacheDir:
    case eIdeParam::kRepoDir:
      return eParamInterpType::Line;
    case eIdeParam::kRecentSolutionDirs:
      return eParamInterpType::List;
    default:
      throw cxx::UnknownEnumEntry<eIdeParam>{};
  }
}

using IdeParamContigEnum =
    mta::contig_enum<eIdeParam, eIdeParam::kBinaryDir,
                     eIdeParam::kRecentSolutionDirs, kIdeParamToStr,
                     kIdeParamToSysStr, kIdeParamToOptStr, kIdeParamInterpType>;

struct IdeParam : public IdeParamContigEnum {
  using At = IdeParamContigEnum::enum_t;
  constexpr cstring ToStr() const;
  constexpr cstring ToSysStr() const;
  constexpr cstring ToOptStr() const;
  constexpr eParamInterpType InterpType() const;
};

/// Global Ide Parameters cached on the filesystem as "CideParams.json"
struct IdeParamList {
  static constexpr cstring kDefaultCacheFilePath = "cache\\CideParams.json";
  static constexpr cstring kOsAppdataCacheFilePath =
      "cide\\cache\\CideParams.json";

 public:
  static JsonObj ToJson(const IdeParamList& params) {
    JsonObj obj;
    obj["bin_dir"] = params.bin_dir;
    obj["repo_dir"] = params.repo_dir;
    obj["cache_dir"] = params.cache_dir;
    obj["recent_repo_dirs"] = params.recent_repo_dirs;
    return obj;
  }
  static IdeParamList FromJson(const JsonObj& obj) {
    IdeParamList params;
    params.bin_dir = obj.at("bin_dir");
    params.repo_dir = obj.at("repo_dir");
    params.cache_dir = obj.at("cache_dir");
    for (auto& dir : obj.at("recent_repo_dirs")) {
      params.recent_repo_dirs.push_back(dir);
    }
    return params;
  }

  ApiRes<void> LoadCached(const string& from);
  ApiRes<void> Load();
  ApiRes<void> Save(const string& to);
  ApiRes<void> SaveToDefault();
  bool CacheRepo(const string& sln_folder);

  ToolchainParams host_params_;
  string appdata_dir;
  string bin_dir;
  string repo_dir;
  string cache_dir;
  vector<string> recent_repo_dirs;
};
static_assert(JsonConvertible<IdeParamList>);

/// Cide's document object model, each 'project' is cached and loaded as
/// a cidr (continious integrated developer repository) file.
/// There can only be one "[name].cidr" file per folder structure.
struct RepoParams {
  ApiRes<void> LoadCached(const string& from);
  ApiRes<void> Load();
  ApiRes<void> Save(const string& to);
  ApiRes<void> SaveToDefault();

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

// IDE startup settings
// Represent a .caide file
class IdeSettings {
 public:
  static constexpr inline const wchar_t* kSolutionCacheSettingTag = L"#\n";
  static constexpr inline auto kDefaultRepoPath = L"C:\\candide\\repository\\";
  static stdfs::path GetDefaultBinaryPath();
  static stdfs::path GetDefaultRepoPath();
  static stdfs::path GetDefaultSettingsFilePath();

  const stdfs::path& ViewRepoPath() const;
  const stdfs::path& ViewBinaryPath() const;
  // Load the settings from a file. Return false if the file is invalid.
  bool Load();
  // Save the settings to a file in the current binary path.
  bool Save();
  // Cache a solution folder, if already chached, returns false.
  bool CacheSolution(const stdfs::path& sln_folder);

  IdeSettings() = default;
  IdeSettings(const stdfs::path& bin_path);
  IdeSettings(const stdfs::path& bin_path, const stdfs::path& repo_path);

 private:
  caf::CacheFile ide_cache_{GetDefaultSettingsFilePath()};

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

// Represent a .casln file which holds metadata about a solution.
// Can be saved and reloaded.
struct SolutionSettings {
  stdfs::path GetSettingsFilePath() const;
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

  const string& GetActiveTextBuffer() const {
    return *active_tab_->text_buffer;
  }

 private:
  FileTabVec file_tabs{};
  FileTabVecIter active_tab_{};
};

struct IdeModel {
  IdeParamList ide_params_{};
  RepoParams active_repo_params_{};

  ApiRes<void> Init() {
    // 1. Load settings from the IDE settings file. If the settings file does
    // not exist, create it.
    auto ide_param_load = ide_params_.Load();
    if (!ide_param_load) return ide_param_load;
    auto ide_param_save = ide_params_.SaveToDefault();
    if (!ide_param_save) return ide_param_save;

    return ApiRes<void>{};
  }

  // Creates a new solution.
  cxx::BoolError NewRepo(const string& name) {
    // Check if the path exists. If it already does cancel.
    // Query for a diffrent solution name.
    stdfs::path sln_folder_path = ide_params_.repo_dir;
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
    ide_params_.CacheRepo(sln_folder_path.string());

    // Create the solution settings file.
    // Arg to constructor is the root solution dir.
    RepoParams sln_settings{sln_folder_path};

    // Generate default solution settings.
    sln_settings.Save(name + ".cidr");

    // Set as current active solution.
    active_repo_params_ = sln_settings;

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
    stdfs::path sln_folder_path = ide_params_.repo_dir;
    sln_folder_path += name;

    if (!stdfs::exists(sln_folder_path)) {
      return "Could not open solution: solution folder does not exist.";
    }

    // Load the solution settings file.
    RepoParams sln_settings{sln_folder_path};
    if (!sln_settings.Load()) {
      return "Could not open solution: .casln' solution settings file possibly "
             "corrupted.";
    }

    // Load the solution settings into the active solution.
    active_repo_params_ = sln_settings;

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
/// @} // end of cand_cide_backend

}  // namespace cide::backend

// Implementation
namespace cide::backend {}

///////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language Environment
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