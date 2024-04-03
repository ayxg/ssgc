#pragma once
#include <atomic>
#include <concepts>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <tuple>
#include <type_traits>
#include <unordered_set>

#include "../submodules/extended-cpp-standard/cppsextended.h"
#include "imgui-SFML.h"
#include "imgui.h"
#include "imgui_stdlib.h"

//===========================================================================//
namespace cgui { /* cgui namespace */
//===========================================================================//
/* C++ 20 Interface to the ImGui library
 * 1. No need to call ImGui::Begin and ImGui::End for each window/widget.
 * 2. No raw pointers.
 * 3. Unified use of std::string.
 * 4. Names of windows must be unique. Handle possible errors from occurence.
 * 5. Widget IDs must be unique, are automatically generated.
 */
//===========================================================================//

//===========================================================================//
/*<decls>*/
//===========================================================================//

using std::size_t;
using std::string;

// Internal vec2 representation, for now its a pair to keep it simple.
using CguiVec2 = std::pair<float, float>;

using GuiFlags = int;
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

class UIDGen;              // Generates unique widget identifiers.
class UniqueNameMap;       // Maintains unique names across widgets.
class ScopedWidgetBase;    // Base class for all scoped widgets.
class SingularWidgetBase;  // Base class for all singular widgets.

namespace scoped_widget {
class Window;
class Subcontext;       // Autogenerates unique id.
class NamedSubcontext;  // Name must be unique or throws.
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
class DirectoryView;  // TreeNode view of a file path,single widget base.
}  // namespace combo_widget

//===========================================================================//
/*<enddecls>*/
//===========================================================================//

// Implicit global unique id generator for cgui widget classes.
// !! Do not use/access directly, called by ScopedWidgetBase on default
// construction.
static UIDGen gCguiDefaultUIDGenerator{};
// Implicit global unique name map for cgui widget classes.
// !! Do not use/access directly, called by ScopedWidgetBase on default
// construction.
static UniqueNameMap gCguiDefaultUniqueNameMap{};

//===========================================================================//
/*<constants>*/
//===========================================================================//

constexpr float kExpandWidgetToRemainingSpace() { return -FLT_MIN; }
const CguiVec2 kExpandWidgetToRemainingSpaceXY = {
    kExpandWidgetToRemainingSpace(), kExpandWidgetToRemainingSpace()};

//===========================================================================//
/*<endconstants>*/
//===========================================================================//

//===========================================================================//
/* <defs> */
//===========================================================================//

//-------------------------------------------------------------------------//
/* <class:UIDGen> */
//-------------------------------------------------------------------------//
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

//-------------------------------------------------------------------------//
/* <impl:UIDGen> */
//-------------------------------------------------------------------------//
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
//-------------------------------------------------------------------------//
/* <endimpl:UIDGen> */
//-------------------------------------------------------------------------//

//-------------------------------------------------------------------------//
/* <class:UniqueNameMap> */
//-------------------------------------------------------------------------//
// Simple map which maintains unique names.
// TODO: improve using appended unique ids,
// in ImGui you can add "id" + "###tag" at the end
// to seperate objects with the same name.
// ## seems to also do something...search imgui src
//   You can use the "##" or "###" markers to use the same label with different
//   id, or same id with different label. See documentation at the top of this
//   file.
class UniqueNameMap {
 public:
  cxx::BoolError AddName(const std::string& str);
  cxx::BoolError RemoveName(const std::string& str);
  bool Contains(const std::string& str) { return names_.contains(str); }

 private:
  std::unordered_set<string> names_;
};

//-------------------------------------------------------------------------//
/* <impl:UniqueNameMap> */
//-------------------------------------------------------------------------//
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
//-------------------------------------------------------------------------//
/* <endimpl:UniqueNameMap> */
//-------------------------------------------------------------------------//

//-------------------------------------------------------------------------//
/* <class:ScopedWidgetBase> */
//-------------------------------------------------------------------------//
class ScopedWidgetBase {
 public:
  using BoundScopeBeginFuncT = std::function<bool()>;
  using BoundScopeEndFuncT = std::function<void()>;

