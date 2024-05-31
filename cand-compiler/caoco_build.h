//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
// Copyright 2024 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
//---------------------------------------------------------------------------//
// Author(s): Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.acpp.dev
//---------------------------------------------------------------------------//
// Project: C& Programming Language Environment
// Directory: cand-official-compiler
// File: caoco_build.h
//---------------------------------------------------------------------------//
#ifndef HEADER_GUARD_CALE_CAND_OFFICIAL_COMPILER_CAOCO_BUILD_H
#define HEADER_GUARD_CALE_CAND_OFFICIAL_COMPILER_CAOCO_BUILD_H
//---------------------------------------------------------------------------//
// Brief:{ Builder for the C& Official Compiler
//    - Handles the build pre-processing includes the lexing, parsing, macro
//      expansion, constevaluator and code generation phases.
//    - Responsible for managing the source files and temporary files.
//    - Provides the interface for the user to build an executable of library.
//    - Provides an interface to set the build options.
//}
//---------------------------------------------------------------------------//
#include <filesystem>

#include "cppsextended.h"
// Includes:
#include "caoco_grammar.h"
#include "caoco_lexer.h"
#include "caoco_parser.h"
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
// Namespace:{caoco}
//---------------------------------------------------------------------------//
namespace caoco {
//---------------------------------------------------------------------------//
namespace stdfs = std::filesystem;
class Builder {
 public:
  struct IncludedFile {
    bool was_tokenized{false};

    // Full absolute path to this file.
    stdfs::path file_path{""};

    // Files which should be included before this file.
    // Dependencies should be stored as their absolute path.
    std::vector<stdfs::path> depencencies{};

    // Root directory which all includes in this file are relative to.
    stdfs::path IncludeDir() const { return file_path.parent_path(); }

    bool DependsOn(const stdfs::path p) const {
      return std::find_if(depencencies.cbegin(), depencencies.cend(),
                          [&p](const auto& v) { return v == p; }) !=
             depencencies.cend();
    }
  };

 public:
  void SetRootFile(string_view p) {
    ValidateSourceFile(p);
    if (ErrorOccured()) return;
    root_file_.file_path = p;
  }
  bool ErrorOccured() const { return not build_error_; }
  cstring ErrorMessage() const { return build_error_.Error().c_str(); }
  const vector<Tk>& GetLexedSource() { return full_lexed_source_; }
  void Build() {
    // 1. Load and lex the root file into temp buffer.
    LoadSourceFileToTempBuffer(root_file_.file_path);
    if (ErrorOccured()) return;
    LexFileIntoTempBuffer(temp_file_buffer_);
    if (ErrorOccured()) return;
    ScanFileDependencies(temp_token_buffer_, root_file_.file_path);
    if (ErrorOccured()) return;

    for (auto& f : included_files_) {
      if (not f.was_tokenized) {
        LoadSourceFileToTempBuffer(f.file_path);
        if (ErrorOccured()) return;
        LexFileIntoTempBuffer(temp_file_buffer_);
        if (ErrorOccured()) return;
        ScanFileDependencies(temp_token_buffer_, f.file_path);
        if (ErrorOccured()) return;
      }
    }

    // 2. Lex all included files into the buffer in the correct order
    // starting from the last included to the root file
    // based on dependencies.
    for (auto it = included_files_.rbegin(); it != included_files_.rend();
         it++) {
      if (it->was_tokenized) {
        LoadSourceFileToTempBuffer(it->file_path);
        if (ErrorOccured()) return;
        LexFileIntoTempBuffer(temp_file_buffer_);
        if (ErrorOccured()) return;
        RemoveIncludeDirectivesFromSource(temp_token_buffer_);

        full_lexed_source_.insert(full_lexed_source_.end(),
                                  temp_token_buffer_.begin(),
                                  temp_token_buffer_.end());

      } else {
        build_error_ =
            "Programmer Logic Error, not all included files were fully scanned "
            "for dependencies before initiaitng final build.";
        return;
      }
    }

    // Add the root file last.
    LoadSourceFileToTempBuffer(root_file_.file_path);
    if (ErrorOccured()) return;
    LexFileIntoTempBuffer(temp_file_buffer_);
    if (ErrorOccured()) return;
    RemoveIncludeDirectivesFromSource(temp_token_buffer_);
    full_lexed_source_.insert(full_lexed_source_.end(),
                              temp_token_buffer_.begin(),
                              temp_token_buffer_.end());

    // 3. Parse entire lexed source.
    auto parse_result = ParseTokens(full_lexed_source_);
    if (not parse_result) {
      build_error_ = parse_result.Error();
      return;
    }
    ast_buffer_ = parse_result.Extract();
   
    // 4. Code generation
     
    
    // 5. Linking
    return;
  }

