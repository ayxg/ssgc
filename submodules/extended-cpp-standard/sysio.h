#pragma once
// Brief: Filesystem I/O utilities.
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "macrodef.h"
//namespace cxx {
//// Method: LoadFileToChars
//// Parameters:
////		1.name: The name of the file to load.
//// Returns:
////		Contents of the file as a vector of chars
////    If the last char is not a null terminator, adds one.
//static std::vector<char> LoadFileToChars(const std::string& name) {
//  std::ifstream ifs(name, std::ios::binary | std::ios::ate);
//
//  if (!ifs)
//    throw std::runtime_error(name +
//                             ": LoadFileToU8Vec : File Could not be opened.");
//
//  auto end = ifs.tellg();
//  ifs.seekg(0, std::ios::beg);
//
//  auto size = std::size_t(end - ifs.tellg());
//
//  if (size == 0)  // avoid undefined behavior due to zero size.
//    return {};
//
//  std::vector<std::byte> buffer(size);
//
//  if (!ifs.read((char*)buffer.data(), buffer.size()))
//    throw std::runtime_error(name +
//                             ": LoadFileToU8Vec : File Could not be read.");
//
//  std::vector<char> chars;
//  chars.reserve(buffer.size());
//  for (auto& c : buffer) {
//    chars.push_back(static_cast<char>(c));
//  }
//
//  if (chars.back() != '\0') chars.push_back('\0');
//
//  return chars;
//}
//};  // namespace cxx