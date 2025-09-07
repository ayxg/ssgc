///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2025 Anton Yashchenko
// Licensed under the Apache License, Version 2.0(the "License");
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: [CAF] Core Application Framework
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup core_app_framework
/// @brief [SOURCE] Animated square example
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup core_app_framework
/// @{

// clang-format off
#include "CAF/Demo.hpp"

#include <thread> // std::this_thread
#include "cxxx.hpp"
#include "CAF/System/Windows.hpp"
// clang-format on

namespace caf::demo {

/// - Shows how to draw and animate a basic object to a window's render buffer.
/// - Shows how to lock the update loop to a fixed time step using a caf::Timeframe.
/// - Shows how to potentially handle unexpected delays using the caf::Timeframe.
/// Press [T] to switch between basic and advanced animation modes. When rendering the basic animation, if you are
/// running on a fast system, you might notice a ghosting effect which makes the animation appear laggy(stuttering).
/// This is due to the high frame rate, the animation logic is also running at a variable speed.
/// In advanced mode, the animation logic is locked to 60 frames per second, and the rendering is
/// only done once the update frame has completed.
int ExAnimatedSquare() {
  using caf::sys::Windows;

  // Create a window
  Windows::Hints hints{};
  hints.InitialTitle = "Hello Square";
  hints.InitialWidth = 1000;
  hints.InitialHeight = 1000;
  hints.FrameLimit = 60;
  Windows::Node* win = Windows::Create(hints);

  // SFML shape to animate.
  sf::RectangleShape shape({250, 250});

  // Animation vars
  float min_x = 0.f;
  float min_y = 0.f;
  float max_x = win->Size().x - shape.getSize().x;
  float max_y = win->Size().y - shape.getSize().x;
  float current_x = min_x;
  float current_y = min_y;
  float movement_x = 5.f;
  float movement_y = 5.f;

  // Logic to animate the shape. speed_scale is used when compensating for lag in the advanced example below.
  auto xAnimateShape = [&](float speed_scale = 1.f) {
    current_x += movement_x * speed_scale;
    if (current_x > max_x) {
      current_x = max_x;
      movement_x = -movement_x;
    }
    if (current_x < min_x) {
      current_x = min_x;
      movement_x = -movement_x;
    }
    current_y += movement_y * speed_scale;
    if (current_y > max_y) {
      current_y = max_y;
      movement_y = -movement_y;
    }
    if (current_y < min_y) {
      current_y = min_y;
      movement_y = -movement_y;
    }
    shape.setPosition(current_x, current_x);
  };

  // Advanced animation setup
  bool show_advanced = false;  // Switch between basic and advanced animation modes.
  Timeframe update_timeframe((1.0 / 60.0) * 1'000'000.0);
  Timeframe::MicroDurationType update_delay{};  // Leftover time for this update step.
  Timeframe::MicroDurationType update_delta{};  // Delta time for this update step.

  // Add the animation switch event handler to the window.
  win->PushEventHandler([&update_timeframe, &show_advanced](const Windows::EventType& e) {
    // On key click
    if (e.type == sf::Event::KeyReleased) {
      // [T] -> Switch between advanced and basic update loop implementation.
      if (e.key.code == sf::Keyboard::T) {
        show_advanced = !show_advanced;
        if (show_advanced) {
          // Reset the update timeframe for advanced animation mode.
          // Usually you wont have to do this but since we are animating the
          // same shape in the main loop, this will keep the animation smooth.
          update_timeframe.Reset();
        }
      }
    }

    if (e.type == sf::Event::Closed) Windows::Destroy(Windows::GetCurrent());
  });

  // Lag simulation / lag compensation setup
  // Loop will run once for a regular animation, or multiple if compensation is occurring.
  sf::Clock fake_lag_clock{};
  int frames_behind{0};
  int frames_to_comp{0};  // Frames compensated per frame.
  sf::Clock testclock;
  sf::Time testtime = sf::seconds(0.0f);
  // Main loop
  while (!Windows::GetWindows().empty()) {
    Windows::ProcessEvents();
    if (!win->IsMarkedForDestruction()) {
      // Simulate approx 1s~ of lag in the update loop. Every 5 seconds.
      if (fake_lag_clock.getElapsedTime() > sf::seconds(5.f)) {
        using namespace std::chrono_literals;
        fake_lag_clock.restart();
        std::this_thread::sleep_for(1s);
      }

      // Basic animation of the square.
      // Shape is BLUE
      if (!show_advanced) {
        shape.setFillColor(sf::Color(0, 0, 255));
        xAnimateShape();
        // Draw to the render buffer.
        win->Clear();
        win->Draw(shape);
        // Apply the render buffer to the window.
        win->Display();
      }
      // Advanced animation using a caf::Timeframe.
      // Shape is GREEN when not compensating, RED when compensating.
      else {
        if (update_timeframe.Begin()) {
          if (update_timeframe.IsDelayed()) {
            frames_behind += update_timeframe.DelayFrames();
          }

          // Change color red when compensating.
          if (frames_behind > 0) {
            shape.setFillColor(sf::Color(255, 0, 0));
            // Clamp max compensation frames before applying. Accumulate negative compensated frames
            // to the frames_behind counter. This is will move at most x2 times as fast when compensating.
            frames_to_comp = std::clamp(frames_behind, 0, 2);
            frames_behind -= frames_to_comp;
            frames_behind = std::max(0, frames_behind);
          } else {
            shape.setFillColor(sf::Color(0, 255, 0));
            frames_to_comp = 0;  // reset comp frames
          }

          // Speed up the animation based on the frames to compensate.
          xAnimateShape(1.f + static_cast<float>(frames_to_comp));
        }

        if (update_timeframe.ShouldUpdate()) {
          // React to an update step completing later...
        }

        // You should keep displaying every frame or else window will glitch when moved or resized.
        win->Clear();
        win->Draw(shape);
        win->Display();
      }
    }
  }
  return 0;
}
}  // namespace caf::demo

/// @} // end of core_app_framework

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: [CAF] Core Application Framework
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
// @created: 2025/07/15
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
