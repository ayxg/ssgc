#pragma once
#include "cxxx.hpp"
#include "caoco_lexer.h"
#include "caoco_parser.h"

namespace caoco {

void RemoveQuotesFromCliArg(std::string& arg) {
  while (arg.size() > 2 && arg.front() == '"' && arg.back() == '"') {
    arg = arg.substr(1, arg.size() - 2);
  }
}

int CliMain(int argc, char* argv[]) {
  std::vector<std::string> args(argv, argv + argc);
  for (auto& arg : args) {
    RemoveQuotesFromCliArg(arg);
  }
  // First arg should be the root cand file.
  auto lex_res = caoco::Lexer::Lex(args[0]);
  if (!lex_res) {
    std::cout << "Lexer error: " << lex_res.Error() << std::endl;
  }
  auto parse_res = caoco::ParseTokens(lex_res.Value());
  if (!parse_res) {
    std::cout << "Parser error: " << parse_res.Error() << std::endl;
  }

  return 0;
}
}  // namespace caoco::cli
