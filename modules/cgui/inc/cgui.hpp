///////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
///////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup cand_cide
/// @brief Interface for the ImGui library.
///////////////////////////////////////////////////////////////////////////////

/// @addtogroup cand_cide_cgui
/// @{
#pragma once
// Includes:
// clang-format off
#include "cxxx.hpp"
#include "caf.hpp"

//#include "SFML/Graphics.hpp"
//#include "imgui.h"
//#include "imgui-SFML.h"
//#include "cgui_imgui_stdlib.hpp"
// clang-format on

///////////////////////////////////////////////////////////////////////////////
/// @namespace cgui
/// @brief A simple unified interface for the underlying graphics and windowing
///        libraries. Only provides bare minimum functionality required for
///        this project. Designed to potentially use other libraries in the
///        future.
///
/// Current Features Related to ImGui:
/// 1. No need to call ImGui::Begin and ImGui::End for each window/widget.
/// 2. No raw pointers.
/// 3. Unified use of std::string.
/// 4. Names of windows must be unique. Handles possible errors from occurrence.
/// 5. Widget IDs must be unique, are automatically generated.
///////////////////////////////////////////////////////////////////////////////
namespace cgui {

///////////////////////////////////////////////////////////////////////////////
// decls
///////////////////////////////////////////////////////////////////////////////

/// Internal vec2 representation, for now its a pair of floats to keep it
/// simple.
using CguiVec2 = std::pair<float, float>;

using GuiFlags = int;  ///> Underlying type of widget gui flags (int).

using eWindowFlags = ImGuiWindowFlags_;
using eSubcontextFlags = ImGuiChildFlags_;
using eTabBarFlags = ImGuiTabBarFlags_;
using eTabItemFlags = ImGuiTabItemFlags_;
using eInputTextFlags = ImGuiInputTextFlags_;

// Flag Structs: Represent ImGui Flags.
using WindowFlags = cxx::EnumeratedFlags<eWindowFlags, GuiFlags>;
using SubcontextFlags = cxx::EnumeratedFlags<eSubcontextFlags, GuiFlags>;
using TabBarFlags = cxx::EnumeratedFlags<eTabBarFlags, GuiFlags>;
using TabItemFlags = cxx::EnumeratedFlags<eTabItemFlags, GuiFlags>;
using InputTextFlags = cxx::EnumeratedFlags<eInputTextFlags, GuiFlags>;

/// Specifies if a widget should be initialized immediately or delayed.
enum eWidgetInit {
  kWidgetInitDelayed = true,
  kWidgetInitImmediate = false,
};

class UIDGen;              ///> Generates unique widget identifiers.
class UniqueNameMap;       ///> Maintains unique names across widgets.
class ScopedWidgetBase;    ///> Base class for all scoped widgets.
class SingularWidgetBase;  ///> Base class for all singular widgets.

namespace scoped_widget {
class Window;
class Subcontext;       ///> Autogenerates unique id.
class NamedSubcontext;  ///> Name must be unique or throws.
class MenuBar;
class Menu;
class TabBar;
class TabItem;
class TreeNode;
}  // namespace scoped_widget

namespace single_widget {
class Button;
class MenuItem;
class Selectable;
class MultilineTextInput;
}  // namespace single_widget

namespace combo_widget {
class DirectoryView;  ///> TreeNode view of a file path,single widget base.
}  // namespace combo_widget

/// The OpenGl and Windowing context. At the moment only
/// SFML. In the future will be extended to OpenGl/Vulkan
/// + GLFW.
class GraphicsContext;

///////////////////////////////////////////////////////////////////////////////
// constants
///////////////////////////////////////////////////////////////////////////////
constexpr float kExpand = -FLT_MIN;
constexpr CguiVec2 kExpandXY = {kExpand, kExpand};

constexpr float kExpandWidgetToRemainingSpace() { return -FLT_MIN; }
const CguiVec2 kExpandWidgetToRemainingSpaceXY = {
    kExpandWidgetToRemainingSpace(), kExpandWidgetToRemainingSpace()};

///////////////////////////////////////////////////////////////////////////////
// defs
///////////////////////////////////////////////////////////////////////////////

class UIDGen {
 public:
  using Iter = std::unordered_set<size_t>::iterator;
  UIDGen();  // Starts at 1 cause ImGui reserves id 0
  // Do a try catch block around this method if you wish to refresh on overflow.
  Iter GetId();
  void PopId();
  void EraseId(Iter it);
  const std::unordered_set<size_t>& Generated();
  void Refresh();
  bool Empty();

 private:
  size_t next_id_;
  std::unordered_set<size_t> generated_ids_;
};

/// Simple map which maintains unique names.
///
/// TODO: improve using appended unique ids,
/// in ImGui you can add "id" + "###tag" at the end
/// to seperate objects with the same name.
/// ## seems to also do something...search imgui src
///   You can use the "##" or "###" markers to use the same label with different
///   id, or same id with different label. See documentation at the top of this
///   file.
class UniqueNameMap {
 public:
  cxx::BoolError AddName(const std::string& str);
  void RemoveName(const std::string& str);
  bool Contains(const std::string& str);

