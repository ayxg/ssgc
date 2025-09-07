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
/// @brief CIDE graphical user interface and presentation implementation.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup cidrlib_frontend
/// @{

// clang-format off
#pragma once
#include <bitset>

#include "Common.hpp"
#include "Backend.hpp"

#include "UserInterface/AstExplorer.hpp"
//#include "UserInterface/CppTestExplorer.hpp"
#include "UserInterface/TextEditor.hpp"
#include "UserInterface/FirstLaunchWizard.hpp"
#include "UserInterface/LoadingScreen.hpp"
// clang-format on

namespace cidr::ui {
/// @addtogroup cand_cide_frontend
/// @{

// Main CIDE Interface
class TopMenuBar;
class FileEditor;
class SolutionExplorer;
class CideUserInterface;
class UserInterface;

struct TopMenuBar {
  bool open_new_project_modal = false;
  bool show_open_repository_modal = false;
  std::unique_ptr<string> new_repo_name_buffer_ = nullptr;
  std::unique_ptr<string> new_repo_path_buffer_ = nullptr;

  // Widgets
  CguiMenuBar main_menu_bar{cgui::kWidgetInitDelayed};
  CguiMenu file_menu{CguiMenu::Delayed("File")};
  CguiMenu edit_menu{CguiMenu::Delayed("Edit")};
  CguiMenu project_menu{CguiMenu::Delayed("Project")};
  CguiMenu action_menu{CguiMenu::Delayed("Action")};
  CguiMenu file_new_submenu{CguiMenu::Delayed("New")};
  CguiMenu file_open_submenu{CguiMenu::Delayed("Open")};
  CguiMenu tools_menu{CguiMenu::Delayed("Tools")};
  CguiMenuItem file_new_solution_item{"Solution", "", true, cgui::kWidgetInitDelayed};

  CguiMenuItem project_addfile_item{"Add File", "", true, cgui::kWidgetInitDelayed};
  CguiMenuItem project_addactivefile_item{"Add Active File", "", true, cgui::kWidgetInitDelayed};
  CguiMenuItem project_addexistingfile_item{"Add Existing File", "", true, cgui::kWidgetInitDelayed};
  CguiMenuItem project_solutionproperties_item{"Solution Properties", "", true, cgui::kWidgetInitDelayed};
  CguiMenuItem project_clonesolution_item{"Clone Solution", "", true, cgui::kWidgetInitDelayed};

  function<std::expected<void, string>(const string&, const string&)> cb_file_new_solution{};
  function<std::expected<void, string>(const string&)> cb_file_open_solution{};

  function<void(void)> cb_edit_undo{xNullCallback};
  function<void(void)> cb_edit_redo{xNullCallback};
  function<void(void)> cb_edit_cut{xNullCallback};
  function<void(void)> cb_edit_copy{xNullCallback};
  function<void(void)> cb_edit_paste{xNullCallback};

  function<void(void)> cb_project_addfile{xNullCallback};
  function<void(void)> cb_project_addactivefile{xNullCallback};
  function<void(void)> cb_project_addexistingfile{xNullCallback};
  function<void(void)> cb_project_solutionproperties{xNullCallback};
  function<void(void)> cb_project_clonesolution{xNullCallback};

  function<void(void)> cb_action_generate{xNullCallback};
  function<void(void)> cb_action_build{xNullCallback};
  function<void(void)> cb_action_run{xNullCallback};

  function<void(void)> cb_tools_astexplorer{xNullCallback};

  void Display() {
    if (main_menu_bar.BeginLate()) {
      if (file_menu.BeginLate()) {
        if (file_new_submenu.BeginLate())
          if (file_new_solution_item.BeginLate())
            if (file_new_solution_item.IsOn()) open_new_project_modal = true;
        file_new_submenu.EndEarly();

        if (file_open_submenu.BeginLate())
          if (CguiMenuItem("Repository...")) show_open_repository_modal = true;
        file_open_submenu.EndEarly();
      }
      file_menu.EndEarly();

      if (edit_menu.BeginLate()) {
        if (CguiMenuItem("Undo", "CTRL+Z")) cb_edit_undo();
        if (CguiMenuItem("Redo", "CTRL+Y")) cb_edit_redo();
        cgui::Separator();
        if (CguiMenuItem("Cut", "CTRL+X")) cb_edit_cut();
        if (CguiMenuItem("Copy", "CTRL+C")) cb_edit_copy();
        if (CguiMenuItem("Paste", "CTRL+V")) cb_edit_paste();
      }
      edit_menu.EndEarly();

      if (action_menu.BeginLate()) {
        if (CguiMenuItem("Generate")) cb_action_generate();
        if (CguiMenuItem("Build")) cb_action_build();
        if (CguiMenuItem("Run")) cb_action_run();
      }
      action_menu.EndEarly();

      if (project_menu.BeginLate()) {
        if (project_addfile_item.BeginLate()) cb_project_addfile();
        if (project_addactivefile_item.BeginLate()) cb_project_addactivefile();
        if (project_addexistingfile_item.BeginLate()) cb_project_addexistingfile();
        cgui::Separator();
        if (project_solutionproperties_item.BeginLate()) cb_project_solutionproperties();
        cgui::Separator();
        if (project_clonesolution_item.BeginLate()) cb_project_clonesolution();
      }
      project_menu.EndEarly();

      if (tools_menu.BeginLate())
        if (CguiMenuItem("C& AST Explorer")) cb_tools_astexplorer();
      tools_menu.EndEarly();
    }
    main_menu_bar.EndEarly();

    // This must be called here, in "global" scope or else modal wont appear.
    DisplayNewRepoModal(open_new_project_modal);
    DisplayOpenRepoModal(show_open_repository_modal);
  }

