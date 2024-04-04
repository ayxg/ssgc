#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <codecvt>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <locale>
#include <unordered_map>

// For static strcpy_s
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <set>

#include "../submodules/extended-cpp-standard/cppsextended.h"
#include "imgui-SFML.h"
#include "imgui.h"
#include "imgui_interface.h"
#include "imgui_stdlib.h"

namespace stdfs = std::filesystem;

namespace gui = ImGui;
namespace cide {
namespace stdfs = std::filesystem;
// Typedefs
using String = std::string;
using WString = std::wstring;
using Path = std::filesystem::path;
using DirectoryEntry = std::filesystem::directory_entry;
using DirectoryIter = std::filesystem::directory_iterator;
// Constants.
namespace file_extension {
constexpr std::string_view kCaIdeSettings = ".caide";
constexpr std::string_view kCaSolution = ".casln";
constexpr std::string_view kCaBuild = ".camake";
constexpr std::string_view kCaSource = ".cand";
constexpr std::string_view kCaHeader = ".candi";
constexpr std::string_view kCailCode = ".cail";
constexpr std::string_view kCppSource = ".cpp";
constexpr std::string_view kCppHeader = ".h";
constexpr std::string_view kCSource = ".c";
constexpr std::string_view kCHeader = ".h";
namespace wide {
constexpr const wchar_t* kCaIdeSettings = L".caide";
constexpr const wchar_t* kCaSolution = L".casln";
constexpr const wchar_t* kCaBuild = L".camake";
constexpr const wchar_t* kCaSource = L".cand";
constexpr const wchar_t* kCaHeader = L".candi";
constexpr const wchar_t* kCailCode = L".cail";
constexpr const wchar_t* kCppSource = L".cpp";
constexpr const wchar_t* kCppHeader = L".h";
constexpr const wchar_t* kCSource = L".c";
constexpr const wchar_t* kCHeader = L".h";
}  // namespace wide
}  // namespace file_extension

const DirectoryIter
    kPathEnd{};  // default construction yields past-the-end path

// Static util methods
WString ToWString(const String& s) { return WString(s.cbegin(), s.cend()); }

std::string loadFileToStr(const char* const name);
std::string loadFileToStr(const std::string& name);
std::string loadFileToStr(const char* const name) {
  std::filesystem::path filepath(
      std::filesystem::absolute(std::filesystem::path(name)));

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

  std::string fileStr;

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
std::string loadFileToStr(const std::string& name) {
  return loadFileToStr(name.c_str());
};

inline std::string GetPathStr(const Path& path) {
  std::wstring native_path = path.native();
  std::string str = std::string(native_path.cbegin(), native_path.cend());
  return str;
}
inline Path GetPathAppended(const Path& path, const Path::string_type& add) {
  Path::string_type new_path = path.native();
  new_path.append(add);
  return Path(new_path);
}
// If this path represents a folder, returns the inside path.
inline Path GetPathInside(const Path& path) {
  assert(
      stdfs::is_directory(path));  // user of method is responsible for checking
                                   // that this refers to a directory.
  return path.native() + L'\\';
}

// IDE startup settings, all values must be known at compile time.
// Represent a .caide file
class IdeSettings {
  static constexpr const wchar_t* kSolutionCacheSettingTag = L"#\n";
  inline Path GetSettingsFilePath() const {
    return binary_path.native() + L"settings" +
           file_extension::wide::kCaIdeSettings;
  }

  inline void GenerateSettingsFile() const {
    std::ofstream(GetSettingsFilePath())
        // 1. Binary Path
        << GetPathStr(binary_path)
        << '\n'
        // 2. Repository Path
        << GetPathStr(repository_path)
        << '\n'
        // 3. Cached Solutions
        << [this]() -> String {
      WString sln_folders = kSolutionCacheSettingTag;
      for (auto& cached_sln : this->cached_solutions) {
        sln_folders += cached_sln.native() + L'\n';
      }
      sln_folders += kSolutionCacheSettingTag;
      return String(sln_folders.cbegin(), sln_folders.cend());
    }();
  }

 public:
  // Get the binary path of the IDE. Current working directory of the C++
  // context. NOTE: this a copy not reference. current_path() is a dangerous
  // global.
  // In the context of development it will be the cand-ide project folder.
  Path binary_path{std::filesystem::current_path().native() +
                   L"\\"};  // Path to the IDE executable/binaries.
                            // This is the current working path of
                            // the C++ backend. Path to the
                            // cand-ide.exe folder.
  Path repository_path{};   // Path to the folder containing all solutions
                            // Which belong to this IDE. Only these will be
                            // Scanned and populated in the solution list.
                            // Each folder which ccontains a '.casln' file
  // is a solution. Only one .casln is loaded per folder.
  // The rest are ignored.

  std::vector<Path> cached_solutions{};

  IdeSettings() = default;
  IdeSettings(const Path& bin_path) : binary_path(bin_path) {}
  IdeSettings(const Path& bin_path, const Path& repo_path)
      : binary_path(bin_path), repository_path(repo_path) {}
  // Load the settings from a file. Return false if the file is invalid.
  bool Load() {
    if (!std::filesystem::exists(GetSettingsFilePath())) {
      // If no settings exist. Load defaults.
      return Default();
    }

    // Load.
    std::string settings = loadFileToStr(GetPathStr(GetSettingsFilePath()));
    // Extract the binary_path
    auto c = settings.begin();
    while (c != settings.end()) {
      if (*c = '\n') {
        break;
      }
      c++;
    }
    binary_path = std::string(settings.begin(), c);
    c++;  // advance past newline.
    std::string::iterator repo_path_begin = c;
    // Extract the repository path.
    while (c != settings.end()) {
      if (*c = '\n') {
        break;
      }
      c++;
    }
    repository_path = std::string(settings.begin(), c);
    c++;
    // If there is more, it must be a # indicating start of
    // solution cache.
    if (c != settings.end() && *c == '#') {
      c += 2;
      std::string::iterator last_cache_begin = c;
      // consume lines until next '#'
      while (c != settings.end() || *c == '#') {
        if (*c = '\n') {
          cached_solutions.push_back(std::string(last_cache_begin, c));
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
    Path file = GetSettingsFilePath();
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
  bool CacheSolution(const Path& sln_folder) {
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
  inline Path GetSettingsFilePath() const;
  bool Default();
  bool Save();
  bool Load();
  // Directory of the solution.[RELATIVE: Relative to the repository path.]
  // This is the working directory of the Solution's filesystem.
  // If you load a file from IDE code. This is the root directory.
  // For this solution's executable.
  Path solution_path;

  // Solution file [RELATIVE: Relative to the repository path]
  // .cansln file where solution metadata is stored. Only 1 per solution.
  // It is autoinferred to be the file named '.casln' inside the solution path.
  Path solution_file;

  // .camake files describing a build process of this solution.Autogenerated,
  // user-editable. unnamed file '.camake' is automatically included as the
  // first and default build file.
  std::vector<Path> build_files;

  // Working files which belong to the solution. Not including .casln / .camake
  // files.
  std::vector<Path> working_files;
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
  bool NewSolution(String name) {
    // Check if the path exists. If it already does cancel.
    // Query for a diffrent solution name.
    Path sln_folder_path =
        GetPathAppended(settings.repository_path, ToWString(name));

    if (stdfs::exists(sln_folder_path)) {
      return false;  // Folder is already in use.
    }

    // Create the solution folder.
    if (!stdfs::create_directories(sln_folder_path)) {
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
}  // namespace cide

/////////////////////////////////////////////////////////////////////////////////
// <cidewidgets>
/////////////////////////////////////////////////////////////////////////////////
const float kWindowWidth = 800.f;
const float kWindowHeight = 800.f;

struct CideTopMenuBarInterface {
  static constexpr LAMBDA xNullCallback = []() {};
  using CallbackT = std::function<void(void)>;
  // File Menu
  CallbackT callback_file_new_solution{xNullCallback};

  // Edit menu
  CallbackT callback_edit_undo{xNullCallback};
  CallbackT callback_edit_redo{xNullCallback};
  CallbackT callback_edit_cut{xNullCallback};
  CallbackT callback_edit_copy{xNullCallback};
  CallbackT callback_edit_paste{xNullCallback};

  // Project Menu
  CallbackT callback_project_addfile{xNullCallback};
  CallbackT callback_project_addactivefile{xNullCallback};
  CallbackT callback_project_addexistingfile{xNullCallback};
  CallbackT callback_project_solutionproperties{xNullCallback};
  CallbackT callback_project_clonesolution{xNullCallback};

  // Widgets
  CguiMenuBar main_menu_bar{cgui::kWidgetInitDelayed};
  CguiMenu file_menu{CguiMenu::Delayed("File")};
  CguiMenu edit_menu{CguiMenu::Delayed("Edit")};
  CguiMenu project_menu{CguiMenu::Delayed("Project")};
  CguiMenu file_new_submenu{CguiMenu::Delayed("New")};

  CguiMenuItem file_new_solution_item{"Solution", "", true,
                                      cgui::kWidgetInitDelayed};
  CguiMenuItem edit_undo_item{"Undo", "CTRL+Z", true, cgui::kWidgetInitDelayed};
  CguiMenuItem edit_redo_item{"Redo", "CTRL+Y", true, cgui::kWidgetInitDelayed};
  CguiMenuItem edit_cut_item{"Cut", "CTRL+X", true, cgui::kWidgetInitDelayed};
  CguiMenuItem edit_copy_item{"Copy", "CTRL+C", true, cgui::kWidgetInitDelayed};
  CguiMenuItem edit_paste_item{"Paste", "CTRL+V", true,
                               cgui::kWidgetInitDelayed};

  CguiMenuItem project_addfile_item{"Add File", "", true,
                                    cgui::kWidgetInitDelayed};
  CguiMenuItem project_addactivefile_item{"Add Active File", "", true,
                                          cgui::kWidgetInitDelayed};
  CguiMenuItem project_addexistingfile_item{"Add Existing File", "", true,
                                            cgui::kWidgetInitDelayed};
  CguiMenuItem project_solutionproperties_item{"Solution Properties", "", true,
                                               cgui::kWidgetInitDelayed};
  CguiMenuItem project_clonesolution_item{"Clone Solution", "", true,
                                          cgui::kWidgetInitDelayed};

  void Display() {
    if (main_menu_bar.BeginLate()) {
      if (file_menu.BeginLate()) {
        if (file_new_submenu.BeginLate()) {
          if (file_new_solution_item.BeginLate()) {
            callback_file_new_solution();
          }
        }
        file_new_submenu.EndEarly();
      }
      file_menu.EndEarly();

      if (edit_menu.BeginLate()) {
        if (edit_undo_item.BeginLate()) {
          callback_edit_undo();
        }
        if (edit_redo_item.BeginLate()) {
          callback_edit_redo();
        }
        if (edit_cut_item.BeginLate()) {
          callback_edit_cut();
        }
        if (edit_copy_item.BeginLate()) {
          callback_edit_copy();
        }
        if (edit_paste_item.BeginLate()) {
          callback_edit_paste();
        }
      }
      edit_menu.EndEarly();

      if (project_menu.BeginLate()) {
        if (project_addfile_item.BeginLate()) {
          callback_project_addfile();
        }
        if (project_addactivefile_item.BeginLate()) {
          callback_project_addactivefile();
        }
        if (project_addexistingfile_item.BeginLate()) {
          callback_project_addexistingfile();
        }
        cgui::Separator();
        if (project_solutionproperties_item.BeginLate()) {
          callback_project_solutionproperties();
        };
        cgui::Separator();
        if (project_clonesolution_item.BeginLate()) {
          callback_project_clonesolution();
        };
      }
      project_menu.EndEarly();
    }
    main_menu_bar.EndEarly();
  }
};

struct CideFileEditorInterface {
  // Properties
  CguiVec2 context_size;
  CguiNamedSubcontext editor_context;
  CguiTabBar editor_tab_bar;
  std::vector<CguiTabItem> open_file_tabs;
  std::vector<CguiMultilineTextInput> open_file_tab_text_inputs;

  CideFileEditorInterface(const std::string& name, CguiVec2 context_size)
      : context_size(context_size),
        editor_context(CguiNamedSubcontext::Delayed(name, context_size)),
        editor_tab_bar(CguiTabBar(
            "##file-tabs", {cgui::eTabBarFlags::ImGuiTabBarFlags_Reorderable},
            cgui::kWidgetInitDelayed)) {}

  void Display() {  // Editor Subcontext
    if (editor_context.BeginLate()) {
      if (editor_tab_bar.BeginLate()) {
        for (size_t idx = 0; auto& file_tab : open_file_tabs) {
          if (file_tab.BeginLate()) {
            open_file_tab_text_inputs[idx].BeginLate();
          }
          file_tab.EndEarly();
          idx++;
        }
      }
      editor_tab_bar.EndEarly();
    }
    editor_context.EndEarly();
  }

  void AddTab(const std::string& tab_name, std::string& text_buffer) {
    open_file_tabs.push_back(
        CguiTabItem(tab_name, CguiTabItemFlags(), cgui::kWidgetInitDelayed));
    open_file_tab_text_inputs.push_back(
        CguiMultilineTextInput::Delayed(tab_name + "###textbox", text_buffer,
                                        cgui::kExpandWidgetToRemainingSpaceXY));
  }

  void PopTab() { open_file_tabs.pop_back(); }
};

struct CideSolutionToolbarInterface {
  static constexpr LAMBDA xNullCallback = [](const stdfs::path&) {};
  using CallbackT = std::function<void(const stdfs::path&)>;

  // By default select file callback will store the selected file in
  // the local temp_file_buffer.
  CallbackT select_file_callback{xNullCallback};

  // Edit menu
  CallbackT callback_edit_open{xNullCallback};
  CallbackT callback_edit_delete{xNullCallback};
  CallbackT callback_edit_cut{xNullCallback};
  CallbackT callback_edit_copy{xNullCallback};
  CallbackT callback_edit_paste{xNullCallback};

  CguiVec2 requested_size;

  stdfs::path root_dir;
  std::string temp_file_buffer;

  // Widgets
  CguiNamedSubcontext solution_toolbar_context;
  CguiTabBar solution_toolbar_tab_bar{
      CguiTabBar::Delayed("solution_toolbar_tab_bar")};
  CguiTabItem solution_explorer_tab_item{CguiTabItem::Delayed("Solution View")};
  CguiDirectoryView dir_tree_view;

  inline void BeginRightClickContextMenu(const stdfs::path& p) const {
    // New Menu:
    if (CguiMenuItem("Open")) {
      callback_edit_open(p);
    }
    if (CguiMenuItem("Copy")) {
      callback_edit_copy(p);
    }
    if (CguiMenuItem("Paste")) {
      callback_edit_paste(p);
    }
    if (CguiMenuItem("Cut")) {
      callback_edit_cut(p);
    }
    if (CguiMenuItem("Delete")) {
      callback_edit_delete(p);
    }
  };

 public:
  CideSolutionToolbarInterface(const CguiVec2& requested_size = {})
      : requested_size(requested_size),
        solution_toolbar_context(
            CguiNamedSubcontext::Delayed("Solution Toolbar", requested_size)),
        root_dir(std::filesystem::current_path()),
        dir_tree_view(
            root_dir,
            // callback for selectting an item
            [this](const stdfs::path& p) {
              temp_file_buffer = cide::loadFileToStr(p.string());
              select_file_callback(p);
            },
            // callback for right clicking an item
            [this](const stdfs::path& p) {
              // New Menu:
              BeginRightClickContextMenu(p);
            },
            cgui::kWidgetInitDelayed) {}

  void Display() {
    if (solution_toolbar_context.BeginLate()) {
      if (solution_toolbar_tab_bar.BeginLate()) {
        if (solution_explorer_tab_item.BeginLate()) {
          dir_tree_view.BeginLate();
        }
        solution_explorer_tab_item.EndEarly();
      }
      solution_toolbar_tab_bar.EndEarly();
    }
    solution_toolbar_context.EndEarly();
  }
};

struct CideUserInterface {
  CideTopMenuBarInterface top_menu_bar_interface{};
  CideFileEditorInterface file_editor_interface{
      "Editor", {kWindowWidth * 0.75f, kWindowHeight * 0.75f}};
  ;
  CideSolutionToolbarInterface solution_toolbar_interface{
      {0, kWindowHeight * 0.75f}};

  CguiWindow main_ide_context{
      "C&-IDE", false, {ImGuiWindowFlags_MenuBar}, cgui::kWidgetInitDelayed};

  void Display() {
    gui::SetNextWindowSize({kWindowWidth, kWindowHeight});
    gui::SetNextWindowPos({0, 0});
    main_ide_context.BeginLate();
    top_menu_bar_interface.Display();
    file_editor_interface.Display();
    cgui::SameLine();
    solution_toolbar_interface.Display();
    main_ide_context.EndEarly();
  }
};

/////////////////////////////////////////////////////////////////////////////////
// <end_cidewidgets>
/////////////////////////////////////////////////////////////////////////////////

int main() {
  sf::RenderWindow window(sf::VideoMode(800, 800), "ImGui + SFML = <3");
  window.setFramerateLimit(60);
  ImGui::SFML::Init(window);

  CideUserInterface cide_ui;
  // cide::IdeInterface idei;

  sf::Clock deltaClock;
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      ImGui::SFML::ProcessEvent(window, event);

      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    ImGui::SFML::Update(window, deltaClock.restart());
    // ImGui::ShowDemoWindow();
    cide_ui.Display();
    window.clear();
    ImGui::SFML::Render(window);
    window.display();
  }

  ImGui::SFML::Shutdown();
}