  // Returns true is the Begin() function has been called
  // further gui commands will add to this scope.
  constexpr inline bool IsScopeActive() const { return is_scope_active_; }

  // The meaning of is_on_ may vary based on the ImGui widget call.
  // Usually indicates if this object was rendered by ImGui.
  // Some Object will always render, while the result represents a state.
  // These objects must be ended with ForceEndImpl().
  // Ex. Windows will return false for this parameter if they are minimized,
  // but the top bar will still be rendered. So the window is NOT displayed
  // but it IS rendered.
  constexpr inline bool IsOn() const { return is_on_; }

  // Implementations of the BeginLate and EndEarly methods.
  // Begin and End implementations are ment for the constructor
  //  and destructor respectivley.
  // ForceEnd is for widgets which must call End()
  //  even if Begin() returns false.
 protected:
  void BindBegin(const BoundScopeBeginFuncT& bound_begin) {
    begin_func = bound_begin;
  }
  void BindEnd(const BoundScopeEndFuncT& bound_end) { end_func = bound_end; }
  bool BeginImpl() {
    if (is_delayed_ == true) {
      is_scope_active_ = true;
      is_on_ = begin_func();
    } else {
      is_scope_active_ = false;
      is_on_ = false;
    }
    return is_on_;
  }
  bool BeginLateImpl() {
    if (not is_scope_active_) {
      is_scope_active_ = true;
      is_on_ = begin_func();
    }
    return is_on_;
  }
  void EndImpl() {
    if (is_scope_active_ && is_on_) end_func();
  }
  void EndEarlyImpl() {
    if (not is_scope_active_) throw "[ EndEarly() called before begin. ]";
    is_scope_active_ = false;
    if (is_on_) end_func();
  };
  void ForceEndImpl() {
    if (is_scope_active_) end_func();
  }
  void ForceEndEarlyImpl() {
    if (not is_scope_active_) throw "[ EndEarly() called before begin. ]";
    is_scope_active_ = false;
    end_func();
  };

  // Public interface to be implemented by the child class.
 public:
  virtual bool BeginLate() = 0;
  virtual void EndEarly() = 0;
  virtual ~ScopedWidgetBase() = default;

 public:
  // Default object is a null widget,Begin() will always return false.
  ScopedWidgetBase(bool is_delayed)
      : name_map_(gCguiDefaultUniqueNameMap),
        id_gen_(gCguiDefaultUIDGenerator),
        is_delayed_(is_delayed) {
    is_on_ = false;
    is_scope_active_ = false;
    begin_func = []() { return false; };
    end_func = []() {};
  };

  // For simple widget which can be bound without other internal dependencies.
  // BeginImpl() must still be called by the child class.
  ScopedWidgetBase(bool is_delayed, const BoundScopeBeginFuncT& bound_begin,
                   const BoundScopeEndFuncT& bound_end)
      : name_map_(gCguiDefaultUniqueNameMap),
        id_gen_(gCguiDefaultUIDGenerator),
        is_delayed_(is_delayed) {
    is_on_ = false;
    is_scope_active_ = false;
    begin_func = bound_begin;
    end_func = bound_end;
  };

  // Move is allowed
  ScopedWidgetBase(ScopedWidgetBase&& other) noexcept
      : name_map_(other.name_map_), id_gen_(other.id_gen_) {
    is_on_ = other.is_on_;
    is_scope_active_ = other.is_scope_active_;
    is_delayed_ = other.is_delayed_;
    begin_func = other.begin_func;
    end_func = other.end_func;
  }

  // Copy is implicitly forbidden.
  ScopedWidgetBase(const ScopedWidgetBase& other) = delete;

  // Boolean conversion, returns IsOn()
  operator bool() { return is_on_; }

 private:
  UniqueNameMap& name_map_;
  UIDGen& id_gen_;
  bool is_scope_active_;
  bool is_on_;
  bool is_delayed_;
  BoundScopeBeginFuncT begin_func;
  BoundScopeEndFuncT end_func;

