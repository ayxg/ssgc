#pragma once
#include <cstdlib>
namespace ssgc {
struct SourceLocation {
  std::size_t line{};
  std::size_t col{};
};
}  // namespace ssgc