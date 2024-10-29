/////////////////////////////////////////////////////////////////////////////////
//// Copyright 2024 Anton Yashchenko
//// Licensed under the GNU Affero General Public License, Version 3.
/////////////////////////////////////////////////////////////////////////////////
//// @project: C& Programming Language Environment
//// @author(s): Anton Yashchenko
//// @website: https://www.acpp.dev
/////////////////////////////////////////////////////////////////////////////////
///// @file
///// @ingroup cand_compiler_data
///// @brief Describes the grammar of the C++ programming language.
/////        Punctuation, keywords, operators, errors and other syntax elements.
/////
///// See compiler reference manual for details.
/////////////////////////////////////////////////////////////////////////////////
//
///// @addtogroup cand_compiler_data
///// @{
//#ifndef HEADER_GUARD_CALE_CAND_OFFICIAL_COMPILER_CAOCO_CPPGRAMMAR_H
//#define HEADER_GUARD_CALE_CAND_OFFICIAL_COMPILER_CAOCO_CPPGRAMMAR_H
//// Includes:
//#include "cxxx.hpp"
//
//#define CPP_CHARACTER_SET                                                    \
//  kEofChar, kWhitespaceChar, kNewlineChar, kPoundChar, kAddChar, kSubChar,   \
//      kMulChar, kDivChar, kModChar, kAndChar, kOrChar, kXorChar, kNotChar,   \
//      kLtChar, kGtChar, kEqChar, kLParenChar, kRParenChar, kLBraceChar,      \
//      kRBraceChar, kLBracketChar, kRBracketChar, kSemicolonChar, kColonChar, \
//      kCommaChar, kPeriodChar, kBacklashChar, kApostropheChar, kTildeChar,   \
//      kDoubleQuoteChar, kBacktickChar, kUnderscoreChar, kSingleQuoteChar
//
//#define CPP_KEYWORD_SYMBOLS_SET                                               \
//  kKwAlignas, kKwAlignof, kKwAsm, kKwAuto, kKwBool, kKwBreak, kKwCase,        \
//      kKwCatch, kKwChar, kKwChar8_t, kKwChar16_t, kKwChar32_t, kKwClass,      \
//      kKwConcept, kKwConst, kKwConsteval, kKwConstinit, kKwConst_cast,        \
//      kKwContinue, kKwCo_await, kKwCo_return, kKwCo_yield, kKwDecltype,       \
//      kKwDefault, kKwDelete, kKwDo, kKwDouble, kKwDynamic_cast, kKwElse,      \
//      kKwEnum, kKwExplicit, kKwExport, kKwExtern, kKwFalse, kKwFloat, kKwFor, \
//      kKwFriend, kKwGoto, kKwIf, kKwInline, kKwInt, kKwLong, kKwMutable,      \
//      kKwNamespace, kKwNew, kKwNoexcept, kKwNullptr, kKwOperator, kKwPrivate, \
//      kKwProtected, kKwPublic, kKwRegister, kKwReinterpret_cast, kKwRequires, \
//      kKwReturn, kKwShort, kKwSigned, kKwSizeof, kKwStatic, kKwStatic_assert, \
//      kKwStatic_cast, kKwStruct, kKwSwitch, kKwTemplate, kKwThis,             \
//      kKwThread_local, kKwThrow, kKwTrue, kKwTry, kKwTypedef, kKwTypeid,      \
//      kKwTypename, kKwUnion, kKwUnsigned, kKwUsing, kKwVirtual, kKwVoid,      \
//      kKwVolatile, kKwWchar_t, kKwWhile
//
//#define CPP_OPERATOR_OR_PUNCTUATOR_SYMBOLS_SET                                 \
//  kLBrace, kRBrace, kLBracket, kRBracket, kLParen, kRParen, kSemicolon,        \
//      kColon, kComma, kPeriod, kDoubleColon, kEllipsis, kLessThan,             \
//      kGreaterThan, kLessThanEqual, kGreaterThanEqual, kEqual, kNotEqual,      \
//      kSpaceship, kAdd, kSub, kMul, kDiv, kMod, kBitAnd, kBitOr, kBitXor,      \
//      kNot, kBitCompl, kLogicalAnd, kLogicalOr, kShiftLeft, kShiftRight, kInc, \
//      kDec, kAssign, kAddAssign, kSubAssign, kMulAssign, kDivAssign,           \
//      kModAssign, kBitAndAssign, kBitOrAssign, kBitXorAssign,                  \
//      kShiftLeftAssign, kShiftRightAssign, kQuestion, kArrow, kArrowStar,      \
//      kDotStar
//
//#define CPP_TRIGRAPH_SYMBOLS_SET                                            \
//  kTrigraphPound, kTrigraphBacklash, kTrigraphXor, kTrigraphNot,            \
//      kTrigraphBnot, kTrigraphLBracket, kTrigraphRBracket, kTrigraphLBrace, \
//      kTrigraph
//
//#define CPP_ALTERNATIVE_OPERATOR_SYMBOLS_SET                                \
//  kAltAnd, kAltAndEq, kAltBitand, kAltBitor, kAltCompl, kAltNot, kAltNotEq, \
//      kAltOr, kAltOrEq, kAltXor, kAltXorEq, kAltLBrace, kAltRBrace,         \
//      kAltLBracket, kAltRBracket, kAltPound, kAltDoublePound
//
//#define CPP_DIRECTIVE_SYMBOLS_SET                                         \
//  kDirectiveInclude, kDirectiveDefine, kDirectiveIf, kDirectiveElse,      \
//      kDirectiveElif, kDirectiveEndif, kDirectiveIfdef, kDirectiveIfndef, \
//      kDirectiveUndef
//
//#define CPP_INVISIBLE_SYMBOLS_SET                                        \
//  kEofile, kWhitespace, kNewline, kOpenBlockComment, kCloseBlockComment, \
//      kLineComment, kNONE, kINVALID
//
//#define CPP_SYMBOLS_SET                                               \
//  CPP_KEYWORD_SYMBOLS_SET, CPP_OPERATOR_OR_PUNCTUATOR_SYMBOLS_SET,    \
//      CPP_TRIGRAPH_SYMBOLS_SET, CPP_ALTERNATIVE_OPERATOR_SYMBOLS_SET, \
//      CPP_DIRECTIVE_SYMBOLS_SET, CPP_INVISIBLE_SYMBOLS_SET
//
//namespace caoco::cpp {
//using cxx::cstring;
//using std::optional;
//using std::string;
//using std::vector;
//
//enum class eCppSrcChar : size_t { CPP_CHARACTER_SET };
//enum class eCppSymbol : size_t { CPP_SYMBOLS_SET };
//
///// @see [ISO/IEC:N3797][lex.charset][1]
//static constexpr char kCppSrcChar(eCppSrcChar src_char) {
//#define CASE(c, v)        \
//  case eCppSrcChar::k##c: \
//    return v;
//  switch (src_char) {
//    CASE(EofChar, '\0');
//    CASE(WhitespaceChar, ' ');
//    CASE(NewlineChar, '\n');
//    CASE(HashChar, '#');
//    CASE(CommercialAtChar, '@');
//    CASE(AddChar, '+');
//    CASE(SubChar, '-');
//    CASE(MulChar, '*');
//    CASE(DivChar, '/');
//    CASE(ModChar, '%');
//    CASE(AndChar, '&');
//    CASE(OrChar, '|');
//    CASE(XorChar, '^');
//    CASE(NotChar, '!');
//    CASE(LtChar, '<');
//    CASE(GtChar, '>');
//    CASE(EqChar, '=');
//    CASE(LParenChar, '(');
//    CASE(RParenChar, ')');
//    CASE(LBraceChar, '{');
//    CASE(RBraceChar, '}');
//    CASE(LBracketChar, '[');
//    CASE(RBracketChar, ']');
//    CASE(SemicolonChar, ';');
//    CASE(ColonChar, ':');
//    CASE(CommaChar, ',');
//    CASE(PeriodChar, '.');
//    CASE(BacklashChar, '\\');
//    CASE(ApostropheChar, '\'');
//    CASE(TildeChar, '~');
//    CASE(DoubleQuoteChar, '"');
//    CASE(BacktickChar, '`');
//    CASE(UnderscoreChar, '_');
//    CASE(SingleQuoteChar, '\'');
//    default:
//      return '\0';
//  }
//#undef CASE
//}
//
//// @see Keywords [ISO/IEC:N49500][lex.key][1]
//// @see Keywords [ISO/IEC:N49500][lex.operators][1]
//// @see Operator or punctuator [ISO/IEC:N49500][lex.operators][1]
//// @see Trigraph [ISO/IEC:N49500][lex.trigraph][1]
//// @see Alternative Operator Representations [ISO/IEC:N3797][lex.key][2]
//static constexpr cstring kGetCppSymbolStr(eCppSymbol symbol) {
//#define CASE(sy, st)      \
//  case eCppSymbol::k##sy: \
//    return #st;
//  switch (symbol) {
//    // Keywords
//    CASE(KwAlignas, "alignas");
//    CASE(KwAlignof, "alignof");
//    CASE(KwAsm, "asm");
//    CASE(KwAuto, "auto");
//    CASE(KwBool, "bool");
//    CASE(KwBreak, "break");
//    CASE(KwCase, "case");
//    CASE(KwCatch, "catch");
//    CASE(KwChar, "char");
//    CASE(KwChar8_t, "char8_t");
//    CASE(KwChar16_t, "char16_t");
//    CASE(KwChar32_t, "char32_t");
//    CASE(KwClass, "class");
//    CASE(KwConcept, "concept");
//    CASE(KwConst, "const");
//    CASE(KwConsteval, "consteval");
//    CASE(KwConstinit, "constinit");
//    CASE(KwConst_cast, "const_cast");
//    CASE(KwContinue, "continue");
//    CASE(KwCo_await, "co_await");
//    CASE(KwCo_return, "co_return");
//    CASE(KwCo_yield, "co_yield");
//    CASE(KwDecltype, "decltype");
//    CASE(KwDefault, "default");
//    CASE(KwDelete, "delete");
//    CASE(KwDo, "do");
//    CASE(KwDouble, "double");
//    CASE(KwDynamic_cast, "dynamic_cast");
//    CASE(KwElse, "else");
//    CASE(KwEnum, "enum");
//    CASE(KwExplicit, "explicit");
//    CASE(KwExport, "export");
//    CASE(KwExtern, "extern");
//    CASE(KwFalse, "false");
//    CASE(KwFloat, "float");
//    CASE(KwFor, "for");
//    CASE(KwFriend, "friend");
//    CASE(KwGoto, "goto");
//    CASE(KwIf, "if");
//    CASE(KwInline, "inline");
//    CASE(KwInt, "int");
//    CASE(KwLong, "long");
//    CASE(KwMutable, "mutable");
//    CASE(KwNamespace, "namespace");
//    CASE(KwNew, "new");
//    CASE(KwNoexcept, "noexcept");
//    CASE(KwNullptr, "nullptr");
//    CASE(KwOperator, "operator");
//    CASE(KwPrivate, "private");
//    CASE(KwProtected, "protected");
//    CASE(KwPublic, "public");
//    CASE(KwRegister, "register");
//    CASE(KwReinterpret_cast, "reinterpret_cast");
//    CASE(KwRequires, "requires");
//    CASE(KwReturn, "return");
//    CASE(KwShort, "short");
//    CASE(KwSigned, "signed");
//    CASE(KwSizeof, "sizeof");
//    CASE(KwStatic, "static");
//    CASE(KwStatic_assert, "static_assert");
//    CASE(KwStatic_cast, "static_cast");
//    CASE(KwStruct, "struct");
//    CASE(KwSwitch, "switch");
//    CASE(KwTemplate, "template");
//    CASE(KwThis, "this");
//    CASE(KwThread_local, "thread_local");
//    CASE(KwThrow, "throw");
//    CASE(KwTrue, "true");
//    CASE(KwTry, "try");
//    CASE(KwTypedef, "typedef");
//    CASE(KwTypeid, "typeid");
//    CASE(KwTypename, "typename");
//    CASE(KwUnion, "union");
//    CASE(KwUnsigned, "unsigned");
//    CASE(KwUsing, "using");
//    CASE(KwVirtual, "virtual");
//    CASE(KwVoid, "void");
//    CASE(KwVolatile, "volatile");
//    CASE(KwWchar_t, "wchar_t");
//    CASE(KwWhile, "while");
//
//    // operator-or-punctuator
//    CASE(LBrace, "{");
//    CASE(RBrace, "}");
//    CASE(LBracket, "[");
//    CASE(RBracket, "]");
//    CASE(LParen, "(");
//    CASE(RParen, ")");
//    CASE(Semicolon, ";");
//    CASE(Colon, ":");
//    CASE(Comma, ",");
//    CASE(Period, ".");
//    CASE(DoubleColon, "::");
//    CASE(Ellipsis, "...");
//    CASE(LessThan, "<");
//    CASE(GreaterThan, ">");
//    CASE(LessThanEqual, "<=");
//    CASE(GreaterThanEqual, ">=");
//    CASE(Equal, "==");
//    CASE(NotEqual, "!=");
//    CASE(Spaceship, "<=>");
//    CASE(Add, "+");
//    CASE(Sub, "-");
//    CASE(Mul, "*");
//    CASE(Div, "/");
//    CASE(Mod, "%");
//    CASE(BitAnd, "&");
//    CASE(BitOr, "|");
//    CASE(BitXor, "^");
//    CASE(Not, "!");
//    CASE(BitCompl, "~");
//    CASE(LogicalAnd, "&&");
//    CASE(LogicalOr, "||");
//    CASE(ShiftLeft, "<<");
//    CASE(ShiftRight, ">>");
//    CASE(Inc, "++");
//    CASE(Dec, "--");
//    CASE(Assign, "=");
//    CASE(AddAssign, "+=");
//    CASE(SubAssign, "-=");
//    CASE(MulAssign, "*=");
//    CASE(DivAssign, "/=");
//    CASE(ModAssign, "%=");
//    CASE(BitAndAssign, "&=");
//    CASE(BitOrAssign, "|=");
//    CASE(BitXorAssign, "^=");
//    CASE(ShiftLeftAssign, "<<=");
//    CASE(ShiftRightAssign, ">>=");
//    CASE(Question, "?");
//    CASE(Arrow, "->");
//    CASE(ArrowStar, "->*");
//    CASE(DotStar, ".*");
//
//    // Trigraph Sequences
//    CASE(TrigraphPound, "??=");     // #
//    CASE(TrigraphBacklash, "??/");  // \ 
//    CASE(TrigraphXor= "??'");       // ^
//    CASE(TrigraphNot, "??!");       // |
//    CASE(TrigraphBnot, "??-");      // ~
//    CASE(TrigraphLBracket, "??(");  // [
//    CASE(TrigraphRBracket, "??)");  // ]
//    CASE(TrigraphLBrace, "??<");    // {
//    CASE(TrigraphLBrace, "??>");    // }
//
//    // Alternative Operator Representations
//    CASE(AltAnd, "and");
//    CASE(AltAndEq, "and_eq");
//    CASE(AltBitand, "bitand");
//    CASE(AltBitor, "bitor");
//    CASE(AltCompl, "compl");
//    CASE(AltNot, "not");
//    CASE(AltNotEq, "not_eq");
//    CASE(AltOr, "or");
//    CASE(AltOrEq, "or_eq");
//    CASE(AltXor, "xor");
//    CASE(AltXorEq, "xor_eq");
//    CASE(AltLBrace, "<%");
//    CASE(AltRBrace, "%>");
//    CASE(AltLBracket, "<:");
//    CASE(AltRBracket, ":>");
//    CASE(AltPound, "%:");
//    CASE(AltDoublePound, "%:%:");
//
//    CASE(DirectiveInclude, "#include");
//    CASE(DirectiveDefine, "#define");
//    CASE(DirectiveIf, "#if");
//    CASE(DirectiveElse, "#else");
//    CASE(DirectiveElif, "#elif");
//    CASE(DirectiveEndif, "#endif");
//    CASE(DirectiveIfdef, "#ifdef");
//    CASE(DirectiveIfndef, "#ifndef");
//    CASE(DirectiveUndef, "#undef");
//
//    // Special
//    CASE(Eofile, "\0");
//    CASE(Whitespace, " ");
//    CASE(Newline, "\n");
//    CASE(OpenBlockComment, "/*");
//    CASE(CloseBlockComment, "*/");
//    CASE(LineComment, "//");
//    default:
//      "";
//  }
//#undef CASE_SYMBOL_STRING
//}
//
///// Any symbol which is a single character operator or punctuator
///// that is not the first character of a multi-character operator,
///// punctuator, alternative representation,or trigraph sequence.
//static constexpr eCppSymbol kGetCppSrcCharSymbol(eCppSrcChar src_char) {
//  switch (src_char) {
//    using enum eCppSrcChar;
//    using enum eCppSymbol;
//    case kEofChar:
//      return kEofile;
//    case kWhitespaceChar:
//      return kWhitespace;
//    case kNewlineChar:
//      return kNewline;
//    case kLParenChar:
//      return kLParen;
//    case kRParenChar:
//      return kRParen;
//    case kLBraceChar:
//      return kLBrace;
//    case kRBraceChar:
//      return kRBrace;
//    case kLBracketChar:
//      return kLBracket;
//    case kRBracketChar:
//      return kRBracket;
//    case kSemicolonChar:
//      return kSemicolon;
//    default:
//      return kNONE;
//  }
//}
//
//struct CppToken {
//  eCppSymbol symbol;
//  const char* begin;
//  const char* end;
//  size_t line;
//  size_t col;
//};
//
//class CppLexer {
//  enum class eApiErr : size_t {
//    kInvalidStringLiteral,
//    kNoMatchingToken,
//  };
//  using SrcData = std::span<const char>;
//  using SrcDataConstIter = SrcData::const_iterator;
//  using TryLexResult =
//      std::expected<std::tuple<eCppSymbol, SrcDataConstIter>, eApiErr>;
//  using TryLexResultFail = TryLexResult::unexpected_type;
//  using LexResult = std::expected<vector<CppToken>, eApiErr>;
//
//  CppLexer(const string& in, vector<CppToken>& out)
//      : in_(in),
//        out_(out),
//        line_(0),
//        col_(0),
//        it_(in_.cbegin()),
//        start_it_(in_.cbegin()),
//        end_(in_.cend()),
//        begin_(in_.cbegin()) {}
//
// private:
//  SrcData in_;
//  vector<CppToken>& out_;
//  size_t line_;
//  size_t col_;
//  SrcDataConstIter it_;
//  SrcDataConstIter start_it_;
//  SrcDataConstIter end_;
//  SrcDataConstIter begin_;
//
//  constexpr optional<char> Peek(const SrcDataConstIter& it, int n) const {
//    if (std::distance(it_, end_) < n)
//      return std::nullopt;  // Out of bounds cant Peek
//    return *(it_ + n);
//  }
//  constexpr optional<char> SafeGet(SrcDataConstIter& it) const {
//    if (it >= end_) return std::nullopt;
//    if (it < begin_) return std::nullopt;
//    return *it;
//  }
//  /// No checks performed. Use with caution.
//  constexpr SrcDataConstIter& Advance(SrcDataConstIter& it, int n = 1) {
//    std::advance(it, n);
//    return it;
//  }
//  /// Searches forward for a complete match of characters. Starting from it,
//  /// inclusive.
//  constexpr bool FindForward(const SrcDataConstIter& it,
//                             std::string_view characters) const {
//    if (std::distance(it, end_) <
//        static_cast<std::ptrdiff_t>(characters.size()))
//      return false;  // Out of bounds cant match
//    auto end = std::next(it, static_cast<std::ptrdiff_t>(characters.size()));
//    if (std::equal(characters.cbegin(), characters.cend(), it, end))
//      return true;
//    return false;
//  }
//
//  constexpr void PushToken(eCppSymbol symbol) {
//    out_.push_back(CppToken{symbol, start_it_._Myptr, it_._Myptr, line_, col_});
//  }
//
//  constexpr bool TryLexSingularToken(char c) {
//    using enum eCppSymbol;
//    switch (c) {
//      case '{':
//        PushToken(kLBrace);
//        Advance(it_);
//        return true;
//      case '}':
//        PushToken(kRBrace);
//        Advance(it_);
//        return true;
//      case '[':
//        PushToken(kLBracket);
//        Advance(it_);
//        return true;
//      case ']':
//        PushToken(kRBracket);
//        Advance(it_);
//        return true;
//      case '(':
//        PushToken(kLParen);
//        Advance(it_);
//        return true;
//      case ')':
//        PushToken(kRParen);
//        Advance(it_);
//        return true;
//      case ';':
//        PushToken(kSemicolon);
//        Advance(it_);
//        return true;
//      case ',':
//        PushToken(kComma);
//        Advance(it_);
//        return true;
//      case '\n':
//        PushToken(kNewline);
//        Advance(it_);
//        return true;
//      case '\0':
//        PushToken(kEofile);
//        Advance(it_);
//        return true;
//      default:
//        return false;
//    }
//
//  }
//  constexpr bool TryLexCompoundOperatorOrPunctuator()
//
//
//  constexpr bool TryLexToken(const SrcDataConstIter& it) { 
//    auto first = *it;
//    if (cpp::IsOperatorOrPunctuator(first)) {
//      if (TryLexSingularToken(*it)) return true;
//      if (TryLexCompoundOperatorOrPunctuator(it)) return true;
//      
//    }
//    if (cpp::IsAlpha(first) || first == '_') {
//      // Identifier or keyword
//      if(TryLexIdentifierOrKeyword(it))return true;
//      if(TryLexStringLiteral) return true;
//    }
//
//    if (TryLexSingularToken(*it)) return true;
//    if (TryLexCompoundOperatorOrPunctuator(it)) return true;
//  }
//
//  LexResult Lex(const string& source) {
//    while (it_ != end_) {
//      LexMethodResult next_token = TryLexToken();
//      if (!next_token) {
//        // Handle error
//        return LexResult::unexpected_type{next_token.error()};
//      }
//      auto& [symbol, begin, end, start_from] = next_token.value();
//      out_.push_back({string(begin, end)});
//      it_ = start_from;
//    }
//  }
//};
//}  // namespace caoco::cpp
//
//#endif HEADER_GUARD_CALE_CAND_OFFICIAL_COMPILER_CAOCO_CPPGRAMMAR_H
///// @} // end of cand_compiler_data
//
/////////////////////////////////////////////////////////////////////////////////
//// @project: C& Programming Language Environment
//// @author(s): Anton Yashchenko
//// @website: https://www.acpp.dev
/////////////////////////////////////////////////////////////////////////////////
//// Copyright 2024 Anton Yashchenko
////
//// Licensed under the GNU Affero General Public License, Version 3.
//// you may not use this file except in compliance with the License.
//// You may obtain a copy of the License at
////
//// http://www.apache.org/licenses/LICENSE-2.0
////
//// Unless required by applicable law or agreed to in writing, software
//// distributed under the License is distributed on an "AS IS" BASIS,
//// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//// See the License for the specific language governing permissions and
//// limitations under the License.
/////////////////////////////////////////////////////////////////////////////////