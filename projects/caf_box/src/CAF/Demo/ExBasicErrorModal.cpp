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
/// @brief [SOURCE] Basic error modal window example
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup core_app_framework
/// @{

// clang-format off
#include "CAF/Demo.hpp"

#include <thread> // std::this_thread
#include "cxxx.hpp"
#include "CAF/System/Windows.hpp"
#include "CAF/Tool/Trigger.hpp"
// clang-format on

namespace caf::demo {
using namespace caf::sys;
enum eModalType { kError, kFatal, kWarning };

struct exModalEventHandler {
  Windows::Node* win;
  eModalType type{kError};
  void operator()(const Windows::EventType& e) {
    if (win->IsOpen()) caf::imgui::ProcessEvent(Windows::GetCurrent(), e);

    // [CLOSE] -> Close proc based on modal type.
    if (e.type == sf::Event::Closed) {
      switch (type) {
        case kError:
          Windows::Destroy(win->stem());  // Parent
          break;
        case kFatal:
          for (auto& wnd : Windows::GetWindows()) Windows::Destroy(&wnd);  // All
          break;
        case kWarning:
          Windows::Destroy(Windows::GetCurrent());  // Current
          break;
        default:
          Windows::Destroy(Windows::GetCurrent());
          break;
      }
    }
  }
};

struct exEventHandler {
  Windows::Node* win;
  std::list<std::pair<Windows::Node*, eModalType>>& modals;
  Windows::Hints& modal_hints;
  caf::Trigger* warning_btn;
  caf::Trigger* fatal_btn;
  caf::Trigger* error_btn;

