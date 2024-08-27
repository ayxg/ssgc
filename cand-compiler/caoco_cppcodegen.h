#pragma once
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

#ifndef HEADER_GUARD_CALE_CAND_OFFICIAL_COMPILER_CAOCO_CPPCODEGEN_H
#define HEADER_GUARD_CALE_CAND_OFFICIAL_COMPILER_CAOCO_CPPCODEGEN_H
#include <cassert>
#include <exception>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include "cppsextended.h"

// Some local macros to reduce noise...
#define _cx constexpr
#define _cxin constexpr inline
#define _scx static constexpr
#define _scxin static constexpr inline
#define _cxin constexpr inline

namespace caoco::cppcodegen {
using std::optional;
using std::string;
using std::vector;
using cstring = const char*;

/// @addtogroup cand_compiler_cppcodegen
/// @{

/// @defgroup cand_compiler_cppcodegen_utils Codegen Utils
/// @addtogroup cand_compiler_cppcodegen
/// @{

template <class T>
concept iCodeGenerator =
    requires(std::add_const_t<std::add_lvalue_reference_t<T>> v) {
      { v.Codegen() } -> std::same_as<string>;
    };

_scxin cstring kCommaSeparator = ", ";

_scxin string GenerateExprList(const vector<string>& exprs,
                               const string& separator = kCommaSeparator,
                               bool separator_after_last = false,
                               const string& open_with = "",
                               const string& close_with = "",
                               const string& prefix = "") {
  string ret = open_with;
  if (exprs.size() > 1) {
    for (auto arg_iter = exprs.cbegin(); arg_iter != std::prev(exprs.cend());
         arg_iter++) {
      ret += prefix + *arg_iter + separator;
    }
  }
  if (not exprs.empty()) {
    ret += prefix + exprs.back();
  }
  return ret + (separator_after_last ? separator : "") + close_with;
}

_scxin string GenerateExprList(
    const vector<std::reference_wrapper<const string>>& exprs,
    const string& separator = kCommaSeparator,
    bool separator_after_last = false, const string& open_with = "",
    const string& close_with = "", const string& prefix = "") {
  string ret = open_with;
  if (exprs.size() > 1) {
    for (auto arg_iter = exprs.cbegin(); arg_iter != std::prev(exprs.cend());
         arg_iter++) {
      ret += prefix + arg_iter->get() + separator;
    }
  }
  if (not exprs.empty()) {
    ret += prefix + exprs.back().get();
  }
  return ret + (separator_after_last ? separator : "") + close_with;
}

template <class T>
  requires iCodeGenerator<T>
_scxin string GenerateExprList(const vector<T>& exprs,
                               const string& separator = kCommaSeparator,
                               bool separator_after_last = false,
                               const string& open_with = "",
                               const string& close_with = "",
                               const string& prefix = "") {
  string ret = open_with;
  if (exprs.size() > 1) {
    for (auto arg_iter = exprs.cbegin(); arg_iter != std::prev(exprs.cend());
         arg_iter++) {
      ret += prefix + arg_iter->Codegen() + separator;
    }
  }
  if (not exprs.empty()) {
    ret += prefix + exprs.back().Codegen();
  }
  return ret + (separator_after_last ? separator : "") + close_with;
}

/// @} // end of cand_compiler_cppcodegen_utils

struct IncludeDirective {
  enum class IncludeType {
    Quotes,        /// Use quotes for the include directive.
    AngleBrackets  /// Use angle brackets for the include directive.
  };
  /// The header to include.
  string header{};                       
  /// The type of include directive
  /// (Quotes or AngleBrackets).
  IncludeType type{IncludeType::Quotes};  

  /// @brief Code generation method.
  ///
  /// @return Generated include directive string.
  _cx string Codegen() const {
    if (type == IncludeType::Quotes)
      return "#include \"" + header + "\"\n";
    else
      return "#include <" + header + ">\n";
  }
};

/// Generates a #define macro.
struct MacroDefine {
  enum class eMacroType : bool { Definition = true, Functional = false };
  eMacroType macro_type{eMacroType::Definition};
  /// Name of the macro.Not checked to be unique.
  string ident{""};  
  /// Definition to give the macro. Make sure to
  /// use the '\' character before newlines in the
  /// macro definition to output correct C++ code.
  string def{""};    
  vector<string> args{""};

