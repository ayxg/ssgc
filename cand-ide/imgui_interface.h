#pragma once
#include <atomic>
#include <unordered_set>

#include "../submodules/extended-cpp-standard/cppsextended.h"
#include "imgui-SFML.h"
#include "imgui.h"
#include "imgui_stdlib.h"

/////////////////////////////////////////////////////////////////////////////////
namespace cgui {
/////////////////////////////////////////////////////////////////////////////////
// C++ 20 Interface to the ImGui library
// 1. No need to call ImGui::Begin and ImGui::End for each window/widget.
// 2. No raw pointers.
// 3. Unified use of std::string.
// 4. Names of windows must be unique. Handle possible errors from occurence.
// 5. Widget IDs must be unique, are automatically generated.
//
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// <decls>
/////////////////////////////////////////////////////////////////////////////////

using std::size_t;
using std::string;

using ImGuiFlags = int;
using eWindowFlags = ImGuiWindowFlags_;
using eSubcontextFlags = ImGuiChildFlags_;
using eTabBarFlags = ImGuiTabBarFlags_;
using eTabItemFlags = ImGuiTabItemFlags_;

class UIDGen;         // Generates unique widget identifiers.
class UniqueNameMap;  // Maintains unique names across widgets.

// Flag Structs: Represent ImGui Flags.
class WindowFlags;
class SubcontextFlags;
class TabBarFlags;
class TabItemFlags;

// Represents open/closed/visible/pressed/hovered state
// depending on the ImGui widget type.
enum class eWidgetState {
  kFree,      // state will be inferred.
  kForceOff,  // force false state (if possible)
  kForceOn,   // force true state(if possible)
};
struct WidgetBase;  // Widget base class

// Widgets
class Window;
class UnnamedSubcontext;
class Subcontext;

class MenuBar;   // main menu bar belonging to a context
class Menu;      // drop down menu, can have submenus
class MenuItem;  // An entry in the menu.

// Single Instance Widgets : which dont require and unique id or name.
// These do not use the WidgetMaker class to construct.
class Button;

// Widget Factory Class: links a UIDGen and UniqueNameMap to a group of widgets.
class WidgetMaker;

/////////////////////////////////////////////////////////////////////////////////
// <enddecls>
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// <defs>
/////////////////////////////////////////////////////////////////////////////////
class UIDGen {
 public:
  using Iter = std::unordered_set<size_t>::iterator;
  UIDGen() : next_id_(1) {}  // Starts at 1 cause ImGui reserves id 0
  // Do a try catch block around this method if you wish to refresh on overflow.
  Iter GetId();
  void PopId();
  void EraseId(Iter it);
  const std::unordered_set<size_t>& Generated();
  void Refresh();
  bool Empty() { return generated_ids_.empty(); }

 private:
  std::atomic<size_t> next_id_;
  std::unordered_set<size_t> generated_ids_;
};

class UniqueNameMap {
 public:
  cxx::BoolError AddName(const std::string& str);
  cxx::BoolError RemoveName(const std::string& str);
  bool Contains(const std::string& str) { return names_.contains(str); }

