///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup cnd_corevals
/// @brief Compiler Diagnostic Message Base
///
/// -[01/13/2025] There may potentially be thousands of unique diagnostic messages that the compiler
/// can generate. We need a way to programmatically enumerate all the diagnostic messages, and to be
/// able to pass data to a predefined formatting method of a specific given diagnostic.
///
/// The main requirements:
///
/// -[01/13/2025] Do NOT create a class or class template specialization for each message. We don't
/// want to overload the type system and bloat the executable. There will be more important uses for
/// types than error messages.
///
/// -[01/13/2025] Each message will depend the message identity, an array of data, a creation
/// method, and a formatting method.
///
/// -[01/13/2025] The data a message is allowed to receive must be one of
/// CompilerMessageDataTypeUnion, which is the union of allowable types. These types may include
/// known C++ types and any 'corevals' namespace types.
///
/// -[01/13/2025] From the compiler developer's user perspective we need to able to get
/// intellisense/compile time validation of the error creation and formatting methods. eg. 'MakeMsg'
/// must have a concrete set of named overloads.
///
/// -[01/13/2025] The error structure should be const-evaluable up to the error code. Since we
/// cannot create formatted dynamic output at compile time. In a constexpr context we can only
/// access the formatted data as an intermediate value.
///
/// -[01/13/2025] Compiler messages must be able to receive a variable quantity and types of data.
/// The allowable data types may be enumerated, the quantity may not. This data is stored in the
/// message and formatted into a string when needed.
///
/// -[01/13/2025] The Diagnostic structure should be tiny(8 bytes) to minimize stack space. When
/// used as an alternative for variant expected , optional: Diagnostic should not cause an increase
/// in the union size. The expectation is that IF a Diagnostic is returned then the program has
/// already encountered an invalid or error state, or we are printing verbose diagnostics in which
/// case unpacking speed does not matter. As such we can use many layers of indirection to
/// dynamically allocate a message upon construction. The overall underlying data structure of
/// Diagnostic currently is:
///    'std::unique_ptr<std::vector<std::variant<Diagnostic,std::vector<Diagnostic>>>>'
/// This is all wrapped up and separated into the Diagnostic, DiagnosticChain, DiagnosticChain
/// and Diagnostic classes.
///
/// Implementation details:
///
/// -[01/13/2025] We can define a method to create a message constrained to a given message enum
/// entry by using 'requires'. The message category and type will be available at compile time based
/// on the enum entry. By static casting the enum to DiagnosticCodeIntT or DiagnosticCategoryIntT,
/// we turn this into a pseudo-'compile time virtual method'. Intellisense should be able to display
/// the required arguments to create the valid data for this message.
///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup cnd_compiler_cldev
/// @{
#pragma once
#include <cstdint>
#include <format>
#include <memory>
#include <source_location>
#include <span>
#include <string>
#include <utility>
#include <variant>
#include <vector>
#include <ranges>

