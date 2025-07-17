#include <concepts>
#include <iterator>
#include <list>
#include <numeric>
#include <optional>
#include <type_traits>
#include <vector>

namespace cxd {
template <class T, class U>
concept copyable_storage = std::same_as<T, typename std::remove_cvref_t<U>> && std::copy_constructible<U>;
//
//template <class T>
//class node {
// private:
//  class depth_first_preorder_iterator;  // fwd
//  class depth_first_preorder_iterator_const;
//
// public:
//  using value_type = T;
//  using reference = T&;
//  using pointer = T*;
//  using const_pointer = const T*;
//  using const_reference = const T&;
//  using difference_type = std::ptrdiff_t;
//  using size_type = std::size_t;
//  using iterator = depth_first_preorder_iterator;
//  using const_iterator = depth_first_preorder_iterator_const;
//
//  using branch_list_type = std::vector<node>;
//
//  class depth_first_preorder_iterator {
//   public:
//    using iterator_category = std::forward_iterator_tag;
//    using value_type = node::value_type;
//    using difference_type = node::difference_type;
//    using size_type = node::size_type;
//    using pointer = node::pointer;
//    using reference = node::reference;
//
//    /// Indicates the current iterator should point to the stem branch of the 'this->ptr_' node, stored directly inside
//    /// 'node.value()'.
//    static constexpr std::size_t kValueIndex = std::size_t(-1);
//
//   private:
//    node* ptr_;
//    std::size_t idx_{kValueIndex};
//
//   public:
//    constexpr depth_first_preorder_iterator(node* ptr, std::size_t branch_index = kValueIndex)
//        : ptr_(ptr), idx_(branch_index) {}
//
//    constexpr depth_first_preorder_iterator& operator++() {
//      if (ptr_ == nullptr) return *this;
//
//      // Value state -> go to first child transition
//      if (idx_ == kValueIndex) {
//        if (!ptr_->branches_.empty()) {
//          ptr_ = &ptr_->branches_.front();
//          return *this;
//        }
//        goto move_to_sibling;
//      }
//
//      // Non-last branch -> advance to next sibling
//      if (idx_ + 1 < ptr_->branches_.size()) {
//        ++idx_;
//        return *this;
//      }
//
//    move_to_sibling:
//      // Find next sibling in parent's branches
//      while (ptr_->stem_ != nullptr) {
//        node* parent = const_cast<node*>(ptr_->stem_);
//        auto& branches = parent->branches_;
//        size_t index = ptr_ - branches.data();
//
//        if (index + 1 < branches.size()) {
//          ptr_ = &branches[index + 1];
//          idx_ = kValueIndex;
//          return *this;
//        }
//        ptr_ = parent;
//      }
//
//      // End of traversal
//      ptr_ = nullptr;
//      return *this;
//    }
//
//    constexpr value_type& operator*() const {
//      if (idx_ == kValueIndex)
//        return ptr_->value();
//      else if (idx_ < ptr_->branches_.size())
//        return ptr_->branches_[idx_].value();
//      else
//        throw std::out_of_range("[cxd::node] Branch index out of range.");
//    }
//
//    constexpr bool operator==(const depth_first_preorder_iterator& other) const {
//      if (ptr_ == nullptr && other.ptr_ == nullptr) return true;
//      if (ptr_ == nullptr || other.ptr_ == nullptr) return false;
//      return ptr_ == other.ptr_ && idx_ == other.idx_;
//    }
//  };
//  class depth_first_preorder_iterator_const {
//   public:
//    using iterator_category = std::forward_iterator_tag;
//    using value_type = node::value_type;
//    using difference_type = node::difference_type;
//    using size_type = node::size_type;
//    using pointer = node::pointer;
//    using const_pointer = node::const_pointer;
//    using reference = node::reference;
//
//    /// Indicates the current iterator should point to the stem branch of the 'this->ptr_' node, stored directly inside
//    /// 'node.value()'.
//    static constexpr std::size_t kValueIndex = std::size_t(-1);
//
//   private:
//    const node* ptr_;
//    std::size_t idx_{kValueIndex};
//
//   public:
//    constexpr depth_first_preorder_iterator_const(const node* ptr, std::size_t branch_index = kValueIndex)
//        : ptr_(ptr), idx_(branch_index) {}
//
//    constexpr depth_first_preorder_iterator_const& operator++() {
//      if (ptr_ == nullptr) return *this;
//
//      // Value state -> go to first child transition
//      if (idx_ == kValueIndex) {
//        if (!ptr_->branches_.empty()) {
//          ptr_ = &ptr_->branches_.front();
//          return *this;
//        }
//        goto move_to_sibling;
//      }
//
//      // Non-last branch -> advance to next sibling
//      if (idx_ + 1 < ptr_->branches_.size()) {
//        ++idx_;
//        return *this;
//      }
//
//    move_to_sibling:
//      // Find next sibling in parent's branches
//      while (ptr_->stem_ != nullptr) {
//        node* parent = const_cast<node*>(ptr_->stem_);
//        auto& branches = parent->branches_;
//        size_t index = ptr_ - branches.data();
//
//        if (index + 1 < branches.size()) {
//          ptr_ = &branches[index + 1];
//          idx_ = kValueIndex;
//          return *this;
//        }
//        ptr_ = parent;
//      }
//
//      // End of traversal
//      ptr_ = nullptr;
//      return *this;
//    }
//
//    constexpr const value_type& operator*() const {
//      if (idx_ == kValueIndex)
//        return ptr_->value();
//      else if (idx_ < ptr_->branches_.size())
//        return ptr_->branches_[idx_].value();
//      else
//        throw std::out_of_range("[cxd::node] Branch index out of range.");
//    }
//
//    constexpr bool operator==(const depth_first_preorder_iterator_const& other) const {
//      if (ptr_ == nullptr && other.ptr_ == nullptr) return true;
//      if (ptr_ == nullptr || other.ptr_ == nullptr) return false;
//      return ptr_ == other.ptr_ && idx_ == other.idx_;
//    }
//  };
//
// private:
// public:
//  constexpr node() = default;
//
//  template <copyable_storage<value_type> U>  // enable ONLY for copyable elements
//  constexpr node(const U& value, const node* stem = nullptr) : value_(value), stem_(stem), branches_() {}
//
//  constexpr node(T&& value, const node* stem = nullptr) : value_(std::forward<T>(value)), stem_(stem), branches_() {}
//
//  constexpr node(node&& other) noexcept
//      : value_(std::move(other.value_)), stem_(other.stem_), branches_(std::move(other.branches_)) {
//    for (auto& branch : branches_) {
//      branch.realign(this);
//    }
//  }
//
//  constexpr node& operator=(node&& other) noexcept {
//    if (this != &other) {
//      value_ = std::move(other.value_);
//      stem_ = other.stem_;
//      branches_ = std::move(other.branches_);
//      for (auto& branch : branches_) {
//        branch.realign(this);
//      }
//    }
//    return *this;
//  }
//
// public:
//  constexpr void clear() {
//    branches_.clear();  // Clear branches
//  }
//  constexpr node& branch(std::size_t idx) { return branches_[idx]; }
//  constexpr branch_list_type& branches() { return branches_; }
//  constexpr const branch_list_type& branches() const { return branches_; }
//  constexpr value_type& value() { return value_; }
//  constexpr const value_type& value() const { return value_; }
//  constexpr bool is_leaf() const { return branches_.empty(); }
//  constexpr bool is_trunk() const { return !stem_; }
//  constexpr const node* stem() const { return stem_; }
//  constexpr node* stem() { return const_cast<node*>(stem_); }
//  constexpr const node* root() const {
//    if (!stem_) return this;
//    return stem_->root();
//  }
//  constexpr node* root() {
//    if (!stem_) return this;
//    return const_cast<node*>(stem_->root());
//  }
//  constexpr void push(const T& value) { branches_.emplace_back(value, this); }
//  constexpr void push(T&& value) { branches_.emplace_back(std::forward<T>(value), this); }
//  constexpr node& back_branch() { return branches_.back(); }
//
//  constexpr void apply(const std::function<void(value_type&)>& func) {
//    func(value_);
//    if (is_leaf()) return;
//    for (auto& branch : branches_) {
//      branch.apply(func);
//    }
//  }
//  constexpr void apply(const std::function<void(const value_type&)>& func) const {
//    func(value_);
//    if (is_leaf()) return;
//    for (const auto& branch : branches_) {
//      branch.apply(func);
//    }
//  }
//  constexpr void apply(const std::function<void(cxd::node<value_type>&)>& func) {
//    func(*this);
//    if (is_leaf()) return;
//    for (auto& branch : branches_) {
//      branch.apply(func);
//    }
//  }
//  constexpr void apply(const std::function<void(const cxd::node<value_type>&)>& func) const {
//    func(*this);
//    if (is_leaf()) return;
//    for (const auto& branch : branches_) {
//      branch.apply(func);
//    }
//  }
//
//  constexpr void apply_branches(const std::function<void(value_type&)>& func) {
//    if (is_leaf()) return;
//    for (auto& branch : branches_) {
//      branch.apply(func);
//    }
//  }
//  constexpr void apply_branches(const std::function<void(const value_type&)>& func) const {
//    if (is_leaf()) return;
//    for (const auto& branch : branches_) {
//      branch.apply(func);
//    }
//  }
//  constexpr void apply_branches(const std::function<void(cxd::node<value_type>&)>& func) {
//    if (is_leaf()) return;
//    for (auto& branch : branches_) {
//      branch.apply(func);
//    }
//  }
//  constexpr void apply_branches(const std::function<void(const cxd::node<value_type>&)>& func) const {
//    if (is_leaf()) return;
//    for (const auto& branch : branches_) {
//      branch.apply(func);
//    }
//  }
//
//  constexpr value_type* operator->() { return &value_; }
//  constexpr const value_type* operator->() const { return &value_; }
//  constexpr value_type& operator*() { return value_; }
//  constexpr const value_type& operator*() const { return value_; }
//
//  constexpr bool operator==(const node& other) const noexcept { return value_ == other.value_; }
//  iterator begin() { return iterator(this); }
//  iterator end() { return iterator(nullptr); }
//  const_iterator cbegin() const { return const_iterator(this); }
//  const_iterator cend() const { return const_iterator(nullptr); }
//
// private:
//  constexpr void realign(node* new_stem) {
//    stem_ = new_stem;
//    for (auto& branch : branches_) {
//      branch.realign(this);  // Recursively fix children
//    }
//  }
//
//  T value_{};
//  const node* stem_{nullptr};
//  branch_list_type branches_{};
//};
//
//template <class T>
//class list_node {
// private:
//  class depth_first_preorder_iterator;  // fwd
//  class depth_first_preorder_iterator_const;
//
// public:
//  using value_type = T;
//  using reference = T&;
//  using pointer = T*;
//  using const_pointer = const T*;
//  using const_reference = const T&;
//  using difference_type = std::ptrdiff_t;
//  using size_type = std::size_t;
//  using iterator = depth_first_preorder_iterator;
//  using const_iterator = depth_first_preorder_iterator_const;
//
//  using branch_list_type = std::list<list_node>;
//
//  class depth_first_preorder_iterator {
//   public:
//    using iterator_category = std::forward_iterator_tag;
//    using value_type = list_node::value_type;
//    using difference_type = list_node::difference_type;
//    using size_type = list_node::size_type;
//    using pointer = list_node::pointer;
//    using reference = list_node::reference;
//
//    /// Indicates the current iterator should point to the stem branch of the 'this->ptr_' list_node, stored directly
//    /// inside 'list_node.value()'.
//    constexpr static std::size_t kValueIndex = std::size_t(-1);
//
//   private:
//    list_node* ptr_;
//    std::size_t idx_{kValueIndex};
//
//   public:
//    depth_first_preorder_iterator(list_node* ptr, std::size_t branch_index = kValueIndex)
//        : ptr_(ptr), idx_(branch_index) {}
//
//    depth_first_preorder_iterator& operator++() {
//      if (ptr_ == nullptr) return *this;
//
//      // Value state -> go to first child transition
//      if (idx_ == kValueIndex) {
//        if (!ptr_->branches_.empty()) {
//          ptr_ = &ptr_->branches_.front();
//          return *this;
//        }
//        goto move_to_sibling;
//      }
//
//      // Non-last branch -> advance to next sibling
//      if (idx_ + 1 < ptr_->branches_.size()) {
//        ++idx_;
//        return *this;
//      }
//
//    move_to_sibling:
//      // Find next sibling in parent's branches
//      while (ptr_->stem_ != nullptr) {
//        list_node* parent = const_cast<list_node*>(ptr_->stem_);
//        auto& branches = parent->branches_;
//        size_t index = ptr_ - branches.data();
//
//        if (index + 1 < branches.size()) {
//          ptr_ = &branches[index + 1];
//          idx_ = kValueIndex;
//          return *this;
//        }
//        ptr_ = parent;
//      }
//
//      // End of traversal
//      ptr_ = nullptr;
//      return *this;
//    }
//
//    value_type& operator*() const {
//      if (idx_ == kValueIndex)
//        return ptr_->value();
//      else if (idx_ < ptr_->branches_.size())
//        return ptr_->branches_[idx_].value();
//      else
//        throw std::out_of_range("[cxd::list_node] Branch index out of range.");
//    }
//
//    bool operator==(const depth_first_preorder_iterator& other) const {
//      if (ptr_ == nullptr && other.ptr_ == nullptr) return true;
//      if (ptr_ == nullptr || other.ptr_ == nullptr) return false;
//      return ptr_ == other.ptr_ && idx_ == other.idx_;
//    }
//  };
//  class depth_first_preorder_iterator_const {
//   public:
//    using iterator_category = std::forward_iterator_tag;
//    using value_type = list_node::value_type;
//    using difference_type = list_node::difference_type;
//    using size_type = list_node::size_type;
//    using pointer = list_node::pointer;
//    using const_pointer = list_node::const_pointer;
//    using reference = list_node::reference;
//
//    /// Indicates the current iterator should point to the stem branch of the 'this->ptr_' list_node, stored directly
//    /// inside 'list_node.value()'.
//    constexpr static std::size_t kValueIndex = std::size_t(-1);
//
//   private:
//    const list_node* ptr_;
//    std::size_t idx_{kValueIndex};
//
//   public:
//    depth_first_preorder_iterator_const(const list_node* ptr, std::size_t branch_index = kValueIndex)
//        : ptr_(ptr), idx_(branch_index) {}
//
//    depth_first_preorder_iterator_const& operator++() {
//      if (ptr_ == nullptr) return *this;
//
//      // Value state -> go to first child transition
//      if (idx_ == kValueIndex) {
//        if (!ptr_->branches_.empty()) {
//          ptr_ = &ptr_->branches_.front();
//          return *this;
//        }
//        goto move_to_sibling;
//      }
//
//      // Non-last branch -> advance to next sibling
//      if (idx_ + 1 < ptr_->branches_.size()) {
//        ++idx_;
//        return *this;
//      }
//
//    move_to_sibling:
//      // Find next sibling in parent's branches
//      while (ptr_->stem_ != nullptr) {
//        list_node* parent = const_cast<list_node*>(ptr_->stem_);
//        auto& branches = parent->branches_;
//        size_t index = ptr_ - branches.data();
//
//        if (index + 1 < branches.size()) {
//          ptr_ = &branches[index + 1];
//          idx_ = kValueIndex;
//          return *this;
//        }
//        ptr_ = parent;
//      }
//
//      // End of traversal
//      ptr_ = nullptr;
//      return *this;
//    }
//
//    const value_type& operator*() const {
//      if (idx_ == kValueIndex)
//        return ptr_->value();
//      else if (idx_ < ptr_->branches_.size())
//        return ptr_->branches_[idx_].value();
//      else
//        throw std::out_of_range("[cxd::list_node] Branch index out of range.");
//    }
//
//    bool operator==(const depth_first_preorder_iterator_const& other) const {
//      if (ptr_ == nullptr && other.ptr_ == nullptr) return true;
//      if (ptr_ == nullptr || other.ptr_ == nullptr) return false;
//      return ptr_ == other.ptr_ && idx_ == other.idx_;
//    }
//  };
//
// private:
// public:
//  list_node() = default;
//
//  template <copyable_storage<value_type> U>  // enable ONLY for copyable elements
//  list_node(const U& value, const list_node* stem = nullptr) : value_(value), stem_(stem), branches_() {}
//
//  list_node(T&& value, const list_node* stem = nullptr) : value_(std::forward<T>(value)), stem_(stem), branches_() {}
//
//  list_node(list_node&& other) noexcept
//      : value_(std::move(other.value_)), stem_(other.stem_), branches_(std::move(other.branches_)) {}
//
//  list_node& operator=(list_node&& other) noexcept {
//    value_ = std::move(other.value_);
//    stem_ = other.stem_;
//    branches_ = std::move(other.branches_);
//    return *this;
//  }
//
// public:
//  void clear() {
//    branches_.clear();  // Clear branches
//  }
//  list_node& branch(std::size_t idx) { return branches_[idx]; }
//  branch_list_type& branches() { return branches_; }
//  const branch_list_type& branches() const { return branches_; }
//  value_type& value() { return value_; }
//  const value_type& value() const { return value_; }
//  bool is_leaf() const { return branches_.empty(); }
//  bool is_trunk() const { return !stem_; }
//  const list_node* stem() const { return stem_; }
//  list_node* stem() { return const_cast<list_node*>(stem_); }
//  const list_node* root() const {
//    if (!stem_) return this;
//    return stem_->root();
//  }
//  list_node* root() {
//    if (!stem_) return this;
//    return const_cast<list_node*>(stem_->root());
//  }
//  void push(const T& value) { branches_.emplace_back(value, this); }
//  void push(T&& value) { branches_.emplace_back(std::forward<T>(value), this); }
//  list_node& back_branch() { return branches_.back(); }
//
//  void apply(const std::function<void(value_type&)>& func) {
//    func(value_);
//    if (is_leaf()) return;
//    for (auto& branch : branches_) {
//      branch.apply(func);
//    }
//  }
//  void apply(const std::function<void(const value_type&)>& func) const {
//    func(value_);
//    if (is_leaf()) return;
//    for (const auto& branch : branches_) {
//      branch.apply(func);
//    }
//  }
//  void apply(const std::function<void(cxd::list_node<value_type>&)>& func) {
//    func(*this);
//    if (is_leaf()) return;
//    for (auto& branch : branches_) {
//      branch.apply(func);
//    }
//  }
//  void apply(const std::function<void(const cxd::list_node<value_type>&)>& func) const {
//    func(*this);
//    if (is_leaf()) return;
//    for (const auto& branch : branches_) {
//      branch.apply(func);
//    }
//  }
//
//  void apply_branches(const std::function<void(value_type&)>& func) {
//    if (is_leaf()) return;
//    for (auto& branch : branches_) {
//      branch.apply(func);
//    }
//  }
//  void apply_branches(const std::function<void(const value_type&)>& func) const {
//    if (is_leaf()) return;
//    for (const auto& branch : branches_) {
//      branch.apply(func);
//    }
//  }
//  void apply_branches(const std::function<void(cxd::list_node<value_type>&)>& func) {
//    if (is_leaf()) return;
//    for (auto& branch : branches_) {
//      branch.apply(func);
//    }
//  }
//  void apply_branches(const std::function<void(const cxd::list_node<value_type>&)>& func) const {
//    if (is_leaf()) return;
//    for (const auto& branch : branches_) {
//      branch.apply(func);
//    }
//  }
//
//  value_type* operator->() { return &value_; }
//  const value_type* operator->() const { return &value_; }
//  value_type& operator*() { return value_; }
//  const value_type& operator*() const { return value_; }
//
//  bool operator==(const list_node& other) const noexcept { return value_ == other.value_; }
//  iterator begin() { return iterator(this); }
//  iterator end() { return iterator(nullptr); }
//  const_iterator cbegin() const { return const_iterator(this); }
//  const_iterator cend() const { return const_iterator(nullptr); }
//
// private:
//  T value_{};
//  const list_node* stem_{nullptr};
//  branch_list_type branches_{};
//};

//template <class T>
//struct node_base {
//  const T* node_base_stem__{nullptr};
//  std::vector<T> node_base_branches__{};
//
//  /////////////////////////////////////////////////////////
//  // Properties
//  ////////////////////////////////////////////////////////
//
//  /// Does this node have any branches?
//  bool is_leaf() const noexcept { return branches_.empty(); }
//
//  /// Is this the root-node in the tree?
//  bool is_trunk() const noexcept { return !stem_; }
//
//  /// Get the direct parent of this node. Returns nullptr if no parent exists.
//  const T* stem() const noexcept { return stem_; }
//
//  /// Get the direct parent of this node. Returns nullptr if no parent exists.
//  T* stem() noexcept { return const_cast<T*>(stem_); }
//
//  /// Get the root node of the tree this node belongs to. Returns 'this' if this is the root node.
//  const T* root() const noexcept {
//    if (!stem_) return this;
//    return stem_->root();
//  }
//
//  /// Get the root node of the tree this node belongs to. Returns 'this' if this is the root node.
//  T* root() noexcept {
//    if (!stem_) return this;
//    return const_cast<T*>(stem_->root());
//  }
//
//  /// Get this node's branches.
//  constexpr std::vector<T>& branches() noexcept { return node_base_branches__; }
//
//  /// Get this node's branches.
//  constexpr const std::vector<T>& branches() const noexcept { return node_base_branches__; }
//
//  /////////////////////////////////////////////////////////
//  // Modification
//  ////////////////////////////////////////////////////////
//
//  // Erase this node's branches and their subnodes.
//  constexpr void prune() noexcept {
//    node_base_branches__.clear(); 
//  }
//
//  constexpr void append(const T& value) { node_base_branches__.emplace_back(value, this); }
//
//  constexpr void append(T&& value) { node_base_branches__.emplace_back(std::forward<T>(value), this); }
//
//  // Get the last branch of this node.
//  constexpr T& back() noexcept { return node_base_branches__.back(); }
//
//  // Get the last branch of this node.
//  constexpr T& back() const noexcept { return node_base_branches__.back(); }
//
//  /// Equality operator enabled for all Us equality comparable to T.
//  template <class U>
//    requires std::equality_comparable_with<T, U>
//  constexpr bool operator==(this T&& self, const U& other) noexcept {
//    return self == other;
//  }
//
//  // Apply a function to this node and all its branches in depth first pre-order.
//  constexpr void apply(const std::function<void(T&)>& func) {
//    func(*this);
//    if (this->is_leaf()) return;
//    for (T& br : node_base_branches__) {
//      br.apply(func);
//    }
//  }
//
//  // Apply a function to this node and all its branches in depth first pre-order.
//  constexpr void apply(const std::function<void(const T&)>& func) const {
//    func(*this);
//    if (this->is_leaf()) return;
//    for (const T& br : node_base_branches__) {
//      br.apply(func);
//    }
//  }
//
//  // Apply a function to this node's branches recursivley in depth first pre-order.
//  constexpr void apply_branches(const std::function<void(T&)>& func) {
//    if (this->is_leaf()) return;
//    for (T& br : node_base_branches__) {
//      br.apply(func);
//    }
//  }
//
//  // Apply a function to this node's branches recursivley in depth first pre-order.
//  constexpr void apply_branches(const std::function<void(const T&)>& func) const {
//    if (this->is_leaf()) return;
//    for (const T& br : node_base_branches__) {
//      br.apply(func);
//    }
//  }
//};

template <class T>
struct list_node_base {
  const T* node_base_stem__{nullptr};
  std::list<T> node_base_branches__{};