 private:
  std::unordered_set<string> names_;
};

class WindowFlags {
 public:
  ImGuiFlags flags_{0};
  WindowFlags() = default;
  WindowFlags(eWindowFlags flags) : flags_(flags) {}
  WindowFlags(eWindowFlags flags,
              std::same_as<eWindowFlags> auto... other_flags)
      : flags_(flags) {
    (..., (flags_ |= other_flags));
  }
  ImGuiFlags Get() const { return flags_; }
  constexpr operator eWindowFlags() const {
    return static_cast<eWindowFlags>(flags_);
  }
};

class SubcontextFlags {
 public:
  ImGuiFlags flags_{0};
  SubcontextFlags() = default;
  SubcontextFlags(eSubcontextFlags flags) : flags_(flags) {}
  SubcontextFlags(eSubcontextFlags flags,
                  std::same_as<eSubcontextFlags> auto... other_flags)
      : flags_(flags) {
    (..., (flags_ |= other_flags));
  }
  int Get() const { return flags_; }
  constexpr operator eSubcontextFlags() const {
    return static_cast<eSubcontextFlags>(flags_);
  }
};

class TabBarFlags {
 public:
  ImGuiFlags flags_{0};
  TabBarFlags() = default;
  TabBarFlags(eTabBarFlags flags) : flags_(flags) {}
  TabBarFlags(eTabBarFlags flags,
              std::same_as<eTabBarFlags> auto... other_flags)
      : flags_(flags) {
    (..., (flags_ |= other_flags));
  }
  int Get() const { return flags_; }
  constexpr operator eTabBarFlags() const {
    return static_cast<eTabBarFlags>(flags_);
  }
};

class TabItemFlags {
 public:
  ImGuiFlags flags_{0};
  TabItemFlags() = default;
  TabItemFlags(eTabItemFlags flags) : flags_(flags) {}
  TabItemFlags(eTabItemFlags flags,
               std::same_as<eTabItemFlags> auto... other_flags)
      : flags_(flags) {
    (..., (flags_ |= other_flags));
  }
  int Get() const { return flags_; }
  constexpr operator eTabItemFlags() const {
    return static_cast<eTabItemFlags>(flags_);
  }
};

struct WidgetBase {
  UniqueNameMap& name_map_;
  UIDGen& id_gen_;
  std::unique_ptr<bool> is_open_{nullptr};
  bool is_scope_active_ = false;
  WidgetBase(UniqueNameMap& name_map, UIDGen& id_gen)
      : name_map_(name_map), id_gen_(id_gen) {}
  virtual bool BeginLate() = 0;
  virtual void EndEarly() = 0;
  virtual ~WidgetBase() = default;

 protected:
  // Name must not exist.
  cxx::BoolError NewName(const std::string& str);
  // Generate a new uuid
  UIDGen::Iter NewId() { return id_gen_.GetId(); }
  // Name must exist. For adding to widgets 'later'.
  cxx::BoolError SetName(const std::string& str);
  inline void HandleWidgetState(eWidgetState state);

 public:
  // Returns true if the widget is active. This may be an open window, or a
  // pressed button.
  bool IsOpen() const { return *is_open_; }
  // Returns true is the Begin() function has been called, further gui commands
  // will add to this scope.
  bool IsScopeActive() const { return is_scope_active_; }
  operator bool() {
    return IsOpen();
  }  // Convertible to bool. Value of is_open.
};

class Window : public WidgetBase {
  std::string title_{""};
  WindowFlags flags_;

 public:
  Window(UniqueNameMap& name_map, UIDGen& id_gen, const std::string& title,
         eWidgetState state = eWidgetState::kFree,
         WindowFlags flags = WindowFlags(), bool delay_begin = false);
  ~Window();
  bool BeginLate() override;
  void EndEarly() override;
  const std::string& Name() { return title_; }
};

class UnnamedSubcontext : public WidgetBase {
  UIDGen::Iter uid_;
  WindowFlags win_flags_;
  SubcontextFlags subcontext_flags_;
  std::pair<float, float> requested_size_;

 public:
  auto Id() const { return *uid_; }
  UnnamedSubcontext(UniqueNameMap& name_map, UIDGen& id_gen,
                    std::pair<float, float> size = {0.f, 0.f},
                    eWidgetState state = eWidgetState::kFree,
                    WindowFlags win_flags = WindowFlags(),
                    SubcontextFlags subcontext_flags = SubcontextFlags(),
                    bool delay_begin = false);
  bool BeginLate() override;
  void EndEarly() override;
  ~UnnamedSubcontext();
};

class Subcontext : public WidgetBase {
  std::string name_;
  WindowFlags win_flags_;
  SubcontextFlags subcontext_flags_;
  std::pair<float, float> requested_size_;

