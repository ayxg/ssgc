///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2025 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: [CAF] Core Application Framework
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup coreappfw
/// @brief [HEADER] Entity system.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup coreappfw
/// @{

// clang-format off
#pragma once
#include <concepts>
#include "cxxx.hpp"

#include "CAF/System/Entity.hpp"
#include "CAF/Entity/TGLWindow.hpp"
// clang-format on

namespace caf {

// Underlying immediate mode GUI library context wrapper.
struct TImContext;
struct TImScopedWidgetBase;
struct TImSingleWidgetBase;

// Scoped widgets.
struct TImWindow;
struct TImFrame;
struct TImMenuBar;
struct TImMenu;
struct TImTabBar;
struct TImTabItem;
struct TImTreeNode;

// Single widgets
struct TImButton;
struct TImTextLabel;
struct TImFormattedTextLabel;
struct TImMenuItem;
struct TImSelectable;
struct TImTextInput;
struct TImMultilineTextInput;
struct TImValidatedTextInput;

struct TImContext {
  Ent* tgt_win{};
  explicit TImContext() {}

  void tCreate(Ent& self_ent) {
    assert(self_ent.IsDescendantOf<TGLWindow>() && "'THudContext' must be a descendant of a 'TGlWindow'.");
    if (!self_ent.IsDescendantOf<TGLWindow>()) throw "'THudContext' must be a descendant of a 'TGlWindow'.";
    Ent* found_parent_win = self_ent.FindAncestor<TGLWindow>();
    if (!caf::ext::imgui::Init(found_parent_win->As<TGLWindow>()))
      throw "Failed to initialize THudContext underlying GUI library on target window.";
    tgt_win = found_parent_win;
  }

  eProcRes tDestroy() { caf::ext::imgui::Shutdown(tgt_win->As<TGLWindow>()); }

  eProcRes tProcOsEvents(Ent& self_ent, const OsEventType& ev) {
    caf::ext::imgui::ProcessEvent(tgt_win->As<TGLWindow>(), ev);
    return self_ent.SubProcOsEvents(ev);
  }

  eProcRes tProcSystems(Ent& self_ent, const caf::TimeType& dt) {
    caf::ext::imgui::SetCurrentWindow(tgt_win->As<TGLWindow>());
    caf::ext::imgui::Update(tgt_win->As<TGLWindow>(), dt);
    return self_ent.SubProcSystems(dt);
  }

  eProcRes tProcGraphics(Ent& self_ent) {
    caf::ext::imgui::Render(tgt_win->As<TGLWindow>());
    return self_ent.SubProcGraphics();
  }

  void tOnMove(Ent* to) {
    // If the moved-to node is still a descendant of the same window, don't have to re-init the context.
    if (to->IsDescendantOf(tgt_win)) return;

    // Otherwise, attempt to find the new target window and re-init the context.
    Ent* new_tgt_win = to->FindAncestor<TGLWindow>();
    assert(new_tgt_win &&
           "[caf::THudContext::tOnMove] 'THudContext' must be a descendant of a 'TGlWindow' or, 'TRenderBuffer' and "
           "'TOsEventHandler'.");
    if (!new_tgt_win)
      throw "[caf::THudContext::tOnMove] 'THudContext' must be a descendant of a 'TGlWindow' or, 'TRenderBuffer' and 'TOsEventHandler'.";

    tgt_win = new_tgt_win;
    caf::ext::imgui::Init(tgt_win->As<TGLWindow>());
  }
};

struct TImWindow : public CguiWindow {

  //TImWindow(const std::string& title = "", bool has_close_button = false,
  //           cgui::WindowFlags flags = cgui::WindowFlags())
  //    : CguiWindow(CguiWindow::Delayed(title, has_close_button, flags)) {}

  void tCreate(Ent& self_ent) {   
    assert(self_ent.IsDescendantOf<TImContext>() && "'THUDWindow' must be a descendant of a 'THUDContext'.");
  }

  eProcRes tProcSystems(Ent& self_ent, const caf::TimeType& dt) {
    eProcRes res{};
    if (BeginLate()) {
      res = self_ent.SubProcSystems(dt);
      EndEarly();    
    }
    return res;
  }
};

struct TImFrame : public CguiNamedSubcontext {
  void tCreate(Ent& self_ent) {
    assert(self_ent.IsDescendantOf<TImFrame>() ||
           self_ent.IsDescendantOf<TImWindow>() &&
           "'TImFrame' must be a descendant of a 'TImWindow' or another 'TImFrame'.");
  }

