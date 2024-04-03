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

struct ExampleAppConsole {
  char InputBuf[256];
  ImVector<char*> Items;
  ImVector<const char*> Commands;
  ImVector<char*> History;
  int HistoryPos;  // -1: new line, 0..History.Size-1 browsing history.
  ImGuiTextFilter Filter;
  bool AutoScroll;
  bool ScrollToBottom;

  ExampleAppConsole() {
    ClearLog();
    memset(InputBuf, 0, sizeof(InputBuf));
    HistoryPos = -1;

    // "CLASSIFY" is here to provide the test case where "C"+[tab] completes to
    // "CL" and display multiple matches.
    Commands.push_back("HELP");
    Commands.push_back("HISTORY");
    Commands.push_back("CLEAR");
    Commands.push_back("CLASSIFY");
    AutoScroll = true;
    ScrollToBottom = false;
    AddLog("Welcome to Dear ImGui!");
  }
  ~ExampleAppConsole() {
    ClearLog();
    for (int i = 0; i < History.Size; i++) free(History[i]);
  }

  // Portable helpers
  static int Stricmp(const char* s1, const char* s2) {
    int d;
    while ((d = toupper(*s2) - toupper(*s1)) == 0 && *s1) {
      s1++;
      s2++;
    }
    return d;
  }
  static int Strnicmp(const char* s1, const char* s2, int n) {
    int d = 0;
    while (n > 0 && (d = toupper(*s2) - toupper(*s1)) == 0 && *s1) {
      s1++;
      s2++;
      n--;
    }
    return d;
  }
  static char* Strdup(const char* s) {
    IM_ASSERT(s);
    size_t len = strlen(s) + 1;
    void* buf = malloc(len);
    IM_ASSERT(buf);
    return (char*)memcpy(buf, (const void*)s, len);
  }
  static void Strtrim(char* s) {
    char* str_end = s + strlen(s);
    while (str_end > s && str_end[-1] == ' ') str_end--;
    *str_end = 0;
  }

  void ClearLog() {
    for (int i = 0; i < Items.Size; i++) free(Items[i]);
    Items.clear();
  }

