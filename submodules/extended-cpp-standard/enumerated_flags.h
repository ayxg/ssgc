#pragma once
// Submodule includes.
#include "../meta-template-library/mta.h"
namespace cxx {
template <req::Enumeration EnumT, req::Integral_ UnderlyingT>
class EnumeratedFlags {
 public:
  UnderlyingT flags_{0};
  EnumeratedFlags() = default;
  EnumeratedFlags(EnumT flags) : flags_(0) { flags_ |= flags; }
  EnumeratedFlags(EnumT flags, std::same_as<EnumT> auto... other_flags)
      : flags_(flags) {
    (..., (flags_ |= other_flags));
  }
  UnderlyingT Get() const { return flags_; }
  constexpr operator EnumT() const { return static_cast<EnumT>(flags_); }
};
};  // namespace cxx
