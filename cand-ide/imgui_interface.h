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

enum eWidgetInit {
  kWidgetInitDelayed = true,
  kWidgetInitImmediate = false,
};

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
  size_t next_id_;
  std::unordered_set<size_t> generated_ids_;
};

//-------------------------------------------------------------------------//
/* <impl:UIDGen> */
//-------------------------------------------------------------------------//
// Do a try catch block around this method if you wish to refresh on overflow.
UIDGen::Iter UIDGen::GetId() {
  if (next_id_ == SIZE_MAX) throw std::overflow_error("UIDGen: ID overflow");
  size_t id = next_id_;
  next_id_++;
  if (generated_ids_.contains(id))
    throw std::runtime_error("UIDGen: ID exists");
  generated_ids_.insert(id);
  return generated_ids_.find(id);
}

void UIDGen::PopId() {
  if (generated_ids_.empty()) throw std::runtime_error("UIDGen: No IDs to pop");
  generated_ids_.erase(next_id_);
  next_id_--;
}

// Erases an id but does not reset counter. If generating many id and deleting
// use Referesh() to reset the counter and set.
void UIDGen::EraseId(Iter it) {
  if (not generated_ids_.empty() and (it == std::prev(generated_ids_.end()))) {
    // If the id we are erasing is the last id, we can also pop to save make
    // more uids
    PopId();
  } else {
    if (generated_ids_.contains(*it)) generated_ids_.erase(it);
  }
}

const std::unordered_set<size_t>& UIDGen::Generated() { return generated_ids_; }

// Pops all ids and sets next_id_ = 0;
void UIDGen::Refresh() {
  generated_ids_.clear();
  next_id_ = 1;
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
  void RemoveName(const std::string& str);
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

void UniqueNameMap::RemoveName(const std::string& str) {
  if (!names_.contains(str)) return;  // Do nothing if the name is not found.
  names_.erase(str);
}
//-------------------------------------------------------------------------//
/* <endimpl:UniqueNameMap> */
//-------------------------------------------------------------------------//

// Implicit global unique id generator for cgui widget classes.
// !! Do not use/access directly, called by ScopedWidgetBase on default
// construction.
static UIDGen gCguiDefaultUIDGenerator{};
// Implicit global unique name map for cgui widget classes.
// !! Do not use/access directly, called by ScopedWidgetBase on default
// construction.
static UniqueNameMap gCguiDefaultUniqueNameMap{};

//-------------------------------------------------------------------------//
/* <class:ScopedWidgetBase> */
//-------------------------------------------------------------------------//
class ScopedWidgetBase {
 public:
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
  // void BindBegin(const BoundScopeBeginFuncT& bound_begin) {
  //   begin_func = bound_begin;
  // }
  // void BindEnd(const BoundScopeEndFuncT& bound_end) { end_func = bound_end; }
  bool BeginImpl() {
    if (is_delayed_ == kWidgetInitImmediate) {
      is_scope_active_ = true;
      is_on_ = BoundBegin();
    } else {
      is_scope_active_ = false;
      is_on_ = false;
    }
    return is_on_;
  }
  bool BeginLateImpl() {
    if (not is_scope_active_) {
      is_scope_active_ = true;
      is_on_ = BoundBegin();
    }
    return is_on_;
  }

  void EndImpl() {
    if (is_scope_active_ && is_on_) BoundEnd();
  }
  void EndEarlyImpl() {
    if (not is_scope_active_) throw "[ EndEarly() called before begin. ]";
    is_scope_active_ = false;
    if (is_on_) BoundEnd();
  };
  void ForceEndImpl() {
    if (is_scope_active_) BoundEnd();
  }
  void ForceEndEarlyImpl() {
    if (not is_scope_active_) throw "[ EndEarly() called before begin. ]";
    is_scope_active_ = false;
    BoundEnd();
  };

  // Public interface to be implemented by the child class.
 public:
  virtual bool BoundBegin() = 0;
  virtual void BoundEnd() = 0;
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
  };

  // Copy is allowed, but the name_map and id_gen are shared.
  // The uniqueness of the name or id is only checked on construction.
  ScopedWidgetBase(const ScopedWidgetBase& other) = default;

  // Move is allowed.
  ScopedWidgetBase(ScopedWidgetBase&& other) noexcept = default;

  // Boolean conversion, returns IsOn()
  operator bool() { return is_on_; }

 protected:
  UniqueNameMap& name_map_;
  UIDGen& id_gen_;
  bool is_scope_active_;
  bool is_on_;
  bool is_delayed_;

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
  // Result of requesting to render this widget.
  // Meaning varies from widget to widget.
  // Indicated a pressed/released/displayed state.
  bool IsOn() const { return is_on_; }

  // True if the widget is currently delayed
  // eg. the widget has not been rendered yet.
  bool IsDelayed() const { return is_delayed_; }

  // Boolean conversion, returns IsOn()
  operator bool() { return is_on_; }

 protected:
  bool BeginImpl() {
    if (is_delayed_ == kWidgetInitImmediate) {
      is_on_ = BoundBegin();
    } else {
      is_on_ = false;
    }
    return is_on_;
  }
  bool BeginLateImpl() {
    is_on_ = BoundBegin();
    return is_on_;
  }

 public:
  SingularWidgetBase(bool is_delayed)
      : is_delayed_(is_delayed), is_on_(false) {}
  virtual bool BeginLate() = 0;
  virtual ~SingularWidgetBase() = default;
  virtual bool BoundBegin() = 0;

 protected:
  bool is_on_;  // Result of requesting to render this widget.
  bool is_delayed_;
};

