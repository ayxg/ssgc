///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2025 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: [CIDR] C& integrated development repository library
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup cidrlib
/// @brief [SOURCE] Main entry point for the CIDR library.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup cidrlib
/// @{

// clang-format off
#include "CIDR/Main.hpp"

#include "cxxx.hpp"
#include "caf.hpp"
#include "cgui.hpp"

#include "CIDR/Common.hpp"
#include "CIDR/Backend.hpp"
#include "CIDR/UserInterface.hpp"

// Unit Testing Framework
// Makes mini-test record all test results, even if they pass.
// For the ui test explorer to work.
#define MINITEST_CONFIG_RECORD_ALL
#include "minitest.hpp"
// clang-format on


// !TEMPORARY FIX!
// Weird Glitch with ImGui.
// When moving around the window, the updated imgui.ini will cause a crash
// For now im clearing the imgui.ini file on startup.
// This is a temporary fix. Cannot find the root cause.
// Error only happens if a window is moved around. When the project is reloaded.
// It crashes saying that endChild() or End() wasn't called..
// static int gFlushImGuiIniFile = []() {
//  std::ofstream ofs;
//  ofs.open("imgui.ini", std::ofstream::out | std::ofstream::trunc);
//  ofs.close();
//  return 0;
//}();

namespace cidr {
int MainGui(int argc, char** argv) {
  caf::WinHints hints;
  hints.EnableImGui = 1;
  hints.FrameLimit = 60;

  caf::Context context;
  auto& window = context.MakeWindow("CIDE", hints);

  cidr::backend::IdeModel ide_model;
  auto ide_model_init = ide_model.Init();
  if (!ide_model_init) {
    std::cout << "Failed to load ide params." << std::endl;
    return EXIT_FAILURE;
  }

  // The HUD is tightly linked to the GFX context. And may control multiple
  // windows.
  cidr::ui::HUD hud{context};

  // The IDE model is loosely linked to the ui callback methods.
  // Link backend to HUD:
  hud.repo_explorer.root_dir = ide_model.active_repo_.solution_path;

  hud.main_menu.cb_action_generate = [&ide_model] { ide_model.ExtCallGenerationStep(); };
  hud.main_menu.cb_action_build = [&ide_model] { ide_model.ExtCallBuildStep(); };
  hud.main_menu.cb_action_run = [&ide_model] { ide_model.ExtCallRunStep(); };

  hud.repo_explorer.cb_update_root_dir = [&ide_model](std::filesystem::path& path) {
    ide_model.RefreshCurrentRepoRootDir(path);
  };

  hud.main_menu.cb_file_new_solution = [&ide_model](const std::string& dir, const std::string& name) {
    return ide_model.NewRepo(dir, name);
  };

  //// Add another window
  // auto& launcher_window = context.MakeWindow("CIDE Launcher", hints);

  // host_params.Load();
  // cidr::ui::Launcher cide_launcher_ui;
  // cidr::ui::Launcher::SettingsTableData host_table_data;
  // for (auto i = 0; i < host_params.ParamCount(); i++) {
  //   host_table_data.push_back(
  //       {cidr::backend::HostParams::eHostVarToOptStr(
  //            static_cast<cidr::backend::eHostVar>(i)),
  //        host_params.ViewParam(static_cast<cidr::backend::eHostVar>(i))});
  // }
  // cide_launcher_ui.callback_get_general_settings = [&host_table_data] {
  //   return host_table_data;
  // };

  //  // Setup Imgui Fonts
  // #define FONT_MATERIALDESIGN_ICON_MIN 0xe000
  // #define FONT_MATERIALDESIGN_ICON_MAX 0xf8ff
  //  static const ImWchar glyph_ranges[] = {FONT_MATERIALDESIGN_ICON_MIN,
  //                                         FONT_MATERIALDESIGN_ICON_MAX, 0};
  //  ImFontConfig config;
  //  // config.MergeMode = true;
  //  ImGuiIO& io = ImGui::GetIO();
  //  io.Fonts->Clear();  // clear fonts if you loaded some before (even if only
  //                      // default one was loaded)
  //  // io.Fonts->AddFontDefault();  // this will load default font as well
  //  io.Fonts->AddFontFromFileTTF("font/Cascadia.ttf", 16.f); // Direct from
  //  TTF
  //
  //  ImGui::SFML::UpdateFontTexture();  // important call: updates font texture

  // Setup CIDE backend.
  // cidr::backend::IdeSettings ide_settings;
  // ide_settings.Load();
  // std::cout << ide_settings.ViewBinaryPath() << std::endl;
  // std::cout << ide_settings.ViewRepoPath() << std::endl;
  // ide_settings.CacheSolution("testingcache");
  // ide_settings.Save();

  cidr::ui::CideTestExplorerInterface cide_test_explorer;
  // cidr::ui::AstExplorerInterface ast_explorer;
  //  cidr::ui::Launcher cide_launcher_ui;

  // Register Test Modules
  // cppstandard extended
  //cide_test_explorer.RegisterTestCase(MINITEST_FUNCTOR_RUN_INLINE(Test_CxxExpected), "UT_EXPECTED_H");

  //// cand compiler
  //cide_test_explorer.RegisterTestCase(MINITEST_FUNCTOR_RUN_INLINE(Test_Lexer), "UT_LEXER_H");

  //cide_test_explorer.RegisterTestCase(MINITEST_FUNCTOR_RUN_INLINE(Test_TkScope), "UT_TOKENSCOPE_H");
  //cide_test_explorer.RegisterTestCase(MINITEST_FUNCTOR_RUN_INLINE(Test_ParserBasics), "UT_PARSER_H");
  //cide_test_explorer.RegisterTestCase(MINITEST_FUNCTOR_RUN_INLINE(Test_Build), "UT_BUILD_H");

  // Setup text editor.
  cidr::ui::TextEditor editor;
  editor.SetLanguageDefinition(cidr::ui::TextEditor::LanguageDefinition::CPlusPlus());

  // Main Loop
  sf::Clock deltaClock;
  while (window.isOpen()) {
    // Process events
    context.ProcessEvents(window, [&window](const sf::Event& e) {
      if (e.type == sf::Event::KeyReleased) {
        if (e.key.code == sf::Keyboard::T) {
          // Do stuff...
        }
      }

      if (e.type == sf::Event::Closed) {
        window.close();
        ImGui::SFML::Shutdown(window);
      }
    });

    // context.ProcessEvents(launcher_window,
    //                       [&launcher_window](const sf::Event& e) {
    //                         if (e.type == sf::Event::KeyReleased) {
    //                           if (e.key.code == sf::Keyboard::T) {
    //                             // Do stuff...
    //                           }
    //                         }

    //                        if (e.type == sf::Event::Closed) {
    //                          launcher_window.close();
    //                          ImGui::SFML::Shutdown(launcher_window);
    //                        }
    //                      });

    // We must check in-case the main window was closed above.
    if (window.isOpen()) {
      context.UpdateFrame(window, deltaClock.restart(),
                          [&editor, &hud, &cide_test_explorer](sf::RenderWindow& window, const sf::Time& delta) {
                            // Do stuff...
                            // editor.Render("Testing");
                            // ImGui::ShowDemoWindow();
                            auto win_size = window.getSize();
                            hud.Display(win_size.x, win_size.y);
                            // ast_explorer.Display();
                            // cide_test_explorer.Display();
                            window.clear();
                            ImGui::SFML::Render(window);
                            window.display();
                            return caf::eAPIError::kNone;
                          });
    }

    // if (launcher_window.isOpen()) {
    //   context.UpdateFrame(launcher_window, deltaClock.restart(),
    //                       [&](sf::RenderWindow& window, const sf::Time&
    //                       delta) {
    //                         // Do stuff...
    //                         ImGui::ShowDemoWindow();
    //                         cide_launcher_ui.Display(launcher_window);
    //                         window.clear();
    //                         ImGui::SFML::Render(window);
    //                         window.display();
    //                         return caf::eAPIError::kNone;
    //                       });
    // }

    context.CleanupContextFrame();
  }
  ImGui::SFML::Shutdown();
}
}  // namespace cidr


/// @} // end of cidrlib

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: [CIDR] C& integrated development repository library
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2025 Anton Yashchenko
//
// Licensed under the GNU Affero General Public License, Version 3.
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
