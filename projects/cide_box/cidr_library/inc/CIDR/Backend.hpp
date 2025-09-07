///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2025 Anton Yashchenko
// Licensed under the Apache License, Version 2.0(the "License");
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: [CIDR] C& integrated development repository library
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup cidrlib
/// @brief CIDR Backend Implementation.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup cidrlib_backend
/// @{

// clang-format off
#pragma once
#include "Common.hpp"
#include "wpl_core.hpp"

#include "Data/LaunchPathsCache.hpp"
// clang-format on
namespace wpl {

static std::string GetExecutablePath() noexcept {
  try {
    char bin_path[MAX_PATH];
    GetModuleFileNameA(NULL, bin_path, MAX_PATH);
    // Extract directory (remove filename)
    return bin_path;
  } catch (const std::exception& e) {
    throw std::runtime_error("[Win32 go BOOM!]" __FUNCSIG__);
  }
}

}  // namespace wpl
namespace cidr::backend {

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

static JsonObj ToJson(const JsonConvertible auto& obj) { return std::decay_t<decltype(obj)>::ToJson(obj); }

static JsonObj ToJson(const ImVec2& obj) { return JsonObj{{obj.x, obj.y}}; }

static void FromJson(ImVec2& obj, const JsonObj& json) { obj = {json.at(0).get<float>(), json.at(1).get<float>()}; }
static JsonObj ToJson(const ImVec4& obj) { return JsonObj{{obj.x, obj.y, obj.z, obj.w}}; }

static void FromJson(ImVec4& obj, const JsonObj& json) {
  obj = {json.at(0).get<float>(), json.at(1).get<float>(), json.at(2).get<float>(), json.at(3).get<float>()};
}

static auto FromJson(const JsonConvertible auto& obj, const JsonObj& json) -> std::decay_t<decltype(obj)> {
  return std::decay_t<decltype(obj)>::FromJson(json);
}

template <class T>
struct CacheFile2 {
  using PathT = stdfs::path;
  using DataT = T;
  DataT data{};
  PathT path{};

  bool Load() {
    if (!stdfs::exists(path)) return false;
    std::ifstream cache_file(path);
    if (!cache_file.is_open()) return false;
    try {
      data = T::FromJson(nlohmann::json::parse(cache_file));
    } catch (nlohmann::json::parse_error& err) {
      return false;
    }
  };

  bool Save() const {
    std::ofstream cache_file(path);
    if (!cache_file.is_open()) return false;
    cache_file << DataT::ToJson(data).dump();
    cache_file.close();
    return true;
  };
};

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

static constexpr cstring kDefaultUserId{"u0"};
static constexpr cstring kLaunchPathsFileName{"launch-paths.json"};
static constexpr cstring kCidrDirName{"cidr"};

static ApiRes<void> IsDirPathValid(std::string_view path_str) {
  using namespace stdfs;
  try {
    path p = stdfs::u8path(path_str);
    if (p.empty()) return ApiFail{ApiErr{eApiErr::kDirDepNotFound, "Path is empty."}};
#if WIN32
    if (!p.root_name().empty() && (p.root_name().string().find_first_of("<>:\"|?*") != std::string::npos))
      return ApiFail{ApiErr{eApiErr::kDirDepNotFound, "Invalid characters in root name."}};
#endif
    for (const auto& component : p) {
      const std::string comp_str = component.string();
      if (comp_str.empty() && !component.has_root_directory())
        return ApiFail{ApiErr{eApiErr::kDirDepNotFound, "Contains empty path component."}};
      if (comp_str.find_first_of("<>:\"|?*") != std::string::npos)
        return ApiFail{ApiErr{eApiErr::kDirDepNotFound, "Invalid characters in path."}};
    }
    if (!is_directory(p)) return ApiFail{{eApiErr::kDirDepNotFound, "Path is not a directory."}};
  } catch (const std::exception& e) {
    return ApiFail{ApiErr{eApiErr::kDirDepNotFound, e.what()}};
  }
  return ApiRes<void>{};  // Path is valid.
}

/// Returns a unique user id for the current user. If the OS does not provide a
/// unique id, defaults to cidr::backend::kDefaultUserId.
/// @see cidr::backend::kDefaultUserId
static const std::string& GetCurrentUserId() {
  static std::string user_id{""};  // We will be calling this quite often so better to cache the result.
  if (!user_id.empty()) return user_id;
  wpl::ApiRes<std::string> os_user_id = wpl::GetCurrentUserId();
  if (os_user_id)
    user_id = *os_user_id;
  else
    user_id = kDefaultUserId;
  return user_id;
}

struct DoUserPathsExistParseSax : public nlohmann::json_sax<nlohmann::json> {
  std::string_view target_user;
  bool user_exists = false;
  bool in_target_user = false;
  std::string current_key;