 private:
  inline bool IsRootFile(const stdfs::path& p) const {
    return p == root_file_.file_path;
  }

  void ValidateSourceFile(const stdfs::path& source_file_path) {
    if (!stdfs::exists(source_file_path)) {
      build_error_ =
          GenerateFileErrorMessage(source_file_path, "Does not exist");
      return;
    }
    if (!stdfs::is_regular_file(source_file_path)) {
      build_error_ =
          GenerateFileErrorMessage(source_file_path, "Not a regular file.");
      return;
    }
    if (source_file_path.extension() != ".cand") {
      build_error_ =
          GenerateFileErrorMessage(source_file_path, "Not a .cand file.");
      return;
    }
  }

  inline string GenerateFileErrorMessage(const stdfs::path& source_file,
                                         const string& error) {
    return "Error in file '" + source_file.string() + "':" + error + "\n";
  }

  void LoadSourceFileToTempBuffer(const stdfs::path& source_file_path) {
    ValidateSourceFile(source_file_path);
    if (ErrorOccured()) {
      return;
    }

    std::ifstream source_file_stream(source_file_path);
    if (!source_file_stream.is_open()) {
      build_error_ =
          GenerateFileErrorMessage(source_file_path, "Could not open file.");
    }

    temp_file_buffer_ =
        vector<char>((std::istreambuf_iterator<char>(source_file_stream)),
                     std::istreambuf_iterator<char>());

    source_file_stream.close();
  }

  void LexFileIntoTempBuffer(const vector<char>& source_file_buffer) {
    auto lex_result = Lexer::Lex(source_file_buffer);
    if (lex_result)
      temp_token_buffer_ = lex_result.Extract();
    else
      build_error_ = lex_result.Error();
  }

