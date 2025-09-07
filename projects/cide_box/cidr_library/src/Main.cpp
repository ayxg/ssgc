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
#include "CAF/Tools/Trigger.hpp"
#include "CAF/Tools/Timer.hpp"
#include "CAF/Tools/Timeframe.hpp"
#include "CAF/Tools/Timeline.hpp"
#include "cgui.hpp"

//#include "CIDR/Common.hpp"
//#include "CIDR/Backend.hpp"
//#include "CIDR/UserInterface.hpp"

// Unit Testing Framework
// Makes mini-test record all test results, even if they pass.
// For the ui test explorer to work.
#define MINITEST_CONFIG_RECORD_ALL
#include "minitest.hpp"
// clang-format on
#include <mutex>
#include <thread>
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

static std::array<std::string_view, 5> kLoadingLabels = {
    "Locating launch_paths.json", "Reading launch_paths.json", "Loading resources",
    "Loading program data",       "Loading user data",
};

namespace caf::demo {

}  // namespace caf::demo

namespace cidr {

/// @brief
/// @param argc
/// @param argv
int MainGui(int argc, char** argv) {
  return 0;
  // sf::Clock clock{};
  // sf::Time run_time = sf::seconds(10);
  // sf::Time frame_delta_lock = sf::seconds(1.f / 60.f);
  
  //// Benchmarking stopwatch.
  // Stopwatch stopwatch{};
  //// Recorded times...
  // int app_run_timer = stopwatch.Start("App Run");
  // int app_construct_timer = stopwatch.Start("App Construct");
  // int window_init_timer = stopwatch.Start("Window Init");
  // int context_frame_timer = Stopwatch::kNullEntryId;

  //// Construct application.
  // ui::UserInterface user_interface{};
  // ui::Theme theme{ui::Theme::DefaultCidr()};  // UI theme. Theme must be applied to each window after creation.
  // stopwatch.Stop(app_construct_timer);

  //// Setup model loading sequence.
  // backend::eIsFirstLaunch is_first_launch{false};
  // backend::CacheFile2<data::LaunchPaths> launch_paths_cache{};
  // LoadingSequence init_load_seq{
  //     {
  //     // 1. Decide if this is the first launch.
  //     [&is_first_launch](float& prog, std::string_view& lbl) {
  //        is_first_launch = backend::IsFirstLaunch();
  //        lbl = kLoadingLabels[0];
  //        prog = 0.2f;
  //      },
  //      // 2. Load launch paths if they exist - or open the ui::FirstLaunchWizard.
  //      //    If this is a new user on the device with existing launch paths, open the ui::UserFirstLaunchWizard.
  //      [&is_first_launch, &launch_paths_cache](float& prog, std::string_view& lbl) {
  //        if (is_first_launch == backend::eIsFirstLaunch::kFirstLaunch) {
  //          lbl = kLoadingLabels[1];
  //          prog = LoadingSequence::kLoadingDone;
  //        } else if (is_first_launch == backend::eIsFirstLaunch::kFirstLaunchNewUser) {
  //          // Load existing launch paths, open new user first launch wizard.

  //         lbl = kLoadingLabels[1];
  //         prog = LoadingSequence::kLoadingDone;
  //       }
  //       // else -> backend::eIsFirstLaunch::kNotFirstLaunch
  //       else {
  //         stdfs::path this_launch_paths = backend::FindLaunchPathsFile();
  //         assert(
  //             !this_launch_paths.empty() &&
  //             "You should have confirmed the launch paths exist in a valid directory before running this load
  //             step.");
  //         launch_paths_cache.path = this_launch_paths;
  //         if (!launch_paths_cache.Load()) {
  //           prog = LoadingSequence::kLoadingError;
  //           lbl = "Failed to load launch paths. Potentially corrupt program data.";
  //           return;
  //         }
  //         lbl = "Loading launch paths.";
  //         prog = 0.4f;
  //       }
  //     },
  //     // 3. Launch paths are loaded. Full load program data.
  //     [](float& prog, std::string_view& lbl) {
  //       // Load resources from the resource paths.
  //       std::this_thread::sleep_for(std::chrono::seconds(2));
  //       lbl = kLoadingLabels[2];
  //       prog = 0.6f;
  //     },
  //     [](float& prog, std::string_view& lbl) {
  //       // Load program data from the program paths.
  //       std::this_thread::sleep_for(std::chrono::seconds(2));
  //       lbl = kLoadingLabels[3];
  //       prog = 0.8f;
  //     },
  //     [](float& prog, std::string_view& lbl) {
  //       // Load user data from the user paths.
  //       std::this_thread::sleep_for(std::chrono::seconds(2));
  //       lbl = kLoadingLabels[4];
  //       prog = LoadingSequence::kLoadingDone;
  //     }}};

  //// Start loading before the window is created, on a separate thread.
  // std::unique_ptr<std::thread> loading_thread{init_load_seq.Dispatch()};

  //// Create the loading screen window.
  // caf::WinHints hints{};
  // hints.InitialWidth = 320;
  // hints.InitialHeight = 146;
  // hints.NoTitleBar = true;
  // hints.EnableImGui = true;
  // hints.FrameLimit = 60;
  // user_interface.ctx.push_back(caf::Window{"C.I.D.R. Loading...", hints});
  // user_interface.ctx.back().SetPos(
  //     (sf::VideoMode::getDesktopMode().width / 2) - (user_interface.ctx.back().Underlying().getSize().x / 2),
  //     (sf::VideoMode::getDesktopMode().height / 2) - (user_interface.ctx.back().Underlying().getSize().y / 2));
  // theme.Apply(ImGui::GetStyle());
  //// Create the loading screen ui.
  // ui::LoadingScreen loading_screen{};

  //// Link the ui to the backend.
  // loading_screen.cbGetLoadingProgress = [&init_load_seq]() -> float { return init_load_seq.progress.load(); };
  // loading_screen.cbGetLoadingLabel = [&init_load_seq]() -> std::string_view { return init_load_seq.label.load(); };

  //// Add the frame handler to the loading window.
  // user_interface.ctx.back().EmplaceFrameHandler(
  //     [&loading_thread, &init_load_seq, &loading_screen](caf::Window& win, const sf::Time& dt) {
  //       loading_screen.Display(win, dt);
  //       if (loading_thread->joinable() && init_load_seq.IsDone()) loading_thread->join();
  //     });

  // stopwatch.Stop(window_init_timer);

  //// Main loop
  // sf::Clock run_clock{};
  // while (run_clock.getElapsedTime() < run_time) {
  //   context_frame_timer == Stopwatch::kNullEntryId ? context_frame_timer = stopwatch.Start("Context Frame")
  //                                                  : context_frame_timer = stopwatch.Start(context_frame_timer);
  //   user_interface.ctx.NextFrame(frame_delta_lock);
  //   stopwatch.Stop(context_frame_timer);
  // }

  // stopwatch.Stop(app_run_timer);
  // std::cout << stopwatch.DebugStr();
  // return EXIT_SUCCESS;

  // caf::Context context{};  // -THE- Window & Graphics Context : singleton context for the entire app.
  // ui::HUD hud{context};  // The HUD is tightly linked to the GFX context. And may control multiple windows.
  // backend::AppModel model{};
  //
  // model.StartUp();
  // hud.Link(model);

  // model.TurnOn();
  // hud.Activate();
  // while (model.IsOn()) {
  //   hud.Update();
  //   model.Step();
  // }

  // model.ShutDown();
  // return model.ExitCode();

  // backend::AppModel model{};
  // cidr::ui::HUD hud{context,model};  // The HUD is tightly linked to the GFX context. And may control multiple
  // windows.

  // model.StateInit();
  // hud.FastLoad();
  // while (model.CurrentState() != backend::AppModel::eAppState::kShutdown) {
  //   model.Step();
  //   hud.Display();
  // }
  // model.StateShutDown();

  // if (model.CurrentState() == backend::AppModel::eAppState::kFirstInit) {
  //   // Display first time initialization dialog.
  // } else if (model.CurrentState() == backend::AppModel::eAppState::kFastLoad) {
  //   model.StateFastLoad();
  //   if (model.CurrentState() == backend::AppModel::eAppState::kError)
  //     ;
  //   else {
  //     model.RequestState(backend::AppModel::eAppState::kLoad);
  //     model.Step();
  //   }
  // } else {
  //   // Some error occured
  // }
  // if (model.CurrentState() == backend::AppModel::eAppState::kLauncher) {
  // }

  // caf::Context context{};                     // -THE- Window & Graphics Context : singleton context for the entire
  // app. ui::Theme theme{ui::Theme::DefaultCidr()};  // UI theme. Theme must be applied to each window after
  // creation.

  // caf::WinHints hints{};  // Window initialization hints.
  // hints.EnableImGui = true;
  // hints.FrameLimit = 60;
  // hints.Hidden = true;
  //// Create the root window. This is also the main window of the application. If this window is closed then all
  /// other / windows will be closed as well. -THE- Context will exit. Kept invisible until after the launcher window
  /// has
  /// exited.
  // sf::RenderWindow& window = context.MakeWindow("CIDE", hints);
  // theme.Apply(ImGui::GetStyle());

  // cidr::backend::IdeModel ide_model;
  // auto ide_model_init = ide_model.Init();
  // if (!ide_model_init) {
  //   std::cout << "Failed to load ide params." << std::endl;
  //   return EXIT_FAILURE;
  // }

  //// The HUD is tightly linked to the GFX context. And may control multiple
  //// windows.
  // cidr::ui::HUD hud{context};

  //// The IDE model is loosely linked to the ui callback methods.
  //// Link backend to HUD:
  // hud.repo_explorer.root_dir = ide_model.active_repo_.solution_path;

  // hud.main_menu.cb_action_generate = [&ide_model] { ide_model.ExtCallGenerationStep(); };
  // hud.main_menu.cb_action_build = [&ide_model] { ide_model.ExtCallBuildStep(); };
  // hud.main_menu.cb_action_run = [&ide_model] { ide_model.ExtCallRunStep(); };

  // hud.repo_explorer.cb_update_root_dir = [&ide_model](std::filesystem::path& path) {
  //   ide_model.RefreshCurrentRepoRootDir(path);
  // };

  // hud.main_menu.cb_file_new_solution = [&ide_model](const std::string& dir, const std::string& name) {
  //   return ide_model.NewRepo(dir, name);
  // };

  //// Add another window
  // hints.InitialWidth = 800;
  // hints.InitialHeight = 600;
  // hints.NoTitleBar = true;
  // hints.Hidden = false;
  // auto& launcher_window = context.MakeWindow("CIDE Launcher", hints);
  // launcher_window.setPosition(
  //     sf::Vector2i{static_cast<int>((sf::VideoMode::getDesktopMode().width / 2) - (launcher_window.getSize().x /
  //     2)),
  //                  static_cast<int>((sf::VideoMode::getDesktopMode().height / 2) - (launcher_window.getSize().y /
  //                  2))});
  // theme.Apply(ImGui::GetStyle());  // Theme should be applied to each window.

  // cidr::backend::ToolchainParams toolchain_params;
  // toolchain_params.Load();
  // cidr::ui::Launcher cide_launcher_ui;
  // cidr::ui::Launcher::SettingsTableData host_table_data;
  // for (auto i = 0; i < toolchain_params.Size(); i++) {
  //   host_table_data.push_back({cidr::backend::eToolchainParamToSysStr(static_cast<cidr::backend::eToolchainParam>(i)),
  //                              toolchain_params.ViewParam(static_cast<cidr::backend::eToolchainParam>(i)).at(0)});
  // }
  // cide_launcher_ui.cb_get_general_settings = [&host_table_data] { return host_table_data; };

  //// Setup CIDE backend.
  //// cidr::backend::IdeSettings ide_settings;
  //// ide_settings.Load();
  //// std::cout << ide_settings.ViewBinaryPath() << std::endl;
  //// std::cout << ide_settings.ViewRepoPath() << std::endl;
  //// ide_settings.CacheSolution("testingcache");
  //// ide_settings.Save();

  //// cidr::ui::CideTestExplorerInterface cide_test_explorer;
  //// cidr::ui::AstExplorerInterface ast_explorer;
  ////  cidr::ui::Launcher cide_launcher_ui;

  //// Register Test Modules
  //// cppstandard extended
  //// cide_test_explorer.RegisterTestCase(MINITEST_FUNCTOR_RUN_INLINE(Test_CxxExpected), "UT_EXPECTED_H");

  ////// cand compiler
  //// cide_test_explorer.RegisterTestCase(MINITEST_FUNCTOR_RUN_INLINE(Test_Lexer), "UT_LEXER_H");

  //// cide_test_explorer.RegisterTestCase(MINITEST_FUNCTOR_RUN_INLINE(Test_TkScope), "UT_TOKENSCOPE_H");
  //// cide_test_explorer.RegisterTestCase(MINITEST_FUNCTOR_RUN_INLINE(Test_ParserBasics), "UT_PARSER_H");
  //// cide_test_explorer.RegisterTestCase(MINITEST_FUNCTOR_RUN_INLINE(Test_Build), "UT_BUILD_H");

  //// Setup text editor.
  // cidr::ui::TextEditor editor;
  // editor.SetLanguageDefinition(cidr::ui::TextEditor::LanguageDefinition::CPlusPlus());

  //// Main Loop
  // sf::Clock deltaClock;
  // while (window.isOpen()) {
  //   // Process events
  //   context.ProcessEvents(window, [&window](const sf::Event& e) {
  //     if (e.type == sf::Event::KeyReleased) {
  //       if (e.key.code == sf::Keyboard::T) {
  //         // Do stuff...
  //       }
  //     }

  //    if (e.type == sf::Event::Closed) {
  //      window.close();
  //      ImGui::SFML::Shutdown(window);
  //    }
  //  });

  //  context.ProcessEvents(launcher_window, [&launcher_window](const sf::Event& e) {
  //    if (e.type == sf::Event::KeyReleased) {
  //      if (e.key.code == sf::Keyboard::T) {
  //        // Do stuff...
  //      }
  //    }

  //    if (e.type == sf::Event::Closed) {
  //      // Store current theme on exit.
  //      // cxx::SaveStrToFile("CidrCurrentTheme.json", backend::ToJson(ui::Theme(ImGui::GetStyle())).dump());

  //      launcher_window.close();
  //      ImGui::SFML::Shutdown(launcher_window);
  //    }
  //  });

  //  // We must check in-case the main window was closed above.
  //  if (window.isOpen()) {
  //    context.UpdateFrame(window, deltaClock.restart(),
  //                        [&editor, &hud /*, &cide_test_explorer*/](sf::RenderWindow& window, const sf::Time& delta)
  //                        {
  //                          // Do stuff...
  //                          // editor.Render("Testing");
  //                          // ImGui::ShowDemoWindow();
  //                          auto win_size = window.getSize();
  //                          hud.Display(win_size.x, win_size.y);
  //                          // ast_explorer.Display();
  //                          // cide_test_explorer.Display();
  //                          window.clear();
  //                          ImGui::SFML::Render(window);
  //                          window.display();
  //                          return caf::eAPIError::kNone;
  //                        });
  //  }

  //  if (launcher_window.isOpen()) {
  //    context.UpdateFrame(launcher_window, deltaClock.restart(), [&](sf::RenderWindow& window, const sf::Time&
  //    delta)
  //    {
  //      // Do stuff...

  //      cide_launcher_ui.Display(launcher_window);
  //      ImGui::ShowDemoWindow();
  //      window.clear();
  //      ImGui::SFML::Render(window);
  //      window.display();
  //      return caf::eAPIError::kNone;
  //    });
  //  }

  //  context.CleanupContextFrame();
  //}
  // ImGui::SFML::Shutdown();
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
