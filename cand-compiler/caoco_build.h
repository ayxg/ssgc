///////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
///////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language Environment
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup cand_compiler_data
/// @brief Builder for the C& Official Compiler
///////////////////////////////////////////////////////////////////////////////
#ifndef HEADER_GUARD_CALE_CAND_OFFICIAL_COMPILER_CAOCO_BUILD_H
#define HEADER_GUARD_CALE_CAND_OFFICIAL_COMPILER_CAOCO_BUILD_H
#include <filesystem>

#include "cppsextended.h"
// Includes:
#include "caoco_grammar.h"
#include "caoco_lexer.h"
#include "caoco_lexerv2.h"
#include "caoco_parser.h"

namespace caoco {
using std::forward;
namespace stdfs = std::filesystem;

struct LoadedFile {
  stdfs::path path;
  vector<char> data;
};

/// @brief Loads a file into a vector<char>, validates before loading.
///        Validates source file.
/// @param fp stdfs::path to source file.
/// @return vector<char> of the file or a specific error.
///
/// - Does the file exist?
/// - Is is a regular readable file?
/// - Is it a '.cand' extension file?
static CompResult<LoadedFile> LoadFile(const stdfs::path& fp) {
  if (not stdfs::exists(fp)) {
    return caerr::MakeError(
        caerr::InclusionFailure(fp.string(), "Does not exist"));
  }
  if (not stdfs::is_regular_file(fp)) {
    return caerr::MakeError(
        caerr::InclusionFailure(fp.string(), "Not a regular file."));
  }
  if (fp.extension() != ".cand") {
    return caerr::MakeError(
        caerr::InclusionFailure(fp.string(), "Not a .cand file."));
  }

  std::ifstream source_file_stream(fp);
  if (!source_file_stream.is_open()) {
    return caerr::MakeError(
        caerr::InclusionFailure(fp.string(), "Could not open file."));
  }
  // Load the file...
  vector<char> temp_file_buffer =
      vector<char>((std::istreambuf_iterator<char>(source_file_stream)),
                   std::istreambuf_iterator<char>());
  source_file_stream.close();

  return LoadedFile{fp, temp_file_buffer};
}

struct SourceFile {
  stdfs::path path;
  std::map<size_t, std::map<size_t, char>> data;
};

/// @brief Seperates a file into lines based on newline characters.
///        All newlines are replaced with a single whitespace.
///        All lines are numbered from 1 to n.
static CompResult<SourceFile> ScanLines(const LoadedFile& loaded_file) {
  vector<vector<char>> lines{};
  vector<char> this_line = {};
  for (auto cit = loaded_file.data.cbegin(); cit != loaded_file.data.cend();
       ++cit) {
    // Reject if character is not allowed.
    if (not cand_char::IsValid(*cit)) {
      return caerr::MakeError(
          caerr::ForbiddenSourceChar(loaded_file.path.string(), string{1, *cit},
                                     lines.size(), this_line.size()));
    }

    this_line.push_back(*cit);
    if (cand_char::IsNewline(*cit)) {
      this_line.push_back(' ');
      lines.push_back(this_line);
      this_line.clear();
    }
  }

  // Converts vector of vectors to map of maps
  static constexpr LAMBDA vector_to_map = [](const vector<vector<char>>& v) {
    std::map<size_t, std::map<size_t, char>> m{};
    for (size_t i = 0; i < v.size(); ++i) {
      std::map<size_t, char> line{};
      for (size_t j = 0; j < v[i].size(); ++j) {
        line[j] = v[i][j];
      }
      m[i] = line;
    }
    return m;
  };

  return SourceFile{loaded_file.path, vector_to_map(lines)};
}

struct FlatSource {
  template <class T>
  using OptRef = std::optional<std::reference_wrapper<T>>;
  std::list<stdfs::path> file_order;
  std::map<stdfs::path, SourceFile> data;

 private:
  /// @brief Finds file path at the given index.
  OptRef<stdfs::path> FindPath(size_t index) {
    auto it = file_order.begin();
    if (index >= file_order.size()) return std::nullopt;
    std::advance(it, index);
    return *it;
  }
  /// @brief Finds file path at the given index.
  OptRef<const stdfs::path> FindPath(size_t index) const {
    auto it = file_order.cbegin();
    if (index >= file_order.size()) return std::nullopt;
    std::advance(it, index);
    return *it;
  }

 public:
  /// @brief Adds a file to the back of source. Replaces if already exists.
  void PushBack(const stdfs::path& path, const SourceFile& file) {
    Erase(path);
    file_order.push_back(path);
    data[path] = file;
  }

  /// @brief Adds a file to the front of source.
  void PushFront(const stdfs::path& path, const SourceFile& file) {
    Erase(path);
    file_order.push_front(path);
    data[path] = file;
  }

  /// @brief Insert a file after the given existing file. If file does
  ///        not exist, it is added to the back of the source.
  void Insert(const stdfs::path& path, const SourceFile& file) {
    Erase(path);
    auto it = std::find(file_order.cbegin(), file_order.cend(), path);
    if (it != file_order.cend()) {
      file_order.insert(std::next(it), path);
    } else {
      PushBack(path, file);
    }
  }

  /// @brief Insert a file before the given existing file. If file does
  ///        not exist, it is added to the front of the source.
  void InsertBefore(const stdfs::path& path, const SourceFile& file) {
    Erase(path);
    auto it = std::find(file_order.cbegin(), file_order.cend(), path);
    if (it != file_order.cend()) {
      file_order.insert(it, path);
    } else {
      PushFront(path, file);
    }
  }