 public:
  const std::string& Name() { return name_; }
  Subcontext(UniqueNameMap& name_map, UIDGen& id_gen, const std::string& name,
             std::pair<float, float> size = {0.f, 0.f},
             eWidgetState state = eWidgetState::kFree,
             WindowFlags win_flags = WindowFlags(),
             SubcontextFlags subcontext_flags = SubcontextFlags(),
             bool delay_begin = false);
  bool BeginLate() override;
  void EndEarly() override;
  ~Subcontext();
};

class MenuBar : public WidgetBase {
 public:
  MenuBar(UniqueNameMap& name_map, UIDGen& id_gen, bool delay_begin = false);
  ~MenuBar();
  bool BeginLate() override;
  void EndEarly() override;
};

class Menu : public WidgetBase {
  std::string title_{""};
  bool is_enabled_ = true;  // enable/disable this menu from showing up
 public:
  Menu(UniqueNameMap& name_map, UIDGen& id_gen, const std::string& title,
       bool is_enabled = true, bool delay_begin = false);
  ~Menu();
  bool BeginLate() override;
  void EndEarly() override;
  const std::string& Name() { return title_; }
};

class TabBar : public WidgetBase {
  std::string name_;
  TabBarFlags flags_;

 public:
  TabBar(UniqueNameMap& name_map, UIDGen& id_gen,
         TabBarFlags flags = TabBarFlags(), bool delay_begin = false);
  ~TabBar();
  bool BeginLate() override;
  void EndEarly() override;
};

class TabItem : public WidgetBase {
  std::string name_;
  TabItemFlags flags_;

 public:
  TabItem(UniqueNameMap& name_map, UIDGen& id_gen,
          eWidgetState state = eWidgetState::kFree,
          TabItemFlags flags = TabItemFlags(), bool delay_begin = false);
  ~TabItem();
  bool BeginLate() override;
  void EndEarly() override;
};

struct Button {
  const std::string& text;
  const bool is_pressed;
  Button(const std::string& text, std::pair<float, float> size = {})
      : text(text),
        is_pressed(ImGui::Button(text.c_str(), {size.first, size.second})) {}
};

struct MenuItem {
  const std::string& text;
  const bool is_selected;
  MenuItem(const std::string& text, const std::string& shortcut_hint = "",
           bool is_enabled = true)
      : text(text),
        is_selected(ImGui::MenuItem(text.c_str(), shortcut_hint.c_str(), false,
                                    is_enabled)) {}
};

class WidgetMaker {
  UniqueNameMap name_map_;
  UIDGen id_gen_;

 public:
  // Window widget.
  Window MakeWindow(const std::string& title,
                    eWidgetState state = eWidgetState::kFree,
                    WindowFlags flags = WindowFlags(),
                    bool delay_begin = false) {
    return Window(name_map_, id_gen_, title, state, flags, delay_begin);
  }

  // Subcontext Widget
  Subcontext MakeSubcontext(
      const std::string& name, std::pair<float, float> size = {0.f, 0.f},
      eWidgetState state = eWidgetState::kFree,
      WindowFlags win_flags = WindowFlags(),
      SubcontextFlags subcontext_flags = SubcontextFlags(),
      bool delay_begin = false) {
    return Subcontext(name_map_, id_gen_, name, size, state, win_flags,
                      subcontext_flags, delay_begin);
  }

  // UnnamedSubcontext
  UnnamedSubcontext MakeUnnamedSubcontext(
      std::pair<float, float> size = {0.f, 0.f},
      eWidgetState state = eWidgetState::kFree,
      WindowFlags win_flags = WindowFlags(),
      SubcontextFlags subcontext_flags = SubcontextFlags(),
      bool delay_begin = false) {
    return UnnamedSubcontext(name_map_, id_gen_, size, state, win_flags,
                             subcontext_flags, delay_begin);
  }

  // MainMenuBar
  MenuBar MakeMenuBar(bool delay_begin = false) {
    return MenuBar(name_map_, id_gen_, delay_begin);
  }

  // Menu
  Menu MakeMenu(const std::string& title, bool is_enabled = true,
                bool delay_begin = false) {
    return Menu(name_map_, id_gen_, title, is_enabled, delay_begin);
  }

