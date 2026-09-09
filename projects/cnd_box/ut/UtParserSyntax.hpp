///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2025 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup cnd_unit_test
/// @brief Tests parsing primary expressions in isolation.
///
/// This is arguably the hardest part of the parser to develop. These tests provide some sanity and
/// prevent regression.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup ssgc_unit_test
/// @{
#pragma once
// clang-format off
#include "minitest.hpp"
#include "compiler/compiler.hpp"
#include "ParserTestUtils.hpp"
// clang-format on

namespace ssgc::unit_test {
// Method used for unit testing in this header
using namespace cnd_unit_test::frontend::test_util;

TEST(UtParserSyntax, AstToNodeFile) { 
  TestTransformMethod("",ParseSyntax,frontend::transformFile); 
}
 
TEST(UtParserSyntax, AstToNodeOperands) {
  TestTransformMethod("true", ParseOperand, frontend::transformBool);
  TestTransformMethod("123i8", ParseOperand, frontend::transformI8);
  TestTransformMethod("123i16", ParseOperand, frontend::transformI16);
  TestTransformMethod("123i32", ParseOperand, frontend::transformI32);
  TestTransformMethod("123i64", ParseOperand, frontend::transformI64);
  TestTransformMethod("123u8", ParseOperand, frontend::transformU8);
  TestTransformMethod("123u16", ParseOperand, frontend::transformU16);
  TestTransformMethod("123u32", ParseOperand, frontend::transformU32);
  TestTransformMethod("123u64", ParseOperand, frontend::transformU64);
  TestTransformMethod("123.f", ParseOperand, frontend::transformF32);
  TestTransformMethod("123.0", ParseOperand, frontend::transformF64);
  TestTransformMethod("none", ParseOperand, frontend::transformNone);
  TestTransformMethod("foo", ParseOperand, frontend::transformIdentifier);
  TestTransformMethod("\"hello world\"", ParseOperand, frontend::transformStringLiteral);

  TestTransformMethod("fn@add;", ParseMethodDecl, frontend::transformMethod);
  TestTransformMethod("fn@add>;", ParseMethodDecl, frontend::transformMethod);
  TestTransformMethod("fn@add>;", ParseMethodDecl, frontend::transformMethod);
}


}// namespace ssgc::unit_test
/// @} // end of ssgc_unit_test

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2025 Anton Yashchenko
//
// This program is free software : you can redistribute it and / or modify it
// under the terms of the GNU Affero General Public License as published by the
// Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////