 private:
  std::unordered_set<std::string> names_;
};

/// Implicit global unique id generator for cgui widget classes.
/// !! Do not use/access directly, called by ScopedWidgetBase on default
/// construction.
static UIDGen gCguiDefaultUIDGenerator{};

/// Implicit global unique name map for cgui widget classes.
/// !! Do not use/access directly, called by ScopedWidgetBase on default
/// construction.
static UniqueNameMap gCguiDefaultUniqueNameMap{};

class ScopedWidgetBase {
 public:
  /// Returns true is the Begin() function has been called
  /// further gui commands will add to this scope.
  constexpr  bool IsScopeActive() const;

  /// The meaning of is_on_ may vary based on the ImGui widget call.
  /// Usually indicates if this object was rendered by ImGui.
  /// Some Object will always render, while the result represents a state.
  /// These objects must be ended with ForceEndImpl().
  /// Ex. Windows will return false for this parameter if they are minimized,
  /// but the top bar will still be rendered. So the window is NOT displayed
  /// but it IS rendered.
  bool IsOn() const;

 protected:
  /// Implementations of the BeginLate and EndEarly methods.
  /// Begin and End implementations are meant for the constructor
  /// and destructor respectivley.
  /// ForceEnd is for widgets which must call End()
  /// even if Begin() returns false.
  bool BeginImpl();
  bool BeginLateImpl();
  void EndImpl();
  void EndEarlyImpl();
  void ForceEndImpl();
  void ForceEndEarlyImpl();

 public:  // Public interface to be implemented by the child class.
  virtual bool BoundBegin() = 0;
  virtual void BoundEnd() = 0;
  virtual bool BeginLate() = 0;
  virtual void EndEarly() = 0;
  virtual ~ScopedWidgetBase() = default;

 public:
  /// Default object is a null widget,Begin() will always return false.
  ScopedWidgetBase(bool is_delayed);

  /// Copy is allowed, but the name_map and id_gen are shared.
  /// The uniqueness of the name or id is only checked on construction.
  ScopedWidgetBase(const ScopedWidgetBase& other) = default;

  /// Move is allowed.
  ScopedWidgetBase(ScopedWidgetBase&& other) noexcept = default;

  /// Boolean conversion, returns IsOn()
  operator bool();

 protected:
  cxx::BoolError RequestNewName(
      const std::string& str);  ///> Name must not exist.
  void ReleaseName(const std::string& str);
  UIDGen::Iter RequestId();  ///> Generate a new uuid
  void ReleaseId(UIDGen::Iter id_iter);

 protected:
  UniqueNameMap& name_map_;
  UIDGen& id_gen_;
  bool is_scope_active_;
  bool is_on_;
  bool is_delayed_;
};

class SingularWidgetBase {
 public:
  /// Result of requesting to render this widget.
  /// Meaning varies from widget to widget.
  /// Indicated a pressed/released/displayed state.
  bool IsOn() const;

  /// True if the widget is currently delayed
  /// eg. the widget has not been rendered yet.
  bool IsDelayed() const;

  /// Boolean conversion, returns IsOn()
  operator bool();

 protected:
  bool BeginImpl();
  bool BeginLateImpl();

 public:
  SingularWidgetBase(bool is_delayed);
  virtual bool BeginLate() = 0;
  virtual ~SingularWidgetBase() = default;
  virtual bool BoundBegin() = 0;

 protected:
  /// Result of requesting to render this widget.
  bool is_on_;
  bool is_delayed_;
};

namespace scoped_widget {

class Window : public ScopedWidgetBase {
 public:
  const std::string& Title() const;

  /// @warning Returns a mutable reference.
  WindowFlags& Flags();

  /// Returns TRUE if the close button of the window was triggered this frame.
  /// From ImGui documentation as to why the implementation is inversed...:
  /// Passing 'bool* p_open' displays a Close button on the upper-right corner
  /// of the window, the pointed value will be set to false when the button is
  /// pressed.
  bool IsCloseButtonTriggered() const;

  const CguiVec2& QuerySize() const;
   float QueryWidth() const;
   float QueryHeight() const;

 public:
  static  Window Delayed(const std::string& title,
                               bool has_close_button = false,
                               WindowFlags flags = WindowFlags());

  Window(const std::string& title, bool has_close_button = false,
         WindowFlags flags = WindowFlags(),
         bool delay_begin = kWidgetInitImmediate);

  bool BeginLate() override;
  void EndEarly() override;
  ~Window();
  Window(const Window& other);
  Window(Window&& other) noexcept;

 protected:
  bool BoundBegin() override;
  void BoundEnd() override;

