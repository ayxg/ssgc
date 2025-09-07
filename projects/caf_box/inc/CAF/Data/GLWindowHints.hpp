///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2025 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: [CAF] Core Application Framework
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup core_app_framework
/// @brief [HEADER]
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup core_app_framework
/// @{

// clang-format off
#pragma once
#include "cxxx.hpp"
#include "CAF/PreConfig.hpp"
// clang-format on

namespace caf {

/// Standard set of parameters passable to an OpenGL capable window upon creation. Its up to the 'Window' class
/// implementation how to use these parameters - based on the underlying window type eg. sf::Window, GLFW, SDL, etc.
struct GLWindowHints {
  std::optional<int> RequestFocus{std::nullopt};      // Request focus
  std::optional<int> Hidden{std::nullopt};            // Window is hidden
  std::optional<int> Fullscreen{std::nullopt};        // Fullscreen window
  std::optional<int> NoTitleBar{std::nullopt};        // No title bar
  std::optional<int> NoResize{std::nullopt};          // Window is resizable
  std::optional<int> NoCloseButton{std::nullopt};     // Show close button
  std::optional<int> InitialWidth{std::nullopt};      // Initial window width
  std::optional<int> InitialHeight{std::nullopt};     // Initial window height
  std::optional<int> InitialPositionX{std::nullopt};  // Initial window width
  std::optional<int> InitialPositionY{std::nullopt};  // Initial window height
  std::optional<int> FrameLimit{std::nullopt};        // default is FalseInt; which means no limit
  std::optional<int> EnableVsync{std::nullopt};       // Enable vertical sync

  std::optional<int> GlDepthBits{std::nullopt};
  std::optional<int> GlAntialiasingLevel{std::nullopt};
  std::optional<int> GlStencilBits{std::nullopt};
  std::optional<int> GlMajorVersion{std::nullopt};
  std::optional<int> GlMinorVersion{std::nullopt};
  std::optional<int> GlAttributeFlags{std::nullopt};  // One of sf::ContextSettings::AttributeFlags
  std::optional<int> GlsRgbCapable{std::nullopt};

  std::optional<std::string_view> InitialTitle{std::nullopt};
};
}  // namespace caf

/// @} // end of core_app_framework

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: [CAF] Core Application Framework
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
// @created: 2025/07/25
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