  void operator()(const Windows::EventType& e) {
    // Handle UI events
    if (win->IsOpen()) caf::imgui::ProcessEvent(Windows::GetCurrent(), e);

    // Handle Window events
    if (e.type == sf::Event::KeyReleased) {
      // [Y] -> Create a new child window.
      if (e.key.code == sf::Keyboard::Y) {
        modal_hints.InitialTitle = "[Child Warning Modal]";
        modal_hints.InitialWidth = 400;
        modal_hints.InitialHeight = 200;
        modal_hints.FrameLimit = 60;
        Windows::Node* new_win = Windows::Create(modal_hints, Windows::GetCurrent());
        caf::imgui::Init(new_win, true);
        new_win->PushEventHandler(exModalEventHandler{new_win, eModalType::kWarning});
        new_win->SetDeallocCallback([new_win]() { caf::imgui::Shutdown(new_win); });
        modals.push_back({new_win, eModalType::kWarning});
      }
    }
    // [CLOSE] -> Close all windows.
    if (e.type == sf::Event::Closed) {
      for (auto& wnd : Windows::GetWindows()) Windows::Destroy(&wnd);
    }

    // Update modal list
    modals.erase(
        std::remove_if(modals.begin(), modals.end(), [](auto& wnd) { return !wnd.first || !wnd.first->IsOpen(); }),
        modals.end());
  }
};

struct exEmptyEventHandler {
  Windows::Node* win;
  void operator()(const Windows::EventType& e) {
    if (win->IsOpen()) caf::imgui::ProcessEvent(Windows::GetCurrent(), e);
    // [CLOSE] -> Close the current window, and all child windows, if not already closed.
    if (e.type == sf::Event::Closed) {
      Windows::Destroy(Windows::GetCurrent());
    }
  }
};

/// Demonstrates how to properly create and destroy modal windows which may control the state of other windows in the
/// window node graph. Demonstrates how to handle UI events which may cause creation or destruction of other
/// contexts.
///
/// Click the buttons in the main window to spawn a new error modal, each type functions differently upon close:
/// - [Error] -> Close to exit parent window.
/// - [Fatal] -> Close to exit all windows.
/// - [Warning] -> Close to exit the current window.
int ExBasicErrorModal()  {
  // Helper lambda for creating a Dear ImGui capable window.
  LAMBDA xCreateImGuiWindow = [](const Windows::Hints& h, Windows::Node* p = nullptr) {
    Windows::Node* w = Windows::Create(h, p);
    caf::imgui::Init(w, true);
    w->SetDeallocCallback([w]() { caf::imgui::Shutdown(w); });
    return w;
  };

  // Modals setup
  std::list<std::pair<Windows::Node*, eModalType>> modals{};
  caf::Trigger warning_btn{false};
  caf::Trigger fatal_btn{false};
  caf::Trigger error_btn{false};

  // Default base modal hints
  Windows::Hints modal_hints{};
  modal_hints.InitialWidth = 400;
  modal_hints.InitialHeight = 200;
  modal_hints.FrameLimit = 60;

  // Main window init
  Windows::Hints hints{};
  hints.InitialTitle = "A Window";
  hints.InitialWidth = 800;
  hints.InitialHeight = 200;
  hints.FrameLimit = 60;
  Windows::Node* win = xCreateImGuiWindow(hints);
  win->PushEventHandler(exEventHandler{win, modals, modal_hints, &warning_btn, &fatal_btn, &error_btn});

  // Other random detached window.
  hints.InitialTitle = "A Separate Window";
  hints.InitialWidth = 500;
  hints.InitialHeight = 500;
  Windows::Node* win_other = xCreateImGuiWindow(hints);
  win_other->PushEventHandler(exEmptyEventHandler{win_other});

  sf::Clock delta_clock{};
  sf::Time delta_time{};
  while (!Windows::GetWindows().empty()) {
    Windows::ProcessEvents();
    // Handle triggers
    if (warning_btn.Reset()) {
      modal_hints.InitialTitle = "[Warning]";
      Windows::Node* new_win = xCreateImGuiWindow(modal_hints, win);
      new_win->PushEventHandler(exModalEventHandler{new_win, eModalType::kWarning});
      modals.push_back({new_win, eModalType::kWarning});
    }

    if (fatal_btn.Reset()) {
      modal_hints.InitialTitle = "[Fatal]";
      Windows::Node* new_win = xCreateImGuiWindow(modal_hints, win);
      new_win->PushEventHandler(exModalEventHandler{new_win, eModalType::kFatal});
      modals.push_back({new_win, eModalType::kFatal});
    }

    if (error_btn.Reset()) {
      modal_hints.InitialTitle = "[Error]";
      Windows::Node* new_win = xCreateImGuiWindow(modal_hints, win);
      new_win->PushEventHandler(exModalEventHandler{new_win, eModalType::kError});
      modals.push_back({new_win, eModalType::kError});
    }

    // Update modal list, we can be more efficient by only erasing if the window graph is changing.
    if (Windows::IsGraphDirty()) {
      modals.erase(
          std::remove_if(modals.begin(), modals.end(), [](auto& wnd) { return !Windows::IsAvailable(wnd.first); }),
          modals.end());
    }

    delta_time = delta_clock.restart();

    // Apply updates
    if (Windows::IsAvailable(win)) {
      caf::imgui::Update(win, delta_time);
      caf::imgui::SetCurrentWindow(win);
      ImGui::SetNextWindowSize(win->Size());
      ImGui::SetNextWindowPos(ImVec2(0.f, 0.f));
      ImGui::Begin("exErrorModalWindow");
      if (ImGui::Button("Warning##btn")) warning_btn.Set(true);
      ImGui::SameLine();
      if (ImGui::Button("Error##btn")) error_btn.Set(true);
      ImGui::SameLine();
      if (ImGui::Button("Fatal##btn")) fatal_btn.Set(true);
      ImGui::Text(R"(Click the buttons in the main window to spawn a new error modal:
- [Error] -> Closes parent window.
- [Fatal] -> Closes all windows.
- [Warning] -> Closes only modal window.
See caf::demo::ExAdvancedErrorModal on how to : freeze other windows when modal is open,
and display/connect buttons which may control the state of other windows.
)");
      ImGui::End();

      for (std::pair<Windows::Node*, eModalType>& lwnd : modals)
        if (Windows::IsAvailable(lwnd.first)) {
          caf::imgui::Update(lwnd.first, delta_time);
          caf::imgui::SetCurrentWindow(lwnd.first);
          ImGui::SetNextWindowSize(lwnd.first->Size());
          ImGui::SetNextWindowPos(ImVec2(0.f, 0.f));
          switch (lwnd.second) {
            case kError:
              ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.5f, 0.0f, 0.0f, 1.0f));
              ImGui::Begin("[Error]");
              ImGui::Text("Exit to close parent window.");
              break;
            case kFatal:
              ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
              ImGui::Begin("[Fatal]");
              ImGui::Text("Exit to close all windows.");
              break;
            case kWarning:
              ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1.0f, 0.5f, 0.0f, 1.0f));
              ImGui::Begin("[Warning]");
              ImGui::Text("Exit closes modal window and it's children.");
              break;
          }
          ImGui::End();
          ImGui::PopStyleColor();
        };
    }

    if (Windows::IsAvailable(win_other)) {
      caf::imgui::Update(win_other, delta_time);
      caf::imgui::SetCurrentWindow(win_other);
      ImGui::SetNextWindowSize(win_other->Size());
      ImGui::SetNextWindowPos(ImVec2(0.f, 0.f));
      ImGui::Begin("A Separate Window");
      ImGui::Text("This is a separate window that can be closed independently.");
      ImGui::End();
    }

    // Apply render
    if (Windows::IsAvailable(win)) {
      win->GetRenderBuffer()->clear();
      caf::imgui::Render(win);
      win->Display();

      for (auto& lwnd : modals)
        if (Windows::IsAvailable(lwnd.first)) {
          assert(lwnd.first->stem() == win);  // Ensure modal is a child of the main window
          lwnd.first->Clear();
          caf::imgui::Render(lwnd.first);
          lwnd.first->Display();
        };
    }

    if (Windows::IsAvailable(win_other)) {
      win_other->GetRenderBuffer()->clear();
      caf::imgui::Render(win_other);
      win_other->Display();
    }
  }
  return EXIT_SUCCESS;
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
