///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2025 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: Minitest Framework
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup
/// @brief Fixture base class.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace minitest {

/// @addtogroup minitest2_framework_impl
/// @{


/// @brief Base class for all fixtures.
/// 
/// Inherit from this class to create a fixture.
/// All members of child class must be public or protected
/// If you wish to access them in your test cases.
struct Fixture {
  void virtual SetUp() { /* Override pre-test actions. */ }
  void virtual TearDown() { /* Override post-test actions. */ }
  void virtual Run() { /* Override test actions. */ }
  virtual ~Fixture() = default;
};

/// @} // end of minitest2_framework_impl

}  // namespace minitest


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: Minitest Framework
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2025 Anton Yashchenko
//
// Licensed under the GNU Affero General Public License, Version 3. you may not
// use this file except in compliance with the License. You may obtain a copy of
// the License at http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
// WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
// License for the specific language governing permissions and limitations under
// the License.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
