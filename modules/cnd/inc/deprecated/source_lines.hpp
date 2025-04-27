#pragma once
// clang-format off
#include "use_ccapi.hpp"
#include "use_corevals.hpp"
// clang-format on

namespace cnd {

struct SrcLines;
struct SrcLinesIter;
struct SrcLinesConstIter;

/// @class SrcLines
/// Represents a set of source lines from one or more files as a contiguous
/// block of characters. Lines and files are stored as arrays of indices to the
/// end of each line or file.
struct SrcLines {
  friend SrcLinesIter;
  friend SrcLinesConstIter;
  using iterator = SrcLinesIter;
  using const_iterator = SrcLinesConstIter;

  CND_CX Size size() const;

  CND_CX void push_back(SrcChar c);
  CND_CX void push_back(StrView sv);
  CND_CX void push_back(const SrcChar* s);
  CND_CX void push_back(const Str& s);

  CND_CX void push_line();
  CND_CX void push_line(SrcChar c);
  CND_CX void push_line(StrView sv);
  CND_CX void push_line(const SrcChar* s);
  CND_CX void push_line(const Str& s);
  CND_CX void push_line(const Vec<SrcChar>& v);

  CND_CX void add_line(Size line_end) { lines.push_back(line_end); }
  CND_CX void add_file(Size file_end) { files.push_back(file_end); }
  CND_CX void scan_lines() {
    auto beg = data.cbegin();
    for (auto it = data.cbegin(); it != data.end(); it++) {
      if (src_traits::IsNewline(*it)) {
        add_line(std::distance(beg, it));
      }
    }
  }
  CND_CX iterator begin();
  CND_CX iterator end();
  CND_CX const_iterator cbegin() const;
  CND_CX const_iterator cend() const;

  CND_CX SrcLines() = default;
  CND_CX SrcLines(const SrcVec& data) : data(data) { scan_lines(); }
  CND_CX SrcLines(SrcVec&& data) : data(forward<SrcVec>(data)) { scan_lines(); }
  CND_CX SrcLines(const SrcVec& data, const Vec<Size>& lines) : data(data), lines{lines} {}
  CND_CX SrcLines(SrcVec&& data, Vec<Size>&& lines) : data(forward<SrcVec>(data)), lines{forward<Vec<Size>>(lines)} {}
  CND_CX SrcLines(const SrcVec& data, const Vec<Size>& lines, const Vec<Size>& files)
      : data(data), lines(lines), files(files) {}
  CND_CX SrcLines(SrcVec&& data, Vec<Size>&& lines, Vec<Size>&& files)
      : data(forward<SrcVec>(data)), lines(forward<Vec<Size>>(lines)), files{forward<Vec<Size>>(files)} {}
  CND_CX SrcLines(const SrcLines& o) : data(o.data), lines(o.lines), files(o.files) {}
  CND_CX SrcLines(SrcLines&& o)
      : data(forward<SrcVec>(o.data)), lines(forward<Vec<Size>>(o.lines)), files{forward<Vec<Size>>(o.files)} {}
  CND_CX SrcLines& operator=(const SrcLines& o) {
    data = o.data;
    lines = o.lines;
    files = o.files;
    return *this;
  }
  CND_CX SrcLines& operator=(SrcLines&& o) {
    data = forward<SrcVec>(o.data);
    lines = forward<Vec<Size>>(o.lines);
    files = forward<Vec<Size>>(o.files);
    return *this;
  }

  CND_CX SrcLines(StrView str_view) : data{} { 
    for (auto c : str_view) push_back(c);
    scan_lines();
  }

 private:
  SrcVec data{};
  Vec<Size> lines{{0}};
  Vec<Size> files{{0}};
};

struct SrcLinesIter {
  CND_CX SrcLinesIter& operator++();
  CND_CX SrcLinesIter& operator--();
  CND_CX SrcChar& operator*();
  CND_CX Bool operator==(const SrcLinesIter& o) const;
  CND_CX Bool operator!=(const SrcLinesIter& o) const;

  CND_CX Size File() const;
  CND_CX Size Line() const;
  CND_CX Size Col() const;

  CND_CX SrcLinesIter Next(int n = 1) const;

  Size idx;
  Size line_idx;
  Size file_idx;
  SrcLines& src;
};

struct SrcLinesConstIter {
  CND_CX SrcLinesConstIter& operator++();
  CND_CX SrcLinesConstIter& operator--();
  CND_CX const SrcChar& operator*() const;
  CND_CX Bool operator==(const SrcLinesConstIter& o) const;
  CND_CX Bool operator!=(const SrcLinesConstIter& o) const;