//===========================================================================//
namespace scoped_widget { /* cgui scoped_widget */
//===========================================================================//

//-------------------------------------------------------------------------//
/* <class:Window> */
//-------------------------------------------------------------------------//
class Window : public ScopedWidgetBase {
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

  const CguiVec2& QuerySize() const { return size_; }
  inline float QueryWidth() const { return size_.first; }
  inline float QueryHeight() const { return size_.second; }

 public:
  static inline Window Delayed(const std::string& title,
                               bool has_close_button = false,
                               WindowFlags flags = WindowFlags()) {
    return Window(title, has_close_button, flags, kWidgetInitDelayed);
  }

  Window(const std::string& title, bool has_close_button = false,
         WindowFlags flags = WindowFlags(),
         bool delay_begin = kWidgetInitImmediate)
      : ScopedWidgetBase(delay_begin) {
    flags_ = flags;
    has_close_button_ = has_close_button;

    // Make sure the name is unique. Add it to the name map.
    cxx::BoolError valid_name = RequestNewName(title);
    if (valid_name) {
      title_ = title;
    } else
      throw valid_name.Exception();

    BeginImpl();
    if (IsScopeActive()) {
      size_.first = ImGui::GetWindowWidth();
      size_.second = ImGui::GetWindowHeight();
    }
  }

  bool BeginLate() override {
    bool on = BeginLateImpl();
    if (IsScopeActive()) {
      size_.first = ImGui::GetWindowWidth();
      size_.second = ImGui::GetWindowHeight();
    }
    return on;
  }

  void EndEarly() override { ForceEndEarlyImpl(); }

  ~Window() {
    ForceEndImpl();
    // Remove the name from name map.
    ReleaseName(title_);
  }

  Window(const Window& other) : ScopedWidgetBase(other.is_delayed_) {
    this->is_scope_active_ = other.is_scope_active_;
    this->is_on_ = other.is_on_;
    this->title_ = other.title_;
    this->flags_ = other.flags_;
    this->has_close_button_ = other.has_close_button_;
    this->close_button_state_ =
        std::make_unique<bool>(*other.close_button_state_);
  }

  Window(Window&& other) noexcept
      : ScopedWidgetBase(std::move(other.is_delayed_)) {
    this->is_scope_active_ = std::move(other.is_scope_active_);
    this->is_on_ = std::move(other.is_on_);
    this->title_ = std::move(other.title_);
    this->flags_ = std::move(other.flags_);
    this->has_close_button_ = std::move(other.has_close_button_);
    this->close_button_state_ = std::move(other.close_button_state_);
  }

 protected:
  bool BoundBegin() override {
    if (has_close_button_) {
      close_button_state_ = std::make_unique<bool>(true);
    }
    return ImGui::Begin(title_.c_str(), close_button_state_.get(),
                        flags_.Get());
  }

  void BoundEnd() override { ImGui::End(); }

 private:
  std::string title_{""};
  WindowFlags flags_{WindowFlags()};
  bool has_close_button_;
  std::unique_ptr<bool> close_button_state_{nullptr};
  CguiVec2 size_{0.f, 0.f};
};

//-------------------------------------------------------------------------//
/* <class:Subcontext> */
//-------------------------------------------------------------------------//
class Subcontext : public ScopedWidgetBase {
 public:
  std::size_t Id() const { return *uid_; }

  // !! Warning: mutable reference
  WindowFlags GetWindowFlags() const { return win_flags_; }

  // !! Warning: mutable reference
  SubcontextFlags GetSubcontextFlags() const { return subcontext_flags_; }

  const CguiVec2& RequestedSize() { return requested_size_; }

