#pragma once

namespace ssgc::frontend {

constexpr bool isCharAlpha(char c) noexcept;
constexpr bool isCharNumeric(char c) noexcept;
constexpr bool isCharUnderscore(char c) noexcept;
constexpr bool isCharAlphanumeric(char c) noexcept;
constexpr bool isCharAlphaUnderscore(char c) noexcept;
constexpr bool isCharAlnumus(char c) noexcept;
constexpr bool isCharPunctuator(char c) noexcept;
constexpr bool isCharSpace(char c) noexcept;
constexpr bool isCharNewline(char c) noexcept;
constexpr bool isCharWhitespace(char c) noexcept;
constexpr bool isCharControl(char c) noexcept;
constexpr bool isCharValid(char c) noexcept;
constexpr bool isCharUpper(char c) noexcept;

}  // namespace ssgc::frontend

namespace ssgc::frontend {
constexpr bool isCharAlpha(char c) noexcept {
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

constexpr bool isCharUpper(char c) noexcept {
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

constexpr bool isCharNumeric(char c) noexcept {
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

constexpr bool isCharAlphanumeric(char c) noexcept { return isCharAlpha(c) || isCharNumeric(c); }

constexpr bool isCharUnderscore(char c) noexcept { return c == '_'; }

constexpr bool isCharAlphaUnderscore(char c) noexcept {
  return isCharAlpha(c) || isCharUnderscore(c);
}

constexpr bool isCharAlnumus(char c) noexcept {
  return isCharAlpha(c) || isCharNumeric(c) || isCharUnderscore(c);
}

/// '!', '@', '#', '$', '%', '^', '&', '*', '-','+', '=', '{', '}', '[',
/// ']','|', '\\', ';', ':', '\'', '\"', '<', '>', '?', '/', '~', '`', '.',
/// ',','(', ')', '_'
constexpr bool isCharPunctuator(char c) noexcept {
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
constexpr bool isCharSpace(char c) noexcept { return c == ' ' || c == '\t'; }

/// '\n', '\r', '\v', '\f'
constexpr bool isCharNewline(char c) noexcept {
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
constexpr bool isCharWhitespace(char c) noexcept {
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
constexpr bool isCharControl(char c) noexcept {
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

constexpr bool isCharValid(char c) noexcept {
  return isCharAlpha(c) || isCharNumeric(c) || isCharPunctuator(c) || isCharWhitespace(c);
}

}  // namespace ssgc::frontend