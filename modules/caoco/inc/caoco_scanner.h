#pragma once
#include "cxxx.hpp"
// Includes:
#include "caoco_grammar.h"
#include "caoco_lexer.h"
namespace caoco {

namespace scanner {
namespace stdfs = std::filesystem;
using std::expected;
using std::unexpected;
using std::vector;

template <class T>
using CompRes = expected<T, caerr::CaErrUptr>;
using CompFail = unexpected<caerr::CaErrUptr>;

template <class T>
using CompResChain = expected<T, caerr::CaErrUptrVec>;
using CompFailChain = unexpected<caerr::CaErrUptrVec>;

struct PreProcessedSrc;
struct TranslationFragment;
struct TranslationUnit;

CompRes<vector<char>> ReadFileAsMultibyte(const char* filepath);
CompRes<vector<char>> ReadFileAsMultibyte(const char* filepath,
                                          const char* where);
CompRes<vector<char>> MapMultibyteToSrcByte(const vector<char>& mbvec);
CompRes<TkVector> LexRaw(const vector<char>& src_bytes);
CompRes<TkVector> ParseComments(const TkVector& lexed_src);
CompRes<TkVector> Preprocess(const TkVector& lexed_src,
                                    const TranslationFragment& tf);

CompRes<vector<char>> ReadFileAsMultibyte(const char* filepath,
                                          const char* where) {
  using namespace caerr;
  stdfs::path fp{where};
  fp.append(filepath);

  if (!stdfs::exists(fp))
    return CompFail(MakeError(FailedToReadFile(fp.string(), "Does not exist")));

  if (!stdfs::is_regular_file(fp))
    return CompFail(
        MakeError(FailedToReadFile(fp.string(), "Not a regular file.")));

  std::ifstream source_file_stream(fp);
  if (!source_file_stream.is_open())
    return CompFail(
        MakeError(FailedToReadFile(fp.string(), "Could not open file.")));

  // Load the file...
  vector<char> temp_file_buffer =
      vector<char>((std::istreambuf_iterator<char>(source_file_stream)),
                   std::istreambuf_iterator<char>());
  source_file_stream.close();

  return temp_file_buffer;
}

CompRes<vector<char>> ReadFileAsMultibyte(const char* filepath) {
  using namespace caerr;
  stdfs::path fp{filepath};

  if (!stdfs::exists(fp))
    return CompFail(MakeError(FailedToReadFile(fp.string(), "Does not exist")));

  if (!stdfs::is_regular_file(fp))
    return CompFail(
        MakeError(FailedToReadFile(fp.string(), "Not a regular file.")));

  std::ifstream source_file_stream(fp);
  if (!source_file_stream.is_open())
    return CompFail(
        MakeError(FailedToReadFile(fp.string(), "Could not open file.")));

  // Load the file...
  vector<char> temp_file_buffer =
      vector<char>((std::istreambuf_iterator<char>(source_file_stream)),
                   std::istreambuf_iterator<char>());
  source_file_stream.close();

  return temp_file_buffer;
}


}  // namespace scanner

}  // namespace caoco
