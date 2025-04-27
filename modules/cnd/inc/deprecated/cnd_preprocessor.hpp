///// Preprocessor
// namespace preprocessor {
// struct TrFrag;
// struct TrUnit;
// ClRes<void> ProcessInclude(const Tk& tokens, TrFrag& tf, const TrFrag & pre);
// ClRes<void> ProcessDefine(const Tk& tokens, TrFrag& tf, const TrFrag & pre);
// ClRes<void> ProcessUndef(const Tk& tokens, TrFrag& tf, const TrFrag & pre);
// ClRes<void> ProcessIf(const Tk& tokens, TrFrag& tf, const TrFrag & pre);
// ClRes<void> ProcessIfdef(const Tk& tokens, TrFrag& tf, const TrFrag & pre);
// ClRes<void> ProcessIfndef(const Tk& tokens, TrFrag& tf, const TrFrag & pre);
// ClRes<void> ProcessElif(const Tk& tokens, TrFrag& tf, const TrFrag & pre);
// ClRes<void> ProcessElse(const Tk& tokens, TrFrag& tf, const TrFrag & pre);
// ClRes<void> ProcessEndif(const Tk& tokens, TrFrag& tf, const TrFrag & pre);
// ClRes<void> ProcessPragma(const Tk& tokens, TrFrag& tf, const TrFrag & pre);
// ClRes<void> ProcessError(const Tk& tokens, TrFrag& tf, const TrFrag & pre);
// ClRes<void> ProcessLine(const Tk& tokens, TrFrag& tf, const TrFrag & pre);
// TrUnit Preprocess(const TkVec& tokens,const TrFrag& pre);
// }  // namespace preprocessor