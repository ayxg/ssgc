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
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup cnd_compiler_cldev
/// @{
#pragma once
// clang-format off
#include "ccapi/CommonCppApi.hpp"
#include "diagnostic/eClErr.hpp"
#include "diagnostic/eClWarning.hpp"
#include "diagnostic/eClGuide.hpp"
#include "diagnostic/eClDiagnostic.hpp"
#include "diagnostic/eClMsgType.hpp"
#include "diagnostic/eClMsg.hpp"
#include "diagnostic/traitsof_eClDiagnostic.hpp"
#include "diagnostic/traitsof_eClErr.hpp"
#include "diagnostic/traitsof_eClWarning.hpp"
#include "diagnostic/traitsof_eClGuide.hpp"
#include "diagnostic/traitsof_eClMsgType.hpp"
// clang-format on

namespace cnd {
namespace cldev {
namespace clmsg {
using corevals::diagnostic::eClDiagnostic;
using corevals::diagnostic::eClErr;
using corevals::diagnostic::eClGuide;
using corevals::diagnostic::eClWarning;
using corevals::diagnostic::eClMsgType;
using corevals::diagnostic::eClMsg;
using corevals::diagnostic::GetClMsgType;

using std::get;                // For ClMsgDataUnionT accessing std::variant.
using std::holds_alternative;  // For ClMsgDataUnionT accessing std::variant.
using std::to_underlying;      // To get the diagnostic enum's underlying value.

/// Integral type which all message enums must fit in. Currently includes:
/// eClErr, eClWarning, eClGuide, eClDiagnostic.
using ClMsgCodeIntT = UI32;

/// Integral type which all message categories must fit in. Currently includes:
/// eClErrCategory, eClWarningCategory, eClGuideCategory, eClDiagnosticCategory.
using ClMsgCategoryIntT = UChar;

/// Integer type for constant embedded data in the message type. eg. is an error recoverable?
/// Interpretation of the data will depend on the message code and category.
using ClMsgParameterIntT = Short;

// Assert size of ClMsgId bitfield's integral types.
static_assert(sizeof(ClMsgCodeIntT) == 4 && "cnd::cldev::clmsg::ClMsgCodeIntT must be 4 bytes.");
static_assert(sizeof(ClMsgCategoryIntT) == 1 && "cnd::cldev::clmsg::ClMsgCategoryIntT must be 1 byte.");
static_assert(sizeof(ClMsgParameterIntT) == 2 && "cnd::cldev::clmsg::ClMsgParameterIntT must be 2 bytes.");

/////////////////////////////////////////////////////////////////////////////
/* Bitfield to identify compiler error messages. */
/////////////////////////////////////////////////////////////////////////////

/// A bitfield structure which identifies a compiler message.
struct ClMsgId {
  ClMsgCodeIntT code{0};            ///> Id of the message within the message type, maps to the diagnostic enums.
  ClMsgCategoryIntT category{0};    ///> Category within the message type, maps to the message category enums.
  ClMsgCategoryIntT msg_type{0};    ///> Message type, maps to eClMsgType
  ClMsgParameterIntT parameter{0};  ///> Placeholder for data dependant on code and category.

  /// Compares based on ClMsgId members 'code', 'category' and 'msg_type'; 'parameter' is ignored.
  constexpr Bool operator==(const ClMsgId& other) const {
    return code == other.code && category == other.category && msg_type == other.msg_type;
  }

