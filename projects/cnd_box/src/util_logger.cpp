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
#include "util_logger.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>

#include "util_enum_verbosity.hpp"

namespace ssgc::util {

// Overriden stream buffer that discards all output.
class NullBuffer : public std::streambuf {
 protected:
  // overflow() is the virtual function that gets called when the stream wants
  // to emit a character and there's no put area available (or, in a minimal
  // streambuf, for every character).
  // For a null stream, we want to accept and discard every character.
  int_type overflow(int_type ch) override { return traits_type::not_eof(ch); }

  // Overrides the xsputn method to discard all output and return the
  // count of characters "written".
  std::streamsize xsputn(const char*, std::streamsize count) override { return count; }
};

static std::ostream& null_stream() {
  static NullBuffer buf;
  static std::ostream stream(&buf);
  return stream;
}

LogStream::LogStream(std::ostream& os, bool enabled)
    : active_(&os), enabled_(enabled), null_(&null_stream()) {}

std::ostream& LogStream::stream() { return enabled_ ? *active_ : *null_; }

LogStream& LogStream::operator<<(std::ostream& (*manip)(std::ostream&)) {
  stream() << manip;
  return *this;
}

void LogStream::enable(bool e) { enabled_ = e; }

void LogStream::redirect(std::ostream& os) { active_ = &os; }

Logger::Logger(eVerbosity verbosity)
    : verbosity_(verbosity),

      out(std::cout, verbosity >= eVerbosity::kDefault),
      verbose(std::cout, verbosity >= eVerbosity::kVerbose),
      debug(std::cout, verbosity >= eVerbosity::kDebug),
      trace(std::cout, verbosity >= eVerbosity::kTrace),

      err(std::cerr, verbosity >= eVerbosity::kDefault),
      err_verbose(std::cerr, verbosity >= eVerbosity::kVerbose),
      err_debug(std::cerr, verbosity >= eVerbosity::kDebug),
      err_trace(std::cerr, verbosity >= eVerbosity::kTrace) {}

void Logger::setVerbosity(eVerbosity verbosity) {
  verbosity_ = verbosity;

  out.enable(verbosity >= eVerbosity::kDefault);
  verbose.enable(verbosity >= eVerbosity::kVerbose);
  debug.enable(verbosity >= eVerbosity::kDebug);
  trace.enable(verbosity >= eVerbosity::kTrace);

  err.enable(verbosity >= eVerbosity::kDefault);
  err_verbose.enable(verbosity >= eVerbosity::kVerbose);
  err_debug.enable(verbosity >= eVerbosity::kDebug);
  err_trace.enable(verbosity >= eVerbosity::kTrace);
}

bool Logger::redirectOut(const std::filesystem::path& path) {
  auto file = std::make_unique<std::ofstream>(path);

  if (!file->is_open()) {
    return false;
  }

  out_file_ = std::move(file);
  out.redirect(*out_file_);
  verbose.redirect(*out_file_);
  debug.redirect(*out_file_);
  trace.redirect(*out_file_);

  return true;
}

bool Logger::redirectErr(const std::filesystem::path& path) {
  auto file = std::make_unique<std::ofstream>(path);

  if (!file->is_open()) {
    return false;
  }

  err_file_ = std::move(file);
  err.redirect(*err_file_);
  err_verbose.redirect(*err_file_);
  err_debug.redirect(*err_file_);
  err_trace.redirect(*err_file_);

  return true;
}

void Logger::redirectOut(std::ostream& os) {
  out.redirect(os);
  verbose.redirect(os);
  debug.redirect(os);
  trace.redirect(os);
}

void Logger::redirectErr(std::ostream& os) {
  err.redirect(os);
  err_verbose.redirect(os);
  err_debug.redirect(os);
  err_trace.redirect(os);
}

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