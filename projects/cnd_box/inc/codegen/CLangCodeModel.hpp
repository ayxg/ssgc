///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2025 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup
/// @brief
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup cnd_compiler
/// @{
#pragma once
// clang-format off
#include "use_ccapi.hpp"
#include "use_corevals.hpp"
#include "use_clmsg.hpp"

#include "cldata/tk.hpp"
#include "trtools/Lexer.hpp"
#include "trtools/Parser.hpp"
//#include "trtools/Compeval.hpp"

#include "TrInput.hpp"
#include "TrOutput.hpp"
#include "TrUnit.hpp"

//#include "VirtualMachine.hpp"
//#include "IrGenerator.hpp"
// clang-format on

namespace cnd {

namespace clang {

namespace codegen {

// C\C++ Codegen Constants
static constexpr CStr kCommaSeparator = ", ";

// C\C++ Codegen Utilities
static constexpr Str GenerateExprList(const Vec<Str>& exprs, const Str& separator = ", ",
                                      bool separator_after_last = false, const Str& open_with = "",
                                      const Str& close_with = "", const Str& prefix = "");
static constexpr Str GenerateExprList(Vec<Str>::const_iterator exprs_beg, Vec<Str>::const_iterator exprs_end,
                                      const Str& separator = ", ", bool separator_after_last = false,
                                      const Str& open_with = "", const Str& close_with = "", const Str& prefix = "");

// @note It makes sense to create a functor object for each syntax element we need to generate. Instead of having huge
// functions which take a lot of params.

struct CodeModel;
struct TrUnit;

// Preprocessor
struct MacroInclude;
struct MacroDef;
struct MacroPragma;

// Primary
struct Expr;        // Value/type expression primitive.
struct Decl;        // Declaration  [syntax]     ::= [decl-sequence]* |  decl-block
struct Stmt;        // Statement    [fn-def]     ::= [stmt-sequence]* |  fn-block
struct LoopStmt;    // Loop    [loop-def]   ::= [loop-stmt-sequence]* |  loop-block
struct StructStmt;  // Struct    [class-def]  ::= [struct-stmt-sequence]* |  struct-block

// Variable
struct VarDecl;

// Method
struct FnDecl;
struct FnDeclMods;
struct FnParam;
struct FnPostInitializer;

// Enum
struct EnumDecl;
struct EnumEntry;

// Union
struct UnionDecl;
struct UnionMember;

// Class (and struct)
struct ClassDecl;

// Templates
struct TemplateTypeParam;
struct TemplateSignature;
struct TemplateSpecialization;

}  // namespace codegen

namespace codegen {
static constexpr Str GenerateExprList(const Vec<Str>& exprs, const Str& separator = ", ",
                                      bool separator_after_last = false, const Str& open_with = "",
                                      const Str& close_with = "", const Str& prefix = "") {
  Str ret = open_with;
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

}  // namespace codegen

namespace codegen {

/// Models an C\C++ #include macro.
struct IncludeDirective {
  enum class IncludeType { Quotes, AngleBrackets };

  Str file{};
  IncludeType type{IncludeType::Quotes};

  constexpr Str Codegen() const {
    if (type == IncludeType::Quotes)
      return "#include \"" + file + "\"\n";
    else
      return "#include <" + file + ">\n";
  }
};

/// Models a C\C++ #define macro.
struct MacroDef {
  enum class eMacroType : bool { Definition = true, Functional = false };
  eMacroType type{eMacroType::Definition};
  Str ident{""};
  Str def{""};  /// @brief Definition to give the macro. Make sure to
                ///        use the '\' character before newlines in the
                ///        macro definition to output correct C++ code.
  Vec<Str> args{""};

  constexpr Str Codegen() const {
    if (type == eMacroType::Definition)
      return "#define " + ident + " " + def + "\n";
    else  // Functional
      return GenerateExprList(args, kCommaSeparator, false, "#define " + ident + "(", ") " + def + "\n");
  }
};

/// Models a C\C++ #pragma macro.
struct PragmaDirective {
  Str params;
  Str Codegen() const { return "#pragma " + params + "\n"; }
};

/// Models a C\C++ primary value or type expression.
struct Expr {
  enum class eOpType { Literal, Ident, Call, Binary, Postfix, Prefix };
  eOpType operation;
  Str lit;
  Vec<Expr> operands;