  /// Compares based on ClMsgId members 'code', 'category' and 'msg_type'; 'parameter' is ignored.
  constexpr Bool operator!=(const ClMsgId& other) const { return !(*this == other); }
};
static_assert(sizeof(ClMsgId) == 8 && "cnd::cldev::clmsg::ClMsgId must be 8 bytes.");

/////////////////////////////////////////////////////////////////////////////
/* Methods for converting diagnostic enums to their corresponding ClMsgId. */
/////////////////////////////////////////////////////////////////////////////

// TODO: For now the parameter of the cl msg will always be 0 as I have not found a use for the data yet.
//       Later make sure to add the parameter retrieving logic to these methods if ever used.

constexpr ClMsgId GetClMsgIdOf(eClErr e) {
  return ClMsgId{static_cast<ClMsgCodeIntT>(e), static_cast<ClMsgCategoryIntT>(GetClErrCategory(e)),
                 static_cast<ClMsgCategoryIntT>(GetClMsgType(e))};
};

constexpr ClMsgId GetClMsgIdOf(eClWarning e) {
  return ClMsgId{static_cast<ClMsgCodeIntT>(e), static_cast<ClMsgCategoryIntT>(GetClWarningCategory(e)),
                 static_cast<ClMsgCategoryIntT>(GetClMsgType(e))};
};

constexpr ClMsgId GetClMsgIdOf(eClGuide e) {
  return ClMsgId{static_cast<ClMsgCodeIntT>(e), static_cast<ClMsgCategoryIntT>(GetClGuideCategory(e)),
                 static_cast<ClMsgCategoryIntT>(GetClMsgType(e))};
};

constexpr ClMsgId GetClMsgIdOf(eClDiagnostic e) {
  return ClMsgId{static_cast<ClMsgCodeIntT>(e), static_cast<ClMsgCategoryIntT>(GetClDiagnosticCategory(e)),
                 static_cast<ClMsgCategoryIntT>(GetClMsgType(e))};
};

///////////////////////////////////////////////////////////////////////////////////
/* Forward decl of 'FormatClMsg'. The message formatting vtable dispatch method. */
///////////////////////////////////////////////////////////////////////////////////

using ClMsgDataUnionT = ccapi::Var<I64, UI64, Str>;  // Union of allowed compiler message data types.
using ClMsgDataBufferT = Vec<ClMsgDataUnionT>;       // Buffer of unions of compiler message data types.

// Internal vtable dispatch method for compiler messages.
//
// @warning DO NOT call directly - used only in ClMsgNode::Format.
// @see definition further in this header file.
constexpr Str FormatClMsg(ClMsgId id, const ClMsgDataBufferT& data) noexcept;  // MUST be forward declared here.

/////////////////////////////////////////////
/* Compiler message structure definitions. */
/////////////////////////////////////////////

/// The fundamental compiler message type. !DO NOT construct directly, use MakeClMsg method to create a message.
///
/// @note All other message structures merely store and provide syntactic sugar for creating dynamic message
///       return values.
struct ClMsgNode {
  ClMsgId id;
  ClMsgDataBufferT data;

  /// Returns formatted message string based on current message data.
  constexpr Str Format() const noexcept {
    // debatable if this 'using' is necessary? may help avoid any attempted inheritance trickery.
    using ::cnd::cldev::clmsg::FormatClMsg;  // make sure to use the global declaration.
    return FormatClMsg(id, data);            // call the formatting dispatch method.
  };
};

struct ClMsgChain {
  Vec<ClMsgNode> messages;

  /// Returns formatted message strings separated by a newline.
  constexpr Str Format() const noexcept {
    Str buff{""};
    for (const auto& msg : messages) {
      buff.append(msg.Format());
      buff.append("\n");
    }
    return buff;
  };

  constexpr Bool IsEmpty() const { return messages.empty(); }
  constexpr void Clear() { return messages.clear(); }
  constexpr void PushBack(const ClMsgNode& e) { messages.push_back(e); }
  constexpr void PushBack(ClMsgNode&& e) { messages.push_back(forward<ClMsgNode>(e)); }
};

// underlying type of ClMsgUnion (usually std::variant).
using ClMsgUnionUnderlyingT = ccapi::Var<ClMsgNode, ClMsgChain>;

struct ClMsgUnion : ClMsgUnionUnderlyingT {
  using ClMsgUnionUnderlyingT::variant;  // C++23 allows inheriting from std::variant.

  constexpr Bool IsSingle() const noexcept { return holds_alternative<ClMsgNode>(*this); }
  constexpr Bool IsChain() const noexcept { return holds_alternative<ClMsgChain>(*this); }
  constexpr Bool IsEmpty() const noexcept {
    // If its a chain check the underlying vector.
    if (IsChain()) GetChain().IsEmpty();
    return false;  // If its a single it cant be empty.
  };