 public:
  static inline Subcontext Delayed(
      CguiVec2 size = {0.f, 0.f}, WindowFlags win_flags = WindowFlags(),
      SubcontextFlags subcontext_flags = SubcontextFlags()) {
    return Subcontext(size, win_flags, subcontext_flags, kWidgetInitDelayed);
  }

  Subcontext(CguiVec2 size = {0.f, 0.f}, WindowFlags win_flags = WindowFlags(),
             SubcontextFlags subcontext_flags = SubcontextFlags(),
             bool delay_begin = kWidgetInitImmediate)
      : ScopedWidgetBase(delay_begin) {
    uid_ = RequestId();
    win_flags_ = win_flags;
    subcontext_flags_ = subcontext_flags;
    requested_size_ = size;

    // Begin Scope if not delayed.
    BeginImpl();
  }
  bool BeginLate() override { return BeginLateImpl(); }

  void EndEarly() override { ForceEndEarlyImpl(); }

  ~Subcontext() {
    ForceEndImpl();
    // Remove the name from name map.
    ReleaseId(uid_);
  }

 protected:
  bool BoundBegin() override {
    return ImGui::BeginChild(*uid_,
                             {requested_size_.first, requested_size_.second},
                             subcontext_flags_, win_flags_);
  }

  void BoundEnd() override { ImGui::EndChild(); }

 private:
  UIDGen::Iter uid_;
  WindowFlags win_flags_;
  SubcontextFlags subcontext_flags_;
  CguiVec2 requested_size_;
};

//-------------------------------------------------------------------------//
/* <class:NamedSubcontext> */
//-------------------------------------------------------------------------//
class NamedSubcontext : public ScopedWidgetBase {
 public:
  const std::string& Name() const { return name_; }

  // !! Warning: mutable reference
  WindowFlags GetWindowFlags() const { return win_flags_; }

  // !! Warning: mutable reference
  SubcontextFlags GetSubcontextFlags() const { return subcontext_flags_; }

  const CguiVec2& RequestedSize() { return requested_size_; }
  void RequestSize(const CguiVec2& size) { requested_size_ = size; }

 public:
  static inline NamedSubcontext Delayed(
      const std::string& name, CguiVec2 size = {0.f, 0.f},
      WindowFlags win_flags = WindowFlags(),
      SubcontextFlags subcontext_flags = SubcontextFlags()) {
    return NamedSubcontext(name, size, win_flags, subcontext_flags,
                           kWidgetInitDelayed);
  }

  NamedSubcontext(const std::string& name, CguiVec2 size = {0.f, 0.f},
                  WindowFlags win_flags = WindowFlags(),
                  SubcontextFlags subcontext_flags = SubcontextFlags(),
                  bool delay_begin = kWidgetInitImmediate)
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

    // Begin Scope if not delayed.
    BeginImpl();
  }

  bool BeginLate() override { return BeginLateImpl(); }

  void EndEarly() override { ForceEndEarlyImpl(); }

  ~NamedSubcontext() {
    ForceEndImpl();
    // Remove the name from name map.
    ReleaseName(name_);
  }

 protected:
  bool BoundBegin() override {
    return ImGui::BeginChild(name_.c_str(),
                             {requested_size_.first, requested_size_.second},
                             subcontext_flags_, win_flags_);
  }

  void BoundEnd() override { ImGui::EndChild(); }

 private:
  std::string name_;
  WindowFlags win_flags_;
  SubcontextFlags subcontext_flags_;
  CguiVec2 requested_size_;
};

//-------------------------------------------------------------------------//
/* <class:MenuBar> */
//-------------------------------------------------------------------------//
class MenuBar : public ScopedWidgetBase {
 public:
  static inline MenuBar Delayed() { return MenuBar(kWidgetInitDelayed); }

  MenuBar(bool delay_begin = kWidgetInitImmediate)
      : ScopedWidgetBase(delay_begin) {
    // Begin Scope if not delayed.
    BeginImpl();
  }

  bool BeginLate() override { return BeginLateImpl(); }

  void EndEarly() override { EndEarlyImpl(); }

  ~MenuBar() { EndImpl(); }

 protected:
  bool BoundBegin() override { return ImGui::BeginMenuBar(); }

  void BoundEnd() override { ImGui::EndMenuBar(); }
};

//-------------------------------------------------------------------------//
/* <class:Menu> */
//-------------------------------------------------------------------------//
class Menu : public ScopedWidgetBase {
 public:
  const std::string& Title() const { return title_; }
  bool IsEnabled() const { return is_enabled_; }

 public:
  static inline Menu Delayed(const std::string& title, bool is_enabled = true) {
    return Menu(title, is_enabled, kWidgetInitDelayed);
  }