  _cx string Codegen() const {
    if (macro_type == eMacroType::Definition) {
      return "#define " + ident + " " + def + "\n";
    } else {
      return GenerateExprList(args, kCommaSeparator, false,
                              "#define " + ident + "(", ") " + def + "\n");
    }
  }
};

/// Represents a template parameter (either a class or a specific type).
struct TemplateTypeParam {
  enum class eTemplateType : bool { Type = true, Value = false };
  /// Name of the template parameter.
  string name{""};  
  eTemplateType template_type{eTemplateType::Type};
  /// The type if this is a value template.
  string type{""};      
  /// True is this is a pack eg. <class...T>
  bool is_pack{false};  
  _cx string Codegen() const {
    if (template_type == eTemplateType::Type)
      if (is_pack)
        return "class ... " + name;
      else
        return "class " + name;
    else {  // eTemplateType::Value
      if (is_pack)
        return type + " ... " + name;
      else
        return type + " " + name;
    }
  }
};

/// Generates a template signature, e.g., template<class T>
/// 
/// This does not include any 'requires' clause or declaration following
/// the template.When passed an empty param list - an empty template
/// signature is generated: "template<>\n".
struct TemplateSignature {
  vector<TemplateTypeParam> params{};
  _cx string Codegen() const {
    return GenerateExprList(params, kCommaSeparator, false, "template<", ">");
  }
};

/// Generates a template specialization, e.g., NAME<TYPE_LIST>
struct TemplateSpecialization {
  /// The name of the template being specialized
  string name;            
  /// List of types for specialization
  vector<string> params;  
  _cx string Codegen() const {
    return GenerateExprList(params, kCommaSeparator, false, "template<", ">");
  }
};

/// Generates a variable declaration eg. int foo = 42;
struct VariableDecl {
  enum class eInitType {
    Declaration,  /// No initializer will be generated. Variable declaration.
    BracketInit,  /// Init will be placed in brackets. eg. int foo(42);
    BraceInit,    /// Init will be placed in braces. eg. int foo{42};
    Assignment    /// Init will be assigned by '=' operator. eg. int foo = 42;
  };
  /// The type of initializer to use for this declaration.
  eInitType init_type{
      eInitType::Declaration};  
  string ident{""};             
  string type{""};              
  /// Primary expression to emplace in the
  /// variable initializer. Based on init type.
  /// Ignore if init_type is eInitType::Declaration.
  /// This primary expression must not end with a
  /// semicolon, codegen will insert a ';' if necessary.
  string init{
      ""};  

  _cx string Codegen() const {
    switch (init_type) {
      case eInitType::Declaration:
        return type + " " + ident + ";\n";
      case eInitType::BracketInit:
        return type + " " + ident + "(" + init + ");\n";
      case eInitType::BraceInit:
        return type + " " + ident + "{" + init + "};\n";
      case eInitType::Assignment:
        return type + " " + ident + " = " + init + ";\n";
      default:
        throw cxx::UnknownEnumEntry<eInitType>();
    }
  }
};

/// @brief Generates a using declaration eg. using Int = int;
struct UsingDecl {
  enum class eTypedefType {
    Typealias,
    NamespaceExposition,
    DeclarationExposition
  };
  /// The category typedef.
  eTypedefType init_type;  
  /// Name of the typedef.
  string ident{""};    

  /// Primary expression to emplace in the type alias.
  /// Based on init type. Ignore if init_type is
  /// eTypedefType::NamespaceExposition or
  /// eTypedefType::DeclarationExposition. This primary
  /// expression must not end with a semicolon, codegen
  /// will insert a ';' if necessary.
  string init{""};  

  _cx string Codegen() const {
    switch (init_type) {
      case eTypedefType::Typealias:
        return "using " + ident + " = " + init + ";\n";
      case eTypedefType::NamespaceExposition:
        return "using namespace " + ident + ";\n";
      case eTypedefType::DeclarationExposition:
        return "using " + ident + ";\n";
      default:
        throw cxx::UnknownEnumEntry<eTypedefType>();
    }
  }
};

struct EnumEntry {
  /// Name of the enum entry.
  string name;             
  /// Optional default value for the enum entry.
  optional<string> value;  

  /// @brief Constructor.
  ///
  /// @param name Name of the enum entry.
  /// @param value Optional default value for the enum entry.
  EnumEntry(const string& name, optional<string> value = std::nullopt)
      : name(name), value(value) {}

  /// @brief Code generation method.
  ///
  /// @return Generated enum entry string.
  string Codegen() const {
    assert(!name.empty() && "Enum entry name cannot be empty.");

    if (value.has_value()) {
      return name + " = " + value.value();
    } else {
      return name;
    }
  }
};

/// Generates an enum declaration.
struct EnumDecl {
  string name;
  /// Type of the enum. Unspecified if none.
  string type;  
  /// If true, generate a scoped enum (enum class).
  bool is_scoped =
      false;  
  /// List of enumerators.
  vector<EnumEntry> enumerators;
  _cx string GetEnumDecl() const {
    return (is_scoped ? "enum class " + name + " : " + type
                      : "enum " + name + " : " + type);
  }

  _cx string Codegen() const {
    if (enumerators.empty())
      return GetEnumDecl() + ";\n";
    else
      return GenerateExprList(enumerators, ",\n  ", false,
                              GetEnumDecl() + " {\n  ", "\n};\n\n");
  }
};

struct MethodDecl;

struct UnionMember {
  string type{};
  string name{};
  optional<string> default_init{std::nullopt};
  _cx string Codegen() const {
    return type + " " + name +
           (default_init ? "{" + default_init.value() + "}" : "");
  }
};

/// Generates a method parameter.
struct MethodParameter {
  string type;                          
  string name;                          
  std::optional<string> default_value;
  /// If true, the parameter is a variadic pack.
  bool is_pack = false;  