 private:
  std::string title_{""};
  WindowFlags flags_{WindowFlags()};
  bool has_close_button_;
  std::unique_ptr<bool> close_button_state_{nullptr};
  CguiVec2 size_{0.f, 0.f};
};

class Subcontext : public ScopedWidgetBase {
 public:
  std::size_t Id() const;
  /// @warning Returns a mutable reference.
  WindowFlags GetWindowFlags() const;
  /// @warning Returns a mutable reference.
  SubcontextFlags GetSubcontextFlags() const;
  const CguiVec2& RequestedSize();

 public:
  static  Subcontext Delayed(
      CguiVec2 size = {0.f, 0.f}, WindowFlags win_flags = WindowFlags(),
      SubcontextFlags subcontext_flags = SubcontextFlags());
  Subcontext(CguiVec2 size = {0.f, 0.f}, WindowFlags win_flags = WindowFlags(),
             SubcontextFlags subcontext_flags = SubcontextFlags(),
             bool delay_begin = kWidgetInitImmediate);
  bool BeginLate() override;
  void EndEarly() override;
  ~Subcontext();

 protected:
  bool BoundBegin() override;
  void BoundEnd() override;

 private:
  UIDGen::Iter uid_;
  WindowFlags win_flags_;
  SubcontextFlags subcontext_flags_;
  CguiVec2 requested_size_;
};

class NamedSubcontext : public ScopedWidgetBase {
 public:
  const std::string& Name() const;
  /// @warning Returns a mutable reference.
  WindowFlags GetWindowFlags() const;
  /// @warning Returns a mutable reference.
  SubcontextFlags GetSubcontextFlags() const;
  const CguiVec2& RequestedSize();
  void RequestSize(const CguiVec2& size);

 public:
  static NamedSubcontext Delayed(
      const std::string& name, CguiVec2 size = {0.f, 0.f},
      WindowFlags win_flags = WindowFlags(),
      SubcontextFlags subcontext_flags = SubcontextFlags());
  NamedSubcontext(const std::string& name, CguiVec2 size = {0.f, 0.f},
                  WindowFlags win_flags = WindowFlags(),
                  SubcontextFlags subcontext_flags = SubcontextFlags(),
                  bool delay_begin = kWidgetInitImmediate);
  bool BeginLate() override;
  void EndEarly() override;
  ~NamedSubcontext();

 protected:
  bool BoundBegin() override;
  void BoundEnd() override;

 private:
  std::string name_;
  WindowFlags win_flags_;
  SubcontextFlags subcontext_flags_;
  CguiVec2 requested_size_;
};

class MenuBar : public ScopedWidgetBase {
 public:
  static  MenuBar Delayed();
  MenuBar(bool delay_begin = kWidgetInitImmediate);
  bool BeginLate() override;
  void EndEarly() override;
  ~MenuBar();

 protected:
  bool BoundBegin() override;
  void BoundEnd() override;
};

class Menu : public ScopedWidgetBase {
 public:
  const std::string& Title() const;
  bool IsEnabled() const;
  static  Menu Delayed(const std::string& title, bool is_enabled = true);
  Menu(const std::string& title, bool is_enabled = true,
       bool delay_begin = kWidgetInitImmediate);
  bool BeginLate() override;
  void EndEarly() override;
  ~Menu();

 protected:
  bool BoundBegin() override;
  void BoundEnd() override;

 private:
  std::string title_;
  bool is_enabled_;
};

class TabBar : public ScopedWidgetBase {
 public:
  const std::string& Name() const;
  TabBarFlags GetFlags() const;
  static TabBar Delayed(const std::string& name,
                               TabBarFlags flags = TabBarFlags());
  TabBar(const std::string& name, TabBarFlags flags = TabBarFlags(),
         bool delay_begin = kWidgetInitImmediate);
  bool BeginLate() override;
  void EndEarly() override;
  ~TabBar();

 protected:
  bool BoundBegin() override;
  void BoundEnd() override;

 private:
  std::string name_;
  TabBarFlags flags_;
};

class TabItem : public ScopedWidgetBase {
 public:
  const std::string& Name() const;

  //!! Warning: mutable reference
  TabItemFlags& GetFlags();

  const TabItemFlags& GetFlags() const;

  bool IsSelected() const;

 public:
  static TabItem Delayed(const std::string& title,
                                TabItemFlags flags = TabItemFlags());

  TabItem(const std::string& title, TabItemFlags flags = TabItemFlags(),
          bool delay_begin = kWidgetInitImmediate);

  bool BeginLate() override;

  void EndEarly() override;

  ~TabItem();

  // TabItem(const TabItem& other) : ScopedWidgetBase(other.is_delayed_) {
  //   this->is_scope_active_ = other.is_scope_active_;
  //   this->is_on_ = other.is_on_;
  //   this->name_ = other.name_;
  //   this->flags_ = other.flags_;
  //   this->is_selected_ = std::make_unique<bool>(*other.is_selected_);
  // }

