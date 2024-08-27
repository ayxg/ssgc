///////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
///////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language Environment
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup unittest1_cand_compiler
/// @brief UT C& Builder 
///////////////////////////////////////////////////////////////////////////////

/// @addtogroup unittest1_cand_compiler
/// @{
#ifndef HEADER_GUARD_CALE_UT_CAND_OFFICIAL_COMPILER_UT_CAOCO_BUILD_H
#define HEADER_GUARD_CALE_UT_CAND_OFFICIAL_COMPILER_UT_CAOCO_BUILD_H

// Includes:
#include "cppsextended.h"
#include "minitest.h"
// Testing:
#include "caoco_build.h"

/// @warning This test currently uses a local file existing only on my computer.
/// C:\\Users\\Anton\\source\\repos\\CandRuntime\\cand-examples\\HelloWorld\\main.cand
INLINE_MINITEST(Test_Build, TestCase_BasicBuild) {
  using namespace caoco;

  //builder.SetRootFile("..\\cand-examples\\ut0_empty_main\\main.cand");
}
INLINE_END_MINITEST;
MINITEST_REGISTER_CASE(Test_Build, TestCase_BasicBuild);

#endif HEADER_GUARD_CALE_UT_CAND_OFFICIAL_COMPILER_UT_CAOCO_BUILD_H
/// @} // end of unittest1_cand_compiler
///////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language Environment
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
