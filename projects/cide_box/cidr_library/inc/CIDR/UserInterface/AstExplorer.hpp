///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2025 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
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
  /// Parse input buffer as a C& source file and store in tokens/ast buffers.
  /// 
  /// Functionality is temporarily built into the interface.
  void ParseInputBuffer() {
    using cnd::trtools::Lexer;
    using cnd::trtools::parser::LLPrsResT;
    using cnd::trtools::parser::ParseSyntax;
    using std::make_unique;
    using std::unique_ptr;

    // In theory the compiler implementation should be no-throw. To be safe while in-development, we catch all
    // exceptions here.
    unique_ptr<Lexer::LexerOutputT> lex_result{};
    // Lex
    try {
      lex_result = make_unique<Lexer::LexerOutputT>(Lexer::Lex(input_text_buffer_));
      if (!*lex_result) {
        error_text_buffer_ = lex_result->error().Format();
        return;
      }
    } catch (const std::exception& e) {
      error_text_buffer_ = "Critical Error: lexing stage caused a throw: " + string(e.what());
      return;
    }

    // Sanitize, store in buffer.
    // The tokens which the ast refers to must remain live when retrieving the node's literal value.
    output_tokens_buffer_ = Lexer::Sanitize(**lex_result);
    std::span<const cnd::Tk> src_view = output_tokens_buffer_;

    // Parse
    unique_ptr<LLPrsResT> parse_result{};
    try {
      parse_result = make_unique<LLPrsResT>(ParseSyntax({src_view.cbegin(), src_view.cend()}));
      if (!*parse_result) {
        error_text_buffer_ = parse_result->error().Format();
        return;
      } else {
        output_ast_ = std::move((**parse_result).ast);
      }
    } catch (const std::exception& e) {
      error_text_buffer_ = "Critical Error: parsing stage caused a throw: " + string(e.what());
      return;
    }

    error_text_buffer_ = "[Completed]";
  }

  void RecursiveDisplayAstTree(const cnd::Ast& node) {
    // Each node is a tree node.
    // Containing:
    // 1. Node Type as Text
    // 2. Node Literal as Text
    // 3. Children As a TreeNode
    bool is_node_open = ImGui::TreeNode(cnd::eAstToCStr(node.type));
    if (is_node_open) {
      if (node.GetLiteral() != "") {
        bool is_data_node_open = ImGui::TreeNode("data:");
        if (is_data_node_open) {
          ImGui::Text("Literal: %s", node.GetLiteral().c_str());
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

  /// Display the ast as a tree node view.
  void DisplayAstView() {
    if (ast_view_context.BeginLate()) {
      RecursiveDisplayAstTree(output_ast_);
    }
    // ast_view_.Display();
    ast_view_context.EndEarly();
  }

  void DisplayTextInput() {
    if (text_input_context_.BeginLate()) {
      text_input_.BeginLate();
    }
    text_input_context_.EndEarly();
  }

  void DisplayToolbar() {
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

  cnd::Vec<cnd::Tk> output_tokens_buffer_;  ///> Lexed tokens which the output AST refers to.
  cnd::Ast output_ast_;                     ///> Output ast after parsing the input text.
  string input_text_buffer_;
  string error_text_buffer_;  ///> Error buffer if any error occurs during parsing or lexing, otherwise "[Completed]".
  CguiWindow window_{CguiWindow::Delayed("Ast Explorer")};
  CguiSubcontext toolbar_context_{CguiSubcontext::Delayed({1280.f, 100.f})};
  CguiButton parse_button_{CguiButton::Delayed("Parse")};
  CguiSubcontext text_input_context_{CguiSubcontext::Delayed({640.f, 600.f})};
  CguiMultilineTextInput text_input_{
      CguiMultilineTextInput::Delayed("Ast Explorer Input", input_text_buffer_, cgui::kExpandWidgetToRemainingSpaceXY)};
  CguiMultilineTextInput error_output_{
      CguiMultilineTextInput::Delayed("Error Output", error_text_buffer_, cgui::kExpandWidgetToRemainingSpaceXY)};
  CguiSubcontext ast_view_context{CguiSubcontext::Delayed({640.f, 600.f})};
};

}  // namespace cidr::ui

/// @} // end of cidrlib_frontend

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: [CIDR] C& integrated development repository library
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
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
