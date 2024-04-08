//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
// Copyright 2024 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
//---------------------------------------------------------------------------//
// Author(s): Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.acpp.dev
//---------------------------------------------------------------------------//
// Project: C& Programming Language Environment
// Directory: cand-official-compiler
// File: caoco_compiler_error.h
//---------------------------------------------------------------------------//
#ifndef HEADER_GUARD_CALE_CAND_OFFICIAL_COMPILER_CAOCO_COMPILER_ERROR_H
#define HEADER_GUARD_CALE_CAND_OFFICIAL_COMPILER_CAOCO_COMPILER_ERROR_H
//---------------------------------------------------------------------------//
// Brief: Lambdas to pretty-print compiler errors, warnings and notes.
//---------------------------------------------------------------------------//
#include "cppsextended.h"
// Includes:
#include "caoco_ast.h"
#include "caoco_enum.h"
#include "caoco_grammar.h"
#include "caoco_token.h"
#include "caoco_token_traits.h"
//---------------------------------------------------------------------------//

namespace caoco {
namespace compiler_error {
namespace tokenizer {

static constexpr LAMBDA xInvalidChar = [](std::size_t line, std::size_t col,
                                          char c, std::string error = "") {
  std::stringstream ss;
  ss << "\n[User Syntax Error]: ";
  ss << "\nInvalid character at line: " << line << " column: " << col
     << " \nerror detail: " << error;
  ss << "\n Offending character: " << static_cast<char>(c);
  return ss.str();
};

static constexpr LAMBDA xLexerSyntaxError =
    [](std::size_t line, std::size_t col, char8_t c, std::string error = "") {
      std::stringstream ss;
      ss << "\n[User Syntax Error]: ";
      ss << "\nLexer syntax error at line: " << line << " column: " << col
         << "\n Offending character: " << static_cast<char>(c)
         << "\nerror detail: " << error;
      return ss.str();
    };

static constexpr LAMBDA xProgrammerLogicError =
    [](std::size_t line, std::size_t col, char8_t c, std::string error = "") {
      std::stringstream ss;
      ss << "\n[Compiler programmer logic error]: ";
      ss << "\nFailed to tokenize at line: " << line << " column: " << col
         << "\n Offending character: " << static_cast<char>(c)
         << " \nerror detail: " << error;
      return ss.str();
    };
}  // namespace tokenizer
namespace parser {
static constexpr LAMBDA xPrettyPrintToken = [](const Tk& token) {
  std::stringstream ss;
  ss << "\nToken: " << ToCStr(token.Type()) << "\nline: " << token.Line()
     << "\ncolumn: " << token.Col() << "\nliteral: " << token.Literal();
  return ss.str();
};

static LAMBDA xProgrammerLogicError =
    [](eAst attempted_astnode_type, TkVectorConstIter error_location,
       std::string error_message = "",
       std::source_location err_loc = std::source_location::current()) {
      std::stringstream ss;
      ss << "\n[Compiler programmer logic error]: ";
      ss << "\nFailed to parse ast of type " << ToCStr(attempted_astnode_type)
         << " at: " << xPrettyPrintToken(*error_location);
      if (!error_message.empty()) {
        ss << "\n[Detail]: " << error_message;
      }
      ss << "\n[Compiler Error Location]: " << err_loc.file_name()
         << " line: " << err_loc.line() << " column: " << err_loc.column();
      ss << "\n[Compiler Method]: " << err_loc.function_name();
      return ss.str();
    };

static constexpr LAMBDA xOperationMissingOperand =
    [](eAst attempted_operator_type, TkVectorConstIter error_location,
       std::string error_message = "") {
      std::stringstream ss;
      ss << "\n[User Syntax Error]: ";
      ss << "\nOperation" << ToCStr(attempted_operator_type)
         << " missing operand at : "
         << " at: " << xPrettyPrintToken(*error_location)
         << "\nerror detail: " << error_message;
      return ss.str();
    };

static LAMBDA xMismatchedParentheses =
    [](TkVectorConstIter error_location, std::string error_message = "",
       std::source_location err_loc = std::source_location::current()) {
      std::stringstream ss;
      ss << "\n[C&][ERROR][xMismatchedParentheses]: ";
      ss << "\nMismatched parenthesis at: "
         << " at: " << xPrettyPrintToken(*error_location);
      if (!error_message.empty()) {
        ss << "\n[Detail]: " << error_message;
      }
      ss << "\n[Compiler Error Location]: " << err_loc.file_name()
         << " line: " << err_loc.line() << " column: " << err_loc.column();
      ss << "\n[Compiler Method]: " << err_loc.function_name();
      return ss.str();
    };

static LAMBDA xInvalidForLoopConditionSyntax =
    [](TkVectorConstIter error_location, std::string error_message = "",
       std::source_location err_loc = std::source_location::current()) {
      std::stringstream ss;
      ss << "\n[C&][ERROR][xInvalidForLoopConditionSyntax]: ";
      ss << "\nInvalid for loop syntax at: "
         << " at: " << xPrettyPrintToken(*error_location);
      if (!error_message.empty()) {
        ss << "\n[Detail]: " << error_message;
      }
      ss << "\n[Compiler Error Location]: " << err_loc.file_name()
         << " line: " << err_loc.line() << " column: " << err_loc.column();
      ss << "\n[Compiler Method]: " << err_loc.function_name();
      return ss.str();
    };

static constexpr LAMBDA xInvalidExpression =
    [](TkVectorConstIter error_location, std::string error_message = "") {
      std::stringstream ss;
      ss << "\n[User Syntax Error]: ";
      ss << "\nInvalid expression at: "
         << " at: " << xPrettyPrintToken(*error_location)
         << "\nerror detail: " << error_message;
      return ss.str();
    };

static LAMBDA xExpectedToken =
    [](const std::string& expected, const std::string& found,
       const std::string& error_message = "",
       std::source_location err_loc = std::source_location::current()) {
      std::stringstream ss;
      ss << "\n[C&][ERROR][xExpectedToken] ";
      ss << "Expected: " << expected << " found: " << found;
      if (!error_message.empty()) {
        ss << "\n[Detail]: " << error_message;
      }
      ss << "\n[Compiler Error Location]: " << err_loc.file_name()
         << " line: " << err_loc.line() << " column: " << err_loc.column();
      ss << "\n[Compiler Method]: " << err_loc.function_name();
      return ss.str();
    };

static constexpr LAMBDA xUserSyntaxError = [](TkVectorConstIter error_location,
                                              std::string error_message = "") {
  std::stringstream ss;
  ss << "\n[C&][ERROR][User Syntax Error]: "
     << " at: " << xPrettyPrintToken(*error_location)
     << "\nerror detail: " << error_message;
  return ss.str();
};
}  // namespace parser
}  // namespace compiler_error

}  // namespace caoco
//---------------------------------------------------------------------------//
// Copyright 2024 Anton Yashchenko
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
//---------------------------------------------------------------------------//
// Author(s): Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.acpp.dev
//---------------------------------------------------------------------------//
// Project: C& Programming Language Environment
// Directory: cand-official-compiler
// File: caoco_compiler_error.h
//---------------------------------------------------------------------------//
#endif HEADER_GUARD_CALE_CAND_OFFICIAL_COMPILER_CAOCO_COMPILER_ERROR_H
//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