  _cx string Codegen() const {
    string result = type + " ";
    if (is_pack) {
      result += "... ";
    }
    result += name;

    if (default_value.has_value()) {
      result += " = " + default_value.value();
    }

    return result;
  }
};

struct MethodDeclModifiers {
  bool is_constexpr{false};
  bool is_static{false};
  bool is_inline{false};
  bool is_noexcept{false};
  bool is_const{false};

  _cxin string GenPrefixMods() const {
    string ret = (is_static ? "static " : "");
    ret += (is_constexpr ? "constexpr " : "");
    ret += (is_inline ? "inline " : "");
    return ret;
  }

  _cxin string GenPostfixMods() const {
    string ret = (is_const ? "const " : "");
    ret += (is_noexcept ? "noexcept " : "");
    return ret;
  }
};
constexpr MethodDeclModifiers kScxinMods = {true, true, true, false, false};
constexpr MethodDeclModifiers kCxinMods = {true, false, true, false, false};

struct MethodPostInitializer {
  string member{};
  string expr{};
  string Codegen() const { return member + "(" + expr + ")"; }
};
/// Represents a method declaration and optionally its definition.
struct MethodDecl {
  string name;                     
  string return_type{"void"};      
  MethodDeclModifiers mods;        
  vector<MethodParameter> params;  
  optional<string> definition;     
  optional<TemplateSignature> template_signature;
  optional<std::vector<string>> template_specialization;
  /// When set, will make the method assigned to
  /// string with '=' operator. Definition will be ignored.
  optional<string> equal_to;  
  /// for constructors, generated when not empty.
  vector<MethodPostInitializer>
      post_initializer;  

  optional<string> comment_before;
  optional<string> comment_after;
  _cx string Codegen() const {
    return GenerateExprList(
        params, kCommaSeparator, false,
        [=]() {
          return comment_before.value_or("") +
                 (template_signature
                      ? template_signature.value().Codegen() + "\n"
                      : "") +
                 mods.GenPrefixMods() +
                 (template_specialization
                      ? (return_type + " " + name +
                         GenerateExprList(template_specialization.value(),
                                          kCommaSeparator, false, "<", ">("))
                      : (return_type + " " + name + "("));
        }(),
        [=] {
          string ret = ")" + mods.GenPostfixMods();
          if (equal_to) {
            ret += " = " + equal_to.value() + ";" + comment_after.value_or("") +
                   "\n\n";
            return ret;
          } else {
            if (not post_initializer.empty())
              ret += GenerateExprList(post_initializer, kCommaSeparator, false,
                                      " : ");
            if (definition)
              ret += "{" + definition.value() + "}" +
                     comment_after.value_or("") + "\n\n";
            else
              ret += ";\n";
            return ret;
          }
        }());
  }
};

/// Generates a union declaration.
struct UnionDecl {
  struct UnionMemberVariant {
    std::variant<std::reference_wrapper<const UnionMember>,
                 std::reference_wrapper<const MethodDecl>>
        member;
    string Codegen() const {
      return std::visit([](const auto& m) { return m.get().Codegen(); },
                        member);
    }
  };

 public:
  string name;                  
  vector<UnionMember> members;  
  vector<MethodDecl> methods;   

  _cx string Codegen() const {
    if (members.empty())
      return "union " + name + ";\n";
    else {
      std::vector<UnionMemberVariant> all_members;
      all_members.reserve(members.size() + methods.size());
      for (const auto& m : members) {
        all_members.push_back(UnionMemberVariant{std::ref(m)});
      }
      for (const auto& m : methods) {
        all_members.push_back(UnionMemberVariant{std::ref(m)});
      }
      return GenerateExprList(all_members, ";\n", true,
                              "union " + name + " {\n", "};\n", "  ");
    }
  }
};

/// Represents a class declaration.
struct ClassDecl {
  string name;                  
  optional<string> base_class;  
  vector<MethodDecl> methods;   
  vector<string>
      member_variables;  

  _cx string Codegen() const {
    string result = "class " + name;

    if (base_class.has_value()) {
      result += " : public " + base_class.value();
    }

    result += " {\npublic:\n";

    // Generate member variables
    for (const auto& member : member_variables) {
      result += "    " + member + ";\n";
    }

    // Generate methods
    for (const auto& method : methods) {
      result += "    " + method.Codegen() + ";\n";
    }

    result += "};\n";

    return result;
  }
};

/// @} // end of cand_compiler_cppcodegen

}  // namespace caoco::cppcodegen

#undef _cx 
#undef _cxin 
#undef _scx 
#undef _scxin 
#undef _cxin 

#endif HEADER_GUARD_CALE_CAND_OFFICIAL_COMPILER_CAOCO_CPPCODEGEN_H
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