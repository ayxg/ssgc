#include "cnd_scanner.hpp"

// clang-format off
#include "cnd_implicit.hpp"
#include "cnd_constdef.hpp"
#include "cnd_clerr.hpp"
#include "cnd_src_traits.hpp"
#include "cnd_src_lines.hpp"
// clang-format on

namespace cnd {
namespace scanner {

ClRes<SrcVec> ReadSourceFile(const Path& fp, eCharEncoding codepoint) {
  using namespace clerr;
  using enum eClErr;

  if (!stdfs::exists(fp)) return ClFail(MakeClErr<kFailedToReadFile>(fp.string(), "Does not exist"));

  if (!stdfs::is_regular_file(fp)) return ClFail(MakeClErr<kFailedToReadFile>(fp.string(), "Not a regular file."));

  std::ifstream source_file_stream(fp);
  if (!source_file_stream.is_open()) return ClFail(MakeClErr<kFailedToReadFile>(fp.string(), "Could not open file."));

  SrcVec temp_file_buffer =
      SrcVec((std::istreambuf_iterator<char>(source_file_stream)), std::istreambuf_iterator<char>());
  source_file_stream.close();

  return temp_file_buffer;
}

SrcLines SplitSourceLines(const SrcVec& src_bytes, eProgLang lang) {
  using namespace clerr;
  using enum eClErr;
  if (src_bytes.empty()) return SrcLines{};
  SrcLines out{};
  for (const SrcChar& c : src_bytes) {
    if (src_traits::IsNewline(c)) {
      out.push_line(CND_SCAST<SrcChar>(eSrcChar::kLineFeed));
    } else {
      out.push_back(c);
    }
  }
  return out;
}

}  // namespace scanner

}  // namespace cnd