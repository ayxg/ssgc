///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2025 Anton Yashchenko
// Licensed under the Apache License, Version 2.0(the "License");
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: [CIDR] C& integrated development repository library
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup cidrlib
/// @brief AST Explorer Widget.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup cidrlib_frontend
/// @{

// clang-format off
#pragma once
#include "../Common.hpp"
#include "../Backend.hpp"

#include "trtools/Lexer.hpp"
#include "trtools/Parser.hpp"
// clang-format on

namespace cidr::ui {

class AstExplorerInterface {
  // Functionality is built into the interface for now.
  inline void ParseInputBuffer() {
    try {
      auto lex_result = cnd::trtools::Lexer::Lex(input_text_buffer_);
      if (not lex_result) {
        error_text_buffer_ = lex_result.error().Format();
        return;
      }
      try {
        auto parse_result = caoco::ParseTokens(lex_result.Value());
        if (not parse_result) {
          error_text_buffer_ = parse_result.Error();
          return;
        }
        output_ast_ = parse_result.Extract();
      } catch (const std::exception&) {
        error_text_buffer_ = "Critical Error: parsing caused a throw.";
      }
    } catch (const std::exception&) {
      error_text_buffer_ = "Critical Error: lexing caused a throw.";
    }

    error_text_buffer_ = "[Parsing Success!]";
  }

  void RecursiveDisplayAstTree(const cnd::Ast& node) {
    // Each node is a tree node.
    // Containing:
    // 1. Node Type as Text
    // 2. Node Literal as Text
    // 3. Children As a TreeNode
    bool is_node_open = ImGui::TreeNode(cnd::eAstToCStr(node.Type()).data());
    if (is_node_open) {
      if (node.GetLiteral() != "") {
        bool is_data_node_open = ImGui::TreeNode("data:");
        if (is_data_node_open) {
          ImGui::Text("Literal: %s", node.Literal().c_str());
          ImGui::TreePop();
        }
      }
      if (not node.children.empty()) {
        bool is_children_node_open = ImGui::TreeNode("children:");
        if (is_children_node_open) {
          for (auto& child : node.children) {
            RecursiveDisplayAstTree(child);
          }
          ImGui::TreePop();
        }
      }
      ImGui::TreePop();
    }
  };

  // Display the ast as a tree node view.
  inline void DisplayAstView() {
    if (ast_view_context.BeginLate()) {
      RecursiveDisplayAstTree(output_ast_);
    }
    // ast_view_.Display();
    ast_view_context.EndEarly();
  }

  inline void DisplayTextInput() {
    if (text_input_context_.BeginLate()) {
      text_input_.BeginLate();
    }
    text_input_context_.EndEarly();
  }

  inline void DisplayToolbar() {
    if (toolbar_context_.BeginLate()) {
      if (parse_button_.BeginLate()) {
        // Parse the input text.
        ParseInputBuffer();
      }
      cgui::SameLine();
      error_output_.BeginLate();
    }
    toolbar_context_.EndEarly();
  }

 public:
  void Display() {
    window_.BeginLate();
    DisplayToolbar();
    cgui::Separator();
    DisplayTextInput();
    cgui::SameLine();
    DisplayAstView();
    window_.EndEarly();
  }

 private:
  cnd::Ast output_ast_;
  string input_text_buffer_;
  string error_text_buffer_;
  CguiWindow window_{CguiWindow::Delayed("Ast Explorer")};
  CguiSubcontext toolbar_context_{CguiSubcontext::Delayed({1280.f, 100.f})};
  CguiButton parse_button_{CguiButton::Delayed("Parse")};
  CguiSubcontext text_input_context_{CguiSubcontext::Delayed({640.f, 600.f})};
  CguiMultilineTextInput text_input_{
      CguiMultilineTextInput::Delayed("Ast Explorer Input", input_text_buffer_,
                                      cgui::kExpandWidgetToRemainingSpaceXY)};
  CguiMultilineTextInput error_output_{
      CguiMultilineTextInput::Delayed("Error Output", error_text_buffer_,
                                      cgui::kExpandWidgetToRemainingSpaceXY)};
  CguiSubcontext ast_view_context{CguiSubcontext::Delayed({640.f, 600.f})};
};

}  // namespace cide::ui

/// @} // end of cidrlib_frontend

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: [CIDR] C& integrated development repository library
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
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