  static constexpr Expr FromCndAst(const Ast& ast) {
    if (ast.IsLiteral() || ast.type == eAst::kIdent)
      return Expr{eOpType::Literal, ast.GetLiteral(), {}};
    else if (ast.type == eAst::kFunctionCall) {
      Expr expr{eOpType::Call, ast.GetLiteral(), {}};
      expr.operands.reserve(ast.children.size() - 1);
      for (const auto& branch : std::ranges::subrange(ast.children.cbegin() + 1, ast.children.cend())) {
        expr.operands.push_back(Expr::FromCndAst(branch));
      }
      return expr;
    } else if (GetAstOperation(ast.type) == eOperation::Binary) {
      Expr expr{eOpType::Binary, {""}, {}};
      // Based on the C& binary op type provide the matching C\C++ operator, or analogous function call.
      switch (ast.type) {
        case eAst::kAdd:
          expr.lit = "+";
          break;
        case eAst::kSub:
          expr.lit = "-";
          break;
        case eAst::kMul:
          expr.lit = "*";
          break;
        case eAst::kDiv:
          expr.lit = "/";
          break;
        case eAst::kMod:
          expr.lit = "%";
          break;
        case eAst::kBand:
          expr.lit = "&";
          break;
        case eAst::kBor:
          expr.lit = "|";
          break;
        case eAst::kXor:
          expr.lit = "^";
          break;
        case eAst::kLsh:
          expr.lit = "<<";
          break;
        case eAst::kRsh:
          expr.lit = ">>";
          break;
        default:
          throw cxx::UnknownEnumEntry<eAst>();
      }
      return Expr{eOpType::Binary, ast.GetLiteral(), {Expr::FromCndAst(ast.At(0)), Expr::FromCndAst(ast.At(1))}};
    } else if (GetAstOperation(ast.type) == eOperation::Postfix) {
      Expr expr{eOpType::Postfix, {""}, {}};
      switch (ast.type) {
        case eAst::kPostfixInc:
          expr.lit = "++";
          break;
        case eAst::kPostfixDec:
          expr.lit = "--";
          break;
        default:
          throw cxx::UnknownEnumEntry<eAst>();
      }
    } else if (GetAstOperation(ast.type) == eOperation::Prefix) {
      Expr expr{eOpType::Prefix, {""}, {}};
      switch (ast.type) {
        case eAst::kPrefixInc:
          expr.lit = "++";
          break;
        case eAst::kPrefixDec:
          expr.lit = "--";
          break;
        case eAst::kPrefixNot:
          expr.lit = "!";
          break;
        case eAst::kPrefixBitNot:
          expr.lit = "~";
          break;
        default:
          throw cxx::UnknownEnumEntry<eAst>();
      }
    }
  }

  constexpr Str Codegen() const {
    switch (operation) {
      case eOpType::Literal:
      case eOpType::Ident:
        return lit;
      case eOpType::Call: {
        Vec<Str> fnargs{};
        fnargs.reserve(operands.size());
        for (const auto& arg : operands) fnargs.push_back(arg.Codegen());
        return lit + "(" + GenerateExprList(fnargs) + ")";
      }
      case eOpType::Binary:
        return "(" + operands[0].Codegen() + " " + lit + " " + operands[1].Codegen() + ")";
      case eOpType::Postfix:
        return "(" + operands[0].Codegen() + lit + ")";
      case eOpType::Prefix:
        return "(" + lit + operands[0].Codegen() + ")";
      default:
        throw cxx::UnknownEnumEntry<eOpType>();
    }
  }
};

/// Models a C\C++ declaration appearing at the top level translation unit syntax. 
///   [syntax] ::= [decl-sequence]* | decl-block
struct Decl {
  enum class eDeclType { Variable, Function, TypeAlias, Struct, Union, Enum, Class };
  eDeclType type{eDeclType::Variable};
  Str ident{""};  /// @brief Name of the declaration.
  Str type_name{""};  /// @brief Type of the declaration.
  Vec<codegen::Expr> init_exprs{};  /// @brief Initializer expressions for the declaration.
  Vec<codegen::Expr> args{};         /// @brief Arguments for function declarations.
  Vec<codegen::Expr> modifiers{};    /// @brief Modifiers for function declarations.

