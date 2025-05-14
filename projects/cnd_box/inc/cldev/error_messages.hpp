///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the Apache License, Version 2.0(the "License");
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
#include "use_ccapi.hpp"
#include "use_corevals.hpp"
#include "compiler_message_base.hpp"
// clang-format on

#define CND_MM_CLMSG_MAKE_RETURN(...)                  \
  return ClMsgUnion {                                \
    ClMsgNode { GetClMsgIdOf(THIS_MESSAGE_ENUM), {__VA_ARGS__} } \
  }
namespace cnd {
namespace cldev {
namespace clmsg {

using std::get;                // For ClMsgDataUnionT accessing std::variant.
using std::holds_alternative;  // For ClMsgDataUnionT accessing std::variant.
using std::to_underlying;      // To get the diagnostic enum's underlying value.

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* kCompilerDevDebugError */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CND_MM_CLMSG_MAKE_FNSIG(eClErr, kCompilerDevDebugError, const std::source_location & cpp_loc, const Str& message) {
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

  return "[kCompilerDevDebugError]" + FormatCppSourceLocationClMsgData(data) + "[" + get<Str>(data[4]) + "]";
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* kParserExpectedDeclaration */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CND_MM_CLMSG_MAKE_FNSIG(eClErr, kParserExpectedDeclaration) {
  return ClMsgUnion{ClMsgNode{GetClMsgIdOf(THIS_MESSAGE_ENUM), {}}};
}

CND_MM_CLMSG_FORMAT_FNSIG(eClErr, kParserExpectedDeclaration) {
  return "[kParserExpectedDeclaration] Parser expects a declarative statement at the pragmatic context level.";
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* kDriverInvalidArg */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CND_MM_CLMSG_MAKE_FNSIG(eClErr, kDriverInvalidArg, StrView msg) {
  return ClMsgUnion{ClMsgNode{GetClMsgIdOf(THIS_MESSAGE_ENUM), {Str{msg}}}};
}

CND_MM_CLMSG_FORMAT_FNSIG(eClErr, kDriverInvalidArg) {
  return std::format("[kDriverInvalidArg][Invalid command line argument detected.]: '{}'", get<Str>(data[0]));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* kDriverFlagMustHavePostfix */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CND_MM_CLMSG_MAKE_FNSIG(eClErr, kDriverFlagMustHavePostfix, StrView flag_str, StrView expected, StrView got) {
  return {ClMsgNode{GetClMsgIdOf(THIS_MESSAGE_ENUM), {Str{flag_str}, Str{expected}, Str{got}}}};
}

CND_MM_CLMSG_FORMAT_FNSIG(eClErr, kDriverFlagMustHavePostfix) {
  return std::format("Flag {} must be followed by: {}. Got : {}", std::get<Str>(data[0]), std::get<Str>(data[1]),
                     std::get<Str>(data[2]));
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* kDriverFlagInvalidArg */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CND_MM_CLMSG_MAKE_FNSIG(eClErr, kDriverFlagInvalidArg, StrView flag_str, StrView expected, StrView got) {
  return {ClMsgNode{GetClMsgIdOf(THIS_MESSAGE_ENUM), {Str{flag_str}, Str{expected}, Str{got}}}};
}

CND_MM_CLMSG_FORMAT_FNSIG(eClErr, kDriverFlagInvalidArg) {
  return std::format("Flag {} is followed by invalid arg '{}'. Expected: {}", std::get<Str>(data[0]),
                     std::get<Str>(data[2]), std::get<Str>(data[1]));
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* kDriverFlagExpectedArgs */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CND_MM_CLMSG_FORMAT_FNSIG(eClErr, kDriverFlagExpectedArgs) {
  return std::format("Flag {} is followed by end of args or empty string. Expected: {}", std::get<Str>(data[0]),
                     std::get<Str>(data[1]));
};

CND_MM_CLMSG_MAKE_FNSIG(eClErr, kDriverFlagExpectedArgs, StrView flag_str, StrView exp) {
  return {ClMsgNode{GetClMsgIdOf(THIS_MESSAGE_ENUM), {Str{flag_str}, Str{exp}}}};
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* kDriverFailedToRedirectStream */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CND_MM_CLMSG_MAKE_FNSIG(eClErr, kDriverFailedToRedirectStream, StrView flag, StrView file) {
  return {ClMsgNode{GetClMsgIdOf(THIS_MESSAGE_ENUM), {Str{flag}, Str{file}}}};
}

CND_MM_CLMSG_FORMAT_FNSIG(eClErr, kDriverFailedToRedirectStream) {
  return std::format("While redirecting for '{}' ,a file could not be opened. File: {}", std::get<Str>(data[0]),
                     std::get<Str>(data[1]));
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* kFailedToReadFile */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CND_MM_CLMSG_FORMAT_FNSIG(eClErr, kDriverDeniedOverwrite) {
  return std::format(
      "'{}' file path already exists. Can't use existing path for {} if --no_overwrite is "
      "enabled.",
      std::get<Str>(data[1]), std::get<Str>(data[0]));
};

CND_MM_CLMSG_MAKE_FNSIG(eClErr, kDriverDeniedOverwrite, StrView file, StrView flag) {
  return {ClMsgNode{GetClMsgIdOf(THIS_MESSAGE_ENUM), {Str{file}, Str{flag}}}};
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* kFailedToReadFile */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CND_MM_CLMSG_MAKE_FNSIG(eClErr, kFailedToReadFile, StrView file, StrView msg) {
  CND_MM_CLMSG_MAKE_RETURN(Str{file}, Str{msg});
}

CND_MM_CLMSG_FORMAT_FNSIG(eClErr, kFailedToReadFile) {
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
// Licensed under the Apache License, Version 2.0(the "License"); you may not use this file except in compliance with
// the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on
// an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
