#pragma once

struct ssgcTranslationOutput {
  int exit_code;
  const char* build_path;
  int build_files_count;
  char* build_files[];
  const char* build_command_line_args;
};

