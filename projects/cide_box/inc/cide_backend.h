///////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the Apache License, Version 2.0(the "License");
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
// clang-format off
#include "cxxx.hpp"
#include "caf.hpp"
#include "wpl_core.hpp"

#include "cide_common.h"
// clang-format on

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
static constexpr cstring kRepoCacheFileName = "CideRepoParams.json";

/// Volatile host env vars which must be loaded at runtime.
/// Cached as "CideToolchainParams.json"
class ToolchainParams {
  static constexpr cstring kDefaultMsvsInstallerPath =
      "\\Microsoft Visual Studio\\Installer";
  static constexpr cstring kDefaultVswherePath =
      "\\Microsoft Visual "
      "Studio\\Installer\\vswhere.exe";

 public:
  using DataT = std::array<std::vector<string>,
                           static_cast<size_t>(eToolchainParam::kCount)>;
  static JsonObj ToJson(const ToolchainParams& obj);
  static ToolchainParams FromJson(const JsonObj& obj);
  constexpr size_t Size() const;
  constexpr const DataT& ViewData() const;
  constexpr const vector<string>& ViewParam(eToolchainParam pr) const;
  constexpr const string& GetFront(eToolchainParam pr) const;
  constexpr string& GetFrontParam(eToolchainParam pr);
  ApiRes<void> Load();
  ApiRes<void> LoadCached(const string& from);
  ApiRes<void> Save(const string& to);
  ApiRes<void> SaveToDefault();

 private:
  constexpr vector<string>& GetParam(eToolchainParam pr);
  constexpr void SetParam(eToolchainParam pr, std::string_view val);
  constexpr void SetParam(eToolchainParam pr, const vector<string>& val);
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
  kRecentRepoDirs,
};

enum class eRepoBuildSystemType : std::size_t {
  kManual,
  kCmake,
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
    case eIdeParam::kRecentRepoDirs:
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
    case eIdeParam::kRecentRepoDirs:
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
    case eIdeParam::kRecentRepoDirs:
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
    case eIdeParam::kRecentRepoDirs:
      return eParamInterpType::List;
    default:
      throw cxx::UnknownEnumEntry<eIdeParam>{};
  }
}

