# In-house logging library

## Preparation
In `code\AFakeLoggingFramework`, make a directory that will contain the build artifacts

`mkdir build`

from within build run cmake
`cmake ..`

Now, you can compile your project via `make` or via `VisualStudio`.

## Generate the compilation database
on Unix

`cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 /path/to/src`

on Windows use [Clang Power tools](https://clangpowertools.com/)

The content of the database should be something as follows, containing the command to compile the file with all the needed includes and macros. 

```
[
  {
    "directory": "path/to/code/AFakeLoggingFramework/build/",
    "command": "\"clang++.exe\" -x c++ \"path/to/code/AFakeLoggingFramework/AFakeLoggingFramework/AFakeLoggingFramework.cpp\" -std=c++14 -Wall -fms-compatibility-version=19.10 -Wmicrosoft -Wno-invalid-token-paste -Wno-unknown-pragmas -Wno-unused-value -fsyntax-only \"-D_MT\" \"-D_DLL\" \"-DWIN32\" \"-D_WINDOWS\" \"-DCMAKE_INTDIR=/\"Debug/\"\" \"-D_DEBUG_FUNCTIONAL_MACHINERY\" -isystem\"C:/VS2019/VC/Tools/MSVC/14.28.29910/include\" -isystem\"C:/VS2019/VC/Tools/MSVC/14.28.29910/atlmfc/include\" -isystem\"C:/VS2019/VC/Auxiliary/VS/include\" -isystem\"C:/Program Files (x86)/Windows Kits/10/Include/10.0.19041.0/ucrt\" -isystem\"C:/Program Files (x86)/Windows Kits/10/Include/10.0.19041.0/um\" -isystem\"C:/Program Files (x86)/Windows Kits/10/Include/10.0.19041.0/shared\" -isystem\"C:/Program Files (x86)/Windows Kits/10/Include/10.0.19041.0/winrt\" -isystem\"C:/Program Files (x86)/Windows Kits/10/Include/10.0.19041.0/cppwinrt\"",
    "file": "path/to/code/AFakeLoggingFramework/AFakeLoggingFramework/AFakeLoggingFramework.cpp"
  }
]
```

Copy the compilation database in `code\AFakeLoggingFramework`.

## Run the code
Run the executable and observe how many logging messages are printed on the screen.


