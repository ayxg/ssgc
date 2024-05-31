//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
// Copyright 2024 Anton Yashchenko
// Licensed under the Apache License, Version 2.0(the "License");
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

namespace caoco {
#define SCXIN static constexpr inline
#define CXIN constexpr inline

using std::list;
using std::size_t;
using std::string;
using std::vector;

namespace caerr {
//---------------------------------------------------------------------------//
// Enum:{eCaErrCategory}
// Brief:{
//  Categories of errors which may be emitted by the C& compiler.
// }
//---------------------------------------------------------------------------//
enum class eCaErrCategory : size_t {
  kWarning = 0,
  kLexer,
  kParser,
  kUserSyntax,
  kIrGen,
  kCompilerImpl,
  kConstEval
};

//-----------------------------------//
// Method:{CppSourceLocStr}
// Brief:{ Generates pretty string representation of a C++ source location.}
//-----------------------------------//
SCXIN string CppSourceLocStr(std::source_location err_loc) {
  return "\n[C++ Source][File: " + string(err_loc.file_name()) +
         "| Line: " + std::to_string(err_loc.line()) +
         "| Column: " + std::to_string(err_loc.column()) + "]" +
         "\n[ Method: " + err_loc.function_name() + "]";
}

//-----------------------------------//
// Method:{PrettyTokenStr}
// Brief:{ Generates pretty string representation of a C& Token.}
//-----------------------------------//
CXIN string PrettyTokenStr(const Tk& token) {
  return string("\nToken: ") + token.TypeStr().data() +
         "\nline: " + std::to_string(token.Line()) +
         "\ncolumn: " + std::to_string(token.Col()) +
         "\nliteral: " + token.Literal();
};

//-----------------------------------//
// Macro:{MACRO_EXPAND}
// Brief:{
// Expands a macro argument when not possible with ##.
// eg. class ##name (not allowed)
// }
//-----------------------------------//
#define MACRO_EXPAND(x) x

//-----------------------------------//
// Macro:{ALL_ERRORS}
// Brief:{
// Defines ALL the errors which may be emitted by the C& compiler.
// Declare all errors here!
// After declaring, use a DEF_ERROR macro to define the error and its details
// inside the CaErr struct.
// }
//-----------------------------------//
#define ALL_ERRORS                                                          \
  NoError, LexerUnknownChar, MismatchedScope, ExpectedPragmaticDeclaration, \
      ExpectedPrimaryExpression, ImplExpectedToken, NotImplemented,         \
      InvalidForLoopSyntax, UserSyntaxError, InvalidSingularOperand,        \
      ParserExpectedToken, CevalIntegerOverflow, CevalUnsignedOverflow,     \
      CevalRealOverflow, CevalInvalidBoolLiteral, CevalInvalidCharLiteral,  \
      CevalInvalidByteLiteral, CevalInvalidStringLiteral,InvalidEscapeSequence
//---------------------------------------------------------------------------//
// Enum:{eCaErrCategory}
// Brief:{
//  Enumeration of errors which may be emitted by the C& compiler.
// }
//---------------------------------------------------------------------------//
enum class eCaErr : size_t { ALL_ERRORS };

//-----------------------------------//
// Macro:{DEF_ERROR_DETAIL_FUNC}
// Brief:{
// DO NOT USE DIRECTLY!
// Defines a signature for a template method to generate a full error detail
// string. Intermediate macro used in the DEF_ERROR macro definition.
// }
//-----------------------------------//
#define DEF_ERROR_DETAIL_FUNC(errname)       \
  template <class ErrorT>                    \
    requires std::same_as<ErrorT, ##errname> \
  SCXIN std::string ErrDetail

//-----------------------------------//
// Macro:{DEF_ERROR}
// Brief:{
// Defines an error type given a name and a category.
// The macro ends with a  signature for a template method to generate a full
// error detail string. Do not add k preceding the category enum.
//
// example(pseudocode):
//  DEF_ERROR(TestError,Parser)(args...){return ...some string; }
//
// Use DEF_ERROR_DEFAULT,DEF_ERROR_DEFAULT_PARSER when explicit definition
// of an error is not needed.
// }
//-----------------------------------//
#define DEF_ERROR(g, category)                                   \
  struct MACRO_EXPAND(##g) {                                     \
    SCXIN eCaErr num = eCaErr::##g;                              \
    SCXIN const char* name = #g;                                 \
    SCXIN eCaErrCategory category = eCaErrCategory::k##category; \
    SCXIN const char* header = "[" #category "][" #g "]";        \
    SCXIN string HeaderStr() { return header; }                  \
    SCXIN string ErrStr() { return name; }                       \
    SCXIN string HeaderWithLocStr(size_t line, size_t col) {     \
      return HeaderStr() + "[ Line:" + std::to_string(line) +    \
             "| Col:" + std::to_string(col) + "]";               \
    }                                                            \
  };                                                             \
  DEF_ERROR_DETAIL_FUNC(##g)

#define DEF_ERROR_DEFAULT(en, ec)                                   \
  DEF_ERROR(##en, ##ec)(string details = "") {                      \
    return MACRO_EXPAND(##en)::HeaderStr() + "\n[" + details + "]"; \
  }

#define DEF_ERROR_DEFAULT_PARSER(en)                                           \
  DEF_ERROR(##en, Parser)                                                      \
  (const TkCursor& tk_loc, std::source_location compiler_fn_loc,               \
   string details = "") {                                                      \
    return MACRO_EXPAND(##en)::HeaderWithLocStr(tk_loc.Line(), tk_loc.Col()) + \
           "\n[" + details + "]\n" + PrettyTokenStr(tk_loc.Get()) +            \
           CppSourceLocStr(compiler_fn_loc);                                   \
  }

//---------------------------------------------------------//
// Class:{CaErr}
// Brief:{
//  Empty struct.
//  - Encapsulates all the error type definitions.
//  - Provides a uniform interface to access error detail printing methods of
//  each error type.
//
//  Use the method ErrDetail<T> where T is the error type.
//  Each ErrDetail method may have unique args but always returns a std::string.
// }
//---------------------------------------------------------//
struct CaErr {
  DEF_ERROR_DEFAULT(NoError, CompilerImpl);

  DEF_ERROR(LexerUnknownChar, Lexer)
  (size_t line, size_t col, char c, string pretty_line_loc,
   string detail = "") {
    // if c == \" then we can assume the user attempted to use " to
    // denote a string literal. C& uses ' for string literals.
    string special_case_error = "";

    if (c == '\"') {
      special_case_error =
          "Did you mean to use ' for a string literal? \nC& uses "
          "single "
          "apostrophes to enclose string literals. ex 'Hello "
          "World!'.\n";
    }
    string ret = LexerUnknownChar::HeaderWithLocStr(line, col) +
                 "\nUnknown character detected in source file at the lexing "
                 "phase." +
                 "\nOffending Char: '" + string(1, c) + "'.\n" +
                 pretty_line_loc;

    if (!special_case_error.empty()) {
      ret += "\nError detail: " + special_case_error;
    }

    if (!detail.empty()) {
      ret += "\n" + detail;
    }

    return ret;
  };

  DEF_ERROR(ParserExpectedToken, Parser)
  (eTk expected, TkCursor got, string detail = "") {
    return ParserExpectedToken::HeaderStr() +
           "Expected: " + eTkEnumStr(expected).data() +
           " Found: " + got.Get().TypeStr().data() + detail;
  }

  DEF_ERROR_DEFAULT_PARSER(InvalidSingularOperand);
  DEF_ERROR_DEFAULT_PARSER(ImplExpectedToken);
  DEF_ERROR_DEFAULT_PARSER(MismatchedScope);
  DEF_ERROR_DEFAULT_PARSER(NotImplemented);
  DEF_ERROR_DEFAULT_PARSER(ExpectedPragmaticDeclaration);
  DEF_ERROR_DEFAULT_PARSER(ExpectedPrimaryExpression);
  DEF_ERROR_DEFAULT_PARSER(InvalidForLoopSyntax);
  DEF_ERROR_DEFAULT_PARSER(UserSyntaxError);

  DEF_ERROR(CevalIntegerOverflow, ConstEval)
  (string lit, size_t line, size_t col) {
    return CevalIntegerOverflow::HeaderWithLocStr(line, col) +
           "\nInteger overflow detected during constant evaluation of the "
           "literal: " +
           lit;
  }

  DEF_ERROR(CevalUnsignedOverflow, ConstEval)
  (string lit, size_t line, size_t col) {
    return CevalIntegerOverflow::HeaderWithLocStr(line, col) +
           "\nUnsigned integer overflow detected during constant evaluation of "
           "the "
           "literal: " +
           lit;
  }

  DEF_ERROR(CevalRealOverflow, ConstEval)
  (string lit, size_t line, size_t col) {
    return CevalRealOverflow::HeaderWithLocStr(line, col) +
           "\nReal number overflow detected during constant evaluation of the "
           "literal: " +
           lit;
  }

  DEF_ERROR(CevalInvalidBoolLiteral, ConstEval)
  (string lit, size_t line, size_t col) {
    return CevalInvalidBoolLiteral::HeaderWithLocStr(line, col) +
           "\nInvalid boolean literal detected during constant evaluation of "
           "the "
           "literal: " +
           lit;
  }

  DEF_ERROR(CevalInvalidCharLiteral, ConstEval)
  (string lit, size_t line, size_t col) {
    return CevalInvalidCharLiteral::HeaderWithLocStr(line, col) +
           "\nInvalid character literal detected during constant evaluation of "
           "the "
           "literal: " +
           lit;
  }

  DEF_ERROR(CevalInvalidStringLiteral, ConstEval)
  (string lit, size_t line, size_t col) {
    return CevalInvalidStringLiteral::HeaderWithLocStr(line, col) +
           "\nInvalid string literal detected during constant evaluation of "
           "the "
           "literal: " +
           lit;
  }

  DEF_ERROR(CevalInvalidByteLiteral, ConstEval)
    (string lit, size_t line, size_t col) {
    return CevalInvalidByteLiteral::HeaderWithLocStr(line, col) +
           "\nInvalid byte literal detected during constant evaluation of the "
           "literal: " +
           lit;
  }

  DEF_ERROR(InvalidEscapeSequence, ConstEval)
    (string lit, size_t line, size_t col) {
    return InvalidEscapeSequence::HeaderWithLocStr(line, col) +
           "\nInvalid escape sequence detected during parsing of the "
           "literal: " +
           lit;
  }

  //---------------//
  // Typedef:{CaErrVariant}
  // Detail:{ Variant of all error types emmited by the C& compiler.}
  //---------------//
  using CaErrVariant = std::variant<ALL_ERRORS>;

  //---------------//
  // Typedef:{CaErrVariant}
  // Detail:{ Type index list of all error types emmited by the C& compiler.}
  //---------------//
  using CaErrIndexList = mta::compile_time_type_index_list<ALL_ERRORS>;
};
//---------------------------------------------------------//
// EndClass:{CaErr}
//---------------------------------------------------------//

// Undefining all the macros used above...
#undef MACRO_EXPAND
#undef ALL_ERRORS
#undef DEF_ERROR_DETAIL_FUNC
#undef DEF_ERROR
#undef DEF_ERROR_DEFAULT
#undef DEF_ERROR_DEFAULT_PARSER
#undef SCXIN
#undef CXIN

}  // namespace caerr
using CaErr = caerr::CaErr;
using CaErrVariant = CaErr::CaErrVariant;
using CaErrIndexList = CaErr::CaErrIndexList;
}  // namespace caoco

//---------------------------------------------------------------------------//
// Copyright 2024 Anton Yashchenko
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
