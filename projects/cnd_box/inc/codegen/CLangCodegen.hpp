///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup
/// @brief C/C++ code generator.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup cnd_compiler_cli
/// @{

#pragma once
// clang-format off
#include "ccapi/CommonCppApi.hpp"
// clang-format on
namespace cnd::clang::codegen {

using std::optional;
using std::string;
using std::vector;
using cstring = const char*;

template <class T>
concept iCodeGenerator = requires(std::add_const_t<std::add_lvalue_reference_t<T>> v) {
  { v.Codegen() } -> std::same_as<std::string>;
};

template <class T>
concept iConstReferenceWrapper = requires(std::add_const_t<std::add_lvalue_reference_t<T>> v) {
  { v.Get() } -> std::same_as<std::add_const_t<std::add_lvalue_reference_t<T>>>;
};

static constexpr const char* kCommaSeparator = ", ";

static constexpr Str GenerateExprList(const vector<string>& exprs, const string& separator = kCommaSeparator,
                                      bool separator_after_last = false, const string& open_with = "",
                                      const string& close_with = "", const string& prefix = "") {
  string ret = open_with;
  if (exprs.size() > 1) {
    for (auto arg_iter = exprs.cbegin(); arg_iter != std::prev(exprs.cend()); arg_iter++) {
      ret += prefix + *arg_iter + separator;
    }
  }
  if (not exprs.empty()) {
    ret += prefix + exprs.back();
  }
  return ret + (separator_after_last ? separator : "") + close_with;
}

static constexpr Str GenerateExprList(vector<string>::const_iterator exprs_beg,
                                      vector<string>::const_iterator exprs_end,
                                      const string& separator = kCommaSeparator, bool separator_after_last = false,
                                      const string& open_with = "", const string& close_with = "",
                                      const string& prefix = "");

struct Primitive {
  ePrimitive type{ePrimitive::eValue};
  Str tk{};
};

/// Categorizes the type of primary expression primitive. Indicates how to interpret a primitive's
/// data points upon C code gen.
enum class ePrimitive {
  // The comments below indicate which [indices] will be used in format of output.
  // '[n]' is the index. '...' indicates one or more. '?...' indicates zero or more.
  eValue,   // [0]
  ePre,     // ([1][0])
  ePost,    // ([0][1])
  eBinary,  // ([-1] [0] [1])
  eCall,    // [0] ( [?...] )
};

struct ValueExpression {
  ePrimitive type{ePrimitive::eValue};
  Vec<Primitive> prims{};

  constexpr Str Codegen() const {
    Str out{};
    for (auto it = prims.cbegin(), nxt = prims.cbegin() + 1; it != prims.cend(); it++) {
      if (it->type == ePrimitive::eValue) {
        // If followed by value then this to be a single value expression or else invalid code (operand following
        // operand). Also early return here if next is the end.
        if ((nxt != prims.cend() && nxt->type == ePrimitive::eValue) || nxt == prims.cend()) {
          out += it->tk;
          return out;
        } else {
          continue;  // Handled the next iteration.
        }
      } else if (it->type == ePrimitive::ePre) {
        // If pre then this is a prefix operator.
        out += it->tk + nxt->tk;
        it++;  // Skip the next one as it was already processed.
      } else if (it->type == ePrimitive::ePost) {
        // If post then this is a postfix operator.
        out += nxt->tk + it->tk;
        it++;  // Skip the next one as it was already processed.
      } else if (it->type == ePrimitive::eBinary) {
        // If binary then this is a binary operator.
        out += it->tk + nxt->tk;
        it++;  // Skip the next one as it was already processed.
      } else if (it->type == ePrimitive::eCall) {
        // If call then this is a function call.
        out += it->tk + "(";
        for (auto arg_iter = nxt; arg_iter != prims.cend(); arg_iter++) {
          out += arg_iter->tk;
          if (arg_iter + 1 != prims.cend()) {
            out += ", ";
          }
        }
        out += ")";
        return out;  // No need to continue, we are done with the call.
      }
    }
  };

  static constexpr string GenerateExprList(const vector<string>& exprs, const string& separator = kCommaSeparator,
                                           bool separator_after_last = false, const string& open_with = "",
                                           const string& close_with = "", const string& prefix = "") {
    string ret = open_with;
    if (exprs.size() > 1) {
      for (auto arg_iter = exprs.cbegin(); arg_iter != std::prev(exprs.cend()); arg_iter++) {
        ret += prefix + *arg_iter + separator;
      }
    }
    if (not exprs.empty()) {
      ret += prefix + exprs.back();
    }
    return ret + (separator_after_last ? separator : "") + close_with;
  }