  void AddLog(const char* fmt, ...) IM_FMTARGS(2) {
    // FIXME-OPT
    char buf[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
    buf[IM_ARRAYSIZE(buf) - 1] = 0;
    va_end(args);
    Items.push_back(Strdup(buf));
  }

  void Draw(const char* title, bool* p_open) {
    ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
    if (!ImGui::BeginChild(title)) {
      ImGui::EndChild();
      return;
    }

    // As a specific feature guaranteed by the library, after calling Begin()
    // the last Item represent the title bar. So e.g. IsItemHovered() will
    // return true when hovering the title bar. Here we create a context menu
    // only available from the title bar.
    if (ImGui::BeginPopupContextItem()) {
      if (ImGui::MenuItem("Close Console")) *p_open = false;
      ImGui::EndPopup();
    }

    ImGui::TextWrapped("");
    ImGui::TextWrapped("Enter 'HELP' for help.");

    // TODO: display items starting from the bottom

    if (ImGui::SmallButton("Add Debug Text")) {
      AddLog("%d some text", Items.Size);
      AddLog("some more text");
      AddLog("display very important message here!");
    }
    ImGui::SameLine();
    if (ImGui::SmallButton("Add Debug Error")) {
      AddLog("[error] something went wrong");
    }
    ImGui::SameLine();
    if (ImGui::SmallButton("Clear")) {
      ClearLog();
    }
    ImGui::SameLine();
    bool copy_to_clipboard = ImGui::SmallButton("Copy");
    // static float t = 0.0f; if (ImGui::GetTime() - t > 0.02f) { t =
    // ImGui::GetTime(); AddLog("Spam %f", t); }

    ImGui::Separator();

    // Options menu
    if (ImGui::BeginPopup("Options")) {
      ImGui::Checkbox("Auto-scroll", &AutoScroll);
      ImGui::EndPopup();
    }

    // Options, Filter
    if (ImGui::Button("Options")) ImGui::OpenPopup("Options");
    ImGui::SameLine();
    Filter.Draw("Filter (\"incl,-excl\") (\"error\")", 180);
    ImGui::Separator();

    // Reserve enough left-over height for 1 separator + 1 input text
    const float footer_height_to_reserve =
        ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
    if (ImGui::BeginChild(
            "ScrollingRegion", ImVec2(0, -footer_height_to_reserve),
            ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar)) {
      if (ImGui::BeginPopupContextWindow()) {
        if (ImGui::Selectable("Clear")) ClearLog();
        ImGui::EndPopup();
      }

      // Display every line as a separate entry so we can change their color or
      // add custom widgets. If you only want raw text you can use
      // ImGui::TextUnformatted(log.begin(), log.end()); NB- if you have
      // thousands of entries this approach may be too inefficient and may
      // require user-side clipping to only process visible items. The clipper
      // will automatically measure the height of your first item and then
      // "seek" to display only items in the visible area.
      // To use the clipper we can replace your standard loop:
      //      for (int i = 0; i < Items.Size; i++)
      //   With:
      //      ImGuiListClipper clipper;
      //      clipper.Begin(Items.Size);
      //      while (clipper.Step())
      //         for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
      // - That your items are evenly spaced (same height)
      // - That you have cheap random access to your elements (you can access
      // them given their index,
      //   without processing all the ones before)
      // You cannot this code as-is if a filter is active because it breaks the
      // 'cheap random-access' property. We would need random-access on the
      // post-filtered list. A typical application wanting coarse clipping and
      // filtering may want to pre-compute an array of indices or offsets of
      // items that passed the filtering test, recomputing this array when user
      // changes the filter, and appending newly elements as they are inserted.
      // This is left as a task to the user until we can manage to improve this
      // example code! If your items are of variable height:
      // - Split them into same height items would be simpler and facilitate
      // random-seeking into your list.
      // - Consider using manual call to IsRectVisible() and skipping extraneous
      // decoration from your items.
      ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,
                          ImVec2(4, 1));  // Tighten spacing
      if (copy_to_clipboard) ImGui::LogToClipboard();
      for (const char* item : Items) {
        if (!Filter.PassFilter(item)) continue;

        // Normally you would store more information in your item than just a
        // string. (e.g. make Items[] an array of structure, store color/type
        // etc.)
        ImVec4 color;
        bool has_color = false;
        if (strstr(item, "[error]")) {
          color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f);
          has_color = true;
        } else if (strncmp(item, "# ", 2) == 0) {
          color = ImVec4(1.0f, 0.8f, 0.6f, 1.0f);
          has_color = true;
        }
        if (has_color) ImGui::PushStyleColor(ImGuiCol_Text, color);
        ImGui::TextUnformatted(item);
        if (has_color) ImGui::PopStyleColor();
      }
      if (copy_to_clipboard) ImGui::LogFinish();

      // Keep up at the bottom of the scroll region if we were already at the
      // bottom at the beginning of the frame. Using a scrollbar or mouse-wheel
      // will take away from the bottom edge.
      if (ScrollToBottom ||
          (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
        ImGui::SetScrollHereY(1.0f);
      ScrollToBottom = false;

      ImGui::PopStyleVar();
    }
    ImGui::EndChild();
    ImGui::Separator();

    // Command-line
    bool reclaim_focus = false;
    ImGuiInputTextFlags input_text_flags =
        ImGuiInputTextFlags_EnterReturnsTrue |
        ImGuiInputTextFlags_EscapeClearsAll |
        ImGuiInputTextFlags_CallbackCompletion |
        ImGuiInputTextFlags_CallbackHistory;
    if (ImGui::InputText("Input", InputBuf, IM_ARRAYSIZE(InputBuf),
                         input_text_flags, &TextEditCallbackStub,
                         (void*)this)) {
      char* s = InputBuf;
      Strtrim(s);
      if (s[0]) ExecCommand(s);
      strcpy_s(s, sizeof s, "");
      reclaim_focus = true;
    }

    // Auto-focus on window apparition
    ImGui::SetItemDefaultFocus();
    if (reclaim_focus)
      ImGui::SetKeyboardFocusHere(-1);  // Auto focus previous widget

    ImGui::EndChild();
  }

  void ExecCommand(const char* command_line) {
    AddLog("# %s\n", command_line);

    // Insert into history. First find match and delete it so it can be pushed
    // to the back. This isn't trying to be smart or optimal.
    HistoryPos = -1;
    for (int i = History.Size - 1; i >= 0; i--)
      if (Stricmp(History[i], command_line) == 0) {
        free(History[i]);
        History.erase(History.begin() + i);
        break;
      }
    History.push_back(Strdup(command_line));

    // Process command
    if (Stricmp(command_line, "CLEAR") == 0) {
      ClearLog();
    } else if (Stricmp(command_line, "HELP") == 0) {
      AddLog("Commands:");
      for (int i = 0; i < Commands.Size; i++) AddLog("- %s", Commands[i]);
    } else if (Stricmp(command_line, "HISTORY") == 0) {
      int first = History.Size - 10;
      for (int i = first > 0 ? first : 0; i < History.Size; i++)
        AddLog("%3d: %s\n", i, History[i]);
    } else {
      AddLog("Unknown command: '%s'\n", command_line);
    }

    // On command input, we scroll to bottom even if AutoScroll==false
    ScrollToBottom = true;
  }

  // In C++11 you'd be better off using lambdas for this sort of forwarding
  // callbacks
  static int TextEditCallbackStub(ImGuiInputTextCallbackData* data) {
    ExampleAppConsole* console = (ExampleAppConsole*)data->UserData;
    return console->TextEditCallback(data);
  }

  int TextEditCallback(ImGuiInputTextCallbackData* data) {
    // AddLog("cursor: %d, selection: %d-%d", data->CursorPos,
    // data->SelectionStart, data->SelectionEnd);
    switch (data->EventFlag) {
      case ImGuiInputTextFlags_CallbackCompletion: {
        // Example of TEXT COMPLETION

        // Locate beginning of current word
        const char* word_end = data->Buf + data->CursorPos;
        const char* word_start = word_end;
        while (word_start > data->Buf) {
          const char c = word_start[-1];
          if (c == ' ' || c == '\t' || c == ',' || c == ';') break;
          word_start--;
        }

        // Build a list of candidates
        ImVector<const char*> candidates;
        for (int i = 0; i < Commands.Size; i++)
          if (Strnicmp(Commands[i], word_start, (int)(word_end - word_start)) ==
              0)
            candidates.push_back(Commands[i]);

        if (candidates.Size == 0) {
          // No match
          AddLog("No match for \"%.*s\"!\n", (int)(word_end - word_start),
                 word_start);
        } else if (candidates.Size == 1) {
          // Single match. Delete the beginning of the word and replace it
          // entirely so we've got nice casing.
          data->DeleteChars((int)(word_start - data->Buf),
                            (int)(word_end - word_start));
          data->InsertChars(data->CursorPos, candidates[0]);
          data->InsertChars(data->CursorPos, " ");
        } else {
          // Multiple matches. Complete as much as we can..
          // So inputing "C"+Tab will complete to "CL" then display "CLEAR" and
          // "CLASSIFY" as matches.
          int match_len = (int)(word_end - word_start);
          for (;;) {
            int c = 0;
            bool all_candidates_matches = true;
            for (int i = 0; i < candidates.Size && all_candidates_matches; i++)
              if (i == 0)
                c = toupper(candidates[i][match_len]);
              else if (c == 0 || c != toupper(candidates[i][match_len]))
                all_candidates_matches = false;
            if (!all_candidates_matches) break;
            match_len++;
          }

          if (match_len > 0) {
            data->DeleteChars((int)(word_start - data->Buf),
                              (int)(word_end - word_start));
            data->InsertChars(data->CursorPos, candidates[0],
                              candidates[0] + match_len);
          }

          // List matches
          AddLog("Possible matches:\n");
          for (int i = 0; i < candidates.Size; i++)
            AddLog("- %s\n", candidates[i]);
        }

        break;
      }
      case ImGuiInputTextFlags_CallbackHistory: {
        // Example of HISTORY
        const int prev_history_pos = HistoryPos;
        if (data->EventKey == ImGuiKey_UpArrow) {
          if (HistoryPos == -1)
            HistoryPos = History.Size - 1;
          else if (HistoryPos > 0)
            HistoryPos--;
        } else if (data->EventKey == ImGuiKey_DownArrow) {
          if (HistoryPos != -1)
            if (++HistoryPos >= History.Size) HistoryPos = -1;
        }

        // A better implementation would preserve the data on the current input
        // line along with cursor position.
        if (prev_history_pos != HistoryPos) {
          const char* history_str =
              (HistoryPos >= 0) ? History[HistoryPos] : "";
          data->DeleteChars(0, data->BufTextLen);
          data->InsertChars(0, history_str);
        }
      }
    }
    return 0;
  }
};
static void ShowAppConsole() {
  static ExampleAppConsole console;
  static bool po{true};
  console.Draw("Example: Console", &po);
}
const DirectoryIter
    kPathEnd{};  // default construction yields past-the-end path
void PrintDirectory(const stdfs::path& path, int depth = 0) {
  if (stdfs::is_directory(path)) {
    std::cout << std::string(depth, '\t') << "[+] " << path.filename() << "/\n";
    for (const auto& entry : stdfs::directory_iterator(path)) {
      PrintDirectory(entry.path(), depth + 1);
    }
  } else if (stdfs::is_regular_file(path)) {
    std::cout << std::string(depth, '\t') << "- " << path.filename() << "\n";
  }
}
static std::function<void(const Path&)> gDisplayDirectorySelectFileCallback;
void DisplayDirectory(const stdfs::path& path, int depth = 0) {
  if (stdfs::is_directory(path)) {
    bool node_open = gui::TreeNode(path.filename().string().c_str());
    if (node_open) {
      for (const auto& entry : stdfs::directory_iterator(path)) {
        DisplayDirectory(entry.path(), depth + 1);
      }
      gui::TreePop();
    }
  } else if (stdfs::is_regular_file(path)) {
    // auto file_selectable = cgui::Selectable(path.filename().string());
    if (cgui::Selectable(path.filename().string())) {
      gDisplayDirectorySelectFileCallback(path);
    }
    // if (gui::Selectable(path.filename().string().c_str())) {
    //   gDisplayDirectorySelectFileCallback(path);
    // };
  }
}

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
// Abstract Backend Interfaces
struct FileSystemInterface;  // OS filesystem

// GUI Backend Interfaces
struct IdeInterface;  // global IDE
struct IdeSettings;   // IDE settings

struct SolutionSettingsInterface;  // interface for the solution properties gui.
struct IdeSettingsInterface;       // interface for the ide glbal settings.

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

static void GuiDrawMenuFile() {
  if (gui::BeginMenu("File")) {
    // New Menu:
    if (gui::BeginMenu("New")) {
      if (gui::MenuItem("File...")) {
      };
      if (gui::MenuItem("Solution...")) {
      };
      gui::EndMenu();
    }

    // Open Menu:
    if (gui::BeginMenu("Open")) {
      if (gui::MenuItem("File...")) {
      };
      if (gui::MenuItem("Solution...")) {
      };
      gui::EndMenu();
    }

    // Add existing file or dir to project.
    if (gui::BeginMenu("Add")) {
      if (gui::MenuItem("File...")) {
      };
      if (gui::MenuItem("Folder...")) {
      };
      gui::EndMenu();
    }

    // Saving options...
    gui::Separator();
    if (gui::MenuItem("Save All")) {
    }
    if (gui::MenuItem("Save File")) {
    }
    if (gui::MenuItem("Save File As...")) {
    };
    gui::EndMenu();
  }
}

static void GuiDrawMenuProject() {
  if (gui::BeginMenu("Project")) {
    if (gui::MenuItem("Add File")) {
    }
    if (gui::MenuItem("Add Active File")) {
    }
    if (gui::MenuItem("Add Existing File")) {
    }
    gui::Separator();
    if (gui::MenuItem("Solution Properties")) {
    };
    gui::Separator();
    if (gui::MenuItem("Clone Solution")) {
    };
    gui::EndMenu();
  }
}

static void GuiDrawMenuEdit() {
  if (ImGui::BeginMenu("Edit")) {
    if (ImGui::MenuItem("Undo", "CTRL+Z")) {
    }
    if (ImGui::MenuItem("Redo", "CTRL+Y")) {
    }
    ImGui::Separator();
    if (ImGui::MenuItem("Cut", "CTRL+X")) {
    }
    if (ImGui::MenuItem("Copy", "CTRL+C")) {
    }
    if (ImGui::MenuItem("Paste", "CTRL+V")) {
    }
    ImGui::EndMenu();
  }
}

// Demonstrate the various window flags. Typically you would just use the
// default!
static bool no_titlebar = false;
static bool no_scrollbar = false;
static bool no_menu = false;
static bool no_move = false;
static bool no_resize = false;
static bool no_collapse = false;
static bool no_close = false;
static bool no_nav = false;
static bool no_background = false;
static bool no_bring_to_front = false;
static bool unsaved_document = false;

auto xMakeWindowFlags = []() constexpr -> ImGuiWindowFlags {
  ImGuiWindowFlags window_flags = 0;
  if (no_titlebar) window_flags |= ImGuiWindowFlags_NoTitleBar;
  if (no_scrollbar) window_flags |= ImGuiWindowFlags_NoScrollbar;
  if (!no_menu) window_flags |= ImGuiWindowFlags_MenuBar;
  if (no_move) window_flags |= ImGuiWindowFlags_NoMove;
  if (no_resize) window_flags |= ImGuiWindowFlags_NoResize;
  if (no_collapse) window_flags |= ImGuiWindowFlags_NoCollapse;
  if (no_nav) window_flags |= ImGuiWindowFlags_NoNav;
  if (no_background) window_flags |= ImGuiWindowFlags_NoBackground;
  if (no_bring_to_front) window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
  if (unsaved_document) window_flags |= ImGuiWindowFlags_UnsavedDocument;
  return window_flags;
  // if (no_close) p_open = NULL;  // Don't pass our bool* to Begin
};
constexpr float kExpandWidgetToRemainingSpace() { return -FLT_MIN; }

/////////////////////////////////////////////////////////////////////////////////
// <cidewidgets>
/////////////////////////////////////////////////////////////////////////////////

struct CideTopMenuBarInterface {
  static constexpr LAMBDA xNullCallback = []() {};
  using CallbackT = std::function<void(void)>;
  cgui::WidgetMaker& make_cgui;
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
  cgui::MenuBar main_menu_bar{make_cgui.MakeDelayedMenuBar()};
  cgui::Menu file_menu{make_cgui.MakeDelayedMenu("File")};
  cgui::Menu edit_menu{make_cgui.MakeDelayedMenu("Edit")};
  cgui::Menu project_menu{make_cgui.MakeDelayedMenu("Project")};
  cgui::Menu file_new_submenu{make_cgui.MakeDelayedMenu("New")};

