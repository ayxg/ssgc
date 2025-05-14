///////////////////////////////////////////////////////////////////////////////
// @project: Sophia Silicon Gold Solutions
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup cppmodule2_cppextended
/// @brief C++ Extended Filesystem Header
///////////////////////////////////////////////////////////////////////////////

/// @addtogroup cppmodule2_cppextended
/// @{

#ifndef HEADER_GUARD_SSG_CXXX_FSYS_H
#define HEADER_GUARD_SSG_CXXX_FSYS_H

// External Includes:
#include "mta.hpp"

// Local includes.
// Cxx C++ Standard Lib
#include "cxxx_import_std.hpp"
// Cxx Macro Definitions
#include "cxxx_macrodef.hpp"
// cxx::BoolError
#include "cxxx_expected.hpp"

namespace cxx {

class EXCFileNotFound : public std::runtime_error {
 public:
  EXCFileNotFound(const stdfs::path& file_path)
      : std::runtime_error("File not found: " + file_path.string()) {
    msg_ = std::runtime_error::what();
  }
  EXCFileNotFound(const std::string& file_path)
      : std::runtime_error("File not found: " + file_path) {
    msg_ = std::runtime_error::what();
  }
  const char* what() const override { return msg_.c_str(); }

 private:
  std::string msg_;
};

static std::string LoadFileToStr(stdfs::path file_path) {
  using namespace std;
  stdfs::path filepath(stdfs::absolute(stdfs::path(file_path)));

  uintmax_t fsize;

  if (stdfs::exists(filepath)) {
    fsize = stdfs::file_size(filepath);
  } else {
    throw(EXCFileNotFound(filepath));
  }

  ifstream infile;
  infile.exceptions(ifstream::failbit | ifstream::badbit);
  try {
    infile.open(filepath.c_str(), ios::in | ifstream::binary);
  } catch (...) {
    throw_with_nested(
        runtime_error("Can't open input file " + filepath.string()));
  }

  string fileStr;

  try {
    fileStr.resize(fsize);
  } catch (...) {
    stringstream err;
    err << "Can't resize to " << fsize << " bytes";
    throw_with_nested(runtime_error(err.str()));
  }

  infile.read(fileStr.data(), fsize);
  infile.close();

  return fileStr;
}

static std::string LoadFileToStr(const char* const file_path) {
  return LoadFileToStr(stdfs::path(file_path));
}

static std::string LoadFileToStr(const std::string& file_path) {
  return LoadFileToStr(file_path.c_str());
};

static void SaveStrToFile(const std::string& file_path,
                          const std::string& file_contents) {
    using namespace std;
  ofstream outfile;
  outfile.exceptions(ifstream::failbit | ifstream::badbit);
  try {
    outfile.open(file_path.c_str(), ios::out | ofstream::binary);
  } catch (...) {
    throw_with_nested(
        runtime_error("Can't open output file " + file_path));
  }

  outfile.write(file_contents.data(), file_contents.size());
  outfile.close();
}
}  // namespace cxx

#endif HEADER_GUARD_SSG_CXXX_FSYS_H
/// @} // end of cppmodule2_cppextended

///////////////////////////////////////////////////////////////////////////////
// @project: Sophia Silicon Gold Solutions
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
