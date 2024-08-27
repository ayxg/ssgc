#ifndef HEADER_GUARD_CALE_CAOCO_TRANSPILER_H
#define HEADER_GUARD_CALE_CAOCO_TRANSPILER_H
///////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
///////////////////////////////////////////////////////////////////////////////
// Author(s): Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////
/// @file C& Official Compiler (CAOCO) transpiler
/// @brief Handles conversion of IRAST Format Program to C++ source code.
///////////////////////////////////////////////////////////////////////////////
#include "caoco_ceval.h"

namespace caoco {

class CppSourceCode {
 public:
  CppSourceCode() = default;
  CppSourceCode(const CppSourceCode&) = delete;
  CppSourceCode& operator=(const CppSourceCode&) = delete;
  CppSourceCode(CppSourceCode&&) = delete;
  CppSourceCode& operator=(CppSourceCode&&) = delete;
  ~CppSourceCode() = default;

  void Add(const std::string& code) { code_ += code; }
  void AddLine(const std::string& code) { code_ += code + '\n'; }
  void AddLine() { code_ += '\n'; }
  void AddLine(const std::string& code, int indent) {
    code_ += std::string(indent, ' ') + code + '\n';
  }
  void AddLine(int indent) { code_ += std::string(indent, ' ') + '\n'; }
  void AddLine(const std::string& code, const std::string& comment) {
    code_ += code + " // " + comment + '\n';
  }

 private:
  string code_;
};

class Transpiler {
  const irast::Program& program_;
  CppSourceCode& out_;

  int Transpile(const irast::Program& program, CppSourceCode& out);
  string TanspilePragmaticVarDecl(const irast::VarDecl& var_decl) { 
    string ret;

    // When no value category mods are applied to the definition
    // it is by default a mutable stack value.
    if (var_decl.IsMutableValue) {
      // C++ Native Definition
      if (var_decl.IsNative()) {
        // Handle Invariants as normal C++ definitions
        if (var_decl.IsInvariant()) {
          if (var_decl.IsStatic()) 
            ret += "static ";
          if (var_decl.IsConst()) 
            ret += "const ";
          ret += var_decl.constraints.First().Literal() + " ";
          ret += var_decl.name.name + '{';
          if (var_decl.HasInitializer()) // transpile initializer
            ret += TranspilePrimaryExpr(var_decl.initializer) + '};';
          else // or call the default constructor of the invariant type.
            ret += var_decl.constraints.First().Literal() + '()};';
          return ret;
        } else {
          // Else its a multi-typed constraint. -> CandUnion<T...>
          if (var_decl.IsStatic()) ret += "static ";
          if (var_decl.IsConst()) ret += "const ";
          if (var_decl.IsConstrained())
            ret += "__cand__::CandUnion<"+ var_decl.constraints.LiteralList() + "> ";
          else {
            throw "Native Invariants must be constrained to one or more types.";
          }
          ret += var_decl.name.name + '{';
          if (var_decl.HasInitializer())  // transpile initializer
            ret += TranspilePrimaryExpr(var_decl.initializer) + '};';
          else  // or call the default constructor of the first index variant type.
                // todo: check if the class implements default - else try next type.
            ret += var_decl.constraints.Invariant().Literal() + '()};';
          return ret;
        }
      } 
      // C& stack value definition.
      else {
        // [mods] __cand__::CandValue<T...> name = [initializer];
      }
    } else if (var_decl.IsMutableReference) {
      // ref def @name : initializer;
      // Mutable references:
      // - Must explicitly be provided an initializer.
      // - Must be of equal or broader but matching constraints. 
      //   eg. 
      //      def [int.float,str] @a; 
      //   Can be referenced by:
      //      ref def [int,float,str] @c:a;
      //      ref def [int,float,str,char] @b:a;
      //      ref def @e:a;
      //      ref def any @g:a;
      //      ref def [str,float,int] @i;
      //   The following definitions will fail:
      //      ref def [int] @h:a; ` [E000122] Constraint Failure. 
      //                            Pragmatic mutable variable reference definition 'h' has narrower constraints than the variable it refers to.
      //                            Must be of equal or broader but matching constraints. 
      // 
      //      ref def [int,float] @j:a; ` [E000122]
      //      ref def [uint,float,str] @k:a; ` [E000122]
      //      ref def @e:a; ` [E000123] No initializer provided for pragmatic mutable variable reference definition. Initializer must be explicitly provided. 

    } else if (var_decl.IsImmutableReference) {
    } else if (var_decl.IsOwnedValue) {
    }

    if (var_decl.IsRef()) {
      
    }


    // Handle cpp def
    if (var_decl.IsNative()) {
      // Handle Invariants as normal C++ definitions
      if (var_decl.IsInvariant()) { 
        out_.AddLine(
          var_decl.constraints.At(0) + ' ' + var_decl.name
      }

    } else {
    
    
    }
  }
};

namespace transpiler {

// Pragmatic Statements.
string Transpile(const irast::VarDecl& var_decl) {
  string out;
  if (var_decl.is_invariant()) {
    out +=
        "static " + Transpile(var_decl.constraints) + " " + var_decl.name.name;
    if (var_decl.is_initialized()) {
      out += " = " + Transpile(var_decl.initializer) + ";\n";
    } else {
      out += ";\n";
    }
  }
};
int TranspilePragmaticStmt(const irast::FnDecl& fn_decl, std::ostream& out);
int TranspilePragmaticStmt(const irast::ClassDecl& class_decl,
                           std::ostream& out);
int TranspilePragmaticStmt(const irast::UsingDecl& using_decl,
                           std::ostream& out);
int TranspilePragmaticStmt(const irast::NamespaceDecl& ns_decl,
                           std::ostream& out);

// Primary Expressions.
int TranspileFunctionalStmt(const irast::PrimaryExpr& primary_expr,
                            std::ostream& out);
int TranspileFunctionalStmt(const irast::VarDecl& var_decl, std::ostream& out);
int TranspileFunctionalStmt(const irast::FnDecl& fn_decl, std::ostream& out);
int TranspileFunctionalStmt(const irast::ClassDecl& class_decl,
                            std::ostream& out);
int TranspileFunctionalStmt(const irast::UsingDecl& using_decl,
                            std::ostream& out);
int TranspileFunctionalStmt(const irast::NamespaceDecl& ns_decl,
                            std::ostream& out);

}  // namespace transpiler

int Transpile(const irast::Program& program, std::ostream& out) {
  EnterProgramDefinition(program, out);
  IncludeCandStandardLibraries(program, out);
  IncludePredefinedCppLibraries(program, out);
  IncludeUserDefinedCppFiles(program, out);
  EnterGlobalNamespace(program, out);
  for (const irast::Statement& stmt : program.statements) {
    std::visit([&](auto&& arg) { TranspilePragmaticStmt(arg, out); }, stmt);
  }
  ExitGlobalNamespace(program, out);
  ExitProgramDefinition(program, out);
}

}  // namespace caoco

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
#endif  // HEADER_GUARD_CALE_CAOCO_TRANSPILER_H