  // TabItem(TabItem&& other) noexcept
  //     : ScopedWidgetBase(std::move(other.is_delayed_)) {
  //   this->is_scope_active_ = std::move(other.is_scope_active_);
  //   this->is_on_ = std::move(other.is_on_);
  //   this->name_ = std::move(other.name_);
  //   this->flags_ = std::move(other.flags_);
  //   this->is_selected_ = std::move(other.is_selected_);
  // }

 protected:
  bool BoundBegin() override;

  void BoundEnd() override;

 private:
  std::string name_;
  TabItemFlags flags_;
  bool* is_selected_;
};

class TreeNode : public ScopedWidgetBase {
  std::string name_;

 public:
  const std::string& Name() const;
  static  TreeNode Delayed(const std::string& name);
  TreeNode(const std::string& name, bool delay_begin = kWidgetInitImmediate);
  bool BeginLate() override;
  void EndEarly() override;
  ~TreeNode();

 protected:
  bool BoundBegin() override;
  void BoundEnd() override;
};

}  // end namespace scoped_widget

namespace single_widget {

class Button : public SingularWidgetBase {
 public:
  const std::string& Text() const;
  const CguiVec2& Size() const;
  static  Button Delayed(const std::string& text, CguiVec2 size = {});
  Button(const std::string& text, CguiVec2 size = {},
         bool delayed_begin = kWidgetInitImmediate);
  bool BeginLate() override;
  ~Button() = default;

 protected:
  bool BoundBegin() override;

 private:
  std::string text_;
  CguiVec2 size_;
};

class MenuItem : public SingularWidgetBase {
 public:
  const std::string& Text() const;
  const std::string& ShortcutHint() const;
  bool IsEnabled() const;
  static  MenuItem Delayed(const std::string& text,
                                 const std::string& shortcut_hint = "",
                                 bool is_enabled = true);
  MenuItem(const std::string& text, const std::string& shortcut_hint = "",
           bool is_enabled = true, bool delayed_begin = kWidgetInitImmediate);
  bool BeginLate() override;
  ~MenuItem() = default;

 protected:
  bool BoundBegin() override;

 private:
  std::string text_;
  std::string shortcut_hint_;
  bool is_enabled_;
};

class Selectable : public SingularWidgetBase {
 public:
  const std::string& Text() const;
  static  Selectable Delayed(const std::string& text);
  Selectable(const std::string& text,
             bool delayed_begin = kWidgetInitImmediate);
  bool BeginLate() override;
  ~Selectable() = default;

 protected:
  bool BoundBegin() override;

 private:
  std::string text_;
};

class MultilineTextInput : public SingularWidgetBase {
  std::string label_;
  const CguiVec2& size_;
  std::string& buffer_;
  InputTextFlags flags_;

 public:
  const std::string& Label() const;
  const CguiVec2& Size() const;
  std::string& Buffer();
  InputTextFlags& InputFlags();
  const std::string& Buffer() const;
  const InputTextFlags& InputFlags() const;
  static MultilineTextInput Delayed(
      const std::string& label, std::string& buffer,
      const CguiVec2& size = {0.f, 0.f},
      InputTextFlags flags = InputTextFlags());
  MultilineTextInput(const std::string& label, std::string& buffer,
                     const CguiVec2& size = {},
                     InputTextFlags flags = InputTextFlags(),
                     bool delayed_begin = kWidgetInitImmediate);
  bool BeginLate() override;
  ~MultilineTextInput() = default;

 protected:
  bool BoundBegin() override;
};

}  // namespace single_widget

namespace combo_widget {

class DirectoryView : public SingularWidgetBase {
 public:
  using PathT = typename std::filesystem::path;
  using SelectedCallbackT = std::function<void(const PathT&)>;

  DirectoryView(
      const PathT& path, SelectedCallbackT selected_callback = [](auto&& a) {},
      bool is_delayed = false);
  DirectoryView(
      const PathT& path, SelectedCallbackT selected_callback = [](auto&& a) {},
      SelectedCallbackT right_click_callback = [](auto&& a) {},
      bool is_delayed = false);
  bool BeginLate();

 private:
  bool BoundBegin() override;
  void RecursiveDisplayDirectory(const PathT& path, int depth = 0);

  SelectedCallbackT select_file_callback;
  SelectedCallbackT right_click_file_callback;
  const PathT& root;
};

};  // namespace combo_widget

class GraphicsContext {


};

///////////////////////////////////////////////////////////////////////////////
}; /* end namespace cgui */
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/*<library interface>*/
///////////////////////////////////////////////////////////////////////////////

// Forwarded methods from ImGui namespace.
namespace cgui {
using ImGui::SameLine;
using ImGui::Separator;
using ImGui::SetNextWindowPos;
using ImGui::SetNextWindowSize;
}  // namespace cgui

// Common objects.
using CguiVec2 = cgui::CguiVec2;
using CguiUIDGen = cgui::UIDGen;
using CguiUIDIter = cgui::UIDGen::Iter;
using CguiUniqueNameMap = cgui::UniqueNameMap;