  CND_CX Size File() const;
  CND_CX Size Line() const;
  CND_CX Size Col() const;

  CND_CX SrcLinesConstIter Next(int n = 1) const;
  CND_CX SrcChar CheckedGet() const;
  CND_CX const SrcChar* Ptr() const { return src->data.data() + idx; }
  Size idx;
  Size line_idx;
  Size file_idx;
  const SrcLines* src;
};

CND_CX SrcLinesIter& SrcLinesIter::operator++() {
  idx++;
  if (idx >= src.lines[line_idx]) line_idx++;
  return *this;
}

CND_CX SrcLinesIter& SrcLinesIter::operator--() {
  idx--;
  if (idx < src.lines[line_idx]) line_idx--;
  return *this;
}

CND_CX SrcChar& SrcLinesIter::operator*() { return src.data[idx]; }

CND_CX Bool SrcLinesIter::operator==(const SrcLinesIter& o) const { return line_idx; }

CND_CX Bool SrcLinesIter::operator!=(const SrcLinesIter& o) const { return idx != o.idx; }

CND_CX Size SrcLinesIter::File() const { return file_idx; };

CND_CX Size SrcLinesIter::Line() const { return line_idx; };

CND_CX Size SrcLinesIter::Col() const { return (line_idx == 0) ? idx : idx - src.lines[line_idx - 1]; };

CND_CX SrcLinesIter SrcLinesIter::Next(int n) const {
  auto next = *this;
  for (n; n != 0; n--) ++next;
  return next;
}

CND_CX SrcLinesConstIter& SrcLinesConstIter::operator++() {
  idx++;
  if (!src->lines.empty())
    if (idx >= src->lines[line_idx]) line_idx++;
  return *this;
}

CND_CX SrcLinesConstIter& SrcLinesConstIter::operator--() {
  idx--;
  if (idx < src->lines[line_idx]) line_idx--;
  return *this;
}

CND_CX const SrcChar& SrcLinesConstIter::operator*() const { return src->data[idx]; }

CND_CX Bool SrcLinesConstIter::operator==(const SrcLinesConstIter& o) const { return line_idx; }

CND_CX Bool SrcLinesConstIter::operator!=(const SrcLinesConstIter& o) const { return idx != o.idx; }

CND_CX SrcChar SrcLinesConstIter::CheckedGet() const {
  if (idx >= src->data.size()) return '\0';
  return src->data[idx];
};

CND_CX Size SrcLinesConstIter::File() const { return file_idx; };

CND_CX Size SrcLinesConstIter::Line() const { return line_idx; };

CND_CX Size SrcLinesConstIter::Col() const { return (line_idx == 0) ? idx : idx - src->lines[line_idx - 1]; };

CND_CX SrcLinesConstIter SrcLinesConstIter::Next(int n) const {
  auto next = *this;
  for (n; n != 0; n--) ++next;
  return next;
}

CND_CX Size SrcLines::size() const { return data.size(); }

CND_CX void SrcLines::push_back(SrcChar c) { data.push_back(c); }
CND_CX void SrcLines::push_back(StrView sv) {
  for (auto c : sv) push_back(c);
}
CND_CX void SrcLines::push_back(const SrcChar* s) {
  for (auto c = s; *c != '\0'; c++) push_back(*c);
}
CND_CX void SrcLines::push_back(const Str& s) {
  for (auto c : s) push_back(c);
}

CND_CX void SrcLines::push_line() { lines.push_back(data.size()); }
CND_CX void SrcLines::push_line(SrcChar c) {
  push_line();
  push_back(c);
}
CND_CX void SrcLines::push_line(StrView sv) {
  push_line();
  for (auto c : sv) SrcLines::push_back(c);
}
CND_CX void SrcLines::push_line(const SrcChar* s) {
  push_line();
  for (auto c = s; *c != '\0'; c++) push_back(*c);
}
CND_CX void SrcLines::push_line(const Str& s) {
  push_line();
  for (auto c : s) push_back(c);
}
CND_CX void SrcLines::push_line(const Vec<SrcChar>& v) {
  push_line();
  for (auto c : v) push_back(c);
}

CND_CX SrcLines::iterator SrcLines::begin() { return iterator{0, 0, 0, *this}; }
CND_CX SrcLines::iterator SrcLines::end() { return iterator{data.size(), lines.size(), files.size(), *this}; }
CND_CX SrcLines::const_iterator SrcLines::cbegin() const { return const_iterator{0, 0, 0, this}; }
CND_CX SrcLines::const_iterator SrcLines::cend() const {
  return const_iterator{data.size(), lines.size(), files.size(), this};
}

}  // namespace cnd

