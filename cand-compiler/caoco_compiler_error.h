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
#include "caoco_grammar.h"
#include "caoco_token.h"
#include "caoco_token_cursor.h"
//---------------------------------------------------------------------------//

#define ERROR_LAMBDA(errname, ...) \
  static inline LAMBDA errname = [](__VA_ARGS__)

namespace caoco {
enum class eCaErr : std::size_t {
  kNone = 0,
  // Lexer errors
  kLexerUnknownChar,
  kLexerUnknownDirective,
  kLexerUnknownElement,
  // Parser errors
  kMismatchedScope,
  kExpectedPragmaticDeclaration,
  kExpectedPrimaryExpression,
  kExpectedToken,
  kNotImplemented,
  kInvalidForLoopSyntax,
  kUserSyntaxError,
  // Error codes for complier implementers. These should never be seen by the
  // end user. If they are, it means the compiler has a bug in its
  // implementation logic.
  kImplParserExpectedToken,
  kImplParserInvalidSingularOperand = std::numeric_limits<std::size_t>::max(),
};

namespace caerr {

constexpr inline string GenLineColLocation(size_t line, size_t col) {
  return "  [ Line: " + std::to_string(line) +
         "| Column: " + std::to_string(col) + "]";
}

constexpr inline string GenErrorName(eCaErr error_code) {
  switch (error_code) {
    case eCaErr::kNone:
      return "[No Error]: ";
    // Lexer errors
    case eCaErr::kLexerUnknownChar:
      return "[Lexer][UnknownChar]: \n";
    case eCaErr::kLexerUnknownDirective:
      return "[Lexer][SyntaxError]: ";

    case eCaErr::kLexerUnknownElement:
      return "[Lexer][UnknownElement]: ";
    // Parser errors
    case eCaErr::kExpectedPragmaticDeclaration:
      return "[Parser][ExpectedPragmaticDeclaration]: ";
    case eCaErr::kExpectedPrimaryExpression:
      return "[Parser][ExpectedPrimaryExpression]: ";
    case eCaErr::kMismatchedScope:
      return "[Parser][MismatchedScope]: ";
    case eCaErr::kExpectedToken:
      return "[Parser][ExpectedToken]: ";
    case eCaErr::kUserSyntaxError:
      return "[Parser][UserSyntaxError]";
    case eCaErr::kNotImplemented:
      return "[Parser][NotImplemented]: ";
    case eCaErr::kInvalidForLoopSyntax:
      return "[Parser][InvalidForLoopSyntax]: ";
      // Compiler Implementation errors
    case eCaErr::kImplParserExpectedToken:
      return "[Compiler Implementation][Parser][ExpectedToken]: ";
    case eCaErr::kImplParserInvalidSingularOperand:
      return "[Compiler Implementation][Parser][InvalidSingularOperand]: ";
    default:
      return "[Unimplemented error desriptor in caoco_compiler_error.h]: ";
  }
}

constexpr inline string GenErrorHeader(eCaErr error_code) {
  return string("[C&][ERROR:") + to_string(static_cast<size_t>(error_code)) +
         "]" + GenErrorName(error_code);
}

constexpr inline string GenErrorHeaderWithLoc(eCaErr error_code, size_t line,
                                              size_t col) {
  return string("[C&][ERROR:") + to_string(static_cast<size_t>(error_code)) +
         "]" + GenErrorName(error_code) + "[Line: " + to_string(line) +
         "| Column: " + to_string(col) + "]";
}

constexpr inline string GenPrettyPrintToken(const Tk& token) {
  return string("\nToken: ") + token.TypeStr().data() +
         "\nline: " + std::to_string(token.Line()) +
         "\ncolumn: " + std::to_string(token.Col()) +
         "\nliteral: " + token.Literal();
};

constexpr inline string GenCppSourceLocation(std::source_location err_loc) {
  return "\n[C++ Source][File: " + string(err_loc.file_name()) +
         "| Line: " + std::to_string(err_loc.line()) +
         "| Column: " + std::to_string(err_loc.column()) + "]" +
         "\n[ Method: " + err_loc.function_name() + "]";
  ;
}

//---------------------------------------------------------------------------//
// Section:{Lexer Errors}
// Brief:{}
//---------------------------------------------------------------------------//
ERROR_LAMBDA(LexerUnknownChar, size_t line, size_t col, char c,
             string pretty_line_loc) {
  // if c == \" then we can assume the user attempted to use " to
  // denote a string literal. C& uses ' for string literals.
  string special_case_error = "";

  if (c == '\"') {
    special_case_error =
        "Did you mean to use ' for a string literal? \nC& uses single "
        "apostrophes to enclose string literals. ex 'Hello World!'.\n";
  }
  string ret =
      GenErrorHeaderWithLoc(eCaErr::kLexerUnknownChar, line, col) +
      "\nUnknown character detected in source file at the lexing phase." +
      "\nOffending Char: '" + string(1, c) + "'.\n" + pretty_line_loc;

  if (!special_case_error.empty()) {
    ret += "\nError detail: " + special_case_error;
  }

  return ret;
};

ERROR_LAMBDA(LexerUnknownElement, size_t line, size_t col, char c,
             string pretty_line_loc, const string& special_case_error) {
  // if c == \" then we can assume the user attempted to use " to
  // denote a string literal. C& uses ' for string literals.
  string ret =
      GenErrorHeaderWithLoc(eCaErr::kLexerUnknownElement, line, col) +
      "\nUnknown token format detected in source file at the lexing phase." +
      "\nOffending Char: '" + string(1, c) + "'.\n" + pretty_line_loc;

  if (!special_case_error.empty()) {
    ret += "\nError detail: " + special_case_error;
  }

  return ret;
};

//---------------------------------------------------------------------------//
// EndSection:{Lexer Errors}
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
// Section:{Compiler Implementation Errors}
// Brief:{}
//---------------------------------------------------------------------------//
ERROR_LAMBDA(ImplParserInvalidSingularOperand, const TkCursor& token_location,
             std::source_location err_loc = {}) {
  err_loc = std::source_location::current();
  return GenErrorHeader(eCaErr::kImplParserInvalidSingularOperand) +
         GenPrettyPrintToken(token_location.Get()) +
         GenCppSourceLocation(err_loc);
};

ERROR_LAMBDA(ImplParserExpectedToken, const TkCursor& token_location,
             std::string error_message = "",
             std::source_location err_loc = {}) {
  err_loc = std::source_location::current();
  return GenErrorHeader(eCaErr::kImplParserExpectedToken) +
         GenPrettyPrintToken(token_location.Get()) +
         GenCppSourceLocation(err_loc);
};
//---------------------------------------------------------------------------//
// EndSection:{Compiler Implementation Errors}
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
// Section:{Parser Errors}
// Brief:{}
//---------------------------------------------------------------------------//
ERROR_LAMBDA(MismatchedScope, const TkCursor& token_location,
             std::string error_message = "",
             std::source_location err_loc = std::source_location::current()) {
  return GenErrorHeader(eCaErr::kMismatchedScope) +
         GenPrettyPrintToken(token_location.Get()) +
         GenCppSourceLocation(err_loc);
};

ERROR_LAMBDA(NotImplemented, const TkCursor& token_location,
             std::string error_message = "",
             std::source_location err_loc = std::source_location::current()) {
  return GenErrorHeader(eCaErr::kNotImplemented) +
         GenPrettyPrintToken(token_location.Get()) +
         GenCppSourceLocation(err_loc);
};

ERROR_LAMBDA(ExpectedPragmaticDeclaration, const TkCursor& token_location,
             std::string error_message = "",
             std::source_location err_loc = std::source_location::current()) {
  return GenErrorHeader(eCaErr::kExpectedPragmaticDeclaration) +
         GenPrettyPrintToken(token_location.Get()) +
         GenCppSourceLocation(err_loc);
};

ERROR_LAMBDA(ExpectedPrimaryExpression, const TkCursor& token_location,
             std::string error_message = "",
             std::source_location err_loc = std::source_location::current()) {
  return GenErrorHeader(eCaErr::kExpectedPrimaryExpression) +
         GenPrettyPrintToken(token_location.Get()) +
         GenCppSourceLocation(err_loc);
};

ERROR_LAMBDA(InvalidForLoopSyntax, const TkCursor& token_location,
             std::string error_message = "",
             std::source_location err_loc = std::source_location::current()) {
  return GenErrorHeader(eCaErr::kInvalidForLoopSyntax) +
         GenPrettyPrintToken(token_location.Get()) +
         GenCppSourceLocation(err_loc);
};

ERROR_LAMBDA(UserSyntaxError, const TkCursor& token_location,
             std::string error_message = "",
             std::source_location err_loc = std::source_location::current()) {
  return GenErrorHeader(eCaErr::kInvalidForLoopSyntax) +
         GenPrettyPrintToken(token_location.Get()) +
         GenCppSourceLocation(err_loc);
};

ERROR_LAMBDA(ExpectedToken, eTk expected, const TkCursor& token_location,
             std::string error_message = "",
             std::source_location err_loc = std::source_location::current()) {
  return GenErrorHeader(eCaErr::kExpectedToken) +
         "Expected: " + eTkEnumStr(expected).data() +
         " Found: " + token_location.Get().TypeStr().data() + error_message +
         GenCppSourceLocation(err_loc);
};

//---------------------------------------------------------------------------//
// EndSection:{Parser Errors}
//---------------------------------------------------------------------------//

}  // namespace caerr

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
