#pragma once
#include <cstddef>
#include <string_view>

#include "../common/format_diagnostic.hpp"
#include "../common/source_manager.hpp"
#include "../common/source_range.hpp"
#include "../frontend/lexer.hpp"
#include "../frontend/parser.hpp"
#include "../frontend/syntax.hpp"
#include "../frontend/syntax_transformer.hpp"
#include "../frontend/token.hpp"
#include "translation_context.hpp"
namespace ssgc {

class Compiler {
  bool error_occured_{false};
  std::size_t error_count_{0};

 public:
  TrContext context{};

 private:
  /// @brief Resets error state. Should be called at the begginning of every compiler operation.
  void resetErrorState() {
    error_count_ = 0;
    error_occured_ = false;
  }
  void pushError(Diagnostic&& diagnostic) {
    error_occured_ = true;
    error_count_ += 1;
    context.diagnostics->push_back(diagnostic);
  }
  void pushError(const Diagnostic& diagnostic) {
    error_occured_ = true;
    error_count_ += 1;
    context.diagnostics->push_back(diagnostic);
  }
  void pushErrors(Diagnostics&& diagnostics) {
    error_occured_ = true;
    error_count_ += diagnostics->size();
    context.diagnostics.append(diagnostics);
  }
  void pushErrors(const Diagnostics& diagnostics) {
    error_occured_ = true;
    error_count_ += diagnostics->size();
    context.diagnostics.append(diagnostics);
  }

 public:
  /// @brief Returns true if an error occured on the last called compiler operation.
  bool errorOccured() const { return error_occured_; }

  /// @brief Get the quantity of errors which occured on the last called compiler operation.
  std::size_t errorCount() const { return error_count_; }

  /// @brief Get an range iterator pair to the errors which occured on the last called compiler
  /// operation.
  std::pair<std::vector<Diagnostic>::const_iterator, std::vector<Diagnostic>::const_iterator>
  getErrorRange() {
    return {context.diagnostics->cend() - error_count_, context.diagnostics->cend()};
  }

  const Diagnostic& getLastError() { return context.diagnostics->back(); }

  std::string formatDiagnostic(const Diagnostic& diag) const {
    return ::ssgc::formatDiagnostic(diag, &context);
  }

  /// @brief Load a source file into memory for future processing.
  /// @param fp File path.
  /// @param overwrite Overwrite currently loaded file data associated with given path.
  const SourceFile* loadSource(std::string_view fp, bool overwrite = true) {
    resetErrorState();
    auto load_result = context.sources.load(fp, overwrite);
    if (!load_result) {
      context.diagnostics->push_back(load_result.error());
      error_occured_ = true;
      error_count_ = 1;
      return nullptr;
    }
    return *load_result;
  };

  /// @brief Store source file data into memory for future processing.
  /// @param fp File path.
  /// @param overwrite Overwrite currently loaded file data associated with given path.
  const SourceFile* generateSource(std::string_view fp, std::string_view data,
                                   bool overwrite = true) {
    resetErrorState();
    auto gen_result = context.sources.generate(fp, data, overwrite);
    if (!gen_result) {
      context.diagnostics->push_back(gen_result.error());
      error_occured_ = true;
      error_count_ = 1;
      return nullptr;
    }
    return *gen_result;
  };

  /// @brief Tokenize a file which has been loaded into the source list using 'loadSource'
  /// @param file_id
  const std::vector<frontend::Token>* tokenize(std::size_t file_id) {
    resetErrorState();
    const SourceFile* source = context.sources.get(file_id);
    if (source == nullptr) {
      error_occured_ = true;
      error_count_ = 1;
      context.diagnostics->push_back(
          makeErrorPlaceholder(std::format("Could not find source file with id : {}", file_id)));
      return nullptr;
    }

    auto [tokens_iter, was_inserted] = context.tokens.insert({file_id, {}});
    if (!was_inserted) {
      error_occured_ = true;
      error_count_ = 1;
      context.diagnostics->push_back(makeErrorPlaceholder(
          std::format("Source file was already tokenized : {}", source->path)));
      return &tokens_iter->second;
    }

    frontend::Lexer lexer{};
    Diagnostics lexer_diagnostics = lexer.tokenize(source->id, source->data, tokens_iter->second);
    if (!lexer_diagnostics->empty()) {
      error_occured_ = true;
      error_count_ = lexer_diagnostics->size();
      context.diagnostics.append(lexer_diagnostics);
    }

    return &tokens_iter->second;
  }