  void DisplayNewRepoModal(bool& is_on) {
    if (is_on) {
      ImGui::OpenPopup("Create New Project...");

      // Always center this window when appearing
      ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    }

    if (ImGui::BeginPopupModal("Create New Project...", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
      if (!new_repo_name_buffer_) new_repo_name_buffer_ = std::make_unique<string>("NewRepo");
      if (!new_repo_path_buffer_) new_repo_path_buffer_ = std::move(std::make_unique<string>("C:/"));

      ImGui::Text("Repository Name:");
      cgui::SameLine();
      ImGui::InputText("##repo_name", new_repo_name_buffer_.get());
      ImGui::Text("Repository Path:");
      cgui::SameLine();
      ImGui::InputText("##repo_path", new_repo_path_buffer_.get());

      if (CguiButton("Browse...")) {
        auto opendir = wpl::OpenFolderDlg();
        if (!opendir.empty()) *new_repo_path_buffer_ = opendir + "\\" + *new_repo_name_buffer_;
      }

      static expected<void, string> create_repo_res{std::unexpected<string>("Choose a project name and repo dir.")};
      if (CguiButton("OK", {100, 0})) {
        create_repo_res = cb_file_new_solution(*new_repo_path_buffer_, *new_repo_name_buffer_);
        if (create_repo_res) {
          ImGui::CloseCurrentPopup();
          is_on = false;
        }
      }
      if (!create_repo_res) ImGui::Text("Error: %s", create_repo_res.error().c_str());

      cgui::SameLine();

      if (ImGui::Button("Cancel", ImVec2(120, 0))) {
        ImGui::CloseCurrentPopup();
        is_on = false;
      }
      ImGui::EndPopup();
    }
  }

  void DisplayOpenRepoModal(bool& is_on) {
    if (is_on) {
      ImGui::OpenPopup("Open a repository...");
      ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    }

    if (ImGui::BeginPopupModal("Open a repository...", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
      if (!new_repo_path_buffer_) new_repo_path_buffer_ = std::make_unique<string>("C:/");
      CguiTextLabel("Repository Path:");
      cgui::SameLine();
      CguiTextInput("##repo_path", *new_repo_path_buffer_);
      // ImGui::InputText("##repo_path", new_repo_path_buffer_.get());

      if (CguiButton("Browse...")) {
        auto opendir = wpl::OpenFolderDlg();
        if (!opendir.empty()) *new_repo_path_buffer_ = opendir;
      }

      static expected<void, string> open_repo_res{std::unexpected<string>("Choose a project name and repo dir.")};

      if (CguiButton("OK", {100, 0})) {
        open_repo_res = cb_file_open_solution(*new_repo_path_buffer_);
        if (open_repo_res) {
          ImGui::CloseCurrentPopup();
          is_on = false;
        }
      }

      if (!open_repo_res) ImGui::Text("Error: %s", open_repo_res.error().c_str());

      cgui::SameLine();

      if (ImGui::Button("Cancel", ImVec2(120, 0))) {
        ImGui::CloseCurrentPopup();
        is_on = false;
      }
      ImGui::EndPopup();
    }
  }
};

struct FileEditor {
  // Properties
  CguiVec2 context_size;
  CguiNamedSubcontext editor_context;
  CguiTabBar editor_tab_bar;
  vector<CguiTabItem> open_file_tabs;
  vector<TextEditor> open_file_tab_text_inputs;
  vector<string> open_file_tab_text_buffers;
  FileEditor(const std::string& name, CguiVec2 context_size)
      : context_size(context_size),
        editor_context(CguiNamedSubcontext::Delayed(name, context_size)),
        editor_tab_bar(
            CguiTabBar("##file-tabs", {cgui::eTabBarFlags::ImGuiTabBarFlags_Reorderable}, cgui::kWidgetInitDelayed)) {}

  void Display() {  // Editor Subcontext
    editor_context.RequestSize(context_size);
    if (editor_context.BeginLate()) {
      if (editor_tab_bar.BeginLate()) {
        for (size_t idx = 0; auto& file_tab : open_file_tabs) {
          if (file_tab.BeginLate()) {
            open_file_tab_text_inputs[idx].Render(file_tab.Name().c_str(), {context_size.first, context_size.second});
          }
          file_tab.EndEarly();
          idx++;
        }
      }
      editor_tab_bar.EndEarly();
    }
    editor_context.EndEarly();
  }

  void AddTab(const string& tab_name, string& text_buffer) {
    if (std::find_if(open_file_tabs.begin(), open_file_tabs.end(),
                     [&tab_name](auto tab) { return tab.Name() == tab_name; }) != open_file_tabs.end()) {
      return;  // Tab already exists.
    }
    open_file_tabs.push_back(CguiTabItem(tab_name, CguiTabItemFlags(), cgui::kWidgetInitDelayed));
    open_file_tab_text_inputs.push_back(TextEditor{});
    open_file_tab_text_buffers.push_back(text_buffer);
    open_file_tab_text_inputs.back().SetText(text_buffer);
    // open_file_tab_text_inputs.push_back(
    //     CguiMultilineTextInput::Delayed(tab_name + "###textbox", text_buffer,
    //                                     cgui::kExpandWidgetToRemainingSpaceXY));
  }

  void PopTab() { open_file_tabs.pop_back(); }
};

struct SolutionExplorer {
  static constexpr LAMBDA xNullCallback = [](const stdfs::path&) {};
  using CallbackT = function<void(const stdfs::path&)>;

  // By default select file callback will store the selected file in
  // the local temp_file_buffer.
  function<void(const stdfs::path&)> select_file_callback{xNullCallback};
  function<void(stdfs::path&)> cb_update_root_dir{xNullCallback};

  // Edit menu
  function<void(const stdfs::path&)> cb_edit_open{xNullCallback};
  function<void(const stdfs::path&)> cb_edit_delete{xNullCallback};
  function<void(const stdfs::path&)> cb_edit_cut{xNullCallback};
  function<void(const stdfs::path&)> cb_edit_copy{xNullCallback};
  function<void(const stdfs::path&)> cb_edit_paste{xNullCallback};

  CguiVec2 requested_size;

  stdfs::path root_dir;
  std::string temp_file_buffer;

  // Widgets
  CguiNamedSubcontext solution_toolbar_context;
  CguiTabBar solution_toolbar_tab_bar{CguiTabBar::Delayed("solution_toolbar_tab_bar")};
  CguiTabItem solution_explorer_tab_item{CguiTabItem::Delayed("Solution View")};
  CguiDirectoryView dir_tree_view;

  inline void BeginRightClickContextMenu(const stdfs::path& p) const {
    // New Menu:
    if (CguiMenuItem("Open")) {
      cb_edit_open(p);
    }
    if (CguiMenuItem("Copy")) {
      cb_edit_copy(p);
    }
    if (CguiMenuItem("Paste")) {
      cb_edit_paste(p);
    }
    if (CguiMenuItem("Cut")) {
      cb_edit_cut(p);
    }
    if (CguiMenuItem("Delete")) {
      cb_edit_delete(p);
    }
  };

 public:
  SolutionExplorer(const CguiVec2& requested_size = {})
      : requested_size(requested_size),
        solution_toolbar_context(CguiNamedSubcontext::Delayed("Solution Toolbar", requested_size)),
        root_dir(std::filesystem::current_path()),
        dir_tree_view(
            root_dir,
            // callback for selectting an item
            [this](const stdfs::path& p) {
              temp_file_buffer = backend::LoadFileToStr(p.string());
              select_file_callback(p);
            },
            // callback for right clicking an item
            [this](const stdfs::path& p) {
              // New Menu:
              BeginRightClickContextMenu(p);
            },
            cgui::kWidgetInitDelayed) {}

  void Display() {
    solution_toolbar_context.RequestSize(requested_size);
    if (solution_toolbar_context.BeginLate()) {
      if (solution_toolbar_tab_bar.BeginLate()) {
        if (solution_explorer_tab_item.BeginLate()) {
          if (ImGui::BeginPopupContextWindow()) {
            if (ImGui::MenuItem("Add File")) {
              auto file_dir = wpl::SaveFileDlg(wpl::WideToMByte(dir_tree_view.root.c_str()).value_or("").c_str());
              if (!file_dir.empty())  // error in save dialog if empty
              {
                if (!stdfs::exists(file_dir)) {
                  std::ofstream ofs(file_dir);
                  ofs.close();
                }
              }
            }

            ImGui::EndPopup();
          }
          cb_update_root_dir(dir_tree_view.root);
          dir_tree_view.BeginLate();
        }
        solution_explorer_tab_item.EndEarly();
      }
      solution_toolbar_tab_bar.EndEarly();
    }
    solution_toolbar_context.EndEarly();
  }
};

struct HUD {
  //// Common 
  //caf::Context& ctx_;
  //caf::WinHints default_hints_{};
  //std::vector<std::pair<bool,sf::RenderWindow*>> windows_{};
  //Theme theme_{Theme::DefaultCidr()};  // UI theme. Theme must be applied to each window after creation.

  //// Predefined windows...
  //LoadingScreen loading_screen_{};     // Loading screen ui
  //std::vector<std::pair<bool, sf::RenderWindow*>>::iterator loading_screen_window_{windows_.end()};

  //// Callbacks
  //std::function<int()> cbGetCurrentAppState{};

  //TopMenuBar main_menu{};
  //FileEditor file_editor_interface{"Editor", {0, 0}};

  //SolutionExplorer repo_explorer{{0, 0}};
  //std::pair<AstExplorerInterface, bool> ast_explorer;
  //CguiWindow main_ide_context{"C&-IDE", false, cgui::WindowFlags{ImGuiWindowFlags_MenuBar, ImGuiWindowFlags_NoTitleBar},
  //                            cgui::kWidgetInitDelayed};

  //void Display(float xsize, float ysize) {
  //  cgui::SetNextWindowSize({xsize, ysize});
  //  cgui::SetNextWindowPos({0, 0});
  //  main_ide_context.BeginLate();
  //  main_menu.Display();
  //  file_editor_interface.context_size = {xsize * 0.75f, ysize * 0.75f};
  //  file_editor_interface.Display();
  //  cgui::SameLine();
  //  repo_explorer.solution_toolbar_context.RequestSize({xsize * 0.25f, ysize * 0.75f});
  //  repo_explorer.Display();
  //  main_ide_context.EndEarly();
  //  if (ast_explorer.second) {
  //    ast_explorer.first.Display();
  //  }
  //}
  //void Link(backend::AppModel& model) {};
  //void Activate() {};
  //HUD(caf::Context& gfx_context) : ctx_(gfx_context) {


  //  main_menu.cb_tools_astexplorer = [this]() {
  //    // Open the AST Explorer
  //    ast_explorer.second = !ast_explorer.second;
  //  };

  //  // Link the file explorer to the file editor.
  //  repo_explorer.select_file_callback = [this](const stdfs::path& p) {
  //    file_editor_interface.AddTab(p.string(), repo_explorer.temp_file_buffer);
  //  };
  //}


  ////std::vector<std::pair<bool, sf::RenderWindow*>>::iterator MakeWindow(std::string_view title) { 
  ////  windows_.push_back({false, &ctx_.MakeWindow(title.data(), default_hints_)});
  ////  theme_.Apply(ImGui::GetStyle());
  ////  return windows_.end()-1;
  ////}

  ////std::vector<std::pair<bool, sf::RenderWindow*>>::iterator MakeWindow(std::string_view title,
  ////                                                                     const caf::WinHints& hints) {
  ////  windows_.push_back({false, &ctx_.MakeWindow(title.data(), hints)});
  ////  theme_.Apply(ImGui::GetStyle());
  ////  return windows_.end() - 1;
  ////}
  ////
  //// Checks if a window is:
  //// 1. Created.
  //// 2. Was activated in CIDR.
  //// 3. The context is visible.
  //bool IsReadyToDraw(const std::vector<std::pair<bool, sf::RenderWindow*>>::iterator win_it) {
  //  return win_it != windows_.end() && win_it->second && win_it->second->isOpen();
  //};

  //// Display method for each app model state.
  //void DisplayLoadingScreen() {
  //  //if (loading_screen_window_ == windows_.end()) {
  //  //  caf::WinHints hints = default_hints_;  // Inherit defaults
  //  //  hints.InitialWidth = 256;
  //  //  hints.InitialHeight = 128;
  //  //  hints.NoTitleBar = true;
  //  //  hints.Hidden = false;
  //  //  auto win = MakeWindow("CIDE Launcher", hints);
  //  //  win->first = true;
  //  //  win->second->setPosition(sf::Vector2i{
  //  //      static_cast<int>((sf::VideoMode::getDesktopMode().width / 2) - (win->second->getSize().x / 2)),
  //  //      static_cast<int>((sf::VideoMode::getDesktopMode().height / 2) - (win->second->getSize().y / 2))});
  //  //  loading_screen_window_ = win;
  //  //}
  //  //if (IsReadyToDraw(loading_screen_window_)) loading_screen_.Display();
  //}

  //void DisplayInitWizard();
  //void DisplayLauncher();
  //void DisplayMainInterface();
  //void Display() {
  //  switch (cbGetCurrentAppState()) {
  //    case 0: {
  //      
  //      DisplayLoadingScreen();
  //      break;
  //    }
  //    case 1: {
  //      DisplayInitWizard();
  //      break;
  //    }
  //    case 2: {
  //      DisplayLauncher();
  //      break;
  //    }
  //    case 3: {
  //      DisplayMainInterface();
  //      break;
  //    }
  //    default: {
  //      // Do nothing.
  //      break;
  //    }
  //  }
  //
  //}

  //void Update() {};
  //void Draw() {};
};

class OutputLog {
  std::string buff_{};
  ImGuiTextFilter filter_{};
  std::vector<int> line_offsets_{{0}};  // Index to lines offset. We maintain this with AddLog() calls.
  bool enable_auto_scroll_{false};      // Keep scrolling if already at the bottom.
  CguiNamedSubcontext logger_context{
      CguiNamedSubcontext::Delayed("###logger_context", cgui::kExpandWidgetToRemainingSpaceXY)};

 public:
  void Clear() {
    buff_.clear();
    line_offsets_.clear();
    line_offsets_.push_back(0);
  }

  void AddLog(std::string_view data) {
    int old_size = buff_.size();
    buff_.append(data.data(), data.size());
    for (int new_size = buff_.size(); old_size < new_size; old_size++)
      if (buff_[old_size] == '\n') line_offsets_.push_back(old_size + 1);
  }

  void Display() {
    if (logger_context.BeginLate()) {
      // Options menu
      if (ImGui::BeginPopup("Options")) {
        ImGui::Checkbox("Auto-scroll", &enable_auto_scroll_);
        ImGui::EndPopup();
      }

      // Main window
      if (ImGui::Button("Options")) ImGui::OpenPopup("Options");
      ImGui::SameLine();
      bool clear = ImGui::Button("Clear");
      ImGui::SameLine();
      bool copy = ImGui::Button("Copy");
      ImGui::SameLine();
      filter_.Draw("Filter", -100.0f);

      ImGui::Separator();

      if (ImGui::BeginChild("scrolling", ImVec2(0, 0), ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar)) {
        if (clear) Clear();
        if (copy) ImGui::LogToClipboard();

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        const char* buf = buff_.data();
        const char* buf_end = buff_.data() + buff_.size();
        if (filter_.IsActive()) {
          // In this example we don't use the clipper when Filter is enabled.
          // This is because we don't have random access to the result of our filter.
          // A real application processing logs with ten of thousands of entries may want to store the result of
          // search/filter.. especially if the filtering function is not trivial (e.g. reg-exp).
          for (std::size_t line_no = 0; line_no < line_offsets_.size(); line_no++) {
            const char* line_start = buf + line_offsets_[line_no];
            const char* line_end =
                (line_no + 1ull < line_offsets_.size()) ? (buf + line_offsets_[line_no + 1ull] - 1) : buf_end;
            if (filter_.PassFilter(line_start, line_end)) ImGui::TextUnformatted(line_start, line_end);
          }
        } else {
          // The simplest and easy way to display the entire buffer:
          //   ImGui::TextUnformatted(buf_begin, buf_end);
          // And it'll just work. TextUnformatted() has specialization for large blob of text and will fast-forward
          // to skip non-visible lines. Here we instead demonstrate using the clipper to only process lines that are
          // within the visible area.
          // If you have tens of thousands of items and their processing cost is non-negligible, coarse clipping them
          // on your side is recommended. Using ImGuiListClipper requires
          // - A) random access into your data
          // - B) items all being the  same height,
          // both of which we can handle since we have an array pointing to the beginning of each line of text.
          // When using the filter (in the block of code above) we don't have random access into the data to display
          // anymore, which is why we don't use the clipper. Storing or skimming through the search result would make
          // it possible (and would be recommended if you want to search through tens of thousands of entries).
          ImGuiListClipper clipper;
          clipper.Begin(line_offsets_.size());
          while (clipper.Step()) {
            for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++) {
              const char* line_start = buf + line_offsets_[line_no];
              const char* line_end =
                  (line_no + 1ull < line_offsets_.size()) ? (buf + line_offsets_[line_no + 1ull] - 1) : buf_end;
              ImGui::TextUnformatted(line_start, line_end);
            }
          }
          clipper.End();
        }
        ImGui::PopStyleVar();

        // Keep up at the bottom of the scroll region if we were already at the bottom at the beginning of the frame.
        // Using a scrollbar or mouse-wheel will take away from the bottom edge.
        if (enable_auto_scroll_ && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) ImGui::SetScrollHereY(1.0f);
      }
      ImGui::EndChild();
    }
    logger_context.EndEarly();
  }
};

/// Launcher UI contains 4 tabs:
///  - Startup(initial tab)
///  - Config
///  - Install
///  - About
class Launcher {
  static constexpr cstring kAppTitleStr = "CIDR : Continuous Integrated Developer Repository";
  static constexpr cstring kAppVersionStr = "Version 0.0.0.0-alpha";
  static constexpr cstring kAppReleaseDateStr = "Released 2025/06/25";
  static constexpr cstring kAppCopyrightStr = "Copyright 2025 Sophia Silicon Gold Solutions Inc.";

