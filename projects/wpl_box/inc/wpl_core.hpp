/// @file Windows Process Library Header
/// @brief Methods for running and interacting with process at runtime. Includes
///        CmdShell class for running an async cmd process with io capabilities.
#pragma once

// User of library may set the namespace by defining WPL_LIBRARY_NAMESPACE
#ifndef WPL_LIBRARY_NAMESPACE
#define WPL_LIBRARY_NAMESPACE wpl
#endif
#define WPLNS WPL_LIBRARY_NAMESPACE

// Prevent min/max macros from being defined by Windows headers
#define NOMINMAX

/// Code-style : label for lambdas, use xLambdaName pattern for the identifier.
#define LAMBDA auto

// win32
#include <Windows.h>
#include <process.h>
#include <shlobj_core.h> // SHGetFolderPath
// stl
#include <array>
#include <cassert>
#include <chrono>
#include <codecvt>
#include <compare>
#include <cwchar>
#include <expected>
#include <filesystem>
#include <format>
#include <functional>
#include <iostream>
#include <limits>
#include <locale>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <thread>
#include <vector>

namespace WPLNS {
using std::expected;
using std::optional;
using std::size_t;
using std::string;
using std::string_view;
using std::unexpected;
using std::unique_ptr;
using std::vector;
using std::wstring;
using std::wstring_view;
namespace stdfs = std::filesystem;

namespace win32 {
using ::FormatMessageA;
using ::GetLastError;
using ::LocalFree;
using ::ReadFile;

// Basic windows types. These should be defined in <minwindef.h> and <windef.h>
using DWord = unsigned long;        // DWORD
using Bool = int;                   // BOOL
using Byte = unsigned char;         // BYTE
using Word = unsigned short;        // WORD
using Float = float;                // FLOAT
using FloatPtr = Float far *;       // PFLOAT
using BoolNearPtr = Bool near *;    // PBOOL
using BoolPtr = Bool far *;         // LPBOOL
using ByteNearPtr = Byte near *;    // PBYTE
using BytePtr = Byte far *;         // LPBYTE
using IntNearPtr = int near *;      // PINT
using IntPtr = int far *;           // LPINT
using WordNearPtr = Word near *;    // PWORD
using WordPtr = Word far *;         // LPWORD
using LongPtr = long far *;         // LPLONG
using DWordNearPtr = DWord near *;  // PDWORD
using DWordPtr = DWord far *;       // LPDWORD
using VoidPtr = void *;             // LPVOID
using VoidConstPtr = const void *;  // LPCVOID
using CharPtr = char far *;         // LPSTR
using Char = char;
using Int = int;                     // INT
using UInt = unsigned int;           // UINT
using UIntNearPtr = unsigned int *;  // PUINT

using Handle = void *;  // HANDLE
using ResultHandle = HRESULT;

// Use wide string for Windows Unicode support.
#ifdef UNICODE
#define WPL_SYSSTR(x) L##x
static constexpr inline DWord kIsUnicode = true;
using CStr = const wchar_t *;
using Str = std::wstring;
using StrView = std::wstring_view;
using StrStream = std::wstringstream;
#else  // ANSI
#define WPL_SYSSTR(x) x
static constexpr inline DWord kIsUnicode = false;
using CStr = const char *;
using Str = std::string;
using StrView = std::string_view;
using StrStream = std::stringstream;
#endif  // UNICODE

// Alias windows types to conform to code style standards.
using ProcInfo = PROCESS_INFORMATION;
using StartupInfo = STARTUPINFO;
using SecurityDescriptor = SECURITY_DESCRIPTOR;
using SecurityAttrs = SECURITY_ATTRIBUTES;
using PseudoConsoleHandle = HPCON;

static constexpr inline ResultHandle kResultSOk = S_OK;
static constexpr inline DWord kInfinite = INFINITE;

}  // namespace win32

///////////////////////////////////////////////////////////////////////////////
// Synopsis
///////////////////////////////////////////////////////////////////////////////

enum class eApiErr : win32::DWord;
template <typename T>
using ApiRes = std::expected<T, eApiErr>;
using ApiFail = unexpected<eApiErr>;
struct SecurityAttrs;
struct ProcParams;
struct IoPipeEnd;
struct IoPipe;
struct RunExeResult;
class CmdShell;

///////////////////////////////////////////////////////////////////////////////
// Constants
///////////////////////////////////////////////////////////////////////////////

enum class eApiErr : win32::DWord {
  kUnknown,
  kInvalidPipe,
  kInvalidProccessParams,
  kFailedProcess,
};

enum class eCastErr : ::errno_t {
  kNullArg = EINVAL,
  kInvalidWchar = EILSEQ,
  kNotEnoughSpace = ERANGE,
  kInputLimitExceeded = E2BIG,
};

/// Maximum buffer size for a single ReadFile/WriteFile call.
static constexpr size_t kWin32MaxIoBufferSize =
    static_cast<size_t>(std::numeric_limits<win32::DWord>::max());

/// Buffer size WPL uses when calling ReadFile/Write file.
/// Value is set to a relatively small size so that any interruptions,
/// which may occur during an IO operation,are polled more frequently.
static constexpr win32::DWord kIoStepBufferSize = 128;

/// Default polling frequecy for continuous reads
static constexpr win32::DWord kDefaultIoPollFreq = 10;

///////////////////////////////////////////////////////////////////////////////
/// API
///////////////////////////////////////////////////////////////////////////////

// Character and string conversion methods.
// See impl details for setting the input string size and use:
// https://en.cppreference.com/w/c/string/multibyte/wcsrtombs
// https://learn.microsoft.com/en-us/archive/msdn-magazine/2015/july/c-using-stl-strings-at-win32-api-boundaries
// https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/wcsrtombs-s?view=msvc-170
// https://learn.microsoft.com/en-us/cpp/text/how-to-convert-between-various-string-types?view=msvc-170
expected<string, eCastErr> WideToMByte(const wchar_t *ws);
expected<wstring, eCastErr> MByteToWide(const char *ws);
expected<string, eCastErr> WideToMByte(const wstring_view &ws);
expected<wstring, eCastErr> MByteToWide(const string_view &mbs);

/// @brief Removes "\r\n" from the end of passed string until it does not end
/// with "\r\n".
constexpr void PopCmdEndline(string &s) {
  while (s.ends_with("\r\n")) s.erase(s.size() - 2, 2);
}
vector<string> GetCmdLines(const string &lines);

ApiRes<string> GetOsTempPath();
ApiRes<string> GetOsProgramDataPath();
win32::Str OpenFileDlg();
win32::Str OpenFolderDlg();
win32::Str SaveFileDlg();
win32::Str SaveFileDlg(win32::CStr init_dir);
    /// Get windows error as a string message.
string WinErrMsg(win32::DWord err_num);

///// Converts last win32 api error to a wpl api error. Does NOT set the
///// LastApiErr.
// eApiErr LastWinErrToApiErr();

ApiRes<void> CreateSubrocess(size_t init_delay, IoPipe &stdout_pipe,
                             IoPipe &stdin_pipe, IoPipe &stderr_pipe,
                             ProcParams &pr, win32::DWord &exit_code);

ApiRes<void> ReadPipeEnd(const IoPipeEnd &read_pipe_end, string &output_buffer,
                         win32::DWord read_limit = win32::kInfinite);

ApiRes<void> WritePipeEnd(const IoPipeEnd &write_pipe_end, string_view input);

ApiRes<RunExeResult> RunExe(win32::Str target, win32::Str command = "",
                            win32::Str init_dir = "",
                            win32::DWord timeout = win32::kInfinite);

// ApiRes<std::pair<bool, optional<vector<string>>>> WinWhere(wstring_view
// what);

///////////////////////////////////////////////////////////////////////////////
/// Decls
///////////////////////////////////////////////////////////////////////////////

struct SecurityAttrs {
  explicit SecurityAttrs(
      bool inherit_handle = false,
      unique_ptr<win32::SecurityDescriptor> &&sec_desc = nullptr) {
    ptr = std::make_unique<win32::SecurityAttrs>();
    ptr->nLength = sizeof(win32::SecurityAttrs);
    ptr->bInheritHandle = inherit_handle;
    ptr->lpSecurityDescriptor = security_descriptor.get();
    security_descriptor = std::move(sec_desc);
  }
  unique_ptr<win32::SecurityDescriptor> security_descriptor;
  unique_ptr<win32::SecurityAttrs> ptr;
};

struct ProcParams {
  optional<win32::Str> target_path{std::nullopt};
  optional<win32::Str> command{std::nullopt};
  SecurityAttrs proc_attrs{};
  SecurityAttrs thread_attrs{};
  win32::Bool inherit_handles{false};
  win32::DWord creation_flags{0};
  optional<win32::VoidPtr> env_ptr{nullptr};
  optional<win32::Str> init_dir{std::nullopt};
  unique_ptr<win32::ProcInfo> process_info{std::make_unique<win32::ProcInfo>()};
  unique_ptr<win32::StartupInfo> startup_info{[]() {
    auto p = std::make_unique<win32::StartupInfo>();
    auto *p_raw = p.get();
    ZeroMemory(&p_raw, sizeof(p_raw));  // reccomended by MSDN
    p->cb = sizeof(p_raw);              // must be set to sizeof(STARTUPINFO)
    return p;
  }()};