#include "meta_applied_enum.hpp"
namespace ssgc::diagnostic {

/// Integral type which all message enums must fit in. Currently includes:
/// eError, eWarning, eGuideline, eInfo.
using DiagnosticCodeIntT = std::uint32_t;

/// Integral type which all diagnostic categories and global diagnostic type must fit in. Currently
/// includes: eErrorCategory, eWarningCategory, eGuidelineCategory, eInfoCategory.
using DiagnosticCategoryIntT = unsigned char;

/// Integral type for constant embedded data in the message type. eg. is an error recoverable?
/// Interpretation of the data will depend on the message code and category.
using DiagnosticParameterIntT = short;

///////////////////////////////////////////////////////////////////////////////////////////////////
// Define diagnostic enums.
///////////////////////////////////////////////////////////////////////////////////////////////////

#define SSGC_AppliedEnum_eError(m, sep, pre, lst) \
  pre m(kError)                                   \
  sep m(kPlaceholder)                             \
  sep m(kDeveloperBug)                            \
  sep m(kCliParserFailure)                        \
  sep m(kLexerUnknownNumericSuffix)               \
  sep m(kLexerUnclosedStringLiteral)              \
  sep m(kLexerUnclosedCharacterLiteral)           \
  sep m(kLexerEmptyCharacterLiteral)              \
  sep m(kLexerInvalidPunctuator)                  \
  lst

//

//sep m(kFailedToReadFile)                        \
  //sep m(kParserExpectedDeclaration)               \
  //sep m(kCliMissingRequiredFlag)                  \
  //sep m(kCliFlagMustHavePostfix)                  \
  //sep m(kCliFlagExpectedArgs)                     \
  //sep m(kCliFlagInvalidArg)                       \
  //sep m(kCliDeniedOverwrite)                      \
  //sep m(kCliFailedToRedirectStream)               \
  //lst

SSGC_MACRO_DefineTypedEnumFromAppliedList(SSGC_AppliedEnum_eError, eError, DiagnosticCodeIntT);

#define SSGC_AppliedEnum_eWarning(m, sep, pre, lst) \
  pre m(kWarning)                                   \
  lst
SSGC_MACRO_DefineTypedEnumFromAppliedList(SSGC_AppliedEnum_eWarning, eWarning, DiagnosticCodeIntT);

#define SSGC_AppliedEnum_eGuideline(m, sep, pre, lst) \
  pre m(kGuideline)                                   \
  lst
SSGC_MACRO_DefineTypedEnumFromAppliedList(SSGC_AppliedEnum_eGuideline, eGuideline,
                                          DiagnosticCodeIntT);

#define SSGC_AppliedEnum_eInfo(m, sep, pre, lst) \
  pre m(kInfo)                                   \
  lst
SSGC_MACRO_DefineTypedEnumFromAppliedList(SSGC_AppliedEnum_eInfo, eInfo, DiagnosticCodeIntT);

///////////////////////////////////////////////////////////////////////////////////////////////////
// Define diagnostic categories.
///////////////////////////////////////////////////////////////////////////////////////////////////

#define SSGC_AppliedEnum_eErrorCategory(m, sep, pre, lst) \
  pre m(kNone)                                            \
  sep m(kWarning)                                         \
  sep m(kCompilerImpl)                                    \
  sep m(kCommandLine)                                     \
  sep m(kGeneral)                                         \
  sep m(kScanner)                                         \
  sep m(kLexer)                                           \
  sep m(kParser)                                          \
  sep m(kPreprocessor)                                    \
  sep m(kConstEval)                                       \
  lst
SSGC_MACRO_DefineTypedEnumFromAppliedList(SSGC_AppliedEnum_eErrorCategory, eErrorCategory,
                                          DiagnosticCategoryIntT);

#define SSGC_AppliedEnum_eWarningCategory(m, sep, pre, lst) \
  pre m(kNone)                                              \
  sep m(kWarning)                                           \
  sep m(kCompilerImpl)                                      \
  sep m(kCommandLine)                                       \
  sep m(kGeneral)                                           \
  sep m(kScanner)                                           \
  sep m(kLexer)                                             \
  sep m(kParser)                                            \
  sep m(kPreprocessor)                                      \
  sep m(kConstEval)                                         \
  lst
SSGC_MACRO_DefineTypedEnumFromAppliedList(SSGC_AppliedEnum_eWarningCategory, eWarningCategory,
                                          DiagnosticCategoryIntT);

#define SSGC_AppliedEnum_eGuidelineCategory(m, sep, pre, lst) \
  pre m(kNone)                                                \
  sep m(kWarning)                                             \
  sep m(kCompilerImpl)                                        \
  sep m(kCommandLine)                                         \
  sep m(kGeneral)                                             \
  sep m(kScanner)                                             \
  sep m(kLexer)                                               \
  sep m(kParser)                                              \
  sep m(kPreprocessor)                                        \
  sep m(kConstEval)                                           \
  lst
SSGC_MACRO_DefineTypedEnumFromAppliedList(SSGC_AppliedEnum_eGuidelineCategory, eGuidelineCategory,
                                          DiagnosticCategoryIntT);

#define SSGC_AppliedEnum_eInfoCategory(m, sep, pre, lst) \
  pre m(kNone)                                           \
  sep m(kWarning)                                        \
  sep m(kCompilerImpl)                                   \
  sep m(kCommandLine)                                    \
  sep m(kGeneral)                                        \
  sep m(kScanner)                                        \
  sep m(kLexer)                                          \
  sep m(kParser)                                         \
  sep m(kPreprocessor)                                   \
  sep m(kConstEval)                                      \
  lst
SSGC_MACRO_DefineTypedEnumFromAppliedList(SSGC_AppliedEnum_eInfoCategory, eInfoCategory,
                                          DiagnosticCategoryIntT);

constexpr eErrorCategory getErrorCategory(eError e) noexcept {
  switch (e) {
    default:
      return eErrorCategory::kNone;
  }
};

constexpr eWarningCategory getWarningCategory(eWarning e) noexcept {
  switch (e) {
    default:
      return eWarningCategory::kNone;
  }
};

constexpr eGuidelineCategory getGuidelineCategory(eGuideline e) noexcept {
  switch (e) {
    default:
      return eGuidelineCategory::kNone;
  }
};

constexpr eInfoCategory getInfoCategory(eInfo e) noexcept {
  switch (e) {
    default:
      return eInfoCategory::kNone;
  }
};

///////////////////////////////////////////////////////////////////////////////////////////////////
// Define message type.
///////////////////////////////////////////////////////////////////////////////////////////////////

#define SSGC_AppliedEnum_eDiagnosticType(m, sep, pre, lst) \
  pre m(kNone)                                             \
  sep m(kWarning)                                          \
  sep m(kError)                                            \
  sep m(kGuideline)                                        \
  sep m(kInfo)                                             \
  sep m(kGeneric)                                          \
  lst

SSGC_MACRO_DefineTypedEnumFromAppliedList(SSGC_AppliedEnum_eDiagnosticType, eDiagnosticType,
                                          DiagnosticCategoryIntT);

constexpr eDiagnosticType getDiagnosticType(eError e) noexcept { return eDiagnosticType::kError; };

constexpr eDiagnosticType getDiagnosticType(eWarning e) noexcept {
  return eDiagnosticType::kWarning;
};

constexpr eDiagnosticType getDiagnosticType(eGuideline e) noexcept {
  return eDiagnosticType::kGuideline;
};

constexpr eDiagnosticType getDiagnosticType(eInfo e) noexcept { return eDiagnosticType::kInfo; };

///////////////////////////////////////////////////////////////////////////////////////////////////
// Define message enum.
///////////////////////////////////////////////////////////////////////////////////////////////////

#define SSGC_AppliedEnum_eDiagnostic(m, sep, pre, lst)                                    \
  pre m(GenericCompilerMessage) SSGC_AppliedEnum_eError(m, sep, sep, )                    \
      SSGC_AppliedEnum_eWarning(m, sep, sep, ) SSGC_AppliedEnum_eGuideline(m, sep, sep, ) \
          SSGC_AppliedEnum_eInfo(m, sep, sep, ) lst

SSGC_MACRO_DefineEnumFromAppliedList(SSGC_AppliedEnum_eDiagnostic, eDiagnostic);

// Assert size of DiagnosticId bitfield's integral types.
static_assert(sizeof(DiagnosticCodeIntT) == 4 && "DiagnosticCodeIntT must be 4 bytes.");
static_assert(sizeof(DiagnosticCategoryIntT) == 1 && "DiagnosticCategoryIntT must be 1 byte.");
static_assert(sizeof(DiagnosticParameterIntT) == 2 && "DiagnosticParameterIntT must be 2 bytes.");

/////////////////////////////////////////////////////////////////////////////
/* Bitfield to identify compiler error messages. */
/////////////////////////////////////////////////////////////////////////////
struct DiagnosticId;

constexpr DiagnosticId getDiagnosticId(eError e);
constexpr DiagnosticId getDiagnosticId(eWarning e);
constexpr DiagnosticId getDiagnosticId(eGuideline e);
constexpr DiagnosticId getDiagnosticId(eInfo e);

/// A bitfield structure which identifies a compiler message.
struct DiagnosticId {
  DiagnosticCodeIntT code{
      0};  ///> Id of the message within the message type, maps to the diagnostic enums.
  DiagnosticCategoryIntT category{
      0};  ///> Category within the message type, maps to the message category enums.
  DiagnosticCategoryIntT msg_type{0};    ///> Message type, maps to eDiagnosticType
  DiagnosticParameterIntT parameter{0};  ///> Placeholder for data dependant on code and category.

