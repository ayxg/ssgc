#pragma once
typedef void * ssgcCompiler;
typedef unsigned int ssgcSize;

struct ssgcToken {
  ssgcSize kind;
  ssgcSize file;
  ssgcSize offset;
  ssgcSize size;
};

extern "C" ssgcCompiler ssgcInitCompiler();

extern "C" void ssgcDeleteCompiler(ssgcCompiler compiler);

extern "C" ssgcSize ssgcCompilerLoadSource(ssgcCompiler compiler, const char* file_path,
                                             bool overwrite);

extern "C" ssgcSize ssgcCompilerGenerateSource(ssgcCompiler compiler, const char* file_path,
                                             const char* data,
                                             bool overwrite);

extern "C" ssgcSize ssgcCompilerTokenize(ssgcCompiler compiler, ssgcSize file_id,const ssgcToken * result);
