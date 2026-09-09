#pragma once
#include <cstdlib>
namespace ssgc {
struct SourceRangeRaw {
  std::size_t file{};
  std::size_t begin{};
  std::size_t end{};

  constexpr std::size_t size() const { return end - begin; }
};
}  // namespace ssgc