  Menu(const std::string& title, bool is_enabled = true,
       bool delay_begin = kWidgetInitImmediate)
      : ScopedWidgetBase(delay_begin) {
    title_ = title;
    is_enabled_ = is_enabled;

    // Make sure the name is unique. Add it to the name map.
    cxx::BoolError valid_name = RequestNewName(title);
    if (valid_name) {
      title_ = title;
    } else
      throw valid_name.Exception();

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

 protected:
  bool BoundBegin() override {
    return ImGui::BeginMenu(title_.c_str(), is_enabled_);
  }

  void BoundEnd() override { ImGui::EndMenu(); }

 private:
  std::string title_;
  bool is_enabled_;
};

//-------------------------------------------------------------------------//
/* <class:TabBar> */
//-------------------------------------------------------------------------//
class TabBar : public ScopedWidgetBase {
 public:
  const std::string& Name() const { return name_; }
  TabBarFlags GetFlags() const { return flags_; }

 public:
  static inline TabBar Delayed(const std::string& name,
                               TabBarFlags flags = TabBarFlags()) {
    return TabBar(name, flags, kWidgetInitDelayed);
  }

  TabBar(const std::string& name, TabBarFlags flags = TabBarFlags(),
         bool delay_begin = kWidgetInitImmediate)
      : ScopedWidgetBase(delay_begin) {
    name_ = name;
    flags_ = flags;

    // Make sure the name is unique. Add it to the name map.
    cxx::BoolError valid_name = RequestNewName(name);
    if (valid_name) {
      name_ = name;
    } else
      throw valid_name.Exception();

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

 protected:
  bool BoundBegin() override {
    return ImGui::BeginTabBar(name_.c_str(), flags_.Get());
  }

  void BoundEnd() override { ImGui::EndTabBar(); }

 private:
  std::string name_;
  TabBarFlags flags_;
};

//-------------------------------------------------------------------------//
/* <class:TabItem> */
//-------------------------------------------------------------------------//
class TabItem : public ScopedWidgetBase {
 public:
  const std::string& Name() const { return name_; }

  //!! Warning: mutable reference
  TabItemFlags& GetFlags() { return flags_; }

  const TabItemFlags& GetFlags() const { return flags_; }

  bool IsSelected() const { return *is_selected_; }

 public:
  static inline TabItem Delayed(const std::string& title,
                                TabItemFlags flags = TabItemFlags()) {
    return TabItem(title, flags, kWidgetInitDelayed);
  }

  TabItem(const std::string& title, TabItemFlags flags = TabItemFlags(),
          bool delay_begin = kWidgetInitImmediate)
      : ScopedWidgetBase(delay_begin) {
    name_ = title;
    flags_ = flags;
    is_selected_ = (bool*)(nullptr);

    // Make sure the name is unique. Add it to the name map.
    cxx::BoolError valid_name = RequestNewName(title);
    if (valid_name) {
      name_ = title;
    } else
      throw valid_name.Exception();

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
  bool BoundBegin() override {
    return ImGui::BeginTabItem(name_.c_str(), is_selected_, flags_.Get());
  }

  void BoundEnd() override { ImGui::EndTabItem(); }

 private:
  std::string name_;
  TabItemFlags flags_;
  bool* is_selected_;
};

//-------------------------------------------------------------------------//
/* <class:TreeNode> */
//-------------------------------------------------------------------------//
class TreeNode : public ScopedWidgetBase {
  std::string name_;

 public:
  const std::string& Name() const { return name_; }

 public:
  static inline TreeNode Delayed(const std::string& name) {
    return TreeNode(name, kWidgetInitDelayed);
  }

  TreeNode(const std::string& name, bool delay_begin = kWidgetInitImmediate)
      : ScopedWidgetBase(delay_begin) {
    name_ = name;

    // Make sure the name is unique. Add it to the name map.
    cxx::BoolError valid_name = RequestNewName(name);
    if (valid_name) {
      name_ = name;
    } else
      throw valid_name.Exception();

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

 protected:
  bool BoundBegin() override { return ImGui::TreeNode(name_.c_str()); }

  void BoundEnd() override { ImGui::TreePop(); }
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
 public:
  const std::string& Text() const { return text_; }
  const CguiVec2& Size() const { return size_; }

 public:
  static inline Button Delayed(const std::string& text, CguiVec2 size = {}) {
    return Button(text, size, kWidgetInitDelayed);
  }
  Button(const std::string& text, CguiVec2 size = {},
         bool delayed_begin = kWidgetInitImmediate)
      : SingularWidgetBase(delayed_begin) {
    text_ = text;
    size_ = size;

    // Begin Scope if not delayed.
    BeginImpl();
  }

  bool BeginLate() override { return BeginLateImpl(); }

  ~Button() = default;

 protected:
  bool BoundBegin() override {
    return ImGui::Button(text_.c_str(), {size_.first, size_.second});
  }

 private:
  std::string text_;
  CguiVec2 size_;
};

//-------------------------------------------------------------------------//
/* <class:MenuItem> */
//-------------------------------------------------------------------------//
class MenuItem : public SingularWidgetBase {
 public:
  const std::string& Text() const { return text_; }
  const std::string& ShortcutHint() const { return shortcut_hint_; }
  bool IsEnabled() const { return is_enabled_; }