  constexpr ClMsgNode& GetSingle() noexcept { return get<ClMsgNode>(*this); };
  constexpr const ClMsgNode& GetSingle() const noexcept { return get<ClMsgNode>(*this); };
  constexpr ClMsgChain& GetChain() noexcept { return get<ClMsgChain>(*this); };
  constexpr const ClMsgChain& GetChain() const noexcept { return get<ClMsgChain>(*this); };

  constexpr Bool TransformToChain() noexcept {
    if (IsChain())
      return false;  // Already a chain.
    else             // Turn this into a chain and add the node as the first element.
      *this = ClMsgChain{{GetSingle()}};
  }

  constexpr Bool TransformToEmpty() noexcept {
    if (IsChain()) {
      if (GetChain().IsEmpty())  // Already empty
        return false;
      // Its a chain with messages, clear it.
      GetChain().Clear();
      return true;
    } else  // Turn this into a chain and add the node as the first element.
      *this = ClMsgChain{{GetSingle()}};
  }

  constexpr ClMsgId GetLastMessageId() const noexcept {
    if (IsSingle())
      return GetSingle().id;
    else
      return GetChain().messages.at(0).id;
  }

  constexpr ClMsgCodeIntT GetLastMessageCode() const noexcept {
    if (IsSingle())
      return GetSingle().id.code;
    else
      return GetChain().messages.back().id.code;
  }

  constexpr Str Format() const noexcept {
    return std::visit([](const auto& msg) { return msg.Format(); }, *this);
  };

  constexpr void PushBack(const ClMsgNode& e) {
    if (IsSingle()) TransformToChain();
    GetChain().PushBack(e);
  };

  constexpr void PushBack(ClMsgNode&& e) {
    if (IsSingle()) TransformToChain();
    GetChain().PushBack(forward<ClMsgNode>(e));
  };

  constexpr Bool PopBack();

  constexpr ClMsgNode& GetLastMessage() noexcept {
    if (IsSingle())
      return GetSingle();
    else
      return GetChain().messages.back();
  }
};

class ClMsgBuffer {
 public:
  explicit constexpr ClMsgBuffer(ClMsgNode&& msg_node) noexcept
      : messages_(make_unique<Vec<ClMsgUnion>>(Vec<ClMsgUnion>{msg_node})) {}

  explicit constexpr ClMsgBuffer(ClMsgChain&& msg_chain) noexcept
      : messages_(make_unique<Vec<ClMsgUnion>>(Vec<ClMsgUnion>{msg_chain})) {}

  explicit constexpr ClMsgBuffer(ClMsgUnion&& msg_union) noexcept
      : messages_(make_unique<Vec<ClMsgUnion>>(Vec<ClMsgUnion>{msg_union})) {}

  ClMsgBuffer(ClMsgBuffer&& other) : messages_(std::move(other.messages_)) {}
  ClMsgBuffer(const ClMsgBuffer& other) : messages_(make_unique<Vec<ClMsgUnion>>(*other.messages_)) {}

  constexpr Vec<ClMsgUnion>& Data() noexcept { return *messages_; }
  constexpr const Vec<ClMsgUnion>& Data() const noexcept { return *messages_; }

  constexpr Str Format() const noexcept {
    Str s;
    for (const auto& msg : *messages_) {
      s.append(msg.Format());
    }
    return s;
  }
  constexpr Str FormatLast() const noexcept { return messages_->back().Format(); }
  constexpr Str FormatLastNode() const noexcept {
    return messages_->back().IsSingle() ? messages_->back().Format()
                                        : messages_->back().GetChain().messages.back().Format();
  }

  constexpr ClMsgNode& GetLastMessage() noexcept { return messages_->back().GetLastMessage(); }
  constexpr ClMsgCodeIntT GetLastMessageCode() { return messages_->back().GetLastMessageCode(); }
  constexpr ClMsgId GetLastMessageId() const noexcept { return messages_->back().GetLastMessageId(); }