  /// Applies process startup parameters to win32::CreateProcess method.
  ///
  /// If the function succeeds, the return value is nonzero.
  /// If the function fails, the return value is zero. To get extended error
  /// information, call GetLastError.
  /// @note the function returns before the process has finished
  /// initialization. If a required DLL cannot be located or fails to
  /// initialize, the process is terminated. To get the termination status of a
  /// process, call GetExitCodeProcess. Source:
  /// https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-createprocessa
  win32::Bool DispatchCreateProcess();
};

struct IoPipeEnd {
  win32::Handle handle{INVALID_HANDLE_VALUE};
  bool IsValid() const { return handle != INVALID_HANDLE_VALUE; }
  win32::Bool SetHandleInfo(win32::DWord mask, win32::DWord flags = 0) const {
    return ::SetHandleInformation(handle, mask, flags);
  }
  // Not actually constant because handle is modified.
  win32::Bool CloseHandle() const { return ::CloseHandle(handle); }

  win32::Bool Peek(win32::CharPtr out_buffer, win32::DWord buffer_size,
                   win32::DWordPtr bytes_read, win32::DWordPtr bytes_avail,
                   win32::DWordPtr bytes_left_this_msg) const {
    return ::PeekNamedPipe(handle, out_buffer, buffer_size, bytes_read,
                           bytes_avail, bytes_left_this_msg);
  }
};

struct IoPipe {
  IoPipe() = default;

