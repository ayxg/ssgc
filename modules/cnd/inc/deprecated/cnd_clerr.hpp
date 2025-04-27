///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup
/// @brief
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup cnd_compiler_tools
/// @{

#pragma once
// clang-format off
#include "cnd_implicit.hpp"
#include "cnd_constdef.hpp"
#include "cnd_tk.hpp"
// clang-format on

namespace cnd::clerr {

using ClErrDataVariant = Var<I64, Ui64, Str>;
using ClErrData = Vec<ClErrDataVariant>;
struct ClErrNode;
struct ClErrChain;
class ClErr;
using ClErrVariant = Var<ClErrNode, ClErrChain>;
using std::format;
using std::get;

CND_CX Str eClErrFormat(eClErr code, const Vec<ClErrDataVariant>& data);

struct ClErrNode {
  CND_CX Str Format() const;
  eClErr code;
  Vec<ClErrDataVariant> data;
};

struct ClErrChain {
  Vec<ClErrNode> errors;
};

class ClErr {
  Uptr<ClErrVariant> err{nullptr};

 public:
  CND_CX bool IsSingle() const;
  CND_CX bool IsChain() const;
  CND_CX bool IsEmpty() const;

  CND_CX ClErrNode& GetSingle();
  CND_CX const ClErrNode& GetSingle() const;
  CND_CX ClErrChain& GetChain();
  CND_CX const ClErrChain& GetChain() const;

  CND_CX void PushError(const ClErrNode& e);
  CND_CX void PushError(ClErrNode&& e);
  CND_CX void PopError();
  CND_CX Str Format() const;
  CND_CX int LastErrorCode() const;