  // std::string_view sliceSource(std::size_t file, std::size_t offset, std::size_t size) const {
  //   const SourceFile* src = context.sources.get(file);
  //   if (src == nullptr) {
  //     return "";
  //   }
  //   return src->slice(offset, offset + size);
  // }

  // std::string_view sliceSource(frontend::Token token) const {
  //   return sliceSource(token.file, token.offset, token.size);
  // }

  SourceRange getSourceRange(const SourceRangeRaw& source_range) const {
    const SourceFile* src = context.sources.get(source_range.file);
    if (src == nullptr) {
      return SourceRange{};
    }
    auto [begline, begcol] = src->linecol(source_range.begin);
    auto [endline, endcol] = src->linecol(source_range.end);
    return SourceRange{.file = source_range.file,
                       .begin = {.line = begline, .col = begcol},
                       .end = {.line = endline, .col = endcol}};
  }

  const frontend::Ast* parse(std::size_t tokenized_file) {
    resetErrorState();

    const SourceFile* source = context.sources.get(tokenized_file);
    if (source == nullptr) {
      error_occured_ = true;
      error_count_ = 1;
      context.diagnostics->push_back(makeErrorPlaceholder(
          std::format("Could not find source file with id : {}", tokenized_file)));
      return nullptr;
    }

    auto found_it = context.tokens.find(tokenized_file);
    if (found_it == context.tokens.end()) {
      context.diagnostics->push_back(std::format("Source file '{}' has not been tokenized.",
                                                 context.sources.pathof(tokenized_file)));
      error_count_ = 1;
      error_occured_ = true;
    }

    auto [parsed_ast, parse_diagnostics] = frontend::parser::parse(
        0, found_it->second.data(), found_it->second.data() + found_it->second.size());

    if (!parse_diagnostics->empty()) {
      error_count_ = parse_diagnostics->size();
      error_occured_ = true;
      context.diagnostics.append(parse_diagnostics);
    }

    auto [ast_iter, was_inserted] =
        context.syntax_trees.insert({tokenized_file, std::move(parsed_ast)});
    if (!was_inserted) {
      error_occured_ = true;
      error_count_ = 1;
      context.diagnostics->push_back(makeErrorPlaceholder(
          std::format("Source file was already tokenized : {}", source->path)));
      return &ast_iter->second;
    }

    return &ast_iter->second;
  }

  const frontend::NodeFile* transform(SourceFileId ast_file_id) {
    resetErrorState();

    const SourceFile* source_file = context.sources.get(ast_file_id);
    if (source_file == nullptr) {
      this->pushError(std::format("Could not find source file with id : {}", source_file->id));
      return nullptr;
    }

    auto found_it = context.syntax_trees.find(ast_file_id);
    if (found_it == context.syntax_trees.end()) {
      this->pushError(std::format("Source file '{}' has not been tokenized.",
                                  context.sources.pathof(source_file->path)));
    }
    auto& [file_id, ast] = *found_it;

    ssgc::frontend::TransformationContext transform_context{
        .id_table = context.id_table,
        .string_table = context.string_table,
        .source_file = *source_file,
        .tokens = context.tokens.at(ast_file_id)};
    auto [transformed_ast, transform_diagnostics] = frontend::transformFile(ast, transform_context);
    if (!transform_diagnostics->empty()) {
      this->pushErrors(transform_diagnostics);
    }

    auto [ast_iter, was_inserted] =
        context.syntax_nodes.insert({ast_file_id, std::move(transformed_ast)});
    if (!was_inserted) {
      error_occured_ = true;
      error_count_ = 1;
      context.diagnostics->push_back(makeErrorPlaceholder(std::format(
          "Source file was already transformed : {}", context.sources.pathof(source_file->path))));
      return &ast_iter->second;
    }

    return &ast_iter->second;
  }

