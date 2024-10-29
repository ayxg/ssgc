// SFML
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

// CppExtended -
// C++ Standard Library Extensions also includes MPA Meta Programming Archive
#include "cppsextended.h"

// Unit Testing Framework
// Makes mini-test record all test results, even if they pass.
// For the ui test explorer to work.
#define MINITEST_CONFIG_RECORD_ALL
#include "minitest.h"

// ImGui
#include "TextEditor.h"
#include "imgui-SFML.h"
#include "imgui.h"
#include "imgui_interface.h"
#include "imgui_stdlib.h"

// CIDE - C& Integrated Developer Environment
#include "cide_backend.h"
#include "cide_ui.h"

// Unit Tests
#include "MaterialDesign.inl"
#include "ForkAwesome.inl"
#include "import_unit_tests.h"
#include "ut_cand_compiler.h"
#include "ut_expected.h"

// !TEMPORARY FIX!
// Weird Glitch with ImGui.
// When moving around the window, the updated imgui.ini will cause a crash
// For now im clearing the imgui.ini file on startup.
// This is a temporary fix. Cannot find the root cause.
// Error only happens if a window is moved around. When the project is reloaded.
// It crashes saying that endChild() or End() wasn't called..
static int gFlushImGuiIniFile = []() {
  std::ofstream ofs;
  ofs.open("imgui.ini", std::ofstream::out | std::ofstream::trunc);
  ofs.close();
  return 0;
}();

int main() {
  sf::RenderWindow window(sf::VideoMode(800, 800), "ImGui + SFML = <3");
  window.setFramerateLimit(60);
  bool default_font = false;
  ImGui::SFML::Init(window, default_font);

  // Setup Imgui Fonts
#define FONT_MATERIALDESIGN_ICON_MIN 0xe000
#define FONT_MATERIALDESIGN_ICON_MAX 0xf8ff
  static const ImWchar glyph_ranges[] = {FONT_MATERIALDESIGN_ICON_MIN,
                                         FONT_MATERIALDESIGN_ICON_MAX, 0};
  ImFontConfig config;
  // config.MergeMode = true;
  ImGuiIO& io = ImGui::GetIO();
  io.Fonts->Clear();  // clear fonts if you loaded some before (even if only
                      // default one was loaded)
  // io.Fonts->AddFontDefault();  // this will load default font as well
  io.Fonts->AddFontFromFileTTF("font/Cascadia.ttf", 16.f); // Direct from TTF

  ImGui::SFML::UpdateFontTexture();  // important call: updates font texture

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
    sf::Event event;
    while (window.pollEvent(event)) {
      ImGui::SFML::ProcessEvent(window, event);

      if (event.type == sf::Event::KeyReleased) {
        if (event.key.code == sf::Keyboard::T) {
        }
      }
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    ImGui::SFML::Update(window, deltaClock.restart());
    editor.Render("Testing");
    ImGui::ShowDemoWindow();
    cide_ui.Display();
    ast_explorer.Display();
    cide_test_explorer.Display();
    window.clear();
    ImGui::SFML::Render(window);
    window.display();
  }
  ImGui::SFML::Shutdown();
}