 public:
  static inline MenuItem Delayed(const std::string& text,
                                 const std::string& shortcut_hint = "",
                                 bool is_enabled = true) {
    return MenuItem(text, shortcut_hint, is_enabled, kWidgetInitDelayed);
  }
  MenuItem(const std::string& text, const std::string& shortcut_hint = "",
           bool is_enabled = true, bool delayed_begin = kWidgetInitImmediate)
      : SingularWidgetBase(delayed_begin) {
    text_ = text;
    shortcut_hint_ = shortcut_hint;
    is_enabled_ = is_enabled;

    // Begin Scope if not delayed.
    BeginImpl();
  }

  bool BeginLate() override { return BeginLateImpl(); }

  ~MenuItem() = default;

 protected:
  bool BoundBegin() override {
    return ImGui::MenuItem(text_.c_str(), shortcut_hint_.c_str(), false,
                           is_enabled_);
  }

 private:
  std::string text_;
  std::string shortcut_hint_;
  bool is_enabled_;
};

//-------------------------------------------------------------------------//
/* <class:Selectable> */
//-------------------------------------------------------------------------//
class Selectable : public SingularWidgetBase {
 public:
  const std::string& Text() const { return text_; }

 public:
  static inline Selectable Delayed(const std::string& text) {
    return Selectable(text, kWidgetInitDelayed);
  }
  Selectable(const std::string& text, bool delayed_begin = kWidgetInitImmediate)
      : SingularWidgetBase(delayed_begin) {
    text_ = text;

    // Begin Scope if not delayed.
    BeginImpl();
  }

  bool BeginLate() override { return BeginLateImpl(); }

  ~Selectable() = default;

 protected:
  bool BoundBegin() override { return ImGui::Selectable(text_.c_str()); }

 private:
  std::string text_;
};

//-------------------------------------------------------------------------//
/* <class:MultilineTextInput> */
//-------------------------------------------------------------------------//
class MultilineTextInput : public SingularWidgetBase {
  std::string label_;
  const CguiVec2& size_;
  std::string& buffer_;
  InputTextFlags flags_;

 public:
  const std::string& Label() const { return label_; }
  const CguiVec2& Size() const { return size_; }
  std::string& Buffer() { return buffer_; }
  InputTextFlags& InputFlags() { return flags_; }

  const std::string& Buffer() const { return buffer_; }
  const InputTextFlags& InputFlags() const { return flags_; }

 public:
  static inline MultilineTextInput Delayed(
      const std::string& label, std::string& buffer,
      const CguiVec2& size = {0.f, 0.f},
      InputTextFlags flags = InputTextFlags()) {
    return MultilineTextInput(label, buffer, size, flags, kWidgetInitDelayed);
  }
  MultilineTextInput(const std::string& label, std::string& buffer,
                     const CguiVec2& size = {},
                     InputTextFlags flags = InputTextFlags(),
                     bool delayed_begin = kWidgetInitImmediate)
      : SingularWidgetBase(delayed_begin),
        label_(label),
        size_(size),
        buffer_(buffer),
        flags_(flags) {
    // Begin Scope if not delayed.
    BeginImpl();
  }

  bool BeginLate() override { return BeginLateImpl(); }

  ~MultilineTextInput() = default;

 protected:
  bool BoundBegin() override {
    return ImGui::InputTextMultiline(label_.c_str(), &buffer_,
                                     {size_.first, size_.second}, flags_.Get());
  }
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
      const PathT& path, SelectedCallbackT selected_callback = [](auto&& a) {},
      bool is_delayed = false)
      : SingularWidgetBase(is_delayed),
        select_file_callback(selected_callback),
        root(path) {
    // Begin Scope if not delayed.
    BeginImpl();
  }
  DirectoryView(
      const PathT& path, SelectedCallbackT selected_callback = [](auto&& a) {},
      SelectedCallbackT right_click_callback = [](auto&& a) {},
      bool is_delayed = false)
      : SingularWidgetBase(is_delayed),
        select_file_callback(selected_callback),
        right_click_file_callback(right_click_callback),
        root(path) {
    // Begin Scope if not delayed.
    BeginImpl();
  }
  bool BeginLate() { return BeginLateImpl(); }

