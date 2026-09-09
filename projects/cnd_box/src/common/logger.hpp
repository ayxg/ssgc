///////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
///////////////////////////////////////////////////////////////////////////////////////////////////
// @project: Sophia Silicon Gold Compiler
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup
/// @brief
///////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup ssgc_util
/// @{
#pragma once
#include <filesystem>
#include <fstream>
#include <iostream>

#include "enum_verbosity.hpp"

namespace ssgc::util {

class LogStream {
 public:
  LogStream(std::ostream& os, bool enabled);
  std::ostream& stream();
  void enable(bool e);
  void redirect(std::ostream& os);

  template <typename T>
  LogStream& operator<<(const T& value) {
    stream() << value;
    return *this;
  }

  LogStream& operator<<(std::ostream& (*manip)(std::ostream&));

 private:
  std::ostream* active_;
  std::ostream* null_;
  bool enabled_;
};

class Logger {
 public:
  explicit Logger(eVerbosity verbosity = eVerbosity::kDefault);
  void setVerbosity(eVerbosity verbosity);
  bool redirectOut(const std::filesystem::path& path);
  bool redirectErr(const std::filesystem::path& path);
  void redirectOut(std::ostream& os);
  void redirectErr(std::ostream& os);

  LogStream out;
  LogStream verbose;
  LogStream debug;
  LogStream trace;

  LogStream err;
  LogStream err_verbose;
  LogStream err_debug;
  LogStream err_trace;

 private:
  eVerbosity verbosity_{eVerbosity::kDefault};
  std::unique_ptr<std::ofstream> out_file_{nullptr};
  std::unique_ptr<std::ofstream> err_file_{nullptr};
};

inline Logger gLog{};

}  // namespace ssgc::util

/// @} // end of ssgc_util

///////////////////////////////////////////////////////////////////////////////////////////////////
// @project: Sophia Silicon Gold Compiler
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
//
// Licensed under the GNU Affero General Public License, Version 3.
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.gnu.org/licenses/agpl-3.0.html
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
///////////////////////////////////////////////////////////////////////////////////////////////////
