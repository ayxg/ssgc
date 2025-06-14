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
#include "Common.hpp"
#include "Backend.hpp"

#include "UserInterface/AstExplorer.hpp"
#include "UserInterface/CppTestExplorer.hpp"
#include "UserInterface/TextEditor.hpp"
// clang-format on

namespace cidr::ui {
/// @addtogroup cand_cide_frontend
/// @{

// Main CIDE Interface
class TopMenuBar;
class FileEditor;
class SolutionExplorer;
class CideUserInterface;

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
  CguiMenuItem file_new_solution_item{"Solution", "", true,
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

  function<std::expected<void, string>(const string&, const string&)>
      cb_file_new_solution{};
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
        if (project_addexistingfile_item.BeginLate())
          cb_project_addexistingfile();
        cgui::Separator();
        if (project_solutionproperties_item.BeginLate())
          cb_project_solutionproperties();
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
      ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(),
                              ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    }

    if (ImGui::BeginPopupModal("Create New Project...", NULL,
                               ImGuiWindowFlags_AlwaysAutoResize)) {
      if (!new_repo_name_buffer_)
        new_repo_name_buffer_ = std::make_unique<string>("NewRepo");
      if (!new_repo_path_buffer_)
        new_repo_path_buffer_ = std::move(std::make_unique<string>("C:/"));

      ImGui::Text("Repository Name:");
      cgui::SameLine();
      ImGui::InputText("##repo_name", new_repo_name_buffer_.get());
      ImGui::Text("Repository Path:");
      cgui::SameLine();
      ImGui::InputText("##repo_path", new_repo_path_buffer_.get());

      if (CguiButton("Browse...")) {
        auto opendir = wpl::OpenFolderDlg();
        if (!opendir.empty())
          *new_repo_path_buffer_ = opendir + "\\" + *new_repo_name_buffer_;
      }

      static expected<void, string> create_repo_res{
          std::unexpected<string>("Choose a project name and repo dir.")};
      if (CguiButton("OK", {100, 0})) {
        create_repo_res = cb_file_new_solution(*new_repo_path_buffer_,
                                               *new_repo_name_buffer_);
        if (create_repo_res) {
          ImGui::CloseCurrentPopup();
          is_on = false;
        }
      }
      if (!create_repo_res)
        ImGui::Text("Error: %s", create_repo_res.error().c_str());

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
      ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(),
                              ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    }