  static constexpr cstring kAppInitWizardPrompt =
      "Looks like its your first time running CIDR on this device.\n"
      "Please choose initial application install paths:";
  static constexpr cstring kAppInitWizardStandardRadioButtonDesc =
      "Standard : Use operating system default directories.";
  static constexpr cstring kAppInitWizardPortableRadioButtonDesc =
      "Portable : Use subdirectories relative to application executable path.";
  static constexpr cstring kAppInitWizardCustomRadioButtonDesc =
      "Custom   : User defined directories. Non-existing directories will be created.";
  static constexpr cstring kAppInitWizardTableDesc =
      "Path List : Modify paths as needed before applying. Hover over path names for documentation.";

 public:
  using RecentSolutionInfo = vector<std::tuple<string_view, string_view, string_view>>;
  using SettingsTableData = vector<std::pair<string_view, string_view>>;
  void DisplayStartupTab() {
    if (startup_tab.BeginLate()) {
      if (CguiButton("New Solution")) {
        // Open a new solution
      }
      cgui::SameLine();
      if (CguiButton("Open Solution")) {
        // Open a solution
      }

      if (ImGui::BeginTable("project_history_table", 3)) {
        ImGui::TableSetupColumn("Name");
        ImGui::TableSetupColumn("Directory");
        ImGui::TableSetupColumn("Type");
        ImGui::TableHeadersRow();

        for (const auto& recent_proj : cb_get_recent_solutions()) {
          ImGui::TableNextRow();
          ImGui::TableNextColumn();
          ImGui::Text(get<0>(recent_proj).data());
          ImGui::TableNextColumn();
          ImGui::Text(get<1>(recent_proj).data());
          ImGui::TableNextColumn();
          ImGui::Text(get<2>(recent_proj).data());
        }
      }
      ImGui::EndTable();
    }
    startup_tab.EndEarly();
  }
  void DisplaySettingsTab(const sf::Vector2u& win_size) {
    if (settings_tab.BeginLate()) {
      // Split into two contexts:
      // Left is a tree list of categories and subcategories.
      CguiNamedSubcontext categories_pane{
          "Categories", cgui::CguiVec2{static_cast<float>(win_size.x / 4), cgui::kExpandWidgetToRemainingSpace()}};
      if (categories_pane.IsOn()) {
        CguiTreeNode root_category{"General"};
        if (root_category.IsOn()) {
          if (CguiButton("Test")) {
            // Open a solution
          }
        }
        root_category.EndEarly();
      }
      categories_pane.EndEarly();
      cgui::SameLine();
      // Right is the settings for the selected category - unique to each.
      // The right will usually be a Table with key-value pairs of strings.
      CguiNamedSubcontext options_pane{"Options", cgui::kExpandWidgetToRemainingSpaceXY};
      if (options_pane.IsOn()) {
        if (ImGui::BeginTable("host_options_table", 2, ImGuiTableFlags_SizingStretchProp)) {
          ImGui::TableSetupColumn("Parameter");
          ImGui::TableSetupColumn("Value");
          ImGui::TableHeadersRow();

          for (const auto& recent_proj : cb_get_general_settings()) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text(get<0>(recent_proj).data());
            ImGui::TableNextColumn();
            ImGui::Text(get<1>(recent_proj).data());
          }
          ImGui::EndTable();
        }
      }
      options_pane.EndEarly();
    }
    settings_tab.EndEarly();
  }
  void DisplayInstallTab() {
    if (install_tab.BeginLate()) {
      // Install and display wizard settings if required.

      // Install extensions from the web.

      // Install updates from the web.
    }
    install_tab.EndEarly();
  }
  void DisplayAboutTab() {
    if (about_tab.BeginLate()) {
      // General information about the IDE.

      // Website link.

      // Documentation link.

      // Donate link.
    }
    about_tab.EndEarly();
  }
  void DisplayInitWizard() {
    using namespace ImGui;
    if (init_wizard_context.BeginLate()) {
      ImVec2 textPos = GetCursorScreenPos(); // Scale title font up a bit to look cooler.
      GetWindowDrawList()->AddText(GetFont(), 24.0f, textPos, IM_COL32(0, 0, 0, 255), kAppTitleStr);
      Dummy(ImVec2(0, 24.0f));  // Advance cursor to avoid overlap
      Text(kAppVersionStr);
      Text(kAppReleaseDateStr);
      Text(kAppCopyrightStr);
      Separator();

      // Path preset options radio buttons...
      Text(kAppInitWizardPrompt);
      // ->[Standard] 
      if (RadioButton(kAppInitWizardStandardRadioButtonDesc, paths_preset_choice[0])) {
        paths_preset_choice.reset();
        paths_preset_choice[0] = true;
        auto gen_paths = backend::GetDefaultLaunchPaths(backend::GetCurrentUserId(), username_textbox_buff);
        launchfile_buff_ = gen_paths.first;
        tmp_launch_paths = gen_paths.second;
      }
      // ->[Portable]
      if (RadioButton(kAppInitWizardPortableRadioButtonDesc, paths_preset_choice[1])) {
        paths_preset_choice.reset();
        paths_preset_choice[1] = true;
        auto gen_paths = backend::GetDefaultLaunchPathsPortable(backend::GetCurrentUserId(), username_textbox_buff);
        launchfile_buff_ = gen_paths.first;
        tmp_launch_paths = gen_paths.second;
      }
      // ->[Custom]
      if (RadioButton(kAppInitWizardCustomRadioButtonDesc, paths_preset_choice[2])) {
        paths_preset_choice.reset();
        paths_preset_choice[2] = true;
      }
      Separator();

      // Launch paths table...
      Text(kAppInitWizardTableDesc);
      if (BeginTable("init_wizard_table", 2, ImGuiTableFlags_SizingStretchProp)) {
        TableSetupColumn("Configuration", 200.f);
        TableSetupColumn("Path");
        TableHeadersRow();
        // -> [Launch Paths File][NON-MODIFIABLE]
        TableNextRow();
        TableNextColumn();
        Text("Launch Paths File");
        if (IsItemHovered())
          SetTooltip(
              "Path to the initial launch paths file.\nMust be located in the binary or OS app data path.\n "
              "CIDR will load this file on startup to retrieve the app resources, app data and user\ndata "
              "directories.");
        TableNextColumn();
        CguiTextLabel{launchfile_buff_};

        // -> [App Resources Dir]
        TableNextRow();
        TableNextColumn();
        Text("Application Resources");
        if (IsItemHovered())
          SetTooltip(
              "Directory.\nMust contain binary data provided with the CIDR installation or source.\n"
              "CIDR will locate a CidrResources.json file within the directory describing the available "
              "resources.");
        TableNextColumn();
        ImGui::SetNextItemWidth(-1);
        InputText("###app_res_dir", &tmp_launch_paths.AppResourcesPath());
        SameLine();
        if (Button("...")) {
          auto opendir = wpl::OpenFolderDlg();
          if (!opendir.empty()) tmp_launch_paths.AppResourcesPath() = opendir;
        }

        // -> [App Data Dir]
        TableNextRow();
        TableNextColumn();
        Text("Application Data");
        if (IsItemHovered())
          SetTooltip("Directory.\nPath where CIDR will store and manage data specific to this device installation.");
        TableNextColumn();
        ImGui::SetNextItemWidth(-1);
        InputText("###app_data_dir", &tmp_launch_paths.AppDataPath());
        SameLine();
        if (Button("...")) {
          auto opendir = wpl::OpenFolderDlg();
          if (!opendir.empty()) tmp_launch_paths.AppDataPath() = opendir;
        }

        // -> [User Data Dir]
        TableNextRow();
        TableNextColumn();
        Text("User Data");
        if (IsItemHovered())
          SetTooltip("Directory.\nPath where CIDR will store and manage data specific to the current device user.\n");

        TableNextColumn();
        ImGui::SetNextItemWidth(-1);
        InputText("###user_data_dir", &tmp_launch_paths.UserDataPath());
        SameLine();
        if (Button("...")) {
          auto opendir = wpl::OpenFolderDlg();
          if (!opendir.empty()) tmp_launch_paths.UserDataPath() = opendir;
        }

        EndTable();
      }
      Separator();
      // User setup.
      // -> [User Name]

      // -> [Default Repo Dir]

      // -> [Package Install Prefix]

      Button("Exit", {100, 0});
      SameLine();
      SetCursorPosX(GetContentRegionAvail().x - 100);
      Button("Apply", {100, 0});
    }
    init_wizard_context.EndEarly();
  };
  void DisplayInitWizard2() { 
    first_launch_wizard.Display();
  };
  void Display(sf::RenderWindow& target_window) {
    auto win_size = target_window.getSize();
    cgui::SetNextWindowSize({static_cast<float>(win_size.x), static_cast<float>(win_size.y)});
    cgui::SetNextWindowPos({0, 0});
    if (main_context.BeginLate()) {
      // If this is the first time launching the app. Display the initial install wizard.
      if (cbIsFirstLaunch()) DisplayInitWizard2();
      // Else display the launcher.
      else {
        launcher_bar_context.RequestSize({static_cast<float>(win_size.x), static_cast<float>(win_size.y) / 3});
        if (launcher_bar_context.BeginLate()) {
          if (launcher_tab_bar.BeginLate()) {
            DisplayStartupTab();
            DisplaySettingsTab(win_size);
            DisplayInstallTab();
            DisplayAboutTab();
          }
          launcher_tab_bar.EndEarly();
        }
        launcher_bar_context.EndEarly();
        output_log.Display();  // Display the output log at the bottom of the launcher window.
      }
    }
    main_context.EndEarly();
  }

