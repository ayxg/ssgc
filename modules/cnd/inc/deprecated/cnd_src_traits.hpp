#pragma once
#include "cnd_implicit.hpp"
#include "cnd_constdef.hpp"

namespace cnd {
namespace src_traits {
CND_CX bool IsAlpha(char c);
CND_CX bool IsNumeric(char c);
CND_CX bool IsUnderscore(char c);
CND_CX bool IsAlphanumeric(char c);
CND_CX bool IsAlphaUnderscore(char c);
CND_CX bool IsAlnumus(char c);
CND_CX bool IsPunctuator(char c);
CND_CX bool IsSpace(char c);
CND_CX bool IsNewline(char c);
CND_CX bool IsWhitespace(char c);
CND_CX bool IsControl(char c);
CND_CX bool IsValid(char c);

CND_CX eAssoc TkAssoc(eTk t);
CND_CX eOperation TkOperation(eTk t);
CND_CX ePriority TkPriority(eTk t);
CND_CX CStr TkLiteral(eTk t);
CND_CX bool TkIsKeyword(eTk t);
CND_CX bool TkIsLScope(eTk t);
CND_CX bool TkIsRScope(eTk t);
CND_CX bool TkIsRScopeOf(eTk topen, eTk tclose);
CND_CX bool TkIsModifier(eTk t);
CND_CX bool TkIsDeclarative(eTk t);
CND_CX bool TkIsAnOperand(eTk t);
CND_CX bool TkIsAPrefixOperator(eTk t);
CND_CX bool TkIsPrimary(eTk t);
CND_CX bool TkIsPragmatic(eTk t);

CND_CX eAssoc AstAssoc(eCst t);
CND_CX eOperation AstOperation(eCst t);
CND_CX ePriority AstPriority(eCst t);
CND_CX CStr AstLiteral(eCst t);
CND_CX bool AstIsKeyword(eCst t);
CND_CX bool AstIsLScope(eCst t);
CND_CX bool AstIsRScope(eCst t);
CND_CX bool AstIsRScopeOf(eCst topen, eCst tclose);
CND_CX bool AstIsModifier(eCst t);
CND_CX bool AstIsDeclarative(eCst t);
CND_CX bool AstIsAnOperand(eCst t);
CND_CX bool AstIsAPrefixOperator(eCst t);
CND_CX bool AstIsPrimary(eCst t);
CND_CX bool AstIsPragmatic(eCst t);

CND_CX eTk KeywordToTk(CStr kw);

}  // namespace src_traits
}

#include "cnd_src_traits.tpp"

