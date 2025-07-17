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
/// @brief [HEADER] Core App Framework macro pre-configuration.
/// @TODO : bettter brief here outlining details and declared macros next time you look! :(
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup coreappfw
/// @{

#pragma once

///////////////////////////////////////////////////////////
// Identify host operating system.
// Define CAF_HOST_OS_[name] macro.
// Current supported operating systems:
// - Windows, Linux, MacOS, iOS, Android, FreeBSD, OpenBSD, NetBSD
// - AppleUnknown, UnixUnknown, Unknown are unsupported
// @see https://sourceforge.net/p/predef/wiki/Home/
///////////////////////////////////////////////////////////

#if defined(_WIN32)  // Windows
#define CAF_HOST_OS_WINDOWS
#elif defined(__APPLE__) && defined(__MACH__)  // Apple based os
#include "TargetConditionals.h"                // Apple specific header

#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
// iOS
#define CAF_HOST_OS_IOS
#elif TARGET_OS_MAC
// macOS
#define CAF_HOST_OS_MACOS
#else
// Unsupported Apple system
#define CAF_HOST_OS_APPLEUNKNOWN
#endif
#elif defined(__unix__)  // UNIX based os
// Android
#if defined(__ANDROID__)
#define CAF_HOST_OS_ANDROID
// Linux
#elif defined(__linux__)
#define CAF_HOST_OS_LINUX
// FreeBSD
#elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
#define CAF_HOST_OS_FREEBSD
// OpenBSD
#elif defined(__OpenBSD__)
#define CAF_HOST_OS_OPENBSD
// NetBSD
#elif defined(__NetBSD__)
#define CAF_HOST_OS_NETBSD
// Unsupported UNIX system
#else
#define CAF_HOST_OS_UNIXUNKNOWN
#error This UNIX operating system is not supported by CAF library
#endif
#else
// Unsupported system
#define CAF_HOST_OS_UNKNOWN
#error This operating system is not supported by CAF library
#endif

////////////////////////////////////////////////////////////
// Define a portable debug macro
////////////////////////////////////////////////////////////
#if !defined(NDEBUG)
#define CAF_DEBUG
#endif

////////////////////////////////////////////////////////////
// Define helpers to create portable import / export macros for each module
////////////////////////////////////////////////////////////
#if defined(CAF_SHARED)

#if defined(CAF_HOST_OS_WINDOWS)

// Windows compilers need specific (and different) keywords for export and
// import
#define CAF_API_EXPORT __declspec(dllexport)
#define CAF_API_IMPORT __declspec(dllimport)

// For Visual C++ compilers, we also need to turn off this annoying C4251
// warning
#ifdef _MSC_VER

#pragma warning(disable : 4251)

#endif

#else  // Linux, FreeBSD, macOS

#define CAF_API_EXPORT __attribute__((__visibility__("default")))
#define CAF_API_IMPORT __attribute__((__visibility__("default")))

#endif

#else

// Static build doesn't need import/export macros
#define CAF_API_EXPORT
#define CAF_API_IMPORT

#endif

// Use export or import based on [project_name]_EXPORTS macro on windows.
#ifdef CAF_HOST_OS_WINDOWS
#ifdef CAF_EXPORTS
#define CAF_API CAF_API_EXPORT
#else
#define CAF_API CAF_API_IMPORT
#endif
#else  // Linux, FreeBSD, macOS same for export/import
#define CAF_API CAF_API_EXPORT
#endif

///////////////////////////////////////////////////////////
// Handle OS-specific definitions
///////////////////////////////////////////////////////////

// min/max standard function conflict with windows.h api,
// and causes knows problems with gdiplus Windows headers files.
// Define the NOMINMAX macro to instruct WinDef.h not to define
// the min/max macros.
// @see http://www.suodenjoki.dk/us/archive/2010/min-max.htm
#if defined(CAF_HOST_OS_WINDOWS)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <algorithm>
using std::max;
using std::min;
#endif

///////////////////////////////////////////////////////////
/// SFML Backend Specific : encourages discrete GPU usage.
/// Code and docs from SFML/GpuPreference.hpp
/// https://github.com/SFML/SFML/blob/master/include/SFML/GpuPreference.hpp
///
/// In order to inform the Nvidia/AMD driver that an SFML
/// application could benefit from using the more powerful
/// discrete GPU, special symbols have to be publicly
/// exported from the final executable.
///
/// SFML defines a helper macro to easily do this.
///
/// Place `SFML_DEFINE_DISCRETE_GPU_PREFERENCE` in the
/// global scope of a source file that will be linked into
/// the final executable. Typically it is best to place it
/// where the main function is also defined.
///////////////////////////////////////////////////////////
#if defined(CAF_HOST_OS_WINDOWS)

#define CAF_DEFINE_DISCRETE_GPU_PREFERENCE                                \
  extern "C" __declspec(dllexport) unsigned long NvOptimusEnablement = 1; \
  extern "C" __declspec(dllexport) unsigned long AmdPowerXpressRequestHighPerformance = 1;

#else

#define CAF_DEFINE_DISCRETE_GPU_PREFERENCE

#endif

///////////////////////////////////////////////////////////
/// Determine the backend context for the application.
/// Currently we support only [OpenGl-SFML-ImGui]
/// Defines:
///   - CAF_CONTEXT_BACKEND_[backend-apis]
///   - CAF_GFX_API_[api] : Graphics API
///   - CAF_SYS_API_[api] : Windowing/System IO API
///   - CAF_GUI_API_[api] : User Interface API
///////////////////////////////////////////////////////////

#define CAF_CONTEXT_BACKEND_OPENGL_SFML_IMGUI

// ImGui-SFML backend
#ifdef CAF_CONTEXT_BACKEND_OPENGL_SFML_IMGUI
#define CAF_GFX_API_OPENGL
#define CAF_SYS_API_SFML
#define CAF_GUI_API_IMGUI

// only include the required SFML modules
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

// On iOS SFML must override the main.
#ifdef CAF_HOST_OS_IOS
#include <SFML/Main.hpp>
#endif  //  CAF_HOST_OS_IOS

// Use the ImGui-SFML bindings.
#include "imgui-SFML.h"
#include "imgui.h"
// Add interface to allow use of std::string with ImGui
#define CAF_ENABLE_EXTENSION_DEARIMGUI 1

#endif

///////////////////////////////////////////////////////////
/// Include a json parser library
/// Currently we support only [nlohmann-json]
///////////////////////////////////////////////////////////
#include <nlohmann/json.hpp>

/// @} // end of coreappfw

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: [CAF] Core Application Framework
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
// @created: 2025/05/09
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
