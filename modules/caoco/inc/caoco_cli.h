//#pragma once
//#include "caoco_lexer.h"
//#include "caoco_parser.h"
//#include "cxxx.hpp"
//
//namespace caoco {
//using std::expected;
//using std::map;
//using std::string;
//using std::unexpected;
//using std::vector;
//
//#ifdef WIN32
//using main_int = unsigned long;
//#else
//using main_int = int;
//#endif
//
//
//void RemoveQuotesFromCliArg(string& arg) {
//  while (arg.size() > 2 && arg.front() == '"' && arg.back() == '"') {
//    arg = arg.substr(1, arg.size() - 2);
//  }
//}
//
//// Format:
//// 1st arg is always the compiler 'mode'.
//// cnd [mode] [command] [args...]
//// cnd pure build . : Locates a root.cnd file which must contain a fn@root.
//// cnd cmake build . : Locates CMakeLists.txt to build the project.
//// cnd cpp build . : Transpiles to C++ Uses native C++ compiler to build the
//// project. Args: When not provided a default is generated. Usually native to
//// native compilation. -CxxCompilerPath=: Path to the C++ compiler.
////  -AddCxxCompilerArgs=: Additional Compiler arguments.
//// cnd c build . : Transpiles to C++/C. Uses native C compiler to build the
//// project.
//
//struct CliArgs {
//  enum Mode {
//    kCnd,    // Pure C& to executable or static library mode.
//    kCmake,  // Uses cmake to drive build process, requires CMake project to
//             // 'include(CandLang)' And add targets using provided cmake
//             // function interface.
//    kCpp,    // Compile to an intermediate representation of C++, for export and
//             // use in C++ code bases, to be built by an external make tool eg.
//             // CMake.
//    kC,  // Uses native C++ compiler in C mode, imposes constraints to make the
//         // exported code callable from C.
//  } mode;
//  enum Action {
//    kCodegen,
//    kBuild,
//  } action;
//  std::map<string, string> options;
//  std::map<string, string> definitions;
//};
//
//struct CompProcessDesc {
//  void* input;
//  vector<std::function<void*(void*)>> procs;
//  void* output;
//  expected<int, caerr::CaErrUptrVec> RunCompiler() {
//    assert(procs.size() > 0 &&
//           "cnd::cli::RunCompiler : Uninitialized process description.");
//    void* last_data = input;
//    for (size_t i = 0; i < procs.size(); i++) {
//      void* res = procs[i](last_data);
//      last_data = res;
//    }
//    output = last_data;
//  }
//};
//
//expected<map<string, string>, caerr::CaErrUptrVec> ParseCliOptions(
//    CliArgs::Mode mode, CliArgs::Action action, const vector<string>& args,
//    CliArgs& apply_to){
//
//};
//
//expected<map<string, string>, caerr::CaErrUptrVec> ParseCliDefinitions(
//    CliArgs::Mode mode, CliArgs::Action action, const vector<string>& args,
//    CliArgs& apply_to){
//
//};
//
//expected<CliArgs, caerr::CaErrUptrVec> ParseCliArguments(int argc,
//                                                         char* argv[]) {
//  using namespace caerr;
//  vector<string> args(argv, argv + argc);
//  for (auto& arg : args) {
//    RemoveQuotesFromCliArg(arg);
//  }
//
//  CliArgs ret;
//  // 1st argument is always the mode:
//  auto& requested_mode = args.front();
//  if (requested_mode == "pure")
//    ret.mode = CliArgs::Mode::kCnd;
//  else if (requested_mode == "cmake")
//    ret.mode = CliArgs::Mode::kCmake;
//  else if (requested_mode == "cpp")
//    ret.mode = CliArgs::Mode::kCpp;
//  else if (requested_mode == "c")
//    ret.mode = CliArgs::Mode::kC;
//  else
//    return unexpected<CaErrUptrVec>{{MakeError(caerr::InvalidCliArg{
//        requested_mode, "Specified compiler mode is unknown."})}};
//
//  // 2nd argument is always the 'action'
//  auto& requested_action = args[1];
//  if (requested_action == "build")
//    ret.action = CliArgs::Action::kBuild;
//  else if (requested_action == "codegen")
//    ret.action = CliArgs::Action::kCodegen;
//  else
//    return unexpected<CaErrUptrVec>{{MakeError(caerr::InvalidCliArg{
//        requested_action, "Specified compiler action is unknown."})}};
//
//  // The rest are the arguments. Format is dependant on the action and mode.
//  auto opt_parse_res = ParseCliOptions(ret.mode, ret.action, args, ret);
//  if (!opt_parse_res) return unexpected<CaErrUptrVec>{opt_parse_res.error()};
//
//  auto def_parse_res = ParseCliDefinitions(ret.mode, ret.action, args, ret);
//  if (!def_parse_res) return unexpected<CaErrUptrVec>{def_parse_res.error()};
//
//  return ret;
//}
//
//
//void PrintErrorToStdout(const caerr::CaErrUptr& err) {
//  std::cout << err->Format() << std::endl;
//}
//void PrintErrorToStdout(const caerr::CaErrUptrVec& errs) {
//  for (auto& e : errs) PrintErrorToStdout(e);
//}
//
//expected<main_int, caerr::CaErrUptrVec> RunCompilationProcess(
//    const CliArgs& clargs) {
//}
//
//expected<main_int, caerr::CaErrUptrVec> RunPureBuild(const string& root_dir,
//                                                     const string& root_src_file,
//                                                     const string& intermediate_dir, const string& out_dir,const string & target) {
//  auto root_file_res = LoadFile(root_src_file);
//  if (!root_file_res) return unexpected<caerr::CaErrUptrVec>(std::move(root_file_res.error()));
//  auto lex_res = caoco::Lexer::Lex(root_file_res.value());
//  if (!lex_res) {
//    std::cout << "Lexer error: " << lex_res.Error() << std::endl;
//  }
//  auto preprocess_res = caoco::Preprocessor::Preprocess(lex_res,intermediate_dir);
//  if (!preprocess_res)
//    return unexpected<caerr::CaErrUptrVec>(std::move(preprocess_res.error()));
//  auto parse_res =
//      parser::ParseProgram({lex_res.Value().begin(), lex_res.Value().end()});
//  if (!parse_res)
//    return unexpected<caerr::CaErrUptrVec>(std::move(parse_res.error()));
//  auto semantic_analysis_res =
//      caoco::SemanticAnalyzer::GenerateIrAst(parse_res.Value());
//  auto constant_evaluation_res =
//      caoco::ConstantEvalutator::Evaluate(semantic_analysis_res);
//  auto cail_gen =
//      caoco::Cail::GenCailFromCndIr(semantic_analysis_res.value());
//  auto optimization_res =
//      caoco::Cail::RunOptimizationPasses(cail_gen.value());
//
//  auto makegen_res =
//      caoco::CodeGenerator::CaMake::GenerateNativeMakeScript(cail_gen);
//  auto codegen_res = caoco::CodeGenerator::Cpp::GenerateNativeCpp(cail_gen);
//
//  // call native script applied to the generated code, based on the mode:
//  // in cmake mode we call cmake.
//  // in pure mode the call in platform dependent and undefined, usually a
//  // shell script or cmake file - implementation dependant.
//  // in cpp mode the code is not built, only generated.
//  // end result should be the executables and dlls of the build.
//    auto composition_res = target.empty()
//                               ? caoco::Composer::ComposeBinaryPackage(
//                                     out_dir, makegen_res, codegen_res)
//                               : caoco::Composer::ComposeBinaryPackage(
//                                     out_dir, makegen_res, codegen_res, target);
//}
//
//
//
//main_int CliMain(int argc, char* argv[]) {
//  auto clargs_parse_res = ParseCliArguments(argc, argv);
//  if (!clargs_parse_res) {
//    PrintErrorToStdout(clargs_parse_res.error());
//    return EXIT_FAILURE;
//  }
//  auto& clargs = clargs_parse_res.value();
//
//  auto comp_proc_gen_res = RunCompilationProcess(clargs);
//  if (!comp_proc_gen_res) {
//    PrintErrorToStdout(comp_proc_gen_res.error());
//    return EXIT_FAILURE;
//  }  
//  
//  
//  // First arg should be the root cand file.
//    auto lex_res =
//        caoco::Lexer::Lex(clargs.options["CND_ROOT_TRANSLATION_FILE"]);
//  if (!lex_res) {
//    std::cout << "Lexer error: " << lex_res.Error() << std::endl;
//  }
//  auto parse_res = caoco::ParseTokens(lex_res.Value());
//  if (!parse_res) {
//    std::cout << "Parser error: " << parse_res.Error() << std::endl;
//  }
//
//  return 0;
//}
//}  // namespace caoco
