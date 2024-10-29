// clang-format off
#include "cgui.hpp"
#include "cxxx.hpp"
#include "caf.hpp"
//#include "imgui.h"
//#include "imgui-SFML.h"
//#include "cgui_imgui_stdlib.hpp"
// clang-format on

namespace cgui {

UIDGen::UIDGen() : next_id_(1) {}

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

void UIDGen::Refresh() {
  generated_ids_.clear();
  next_id_ = 1;
}

bool UIDGen::Empty() { return generated_ids_.empty(); }

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

bool UniqueNameMap::Contains(const std::string& str) {
  return names_.contains(str);
}

constexpr bool ScopedWidgetBase::IsScopeActive() const {
  return is_scope_active_;
}

bool ScopedWidgetBase::IsOn() const { return is_on_; }

bool ScopedWidgetBase::BeginImpl() {
  if (is_delayed_ == kWidgetInitImmediate) {
    is_scope_active_ = true;
    is_on_ = BoundBegin();
  } else {
    is_scope_active_ = false;
    is_on_ = false;
  }
  return is_on_;
}

bool ScopedWidgetBase::BeginLateImpl() {
  if (not is_scope_active_) {
    is_scope_active_ = true;
    is_on_ = BoundBegin();
  }
  return is_on_;
}

void ScopedWidgetBase::EndImpl() {
  if (is_scope_active_ && is_on_) BoundEnd();
}

void ScopedWidgetBase::EndEarlyImpl() {
  if (not is_scope_active_) throw "[ EndEarly() called before begin. ]";
  is_scope_active_ = false;
  if (is_on_) BoundEnd();
}

void ScopedWidgetBase::ForceEndImpl() {
  if (is_scope_active_) BoundEnd();
}

void ScopedWidgetBase::ForceEndEarlyImpl() {
  if (not is_scope_active_) throw "[ EndEarly() called before begin. ]";
  is_scope_active_ = false;
  BoundEnd();
}

ScopedWidgetBase::ScopedWidgetBase(bool is_delayed)
    : name_map_(gCguiDefaultUniqueNameMap),
      id_gen_(gCguiDefaultUIDGenerator),
      is_delayed_(is_delayed) {
  is_on_ = false;
  is_scope_active_ = false;
}

ScopedWidgetBase::operator bool() { return is_on_; }

cxx::BoolError ScopedWidgetBase::RequestNewName(const std::string& str) {
  if (name_map_.Contains(str))
    return "[WidgetBase:NewName:This widget name is already in use.]";
  name_map_.AddName(str);
  return true;
};

void ScopedWidgetBase::ReleaseName(const std::string& str) {
  /// Remove the name from name map.
  name_map_.RemoveName(str);
}

UIDGen::Iter ScopedWidgetBase::RequestId() { return id_gen_.GetId(); }

void ScopedWidgetBase::ReleaseId(UIDGen::Iter id_iter) {
  /// Remove the id
  id_gen_.EraseId(id_iter);
}

bool SingularWidgetBase::IsOn() const { return is_on_; }

bool SingularWidgetBase::IsDelayed() const { return is_delayed_; }

/// Boolean conversion, returns IsOn()
SingularWidgetBase::operator bool() { return is_on_; }

bool SingularWidgetBase::BeginImpl() {
  if (is_delayed_ == kWidgetInitImmediate) {
    is_on_ = BoundBegin();
  } else {
    is_on_ = false;
  }
  return is_on_;
}

bool SingularWidgetBase::BeginLateImpl() {
  is_on_ = BoundBegin();
  return is_on_;
}

SingularWidgetBase::SingularWidgetBase(bool is_delayed)
    : is_delayed_(is_delayed), is_on_(false) {}

namespace scoped_widget {

const std::string& Window::Title() const { return title_; }

WindowFlags& Window::Flags() { return flags_; }

bool Window::IsCloseButtonTriggered() const { return not *close_button_state_; }

const CguiVec2& Window::QuerySize() const { return size_; }

float Window::QueryWidth() const { return size_.first; }

float Window::QueryHeight() const { return size_.second; }

Window Window::Delayed(const std::string& title, bool has_close_button,
                       WindowFlags flags) {
  return Window(title, has_close_button, flags, kWidgetInitDelayed);
}

Window::Window(const std::string& title, bool has_close_button,
               WindowFlags flags, bool delay_begin)
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

bool Window::BeginLate() {
  bool on = BeginLateImpl();
  if (IsScopeActive()) {
    size_.first = ImGui::GetWindowWidth();
    size_.second = ImGui::GetWindowHeight();
  }
  return on;
}

void Window::EndEarly() { ForceEndEarlyImpl(); }

Window::~Window() {
  ForceEndImpl();
  // Remove the name from name map.
  ReleaseName(title_);
}

Window::Window(const Window& other) : ScopedWidgetBase(other.is_delayed_) {
  this->is_scope_active_ = other.is_scope_active_;
  this->is_on_ = other.is_on_;
  this->title_ = other.title_;
  this->flags_ = other.flags_;
  this->has_close_button_ = other.has_close_button_;
  this->close_button_state_ =
      std::make_unique<bool>(*other.close_button_state_);
}

Window::Window(Window&& other) noexcept
    : ScopedWidgetBase(std::move(other.is_delayed_)) {
  this->is_scope_active_ = std::move(other.is_scope_active_);
  this->is_on_ = std::move(other.is_on_);
  this->title_ = std::move(other.title_);
  this->flags_ = std::move(other.flags_);
  this->has_close_button_ = std::move(other.has_close_button_);
  this->close_button_state_ = std::move(other.close_button_state_);
}

bool Window::BoundBegin() {
  if (has_close_button_) {
    close_button_state_ = std::make_unique<bool>(true);
  }
  return ImGui::Begin(title_.c_str(), close_button_state_.get(), flags_.Get());
}

void Window::BoundEnd() { ImGui::End(); }

std::size_t Subcontext::Id() const { return *uid_; }

/// @warning Returns a mutable reference.
WindowFlags Subcontext::GetWindowFlags() const { return win_flags_; }

/// @warning Returns a mutable reference.
SubcontextFlags Subcontext::GetSubcontextFlags() const {
  return subcontext_flags_;
}

const CguiVec2& Subcontext::RequestedSize() { return requested_size_; }

Subcontext Subcontext::Delayed(CguiVec2 size, WindowFlags win_flags,
                               SubcontextFlags subcontext_flags) {
  return Subcontext(size, win_flags, subcontext_flags, kWidgetInitDelayed);
}

Subcontext::Subcontext(CguiVec2 size, WindowFlags win_flags,
                       SubcontextFlags subcontext_flags, bool delay_begin)
    : ScopedWidgetBase(delay_begin) {
  uid_ = RequestId();
  win_flags_ = win_flags;
  subcontext_flags_ = subcontext_flags;
  requested_size_ = size;

  // Begin Scope if not delayed.
  BeginImpl();
}
bool Subcontext::BeginLate() { return BeginLateImpl(); }

void Subcontext::EndEarly() { ForceEndEarlyImpl(); }

Subcontext::~Subcontext() {
  ForceEndImpl();
  // Remove the name from name map.
  ReleaseId(uid_);
}

bool Subcontext::BoundBegin() {
  return ImGui::BeginChild(*uid_,
                           {requested_size_.first, requested_size_.second},
                           subcontext_flags_, win_flags_);
}

void Subcontext::BoundEnd() { ImGui::EndChild(); }

const std::string& NamedSubcontext::Name() const { return name_; }

/// @warning Returns a mutable reference.
WindowFlags NamedSubcontext::GetWindowFlags() const { return win_flags_; }

/// @warning Returns a mutable reference.
SubcontextFlags NamedSubcontext::GetSubcontextFlags() const {
  return subcontext_flags_;
}

const CguiVec2& NamedSubcontext::RequestedSize() { return requested_size_; }
void NamedSubcontext::RequestSize(const CguiVec2& size) {
  requested_size_ = size;
}

NamedSubcontext NamedSubcontext::Delayed(const std::string& name, CguiVec2 size,
                                         WindowFlags win_flags,
                                         SubcontextFlags subcontext_flags) {
  return NamedSubcontext(name, size, win_flags, subcontext_flags,
                         kWidgetInitDelayed);
}

NamedSubcontext::NamedSubcontext(const std::string& name, CguiVec2 size,
                                 WindowFlags win_flags,
                                 SubcontextFlags subcontext_flags,
                                 bool delay_begin)
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

bool NamedSubcontext::BeginLate() { return BeginLateImpl(); }

void NamedSubcontext::EndEarly() { ForceEndEarlyImpl(); }

NamedSubcontext::~NamedSubcontext() {
  ForceEndImpl();
  // Remove the name from name map.
  ReleaseName(name_);
}

bool NamedSubcontext::BoundBegin() {
  return ImGui::BeginChild(name_.c_str(),
                           {requested_size_.first, requested_size_.second},
                           subcontext_flags_, win_flags_);
}

void NamedSubcontext::BoundEnd() { ImGui::EndChild(); }

MenuBar MenuBar::Delayed() { return MenuBar(kWidgetInitDelayed); }

MenuBar::MenuBar(bool delay_begin) : ScopedWidgetBase(delay_begin) {
  // Begin Scope if not delayed.
  BeginImpl();
}

bool MenuBar::BeginLate() { return BeginLateImpl(); }

void MenuBar::EndEarly() { EndEarlyImpl(); }

MenuBar::~MenuBar() { EndImpl(); }

bool MenuBar::BoundBegin() { return ImGui::BeginMenuBar(); }

void MenuBar::BoundEnd() { ImGui::EndMenuBar(); }

const std::string& Menu::Title() const { return title_; }

bool Menu::IsEnabled() const { return is_enabled_; }

Menu Menu::Delayed(const std::string& title, bool is_enabled) {
  return Menu(title, is_enabled, kWidgetInitDelayed);
}

Menu::Menu(const std::string& title, bool is_enabled, bool delay_begin)
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

bool Menu::BeginLate() { return BeginLateImpl(); }

void Menu::EndEarly() { EndEarlyImpl(); }

Menu::~Menu() {
  EndImpl();
  // Remove the name from name map.
  ReleaseName(title_);
}

bool Menu::BoundBegin() {
  return ImGui::BeginMenu(title_.c_str(), is_enabled_);
}

void Menu::BoundEnd() { ImGui::EndMenu(); }

const std::string& TabBar::Name() const { return name_; }

TabBarFlags TabBar::GetFlags() const { return flags_; }

TabBar TabBar::Delayed(const std::string& name, TabBarFlags flags) {
  return TabBar(name, flags, kWidgetInitDelayed);
}

TabBar::TabBar(const std::string& name, TabBarFlags flags, bool delay_begin)
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

bool TabBar::BeginLate() { return BeginLateImpl(); }

void TabBar::EndEarly() { EndEarlyImpl(); }

TabBar::~TabBar() {
  EndImpl();
  // Remove the name from name map.
  ReleaseName(name_);
}

bool TabBar::BoundBegin() {
  return ImGui::BeginTabBar(name_.c_str(), flags_.Get());
}

void TabBar::BoundEnd() { ImGui::EndTabBar(); }

const std::string& TabItem::Name() const { return name_; }

//!! Warning: mutable reference
TabItemFlags& TabItem::GetFlags() { return flags_; }

const TabItemFlags& TabItem::GetFlags() const { return flags_; }

bool TabItem::IsSelected() const { return *is_selected_; }

TabItem TabItem::Delayed(const std::string& title, TabItemFlags flags) {
  return TabItem(title, flags, kWidgetInitDelayed);
}

TabItem::TabItem(const std::string& title, TabItemFlags flags, bool delay_begin)
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

bool TabItem::BeginLate() { return BeginLateImpl(); }

void TabItem::EndEarly() { EndEarlyImpl(); }

TabItem::~TabItem() {
  EndImpl();
  // Remove the name from name map.
  ReleaseName(name_);
}

bool TabItem::BoundBegin() {
  return ImGui::BeginTabItem(name_.c_str(), is_selected_, flags_.Get());
}

void TabItem::BoundEnd() { ImGui::EndTabItem(); }

const std::string& TreeNode::Name() const { return name_; }

TreeNode TreeNode::Delayed(const std::string& name) {
  return TreeNode(name, kWidgetInitDelayed);
}

TreeNode::TreeNode(const std::string& name, bool delay_begin)
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

bool TreeNode::BeginLate() { return BeginLateImpl(); }

void TreeNode::EndEarly() { EndEarlyImpl(); }

TreeNode::~TreeNode() {
  EndImpl();
  // Remove the name from name map.
  ReleaseName(name_);
}

bool TreeNode::BoundBegin() { return ImGui::TreeNode(name_.c_str()); }

void TreeNode::BoundEnd() { ImGui::TreePop(); }

}  // end namespace scoped_widget

namespace single_widget {

const std::string& Button::Text() const { return text_; }

const CguiVec2& Button::Size() const { return size_; }

Button Button::Delayed(const std::string& text, CguiVec2 size) {
  return Button(text, size, kWidgetInitDelayed);
}
Button::Button(const std::string& text, CguiVec2 size, bool delayed_begin)
    : SingularWidgetBase(delayed_begin) {
  text_ = text;
  size_ = size;

  // Begin Scope if not delayed.
  BeginImpl();
}

bool Button::BeginLate() { return BeginLateImpl(); }

bool Button::BoundBegin() {
  return ImGui::Button(text_.c_str(), {size_.first, size_.second});
}

const std::string& MenuItem::Text() const { return text_; }

const std::string& MenuItem::ShortcutHint() const { return shortcut_hint_; }

bool MenuItem::IsEnabled() const { return is_enabled_; }

MenuItem MenuItem::Delayed(const std::string& text,
                           const std::string& shortcut_hint, bool is_enabled) {
  return MenuItem(text, shortcut_hint, is_enabled, kWidgetInitDelayed);
}
MenuItem::MenuItem(const std::string& text, const std::string& shortcut_hint,
                   bool is_enabled, bool delayed_begin)
    : SingularWidgetBase(delayed_begin) {
  text_ = text;
  shortcut_hint_ = shortcut_hint;
  is_enabled_ = is_enabled;

  // Begin Scope if not delayed.
  BeginImpl();
}

bool MenuItem::BeginLate() { return BeginLateImpl(); }

bool MenuItem::BoundBegin() {
  return ImGui::MenuItem(text_.c_str(), shortcut_hint_.c_str(), false,
                         is_enabled_);
}

const std::string& Selectable::Text() const { return text_; }

Selectable Selectable::Delayed(const std::string& text) {
  return Selectable(text, kWidgetInitDelayed);
}

Selectable::Selectable(const std::string& text, bool delayed_begin)
    : SingularWidgetBase(delayed_begin) {
  text_ = text;

  // Begin Scope if not delayed.
  BeginImpl();
}

bool Selectable::BeginLate() { return BeginLateImpl(); }

bool Selectable::BoundBegin() { return ImGui::Selectable(text_.c_str()); }

const std::string& MultilineTextInput::Label() const { return label_; }

const CguiVec2& MultilineTextInput::Size() const { return size_; }

std::string& MultilineTextInput::Buffer() { return buffer_; }

InputTextFlags& MultilineTextInput::InputFlags() { return flags_; }

const std::string& MultilineTextInput::Buffer() const { return buffer_; }

const InputTextFlags& MultilineTextInput::InputFlags() const { return flags_; }

MultilineTextInput MultilineTextInput::Delayed(const std::string& label,
                                               std::string& buffer,
                                               const CguiVec2& size,
                                               InputTextFlags flags) {
  return MultilineTextInput(label, buffer, size, flags, kWidgetInitDelayed);
}

MultilineTextInput::MultilineTextInput(const std::string& label,
                                       std::string& buffer,
                                       const CguiVec2& size,
                                       InputTextFlags flags, bool delayed_begin)
    : SingularWidgetBase(delayed_begin),
      label_(label),
      size_(size),
      buffer_(buffer),
      flags_(flags) {
  // Begin Scope if not delayed.
  BeginImpl();
}

bool MultilineTextInput::BeginLate() { return BeginLateImpl(); }

bool MultilineTextInput::BoundBegin() {
  return ImGui::InputTextMultiline(label_.c_str(), &buffer_,
                                   {size_.first, size_.second}, flags_.Get());
}

}  // namespace single_widget

namespace combo_widget {

DirectoryView::DirectoryView(const PathT& path,
                             SelectedCallbackT selected_callback,
                             bool is_delayed)
    : SingularWidgetBase(is_delayed),
      select_file_callback(selected_callback),
      root(path) {
  // Begin Scope if not delayed.
  BeginImpl();
}

DirectoryView::DirectoryView(const PathT& path,
                             SelectedCallbackT selected_callback,
                             SelectedCallbackT right_click_callback,
                             bool is_delayed)
    : SingularWidgetBase(is_delayed),
      select_file_callback(selected_callback),
      right_click_file_callback(right_click_callback),
      root(path) {
  // Begin Scope if not delayed.
  BeginImpl();
}

bool DirectoryView::BeginLate() { return BeginLateImpl(); }

bool DirectoryView::BoundBegin() {
  RecursiveDisplayDirectory(root);
  return true;
}

void DirectoryView::RecursiveDisplayDirectory(const PathT& path, int depth) {
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

///////////////////////////////////////////////////////////////////////////////
}; /* end namespace combo_widget */
///////////////////////////////////////////////////////////////////////////////

namespace example {
void ExampleHelloWindow() {
  CguiWindow my_window{"My Window"};

  // Inside your main loop or update function....
  {
    // Do things based on if the window is open or not.
    // Use my_window.IsOn() to be explicit.
    if (my_window)                                 // Use implict bool
                                                   // conversion.
      std::cout << "Hello World from my window!";  // Draw other elements
    //  inside
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
  }
  // add more stuff.....

  // window will end itself here.
}
}  // namespace example

}  // namespace cgui
