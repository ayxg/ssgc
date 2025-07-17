///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2025 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: [CAF] Core Application Framework
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup coreappfw
/// @brief [HEADER] Dear ImGui theme object
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
#include "CAF/Tool/JsonObject.hpp"
// clang-format on

#ifdef CAF_ENABLE_EXTENSION_DEARIMGUI

namespace caf::imgui {

/// Object representation of a UI theme.
class Theme {
  using StyleT = ImGuiStyle;
  /// This structure is huge so better store it on the heap. After applying the theme. Object can be destroyed.
  std::unique_ptr<StyleT> style{nullptr};
  std::string font_path{""};  ///< Path to a TTF font file. If empty, default font will be used.
  std::string font_name{""};
  int font_size{0};  ///< Font size. If zero, default font size will be used.
 private:
  /// You should only create themes using static creation methods - so that defaults are set correctly.
  Theme() = default;

 public:
  Theme(const StyleT& s) : style(std::make_unique<StyleT>(s)), font_path("") {}

  /// Returns true if theme is empty (not initialized). Uninitialized themes may be a result of loading a json theme
  /// from file that does not exist or is empty.
  bool IsEmpty() const { return !!(style); }

  void Clear() noexcept;

  /// Apply the theme to the target UI. Retrieve the current style using ImGui::GetStyle().
  void Apply(StyleT& curr_style) noexcept;

  void Copy(const StyleT& curr_style) noexcept;

  static caf::JsonObj ToJson(const Theme& obj) noexcept;

  static Theme FromJson(const caf::JsonObj& obj) noexcept;

  // Generates default underlying UI framework theme.
  static Theme Default() noexcept;

  // Generates default dark underlying UI framework theme.
  static Theme DefaultDark() noexcept;

  // Generates default light underlying UI framework theme.
  static Theme DefaultLight() noexcept;

  // Generates default CIDR theme. (light green)
  static Theme DefaultCidr() noexcept;
};
}  // namespace caf::imgui

#endif  // CAF_ENABLE_EXTENSION_DEARIMGUI

/// @} // end of coreappfw

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: [CAF] Core Application Framework
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
// @created: 2025/07/16
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2025 Anton Yashchenko
//
// Licensed under the GNU Affero General Public License, Version 3.
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