  cgui::MenuItem file_new_solution_item{"Solution", "", true, true};

  cgui::MenuItem edit_undo_item{"Undo", "CTRL+Z", true, true};
  cgui::MenuItem edit_redo_item{"Redo", "CTRL+Y", true, true};
  cgui::MenuItem edit_cut_item{"Cut", "CTRL+X", true, true};
  cgui::MenuItem edit_copy_item{"Copy", "CTRL+C", true, true};
  cgui::MenuItem edit_paste_item{"Paste", "CTRL+V", true, true};

  cgui::MenuItem project_addfile_item{"Add File", "", true, true};
  cgui::MenuItem project_addactivefile_item{"Add Active File", "", true, true};
  cgui::MenuItem project_addexistingfile_item{"Add Existing File", "", true,
                                              true};
  cgui::MenuItem project_solutionproperties_item{"Solution Properties", "",
                                                 true, true};
  cgui::MenuItem project_clonesolution_item{"Clone Solution", "", true, true};

  void Display(cgui::WidgetMaker& make_cgui) {
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
  struct FileTab {
    cgui::TabItem tab_item;
    cgui::MultiLineTextInput text_box;
    bool marked_for_destruction;
    FileTab(cgui::TabItem&& tab_item, cgui::MultiLineTextInput text_box,
            bool marked_for_destruction)
        : tab_item(std::move(tab_item)),
          text_box(text_box),
          marked_for_destruction(marked_for_destruction) {}
  };
  // Properties
  cgui::CguiVec2 context_size;

  // Widgets
  cgui::WidgetMaker& make_cgui;
  cgui::Subcontext editor_context;
  cgui::TabBar editor_tab_bar;
  std::vector<FileTab> open_file_tabs;

  CideFileEditorInterface(cgui::WidgetMaker& make_cgui, const std::string& name,
                          cgui::CguiVec2 context_size)
      : make_cgui(make_cgui),
        context_size(context_size),
        editor_context(make_cgui.MakeDelayedSubcontext(name, context_size)),
        editor_tab_bar(make_cgui.MakeDelayedTabBar(
            "##file-tabs",
            {cgui::eTabBarFlags::ImGuiTabBarFlags_Reorderable})) {}

  void Display() {  // Editor Subcontext
    if (editor_context.BeginLate()) {
      if (editor_tab_bar.BeginLate()) {
        for (auto& file_tab : open_file_tabs) {
          if (file_tab.tab_item.BeginLate()) {
            file_tab.text_box.BeginLate();
          }
          file_tab.tab_item.EndEarly();
        }
      }
      editor_tab_bar.EndEarly();
    }
    editor_context.EndEarly();
  }

  void AddTab(const std::string& tab_name, std::string& text_buffer) {
    open_file_tabs.push_back(FileTab{
        make_cgui.MakeTabItem(tab_name, cgui::TabItemFlags(),
                              cgui::kDelayWidget),
        cgui::MultiLineTextInput(tab_name + "###textbox", &text_buffer,
                                 cgui::kExpandWidgetToRemainingSpaceXY,
                                 cgui::InputTextFlags(), cgui::kDelayWidget),
        false});
  }

  void PopTab() { open_file_tabs.pop_back(); }
};

struct CideSolutionExplorerInterface {
  stdfs::path root_dir;
  std::string temp_file_buffer;