  // Helper methods to keep the above members private,
  // They are to be used by the child class implementation.
 protected:
  // Name must not exist.
  cxx::BoolError RequestNewName(const std::string& str) {
    if (name_map_.Contains(str))
      return "[WidgetBase:NewName:This widget name is already in use.]";
    name_map_.AddName(str);
    return true;
  };
  void ReleaseName(const std::string& str) {
    // Remove the name from name map.
    name_map_.RemoveName(str);
  }
  // Generate a new uuid
  UIDGen::Iter RequestId() { return id_gen_.GetId(); }
  void ReleaseId(UIDGen::Iter id_iter) {
    // Remove the id
    id_gen_.EraseId(id_iter);
  }
};

//-------------------------------------------------------------------------//
/* <class:SingularWidgetBase> */
//-------------------------------------------------------------------------//
class SingularWidgetBase {
 public:
  using BoundBeginFuncT = std::function<bool()>;

 protected:
  void BindBegin(const BoundBeginFuncT& bound_begin) {
    begin_func = bound_begin;
  }
  bool BeginImpl() {
    if (is_delayed_ == true) {
      is_scope_active_ = true;
      is_on_ = begin_func();
    } else {
      is_scope_active_ = false;
      is_on_ = false;
    }
    return is_on_;
  }
  bool BeginLateImpl() {
    if (not is_scope_active_) {
      is_scope_active_ = true;
      is_on_ = begin_func();
    }
    return is_on_;
  }

 public:
  SingularWidgetBase(bool is_delayed)
      : is_delayed_(is_delayed), is_scope_active_(false), is_on_(false) {}
  virtual bool BeginLate() = 0;
  virtual ~SingularWidgetBase() = default;

  // Result of requesting to render this widget.
  // Meaning varies from widget to widget.
  // Indicated a pressed/released/displayed state.
  bool IsOn() const { return is_on_; }

  // Has this widget's scope been initiated?
  // If true -> not necessarily rendered, but a request to render has
  // been sent.
  bool IsScopeActive() const { return is_scope_active_; }

  // Boolean conversion, returns IsOn()
  operator bool() { return is_on_; }

 private:
  bool is_scope_active_;  // Has this widget's scope been initiated?
  bool is_on_;            // Result of requesting to render this widget.
  bool is_delayed_;
  BoundBeginFuncT begin_func;
};

//===========================================================================//
namespace scoped_widget { /* cgui scoped_widget */
//===========================================================================//

//-------------------------------------------------------------------------//
/* <class:Window> */
//-------------------------------------------------------------------------//
class Window : public ScopedWidgetBase {
 public:
  Window(const std::string& title, bool has_close_button = false,
         WindowFlags flags = WindowFlags(), bool delay_begin = true)
      : ScopedWidgetBase(delay_begin) {
    flags_ = flags;
    has_close_button_ = has_close_button;

    // Make sure the name is unique. Add it to the name map.
    cxx::BoolError valid_name = RequestNewName(title);
    if (valid_name) {
      title_ = title;
    } else
      throw valid_name.Exception();

    // Bind the begin method.
    BindBegin([this]() -> bool {
      if (has_close_button_) {
        close_button_state_ = std::make_unique<bool>(true);
      }
      return ImGui::Begin(title_.c_str(), close_button_state_.get(),
                          flags_.Get());
    });

    // Bind the end method
    BindEnd(ImGui::End);

    // Begin Scope if not delayed.
    BeginImpl();
  }

  bool BeginLate() override { return BeginLateImpl(); }

  void EndEarly() override { ForceEndEarlyImpl(); }

  ~Window() {
    ForceEndImpl();
    // Remove the name from name map.
    ReleaseName(title_);
  }

  // Properties
 public:
  const std::string& Title() const { return title_; }

  // !! Warning: Returns a mutable reference.
  WindowFlags& Flags() { return flags_; }