 private:
  SelectedCallbackT select_file_callback;
  SelectedCallbackT right_click_file_callback;
  const PathT& root;

  bool BoundBegin() override {
    RecursiveDisplayDirectory(root);
    return true;
  }

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
            CguiMultilineTextInput("Selected_Code", gEditorStringBuffer,
                                   cgui::kExpandWidgetToRemainingSpaceXY);
      selected_tab.EndEarly();
      auto other_tab = CguiTabItem("[Other]");
      if (other_tab)
        auto file_text_box =
            CguiMultilineTextInput("Other_Code", gEditorStringBuffer,
                                   cgui::kExpandWidgetToRemainingSpaceXY);
    }
  }
}
};  // namespace cgui::example

struct ExampleAppConsole {
  char InputBuf[256];
  ImVector<char*> Items;
  ImVector<const char*> Commands;
  ImVector<char*> History;
  int HistoryPos;  // -1: new line, 0..History.Size-1 browsing history.
  ImGuiTextFilter Filter;
  bool AutoScroll;
  bool ScrollToBottom;

  ExampleAppConsole() {
    ClearLog();
    memset(InputBuf, 0, sizeof(InputBuf));
    HistoryPos = -1;

    // "CLASSIFY" is here to provide the test case where "C"+[tab] completes to
    // "CL" and display multiple matches.
    Commands.push_back("HELP");
    Commands.push_back("HISTORY");
    Commands.push_back("CLEAR");
    Commands.push_back("CLASSIFY");
    AutoScroll = true;
    ScrollToBottom = false;
    AddLog("Welcome to Dear ImGui!");
  }
  ~ExampleAppConsole() {
    ClearLog();
    for (int i = 0; i < History.Size; i++) free(History[i]);
  }

  // Portable helpers
  static int Stricmp(const char* s1, const char* s2) {
    int d;
    while ((d = toupper(*s2) - toupper(*s1)) == 0 && *s1) {
      s1++;
      s2++;
    }
    return d;
  }
  static int Strnicmp(const char* s1, const char* s2, int n) {
    int d = 0;
    while (n > 0 && (d = toupper(*s2) - toupper(*s1)) == 0 && *s1) {
      s1++;
      s2++;
      n--;
    }
    return d;
  }
  static char* Strdup(const char* s) {
    IM_ASSERT(s);
    size_t len = strlen(s) + 1;
    void* buf = malloc(len);
    IM_ASSERT(buf);
    return (char*)memcpy(buf, (const void*)s, len);
  }
  static void Strtrim(char* s) {
    char* str_end = s + strlen(s);
    while (str_end > s && str_end[-1] == ' ') str_end--;
    *str_end = 0;
  }

  void ClearLog() {
    for (int i = 0; i < Items.Size; i++) free(Items[i]);
    Items.clear();
  }

