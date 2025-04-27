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
/// @brief cide_ide_settings.hpp source
///////////////////////////////////////////////////////////////////////////////

#include "cide_backend.h"
// Standard
#include "caf.hpp"
#include "cxxx.hpp"
// CIDE Common Defs
#include "cide_common.h"
#include "wpl_core.hpp"

namespace cide::backend {
/// @addtogroup cand_cide_backend
/// @{

static constexpr ApiFail MakeApiFail(eApiErr err, cstring data) {
  return ApiFail{{err, data}};
}

static constexpr ApiFail MakeApiFail(ApiErr fail) { return ApiFail{fail}; }

/// Returns the system environment variable name for the given host variable.
static constexpr string_view eToolchainParamToSysStr(eToolchainParam v) {
  switch (v) {
    case eToolchainParam::kCmakePath:
      return "_CandLang_CmakePath";
    case eToolchainParam::kCppCompilerPath:
      return "_CandLang_CppCompilerPath";
    case eToolchainParam::kCppLinkerPath:
      return "_CandLang_CppLinkerPath";
    case eToolchainParam::kOsTempPath:
      return "_CandLang_OsTempPath";
    case eToolchainParam::kOsProgramDataPath:
      return "_CandLang_OsProgramDataPath";
    case eToolchainParam::kOsCliPath:
      return "_CandLang_OsCliPath";
    case eToolchainParam::kWin32ProgramFilesDir:
      return "_CandLang_Win32ProgramFilesDir";
    case eToolchainParam::kWin32VswherePath:
      return "_CandLang_Win32VswherePath";
    case eToolchainParam::kWin32MsvsPath:
      return "_CandLang_Win32MsvsPath";
    case eToolchainParam::kWin32MsvsInstallerPath:
      return "_CandLang_Win32MsvsInstallerPath";
    case eToolchainParam::kWin32VsdevcmdPath:
      return "_CandLang_Win32VsdevcmdPath";
    default:
      return "_CandLang_UNKNOWN";
  }
}

/// Returns the pretty name of the host variable.
static constexpr string_view eToolchainParamToOptStr(eToolchainParam v) {
  switch (v) {
    case eToolchainParam::kCmakePath:
      return "CMake Path";
    case eToolchainParam::kCppCompilerPath:
      return "C++ Compiler Path";
    case eToolchainParam::kCppLinkerPath:
      return "C++ Linker Path";
    case eToolchainParam::kOsTempPath:
      return "OS Temp Path";
    case eToolchainParam::kOsProgramDataPath:
      return "OS Local Appdata Path";
    case eToolchainParam::kOsCliPath:
      return "OS CLI Path";
    case eToolchainParam::kWin32ProgramFilesDir:
      return "Win32 Program Files Dir";
    case eToolchainParam::kWin32VswherePath:
      return "Win32 Vswhere Path";
    case eToolchainParam::kWin32MsvsPath:
      return "Win32 MSVS Path";
    case eToolchainParam::kWin32MsvsInstallerPath:
      return "Win32 MSVS Installer Path";
    case eToolchainParam::kWin32VsdevcmdPath:
      return "Win32 VsDevCmd Path";
    default:
      return "UNKNOWN";
  }
}

constexpr const char* IdeParam::ToStr() const {
  return IdeParamContigEnum::assoc<0>(val);
}

constexpr const char* IdeParam::ToSysStr() const {
  return IdeParamContigEnum::assoc<1>(val);
}

constexpr const char* IdeParam::ToOptStr() const {
  return IdeParamContigEnum::assoc<2>(val);
}

constexpr eParamInterpType IdeParam::InterpType() const {
  return IdeParamContigEnum::assoc<3>(val);
}

JsonObj ToolchainParams::ToJson(const ToolchainParams& obj) {
  JsonObj j;
  for (size_t i = 0; i < obj.Size(); i++) {
    j[eToolchainParamToSysStr(static_cast<eToolchainParam>(i))] =
        obj.ViewParam(static_cast<eToolchainParam>(i));
  }
  return j;
};

ToolchainParams ToolchainParams::FromJson(const JsonObj& obj) {
  ToolchainParams hp;
  for (size_t i = 0; i < hp.Size(); i++) {
    vector<string> loaded_params;
    const auto& json_arr =
        obj.at(eToolchainParamToSysStr(static_cast<eToolchainParam>(i)));
    for (const auto& el : json_arr) {
      loaded_params.push_back(el.get<string>());
    }
    hp.SetParam(static_cast<eToolchainParam>(i), loaded_params);
  }
  return hp;
};

constexpr size_t ToolchainParams::Size() const {
  return static_cast<size_t>(eToolchainParam::kCount);
}

constexpr const ToolchainParams::DataT& ToolchainParams::ViewData() const {
  return data;
}

constexpr const vector<string>& ToolchainParams::ViewParam(
    eToolchainParam pr) const {
  return data.at(static_cast<size_t>(pr));
}

ApiRes<void> ToolchainParams::LoadCached(const string& from) {
  if (!stdfs::exists(from))
    return MakeApiFail(eApiErr::kFileNotFound, from.c_str());
  std::ifstream cache_file(from);
  if (!cache_file.is_open())
    return MakeApiFail(eApiErr::kFileNotFound, from.c_str());
  try {
    *this = FromJson(jsonlib::json::parse(cache_file));
  } catch (jsonlib::json::parse_error& e) {
    return MakeApiFail(eApiErr::kJsonParseError, e.what());
  }
}

ApiRes<void> ToolchainParams::Load() {
  using enum eApiErr;

  if (LoadCached(kDefaultToolchainCacheFilePath)) return ApiRes<void>{};

  auto os_temp_path = wpl::GetOsTempPath();
  if (!os_temp_path) return MakeApiFail(eApiErr::kDirDepNotFound, "Temp");
  SetParam(eToolchainParam::kOsTempPath, os_temp_path.value().c_str());

  auto os_program_data_path = wpl::GetOsProgramDataPath();
  if (!os_program_data_path)
    return MakeApiFail(eApiErr::kDirDepNotFound, "ProgramData");
  SetParam(eToolchainParam::kOsProgramDataPath,
           os_program_data_path.value().c_str());

  // Check for a cached file in local appdata
  string cache_file_path = os_program_data_path.value().c_str();
  cache_file_path += "\\";
  cache_file_path += +kOsAppdataToolchainCacheFilePath;
  if (LoadCached(cache_file_path)) return ApiRes<void>{};

  wpl::ApiRes<wpl::RunExeResult> cmd_res = wpl::RunExe("where", "cmd");
  if (!cmd_res) return MakeApiFail(kSubprocessCannotRun, "where");
  string cmd_path = cmd_res.value().out.value();
  wpl::PopCmdEndline(cmd_path);
  SetParam(eToolchainParam::kOsCliPath, cmd_path);

  wpl::ApiRes<wpl::RunExeResult> program_files_res =
      wpl::RunExe(cmd_path, "/c echo %ProgramFiles(x86)%");
  if (!program_files_res) return MakeApiFail(kSubprocessCannotRun, "echo");
  string program_files_dir = program_files_res.value().out.value();
  wpl::PopCmdEndline(program_files_dir);
  SetParam(eToolchainParam::kWin32ProgramFilesDir, program_files_dir);

  string expected_msvs_install_dir =
      program_files_dir + kDefaultMsvsInstallerPath;
  if (!stdfs::exists(expected_msvs_install_dir))
    return MakeApiFail(kDirDepNotFound, "msvs installer path");
  SetParam(eToolchainParam::kWin32MsvsInstallerPath, expected_msvs_install_dir);

  ApiRes<string> vswhere_res = FindVswhere(program_files_dir);
  if (!vswhere_res) {
    // Attempt to recover by downloading.
    if (vswhere_res.error().num == kExeDepNotFound) {
      ApiRes<void> vswhere_dl_res = DownloadVswhere();
      if (!vswhere_dl_res)  // Download failed.
        return vswhere_dl_res;
      else {  // Try again
        vswhere_res = FindVswhere(program_files_dir);
        if (!vswhere_res) return MakeApiFail(vswhere_res.error());
      }
    } else
      return MakeApiFail(vswhere_res.error());
  }
  SetParam(eToolchainParam::kWin32VswherePath, vswhere_res.value());

  // Find location of !latest! visual studio install folder
  // Details: https://github.com/microsoft/vswhere/wiki/Find-VC
  wpl::ApiRes<wpl::RunExeResult> msvs_res =
      wpl::RunExe(GetFrontParam(eToolchainParam::kWin32VswherePath),
                  "-latest -nocolor -utf8 -property installationPath");

  if (!msvs_res) return MakeApiFail(kSubprocessCannotRun, "vswhere");
  if (msvs_res.value().exit_code != EXIT_SUCCESS)
    return MakeApiFail(kExeDepNotFound, "msvs");
  wpl::PopCmdEndline(msvs_res.value().out.value());
  SetParam(eToolchainParam::kWin32MsvsPath, msvs_res.value().out.value());

  string expected_vsdevcmd_path =
      GetFrontParam(eToolchainParam::kWin32MsvsPath) +
      "\\Common7\\Tools\\VsDevCmd.bat";
  if (!stdfs::exists(expected_vsdevcmd_path))
    return MakeApiFail(kExeDepNotFound, "VsDevCmd.bat");
  SetParam(eToolchainParam::kWin32VsdevcmdPath, expected_vsdevcmd_path);

  // Get win32 dirs from vsdevcmd session
  wpl::CmdShell shell{};
  shell.SetInitDir(GetFrontParam(eToolchainParam::kWin32MsvsPath));
  if (!shell.Create()) return MakeApiFail(kSubprocessCannotRun, "cmd");

  if (!shell.SendMsg("\"" + expected_vsdevcmd_path + "\""))
    return MakeApiFail(kSubprocessCannotRun, "VsDevCmd.bat");

  wpl::ApiRes<string> find_cl = shell.SendMsg("where cl");
  if (!find_cl) MakeApiFail(kSubprocessCannotRun, "where cl");
  SetParam(eToolchainParam::kCppCompilerPath, find_cl.value());

  wpl::ApiRes<string> find_link = shell.SendMsg("where link");
  if (!find_link) MakeApiFail(kSubprocessCannotRun, "where link");
  SetParam(eToolchainParam::kCppLinkerPath, find_link.value());

  // cmake may return multiple values, use the builtin msvc cmake
  wpl::ApiRes<string> find_cmake = shell.SendMsg("where cmake");
  if (!find_cmake) MakeApiFail(kSubprocessCannotRun, "where cmake");
  SetParam(eToolchainParam::kCmakePath,
           find_cmake.value());  // for now assume last one is msvc's cmake

  shell.Close();

  return ApiRes<void>{};
};

ApiRes<void> ToolchainParams::Save(const string& to) {
  using enum eApiErr;
  std::ofstream cache_file(to);
  if (!cache_file.is_open()) return MakeApiFail(kFileNotFound, to.c_str());
  cache_file << ToJson(*this).dump(2);
  cache_file.close();
  last_cache_path_ = to;
  return ApiRes<void>{};
}

ApiRes<void> ToolchainParams::SaveToDefault() {
  if (!GetParam(eToolchainParam::kOsProgramDataPath).empty())
    return Save(GetParam(eToolchainParam::kOsProgramDataPath).front() + "\\" +
                kOsAppdataToolchainCacheFilePath);
  return Save(kDefaultToolchainCacheFilePath);
}

constexpr vector<string>& ToolchainParams::GetParam(eToolchainParam pr) {
  return data.at(static_cast<size_t>(pr));
}

constexpr string& ToolchainParams::GetFrontParam(eToolchainParam pr) {
  return data.at(static_cast<size_t>(pr)).front();
}

constexpr const string& ToolchainParams::GetFront(
    eToolchainParam pr) const {
  return data.at(static_cast<size_t>(pr)).front();
}

constexpr void ToolchainParams::SetParam(eToolchainParam pr,
                                         std::string_view val) {
  data.at(static_cast<size_t>(pr)) = wpl::GetCmdLines(val.data());
}

constexpr void ToolchainParams::SetParam(eToolchainParam pr,
                                         const vector<string>& val) {
  data.at(static_cast<size_t>(pr)) = val;
}

/// Finds location of vswhere on win32 system.
///
/// Possible Errors:
///   - eApiErr::kSubprocessCannotRun, "where"
///       Could not launch win32 system command "where"
///   - eApiErr::kExeDepNotFound, "vswhere"
///       Returns when vswhere could not be found.
/// Default path: vswhere is included with the installer as of Visual Studio
///  2017 version 15.2 and later, and can be found at the following
/// location:
///  %ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe
/// Alternative: Use "winget install vswhere --nointeractivity"
ApiRes<string> ToolchainParams::FindVswhere(const string& program_files_dir) {
  // Try finding using windows where
  auto run_res = wpl::RunExe("where", "vswhere");
  if (!run_res) return ApiFail{{eApiErr::kSubprocessCannotRun, "where"}};
  if (run_res.value().exit_code == EXIT_SUCCESS) {
    wpl::PopCmdEndline(run_res.value().out.value());
    return run_res.value().out.value();
  }

  // Try finding in the default directory
  string expected_dir = program_files_dir + kDefaultVswherePath;
  if (stdfs::exists(expected_dir)) return expected_dir;

  return ApiFail{{eApiErr::kExeDepNotFound, "vswhere"}};
}

/// Downloads and installs vswhere on win32 system using winget.
///
/// @errors
///   - eFatalErr::kSubprocessCannotRun, "winget"
///       Could not launch win32 system command "winget".
///   - eFatalErr::kSubprocessFailedExit,"winget install vswhere"
///       Possible install fail, winget returned a non-zero value on exit.
ApiRes<void> ToolchainParams::DownloadVswhere() {
  auto run_res = wpl::RunExe("winget", "install vswhere --nointeractivity");
  if (!run_res) return ApiFail{{eApiErr::kSubprocessCannotRun, "winget"}};
  if (run_res.value().exit_code != 0)
    return ApiFail{{eApiErr::kSubprocessFailedExit, "winget install vswhere"}};
}

ApiRes<void> IdeParamList::LoadCached(const string& from) {
  if (!stdfs::exists(from))
    return MakeApiFail(eApiErr::kFileNotFound, from.c_str());
  std::ifstream cache_file(from);
  if (!cache_file.is_open())
    return MakeApiFail(eApiErr::kFileNotFound, from.c_str());
  try {
    auto jsonrepr = FromJson(jsonlib::json::parse(cache_file));
    appdata_dir = jsonrepr.appdata_dir;
    bin_dir = jsonrepr.bin_dir;
    repo_dir = jsonrepr.repo_dir;
    cache_dir = jsonrepr.cache_dir;
    recent_repo_dirs = jsonrepr.recent_repo_dirs;
   
  } catch (jsonlib::json::parse_error& e) {
    return MakeApiFail(eApiErr::kJsonParseError, e.what());
  }
}

ApiRes<void> IdeParamList::Load() {
  auto tc_params = host_params_.Load();
  if (!tc_params) return tc_params;

  if (host_params_.ViewParam(eToolchainParam::kOsProgramDataPath).empty()) {
    if (stdfs::exists(kDefaultCacheFilePath)) {
      return LoadCached(kDefaultCacheFilePath);
    }

    appdata_dir = stdfs::current_path().string();
    if (!stdfs::exists(appdata_dir)) stdfs::create_directory(appdata_dir);
    bin_dir = appdata_dir + "\\bin";
    if (!stdfs::exists(bin_dir)) stdfs::create_directory(bin_dir);
    repo_dir = appdata_dir + "\\repo";
    if (!stdfs::exists(repo_dir)) stdfs::create_directory(repo_dir);
    cache_dir = appdata_dir + "\\cache";
    if (!stdfs::exists(cache_dir)) stdfs::create_directory(cache_dir);
    return ApiRes<void>{};
  }
  const auto& win32_appdata_dir =
      host_params_.GetFront(eToolchainParam::kOsProgramDataPath);
  appdata_dir = win32_appdata_dir + "\\cide";

  if (stdfs::exists(win32_appdata_dir + "\\" + kOsAppdataCacheFilePath)) {
    return LoadCached(win32_appdata_dir + "\\" + kOsAppdataCacheFilePath);
  }

  if (!stdfs::exists(appdata_dir)) stdfs::create_directory(appdata_dir);
  bin_dir = appdata_dir + "\\bin";
  if (!stdfs::exists(bin_dir)) stdfs::create_directory(bin_dir);
  repo_dir = appdata_dir + "\\repo";
  if (!stdfs::exists(repo_dir)) stdfs::create_directory(repo_dir);
  cache_dir = appdata_dir + "\\cache";
  if (!stdfs::exists(cache_dir)) stdfs::create_directory(cache_dir);

  return ApiRes<void>{};
}

bool IdeParamList::CacheRepo(const string& sln_folder) {
  if (recent_repo_dirs.end() !=
      std::find(recent_repo_dirs.begin(), recent_repo_dirs.end(), sln_folder)) {
    return false;  // Solution is already cached...
  }
  recent_repo_dirs.push_back(sln_folder);
  return true;
}

ApiRes<void> IdeParamList::Save(const string& to) {
  using enum eApiErr;
  std::ofstream cache_file(to);
  if (!cache_file.is_open()) return MakeApiFail(kFileNotFound, to.c_str());
  cache_file << ToJson(*this).dump(2);
  cache_file.close();
  return ApiRes<void>{};
}

ApiRes<void> IdeParamList::SaveToDefault() {
  if (!host_params_.ViewParam(eToolchainParam::kOsProgramDataPath).empty())
    return Save(
        host_params_.GetFront(eToolchainParam::kOsProgramDataPath) +
        "\\" + kOsAppdataCacheFilePath);
  return Save(kDefaultCacheFilePath);
  return ApiRes<void>();
}