  void push_error(Diagnostic&& diagnostic) {
    this->error_occured_ = true;
    this->error_count_ += 1;
    this->context.diagnostics->push_back(diagnostic);
  }

  void push_errors(Diagnostics&& diagnostics) {
    this->error_occured_ = true;
    this->error_count_ += diagnostics->size();
    this->context.diagnostics.append(diagnostics);
  }

  /// @brief Load a source file into memory for future processing.
  /// @param fp File path.
  /// @param overwrite Overwrite currently loaded file data associated with given path.
  const SourceFile* load_source(std::string_view fp, bool overwrite = true) {
    auto load_result = context.sources.load(fp, overwrite);
    if (!load_result) {
      this->push_error(std::move(load_result.error()));
      return nullptr;
    }
    return *load_result;
  };

  const frontend::NodeFile* parse_file(const SourceFile& source_file) {
    // Make sure file was no already parsed and create entries for each stage in the
    // maps which will contain the resulting data.
    // - this->context.tokens
    // - this->context.syntax_trees
    // - this->context.syntax_nodes
    std::string_view source_file_path = this->context.sources.pathof(source_file.id);

    auto [tokens_data_iter, tokens_data_was_inserted] =
        this->context.tokens.insert({source_file.id, {}});
    if (!tokens_data_was_inserted) {
      this->push_error(std::format("Source file was already tokenized : {}", source_file_path));
      return nullptr;
    }

    auto [ast_iter, ast_was_inserted] = context.syntax_trees.insert({source_file.id, {}});
    if (!ast_was_inserted) {
      this->push_error(std::format("Source file was already parsed : {}", source_file_path));
      return nullptr;
    }

    auto [node_iter, node_was_inserted] = context.syntax_nodes.insert({source_file.id, {}});
    if (!node_was_inserted) {
      this->push_error(std::format("Source file was already transformed : {}", source_file_path));
      return nullptr;
    }

    // Perform all parsing steps for this source file and store data in the maps.
    // - Tokenize raw source data
    // - Parse tokens into abstract syntax tree
    // - Validate and transform into node tree
    std::vector<frontend::Token>& tokens = tokens_data_iter->second;
    frontend::Ast& ast = ast_iter->second;
    frontend::NodeFile& node = node_iter->second;

    frontend::Lexer lexer{};
    Diagnostics lexer_diagnostics = lexer.tokenize(source_file.id, source_file.data, tokens);
    if (!lexer_diagnostics->empty()) {
      this->push_errors(std::move(lexer_diagnostics));
    }

    auto [parsed_ast, parse_diagnostics] =
        frontend::parser::parse(0, tokens.data(), tokens.data() + tokens.size());
    if (!parse_diagnostics->empty()) {
      this->push_errors(std::move(parse_diagnostics));
    }

    ssgc::frontend::TransformationContext transform_context{
        .id_table = context.id_table,
        .string_table = context.string_table,
        .source_file = source_file,
        .tokens = context.tokens.at(source_file.id)};
    auto [transformed_ast, transform_diagnostics] = frontend::transformFile(ast, transform_context);
    if (!transform_diagnostics->empty()) {
      this->pushErrors(transform_diagnostics);
    }

    return &node;
  }

  Diagnostics compeval(const frontend::NodeFile& root_node) { return Diagnostics{}; }

  void compile(std::string_view root_source_file_path) {
    const SourceFile* source_file = this->load_source(root_source_file_path);
    if (source_file == nullptr) return;

    const frontend::NodeFile* root_node = this->parse_file(*source_file);
    
    compeval(*root_node);


  }
};

}  // namespace ssgc