  constexpr Str Codegen() const {
    switch (type) {
      case eDeclType::Variable:
        return codegen::VariableDecl{codegen::VariableDecl::eInitType::Assignment, ident, type_name,
                                     init_exprs.empty() ? "" : init_exprs[0].Codegen()}
            .Codegen();
      case eDeclType::Function:
        return codegen::FnDecl{ident, type_name, args, modifiers}.Codegen();
      case eDeclType::TypeAlias:
        return "using " + ident + " = " + type_name + ";\n";
      case eDeclType::Struct:
        return "struct " + ident + " {\n" + GenerateExprList(init_exprs) + "\n};\n";
      case eDeclType::Union:
        return "union " + ident + " {\n" + GenerateExprList(init_exprs) + "\n};\n";
      case eDeclType::Enum:
        return "enum " + ident + " {\n" + GenerateExprList(init_exprs) + "\n};\n";
      case eDeclType::Class:
        return "class " + ident + " {\n" + GenerateExprList(init_exprs) + "\n};\n";
      default:
        throw cxx::UnknownEnumEntry<eDeclType>();
    }
  }

  static constexpr Decl FromCndAst(const Ast& ast) {
    Decl decl;
    decl.ident = ast.GetLiteral();
    decl.type_name = ast.At(0).GetLiteral();
    if (ast.type == eAst::kVariableDecl) {
      decl.type = eDeclType::Variable;
      if (ast.children.size() > 1) {
        decl.init_exprs.push_back(codegen::Expr::FromCndAst(ast.At(1)));
      }
    } else if (ast.type == eAst::kFunctionDecl) {
      decl.type = eDeclType::Function;
      for (const auto& arg : std::ranges::subrange(ast.children.cbegin() + 1, ast.children.cend())) {
        decl.args.push_back(codegen::Expr::FromCndAst(arg));
      }
    } else if (ast.type == eAst::kTypeAlias) {
      decl.type = eDeclType::TypeAlias;
    } else if (ast.type == eAst::kStructDecl) {
      decl.type = eDeclType::Struct;
    } else if (ast.type == eAst::kUnionDecl) {
      decl.type = eDeclType::Union;
    } else if (ast.type == eAst::kEnumDecl) {
      decl.type = eDeclType::Enum;
    } else if (ast.type == eAst::kClassDecl) {
      decl.type = eDeclType::Class;
    }
    return decl;
  }

};


// Variable
struct VarDecl {
  enum class eInitType {
    Declaration,  ///> No initializer will be generated. Variable declaration.
    BracketInit,  ///> Init will be placed in brackets. eg. int foo(42);
    BraceInit,    ///> Init will be placed in braces. eg. int foo{42};
    Assignment    ///> Init will be assigned by '=' operator. eg. int foo = 42;
  };
  eInitType init_type{eInitType::Declaration};  ///@brief The type of initializer to use
                                                /// for this declaration.
  Str ident{""};                                /// @brief Name of the variable.
  Str type{""};                                 /// @brief Type of the variable.
  Str init{""};                                 /// @brief Primary expression to emplace in the
                                                ///        variable initializer. Based on init type.
                                                ///        Ignore if init_type is eInitType::Declaration.
                                                ///        This primary expression must not end with a
                                                ///        semicolon, codegen will insert a ';' if necessary.

