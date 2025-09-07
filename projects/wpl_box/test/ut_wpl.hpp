#include "minitest.hpp"
#include "wpl_core.hpp"

TEST(WplCliSession, SendMessagePing) {
  wpl::CmdShell cmd_sesh{};
  std::string read_buffer{};
  EXPECT_TRUE(wpl::ApiRes<void>{} == cmd_sesh.Create(10));
  EXPECT_TRUE(
      cmd_sesh.SendMsg("ping localhost")
          .value()
          .starts_with("Pinging DESKTOP-KSJHG2V [::1] with 32 bytes of data:"));
}


TEST(WplRunExe, SimpleCommand) {
  // @note the use of /c to run the cmd.exe in close mode.
  // You may also call cmd or cmd.exe, system PATHS are searched for files.
  auto run_res =
      wpl::RunExe("C:\\Windows\\System32\\cmd.exe", "/c echo ILoveCpp");
  if (!run_res) {
    std::wcout << L"Error:" << std::to_underlying(run_res.error()) << std::endl;
  } else {
    EXPECT_TRUE(run_res.value().exit_code == 0);
    EXPECT_TRUE(run_res.value().out.value_or("") == "ILoveCpp\r\n");
  }
}


TEST(WplCliSession, WriteToStdin) {
  wpl::CmdShell cmd_sesh;
  std::string read_buffer{};
  auto curr_path = wpl::stdfs::current_path().string();
  std::string expected_answer = "hello";

  EXPECT_TRUE(wpl::ApiRes<void>{} == cmd_sesh.Create(10));
  EXPECT_TRUE("hello" == cmd_sesh.SendMsg("echo hello"));
  EXPECT_TRUE(wpl::ApiRes<void>{} == cmd_sesh.Close());
}


TEST(WplCliSession, WriteToStdinLargeString) {
  // Test writing a string which exceed the input buffer step
  // size, usually a small amount.
  // Find the actual size in : wpl::CmdSession::kIoStepBufferSize
  auto input = "echo " + std::string(1024, 'x');
  wpl::CmdShell cmd_sesh;
  std::string read_buffer{};
  auto expected_answer = std::string(1024, 'x');

  EXPECT_TRUE(wpl::ApiRes<void>{} == cmd_sesh.Create(10));
  EXPECT_TRUE(expected_answer == cmd_sesh.SendMsg(input));
}


TEST(WplRunExe, CidrLoad) {
  // Find vswhere executable.
  // Default path: vswhere is included with the installer as of Visual Studio
  //  2017 version 15.2 and later, and can be found at the following
  // location:
  //  %ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe
  // Alternative: Use "winget install vswhere --nointeractivity"
  auto vswhere_path_res = wpl::RunExe("where", "vswhere");
  if (!vswhere_path_res) throw "Could not find vswhere.";
  auto& vswhere_path = vswhere_path_res.value();

  // Remove "\r\n"
  vswhere_path.out.value().pop_back();
  vswhere_path.out.value().pop_back();
  // Find location of latest visual studio install folder
  // Details: https://github.com/microsoft/vswhere/wiki/Find-VC
  auto msvs_path_res = wpl::RunExe(
      "vswhere", "-latest -nocolor -utf8 -property installationPath");
  if (!msvs_path_res) throw "Could not find MSVC install paths.";
  auto& msvs_path = msvs_path_res.value();
  // Remove "\r\n"
  msvs_path.out.value().pop_back();
  msvs_path.out.value().pop_back();

  // Find vsdevcmd.bat path
  wpl::stdfs::path expected_vsdevcmd_path =
      wpl::stdfs::path(msvs_path.out.value())
          .append("Common7\\Tools\\VsDevCmd.bat");
  std::wstring vsdevcmd_path{};
  if (wpl::stdfs::exists(expected_vsdevcmd_path))
    vsdevcmd_path = expected_vsdevcmd_path.native();

  //// Try compiling a C++ source file with msvc.
  //auto compile_res = wpl::RunExe(
  //    L"C:\\Windows\\System32\\cmd.exe",
  //    L"/c \"\"" + vsdevcmd_path + L"\" && cl test_main_file.cpp\"");
  //if (!compile_res) throw "Failed to run C++ compiler.";
  //auto& compile_result = compile_res.value();

  //std::cout << std::format("[MSVC][Exit Code]:{}\n[MSVC][Stdout]:{}",
  //                         compile_result.exit_code, compile_result.out.value())
  //          << std::endl;
}