  DoUserPathsExistParseSax(std::string_view user) : target_user(user) {}

  bool key(std::string& key) override {
    current_key = key;
    if (key == target_user) {
      in_target_user = true;
      user_exists = true;
      return false;  // If parsing was aborted, user was found.
    }
    return true;
  }

  bool binary(nlohmann::json::binary_t&) override {
    return !user_exists;  // continue unless we found our user
  }
  bool parse_error(std::size_t position, const std::string& last_token, const nlohmann::json::exception& ex) override {
    return !user_exists;
  }
  bool null() override { return !user_exists; }
  bool boolean(bool) override { return !user_exists; }
  bool number_integer(int64_t) override { return !user_exists; }
  bool number_unsigned(uint64_t) override { return !user_exists; }
  bool number_float(double, const std::string&) override { return !user_exists; }
  bool string(std::string&) override { return !user_exists; }
  bool start_object(std::size_t) override { return !user_exists; }
  bool end_object() override {
    if (in_target_user) in_target_user = false;

    return !user_exists;
  }
  bool start_array(std::size_t) override { return !user_exists; }
  bool end_array() override { return !user_exists; }
};

/// Checks if a user exists in the given JSON file. The file must be in the format of a data::LaunchPaths cache object.
static bool DoUserPathsExist(std::string_view user_id, const std::filesystem::path& json_file) {
  try {
    std::ifstream file(json_file);
    if (!file.is_open()) return false;
    DoUserPathsExistParseSax sax(user_id);
    bool parse_result = nlohmann::json::sax_parse(file, &sax);
    return !parse_result || sax.user_exists;
  } catch (...) {
    return false;
  }
}

/// The file must exist inside either or :
///   - "{ExeDir}/cidr/data/launch-paths.json"
///   - "{ProgramData}/cidr/launch-paths.json"
static stdfs::path FindLaunchPathsFile() {
  stdfs::path bin_loc =
      stdfs::u8path(wpl::GetExecutablePath()).parent_path() / kCidrDirName / "data" / kLaunchPathsFileName;
  if (stdfs::exists(bin_loc) && stdfs::is_regular_file(bin_loc) && (stdfs::file_size(bin_loc) != 0)) return bin_loc;

  auto os_program_data_path = wpl::GetOsProgramDataPath();
  if (!os_program_data_path) return {};

  stdfs::path os_loc = stdfs::u8path(*os_program_data_path) / kCidrDirName / kLaunchPathsFileName;
  if (stdfs::exists(os_loc) && stdfs::is_regular_file(os_loc) && (stdfs::file_size(os_loc) != 0))
    return os_loc;
  else
    return {};
}

enum eIsFirstLaunch : int {
  kNotFirstLaunch = 0, 
  kFirstLaunch = 1,    
  kFirstLaunchNewUser = 2, 
};

/// Determines if app is run for the first time. Based on wether a LaunchPaths cache file exits relative to the
/// executable's directory or OS program data directory.
/// Returns 
/// - 0 Not the first launch for this user on this device.
/// - 1 First launch on this device.
/// - 2 First launch on this device for a new user.
/// 
/// @see FindLaunchPathsFile for path location details.
/// @see GetDefaultLaunchPaths
/// @see GetDefaultLaunchPathsPortable
///
/// The working directory file will override the ProgramData file. Call once on startup. If the file does not exist,
/// ui::Launcher menu must display first time installation wizard. 
static eIsFirstLaunch IsFirstLaunch() {
  auto found_path = FindLaunchPathsFile();
  if (found_path.empty()) return eIsFirstLaunch::kFirstLaunch;  // No launch paths file found.
  if (DoUserPathsExist(GetCurrentUserId(), found_path))
    return eIsFirstLaunch::kNotFirstLaunch;
  else
    return eIsFirstLaunch::kFirstLaunchNewUser;  // Launch paths file found, but user paths do not exist.
}


/// Returns the path where CIDR expects the launch paths file to be located AND the default OS launch paths for the
/// given user id and name when using system install paths. When a system path is not available, it will fallback to the
/// portable path.
///
/// User id and name should be retrieved from the system api, if available.
/// Produced paths:
/// - AppDataPath : "C:\ProgramData\cidr" | "${exe_dir}/cidr/data"
/// - AppResourcesPath : "${exe_dir}/cidr/res"
/// - UserDataPath : "C:\Users\${username}\AppData\Local\cidr" | "${exe_dir}/cidr/user/${user_id}"
/// - UserName : "user-${user_id}" | "${username}"
static std::pair<std::string, data::UserLaunchPaths> GetDefaultLaunchPaths(std::string_view user_id,
                                                                           std::string_view username) {
  // A note on stdfs::path and std::string interaction on windows.
  // Source : https://stackoverflow.com/questions/43114174/convert-a-string-to-std-filesystem-path/73686919#73686919
  // "For Windows systems, the implicit construction of a filesystem::path from string is not safe with respect to
  // encoding (despite the fact that it is legal according to the standard). If you have a path as a std::string that
  // contains non-ASCII characters, you have to be absolutely clear about whether it is encoded in the local code-page
  // or UTF-8 (there is no such distinction under Linux). The implicit construction shown in the question will assume
  // encoding in the local code-page, while many 3rd party libraries will assume UTF-8 as the standard (e.g. QT in
  // QString::toStdString() return UTF-8; strings in gRPC/protobuf should be expected being UTF-8 etc.). Therefore, you
  // have to be very careful not to mix these"

  data::UserLaunchPaths ret;
  stdfs::path exe_dir = std::filesystem::u8path(wpl::GetExecutablePath());
  exe_dir = exe_dir.parent_path();
  // Get common OS data usually "C:\ProgramData" on Windows.
  auto data_path_result = wpl::GetOsProgramDataPath();
  // !!If you're wondering why we are calling ->c_str() instead of passing a string directly think again.
  // An awfully irritating glitch, the string constructor of stdfs::file fails to properly initialize the
  // data, and will end up being empty. Why? Its not a temporary...not sure best guess is some encoding issue.
  stdfs::path data_dir = data_path_result ? std::filesystem::u8path(data_path_result->c_str()) : exe_dir;

  // Get user data path, usually "C:\Users\${username}\AppData\Roaming" on Windows.
  auto user_data_result = wpl::GetOsUserDataPath();
  stdfs::path user_data_dir = user_data_result ? std::filesystem::u8path(user_data_result->c_str()) : exe_dir;
  // Set default cache paths...
  ret.AppDataPath() = (data_dir / kCidrDirName / "data").string();
  ret.AppResourcesPath() = (exe_dir / kCidrDirName / "res").string();  // Assume initial resource install path local.
  ret.UserDataPath() = user_data_result ? (user_data_dir / kCidrDirName).string()
                                        : (user_data_dir / kCidrDirName / "user" / user_id).string();
  // Set the user name, if empty use "user-${user_id}".
  if (username.empty())
    ret.UserName() = "user-" + std::string(user_id.data(), user_id.size());
  else
    ret.UserName() = username;
  // Return the path where the these launch paths must be stored.
  // This can only be one of two directories:
  // - exe-dir/cidr/data/launch-paths.json
  // - data-dir/cidr/data/launch-paths.json
  return {(data_dir / kCidrDirName / "data" / kLaunchPathsFileName).string(), ret};
}

/// Returns the path where CIDR expects the launch paths file to be located AND the default portable launch paths for
/// the given user id and name when using portable install paths.
/// @see GetDefaultLaunchPaths
///
/// User id and name should be retrieved from the system api, if available.
/// All paths will be inside a "cidr" directory relative to the executable's directory.
static std::pair<std::string, data::UserLaunchPaths> GetDefaultLaunchPathsPortable(std::string_view user_id,
                                                                                   std::string_view username) {
  data::UserLaunchPaths ret;
  stdfs::path exe_dir = std::filesystem::u8path(wpl::GetExecutablePath());
  exe_dir = exe_dir.parent_path();
  ret.AppDataPath() = (exe_dir / kCidrDirName / "data").string();
  ret.AppResourcesPath() = (exe_dir / kCidrDirName / "res").string();
  ret.UserDataPath() = (exe_dir / kCidrDirName / "user" / user_id).string();
  if (username.empty())
    ret.UserName() = "user-" + std::string(user_id.data(), user_id.size());
  else
    ret.UserName() = username;
  return {(exe_dir / kCidrDirName / kLaunchPathsFileName).string(), ret};
}

static ApiRes<void> ValidateAppDataPath() { return ApiRes<void>{}; };
static ApiRes<void> ValidateAppResourcesPath() { return ApiRes<void>{}; };
static ApiRes<void> ValidateUserDataPath() { return ApiRes<void>{}; };
static ApiRes<void> ValidateUserLaunchPaths(const data::UserLaunchPaths&) { return ApiRes<void>{}; };

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
  kWin32VswherePath,        /// Detects Visual Studio installations.
  kWin32MsvsPath,           /// Used to find VsDevCmd.bat(vs dev cli) and other MSVC
                            /// C++ Toolchain components.
  kWin32MsvsInstallerPath,  /// %ProgramFiles(x86)%\Microsoft Visual
                            /// Studio\Installer (use %ProgramFiles%
                            /// in a 32-bit program prior to Windows 10). This
                            /// is a fixed location that will be maintained.
                            /// Details:
                            /// https://github.com/microsoft/vswhere/wiki/Installing
  kWin32VsdevcmdPath,       /// Path to the MSVS DevCmd.bat.
  kCount
};

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
static constexpr cstring kDefaultToolchainCacheFilePath = "cache\\CideToolchainParams.json";
static constexpr cstring kOsAppdataToolchainCacheFilePath = "cide\\cache\\CideToolchainParams.json";
static constexpr cstring kRepoCacheFileName = "CideRepoParams.json";

