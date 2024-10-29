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
/// @brief CIDE AST Explorer Widget.
///////////////////////////////////////////////////////////////////////////////

#pragma once
// CIDE Common Defs
#include "cide_common.h"
// CIDE Backend
#include "cide_backend.h"
// ImGui
#include "cgui.hpp"
namespace cide::ui {
/// @addtogroup cand_cide_frontend
/// @{

// Popup Windows
class CideTestExplorerInterface {
 public:
  // Structure stored for each callback set added to the test explorer.
  // The callback does not neccesarily have to be a minitest test case.
  // Only requirement is that it returns a bool and takes no arguments.
  class TestModuleResult {
    friend class CideTestExplorerInterface;
   public:
    // Run the scase and store the result in this class.
    void Run() { is_test_passed_ = test_case_(); }

    bool IsEnabled() const { return *run_this_test; }

    bool IsPassed() const { return is_test_passed_; }

    const string& Name() const { return test_module_name_; }

    // Enable or disable the test case.
    void SetEnabled(bool value = true) { *run_this_test = value; }

   public:
    TestModuleResult(const string& test_module_name, bool is_test_passed,
                     BoolCallbackT test_case)
        : test_module_name_(test_module_name),
          is_test_passed_(is_test_passed),
          test_case_(test_case) {}
   private:
    //! Only use when required for ImGui Callbacks.
    bool* GetRawEnableTestPtr() { return run_this_test; }

    bool is_test_passed_{false};
    BoolCallbackT test_case_{xNullBoolCallback};
    bool* run_this_test = new (bool)(false); 
    string test_module_name_{""};
  };

 public:
  void Display() {
    test_explorer_window.BeginLate();
    window_width = test_explorer_window.QueryWidth();
    window_height = test_explorer_window.QueryHeight();
    DisplayToolbar();
    cgui::Separator();
    DisplayModulesTable();
    cgui::SameLine();
    DisplayChecksTable();
    test_explorer_window.EndEarly();
  }

  void RegisterTestCase(BoolCallbackT test_case,
                        const std::string& test_module_name) {
    registered_test_cases.push_back(
        TestModuleResult{test_module_name, false, test_case});
  }

 private:
  inline void RunEnabledModuleTests() {
    for (auto& test_case : registered_test_cases) {
      if (test_case.IsEnabled()) test_case.Run();
    }
  }

  inline void FillModulesTableFailRowData(size_t& style_id,
                                          TestModuleResult& t_module) {
    ImGui::TableNextRow();
    ImGui::TableNextColumn();
    ImGui::Checkbox((std::string("Run##") + std::to_string(style_id)).c_str(),
                    t_module.GetRawEnableTestPtr());
    ImGui::SameLine();
    ImGui::PushID(style_id);
    style_id++;
    ImGui::PushStyleColor(ImGuiCol_Button,
                          (ImVec4)ImColor::HSV(1.0f, .75f, .75f));
    ImGui::Button("Fail");
    ImGui::PopStyleColor(1);
    ImGui::PopID();
    ImGui::TableNextColumn();
    ImGui::Text(t_module.Name().c_str());
  }

  inline void FillModulesTablePassRowData(size_t& style_id,
                                          TestModuleResult& t_module) {
    ImGui::TableNextRow();
    ImGui::TableNextColumn();
    ImGui::Checkbox((std::string("Run##") + std::to_string(style_id)).c_str(),
                    t_module.GetRawEnableTestPtr());
    ImGui::SameLine();
    ImGui::PushID(style_id);
    style_id++;
    ImGui::PushStyleColor(ImGuiCol_Button,
                          (ImVec4)ImColor::HSV(0.4f, .75f, .75f));
    ImGui::Button("Pass");
    ImGui::PopStyleColor(1);
    ImGui::PopID();
    ImGui::TableNextColumn();
    ImGui::Text(t_module.Name().c_str());
  }

  inline void FillChecksTableFailRowData(size_t& style_id,
                                         const minitest::TestResult& t_case) {
    ImGui::TableNextRow();
    ImGui::TableNextColumn();
    ImGui::PushID(style_id);
    style_id++;
    ImGui::PushStyleColor(ImGuiCol_Button,
                          (ImVec4)ImColor::HSV(1.0f, .75f, .75f));
    ImGui::Button("Fail");
    ImGui::PopStyleColor(1);
    ImGui::PopID();
    ImGui::TableNextColumn();
    ImGui::Text(t_case.test_name.c_str());

    ImGui::TableNextColumn();
    ImGui::Text(t_case.test_case_name.c_str());

    ImGui::TableNextColumn();
    ImGui::Text(t_case.log.c_str());

    ImGui::TableNextColumn();
    std::string temp_s;
    temp_s +=
        std::filesystem::path(t_case.location.file_name()).filename().string();
    temp_s += " [Line:" + std::to_string(t_case.location.line()) +
              "] [Col:" + std::to_string(t_case.location.column()) + "]";
    ImGui::TextWrapped(temp_s.c_str());
  }