  // Tree
};

/////////////////////////////////////////////////////////////////////////////////
// <enddefs>
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// <impl:UIDGen>
/////////////////////////////////////////////////////////////////////////////////

// Do a try catch block around this method if you wish to refresh on overflow.
UIDGen::Iter UIDGen::GetId() {
  if (next_id_ == SIZE_MAX) throw std::overflow_error("UIDGen: ID overflow");
  size_t id = next_id_.fetch_add(1);
  generated_ids_.insert(id);
  return generated_ids_.find(id);
}

void UIDGen::PopId() {
  if (generated_ids_.empty()) throw std::runtime_error("UIDGen: No IDs to pop");
  generated_ids_.erase(next_id_);
  next_id_.fetch_sub(1);
}

// Erases an id but does not reset counter. If generating many id and deleting
// use Referesh() to reset the counter and set.
void UIDGen::EraseId(Iter it) {
  if (not generated_ids_.empty() and (it == std::prev(generated_ids_.end()))) {
    // If the id we are erasing is the last id, we can also pop to save make
    // more uids
    PopId();
  }
  generated_ids_.erase(it);
}

const std::unordered_set<size_t>& UIDGen::Generated() { return generated_ids_; }

// Pops all ids and sets next_id_ = 0;
void UIDGen::Refresh() {
  generated_ids_.clear();
  next_id_.store(1);
}
/////////////////////////////////////////////////////////////////////////////////
// <endimpl:UIDGen>
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// <impl:UniqueNameMap>
/////////////////////////////////////////////////////////////////////////////////
cxx::BoolError UniqueNameMap::AddName(const std::string& str) {
  if (names_.contains(str))
    return "[UniqueNameMap:AddName:This widget name is already in use.]";
  names_.insert(str);
  return true;
}

cxx::BoolError UniqueNameMap::RemoveName(const std::string& str) {
  if (!names_.contains(str))
    return "[UniqueNameMap:RemoveName:This widget name does not exist.]";
  names_.erase(str);
  return true;
}

/////////////////////////////////////////////////////////////////////////////////
// <endimpl:UniqueNameMap>
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// <impl:WidgetBase>
/////////////////////////////////////////////////////////////////////////////////

// Name must not exist.
cxx::BoolError WidgetBase::NewName(const std::string& str) {
  if (name_map_.Contains(str))
    return "[WidgetBase:NewName:This widget name is already in use.]";
  name_map_.AddName(str);
  return true;
};

// Name must exist. For adding to widgets 'later'.
cxx::BoolError WidgetBase::SetName(const std::string& str) {
  if (!name_map_.Contains(str))
    return "[WidgetBase:SetName:No widget with such name exists.]";
  return true;
}

inline void WidgetBase::HandleWidgetState(eWidgetState state) {
  // handle is_open based on widget state
  if (state == eWidgetState::kForceOff) {
    is_open_ = std::make_unique<bool>(false);
  } else if (state == eWidgetState::kForceOn) {
    is_open_ = std::make_unique<bool>(true);
  }
}
/////////////////////////////////////////////////////////////////////////////////
// <endimpl:WidgetBase>
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// <impl:Window>
/////////////////////////////////////////////////////////////////////////////////
Window::Window(UniqueNameMap& name_map, UIDGen& id_gen,
               const std::string& title, eWidgetState state, WindowFlags flags,
               bool delay_begin)
    : WidgetBase(name_map, id_gen) {
  cxx::BoolError valid_name = NewName(title);
  if (valid_name) {
    title_ = title;
  } else
    throw valid_name.Exception();

  // handle is_open based on widget state
  if (state == eWidgetState::kForceOff) {
    is_open_ = std::make_unique<bool>(false);
  } else if (state == eWidgetState::kForceOn) {
    is_open_ = std::make_unique<bool>(true);
  }
  flags_ = flags;

  if (not delay_begin) {
    is_scope_active_ = true;
    ImGui::Begin(title_.c_str(), is_open_.get(), flags_);
  }
}

bool Window::BeginLate() {
  if (not is_scope_active_) {
    is_scope_active_ = true;
    ImGui::Begin(title_.c_str(), is_open_.get(), flags_);
  }
  return IsOpen();
}

void Window::EndEarly() {
  if (not is_scope_active_) throw "[WindowWidget:End:end called before begin.]";
  is_scope_active_ = false;
  ImGui::End();
}

Window::~Window() {
  // End the gui object.
  if (is_scope_active_) ImGui::End();

  // Remove the name from name map.
  name_map_.RemoveName(title_);
}
/////////////////////////////////////////////////////////////////////////////////
// <endimpl:Window>
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// <impl:UnnamedSubcontext>
/////////////////////////////////////////////////////////////////////////////////

UnnamedSubcontext::UnnamedSubcontext(UniqueNameMap& name_map, UIDGen& id_gen,
                                     std::pair<float, float> size,
                                     eWidgetState state, WindowFlags win_flags,
                                     SubcontextFlags subcontext_flags,
                                     bool delay_begin)
    : WidgetBase(name_map, id_gen) {
  HandleWidgetState(state);
  uid_ = NewId();
  win_flags_ = win_flags;
  subcontext_flags_ = subcontext_flags;
  requested_size_ = size;
  if (not delay_begin) {
    is_scope_active_ = true;
    is_open_ = std::make_unique<bool>(ImGui::BeginChild(
        *uid_, {requested_size_.first, requested_size_.second},
        subcontext_flags_, win_flags_));
  }
}

bool UnnamedSubcontext::BeginLate() {
  if (not is_scope_active_) {
    is_scope_active_ = true;
    is_open_ = std::make_unique<bool>(ImGui::BeginChild(
        *uid_, {requested_size_.first, requested_size_.second},
        subcontext_flags_, win_flags_));
    return IsOpen();
  }
  throw "[UnnamedSubcontext:BeginLate:begin called twice, set delay_begin true on construct.]";
}

void UnnamedSubcontext::EndEarly() {
  if (not is_scope_active_)
    throw "[UnnamedSubcontext:End:end called before begin.]";
  is_scope_active_ = false;
  if (*is_open_) ImGui::EndChild();
}

UnnamedSubcontext::~UnnamedSubcontext() {
  // End the gui object.
  if (is_scope_active_ && *is_open_) ImGui::EndChild();

  // Remove the id
  id_gen_.EraseId(uid_);
}

/////////////////////////////////////////////////////////////////////////////////
// <endimpl:UnnamedSubcontext>
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// <impl:Subcontext>
/////////////////////////////////////////////////////////////////////////////////
Subcontext::Subcontext(UniqueNameMap& name_map, UIDGen& id_gen,
                       const std::string& name, std::pair<float, float> size,
                       eWidgetState state, WindowFlags win_flags,
                       SubcontextFlags subcontext_flags, bool delay_begin)
    : WidgetBase(name_map, id_gen) {
  HandleWidgetState(state);
  cxx::BoolError valid_name = NewName(name);
  if (valid_name) {
    name_ = name;
  } else
    throw valid_name.Exception();
  win_flags_ = win_flags;
  subcontext_flags_ = subcontext_flags;
  requested_size_ = size;
  if (not delay_begin) {
    is_scope_active_ = true;
    is_open_ = std::make_unique<bool>(ImGui::BeginChild(
        name_.c_str(), {requested_size_.first, requested_size_.second},
        subcontext_flags_, win_flags_));
  }
}

bool Subcontext::BeginLate() {
  if (not is_scope_active_) {
    is_scope_active_ = true;
    is_open_ = std::make_unique<bool>(ImGui::BeginChild(
        name_.c_str(), {requested_size_.first, requested_size_.second},
        subcontext_flags_, win_flags_));
    return IsOpen();
  }
  throw "[Subcontext:BeginLate:begin called twice, set delay_begin true on construct.]";
}

void Subcontext::EndEarly() {
  if (not is_scope_active_) throw "[Subcontext:End:end called before begin.]";
  is_scope_active_ = false;
  if (*is_open_) ImGui::EndChild();
}

Subcontext::~Subcontext() {
  // End the gui object.
  if (is_scope_active_ && *is_open_) ImGui::EndChild();

  // Remove the name from name map.
  name_map_.RemoveName(name_);
}
/////////////////////////////////////////////////////////////////////////////////
// <endimpl:Subcontext>
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// <impl:MenuBar>
/////////////////////////////////////////////////////////////////////////////////
MenuBar::MenuBar(UniqueNameMap& name_map, UIDGen& id_gen, bool delay_begin)
    : WidgetBase(name_map, id_gen) {
  if (not delay_begin) {
    is_scope_active_ = true;
    is_open_ = std::make_unique<bool>(ImGui::BeginMenuBar());
  }
};
MenuBar::~MenuBar() {
  if (is_scope_active_ && IsOpen()) ImGui::EndMenuBar();
};
bool MenuBar::BeginLate() {
  if (not is_scope_active_) {
    is_scope_active_ = true;
    is_open_ = std::make_unique<bool>(ImGui::BeginMenuBar());
  }
  return IsOpen();
};
void MenuBar::EndEarly() {
  if (not is_scope_active_) throw "[Subcontext:End:end called before begin.]";
  is_scope_active_ = false;
  if (IsOpen()) ImGui::EndMenuBar();
};
/////////////////////////////////////////////////////////////////////////////////
// <endimpl:MenuBar>
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// <impl:Menu>
/////////////////////////////////////////////////////////////////////////////////

Menu::Menu(UniqueNameMap& name_map, UIDGen& id_gen, const std::string& title,
           bool is_enabled, bool delay_begin)
    : WidgetBase(name_map, id_gen) {
  cxx::BoolError valid_name = NewName(title);
  if (valid_name) {
    title_ = title;
  } else
    throw valid_name.Exception();

  if (not delay_begin) {
    is_scope_active_ = true;
    is_open_ =
        std::make_unique<bool>(ImGui::BeginMenu(title_.c_str(), is_enabled_));
  }
};
Menu::~Menu() {
  // End the gui object.
  if (is_scope_active_ && *is_open_) ImGui::EndMenu();

  // Remove the name from name map.
  name_map_.RemoveName(title_);
};
bool Menu::BeginLate() {
  if (not is_scope_active_) {
    is_scope_active_ = true;
    is_open_ =
        std::make_unique<bool>(ImGui::BeginMenu(title_.c_str(), is_enabled_));
  }
  return IsOpen();
};
void Menu::EndEarly() {
  if (not is_scope_active_) throw "[WindowWidget:End:end called before begin.]";
  is_scope_active_ = false;
  if (*is_open_) ImGui::EndMenu();
};
/////////////////////////////////////////////////////////////////////////////////
// <endimpl:Menu>
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// <impl:MenuBar>
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// <endimpl:MenuBar>
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
}  // end namespace cgui
/////////////////////////////////////////////////////////////////////////////////