  // Returns TRUE if the close button of the window was triggered this frame.
  // From ImGui documentation as to why the implementation is inversed...:
  // Passing 'bool* p_open' displays a Close button on the upper-right corner
  // of the window, the pointed value will be set to false when the button is
  // pressed.
  bool IsCloseButtonTriggered() const { return not *close_button_state_; }

 private:
  std::string title_{""};
  WindowFlags flags_{WindowFlags()};
  bool has_close_button_;
  std::unique_ptr<bool> close_button_state_{nullptr};
};

//-------------------------------------------------------------------------//
/* <class:Subcontext> */
//-------------------------------------------------------------------------//
class Subcontext : public ScopedWidgetBase {
  UIDGen::Iter uid_;
  WindowFlags win_flags_;
  SubcontextFlags subcontext_flags_;
  CguiVec2 requested_size_;

 public:
  std::size_t Id() const { return *uid_; }
  // !! Warning: mutable reference
  WindowFlags GetWindowFlags() const { return win_flags_; }
  // !! Warning: mutable reference
  SubcontextFlags GetSubcontextFlags() const { return subcontext_flags_; }
  const CguiVec2& RequestedSize() { return requested_size_; }

  Subcontext(CguiVec2 size = {0.f, 0.f}, WindowFlags win_flags = WindowFlags(),
             SubcontextFlags subcontext_flags = SubcontextFlags(),
             bool delay_begin = false)
      : ScopedWidgetBase(delay_begin) {
    uid_ = RequestId();
    win_flags_ = win_flags;
    subcontext_flags_ = subcontext_flags;
    requested_size_ = size;

    // Bind the begin method.
    BindBegin([this]() -> bool {
      return ImGui::BeginChild(*uid_,
                               {requested_size_.first, requested_size_.second},
                               subcontext_flags_, win_flags_);
    });

    // Bind the end method
    BindEnd(ImGui::EndChild);

    // Begin Scope if not delayed.
    BeginImpl();
  }
  bool BeginLate() override { return BeginLateImpl(); }

  void EndEarly() override { EndEarlyImpl(); }

  ~Subcontext() {
    EndImpl();
    // Remove the name from name map.
    ReleaseId(uid_);
  }
};

//-------------------------------------------------------------------------//
/* <class:NamedSubcontext> */
//-------------------------------------------------------------------------//
class NamedSubcontext : public ScopedWidgetBase {
  std::string name_;
  WindowFlags win_flags_;
  SubcontextFlags subcontext_flags_;
  CguiVec2 requested_size_;

 public:
  const std::string& Name() const { return name_; }
  // !! Warning: mutable reference
  WindowFlags GetWindowFlags() const { return win_flags_; }
  // !! Warning: mutable reference
  SubcontextFlags GetSubcontextFlags() const { return subcontext_flags_; }
  const CguiVec2& RequestedSize() { return requested_size_; }

  NamedSubcontext(const std::string& name, CguiVec2 size = {0.f, 0.f},
                  WindowFlags win_flags = WindowFlags(),
                  SubcontextFlags subcontext_flags = SubcontextFlags(),
                  bool delay_begin = false)
      : ScopedWidgetBase(delay_begin) {
    win_flags_ = win_flags;
    subcontext_flags_ = subcontext_flags;
    requested_size_ = size;

    // Make sure the name is unique. Add it to the name map.
    cxx::BoolError valid_name = RequestNewName(name);
    if (valid_name) {
      name_ = name;
    } else
      throw valid_name.Exception();

    // Bind the begin method.
    BindBegin([this]() -> bool {
      return ImGui::BeginChild(name_.c_str(),
                               {requested_size_.first, requested_size_.second},
                               subcontext_flags_, win_flags_);
    });

    // Bind the end method
    BindEnd(ImGui::EndChild);

    // Begin Scope if not delayed.
    BeginImpl();
  }

  bool BeginLate() override { return BeginLateImpl(); }

  void EndEarly() override { EndEarlyImpl(); }