  /// Compares based on DiagnosticId members 'code', 'category' and 'msg_type'; 'parameter' is
  /// ignored.
  constexpr bool operator==(const DiagnosticId& other) const {
    return code == other.code && category == other.category && msg_type == other.msg_type;
  }

  /// Compares based on DiagnosticId members 'code', 'category' and 'msg_type'; 'parameter' is
  /// ignored.
  constexpr bool operator!=(const DiagnosticId& other) const { return !(*this == other); }

  constexpr DiagnosticId() {}

  constexpr DiagnosticId(DiagnosticCodeIntT c, DiagnosticCategoryIntT cat,
                         DiagnosticCategoryIntT type, DiagnosticParameterIntT param = 0) noexcept
      : code(c), category(cat), msg_type(type), parameter(param) {}

  constexpr DiagnosticId(DiagnosticId&& other) noexcept
      : code(std::move(other.code)),
        category(std::move(other.category)),
        msg_type(std::move(other.msg_type)),
        parameter(std::move(other.parameter)) {}

  constexpr DiagnosticId(const DiagnosticId& other) noexcept
      : code(other.code),
        category(other.category),
        msg_type(other.msg_type),
        parameter(other.parameter) {}

  constexpr DiagnosticId& operator=(const DiagnosticId&) noexcept = default;
  constexpr DiagnosticId& operator=(DiagnosticId&&) noexcept = default;

