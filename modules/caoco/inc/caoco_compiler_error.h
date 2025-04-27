///////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
///////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language Environment
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup cand_compiler_data
/// @brief Lambdas to pretty-print compiler errors, warnings and notes.
///////////////////////////////////////////////////////////////////////////////

/// @addtogroup cand_compiler_data
/// @{

#ifndef HEADER_GUARD_CALE_CAND_OFFICIAL_COMPILER_CAOCO_COMPILER_ERROR_H
#define HEADER_GUARD_CALE_CAND_OFFICIAL_COMPILER_CAOCO_COMPILER_ERROR_H

#include "cxxx.hpp"
// Includes:
#include "caoco_ast.h"
#include "caoco_grammar.h"
#include "caoco_token.h"
#include "caoco_token_cursor.h"

namespace caoco {
#define SCXIN static constexpr inline
#define CXIN constexpr inline
using std::list;
using std::size_t;
using std::string;
using std::vector;

namespace caerr {

/// Categories of errors which may be emitted by the C& compiler.
enum class eCaErrCategory : size_t {
  kWarning = 0,
  kCompilerImpl,
  kBuild,
  kLexer,
  kParser,
  kProcessor,
  kConstEval,
};

/// Generates pretty string representation of a C++ source location.
SCXIN string CppSourceLocStr(std::source_location err_loc) {
  return "\n[C++ Source][File: " + string(err_loc.file_name()) +
         "| Line: " + std::to_string(err_loc.line()) +
         "| Column: " + std::to_string(err_loc.column()) + "]" +
         "\n[ Method: " + err_loc.function_name() + "]";
}

/// Generates pretty string representation of a C& Token.
CXIN string PrettyTokenStr(const Tk& token) {
  return string("\nToken: ") + token.TypeStr().data() +
         "\nline: " + std::to_string(token.Line()) +
         "\ncolumn: " + std::to_string(token.Col()) +
         "\nliteral: " + token.Literal();
};

/// Expands a macro argument when not possible with ##.
/// eg. class ##name (not allowed)
#define MACRO_EXPAND(x) x

// clang-format off

/* Impl Errors */
#define APPLY_MACRO_TO_IMPL_ERRORS(macro) \
macro(ImplExpectedToken) \
/* Build Errors */ 
#define APPLY_MACRO_TO_BUILD_ERRORS(macro) \
macro(FailedToReadFile)\
macro(InvalidCliArg)\
macro(InclusionFailure)\
macro(ForbiddenSourceChar)
/* Lexer Errors */  
#define APPLY_MACRO_TO_LEXER_ERRORS(macro) \
macro(LexerUnknownChar) \
/* Parser Errors */
#define APPLY_MACRO_TO_PARSER_ERRORS(macro) \
macro(MismatchedScope) \
macro(ExpectedPragmaticDeclaration) \
macro(ExpectedPrimaryExpression) \
macro(NotImplemented) \
macro(InvalidForLoopSyntax) \
macro(UserSyntaxError) \
macro(InvalidSingularOperand)\
macro(ParserExpectedToken) \
/* Processor Errors */
#define APPLY_MACRO_TO_PROCESSOR_ERRORS(macro)
/* Consteval Errors */
#define APPLY_MACRO_TO_CONSTEVAL_ERRORS(macro) \
macro(CevalIntegerOverflow) \
macro(CevalUnsignedOverflow) \
macro(CevalRealOverflow) \
macro(CevalInvalidBoolLiteral) \
macro(CevalInvalidCharLiteral) \
macro(CevalInvalidByteLiteral) \
macro(CevalInvalidStringLiteral) \
macro(InvalidEscapeSequence)

#define APPLY_MACRO_TO_NO_ERROR(macro)\
macro(CustomError)\

/// Applies a macro to all errors, also defines every compiler error.
/// Define compiler errors here.
/// The last error is always NoError.
#define APPLY_MACRO_TO_ALL_ERRORS(macro,last) \
/* Impl Errors */    \
APPLY_MACRO_TO_IMPL_ERRORS(macro)\
/* Build Errors */    \
APPLY_MACRO_TO_BUILD_ERRORS(macro)\
/* Lexer Errors */    \
APPLY_MACRO_TO_LEXER_ERRORS(macro) \
/* Parser Errors */ \
APPLY_MACRO_TO_PARSER_ERRORS(macro)\
/* Processor Errors */ \
APPLY_MACRO_TO_PROCESSOR_ERRORS(macro)\
/* Consteval Errors */ \
APPLY_MACRO_TO_CONSTEVAL_ERRORS(macro)\
last(CustomError)
// clang-format on

#define LIST_ERROR(e) e,
#define LIST_ALL_ERRORS APPLY_MACRO_TO_ALL_ERRORS(LIST_ERROR, MACRO_EXPAND)

/// Defines ALL the errors which may be emitted by the C& compiler.
///
/// Declare all errors here!
/// After declaring, use a DEF_ERROR macro to define the error and its details
/// inside the CaErr struct.
#define ALL_ERRORS                                                          \
  NoError, LexerUnknownChar, MismatchedScope, ExpectedPragmaticDeclaration, \
      ExpectedPrimaryExpression, ImplExpectedToken, NotImplemented,         \
      InvalidForLoopSyntax, UserSyntaxError, InvalidSingularOperand,        \
      ParserExpectedToken, CevalIntegerOverflow, CevalUnsignedOverflow,     \
      CevalRealOverflow, CevalInvalidBoolLiteral, CevalInvalidCharLiteral,  \
      CevalInvalidByteLiteral, CevalInvalidStringLiteral,                   \
      InvalidEscapeSequence

/// Enumeration of errors which may be emitted by the C& compiler.
enum class eCaErr : size_t { LIST_ALL_ERRORS };

/// Convert error enum to string.
static constexpr cxx::cstring CaErrToStr(eCaErr e) {
#define ECAERR_TO_STR_CASE(err) \
  case eCaErr::err:             \
    return #err;

  switch (e) {
    APPLY_MACRO_TO_ALL_ERRORS(ECAERR_TO_STR_CASE, ECAERR_TO_STR_CASE)
    default:
      return "[ImplementationFailure] eCaErrToStr not defined for enum value.";
  }
}

/// Convert error category enum to string.
static constexpr cxx::cstring CaErrCatToStr(eCaErrCategory e) {
  switch (e) {
    case caoco::caerr::eCaErrCategory::kWarning:
      return "Warning";
    case caoco::caerr::eCaErrCategory::kCompilerImpl:
      return "Compiler Impl";
    case caoco::caerr::eCaErrCategory::kLexer:
      return "Lexer";
    case caoco::caerr::eCaErrCategory::kParser:
      return "Parser";
    case caoco::caerr::eCaErrCategory::kConstEval:
      return "Consteval";
    case caoco::caerr::eCaErrCategory::kBuild:
      return "Build";
    case caoco::caerr::eCaErrCategory::kProcessor:
      return "Processor";
    default:
      return "Unknown";
  }
}

/// Convert error enum to error category.
static constexpr eCaErrCategory CaErrCat(eCaErr e) {
#define ECAERR_TO_STR_CASE(err) case eCaErr::err:

  switch (e) {
    APPLY_MACRO_TO_IMPL_ERRORS(ECAERR_TO_STR_CASE)
    return eCaErrCategory::kCompilerImpl;
    APPLY_MACRO_TO_BUILD_ERRORS(ECAERR_TO_STR_CASE)
    return eCaErrCategory::kBuild;
    APPLY_MACRO_TO_LEXER_ERRORS(ECAERR_TO_STR_CASE)
    return eCaErrCategory::kLexer;
    APPLY_MACRO_TO_PARSER_ERRORS(ECAERR_TO_STR_CASE)
    return eCaErrCategory::kParser;
    APPLY_MACRO_TO_PROCESSOR_ERRORS(ECAERR_TO_STR_CASE)
    return eCaErrCategory::kProcessor;
    APPLY_MACRO_TO_CONSTEVAL_ERRORS(ECAERR_TO_STR_CASE)
    return eCaErrCategory::kConstEval;
    default:
      return eCaErrCategory::kCompilerImpl;
  }
}

/// Virtual base class for a compiler error must implement the Format() method.
struct CompilerErrorBASE {
  using args_list = vector<std::variant<size_t, string>>;
  const eCaErr code{eCaErr::CustomError};
  const args_list args{};
  size_t file{0};
  size_t line{0};
  size_t col{0};

