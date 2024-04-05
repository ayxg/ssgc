#pragma once
#include <filesystem>
#include <functional>
#include <string>
#include <vector>

#include "cide_backend.h"
#include "imgui_interface.h"

namespace cide::ui {

namespace fs = std::filesystem;
using std::function;
using std::string;
using std::vector;

class CideTopMenuBarInterface;
class CideFileEditorInterface;
class CideSolutionToolbarInterface;
class CideUserInterface;

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
  vector<CguiTabItem> open_file_tabs;
  vector<CguiMultilineTextInput> open_file_tab_text_inputs;

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

  void AddTab(const string& tab_name, string& text_buffer) {
    open_file_tabs.push_back(
        CguiTabItem(tab_name, CguiTabItemFlags(), cgui::kWidgetInitDelayed));
    open_file_tab_text_inputs.push_back(
        CguiMultilineTextInput::Delayed(tab_name + "###textbox", text_buffer,
                                        cgui::kExpandWidgetToRemainingSpaceXY));
  }

  void PopTab() { open_file_tabs.pop_back(); }
};

struct CideSolutionToolbarInterface {
  static constexpr LAMBDA xNullCallback = [](const fs::path&) {};
  using CallbackT = function<void(const fs::path&)>;

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

  fs::path root_dir;
  std::string temp_file_buffer;

  // Widgets
  CguiNamedSubcontext solution_toolbar_context;
  CguiTabBar solution_toolbar_tab_bar{
      CguiTabBar::Delayed("solution_toolbar_tab_bar")};
  CguiTabItem solution_explorer_tab_item{CguiTabItem::Delayed("Solution View")};
  CguiDirectoryView dir_tree_view;

  inline void BeginRightClickContextMenu(const fs::path& p) const {
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
            [this](const fs::path& p) {
              temp_file_buffer = backend::LoadFileToStr(p.string());
              select_file_callback(p);
            },
            // callback for right clicking an item
            [this](const fs::path& p) {
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
    cgui::SetNextWindowSize({kWindowWidth, kWindowHeight});
    cgui::SetNextWindowPos({0, 0});
    main_ide_context.BeginLate();
    top_menu_bar_interface.Display();
    file_editor_interface.Display();
    cgui::SameLine();
    solution_toolbar_interface.Display();
    main_ide_context.EndEarly();
  }
};
};  // namespace cide::ui