  bool Create(win32::SecurityAttrs *attrs, win32::DWord buffer_size = 0) {
    return ::CreatePipe(&read_side.handle, &write_side.handle, attrs,
                        buffer_size);
  }

  void CloseHandles() const {
    if (read_side.IsValid()) read_side.CloseHandle();
    if (write_side.IsValid()) write_side.CloseHandle();
  }

  IoPipeEnd read_side{};
  IoPipeEnd write_side{};
};

struct RunExeResult {
  win32::DWord exit_code;
  optional<string> out;
  optional<string> err;
};

class CmdShell {
 public:
  using ReadCondFunc = std::function<bool(string_view)>;

  CmdShell() = default;  // nothing is init.
  CmdShell(CmdShell &&other) = default;
  CmdShell(const CmdShell &other) = delete;  // dont copy.
  ~CmdShell() {
    if (!closed_) Close();
  }

  /// Initialize a cmd shell process.
  ApiRes<void> Create(win32::DWord init_delay = 10,
                      bool wait_for_prompt = true);

  /// Wait for up to timeout for process to exit, then force terminate.
  /// @return Stored in this->exit_code_ ,returned by cmd process.
  ApiRes<void> Close();

  ApiRes<void> WriteToStdin(string_view input) const {
    return WritePipeEnd(stdin_pipe_.write_side, input);
  }

  ApiRes<void> ReadFromStdout(
      string &buffer, win32::DWord poll_freq = kDefaultIoPollFreq,
      win32::DWord read_limit = {win32::kInfinite},
      unique_ptr<ReadCondFunc> endread_cond = nullptr) const;

  /// Send a message to the cmd shell process.
  ApiRes<string> SendMsg(string_view msg) const;

  // Properties

  /// Prompt is equal to "[working_dir_]>".
  string GetCurrentPrompt() const { return working_dir_.string() + ">"; }

  /// Path to cmd shell executable.
  /// @warning Changing the target may prevent a cmd session from launching.
  void SetTargetPath(win32::StrView target_path) {
    pr_.target_path = target_path;
  };

  /// Changes the current path, also affects the default SendMessage prompt end
  /// condition.
  void SetInitDir(win32::StrView dir_path) {
    pr_.init_dir = dir_path;
    working_dir_ = dir_path;
  };

  /// Commands passed with process init.
  /// @warning Changing the init command may make the class behave unexpectedly.
  void SetInitCommand(win32::StrView command) { pr_.command = command; };

  /// Creation flags passed with process init.
  /// @warning Changing the init command may make the class behave unexpectedly.
  void SetInitFlags(win32::DWord creation_flags) {
    pr_.creation_flags = creation_flags;
  };

 private:
  ProcParams pr_{};
  IoPipe stdin_pipe_{};
  IoPipe stdout_pipe_{};
  IoPipe stderr_pipe_{};
  stdfs::path working_dir_{stdfs::current_path()};
  bool closed_ = false;  // Closed() was called.
  win32::DWord exit_code_{0};
};

}  // namespace WPLNS