  void AddLog(const char* fmt, ...) IM_FMTARGS(2) {
    // FIXME-OPT
    char buf[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
    buf[IM_ARRAYSIZE(buf) - 1] = 0;
    va_end(args);
    Items.push_back(Strdup(buf));
  }

  void Draw(const char* title, bool* p_open) {
    ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
    if (!ImGui::BeginChild(title)) {
      ImGui::EndChild();
      return;
    }

    // As a specific feature guaranteed by the library, after calling Begin()
    // the last Item represent the title bar. So e.g. IsItemHovered() will
    // return true when hovering the title bar. Here we create a context menu
    // only available from the title bar.
    if (ImGui::BeginPopupContextItem()) {
      if (ImGui::MenuItem("Close Console")) *p_open = false;
      ImGui::EndPopup();
    }

    ImGui::TextWrapped("");
    ImGui::TextWrapped("Enter 'HELP' for help.");

    // TODO: display items starting from the bottom

    if (ImGui::SmallButton("Add Debug Text")) {
      AddLog("%d some text", Items.Size);
      AddLog("some more text");
      AddLog("display very important message here!");
    }
    ImGui::SameLine();
    if (ImGui::SmallButton("Add Debug Error")) {
      AddLog("[error] something went wrong");
    }
    ImGui::SameLine();
    if (ImGui::SmallButton("Clear")) {
      ClearLog();
    }
    ImGui::SameLine();
    bool copy_to_clipboard = ImGui::SmallButton("Copy");
    // static float t = 0.0f; if (ImGui::GetTime() - t > 0.02f) { t =
    // ImGui::GetTime(); AddLog("Spam %f", t); }

    ImGui::Separator();

    // Options menu
    if (ImGui::BeginPopup("Options")) {
      ImGui::Checkbox("Auto-scroll", &AutoScroll);
      ImGui::EndPopup();
    }

    // Options, Filter
    if (ImGui::Button("Options")) ImGui::OpenPopup("Options");
    ImGui::SameLine();
    Filter.Draw("Filter (\"incl,-excl\") (\"error\")", 180);
    ImGui::Separator();

    // Reserve enough left-over height for 1 separator + 1 input text
    const float footer_height_to_reserve =
        ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
    if (ImGui::BeginChild(
            "ScrollingRegion", ImVec2(0, -footer_height_to_reserve),
            ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar)) {
      if (ImGui::BeginPopupContextWindow()) {
        if (ImGui::Selectable("Clear")) ClearLog();
        ImGui::EndPopup();
      }

      // Display every line as a separate entry so we can change their color or
      // add custom widgets. If you only want raw text you can use
      // ImGui::TextUnformatted(log.begin(), log.end()); NB- if you have
      // thousands of entries this approach may be too inefficient and may
      // require user-side clipping to only process visible items. The clipper
      // will automatically measure the height of your first item and then
      // "seek" to display only items in the visible area.
      // To use the clipper we can replace your standard loop:
      //      for (int i = 0; i < Items.Size; i++)
      //   With:
      //      ImGuiListClipper clipper;
      //      clipper.Begin(Items.Size);
      //      while (clipper.Step())
      //         for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
      // - That your items are evenly spaced (same height)
      // - That you have cheap random access to your elements (you can access
      // them given their index,
      //   without processing all the ones before)
      // You cannot this code as-is if a filter is active because it breaks the
      // 'cheap random-access' property. We would need random-access on the
      // post-filtered list. A typical application wanting coarse clipping and
      // filtering may want to pre-compute an array of indices or offsets of
      // items that passed the filtering test, recomputing this array when user
      // changes the filter, and appending newly elements as they are inserted.
      // This is left as a task to the user until we can manage to improve this
      // example code! If your items are of variable height:
      // - Split them into same height items would be simpler and facilitate
      // random-seeking into your list.
      // - Consider using manual call to IsRectVisible() and skipping extraneous
      // decoration from your items.
      ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,
                          ImVec2(4, 1));  // Tighten spacing
      if (copy_to_clipboard) ImGui::LogToClipboard();
      for (const char* item : Items) {
        if (!Filter.PassFilter(item)) continue;

        // Normally you would store more information in your item than just a
        // string. (e.g. make Items[] an array of structure, store color/type
        // etc.)
        ImVec4 color;
        bool has_color = false;
        if (strstr(item, "[error]")) {
          color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f);
          has_color = true;
        } else if (strncmp(item, "# ", 2) == 0) {
          color = ImVec4(1.0f, 0.8f, 0.6f, 1.0f);
          has_color = true;
        }
        if (has_color) ImGui::PushStyleColor(ImGuiCol_Text, color);
        ImGui::TextUnformatted(item);
        if (has_color) ImGui::PopStyleColor();
      }
      if (copy_to_clipboard) ImGui::LogFinish();

      // Keep up at the bottom of the scroll region if we were already at the
      // bottom at the beginning of the frame. Using a scrollbar or mouse-wheel
      // will take away from the bottom edge.
      if (ScrollToBottom ||
          (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
        ImGui::SetScrollHereY(1.0f);
      ScrollToBottom = false;

      ImGui::PopStyleVar();
    }
    ImGui::EndChild();
    ImGui::Separator();

    // Command-line
    bool reclaim_focus = false;
    ImGuiInputTextFlags input_text_flags =
        ImGuiInputTextFlags_EnterReturnsTrue |
        ImGuiInputTextFlags_EscapeClearsAll |
        ImGuiInputTextFlags_CallbackCompletion |
        ImGuiInputTextFlags_CallbackHistory;
    if (ImGui::InputText("Input", InputBuf, IM_ARRAYSIZE(InputBuf),
                         input_text_flags, &TextEditCallbackStub,
                         (void*)this)) {
      char* s = InputBuf;
      Strtrim(s);
      if (s[0]) ExecCommand(s);
      strcpy_s(s, sizeof s, "");
      reclaim_focus = true;
    }

    // Auto-focus on window apparition
    ImGui::SetItemDefaultFocus();
    if (reclaim_focus)
      ImGui::SetKeyboardFocusHere(-1);  // Auto focus previous widget

    ImGui::EndChild();
  }

  void ExecCommand(const char* command_line) {
    AddLog("# %s\n", command_line);

    // Insert into history. First find match and delete it so it can be pushed
    // to the back. This isn't trying to be smart or optimal.
    HistoryPos = -1;
    for (int i = History.Size - 1; i >= 0; i--)
      if (Stricmp(History[i], command_line) == 0) {
        free(History[i]);
        History.erase(History.begin() + i);
        break;
      }
    History.push_back(Strdup(command_line));

    // Process command
    if (Stricmp(command_line, "CLEAR") == 0) {
      ClearLog();
    } else if (Stricmp(command_line, "HELP") == 0) {
      AddLog("Commands:");
      for (int i = 0; i < Commands.Size; i++) AddLog("- %s", Commands[i]);
    } else if (Stricmp(command_line, "HISTORY") == 0) {
      int first = History.Size - 10;
      for (int i = first > 0 ? first : 0; i < History.Size; i++)
        AddLog("%3d: %s\n", i, History[i]);
    } else {
      AddLog("Unknown command: '%s'\n", command_line);
    }

    // On command input, we scroll to bottom even if AutoScroll==false
    ScrollToBottom = true;
  }

  // In C++11 you'd be better off using lambdas for this sort of forwarding
  // callbacks
  static int TextEditCallbackStub(ImGuiInputTextCallbackData* data) {
    ExampleAppConsole* console = (ExampleAppConsole*)data->UserData;
    return console->TextEditCallback(data);
  }

  int TextEditCallback(ImGuiInputTextCallbackData* data) {
    // AddLog("cursor: %d, selection: %d-%d", data->CursorPos,
    // data->SelectionStart, data->SelectionEnd);
    switch (data->EventFlag) {
      case ImGuiInputTextFlags_CallbackCompletion: {
        // Example of TEXT COMPLETION

        // Locate beginning of current word
        const char* word_end = data->Buf + data->CursorPos;
        const char* word_start = word_end;
        while (word_start > data->Buf) {
          const char c = word_start[-1];
          if (c == ' ' || c == '\t' || c == ',' || c == ';') break;
          word_start--;
        }

        // Build a list of candidates
        ImVector<const char*> candidates;
        for (int i = 0; i < Commands.Size; i++)
          if (Strnicmp(Commands[i], word_start, (int)(word_end - word_start)) ==
              0)
            candidates.push_back(Commands[i]);

        if (candidates.Size == 0) {
          // No match
          AddLog("No match for \"%.*s\"!\n", (int)(word_end - word_start),
                 word_start);
        } else if (candidates.Size == 1) {
          // Single match. Delete the beginning of the word and replace it
          // entirely so we've got nice casing.
          data->DeleteChars((int)(word_start - data->Buf),
                            (int)(word_end - word_start));
          data->InsertChars(data->CursorPos, candidates[0]);
          data->InsertChars(data->CursorPos, " ");
        } else {
          // Multiple matches. Complete as much as we can..
          // So inputing "C"+Tab will complete to "CL" then display "CLEAR" and
          // "CLASSIFY" as matches.
          int match_len = (int)(word_end - word_start);
          for (;;) {
            int c = 0;
            bool all_candidates_matches = true;
            for (int i = 0; i < candidates.Size && all_candidates_matches; i++)
              if (i == 0)
                c = toupper(candidates[i][match_len]);
              else if (c == 0 || c != toupper(candidates[i][match_len]))
                all_candidates_matches = false;
            if (!all_candidates_matches) break;
            match_len++;
          }

          if (match_len > 0) {
            data->DeleteChars((int)(word_start - data->Buf),
                              (int)(word_end - word_start));
            data->InsertChars(data->CursorPos, candidates[0],
                              candidates[0] + match_len);
          }

          // List matches
          AddLog("Possible matches:\n");
          for (int i = 0; i < candidates.Size; i++)
            AddLog("- %s\n", candidates[i]);
        }

        break;
      }
      case ImGuiInputTextFlags_CallbackHistory: {
        // Example of HISTORY
        const int prev_history_pos = HistoryPos;
        if (data->EventKey == ImGuiKey_UpArrow) {
          if (HistoryPos == -1)
            HistoryPos = History.Size - 1;
          else if (HistoryPos > 0)
            HistoryPos--;
        } else if (data->EventKey == ImGuiKey_DownArrow) {
          if (HistoryPos != -1)
            if (++HistoryPos >= History.Size) HistoryPos = -1;
        }

        // A better implementation would preserve the data on the current input
        // line along with cursor position.
        if (prev_history_pos != HistoryPos) {
          const char* history_str =
              (HistoryPos >= 0) ? History[HistoryPos] : "";
          data->DeleteChars(0, data->BufTextLen);
          data->InsertChars(0, history_str);
        }
      }
    }
    return 0;
  }
};
static void ShowAppConsole() {
  static ExampleAppConsole console;
  static bool po{true};
  console.Draw("Example: Console", &po);
}