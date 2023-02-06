## Clang AST: 

Clang AST is very complex and can change in time. 
The aim is not to learn it by heart, but lear to read it and navigate it with the appropriate tools. 
A good introduction to the AST can be found here:
- [Introduction to the Clang AST](https://clang.llvm.org/docs/IntroductionToTheClangAST.html)
- [Understanding the Clang AST](https://jonasdevlieghere.com/understanding-the-clang-ast/)

Below we summarize the main three parts that you need to know to navigate the AST:
1. The `ASTContext`, which keeps information around the AST
    - Identifier Table 
    - Source Manager
    - Long-lived AST nodes: types and declarations
    - Entry point into the AST

    Examples of usage of the `ASTContext`:
    - get the parent of a statement:
    > auto parents = Context->getParents(*statement);
    - Get the entry point of the AST:
    > Context-> getTranslationUnitDecl();
    - Get information about the location in the file of a given expression:
    > expr->getExprLoc().printToString(Context->getSourceManager())


2. `Core classes`
    - Decl
        - CxxRecodDecl
        - VarDecl
        - ...
    - Statements
        - CompoundStmt
        - BinaryOperator
        - ...
    - Types (Complicated ... out of this initial overview) 


3. `Glue Methods` 
    - IfStmt: getThen(), getElse(), getCond() 
    - CXXRecordDecl: getDescribedClassTemplate() 
    - Type: getAsCXXRecordDecl()
    - ...

However, clang AST is a rich representation, with fully type resolved and more than 100k lines of code. One should refer to the [online documentation](https://clang.llvm.org/doxygen/index.html) and use [available tools for the AST navigation](navigate-the-ast).