  constexpr DiagnosticId(eError e) noexcept { *this = getDiagnosticId(e); }
  constexpr DiagnosticId(eWarning e) noexcept { *this = getDiagnosticId(e); }
  constexpr DiagnosticId(eGuideline e) noexcept { *this = getDiagnosticId(e); }
  constexpr DiagnosticId(eInfo e) noexcept { *this = getDiagnosticId(e); }
};
static_assert(sizeof(DiagnosticId) == 8 && "DiagnosticId must be 8 bytes.");

/////////////////////////////////////////////////////////////////////////////
/* Methods for converting diagnostic enums to their corresponding DiagnosticId. */
/////////////////////////////////////////////////////////////////////////////

// TODO: For now the parameter of the cl msg will always be 0 as I have not found a use for the data
// yet.
//       Later make sure to add the parameter retrieving logic to these methods if ever used
constexpr DiagnosticId getDiagnosticId(eError e) {
  return DiagnosticId{static_cast<DiagnosticCodeIntT>(e),
                      static_cast<DiagnosticCategoryIntT>(getErrorCategory(e)),
                      static_cast<DiagnosticCategoryIntT>(getDiagnosticType(e))};
};

constexpr DiagnosticId getDiagnosticId(eWarning e) {
  return DiagnosticId{static_cast<DiagnosticCodeIntT>(e),
                      static_cast<DiagnosticCategoryIntT>(getWarningCategory(e)),
                      static_cast<DiagnosticCategoryIntT>(getDiagnosticType(e))};
};

constexpr DiagnosticId getDiagnosticId(eGuideline e) {
  return DiagnosticId{static_cast<DiagnosticCodeIntT>(e),
                      static_cast<DiagnosticCategoryIntT>(getGuidelineCategory(e)),
                      static_cast<DiagnosticCategoryIntT>(getDiagnosticType(e))};
};

constexpr DiagnosticId getDiagnosticId(eInfo e) {
  return DiagnosticId{static_cast<DiagnosticCodeIntT>(e),
                      static_cast<DiagnosticCategoryIntT>(getInfoCategory(e)),
                      static_cast<DiagnosticCategoryIntT>(getDiagnosticType(e))};
};
///////////////////////////////////////////////////////////////////////////////////
/* Forward decl of 'formatDiagnostic'. The message formatting vtable dispatch method. */
///////////////////////////////////////////////////////////////////////////////////

using DiagnosticDataUnionT =
    std::variant<std::int64_t, std::uint64_t,
                 std::string>;  // Union of allowed compiler message data types.
using DiagnosticDataBufferT =
    std::vector<DiagnosticDataUnionT>;  // Buffer of unions of compiler message data types.

// Internal vtable dispatch method for compiler messages.
//
// @warning DO NOT call directly - used only in Diagnostic::Format.
// @see definition further in this header file.
inline std::string formatDiagnostic(
    DiagnosticId id,
    const DiagnosticDataBufferT& data) noexcept;  // MUST be forward declared here.

/////////////////////////////////////////////
/* Compiler message structure definitions. */
/////////////////////////////////////////////

/// The fundamental compiler message type. !DO NOT construct directly, use makeDiagnostic method to
/// create a message.
///
/// @note All other message structures merely store and provide syntactic sugar for creating dynamic
/// message
///       return values.
struct Diagnostic {
  DiagnosticId id{};
  DiagnosticDataBufferT data{};