// Flag structures.
using CguiFlags = cgui::GuiFlags;
using CguiWindowFlags = cgui::WindowFlags;
using CguiSubcontextFlags = cgui::SubcontextFlags;
using CguiTabBarFlags = cgui::TabBarFlags;
using CguiTabItemFlags = cgui::TabItemFlags;
using CguiInputTextFlags = cgui::InputTextFlags;

// Flag enums.
using CguiWindowFlagEnum = cgui::eWindowFlags;
using CguiSubcontextFlagEnum = cgui::eSubcontextFlags;
using CguiTabBarFlagEnum = cgui::eTabBarFlags;
using CguiTabItemFlagEnum = cgui::eTabItemFlags;
using CguiInputTextFlagEnum = cgui::eInputTextFlags;

// Scoped widgets.
using CguiWindow = cgui::scoped_widget::Window;
using CguiSubcontext = cgui::scoped_widget::Subcontext;
using CguiNamedSubcontext = cgui::scoped_widget::NamedSubcontext;
using CguiMenuBar = cgui::scoped_widget::MenuBar;
using CguiMenu = cgui::scoped_widget::Menu;
using CguiTabBar = cgui::scoped_widget::TabBar;
using CguiTabItem = cgui::scoped_widget::TabItem;
using CguiTreeNode = cgui::scoped_widget::TreeNode;

// Single widgets
using CguiButton = cgui::single_widget::Button;
using CguiMenuItem = cgui::single_widget::MenuItem;
using CguiSelectable = cgui::single_widget::Selectable;
using CguiMultilineTextInput = cgui::single_widget::MultilineTextInput;

// Combo widgets
using CguiDirectoryView = cgui::combo_widget::DirectoryView;

///////////////////////////////////////////////////////////////////////////////
/*<end library interface>*/
///////////////////////////////////////////////////////////////////////////////