  ~NamedSubcontext() {
    EndImpl();
    // Remove the name from name map.
    ReleaseName(name_);
  }
};

//-------------------------------------------------------------------------//
/* <class:MenuBar> */
//-------------------------------------------------------------------------//
class MenuBar : public ScopedWidgetBase {
 public:
  MenuBar(bool delay_begin = false) : ScopedWidgetBase(delay_begin) {
    // Bind the begin method.
    BindBegin(ImGui::BeginMainMenuBar);

    // Bind the end method
    BindEnd(ImGui::EndMainMenuBar);

    // Begin Scope if not delayed.
    BeginImpl();
  }

  bool BeginLate() override { return BeginLateImpl(); }

  void EndEarly() override { EndEarlyImpl(); }

  ~MenuBar() { EndImpl(); }
};

//-------------------------------------------------------------------------//
/* <class:Menu> */
//-------------------------------------------------------------------------//
class Menu : public ScopedWidgetBase {
  std::string title_;
  bool is_enabled_;

 public:
  const std::string& Title() const { return title_; }
  bool IsEnabled() const { return is_enabled_; }

  Menu(const std::string& title, bool is_enabled = true,
       bool delay_begin = false)
      : ScopedWidgetBase(delay_begin) {
    title_ = title;
    is_enabled_ = is_enabled;

    // Make sure the name is unique. Add it to the name map.
    cxx::BoolError valid_name = RequestNewName(title);
    if (valid_name) {
      title_ = title;
    } else
      throw valid_name.Exception();

    // Bind the begin method.
    BindBegin([this]() -> bool {
      return ImGui::BeginMenu(title_.c_str(), is_enabled_);
    });

    // Bind the end method
    BindEnd(ImGui::EndMenu);

    // Begin Scope if not delayed.
    BeginImpl();
  }

  bool BeginLate() override { return BeginLateImpl(); }

  void EndEarly() override { EndEarlyImpl(); }

  ~Menu() {
    EndImpl();
    // Remove the name from name map.
    ReleaseName(title_);
  }
};

//-------------------------------------------------------------------------//
/* <class:TabBar> */
//-------------------------------------------------------------------------//
class TabBar : public ScopedWidgetBase {
  std::string name_;
  TabBarFlags flags_;

 public:
  const std::string& Name() const { return name_; }
  TabBarFlags GetFlags() const { return flags_; }

  TabBar(const std::string& name, TabBarFlags flags = TabBarFlags(),
         bool delay_begin = false)
      : ScopedWidgetBase(delay_begin) {
    name_ = name;
    flags_ = flags;

    // Make sure the name is unique. Add it to the name map.
    cxx::BoolError valid_name = RequestNewName(name);
    if (valid_name) {
      name_ = name;
    } else
      throw valid_name.Exception();

    // Bind the begin method.
    BindBegin([this]() -> bool {
      return ImGui::BeginTabBar(name_.c_str(), flags_.Get());
    });

    // Bind the end method
    BindEnd(ImGui::EndTabBar);

    // Begin Scope if not delayed.
    BeginImpl();
  }

  bool BeginLate() override { return BeginLateImpl(); }

  void EndEarly() override { EndEarlyImpl(); }

  ~TabBar() {
    EndImpl();
    // Remove the name from name map.
    ReleaseName(name_);
  }
};

//-------------------------------------------------------------------------//
/* <class:TabItem> */
//-------------------------------------------------------------------------//
class TabItem : public ScopedWidgetBase {
  std::string name_;
  TabItemFlags flags_;
  std::unique_ptr<bool> is_selected_;

 public:
  const std::string& Name() const { return name_; }
  TabItemFlags GetFlags() const { return flags_; }
  bool IsSelected() const { return *is_selected_; }

  TabItem(const std::string& title, TabItemFlags flags = TabItemFlags(),
          bool delay_begin = false)
      : ScopedWidgetBase(delay_begin) {
    name_ = title;
    flags_ = flags;
    is_selected_ = std::make_unique<bool>(false);

    // Make sure the name is unique. Add it to the name map.
    cxx::BoolError valid_name = RequestNewName(title);
    if (valid_name) {
      name_ = title;
    } else
      throw valid_name.Exception();

    // Bind the begin method.
    BindBegin([this]() -> bool {
      return ImGui::BeginTabItem(name_.c_str(), is_selected_.get(),
                                 flags_.Get());
    });

    // Bind the end method
    BindEnd(ImGui::EndTabItem);

    // Begin Scope if not delayed.
    BeginImpl();
  }

