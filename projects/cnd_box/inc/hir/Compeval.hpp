#pragma once
// First pass constant evaluation. Acts like a 'calculator'.
// Performs reductions on literal and constant expressions in the AST.
#include "ccapi/CommonCppApi.hpp"
#include "compiler/TranslationInput.hpp"
#include "compiler/TranslationOutput.hpp"
#include "compiler_utils/CompilerProcessResult.hpp"
#include "frontend/Ast.hpp"
#include "frontend/Lexer.hpp"
#include "frontend/Parser.hpp"
#include "hir/AnyValue.hpp"
#include "hir/HirOp.hpp"

namespace cnd {

namespace hir {

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Intermediate Representation Objects
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum class eValCat : unsigned char { Value, Mut, Imut, Ref, Cref, IRef, Owned, Shared, View, __count__ };

// Translation Units
struct TrUnit;

// Declarations
struct Namespace;
struct Variable;
struct PrimaryExpr;
struct FunctionParameter;
struct FunctionDefinition;

struct Namespace {
  Namespace* parent{nullptr};
  StrView ident{};
  std::unordered_map<StrView, Namespace> subspaces;
  std::unordered_map<StrView, AV> vars;
  std::unordered_map<StrView, FunctionDefinition> funcs;
  ClRes<std::reference_wrapper<AV>> ResolveVariable(StrView ident);
  ClRes<std::reference_wrapper<const FunctionDefinition>> ResolveFunction(StrView ident);
  bool ContainsLocalVariable(StrView ident) const { return vars.contains(ident); }
};

struct FunctionArgument {
  AnyValue* data;
  eValCat valcat;
};

struct FunctionParameter {
  StrView name;
  eTypeIndex type;
  eValCat valcat;
};

struct FunctionDefinition {
  StrView name;
  std::vector<FunctionParameter> params;
  std::map<StrView, std::size_t> lookup_params;
  eTypeIndex return_type;
  std::vector<HirOp> impl;
};

struct FunctionCall {
  const FunctionDefinition& definition;
  std::vector<FunctionArgument> args;
};

struct TrUnit {
  const TrInput& input_;
  TrOutput& output_;

  static constexpr inline StrView kGlobalNamespaceName = "__global__";
  bool is_terminated{false};  // Has the compile time evaluation been terminated
  int exit_code{};            // Exit code returned by the compile time evaluation.
  std::unordered_map<Str, Vec<char>> sources{};
  std::unordered_map<StrView, Vec<Tk>> tokens{};
  std::unordered_map<StrView, Vec<Tk>> sanitized_tokens{};
  std::unordered_map<StrView, Span<const Tk>> span_tokens{};
  std::unordered_map<StrView, Ast> trees{};
  Namespace global{.parent = nullptr, .ident = kGlobalNamespaceName};

  ClRes<std::unordered_map<StrView, Ast>::iterator> ParseSourceFile(StrView fp) noexcept;
  ClRes<std::unordered_map<Str, Vec<char>>::iterator> ReadSourceFile(StrView fp) noexcept;

