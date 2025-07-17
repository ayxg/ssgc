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
#include "cxxx.hpp"
// clang-format on

namespace caf {

// JSON Serialization
namespace jsonlib = nlohmann;
using JsonObj = jsonlib::json;

template <class T>
concept JsonConvertible = requires(const T& t, const JsonObj& o) {
  { T::ToJson(t) } -> std::same_as<JsonObj>;
  { T::FromJson(o) } -> std::same_as<T>;
};

static JsonObj ToJson(const JsonConvertible auto& obj) { return std::decay_t<decltype(obj)>::ToJson(obj); }

static JsonObj ToJson(const ImVec2& obj) { return JsonObj{{obj.x, obj.y}}; }

static void FromJson(ImVec2& obj, const JsonObj& json) { obj = {json.at(0).get<float>(), json.at(1).get<float>()}; }
static JsonObj ToJson(const ImVec4& obj) { return JsonObj{{obj.x, obj.y, obj.z, obj.w}}; }

static void FromJson(ImVec4& obj, const JsonObj& json) {
  obj = {json.at(0).get<float>(), json.at(1).get<float>(), json.at(2).get<float>(), json.at(3).get<float>()};
}

static auto FromJson(const JsonConvertible auto& obj, const JsonObj& json) -> std::decay_t<decltype(obj)> {
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