  virtual string Format() const = 0;
  ~CompilerErrorBASE() = default;
  string FormattedHeader() const {
    string header = string{"["} + CaErrCatToStr(CaErrCat(code)) + "][" +
                    CaErrToStr(code) + "]";

    if (line != 0) {
      header += "[ Line:" + std::to_string(line) +
                "| Col:" + std::to_string(col) + "]";
    }
    return header;
  }

  CompilerErrorBASE(){};

  CompilerErrorBASE(eCaErr code) : code(code) {}
  CompilerErrorBASE(eCaErr code, args_list args) : code(code), args(args) {}
  CompilerErrorBASE(eCaErr code, args_list args, size_t file)
      : code(code), args(args), file(file) {}
  CompilerErrorBASE(eCaErr code, args_list args, size_t file, size_t line)
      : code(code), args(args), file(file), line(line) {}
  CompilerErrorBASE(eCaErr code, args_list args, size_t file, size_t line,
                    size_t col)
      : code(code), args(args), file(file), line(line), col(col) {}
};

/// Used to store/pass errors around.
using CaErrUptr = std::unique_ptr<CompilerErrorBASE>;

/// Helper method for creating an error uptr to avoid 'new' call.
template <class T>
  requires std::is_base_of_v<CompilerErrorBASE, T>
static constexpr inline CaErrUptr&& MakeError(T&& err) {
  return CaErrUptr{std::make_unique<T>(std::forward<T>(err))};
};

#define DECL_STRUCT(x) struct x;

/// Declare all error structs.
APPLY_MACRO_TO_ALL_ERRORS(DECL_STRUCT, DECL_STRUCT);

///////////////////////////////////////////////////////////////////////////////
// Implementation of errors:
///////////////////////////////////////////////////////////////////////////////

/// Custom error, last error in error list.
struct CustomError : public CompilerErrorBASE {
    using CompilerErrorBASE::CompilerErrorBASE;
  CustomError(const string& message = "", size_t file = 0, size_t line = 0,
              size_t col = 0)
      : CompilerErrorBASE(eCaErr::CustomError, args_list{message}) {}
  string Format() const {
    return FormattedHeader() + "[" + std::get<string>(args[0]) + "]";
  };
};
static_assert(
    std::same_as<decltype(MakeError(CustomError("Hi"))), CaErrUptr&&>);

struct ImplExpectedToken : public CompilerErrorBASE {
  using CompilerErrorBASE::CompilerErrorBASE;
  string expected;
  string got;