  /////////////////////////////////////////////////////////
  // Properties
  ////////////////////////////////////////////////////////

  /// Does this node have any branches?
  bool is_leaf() const noexcept { return node_base_branches__.empty(); }

  /// Is this the root-node in the tree?
  bool is_trunk() const noexcept { return !node_base_stem__; }

  /// Get the direct parent of this node. Returns nullptr if no parent exists.
  const T* stem() const noexcept { return node_base_stem__; }

  /// Get the direct parent of this node. Returns nullptr if no parent exists.
  T* stem() noexcept { return const_cast<T*>(node_base_stem__); }

  /// Get the root node of the tree this node belongs to. Returns 'this' if this is the root node.
  const T* root(this const T& self) noexcept {
    if (!self.node_base_stem__) return &self;
    return self.node_base_stem__->root();
  }

  /// Get the root node of the tree this node belongs to. Returns 'this' if this is the root node.
  T* root(this T&& self) noexcept {
    if (!self.node_base_stem__) return &self;
    return const_cast<T*>(self.node_base_stem__->root());
  }

  /// Get this node's branches.
  constexpr std::list<T>& branches() noexcept { return node_base_branches__; }

  /// Get this node's branches.
  constexpr const std::list<T>& branches() const noexcept { return node_base_branches__; }

