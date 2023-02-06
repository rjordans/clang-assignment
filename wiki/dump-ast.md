# Dump the AST
The first tool to visualize the AST of a CPP code is `clang-check.exe.`
Typical and useful commands are:
> clang-check.exe --help 

To visualize the help. 

> clang-check file.cpp --ast-dump 

To visualize the AST of `file.cpp`.

>clang-check.exe file.cpp --ast-dump --ast-dump--filter="AString"

To visualize the AST around any identifier containing "AString" in its name.