  ImplExpectedToken(string expected, string got, size_t file, size_t line,
                    size_t col)
      : CompilerErrorBASE(eCaErr::LexerUnknownChar, args_list{expected, got},
                          file, line, col) {}
  string Format() const {
    return FormattedHeader() + "[Implementation expected token [" +
           std::get<string>(args[0]) + "] got token [" +
           std::get<string>(args[1]) + "].]";
  };
};

struct InvalidCliArg : public CompilerErrorBASE {
    using CompilerErrorBASE::CompilerErrorBASE;
  InvalidCliArg(string arg, string detail)
      : CompilerErrorBASE(eCaErr::LexerUnknownChar, args_list{arg, detail}) {}
  string Format() const {
    return "[Invalid cli argument detected '" + std::get<string>(args[0]) +
           "':" + std::get<string>(args[1]) + "]";
  };
};

struct FailedToReadFile : public CompilerErrorBASE {
    using CompilerErrorBASE::CompilerErrorBASE;
  FailedToReadFile(string filename, string detail)
      : CompilerErrorBASE(eCaErr::LexerUnknownChar,
                          args_list{filename, detail}) {}
  string Format() const {
    return "[Could not read file '" + std::get<string>(args[0]) +
           "':" + std::get<string>(args[1]) + "]";
  };
};
struct InclusionFailure : public CompilerErrorBASE {
    using CompilerErrorBASE::CompilerErrorBASE;
  InclusionFailure(string filename, string detail)
      : CompilerErrorBASE(eCaErr::LexerUnknownChar,
                          args_list{filename, detail}) {}
  string Format() const {
    return "[Error including file '" + std::get<string>(args[0]) +
           "':" + std::get<string>(args[1]) + "]";
  };
};
struct ForbiddenSourceChar : public CompilerErrorBASE {
    using CompilerErrorBASE::CompilerErrorBASE;
  ForbiddenSourceChar(string filename, string character, size_t line,
                      size_t col)
      : CompilerErrorBASE(eCaErr::LexerUnknownChar,
                          args_list{filename, character}, 0, line, col) {}
  string Format() const {
    return "[Forbidden character in file '" + std::get<string>(args[0]) +
           "': Character: '" + std::get<string>(args[1]) + "' ]";
  };
};

struct LexerUnknownChar : public CompilerErrorBASE {
    using CompilerErrorBASE::CompilerErrorBASE;
  LexerUnknownChar(char invalid_char, size_t file, size_t line, size_t col)
      : CompilerErrorBASE(eCaErr::LexerUnknownChar,
                          args_list{string{1, invalid_char}}, file, line, col) {
  }
  string Format() const {
    return FormattedHeader() +
           "[Unknown ASCII character encountered in source file.]";
  };
};

struct MismatchedScope : public CompilerErrorBASE {
    using CompilerErrorBASE::CompilerErrorBASE;
  MismatchedScope(size_t file, size_t line, size_t col)
      : CompilerErrorBASE(eCaErr::MismatchedScope, args_list{}, file, line,
                          col) {}
  string Format() const { return FormattedHeader(); };
};
struct ExpectedPragmaticDeclaration : public CompilerErrorBASE {
    using CompilerErrorBASE::CompilerErrorBASE;
  ExpectedPragmaticDeclaration(size_t file, size_t line, size_t col)
      : CompilerErrorBASE(eCaErr::ExpectedPragmaticDeclaration, args_list{},
                          file, line, col) {}
  string Format() const { return FormattedHeader(); };
};
struct ExpectedPrimaryExpression : public CompilerErrorBASE {
    using CompilerErrorBASE::CompilerErrorBASE;
  ExpectedPrimaryExpression(size_t file, size_t line, size_t col)
      : CompilerErrorBASE(eCaErr::ExpectedPrimaryExpression, args_list{}, file,
                          line, col) {}
  string Format() const { return FormattedHeader(); };
};

struct NotImplemented : public CompilerErrorBASE {
    using CompilerErrorBASE::CompilerErrorBASE;
  NotImplemented(size_t file, size_t line, size_t col)
      : CompilerErrorBASE(eCaErr::NotImplemented, args_list{}, file, line,
                          col) {}
  string Format() const { return FormattedHeader(); };
};
struct InvalidForLoopSyntax : public CompilerErrorBASE {
    using CompilerErrorBASE::CompilerErrorBASE;
  InvalidForLoopSyntax(size_t file, size_t line, size_t col)
      : CompilerErrorBASE(eCaErr::InvalidForLoopSyntax, args_list{}, file, line,
                          col) {}
  string Format() const { return FormattedHeader(); };
};
struct UserSyntaxError : public CompilerErrorBASE {
    using CompilerErrorBASE::CompilerErrorBASE;
  UserSyntaxError(size_t file, size_t line, size_t col)
      : CompilerErrorBASE(eCaErr::UserSyntaxError, args_list{}, file, line,
                          col) {}
  string Format() const { return FormattedHeader(); };
};
struct InvalidSingularOperand : public CompilerErrorBASE {
    using CompilerErrorBASE::CompilerErrorBASE;
  InvalidSingularOperand(size_t file, size_t line, size_t col)
      : CompilerErrorBASE(eCaErr::InvalidSingularOperand, args_list{}, file,
                          line, col) {}
  string Format() const { return FormattedHeader(); };
};
struct ParserExpectedToken : public CompilerErrorBASE {
    using CompilerErrorBASE::CompilerErrorBASE;
  ParserExpectedToken(size_t file, size_t line, size_t col)
      : CompilerErrorBASE(eCaErr::ParserExpectedToken, args_list{}, file, line,
                          col) {}
  string Format() const { return FormattedHeader(); };
};

struct CevalIntegerOverflow : public CompilerErrorBASE {
    using CompilerErrorBASE::CompilerErrorBASE;
  CevalIntegerOverflow(size_t file, size_t line, size_t col)
      : CompilerErrorBASE(eCaErr::CevalIntegerOverflow, args_list{}, file, line,
                          col) {}
  string Format() const { return FormattedHeader(); };
};
struct CevalUnsignedOverflow : public CompilerErrorBASE {
    using CompilerErrorBASE::CompilerErrorBASE;
  CevalUnsignedOverflow(size_t file, size_t line, size_t col)
      : CompilerErrorBASE(eCaErr::CevalUnsignedOverflow, args_list{}, file,
                          line, col) {}
  string Format() const { return FormattedHeader(); };
};
struct CevalRealOverflow : public CompilerErrorBASE {
    using CompilerErrorBASE::CompilerErrorBASE;
  CevalRealOverflow(size_t file, size_t line, size_t col)
      : CompilerErrorBASE(eCaErr::CevalRealOverflow, args_list{}, file, line,
                          col) {}
  string Format() const { return FormattedHeader(); };
};
struct CevalInvalidBoolLiteral : public CompilerErrorBASE {
    using CompilerErrorBASE::CompilerErrorBASE;
  CevalInvalidBoolLiteral(size_t file, size_t line, size_t col)
      : CompilerErrorBASE(eCaErr::CevalInvalidBoolLiteral, args_list{}, file,
                          line, col) {}
  string Format() const { return FormattedHeader(); };
};
struct CevalInvalidCharLiteral : public CompilerErrorBASE {
    using CompilerErrorBASE::CompilerErrorBASE;
  CevalInvalidCharLiteral(size_t file, size_t line, size_t col)
      : CompilerErrorBASE(eCaErr::CevalInvalidCharLiteral, args_list{}, file,
                          line, col) {}
  string Format() const { return FormattedHeader(); };
};
struct CevalInvalidByteLiteral : public CompilerErrorBASE {
    using CompilerErrorBASE::CompilerErrorBASE;
  CevalInvalidByteLiteral(size_t file, size_t line, size_t col)
      : CompilerErrorBASE(eCaErr::CevalInvalidByteLiteral, args_list{}, file,
                          line, col) {}
  string Format() const { return FormattedHeader(); };
};
struct CevalInvalidStringLiteral : public CompilerErrorBASE {
    using CompilerErrorBASE::CompilerErrorBASE;
  CevalInvalidStringLiteral(size_t file, size_t line, size_t col)
      : CompilerErrorBASE(eCaErr::CevalInvalidStringLiteral, args_list{}, file,
                          line, col) {}
  string Format() const { return FormattedHeader(); };
};
struct InvalidEscapeSequence : public CompilerErrorBASE {
    using CompilerErrorBASE::CompilerErrorBASE;
  InvalidEscapeSequence(size_t file, size_t line, size_t col)
      : CompilerErrorBASE(eCaErr::InvalidEscapeSequence, args_list{}, file,
                          line, col) {}
  string Format() const { return FormattedHeader(); };
};

///////////////////////////////////////////////////////////////////////////////
// end implementation of errors
///////////////////////////////////////////////////////////////////////////////

using CaErrUptrVec = vector<CaErrUptr>;

template <class T>
concept iCompilerErrorData = std::is_base_of<CompilerErrorBASE, T>::value;

template <class T>
concept iNotCompilerError =
    (not std::same_as<std::remove_cvref_t<std::decay_t<T>>, CaErrUptr>)&&(
        not std::same_as<std::remove_cvref_t<std::decay_t<T>>, CaErrUptrVec>);

class BooleanCompilerProcessResult {
 public:
  constexpr bool Valid() const { return valid_; }
  constexpr const CaErrUptrVec& Errors() const { return errors_.value(); }
  constexpr const CaErrUptr& LastError() const {
    return errors_.value().back();
  }
  /// True if object is in a valid state.
  constexpr operator bool() const { return valid_; }