  ClRes<void> Evaluate();
  ClRes<bool> EvalSourceFile(StrView fp) noexcept;
  ClRes<void> EvalPragmaticReturnStmt(const Ast& ast, Namespace& ns) noexcept;
  ClRes<void> EvalPragmaticVariableDefinition(const Ast& ast, Namespace& ns) noexcept;
  ClRes<void> EvalPragmaticFunctionDefinition(const Ast& ast, Namespace& ns) noexcept;
  ClRes<AV> EvalPrimaryExpr(const Ast& ast, Namespace& ns) noexcept;
  ClRes<AV> EvaluateFunctionCall(const FunctionCall& call, Namespace& caller_ns) noexcept;
  ClRes<AV> ComputeBinop(const Ast& lhs, const Ast& rhs, Namespace& ns, AV (*binop)(const AV&, const AV&));
};

// Loads source file at given path and stores in 'sources' at file path key. Currently only used  internally by
// 'LoadSourceFile' method.
ClRes<std::unordered_map<Str, Vec<char>>::iterator> TrUnit::ReadSourceFile(StrView fp) noexcept {
  if (!stdfs::exists(fp)) return ClFail(MakeClMsg<eClErr::kFailedToReadFile>(fp, "Does not exist"));

  if (!stdfs::is_regular_file(fp)) return ClFail(MakeClMsg<eClErr::kFailedToReadFile>(fp, "Not a regular file."));

  std::ifstream source_file_stream(fp.data());
  if (!source_file_stream.is_open()) return ClFail(MakeClMsg<eClErr::kFailedToReadFile>(fp, "Could not open file."));

  std::vector<char> temp_file_buffer =
      std::vector<char>((std::istreambuf_iterator<char>(source_file_stream)), std::istreambuf_iterator<char>());
  source_file_stream.close();

  // Add \0 if not already at end.
  if (temp_file_buffer.back() != '\0') temp_file_buffer.push_back('\0');
  
  auto new_key = std::string{fp.begin(), fp.end()};
  sources[new_key] = temp_file_buffer;

  return sources.find(new_key);
}

// Loads, lexes, sanitizes and parses a C& source file. Stores result of operations into associated maps at file path
// key. Assert a file has not been already loaded for this compiler instance before calling this method on a given
// path.
ClRes<std::unordered_map<StrView, Ast>::iterator> TrUnit::ParseSourceFile(StrView fp) noexcept {
  // Load file data.
  auto src_read = ReadSourceFile(fp);
  if (!src_read) return ClFail(src_read.error());
  StrView src_key = src_read.value()->first;
  const auto & src_data = src_read.value()->second;

  // Lex and store tokens.
  StrView src_view = {src_data.cbegin(), src_data.cend()};
  auto lex_res = trtools::Lexer::Lex(src_view);
  if (!lex_res) return ClFail(lex_res.error());
  tokens[src_key] = lex_res.value();

  // Sanitize and store sanitized tokens.
  sanitized_tokens[src_key] = trtools::Lexer::Sanitize(tokens[src_key]);
  span_tokens[src_key] = Span{sanitized_tokens[src_key].data(), sanitized_tokens[src_key].size()};

  // Parse and store abstract syntax tree.
  auto parse_res = trtools::parser::ParseSyntax({span_tokens[src_key].cbegin(), span_tokens[src_key].cend()});
  if (!parse_res) return ClFail(parse_res.error());
  trees[src_key] = parse_res.Extract().ast;

  return trees.find(src_key);
}

ClRes<void> TrUnit::Evaluate() {
  // Process all input source files in order.
  for (auto src_file_it = input_.src_files.cbegin(); src_file_it != input_.src_files.cend(); src_file_it++) {
    
    auto parse_res = ParseSourceFile(src_file_it->string());
    if (!parse_res) return ClFail(parse_res.error());

    auto eval_res = EvalSourceFile(parse_res.value()->first);
    if (!eval_res) return ClFail(eval_res.error());
    if (*eval_res) return ClRes<void>{};  // Check if evaluation was terminated early by the source.
  }
  return ClRes<void>{};
}

// Evaluates a source file as a fragment of the translation unit. Returns true if further source files should be
// evaluated.
ClRes<bool> TrUnit::EvalSourceFile(StrView src_key) noexcept {
  const Ast& ast = trees[src_key];

  if (ast.TypeIsnt(eAst::kProgram)) {
    return ClFail(
        MakeClMsg<eClErr::kCompilerDevDebugError>(std::source_location::current(), "Root ast must be a program."));
  }

  for (const auto& stmt : ast.children) {
    if (stmt.TypeIs(eAst::kKwReturn)) {
      auto eval_res = EvalPragmaticReturnStmt(stmt, global);
      if (!eval_res) return ClFail(eval_res.Error());
      return false;  // Stop eval, translation was terminated by pragmatic return.
    } else if (stmt.TypeIs(eAst::kVariableDeclaration)) {
      auto eval_res = EvalPragmaticVariableDefinition(stmt, global);
      if (!eval_res) return ClFail(eval_res.Error());
    }
  }

  return true;
}

ClRes<void> TrUnit::EvalPragmaticReturnStmt(const Ast& ast, Namespace& ns) noexcept {
  assert(ast.TypeIs(eAst::kKwReturn) && __FUNCTION__ ": Expected eAst::kKwReturn ast type.");
  auto eval_res = EvalPrimaryExpr(ast.At(0), ns);
  if (!eval_res) return ClFail(eval_res.Error());

  if (!AV::Is<I32>(*eval_res))
    return ClFail(MakeClMsg<eClErr::kCompilerDevDebugError>(
        std::source_location::current(), "Return type of a pragmatic return statement must be an int."));
  exit_code = 0;
  output_.return_value = AV::CppRef<I32>(*eval_res).data;
};

ClRes<void> TrUnit::EvalPragmaticVariableDefinition(const Ast& ast, Namespace& ns) noexcept {
  assert(ast.TypeIs(eAst::kVariableDeclaration) && __FUNCTION__ ": Expected eAst::kVariableDeclaration ast type.");
  // TODO: mods

  // Get the variable identifier. Assert it's unique in the namespace.
  StrView ident = ast.At(ast.children.size() - 2).RawLiteral();
  if (ns.ContainsLocalVariable(ident))
    return ClFail(MakeClMsg<eClErr::kCompilerDevDebugError>(
        std::source_location::current(),
        std::format("Variable '{}' already exists in namespace '{}'.", ident, ns.ident)));

  // Evaluate the initalizer.
  auto initializer_res = EvalPrimaryExpr(ast.At(ast.children.size() - 1).At(0), ns);
  if (!initializer_res) return ClFail(initializer_res.Error());

  // Add to var map in this namespace.
  ns.vars[ident] = *initializer_res;

  return ClRes<void>{};
}

template <class T>
ClRes<AV> EvalLiteral(const Ast& ast) {
  auto ev_res = T::FromLiteral(ast.src_begin->Literal());
  if (ev_res)
    return AV::Make<T>(*ev_res);
  else
    return ClFail(ev_res.error());
}

ClRes<AV> TrUnit::ComputeBinop(const Ast& lhs_ast, const Ast& rhs_ast, Namespace& ns,
                               AV (*binop)(const AV&, const AV&)) {
  auto lhs = EvalPrimaryExpr(lhs_ast, ns);
  auto rhs = EvalPrimaryExpr(rhs_ast, ns);
  if (!lhs) return ClFail(lhs.error());
  if (!rhs) return ClFail(rhs.error());
  auto opres = binop(*lhs, *rhs);
  if (AV::Is<Error>(opres))
    return ClFail(
        MakeClMsg<eClErr::kCompilerDevDebugError>(std::source_location::current(), AV::CppRef<Error>(opres).data));
  return opres;
}

ClRes<AV> TrUnit::EvalPrimaryExpr(const Ast& ast, Namespace& ns) noexcept {
  switch (ast.type) {
    // Literals
    case eAst::kLitBool:
      return EvalLiteral<Bool>(ast);
    case eAst::kLitChar:
      return EvalLiteral<I8>(ast);
    case eAst::kLitInt:
      return EvalLiteral<I32>(ast);
    case eAst::kLitCstr:
      return EvalLiteral<CStr>(ast);

    // Arithmetic Binary Operations
    case eAst::kAdd:
      return ComputeBinop(ast.At(0), ast.At(1), ns, AV::Add);
    case eAst::kSub:
      return ComputeBinop(ast.At(0), ast.At(1), ns, AV::Sub);
    case eAst::kMul:
      return ComputeBinop(ast.At(0), ast.At(1), ns, AV::Mul);
    case eAst::kDiv:
      return ComputeBinop(ast.At(0), ast.At(1), ns, AV::Div);
    case eAst::kMod:
      return ComputeBinop(ast.At(0), ast.At(1), ns, AV::Mod);
    case eAst::kAnd:
      return ComputeBinop(ast.At(0), ast.At(1), ns, AV::And);
    case eAst::kOr:
      return ComputeBinop(ast.At(0), ast.At(1), ns, AV::Or);
    case eAst::kXor:
      return ComputeBinop(ast.At(0), ast.At(1), ns, AV::Xor);
    case eAst::kLsh:
      return ComputeBinop(ast.At(0), ast.At(1), ns, AV::Lsh);
    case eAst::kRsh:
      return ComputeBinop(ast.At(0), ast.At(1), ns, AV::Rsh);
    case eAst::kEq:
      return ComputeBinop(ast.At(0), ast.At(1), ns, AV::Eq);
    case eAst::kNeq:
      return ComputeBinop(ast.At(0), ast.At(1), ns, AV::Neq);
    case eAst::kLt:
      return ComputeBinop(ast.At(0), ast.At(1), ns, AV::Lt);
    case eAst::kGt:
      return ComputeBinop(ast.At(0), ast.At(1), ns, AV::Gt);
    case eAst::kLte:
      return ComputeBinop(ast.At(0), ast.At(1), ns, AV::Lte);
    case eAst::kGte:
      return ComputeBinop(ast.At(0), ast.At(1), ns, AV::Gte);

    //// Arithmeric Unary Operators
    // case eAst::kNot:
    //   return ComputeBinop(ast.At(0), ast.At(1), ns, AV::Not);

    // Inline Binary Operations (Assignments)
    case eAst::kAssign:
    case eAst::kNewAssign:
    case eAst::kAddAssign:
    case eAst::kSubAssign:
    case eAst::kMulAssign:
    case eAst::kDivAssign:
    case eAst::kModAssign:
    case eAst::kAndAssign:
    case eAst::kOrAssign:
    case eAst::kXorAssign:
    case eAst::kLshAssign:
    case eAst::kRshAssign:
      return DEBUG_FAIL("Inline binary operations not implemented.");
    // Other
    case eAst::kIdent: {
      auto resolution_res = ns.ResolveVariable(ast.src_begin->Literal());
      if (!resolution_res) return ClFail(resolution_res.error());
      return AV::WeakRef(resolution_res->get());
    }
    case eAst::kSubexpression:
      return EvalPrimaryExpr(ast.At(0), ns);
  }

  return DEBUG_FAIL("Cannot evaluate primary expression.");
}

ClRes<std::reference_wrapper<AV>> Namespace::ResolveVariable(StrView ident) {
  if (vars.contains(ident))
    return vars[ident];
  else if (parent != nullptr)
    return parent->ResolveVariable(ident);
  else
    return DEBUG_FAIL("Cannot resolve variable value.");
}

ClRes<std::reference_wrapper<const FunctionDefinition>> Namespace::ResolveFunction(StrView ident) {
  if (funcs.contains(ident))
    return funcs[ident];
  else if (parent != nullptr)
    return parent->ResolveFunction(ident);
  else
    return DEBUG_FAIL(std::format("Cannot resolve function {}.", ident));
}

ClRes<AV> EvaluateFunctionCall(StrView name, std::vector<FunctionArgument> args, Namespace& ns) noexcept {
  auto resolution_res = ns.ResolveFunction(name);
  if (!resolution_res) return ClFail(resolution_res.Error());
  const FunctionDefinition& def = resolution_res->get();
  if (def.params.size() != args.size())
    return DEBUG_FAIL(std::format("Function '{}' expects {} arguments but {} were provided.", def.name,
                                  def.params.size(), args.size()));


}
// using cxx::Expected;
//
// enum eValCategory { Native, Owned, Borrowed, Reference, View, ConstReference };
// enum eDeclVisibility { Public, Private };

//// Operands
// struct LitInt;
// struct LitUint;
// struct LitReal;
// struct LitBool;
// struct LitChar;
// struct LitByte;
// struct LitCstr;
// struct Ident;
//
// struct ValList;
// struct TypeList;
//
//// Operands which are actually builtin values.
// struct LitNone;     // Keyword 'none' is of type none_t
// struct LitNullref;  // Keyword 'nullref'
//
//// Types
// struct AnyType;
// struct AnyRefType;
// struct IntType;
// struct UintType;
// struct RealType;
// struct BoolType;
// struct CharType;
// struct ByteType;
// struct CstrType;
// struct StrType;
// struct PtrType;
// struct ArrayType;
// struct DictType;
// struct TupType;
// struct VoidType;
// struct NoneType;
// struct UndefinedType;
// struct MetaAutoType;
// struct MetaTypeType;
// struct MetaValueType;
//
//// operators.
//// Arithmetic Operators
// struct OpAdd;
// struct OpSub;
// struct OpMul;
// struct OpDiv;
// struct OpMod;
// struct OpAnd;
// struct OpOr;
// struct OpXor;
// struct OpNot;
// struct OpLsh;
// struct OpRsh;
// struct OpEq;
// struct OpNeq;
// struct OpLt;
// struct OpGt;
// struct OpLte;
// struct OpGte;
// struct OpSpaceship;  // <=>
// struct OpAssign;
// struct OpNewAssign;
// struct OpAddAssign;
// struct OpSubAssign;
// struct OpMulAssign;
// struct OpDivAssign;
// struct OpModAssign;
// struct OpAndAssign;
// struct OpOrAssign;
// struct OpXorAssign;
// struct OpLshAssign;
// struct OpRshAssign;
// struct OpInc;
// struct OpDec;
// struct OpDot;
// struct OpBnot;
// struct OpBand;
// struct OpBor;
//
//// Functional Operators
// struct OpResolve;  // :: operator.
//                    // Used to access static members of a class or namespace.
// struct OpAccess;   // . operator.
//                    // Used to access members of a class or namespace.
// struct OpRange;    // ... range operator.
// struct OpAt;       // @ operator.
// struct OpCall;     // (...) function call
// struct OpIndex;    // [] operator.
// struct OpListing;  // {...} listing operator.
//
//// Language Operators
// struct LangFnAs;
// struct LangFnIn;
// struct LangFnAs;
// struct LangFnCin;
// struct LangFnCout;
//
//// Value Modifiers
// struct ModConst;
// struct ModStatic;
// struct ModNative;
// struct ModOwned;
// struct ModBorrowed;
// struct ModView;
// struct ModRef;
//
//// Visibility Modifiers
// struct ModPrivate;
// struct ModPublic;
//
//// abstract views
// struct PragmaticStmt;
// struct FunctionalStmt;
// struct ConditionalStmt;
// struct FnArgs;
// struct IndexingArgs;
// struct ListingArgs;
// struct PrimaryExpr;
// struct ValMods;
// struct Constraints;
// struct ReturnStmt;
// struct ContinueStmt;
// struct BreakStmt;
// struct SwitchStmt;
// struct IfStmt;
// struct WhileStmt;
// struct ForStmt;
// struct UsingDecl;
// struct NamespaceDecl;
// struct VarDecl;
// struct FnDecl;
// struct ClassDecl;
// struct ImportDecl;
// struct LibDecl;
// struct MainDecl;
// struct FnParam;
// struct FnSignature;  // FnParams + Constraints + Mods
// struct ValueList;
// struct TypeList;
// struct Program;

// using PragmaticStmtVariant =
//     std::variant<UsingDecl, NamespaceDecl, VarDecl, FnDecl, ClassDecl, ImportDecl, LibDecl, MainDecl>;
// struct PragmaticStmt : public PragmaticStmtVariant {
//   using PragmaticStmtVariant::variant;
// };
//
// using FunctionalStmtVariant =
//     std::variant<UsingDecl, VarDecl, ReturnStmt, SwitchStmt, IfStmt, WhileStmt, ForStmt, UsingDecl>;
// struct FunctionalStmt : public FunctionalStmtVariant {
//   using FunctionalStmtVariant::variant;
// };
//
// using ConditionalStmtVariant = std::variant<UsingDecl, VarDecl, ReturnStmt, SwitchStmt, IfStmt, WhileStmt, ForStmt,
//                                             UsingDecl, BreakStmt, ContinueStmt>;
//
// struct ConditionalStmt : public ConditionalStmtVariant {
//   using ConditionalStmtVariant::variant;
// };
//
// using PrimaryExprVariant = std::variant<
//   // Operands
//   LitInt, LitUint, LitReal, LitBool, LitChar, LitByte, LitCstr, Ident,
//   //// Types
//   //AnyType, AnyRefType, IntType, UintType, RealType, BoolType, CharType,
//   //ByteType, CstrType, StrType, PtrType, ArrayType, DictType, TupType,
//   //VoidType, NoneType, UndefinedType, MetaAutoType, MetaTypeType, MetaValueType,
//   //// Operators
//   //OpAdd, OpSub, OpMul, OpDiv, OpMod, OpAnd, OpOr, OpXor, OpNot, OpLsh, OpRsh,
//   //OpEq, OpNeq, OpLt, OpGt, OpLte, OpGte, OpSpaceship, OpAssign, OpNewAssign,
//   //OpAddAssign, OpSubAssign, OpMulAssign, OpDivAssign, OpModAssign, OpAndAssign,
//   //OpOrAssign, OpXorAssign, OpLshAssign, OpRshAssign, OpInc, OpDec, OpDot,
//   //OpBnot, OpBand, OpBor, OpResolve, OpAccess, OpRange, OpAt, OpCall, OpIndex,
//   //OpListing, LangFnIn, LangFnAs, LangFnCin, LangFnCout, ModConst, ModStatic,
//   //ModNative, ModOwned, ModBorrowed, ModView, ModRef, ModPrivate, ModPublic,
//>;
//
// struct PrimaryExpr : public PrimaryExprVariant {
//   using PrimaryExprVariant::variant;
//   // True if this expression evaluates to a literal type at compile time
//   // without external dependencies.
//   // eg. one of LitInt, LitBool , LitReal, LitCstr, etc...
//   bool IsLiteralConsteval();
//
//   // True if this expression evaluates to a literal type at compile time
//   // with external dependencies, which must also be evauable at compile time.
//   // eg. may contain identifiers ,methods, or non-literal classes.
//   //     which must perform a fully qualified name lookup.
//   bool IsConsteval();
//
//   // Returns this primary expr as a Typelist given the variant is
//   // reduced to a typelist or is directly convertible to a typelist.
//   Expected<TypeList> GetAsTypelist() {}
//
//   // First-pass: constant literal reduction of the primary expr ast.
//   // Combines literal types and operators to reduce noise.
//   Expected<PrimaryExpr> ReduceLiteral() {}
//
//   // Second-pass: constant literal reduction with name lookup to consteval
//   //              dependencies.
//   //              Inlines literal values which may be evaluated at compile
//   //              time given all dependencies are also compile time evaluable.
//   Expected<PrimaryExpr> ReduceConsteval() {}
//
//   // Third-Pass: constexpr evaluation reduction within non-consteval context.
//   //             Inlines constevaluable values within non consteval contexts
//   //             which are ignored on the second-pass.
//   Expected<PrimaryExpr> ReduceConstexpr() {}
//
//   static Expected<PrimaryExpr> FromAst(const Ast& ast, Program& program);
// };
//
//// Base classes for similar node types.
// struct BinaryExprBase {
//   PrimaryExpr lhs;
//   PrimaryExpr rhs;
// };
//
// struct UnaryExprBase {
//   PrimaryExpr operand;
// };
//
// struct CallExprBase {
//   PrimaryExpr callee;
//   FnArgs args;
// };
//
// struct LitInt {
//   CtInt val;
//   static LitInt Create(CtInt&& v);
//   static Expected<LitInt> FromAst(const Ast& ast);
// };
// struct LitUint {
//   CtUint val;
//   static LitUint Create(CtUint&& v);
//   static Expected<LitUint> FromAst(const Ast& ast);
// };
// struct LitReal {
//   CtReal val;
//   static LitReal Create(CtReal&& v);
//   static Expected<LitReal> FromAst(const Ast& ast);
// };
// struct LitBool {
//   CtBool val;
//   static LitBool Create(CtBool&& v);
//   static Expected<LitBool> FromAst(const Ast& ast);
// };
// struct LitChar {
//   CtChar val;
//   static LitChar Create(CtChar&& v);
//   static Expected<LitChar> FromAst(const Ast& ast);
// };
// struct LitByte {
//   CtByte val;
//   static LitByte Create(CtByte&& v);
//   static Expected<LitByte> FromAst(const Ast& ast);
// };
// struct LitCstr {
//   CtCstr val;
//   static LitCstr Create(CtCstr&& v);
//   static Expected<LitCstr> FromAst(const Ast& ast);
// };
// struct Ident {
//   CtCstr name;
//   static Ident Create(CtCstr&& v);
//   static Expected<Ident> FromAst(const Ast& ast);
// };
//
// struct ValList {
//   Vec<PrimaryExpr> values;
//   static ValList Create(Vec<PrimaryExpr>&& v);
//   static ValList FromAst(const Ast& ast, Program& program);
// };
//
// struct TypeList {
//   Vec<PrimaryExpr> types;
//   bool IsAny();
//   static TypeList Create(Vec<PrimaryExpr>&& v);
//   static TypeList FromAst(const Ast& ast, Program& program);
// };
//
//// Operands which are actually builtin values.
// struct LitNone {};
// struct LitNullref {};
//
// struct AnyType {};
// struct AnyRefType {};
// struct IntType {};
// struct UintType {};
// struct RealType {};
// struct BoolType {};
// struct CharType {};
// struct ByteType {};
// struct CstrType {};
// struct StrType {};
// struct PtrType {};
// struct ArrayType {};
// struct DictType {};
// struct TupType {};
// struct VoidType {};
// struct NoneType {};
// struct UndefinedType {};
// struct MetaAutoType {};
// struct MetaTypeType {};
// struct MetaValueType {};
//
//// Arithmetic Operators
// struct OpAdd : public BinaryExprBase {
//   static OpAdd Create(Vec<PrimaryExpr>&& v);
//   static Expected<PrimaryExpr> FromAst(const Ast& ast, Program& program);
// };
// struct OpSub : public BinaryExprBase {
//   static TypeList Create(Vec<PrimaryExpr>&& v);
//   static TypeList FromAst(const Ast& ast, Program& program);
// };
// struct OpMul : public BinaryExprBase {
//   static TypeList Create(Vec<PrimaryExpr>&& v);
//   static TypeList FromAst(const Ast& ast, Program& program);
// };
// struct OpDiv : public BinaryExprBase {
//   static TypeList Create(Vec<PrimaryExpr>&& v);
//   static TypeList FromAst(const Ast& ast, Program& program);
// };
// struct OpMod : public BinaryExprBase {};
// struct OpAnd : public BinaryExprBase {};
// struct OpOr : public BinaryExprBase {};
// struct OpXor : public BinaryExprBase {};
// struct OpNot : public BinaryExprBase {};
// struct OpLsh : public BinaryExprBase {};
// struct OpRsh : public BinaryExprBase {};
// struct OpEq : public BinaryExprBase {};
// struct OpNeq : public BinaryExprBase {};
// struct OpLt : public BinaryExprBase {};
// struct OpGt : public BinaryExprBase {};
// struct OpLte : public BinaryExprBase {};
// struct OpGte : public BinaryExprBase {};
// struct OpSpaceship : public BinaryExprBase {};
// struct OpAssign : public BinaryExprBase {};
// struct OpNewAssign : public BinaryExprBase {};
// struct OpAddAssign : public BinaryExprBase {};
// struct OpSubAssign : public BinaryExprBase {};
// struct OpMulAssign : public BinaryExprBase {};
// struct OpDivAssign : public BinaryExprBase {};
// struct OpModAssign : public BinaryExprBase {};
// struct OpAndAssign : public BinaryExprBase {};
// struct OpOrAssign : public BinaryExprBase {};
// struct OpXorAssign : public BinaryExprBase {};
// struct OpLshAssign : public BinaryExprBase {};
// struct OpRshAssign : public BinaryExprBase {};
// struct OpBnot : public BinaryExprBase {};
// struct OpBand : public BinaryExprBase {};
// struct OpBor : public BinaryExprBase {};
//
// struct OpInc : public UnaryExprBase {};
// struct OpDec : public UnaryExprBase {};
//
// struct OpAccess : public BinaryExprBase {};
// struct OpResolve : public BinaryExprBase {};  // :: operator.
// struct OpRange : public BinaryExprBase {};    // ... range operator.
// struct OpAt : public BinaryExprBase {};       // @ operator.
//
// struct OpCall : public CallExprBase {};     // (...) function call
// struct OpIndex : public CallExprBase {};    // [] operator.
// struct OpListing : public CallExprBase {};  // {...} listing operator.
//
//// Language Operators
// struct LangFnIn : public CallExprBase {};
// struct LangFnAs : public CallExprBase {};
// struct LangFnCin : public CallExprBase {};
// struct LangFnCout : public CallExprBase {};
//
//// Value Modifiers
// struct ModConst {};
// struct ModStatic {};
// struct ModNative {};
// struct ModOwned {};
// struct ModBorrowed {};
// struct ModView {};
// struct ModRef {};
//
//// Visibility Modifiers
// struct ModPrivate {};
// struct ModPublic {};
//
//// abstract views
// struct FnArgs {};
// struct IndexingArgs {};
// struct ListingArgs {};
// struct ValMods {
//   bool is_const;
//   bool is_static;
//   eValCategory valcat;
// };
// struct Constraints {};
// struct ReturnStmt {};
// struct ContinueStmt {};
// struct BreakStmt {};
// struct SwitchStmt {};
// struct IfStmt {};
// struct WhileStmt {};
// struct ForStmt {};
// struct UsingDecl {
//   size_t eval_order;
//   Ident name;
//   PrimaryExpr type;
//   static Expected<UsingDecl> FromAst(const Ast& ast, Program& program, NamespaceDecl& ns, size_t& eval_order);
// };
//
// struct Namespace {
//   Namespace* parent{nullptr};
//   Ident name{};
//   std::unordered_map<Str, Namespace> subspaces;
//
//   //std::unordered_map<Str, VarDecl> const_fields;
//   //std::unordered_map<Str, VarDecl> fields;
//   //std::unordered_map<Str, FnDecl> methods;
//   //std::unordered_map<Str, ClassDecl> classes;
//   //std::unordered_map<Str, NamespaceDecl> namespaces;
//   //Vec<UsingDecl> typedefs;
//   //size_t next_evaluation_index;
//
//   //PrimaryExpr ResolvePrimaryId(const Ident& ident) const {
//   //  // Resolve the identifier in this namespace and return the primary expression.
//   //  if (const auto it = const_fields.find(ident.name); it != const_fields.end()) {
//   //    return PrimaryExpr{it->second.initializer};
//   //  } else if (const auto it = fields.find(ident.name); it != fields.end()) {
//   //    return PrimaryExpr{it->second.initializer};
//   //  } else if (const auto it = methods.find(ident.name); it != methods.end()) {
//   //    return PrimaryExpr{it->second};
//   //  } else if (const auto it = classes.find(ident.name); it != classes.end()) {
//   //    return PrimaryExpr{it->second};
//   //  } else if (const auto it = namespaces.find(ident.name); it != namespaces.end()) {
//   //    return PrimaryExpr{it->second};
//   //  }
//   //  return PrimaryExpr{};  // Not found
//   //}
//   //static Expected<NamespaceDecl> FromAst(const Ast& ast, Program& program, NamespaceDecl& ns, size_t& eval_order);
// };
//
////---------------------------------------------------------//
//// Class:{VarDecl}
//// Brief:{ IRAST Variable Declaration.}
//// Detail:{
////
//// }
////---------------------------------------------------------//
// struct Variable {
//   // Variable declarations may appear in 4 types of contexts.
//   // 1. At the namespace level, in which case the decl is implicitly static.
//   // 2. At the class level, in this case the decl is a member or static member
//   //    of the class in which it appears.
//   // 3. At the local function level.
//   // 4 .Inside a loop which is part of a function or a sub-loop of another loop.
//   //enum eVarDeclCategory { kProgramVar, kClassMember, kFunctional, kIterative } VarDeclCategory;
//
//   //size_t eval_order;
//   //eDeclVisibility visibility;
//   Ident ident;
//   PrimaryExpr initializer;
//   //Vec<Ident> qualified_name;  // Chain of identifiers through namespaces rooting from
//   //                            // the __global__ namespace which may be used to look up
//   //                            // the variable from the top level. eg.
//   //                            //     namespace @Details { def @hidden; }
//   //                            // qualified name of 'hidden' variable definition:
//   //                            //     vector<Ident>{"__global__","Details",'hidden'}
//   //ValMods mods;
//   //TypeList constraints;
//   //bool is_invariant;
//   //bool is_initialized;
//   //PrimaryExpr initializer;
//   //static Expected<VarDecl*> FromAst(const Ast& ast, Program& program, NamespaceDecl& ns, size_t& eval_order);
// };
//
// struct FnDecl {
//   size_t eval_order;
//   eDeclVisibility visibility;
//   Vec<Ident> qualified_name;
//   Ident name;
//   TypeList return_type;
//   Vec<FnParam> parameters;
//   Vec<FunctionalStmt> definition;
//   Vec<UsingDecl> typedefs;
//
//   static Expected<FnDecl*> FromAst(const Ast& ast, Program& program, NamespaceDecl& ns, size_t& eval_order);
// };
// struct ClassDecl {
//   size_t eval_order;
//   Ident name;
//   Vec<Ident> qualified_name;
//   NamespaceDecl static_ns;
//   Vec<VarDecl> member_fields;
//   Vec<FnDecl> member_methods;
//
//   bool has_ctor_def;
//   bool is_default_constructible;
//   bool is_copyable;
//   bool is_moveable;
//   bool is_trivial;
//   bool is_constexpr_evaluable;
//   bool is_const_evaluable;
//
//   FnDecl ctor;
//   FnDecl copy_ctor;
//   FnDecl move_ctor;
//   FnDecl destructor;
// };
// struct ImportDecl {};
// struct LibDecl {};
// struct MainDecl {};
// struct FnParam {};
// struct FnSignature {};  // FnParams + Constraints + Mods
// struct ValueList {};
// struct TypeList {};
// struct Library {};
// struct Process {};
//
///// @brief C& Program IR
///// Root IR node for a C& translation unit.
/////
///// This object representation is both a runtime and code model. Statements which are considered "compeval"
///// (occurs during compiler-evaluation stage) are computed in the IR virtual machine. Runtime code, on the
///// other hand, is passed forward to the IR code generator. This IR code is then used to  generate the final
///// executable code for processes and libraries within this translation unit.
// struct Program {
//   NamespaceDecl global_ns;
//
//   static Program Create();
//   static Expected<Program> FromAst(const Ast& ast);
// };
//
/////////////////////////////////////////////////////////////////////////////////
///// Implementations
/////////////////////////////////////////////////////////////////////////////////
//
// decltype(auto) CtAddOp(const auto& lhsv, const auto& rhsv) { return lhsv + rhsv; }
//
/////////////////////////////////////////////////////////////////////////////////
///// Create methods
/////////////////////////////////////////////////////////////////////////////////
// LitInt LitInt::Create(CtInt&& v) { return LitInt{v}; }
// LitUint LitUint::Create(CtUint&& v) { return LitUint{v}; }
// LitReal LitReal::Create(CtReal&& v) { return LitReal{v}; }
// LitBool LitBool::Create(CtBool&& v) { return LitBool{v}; }
// LitChar LitChar::Create(CtChar&& v) { return LitChar{v}; }
// LitByte LitByte::Create(CtByte&& v) { return LitByte{v}; }
// LitCstr LitCstr::Create(CtCstr&& v) { return LitCstr{v}; }
// Ident Ident::Create(CtCstr&& v) { return Ident{v}; }
//
/////////////////////////////////////////////////////////////////////////////////
///// FromAst methods
/////////////////////////////////////////////////////////////////////////////////
// Expected<LitInt> LitInt::FromAst(const Ast& ast) {
//   assert(ast.TypeIs(eAst::kLitInt) && "Expected kLitInt ast type.");
//   CtInt value{};
//   try {
//     value = std::stoll(ast.GetLiteral());
//   } catch (const std::exception&) {
//     return Expected<LitInt>::Failure("Failed to convert number literal to integer value.");
//   }
//   return Expected<LitInt>::Success(LitInt{.val = value});
// }
//
// Expected<LitUint> LitUint::FromAst(const Ast& ast) {
//   assert(ast.TypeIs(eAst::kLitUint) && "Expected kLitUint ast type.");
//   auto lit = ast.GetLiteral();
//   assert((lit.back() != 'u') && "Implementation failure. Unsigned literal not followed by 'u'.");
//   auto unsigned_str = lit.substr(0, lit.size() - 1);
//   LitUint ret{};
//   try {
//     ret.val = std::stoull(unsigned_str);
//   } catch (std::exception&) {
//     return Expected<LitUint>::Failure("Failed to convert unsigned number literal to unsigned integer value.");
//   }
//   return Expected<LitUint>::Success(ret);
// }
//
// Expected<LitReal> LitReal::FromAst(const Ast& ast) {
//   assert(ast.TypeIs(eAst::kLitReal) && "Expected kLitReal ast type.");
//   LitReal ret{};
//   auto lit = ast.GetLiteral();
//   try {
//     ret.val = std::stod(lit);
//   } catch (std::exception&) {
//     return Expected<LitReal>::Failure("Failed to convert decimal literal to floating point value.");
//   }
//   return Expected<LitReal>::Success(ret);
// }
//
// Expected<LitBool> LitBool::FromAst(const Ast& ast) {
//   assert((ast.TypeIs(eAst::kLitBool) || ast.TypeIs(eAst::kKwTrue) || ast.TypeIs(eAst::kKwFalse)) &&
//          "Expected kLitBool, kKwTrue or kKwFalse ast type.");
//   LitBool ret{};
//   auto lit = ast.GetLiteral();
//   if (ast.TypeIs(eAst::kLitBool)) {
//     assert((lit.back() != 'b') && "Implementation failure. Bit literal not followed by 'b'.");
//     if (lit == "1b") {
//       ret.val = true;
//     } else if (lit == "0b") {
//       ret.val = false;
//     } else {
//       return Expected<LitBool>::Failure("Failed to bit literal to boolean value.");
//     }
//   } else if (ast.TypeIs(eAst::kKwTrue))
//     ret.val = true;
//   else  // else has to be 'ast.TypeIs(eAst::kKwFalse) == true'
//     ret.val = false;
//   return Expected<LitBool>::Success(ret);
// }
//
// Expected<LitByte> LitByte::FromAst(const Ast& ast) {
//   assert(ast.TypeIs(eAst::kLitByte) && "Expected kLitByte ast type.");
//   auto lit = ast.GetLiteral();
//   assert((lit.back() != 'B') && "Implementation failure. Byte literal not followed by 'B'.");
//   auto byte_str = lit.substr(0, lit.size() - 1);
//
//   LitByte ret{};
//   try {
//     ret.val = static_cast<unsigned char>(std::stoi(byte_str));
//   } catch (std::exception&) {
//     return Expected<LitByte>::Failure("Failed to convert byte literal to unsigned char value.");
//   }
//   return Expected<LitByte>::Success(ret);
// }
//
// Expected<LitChar> LitChar::FromAst(const Ast& ast) {
//   assert(ast.TypeIs(eAst::kLitChar) && "Expected kLitChar ast type.");
//   auto lit = ast.GetLiteral();
//   assert((lit.back() != 'c') && "Implementation failure. Byte literal not followed by 'c'.");
//   // literal will be in the form [0-255]c or '[character]'c
//   LitChar ret{};
//   // if the literal is a character it begins with apostrophe
//   if (lit[0] == '\'') {
//     // remove the quotes and the c
//     auto byte_str = lit.substr(1, lit.size() - 3);
//     // check for escape characters.
//     auto escape_pos = byte_str.find('\\');
//     if (escape_pos != Str::npos) {
//       // replace the escape characters
//       switch (lit[escape_pos + 1]) {
//         case 'n':
//           byte_str.replace(escape_pos, 2, "\n");
//           break;
//         case 't':
//           byte_str.replace(escape_pos, 2, "\t");
//           break;
//         case 'r':
//           byte_str.replace(escape_pos, 2, "\r");
//           break;
//         case '0':
//           byte_str.replace(escape_pos, 2, "\0");
//           break;
//         case '\\':
//           byte_str.replace(escape_pos, 2, "\\");
//           break;
//         case '\'':
//           byte_str.replace(escape_pos, 2, "'");
//           break;
//         case '\"':
//           byte_str.replace(escape_pos, 2, "\"");
//           break;
//         default:
//           return Expected<LitChar>::Failure("Invalid escape character:" + lit);
//       }
//     }
//     if (byte_str.size() != 1)
//       return Expected<LitChar>::Failure("Invalid character literal, maximum of one character allowed:" + lit);
//     ret.val = static_cast<char>(byte_str.at(0));
//   }
//   // else it has to be a number character literal
//   else {
//     auto char_str = ast.GetLiteral().substr(0, ast.GetLiteral().size() - 1);
//     try {
//       ret.val = static_cast<char>(std::stoi(char_str));
//     } catch (const std::exception&) {
//       return Expected<LitChar>::Failure("Failed to convert character literal to char value.");
//     }
//   }
//   return Expected<LitChar>::Success(ret);
// }
//
// Expected<LitCstr> LitCstr::FromAst(const Ast& ast) {
//   assert(ast.TypeIs(eAst::kLitCstr) && "Expected kLitCstr ast type.");
//   Str literal = ast.GetLiteral();
//   // Remove the quotes
//   literal = literal.substr(1, literal.size() - 2);
//   // Check for escape characters.
//   bool no_escape = false;
//   size_t current_pos = 0;
//   while (!no_escape) {
//     auto escape_pos = literal.find('\\', current_pos);
//     if (escape_pos == Str::npos) {
//       no_escape = true;
//     } else {
//       // Replace the escape characters
//       switch (literal[escape_pos + 1]) {
//         case 'n':
//           literal.replace(escape_pos, 2, "\n");
//           current_pos = escape_pos + 1;
//           break;
//         case 't':
//           literal.replace(escape_pos, 2, "\t");
//           current_pos = escape_pos + 1;
//           break;
//         case 'r':
//           literal.replace(escape_pos, 2, "\r");
//           current_pos = escape_pos + 1;
//           break;
//         case '0':
//           literal.replace(escape_pos, 2, "\0");
//           current_pos = escape_pos + 1;
//           break;
//         case '\\':
//           literal.replace(escape_pos, 2, "\\");
//           current_pos = escape_pos + 1;
//           break;
//         case '\'':
//           literal.replace(escape_pos, 2, "'");
//           current_pos = escape_pos + 1;
//           break;
//         case '\"':
//           literal.replace(escape_pos, 2, "\"");
//           current_pos = escape_pos + 1;
//           break;
//         default:
//           return Expected<LitCstr>::Failure("Invalid escape character:" + literal);
//       }
//     }
//   }
//   return Expected<LitCstr>::Success(LitCstr{.val = literal});
// }
//
// Expected<Ident> Ident::FromAst(const Ast& ast) {
//   assert(ast.TypeIs(eAst::kIdent) && "Expected kIdent ast type.");
//   return Expected<Ident>::Success(Ident{.name = ast.GetLiteral()});
// }
//
// Expected<PrimaryExpr> OpAdd::FromAst(const Ast& ast, Program& program) {
//   assert(ast.TypeIs(eAst::kAdd) && "Expected kAdd ast type.");
//   assert((ast.children.size() >= 2) && "Expected kAdd ast type to contain 2 operand branches.");
//   const Ast& lhs = ast.children[0];
//   const Ast& rhs = ast.children[1];
//   if (lhs.IsLiteral()) {
//     if (rhs.IsLiteral()) {
//       // Both sides are literals -> perform constant fold.
//       auto lhs_lit = PrimaryExpr::FromAst(lhs, program);
//       if (not lhs_lit) return Expected<PrimaryExpr>::Failure(lhs_lit.Error());
//       auto rhs_lit = PrimaryExpr::FromAst(rhs, program);
//       if (not rhs_lit) return Expected<PrimaryExpr>::Failure(rhs_lit.Error());
//       auto result = CtAddOp(lhs_lit.Extract(), rhs_lit.Extract());
//       return Expected<PrimaryExpr>::Success(result);
//     } else if (rhs.TypeIs(eAst::kIdent)) {
//       auto id = Ident::FromAst(rhs);
//       if (!id) return Expected<PrimaryExpr>::Failure(id.Error());
//       PrimaryExpr ident_val_ir = program.global_ns.ResolvePrimaryId(id.Value());
//       if (ident_val_ir.IsConsteval()) {
//         auto rhs_lit = ident_val_ir.ReduceConsteval(program);
//         if (not rhs_lit) return Expected<PrimaryExpr>::Failure(rhs_lit.Error());
//         auto result = CtAddOp(lhs_lit.Extract(), rhs_lit.Extract());
//         return Expected<PrimaryExpr>::Success(result);
//       }
//       // Else its not possible to eval the id at this stage. Result is a partial expression.
//       auto operation_ir = OpAdd::Create({lhs, rhs});
//       return Expected<PrimaryExpr>::Success(operation_ir);
//     }
//   }
// }
//
// Expected<Program> Program::FromAst(const Ast& ast) {
//   // 1. Root ast must be a program.
//   if (ast.TypeIsnt(eAst::Program)) {
//     return Expected<Program>::Failure("Root ast must be a program.");
//   }
//
//   // 2. Create a base program definition:
//   //   - global namespace
//   //   - predefined methods vars namespaces can go here.
//   Program program;
//   program.global_ns.eval_order = 0;
//   program.global_ns.name = Ident{kGlobalNamespaceName.data()};
//
//   // 3. Iterate over all the statements and generate IrAst for each.
//   size_t eval_order = 0;
//   for (const auto& stmt : ast.Children()) {
//     // All statements must be pragmatic at the top level.
//     if (not stmt.IsPragmatic()) {
//       return Expected<Program>::Failure("All statements must be pragmatic at the top level.");
//     }
//     // Generate IrAst for the statements.
//     // Keep track of the order of evaluation
//     if (stmt.TypeIs(eAst::VariableDeclaration) || stmt.TypeIs(eAst::VariableDefinition)) {
//       auto var_def_ir = VarDecl::FromAst(stmt, program, program.global_ns, eval_order);
//       if (not var_def_ir) return Expected<Program>::Failure(var_def_ir.Error());
//     } else if (stmt.TypeIs(eAst::MethodDeclaration) || stmt.TypeIs(eAst::MethodDefinition)) {
//       auto fn_def_ir = FnDecl::FromAst(stmt, program, program.global_ns, eval_order);
//       if (not fn_def_ir) return Expected<Program>::Failure(fn_def_ir.Error());
//     } else if (stmt.TypeIs(eAst::KwUse)) {
//       auto use_def_ir = UsingDecl::FromAst(stmt, program, program.global_ns, eval_order);
//       if (not use_def_ir) return Expected<Program>::Failure(use_def_ir.Error());
//     } else if (stmt.TypeIs(eAst::ClassDeclaration)) {
//       auto use_def_ir = UsingDecl::FromAst(stmt, program, program.global_ns, eval_order);
//       if (not use_def_ir) return Expected<Program>::Failure(use_def_ir.Error());
//     } else if (stmt.TypeIs(eAst::KwNamespace)) {
//       auto use_def_ir = UsingDecl::FromAst(stmt, program, program.global_ns, eval_order);
//       if (not use_def_ir) return Expected<Program>::Failure(use_def_ir.Error());
//     }
//   }
//   // 4. Return the program.
//   return Expected<Program>::Success(program);
// }
//
// Expected<NamespaceDecl*> NamespaceDecl::FromAst(const Ast& ast, Program& program, NamespaceDecl& ns,
//                                                 size_t& eval_order) {
//   if (ast.TypeIsnt(eAst::KwNamespace)) {
//     return Expected<NamespaceDecl*>::Failure("Expected Namespace Declaration");
//   }
//   // Get the namespace name
//   auto ns_name = Ident::FromAst(ast[0]);
//   if (not ns_name) return Expected<NamespaceDecl*>::Failure(ns_name.Error());
//   size_t internal_eval_order = 0;
//   // Check if the namespace is already defined.
//   if (ns.namespaces.contains(ns_name.Value().name)) {
//     auto& existing_ns = ns.namespaces[ns_name.Value().name];
//     // Get the last evaluation order from within that namespace.
//     auto& internal_eval_order = existing_ns.next_evaluation_index;
//     // Add statements to the previously created namespace.
//     for (const auto& stmt : ast.Children()) {
//       if (not stmt.IsPragmatic()) {
//         return Expected<NamespaceDecl*>::Failure("All statements must be pragmatic at the namespace level.");
//       }
//       if (stmt.TypeIs(eAst::VariableDeclaration) || stmt.TypeIs(eAst::VariableDefinition)) {
//         auto var_def_ir = VarDecl::FromAst(stmt, program, existing_ns, internal_eval_order);
//         if (not var_def_ir) return Expected<NamespaceDecl*>::Failure(var_def_ir.Error());
//       } else if (stmt.TypeIs(eAst::MethodDeclaration) || stmt.TypeIs(eAst::MethodDefinition)) {
//         auto fn_def_ir = FnDecl::FromAst(stmt, program, existing_ns, internal_eval_order);
//         if (not fn_def_ir) return Expected<NamespaceDecl*>::Failure(fn_def_ir.Error());
//       } else if (stmt.TypeIs(eAst::KwUse)) {
//         auto use_def_ir = UsingDecl::FromAst(stmt, program, existing_ns, internal_eval_order);
//         if (not use_def_ir) return Expected<NamespaceDecl*>::Failure(use_def_ir.Error());
//       } else if (stmt.TypeIs(eAst::ClassDeclaration)) {
//         auto use_def_ir = UsingDecl::FromAst(stmt, program, existing_ns, internal_eval_order);
//         if (not use_def_ir) return Expected<NamespaceDecl*>::Failure(use_def_ir.Error());
//       } else if (stmt.TypeIs(eAst::KwNamespace)) {
//         auto use_def_ir = UsingDecl::FromAst(stmt, program, existing_ns, internal_eval_order);
//         if (not use_def_ir) return Expected<NamespaceDecl*>::Failure(use_def_ir.Error());
//       }
//     }
//     return Expected<NamespaceDecl*>::Success(&existing_ns);
//   }
//   // Else this is a new namespace.
//   else {
//     // Make sure namespace does not have
//     // the same name as a class, typedef or variable
//     // in the parent namespace.
//     if (ns.classes.contains(ns_name.Value().name)) {
//       return Expected<NamespaceDecl*>::Failure("Namespace cannot have the same name as a class.");
//     }
//     if (ns.typedefs.contains(ns_name.Value().name)) {
//       return Expected<NamespaceDecl*>::Failure("Namespace cannot have the same name as a typedef.");
//     }
//     if (ns.fields.contains(ns_name.Value().name)) {
//       return Expected<NamespaceDecl*>::Failure("Namespace cannot have the same name as a variable.");
//     }
//     if (ns.const_fields.contains(ns_name.Value().name)) {
//       return Expected<NamespaceDecl*>::Failure("Namespace cannot have the same name as a variable.");
//     }
//     // Create a new namespace and add to parent namespace.
//     ns.namespaces[ns_name.Value().name] = NamespaceDecl{};
//     NamespaceDecl& ns_decl = ns.namespaces[ns_name.Value().name];
//     ns_decl.parent = &ns;
//     ns_decl.eval_order = eval_order;
//     ns_decl.next_evaluation_index = 0;
//     ns_decl.name = ns_name.Extract();
//
//     // Add statements to the newly created namespace.
//     for (const auto& stmt : ast.Children()) {
//       if (not stmt.IsPragmatic()) {
//         return Expected<NamespaceDecl*>::Failure("All statements must be pragmatic at the namespace level.");
//       }
//       if (stmt.TypeIs(eAst::VariableDeclaration) || stmt.TypeIs(eAst::VariableDefinition)) {
//         auto var_def_ir = VarDecl::FromAst(stmt, program, ns_decl, internal_eval_order);
//         if (not var_def_ir) return Expected<NamespaceDecl*>::Failure(var_def_ir.Error());
//       } else if (stmt.TypeIs(eAst::MethodDeclaration) || stmt.TypeIs(eAst::MethodDefinition)) {
//         auto fn_def_ir = FnDecl::FromAst(stmt, program, ns_decl, internal_eval_order);
//         if (not fn_def_ir) return Expected<NamespaceDecl*>::Failure(fn_def_ir.Error());
//       } else if (stmt.TypeIs(eAst::KwUse)) {
//         auto use_def_ir = UsingDecl::FromAst(stmt, program, ns_decl, internal_eval_order);
//         if (not use_def_ir) return Expected<NamespaceDecl*>::Failure(use_def_ir.Error());
//       } else if (stmt.TypeIs(eAst::ClassDeclaration)) {
//         auto use_def_ir = UsingDecl::FromAst(stmt, program, ns_decl, internal_eval_order);
//         if (not use_def_ir) return Expected<NamespaceDecl*>::Failure(use_def_ir.Error());
//       } else if (stmt.TypeIs(eAst::KwNamespace)) {
//         auto use_def_ir = UsingDecl::FromAst(stmt, program, ns_decl, internal_eval_order);
//         if (not use_def_ir) return Expected<NamespaceDecl*>::Failure(use_def_ir.Error());
//       }
//     }
//     // return ptr to created namespace
//     return Expected<NamespaceDecl*>::Success(&ns_decl);
//   }
// }
//
// Expected<VarDecl*> VarDecl::FromAst(const Ast& ast, Program& program, NamespaceDecl& context, size_t& eval_order,
//                                     eVarDeclCategory decl_category) {
//   if (not(ast.TypeIs(eAst::VariableDeclaration) || ast.TypeIs(eAst::VariableDefinition))) {
//     return cxx::Expected<VarDecl*>::Failure("Root ast must be VariableDeclaration or VariableDefinition");
//   }
//
//   // If this is a static or member variable assert the name is unique
//   // in the namespace or class or functional or iterative context.
//   // Iterative variables which collide with names outside of
//   // the iterative context - up to the owning functional context,
//   // will emit an error of shadowed identifiers.
//   // (Warning or Error? Warnings are bad so for now error.)
//   if (decl_category == eVarDeclCategory::kProgramVar || decl_category == eVarDeclCategory::kClassMember) {
//     auto var_name = Ident::FromAst(ast[2]);
//     if (not var_name) return Expected<VarDecl*>::Failure(var_name.Error());
//     if (context.fields.contains(var_name.Value().name) || context.const_fields.contains(var_name.Value().name)) {
//       return Expected<VarDecl*>::Failure("Variable name must be unique in the namespace.");
//     }
//   }
//
//   // 3. Create a new variable definition.
//   ns.fields[var_name.Value().name] = VarDecl{};
//   VarDecl& var_def = ns.fields[var_name.Value().name];
//   var_def.eval_order = eval_order;
//   var_def.name = var_name.Extract();
//   var_def.qualified_name = cxx::MergeVectors(ns.qualified_name, {var_def.name});
//
//   // 4. Increment eval order.
//   eval_order++;
//
//   // 5. Get the modifiers.
//   auto mods_ir = ValMods::FromAst(ast[0]);
//   if (not mods_ir) return mods_ir;
//   var_def.mods = mods_ir.Extract();
//
//   // 6. Type Constraints
//   auto constraints_ir = TypeList::FromAst(ast[1]);
//   if (not constraints_ir) return constraints_ir;
//   var_def.constraints = constraints_ir.Extract();
//
//   // 7. Initializer
//   //    If its a var decl with no provided initializer:
//   //    -> Provide a default initializer.
//   if (ast.TypeIs(eAst::VariableDeclaration)) {
//     // -> Const ?
//     if (var_def.mods.is_const) {
//       return Expected<VarDecl*>::Failure("Constant values must be explicitly initialized.");
//     }
//     // If the variable is an Owned Value (the default)
//     //   -> If there is a type constraint.
//     //       -> If the type is default initializable provide default init.
//     //       -> Else report an error.
//     //   -> If there is no type constraint and no initializer.
//     //     -> Default initialize to a NoneT
//     if (var_def.mods.valcat == eValCategory::Owned) {
//       if (var_def.constraints.IsAny()) {
//         // Create Default Initializer for NoneT
//         var_def.initializer = FuncCall(KwNone(), FuncArgs::VoidArgs());
//       } else {
//         var_def.initializer = FuncCall(var_def.constraints.types.at(0), FuncArgs::VoidArgs());
//       }
//     }
//     // If the variable is marked as Reference Value('ref').
//     //  -> If constrained default init to a nullreference of type T
//     //  -> If not constrained default init to a void_reference
//     else if (var_def.mods.valcat == eValCategory::Reference) {
//       var_def.initializer = FuncCall(KwVoid(), FuncArgs::VoidArgs());
//     }
//     // Is a view?
//     else if (var_def.mods.valcat == eValCategory::View) {
//       return Expected<VarDecl*>::Failure(
//           "Views must always be initialized to a left hand side value.Cannot "
//           "view void.");
//     }
//   } else {  // Variable Definition
//     auto init_expr_ir = PrimaryExpr::FromAst(ast[3], program);
//     if (not init_expr_ir) return init_expr_ir;
//     var_def.initializer = init_expr_ir.Extract();
//     if (var_def.initializer.IsConstEval()) {
//       var_def.is_consteval = true;
//     }
//   }
//   return Expected<VarDecl*>::Success(&var_def);
// }
//
// Expected<FnDecl*> FnDecl::FromAst(const Ast& ast, Program& program, NamespaceDecl& ns, size_t& eval_order) {
//   // 1. Root must be a fn def or fn decl.
//   if (not(ast.TypeIs(eAst::MethodDeclaration) || ast.TypeIs(eAst::MethodDefinition))) {
//     return cxx::Expected<FnDecl*>::Failure("Root ast must be MethodDeclaration or MethodDefinition");
//   }
//   // 2. Make sure the function name is unique in the namespace.
//   auto fn_name = Ident::FromAst(ast[2]);
//   if (not fn_name) return Expected<FnDecl*>::Failure(fn_name.Error());
//   if (ns.methods.contains(fn_name.Value().name)) {
//     return Expected<FnDecl*>::Failure("Function name must be unique in the namespace.");
//   }
//
//   // 3. Create a new function definition.
//   ns.methods[fn_name.Value().name] = FnDecl{};
//   FnDecl& fn_def = ns.methods[fn_name.Value().name];
//   fn_def.eval_order = eval_order;
//   fn_def.name = fn_name.Extract();
//
//   // 4. Increment eval order.
//   eval_order++;
//
//   // 5. Get the modifiers.
//   auto mods_ir = ValMods::FromAst(ast[0]);
//   if (not mods_ir) return mods_ir;
//   fn_def.mods = mods_ir.Extract();
//
//   // 6. Return Type
//   auto return_type_ir = TypeList::FromAst(ast[1]);
//   if (not return_type_ir) return return_type_ir;
//   fn_def.return_type = return_type_ir.Extract();
//
//   // 7. Parameters
//   for (const auto& param : ast[3].Children()) {
//     auto param_ir = FnParam::FromAst(param);
//     if (not param_ir) return param_ir;
//     fn_def.parameters.push_back(param_ir.Extract());
//   }
//
//   // 8. Function Definition
//   for (const auto& stmt : ast[4].Children()) {
//     if (not stmt.IsFunctional()) {
//       return Expected<FnDecl*>::Failure("All statements must be functional at the function level.");
//     }
//     auto stmt_ir = FunctionalStmt::FromAst(stmt, program);
//     if (not stmt_ir) return stmt_ir;
//     fn_def.definition.push_back(stmt_ir.Extract());
//   }
//   return Expected<FnDecl*>::Success(&fn_def);
// }
//
// Expected<UsingDecl*> UsingDecl::FromAst(const Ast& ast, Program& program, NamespaceDecl& ns, size_t& eval_order) {
//   if (ast.TypeIsnt(eAst::KwUse)) {
//     return Expected<UsingDecl>::Failure("Expected Using Declaration");
//   }
//
//   // Make sure the using name is unique in the namespace.
//   auto using_name = Ident::FromAst(ast[0]);
//   if (not using_name) return Expected<UsingDecl>::Failure(using_name.Error());
//   if (ns.typedefs.contains(using_name.Value().name) || ns.classes.contains(using_name.Value().name) ||
//       ns.fields.contains(using_name.Value().name) || ns.const_fields.contains(using_name.Value().name)) {
//     return Expected<UsingDecl>::Failure("Using name must be unique in the namespace.");
//   }
//
//   // Create a new using declaration.
//   ns.typedefs[using_name.Value().name] = UsingDecl{};
//   UsingDecl& using_decl = ns.typedefs[using_name.Value().name()];
//   using_decl.eval_order = eval_order;
//   using_decl.name = using_name.Extract();
//
//   // Determine the type of using declaration.
//   // Must be const_evaluable.
//   auto primary_type_expr = PrimaryExpr::FromAst(ast[1], program, ns, eval_order);
//   if (not primary_type_expr) return primary_type_expr;
//   if (not primary_type_expr.Value().IsConsteval()) {
//     return Expected<UsingDecl>::Failure("Using declaration must be const_evaluable.");
//   }
//   auto primary_type = primary_type_expr.Value().GetAstTypelist();
//   if (not primary_type)
//     return Expected<UsingDecl>::Failure(
//         "Right hand side of using declaration must evaluate to a Typelist at "
//         "compile time.");
//   using_decl.type = primary_type.Extract();
//
//   // Increment eval order.
//   eval_order++;
// }
//
// Expected<ClassDecl*> ClassDecl::FromAst(const Ast& ast, Program& program, NamespaceDecl& ns, size_t& eval_order) {
//   if (ast.TypeIsnt(eAst::ClassDeclaration)) {
//     return Expected<ClassDecl*>::Failure("Expected Class Declaration");
//   }
//   // Get the class name
//   auto class_name = Ident::FromAst(ast[0]);
//   if (not class_name) return Expected<ClassDecl*>::Failure(class_name.Error());
//   // Check if the class is already defined.
//   if (ns.classes.contains(class_name.Value().name)) {
//     return Expected<ClassDecl*>::Failure("Class name must be unique in the namespace.");
//   }
//   // Create a new class and add to parent namespace.
//   ns.classes[class_name.Value().name] = ClassDecl{};
//   ClassDecl& class_decl = ns.classes[class_name.Value().name];
//   class_decl.eval_order = eval_order;
//   class_decl.name = class_name.Extract();
//   class_decl.qualified_name = cxx::MergeVectors(ns.qualified_name, {class_decl.name});
//
//   // Add statements to the newly created class.
//   for (const auto& stmt : ast.Children()) {
//     if (not stmt.IsPragmatic()) {
//       return Expected<ClassDecl*>::Failure("All statements must be declarative at the class level.");
//     }
//     if (stmt.TypeIs(eAst::VariableDeclaration) || stmt.TypeIs(eAst::VariableDefinition)) {
//       auto var_def_ir = VarDecl::FromAst(stmt, program, class_decl, eval_order);
//       if (not var_def_ir) return Expected<ClassDecl*>::Failure(var_def_ir.Error());
//     } else if (stmt.TypeIs(eAst::MethodDeclaration) || stmt.TypeIs(eAst::MethodDefinition)) {
//       auto fn_def_ir = FnDecl::FromAst(stmt, program, class_decl, eval_order);
//       if (not fn_def_ir) return Expected<ClassDecl*>::Failure(fn_def_ir.Error());
//     } else if (stmt.TypeIs(eAst::KwUse)) {
//       auto use_def_ir = UsingDecl::FromAst(stmt, program, class_decl, eval_order);
//       if (not use_def_ir) return Expected<ClassDecl*>::Failure(use_def_ir.Error());
//     } else if (stmt.TypeIs(eAst::ClassDeclaration)) {
//       auto use_def_ir = UsingDecl::FromAst(stmt, program, class_decl, eval_order);
//       if (not use_def_ir) return Expected<ClassDecl*>::Failure(use_def_ir.Error());
//     } else if (stmt.TypeIs(eAst::KwNamespace)) {
//       auto use_def_ir = UsingDecl::FromAst(stmt, program, class_decl, eval_order);
//       if (not use_def_ir) return Expected<ClassDecl*>::Failure(use_def_ir.Error());
//     }
//   }
//
//   // return ptr to created class
//   return Expected<ClassDecl*>::Success(&class_decl);
// }
}  // namespace hir
}  // namespace cnd