  /// @brief Removes a file from the source.
  void Erase(const stdfs::path& path) {
    if (data.contains(path)) {
      data.erase(path);
      file_order.remove(path);
    }
  }

  /// @brief Clears the source.
  void Clear() {
    data.clear();
    file_order.clear();
  }

  /// @brief Returns the source file at the given path.
  OptRef<SourceFile> At(const stdfs::path& path) {
    if (data.contains(path)) return data.at(path);
    return std::nullopt;
  }
  /// @brief Returns the source file at the given path.
  OptRef<const SourceFile> At(const stdfs::path& path) const {
    if (data.contains(path)) return data.at(path);
    return std::nullopt;
  }

  /// @brief Returns the source file at the given index.
  OptRef<SourceFile> At(size_t index) {
    auto path = FindPath(index);
    if (not path.has_value()) return std::nullopt;
    return data.at(path.value());
  }

  /// @brief Returns the source file at the given index.
  OptRef<const SourceFile> At(size_t index) const {
    auto path = FindPath(index);
    if (not path.has_value()) return std::nullopt;
    return data.at(path.value());
  }
};
class FlatSourceIter {
  using iterator_category = std::random_access_iterator_tag;
  using value_type = char;
  using difference_type = std::ptrdiff_t;
  using pointer = const char*;
  using reference = const char&;

  const FlatSource& flat_source_;
  FlatSource::data_type::const_iterator file_it_;
  SourceFile::data_type::const_iterator line_it_;
  SourceFile::line_data_type::const_iterator char_it_;

