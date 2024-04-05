#pragma once
#include <filesystem>
#include <string>

#include "cide_file_extensions.h"

namespace cide::backend {
namespace fs = std::filesystem;
using std::string;
// Utils
static string LoadFileToStr(const char* const file_path) {
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
static string LoadFileToStr(const string& file_path) {
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

// IDE startup settings, all values must be known at compile time.
// Represent a .caide file
class IdeSettings {
  static constexpr const wchar_t* kSolutionCacheSettingTag = L"#\n";
  inline fs::path GetSettingsFilePath() const {
    return binary_path.native() + L"settings" +
           file_extension::wide::kCaIdeSettings;
  }

  inline void GenerateSettingsFile() const {
    std::ofstream(GetSettingsFilePath())
        // 1. Binary fs::path
        << binary_path.string()
        << '\n'
        // 2. Repository fs::path
        << repository_path.string()
        << '\n'
        // 3. Cached Solutions
        << [this]() -> string {
      std::wstring sln_folders = kSolutionCacheSettingTag;
      for (auto& cached_sln : this->cached_solutions) {
        sln_folders += cached_sln.native() + L'\n';
      }
      sln_folders += kSolutionCacheSettingTag;
      return string(sln_folders.cbegin(), sln_folders.cend());
    }();
  }

 public:
  // Get the binary path of the IDE. Current working directory of the C++
  // context. NOTE: this a copy not reference. current_path() is a dangerous
  // global.
  // In the context of development it will be the cand-ide project folder.
  fs::path binary_path{std::filesystem::current_path().native() +
                       L"\\"};  // fs::path to the IDE executable/binaries.
                                // This is the current working path of
                                // the C++ backend. fs::path to the
                                // cand-ide.exe folder.
  fs::path
      repository_path{};  // fs::path to the folder containing all solutions
                          // Which belong to this IDE. Only these will be
                          // Scanned and populated in the solution list.
                          // Each folder which ccontains a '.casln' file
  // is a solution. Only one .casln is loaded per folder.
  // The rest are ignored.

  std::vector<fs::path> cached_solutions{};

  IdeSettings() = default;
  IdeSettings(const fs::path& bin_path) : binary_path(bin_path) {}
  IdeSettings(const fs::path& bin_path, const fs::path& repo_path)
      : binary_path(bin_path), repository_path(repo_path) {}
  // Load the settings from a file. Return false if the file is invalid.
  bool Load() {
    if (!std::filesystem::exists(GetSettingsFilePath())) {
      // If no settings exist. Load defaults.
      return Default();
    }

    // Load.
    string settings = LoadFileToStr(GetSettingsFilePath().string());
    // Extract the binary_path
    auto c = settings.begin();
    while (c != settings.end()) {
      if (*c = '\n') {
        break;
      }
      c++;
    }
    binary_path = string(settings.begin(), c);
    c++;  // advance past newline.
    string::iterator repo_path_begin = c;
    // Extract the repository path.
    while (c != settings.end()) {
      if (*c = '\n') {
        break;
      }
      c++;
    }
    repository_path = string(settings.begin(), c);
    c++;
    // If there is more, it must be a # indicating start of
    // solution cache.
    if (c != settings.end() && *c == '#') {
      c += 2;
      string::iterator last_cache_begin = c;
      // consume lines until next '#'
      while (c != settings.end() || *c == '#') {
        if (*c = '\n') {
          cached_solutions.push_back(string(last_cache_begin, c));
          c++;
          last_cache_begin = c;
        } else
          c++;
      }
    }
    return true;
  }

  // Save the settings to a file in the current binary path.
  bool Save() const {
    // Overwrite previous file. Delete it.
    fs::path file = GetSettingsFilePath();
    if (std::filesystem::exists(file)) {
      // If its empty dont have to clear.
      if (!file.empty()) {
        std::filesystem::remove(file);
      }
      // Generate the new file.
      GenerateSettingsFile();

    } else {
      GenerateSettingsFile();
    }
  }

  // Create folder default repository path of C://candide/repository/
  // If folder does not exist. Adds repo path.
  // Generate a default settings file and save it in the current binary path.
  bool Default() {
    repository_path = "C:\\candide\\repository\\";
    try {
      std::filesystem::create_directories(repository_path);
      GenerateSettingsFile();
    } catch (...) {
      // Failed to load/create file.
      return false;
    }
    return true;
  };

  // Cache a solution folder, if already chached, returns false.
  bool CacheSolution(const fs::path& sln_folder) {
    if (cached_solutions.end() != std::find(cached_solutions.begin(),
                                            cached_solutions.end(),
                                            sln_folder)) {
      return false;  // Solution is already cached...
    }
    cached_solutions.push_back(sln_folder);
    return true;
  }
};

// Represent a .casln file which holds metadata about the solution.
// Can be saved and reloaded.
struct SolutionSettings {
  inline fs::path GetSettingsFilePath() const;
  bool Default();
  bool Save();
  bool Load();
  // Directory of the solution.[RELATIVE: Relative to the repository path.]
  // This is the working directory of the Solution's filesystem.
  // If you load a file from IDE code. This is the root directory.
  // For this solution's executable.
  fs::path solution_path;

  // Solution file [RELATIVE: Relative to the repository path]
  // .cansln file where solution metadata is stored. Only 1 per solution.
  // It is autoinferred to be the file named '.casln' inside the solution path.
  fs::path solution_file;

  // .camake files describing a build process of this solution.Autogenerated,
  // user-editable. unnamed file '.camake' is automatically included as the
  // first and default build file.
  std::vector<fs::path> build_files;

  // Working files which belong to the solution. Not including .casln / .camake
  // files.
  std::vector<fs::path> working_files;
};

struct IdeInterface {
  IdeSettings settings{};
  SolutionSettings
      active_sln_settings;  // Settings of the currently active solution.
  // Initialize the IDE.
  // 1. Load settings from the IDE settings file. If the settings file does not
  // exist, create it.
  //    Default settings file:
  //
  bool startup() { return settings.Load(); }

  // Create a new solution.
  bool NewSolution(const string& name) {
    // Check if the path exists. If it already does cancel.
    // Query for a diffrent solution name.
    fs::path sln_folder_path = settings.repository_path;
    sln_folder_path += name;

    if (fs::exists(sln_folder_path)) {
      return false;  // Folder is already in use.
    }

    // Create the solution folder.
    if (!fs::create_directories(sln_folder_path)) {
      return false;  // Something went wrong creating dirs...
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
  }
};

}  // namespace cide::backend