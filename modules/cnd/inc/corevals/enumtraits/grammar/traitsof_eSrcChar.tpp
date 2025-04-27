///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the Apache License, Version 2.0(the "License");
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup cnd_compiler_corevals
/// @brief Traits of eSrcChar
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "corevals/enumtraits/grammar/traitsof_eSrcChar.hpp"

// clang-format off
#include "use_ccapi.hpp"
#include "corevals/enumdefs/grammar/eSrcChar.hpp"
// clang-format on

/// @addtogroup cnd_compiler_corevals
/// @{

namespace cnd::corevals::grammar {
constexpr bool IsSrcCharAlpha(iConvertibleToSrcChar auto c) noexcept {
  switch (c) {
    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
    case 'g':
    case 'h':
    case 'i':
    case 'j':
    case 'k':
    case 'l':
    case 'm':
    case 'n':
    case 'o':
    case 'p':
    case 'q':
    case 'r':
    case 's':
    case 't':
    case 'u':
    case 'v':
    case 'w':
    case 'x':
    case 'y':
    case 'z':
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
    case 'G':
    case 'H':
    case 'I':
    case 'J':
    case 'K':
    case 'L':
    case 'M':
    case 'N':
    case 'O':
    case 'P':
    case 'Q':
    case 'R':
    case 'S':
    case 'T':
    case 'U':
    case 'V':
    case 'W':
    case 'X':
    case 'Y':
    case 'Z':
      return true;
    default:
      return false;
      break;
  }
}

constexpr bool IsSrcCharUpper(iConvertibleToSrcChar auto c) noexcept {
  switch (c) {
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
    case 'G':
    case 'H':
    case 'I':
    case 'J':
    case 'K':
    case 'L':
    case 'M':
    case 'N':
    case 'O':
    case 'P':
    case 'Q':
    case 'R':
    case 'S':
    case 'T':
    case 'U':
    case 'V':
    case 'W':
    case 'X':
    case 'Y':
    case 'Z':
      return true;
    default:
      return false;
      break;
  }
}


constexpr bool IsSrcCharNumeric(iConvertibleToSrcChar auto c) noexcept {
  switch (c) {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      return true;
    default:
      return false;
      break;
  }
}

constexpr bool IsSrcCharAlphanumeric(iConvertibleToSrcChar auto c) noexcept {
  return IsSrcCharAlpha(c) || IsSrcCharNumeric(c);
}


constexpr bool IsSrcCharUnderscore(iConvertibleToSrcChar auto c) noexcept { return c == '_'; }

constexpr bool IsSrcCharAlphaUnderscore(iConvertibleToSrcChar auto c) noexcept {
  return IsSrcCharAlpha(c) || IsSrcCharUnderscore(c);
}

constexpr bool IsSrcCharAlnumus(iConvertibleToSrcChar auto c) noexcept {
  return IsSrcCharAlpha(c) || IsSrcCharNumeric(c) || IsSrcCharUnderscore(c);
}

/// '!', '@', '#', '$', '%', '^', '&', '*', '-','+', '=', '{', '}', '[',
/// ']','|', '\\', ';', ':', '\'', '\"', '<', '>', '?', '/', '~', '`', '.',
/// ',','(', ')', '_'
constexpr bool IsSrcCharPunctuator(iConvertibleToSrcChar auto c) noexcept {
  switch (c) {
    case '!':
    case '@':
    case '#':
    case '$':
    case '%':
    case '^':
    case '&':
    case '*':
    case '-':
    case '+':
    case '=':
    case '{':
    case '}':
    case '[':
    case ']':
    case '|':
    case '\\':
    case ';':
    case ':':
    case '\'':
    case '\"':
    case '<':
    case '>':
    case '?':
    case '/':
    case '~':
    case '`':
    case '.':
    case ',':
    case '(':
    case ')':
    case '_':
      return true;
    default:
      return false;
      break;
  }
}

/// Tab or space.
constexpr bool IsSrcCharSpace(iConvertibleToSrcChar auto c) noexcept { return c == ' ' || c == '\t'; }

/// '\n', '\r', '\v', '\f'
constexpr bool IsSrcCharNewline(iConvertibleToSrcChar auto c) noexcept {
  switch (c) {
    case '\n':
    case '\r':
    case '\v':
    case '\f':
      return true;
    default:
      return false;
      break;
  }
}

/// ' ', '\n', '\r', '\v', '\f'
constexpr bool IsSrcCharWhitespace(iConvertibleToSrcChar auto c) noexcept {
  switch (c) {
    case ' ':
    case '\n':
    case '\r':
    case '\v':
    case '\f':
    case '\t':
      return true;
    default:
      return false;
      break;
  }
}

// '\0', '\a', '\b', '\t', '\n', '\v', '\f', '\r', '\x1b'
constexpr bool IsSrcCharControl(iConvertibleToSrcChar auto c) noexcept {
  switch (c) {
    case '\0':
    case '\a':
    case '\b':
    case '\t':
    case '\n':
    case '\v':
    case '\f':
    case '\r':
    case '\x1b':
      return true;
    default:
      return false;
  }
}

constexpr bool IsSrcCharValid(iConvertibleToSrcChar auto c) noexcept {
  return IsSrcCharAlpha(c) || IsSrcCharNumeric(c) || IsSrcCharPunctuator(c) || IsSrcCharWhitespace(c);
}
}  // namespace cnd::corevals::grammar

/// @} // end of cnd_compiler_corevals

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
//
// Licensed under the Apache License, Version 2.0(the "License"); you may not use this file except in compliance with
// the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on
// an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////