  /// Returns formatted message string based on current message data.
  inline std::string format() const noexcept;

  constexpr Diagnostic() noexcept = default;
  constexpr Diagnostic(Diagnostic&& other) noexcept = default;
  constexpr Diagnostic(const Diagnostic& other) noexcept = default;
  constexpr Diagnostic& operator=(Diagnostic&& other) = default;
  constexpr Diagnostic& operator=(const Diagnostic& other) = default;

  constexpr Diagnostic(DiagnosticId id, DiagnosticDataBufferT data = {}) : id(id), data(data) {}
  constexpr Diagnostic(eError id, DiagnosticDataBufferT data = {})
      : id(getDiagnosticId(id)), data(data) {}
  constexpr Diagnostic(eWarning id, DiagnosticDataBufferT data = {})
      : id(getDiagnosticId(id)), data(data) {}
  constexpr Diagnostic(eGuideline id, DiagnosticDataBufferT data = {})
      : id(getDiagnosticId(id)), data(data) {}
  constexpr Diagnostic(eInfo id, DiagnosticDataBufferT data = {})
      : id(getDiagnosticId(id)), data(data) {}

  constexpr Diagnostic(const std::string& message)
      : id(getDiagnosticId(eError::kError)), data({message}) {}
  constexpr Diagnostic(const char* message)
      : id(getDiagnosticId(eError::kError)), data({std::string{message}}) {}
};

class Diagnostics {
 private:
  std::unique_ptr<std::vector<Diagnostic>> diagnostics_;

 public:
  explicit constexpr Diagnostics() noexcept
      : diagnostics_(std::make_unique<std::vector<Diagnostic>>(std::vector<Diagnostic>{})) {}

  explicit constexpr Diagnostics(Diagnostic&& diagnostic) noexcept
      : diagnostics_(
            std::make_unique<std::vector<Diagnostic>>(std::vector<Diagnostic>{diagnostic})) {}

  explicit constexpr Diagnostics(const Diagnostic& diagnostic) noexcept
      : diagnostics_(
            std::make_unique<std::vector<Diagnostic>>(std::vector<Diagnostic>{diagnostic})) {}

  Diagnostics(Diagnostics&& other) noexcept : diagnostics_(std::move(other.diagnostics_)) {}

  Diagnostics(const Diagnostics& other)
      : diagnostics_(std::make_unique<std::vector<Diagnostic>>(*other.diagnostics_)) {}

  /// Returns formatted message strings separated by a newline.
  std::string format(const std::string& prefix = "") const noexcept {
    std::string buff{""};
    for (const auto& msg : *diagnostics_) {
      buff.append(prefix);
      buff.append(msg.format());
      buff.append("\n");
    }
    return buff;
  };

  /// @brief Append a range of diagnostics.
  constexpr void append(const Diagnostics& other) noexcept { 
    diagnostics_->append_range(std::ranges::subrange(other->cbegin(),other->cend()));
  }
   
  constexpr std::vector<Diagnostic>& operator*() { return *diagnostics_; }

  constexpr std::vector<Diagnostic>* operator->() { return diagnostics_.get(); }

  constexpr std::vector<Diagnostic>& operator*() const { return *diagnostics_; }

