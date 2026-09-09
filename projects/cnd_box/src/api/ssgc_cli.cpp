#include "ssgc_cli.hpp"
#include "../cli/app.hpp"

extern "C" int ssgcRunCommandLineInterface(int argc, char* argv[]) {
  return ssgc::cli::run(argc, argv);
}