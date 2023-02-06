## How to write a clang tools
Clang provides infrastructures to write tools that need syntactic and semantic information about a program.
You can find a detailed description of those infrastructures here: 
1. [Choosing the Right Interface for Your Application](https://clang.llvm.org/docs/Tooling.html)
2. [CLANG TUTORIAL PART I: INTRODUCTION](https://kevinaboos.wordpress.com/2013/07/23/clang-tutorial-part-i-introduction/)

Here we summarize the three available interfaces and highlight what we choose for our implementation: 
1. LibClang is a stable high-level C interface to clang.
However, it does not give you complete control over the Clang Abstract Syntax Tree (AST).
More information [here: Using libclang to Parse C++ (aka libclang 101)](https://shaharmike.com/cpp/libclang/).

2. Clang Plugins
allow you to run additional actions on the AST as part of a clang compilation. 
Plugins are dynamic libraries loaded at runtime by the compiler and easy to integrate into your build environment.
Clang Plugin Example can be found [here: Writing a PluginASTAction](https://opensource.apple.com/source/clang/clang-425.0.24/src/tools/clang/docs/ClangPlugins.html) and [here: Clang Plugins](https://clang.llvm.org/docs/ClangPlugins.html).
Please do not use them when you want to run tools outside of your build environment or 
need to run it over a specific subset of files in your project which is not necessarily related 
to any changes which would trigger rebuilds.

3. LibTooling allows building standalone tools integrated with clang or clang tools.
Examples of tools are syntax checkers and refactoring tools.
Use LibTooling when you want to run tools over a single file or a specific subset of files independently of the build system and need complete control over the Clang AST. 
You can find more information [here: LibTooling](https://clang.llvm.org/docs/LibTooling.html).
LibTooling is the interface we are going to use in our case, and we are going to have a couple of tutorials on it:
- [recursive visitors](ast-visitors)
- [matchers](matchers)
