///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2025 Anton Yashchenko
// Licensed under the Apache License, Version 2.0(the "License");
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: [CAF] Core Application Framework
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup coreappfw
/// @brief [UNIT-TEST] Core App Framework demo examples. Each example can be run as an individual unit test to observe.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup coreappfw
/// @{

// clang-format off
#pragma once
#include "minitest.hpp"
#include "cxxx.hpp"
#include "CAF/Demo.hpp"
// clang-format on

namespace caf_unit_test {

TEST(UtDemoExamples, ExWindowHandling) { ASSERT_FALSE(caf::demo::ExWindowHandling()); }
TEST(UtDemoExamples, ExAnimatedSquare) { ASSERT_FALSE(caf::demo::ExAnimatedSquare()); }
TEST(UtDemoExamples, ExBasicUserInterface) { ASSERT_FALSE(caf::demo::ExBasicUserInterface()); }
TEST(UtDemoExamples, ExBasicErrorModal) { ASSERT_FALSE(caf::demo::ExBasicErrorModal()); }
TEST(UtDemoExamples, ExAdvancedErrorModal) { ASSERT_FALSE(caf::demo::ExAdvancedErrorModal()); }
TEST(UtDemoExamples, ExLoadingBanner) { ASSERT_FALSE(caf::demo::ExLoadingBanner()); }
TEST(UtDemoExamples, ExImGuiTheme) { ASSERT_FALSE(caf::demo::ExImGuiTheme()); }
TEST(UtDemoExamples, ExUsingEntities) { ASSERT_FALSE(caf::demo::ExUsingEntities()); }
}  // namespace caf_unit_test

/// @} // end of coreappfw

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: [CAF] Core Application Framework
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
// @created: 2025/07/14
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2025 Anton Yashchenko
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
