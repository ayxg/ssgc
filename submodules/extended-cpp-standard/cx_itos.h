/* License provided with file:
 * f_to_string.hpp - Provides compile-time floating-point-to-string conversion.
 * Written by Clyne Sullivan.
 * Modified by Anton Yashchenko for the C++ Extended Standard Library.
 * https://github.com/tcsullivan/constexpr-to-string

This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to <https://unlicense.org>
*/
#ifndef HEADER_GUARD_CALE_EXTENDED_CPP_STANDARD_CX_ITOS_H
#define HEADER_GUARD_CALE_EXTENDED_CPP_STANDARD_CX_ITOS_H

#include <cstdint>
#include <type_traits>

namespace cxx {
namespace constexpr_to_string {

constexpr char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

/**
 * @struct to_string_t
 * @brief Provides the ability to convert any integral to a string at
 * compile-time.
 * @tparam N Number to convert
 * @tparam base Desired base, can be from 2 to 36
 */
template <std::intmax_t N, int base, typename char_type,
          std::enable_if_t<(base > 1 && base < sizeof(digits)), int> = 0>
class to_string_t {
  constexpr static auto buflen() noexcept {
    unsigned int len = N > 0 ? 1 : 2;
    for (auto n = N; n; len++, n /= base)
      ;
    return len;
  }

  char_type buf[buflen()] = {};

 public:
  /**
   * Constructs the object, filling `buf` with the string representation of N.
   */
  constexpr to_string_t() noexcept {
    auto ptr = end();
    *--ptr = '\0';

    if (N != 0) {
      for (auto n = N; n; n /= base)
        *--ptr = digits[(N < 0 ? -1 : 1) * (n % base)];
      if (N < 0) *--ptr = '-';
    } else {
      buf[0] = '0';
    }
  }

  // Support implicit casting to `char *` or `const char *`.
  constexpr operator char_type*() noexcept { return buf; }
  constexpr operator const char_type*() const noexcept { return buf; }

  constexpr auto size() const noexcept { return sizeof(buf) / sizeof(buf[0]); }

  // Element access
  constexpr auto data() noexcept { return buf; }
  constexpr const auto data() const noexcept { return buf; }
  constexpr auto& operator[](unsigned int i) noexcept { return buf[i]; }
  constexpr const auto& operator[](unsigned int i) const noexcept {
    return buf[i];
  }
  constexpr auto& front() noexcept { return buf[0]; }
  constexpr const auto& front() const noexcept { return buf[0]; }
  constexpr auto& back() noexcept { return buf[size() - 1]; }
  constexpr const auto& back() const noexcept { return buf[size() - 1]; }

  // Iterators
  constexpr auto begin() noexcept { return buf; }
  constexpr const auto begin() const noexcept { return buf; }
  constexpr auto end() noexcept { return buf + size(); }
  constexpr const auto end() const noexcept { return buf + size(); }
};

}  // namespace constexpr_to_string

/**
 * Simplifies use of `to_string_t` from `to_string_t<N>()` to `to_string<N>`.
 */
template <std::intmax_t N, int base = 10, typename char_type = char>
constexpr constexpr_to_string::to_string_t<N, base, char_type> cx_itos;
}  // namespace cxx

#endif  // HEADER_GUARD_CALE_EXTENDED_CPP_STANDARD_CX_ITOS_H