  inline void FillChecksTablePassRowData(size_t& style_id,
                                         const minitest::TestResult& t_case) {
    ImGui::TableNextRow();
    ImGui::TableNextColumn();
    ImGui::PushID(style_id);
    style_id++;
    ImGui::PushStyleColor(ImGuiCol_Button,
                          (ImVec4)ImColor::HSV(0.4f, .75f, .75f));
    ImGui::Button("Pass");
    ImGui::PopStyleColor(1);
    ImGui::PopID();
    ImGui::TableNextColumn();
    ImGui::Text(t_case.test_name.c_str());

    ImGui::TableNextColumn();
    ImGui::Text(t_case.test_case_name.c_str());

    ImGui::TableNextColumn();
    ImGui::Text(t_case.log.c_str());

    ImGui::TableNextColumn();
    std::string temp_s;
    temp_s +=
        std::filesystem::path(t_case.location.file_name()).filename().string();
    temp_s += " [Line:" + std::to_string(t_case.location.line()) +
              "] [Col:" + std::to_string(t_case.location.column()) + "]";
    ImGui::TextWrapped(temp_s.c_str());
  }

  void DisplayToolbar() {
    toolbar_context.RequestSize({window_width, window_height * 0.1f});
    if (toolbar_context.BeginLate()) {
      if (run_all_tests_button.BeginLate()) {
        RunEnabledModuleTests();
      }
      cgui::SameLine();
      if (flush_results_button.BeginLate()) {
        minitest::FlushTestResults();
      }
    }
    toolbar_context.EndEarly();
  }

  void DisplayModulesTable() {
    modules_context.RequestSize({window_width * 0.30f, window_height});
    if (modules_context.BeginLate()) {
      if (ImGui::BeginTable("test-explorer-modules-table", 2, 1)) {
        int flags = 0;
        flags |= ImGuiTableFlags_SizingFixedFit;
        ImGui::TableSetupColumn("Status", flags);
        ImGui::TableSetupColumn("Module", flags);
        ImGui::TableHeadersRow();
        for (size_t style_id = 0; auto& test_case : registered_test_cases) {
          if (test_case.IsPassed()) {
            FillModulesTablePassRowData(style_id, test_case);
          } else {
            FillModulesTableFailRowData(style_id, test_case);
          }
        }
        ImGui::EndTable();
      }
    }
    modules_context.EndEarly();
  }

  void DisplayChecksTable() {
    test_results_context.RequestSize({window_width * 0.70f, window_height});
    if (test_results_context.BeginLate()) {
      if (ImGui::BeginTable("test-explorer-checks-table", 5, 1)) {
        int flags = 0;
        flags |= ImGuiTableFlags_SizingFixedFit;
        ImGui::TableSetupColumn("Status", flags);
        ImGui::TableSetupColumn("Test", flags);
        ImGui::TableSetupColumn("Test Case", flags);
        ImGui::TableSetupColumn("Message", flags);
        ImGui::TableSetupColumn("Location");
        ImGui::TableHeadersRow();

        for (size_t style_id = 0; const auto& test_case : MINITESTS_RECORDED) {
          if (test_case.is_test_passed) {
            FillChecksTablePassRowData(style_id, test_case);
          } else {
            FillChecksTableFailRowData(style_id, test_case);
          }
        }
        ImGui::EndTable();
      }
    }
    test_results_context.EndEarly();
  }

 private:
  // local storage to avoid multiple calls to ImGui::GetWindowSize
  float window_width;
  float window_height;

  vector<TestModuleResult> registered_test_cases{};
  CguiWindow test_explorer_window = CguiWindow::Delayed("Test Explorer");
  CguiButton run_all_tests_button{
      "Run All Tests", {}, cgui::kWidgetInitDelayed};
  CguiButton flush_results_button{
      "Flush Results", {}, cgui::kWidgetInitDelayed};

  CguiNamedSubcontext toolbar_context =
      CguiNamedSubcontext::Delayed("testtoolbar");

  CguiNamedSubcontext modules_context =
      CguiNamedSubcontext::Delayed("testmodules");
  CguiNamedSubcontext test_results_context =
      CguiNamedSubcontext::Delayed("testchecks");
};

/// @} // end of cand_cide_frontend

}  // namespace cide::ui

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
