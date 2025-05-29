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

/// @addtogroup cnd_compiler
/// @{
#pragma once
// clang-format off
#include "use_ccapi.hpp"
#include "use_corevals.hpp"
#include "use_clmsg.hpp"

namespace cnd {
namespace trtools {

template <class SourceCharT>
CompilerProcessResult<Vec<SourceCharT>> LoadSourceFile(std::string_view fp) {
  using cldev::clmsg::MakeClMsg;

  if (!stdfs::exists(fp)) return CompilerProcessFailure(MakeClMsg<eClErr::kFailedToReadFile>(fp, "Does not exist"));

  if (!stdfs::is_regular_file(fp))
    return CompilerProcessFailure(MakeClMsg<eClErr::kFailedToReadFile>(fp, "Not a regular file."));

  std::ifstream source_file_stream(fp.data());
  if (!source_file_stream.is_open())
    return CompilerProcessFailure(MakeClMsg<eClErr::kFailedToReadFile>(fp, "Could not open file."));

  Vec<SourceCharT> temp_file_buffer = Vec<SourceCharT>((std::istreambuf_iterator<SourceCharT>(source_file_stream)),
                                                       std::istreambuf_iterator<SourceCharT>());
  source_file_stream.close();

  // Add \0 if not already at end.
  if (temp_file_buffer.back() != '\0') temp_file_buffer.push_back('\0');

  return temp_file_buffer;
}
};  // namespace trtools
}  // namespace cnd

/// @} // end of cnd_compiler

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