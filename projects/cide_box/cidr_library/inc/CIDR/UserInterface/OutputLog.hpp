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
// clang-format on

namespace cidr::ui {
/// @addtogroup cand_cide_frontend
/// @{

class OutputLog {
  std::string buff_{};
  ImGuiTextFilter filter_{};
  std::vector<int> line_offsets_{{0}};  // Index to lines offset. We maintain this with AddLog() calls.
  bool enable_auto_scroll_{false};      // Keep scrolling if already at the bottom.
  CguiNamedSubcontext logger_context{
      CguiNamedSubcontext::Delayed("###logger_context", cgui::kExpandWidgetToRemainingSpaceXY)};
  std::function<std::string_view()> cb_get_log_data{
      []() -> std::string_view { return ""; }};  // Callback to get log data.
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

  void Display(sf::RenderWindow& target_window) {
    auto win_size = target_window.getSize();
    cgui::SetNextWindowSize({static_cast<float>(win_size.x), static_cast<float>(win_size.y)});
    cgui::SetNextWindowPos({0, 0});
    if (main_context.BeginLate()) {
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
    main_context.EndEarly();
  }
  Launcher() {
    // Add a log just to test the logger...
    output_log.AddLog("[C& IDR] Performing first launch initialization sequence.\n");
  };

 private:
  CguiWindow main_context{"C& IDE Launcher", false,
                          cgui::WindowFlags{ImGuiWindowFlags_MenuBar, ImGuiWindowFlags_NoTitleBar},
                          cgui::kWidgetInitDelayed};

  CguiNamedSubcontext launcher_bar_context{
      CguiNamedSubcontext::Delayed("###launcher_toolbar", cgui::kExpandWidgetToRemainingSpaceXY)};

  CguiTabBar launcher_tab_bar{"###launcher_tab_bar", CguiTabBarFlags{}, cgui::kWidgetInitDelayed};

  CguiTabItem startup_tab{"Startup##launcher_tab0", CguiTabItemFlags(), cgui::kWidgetInitDelayed};
  CguiTabItem settings_tab{"Settings##launcher_tab1", CguiTabItemFlags(), cgui::kWidgetInitDelayed};
  CguiTabItem install_tab{"Install##launcher_tab2", CguiTabItemFlags(), cgui::kWidgetInitDelayed};
  CguiTabItem about_tab{"About##launcher_tab3", CguiTabItemFlags(), cgui::kWidgetInitDelayed};
  OutputLog output_log{};

 public:
  function<RecentSolutionInfo(void)> cb_get_recent_solutions{[]() { return RecentSolutionInfo{}; }};

  function<SettingsTableData(void)> cb_get_general_settings{};
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
