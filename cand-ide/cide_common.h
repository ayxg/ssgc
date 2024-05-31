#pragma once
#include <filesystem>
#include <string>
#include <vector>
#include <functional>
#include <string_view>

// C++ Standard Extended
#include "cppsextended.h"

// Minitest Framework - for cpp test explorer
#include "minitest.h"

namespace cide {

namespace stdfs = std::filesystem;

using std::size_t;
using std::function;
using std::string;
using std::vector;
using std::string_view;

using cstring = const char*;
using wcstring = const wchar_t*;

// The lambdas and typedefs below are default values for callbacks 
// from frontend.
// They are used to avoid null checks in the backend code.
static constexpr LAMBDA xNullVoidCallback = []() {};
using VoidCallbackT = function<void(void)>;

// Boolean callback should be used when an error may occur.
// The callback should return false if an error occurred.
static constexpr LAMBDA xNullBoolCallback = []() { return false; };
using BoolCallbackT = function<bool(void)>;

// Defines file extensions used by the IDE.
namespace file_extension {
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
constexpr wcstring kCaIdeSettings = L".caide";
constexpr wcstring kCaSolution = L".casln";
constexpr wcstring kCaBuild = L".camake";
constexpr wcstring kCaSource = L".cand";
constexpr wcstring kCaHeader = L".candi";
constexpr wcstring kCailCode = L".cail";
constexpr wcstring kCppSource = L".cpp";
constexpr wcstring kCppHeader = L".hpp";
constexpr wcstring kCSource = L".c";
constexpr wcstring kCHeader = L".h";
}  // namespace wide
}  // namespace file_extension



}  // namespace cide::backend
