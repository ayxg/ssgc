//#pragma once
//#include <cassert>
//#include <functional>
//#include <iostream>
//#include <map>
//#include <string>
//#include <typeindex>
//#include <typeinfo>
//#include <vector>
//namespace cxx {
//class MethodMap {
//  typedef void (*void_func_t)(void);
//
//  std::map<std::size_t, std::pair<void_func_t, std::type_index>> func_map_;
//
// public:
//  MethodMap() = default;
//  // Initialize the map with a list of indexes and functions.
//  MethodMap(
//      std::initializer_list<std::pair<std::size_t, void_func_t>> init_list) {
//    for (auto& elem : init_list) {
//      func_map_.insert(std::make_pair(
//          elem.first,
//          std::make_pair(elem.second, std::type_index(typeid(elem.second)))));
//    }
//  }
//
//  // copy is not allowed
//  MethodMap(const MethodMap&) = delete;
//  MethodMap& operator=(const MethodMap&) = delete;
//
//  MethodMap(MethodMap&&) = default;
//  MethodMap& operator=(MethodMap&&) = default;
//
//  // Insert a function into the map at index. Overwrites if already exists.
//  template <typename T>
//  constexpr void insert(std::size_t func_index, T func) {
//    auto func_type_index = std::type_index(typeid(func));
//    func_map_.insert(std::make_pair(
//        func_index, std::make_pair((void_func_t)func, func_type_index)));
//  }
//
//  // Returns the std::type_index of the function at index.
//  // Returns void typeid if not found.
//  std::type_index getTypeIndex(std::size_t func_index) {
//    auto mapIter = func_map_.find(func_index);
//    if (mapIter == func_map_.end()) {
//      return std::type_index(typeid(void));
//    }
//    return mapIter->second.second;
//  }
//
//  // Search for a function in the map and call it.
//  template <typename T, typename... Args>
//  T searchAndCall(std::size_t func_index, Args&&... args) {
//    auto mapIter = func_map_.find(func_index);
//    /*chk if not end*/
//    auto mapVal = mapIter->second;
//
//    auto typeCastedFun = reinterpret_cast<T (*)(Args...)>(mapVal.first);
//    // auto typeCastedFun = (T(*)(Args ...))(mapVal.first);
//
//    // compare the types is equal or not
//    assert(mapVal.second == std::type_index(typeid(typeCastedFun)));
//    return typeCastedFun(std::forward<Args>(args)...);
//  }
//};
//};  // namespace cxx