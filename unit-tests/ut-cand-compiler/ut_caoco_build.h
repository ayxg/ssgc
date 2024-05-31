//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
// Copyright 2024 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
//---------------------------------------------------------------------------//
// Author(s): Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.acpp.dev
//---------------------------------------------------------------------------//
// Project: C& Programming Language Environment
// Directory: ut-cand-official-compiler
// File: ut_caoco_lexer.h
//---------------------------------------------------------------------------//
#ifndef HEADER_GUARD_CALE_UT_CAND_OFFICIAL_COMPILER_UT_CAOCO_BUILD_H
#define HEADER_GUARD_CALE_UT_CAND_OFFICIAL_COMPILER_UT_CAOCO_BUILD_H
//---------------------------------------------------------------------------//
// Brief: brief
//---------------------------------------------------------------------------//
// Includes:
#include "cppsextended.h"
#include "minitest.h"
// Testing:
#include "caoco_build.h"
//---------------------------------------------------------------------------//

INLINE_MINITEST(Test_Build, TestCase_BasicBuild) {
  using namespace caoco;
  Builder builder;
  builder.SetRootFile("C:\\Users\\Anton\\source\\repos\\CandRuntime\\cand-examples\\HelloWorld\\main.cand");
  builder.Build();
  if (builder.ErrorOccured()) {
    std::cout << "Error occured: " << builder.ErrorMessage() << std::endl;
  }
  else {
    const auto & test = builder.GetLexedSource();
    for (auto & t : test) {
      std::cout << t.Literal() << " ";
    }
    std::cout << "Build success" << std::endl;
  }

}
INLINE_END_MINITEST;
MINITEST_REGISTER_CASE(Test_Build, TestCase_BasicBuild);

//---------------------------------------------------------------------------//
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
//---------------------------------------------------------------------------//
// Author(s): Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.acpp.dev
//---------------------------------------------------------------------------//
// Project: C& Programming Language Environment
// Directory: ut-cand-official-compiler
// File: ut_caoco_build.h
//---------------------------------------------------------------------------//
#endif HEADER_GUARD_CALE_UT_CAND_OFFICIAL_COMPILER_UT_CAOCO_BUILD_H
//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//