/// Volatile host env vars which must be loaded at runtime.
/// Cached as "CideToolchainParams.json"
class ToolchainParams {
  static constexpr cstring kDefaultMsvsInstallerPath = "\\Microsoft Visual Studio\\Installer";
  static constexpr cstring kDefaultVswherePath =
      "\\Microsoft Visual "
      "Studio\\Installer\\vswhere.exe";

 public:
  using DataT = std::array<std::vector<string>, static_cast<size_t>(eToolchainParam::kCount)>;
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

using IdeParamContigEnum = mta::contig_enum<eIdeParam, eIdeParam::kAppdataDir, eIdeParam::kRecentRepoDirs,
                                            kIdeParamToStr, kIdeParamToSysStr, kIdeParamToOptStr, kIdeParamInterpType>;

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
  static constexpr cstring kOsAppdataCacheFilePath = "cide\\cache\\CideParams.json";

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
    obj.build_system_type = static_cast<eRepoBuildSystemType>(json.at("build_system_type"));
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
        std::find_if(file_tabs.begin(), file_tabs.end(), [](const FileTab& tab) { return tab.is_active; });

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

  const string& GetActiveTextBuffer() const { return *active_tab_->text_buffer; }

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

  int curr_user;
  int GetCurrentUserId() const { return curr_user; }

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
              << "Command: " << ide_params_.host_params_.GetFront(kCmakePath) << " " << GetCMakeGenerationStepCommand()
              << std::endl;
    auto gen_res = wpl::RunExe(ide_params_.host_params_.GetFront(kCmakePath), GetCMakeGenerationStepCommand(),
                               active_repo_.solution_path.string());
    if (!gen_res) return ApiFail{{eApiErr::kSubprocessCannotRun, ""}};
    if (gen_res.value().exit_code != EXIT_SUCCESS)
      return ApiFail{{eApiErr::kSubprocessFailedExit, gen_res.value().out.value_or("").c_str()}};
    return ApiRes<void>{};
  }

  ApiRes<void> CallBuildStep() {
    using enum eToolchainParam;
    auto build_res = wpl::RunExe(ide_params_.host_params_.GetFront(kCmakePath), GetCMakeBuildStepCommand(),
                                 active_repo_.solution_path.string());
    if (!build_res) return ApiFail{{eApiErr::kSubprocessCannotRun, ""}};
    if (build_res.value().exit_code != EXIT_SUCCESS)
      return ApiFail{{eApiErr::kSubprocessFailedExit, build_res.value().out.value_or("").c_str()}};
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
    active_repo_.solution_path = ide_params_.host_params_.GetFront(eToolchainParam::kOsTempPath) + "cide\\temp_repo\\";
    if (!stdfs::exists(active_repo_.solution_path)) {
      stdfs::create_directories(active_repo_.solution_path);
    }
    active_repo_.solution_file = active_repo_.solution_path;
    active_repo_.solution_file.append("temp_repo.cidr");

    return ApiRes<void>{};
  }

  expected<void, string> NewRepo(const string& repo_dir, const string& repo_name) {
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
    if (!stdfs::exists(dir)) return std::unexpected<string>{"Could not open repository. Folder does not exist."};
    if (!stdfs::exists(dir / kRepoCacheFileName))
      return std::unexpected<string>{"Could not open repository. CideRepoParams.json not found."};
    RepoParams repo_params{};
    repo_params.solution_path = dir;
    auto load_result = repo_params.Load((dir / kRepoCacheFileName).string());
    if (!load_result)
      return std::unexpected<string>{string("Could not open repository. CideRepoParams.json possibly "
                                            "corrupted. Error detail: ") +
                                     load_result.error().data};
    this->active_repo_ = repo_params;
    return expected<void, string>{};
  }

  void RefreshCurrentRepoRootDir(stdfs::path& old_root_dir) const {
    if (old_root_dir != active_repo_.solution_path) old_root_dir = active_repo_.solution_path;
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
    // if (!sln_settings.Load()) {
    //   return "Could not open solution: .casln' solution settings file possibly "
    //          "corrupted.";
    // }

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

class AppModel {
 public:
  enum class eAppState : int {
    kShutDown = 0,
    kStartUp,
    kInitial,    // Initial state, the AppModel class constructor has been called.
    kFirstInit,  // Expecting cidr-paths.json configuration  .
    kFastLoad,   // Performing runtime startup sequence.
    kLoad,
    kReady,
    kError,
    kOff,
  };

 private:
  eAppState state_{eAppState::kInitial};
  bool is_state_requested{false};
  eAppState requested_state_{eAppState::kInitial};
  int error_code_{EXIT_SUCCESS};
  std::string error_message_{"No error occured."};
  CacheFile2<data::LaunchPaths> launch_paths_{};  // Launch paths for the app, loaded from cidr-paths.json.
 public:
  bool IsOn() const { return !std::to_underlying(state_); }
  bool IsOff() const { return std::to_underlying(state_); }
  int ExitCode() const { return EXIT_SUCCESS; }
  void StartUp() {
    assert(state_ == eAppState::kInitial && "Cannot start up from current state.");
    state_ = eAppState::kStartUp;
  }
  void TurnOn() {};
  void TurnOff() {};
  void ShutDown() {
    // Perform exit procedure and set exit code. At this point the UI is already deallocated.
  }

  void RequestState(eAppState new_state) {
    is_state_requested = true;
    requested_state_ = new_state;
  }

  void StateInit() {
    if (IsFirstLaunch()) {
      state_ = eAppState::kFirstInit;
    } else {
      state_ = eAppState::kFastLoad;
    }
  }

  void StateFirstInit() {

  };

  void StateFastLoad() {
    static constexpr cstring kDefaultLaunchPathsFilePath = "cidr/launch-paths.json";
    static constexpr cstring kCorruptedPathsError = "'launch-paths.json' file corrupted. Path : {}";
    static constexpr cstring kNonExistingFileError =
        "Could not find 'cidr/launch-paths.json' in the executable or OS program data directories.";

    auto portable_path = stdfs::u8path(wpl::GetExecutablePath()) / kDefaultLaunchPathsFilePath;
    if (stdfs::exists(portable_path)) {
      launch_paths_.path = portable_path;
      if (!launch_paths_.Load()) {
        state_ = eAppState::kError;
        error_message_ = std::format(kCorruptedPathsError, portable_path.string());
        return;
      }
      return;
    } else {
      auto os_data_path = wpl::GetOsProgramDataPath();
      if (!os_data_path) {
        state_ = eAppState::kError;
        error_message_ = kNonExistingFileError;
        return;
      }
      stdfs::path sys_path = stdfs::u8path(*os_data_path) / kDefaultLaunchPathsFilePath;

      if (stdfs::exists(sys_path)) {
        launch_paths_.path = sys_path;
        if (!launch_paths_.Load()) {
          state_ = eAppState::kError;
          error_message_ = std::format(kCorruptedPathsError, portable_path.string());
          return;
        }
      } else {
        state_ = eAppState::kError;
        error_message_ = kNonExistingFileError;
        return;
      }
    }

    // If we reach here, we have successfully loaded the launch paths. Get the launch paths for this specific user,
    // validate them before performing the full load.
    auto& user_launch_paths = launch_paths_.data.GetUserLaunchPaths(GetCurrentUserId());
    auto paths_valid = ValidateUserLaunchPaths(user_launch_paths);
    if (!paths_valid) {
      state_ = eAppState::kError;
      error_message_ = std::format("User launch paths are invalid: {}", paths_valid.error().data);
      error_code_ = std::to_underlying(paths_valid.error().num);
      return;
    }

    state_ = eAppState::kLoad;
  }

  void StateLoad() {}
  void StateError() {
    // Allow recovery from error by call of 'RequestState'.
    if (is_state_requested) {
      state_ = requested_state_;
      is_state_requested = false;
    }
  }

  void Step() {}
};

/// @} // end of cand_cide_backend

}  // namespace cidr::backend

/// @} // end of cidrlib_backend

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: [CIDR] C& integrated development repository library
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2025 Anton Yashchenko
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
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