 private:
  UPtr<Vec<ClMsgUnion>> messages_;  // Not default constructible.
};

//////////////////////////////////////////////////////////////////////
/* Forward declare all required FormatClMsg method specializations. */
//////////////////////////////////////////////////////////////////////
//
// This is required or else the vtable method in FormatClErr cannot lookup the method defs. Note the use of
// CND_MM_CONST_ID_TAG to get the class enum symbol from the applied enum. Since every format method signature is the
// same, we can pre-declare them using macros.
//
// @note : When adding new diagnostic enum entries, and the format method has not been defined. A compile time error
//         will be triggered as all enum entries must have a formatter method defined.

// Generated eClWarning FormatClMsg template specialization decls.
#define CND_MM_LOCAL_CLMSG_ERROR_FORMAT_FNSIG_DECL(en)             \
  template <eClErr THIS_MESSAGE_ENUM>                              \
    requires(THIS_MESSAGE_ENUM == eClErr::CND_MM_CONST_ID_TAG(en)) \
  constexpr Str FormatClMsg(const ClMsgDataBufferT& data) noexcept;
// Call the applied enum macro for eClErr
CND_APPLIED_ENUM_eClErr(CND_MM_LOCAL_CLMSG_ERROR_FORMAT_FNSIG_DECL, , , );

// Generated eClWarning FormatClMsg template specialization decls.
#define CND_MM_LOCAL_CLMSG_WARNING_FORMAT_FNSIG_DECL(en)               \
  template <eClWarning THIS_MESSAGE_ENUM>                              \
    requires(THIS_MESSAGE_ENUM == eClWarning::CND_MM_CONST_ID_TAG(en)) \
  constexpr Str FormatClMsg(const ClMsgDataBufferT& data) noexcept;
CND_APPLIED_ENUM_eClWarning(CND_MM_LOCAL_CLMSG_WARNING_FORMAT_FNSIG_DECL, , , );

// Generated eClGuide FormatClMsg template specialization decls.
#define CND_MM_LOCAL_CLMSG_GUIDE_FORMAT_FNSIG_DECL(en)               \
  template <eClGuide THIS_MESSAGE_ENUM>                              \
    requires(THIS_MESSAGE_ENUM == eClGuide::CND_MM_CONST_ID_TAG(en)) \
  constexpr Str FormatClMsg(const ClMsgDataBufferT& data) noexcept;
CND_APPLIED_ENUM_eClGuide(CND_MM_LOCAL_CLMSG_GUIDE_FORMAT_FNSIG_DECL, , , );

// Generated eClDiagnostic FormatClMsg template specialization decls.
#define CND_MM_LOCAL_CLMSG_DIAGNOSTIC_FORMAT_FNSIG_DECL(en)               \
  template <eClDiagnostic THIS_MESSAGE_ENUM>                              \
    requires(THIS_MESSAGE_ENUM == eClDiagnostic::CND_MM_CONST_ID_TAG(en)) \
  constexpr Str FormatClMsg(const ClMsgDataBufferT& data) noexcept;
CND_APPLIED_ENUM_eClDiagnostic(CND_MM_LOCAL_CLMSG_DIAGNOSTIC_FORMAT_FNSIG_DECL, , , );

#undef CND_MM_LOCAL_CLMSG_ERROR_FORMAT_FNSIG_DECL       // undef local
#undef CND_MM_LOCAL_CLMSG_WARNING_FORMAT_FNSIG_DECL     // undef local
#undef CND_MM_LOCAL_CLMSG_GUIDE_FORMAT_FNSIG_DECL       // undef local
#undef CND_MM_LOCAL_CLMSG_DIAGNOSTIC_FORMAT_FNSIG_DECL  // undef local

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Define the manual formatting dispatch method FormatClMsg(ClMsgId id, const ClMsgDataBufferT& data) */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

// The 'manual vtable' for FormatClErr method. Dispatches to the specialized template implementations.
// !warning DO NOT call directly. Called only by 'ClMsgNode.Format()'.
constexpr Str FormatClMsg(ClMsgId id, const ClMsgDataBufferT& data) noexcept {
#define CND_MM_LOCAL_DISPATCH_MESSAGE_FORMAT(ec) \
  case CND_MM_CONST_ID_TAG(ec):                  \
    return FormatClMsg<CND_MM_CONST_ID_TAG(ec)>(data);

  // Make sure we are using the correct global specializations.
  using ::cnd::cldev::clmsg::FormatClMsg;

  switch (static_cast<eClMsgType>(id.msg_type)) {
    case eClMsgType::kError:
      // Handle eClErr enumeration.
      {
        using enum eClErr;
        switch (static_cast<eClErr>(id.code)) {
          CND_APPLIED_ENUM_eClErr(CND_MM_LOCAL_DISPATCH_MESSAGE_FORMAT, , , );
          default:
            return "<invalid>";
        }
      }
    case eClMsgType::kWarning:

      // Handle eClGuide enumeration.
      {
        using enum eClErr;
        switch (static_cast<eClErr>(id.code)) {
          CND_APPLIED_ENUM_eClErr(CND_MM_LOCAL_DISPATCH_MESSAGE_FORMAT, , , );
          default:
            return "<invalid>";
        }
      }

    case eClMsgType::kGuideline:
      // Handle eClGuide enumeration.
      {
        using enum eClErr;
        switch (static_cast<eClErr>(id.code)) {
          CND_APPLIED_ENUM_eClErr(CND_MM_LOCAL_DISPATCH_MESSAGE_FORMAT, , , );
          default:
            return "<invalid>";
        }
      }
    case eClMsgType::kDiagnostic:
      // Handle eClDiagnostic enumeration.
      {
        using enum eClErr;
        switch (static_cast<eClErr>(id.code)) {
          CND_APPLIED_ENUM_eClErr(CND_MM_LOCAL_DISPATCH_MESSAGE_FORMAT, , , );
          default:
            return "<invalid>";
        }
      }

    default:
      return "[Uncategorized Compiler Message]";
  }

#undef CND_MM_LOCAL_DISPATCH_MESSAGE_FORMAT  // undef function-local
}

constexpr ClMsgDataBufferT ConvertCppSourceLocationToClMsgData(const std::source_location& loc) noexcept {
  ClMsgDataBufferT ret;
  ret.reserve(4);
  // Fill the vector in declaration order
  ret.push_back(Str{loc.file_name()});      // file name (std::string_view)
  ret.push_back(UI64{loc.line()});          // line number (size_t)
  ret.push_back(UI64{loc.column()});        // column number (size_t)
  ret.push_back(Str{loc.function_name()});  // function name (std::string_view)
  return ret;
}

constexpr Str FormatCppSourceLocationClMsgData(const ClMsgDataBufferT& data) {
  Str ret{"[File: "};
  ret += get<Str>(data[0]);
  ret += "][Func: ";
  ret += get<Str>(data[3]);
  ret += "]";
  return ret;
}

/////////////////////////////////////////////////////////
/* Implement ClMsgChain, ClMsgUnion, and ClMsg classes */
/////////////////////////////////////////////////////////
#include "CompilerMessageBase.tpp"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Define user-space macros to generate the correct template specializations for a given diagnostic enum entry */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @macro CND_MM_CLMSG_FORMAT_FNSIG
/// @brief Generates a FormatClMsg method template specialization signature.
/// @param en Enum Name.
/// @param ee Enum Entry.
///
/// Used to define a formatting method for a specific eClMsg id based on a diagnostic enum.
/// Method signature:
///   'Str FormatClMsg(const ClMsgDataBufferT& data)'
/// Method Locals:
///   THIS_MESSAGE_ENUM : the enum entry passed as a template argument.
///   data : will be the data contained in the ClMsgNode which is being formatted.
#define CND_MM_CLMSG_FORMAT_FNSIG(en, ee) \
  template <en THIS_MESSAGE_ENUM>         \
    requires(THIS_MESSAGE_ENUM == en::ee) \
  constexpr Str FormatClMsg(const ClMsgDataBufferT& data) noexcept

/// @macro CND_MM_CLMSG_FORMAT_FNSIG
/// @brief Generates a MakeClMsg method signature.
/// @param en Enum Name.
/// @param ee Enum Entry.
/// @param ... Make method arguments. Types may be anything as long as the developer can provide the logic to
///              convert them into a ClMsgDataBufferT.
///
/// Used to define compiler message creation method for a specific eClMsg id based on the enum template parameter.
/// Method signature:
///   'ClMsgUnion MakeClMsg(...)'
///
/// Method Locals:
///   'THIS_MESSAGE_ENUM' : the enum entry passed as a template argument.
#define CND_MM_CLMSG_MAKE_FNSIG(en, ee, ...) \
  template <en THIS_MESSAGE_ENUM>            \
    requires(THIS_MESSAGE_ENUM == en::ee)    \
  constexpr ClMsgUnion MakeClMsg(__VA_ARGS__) noexcept

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Define example make and formatting methods for the first entry of eClErr, eClWarning, eClGuide and eClDiagnostic */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// The following definitions server only as an example. Other specializations may be defined in 'user-space' from the
// context of the compiler_message_base implementation.
//
// While these method definitions may be defined after, they MUST all be available in the translation unit for the
// dispatch method to be to look them up.
//
// @see below - an expanded example message make method for eClErr::kNoError.
template <eClErr THIS_MESSAGE_ENUM>
  requires(THIS_MESSAGE_ENUM == eClErr::kNoError)
constexpr ClMsgUnion MakeClMsg(/* no args : no message false positive */) noexcept {
  return ClMsgUnion{ClMsgNode{GetClMsgIdOf(THIS_MESSAGE_ENUM), ClMsgDataBufferT{}}};
}

// The following MakeClMsg definitions use CND_MM_CLMSG_MAKE_FNSIG to create the method.

CND_MM_CLMSG_MAKE_FNSIG(eClWarning, kNoWarning) {
  return ClMsgUnion{ClMsgNode{GetClMsgIdOf(THIS_MESSAGE_ENUM), ClMsgDataBufferT{}}};
}

CND_MM_CLMSG_MAKE_FNSIG(eClGuide, kNoGuide) {
  return ClMsgUnion{ClMsgNode{GetClMsgIdOf(THIS_MESSAGE_ENUM), ClMsgDataBufferT{}}};
}

CND_MM_CLMSG_MAKE_FNSIG(eClDiagnostic, kNoDiagnostic) {
  return ClMsgUnion{ClMsgNode{GetClMsgIdOf(THIS_MESSAGE_ENUM), ClMsgDataBufferT{}}};
}

// Formatting methods can be defined in a similar fashion using CND_MM_CLMSG_FORMAT_FNSIG.
// @see below - an expanded example message formatting method for eClErr::kNoError.

// Inline expanded example for brevity.
template <eClErr THIS_MESSAGE_ENUM>
  requires(THIS_MESSAGE_ENUM == eClErr::kNoError)
constexpr Str FormatClMsg(const ClMsgDataBufferT& data) noexcept {
  return "[False Positive Error]";
}

CND_MM_CLMSG_FORMAT_FNSIG(eClWarning, kNoWarning) { return "[False Positive Warning]"; }
CND_MM_CLMSG_FORMAT_FNSIG(eClGuide, kNoGuide) { return "[False Positive Guide]"; }
CND_MM_CLMSG_FORMAT_FNSIG(eClDiagnostic, kNoDiagnostic) { return "[False Positive Diagnostic]"; }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Static asserts to validate we are able to use ClMsg in a constexpr context. */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define CND_STATIC_UNIT_TEST_COMPILER_MESSAGE_BASE true  // set false to disable static asserts.
#if !defined(CND_COMPILER_IMPL_DISABLE_STATIC_UNIT_TESTS) && CND_STATIC_UNIT_TEST_COMPILER_MESSAGE_BASE

// Assert we can access the last error code in a constexpr context.
static_assert(MakeClMsg<eClErr::kNoError>().GetLastMessageCode() == to_underlying(eClErr::kNoError));
static_assert(MakeClMsg<eClWarning::kNoWarning>().GetLastMessageCode() == to_underlying(eClWarning::kNoWarning));
static_assert(MakeClMsg<eClGuide::kNoGuide>().GetLastMessageCode() == to_underlying(eClGuide::kNoGuide));
static_assert(MakeClMsg<eClDiagnostic::kNoDiagnostic>().GetLastMessageCode() ==
              to_underlying(eClDiagnostic::kNoDiagnostic));

// Assert that we can access the formatted string in a constexpr context.
static_assert(FormatClMsg<eClErr::kNoError>(MakeClMsg<eClErr::kNoError>().GetSingle().data) ==
              "[False Positive Error]");
static_assert(FormatClMsg<eClWarning::kNoWarning>(MakeClMsg<eClWarning::kNoWarning>().GetSingle().data) ==
              "[False Positive Warning]");
static_assert(FormatClMsg<eClGuide::kNoGuide>(MakeClMsg<eClGuide::kNoGuide>().GetSingle().data) ==
              "[False Positive Guide]");
static_assert(FormatClMsg<eClDiagnostic::kNoDiagnostic>(MakeClMsg<eClDiagnostic::kNoDiagnostic>().GetSingle().data) ==
              "[False Positive Diagnostic]");

#endif  // CND_STATIC_UNIT_TEST_COMPILER_MESSAGE_BASE

}  // namespace clmsg
}  // namespace cldev
}  // namespace cnd

