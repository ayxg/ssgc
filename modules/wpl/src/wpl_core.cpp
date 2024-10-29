// clang-format off
#include "wpl_core.hpp"
// clang-format on

namespace WPLNS {

expected<unique_ptr<char[]>, eCastErr> WideToMByte(const wchar_t* ws) {
  if (ws == nullptr) return unexpected<eCastErr>(eCastErr::kNullArg);

  auto in_size = std::wcslen(ws);
  if (in_size == 0) return std::make_unique<char[]>(1);

  std::mbstate_t state;
  std::memset((void*)&state, 0, sizeof state);

  size_t mb_size = (in_size + 1) * 2;
  auto mb_str = std::make_unique<char[]>(mb_size);
  size_t chars_converted{};
  ::errno_t error =
      ::wcstombs_s(&chars_converted, mb_str.get(), mb_size, ws, _TRUNCATE);
  if (error != 0)
    return unexpected<eCastErr>(static_cast<eCastErr>(error));
  else
    return std::move(mb_str);
}

expected<unique_ptr<wchar_t[]>, eCastErr> MByteToWide(const char* ws) {
  if (ws == nullptr) return unexpected<eCastErr>(eCastErr::kNullArg);

  auto in_size = std::strlen(ws);
  if (in_size == 0) return std::make_unique<wchar_t[]>(1);

  std::mbstate_t state;
  std::memset((void*)&state, 0, sizeof state);
  size_t wide_size = in_size + 1;
  size_t chars_converted{};
  auto wide_str = std::make_unique<wchar_t[]>(wide_size);
  ::errno_t error =
      ::mbstowcs_s(&chars_converted, wide_str.get(), wide_size, ws, _TRUNCATE);
  if (error != 0)
    return unexpected<eCastErr>(static_cast<eCastErr>(error));
  else
    return std::move(wide_str);
}

expected<string, eCastErr> WideToMByte(const wstring_view& ws) {
  auto mbyte_str = WideToMByte(ws.data());
  if (mbyte_str.has_value())
    return string(mbyte_str.value().get());
  else
    return unexpected<eCastErr>(mbyte_str.error());
}

expected<wstring, eCastErr> MByteToWide(const string_view& mbs) {
  auto wide_str = MByteToWide(mbs.data());
  if (wide_str.has_value())
    return wstring(wide_str.value().get());
  else
    return unexpected<eCastErr>(wide_str.error());
}


/// @brief Given a string seperates it into a vector of string
/// by command line "\r\n", ommiting the "\r\n" from the result.
vector<string> GetCmdLines(const string& lines) {
  if (lines.empty()) return vector<string>{string{""}};  // empty string
  vector<string> ret{};
  string::const_iterator last_line_end = lines.cbegin();
  for (string::const_iterator i = last_line_end; i < lines.cend(); i++) {
    if (*i == '\r') {
      if ((i + 1) != lines.cend() && *(i + 1) == '\n') {
        ret.push_back(string{last_line_end, (i - 1)});
        i += 2;
        last_line_end = i;
      }
    }
  }
  if (last_line_end != lines.cend())
    ret.push_back({last_line_end, lines.cend()});
  return ret;
}

ApiRes<string> GetOsTempPath() {
  string buff;
  buff.reserve(MAX_PATH + 1);
  // Returns the length of the string copied to the buffer, not including the
  // terminating null character.
  if (0 == ::GetTempPath(MAX_PATH + 1, buff.data()))
    return ApiFail{eApiErr::kUnknown};
  return buff;
}

ApiRes<string> GetOsProgramDataPath() {
  string buff;
  buff.reserve(MAX_PATH + 1);
  if (::SHGetFolderPathA(NULL, CSIDL_COMMON_APPDATA, NULL, 0, buff.data()) !=
      S_OK)
    return ApiFail{eApiErr::kUnknown};
  return buff;
}

string WinErrMsg(win32::DWord err_num) {
  LPSTR messageBuffer = nullptr;
  size_t size = win32::FormatMessageA(
      FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
          FORMAT_MESSAGE_IGNORE_INSERTS,
      nullptr, err_num, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
      (LPSTR)&messageBuffer, 0, nullptr);
  string message(messageBuffer, size);
  win32::LocalFree(messageBuffer);
  return message;
}

win32::Bool ProcParams::DispatchCreateProcess() {
  return ::CreateProcess(
      target_path ? target_path->c_str() : NULL,
      command ? command->data() : NULL,
      proc_attrs.ptr ? proc_attrs.ptr.get() : nullptr,
      thread_attrs.ptr ? thread_attrs.ptr.get() : nullptr, inherit_handles,
      creation_flags, env_ptr.has_value() ? env_ptr.value() : NULL,
      init_dir && *init_dir != "" ? init_dir->c_str() : NULL,
      startup_info.get(), process_info.get());
}

ApiRes<void> CreateSubrocess(size_t init_delay, IoPipe& stdout_pipe,
                             IoPipe& stdin_pipe, IoPipe& stderr_pipe,
                             ProcParams& pr, win32::DWord& exit_code) {
  SecurityAttrs sa_attr{true /* true == INHERIT_HANDLES enabled */};
  auto* sa_attr_ptr = sa_attr.ptr.get();
  // Create pipe handles
  bool pipe_init = true;
  if (!stderr_pipe.Create(sa_attr_ptr)) pipe_init = false;
  if (!stderr_pipe.read_side.SetHandleInfo(HANDLE_FLAG_INHERIT))
    pipe_init = false;
  if (!stdout_pipe.Create(sa_attr_ptr)) pipe_init = false;
  if (!stdout_pipe.read_side.SetHandleInfo(HANDLE_FLAG_INHERIT))
    pipe_init = false;
  if (!stdin_pipe.Create(sa_attr_ptr)) pipe_init = false;
  if (!stdin_pipe.write_side.SetHandleInfo(HANDLE_FLAG_INHERIT))
    pipe_init = false;
  if (!pipe_init) return ApiFail{eApiErr::kInvalidPipe};

  if (!pr.startup_info) return ApiFail{eApiErr::kInvalidProccessParams};
  pr.startup_info->dwFlags = STARTF_USESTDHANDLES;
  pr.startup_info->hStdError = stderr_pipe.write_side.handle;
  pr.startup_info->hStdOutput = stdout_pipe.write_side.handle;
  pr.startup_info->hStdInput = stdin_pipe.read_side.handle;
  pr.inherit_handles = true;

  // Call CreateProcess using current proc params
  win32::ResultHandle proc_init = pr.DispatchCreateProcess();
  std::this_thread::sleep_for(std::chrono::milliseconds(init_delay));
  if (!pr.process_info) return ApiFail{eApiErr::kInvalidProccessParams};
  auto& proc_handle = pr.process_info->hProcess;
  if (!proc_init&& proc_init != S_OK) {  // Init failed, close handles.
    CloseHandle(pr.process_info->hThread);
    CloseHandle(proc_handle);
    TerminateProcess(proc_handle, 0);
    stdin_pipe.CloseHandles();
    stdout_pipe.CloseHandles();
    stderr_pipe.CloseHandles();
#ifndef NDEBUG
    assert(false &&
           "Implement this unknown error conversion to eApiErr using "
           "win32::GetLastError().");
#endif  // NDEBUG
    return ApiFail{eApiErr::kUnknown};
  }

  // TODO: Is this doing anything??
  // Confirm valid init in-case a required DLL cannot be located or fails to
  // initialize.
  if (GetExitCodeProcess(proc_handle, &exit_code) &&
      exit_code != STILL_ACTIVE) {
    WaitForSingleObject(proc_handle, win32::kInfinite);
    GetExitCodeProcess(proc_handle, &exit_code);
    //return ApiFail{eApiErr::kFailedProcess};
  }

  return ApiRes<void>{};
}

ApiRes<void> ReadPipeEnd(const IoPipeEnd& read_pipe_end, string& output_buffer,
                         win32::DWord read_limit) {
  // Ready to read input - read in small chunks until end-of-msg conditions
  // are met.
  win32::DWord total_bytes_read = 0;
  win32::DWord step_bytes_read = 0;
  string step_read_buffer = "";
  string output_read_stream = "";
  while (total_bytes_read < read_limit) {
    win32::DWord bytes_avail = 0;
    if (!read_pipe_end.Peek(nullptr, 0, nullptr, &bytes_avail, nullptr)) {
      auto win_error = GetLastError();
      if (win_error == ERROR_BROKEN_PIPE)  // Pipe closed.
        return ApiFail{eApiErr::kInvalidPipe};
      else if (win_error == ERROR_NO_DATA)
        break;  // No data left.
      else {
#ifndef NDEBUG
        assert(false &&
               "Implement this unknown error conversion to eApiErr using "
               "win32::GetLastError().");
#endif  // NDEBUG
        return ApiFail{eApiErr::kUnknown};
      }
    }

    if (!bytes_avail) break;

    win32::DWord bytes_left_to_read = read_limit - total_bytes_read;
    auto buffer_size =
        std::min(std::min(kIoStepBufferSize, bytes_avail), bytes_left_to_read);
    step_read_buffer.resize(buffer_size);
    if (!::ReadFile(read_pipe_end.handle, step_read_buffer.data(), buffer_size,
                    &step_bytes_read, nullptr) ||
        step_bytes_read == 0)
      break;

    total_bytes_read += step_bytes_read;
    output_read_stream.append(step_read_buffer);
  }
  output_buffer.append(output_read_stream);
  return ApiRes<void>{};
}

ApiRes<void> WritePipeEnd(const IoPipeEnd& write_pipe_end, string_view input) {
  if (!write_pipe_end.IsValid())  // Pipe not available to write
    return ApiFail{eApiErr::kInvalidPipe};

  // Return early if we aren't writing anything.
  if (input == "") return ApiRes<void>{};

  win32::DWord nbytes{0};
  win32::DWord total_bytes = input.size() > kWin32MaxIoBufferSize
                                 ? kWin32MaxIoBufferSize
                                 : static_cast<win32::DWord>(input.size());
  win32::DWord n_step_bytes = (total_bytes - nbytes) > kIoStepBufferSize
                                  ? kIoStepBufferSize
                                  : (total_bytes - nbytes);
  win32::DWord step_bytes_written = {0};
  string step_input = input.substr(0, n_step_bytes).data();
  size_t step_offset = 0;

  while (nbytes < total_bytes) {
    if (n_step_bytes > step_bytes_written) {
      win32::Bool write_result =
          WriteFile(write_pipe_end.handle, step_input.data(),
                    n_step_bytes - step_bytes_written, &step_bytes_written,
                    nullptr /* overlapped io disabled */);
      if (!write_result) {
#ifndef NDEBUG
        assert(false &&
               "Implement this unknown error conversion to eApiErr using "
               "win32::GetLastError().");
#endif  // NDEBUG
        return ApiFail{eApiErr::kUnknown};
      }
    }
    nbytes += step_bytes_written;
    step_offset += step_bytes_written;
    step_input = step_offset >= total_bytes
                     ? ""  // Reached end of input
                     : input.substr(step_offset, n_step_bytes);
    n_step_bytes = (total_bytes - nbytes) > kIoStepBufferSize
                       ? kIoStepBufferSize
                       : (total_bytes - nbytes);  // Update step bytes
    step_bytes_written = 0;                       // Reset written bytes.
  }
  return ApiRes<void>{};
}

ApiRes<RunExeResult> RunExe(win32::Str target, win32::Str command,
                            win32::Str init_dir, win32::DWord timeout) {
  RunExeResult ret{};
  ProcParams pr{};
  IoPipe stdin_pipe{};
  IoPipe stdout_pipe{};
  IoPipe stderr_pipe{};
  auto& proc_handle = pr.process_info->hProcess;
  LAMBDA xCloseHandles = [&]() {
    CloseHandle(pr.process_info->hThread);
    CloseHandle(proc_handle);
    stdin_pipe.CloseHandles();
    stdout_pipe.CloseHandles();
    stderr_pipe.CloseHandles();
  };

  if (init_dir.size() > 0) pr.init_dir = init_dir.data();
  win32::Str init_command = target.data();
  init_command += " ";
  init_command += command.data();
  pr.command = init_command;
  auto init_res = CreateSubrocess(0, stdout_pipe, stdin_pipe, stderr_pipe, pr,
                                  ret.exit_code);
  if (!init_res) {
    xCloseHandles();
    return ApiFail{init_res.error()};
  }
  if (GetExitCodeProcess(proc_handle, &ret.exit_code) &&
      ret.exit_code == STILL_ACTIVE) {
    WaitForSingleObject(proc_handle, timeout);
    TerminateProcess(proc_handle, 0);
    WaitForSingleObject(proc_handle, win32::kInfinite);
    GetExitCodeProcess(proc_handle, &ret.exit_code);
  }

  ret.out = string{""};
  auto read_res = ReadPipeEnd(stdout_pipe.read_side, ret.out.value());
  if (!read_res) {
    xCloseHandles();
    return ApiFail{read_res.error()};
  }
  if (ret.out.value().empty()) ret.out.reset();

  xCloseHandles();

  return ret;
}

//ApiRes<std::pair<bool, optional<vector<string>>>> WinWhere(wstring_view what) {
//  // @ref
//  // https://learn.microsoft.com/en-us/windows-server/administration/windows-commands/where
//  // /q	Returns an exit code (0 for success, 1 for failure)
//  //    without displaying the list of matched files.
//  auto where_exists = wpl::RunExe(L"where /q", what);
//  if (!where_exists) return ApiFail{where_exists.error()};
//
//  if (wpl::RunExe(L"where /q", what).value().exit_code != 0)
//    return ApiRes<std::pair<bool, optional<vector<string>>>>{
//        std::make_pair(false, std::nullopt)};
//
//  auto where_paths = wpl::RunExe(L"where", what);
//  auto& out_paths = where_paths.value().out.value();
//  vector<string> path_list{};
//  string_view newline = "\n";
//  for (auto eol_it = out_paths.begin(); eol_it < out_paths.end();) {
//
//  }
//  return ApiRes<std::pair<bool, optional<vector<string>>>>{
//      std::make_pair(true, path_list)};
//}

ApiRes<void> CmdShell::ReadFromStdout(
    string& buffer, win32::DWord poll_freq, win32::DWord read_limit,
    unique_ptr<ReadCondFunc> endread_cond) const {
  string ret_buff{};
  ApiRes<void> read_res = ReadPipeEnd(stdout_pipe_.read_side, ret_buff);
  if (!read_res) return read_res;
  if ((*endread_cond)(ret_buff)) {
    buffer = ret_buff;
    return ApiRes<void>();
  } else {
    bool endread_detected = false;
    string temp_buff{};
    while (!endread_detected) {
      std::this_thread::sleep_for(std::chrono::microseconds(poll_freq));
      ApiRes<void> read_res = ReadPipeEnd(stdout_pipe_.read_side, temp_buff);
      if (!read_res) return read_res;
      ret_buff += temp_buff;
      endread_detected = (*endread_cond)(ret_buff);
      temp_buff.clear();
    }
  }
  buffer = ret_buff;
  return ApiRes<void>();
}

ApiRes<void> CmdShell::Create(win32::DWord init_delay, bool wait_for_prompt) {
  // @TODO maybe use %comspec% ? COMSPEC or ComSpec is one of the environment
  // variables used in DOS, OS/2 and Windows, which normally points to the
  // command line interpreter.
  // Use wincon by default.
  if (!pr_.target_path) SetTargetPath("C:\\Windows\\System32\\cmd.exe");
  if (!pr_.target_path) SetInitCommand(win32::Str{"/k "});
  auto init_res = CreateSubrocess(init_delay, stdout_pipe_, stdin_pipe_,
                                  stderr_pipe_, pr_, exit_code_);
  if (!init_res) return init_res;

  if (wait_for_prompt) {
    string curr_prompt{GetCurrentPrompt()};
    string read_buff{};
    auto read_res = ReadFromStdout(
        read_buff, kDefaultIoPollFreq,
        win32::kInfinite,
        std::make_unique<ReadCondFunc>([&curr_prompt](string_view sb) {
          return sb.ends_with(curr_prompt);
        }));
    if (!read_res) return read_res;
  }

  return ApiRes<void>();
}

ApiRes<void> CmdShell::Close() {
  win32::DWord exit_code = 0;
  bool is_terminated = false;
  // Send exit command to try to terminate gracefully
  WriteToStdin("exit\r\n");

  if (!pr_.process_info) return ApiFail{eApiErr::kInvalidProccessParams};
  auto& proc_handle = pr_.process_info->hProcess;
  // Attempt getting the exit close. If open, force terminate, wait for exit
  // and attempt getting exit code again.
  if (GetExitCodeProcess(proc_handle, &exit_code) &&
      exit_code == STILL_ACTIVE) {
    TerminateProcess(proc_handle, 0);
    WaitForSingleObject(proc_handle, win32::kInfinite);
    GetExitCodeProcess(proc_handle, &exit_code);
    is_terminated = true;
  }
  exit_code_ = exit_code;

  CloseHandle(pr_.process_info->hThread);
  CloseHandle(proc_handle);
  stdin_pipe_.CloseHandles();
  stdout_pipe_.CloseHandles();
  stderr_pipe_.CloseHandles();
  // In case we didn't terminate above.
  if (!is_terminated) TerminateProcess(proc_handle, 0);
  closed_ = true;
  return ApiRes<void>();
}

ApiRes<string> CmdShell::SendMsg(string_view input) const {
  string ret{""};
  string curr_prompt = "\r\n" + GetCurrentPrompt();
  WriteToStdin(input.data());
  WriteToStdin("\r\n");
  ReadFromStdout(ret, 10, win32::kInfinite,
                 std::make_unique<ReadCondFunc>([&curr_prompt](string_view s) {
                   return s.ends_with(curr_prompt);
                 }));

  // Remove cmd prompts and echoed input from the response.
  while (ret.starts_with(curr_prompt)) ret.erase(0, curr_prompt.size());

  if (ret.starts_with(input.data())) ret.erase(0, input.size());

  while (ret.starts_with("\r\n")) ret.erase(0, 2);

  while (ret.ends_with(curr_prompt))
    ret.erase(ret.size() - curr_prompt.size(), curr_prompt.size());

  while (ret.ends_with("\r\n")) ret.erase(ret.size() - 2, 2);

  return ApiRes<string>(ret);
}

}  // namespace WPLNS
