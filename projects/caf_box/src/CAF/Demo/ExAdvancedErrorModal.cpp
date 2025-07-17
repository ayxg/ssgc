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
/// @brief [SOURCE] Advanced error modal window example
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

/// Demonstrates how to trigger window blocking events, and how to organize application entities into classes
/// which are independent of the window system graph.
///
/// Click the buttons in the main window to spawn a new error modal:
/// - [Error] -> Blocks main window, Abort exits parent window, Resume closes the error modal and unblocks.
/// - [Fatal] -> Blocks all windows, Abort exits app, Resume closes the error modal and unblocks.
/// - [Warning] -> Does not block, Abort exits parent window, Resume closes the error modal.
 int ExAdvancedErrorModal() {
   using namespace caf::sys;

   struct ErrorModal {
     enum eModalType { kError, kFatal, kWarning };
     Windows::Node* mwin{nullptr};
     eModalType type{kError};
     caf::Trigger abort_btn{false};
     caf::Trigger resume_btn{false};
     bool isopen{false};
     ErrorModal(eModalType modal_type) : type(modal_type) {}

     void Init(Windows::Hints& modal_hints, Windows::Node* parent_wnd) {
       modal_hints.InitialTitle = "[Warning]";
       modal_hints.InitialWidth = 400;
       modal_hints.InitialHeight = 200;
       modal_hints.FrameLimit = 60;
       if (parent_wnd)
         mwin = Windows::Create(modal_hints, parent_wnd);
       else
         mwin = Windows::Create(modal_hints);
       caf::imgui::Init(mwin, true);
       mwin->PushEventHandler([this](const auto& ev) { this->ProcessWindowEvent(ev); });
       mwin->SetDeallocCallback([this]() {
         std::cout << "[OnDealloc]" << mwin->Title() << "[Sys Ptr]" << mwin->GetSystemHandle() <<
         '\n'; if (mwin) caf::imgui::Shutdown(mwin); isopen = false;
       });
       isopen = true;
     }

     void ProcessEvents() {
       if (abort_btn.Reset()) {
         std::cout << "[Abort Button]" << mwin->Title() << "[Sys Ptr]" << mwin->GetSystemHandle()
                   << '\n';
         switch (type) {
           if (mwin->stem()) mwin->stem()->Freeze(false);
           case kError:
             Windows::Destroy(mwin->stem());  // Parent
             break;
           case kFatal:
             for (auto& wnd : Windows::GetWindows()) Windows::Destroy(&wnd);  // All
             break;
           case kWarning:
             Windows::Destroy(mwin);  // Current
             break;
           default:
             Windows::Destroy(mwin);
             break;
         }
       }
       if (resume_btn.Reset()) {
         std::cout << "[Resume Button]" << mwin->Title() << "[Sys Ptr]" << mwin->GetSystemHandle()
                   << '\n';
         if (mwin->stem()) mwin->stem()->Freeze(false);
         Windows::Destroy(mwin);
       }
     };

     void ProcessWindowEvent(const caf::sys::Windows::EventType& e) {
       if (mwin->IsOpen()) caf::imgui::ProcessEvent(mwin, e);

       // [CLOSE] -> Close proc based on modal type.
       if (e.type == sf::Event::Closed) {
         std::cout << "[Closed Event]" << mwin->Title() << "[Sys Ptr]" << mwin->GetSystemHandle()
                   << '\n';
         // Unfreeze the parent window if it was frozen by this modal.
         if (mwin->stem()) mwin->stem()->Freeze(false);

         switch (type) {
           case kError:
             Windows::Destroy(mwin->stem());  // Parent
             break;
           case kFatal:
             for (auto& wnd : Windows::GetWindows()) Windows::Destroy(&wnd);  // All
             break;
           case kWarning:
             Windows::Destroy(mwin);  // Current
             break;
           default:
             Windows::Destroy(mwin);
             break;
         }
       }
     };

     void Update(const sf::Time& dt) {
       if (isopen && mwin && mwin->IsAllocated() && mwin->IsOpen()) {
         caf::imgui::Update(mwin, dt);
         caf::imgui::SetCurrentWindow(mwin);
         ImGui::SetNextWindowSize(mwin->Size());
         ImGui::SetNextWindowPos(ImVec2(0.f, 0.f));
         switch (type) {
           case kError:
             ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.5f, 0.0f, 0.0f, 1.0f));
             ImGui::Begin("[Error]");
             ImGui::Text("Exit to close parent window.");
             if (ImGui::Button("Resume##btn")) resume_btn.Set(true);
             ImGui::SameLine();
             if (ImGui::Button("Abort##btn")) abort_btn.Set(true);
             break;
           case kFatal:
             ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
             ImGui::Begin("[Fatal]");
             ImGui::Text("Exit to close all windows.");
             if (ImGui::Button("Resume##btn")) resume_btn.Set(true);
             ImGui::SameLine();
             if (ImGui::Button("Abort##btn")) abort_btn.Set(true);
             break;
           case kWarning:
             ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1.0f, 0.5f, 0.0f, 1.0f));
             ImGui::Begin("[Warning]");
             ImGui::Text("Exit to close modal window.");
             if (ImGui::Button("Resume##btn")) resume_btn.Set(true);
             ImGui::SameLine();
             if (ImGui::Button("Abort##btn")) abort_btn.Set(true);
             break;
         }
         ImGui::End();
         ImGui::PopStyleColor();
       }
     };

     void Render() {
       if (isopen && mwin && mwin->IsAllocated() && mwin->IsOpen()) {
         mwin->GetRenderBuffer()->clear();
         caf::imgui::Render(mwin);
         mwin->Display();
       }
     };
   };

   struct AdvancedErrorModalExample {
     Windows::Node* mwin{nullptr};
     std::list<ErrorModal> modals{};
     caf::Trigger warning_btn{false};
     caf::Trigger fatal_btn{false};
     caf::Trigger error_btn{false};
     Windows::Hints* base_hints{nullptr};
     void Init(Windows::Hints& hints) {
       base_hints = &hints;
       hints.InitialTitle = "A Window";
       hints.InitialWidth = 800;
       hints.InitialHeight = 200;
       hints.FrameLimit = 60;
       mwin = Windows::Create(hints);
       mwin->PushEventHandler([this](const auto& e) { this->ProcessWindowEvent(e); });
       caf::imgui::Init(mwin, true);
       mwin->SetDeallocCallback([this]() {
         std::cout << "[OnDealloc]" << mwin->Title() << "[Sys Ptr]" << mwin->GetSystemHandle() <<
         '\n'; if (mwin) caf::imgui::Shutdown(mwin);
       });
     };

     void ProcessEvents() {
       assert(base_hints != nullptr && "[AdvancedErrorModalExample] You forgot to call Init() before ProcessEvents()."); 
       if (warning_btn.Reset()) {
         modals.push_back(ErrorModal(ErrorModal::kWarning));
         modals.back().Init(*base_hints, mwin);
       }

       if (fatal_btn.Reset()) {
         modals.push_back(ErrorModal(ErrorModal::kFatal));
         modals.back().Init(*base_hints, mwin);
       }

       if (error_btn.Reset()) {
         modals.push_back(ErrorModal(ErrorModal::kError));
         modals.back().Init(*base_hints, mwin);
         // Freeze main window until error modal is closed.
         mwin->Freeze(modals.back().mwin);
       }

       // Update modal list
       modals.erase(std::remove_if(modals.begin(), modals.end(),
                                   [](ErrorModal& modal) {
                                     bool isdone{false};
                                     // if (!modal.mwin) {
                                     //   isdone = true;
                                     //   std::cout << "[Removing Modal because node is nullptr]" << modal.mwin <<
                                     '\n';
                                     // } else if (!modal.mwin->Exists()) {
                                     //   isdone = true;

                                     //  std::cout << "[Removing Modal because window !Exists]" << modal.mwin << '\n';
                                     //} /*else if (modal.mwin->MarkedForDestruction()) {
                                     //  isdone = true;

                                     //  std::cout << "[Removing Modal because window is MarkedForDestruction]" <<
                                     '\n';
                                     //} else if (!modal.mwin->GetSystemHandle()) {
                                     //  isdone = true;

                                     //  std::cout << "[Removing Modal because window !IsOpen]" << modal.mwin << '\n';
                                     if (!modal.isopen) {
                                       isdone = true;
                                       std::cout << "[Removing Modal because tagged closed]" << modal.mwin << '\n';
                                     }

                                     return isdone;
                                   }),
                    modals.end());

       for (auto& modal : modals) {
         modal.ProcessEvents();
       }
     };

     void ProcessWindowEvent(const caf::sys::Windows::EventType& e) {
       assert(base_hints != nullptr && "[AdvancedErrorModalExample] You forgot to call Init() before ProcessEvents().");
       // Handle UI events
       if (mwin->IsOpen()) caf::imgui::ProcessEvent(Windows::GetCurrent(), e);

       // Handle Window events
       if (e.type == sf::Event::KeyReleased) {
         // [Y] -> Create a new child window.
         if (e.key.code == sf::Keyboard::Y) {
           modals.push_back(ErrorModal(ErrorModal::kWarning));
           modals.back().Init(*base_hints, Windows::GetCurrent());
         }
       }
       // [CLOSE] -> Close all windows.
       if (e.type == sf::Event::Closed) {
         for (auto& wnd : Windows::GetWindows()) Windows::Destroy(&wnd);
       }
     };

     void Update(const sf::Time& dt) {
       if (mwin && mwin->IsOpen()) {
         caf::imgui::Update(mwin, dt);
         caf::imgui::SetCurrentWindow(mwin);
         ImGui::SetNextWindowSize(mwin->Size());
         ImGui::SetNextWindowPos(ImVec2(0.f, 0.f));
         ImGui::Begin("exErrorModalWindow");
         if (ImGui::Button("Warning##btn")) warning_btn.Set(true);
         ImGui::SameLine();
         if (ImGui::Button("Error##btn")) error_btn.Set(true);
         ImGui::SameLine();
         if (ImGui::Button("Fatal##btn")) fatal_btn.Set(true);
         ImGui::Text(R"(Click the buttons in the main window to spawn a new error modal:
- [Error] -> Blocks main window, Abort exits parent window, Resume closes the error modal and unblocks.
- [Fatal] -> Blocks all windows, Abort exits app, Resume closes the error modal and unblocks.
- [Warning] -> Does not block, Abort exits parent window, Resume closes the error modal.)");
         ImGui::End();
       }

       for (auto& modal : modals) {
         modal.Update(dt);
       }
     };

     void Render() {
       if (mwin && mwin->IsOpen()) {
         mwin->GetRenderBuffer()->clear();
         caf::imgui::Render(mwin);
         mwin->Display();
       }

       for (auto& modal : modals) {
         modal.Render();
       }
     };
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

   Windows::Hints hints{};
   AdvancedErrorModalExample main_wnd{};
   main_wnd.Init(hints);

   // Other random detached window.
   hints.InitialTitle = "A Separate Window";
   hints.InitialWidth = 500;
   hints.InitialHeight = 500;
   hints.FrameLimit = 60;
   Windows::Node* win_other = Windows::Create(hints);
   win_other->PushEventHandler(exEmptyEventHandler{win_other});
   caf::imgui::Init(win_other, true);
   win_other->SetDeallocCallback([win_other]() {
     if (win_other) caf::imgui::Shutdown(win_other);
   });

   sf::Clock delta_clock{};
   sf::Time delta_time{};
   while (!Windows::GetWindows().empty()) {
     Windows::ProcessEvents();
     main_wnd.ProcessEvents();
     delta_time = delta_clock.restart();

     main_wnd.Update(delta_time);

     if (win_other && win_other->IsOpen()) {
       caf::imgui::Update(win_other, delta_time);
       caf::imgui::SetCurrentWindow(win_other);
       ImGui::SetNextWindowSize(win_other->Size());
       ImGui::SetNextWindowPos(ImVec2(0.f, 0.f));
       ImGui::Begin("A Separate Window");
       ImGui::Text("This is a separate window that can be closed independently.");
       ImGui::End();
     }

     // Apply render
     main_wnd.Render();

     if (win_other && win_other->IsOpen()) {
       win_other->Clear();
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
