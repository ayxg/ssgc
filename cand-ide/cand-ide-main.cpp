// Makes mini-test record all test results, even if they pass.
// For the ui test explorer to work.
#define MINITEST_CONFIG_RECORD_ALL

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include "../submodules/extended-cpp-standard/cppsextended.h"

// Unit Testing
#include "../submodules/mini-test/minitest.h"
#include "../unit-tests/ut-extended-cpp-standard/ut_expected.h"
#include "cide_backend.h"
#include "cide_ui.h"
#include "imgui-SFML.h"
#include "imgui.h"
#include "imgui_interface.h"
#include "imgui_stdlib.h"
#include "import_unit_tests.h"

/// Weird Glitch with ImGui.
// When moving around the window, the updated imgui.ini will cause a crash
// For now im clearing the imgui.ini file on startup.
// This is a temporary fix. Cannot find the root cause.
// Error only happens if a window is moved around. When the project is reloaded.
// It crashes saying that endChild() or End() wasnt called.. 
static int gFlushImGuiIniFile = []() {
  std::ofstream ofs;
  ofs.open("imgui.ini", std::ofstream::out | std::ofstream::trunc);
  ofs.close();
  return 0;
}();

int main() {
  sf::RenderWindow window(sf::VideoMode(800, 800), "ImGui + SFML = <3");
  window.setFramerateLimit(60);
  ImGui::SFML::Init(window);

  cide::ui::CideUserInterface cide_ui;
  cide::ui::CideTestExplorerInterface cide_test_explorer;
  //cide_test_explorer.RegisterTestCase([]() {
  //  using namespace ut;
  //  RUN_INLINE_MINITEST(Test_CxxExpected, TestCase_CxxExpected);
  //  RUN_INLINE_MINITEST(Test_CxxExpected, TestCase_PartialExpectedConstrtuct);
  //  RUN_INLINE_MINITEST(Test_CxxExpected, TestCase_BoolError);
  //});
  cide_test_explorer.RegisterTestCase(
      ut::modules::RUN_INLINE_MINITESTS_UT_EXPECTED_H, "UT_EXPECTED_H");
  // cide::IdeInterface idei;
  sf::Clock deltaClock;
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      ImGui::SFML::ProcessEvent(window, event);

      if (event.type == sf::Event::KeyReleased) {
        if (event.key.code == sf::Keyboard::T) {
          using namespace ut;  // All unit tests are in the ut namespace.
          RUN_INLINE_MINITEST(Test_CxxExpected, TestCase_CxxExpected);
        }
      }
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    ImGui::SFML::Update(window, deltaClock.restart());
    // ImGui::ShowDemoWindow();
    cide_ui.Display();
    cide_test_explorer.Display();
    window.clear();
    ImGui::SFML::Render(window);
    window.display();
  }

  ImGui::SFML::Shutdown();
}