 public:
  constexpr BooleanCompilerProcessResult()
      : valid_(true), errors_(std::nullopt) {}
  constexpr BooleanCompilerProcessResult(bool value)
      : valid_(value), errors_(std::nullopt) {}
  constexpr BooleanCompilerProcessResult(CaErrUptrVec&& errors)
      : valid_(false), errors_(std::forward<CaErrUptrVec>(errors)) {}
  constexpr BooleanCompilerProcessResult(CaErrUptr&& error)
      : valid_(false), errors_(CaErrUptrVec{}) {
    errors_.value().push_back(std::forward<CaErrUptr>(error));
  }
  constexpr CaErrUptrVec ExtractErrors() {
    CaErrUptrVec val = std::move(errors_.value());
    errors_.reset();
    return val;
  }

 private:
  /// Is this object in a valid state?
  bool valid_;
  /// Error messages if in error state.
  std::optional<CaErrUptrVec> errors_{};
};

template <typename T>
  requires iNotCompilerError<T>
class CompilerProcessResult {
 public:
  constexpr bool Valid() const { return expected_.has_value(); }
  constexpr T Extract() {
    T val = std::move(expected_.value());
    expected_.reset();
    errors_.clear();
    return val;
  }
  constexpr const T& Value() const { return expected_.value(); }
  constexpr const auto& Errors() const { return errors_; }
  constexpr const auto& LastError() const { return errors_.back(); }
  constexpr auto& Errors() { return errors_; }
  constexpr auto& LastError() { return errors_.back(); }
  constexpr operator bool() const { return expected_.has_value(); }

