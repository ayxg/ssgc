#pragma once
#include "cnd_implicit.hpp"
#include "cnd_constdef.hpp"

// Class Defs
struct SrcCursor {
  CND_CX Bool AtEof() const;
  CND_CX Bool NotAtEof() const;
  CND_CX char Get() const;
  CND_CX char GetUnchecked() const;
  CND_CX char Peek(int n) const;
  CND_CX Bool FindForward(std::string_view characters) const;
  CND_CX SrcCursor& Advance(int n = 1);
  CND_CX SrcCursor Next(int n = 1);
  CND_CX Str GetStrTo(const SrcCursor& end) const;
  CND_CX Str GetStrFrom(const SrcCursor& beg) const;

  const SrcLines* src{};
  Size file{};
  Size line{};
  Size col{};
};