/// @} // end of cnd_compiler_cldev

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @devlog Compiler Error/Diagnostic Messages
///
/// -[01/13/2025] There may potentially be thousands of unique diagnostic messages that the compiler can generate. We
/// need a way to programmatically enumerate all the diagnostic messages, and to be able to pass data to a predefined
/// formatting method of a specific given diagnostic.
///
/// The main requirements:
///
/// -[01/13/2025] Do NOT create a class or class template specialization for each message. We don't want to overload
/// the type system and bloat the executable. There will be more important uses for types than error messages.
///
/// -[01/13/2025] Each message will depend the message identity, an array of data, a creation method, and a formatting
/// method.
///
/// -[01/13/2025] The data a message is allowed to receive must be one of CompilerMessageDataTypeUnion, which is the
/// union of allowable types. These types may include known C++ types and any 'corevals' namespace types.
///
/// -[01/13/2025] From the compiler developer's user perspective we need to able to get intellisense/compile time
/// validation of the error creation and formatting methods. eg. 'MakeMsg' must have a concrete set of named overloads.
///
/// -[01/13/2025] The error structure should be const-evaluable up to the error code. Since we cannot create formatted
/// dynamic output at compile time. In a constexpr context we can only access the formatted data as an intermediate
/// value.
///
/// -[01/13/2025] Compiler messages must be able to receive a variable quantity and types of data. The allowable data
/// types may be enumerated, the quantity may not. This data is stored in the message and formatted into a string when
/// needed.
///
/// -[01/13/2025] The ClMsg structure should be tiny(8 bytes) to minimize stack space. When used as an alternative for
/// variant expected , optional: ClMsg should not cause an increase in the union size. The expectation is that IF a
/// ClMsg is returned then the program has already encountered an invalid or error state, or we are printing verbose
/// diagnostics in which case unpacking speed does not matter. As such we can use many layers of indirection to
/// dynamically allocate a message upon construction. The overall underlying data structure of ClMsg currently is:
///    'std::unique_ptr<std::vector<std::variant<ClMsgNode,std::vector<ClMsgNode>>>>'
/// This is all wrapped up and separated into the ClMsgNode, ClMsgChain, ClMsgUnion and ClMsg classes.
///
/// Implementation details:
///
/// -[01/13/2025] We can define a method to create a message constrained to a given message enum entry by using
/// 'requires'. The message category and type will be available at compile time based on the enum entry. By static
/// casting the enum to ClMsgCodeIntT or ClMsgCategoryIntT, we turn this into a pseudo-'compile time virtual method'.
/// Intellisense should be able to display the required arguments to create the valid data for this message.
///
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