    if (ImGui::BeginPopupModal("Open a repository...", NULL,
                               ImGuiWindowFlags_AlwaysAutoResize)) {
      if (!new_repo_path_buffer_)
        new_repo_path_buffer_ = std::make_unique<string>("C:/");
      CguiTextLabel("Repository Path:");
      cgui::SameLine();
      CguiTextInput("##repo_path", *new_repo_path_buffer_);
      // ImGui::InputText("##repo_path", new_repo_path_buffer_.get());

      if (CguiButton("Browse...")) {
        auto opendir = wpl::OpenFolderDlg();
        if (!opendir.empty()) *new_repo_path_buffer_ = opendir;
      }

      static expected<void, string> open_repo_res{
          std::unexpected<string>("Choose a project name and repo dir.")};

      if (CguiButton("OK", {100, 0})) {
        open_repo_res = cb_file_open_solution(*new_repo_path_buffer_);
        if (open_repo_res) {
          ImGui::CloseCurrentPopup();
          is_on = false;
        }
      }

      if (!open_repo_res)
        ImGui::Text("Error: %s", open_repo_res.error().c_str());

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
        editor_tab_bar(CguiTabBar(
            "##file-tabs", {cgui::eTabBarFlags::ImGuiTabBarFlags_Reorderable},
            cgui::kWidgetInitDelayed)) {}

  void Display() {  // Editor Subcontext
    editor_context.RequestSize(context_size);
    if (editor_context.BeginLate()) {
      if (editor_tab_bar.BeginLate()) {
        for (size_t idx = 0; auto& file_tab : open_file_tabs) {
          if (file_tab.BeginLate()) {
            open_file_tab_text_inputs[idx].Render(
                file_tab.Name().c_str(),
                {context_size.first, context_size.second});
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
                     [&tab_name](auto tab) {
                       return tab.Name() == tab_name;
                     }) != open_file_tabs.end()) {
      return;  // Tab already exists.
    }
    open_file_tabs.push_back(
        CguiTabItem(tab_name, CguiTabItemFlags(), cgui::kWidgetInitDelayed));
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
  CguiTabBar solution_toolbar_tab_bar{
      CguiTabBar::Delayed("solution_toolbar_tab_bar")};
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
        solution_toolbar_context(
            CguiNamedSubcontext::Delayed("Solution Toolbar", requested_size)),
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
              auto file_dir =
                  wpl::SaveFileDlg(wpl::WideToMByte(dir_tree_view.root.c_str())
                                       .value_or("")
                                       .c_str());
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
  caf::Context& gfx_context_;
  TopMenuBar main_menu{};
  FileEditor file_editor_interface{"Editor", {0, 0}};

  SolutionExplorer repo_explorer{{0, 0}};
  std::pair<AstExplorerInterface, bool> ast_explorer;
  CguiWindow main_ide_context{
      "C&-IDE", false,
      cgui::WindowFlags{ImGuiWindowFlags_MenuBar, ImGuiWindowFlags_NoTitleBar},
      cgui::kWidgetInitDelayed};

  void Display(float xsize, float ysize) {
    cgui::SetNextWindowSize({xsize, ysize});
    cgui::SetNextWindowPos({0, 0});
    main_ide_context.BeginLate();
    main_menu.Display();
    file_editor_interface.context_size = {xsize * 0.75f, ysize * 0.75f};
    file_editor_interface.Display();
    cgui::SameLine();
    repo_explorer.solution_toolbar_context.RequestSize(
        {xsize * 0.25f, ysize * 0.75f});
    repo_explorer.Display();
    main_ide_context.EndEarly();
    if (ast_explorer.second) {
      ast_explorer.first.Display();
    }
  }

  HUD(caf::Context& gfx_context) : gfx_context_(gfx_context) {
    main_menu.cb_tools_astexplorer = [this]() {
      // Open the AST Explorer
      ast_explorer.second = !ast_explorer.second;
    };

    // Link the file explorer to the file editor.
    repo_explorer.select_file_callback = [this](const stdfs::path& p) {
      file_editor_interface.AddTab(p.string(), repo_explorer.temp_file_buffer);
    };
  }
};

/// Launcher UI contains 4 tabs:
///  - Startup(initial tab)
///  - Config
///  - Install
///  - About
class Launcher {
 public:
  using RecentSolutionInfo =
      vector<std::tuple<string_view, string_view, string_view>>;
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
          "Categories", cgui::CguiVec2{static_cast<float>(win_size.x / 4),
                                       cgui::kExpandWidgetToRemainingSpace()}};
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
      CguiNamedSubcontext options_pane{"Options",
                                       cgui::kExpandWidgetToRemainingSpaceXY};
      if (options_pane.IsOn()) {
        if (ImGui::BeginTable("host_options_table", 2,
                              ImGuiTableFlags_SizingStretchProp)) {
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

  void Display(sf::RenderWindow& target_window) {
    auto win_size = target_window.getSize();
    cgui::SetNextWindowSize(
        {static_cast<float>(win_size.x), static_cast<float>(win_size.y)});
    cgui::SetNextWindowPos({0, 0});
    if (main_context.BeginLate()) {
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
    }
    main_context.EndEarly();
  }
  Launcher() = default;

 private:
  CguiWindow main_context{
      "C& IDE Launcher", false,
      cgui::WindowFlags{ImGuiWindowFlags_MenuBar, ImGuiWindowFlags_NoTitleBar},
      cgui::kWidgetInitDelayed};

  CguiNamedSubcontext launcher_bar_context{CguiNamedSubcontext::Delayed(
      "###launcher_toolbar", cgui::kExpandWidgetToRemainingSpaceXY)};

  CguiTabBar launcher_tab_bar{"###launcher_tab_bar", CguiTabBarFlags{},
                              cgui::kWidgetInitDelayed};

  CguiTabItem startup_tab{"Startup##launcher_tab0", CguiTabItemFlags(),
                          cgui::kWidgetInitDelayed};
  CguiTabItem settings_tab{"Settings##launcher_tab1", CguiTabItemFlags(),
                           cgui::kWidgetInitDelayed};
  CguiTabItem install_tab{"Install##launcher_tab2", CguiTabItemFlags(),
                          cgui::kWidgetInitDelayed};
  CguiTabItem about_tab{"About##launcher_tab3", CguiTabItemFlags(),
                        cgui::kWidgetInitDelayed};

 public:
  function<RecentSolutionInfo(void)> cb_get_recent_solutions{
      []() { return RecentSolutionInfo{}; }};

  function<SettingsTableData(void)> cb_get_general_settings{};
};

};  // namespace cide::ui
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
