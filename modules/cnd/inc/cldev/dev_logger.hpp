///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the Apache License, Version 2.0(the "License");
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup
/// @brief
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup cnd_compiler_cldev
/// @{
#pragma once
// clang-format off
#include "use_ccapi.hpp"
#include "use_corevals.hpp"
#include "use_clmsg.hpp"
// clang-format on

namespace cnd {
namespace cldev {
namespace util {
template <class T>
concept iStreamOutputable = requires(std::ostream& os, T in) {
  { os << in } -> std::same_as<std::ostream&>;
};

struct Logger {
 public:
  enum class eRetargetingError {
    kCouldNotOpenFile,
  };

 public:
  inline std::ostream& GetOutStream() { return *out_stream; }
  inline std::ostream& GetErrStream() { return *err_stream; }
  inline std::istream& GetInStream() { return *in_stream; }

  inline decltype(auto) Print(iStreamOutputable auto&& msg...) {
    return {*out_stream << std::forward<decltype(msg)>(msg)...};
  }

  inline decltype(auto) PrintDiagnostic(iStreamOutputable auto&&... msg) {
    if (std::to_underlying(verbosity) >= std::to_underlying(eDriverVerbosity::kDebug)) {
      ((*out_stream << std::forward<decltype(msg)>(msg)), ...);
      return *out_stream;
    } else {
      return *out_stream;
    }
  }

  inline decltype(auto) PrintErr(iStreamOutputable auto&& msg...) {
    return {*err_stream << std::forward<decltype(msg)>(msg)...};
  }

  inline decltype(auto) PrintForward(iStreamOutputable auto&& msg, auto&& in) {
    *out_stream << std::forward<decltype(msg)>(msg);
    return std::forward<decltype(in)>(in);
  }

  inline decltype(auto) PrintErrForward(iStreamOutputable auto&& msg, auto&& in) {
    *err_stream << std::forward<decltype(msg)>(msg);
    return std::forward<decltype(in)>(in);
  }

  int PrintErrForward(const cldev::clmsg::ClMsgBuffer& e) {
    *err_stream << e.Format();
    return e.GetLastMessageId().code;
  }

  int PrintErrForward(const cldev::clmsg::ClMsgUnion& e) {
    *err_stream << e.Format();
    return e.GetLastMessageId().code;
  }

  Ex<void, eRetargetingError> SetOutStream(std::ostream& out) {
    ResetOutStream();
    out_stream = &out;
    return Ex<void, eRetargetingError>{};
  }
  Ex<void, eRetargetingError> SetErrStream(std::ostream& err) {
    ResetErrStream();
    err_stream = &err;
    return Ex<void, eRetargetingError>{};
  }
  Ex<void, eRetargetingError> SetInStream(std::istream& in) {
    ResetInStream();
    in_stream = &in;
    return Ex<void, eRetargetingError>{};
  }

  Ex<void, eRetargetingError> SetOutStream(const stdfs::path& file_path) {
    ResetOutStream();
    out_stream = new std::ofstream(file_path.c_str(), std::ios::out);
    if (!out_stream->good()) {
      return Unex<eRetargetingError>{eRetargetingError::kCouldNotOpenFile};
    }
    return Ex<void, eRetargetingError>{};
  }
  Ex<void, eRetargetingError> SetErrStream(const stdfs::path& file_path) {
    ResetErrStream();
    err_stream = new std::ofstream(file_path.c_str(), std::ios::out);
    if (!err_stream->good()) {
      return Unex<eRetargetingError>{eRetargetingError::kCouldNotOpenFile};
    }
    return Ex<void, eRetargetingError>{};
  }
  Ex<void, eRetargetingError> SetInStream(const stdfs::path& file_path) {
    ResetInStream();
    in_stream = new std::ifstream(file_path.c_str(), std::ios::in);
    if (!in_stream->good()) {
      return Unex<eRetargetingError>{eRetargetingError::kCouldNotOpenFile};
    }
    return Ex<void, eRetargetingError>{};
  }

  bool ResetOutStream() {
    if (owned_out_stream) {
      delete out_stream;
      out_stream = &std::cout;
      return true;
    }
    return false;
  }
  bool ResetErrStream() {
    if (owned_err_stream) {
      delete err_stream;
      err_stream = &std::cout;
      return true;
    }
    return false;
  }
  bool ResetInStream() {
    if (owned_in_stream) {
      delete in_stream;
      in_stream = &std::cin;
      return true;
    }
    return false;
  }

 public:
  eDriverVerbosity verbosity{eDriverVerbosity::kStd};

 private:
  std::ostream* out_stream{&std::cout};
  std::ostream* err_stream{&std::cout};
  std::istream* in_stream{&std::cin};
  bool owned_out_stream{false};
  bool owned_err_stream{false};
  bool owned_in_stream{false};

 public:
  constexpr Logger() = default;
};

static Logger& gStdLog() {
  static Logger log;
  return log;
}
#define CND_STDLOG ::cnd::cldev::util::gStdLog()
}  // namespace util
}  // namespace cldev_util
}  // namespace cnd

/// @} // end of cnd_compiler_cldev

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