  constexpr Str Codegen() const {
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

// Method
struct FnDecl{
  static constexpr Expr FromCndAst(const Ast& ast);
  constexpr Str Codegen() const;
};

// Class (and struct)
struct ClassDecl {
  static constexpr Expr FromCndAst(const Ast& ast);
  constexpr Str Codegen() const;
};

struct TrUnit {
  CodeModel* model_ptr;
  std::vector<Decl> decl_sequence;
  std::map<Str, FnDecl*> fns;
  std::map<Str, ClassDecl*> types;
  std::map<Str, VarDecl*> globals;

  Str Codegen() const {
    Str out{};
    for (const auto& decl : decl_sequence) {
      out += decl.Codegen();
    }
    return out;
  }
};

struct CodeModel {
  std::map<Str, TrUnit> unitmap;

  // Maps : Function or TypeIdentity pointed to should be stored in a tr unit's storage. (most of the time)
  std::map<Str, FnDecl*> fns;
  std::map<Str, ClassDecl*> types;
  std::map<Str, VarDecl*> globals;

  static constexpr CodeModel FromCndAst(const Ast& ast,StrView tr) {
    CodeModel model{};
    TrUnit& unit = model.unitmap[tr.data()];
    unit.model_ptr = &model;

    if (!(ast.TypeIs(eAst::kProcessDefinition) || ast.TypeIs(eAst::kLibraryDefinition))) 
      throw std::exception("Expected process or library definition at the top of the AST.");

    for (const auto& decl_ast : ast.children) {
      switch (decl_ast.type) {
        case eAst::kVariableDeclaration:
          unit.decl_sequence.push_back(VarDecl::FromCndAst(decl_ast));
          auto& this_decl = unit.decl_sequence.back();
          unit.globals[this_decl.ident] = &this_decl.As<VarDecl>();
          model.globals[this_decl.ident] = unit.globals[this_decl.ident];
          break;
        case eAst::kMethodDeclaration:
          unit.decl_sequence.push_back(FnDecl::FromCndAst(decl_ast));
          auto& this_decl = unit.decl_sequence.back();
          unit.fns[this_decl.ident] = &this_decl.As<FnDecl>();
          model.fns[this_decl.ident] = unit.fns[this_decl.ident];
          break;
        case eAst::kClassDeclaration:
          unit.decl_sequence.push_back(ClassDecl::FromCndAst(decl_ast));
          auto& this_decl = unit.decl_sequence.back();
          unit.types[this_decl.ident] = &this_decl.As<ClassDecl>();
          model.types[this_decl.ident] = unit.types[this_decl.ident];
          break;
        default:
          throw cxx::UnknownEnumEntry<eAst>();
      }
    }
    return model;
  }

  CompilerProcessResult<int> AppendAst(const Ast& ast) {
    if (unitmap.find(tr.data()) == unitmap.end()) {
      unitmap[tr.data()] = TrUnit{this};
    }
    TrUnit& unit = unitmap[tr.data()];
    unit.model_ptr = this;
    if (!(ast.TypeIs(eAst::kProcessDefinition) || ast.TypeIs(eAst::kLibraryDefinition))) 
      throw std::exception("Expected process or library definition at the top of the AST.");
    for (const auto& decl_ast : ast.children) {
      switch (decl_ast.type) {
        case eAst::kVariableDeclaration:
          unit.decl_sequence.push_back(VarDecl::FromCndAst(decl_ast));
          auto& this_decl = unit.decl_sequence.back();
          unit.globals[this_decl.ident] = &this_decl.As<VarDecl>();
          globals[this_decl.ident] = unit.globals[this_decl.ident];
          break;
        case eAst::kMethodDeclaration:
          unit.decl_sequence.push_back(FnDecl::FromCndAst(decl_ast));
          auto& this_decl = unit.decl_sequence.back();
          unit.fns[this_decl.ident] = &this_decl.As<FnDecl>();
          fns[this_decl.ident] = unit.fns[this_decl.ident];
          break;
        case eAst::kClassDeclaration:
          unit.decl_sequence.push_back(ClassDecl::FromCndAst(decl_ast));
          auto& this_decl = unit.decl_sequence.back();
          unit.types[this_decl.ident] = &this_decl.As<ClassDecl>();
          types[this_decl.ident] = unit.types[this_decl.ident];
          break;
        default:
          throw cxx::UnknownEnumEntry<eAst>();
      }
    }
  }

  /// Generates the code for all translation units in the model. 
  /// Each produced translation unit will be associated with a string key. (usually indicates the output file path)
  Vec<Pair<Str,Str>> Codegen() const {
    Vec<Pair<Str, Str>> out;
    for (const auto& unit : unitmap) {
      out.push_back(std::make_pair(unit.first, unit.second.Codegen()));
    }
    return out;
  }
};


}  // namespace codegen


//Str CodegenStmt(const Ast& ast) {
//  if (IsAstPrimary(ast.type)) {
//    return CodegenExpr(ast) + ";";
//  } else if (ast.type == eAst::kUnnamedScope) {
//    Str result;
//    for (const auto& child : ast.children) {
//      result += CodegenStmt(child) + ";\n";
//    }
//    return "{\n" + result + "}";
//  } else if (ast.type == eAst::kKwIf) {
//    return "if (" + CodegenExpr(ast.At(0)) + ") " + CodegenStmt(ast.At(1));
//  } else if (ast.type == eAst::kKwWhile) {
//    return "while (" + CodegenExpr(ast.At(0)) + ") " + CodegenStmt(ast.At(1));
//  } else if (ast.type == eAst::kKwFor) {
//    return "for (" + CodegenExpr(ast.At(0)) + "; " + CodegenExpr(ast.At(1)) + "; " + CodegenExpr(ast.At(2)) + ") " +
//           CodegenMethodBody(ast.At(3));
//  } else if (ast.type == eAst::kKwReturn) {
//    return "return " + CodegenExpr(ast.At(0));
//  }
//}
//
//Str CodegenIterativeStmt(const Ast& ast) {
//  Str out{"{"};
//  for (const Ast& stmt : std::ranges::subrange(ast.children.cbegin() + 1, ast.children.cend())) {
//    out += CodegenStmt(stmt) + "\n";
//  }
//  out += "}\n";
//  return out;
//}
//
//Str CodegenFunctionalBody(const Ast& ast) {
//  Str out{"{"};
//  for (const Ast& stmt : std::ranges::subrange(ast.children.cbegin() + 1, ast.children.cend())) {
//    out += CodegenStmt(stmt) + "\n";
//  }
//  out += "}\n";
//  return out;
//}
//
//Str CodegenMethodBody(const Ast& ast) {
//  Str out{"{"};
//  for (const Ast& stmt : std::ranges::subrange(ast.children.cbegin() + 1, ast.children.cend())) {
//    out += CodegenStmt(stmt) + "\n";
//  }
//  out += "}\n";
//  return out;
//}
//
///// @brief Generates a variable declaration eg. int foo = 42;
//struct VariableDecl {
//  enum class eInitType {
//    Declaration,  ///> No initializer will be generated. Variable declaration.
//    BracketInit,  ///> Init will be placed in brackets. eg. int foo(42);
//    BraceInit,    ///> Init will be placed in braces. eg. int foo{42};
//    Assignment    ///> Init will be assigned by '=' operator. eg. int foo = 42;
//  };
//  eInitType init_type{eInitType::Declaration};  ///@brief The type of initializer to use
//                                                /// for this declaration.
//  Str ident{""};                                /// @brief Name of the variable.
//  Str type{""};                                 /// @brief Type of the variable.
//  Str init{""};                                 /// @brief Primary expression to emplace in the
//                                                ///        variable initializer. Based on init type.
//                                                ///        Ignore if init_type is eInitType::Declaration.
//                                                ///        This primary expression must not end with a
//                                                ///        semicolon, codegen will insert a ';' if necessary.
//
//  constexpr Str Codegen() const {
//    switch (init_type) {
//      case eInitType::Declaration:
//        return type + " " + ident + ";\n";
//      case eInitType::BracketInit:
//        return type + " " + ident + "(" + init + ");\n";
//      case eInitType::BraceInit:
//        return type + " " + ident + "{" + init + "};\n";
//      case eInitType::Assignment:
//        return type + " " + ident + " = " + init + ";\n";
//      default:
//        throw cxx::UnknownEnumEntry<eInitType>();
//    }
//  }
//};


}  // namespace clang
using CLangCodeModel = clang::codegen::CodeModel;

}  // namespace cnd

/// @} // end of cnd_compiler

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: C& Programming Language
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @notes:
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2025 Anton Yashchenko
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
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
