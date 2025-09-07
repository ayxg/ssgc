///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2025 Anton Yashchenko
// Licensed under the Apache License, Version 2.0(the "License");
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: [CAF] Core Application Framework
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup coreappfw
/// @brief [HEADER] 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup coreappfw
/// @{

// clang-format off
#pragma once
#include "CAF/PreConfig.hpp"
#include <nlohmann/json.hpp>
#include "cxxx.hpp"

// clang-format on

namespace caf {

// JSON Serialization
namespace jsonlib = nlohmann;
using JsonObject = jsonlib::json;

template <class T>
concept JsonConvertible = requires(const T& t, const JsonObject& o) {
  { T::ToJson(t) } -> std::same_as<JsonObject>;
  { T::FromJson(o) } -> std::same_as<T>;
};

static JsonObject ToJson(const JsonConvertible auto& obj) { return std::decay_t<decltype(obj)>::ToJson(obj); }

static auto FromJson(const JsonConvertible auto& obj, const JsonObject& json) -> std::decay_t<decltype(obj)> {
  return std::decay_t<decltype(obj)>::FromJson(json);
}
}  // namespace caf

/// @} // end of coreappfw

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: [CAF] Core Application Framework
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
// @created: 2025/05/15
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2025 Anton Yashchenko
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
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
