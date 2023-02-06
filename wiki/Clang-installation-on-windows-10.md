In the last years, Clang has made an effort to be easy to use and available for Windows users. It is possible to [install clang](https://docs.microsoft.com/en-us/cpp/build/clang-support-msbuild?view=msvc-170) and [clang power Tools](https://clangpowertools.com/) on Visual Studio 2019 in few clicks.

However, we want to build clang from the source because we need to construct clang-based tools. Don't worry, though. That is just a few commands and clicks.
There are two tutorials on how to do that, [tutorial 1](http://www.justiceadams.com/blog/2019/8/26/a-guide-to-hacking-on-llvmclang-with-visual-studio-windows-2019-edition#:~:text=Build%20LLVM%20with%20Visual%20Studio%20Your%20build%20folder,%E2%80%9CBuild%E2%80%9D%20to%20build%20all%20of%20the%20LLVM%20projects.) and [tutorial 2](https://phasetw0.com/llvm/getting-started-on-windows/). The procedure is roughly the same:

1. Clone clang and llvm projects: 
```
git clone https://github.com/llvm/llvm-project.git
```
2. Check that your visual studio has support for CMake or [install it from Visual Studio Installer.](https://docs.microsoft.com/en-us/cpp/build/cmake-projects-in-visual-studio?view=msvc-170)
3. Open visual studio and from the `Developer PowerShell` run:
```
cd <CODE>\llvm-project
mkdir build
cd build
cmake -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra" -A x64 -Thost=x64 ..\llvm
``` 
Clang and clang-tools-extra are projects within the llvm source tree, and the other options specify which target the compilation sets.  CMake generates a visual studio solution file called `LLVM.sln` in the' build' directory. Please open it and compile the solution with visual studio. Compiling in debug mode is longer and takes more space, but then you can also link your tools to this library and build it in debug mode, which for development can be pretty handy.

4. Take a long cup of coffee or do this in the evening before sleep :) Now you are all set.