  eProcRes tProcSystems(Ent& self_ent, const caf::TimeType& dt) {
    eProcRes res{};
    if (BeginLate()) {
      res = self_ent.SubProcSystems(dt);
      EndEarly();
    }
    return res;
  }
};

struct TImMenuBar : CguiMenuBar {
  void tCreate(Ent& self_ent) {
    assert(self_ent.IsDescendantOf<TImFrame>() ||
           self_ent.IsDescendantOf<TImWindow>() &&
               "'TImFrame' must be a descendant of a 'TImWindow' or 'TImFrame'.");
  }

  eProcRes tProcSystems(Ent& self_ent, const caf::TimeType& dt) {
    eProcRes res{};
    if (BeginLate()) {
      res = self_ent.SubProcSystems(dt);
      EndEarly();
    }
    return res;
  }
};

struct TImMenu : CguiMenu {
  void tCreate(Ent& self_ent) {
    assert(self_ent.IsDescendantOf<TImFrame>() ||
           self_ent.IsDescendantOf<TImWindow>() && "'TImFrame' must be a descendant of a 'TImWindow' or 'TImFrame'.");
  }

  eProcRes tProcSystems(Ent& self_ent, const caf::TimeType& dt) {
    eProcRes res{};
    if (BeginLate()) {
      res = self_ent.SubProcSystems(dt);
      EndEarly();
    }
    return res;
  }
};

struct TImTabBar : CguiTabBar {
  void tCreate(Ent& self_ent) {
    assert(self_ent.IsDescendantOf<TImFrame>() ||
           self_ent.IsDescendantOf<TImWindow>() && "'TImFrame' must be a descendant of a 'TImWindow' or 'TImFrame'.");
  }

  eProcRes tProcSystems(Ent& self_ent, const caf::TimeType& dt) {
    eProcRes res{};
    if (BeginLate()) {
      res = self_ent.SubProcSystems(dt);
      EndEarly();
    }
    return res;
  }
};

struct TImTabItem : CguiTabItem {
  void tCreate(Ent& self_ent) {
    assert(
           self_ent.IsDescendantOf<CguiTabBar>() && "'TImTabItem' must be a descendant of a 'TImTabBar'.");
  }

  eProcRes tProcSystems(Ent& self_ent, const caf::TimeType& dt) {
    eProcRes res{};
    if (BeginLate()) {
      res = self_ent.SubProcSystems(dt);
      EndEarly();
    }
    return res;
  }
};

struct TImTreeNode : CguiTreeNode {
  void tCreate(Ent& self_ent) {
    assert(self_ent.IsDescendantOf<TImFrame>() ||
           self_ent.IsDescendantOf<TImWindow>() && "'TImFrame' must be a descendant of a 'TImWindow' or 'TImFrame'.");
  }

  eProcRes tProcSystems(Ent& self_ent, const caf::TimeType& dt) {
    eProcRes res{};
    if (BeginLate()) {
      res = self_ent.SubProcSystems(dt);
      EndEarly();
    }
    return res;
  }
};

// Single widgets

struct TImButton : CguiButton {
  void tCreate(Ent& self_ent) {
    assert(self_ent.IsDescendantOf<TImWindow>() && "'THUDWindow' must be a descendant of a 'THUDContext'.");
  }

  eProcRes tProcSystems(Ent& self_ent, const caf::TimeType& dt) {
    eProcRes res{};
    if (BeginLate()) {
      // send clicked signal
    }
    res = self_ent.SubProcSystems(dt);
    return res;
  }

};

struct TImTextLabel : CguiTextLabel {};

struct TImFormattedTextLabel : CguiNamedSubcontext {};

struct TImMenuItem : CguiNamedSubcontext {};

struct TImSelectable : CguiNamedSubcontext {};

struct TImTextInput : CguiNamedSubcontext {};

struct TImMultilineTextInput : CguiNamedSubcontext {};

struct TImValidatedTextInput : CguiTextInput {};

}  // namespace caf

/// @} // end of coreappfw

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: [CAF] Core Application Framework
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
// @created: 2025/07/25
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