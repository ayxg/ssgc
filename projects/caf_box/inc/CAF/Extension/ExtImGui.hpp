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
/// @brief [HEADER] Dear ImGui extension for caf
/// "dear imgui: wrappers for C++ standard library (STL) types (std::string, etc.)
/// This is also an example of how you may wrap your own similar types.
/// Changelog:
/// - v0.10: Initial version. Added InputText() / InputTextMultiline() calls with
/// std::string
/// See more C++ related extension (fmt, RAII, syntaxis sugar) on Wiki:
///   https://github.com/ocornut/imgui/wiki/Useful-Extensions#cness"
///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup coreappfw
/// @{

// clang-format off
#pragma once
#include <string>

#include "CAF/PreConfig.hpp"
#include "imgui-SFML.h"
#include "imgui.h"
#include "CAF/System/Windows.hpp"
// clang-format on


#ifdef CAF_ENABLE_EXTENSION_DEARIMGUI

namespace ImGui {
// ImGui::InputText() with std::string
// Because text input needs dynamic resizing, we need to setup a callback to grow the capacity
IMGUI_API bool InputText(const char* label, std::string* str, ImGuiInputTextFlags flags = 0,
                         ImGuiInputTextCallback callback = nullptr, void* user_data = nullptr);

IMGUI_API bool InputTextMultiline(const char* label, std::string* str, const ImVec2& size = ImVec2(0, 0),
                                  ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = nullptr,
                                  void* user_data = nullptr);

IMGUI_API bool InputTextWithHint(const char* label, const char* hint, std::string* str, ImGuiInputTextFlags flags = 0,
                                 ImGuiInputTextCallback callback = nullptr, void* user_data = nullptr);
}

namespace caf::imgui {

static void ProcessEvent(caf::sys::Windows::Node* wnd, const sf::Event& event) {
  ImGui::SFML::ProcessEvent(wnd->GetUnderlying(), event);
}

static bool Init(caf::sys::Windows::Node* wnd, bool load_default_font = true) {
  return ImGui::SFML::Init(wnd->GetUnderlying(), load_default_font);
}

static void Shutdown(caf::sys::Windows::Node* wnd) {
  ImGui::SFML::Shutdown(wnd->GetUnderlying());
}

static void Update(caf::sys::Windows::Node* wnd, const sf::Time& dt) {
  ImGui::SFML::Update(wnd->GetUnderlying(), dt);
}

static void Render(caf::sys::Windows::Node* wnd) { ImGui::SFML::Render(wnd->GetUnderlying()); }

static void SetCurrentWindow(caf::sys::Windows::Node* wnd) {
  ImGui::SFML::SetCurrentWindow(wnd->GetUnderlying());
}

}  // namespace CAF

#endif  // CAF_ENABLE_EXTENSION_DEARIMGUI

/// @} // end of coreappfw

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: [CAF] Core Application Framework
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
// @created: 2025/07/14
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