  constexpr std::vector<Diagnostic>* operator->() const { return diagnostics_.get(); }
};

//////////////////////////////////////////////////////////////////////
/* Forward declare all required formatDiagnostic method specializations. */
//////////////////////////////////////////////////////////////////////
//
// This is required or else the vtable method in formatDiagnostic cannot lookup the method defs.
// Since every format method signature is the same, we can pre-declare them using macros.
//
// @note : When adding new diagnostic enum entries, and the format method has not been defined. A
// compile time error will be triggered as all enum entries must have a formatter method defined.
// @important The typename T paramater must be present or else the enum value template will
// be decayed to an integral so equal values enums will trigger ODR/COMDAT error which is impossible
// to figure out! You will only get this error if you attempt using this header in multiple
// translation units. Generated eWarning formatDiagnostic template specialization decls.

template <typename T, eError DIAGNOSTIC>
inline std::string formatDiagnostic(const DiagnosticDataBufferT& data) noexcept;

template <typename T, eWarning DIAGNOSTIC>
inline std::string formatDiagnostic(const DiagnosticDataBufferT& data) noexcept;

template <typename T, eGuideline DIAGNOSTIC>
inline std::string formatDiagnostic(const DiagnosticDataBufferT& data) noexcept;

template <typename T, eInfo DIAGNOSTIC>
inline std::string formatDiagnostic(const DiagnosticDataBufferT& data) noexcept;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Define the manual formatting dispatch method formatDiagnostic(DiagnosticId id, const
 * DiagnosticDataBufferT& data) */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Declare all specializations here first. So they don't get triggered outside.
#define SSGC_LOCAL_MACRO_DeclareFormatDiagnosticSpecialization_eError(en) \
  template <>                                                             \
  inline std::string formatDiagnostic<eError, eError::en>(                \
      const DiagnosticDataBufferT& data) noexcept;

#define SSGC_LOCAL_MACRO_DeclareFormatDiagnosticSpecialization_eGuideline(en) \
  template <>                                                                 \
  inline std::string formatDiagnostic<eGuideline, eGuideline::en>(            \
      const DiagnosticDataBufferT& data) noexcept;

#define SSGC_LOCAL_MACRO_DeclareFormatDiagnosticSpecialization_eInfo(en) \
  template <>                                                            \
  inline std::string formatDiagnostic<eInfo, eInfo::en>(                 \
      const DiagnosticDataBufferT& data) noexcept;

#define SSGC_LOCAL_MACRO_DeclareFormatDiagnosticSpecialization_eWarning(en) \
  template <>                                                               \
  inline std::string formatDiagnostic<eWarning, eWarning::en>(              \
      const DiagnosticDataBufferT& data) noexcept;

SSGC_AppliedEnum_eError(SSGC_LOCAL_MACRO_DeclareFormatDiagnosticSpecialization_eError, , , );
SSGC_AppliedEnum_eGuideline(SSGC_LOCAL_MACRO_DeclareFormatDiagnosticSpecialization_eGuideline, ,
                            , );
SSGC_AppliedEnum_eInfo(SSGC_LOCAL_MACRO_DeclareFormatDiagnosticSpecialization_eInfo, , , );
SSGC_AppliedEnum_eWarning(SSGC_LOCAL_MACRO_DeclareFormatDiagnosticSpecialization_eWarning, , , );

// The 'manual vtable' for FormatClErr method. Dispatches to the specialized template
// implementations. !warning DO NOT call directly. Called only by 'Diagnostic.Format()'.
inline std::string formatDiagnostic(DiagnosticId id, const DiagnosticDataBufferT& data) noexcept {
#define SSGC_LOCAL_MACRO_DispatchDiagnosticFormatError(ec) \
  case ec:                                                 \
    return formatDiagnostic<eError, ec>(data);
#define SSGC_LOCAL_MACRO_DispatchDiagnosticFormatWarning(ec) \
  case ec:                                                   \
    return formatDiagnostic<eWarning, ec>(data);
#define SSGC_LOCAL_MACRO_DispatchDiagnosticFormatGuideline(ec) \
  case ec:                                                     \
    return formatDiagnostic<eGuideline, ec>(data);
#define SSGC_LOCAL_MACRO_DispatchDiagnosticFormatInfo(ec) \
  case ec:                                                \
    return formatDiagnostic<eInfo, ec>(data);

  // Make sure we are using the correct global specializations.
  using ::ssgc::diagnostic::formatDiagnostic;

  switch (static_cast<eDiagnosticType>(id.msg_type)) {
    case eDiagnosticType::kError:
      // Handle eError enumeration.
      {
        using enum eError;
        switch (static_cast<eError>(id.code)) {
          SSGC_AppliedEnum_eError(SSGC_LOCAL_MACRO_DispatchDiagnosticFormatError, , , );
          default:
            return "<invalid>";
        }
      }
    case eDiagnosticType::kWarning:

      // Handle eWarning enumeration.
      {
        using enum eWarning;
        switch (static_cast<eWarning>(id.code)) {
          SSGC_AppliedEnum_eWarning(SSGC_LOCAL_MACRO_DispatchDiagnosticFormatWarning, , , );
          default:
            return "<invalid>";
        }
      }

    case eDiagnosticType::kGuideline:
      // Handle eGuideline enumeration.
      {
        using enum eGuideline;
        switch (static_cast<eGuideline>(id.code)) {
          SSGC_AppliedEnum_eGuideline(SSGC_LOCAL_MACRO_DispatchDiagnosticFormatGuideline, , , );
          default:
            return "<invalid>";
        }
      }
    case eDiagnosticType::kInfo:
      // Handle eInfo enumeration.
      {
        using enum eInfo;
        switch (static_cast<eInfo>(id.code)) {
          SSGC_AppliedEnum_eInfo(SSGC_LOCAL_MACRO_DispatchDiagnosticFormatInfo, , , );
          default:
            return "<invalid>";
        }
      }

    default:
      return "[Uncategorized Compiler Message]";
  }

// undef function-local
#undef SSGC_LOCAL_MACRO_DispatchDiagnosticFormat
}

std::string Diagnostic::format() const noexcept {
  // debatable if this 'using' is necessary? may help avoid any attempted inheritance trickery.
  using ::ssgc::diagnostic::formatDiagnostic;  // make sure to use the global declaration.
  return formatDiagnostic(id, data);           // call the formatting dispatch method.
};

constexpr DiagnosticDataBufferT convertCppSourceLocationToDiagnosticData(
    const std::source_location& loc) noexcept {
  DiagnosticDataBufferT ret;
  ret.reserve(4);
  // Fill the vector in declaration order
  ret.push_back(std::string{loc.file_name()});      // file name (std::string_view)
  ret.push_back(std::uint64_t{loc.line()});         // line number (size_t)
  ret.push_back(std::uint64_t{loc.column()});       // column number (size_t)
  ret.push_back(std::string{loc.function_name()});  // function name (std::string_view)
  return ret;
}

inline std::string formatCppSourceLocationDiagnosticData(
    DiagnosticDataBufferT::const_iterator loc_begin,
    DiagnosticDataBufferT::const_iterator loc_end) {
  std::span<const DiagnosticDataUnionT> loc = {loc_begin, loc_end};
  return std::format("{{file : {}\nline : {}\ncolumn : {}\nfunction : {}}}",
                     std::get<std::string>(loc[0]), std::get<std::uint64_t>(loc[1]),
                     std::get<std::uint64_t>(loc[2]), std::get<std::string>(loc[3]));
}

constexpr std::string formatDiagnosticPrefix(eError e) {
  std::string ret{"["};
  ret += eDiagnosticTypeToCStr(getDiagnosticType(e));
  ret += "][";
  auto category = getErrorCategory(e);
  if (category != eErrorCategory::kNone) {
    ret += eErrorCategoryToCStr(category);
    ret += "][";
  }
  ret += eErrorToCStr(e);
  ret += "]: ";
  return ret;
}

constexpr std::string formatDiagnosticPrefix(eWarning e) {
  std::string ret{"["};
  ret += eDiagnosticTypeToCStr(getDiagnosticType(e));
  ret += "][";
  auto category = getWarningCategory(e);
  if (category != eWarningCategory::kNone) {
    ret += eWarningCategoryToCStr(category);
    ret += "][";
  }
  ret += eWarningToCStr(e);
  ret += "]: ";
  return ret;
}

constexpr std::string formatDiagnosticPrefix(eGuideline e) {
  std::string ret{"["};
  ret += eDiagnosticTypeToCStr(getDiagnosticType(e));
  ret += "][";
  auto category = getGuidelineCategory(e);
  if (category != eGuidelineCategory::kNone) {
    ret += eGuidelineCategoryToCStr(category);
    ret += "][";
  }
  ret += eGuidelineToCStr(e);
  ret += "]: ";
  return ret;
}

constexpr std::string formatDiagnosticPrefix(eInfo e) {
  std::string ret{"["};
  ret += eDiagnosticTypeToCStr(getDiagnosticType(e));
  ret += "][";
  auto category = getInfoCategory(e);
  if (category != eInfoCategory::kNone) {
    ret += eInfoCategoryToCStr(category);
    ret += "][";
  }
  ret += eInfoToCStr(e);
  ret += "]: ";
  return ret;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Define user-space macros to generate the correct template specializations for a given diagnostic
 * enum entry */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @macro CND_MM_CLMSG_FORMAT_FNSIG
/// @brief Generates a formatDiagnostic method template specialization signature.
/// @param en Enum Name.
/// @param ee Enum Entry.
///
/// Used to define a formatting method for a specific eDiagnostic id based on a diagnostic enum.
/// Method signature:
///   'std::string formatDiagnostic(const DiagnosticDataBufferT& data)'
/// Method Locals:
///   DIAGNOSTIC : the enum entry passed as a template argument.
///   data : will be the data contained in the Diagnostic which is being formatted.
#define CND_MM_CLMSG_FORMAT_FNSIG(en, ee) \
  template <en DIAGNOSTIC>                \
    requires(DIAGNOSTIC == en::ee)        \
  constexpr std::string formatDiagnostic(const DiagnosticDataBufferT& data) noexcept

/// @macro CND_MM_CLMSG_FORMAT_FNSIG
/// @brief Generates a makeDiagnostic method signature.
/// @param en Enum Name.
/// @param ee Enum Entry.
/// @param ... Make method arguments. Types may be anything as long as the developer can provide the
/// logic to
///              convert them into a DiagnosticDataBufferT.
///
/// Used to define compiler message creation method for a specific eDiagnostic id based on the enum
/// template parameter. Method signature:
///   'DiagnosticChain makeDiagnostic(...)'
///
/// Method Locals:
///   'DIAGNOSTIC' : the enum entry passed as a template argument.
#define CND_MM_CLMSG_MAKE_FNSIG(en, ee, ...) \
  template <en DIAGNOSTIC>                   \
    requires(DIAGNOSTIC == en::ee)           \
  constexpr DiagnosticChain makeDiagnostic(__VA_ARGS__) noexcept

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Define example make and formatting methods for the first entry of eError, eWarning, eGuideline
 * and eInfo */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// The following definitions server only as an example. Other specializations may be defined in
// 'user-space' from the context of the compiler_message_base implementation.
//
// While these method definitions may be defined after, they MUST all be available in the
// translation unit for the dispatch method to be to look them up.

template <>
std::string formatDiagnostic<eError, eError::kError>(const DiagnosticDataBufferT& data) noexcept {
  if (data.size() > 0) {
    return std::get<std::string>(data[0]);
  }
  return "Unknown Error";
}

template <>
std::string formatDiagnostic<eWarning, eWarning::kWarning>(
    const DiagnosticDataBufferT& data) noexcept {
  if (data.size() > 0) {
    return std::get<std::string>(data[0]);
  }
  return "Unknown Warning";
}
template <>
std::string formatDiagnostic<eGuideline, eGuideline::kGuideline>(
    const DiagnosticDataBufferT& data) noexcept {
  if (data.size() > 0) {
    return std::get<std::string>(data[0]);
  }
  return "Unknown Guide";
}
template <>
std::string formatDiagnostic<eInfo, eInfo::kInfo>(const DiagnosticDataBufferT& data) noexcept {
  if (data.size() > 0) {
    return std::get<std::string>(data[0]);
  }
  return "Unknown Diagnostic";
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Static asserts to validate we are able to use Diagnostic in a constexpr context. */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Assert we can access the last error code in a constexpr context.
static_assert(Diagnostic{eError::kError}.id.code == std::to_underlying(eError::kError));
static_assert(Diagnostic{eWarning::kWarning}.id.code == std::to_underlying(eWarning::kWarning));
static_assert(Diagnostic{eGuideline::kGuideline}.id.code ==
              std::to_underlying(eGuideline::kGuideline));
static_assert(Diagnostic{eInfo::kInfo}.id.code == std::to_underlying(eInfo::kInfo));

//// Assert that we can access the formatted string in a constexpr context.
// static_assert(Diagnostic{eError::kError}.format() == "Unknown Error");
// static_assert(Diagnostic{eWarning::kWarning}.format() == "Unknown Warning");
// static_assert(Diagnostic{eGuideline::kGuideline}.format() == "Unknown Guide");
// static_assert(Diagnostic{eInfo::kInfo}.format() == "Unknown Diagnostic");

}  // namespace ssgc::diagnostic
/// @} // end of cnd_compiler_cldev

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
//
// This program is free software : you can redistribute it and / or modify it
// under the terms of the GNU Affero General Public License as published by the
// Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////