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
/// @brief Launcher window for CIDR executable.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup cidrlib_frontend
/// @{

#pragma once
#include "../Backend.hpp"
#include "../Common.hpp"

namespace cidr::ui {

class FirstLaunchWizard {
  enum ePathPresetChoice : int {
    kStandard = 0,
    kPortable,
    kCustom,
  };

  enum eInputFields : int {
    kAppResourcesPath = 0,
    kAppDataPath,
    kUserDataPath,
    kUserName,
    kDefaultRepoDir,
  };

  static constexpr std::string_view kAppTitleStr = "CIDR : Continuous Integrated Developer Repository";
  static constexpr std::string_view kAppVersionStr = "Version 0.0.0.0-alpha";
  static constexpr std::string_view kAppReleaseDateStr = "Released 2025/06/25";
  static constexpr std::string_view kAppCopyrightStr = "Copyright 2025 Sophia Silicon Gold Solutions Inc.";

  static constexpr std::string_view kAppInitWizardPrompt =
      "Looks like its your first time running CIDR on this device.\n"
      "Please choose initial application install paths:";
  static constexpr std::string_view kAppInitWizardStandardRadioButtonDesc =
      "Standard : Use operating system default directories.";
  static constexpr std::string_view kAppInitWizardPortableRadioButtonDesc =
      "Portable : Use subdirectories relative to application executable path.";
  static constexpr std::string_view kAppInitWizardCustomRadioButtonDesc =
      "Custom   : User defined directories. Non-existing directories will be created.";
  static constexpr std::string_view kAppInitWizardTableDesc =
      "Path List : Modify paths as needed before applying. Hover over path names for documentation.";

  static constexpr std::string_view kAppInitWizardUserPrefDesc =
      "User Preferences : Set basic user preferences (optional).";

  static constexpr std::string_view kTooltipLaunchPathsFile =
      "Path to the initial launch paths file.\n"
      "Must be located in the binary or OS app data path.\n"
      "CIDR will load this file on startup to retrieve the app resources, app data and user data directories.";

  static constexpr std::string_view kTooltipAppResourcesDir =
      "Application resources directory.\n"
      "Must contain binary data provided with the CIDR installation or source.\n"
      "CIDR will locate a CidrResources.json file within the directory describing the available resources.";

  static constexpr std::string_view kTooltipAppDataDir =
      "Application data directory.\n"
      "Path where CIDR will store and manage data specific to this device installation.";

  static constexpr std::string_view kTooltipUserDataDir =
      "User data directory.\n"
      "Path where CIDR will store and manage data specific to the current device user.\n";

 public:
  FirstLaunchWizard()
      : subcontext_(
            CguiNamedSubcontext::Delayed("###FirstLaunchWizard_Subcontext", cgui::kExpandWidgetToRemainingSpaceXY)),
        text_label_(CguiFormattedTextLabel::Delayed(kAppVersionStr)),
        inputs_{},
        preset_choice_(ePathPresetChoice::kCustom),
        launchfile_buff_(""),
        are_paths_valid_("true"),
        paths_err_buff_("") {
    // kAppResourcesPath
    inputs_[eInputFields::kAppResourcesPath].label = "###AppResourcesPath";
    inputs_[eInputFields::kAppResourcesPath].validator = [this](std::string& data) -> bool {
      auto is_valid = backend::IsDirPathValid({inputs_[eInputFields::kAppResourcesPath].buffer.c_str(),
                                               inputs_[eInputFields::kAppResourcesPath].buffer.size()});
      if (!is_valid) {
        inputs_[eInputFields::kAppResourcesPath].error = is_valid.error().data;
        return false;
      } else
        return true;
    };

    // kAppDataPath
    inputs_[eInputFields::kAppDataPath].label = "###AppDataPath";
    inputs_[eInputFields::kAppDataPath].validator = [this](std::string& data) -> bool {
      auto is_valid = backend::IsDirPathValid(
          {inputs_[eInputFields::kAppDataPath].buffer.c_str(), inputs_[eInputFields::kAppDataPath].buffer.size()});
      if (!is_valid) {
        inputs_[eInputFields::kAppDataPath].error = is_valid.error().data;
        return false;
      } else
        return true;
    };

    // kUserDataPath
    inputs_[eInputFields::kUserDataPath].label = "###UserDataPath";
    inputs_[eInputFields::kUserDataPath].validator = [this](std::string& data) -> bool {
      auto is_valid = backend::IsDirPathValid(
          {inputs_[eInputFields::kUserDataPath].buffer.c_str(), inputs_[eInputFields::kUserDataPath].buffer.size()});
      if (!is_valid) {
        inputs_[eInputFields::kUserDataPath].error = is_valid.error().data;
        return false;
      } else
        return true;
    };

    // kUserName
    inputs_[eInputFields::kUserName].label = "###UserName";
    inputs_[eInputFields::kUserName].validator = [this](std::string& data) -> bool {
      if (data.empty()) {
        data = "user-" + backend::GetCurrentUserId();
        inputs_[eInputFields::kUserName].error = "User name is empty. Defaulting to 'user-<user_id>'.";
        return false;
      } else if (inputs_[eInputFields::kUserName].buffer.length() > 64) {
        inputs_[eInputFields::kUserName].buffer = inputs_[eInputFields::kUserName].buffer.substr(0, 64);
        inputs_[eInputFields::kUserName].error = "User name is too long. Maximum 64 characters.";
        return false;
      } else
        return true;
    };

    // kDefaultRepoDir
    inputs_[eInputFields::kDefaultRepoDir].label = "#DefaultRepoDir";
    inputs_[eInputFields::kDefaultRepoDir].validator = [this](std::string& data) -> bool {
      auto is_valid = backend::IsDirPathValid({inputs_[eInputFields::kDefaultRepoDir].buffer.c_str(),
                                               inputs_[eInputFields::kDefaultRepoDir].buffer.size()});
      if (!is_valid) {
        inputs_[eInputFields::kDefaultRepoDir].error = is_valid.error().data;
        return false;
      } else
        return true;
    };
  }