void WIDGET_BASE_USE_EXAMPLE() {
  // "global" id generators.
  cgui::UIDGen idgen;
  cgui::UniqueNameMap nmap;
  cgui::Window my_window{nmap, idgen, "Cool0!"};

  // Do things based on if the window is open or not.
  // Use my_window.IsOpen() to be explicit.
  if (my_window)            // use implict bool conversion.
    std::cout << "Cool1!";  // Draw other elements inside window etc.

  // Widget will automaticall call end when going out of scope.
  // or we can call end early.
  my_window.EndEarly();

  // This is now outside the window's scope.
  my_window.IsScopeActive();  // to query scope state.

  // restart the same window's scope if it still in the overall C++ scope.
  if (my_window.BeginLate()) {
    // ImGui standard syntax for Begin/End. But calling End is not necessary.
    std::cout << "Cool2!";
  };
  // add more stuff.

  // window will end itself here.

  // Using the widget maker is easier!
  cgui::WidgetMaker make_gui;
  auto a_window = make_gui.MakeWindow("Coooool!");
}
static void cGuiDrawMenuFile() {
  cgui::WidgetMaker make_gui;

  // creating a menu bar
  {
    auto file_menu = make_gui.MakeMenu("File");
    if (file_menu.IsOpen()) {
      auto new_submenu = make_gui.MakeMenu("New");
      if (new_submenu.IsOpen()) {
        auto solution = cgui::MenuItem("Solution");
      }
    }
  }
}