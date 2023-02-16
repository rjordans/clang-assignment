# Matchers
From the [official web page on clang matchers](https://clang.llvm.org/docs/LibASTMatchers.html), we can read: 

- LibASTMatchers provides a domain-specific language to create predicates on Clang's AST. 
- This DSL is written in and can be used from C++, allowing users to write **a single program** to both match AST nodes and access the node's C++ interface to extract attributes, source 
locations, or any other information provided on the AST level.
- AST matchers are predicates on nodes in the AST. Matchers are created by calling creator 
functions that allow building up a tree of matchers, where inner matchers are used to making the 
match more specific.

There are three different basic categories of matchers:
1. [Node Matchers:](https://clang.llvm.org/docs/LibASTMatchersReference.html#decl-matchers) Matchers that match a specific type of AST node.
2. [Narrowing Matchers:](https://clang.llvm.org/docs/LibASTMatchersReference.html#narrowing-matchers) Matchers that match attributes on AST nodes.
3. [Traversal Matchers:](https://clang.llvm.org/docs/LibASTMatchersReference.html#traversal-matchers) Matchers that allow traversal between related AST nodes.

## Quickly explore AST matchers: clang-query
Let's start a hands-on session. In Visual Studio's terminal, run the command:
> <LLVM_DIR>\build\Release\bin\clang-query.exe .\AFakeLoggingFramework.cpp --

This command starts a clang-query session on the given CPP file. Let's look for all the function declarations of functions named `log` using a node matcher:

``` 
match functionDecl(hasName("log"))
```

Please notice that the keyword `match` can be replaced by the shortcut ` m'. This command returns five matches, but not all of them are relevant. 

We want to match the function declaration with this signature: `Event& log(int index).` To do that, we can add narrowing and traversal matchers. 
To prune the found matches based on the return type of the function declaration, we use:

```
 m functionDecl(
    allOf( 
        hasName("log"), 
        hasReturnTypeLoc(loc(asString("Event &"))) 
    ) 
)
 ```

Here we have used a narrowing matcher `allOf` and two traversal matchers, `hasName` and `hasReturnTypeLoc` (this last one to be always used in combination with `loc`). 

To narrow based on the first parameter's name, we use:

```
m functionDecl(
    allOf( 
        hasName("log"), 
        hasParameter(0,   hasName( "index" )) 
    ) 
)
```

We can combine more matchers to create a more specialized matcher.

Any found match creates a binding with a variable called `root,` which means that in CPP, the matchers return a variable of type
`MatchFinder::MatchResult& Result` and from this value one can access the binding via `Result.Nodes.getNodeAs<clang::FunctionDecl>("root")`. 

It is possible to create other bindings to access specific nodes in the matched subtree of the AST. 

``` 
m functionDecl(
    allOf( 
        hasName("log"), 
        hasParameter(0,  
            allOf(
                varDecl().bind("param"), 
                hasType( asString("int") ) 
            )
        ) 
    ) 
).bind("x")
```

In the last example, since we created an alternative binding for the root, we will have two variables, `root` and `x,` that point to the same match, plus the variable pointing at the parameter (i.e., `param`).

We can disable the creation of `root` from clang-query typing: 

> set bind-root false

We can also see the found matches in the AST format by setting:

> set output dump

or in the concrete syntax format 

> set output diag

## Include clang matchers in a CPP code 

To describe this way of navigating the AST, we will use an example of a custom static analysis stool. This tool finds all the if statement in the input AST, and its code can be found [here](TODO). 

We will not describe how to compile and link this code to clang. For such a description, refer to the [README](TODO) of the code.

Suppose we want to find the code:

```
if(key_.empty()){
    return value_;
}
```

We use the following matcher, found experimenting with clang-query.exe: 

```
 m ifStmt(
    hasCondition(
        cxxMemberCallExpr(
            callee(
                cxxMethodDecl(hasName("empty")) 
            )
        )
    )
)
```

Please notice that this matcher detects if statement with and without the else clause.

After creating the tool, as also explained [here](ast-visitors.md), we need to create a MatchFinder, which is a clang class allowing us to apply AST matchers, and a MatchCallback, which is a clang class whose `run` method is called back each time a match registered for it is met. In our case, the MatchFinder is: 

```cpp
StatementMatcher ifMatcher =
ifStmt(
    hasCondition(
        cxxMemberCallExpr(
            callee(cxxMethodDecl(hasName("empty")))
        )
    )
).bind("ifStmt");
```

And the callback, which distinguishes between if statement with and else or not, is: 

```cpp
class IfMatcherCallback : public MatchFinder::MatchCallback {
public:
    virtual void run(const MatchFinder::MatchResult& Result) {
        if (const IfStmt* IS = Result.Nodes.getNodeAs<clang::IfStmt>("ifStmt"))
            if (IS->hasElseStorage()) {
                llvm::outs() << "\nThis match has an else clause:\n";
                outs() << RawSignature<const clang::IfStmt*>(IS, Result.Context->getSourceManager());
            }
            else {
                llvm::outs() << "\nThis match DOES NOT have an else clause:\n";
                outs() << RawSignature<const clang::IfStmt*>(IS, Result.Context->getSourceManager());
            }
    }
};
```
Please notice that the match results also give access to the ASTContext, which is here used to print the raw signature of the match. 

The last step is to run, in the main, the clang tool that we have created with the finder as front-end action: 

```cpp
int main(int argc, const char** argv) {
    ...
    ClangTool Tool(OptionsParser.getCompilations(), OptionsParser.getSourcePathList());

    IfMatcherCallback IfCallback;
    MatchFinder Finder;
    Finder.addMatcher(ifMatcher, &IfCallback);

    return Tool.run(newFrontendActionFactory(&Finder).get());
}
```