  /////////////////////////////////////////////////////////
  // Modification
  ////////////////////////////////////////////////////////

  // Erase this node's branches and their subnodes.
  constexpr void prune() noexcept { node_base_branches__.clear(); }

  constexpr void append(const T& value) { 
    node_base_branches__.emplace_back(value);
    node_base_branches__.back().node_base_stem__ = static_cast<const T*>(this);
  }

  constexpr void append(T&& value) {
    node_base_branches__.emplace_back(std::forward<T>(value));
    node_base_branches__.back().node_base_stem__ = static_cast<const T*>(this);
  }

  // Get the last branch of this node.
  constexpr T& back(this T& self) noexcept { return self.node_base_branches__.back(); }

  // Get the last branch of this node.
  constexpr const T& back(this const T& self) noexcept { return self.node_base_branches__.back(); }

  /// Equality operator enabled for all Us equality comparable to T.
  template <class U>
    requires std::equality_comparable_with<T, U>
  constexpr bool operator==(this T&& self, const U& other) noexcept {
    return self == other;
  }

  // Apply a function to this node and all its branches in depth first pre-order.
  constexpr void apply(this T& self,const std::function<void(T&)>& func) {
    func(self);
    if (self.is_leaf()) return;
    for (T& br : self.node_base_branches__) {
      br.apply(func);
    }
  }