  void Display() {
    using namespace ImGui;
    if (subcontext_.BeginLate()) {
      ///////////////////////////////////////////////////////
      // App title...
      // Scale name title font up a bit, to look cooler.
      ImVec2 textPos = GetCursorScreenPos();
      GetWindowDrawList()->AddText(GetFont(), 24.0f, textPos, IM_COL32(0, 0, 0, 255), kAppTitleStr.data());
      Dummy(ImVec2(0, 24.0f));  // Advance cursor to avoid overlap
      text_label_.BeginLate();
      text_label_.Text(kAppReleaseDateStr);
      text_label_.BeginLate();
      text_label_.Text(kAppCopyrightStr);
      text_label_.BeginLate();
      cgui::Separator();

      ///////////////////////////////////////////////////////
      // Path preset options radio buttons...
      text_label_.Text(kAppInitWizardPrompt);
      text_label_.BeginLate();
      // ->[Standard]
      if (RadioButton(kAppInitWizardStandardRadioButtonDesc.data(), preset_choice_[ePathPresetChoice::kStandard])) {
        preset_choice_.reset();
        preset_choice_[ePathPresetChoice::kStandard] = true;
        auto gen_paths = backend::GetDefaultLaunchPaths(backend::GetCurrentUserId(), inputs_[eInputFields::kUserName].buffer);
        launchfile_buff_ = gen_paths.first;
        inputs_[eInputFields::kAppDataPath].buffer = gen_paths.second.AppDataPath();
        inputs_[eInputFields::kAppResourcesPath].buffer = gen_paths.second.AppResourcesPath();
        inputs_[eInputFields::kUserDataPath].buffer = gen_paths.second.UserDataPath();
        inputs_[eInputFields::kUserName].buffer = gen_paths.second.UserName();

      }
      // ->[Portable]
      if (RadioButton(kAppInitWizardPortableRadioButtonDesc.data(), preset_choice_[ePathPresetChoice::kPortable])) {
        preset_choice_.reset();
        preset_choice_[ePathPresetChoice::kPortable] = true;
        auto gen_paths = backend::GetDefaultLaunchPathsPortable(backend::GetCurrentUserId(),
                                                                inputs_[eInputFields::kUserName].buffer);
        launchfile_buff_ = gen_paths.first;
        inputs_[eInputFields::kAppDataPath].buffer = gen_paths.second.AppDataPath();
        inputs_[eInputFields::kAppResourcesPath].buffer = gen_paths.second.AppResourcesPath();
        inputs_[eInputFields::kUserDataPath].buffer = gen_paths.second.UserDataPath();
        inputs_[eInputFields::kUserName].buffer = gen_paths.second.UserName();
      }
      // ->[Custom]
      if (RadioButton(kAppInitWizardCustomRadioButtonDesc.data(), preset_choice_[ePathPresetChoice::kCustom])) {
        preset_choice_.reset();
        preset_choice_[ePathPresetChoice::kCustom] = true;
      }
      cgui::Separator();

      ///////////////////////////////////////////////////////
      // Launch paths table...
      text_label_.Text(kAppInitWizardTableDesc);
      text_label_.BeginLate();
      if (BeginTable("###paths_table", 2, ImGuiTableFlags_SizingStretchProp)) {
        TableSetupColumn("Configuration", 200.f);
        TableSetupColumn("Path");
        TableHeadersRow();

        // Launch Paths File NON-MODIFIABLE
        TableNextRow();
        TableNextColumn();
        text_label_.Text("Launch Paths File");
        text_label_.BeginLate();
        if (IsItemHovered()) SetTooltip(kTooltipLaunchPathsFile.data());
        TableNextColumn();
        text_label_.Text(launchfile_buff_);
        text_label_.BeginLate();

        // App Resources Dir
        TableNextRow();
        TableNextColumn();
        text_label_.Text("Application Resources");
        text_label_.BeginLate();
        if (IsItemHovered()) SetTooltip(kTooltipAppResourcesDir.data());
        TableNextColumn();
        //ImGui::SetNextItemWidth(cgui::kExpand);
        inputs_[eInputFields::kAppResourcesPath].Display();
        cgui::SameLine();
        if (Button("...###1")) {
          auto opendir = wpl::OpenFolderDlg();
          if (!opendir.empty()) inputs_[eInputFields::kAppResourcesPath].buffer = opendir;
        }

        // App Data Dir
        TableNextRow();
        TableNextColumn();
        text_label_.Text("Application Data");
        text_label_.BeginLate();
        if (IsItemHovered()) SetTooltip(kTooltipAppDataDir.data());
        TableNextColumn();
        //ImGui::SetNextItemWidth(cgui::kExpand);
        inputs_[eInputFields::kAppDataPath].Display();
        SameLine();
        if (Button("...###2")) {
          auto opendir = wpl::OpenFolderDlg();
          if (!opendir.empty()) inputs_[eInputFields::kAppDataPath].buffer = opendir;
        }

        // User Data Dir
        TableNextRow();
        TableNextColumn();
        Text("User Data");
        if (IsItemHovered()) SetTooltip(kTooltipUserDataDir.data());
        TableNextColumn();
        //ImGui::SetNextItemWidth(cgui::kExpand);
        inputs_[eInputFields::kUserDataPath].Display();
        SameLine();
        if (Button("...###3")) {
          auto opendir = wpl::OpenFolderDlg();
          if (!opendir.empty()) inputs_[eInputFields::kUserDataPath].buffer = opendir;
        }
        EndTable();
      }
      Separator();

      ///////////////////////////////////////////////////////
      // User preferences ...
      text_label_.Text(kAppInitWizardUserPrefDesc);
      text_label_.BeginLate();

      text_label_.Text("User Name : ");
      text_label_.BeginLate();
      SameLine();
      ImGui::SetNextItemWidth(cgui::kExpand);
      inputs_[eInputFields::kUserName].Display();

      text_label_.Text("Default Repository Path : ");
      text_label_.BeginLate();
      SameLine();
      ImGui::SetNextItemWidth(cgui::kExpand);
      inputs_[eInputFields::kDefaultRepoDir].Display();
      ///////////////////////////////////////////////////////
      /// Controls ...
      if (Button("Exit", {100, 0})) ExitProgram();

      SameLine();
      if (!are_paths_valid_) {
        text_label_.Text(paths_err_buff_);
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 122));
        text_label_.BeginLate();
        ImGui::PopStyleColor();
      }
      SameLine();

      SetCursorPosX(ImGui::GetWindowWidth() - 101);
      if (Button("Apply", {100, 0})) {
        // We should validate the paths here. If valid continue to launcher.
        // Otherwise we should highlight the invalid paths. Add an error message.
        // And remain in the wizard. Prompt user to change paths.
        are_paths_valid_ = this->ValidateAllPaths();
        if (are_paths_valid_) ContinueToLauncher();
        // else total error will be drawn before this button on the next frame.
      }
    }
    subcontext_.EndEarly();
  }

 private:
  CguiNamedSubcontext subcontext_;
  CguiFormattedTextLabel text_label_;
  std::array<CguiValidatedTextInput, 5> inputs_;

  std::bitset<3> preset_choice_;  // Radio button choices for path presets.
//  data::UserLaunchPaths tmp_launch_paths;
  std::string launchfile_buff_;
  bool are_paths_valid_ = false;
  std::string paths_err_buff_;
  bool ValidateAllPaths() {
    return true;
  };  // The a thourough validation to run after the user has pressed 'Apply' button.
  void ExitProgram() {};
  void ContinueToLauncher() {};
};

}  // namespace cidr::ui

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
