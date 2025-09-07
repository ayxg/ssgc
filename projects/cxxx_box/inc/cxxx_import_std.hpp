///////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the Apache License, Version 2.0(the "License");
///////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language Environment
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup cppmodule2_cppextended
/// @brief Project-Wide C++ Standard Library Includes
///////////////////////////////////////////////////////////////////////////////

/// @addtogroup cppmodule2_cppextended
/// @{
#ifndef HEADER_GUARD_CALE_EXTENDED_CPP_STANDARD_IMPORT_STD_H
#define HEADER_GUARD_CALE_EXTENDED_CPP_STANDARD_IMPORT_STD_H

// Project-Wide C++ Standard Library Includes

#include <algorithm>  // std::move, std::forward, std::get, std::ref, std::cref, std::any_of
#include <array>  // std::array
#include <atomic>
#include <cmath>
#include <chrono>
#include <cassert>
#include <concepts>
#include <cstddef>  // std::size_t
#include <cstdlib>  // numeric string conversions
#include <deque>
#include <exception>
#include <filesystem>
#include <fstream>
#include <functional>        // std::reference_wrapper
#include <initializer_list>  // std::initializer_list
#include <iostream>
#include <iterator>  // reverse_iterator
#include <limits>    // std::numeric_limits
#include <list>      // std::list
#include <map>       // std::map
#include <memory>    // std::unique_ptr , std::shared_ptr
#include <optional>
#include <set>
#include <source_location>
#include <span>  // std::span
#include <sstream>
#include <stack>
#include <format>
#include <stdexcept>
#include <string>  // std::string
#include <tuple>
#include <type_traits>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>  // std::unordered_map
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>  // std::vector
#include <expected>
#include <span>

// Common typedefs from std to bring into global namespace.
namespace cxx {
namespace stdfs = std::filesystem;
// Typedefs for some literal types/pointers.
using cstring = const char*;
using cwstring = const wchar_t*;
using void_ptr = void*;
using const_void_ptr = const void*;
}  // namespace cxx


#endif HEADER_GUARD_CALE_EXTENDED_CPP_STANDARD_IMPORT_STD_H
/// @} // end of cppmodule2_cppextended
///////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language Environment
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
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
///////////////////////////////////////////////////////////////////////////////
