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
/// @brief
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup cnd_compiler_cldev
/// @{
#pragma once
// clang-format off
#include "ccapi/CommonCppApi.hpp"
#include "compiler_utils/CompilerMessageBase.hpp"
#include "diagnostic/eClErr.hpp"
// clang-format on

#define CND_MM_CLMSG_MAKE_RETURN(...)                  \
  return ClMsgUnion {                                  \
    ClMsgNode {                                        \
      GetClMsgIdOf(THIS_MESSAGE_ENUM), { __VA_ARGS__ } \
    }                                                  \
  }
namespace cnd {
namespace cldev {
namespace clmsg {

using corevals::diagnostic::eClErr;
using std::get;                // For ClMsgDataUnionT accessing std::variant.
using std::holds_alternative;  // For ClMsgDataUnionT accessing std::variant.
using std::to_underlying;      // To get the diagnostic enum's underlying value.

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* kCompilerDevDebugError */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// Developer debug error. Has to be a macro to pass current source location at call site.
#define CND_ERROR_DEV_DEBUG(msg)                                                           \
  cnd::cldev::clmsg::MakeClMsg<cnd::corevals::diagnostic::eClErr::kCompilerDevDebugError>( \
      std::source_location{}.current(), msg)

CND_MM_CLMSG_MAKE_FNSIG(eClErr, kCompilerDevDebugError, const std::source_location& cpp_loc, const Str& message) {
  ClMsgDataBufferT data = ConvertCppSourceLocationToClMsgData(cpp_loc);
  data.push_back(message);
  CND_MM_CLMSG_MAKE_RETURN(data);
}

CND_MM_CLMSG_FORMAT_FNSIG(eClErr, kCompilerDevDebugError) {
  // Processing data:
  // file name (StrView)
  // line number (UI64)
  // column number (UI64)
  // function name (StrView)
  // message (Str)

  return "[kCompilerDevDebugError]\n" + FormatCppSourceLocationClMsgData(data) + "\n[" + get<Str>(data[4]) + "]";
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* kLexerUnclosedStringLiteral */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <eClErr THIS_MESSAGE_ENUM>
  requires(THIS_MESSAGE_ENUM == eClErr::kLexerUnclosedStringLiteral)
constexpr ClMsgUnion MakeClMsg() noexcept {
  return ClMsgUnion{ClMsgNode{GetClMsgIdOf(THIS_MESSAGE_ENUM), {}}};
}

template <eClErr THIS_MESSAGE_ENUM>
  requires(THIS_MESSAGE_ENUM == eClErr::kLexerUnclosedStringLiteral)
constexpr Str FormatClMsg(const ClMsgDataBufferT& data) noexcept {
  return "[kLexerUnclosedStringLiteral] String literal is not closed.";
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* kLexerUnclosedCharacterLiteral */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <eClErr THIS_MESSAGE_ENUM>
  requires(THIS_MESSAGE_ENUM == eClErr::kLexerUnclosedCharacterLiteral)
constexpr ClMsgUnion MakeClMsg() noexcept {
  return ClMsgUnion{ClMsgNode{GetClMsgIdOf(THIS_MESSAGE_ENUM), {}}};
}

template <eClErr THIS_MESSAGE_ENUM>
  requires(THIS_MESSAGE_ENUM == eClErr::kLexerUnclosedCharacterLiteral)
constexpr Str FormatClMsg(const ClMsgDataBufferT& data) noexcept {
  return "[kLexerUnclosedCharacterLiteral] Character literal must be closed.";
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* kLexerEmptyCharacterLiteral */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <eClErr THIS_MESSAGE_ENUM>
  requires(THIS_MESSAGE_ENUM == eClErr::kLexerEmptyCharacterLiteral)
constexpr ClMsgUnion MakeClMsg() noexcept {
  return ClMsgUnion{ClMsgNode{GetClMsgIdOf(THIS_MESSAGE_ENUM), {}}};
}

template <eClErr THIS_MESSAGE_ENUM>
  requires(THIS_MESSAGE_ENUM == eClErr::kLexerEmptyCharacterLiteral)
constexpr Str FormatClMsg(const ClMsgDataBufferT& data) noexcept {
  return "[kLexerEmptyCharacterLiteral] Empty character literal.";
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* kLexerUnknownScalarSuffix */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <eClErr THIS_MESSAGE_ENUM>
  requires(THIS_MESSAGE_ENUM == eClErr::kLexerUnknownNumericSuffix)
constexpr ClMsgUnion MakeClMsg(StrView suffix) noexcept {
  return ClMsgUnion{ClMsgNode{GetClMsgIdOf(THIS_MESSAGE_ENUM), {Str{suffix}}}};
}

template <eClErr THIS_MESSAGE_ENUM>
  requires(THIS_MESSAGE_ENUM == eClErr::kLexerUnknownNumericSuffix)
constexpr Str FormatClMsg(const ClMsgDataBufferT& data) noexcept {
  return std::format("[kLexerUnclosedStringLiteral] Unknown scalar size suffix '{}'", get<Str>(data[0]));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* kLexerInvalidPunctuator */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <eClErr THIS_MESSAGE_ENUM>
  requires(THIS_MESSAGE_ENUM == eClErr::kLexerInvalidPunctuator)
constexpr ClMsgUnion MakeClMsg(StrView punctuator) noexcept {
  return ClMsgUnion{ClMsgNode{GetClMsgIdOf(THIS_MESSAGE_ENUM), {Str{punctuator}}}};
}

template <eClErr THIS_MESSAGE_ENUM>
  requires(THIS_MESSAGE_ENUM == eClErr::kLexerInvalidPunctuator)
constexpr Str FormatClMsg(const ClMsgDataBufferT& data) noexcept {
  return std::format("[kLexerInvalidPunctuator] Invalid punctuator '{}'", get<Str>(data[0]));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* kParserExpectedDeclaration */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <eClErr THIS_MESSAGE_ENUM>
  requires(THIS_MESSAGE_ENUM == eClErr::kParserExpectedDeclaration)
constexpr ClMsgUnion MakeClMsg() noexcept {
  return ClMsgUnion{ClMsgNode{GetClMsgIdOf(THIS_MESSAGE_ENUM), {}}};
}

template <eClErr THIS_MESSAGE_ENUM>
  requires(THIS_MESSAGE_ENUM == eClErr::kParserExpectedDeclaration)
constexpr Str FormatClMsg(const ClMsgDataBufferT& data) noexcept {
  return "[kParserExpectedDeclaration] Parser expects a declarative statement at the pragmatic context level.";
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* kDriverInvalidArg */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <eClErr THIS_MESSAGE_ENUM>
  requires(THIS_MESSAGE_ENUM == eClErr::kDriverInvalidArg)
constexpr ClMsgUnion MakeClMsg(StrView msg) noexcept {
  return ClMsgUnion{ClMsgNode{GetClMsgIdOf(THIS_MESSAGE_ENUM), {Str{msg}}}};
}

template <eClErr THIS_MESSAGE_ENUM>
  requires(THIS_MESSAGE_ENUM == eClErr::kDriverInvalidArg)
constexpr Str FormatClMsg(const ClMsgDataBufferT& data) noexcept {
  return std::format("[kDriverInvalidArg][Invalid command line argument detected.]: '{}'", get<Str>(data[0]));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* kDriverFlagMustHavePostfix */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <eClErr THIS_MESSAGE_ENUM>
  requires(THIS_MESSAGE_ENUM == eClErr::kDriverFlagMustHavePostfix)
constexpr ClMsgUnion MakeClMsg(StrView flag_str, StrView expected, StrView got) noexcept {
  return {ClMsgNode{GetClMsgIdOf(THIS_MESSAGE_ENUM), {Str{flag_str}, Str{expected}, Str{got}}}};
}

template <eClErr THIS_MESSAGE_ENUM>
  requires(THIS_MESSAGE_ENUM == eClErr::kDriverFlagMustHavePostfix)
constexpr Str FormatClMsg(const ClMsgDataBufferT& data) noexcept {
  return std::format("Flag {} must be followed by: {}. Got : {}", std::get<Str>(data[0]), std::get<Str>(data[1]),
                     std::get<Str>(data[2]));
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* kDriverFlagInvalidArg */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <eClErr THIS_MESSAGE_ENUM>
  requires(THIS_MESSAGE_ENUM == eClErr::kDriverFlagInvalidArg)
constexpr ClMsgUnion MakeClMsg(StrView flag_str, StrView expected, StrView got) noexcept {
  return {ClMsgNode{GetClMsgIdOf(THIS_MESSAGE_ENUM), {Str{flag_str}, Str{expected}, Str{got}}}};
}

template <eClErr THIS_MESSAGE_ENUM>
  requires(THIS_MESSAGE_ENUM == eClErr::kDriverFlagInvalidArg)
constexpr Str FormatClMsg(const ClMsgDataBufferT& data) noexcept {
  return std::format("Flag {} is followed by invalid arg '{}'. Expected: {}", std::get<Str>(data[0]),
                     std::get<Str>(data[2]), std::get<Str>(data[1]));
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* kDriverFlagExpectedArgs */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <eClErr THIS_MESSAGE_ENUM>
  requires(THIS_MESSAGE_ENUM == eClErr::kDriverFlagExpectedArgs)
constexpr Str FormatClMsg(const ClMsgDataBufferT& data) noexcept {
  return std::format("Flag {} is followed by end of args or empty string. Expected: {}", std::get<Str>(data[0]),
                     std::get<Str>(data[1]));
};

template <eClErr THIS_MESSAGE_ENUM>
  requires(THIS_MESSAGE_ENUM == eClErr::kDriverFlagExpectedArgs)
constexpr ClMsgUnion MakeClMsg(StrView flag_str, StrView exp) noexcept {
  return {ClMsgNode{GetClMsgIdOf(THIS_MESSAGE_ENUM), {Str{flag_str}, Str{exp}}}};
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* kDriverFailedToRedirectStream */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <eClErr THIS_MESSAGE_ENUM>
  requires(THIS_MESSAGE_ENUM == eClErr::kDriverFailedToRedirectStream)
constexpr ClMsgUnion MakeClMsg(StrView flag, StrView file) noexcept {
  return {ClMsgNode{GetClMsgIdOf(THIS_MESSAGE_ENUM), {Str{flag}, Str{file}}}};
}

template <eClErr THIS_MESSAGE_ENUM>
  requires(THIS_MESSAGE_ENUM == eClErr::kDriverFailedToRedirectStream)
constexpr Str FormatClMsg(const ClMsgDataBufferT& data) noexcept {
  return std::format("While redirecting for '{}' ,a file could not be opened. File: {}", std::get<Str>(data[0]),
                     std::get<Str>(data[1]));
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* kFailedToReadFile */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <eClErr THIS_MESSAGE_ENUM>
  requires(THIS_MESSAGE_ENUM == eClErr::kDriverDeniedOverwrite)
constexpr Str FormatClMsg(const ClMsgDataBufferT& data) noexcept {
  return std::format(
      "'{}' file path already exists. Can't use existing path for {} if --no_overwrite is "
      "enabled.",
      std::get<Str>(data[1]), std::get<Str>(data[0]));
};

template <eClErr THIS_MESSAGE_ENUM>
  requires(THIS_MESSAGE_ENUM == eClErr::kDriverDeniedOverwrite)
constexpr ClMsgUnion MakeClMsg(StrView file, StrView flag) noexcept {
  return {ClMsgNode{GetClMsgIdOf(THIS_MESSAGE_ENUM), {Str{file}, Str{flag}}}};
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* kFailedToReadFile */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <eClErr THIS_MESSAGE_ENUM>
  requires(THIS_MESSAGE_ENUM == eClErr::kFailedToReadFile)
constexpr ClMsgUnion MakeClMsg(StrView file, StrView msg) noexcept {
  CND_MM_CLMSG_MAKE_RETURN(Str{file}, Str{msg});
}

template <eClErr THIS_MESSAGE_ENUM>
  requires(THIS_MESSAGE_ENUM == eClErr::kFailedToReadFile)
constexpr Str FormatClMsg(const ClMsgDataBufferT& data) noexcept {
  return std::format("[kFailedToReadFile] File: {} \nReason: {}.", std::get<Str>(data[0]), std::get<Str>(data[1]));
}

}  // namespace clmsg
}  // namespace cldev

// template <class T>
// using CompilerProcessResult = Ex<T, CompilerMessageBuffer>;
// using CompilerProcessFailure = CompilerProcessResult<CompilerMessageBuffer>;

}  // namespace cnd

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