  std::optional<const char> current_char_;
  void Advance(difference_type n) {
    // Implement logic to advance the iterator by n positions

  }
  public:
    FlatSourceIter(FlatSource& fs, size_t file_idx, size_t line_idx, size_t char_idx)
      : flat_source_(fs), file_idx_(file_idx), line_idx_(line_idx), char_idx_(char_idx) {

  }
};

struct Tokenizer {};

/// @brief Removes comments and line comments from the source.
///        Labels each line with its line number.
static CompResult<
    std::pair<stdfs::path, std::map<size_t, vector<std::pair<size_t, char>>>>>
RemoveComments(
    const std::pair<stdfs::path, vector<vector<char>>>& loaded_file) {
  std::map<size_t, vector<std::pair<size_t, char>>> lines{};
  size_t line_number = 1;
  bool inside_multi_line_comment = false;

  for (const auto& line : loaded_file.data) {
    vector<std::pair<size_t, char>> this_line = {};
    for (auto cit = line.cbegin(); cit != line.cend(); ++cit) {
      if (not inside_multi_line_comment) {
        if (*cit == '`') {  // Line comment -> ignore rest of line.
          line_number++;
          break;
        }
        if (*cit == '/' && *(cit + 1) == '`') {  // Multi-line comment.
          line_number++;
          inside_multi_line_comment = true;
          break;
        }
        this_line.push_back(
            std::make_pair(std::distance(line.cbegin(), cit), *cit));
      } else {
        if (*cit == '`' && *(cit - 1) == '/') {  // End of multi-line comment.
          inside_multi_line_comment = false;
        }
      }
    }
    lines[line_number] = this_line;
    line_number++;
  }

  if (inside_multi_line_comment) {
    return caerr::MakeError(caerr::UnclosedComment(loaded_file.first));
  }

  return std::make_pair(loaded_file.first, lines);
}

// class Preprocessor {};
//
//  class BuildLog {
//  public:
//   void PushError(caerr::CaErrUptr&& err) {
//     errors_.push_back(forward<caerr::CaErrUptr>(err));
//   }
//   void PushWarning(caerr::CaErrUptr&& err) {
//     warnings_.push_back(forward<caerr::CaErrUptr>(err));
//   }
//   void PushGuideline(caerr::CaErrUptr&& err) {
//     guidelines_.push_back(forward<caerr::CaErrUptr>(err));
//   }
//   void PushTutorial(caerr::CaErrUptr&& err) {
//     tutorials_.push_back(forward<caerr::CaErrUptr>(err));
//   }
//   void PushInfo(const string& msg) { build_info_.push_back(msg); }
//
//   void PushError(caerr::CaErrUptrVec&& err) {
//     errors_.reserve(errors_.size() + err.size());
//     for (auto& e : err) {
//       errors_.push_back(std::move(e));
//     }
//   }
//   void PushWarning(caerr::CaErrUptrVec&& err) {
//     warnings_.reserve(warnings_.size() + err.size());
//     for (auto& e : err) {
//       warnings_.push_back(std::move(e));
//     }
//   }
//   void PushGuideline(caerr::CaErrUptrVec&& err) {
//     guidelines_.reserve(guidelines_.size() + err.size());
//     for (auto& e : err) {
//       guidelines_.push_back(std::move(e));
//     }
//   }
//   void PushTutorial(caerr::CaErrUptrVec&& err) {
//     tutorials_.reserve(tutorials_.size() + err.size());
//     for (auto& e : err) {
//       tutorials_.push_back(std::move(e));
//     }
//   }
//   void PushInfo(const vector<string>& msg) {
//     build_info_.reserve(build_info_.size() + msg.size());
//     for (auto& m : msg) {
//       build_info_.push_back(m);
//     }
//   }
//
//   void PrintErrors() {
//     for (const auto& err : errors_) {
//       out_stream_ << FormatLog(err, "Error");
//     }
//   }
//   void PrintWarnings() {
//     for (const auto& err : errors_) {
//       out_stream_ << FormatLog(err, "Warning");
//     }
//   }
//   void PrintGuidelines() {
//     for (const auto& err : errors_) {
//       out_stream_ << FormatLog(err, "Guideline");
//     }
//   }
//   void PrintTutorials() {
//     for (const auto& err : errors_) {
//       out_stream_ << FormatLog(err, "Tutorial");
//     }
//   }
//   void PrintInfo() {
//     for (const auto& err : errors_) {
//       out_stream_ << FormatLog(err, "Info");
//     }
//   }
//
//   void DumpErrors() {
//     for (const auto& err : errors_) {
//       out_stream_ << FormatLog(err, "Error");
//     }
//     errors_.clear();
//   }
//   void DumpWarnings() {
//     for (const auto& err : errors_) {
//       out_stream_ << FormatLog(err, "Warning");
//     }
//     errors_.clear();
//   }
//   void DumpGuidelines() {
//     for (const auto& err : errors_) {
//       out_stream_ << FormatLog(err, "Guideline");
//     }
//     errors_.clear();
//   }
//   void DumpTutorials() {
//     for (const auto& err : errors_) {
//       out_stream_ << FormatLog(err, "Tutorial");
//     }
//     errors_.clear();
//   }
//   void DumpInfo() {
//     for (const auto& err : errors_) {
//       out_stream_ << FormatLog(err, "Info");
//     }
//     errors_.clear();
//   }
//
//   void DumpLastError() {
//     for (const auto& err : errors_) {
//       out_stream_ << FormatLog(err, "Error");
//     }
//     errors_.clear();
//   }
//   void DumpLastWarning() {
//     for (const auto& err : errors_) {
//       out_stream_ << FormatLog(err, "Warning");
//     }
//     errors_.clear();
//   }
//   void DumpLastGuideline() {
//     for (const auto& err : errors_) {
//       out_stream_ << FormatLog(err, "Guideline");
//     }
//     errors_.clear();
//   }
//   void DumpLastTutorial() {
//     for (const auto& err : errors_) {
//       out_stream_ << FormatLog(err, "Tutorial");
//     }
//     errors_.clear();
//   }
//   void DumpLastInfo() {
//     for (const auto& err : errors_) {
//       out_stream_ << FormatLog(err, "Info");
//     }
//     errors_.clear();
//   }
//
//   void OutputLogs() {
//     for (const auto& err : errors_) {
//       out_stream_ << FormatLog(err, "Error");
//     }
//     for (const auto& err : warnings_) {
//       out_stream_ << FormatLog(err, "Warning");
//     }
//     for (const auto& err : guidelines_) {
//       out_stream_ << FormatLog(err, "Guideline");
//     }
//     for (const auto& err : tutorials_) {
//       out_stream_ << FormatLog(err, "Tutorial");
//     }
//   }
//
//  public:
//   explicit BuildLog(const FlatSource& src_code) : src_code_(&src_code) {}
//
//   size_t AddTempFileIndex(const string& name) {
//     temp_file_indices_[temp_index] = name;
//     auto ret = temp_index;
//     temp_index++;
//     return ret;
//   }
//   void ClearTempFileIndices() { temp_file_indices_.clear(); }
//
//  private:
//   /// Given a file index, returns the file name which
//   /// belongs to this index in the src_code_.
//   const string& GetCaErrSourceFilename(size_t file_index) {
//     if (temp_file_indices_.contains(file_index))
//       return temp_file_indices_.at(file_index);
//     return src_code_->files.at(file_index).file_path;
//   }
//
//   /// Given a file index returns a "[formatted-filename]" string.
//   string FormatFilename(size_t file_index) {
//     return "[" + GetCaErrSourceFilename(file_index) + "]";
//   }
//
//   /// Given a file,line and col index formats a pretty highlighted line
//   /// for messages. Prints up to 80 characters per line with the highlighted
//   /// character centred as much as possible. Below is a whitespace line with
//   /// an arrow indicator("/^\") below the highlighted character.
//   /// @code
//   ///   "You made an error here!\n"
//   ///   "                 /^\     "
//   /// @endcode
//   string FormatHighlightedLine(size_t file_index, size_t line, size_t col) {
//     if (src_code_.files.find(file_index) == src_code_.files.end()) {
//       return "File not found!";
//     }
//
//     const auto& file = src_code_.files.at(file_index);
//     if (file.lines.find(line) == file.lines.end()) {
//       return "Line not found!";
//     }
//
//     const std::string& code_line = file.lines.at(line);
//
//     // Determine the start and end indices for the substring to display
//     size_t line_length = code_line.length();
//     size_t display_start = 0;
//     size_t display_length = std::min(static_cast<size_t>(80), line_length);
//
//     if (line_length > 80) {
//       if (col > 40) {
//         if (col + 40 < line_length) {
//           display_start = col - 40;
//         } else {
//           display_start = line_length - 80;
//         }
//       }
//       display_length =
//           std::min(static_cast<size_t>(80), line_length - display_start);
//     }
//
//     std::string display_line = code_line.substr(display_start,
//     display_length);
//
//     // Create the indicator line
//     std::string indicator_line(display_length, ' ');
//     size_t indicator_position = col - display_start;
//
//     if (indicator_position < display_length) {
//       indicator_line[indicator_position] = '/';
//       if (indicator_position + 1 < display_length) {
//         indicator_line[indicator_position + 1] = '^';
//       }
//       if (indicator_position + 2 < display_length) {
//         indicator_line[indicator_position + 2] = '\\';
//       }
//     }
//
//     // Combine and format the output string
//     std::string formatted_output = display_line + "\n" + indicator_line;
//
//     return formatted_output;
//   }
//
//   string FormatLog(const caerr::CaErrUptr& err, const string& category = "")
//   {
//     string ret{""};
//     bool is_file_significant = (err->file != 0);
//     if (is_file_significant)  // File attributed to message.
//       ret += FormatFilename(err->file);
//
//     if (not category.empty()) {
//       ret += "[" + category + "]";
//     }
//
//     // Add the actual error details.
//     ret += err->Format();
//
//     // If line is not 0, and file is significant,
//     // then the error's line loc is significant.
//     if (is_file_significant && (err->line != 0)) {
//       // Print a pretty line of up to 80 chars,
//       // with the highlighted character in the centre.
//       ret += FormatHighlightedLine(err->file, err->line, err->col);
//     }
//
//     return ret;
//   }
//   size_t temp_index{1};
//   // These logs may refer to a line/col in
//   // the source code.
//   caerr::CaErrUptrVec errors_;
//   caerr::CaErrUptrVec warnings_;
//   caerr::CaErrUptrVec guidelines_;
//   caerr::CaErrUptrVec tutorials_;
//
//   /// Generic logs for the build process.
//   /// Only enabled when FULL_BUILD_LOG build option is set.
//   vector<string> build_info_;
//
//   /// Code to refer to when printing log location.
//   std::map<size_t, string> temp_file_indices_;
//   const FlatSource* src_code_;
//
//   /// Stream to output the logs to, default is std::cout.
//   std::ostream& out_stream_{std::cout};
// };
//
//  struct IncludedFile {
//   bool was_tokenized{false};
//
//   // Full absolute path to this file.
//   stdfs::path file_path{""};
//
//   // Files which should be included before this file.
//   // Dependencies should be stored as their absolute path.
//   std::vector<stdfs::path> depencencies{};
//
//   // Root directory which all includes in this file are relative to.
//   stdfs::path IncludeDir() const { return file_path.parent_path(); }
//
//   bool DependsOn(const stdfs::path p) const {
//     return std::find_if(depencencies.cbegin(), depencencies.cend(),
//                         [&p](const auto& v) { return v == p; }) !=
//            depencencies.cend();
//   }
// };
//  namespace stdfs = std::filesystem;
//  template <class T>
//  using CompResult = caerr::CompilerProcessResult<T>;
//  using CompResultBool = caerr::BooleanCompilerProcessResult;
//  class Builder2 {
//  public:
//   CompResultBool Build(const stdfs::path& main_file);
//
//  private:
//   void ImbueFileIndex(size_t index, caerr::CaErrUptrVec& errors) {
//     for (auto& err : errors) {
//       err->file = index;
//     }
//   }
//   CompResultBool ValidateSrcFile(const stdfs::path& fp);
//   CompResult<vector<char>> BufferSrcFile(const stdfs::path& fp);
//   CompResult<TkVector> LexFileBuffer(const vector<char>& char_buff);
//   CompResult<std::list<IncludedFile>> ApplyIncludeDirectives(
//       const vector<Tk>& lexed_src, const stdfs::path& lexed_fp,
//       std::list<IncludedFile>& included);
//   CompResult<std::list<IncludedFile>> ScanDependencies(const stdfs::path&
//   fp); CompResult<FlatSource> FlattenIncluded(
//       const stdfs::path& root_file, const std::list<IncludedFile>&
//       inc_files);
//
//  private:
//   FlatSource flat_src_;
//   BuildLog log_{flat_src_};
//   IncludedFile root_file_;
//   std::list<IncludedFile> included_files_;
// };
//
///// @brief Validates source file.
///// @param fp stdfs::path to source file.
///// @return true if valid, specific error otherwise.
/////
///// - Does the file exist?
///// - Is is a regular readable file?
///// - Is it a '.cand' extension file?
// CompResultBool Builder2::ValidateSrcFile(const stdfs::path& fp) {
//   if (not stdfs::exists(fp)) {
//     return caerr::MakeError(
//         caerr::InclusionFailure(fp.string(), "Does not exist"));
//   }
//   if (not stdfs::is_regular_file(fp)) {
//     return caerr::MakeError(
//         caerr::InclusionFailure(fp.string(), "Not a regular file."));
//   }
//   if (fp.extension() != ".cand") {
//     return caerr::MakeError(
//         caerr::InclusionFailure(fp.string(), "Not a .cand file."));
//   }
//   return true;
// }
//
///// @brief Loads a file into a vector<char>, validates before loading.
///// @param fp stdfs::path to source file.
///// @return vector<char> of the file or a specific error.
// caerr::CompilerProcessResult<vector<char>> Builder2::BufferSrcFile(
//     const stdfs::path& fp) {
//   auto is_valid = ValidateSrcFile(fp);
//   if (not is_valid) {
//     return is_valid.ExtractErrors();
//   }
//
//   std::ifstream source_file_stream(fp);
//   if (!source_file_stream.is_open()) {
//     log_.PushError(caerr::MakeError(
//         caerr::InclusionFailure(fp.string(), "Could not open file.")));
//     return;
//   }
//   // Load the file...
//   vector<char> temp_file_buffer =
//       vector<char>((std::istreambuf_iterator<char>(source_file_stream)),
//                    std::istreambuf_iterator<char>());
//   source_file_stream.close();
//
//   return temp_file_buffer;
// }
//
// CompResult<TkVector> Builder2::LexFileBuffer(const vector<char>& char_buff) {
//   Lexer2 lexer{char_buff.cbegin(), char_buff.cend()};
//   auto lex_res = lexer.Lex();
//   if (not lex_res) {
//     return lex_res;
//   } else
//     return lex_res.Extract();
// };
//
// CompResult<std::list<IncludedFile>> Builder2::ApplyIncludeDirectives(
//     const vector<Tk>& lexed_src, const stdfs::path& lexed_fp,
//     std::list<IncludedFile>& included, IncludedFile& root) {
//   TkCursor c = {lexed_src.begin(), lexed_src.end()};
//   string lexed_file_name = lexed_fp.filename().string();
//   stdfs::path lexed_file_dir_path = lexed_fp.parent_path();
//   vector<IncludedFile> scanned_included_files;
//
//   // Scan for include directives
//   while (!c.AtEnd()) {
//     if (c.TypeIsnt(eTk::DirectiveInclude)) {
//       c.Advance();
//     } else {
//       // Include directive must be followed by string literal file name.
//       c.Advance();
//       if (c.TypeIsnt(eTk::LitCstr)) {
//         return caerr::MakeError(caerr::InclusionFailure(
//             lexed_fp.string(),
//             "#include directive not followed by a string literal ."));
//       }
//       // Remove quotes from string literal.
//       stdfs::path included_file_path =
//           c.Literal().substr(1, c.Literal().size() - 2);
//
//       // File cannot include itself.
//       if (included_file_path == lexed_file_name ||
//           included_file_path == lexed_fp) {
//         return caerr::MakeError(caerr::InclusionFailure(
//             lexed_fp.string(), "File cannot recursiveley include self."));
//       }
//
//       // If the path is relative. Its relative to the lexed_file_dir_path.
//       if (included_file_path.is_relative()) {
//         included_file_path =
//             lexed_file_dir_path.native() + L"\\" +
//             included_file_path.native();
//       }
//
//       // If the resulting absolute path is the root path, error. Cannot
//       // include the root path.
//       if (included_file_path == lexed_fp) {
//         return caerr::MakeError(caerr::InclusionFailure(
//             lexed_fp.string(), "Cannot include the root build file."));
//         return;
//       }
//
//       // Verify file and add to the scanned_included_files if not duplicate.
//       auto is_valid = ValidateSrcFile(included_file_path);
//       if (not is_valid) {
//         return is_valid.ExtractErrors();
//       }
//
//       auto is_duplicate_found = std::find_if(
//           scanned_included_files.cbegin(), scanned_included_files.cend(),
//           [&included_file_path](const IncludedFile& v) {
//             return v.file_path == included_file_path;
//           });
//
//       // Duplicate found ->(do nothing) else add.
//       if (is_duplicate_found != scanned_included_files.cend()) {
//       } else {
//         scanned_included_files.push_back(
//             IncludedFile{.file_path = included_file_path});
//       }
//
//       // If the file is included in the overall build already
//       // Extract it from the build files and replace this included
//       // file metadata.
//       auto found_in_included =
//           std::find_if(included.cbegin(), included.cend(),
//                        [&included_file_path](const IncludedFile& v) {
//                          return v.file_path == included_file_path;
//                        });
//
//       if (found_in_included != included.cend()) {
//         // Check all dependencies of file, make sure it does not
//         // depend on this file.
//         if (DetectCircularDependency(lexed_fp, included_file_path)) {
//           return caerr::MakeError(caerr::InclusionFailure(
//               lexed_fp.string(), "\nCircular dependency detected with file: "
//               +
//                                      included_file_path.string()));
//         }
//         scanned_included_files.back() = *found_in_included;
//         included.erase(found_in_included);
//       }
//     }
//   }
//
//   // Add the scanned files to the included files.
//   for (const auto& f : scanned_included_files) {
//     included.push_back(f);
//   }
//
//   // Find this file in the included files and mark it as lexed.
//   if (lexed_fp == root.file_path) {
//     for (const auto& f : scanned_included_files) {
//       root.depencencies.push_back(f.file_path);
//     }
//     root.was_tokenized = true;
//   } else {
//     auto in_included = std::find_if(
//         included.begin(), included.end(),
//         [&lexed_fp](const IncludedFile& v) { return v.file_path == lexed_fp;
//         });
//     assert(in_included != included.end() &&
//            "Programmer Logic Error: File was not added to build files before
//            " "attempting to scan for dependencies.");
//     in_included->was_tokenized = true;
//     for (const auto& f : scanned_included_files) {
//       in_included->depencencies.push_back(f.file_path);
//     }
//   }
// }
//
// CompResult<std::list<IncludedFile>> Builder2::ScanDependencies(
//     const stdfs::path& fp) {
//   // Load and lex the root file into temp buffer.
//   auto buffer_file_res = BufferSrcFile(fp);
//   if (not buffer_file_res) {
//     return buffer_file_res.ExtractErrors();
//   }
//   auto token_buffer_res = LexFileBuffer(buffer_file_res.Value());
//   if (not token_buffer_res) {
//     return token_buffer_res.ExtractErrors();
//   }
//
//   // Create a list of included files, apply root file inclusions first.
//   std::list<IncludedFile> included_files;
//   auto included_files_res =
//       ApplyIncludeDirectives(token_buffer_res.Value(), fp, included_files);
//
//   if (not included_files_res) {
//     return included_files_res.ExtractErrors();
//   }
//
//   // Continue scanning inclusions until all files have been scanned.
//   for (auto& f : included_files) {
//     if (not f.was_tokenized) {
//       buffer_file_res = BufferSrcFile(f.file_path);
//       if (not buffer_file_res) {
//         return buffer_file_res.ExtractErrors();
//       }
//       token_buffer_res = LexFileBuffer(buffer_file_res.Value());
//       if (not token_buffer_res) {
//         return token_buffer_res.ExtractErrors();
//       }
//
//       included_files_res = ApplyIncludeDirectives(token_buffer_res.Value(),
//                                                   f.file_path,
//                                                   included_files);
//       if (not included_files_res) {
//         return included_files_res.ExtractErrors();
//       }
//     }
//   }
//   return included_files;
// };
//
// Builder2::BuildResult Builder2::Build(const stdfs::path& main_file) {
//   // 1. Lex and load all dependencies into a list which
//   // dictates the inclusion order of files, removing
//   // duplicate inclusions, and detecting ciruclar
//   // dependencies. De-allocated lexed files.
//   auto included_deps_res = ScanDependencies(main_file);
//   if (not included_deps_res) {
//     return included_deps_res.ExtractErrors();
//   }
//   const auto& included_deps = included_deps_res.Value();
//
//   // 2. Lex all included files into a flat source in the correct order
//   // starting from the last included to the root file based on dependencies.
//   // Offset the file index by 1, 0 will represent NO FILE
//   auto flat_src_res = FlattenIncluded(main_file, included_deps);
//   if (not flat_src_res) return flat_src_res.ExtractErrors();
//   const FlatSource& flat_src = flat_src_res.Value();
//   log_.SetSrcCode(flat_src);  // Set the source code to refer to when
//                               // formatting errors.
//
//   //// 3. Parse entire source.
//   // auto parse_result = ParseTokens(flat_src);
//   // if (not parse_result) {
//   //   return parse_result.ExtractErrors();
//   // }
//   // auto ast_buffer = parse_result.Extract();
// };
//
///// @addtogroup cand_compiler_builder
///// @{
//
///// @brief Builder for the C& Official Compiler
/////
/////    - Handles the build pre-processing includes the lexing, parsing, macro
/////      expansion, constevaluator and code generation phases.
/////    - Responsible for managing the source files and temporary files.
/////    - Provides the interface for the user to build an executable of
/// library.
/////    - Provides an interface to set the build options.
//
// class Builder {
// public:
//  void SetRootFile(string_view p);
//  bool ErrorOccured() const { return not build_errors_.empty(); }
//  cstring ErrorMessage() const {
//    return build_errors_.back()->Format().c_str();
//  }
//  const vector<Tk>& GetLexedSource() { return flat_source_.tokens; }
//  void Build();
//
// private:
//  inline bool IsRootFile(const stdfs::path& p) const;
//  void ValidateSourceFile(const stdfs::path& source_file_path);
//  inline string GenerateFileErrorMessage(const stdfs::path& source_file,
//                                         const string& error);
//  void LoadSourceFileToTempBuffer(const stdfs::path& source_file_path);
//  void LexFileIntoTempBuffer(const vector<char>& source_file_buffer);
//  void LexFileIntoTempBufferAndFlattenSource(
//      const vector<char>& source_file_buffer, size_t file_index);
//  void ScanFileDependencies(const vector<Tk>& lexed_source,
//                            const stdfs::path& lexed_file_path);
//  void RemoveIncludeDirectivesFromSource(vector<Tk>& lexed_source);
//  bool DetectCircularDependency(const stdfs::path& dependant,
//                                const stdfs::path& dependency);
//  void AppendFlatSourceFromFile(const stdfs::path& filename,
//                                size_t file_index) {
//    LoadSourceFileToTempBuffer(filename);
//    if (ErrorOccured()) return;
//    LexFileIntoTempBufferAndFlattenSource(temp_file_buffer_, file_index);
//    if (ErrorOccured()) return;
//    RemoveIncludeDirectivesFromSource(temp_token_buffer_);
//    flat_source_.tokens.insert(flat_source_.tokens.end(),
//                               temp_token_buffer_.begin(),
//                               temp_token_buffer_.end());
//    flat_source_.files[file_index] = temp_flat_src_buffer_;
//  }
//
// public:
//  struct IncludedFile {
//    bool was_tokenized{false};
//
//    // Full absolute path to this file.
//    stdfs::path file_path{""};
//
//    // Files which should be included before this file.
//    // Dependencies should be stored as their absolute path.
//    std::vector<stdfs::path> depencencies{};
//
//    // Root directory which all includes in this file are relative to.
//    stdfs::path IncludeDir() const { return file_path.parent_path(); }
//
//    bool DependsOn(const stdfs::path p) const {
//      return std::find_if(depencencies.cbegin(), depencencies.cend(),
//                          [&p](const auto& v) { return v == p; }) !=
//             depencencies.cend();
//    }
//  };
//
// private:
//  std::stringstream warning_log_;
//  caerr::CaErrUptrVec build_errors_;
//  IncludedFile root_file_{};
//  FlatSourceCode temp_flat_src_buffer_;
//  vector<char> temp_file_buffer_;
//  vector<Tk> temp_token_buffer_;
//  std::list<IncludedFile> included_files_;
//  Lexer2 lexer_{};
//  FlatSource flat_source_{};
//  BuildLog log_{flat_source_};
//  vector<Tk> full_lexed_source_{};
//  Ast ast_buffer_;
//};
// void Builder::SetRootFile(string_view p) {
//  ValidateSourceFile(p);
//  if (ErrorOccured()) return;
//  root_file_.file_path = p;
//}
// void Builder::Build() {
//  // 1. Load and lex the root file into temp buffer.
//  LoadSourceFileToTempBuffer(root_file_.file_path);
//  if (ErrorOccured()) return;
//  LexFileIntoTempBuffer(temp_file_buffer_);
//  if (ErrorOccured()) return;
//  ScanFileDependencies(temp_token_buffer_, root_file_.file_path);
//  if (ErrorOccured()) return;
//
//  for (auto& f : included_files_) {
//    if (not f.was_tokenized) {
//      LoadSourceFileToTempBuffer(f.file_path);
//      if (ErrorOccured()) return;
//      LexFileIntoTempBuffer(temp_file_buffer_);
//      if (ErrorOccured()) return;
//      ScanFileDependencies(temp_token_buffer_, f.file_path);
//      if (ErrorOccured()) return;
//    }
//  }
//
//  // 2. Lex all included files into the buffer in the correct order
//  // starting from the last included to the root file
//  // based on dependencies.
//  // Offset the file index by 1, 0 will represent NO FILE
//  for (auto it = included_files_.rbegin(); it != included_files_.rend(); it++)
//  {
//    if (it->was_tokenized) {
//      AppendFlatSourceFromFile(it->file_path,
//                               std::distance(included_files_.rbegin(), it) +
//                               1);
//
//    } else {
//      build_errors_.push_back(caerr::MakeError(caerr::CustomError(
//          "Programmer Logic Error, not all included files were fully scanned "
//          "for dependencies before initiaitng final build.")));
//      return;
//    }
//  }
//
//  // Add the root file last. Offset by +1,and +1 for being outside of the
//  // included files vecctor.
//  AppendFlatSourceFromFile(root_file_.file_path, included_files_.size() + 2);
//
//  // 3. Parse entire lexed source.
//  auto parse_result = ParseTokens(full_lexed_source_);
//  if (not parse_result) {
//    // build_errors_.push_back(parse_result.Error());
//    return;
//  }
//  ast_buffer_ = parse_result.Extract();
//
//  // 4. IRAST Generation
//  // generate an IR-Ast out of the Ast.
//  // - Generates Symbol, Type, Constant tables.
//  // - Performs literal reduction.
//
//  // 6. Transpile collected program data to C++(cail)
//
//  return;
//}
//
// inline bool Builder::IsRootFile(const stdfs::path& p) const {
//  return p == root_file_.file_path;
//}
//
// void Builder::ValidateSourceFile(const stdfs::path& source_file_path) {
//  if (not stdfs::exists(source_file_path)) {
//    build_errors_.push_back(caerr::MakeError(caerr::CustomError(
//        GenerateFileErrorMessage(source_file_path, "Does not exist"))));
//    return;
//  }
//  if (not stdfs::is_regular_file(source_file_path)) {
//    build_errors_.push_back(caerr::MakeError(caerr::CustomError(
//        GenerateFileErrorMessage(source_file_path, "Not a regular file."))));
//    return;
//  }
//  if (source_file_path.extension() != ".cand") {
//    build_errors_.push_back(caerr::MakeError(caerr::CustomError(
//        GenerateFileErrorMessage(source_file_path, "Not a .cand file."))));
//    return;
//  }
//}
//
// inline string Builder::GenerateFileErrorMessage(const stdfs::path&
// source_file,
//                                                const string& error) {
//  return "Error in file '" + source_file.string() + "':" + error + "\n";
//}
//
// void Builder::LoadSourceFileToTempBuffer(const stdfs::path& source_file_path)
// {
//  ValidateSourceFile(source_file_path);
//  if (ErrorOccured()) {
//    return;
//  }
//
//  std::ifstream source_file_stream(source_file_path);
//  if (!source_file_stream.is_open()) {
//    build_errors_.push_back(caerr::MakeError(caerr::CustomError(
//        GenerateFileErrorMessage(source_file_path, "Could not open file."))));
//    return;
//  }
//
//  temp_file_buffer_ =
//      vector<char>((std::istreambuf_iterator<char>(source_file_stream)),
//                   std::istreambuf_iterator<char>());
//
//  source_file_stream.close();
//}
//
// void Builder::LexFileIntoTempBuffer(const vector<char>& source_file_buffer) {
//  auto lex_result = Lexer::Lex(source_file_buffer);
//  if (lex_result) temp_token_buffer_ = lex_result.Extract();
//  // else
//  // build_error_ = lex_result.Error();
//}
//
// void Builder::LexFileIntoTempBufferAndFlattenSource(
//    const vector<char>& source_file_buffer, size_t file_index) {
//  lexer_.Refresh();
//  lexer_.SetSource(source_file_buffer.cbegin(), source_file_buffer.cend());
//  auto lex_result = lexer_.Lex();
//
//  if (lex_result) {
//    temp_token_buffer_ = lex_result.Extract();
//    // Imbue the file index on all tokens of this lex.
//    for (auto& tk : temp_token_buffer_) {
//      tk.SetFile(file_index);
//    }
//    // Get the flat source of this file.
//    temp_flat_src_buffer_ = lexer_.FlatSrc();
//    temp_flat_src_buffer_.file_index = file_index;
//  } else {
//    build_errors_ = lex_result.ExtractErrors();
//  }
//}
//
// void Builder::ScanFileDependencies(const vector<Tk>& lexed_source,
//                                   const stdfs::path& lexed_file_path) {
//  TkCursor c = {lexed_source.begin(), lexed_source.end()};
//  string lexed_file_name = lexed_file_path.filename().string();
//  stdfs::path lexed_file_dir_path = lexed_file_path.parent_path();
//  vector<IncludedFile> scanned_included_files;
//
//  // Scan for include directives
//  while (!c.AtEnd()) {
//    if (c.TypeIsnt(eTk::DirectiveInclude)) {
//      c.Advance();
//    } else {
//      // Include directive must be followed by string literal file name.
//      c.Advance();
//      if (c.TypeIsnt(eTk::LitCstr)) {
//        build_errors_.push_back(
//            caerr::MakeError(caerr::CustomError(GenerateFileErrorMessage(
//                lexed_file_path,
//                "#include directive not followed by a string literal ."))));
//        return;
//      }
//      // Remove quotes from string literal.
//      stdfs::path included_file_path =
//          c.Literal().substr(1, c.Literal().size() - 2);
//
//      // File cannot include itself.
//      if (included_file_path == lexed_file_name ||
//          included_file_path == lexed_file_path) {
//        build_errors_.push_back(
//            caerr::MakeError(caerr::CustomError(GenerateFileErrorMessage(
//                lexed_file_path, "File cannot recursiveley include self."))));
//        return;
//      }
//
//      // If the path is relative. Its relative to the lexed_file_dir_path.
//      if (included_file_path.is_relative()) {
//        included_file_path =
//            lexed_file_dir_path.native() + L"\\" +
//            included_file_path.native();
//      }
//
//      // If the resulting absolute path is the root path, error. Cannot
//      // include the root path.
//      if (included_file_path == lexed_file_path) {
//        build_errors_.push_back(
//            caerr::MakeError(caerr::CustomError(GenerateFileErrorMessage(
//                lexed_file_path, "Cannot include the root build file."))));
//        return;
//      }
//
//      // Verify file and add to the scanned_included_files if not duplicate.
//      ValidateSourceFile(included_file_path);
//      if (ErrorOccured()) {
//        return;
//      }
//
//      auto is_duplicate_found = std::find_if(
//          scanned_included_files.cbegin(), scanned_included_files.cend(),
//          [&included_file_path](const IncludedFile& v) {
//            return v.file_path == included_file_path;
//          });
//
//      // Duplicate found. Log warning.
//      if (is_duplicate_found != scanned_included_files.cend()) {
//        warning_log_ << GenerateFileErrorMessage(
//            included_file_path,
//            "[Warning] Duplicate file inclusion will be implicitly "
//            "ignored.");
//      } else {
//        scanned_included_files.push_back(
//            IncludedFile{.file_path = included_file_path});
//      }
//
//      // If the file is included in the overall build already
//      // Extract it from the build files and replace this included
//      // file metadata.
//      auto found_in_included =
//          std::find_if(included_files_.cbegin(), included_files_.cend(),
//                       [&included_file_path](const IncludedFile& v) {
//                         return v.file_path == included_file_path;
//                       });
//
//      if (found_in_included != included_files_.cend()) {
//        // Check all dependencies of file, make sure it does not
//        // depend on this file.
//        if (DetectCircularDependency(lexed_file_path, included_file_path)) {
//          build_errors_.push_back(
//              caerr::MakeError(caerr::CustomError(GenerateFileErrorMessage(
//                  lexed_file_path,
//                  "\nCircular dependency detected with file: " +
//                      included_file_path.string()))));
//          return;
//        }
//        scanned_included_files.back() = *found_in_included;
//        included_files_.erase(found_in_included);
//      }
//    }
//  }
//
//  // Add the scanned files to the included files.
//  for (const auto& f : scanned_included_files) {
//    included_files_.push_back(f);
//  }
//
//  // Find this file in the included files and mark it as lexed.
//  if (IsRootFile(lexed_file_path)) {
//    for (const auto& f : scanned_included_files) {
//      root_file_.depencencies.push_back(f.file_path);
//    }
//    root_file_.was_tokenized = true;
//  } else {
//    auto in_included =
//        std::find_if(included_files_.begin(), included_files_.end(),
//                     [&lexed_file_path](const IncludedFile& v) {
//                       return v.file_path == lexed_file_path;
//                     });
//    assert(in_included != included_files_.end() &&
//           "Programmer Logic Error: File was not added to build files before "
//           "attempting to scan for dependencies.");
//    in_included->was_tokenized = true;
//    for (const auto& f : scanned_included_files) {
//      in_included->depencencies.push_back(f.file_path);
//    }
//  }
//}
//
// void Builder::RemoveIncludeDirectivesFromSource(vector<Tk>& lexed_source) {
//  vector<Tk> cleaned_source;
//  TkCursor c = {lexed_source.begin(), lexed_source.end()};
//  while (!c.AtEnd()) {
//    if (c.TypeIs(eTk::DirectiveInclude)) {
//      c.Advance();
//      if (c.TypeIs(eTk::LitCstr)) {
//        c.Advance();
//      }
//    } else {
//      cleaned_source.push_back(c.Get());
//      c.Advance();
//    }
//  }
//  lexed_source = cleaned_source;
//}
//
// bool Builder::DetectCircularDependency(const stdfs::path& dependant,
//                                       const stdfs::path& dependency) {
//  auto in_included =
//      std::find_if(included_files_.begin(), included_files_.end(),
//                   [&dependency](const IncludedFile& v) {
//                     return v.file_path == dependency;
//                   });
//
//  if (in_included == included_files_.end()) return false;
//  if (in_included->DependsOn(dependant)) return true;
//  if (not in_included->depencencies.empty()) {
//    for (auto& d : in_included->depencencies) {
//      if (DetectCircularDependency(dependant, d)) return true;
//    }
//  }
//  return false;
//}
//
///// @} // end of cand_compiler_builder

}  // namespace caoco

#endif HEADER_GUARD_CALE_CAND_OFFICIAL_COMPILER_CAOCO_BUILD_H
///////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language Environment
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////////////////