  Launcher() {
    // Add a log just to test the logger...
    output_log.AddLog("[C& IDR] Performing first launch initialization sequence.\n");
  };

 private:
  CguiWindow main_context{"C& IDE Launcher", false,
                          cgui::WindowFlags{/*ImGuiWindowFlags_MenuBar,*/ ImGuiWindowFlags_NoTitleBar},
                          cgui::kWidgetInitDelayed};

  CguiNamedSubcontext launcher_bar_context{
      CguiNamedSubcontext::Delayed("###launcher_toolbar", cgui::kExpandWidgetToRemainingSpaceXY)};
  CguiTabBar launcher_tab_bar{"###launcher_tab_bar", CguiTabBarFlags{}, cgui::kWidgetInitDelayed};

  CguiTabItem startup_tab{"Startup##launcher_tab0", CguiTabItemFlags(), cgui::kWidgetInitDelayed};
  CguiTabItem settings_tab{"Settings##launcher_tab1", CguiTabItemFlags(), cgui::kWidgetInitDelayed};
  CguiTabItem install_tab{"Install##launcher_tab2", CguiTabItemFlags(), cgui::kWidgetInitDelayed};
  CguiTabItem about_tab{"About##launcher_tab3", CguiTabItemFlags(), cgui::kWidgetInitDelayed};