  static constexpr string GenerateExprList(const vector<std::reference_wrapper<const string>>& exprs,
                                           const string& separator = kCommaSeparator, bool separator_after_last = false,
                                           const string& open_with = "", const string& close_with = "",
                                           const string& prefix = "") {
    string ret = open_with;
    if (exprs.size() > 1) {
      for (auto arg_iter = exprs.cbegin(); arg_iter != std::prev(exprs.cend()); arg_iter++) {
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
  static constexpr string GenerateExprList(const vector<T>& exprs, const string& separator = kCommaSeparator,
                                           bool separator_after_last = false, const string& open_with = "",
                                           const string& close_with = "", const string& prefix = "") {
    string ret = open_with;
    if (exprs.size() > 1) {
      for (auto arg_iter = exprs.cbegin(); arg_iter != std::prev(exprs.cend()); arg_iter++) {
        ret += prefix + arg_iter->Codegen() + separator;
      }
    }
    if (not exprs.empty()) {
      ret += prefix + exprs.back().Codegen();
    }
    return ret + (separator_after_last ? separator : "") + close_with;
  }
};

struct IncludeDirective {
  enum class IncludeType {
    Quotes,        ///< Use quotes for the include directive.
    AngleBrackets  ///< Use angle brackets for the include directive.
  };

  string header{};                        ///< The header to include.
  IncludeType type{IncludeType::Quotes};  ///< The type of include directive
                                          ///< (Quotes or AngleBrackets).

  /// @brief Code generation method.
  ///
  /// @return Generated include directive string.
  CND_CX string Codegen() const {
    if (type == IncludeType::Quotes)
      return "#include \"" + header + "\"\n";
    else
      return "#include <" + header + ">\n";
  }
};

/// @brief Generates a #define macro.
struct MacroDefine {
  enum class eMacroType : bool { Definition = true, Functional = false };
  eMacroType macro_type{eMacroType::Definition};
  string ident{""};  /// @brief Name of the macro. Not checked to be unique.
  string def{""};    /// @brief Definition to give the macro. Make sure to
                     ///        use the '\' character before newlines in the
                     ///        macro definition to output correct C++ code.
  vector<string> args{""};

  CND_CX string Codegen() const {
    if (macro_type == eMacroType::Definition) {
      return "#define " + ident + " " + def + "\n";
    } else {
      return GenerateExprList(args, kCommaSeparator, false, "#define " + ident + "(", ") " + def + "\n");
    }
  }
};

/// @brief Represents a template parameter (either a class or a specific type).
struct TemplateTypeParam {
  enum class eTemplateType : bool { Type = true, Value = false };
  string name{""};  ///> Name of the template parameter.
  eTemplateType template_type{eTemplateType::Type};
  string type{""};      ///> The type if this is a value template.
  bool is_pack{false};  ///> True is this is a pack eg. <class...T>
  CND_CX string Codegen() const {
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

/// @brief Generates a template signature, e.g., template<class T>
/// This does not include any 'requires' clause or declaration following
/// the template.When passed an empty param list - an empty template
/// signature is generated: "template<>\n".
struct TemplateSignature {
  vector<TemplateTypeParam> params{};
  CND_CX string Codegen() const { return GenerateExprList(params, kCommaSeparator, false, "template<", ">"); }
};

/// @brief Generates a template specialization, e.g., NAME<TYPE_LIST>
struct TemplateSpecialization {
  string name;            /// @brief The name of the template being specialized
  vector<string> params;  /// @brief List of types for specialization
  CND_CX string Codegen() const { return GenerateExprList(params, kCommaSeparator, false, "template<", ">"); }
};

/// @brief Generates a variable declaration eg. int foo = 42;
struct VariableDecl {
  enum class eInitType {
    Declaration,  ///> No initializer will be generated. Variable declaration.
    BracketInit,  ///> Init will be placed in brackets. eg. int foo(42);
    BraceInit,    ///> Init will be placed in braces. eg. int foo{42};
    Assignment    ///> Init will be assigned by '=' operator. eg. int foo = 42;
  };
  eInitType init_type{eInitType::Declaration};  ///@brief The type of initializer to use
                                                /// for this declaration.
  string ident{""};                             /// @brief Name of the variable.
  string type{""};                              /// @brief Type of the variable.
  string init{""};                              /// @brief Primary expression to emplace in the
                                                ///        variable initializer. Based on init type.
                                                ///        Ignore if init_type is eInitType::Declaration.
                                                ///        This primary expression must not end with a
                                                ///        semicolon, codegen will insert a ';' if necessary.

  CND_CX string Codegen() const {
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
  enum class eTypedefType { Typealias, NamespaceExposition, DeclarationExposition };

  eTypedefType init_type;  ///@brief The category typedef.
  string ident{""};        /// @brief Name of the typedef.
  string init{""};         /// @brief Primary expression to emplace in the type alias.
                           ///        Based on init type. Ignore if init_type is
                           ///        eTypedefType::NamespaceExposition or
                           ///        eTypedefType::DeclarationExposition. This primary
                           ///        expression must not end with a semicolon, codegen
                           ///        will insert a ';' if necessary.

  CND_CX string Codegen() const {
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
  string name;             ///< Name of the enum entry.
  optional<string> value;  ///< Optional default value for the enum entry.

  /// @brief Constructor.
  ///
  /// @param name Name of the enum entry.
  /// @param value Optional default value for the enum entry.
  EnumEntry(const string& name, optional<string> value = std::nullopt) : name(name), value(value) {}

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

/// @brief Generates an enum declaration.
struct EnumDecl {
  string name;
  string type;                    /// @brief Type of the enum. Unspecified if none.
  bool is_scoped = false;         /// @brief If true, generate a scoped enum (enum class).
  vector<EnumEntry> enumerators;  /// @brief List of enumerators.
  CND_CX string GetEnumDecl() const {
    return (is_scoped ? "enum class " + name + " : " + type : "enum " + name + " : " + type);
  }

  CND_CX string Codegen() const {
    if (enumerators.empty())
      return GetEnumDecl() + ";\n";
    else
      return GenerateExprList(enumerators, ",\n  ", false, GetEnumDecl() + " {\n  ", "\n};\n\n");
  }
};

struct MethodDecl;

struct UnionMember {
  string type{};
  string name{};
  optional<string> default_init{std::nullopt};
  CND_CX string Codegen() const { return type + " " + name + (default_init ? "{" + default_init.value() + "}" : ""); }
};

/// @brief Generates a method parameter.
struct MethodParameter {
  string type;                          /// @brief Type of the parameter.
  string name;                          /// @brief Name of the parameter.
  std::optional<string> default_value;  /// @brief Default value initializer.
  bool is_pack = false;                 /// @brief If true, the parameter is a variadic pack.

  CND_CX string Codegen() const {
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

  CND_CX string GenPrefixMods() const {
    string ret = (is_static ? "static " : "");
    ret += (is_constexpr ? "constexpr " : "");
    ret += (is_inline ? "inline " : "");
    return ret;
  }

  CND_CX string GenPostfixMods() const {
    string ret = (is_const ? "const " : "");
    ret += (is_noexcept ? "noexcept " : "");
    return ret;
  }
};
static constexpr MethodDeclModifiers kScxinMods = {true, true, true, false, false};
static constexpr MethodDeclModifiers kCxinMods = {true, false, true, false, false};

struct MethodPostInitializer {
  string member{};
  string expr{};
  string Codegen() const { return member + "(" + expr + ")"; }
};

/// @brief Represents a method declaration and optionally its definition.
struct MethodDecl {
  string name;                     /// @brief Name of the method.
  string return_type{"void"};      /// @brief Return type of the method.
  MethodDeclModifiers mods;        /// @brief If true, the method is static.
  vector<MethodParameter> params;  /// @brief List of parameters in the method.
  optional<string> definition;     /// @brief Optional method definition.
  optional<TemplateSignature> template_signature;
  optional<std::vector<string>> template_specialization;
  optional<string> equal_to;                       // When set, will make the method assigned to
                                                   // string. Definition will be ignored.
  vector<MethodPostInitializer> post_initializer;  // for constructors, generated when not empty.

  optional<string> comment_before;
  optional<string> comment_after;
  CND_CX string Codegen() const {
    return GenerateExprList(
        params, kCommaSeparator, false,
        [=]() {
          return comment_before.value_or("") + (template_signature ? template_signature.value().Codegen() + "\n" : "") +
                 mods.GenPrefixMods() +
                 (template_specialization
                      ? (return_type + " " + name +
                         GenerateExprList(template_specialization.value(), kCommaSeparator, false, "<", ">("))
                      : (return_type + " " + name + "("));
        }(),
        [=] {
          string ret = ")" + mods.GenPostfixMods();
          if (equal_to) {
            ret += " = " + equal_to.value() + ";" + comment_after.value_or("") + "\n\n";
            return ret;
          } else {
            if (not post_initializer.empty()) ret += GenerateExprList(post_initializer, kCommaSeparator, false, " : ");
            if (definition)
              ret += "{" + definition.value() + "}" + comment_after.value_or("") + "\n\n";
            else
              ret += ";\n";
            return ret;
          }
        }());
  }
};

/// @brief Generates a union declaration.
struct UnionDecl {
  struct UnionMemberVariant {
    std::variant<std::reference_wrapper<const UnionMember>, std::reference_wrapper<const MethodDecl>> member;
    string Codegen() const {
      return std::visit([](const auto& m) { return m.get().Codegen(); }, member);
    }
  };

 public:
  string name;                  /// @brief Name of the union.
  vector<UnionMember> members;  /// @brief List of members in the union.
  vector<MethodDecl> methods;   ///@brief List of methods.

  CND_CX string Codegen() const {
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
      return GenerateExprList(all_members, ";\n", true, "union " + name + " {\n", "};\n", "  ");
    }
  }
};

/// @brief Represents a class declaration.
struct ClassDecl {
  string name;                      /// @brief Name of the class.
  optional<string> base_class;      /// @brief Optional base class of the class.
  vector<MethodDecl> methods;       /// @brief List of methods in the class.
  vector<string> member_variables;  /// @brief List of member variables in the class.

  CND_CX string Codegen() const {
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
}  // namespace cnd::clang::codegen

/// @} // end of cnd_compiler_cli

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
//
// This program is free software : you can redistribute it and / or modify it
// under the terms of the GNU Affero General Public License as published by the
// Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////