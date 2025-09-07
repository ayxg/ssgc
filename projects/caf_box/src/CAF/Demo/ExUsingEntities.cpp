///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2025 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: [CAF] Core Application Framework
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup core_app_framework
/// @brief [SOURCE] Basic user interface example
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup core_app_framework
/// @{

// clang-format off
#include "CAF/Demo.hpp"

#include <thread> // std::this_thread
#include "cxxx.hpp"
#include "CAF/System/Windows.hpp"
//#include "CAF/System/Entity.hpp"
// clang-format on

//struct YExampleWindow {
//  caf::sys::Windows::Node* mwin_;
//  bool IsOpen() const { return mwin_ && mwin_->IsOpen(); }
//  void entCreate(caf::Ent& self_ent) {
//    caf::sys::Windows::Hints hints{};
//    hints.InitialTitle = "A Window";
//    hints.InitialWidth = 500;
//    hints.InitialHeight = 500;
//    hints.FrameLimit = 60;
//    mwin_ = caf::sys::Windows::Create(hints);
//    caf::imgui::Init(mwin_, true);
//  }
//
//  void entDestroy(caf::Ent& self_ent) {
//    caf::imgui::Shutdown(mwin_);
//    mwin_->Reset();
//  }
//
//  void entHandleOsEvent(const caf::sys::Windows::EventType& e) {
//    if (mwin_->IsOpen()) caf::imgui::ProcessEvent(mwin_, e);
//    //  On key click
//    if (e.type == sf::Event::KeyReleased) {
//      // [T] -> Create a new detached window.
//      if (e.key.code == sf::Keyboard::T) {
//        // caf::sys::Windows::Node* new_win = caf::sys::Windows::Create(hints);
//        // caf::imgui::Init(new_win, true);
//        // new_win->PushEventHandler(exEventHandler{hints, new_win});
//        // new_win->SetDeallocCallback([new_win]() { caf::imgui::Shutdown(new_win); });
//      }
//      // [Y] -> Create a new child window.
//
//      if (e.key.code == sf::Keyboard::Y) {
//        // Windows::Node* new_win = Windows::Create(hints, Windows::GetCurrent());
//        // caf::imgui::Init(new_win, true);
//        // new_win->PushEventHandler(exEventHandler{hints, new_win});
//        // new_win->SetDeallocCallback([new_win]() { caf::imgui::Shutdown(new_win); });
//      }
//    }
//
//    // [CLOSE] -> Close the current window, and all child windows, if not already closed.
//    if (e.type == sf::Event::Closed) {
//      if (mwin_->IsOpen()) mwin_->Close();
//      // caf::sys::Windows::Destroy(mwin_);
//    }
//  }
//
//  bool entProcessSystems(caf::Ent& self_ent, const sf::Time& delta_time) {
//    if (mwin_->IsOpen() && !mwin_->IsMarkedForDestruction()) {
//      caf::imgui::Update(mwin_, delta_time);
//      caf::imgui::SetCurrentWindow(mwin_);
//      ImGui::SetNextWindowSize(mwin_->Size());
//      ImGui::SetNextWindowPos(ImVec2(0.f, 0.f));
//      ImGui::Begin("Hello, Dear ImGui!");
//      ImGui::Text("Click [T] for a new detached window.\nClick [Y] for a new child window.");
//      ImGui::End();
//    }
//    return true;
//  }
//
//  bool entProcessGraphics(caf::Ent& self_ent) {
//    if (mwin_->IsOpen() && !mwin_->IsMarkedForDestruction()) {
//      mwin_->Clear();
//      // Render your shapes here, e.g.:
//      // mwin_->Draw(shape);
//      caf::imgui::Render(mwin_);
//      mwin_->Display();
//    }
//    return true;
//  }
//
//  bool entPollOsEvent(caf::sys::Windows::EventType& e) { return mwin_->PollEvent(e); }
//};
//static_assert(caf::EntHasMemberProcessGraphics<YExampleWindow>);