  // First launch install wizard.
  CguiNamedSubcontext init_wizard_context{
      CguiNamedSubcontext::Delayed("###launcher_init_wizard", cgui::kExpandWidgetToRemainingSpaceXY)};
  std::array<std::string, 4> init_wizard_paths_buffer{"cidr/cidr-launch.json", "cidr/res", "cidr/app-data",
                                                      "cidr/user-data"};
  std::bitset<3> paths_preset_choice{0b100};
  std::string launchfile_buff_{""}; 
  data::UserLaunchPaths tmp_launch_paths{};
  std::string username_textbox_buff{""};
  OutputLog output_log{};
  FirstLaunchWizard first_launch_wizard{};
 public:
  function<bool(void)> cbIsFirstLaunch{[]() { return true; }};
  function<std::array<std::string, 4>(void)> cbGetLaunchPaths{[]() {
    return std::array<std::string, 4>{"C:/ProgramData/cidr/cidr-launch.json", "cidr/res",
                                      "C:/ProgramData/cidr/app-data", "C:/Users/Anton/AppData/Roaming/cidr/user-data"};

    return std::array<std::string, 4>{"cidr/cidr-launch.json", "cidr/res", "cidr/app-data", "cidr/user-data"};
  }};
  function<std::array<std::string, 4>(void)> cbGetLaunchPathsPortable{[]() {
    return std::array<std::string, 4>{"cidr/cidr-launch.json", "cidr/res", "cidr/app-data", "cidr/user-data"};
  }};

  function<RecentSolutionInfo(void)> cb_get_recent_solutions{[]() { return RecentSolutionInfo{}; }};

  function<SettingsTableData(void)> cb_get_general_settings{};
};

class UserInterface {
 public:
  //caf::Context ctx;

};


};  // namespace cidr::ui
/// @} // end of cidrlib_frontend

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
