// CppExtended -
// C++ Standard Library Extensions also includes MPA Meta Programming Archive
#include "cxxx.hpp"

// Unit Testing Framework
// Makes mini-test record all test results, even if they pass.
// For the ui test explorer to work.
#define MINITEST_CONFIG_RECORD_ALL
#include "minitest.hpp"

// CGUI - Graphical User Interface (ImGui extension)
#include "caf.hpp"
#include "cgui.hpp"

// CIDE - C& Integrated Developer Environment
#include "cide_backend.h"
#include "cide_ui.h"
#include "cide_ui_text_editor.hpp"

// Unit Tests
#include "ut_caoco.h"
#include "ut_cxxx.h"

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

int main() {
  caf::WinHints hints;
  hints.EnableImGui = 1;
  hints.FrameLimit = 60;

  caf::Context context;
  auto& window = context.MakeWindow("CIDE", hints);

  cide::backend::IdeParams ide_params;
  auto ide_param_load = ide_params.Load();
  if (!ide_param_load) {
    std::cout << "Failed to load ide params." << std::endl;
    return EXIT_FAILURE;
  }

  //// Add another window
  //auto& launcher_window = context.MakeWindow("CIDE Launcher", hints);


  //host_params.Load();
  //cide::ui::Launcher cide_launcher_ui;
  //cide::ui::Launcher::SettingsTableData host_table_data;
  //for (auto i = 0; i < host_params.ParamCount(); i++) {
  //  host_table_data.push_back(
  //      {cide::backend::HostParams::eHostVarToOptStr(
  //           static_cast<cide::backend::eHostVar>(i)),
  //       host_params.ViewParam(static_cast<cide::backend::eHostVar>(i))});
  //}
  //cide_launcher_ui.callback_get_general_settings = [&host_table_data] {
  //  return host_table_data;
  //};

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
  // cide::backend::IdeSettings ide_settings;
  // ide_settings.Load();
  // std::cout << ide_settings.ViewBinaryPath() << std::endl;
  // std::cout << ide_settings.ViewRepoPath() << std::endl;
  // ide_settings.CacheSolution("testingcache");
  // ide_settings.Save();

  cide::ui::CideUserInterface cide_ui;
  cide::ui::CideTestExplorerInterface cide_test_explorer;
  cide::ui::AstExplorerInterface ast_explorer;
  // cide::ui::Launcher cide_launcher_ui;

  // Register Test Modules
  // cppstandard extended
  cide_test_explorer.RegisterTestCase(
      MINITEST_FUNCTOR_RUN_INLINE(Test_CxxExpected), "UT_EXPECTED_H");

  // cand compiler
  cide_test_explorer.RegisterTestCase(MINITEST_FUNCTOR_RUN_INLINE(Test_Lexer),
                                      "UT_LEXER_H");

  cide_test_explorer.RegisterTestCase(MINITEST_FUNCTOR_RUN_INLINE(Test_TkScope),
                                      "UT_TOKENSCOPE_H");
  cide_test_explorer.RegisterTestCase(
      MINITEST_FUNCTOR_RUN_INLINE(Test_ParserBasics), "UT_PARSER_H");
  cide_test_explorer.RegisterTestCase(MINITEST_FUNCTOR_RUN_INLINE(Test_Build),
                                      "UT_BUILD_H");

  // Setup text editor.
  TextEditor editor;
  editor.SetLanguageDefinition(TextEditor::LanguageDefinition::CPlusPlus());

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

    //context.ProcessEvents(launcher_window,
    //                      [&launcher_window](const sf::Event& e) {
    //                        if (e.type == sf::Event::KeyReleased) {
    //                          if (e.key.code == sf::Keyboard::T) {
    //                            // Do stuff...
    //                          }
    //                        }

    //                        if (e.type == sf::Event::Closed) {
    //                          launcher_window.close();
    //                          ImGui::SFML::Shutdown(launcher_window);
    //                        }
    //                      });

    // We must check in-case the main window was closed above.
    if (window.isOpen()) {
      context.UpdateFrame(
          window, deltaClock.restart(),
          [&editor, &cide_ui, &ast_explorer, &cide_test_explorer](
              sf::RenderWindow& window, const sf::Time& delta) {
            // Do stuff...
            editor.Render("Testing");
            ImGui::ShowDemoWindow();
            cide_ui.Display();
            ast_explorer.Display();
            cide_test_explorer.Display();
            window.clear();
            ImGui::SFML::Render(window);
            window.display();
            return caf::eAPIError::kNone;
          });
    }

    //if (launcher_window.isOpen()) {
    //  context.UpdateFrame(launcher_window, deltaClock.restart(),
    //                      [&](sf::RenderWindow& window, const sf::Time& delta) {
    //                        // Do stuff...
    //                        ImGui::ShowDemoWindow();
    //                        cide_launcher_ui.Display(launcher_window);
    //                        window.clear();
    //                        ImGui::SFML::Render(window);
    //                        window.display();
    //                        return caf::eAPIError::kNone;
    //                      });
    //}

    context.CleanupContextFrame();
  }
  ImGui::SFML::Shutdown();
}