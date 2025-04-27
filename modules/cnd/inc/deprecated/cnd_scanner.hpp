#pragma once
// clang-format off
#include "cnd_implicit.hpp"
#include "cnd_constdef.hpp"
#include "cnd_clerr.hpp"
#include "cnd_src_traits.hpp"
#include "cnd_src_lines.hpp"
// clang-format on

namespace cnd {
namespace scanner {
using clerr::ClErr;
using clerr::ClFail;
using clerr::ClRes;
using clerr::MakeClErr;

ClRes<SrcVec> ReadSourceFile(const Path& fp,
                                eCharEncoding encoding = eCharEncoding::kAscii);
SrcLines SplitSourceLines(const SrcVec& src_bytes, eProgLang lang = eProgLang::kCand);

}  // namespace scanner
}  // namespace cnd