  // Widgets
  cgui::DirectoryView dir_tree_view;
};

/////////////////////////////////////////////////////////////////////////////////
// <end_cidewidgets>
/////////////////////////////////////////////////////////////////////////////////

int main() {
  cgui::WidgetMaker make_cgui;
  const ImVec2 kExpandWidgetToRemainingSpaceXY = {
      kExpandWidgetToRemainingSpace(), kExpandWidgetToRemainingSpace()};
  std::string gEditorStringBuffer{""};
  sf::RenderWindow window(sf::VideoMode(800, 800), "ImGui + SFML = <3");
  window.setFramerateLimit(60);
  ImGui::SFML::Init(window);

  // cgui
  CideFileEditorInterface file_editor_interface{
      make_cgui,
      "Editor",
      {static_cast<float>(window.getSize().x * 0.75f),
       static_cast<float>(window.getSize().y * 0.75f)}};
  CideTopMenuBarInterface topbarmenu_interface{make_cgui};
  file_editor_interface.AddTab("TestingTab", gEditorStringBuffer);

  sf::CircleShape shape(100.f);
  shape.setFillColor(sf::Color::Green);

  cide::IdeInterface idei;
  idei.startup();
  cide::gDisplayDirectorySelectFileCallback =
      [&gEditorStringBuffer](const auto& p) {
        // Load file into the string buffer;
        gEditorStringBuffer = cide::loadFileToStr(p.string());
      };
  cide::PrintDirectory(cide::stdfs::current_path());
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
    ImGui::ShowDemoWindow();

    // Main window, same size a sfml window, pos at 0,0.
    gui::SetNextWindowSize(window.getSize());
    gui::SetNextWindowPos({0, 0});
    {  // Main Ide Window Context
      auto main_ide_context = make_cgui.MakeWindow(
          "C&-IDE", cgui::eWidgetState::kForceOn, {ImGuiWindowFlags_MenuBar});
      topbarmenu_interface.Display(make_cgui);
      file_editor_interface.Display();


      // File Explorer
      cgui::SameLine();
      ImGui::BeginChild("File Explorer",
                        {0, static_cast<float>(window.getSize().y * 0.75f)}, 1);

      ImGuiTabBarFlags tab_bar_flags2 =
          (true ? ImGuiTabBarFlags_Reorderable : 0);
      if (ImGui::BeginTabBar("##file-tabs", tab_bar_flags2)) {
        static bool opn2 = true;
        ImGuiTabItemFlags tab_flags2 =
            (true ? ImGuiTabItemFlags_UnsavedDocument : 0);
        if (ImGui::BeginTabItem("Solution", &opn2, tab_flags2)) {
          auto dir_view = cgui::DirectoryView(
              cide::stdfs::current_path(),
              // callback for selectting an item
              [&gEditorStringBuffer](const auto& p) {
                // Load file into the string buffer;
                gEditorStringBuffer = cide::loadFileToStr(p.string());
              },
              // callback for right clicking an item
              [](const auto& p) {
                // New Menu:
                if (cgui::MenuItem("Open")) {
                };
                if (cgui::MenuItem("Copy")) {
                };
                if (cgui::MenuItem("Paste")) {
                };
                if (cgui::MenuItem("Cut")) {
                };
                if (cgui::MenuItem("Delete")) {
                };
              });
          ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
      }
      // End of File explorer
      ImGui::EndChild();

      //  // Console
      cide::ShowAppConsole();
    }  // End Main Ide Window Context

    cgui::WindowWidget a{"wowowoow"};
    a.EndEarly();
    // End of App.
    // ImGui::End();

    window.clear();
    window.draw(shape);
    ImGui::SFML::Render(window);
    window.display();
  }

  ImGui::SFML::Shutdown();
}