  ApiRes<void> RepoParams::Load(const string& from){
  if (!stdfs::exists(from))
    return MakeApiFail(eApiErr::kFileNotFound, from.c_str());
  std::ifstream cache_file(from);
  if (!cache_file.is_open())
    return MakeApiFail(eApiErr::kFileNotFound, from.c_str());
  try {
    *this = FromJson(jsonlib::json::parse(cache_file));
  } catch (jsonlib::json::parse_error& e) {
    return MakeApiFail(eApiErr::kJsonParseError, e.what());
  }
  };
  ApiRes<void> RepoParams::Save(const string& to){ using enum eApiErr;
    std::ofstream cache_file(to);
    if (!cache_file.is_open()) return MakeApiFail(kFileNotFound, to.c_str());
    cache_file << ToJson(*this).dump(2);
    cache_file.close();
    return ApiRes<void>{};
  };

/// @} // end of cand_cide_backend

// inline stdfs::path IdeSettings::GetDefaultBinaryPath() {
//   return stdfs::path{std::filesystem::current_path().native() + L"\\"};
// }
//
// inline stdfs::path IdeSettings::GetDefaultRepoPath() {
//   return stdfs::path(L"C:\\candide\\repository\\");
// }
//
// inline stdfs::path IdeSettings::GetDefaultSettingsFilePath() {
//   return GetDefaultBinaryPath().append(file_extension::wide::kCaIdeSettings);
// }
//
// inline const stdfs::path& IdeSettings::ViewRepoPath() const {
//   return repository_path;
// }
//
// inline const stdfs::path& IdeSettings::ViewBinaryPath() const {
//   return binary_path;
// }
//
//// Load the settings from a file. Return false if the file is invalid.
// inline bool IdeSettings::Load() {
//   // If file does not exist create a new default settings file.
//   if (not ide_cache_.DoesFileExist()) {
//     ide_cache_.RefreshCacheFile();
//     ide_cache_.PushLinesToLiveCache("binary_path",
//                                     {GetDefaultBinaryPath().string()});
//     ide_cache_.PushLinesToLiveCache("repository_path",
//                                     {GetDefaultRepoPath().string()});
//     ide_cache_.SaveLiveCacheToFile();
//   }
//
//   ide_cache_.LoadLinesToLiveCache();
//
//   if (ide_cache_.ContainsTag("binary_path")) {
//     binary_path = ide_cache_.GetLineByTagAndIndex("binary_path", 0);
//   }
//
//   if (ide_cache_.ContainsTag("repository_path")) {
//     repository_path = ide_cache_.GetLineByTagAndIndex("repository_path", 0);
//   }
//
//   if (ide_cache_.ContainsTag("cached_solutions")) {
//     cached_solutions = ide_cache_.GetLinesByTagAsPath("cached_solutions");
//   }
//
//   ide_cache_.ClearLiveCache();
//
//   return true;
// }
//
//// Save the settings to a file in the current binary path.
// inline bool IdeSettings::Save() {
//   ide_cache_.RefreshCacheFile();
//   ide_cache_.PushLinesToLiveCache("binary_path", {binary_path.string()});
//   ide_cache_.PushLinesToLiveCache("repository_path",
//                                   {repository_path.string()});
//   vector<std::string> cached_sln_lines;
//   for (auto& sln : cached_solutions) {
//     cached_sln_lines.push_back(sln.string());
//   }
//   ide_cache_.PushLinesToLiveCache("cached_solutions", cached_sln_lines);
//   ide_cache_.SaveLiveCacheToFile();
//   return true;
//   //// Overwrite previous file. Delete it.
//   // stdfs::path file = GetSettingsFilePath();
//   // if (std::filesystem::exists(file)) {
//   //   // If its empty dont have to clear.
//   //   if (!file.empty()) {
//   //     std::filesystem::remove(file);
//   //   }
//   //   // Generate the new file.
//   //   GenerateSettingsFile();
//
//   //} else {
//   //  GenerateSettingsFile();
//   //}
// }
//
//// Cache a solution folder, if already chached, returns false.
// inline bool IdeSettings::CacheSolution(const stdfs::path& sln_folder) {
//   if (cached_solutions.end() !=
//       std::find(cached_solutions.begin(), cached_solutions.end(),
//       sln_folder)) {
//     return false;  // Solution is already cached...
//   }
//   cached_solutions.push_back(sln_folder);
//   return true;
// }
//
// inline IdeSettings::IdeSettings(const stdfs::path& bin_path)
//     : binary_path(bin_path) {}
//
// inline IdeSettings::IdeSettings(const stdfs::path& bin_path,
//                                 const stdfs::path& repo_path)
//     : binary_path(bin_path), repository_path(repo_path) {}

}  // namespace cide::backend

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