  constexpr CompilerProcessResult(T&& expected)
      : expected_(std::forward<T>(expected)) {}
  constexpr CompilerProcessResult(const T& expected) : expected_(expected) {}
  constexpr CompilerProcessResult(CaErrUptrVec&& errors)
      : expected_(std::nullopt), errors_(std::forward<CaErrUptrVec>(errors)) {}
  constexpr CompilerProcessResult(CaErrUptrVec& errors)
      : expected_(std::nullopt), errors_(errors) {}
  constexpr CompilerProcessResult(CaErrUptr&& error)
      : expected_(std::nullopt), errors_(CaErrUptrVec{}) {
    errors_.push_back(std::forward<CaErrUptr>(error));
  }
  constexpr CaErrUptrVec ExtractErrors() {
    CaErrUptrVec val = std::move(errors_);
    errors_.clear();
    return val;
  }

 private:
  std::optional<T> expected_{std::nullopt};
  CaErrUptrVec errors_{};
};

template <typename T, typename AlwaysT>
  requires(iNotCompilerError<T> && iNotCompilerError<AlwaysT>)
class PartialCompilerProcessResult {
 public:
  explicit constexpr PartialCompilerProcessResult(const AlwaysT& always,
                                                  T expected)
      : always_(always), value_(expected), errors_(std::nullopt) {}