// struct TGlWindow {
//   std::unique_ptr<sf::RenderWindow> pwin_;
//   void entCreate(caf::Ent& self_ent) {
//     std::cout << "THudContext::entCreate" << std::endl;
//   }
//   void entDestroy(caf::Ent& self_ent) { std::cout << "THudContext::entDestroy" << std::endl; }
//   void entHandleOsEvent(const caf::OsEventType& event) { std::cout << "THudContext::entHandleOsEvent" << std::endl; }
//   bool entProcessEvents(caf::Ent& self_ent, caf::EventBufferType& evs) {
//     // Process systems here.
//     std::cout << "THudContext::entProcessEvents" << std::endl;
//     return self_ent.EntProcessBranchEvents(evs);
//   }
// };
//
// struct THudContext {
//   EntPtr<TGlWindow> host_window_{nullptr};
//   void entCreate(caf::Ent& self_ent) { std::cout << "THudContext::entCreate" << std::endl;
//     host_window_ = self_ent.tFindAncestor<TGlWindow>();
//     assert(host_window_ && "THudContext must be a direct or indirect branch of a TGlWindow");
//
//     caf::imgui::Init(host_window_, true);
//   }
//   void entDestroy(caf::Ent& self_ent) { std::cout << "THudContext::entDestroy" << std::endl;
//     caf::imgui::Shutdown(host_window_);
//   }
//   void entHandleOsEvent(const caf::OsEventType& e) { std::cout << "THudContext::entHandleOsEvent" << std::endl;
//     if (host_window_->IsOpen()) caf::imgui::ProcessEvent(host_window_, e);
//   }
//   bool entProcessEvents(caf::Ent& self_ent, caf::EventBufferType& evs) {
//     // Process systems here.
//     std::cout << "THudContext::entProcessEvents" << std::endl;
//     return self_ent.EntProcessBranchEvents(evs);
//   }
// };
//
// struct THudWindow {
//
//   void entCreate(caf::Ent& self_ent) { std::cout << "THudWindow::entCreate" << std::endl; }
//   void entDestroy(caf::Ent& self_ent) { std::cout << "THudWindow::entDestroy" << std::endl; }
//   void entHandleOsEvent(const caf::OsEventType& event) { std::cout << "THudWindow::entHandleOsEvent" << std::endl; }
//   bool entProcessEvents(caf::Ent& self_ent, caf::EventBufferType& evs) {
//     // Process systems here.
//     std::cout << "THudWindow::entProcessEvents" << std::endl;
//     return self_ent.EntProcessBranchEvents(evs);
//   }
//   bool entProcessSystems(caf::Ent& self_ent, const sf::Time& dt) {
//       ImGui::SetNextWindowSize(ImGui::GetContentRegionAvail());
//       ImGui::SetNextWindowPos(ImVec2(0.f, 0.f));
//       if (ImGui::Begin("Hello, Dear ImGui!")) {
//         self_ent.EntProcessBranchSystems(dt);
//       }
//       ImGui::End();
//     return true;
//   }
// };
//
// struct THudLayout {
//   void entCreate(caf::Ent& self_ent) { std::cout << "THudLayout::entCreate" << std::endl; }
//   void entDestroy(caf::Ent& self_ent) { std::cout << "THudLayout::entDestroy" << std::endl; }
//   void entHandleOsEvent(const caf::OsEventType& event) { std::cout << "THudLayout::entHandleOsEvent" << std::endl; }
//   bool entProcessEvents(caf::Ent& self_ent, caf::EventBufferType& evs) {
//     // Process systems here.
//     std::cout << "THudLayout::entProcessEvents" << std::endl;
//     return self_ent.EntProcessBranchEvents(evs);
//   }
// };
//
// struct THudLabel {
//   std::string text;
//   void entCreate(caf::Ent& self_ent) { std::cout << "THudLabel::entCreate" << std::endl; }
//   void entDestroy(caf::Ent& self_ent) { std::cout << "THudLabel::entDestroy" << std::endl; }
//   void entHandleOsEvent(const caf::OsEventType& event) { std::cout << "THudLabel::entHandleOsEvent" << std::endl; }
//   bool entProcessEvents(caf::Ent& self_ent, caf::EventBufferType& evs) {
//     // Process systems here.
//     std::cout << "THudLabel::entProcessEvents" << std::endl;
//     return self_ent.EntProcessBranchEvents(evs);
//   }
//   bool entProcessSystems(caf::Ent& self_ent, const sf::Time& dt) {
//     ImGui::Text(text.c_str());
//     return true;
//   }
// };
//
// struct THudButton {
//   std::string text;
//   void entCreate(caf::Ent& self_ent) { std::cout << "THudButton::entCreate" << std::endl; }
//   void entDestroy(caf::Ent& self_ent) { std::cout << "THudButton::entDestroy" << std::endl; }
//   void entHandleOsEvent(const caf::OsEventType& event) { std::cout << "THudButton::entHandleOsEvent" << std::endl; }
//   bool entProcessEvents(caf::Ent& self_ent, caf::EventBufferType& evs) {
//     // Process systems here.
//     std::cout << "THudButton::entProcessEvents" << std::endl;
//     return self_ent.EntProcessBranchEvents(evs);
//   }
//   bool entProcessSystems(caf::Ent& self_ent, const sf::Time& dt) {
//     if (ImGui::Button(text.c_str())) sendButtonClicked();
//     return true;
//   }
//
//   void sendButtonClicked() {
//     // Here you can send a message to the application or perform some action.
//     std::cout << "Button '" << text << "' clicked!" << std::endl;
//   }
// };


namespace caf::demo {
/// Demonstrates how to use CAF's entity system to organize your application into an node-based heiarchy.
int ExUsingEntities() {
  using namespace caf::sys;
  //caf::Ent pwnd = YExampleWindow();
  //auto& wnd = pwnd.as<YExampleWindow>();

  //// caf::Ent composed_pwnd = TGlWindow();
  //// caf::Ent* hud_window  = composed_pwnd.EntPush(THudContext())->EntPush(THudWindow());
  //// hud_window->EntPush(THudLabel("Something went wrong..."));
  //// composed_pwnd.EntPush(THudButton("Abort."));

  //sf::Clock delta_clock{};
  //sf::Time delta_time{};
  //// sf::CircleShape shape(100.f);
  //std::vector<sf::Event> evs;
  //// shape.setFillColor(sf::Color::Green);

  //while (wnd.IsOpen()) {
  //  pwnd.EntProcessEvents(evs);
  //  delta_time = delta_clock.restart();
  //  pwnd.EntProcessSystems(delta_time);
  //  pwnd.EntProcessGraphics();
  //}

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