 public:
  CND_CX ClErr() = default;
  // CND_CX ClErr(ClErrNode&& e);
  // CND_CX ClErr(SameAs<ClErrNode> auto&&... es);
  // CND_CX ClErr(const ClErrNode& e);
  // CND_CX ClErr(const SameAs<ClErrNode> auto&... es);
  // CND_CX ClErr(Uptr<ClErrVariant>&& e);
  // CND_CX ClErr(SameAs<Uptr<ClErrVariant>> auto&&... es);
  // CND_CX ClErr(const Uptr<ClErrVariant>& e);
  // CND_CX ClErr(const SameAs<Uptr<ClErrVariant>> auto&... es);
  CND_CX ClErr(ClErr&&) noexcept = default;
  CND_CX ClErr& operator=(ClErr&&) noexcept = default;
  CND_CX ClErr(const ClErr& e) : err(make_unique<ClErrVariant>(*e.err)) {}
  CND_CX ClErr(ClErrNode&& e) : err(make_unique<ClErrVariant>(ClErrVariant{e})) {}
  // CND_CX ClErr(SameAs<ClErrNode> auto&&... es)
  //     : err(make_unique<ClErrVariant>(es...)) {}
  // CND_CX ClErr(const ClErrNode& e) : err(make_unique<ClErrVariant>(e)) {}
  // CND_CX ClErr(const SameAs<ClErrNode> auto&... es)
  //     : err(make_unique<ClErrVariant>(es...)) {}
  // CND_CX ClErr(Uptr<ClErrVariant>&& e)
  //     : err(make_unique<ClErrVariant>(e)) {}
  // CND_CX ClErr(SameAs<Uptr<ClErrVariant>> auto&&... es)
  //     : err(make_unique<ClErrVariant>(es...)) {}
  // CND_CX ClErr(const Uptr<ClErrVariant>& e)
  //     : err(make_unique<ClErrVariant>(e)) {}
  // CND_CX ClErr(const SameAs<Uptr<ClErrVariant>> auto&... es)
  //     : err(make_unique<ClErrVariant>(es...)) {}
};

#define CND_DEFINE_ERROR_FORMAT_IMPL(ec) \
  template <eClErr CODE>                 \
    requires(CODE == ec)                 \
  CND_CX Str FormatClErr(const ClErrData& data)

#define CND_DEFINE_ERROR_MAKE_IMPL(ec) \
  template <eClErr CODE>               \
    requires(CODE == ec)               \
  CND_CX ClErrNode MakeClErr

// Error Maker Impls : ClErrNode MakeClErr(...)

CND_DEFINE_ERROR_MAKE_IMPL(eClErr::kCustomError)(StrView msg) { return {eClErr::kCustomError, {msg}}; }

CND_DEFINE_ERROR_MAKE_IMPL(eClErr::kCustomError)() { return {eClErr::kCustomError, {}}; }

CND_DEFINE_ERROR_MAKE_IMPL(eClErr::kNoError)() { return {eClErr::kNoError, {}}; }

CND_DEFINE_ERROR_MAKE_IMPL(eClErr::kNotImplemented)(StrView feature) {
  return {eClErr::kNotImplemented, {Str{feature}}};
}

CND_DEFINE_ERROR_MAKE_IMPL(eClErr::kNotImplemented)() { return {eClErr::kNotImplemented, {}}; }

CND_DEFINE_ERROR_MAKE_IMPL(eClErr::kFailedToReadFile)(StrView file, StrView reason) {
  return {eClErr::kFailedToReadFile, {Str(file), Str(reason)}};
}

CND_DEFINE_ERROR_MAKE_IMPL(eClErr::kUnknownSrcChar)(char c) { return {eClErr::kUnknownSrcChar, {Str(1, c)}}; }

CND_DEFINE_ERROR_MAKE_IMPL(eClErr::kInvalidCliArg)(StrView c) { return {eClErr::kInvalidCliArg, {Str{c}}}; }

CND_DEFINE_ERROR_MAKE_IMPL(eClErr::kCliFlagMustHavePostfix)(StrView flag_str, StrView expected, StrView got) {
  return {eClErr::kCliFlagMustHavePostfix, {Str{flag_str}, Str{expected}, Str{got}}};
}

CND_DEFINE_ERROR_MAKE_IMPL(eClErr::kCliFlagExpectedArgs)(StrView flag_str, StrView exp) {
  return {eClErr::kCliFlagExpectedArgs, {Str{flag_str}, Str{exp}}};
}

CND_DEFINE_ERROR_MAKE_IMPL(eClErr::kCliFlagInvalidArg)(StrView flag_str, StrView expected, StrView got) {
  return {eClErr::kCliFlagInvalidArg, {Str{flag_str}, Str{expected}, Str{got}}};
}

CND_DEFINE_ERROR_MAKE_IMPL(eClErr::kCliDeniedOverwrite)(StrView file, StrView flag) {
  return {eClErr::kCliDeniedOverwrite, {Str{file}, Str{flag}}};
}

CND_DEFINE_ERROR_MAKE_IMPL(eClErr::kCliFailedToRedirectStream)(StrView flag, StrView file) {
  return {eClErr::kCliFailedToRedirectStream, {Str{flag}, Str{file}}};
}

template <eClErr CODE>
  requires(CODE == eClErr::kParserExpectedOpeningScope)
constexpr ClErrNode MakeClErr(const Tk& bad_tk, eTk expected_tk_type) {
  return {eClErr::kParserExpectedOpeningScope,
          {bad_tk.Literal(), bad_tk.File(), bad_tk.BegLine(), bad_tk.BegCol(), eTkToCStr(expected_tk_type)}};
}

CND_DEFINE_ERROR_MAKE_IMPL(eClErr::kParserOpeningScopeAtEof)(const Tk& open_tk) {
  return {eClErr::kParserOpeningScopeAtEof, {open_tk.Literal(), open_tk.File(), open_tk.BegLine(), open_tk.BegCol()}};
}

CND_DEFINE_ERROR_MAKE_IMPL(eClErr::kParserClosingScopeBeforeOpen)(const Tk& close_tk, eTk expected) {
  return {eClErr::kParserClosingScopeBeforeOpen,
          {close_tk.Literal(), close_tk.File(), close_tk.BegLine(), close_tk.BegCol(), expected}};
}

CND_DEFINE_ERROR_MAKE_IMPL(eClErr::kParserMismatchedParenScope)(const Tk& open_tk, const Tk& close_tk) {
  return {eClErr::kParserMismatchedScope,
          {open_tk.Literal(), open_tk.File(), open_tk.BegLine(), open_tk.BegCol(), close_tk.Literal(), close_tk.File(),
           close_tk.BegLine(), close_tk.BegCol()}};
}

CND_DEFINE_ERROR_MAKE_IMPL(eClErr::kParserMismatchedBracketScope)(const Tk& open_tk, const Tk& close_tk) {
  return {eClErr::kParserMismatchedScope,
          {open_tk.Literal(), open_tk.File(), open_tk.BegLine(), open_tk.BegCol(), close_tk.Literal(), close_tk.File(),
           close_tk.BegLine(), close_tk.BegCol()}};
}

CND_DEFINE_ERROR_MAKE_IMPL(eClErr::kParserMismatchedBraceScope)(const Tk& open_tk, const Tk& close_tk) {
  return {eClErr::kParserMismatchedScope,
          {open_tk.Literal(), open_tk.File(), open_tk.BegLine(), open_tk.BegCol(), close_tk.Literal(), close_tk.File(),
           close_tk.BegLine(), close_tk.BegCol()}};
}

// Error Formatter Impls : std::string FormatClErr(...)
// For each enum of eClErr defined in "constdef.hpp", define a format function which interprets the data
// created in the make impl and returns a formatted string.
// !!warning: Be sure that all format methods match the corresponding make methods in terms of
//            the data they expect- or else out of range errors will occur!
CND_DEFINE_ERROR_FORMAT_IMPL(eClErr::kCustomError) {
  return std::format("Uncategorized error:{}.", std::get<Str>(data[0]));
}

CND_DEFINE_ERROR_FORMAT_IMPL(eClErr::kNoError) { return "False Positive Error."; }

CND_DEFINE_ERROR_FORMAT_IMPL(eClErr::kNotImplemented) {
  if (data.empty()) return "[Unspecified Feature]: Not implemented.";
  return std::format("[{}]: Not implemented.", std::get<Str>(data[0]));
}

CND_DEFINE_ERROR_FORMAT_IMPL(eClErr::kFailedToReadFile) {
  return std::format("Could not load file {} because: {}.", std::get<Str>(data[0]), std::get<Str>(data[1]));
};

CND_DEFINE_ERROR_FORMAT_IMPL(eClErr::kUnknownSrcChar) {
  return std::format("Invalid character '{}' detected in source file.", std::get<Str>(data[0]));
};

CND_DEFINE_ERROR_FORMAT_IMPL(eClErr::kInvalidCliArg) {
  return std::format("Invalid command line argument detected: '{}'", std::get<Str>(data[0]));
};

CND_DEFINE_ERROR_FORMAT_IMPL(eClErr::kCliFlagMustHavePostfix) {
  return std::format("Flag {} must be followed by: {}. Got : {}", std::get<Str>(data[0]), std::get<Str>(data[1]),
                     std::get<Str>(data[2]));
};

CND_DEFINE_ERROR_FORMAT_IMPL(eClErr::kCliFlagExpectedArgs) {
  return std::format("Flag {} is followed by end of args or empty string. Expected: {}", std::get<Str>(data[0]),
                     std::get<Str>(data[1]));
};

CND_DEFINE_ERROR_FORMAT_IMPL(eClErr::kCliFlagInvalidArg) {
  return std::format("Flag {} is followed by invalid arg '{}'. Expected: {}", std::get<Str>(data[0]),
                     std::get<Str>(data[2]), std::get<Str>(data[1]));
};

CND_DEFINE_ERROR_FORMAT_IMPL(eClErr::kCliDeniedOverwrite) {
  return std::format(
      "'{}' file path already exists. Can't use existing path for {} if --no_overwrite is "
      "enabled.",
      std::get<Str>(data[1]), std::get<Str>(data[0]));
};

CND_DEFINE_ERROR_FORMAT_IMPL(eClErr::kCliFailedToRedirectStream) {
  return std::format("While redirecting for '{}' ,a file could not be opened. File: {}", std::get<Str>(data[0]),
                     std::get<Str>(data[1]));
};

CND_DEFINE_ERROR_FORMAT_IMPL(eClErr::kParserExpectedOpeningScope) {
  return std::format("Opening lexical scope token \"{}\" expected.\nInvalid Token:{}\nFile:{}\nLine:{} Col:{}",
                     get<Str>(data[0]), get<Size>(data[1]), get<Size>(data[2]), get<Size>(data[3]), get<Str>(data[4]));
}

CND_DEFINE_ERROR_FORMAT_IMPL(eClErr::kParserOpeningScopeAtEof) {
  return std::format(
      "Lexical opening scope token detected at end of a file. Therefore, the scope cannot possibly be closed.\nInvalid "
      "Token:{}\nFile:{}\nLine:{} Col:{}",
      get<Str>(data[0]), get<Size>(data[1]), get<Size>(data[2]), get<Size>(data[3]));
}

CND_DEFINE_ERROR_FORMAT_IMPL(eClErr::kParserClosingScopeBeforeOpen) {
  return std::format(
      "Lexical opening scope token detected at end of a file. Therefore, the scope cannot possibly be closed.\nInvalid "
      "Token:{}\nFile:{}\nLine:{} Col:{}",
      get<Str>(data[0]), get<Size>(data[1]), get<Size>(data[2]), get<Size>(data[3]));
}

CND_DEFINE_ERROR_FORMAT_IMPL(eClErr::kParserMismatchedParenScope) {
  return std::format(
      "Lexical opening scope token detected at end of a file. Therefore, the scope cannot possibly be closed.\nInvalid "
      "Token:{}\nFile:{}\nLine:{} Col:{}",
      get<Str>(data[0]), get<Size>(data[1]), get<Size>(data[2]), get<Size>(data[3]));
}

CND_DEFINE_ERROR_FORMAT_IMPL(eClErr::kParserMismatchedBracketScope) {
  return std::format(
      "Lexical opening scope token detected at end of a file. Therefore, the scope cannot possibly be closed.\nInvalid "
      "Token:{}\nFile:{}\nLine:{} Col:{}",
      get<Str>(data[0]), get<Size>(data[1]), get<Size>(data[2]), get<Size>(data[3]));
}

CND_DEFINE_ERROR_FORMAT_IMPL(eClErr::kParserMismatchedBraceScope) {
  return std::format(
      "Lexical opening scope token detected at end of a file. Therefore, the scope cannot possibly be closed.\nInvalid "
      "Token:{}\nFile:{}\nLine:{} Col:{}",
      get<Str>(data[0]), get<Size>(data[1]), get<Size>(data[2]), get<Size>(data[3]));
}

#undef CND_DEFINE_ERROR_FORMAT_IMPL
#undef CND_DEFINE_ERROR_MAKE_IMPL

// Dispatches to the above implementations.
CND_CX Str eClErrFormat(eClErr code, const ClErrData& data) {
#define CND_DISPATCH_ERROR_FORMAT(ec) \
  case CND_MM_CONST_ID_TAG(ec):       \
    return FormatClErr<CND_MM_CONST_ID_TAG(ec)>(data);
  using enum eClErr;
  switch (code) {
    CND_APPLIED_ENUM_eClErr(CND_DISPATCH_ERROR_FORMAT, , , );
    default:
      return "<invalid>";
  }
#undef CND_DISPATCH_ERROR_FORMAT
}

template <class T>
using ClRes = Ex<T, ClErr>;
using ClFail = Unex<ClErr>;

CND_CX ClFail Fail(ClErrNode&& err) { return ClFail{std::forward<ClErrNode>(err)}; }
CND_CX ClFail Fail(ClErr&& err) { return ClFail{std::forward<ClErr>(err)}; }
CND_CX Str ClErrNode::Format() const { return eClErrFormat(code, data); }

CND_CX bool ClErr::IsSingle() const { return std::holds_alternative<ClErrNode>(*err); }

CND_CX bool ClErr::IsChain() const { return std::holds_alternative<ClErrChain>(*err); }

CND_CX bool ClErr::IsEmpty() const { return err == nullptr; }

CND_CX ClErrNode& ClErr::GetSingle() { return std::get<ClErrNode>(*err); }

CND_CX const ClErrNode& ClErr::GetSingle() const { return std::get<ClErrNode>(*err); }

CND_CX ClErrChain& ClErr::GetChain() { return std::get<ClErrChain>(*err); }

CND_CX const ClErrChain& ClErr::GetChain() const { return std::get<ClErrChain>(*err); }

CND_CX void ClErr::PushError(const ClErrNode& e) {
  if (IsSingle())
    GetSingle() = e;
  else
    GetChain().errors.push_back(e);
}

CND_CX void ClErr::PushError(ClErrNode&& e) {
  if (IsSingle())
    GetSingle() = e;
  else
    GetChain().errors.push_back(std::forward<ClErrNode>(e));
}

CND_CX void ClErr::PopError() {
  if (IsSingle())
    err.reset();
  else {
    if (!GetChain().errors.empty()) GetChain().errors.pop_back();
    if (GetChain().errors.empty()) err.reset();
  }
}

CND_CX Str ClErr::Format() const {
  if (IsEmpty()) return "";
  if (IsSingle()) return GetSingle().Format();
  // Else, it's a chain.
  Str result;
  for (const auto& e : GetChain().errors) result += e.Format();
  return result;
}

CND_CX int ClErr::LastErrorCode() const {
  if (IsEmpty()) return 0;
  if (IsSingle()) return CND_SCAST<int>(GetSingle().code);
  return CND_SCAST<int>(GetChain().errors.back().code);
}

}  // namespace cnd::clerr

/// @} // end of cnd_compiler_tools

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////