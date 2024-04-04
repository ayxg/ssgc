#pragma once
#include <string_view>

namespace file_extension {
using std::string_view;
constexpr string_view kCaIdeSettings = ".caide";
constexpr string_view kCaSolution = ".casln";
constexpr string_view kCaBuild = ".camake";
constexpr string_view kCaSource = ".cand";
constexpr string_view kCaHeader = ".candi";
constexpr string_view kCailCode = ".cail";
constexpr string_view kCppSource = ".cpp";
constexpr string_view kCppHeader = ".hpp";
constexpr string_view kCSource = ".c";
constexpr string_view kCHeader = ".h";
namespace wide {
constexpr const wchar_t* kCaIdeSettings = L".caide";
constexpr const wchar_t* kCaSolution = L".casln";
constexpr const wchar_t* kCaBuild = L".camake";
constexpr const wchar_t* kCaSource = L".cand";
constexpr const wchar_t* kCaHeader = L".candi";
constexpr const wchar_t* kCailCode = L".cail";
constexpr const wchar_t* kCppSource = L".cpp";
constexpr const wchar_t* kCppHeader = L".hpp";
constexpr const wchar_t* kCSource = L".c";
constexpr const wchar_t* kCHeader = L".h";
}  // namespace wide
}  // namespace file_extension