  bool BeginLate() override { return BeginLateImpl(); }

  void EndEarly() override { EndEarlyImpl(); }

  ~TabItem() {
    EndImpl();
    // Remove the name from name map.
    ReleaseName(name_);
  }
};

//-------------------------------------------------------------------------//
/* <class:TreeNode> */
//-------------------------------------------------------------------------//
class TreeNode : public ScopedWidgetBase {
  std::string name_;

 public:
  const std::string& Name() const { return name_; }
  TreeNode(const std::string& name, bool delay_begin = false)
      : ScopedWidgetBase(delay_begin) {
    name_ = name;

    // Make sure the name is unique. Add it to the name map.
    cxx::BoolError valid_name = RequestNewName(name);
    if (valid_name) {
      name_ = name;
    } else
      throw valid_name.Exception();

    // Bind the begin method.
    BindBegin([this]() -> bool { return ImGui::TreeNode(name_.c_str()); });

    // Bind the end method
    BindEnd(ImGui::TreePop);

    // Begin Scope if not delayed.
    BeginImpl();
  }

  bool BeginLate() override { return BeginLateImpl(); }

  void EndEarly() override { EndEarlyImpl(); }

  ~TreeNode() {
    EndImpl();
    // Remove the name from name map.
    ReleaseName(name_);
  }
};

//===========================================================================//
};  // end namespace scoped_widget
//===========================================================================//

//===========================================================================//
namespace single_widget { /* cgui single_widget */
//===========================================================================//

//-------------------------------------------------------------------------//
/* <class:Button> */
//-------------------------------------------------------------------------//
class Button : public SingularWidgetBase {
  std::string text_;
  CguiVec2 size_;

 public:
  Button(const std::string& text, CguiVec2 size = {},
         bool delayed_begin = false)
      : SingularWidgetBase(delayed_begin) {
    text_ = text;
    size_ = size;

    // Bind the begin method.
    BindBegin([this]() -> bool {
      return ImGui::Button(text_.c_str(), {size_.first, size_.second});
    });

    // Begin Scope if not delayed.
    BeginImpl();
  }

  bool BeginLate() override { return BeginLateImpl(); }

  ~Button() = default;
};

//-------------------------------------------------------------------------//
/* <class:MenuItem> */
//-------------------------------------------------------------------------//
class MenuItem : public SingularWidgetBase {
  std::string text_;
  std::string shortcut_hint_;
  bool is_enabled_;

 public:
  MenuItem(const std::string& text, const std::string& shortcut_hint = "",
           bool is_enabled = true, bool delayed_begin = false)
      : SingularWidgetBase(delayed_begin) {
    text_ = text;
    shortcut_hint_ = shortcut_hint;
    is_enabled_ = is_enabled;

    // Bind the begin method.
    BindBegin([this]() -> bool {
      return ImGui::MenuItem(text_.c_str(), shortcut_hint_.c_str(), false,
                             is_enabled_);
    });

    // Begin Scope if not delayed.
    BeginImpl();
  }

  bool BeginLate() override { return BeginLateImpl(); }

  ~MenuItem() = default;
};

//-------------------------------------------------------------------------//
/* <class:Selectable> */
//-------------------------------------------------------------------------//
class Selectable : public SingularWidgetBase {
  std::string text_;

 public:
  Selectable(const std::string& text, bool delayed_begin = false)
      : SingularWidgetBase(delayed_begin) {
    text_ = text;

    // Bind the begin method.
    BindBegin([this]() -> bool { return ImGui::Selectable(text_.c_str()); });

    // Begin Scope if not delayed.
    BeginImpl();
  }

  bool BeginLate() override { return BeginLateImpl(); }

