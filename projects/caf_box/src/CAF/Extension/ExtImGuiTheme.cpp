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
/// @brief [SOURCE] Dear ImGui theme object
///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup coreappfw
/// @{

// clang-format off
#pragma once
#include "CAF/Extension/ExtImGuiTheme.hpp"

#include <string>

#include "CAF/PreConfig.hpp"
#include "imgui-SFML.h"
#include "imgui.h"
#include "CAF/System/Windows.hpp"
#include "CAF/Tool/JsonObject.hpp"
#include "CAF/Extension/ExtImGui.hpp"
// clang-format on

#ifdef CAF_ENABLE_EXTENSION_DEARIMGUI

namespace caf::imgui {
caf::JsonObject Theme::ToJson(const Theme& obj) noexcept {
  caf::JsonObject j{};
  if (obj.style) {
    // Settings
    j["Alpha"] = obj.style->Alpha;
    j["DisabledAlpha"] = obj.style->DisabledAlpha;
    j["WindowPadding"] = caf::ToJson(obj.style->WindowPadding);
    j["WindowRounding"] = obj.style->WindowRounding;
    j["WindowBorderSize"] = obj.style->WindowBorderSize;
    j["WindowMinSize"] = caf::ToJson(obj.style->WindowMinSize);
    j["WindowTitleAlign"] = caf::ToJson(obj.style->WindowTitleAlign);
    j["WindowMenuButtonPosition"] = static_cast<int>(obj.style->WindowMenuButtonPosition);
    j["ChildRounding"] = obj.style->ChildRounding;
    j["ChildBorderSize"] = obj.style->ChildBorderSize;
    j["PopupRounding"] = obj.style->PopupRounding;
    j["PopupBorderSize"] = obj.style->PopupBorderSize;
    j["FramePadding"] = caf::ToJson(obj.style->FramePadding);
    j["FrameRounding"] = obj.style->FrameRounding;
    j["FrameBorderSize"] = obj.style->FrameBorderSize;
    j["ItemSpacing"] = caf::ToJson(obj.style->ItemSpacing);
    j["ItemInnerSpacing"] = caf::ToJson(obj.style->ItemInnerSpacing);
    j["CellPadding"] = caf::ToJson(obj.style->CellPadding);
    j["TouchExtraPadding"] = caf::ToJson(obj.style->TouchExtraPadding);
    j["IndentSpacing"] = obj.style->IndentSpacing;
    j["ColumnsMinSpacing"] = obj.style->ColumnsMinSpacing;
    j["ScrollbarSize"] = obj.style->ScrollbarSize;
    j["ScrollbarRounding"] = obj.style->ScrollbarRounding;
    j["GrabMinSize"] = obj.style->GrabMinSize;
    j["GrabRounding"] = obj.style->GrabRounding;
    j["LogSliderDeadzone"] = obj.style->LogSliderDeadzone;
    j["TabRounding"] = obj.style->TabRounding;
    j["TabBorderSize"] = obj.style->TabBorderSize;
    j["TabMinWidthForCloseButton"] = obj.style->TabMinWidthForCloseButton;
    j["TabBarBorderSize"] = obj.style->TabBarBorderSize;
    j["TabBarOverlineSize"] = obj.style->TabBarOverlineSize;
    j["TableAngledHeadersAngle"] = obj.style->TableAngledHeadersAngle;
    j["TableAngledHeadersTextAlign"] = caf::ToJson(obj.style->TableAngledHeadersTextAlign);
    j["ColorButtonPosition"] = static_cast<int>(obj.style->ColorButtonPosition);
    j["ButtonTextAlign"] = caf::ToJson(obj.style->ButtonTextAlign);
    j["SelectableTextAlign"] = caf::ToJson(obj.style->SelectableTextAlign);
    j["SeparatorTextBorderSize"] = obj.style->SeparatorTextBorderSize;
    j["SeparatorTextAlign"] = caf::ToJson(obj.style->SeparatorTextAlign);
    j["SeparatorTextPadding"] = caf::ToJson(obj.style->SeparatorTextPadding);
    j["DisplayWindowPadding"] = caf::ToJson(obj.style->DisplayWindowPadding);
    j["DisplaySafeAreaPadding"] = caf::ToJson(obj.style->DisplaySafeAreaPadding);
    j["MouseCursorScale"] = obj.style->MouseCursorScale;
    j["AntiAliasedLines"] = obj.style->AntiAliasedLines;
    j["AntiAliasedLinesUseTex"] = obj.style->AntiAliasedLinesUseTex;
    j["AntiAliasedFill"] = obj.style->AntiAliasedFill;
    j["CurveTessellationTol"] = obj.style->CurveTessellationTol;
    j["CircleTessellationMaxError"] = obj.style->CircleTessellationMaxError;
    // Colors
    for (int i = 0; i < ImGuiCol_COUNT; ++i) {
      j["Colors"][std::string{ImGui::GetStyleColorName(static_cast<ImGuiCol>(i))}] =
          caf::ToJson(obj.style->Colors[i]);
    }
  }
  // else export empty object
  return j;
};

Theme Theme::FromJson(const caf::JsonObject& obj) noexcept {
  if (obj.empty()) return Theme{};  // Error. Nothing to parse.
  Theme t{Theme::Default()};        // Create default in case any params are missing.
  // Settings
  t.style->Alpha = obj.value("Alpha", t.style->Alpha);
  t.style->DisabledAlpha = obj.value("DisabledAlpha", t.style->DisabledAlpha);
  if (obj.contains("WindowPadding")) caf::FromJson(t.style->WindowPadding, obj["WindowPadding"]);
  t.style->WindowRounding = obj.value("WindowRounding", t.style->WindowRounding);
  t.style->WindowBorderSize = obj.value("WindowBorderSize", t.style->WindowBorderSize);
  if (obj.contains("WindowMinSize")) caf::FromJson(t.style->WindowMinSize, obj["WindowMinSize"]);
  if (obj.contains("WindowTitleAlign")) caf::FromJson(t.style->WindowTitleAlign, obj["WindowTitleAlign"]);
  t.style->WindowMenuButtonPosition =
      static_cast<ImGuiDir>(obj.value("WindowMenuButtonPosition", static_cast<int>(t.style->WindowMenuButtonPosition)));
  t.style->ChildRounding = obj.value("ChildRounding", t.style->ChildRounding);
  t.style->ChildBorderSize = obj.value("ChildBorderSize", t.style->ChildBorderSize);
  t.style->PopupRounding = obj.value("PopupRounding", t.style->PopupRounding);
  t.style->PopupBorderSize = obj.value("PopupBorderSize", t.style->PopupBorderSize);
  if (obj.contains("FramePadding")) caf::FromJson(t.style->FramePadding, obj["FramePadding"]);
  t.style->FrameRounding = obj.value("FrameRounding", t.style->FrameRounding);
  t.style->FrameBorderSize = obj.value("FrameBorderSize", t.style->FrameBorderSize);
  if (obj.contains("ItemSpacing")) caf::FromJson(t.style->ItemSpacing, obj["ItemSpacing"]);
  if (obj.contains("ItemInnerSpacing")) caf::FromJson(t.style->ItemSpacing, obj["ItemInnerSpacing"]);
  if (obj.contains("CellPadding")) caf::FromJson(t.style->CellPadding, obj["CellPadding"]);
  if (obj.contains("TouchExtraPadding")) caf::FromJson(t.style->TouchExtraPadding, obj["TouchExtraPadding"]);
  t.style->IndentSpacing = obj.value("IndentSpacing", t.style->IndentSpacing);
  t.style->ColumnsMinSpacing = obj.value("ColumnsMinSpacing", t.style->ColumnsMinSpacing);
  t.style->ScrollbarSize = obj.value("ScrollbarSize", t.style->ScrollbarSize);
  t.style->ScrollbarRounding = obj.value("ScrollbarRounding", t.style->ScrollbarRounding);
  t.style->GrabMinSize = obj.value("GrabMinSize", t.style->GrabMinSize);
  t.style->GrabRounding = obj.value("GrabRounding", t.style->GrabRounding);
  t.style->LogSliderDeadzone = obj.value("LogSliderDeadzone", t.style->LogSliderDeadzone);
  t.style->TabRounding = obj.value("TabRounding", t.style->TabRounding);
  t.style->TabBorderSize = obj.value("TabBorderSize", t.style->TabBorderSize);
  t.style->TabMinWidthForCloseButton = obj.value("TabMinWidthForCloseButton", t.style->TabMinWidthForCloseButton);
  t.style->TabBarBorderSize = obj.value("TabBarBorderSize", t.style->TabBarBorderSize);
  t.style->TabBarOverlineSize = obj.value("TabBarOverlineSize", t.style->TabBarOverlineSize);
  t.style->TableAngledHeadersAngle = obj.value("TableAngledHeadersAngle", t.style->TableAngledHeadersAngle);
  if (obj.contains("TableAngledHeadersTextAlign"))
    caf::FromJson(t.style->TableAngledHeadersTextAlign, obj["TableAngledHeadersTextAlign"]);
  t.style->ColorButtonPosition =
      static_cast<ImGuiDir>(obj.value("ColorButtonPosition", static_cast<int>(t.style->ColorButtonPosition)));
  if (obj.contains("ButtonTextAlign")) caf::FromJson(t.style->ButtonTextAlign, obj["ButtonTextAlign"]);
  if (obj.contains("SelectableTextAlign")) caf::FromJson(t.style->SelectableTextAlign, obj["SelectableTextAlign"]);
  t.style->SeparatorTextBorderSize = obj.value("SeparatorTextBorderSize", t.style->SeparatorTextBorderSize);
  if (obj.contains("SeparatorTextAlign")) caf::FromJson(t.style->SeparatorTextAlign, obj["SeparatorTextAlign"]);
  if (obj.contains("SeparatorTextPadding"))
    caf::FromJson(t.style->SeparatorTextPadding, obj["SeparatorTextPadding"]);
  if (obj.contains("DisplayWindowPadding"))
    caf::FromJson(t.style->DisplayWindowPadding, obj["DisplayWindowPadding"]);
  if (obj.contains("DisplaySafeAreaPadding"))
    caf::FromJson(t.style->DisplaySafeAreaPadding, obj["DisplaySafeAreaPadding"]);
  t.style->MouseCursorScale = obj.value("MouseCursorScale", t.style->MouseCursorScale);
  t.style->AntiAliasedLines = obj.value("AntiAliasedLines", t.style->AntiAliasedLines);
  t.style->AntiAliasedLinesUseTex = obj.value("AntiAliasedLinesUseTex", t.style->AntiAliasedLinesUseTex);
  t.style->AntiAliasedFill = obj.value("AntiAliasedFill", t.style->AntiAliasedFill);
  t.style->CurveTessellationTol = obj.value("CurveTessellationTol", t.style->CurveTessellationTol);
  t.style->CircleTessellationMaxError = obj.value("CircleTessellationMaxError", t.style->CircleTessellationMaxError);
  return t;
};

void Theme::Apply(StyleType& curr_style) noexcept {
  //  Apply font, failure will revert to UI framework default font.
  if (!font_path.empty() && std::filesystem::exists(font_path)) {
    ImGuiIO& io = ImGui::GetIO();
    auto font_exists = std::find_if(io.Fonts->Fonts.begin(), io.Fonts->Fonts.end(),
                                    [&](const auto& font) { return font->ConfigData->Name == font_name; });
    // io.Fonts->Clear(); should we do this???
    if (font_exists != io.Fonts->Fonts.end()) {
      io.FontDefault = *font_exists;
      ImGui::SFML::UpdateFontTexture();
    } else {
      ImFontConfig cfg;
      // Set 8x8 Oversampling so that manual font scaling works well.
      cfg.OversampleH = 8;
      cfg.OversampleV = 8;
      io.Fonts->AddFontFromFileTTF(font_path.c_str(), font_size == 0 ? 12 : font_size, &cfg);
      font_name = io.Fonts->Fonts.back()->ConfigData->Name;  // Save the imgui generated name for next time.
      io.FontDefault = io.Fonts->Fonts.back();
      ImGui::SFML::UpdateFontTexture();
    }
  }

  // Apply style if it exists.
  if (style) {
    ImGuiStyle& curr_style = ImGui::GetStyle();
    curr_style.Alpha = style->Alpha;
    curr_style.DisabledAlpha = style->DisabledAlpha;
    curr_style.WindowPadding = style->WindowPadding;
    curr_style.WindowRounding = style->WindowRounding;
    curr_style.WindowBorderSize = style->WindowBorderSize;
    curr_style.WindowMinSize = style->WindowMinSize;
    curr_style.WindowTitleAlign = style->WindowTitleAlign;
    curr_style.WindowMenuButtonPosition = style->WindowMenuButtonPosition;
    curr_style.ChildRounding = style->ChildRounding;
    curr_style.ChildBorderSize = style->ChildBorderSize;
    curr_style.PopupRounding = style->PopupRounding;
    curr_style.PopupBorderSize = style->PopupBorderSize;
    curr_style.FramePadding = style->FramePadding;
    curr_style.FrameRounding = style->FrameRounding;
    curr_style.FrameBorderSize = style->FrameBorderSize;
    curr_style.ItemSpacing = style->ItemSpacing;
    curr_style.ItemInnerSpacing = style->ItemInnerSpacing;
    curr_style.CellPadding = style->CellPadding;
    curr_style.TouchExtraPadding = style->TouchExtraPadding;
    curr_style.IndentSpacing = style->IndentSpacing;
    curr_style.ColumnsMinSpacing = style->ColumnsMinSpacing;
    curr_style.ScrollbarSize = style->ScrollbarSize;
    curr_style.ScrollbarRounding = style->ScrollbarRounding;
    curr_style.GrabMinSize = style->GrabMinSize;
    curr_style.GrabRounding = style->GrabRounding;
    curr_style.LogSliderDeadzone = style->LogSliderDeadzone;
    curr_style.TabRounding = style->TabRounding;
    curr_style.TabBorderSize = style->TabBorderSize;
    curr_style.TabMinWidthForCloseButton = style->TabMinWidthForCloseButton;
    curr_style.TabBarBorderSize = style->TabBarBorderSize;
    curr_style.TabBarOverlineSize = style->TabBarOverlineSize;
    curr_style.TableAngledHeadersAngle = style->TableAngledHeadersAngle;
    curr_style.TableAngledHeadersTextAlign = style->TableAngledHeadersTextAlign;
    curr_style.ColorButtonPosition = style->ColorButtonPosition;
    curr_style.ButtonTextAlign = style->ButtonTextAlign;
    curr_style.SelectableTextAlign = style->SelectableTextAlign;
    curr_style.SeparatorTextBorderSize = style->SeparatorTextBorderSize;
    curr_style.SeparatorTextAlign = style->SeparatorTextAlign;
    curr_style.SeparatorTextPadding = style->SeparatorTextPadding;
    curr_style.DisplayWindowPadding = style->DisplayWindowPadding;
    curr_style.DisplaySafeAreaPadding = style->DisplaySafeAreaPadding;
    curr_style.MouseCursorScale = style->MouseCursorScale;
    curr_style.AntiAliasedLines = style->AntiAliasedLines;
    curr_style.AntiAliasedLinesUseTex = style->AntiAliasedLinesUseTex;
    curr_style.AntiAliasedFill = style->AntiAliasedFill;
    curr_style.CurveTessellationTol = style->CurveTessellationTol;
    curr_style.CircleTessellationMaxError = style->CircleTessellationMaxError;
    // Colors
    for (int i = 0; i < ImGuiCol_COUNT; ++i) {
      curr_style.Colors[i] = style->Colors[i];
    }
  }
  // Apply default
  else
    ImGui::StyleColorsDark(&ImGui::GetStyle());
}

// Clears the stored style data. Do not apply after clearing.
void Theme::Clear() noexcept { style.reset(); }

void Theme::Copy(const StyleType& other) noexcept {
  if (style)
    *style = other;
  else
    style = std::make_unique<StyleType>(other);
}

void Theme::Copy(const Theme& other) noexcept {
  if (style)
    *style = *other.style;
  else
    style = std::make_unique<StyleType>(*other.style);
  font_path = other.font_path;
  font_name = other.font_name;
  font_size = other.font_size;
}

// Generates default underlying UI framework theme.
Theme Theme::Default() noexcept {
  Theme t{};
  t.style = std::make_unique<StyleType>();
  ImGui::StyleColorsClassic(t.style.get());
  return t;
};

// Generates default underlying UI framework theme.
Theme Theme::DefaultDark() noexcept {
  Theme t{};
  t.style = std::make_unique<StyleType>();
  ImGui::StyleColorsDark(t.style.get());
  return t;
};

// Generates default underlying UI framework theme.
Theme Theme::DefaultLight() noexcept {
  Theme t{};
  t.style = std::make_unique<StyleType>();
  ImGui::StyleColorsLight(t.style.get());
  return t;
};

// Generates default CIDR theme. (Light)
Theme Theme::DefaultCidr() noexcept {
  Theme t{Theme::DefaultLight()};
  t.font_path = "font/DroidSans/DroidSans.ttf";
  t.font_size = 15.f;

  // Rounding
  t.style->WindowRounding = 1.0f;
  t.style->ChildRounding = 1.0f;
  t.style->PopupRounding = 1.0f;
  t.style->FrameRounding = 1.0f;
  t.style->ScrollbarRounding = 1.0f;
  t.style->GrabRounding = 1.0f;
  t.style->TabRounding = 1.0f;

  // Borders
  t.style->WindowBorderSize = 1.0f;
  t.style->ChildBorderSize = 1.0f;
  t.style->PopupBorderSize = 1.0f;
  t.style->FrameBorderSize = 1.0f;
  t.style->TabBorderSize = 1.0f;
  t.style->TabBarBorderSize = 1.0f;
  t.style->TabBarOverlineSize = 2.0f;

  // Colors (Generated code)
  ImVec4* colors = t.style->Colors;
  colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
  colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
  colors[ImGuiCol_WindowBg] = ImVec4(0.98f, 0.95f, 0.94f, 1.00f);
  colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
  colors[ImGuiCol_PopupBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.98f);
  colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.30f);
  colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.04f);
  colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.85f, 0.65f, 0.10f);
  colors[ImGuiCol_FrameBgHovered] = ImVec4(0.16f, 0.85f, 0.65f, 0.04f);
  colors[ImGuiCol_FrameBgActive] = ImVec4(0.16f, 0.85f, 0.65f, 0.48f);
  colors[ImGuiCol_TitleBg] = ImVec4(0.83f, 0.94f, 0.91f, 1.00f);
  colors[ImGuiCol_TitleBgActive] = ImVec4(0.58f, 0.94f, 0.83f, 1.00f);
  colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
  colors[ImGuiCol_MenuBarBg] = ImVec4(0.27f, 0.52f, 0.45f, 0.26f);
  colors[ImGuiCol_ScrollbarBg] = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
  colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.85f, 0.85f, 0.85f, 0.80f);
  colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.16f, 0.85f, 0.65f, 0.48f);
  colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.16f, 0.85f, 0.65f, 0.30f);
  colors[ImGuiCol_CheckMark] = ImVec4(0.16f, 0.85f, 0.65f, 1.00f);
  colors[ImGuiCol_SliderGrab] = ImVec4(0.16f, 0.85f, 0.65f, 0.53f);
  colors[ImGuiCol_SliderGrabActive] = ImVec4(0.27f, 0.52f, 0.45f, 1.00f);
  colors[ImGuiCol_Button] = ImVec4(0.16f, 0.85f, 0.65f, 0.26f);
  colors[ImGuiCol_ButtonHovered] = ImVec4(0.16f, 0.85f, 0.65f, 0.48f);
  colors[ImGuiCol_ButtonActive] = ImVec4(0.16f, 0.85f, 0.65f, 1.00f);
  colors[ImGuiCol_Header] = ImVec4(0.16f, 0.85f, 0.65f, 0.48f);
  colors[ImGuiCol_HeaderHovered] = ImVec4(0.16f, 0.85f, 0.65f, 0.48f);
  colors[ImGuiCol_HeaderActive] = ImVec4(0.16f, 0.85f, 0.65f, 1.00f);
  colors[ImGuiCol_Separator] = ImVec4(0.39f, 0.39f, 0.39f, 0.62f);
  colors[ImGuiCol_SeparatorHovered] = ImVec4(0.16f, 0.85f, 0.27f, 1.00f);
  colors[ImGuiCol_SeparatorActive] = ImVec4(0.85f, 0.31f, 0.16f, 1.00f);
  colors[ImGuiCol_ResizeGrip] = ImVec4(0.35f, 0.35f, 0.35f, 0.17f);
  colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.16f, 0.85f, 0.27f, 1.00f);
  colors[ImGuiCol_ResizeGripActive] = ImVec4(0.85f, 0.31f, 0.16f, 1.00f);
  colors[ImGuiCol_TabHovered] = ImVec4(0.16f, 0.85f, 0.65f, 1.00f);
  colors[ImGuiCol_Tab] = ImVec4(0.16f, 0.85f, 0.65f, 0.26f);
  colors[ImGuiCol_TabSelected] = ImVec4(0.16f, 0.85f, 0.65f, 0.48f);
  colors[ImGuiCol_TabSelectedOverline] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
  colors[ImGuiCol_TabDimmed] = ImVec4(0.92f, 0.93f, 0.94f, 0.99f);
  colors[ImGuiCol_TabDimmedSelected] = ImVec4(0.74f, 0.82f, 0.91f, 1.00f);
  colors[ImGuiCol_TabDimmedSelectedOverline] = ImVec4(0.26f, 0.59f, 1.00f, 1.00f);
  colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
  colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
  colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
  colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.45f, 0.00f, 1.00f);
  colors[ImGuiCol_TableHeaderBg] = ImVec4(0.16f, 0.85f, 0.65f, 0.48f);
  colors[ImGuiCol_TableBorderStrong] = ImVec4(0.57f, 0.57f, 0.64f, 1.00f);
  colors[ImGuiCol_TableBorderLight] = ImVec4(0.68f, 0.68f, 0.74f, 1.00f);
  colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
  colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.30f, 0.30f, 0.30f, 0.09f);
  colors[ImGuiCol_TextLink] = ImVec4(0.27f, 0.52f, 0.45f, 1.00f);
  colors[ImGuiCol_TextSelectedBg] = ImVec4(0.16f, 0.85f, 0.65f, 0.48f);
  colors[ImGuiCol_DragDropTarget] = ImVec4(0.16f, 0.85f, 0.65f, 0.95f);
  colors[ImGuiCol_NavHighlight] = ImVec4(0.16f, 0.85f, 0.65f, 0.94f);
  colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.70f, 0.70f, 0.70f, 0.70f);
  colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.20f);
  colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);

  return t;
}

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