using IdeParamContigEnum =
    mta::contig_enum<eIdeParam, eIdeParam::kAppdataDir,
                     eIdeParam::kRecentRepoDirs, kIdeParamToStr,
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
    using enum eIdeParam;
    JsonObj obj;
    obj[kIdeParamToSysStr(kAppdataDir)] = params.appdata_dir;
    obj[kIdeParamToSysStr(kBinaryDir)] = params.bin_dir;
    obj[kIdeParamToSysStr(kCacheDir)] = params.repo_dir;
    obj[kIdeParamToSysStr(kRepoDir)] = params.cache_dir;
    obj[kIdeParamToSysStr(kRecentRepoDirs)] = params.recent_repo_dirs;
    return obj;
  }

  static IdeParamList FromJson(const JsonObj& obj) {
    using enum eIdeParam;
    IdeParamList params;
    params.appdata_dir = obj.at(kIdeParamToSysStr(kAppdataDir));
    params.bin_dir = obj.at(kIdeParamToSysStr(kBinaryDir));
    params.repo_dir = obj.at(kIdeParamToSysStr(kRepoDir));
    params.cache_dir = obj.at(kIdeParamToSysStr(kCacheDir));
    for (auto& dir : obj.at(kIdeParamToSysStr(kRecentRepoDirs))) {
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
  static JsonObj ToJson(const RepoParams& obj) {
    JsonObj json;
    json["build_system_type"] = static_cast<int>(obj.build_system_type);
    json["solution_path"] = obj.solution_path.string();
    json["cache_path"] = obj.cache_path.string();
    json["solution_file"] = obj.solution_file.string();
    json["build_dir"] = obj.build_dir.string();
    json["build_files"] = obj.build_files;
    json["working_files"] = obj.working_files;
    return json;
  }

  static RepoParams FromJson(const JsonObj& json) {
    RepoParams obj;
    obj.build_system_type =
        static_cast<eRepoBuildSystemType>(json.at("build_system_type"));
    obj.solution_path = json.at("solution_path").get<string>();
    obj.cache_path = json.at("cache_path").get<string>();
    obj.solution_file = json.at("solution_file").get<string>();
    obj.build_dir = json.at("build_dir").get<string>();
    obj.build_files = json.at("build_files");
    obj.working_files = json.at("working_files");
    return obj;
  }

  ApiRes<void> Load(const string& from);
  ApiRes<void> Save(const string& to);
  eRepoBuildSystemType build_system_type{eRepoBuildSystemType::kManual};
  stdfs::path solution_path;

  // Directory of the solution.[RELATIVE: Relative to the repository path.]
  // This is the working directory of the Solution's filesystem.
  // If you load a file from IDE code. This is the root directory.
  // For this solution's executable.
  stdfs::path cache_path;

  // Solution file [RELATIVE: Relative to the repository path]
  // .cansln file where solution metadata is stored. Only 1 per solution.
  // It is auto-inferred to be the file named '.casln' inside the solution path.
  stdfs::path solution_file;

  // Build dir relative to the solution path.
  stdfs::path build_dir{"out"};

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
  RepoParams active_repo_{};
  ApiErr last_error_{};
  bool is_gen_step_complete{false};
  bool is_build_step_complete{false};
  bool is_run_step_complete{false};

  string GetCMakeGenerationStepCommand() const {
    using enum eToolchainParam;
    return " . -B" + active_repo_.build_dir.string();
  }
  string GetCMakeBuildStepCommand() const {
    using enum eToolchainParam;
    return " --build " + active_repo_.build_dir.string();
  }

  ApiRes<void> CallGenerationStep() {
    using enum eToolchainParam;
    std::cout << "Running CMake Generation Step\n"
              << "Working Dir: " << active_repo_.solution_path.string()
              << "Command: " << ide_params_.host_params_.GetFront(kCmakePath)
              << " " << GetCMakeGenerationStepCommand() << std::endl;
    auto gen_res = wpl::RunExe(ide_params_.host_params_.GetFront(kCmakePath),
                               GetCMakeGenerationStepCommand(),
                               active_repo_.solution_path.string());
    if (!gen_res) return ApiFail{{eApiErr::kSubprocessCannotRun, ""}};
    if (gen_res.value().exit_code != EXIT_SUCCESS)
      return ApiFail{{eApiErr::kSubprocessFailedExit,
                      gen_res.value().out.value_or("").c_str()}};
    return ApiRes<void>{};
  }

  ApiRes<void> CallBuildStep() {
    using enum eToolchainParam;
    auto build_res = wpl::RunExe(ide_params_.host_params_.GetFront(kCmakePath),
                                 GetCMakeBuildStepCommand(),
                                 active_repo_.solution_path.string());
    if (!build_res) return ApiFail{{eApiErr::kSubprocessCannotRun, ""}};
    if (build_res.value().exit_code != EXIT_SUCCESS)
      return ApiFail{{eApiErr::kSubprocessFailedExit,
                      build_res.value().out.value_or("").c_str()}};
    return ApiRes<void>{};
  }

  ApiRes<void> CallRunStep() {
    // We can run a built target by checking CMakeFiles/TargetDirectories.txt in
    // the cmake generated output. Given a target name extract by finding stem
    // named [target].dir in the list of directories.

    // Open CMakeFiles/TargetDirectories.txt in the bin dir.

    // Find the line with the target name.

    // Extract the directory name.

    // Run the executable in the directory with the target name.

    return ApiRes<void>{};
  }

  void ExtCallGenerationStep() {
    auto gen_res = CallGenerationStep();
    if (!gen_res) {
      last_error_ = gen_res.error();
      is_gen_step_complete = false;
    } else {
      is_gen_step_complete = true;
    }
  }

  void ExtCallBuildStep() {
    if (!is_gen_step_complete) ExtCallGenerationStep();

    auto build_res = CallBuildStep();
    if (!build_res) {
      last_error_ = build_res.error();
      is_build_step_complete = false;
    } else {
      is_build_step_complete = true;
    }
  }

  void ExtCallRunStep() {
    auto run_res = CallRunStep();
    if (!run_res) {
      last_error_ = run_res.error();
      is_run_step_complete = false;
    } else {
      is_run_step_complete = true;
    }
  }

  ApiRes<void> Init() {
    // 1. Load settings from the IDE settings file. If the settings file does
    // not exist, create it.
    auto ide_param_load = ide_params_.Load();
    if (!ide_param_load) return ide_param_load;
    auto ide_param_save = ide_params_.SaveToDefault();
    if (!ide_param_save) return ide_param_save;
    ide_params_.host_params_.SaveToDefault();

    // 2. Create a temporary empty repo in the temp folder.
    active_repo_.solution_path =
        ide_params_.host_params_.GetFront(eToolchainParam::kOsTempPath) +
        "cide\\temp_repo\\";
    if (!stdfs::exists(active_repo_.solution_path)) {
      stdfs::create_directories(active_repo_.solution_path);
    }
    active_repo_.solution_file = active_repo_.solution_path;
    active_repo_.solution_file.append("temp_repo.cidr");

    return ApiRes<void>{};
  }

  expected<void, string> NewRepo(const string& repo_dir,
                                 const string& repo_name) {
    stdfs::path repo_path = repo_dir + "\\" + repo_name;

    if (stdfs::exists(repo_path))
      return std::unexpected<string>{
          "Could not create solution dirs: Solution folder is already in "
          "use."};

    if (!stdfs::create_directories(repo_path))
      return std::unexpected<string>{
          "Could not create solution dirs: OS write access may not be "
          "available."};

    ide_params_.CacheRepo(repo_path.string());
    RepoParams sln_settings{};
    sln_settings.solution_path = repo_path;  // Root repo dir.
    // sln_settings.Save((repo_path / ".cidr").string());  // Gens default.
    sln_settings.working_files.push_back(repo_path / kRepoCacheFileName);
    active_repo_ = sln_settings;
    return expected<void, string>{};
  }

  expected<void, string> OpenRepo(const stdfs::path& dir) {
    if (!stdfs::exists(dir))
      return std::unexpected<string>{
          "Could not open repository. Folder does not exist."};
    if (!stdfs::exists(dir / kRepoCacheFileName))
      return std::unexpected<string>{
          "Could not open repository. CideRepoParams.json not found."};
    RepoParams repo_params{};
    repo_params.solution_path = dir;
    auto load_result = repo_params.Load((dir / kRepoCacheFileName).string());
    if (!load_result)
      return std::unexpected<string>{
          string("Could not open repository. CideRepoParams.json possibly "
                 "corrupted. Error detail: ") +
          load_result.error().data};
    this->active_repo_ = repo_params;
    return expected<void, string>{};
  }

  void RefreshCurrentRepoRootDir(stdfs::path& old_root_dir) const {
    if (old_root_dir != active_repo_.solution_path)
      old_root_dir = active_repo_.solution_path;
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
    RepoParams sln_settings{};
    sln_settings.solution_path = sln_folder_path;
    //if (!sln_settings.Load()) {
    //  return "Could not open solution: .casln' solution settings file possibly "
    //         "corrupted.";
    //}

    // Load the solution settings into the active solution.
    active_repo_ = sln_settings;

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

///////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language Environment
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