  ~Selectable() = default;
};

//-------------------------------------------------------------------------//
/* <class:MultilineTextInput> */
//-------------------------------------------------------------------------//
class MultilineTextInput : public SingularWidgetBase {
  std::string label_;
  const CguiVec2& size_;
  std::string* buffer_;
  InputTextFlags flags_;

 public:
  MultilineTextInput(const std::string& label, std::string* buffer,
                     const CguiVec2& size = {},
                     InputTextFlags flags = InputTextFlags(),
                     bool delayed_begin = false)
      : SingularWidgetBase(delayed_begin),
        label_(label),
        size_(size),
        buffer_(buffer),
        flags_(flags) {
    // Bind the begin method.
    BindBegin([this]() -> bool {
      return ImGui::InputTextMultiline(
          label_.c_str(), buffer_, {size_.first, size_.second}, flags_.Get());
    });

    // Begin Scope if not delayed.
    BeginImpl();
  }

  bool BeginLate() override { return BeginLateImpl(); }

  ~MultilineTextInput() = default;
};

//===========================================================================//
};  // namespace single_widget
//===========================================================================//

//===========================================================================//
namespace combo_widget {
//===========================================================================//

//-----------------------------------------------------------------------//
/*<class:DirectoryView>*/
//-----------------------------------------------------------------------//
class DirectoryView : public SingularWidgetBase {
 public:
  using PathT = typename std::filesystem::path;
  using SelectedCallbackT = std::function<void(const PathT&)>;
 public:
  DirectoryView(
      const PathT& path,
      SelectedCallbackT selected_callback = [](auto&& a) {},
      bool is_delayed = false)
      : SingularWidgetBase(is_delayed),
        select_file_callback(selected_callback),
        root(path) {
    // Bind the begin method.
    BindBegin([this]() -> bool {
      RecursiveDisplayDirectory(root);
      return true;
    });

    // Begin Scope if not delayed.
    BeginImpl();
  }
  DirectoryView(
      const PathT& path,
      SelectedCallbackT selected_callback = [](auto&& a) {},
      SelectedCallbackT right_click_callback = [](auto&& a) {},
      bool is_delayed = false)
      : SingularWidgetBase(is_delayed),
        select_file_callback(selected_callback),
        right_click_file_callback(right_click_callback),
        root(path) {
    // Bind the begin method.
    BindBegin([this]() -> bool {
      RecursiveDisplayDirectory(root);
      return true;
    });

    // Begin Scope if not delayed.
    BeginImpl();
  }
  bool BeginLate() { return BeginLateImpl(); }
 private:
  SelectedCallbackT select_file_callback;
  SelectedCallbackT right_click_file_callback;
  const PathT& root;

  void RecursiveDisplayDirectory(const PathT& path, int depth = 0) {
    if (std::filesystem::is_directory(path)) {
      bool node_open = ImGui::TreeNode(path.filename().string().c_str());
      if (node_open) {
        for (const auto& entry : std::filesystem::directory_iterator(path)) {
          RecursiveDisplayDirectory(entry.path(), depth + 1);
        }
        ImGui::TreePop();
      }
    } else if (std::filesystem::is_regular_file(path)) {
      // Select (left click) on a leaf callback.
      if (single_widget::Selectable(path.filename().string())) {
        select_file_callback(path);
      }
      // Right click on a leaf callback.
      if (ImGui::BeginPopupContextItem(
              (std::string("dir-file-right-click-context###") + path.string())
                  .c_str())) {
        right_click_file_callback(path);
        ImGui::EndPopup();
      }
    }
  }
};

//===========================================================================//
}; /* end namespace combo_widget */
//===========================================================================//

//===========================================================================//
}; /* end namespace cgui */
//===========================================================================//

//===========================================================================//
/*<library interface>*/
//===========================================================================//

