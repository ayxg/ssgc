#pragma once

#include "../frontend/syntax.hpp"
#include "translation_context.hpp";


namespace ssgc {

  class CompEvalutator {

  std::function<const frontend::NodeFile*(const SourceFile& source_file)> parse_file{};
    std::function<std::string_view(StringId)> get_string{};
  std::function<const SourceFile * (std::string_view, bool overwrite)> load_source{};

    void evalFile(const frontend::NodeFile& node) { 

      //for (const frontend::NodeDirectiveStatement& statement : node.statements) {
      //  compevalFile
      //
      //}
    }

    //void evalDirectiveStatement(const frontend::NodeDirectiveStatement& node) { 
    //  
    //}

    //void evalIncludeStatement(const frontend::NodeStatementInclude & node) { 
    //  std::string_view file_path = get_string(node.id);
    //  if (file_path.empty()) throw "Cannot find file path.";
    //  const SourceFile * src_file = load_source(file_path,false);
    //  if (!src_file) throw "Failed to load source file.";

    //  const frontend::NodeFile* parsed_file_node = parse_file(*src_file);
    //  if (!parsed_file_node) throw "Failed to parse source file.";

    //  evalFile(*parsed_file_node);

    //  return;
    //}


  };


}