// Some examples of how to use the library.
namespace cgui::example {
void ExampleHelloWindow();
//void ExampleMenuBar() {  // creating a menu bar
//  {
//    auto file_menu = CguiMenu("File");
//    if (file_menu) {
//      auto new_submenu = CguiMenu("New");
//      if (new_submenu) {
//        auto solution = CguiMenuItem("Solution");
//      }
//    }
//  }
//}
//void ExampleWindowWithSubcontext() {
//  // A window with two subcontexts.
//  if (true) {
//    // This is to demonstrate windows that go out of scope
//    // automatically.
//    auto new_window = CguiWindow("CoolWindow!");
//    auto new_named_subcontext = CguiNamedSubcontext("HelloContext");
//    auto my_button = CguiButton(new_named_subcontext.Name() +
//                                "'s Button inside" + new_window.Title());
//
//    // End subcontext early to begin new one within the same window context.
//    new_named_subcontext.EndEarly();
//
//    // Can query button properties.It hasnt gone out of scope yet!
//    static bool draw_subcontext_switch =
//        false;  // Note this switch has to be static to
//                // persist throught the frames.
//    if (my_button) {
//      draw_subcontext_switch = not draw_subcontext_switch;
//    }
//
//    if (draw_subcontext_switch) {
//      auto next_unnamed_subcontext = CguiSubcontext();
//      auto abtn = CguiButton(std::to_string(next_unnamed_subcontext.Id()) +
//                             "# Unnamed Subcontext's Btn ");
//    }
//    // automatically goes out of scope.
//  }
//}
//void ExampleTabBar() {
//  // A window with a tab bar.
//  if (true) {
//    auto new_window = CguiWindow("TabBarWindow");
//    auto new_tab_bar = CguiTabBar("TabBar");
//    if (new_tab_bar) {
//      auto tab1 = CguiTabItem("Tab1");
//      if (tab1) {
//        auto btn1 = CguiButton("Button1");
//      }
//      auto tab2 = CguiTabItem("Tab2");
//      if (tab2) {
//        auto btn2 = CguiButton("Button2");
//      }
//    }
//  }
//}
//void ExampleEditorTabs(sf::Window& window) {
//  // A file editor with tabs
//  // Demonstrates use of cgui::kExpandWidgetToRemainingSpaceXY to stretch
//  // widgets. Function is in the context of immediate mode eg. called once
//  // per
//  // render frame.
//  std::string gEditorStringBuffer{""};
//  auto editor_context = CguiNamedSubcontext(
//      "Editor", {static_cast<float>(window.getSize().x * 0.75f),
//                 static_cast<float>(window.getSize().y * 0.75f)});
//  if (editor_context) {
//    auto editor_btn = CguiButton("Editor");
//    auto editor_tab_bar = CguiTabBar(
//        "##file-tabs", {CguiTabBarFlagEnum::ImGuiTabBarFlags_Reorderable});
//    if (editor_tab_bar) {
//      auto selected_tab = CguiTabItem("[Selected]");
//      if (selected_tab)
//        auto file_text_box =
//            CguiMultilineTextInput("Selected_Code", gEditorStringBuffer,
//                                   cgui::kExpandWidgetToRemainingSpaceXY);
//      selected_tab.EndEarly();
//      auto other_tab = CguiTabItem("[Other]");
//      if (other_tab)
//        auto file_text_box =
//            CguiMultilineTextInput("Other_Code", gEditorStringBuffer,
//                                   cgui::kExpandWidgetToRemainingSpaceXY);
//    }
//  }
//}
//struct ExampleAppConsole {
//  char InputBuf[256];
//  ImVector<char*> Items;
//  ImVector<const char*> Commands;
//  ImVector<char*> History;
//  int HistoryPos;  // -1: new line, 0..History.Size-1 browsing history.
//  ImGuiTextFilter Filter;
//  bool AutoScroll;
//  bool ScrollToBottom;
//
//  ExampleAppConsole() {
//    ClearLog();
//    memset(InputBuf, 0, sizeof(InputBuf));
//    HistoryPos = -1;
//
//    // "CLASSIFY" is here to provide the test case where "C"+[tab] completes
//    // to
//    // "CL" and display multiple matches.
//    Commands.push_back("HELP");
//    Commands.push_back("HISTORY");
//    Commands.push_back("CLEAR");
//    Commands.push_back("CLASSIFY");
//    AutoScroll = true;
//    ScrollToBottom = false;
//    AddLog("Welcome to Dear ImGui!");
//  }
//  ~ExampleAppConsole() {
//    ClearLog();
//    for (int i = 0; i < History.Size; i++) free(History[i]);
//  }
//
//  // Portable helpers
//  static int Stricmp(const char* s1, const char* s2) {
//    int d;
//    while ((d = toupper(*s2) - toupper(*s1)) == 0 && *s1) {
//      s1++;
//      s2++;
//    }
//    return d;
//  }
//  static int Strnicmp(const char* s1, const char* s2, int n) {
//    int d = 0;
//    while (n > 0 && (d = toupper(*s2) - toupper(*s1)) == 0 && *s1) {
//      s1++;
//      s2++;
//      n--;
//    }
//    return d;
//  }
//  static char* Strdup(const char* s) {
//    IM_ASSERT(s);
//    size_t len = strlen(s) + 1;
//    void* buf = malloc(len);
//    IM_ASSERT(buf);
//    return (char*)memcpy(buf, (const void*)s, len);
//  }
//  static void Strtrim(char* s) {
//    char* str_end = s + strlen(s);
//    while (str_end > s && str_end[-1] == ' ') str_end--;
//    *str_end = 0;
//  }
//
//  void ClearLog() {
//    for (int i = 0; i < Items.Size; i++) free(Items[i]);
//    Items.clear();
//  }
//
//  void AddLog(const char* fmt, ...) IM_FMTARGS(2) {
//    // FIXME-OPT
//    char buf[1024];
//    va_list args;
//    va_start(args, fmt);
//    vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
//    buf[IM_ARRAYSIZE(buf) - 1] = 0;
//    va_end(args);
//    Items.push_back(Strdup(buf));
//  }
//
//  void Draw(const char* title, bool* p_open) {
//    ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
//    if (!ImGui::BeginChild(title)) {
//      ImGui::EndChild();
//      return;
//    }
//
//    // As a specific feature guaranteed by the library, after calling Begin()
//    // the last Item represent the title bar. So e.g. IsItemHovered() will
//    // return true when hovering the title bar. Here we create a context menu
//    // only available from the title bar.
//    if (ImGui::BeginPopupContextItem()) {
//      if (ImGui::MenuItem("Close Console")) *p_open = false;
//      ImGui::EndPopup();
//    }
//
//    ImGui::TextWrapped("");
//    ImGui::TextWrapped("Enter 'HELP' for help.");
//
//    // TODO: display items starting from the bottom
//
//    if (ImGui::SmallButton("Add Debug Text")) {
//      AddLog("%d some text", Items.Size);
//      AddLog("some more text");
//      AddLog("display very important message here!");
//    }
//    ImGui::SameLine();
//    if (ImGui::SmallButton("Add Debug Error")) {
//      AddLog("[error] something went wrong");
//    }
//    ImGui::SameLine();
//    if (ImGui::SmallButton("Clear")) {
//      ClearLog();
//    }
//    ImGui::SameLine();
//    bool copy_to_clipboard = ImGui::SmallButton("Copy");
//    // static float t = 0.0f; if (ImGui::GetTime() - t > 0.02f) { t =
//    // ImGui::GetTime(); AddLog("Spam %f", t); }
//
//    ImGui::Separator();
//
//    // Options menu
//    if (ImGui::BeginPopup("Options")) {
//      ImGui::Checkbox("Auto-scroll", &AutoScroll);
//      ImGui::EndPopup();
//    }
//
//    // Options, Filter
//    if (ImGui::Button("Options")) ImGui::OpenPopup("Options");
//    ImGui::SameLine();
//    Filter.Draw("Filter (\"incl,-excl\") (\"error\")", 180);
//    ImGui::Separator();
//
//    // Reserve enough left-over height for 1 separator + 1 input text
//    const float footer_height_to_reserve =
//        ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
//    if (ImGui::BeginChild(
//            "ScrollingRegion", ImVec2(0, -footer_height_to_reserve),
//            ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar)) {
//      if (ImGui::BeginPopupContextWindow()) {
//        if (ImGui::Selectable("Clear")) ClearLog();
//        ImGui::EndPopup();
//      }
//
//      // Display every line as a separate entry so we can change their color
//      // or
//      // add custom widgets. If you only want raw text you can use
//      // ImGui::TextUnformatted(log.begin(), log.end()); NB- if you have
//      // thousands of entries this approach may be too inefficient and may
//      // require user-side clipping to only process visible items. The
//      // clipper
//      // will automatically measure the height of your first item and then
//      // "seek" to display only items in the visible area.
//      // To use the clipper we can replace your standard loop:
//      //      for (int i = 0; i < Items.Size; i++)
//      //   With:
//      //      ImGuiListClipper clipper;
//      //      clipper.Begin(Items.Size);
//      //      while (clipper.Step())
//      //         for (int i = clipper.DisplayStart; i < clipper.DisplayEnd;
//      // i++)
//      // - That your items are evenly spaced (same height)
//      // - That you have cheap random access to your elements (you can access
//      // them given their index,
//      //   without processing all the ones before)
//      // You cannot this code as-is if a filter is active because it breaks
//      // the
//      // 'cheap random-access' property. We would need random-access on the
//      // post-filtered list. A typical application wanting coarse clipping
//      // and
//      // filtering may want to pre-compute an array of indices or offsets of
//      // items that passed the filtering test, recomputing this array when
//      // user
//      // changes the filter, and appending newly elements as they are
//      // inserted.
//      // This is left as a task to the user until we can manage to improve
//      // this
//      // example code! If your items are of variable height:
//      // - Split them into same height items would be simpler and facilitate
//      // random-seeking into your list.
//      // - Consider using manual call to IsRectVisible() and skipping
//      // extraneous
//      // decoration from your items.
//      ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,
//                          ImVec2(4, 1));  // Tighten spacing
//      if (copy_to_clipboard) ImGui::LogToClipboard();
//      for (const char* item : Items) {
//        if (!Filter.PassFilter(item)) continue;
//
//        // Normally you would store more information in your item than just a
//        // string. (e.g. make Items[] an array of structure, store color/type
//        // etc.)
//        ImVec4 color;
//        bool has_color = false;
//        if (strstr(item, "[error]")) {
//          color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f);
//          has_color = true;
//        } else if (strncmp(item, "# ", 2) == 0) {
//          color = ImVec4(1.0f, 0.8f, 0.6f, 1.0f);
//          has_color = true;
//        }
//        if (has_color) ImGui::PushStyleColor(ImGuiCol_Text, color);
//        ImGui::TextUnformatted(item);
//        if (has_color) ImGui::PopStyleColor();
//      }
//      if (copy_to_clipboard) ImGui::LogFinish();
//
//      // Keep up at the bottom of the scroll region if we were already at the
//      // bottom at the beginning of the frame. Using a scrollbar or
//      // mouse-wheel
//      // will take away from the bottom edge.
//      if (ScrollToBottom ||
//          (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
//        ImGui::SetScrollHereY(1.0f);
//      ScrollToBottom = false;
//
//      ImGui::PopStyleVar();
//    }
//    ImGui::EndChild();
//    ImGui::Separator();
//
//    // Command-line
//    bool reclaim_focus = false;
//    ImGuiInputTextFlags input_text_flags =
//        ImGuiInputTextFlags_EnterReturnsTrue |
//        ImGuiInputTextFlags_EscapeClearsAll |
//        ImGuiInputTextFlags_CallbackCompletion |
//        ImGuiInputTextFlags_CallbackHistory;
//    if (ImGui::InputText("Input", InputBuf, IM_ARRAYSIZE(InputBuf),
//                         input_text_flags, &TextEditCallbackStub,
//                         (void*)this)) {
//      char* s = InputBuf;
//      Strtrim(s);
//      if (s[0]) ExecCommand(s);
//      strcpy_s(s, sizeof s, "");
//      reclaim_focus = true;
//    }
//
//    // Auto-focus on window apparition
//    ImGui::SetItemDefaultFocus();
//    if (reclaim_focus)
//      ImGui::SetKeyboardFocusHere(-1);  // Auto focus previous widget
//
//    ImGui::EndChild();
//  }
//
//  void ExecCommand(const char* command_line) {
//    AddLog("# %s\n", command_line);
//
//    // Insert into history. First find match and delete it so it can be
//    // pushed
//    // to the back. This isn't trying to be smart or optimal.
//    HistoryPos = -1;
//    for (int i = History.Size - 1; i >= 0; i--)
//      if (Stricmp(History[i], command_line) == 0) {
//        free(History[i]);
//        History.erase(History.begin() + i);
//        break;
//      }
//    History.push_back(Strdup(command_line));
//
//    // Process command
//    if (Stricmp(command_line, "CLEAR") == 0) {
//      ClearLog();
//    } else if (Stricmp(command_line, "HELP") == 0) {
//      AddLog("Commands:");
//      for (int i = 0; i < Commands.Size; i++) AddLog("- %s", Commands[i]);
//    } else if (Stricmp(command_line, "HISTORY") == 0) {
//      int first = History.Size - 10;
//      for (int i = first > 0 ? first : 0; i < History.Size; i++)
//        AddLog("%3d: %s\n", i, History[i]);
//    } else {
//      AddLog("Unknown command: '%s'\n", command_line);
//    }
//
//    // On command input, we scroll to bottom even if AutoScroll==false
//    ScrollToBottom = true;
//  }
//
//  // In C++11 you'd be better off using lambdas for this sort of forwarding
//  // callbacks
//  static int TextEditCallbackStub(ImGuiInputTextCallbackData* data) {
//    ExampleAppConsole* console = (ExampleAppConsole*)data->UserData;
//    return console->TextEditCallback(data);
//  }
//
//  int TextEditCallback(ImGuiInputTextCallbackData* data) {
//    // AddLog("cursor: %d, selection: %d-%d", data->CursorPos,
//    // data->SelectionStart, data->SelectionEnd);
//    switch (data->EventFlag) {
//      case ImGuiInputTextFlags_CallbackCompletion: {
//        // Example of TEXT COMPLETION
//
//        // Locate beginning of current word
//        const char* word_end = data->Buf + data->CursorPos;
//        const char* word_start = word_end;
//        while (word_start > data->Buf) {
//          const char c = word_start[-1];
//          if (c == ' ' || c == '\t' || c == ',' || c == ';') break;
//          word_start--;
//        }
//
//        // Build a list of candidates
//        ImVector<const char*> candidates;
//        for (int i = 0; i < Commands.Size; i++)
//          if (Strnicmp(Commands[i], word_start, (int)(word_end - word_start)) ==
//              0)
//            candidates.push_back(Commands[i]);
//
//        if (candidates.Size == 0) {
//          // No match
//          AddLog("No match for \"%.*s\"!\n", (int)(word_end - word_start),
//                 word_start);
//        } else if (candidates.Size == 1) {
//          // Single match. Delete the beginning of the word and replace it
//          // entirely so we've got nice casing.
//          data->DeleteChars((int)(word_start - data->Buf),
//                            (int)(word_end - word_start));
//          data->InsertChars(data->CursorPos, candidates[0]);
//          data->InsertChars(data->CursorPos, " ");
//        } else {
//          // Multiple matches. Complete as much as we can..
//          // So inputing "C"+Tab will complete to "CL" then display "CLEAR"
//          // and
//          // "CLASSIFY" as matches.
//          int match_len = (int)(word_end - word_start);
//          for (;;) {
//            int c = 0;
//            bool all_candidates_matches = true;
//            for (int i = 0; i < candidates.Size && all_candidates_matches; i++)
//              if (i == 0)
//                c = toupper(candidates[i][match_len]);
//              else if (c == 0 || c != toupper(candidates[i][match_len]))
//                all_candidates_matches = false;
//            if (!all_candidates_matches) break;
//            match_len++;
//          }
//
//          if (match_len > 0) {
//            data->DeleteChars((int)(word_start - data->Buf),
//                              (int)(word_end - word_start));
//            data->InsertChars(data->CursorPos, candidates[0],
//                              candidates[0] + match_len);
//          }
//
//          // List matches
//          AddLog("Possible matches:\n");
//          for (int i = 0; i < candidates.Size; i++)
//            AddLog("- %s\n", candidates[i]);
//        }
//
//        break;
//      }
//      case ImGuiInputTextFlags_CallbackHistory: {
//        // Example of HISTORY
//        const int prev_history_pos = HistoryPos;
//        if (data->EventKey == ImGuiKey_UpArrow) {
//          if (HistoryPos == -1)
//            HistoryPos = History.Size - 1;
//          else if (HistoryPos > 0)
//            HistoryPos--;
//        } else if (data->EventKey == ImGuiKey_DownArrow) {
//          if (HistoryPos != -1)
//            if (++HistoryPos >= History.Size) HistoryPos = -1;
//        }
//
//        // A better implementation would preserve the data on the current
//        // input
//        // line along with cursor position.
//        if (prev_history_pos != HistoryPos) {
//          const char* history_str =
//              (HistoryPos >= 0) ? History[HistoryPos] : "";
//          data->DeleteChars(0, data->BufTextLen);
//          data->InsertChars(0, history_str);
//        }
//      }
//    }
//    return 0;
//  }
//};
//static void ShowAppConsole() {
//  static ExampleAppConsole console;
//  static bool po{true};
//  console.Draw("Example: Console", &po);
//}
};  // namespace cgui::example

/// @} // end of cand_cide_cgui

///////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language Environment
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
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
///////////////////////////////////////////////////////////////////////////////