// Forwarded methods from ImGui namespace.
namespace cgui {
using ImGui::SameLine;
using ImGui::Separator;
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

//===========================================================================//
/*<end library interface>*/
//===========================================================================//

// Some examples of how to use the library.
namespace cgui::example {
void ExampleHelloWindow() {
  CguiWindow my_window{"My Window"};

  // Do things based on if the window is open or not.
  // Use my_window.IsOn() to be explicit.
  if (my_window)                                 // Use implict bool conversion.
    std::cout << "Hello World from my window!";  // Draw other elements inside
                                                 // window etc.

  // Widget will automaticall call end when going out of scope.
  // or we can call end early.
  my_window.EndEarly();

  // This is now outside the window's scope.
  my_window.IsScopeActive();  // to query scope state.

  // restart the same window's scope if it still in the overall C++ scope.
  if (my_window.BeginLate()) {
    // ImGui standard syntax for BeginLate/EndEarly.
    // But calling End is not necessary if obj will go out of scope.
    std::cout << "Hello again from my window!";
  };
  // add more stuff.....

  // window will end itself here.
}
void ExampleMenuBar() {  // creating a menu bar
  {
    auto file_menu = CguiMenu("File");
    if (file_menu) {
      auto new_submenu = CguiMenu("New");
      if (new_submenu) {
        auto solution = CguiMenuItem("Solution");
      }
    }
  }
}
void ExampleWindowWithSubcontext() {
  // A window with two subcontexts.
  if (true) {
    // This is to demonstrate windows that go out of scope
    // automatically.
    auto new_window = CguiWindow("CoolWindow!");
    auto new_named_subcontext = CguiNamedSubcontext("HelloContext");
    auto my_button = CguiButton(new_named_subcontext.Name() +
                                "'s Button inside" + new_window.Title());

    // End subcontext early to begin new one within the same window context.
    new_named_subcontext.EndEarly();

    // Can query button properties.It hasnt gone out of scope yet!
    static bool draw_subcontext_switch =
        false;  // Note this switch has to be static to
                // persist throught the frames.
    if (my_button) {
      draw_subcontext_switch = not draw_subcontext_switch;
    }

    if (draw_subcontext_switch) {
      auto next_unnamed_subcontext = CguiSubcontext();
      auto abtn = CguiButton(std::to_string(next_unnamed_subcontext.Id()) +
                             "# Unnamed Subcontext's Btn ");
    }
    // automatically goes out of scope.
  }
}
void ExampleTabBar() {
  // A window with a tab bar.
  if (true) {
    auto new_window = CguiWindow("TabBarWindow");
    auto new_tab_bar = CguiTabBar("TabBar");
    if (new_tab_bar) {
      auto tab1 = CguiTabItem("Tab1");
      if (tab1) {
        auto btn1 = CguiButton("Button1");
      }
      auto tab2 = CguiTabItem("Tab2");
      if (tab2) {
        auto btn2 = CguiButton("Button2");
      }
    }
  }
}
void ExampleEditorTabs(sf::Window& window) {
  // A file editor with tabs
  // Demonstrates use of cgui::kExpandWidgetToRemainingSpaceXY to stretch
  // widgets. Function is in the context of immediate mode eg. called once per
  // render frame.
  std::string gEditorStringBuffer{""};
  auto editor_context = CguiNamedSubcontext(
      "Editor", {static_cast<float>(window.getSize().x * 0.75f),
                 static_cast<float>(window.getSize().y * 0.75f)});
  if (editor_context) {
    auto editor_btn = CguiButton("Editor");
    auto editor_tab_bar = CguiTabBar(
        "##file-tabs", {CguiTabBarFlagEnum::ImGuiTabBarFlags_Reorderable});
    if (editor_tab_bar) {
      auto selected_tab = CguiTabItem("[Selected]");
      if (selected_tab)
        auto file_text_box =
            CguiMultilineTextInput("Selected_Code", &gEditorStringBuffer,
                                   cgui::kExpandWidgetToRemainingSpaceXY);
      selected_tab.EndEarly();
      auto other_tab = CguiTabItem("[Other]");
      if (other_tab)
        auto file_text_box =
            CguiMultilineTextInput("Other_Code", &gEditorStringBuffer,
                                   cgui::kExpandWidgetToRemainingSpaceXY);
    }
  }
}
};  // namespace cgui::example