  void ScanFileDependencies(const vector<Tk>& lexed_source,
                            const stdfs::path& lexed_file_path) {
    TkCursor c = {lexed_source.begin(), lexed_source.end()};
    string lexed_file_name = lexed_file_path.filename().string();
    stdfs::path lexed_file_dir_path = lexed_file_path.parent_path();
    vector<IncludedFile> scanned_included_files;

    // Scan for include directives
    while (!c.AtEnd()) {
      if (c.TypeIsnt(eTk::DirectiveInclude)) {
        c.Advance();
      } else {
        // Include directive must be followed by string literal file name.
        c.Advance();
        if (c.TypeIsnt(eTk::LitCstr)) {
          build_error_ = GenerateFileErrorMessage(
              lexed_file_path,
              "#include directive not followed by a string literal .");
          return;
        }
        // Remove quotes from string literal.
        stdfs::path included_file_path =
            c.Literal().substr(1, c.Literal().size() - 2);

        // File cannot include itself.
        if (included_file_path == lexed_file_name ||
            included_file_path == lexed_file_path) {
          build_error_ = GenerateFileErrorMessage(
              lexed_file_path, "File cannot recursiveley include self.");
          return;
        }

        // If the path is relative. Its relative to the lexed_file_dir_path.
        if (included_file_path.is_relative()) {
          included_file_path = lexed_file_dir_path.native() + L"\\" +
                               included_file_path.native();
        }

        // If the resulting absolute path is the root path, error. Cannot
        // include the root path.
        if (included_file_path == lexed_file_path) {
          build_error_ = GenerateFileErrorMessage(
              lexed_file_path, "Cannot include the root build file.");
          return;
        }

        // Verify file and add to the scanned_included_files if not duplicate.
        ValidateSourceFile(included_file_path);
        if (ErrorOccured()) {
          build_error_ = GenerateFileErrorMessage(
              lexed_file_path.filename(),
              "Invalid inclusion:\n" + build_error_.Error());
          return;
        }
        auto is_duplicate_found = std::find_if(
            scanned_included_files.cbegin(), scanned_included_files.cend(),
            [&included_file_path](const IncludedFile& v) {
              return v.file_path == included_file_path;
            });

        // Duplicate found. Log warning.
        if (is_duplicate_found != scanned_included_files.cend()) {
          warning_log_ << GenerateFileErrorMessage(
              included_file_path,
              "[Warning] Duplicate file inclusion will be implicitly "
              "ignored.");
        } else {
          scanned_included_files.push_back(
              IncludedFile{.file_path = included_file_path});
        }

        // If the file is included in the overall build already
        // Extract it from the build files and replace this included
        // file metadata.
        auto found_in_included =
            std::find_if(included_files_.cbegin(), included_files_.cend(),
                         [&included_file_path](const IncludedFile& v) {
                           return v.file_path == included_file_path;
                         });

        if (found_in_included != included_files_.cend()) {
          // Check all dependencies of file, make sure it does not
          // depend on this file.
          if (DetectCircularDependency(lexed_file_path, included_file_path)) {
            build_error_ = GenerateFileErrorMessage(
                lexed_file_path, "\nCircular dependency detected with file: " +
                                     included_file_path.string());
            return;
          }
          scanned_included_files.back() = *found_in_included;
          included_files_.erase(found_in_included);
        }
      }
    }

    // Add the scanned files to the included files.
    for (const auto& f : scanned_included_files) {
      included_files_.push_back(f);
    }

    // Find this file in the included files and mark it as lexed.
    if (IsRootFile(lexed_file_path)) {
      for (const auto& f : scanned_included_files) {
        root_file_.depencencies.push_back(f.file_path);
      }
      root_file_.was_tokenized = true;
    } else {
      auto in_included =
          std::find_if(included_files_.begin(), included_files_.end(),
                       [&lexed_file_path](const IncludedFile& v) {
                         return v.file_path == lexed_file_path;
                       });
      assert(in_included != included_files_.end() &&
             "Programmer Logic Error: File was not added to build files before "
             "attempting to scan for dependencies.");
      in_included->was_tokenized = true;
      for (const auto& f : scanned_included_files) {
        in_included->depencencies.push_back(f.file_path);
      }
    }
  }

  void RemoveIncludeDirectivesFromSource(vector<Tk>& lexed_source) {
    vector<Tk> cleaned_source;
    TkCursor c = {lexed_source.begin(), lexed_source.end()};
    while (!c.AtEnd()) {
      if (c.TypeIs(eTk::DirectiveInclude)) {
        c.Advance();
        if (c.TypeIs(eTk::LitCstr)) {
          c.Advance();
        }
      } else {
        cleaned_source.push_back(c.Get());
        c.Advance();
      }
    }
    lexed_source = cleaned_source;
  }

  bool DetectCircularDependency(const stdfs::path& dependant,
                                const stdfs::path& dependency) {
    auto in_included =
        std::find_if(included_files_.begin(), included_files_.end(),
                     [&dependency](const IncludedFile& v) {
                       return v.file_path == dependency;
                     });

    if (in_included == included_files_.end()) return false;
    if (in_included->DependsOn(dependant)) return true;
    if (not in_included->depencencies.empty()) {
      for (auto& d : in_included->depencencies) {
        if (DetectCircularDependency(dependant, d)) return true;
      }
    }
    return false;
  }

 private:
  std::stringstream warning_log_;
  cxx::BoolError build_error_;
  IncludedFile root_file_;
  vector<char> temp_file_buffer_;
  vector<Tk> temp_token_buffer_;
  std::list<IncludedFile> included_files_;
  vector<Tk> full_lexed_source_;
  Ast ast_buffer_;
};

//---------------------------------------------------------------------------//
}  // namespace caoco
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
// Copyright 2024 Anton Yashchenko
//
// Licensed under the GNU Affero General Public License, Version 3.
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//---------------------------------------------------------------------------//
// Author(s): Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.acpp.dev
//---------------------------------------------------------------------------//
// Project: C& Programming Language Environment
// Directory: cand-official-compiler
// File: caoco_build.h
//---------------------------------------------------------------------------//
#endif HEADER_GUARD_CALE_CAND_OFFICIAL_COMPILER_CAOCO_BUILD_H
//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//