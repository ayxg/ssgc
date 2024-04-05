#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include "../submodules/extended-cpp-standard/cppsextended.h"

// Unit Testing
#include "../submodules/mini-test/minitest.h"
#include "../unit-tests/ut-extended-cpp-standard/ut_expected.h"

#include "import_unit_tests.h"

#include "imgui-SFML.h"
#include "imgui.h"
#include "imgui_interface.h"
#include "imgui_stdlib.h"

#include "cide_backend.h"
#include "cide_ui.h"

int main() {
  sf::RenderWindow window(sf::VideoMode(800, 800), "ImGui + SFML = <3");
  window.setFramerateLimit(60);
  ImGui::SFML::Init(window);

  cide::ui::CideUserInterface cide_ui;
  // cide::IdeInterface idei;
  sf::Clock deltaClock;
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      ImGui::SFML::ProcessEvent(window, event);

      if (event.type == sf::Event::KeyReleased) {
        if (event.key.code == sf::Keyboard::T) {
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
    window.clear();
    ImGui::SFML::Render(window);
    window.display();
  }

  ImGui::SFML::Shutdown();
}