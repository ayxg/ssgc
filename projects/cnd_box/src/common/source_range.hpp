#pragma once
#include <cstdlib>
#include "source_location.hpp"

namespace ssgc {
struct SourceRange {
  std::size_t file{};
  SourceLocation begin{};
  SourceLocation end{};
};
}  // namespace ssgc