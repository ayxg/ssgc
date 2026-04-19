@echo off

call "C:\Program Files\Microsoft Visual Studio\18\Community\Common7\Tools\VsDevCmd.bat" -no_logo

cmake --preset %1
if errorlevel 1 exit /b %errorlevel%

cmake --build out/build/%1 --parallel
exit /b %errorlevel%