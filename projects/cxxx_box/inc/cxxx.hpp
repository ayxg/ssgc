///////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language Environment
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup cppmodule2_cppextended
/// @brief C++ Extended Standard Library Includes
///////////////////////////////////////////////////////////////////////////////

/// @addtogroup cppmodule2_cppextended
/// @{

#ifndef HEADER_GUARD_SSG_CXXX_CXXX_H
#define HEADER_GUARD_SSG_CXXX_CXXX_H

// External Includes:
#include "mta.hpp"

// Local includes.
// Cxx C++ Standard Lib
#include "cxxx_import_std.hpp"
// Cxx Macro Definitions
#include "cxxx_macrodef.hpp"
// cxx::BoolError, cxx::Expected, cxx::PartialExpected
#include "cxxx_expected.hpp"
/// cxx::EnumeratedFlags
#include "cxxx_enumerated_flags.hpp"
// Filesystem utilities
#include "cxxx_fsys.hpp"

// Data Structures
#include "cxxx_tree.hpp"

namespace cxx {
constexpr auto AdvanceIt(auto&& iter, size_t i) {
  std::advance(iter, i);
  return iter;
}
constexpr auto AdvanceIt(auto& iter, size_t i) {
  std::advance(iter, i);
  return iter;
}

constexpr bool StrEq(char const* a, char const* b) { return *a == *b && (*a == '\0' || StrEq(a + 1, b + 1)); }

constexpr bool StrEq(std::string_view&& a, std::string_view&& b) {
  return std::forward<std::string_view>(a) == std::forward<std::string_view>(b);
}

constexpr bool StrEq(const std::string_view& a, const std::string_view& b) { return a == b; }

constexpr int StrToInt(const char* str) noexcept {
  if (std::is_constant_evaluated()) {
    int result;
    int puiss;

    result = 0;
    puiss = 1;
    while (('-' == (*str)) || ((*str) == '+')) {
      if (*str == '-') puiss = puiss * -1;
      str++;
    }
    while ((*str >= '0') && (*str <= '9')) {
      result = (result * 10) + ((*str) - '0');
      str++;
    }
    return (result * puiss);
  } else {
    return std::stoi(str);
  }
}

static constexpr inline bool IsInRange(const auto& iter, const auto& container) noexcept {
  return iter != container.end();
};

/*
 * https://github.com/gcc-mirror/gcc/blob/master/libiberty/strtol.c
 * Convert a string to a long integer.
 *
 * Ignores `locale' stuff.  Assumes that the upper and lower case
 * alphabets and digits are each contiguous.
 *
 */
// constexpr long StrToLong(const char* nptr, char** err_out, int base) noexcept {
//   const char* s = nptr;
//   unsigned long acc;
//   int c;
//   unsigned long cutoff;
//   int neg = 0, any, cutlim;
//
//   /*
//    * Skip white space and pick up leading +/- sign if any.
//    * If base is 0, allow 0x for hex and 0 for octal, else
//    * assume decimal; if base is already 16, allow 0x.
//    */
//   do {
//     c = *s++;
//   } while (cnd::IsSrcCharWhitespace(c));
//   if (c == '-') {
//     neg = 1;
//     c = *s++;
//   } else if (c == '+')
//     c = *s++;
//   if ((base == 0 || base == 16) && c == '0' && (*s == 'x' || *s == 'X')) {
//     c = s[1];
//     s += 2;
//     base = 16;
//   }
//   if (base == 0) base = c == '0' ? 8 : 10;
//
//   /*
//    * Compute the cutoff value between legal numbers and illegal
//    * numbers.  That is the largest legal value, divided by the
//    * base.  An input number that is greater than this value, if
//    * followed by a legal input character, is too big.  One that
//    * is equal to this value may be valid or not; the limit
//    * between valid and invalid numbers is then based on the last
//    * digit.  For instance, if the range for longs is
//    * [-2147483648..2147483647] and the input base is 10,
//    * cutoff will be set to 214748364 and cutlim to either
//    * 7 (neg==0) or 8 (neg==1), meaning that if we have accumulated
//    * a value > 214748364, or equal but the next digit is > 7 (or 8),
//    * the number is too big, and we will return a range error.
//    *
//    * Set any if any `digits' consumed; make it negative to indicate
//    * overflow.
//    */
//   cutoff = neg ? -(long)LONG_MIN : LONG_MAX;
//   cutlim = cutoff % (unsigned long)base;
//   cutoff /= (unsigned long)base;
//   for (acc = 0, any = 0;; c = *s++) {
//     if (cnd::IsSrcCharNumeric(c))
//       c -= '0';
//     else if (cnd::IsSrcCharAlpha(c))
//       c -= cnd::IsSrcCharUpper(c) ? 'A' - 10 : 'a' - 10;
//     else
//       break;
//     if (c >= base) break;
//     if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
//       any = -1;
//     else {
//       any = 1;
//       acc *= base;
//       acc += c;
//     }
//   }
//
//   if (any < 0) {
//     acc = neg ? LONG_MIN : LONG_MAX;
//     errno = ERANGE;
//   } else if (neg)
//     acc = -(long)acc;
//   if (err_out != 0) *err_out = (char*)(any ? s - 1 : nptr);
//   return (acc);
// }

template <class EnumT>
  requires std::is_enum_v<EnumT>
struct UnknownEnumEntry : std::exception {
  using enum_type = EnumT;
  constexpr cstring what() const override { return "Unkown Enum Entry."; }
};

template <size_t Footprint>
struct StrLiteral {
  char data[Footprint];
  constexpr size_t size() const { return Footprint - 1; }
  constexpr StrLiteral(const char (&init)[Footprint]) { std::copy_n(init, Footprint, data); }
};

static std::string ReadFile(const char * fp) {
  std::ifstream file(fp, std::ios::binary | std::ios::ate);
  if (!file.is_open()) throw std::runtime_error(std::format("Failed to open file: {}",fp));
  std::size_t fileSize = std::filesystem::file_size(fp);
  std::string content(fileSize, '\0');  // Pre-allocate space
  file.seekg(0);
  file.read(&content[0], fileSize);
  return content;
}


constexpr std::string_view Dedent(std::string_view input) {
  if (input.empty()) return input;

  // Skip the first newline if it exists
  size_t start_pos = (input[0] == '\n') ? 1 : 0;
  if (start_pos >= input.size()) return "";

  // Split into lines (at compile time)
  std::vector<std::string_view> lines;
  size_t line_start = start_pos;
  while (line_start < input.size()) {
    size_t line_end = input.find('\n', line_start);
    if (line_end == std::string_view::npos) line_end = input.size();
    lines.push_back(input.substr(line_start, line_end - line_start));
    line_start = line_end + 1;
  }

  // Find the minimum leading whitespace
  size_t min_whitespace = std::string_view::npos;
  for (const auto& line : lines) {
    if (line.empty()) continue;  // Skip empty lines
    size_t leading_ws = line.find_first_not_of(" \t");
    if (leading_ws != std::string_view::npos) {
      if (min_whitespace == std::string_view::npos || leading_ws < min_whitespace) {
        min_whitespace = leading_ws;
      }
    }
  }

  // If no uniform whitespace, return as-is
  if (min_whitespace == 0 || min_whitespace == std::string_view::npos) {
    return input.substr(start_pos);
  }

  // Compute the new start and length
  size_t new_start = start_pos;
  size_t new_length = 0;

  for (const auto& line : lines) {
    if (line.size() > min_whitespace) {
      new_length += line.size() - min_whitespace;
    } else {
      new_length += line.size();
    }
    new_length += 1;  // For '\n'
  }

  if (new_length > 0) new_length -= 1;  // Remove last '\n'

  // Return a substring view (no allocation)
  return input.substr(new_start, new_length);
}

}  // namespace cxx

#endif HEADER_GUARD_SSG_CXXX_CXXX_H
/// @} // end of cppmodule2_cppextended

///////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language Environment
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////////////////
