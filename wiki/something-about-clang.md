# Something about clang and how we are going to use it
## What is clang?
Clang is a language front-end and tooling infrastructure for languages in the C language family.

On the official webpage ["Clang C Language Family Frontend"](https://clang.llvm.org/) it is said:
>If you are looking for source analysis or source-to-source transformation tools, Clang is probably a great solution for you.

That is precisely what we want to do, so let's take a look!

First of all, where to find information: many tutorials, informational blog pages, and videos are available on the internet. If you want to read some, I suggest the following pages:
- [Clang Developer documentation](https://clang.llvm.org/get_started.html)
- [Clang Internals](https://clang.llvm.org/docs/InternalsManual.html)
- [Clang Tutorial](https://kevinaboos.wordpress.com/2013/07/23/clang-tutorial-part-i-introduction/)
- [Tutorial for building tools using LibTooling and LibASTMatchers](https://intel.github.io/llvm-docs/clang/LibASTMatchersTutorial.html)

However, next, I will briefly introduce clang in a summary of all collected information, with the focus on what we need to be fully operational for building custom static analysis tools.

1) ### [First, we need to be sufficiently fluent in **a reasonably recent version of C++**](Recent-cpp.md)

2) ### [Then we need to know **how to write a clang tools**](how-to-write-clang-tools.md)

3) ### [We can spend a word on the **clang AST**](clang-ast.md)

4) ### [Finally, we need to know **how to navigate the AST**](navigate-the-ast.md)