  // Apply a function to this node and all its branches in depth first pre-order.
  constexpr void apply(this const T& self, const std::function<void(const T&)>& func) {
    func(self);
    if (self.is_leaf()) return;
    for (const T& br : self.node_base_branches__) {
      br.apply(func);
    }
  }

  // Apply a function to this node's branches recursivley in depth first pre-order.
  constexpr void apply_branches(this T& self, const std::function<void(T&)>& func) {
    if (self.is_leaf()) return;
    for (T& br : self.node_base_branches__) {
      br.apply(func);
    }
  }

  // Apply a function to this node's branches recursivley in depth first pre-order.
  constexpr void apply_branches(this const T& self, const std::function<void(const T&)>& func) {
    if (self.is_leaf()) return;
    for (const T& br : self.node_base_branches__) {
      br.apply(func);
    }
  }
};


//struct MyNode : node_base<MyNode> {
//  int my_val{0};
//  constexpr bool operator==(const MyNode& other) noexcept { return my_val == other.my_val; }
//};
//
//static_assert(MyNode{}.node_base_stem__ == nullptr);
//static_assert(MyNode{}.node_base_branches__.empty());
//static_assert(MyNode{}.branches().empty());
//
//static_assert(MyNode{}.my_val == 0);
//static_assert(MyNode{} == MyNode{});
//static_assert(std::same_as<void, decltype(MyNode{}.apply([](auto&){}))>);
}  // namespace cxd
