extern["C"]callconv["__stdcall"] fn
    @WriteFile(void ptr hFile, const void ptr lpBuffer,
               uint nNumberOfBytesToWrite, uint ptr lpNumberOfBytesWritten,
               void ptr lpOverlapped) > int;

extern "C" __stdcall void * GetStdHandle(unsigned long nStdHandle);

extern "C" __stdcall" GetLastError() > uint;

.macrodef STD_INPUT_HANDLE(-10 as uint)
    .macrodef EXIT_FAILURE(1 as uint)
    .macrodef EXIT_SUCCESS(0 as uint)