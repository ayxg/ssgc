//#pragma once
//#include "cnd_compiler.hpp"
//
//namespace cnd {
//
//
////namespace lexer {
////constexpr bool operator==(const eSrcChar& lhs, const char& rhs) {
////  return static_cast<char>(lhs) == rhs;
////}
////
////// <id> ::= <alpha> <alnumus>*
////// <keyword> ::= <keyword-id>
////// <string-literal-operator> ::= <id> <quotation> !(<quotation> <id>)
////// <quotation> <id>
////CND_CX LexRes LexIdentifier(SrcCursor crsr){
////  Size id_size = 0;
////  SrcCursor id_beg = crsr;
////  while (src_traits::IsAlnumus(crsr.Get())) {
////    id_size++;
////  }
////  crsr.Advance(id_size);
////  // Todo: optimization, if id_size is not any of keyword sizes, can skip 
////  // keyword check.
////  Str id = crsr.GetStrFrom(id_beg);
////  eTk etk = src_traits::KeywordOf(id);
////  if (etk != eTk::kNONE) {
////    Tk tk = etk;
////    tk.SetBeginLoc(id_beg);
////    tk.SetEndLoc(crsr);
////    return OffsetLexRes{tk, crsr};
////  }
////
////  // Check for string literal operator.
////  if (crsr.Get() == eSrcChar::kQuotation) {
////    crsr.Advance();
////    while (true) {
////      if (crsr.Get() == eSrcChar::kQuotation && crsr.Next().FindForward(id)) {
////          //end of literal
////          break;
////      }
////      crsr.Advance();
////      if (crsr.AtEof())
////        return ClFail(MakeClErr<kUnterminatedStringLiteral>());
////    }
////
////    Tk tk = eTk::kLitCstr;
////    tk.SetBeginLoc(id_beg);
////    tk.SetEndLoc(crsr);
////    return OffsetLexRes{tk, crsr};
////  }
////  
////  Tk tk = eTk::kIdent;
////  tk.SetBeginLoc(id_beg);
////  tk.SetEndLoc(crsr);
////  return OffsetLexRes{tk, crsr};
////
////}
////CND_CX LexRes LexBacktick(SrcCursor crsr);
////CND_CX LexRes LexSolidus(SrcCursor crsr);
////CND_CX LexRes LexQuotation(SrcCursor crsr);
////CND_CX LexRes LexNewline(SrcCursor crsr);
////CND_CX LexRes LexWhitespace(SrcCursor crsr);
////CND_CX LexRes LexEof(SrcCursor crsr);
////CND_CX LexRes LexNumber(SrcCursor crsr);
////CND_CX LexRes LexIdentifier(SrcCursor crsr);
////CND_CX LexRes LexDirective(SrcCursor crsr);
////CND_CX LexRes LexKeyword(SrcCursor crsr);
////CND_CX LexRes LexOperator(SrcCursor crsr);
////CND_CX LexRes LexScopes(SrcCursor crsr);
////CND_CX LexRes LexSemicolon(SrcCursor crsr);
////CND_CX LexRes LexColon(SrcCursor crsr);
////CND_CX LexRes LexComma(SrcCursor crsr);
////CND_CX LexRes LexPeriod(SrcCursor crsr);
////
////CND_CX ClRes<TkVec> Lex(const SrcLines& cand_src_lines,Size file_idx) {
////  // Check main conditions for an initial character of a token.
////  // Main categories are: whitespace, newlines , identifiers, numbers and
////  // symbols. Control characters are considered invalid unless newline or
////  // whitespace alternatives.
////  using namespace clerr;
////  using enum eClErr;
////  SrcCursor crsr{&cand_src_lines, file_idx};
////  TkVec out{};
////  while (crsr.NotAtEof()) {
////    SrcChar c = crsr.Get();
////    if (src_traits::IsNewline(c)) {
////      Tk tk = eTk::kNewline;
////      tk.SetBeginLoc(crsr.file,crsr.line, crsr.col);
////      crsr.Advance();
////      tk.SetEndLoc(crsr.file, crsr.line, crsr.col);
////      out.push_back(tk);
////    } 
////    else if (src_traits::IsSpace(c)) {
////      Tk tk = eTk::kWhitespace;
////      tk.SetBeginLoc(crsr.file, crsr.line, crsr.col);
////      crsr.Advance();
////      while (crsr.NotAtEof() && src_traits::IsSpace(c)) crsr.Advance();
////      tk.SetEndLoc(crsr.file, crsr.line, crsr.col);
////      out.push_back(tk);
////    } 
////    else {
////      LexRes res_buff;
////      if (src_traits::IsAlnumus(c)) 
////        res_buff = LexIdentifier(crsr);
////      else if (src_traits::IsNumeric(c)) 
////        res_buff = LexNumber(crsr);  
////      else if (src_traits::IsPunctuator(c)) 
////        res_buff = LexPunctuator(crsr); 
////      else 
////        return ClFail(MakeClErr<kUnknownSrcChar>(c));  // Unknown character.
////      
////      if (!res_buff) return ClFail(res_buff.error());
////      crsr.Advance(res_buff.value().offset);
////      out.push_back(std::move(res_buff.value().token));
////    }
////  }
////  return out;
////}
//
//}  // namespace lexer
//
//}  // namespace cnd