  constexpr PartialCompilerProcessResult(const AlwaysT& always)
      : always_(always), value_(std::nullopt), errors_(std::nullopt) {}

  constexpr PartialCompilerProcessResult(const AlwaysT& always,
                                         CaErrUptrVec&& errors)
      : always_(always),
        value_(std::nullopt),
        errors_(std::forward<CaErrUptrVec>(errors)) {}

  constexpr PartialCompilerProcessResult(const AlwaysT& always,
                                         CaErrUptr&& error)
      : always_(always), value_(std::nullopt), errors_(CaErrUptrVec()) {
    errors_.value().push_back(std::forward<CaErrUptr>(error));
  }

  constexpr bool Valid() const { return value_.has_value(); }

  constexpr const T& Value() const { return value_.value(); }

  constexpr const AlwaysT& Always() const { return always_; }

  constexpr const CaErrUptrVec& Errors() const { return errors_.value(); }
  constexpr const CaErrUptr& LastError() const {
    return errors_.value().back();
  }

  constexpr operator bool() const { return value_.has_value(); }
  constexpr T Extract() {
    T val = std::move(value_.value());
    value_.reset();
    errors_.reset();
    return val;
  }
  constexpr CaErrUptrVec ExtractErrors() {
    CaErrUptrVec val = std::move(errors_.value());
    errors_.reset();
    return val;
  }

 private:
  AlwaysT always_;
  std::optional<T> value_{std::nullopt};
  std::optional<CaErrUptrVec> errors_{std::nullopt};
};

/// @warning DO NOT USE DIRECTLY!
///
/// Defines a signature for a template method to generate a full error detail
/// string. Intermediate macro used in the DEF_ERROR macro definition.
#define DEF_ERROR_DETAIL_FUNC(errname)       \
  template <class ErrorT>                    \
    requires std::same_as<ErrorT, ##errname> \
  SCXIN std::string ErrDetail

/// Defines an error type given a name and a category.
///
/// The macro ends with a  signature for a template method to generate a full
/// error detail string. Do not add k preceding the category enum.
/// example:
/// @code
///  DEF_ERROR(TestError,Parser)(args...){return ...some string; }
/// @endcode
/// Use DEF_ERROR_DEFAULT,DEF_ERROR_DEFAULT_PARSER when explicit definition
/// of an error is not needed.
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

/// Empty struct which defines traits of C& compiler errors.
///
///  - Encapsulates all the error type definitions.
///  - Provides a uniform interface to access error detail printing methods of
///  each error type.
///
///  Use the method ErrDetail<T> where T is the error type.
///  Each ErrDetail method may have unique args but always returns a
///  std::string.
struct CaErr {
  DEF_ERROR_DEFAULT(CustomError, CompilerImpl);

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
};

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
template <class T>
using CompResult = caerr::CompilerProcessResult<T>;
using CompResultBool = caerr::BooleanCompilerProcessResult;
template <class T, class AlwaysT>
using CompResultPartial = caerr::PartialCompilerProcessResult<T, AlwaysT>;
}  // namespace caoco

#endif HEADER_GUARD_CALE_CAND_OFFICIAL_COMPILER_CAOCO_COMPILER_ERROR_H
/// @} // end of cand_compiler_data

///////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language Environment
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////////////////
