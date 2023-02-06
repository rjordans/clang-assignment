# Recursive AST visitors
To describe this way of navigating the AST, we will use an example of a custom static analysis stool. This tool finds all the string literal in the input AST, and its code can be found [here](TODO). 

We will not describe how to compile and link this code to clang. For such a description, refer to the [README](TODO) of the code.

First, we create an option category to contain the options of our tool (if any).

```cpp
static llvm::cl::OptionCategory MyToolCategory("my-tool options");
llvm::cl::opt<bool> MyOption("myOption", 
                            llvm::cl::desc("description of myOption"), 
                            llvm::cl::cat(MyToolCategory));
```

More information on how to deal with user-defined options can be found [here: CommandLine 2.0 Library Manual](https://releases.llvm.org/8.0.0/docs/CommandLine.html).

In the main, we create an option parser and a tool that takes the parsed options: compilation settings and files.
Finally, we run the tool with a specific frontend action. In our case `PrintStringAction`
```cpp
int main(int argc, const char** argv) {
    auto ExpectedParser = clang::tooling::CommonOptionsParser::create(argc, argv, MyToolCategory);
    if (!ExpectedParser) {
        llvm::errs() << ExpectedParser.takeError();
        return 1;
    }
    llvm::outs().flush();
    clang::tooling::CommonOptionsParser& OptionsParser = ExpectedParser.get();
    
    clang::tooling::ClangTool Tool(OptionsParser.getCompilations(),
                                   OptionsParser.getSourcePathList());
    return Tool.run(clang::tooling::newFrontendActionFactory<PrintStringAction>().get());

}
```
The frontend action derives from `ASTFrontendAction` and must redefine a virtual method `CreateASTConsumer.` This method creates an ASTConsumer that, in our case, is called `PrintStringConsumer.`
```cpp
class PrintStringAction : public clang::ASTFrontendAction {
public:
    virtual std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
        clang::CompilerInstance& Compiler, llvm::StringRef InFile) override {
        return std::make_unique<PrintStringConsumer>(&Compiler.getASTContext());
    }
};
```
The `PrintStringConsumer` allocates a Visitor of type `PrintString` that takes the `ASTContext` as a parameter of its constructor. Then `PrintStringConsumer`, in a virtual method called `HandleTranslationUnit` that it must override, starts the visit form the AST entry point `Context.getTranslationUnitDecl()`.

```cpp
class PrintStringConsumer : public clang::ASTConsumer {
public:
    explicit PrintStringConsumer(clang::ASTContext* Context)
        : Visitor(Context) {}

    virtual void HandleTranslationUnit(clang::ASTContext& Context) override {
        Visitor.TraverseDecl(Context.getTranslationUnitDecl());
    }
private:
    PrintString Visitor;
};
```

This code is all boilerplate code to link our tool with the clang and llvm environment. In the `PrintString` visitor, we encounter the actual processing to detect the string literal and print them out. 
Here it is crucial to notice that:
- we use the CRTP for the definition of `PrintString;`
- the visitor visiting the string literal must be called `VisitStringLiteral,` including the string `Visit` and the name of the node it will need. It is thus essential to dump the AST of the code to be analyzed to get the node's name to visit. 
The visitor returns false to stop the traversal of the AST. 
```cpp
class PrintString : public clang::RecursiveASTVisitor<PrintString> {
public:
    PrintString(clang::ASTContext* Context)
        : Context(Context) {}

    bool VisitStringLiteral(const clang::StringLiteral* stmt)
    {
        auto location = stmt->getExprLoc()
            .printToString(Context->getSourceManager());
        if (location.find(".cpp")!= std::string::npos) {
            llvm::outs() << "--------------------------------------------------\n";
            llvm::outs() <<"\"" << stmt->getString() << "\""
                << "| @ " << location<<"\n";

        }
        return true;
    }

private:
    clang::ASTContext* Context;
};
```
Other information on how to build your recursive AST visitors can be found
[here: How to write RecursiveASTVisitor based ASTFrontendActions](https://clang.llvm.org/docs/RAVFrontendAction.html)
