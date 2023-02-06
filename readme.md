# In-house logging library overview generation 
This exercise will teach us how to build a custom static analysis tool with Clang. 

We will use Visual Studio 2019 and CMake.

# Preparation
## Install and compile clang
This step takes a long time, so running it well before the hands-on session is advised.

Follow these instructions  [Clang installation on windows 10](wiki/Clang-installation-on-windows-10)

## Compile the code 
Set the environment variables:
`CLANG_DIR` and `LLVM_DIR`

Open Visual Studio 2019 and use the `Developer PowerShell`.
Change the directory to the directory of the exercise:
`<EXERCISE>\code\Stand-Alone-Libtooling`.
Make a directory containing all the build artifacts and run CMake as follows. 

```
mkdir build
cd build
cmake ..
```

There is a Visual Studio solution file in the build directory: `analysis-tools.sln`.
Open that file:
`File>Open>Project\Solution`, navigate to the build directory, select `analysis-tools.sln`, and click on `Open`.

This will open the solution in Visual Studio. 
Right-click on the solution at the top of the `Solution Explorer` pane on the right, and run `Build Solution`.

## Run the tests (in Debug mode)

Right-click on the project `all_tests`. Select
`Set as Startup Project`. Right-click again and select `Debug\Start New Instance`.
You should see the test `LOG_FINDER.test1` running and passing with an empty output on the Console.

# Coding
You may start exploring the resources on clang summarized here: [something-about-clang](wiki/something-about-clang).

## Create clang matchers 
Before starting this session, read about clang [ASTMatchers](wiki/matchers) and [AST-dump](wiki/dump-ast.md)

### Detect the log_events declaration

In the project, `log-event-finder` open the file `LogEventFinder.cpp`.

Right-click on `Matcher.hpp` and select `Go to Document`.

In the file `matchers.hpp`, complete the matcher 
`LogEventsMatcher`.

This matcher matches the variable declaration with the name "log_events" on line 22 of 
path-to\AFakeLoggingFramework\AFakeLoggingFramework.cpp,

For each descendant, make sure it's an initListExpr, and from it, take 

1) parameter number 0, which is an expression and bind it to the symbol "id"
2) parameter number 1, which is a stringLiteral and bind it to the symbol "unit"
3) parameter number 2, which is a stringLiteral and bind it to the symbol "desc"

    Hint: use clang-query and clang-check to check your matcher. 

In clang-query, you may want to use:
    `set bind-root false` and 
    `set output dump`.

In clang-check, you may want to use:
    `--ast-dump --ast-dump-filter=...`

Before continuing, in the file `matchers.hpp`, study the class `LogEventsPrinter`. Notice that it inherits from the `MatchFinder::MatchCallback` and has a method `run`. What is the purpose of this method?

In the file `LogEventFinder.cpp`, add the matcher callback (`LogEventsPrinter`) to the `MatchFinder Finder`.

At the end of this [page](wiki/matchers.md), there is an example of how to do this.

Compile and run. You will get the list of events as declared in `log_events` but with the wrong IDs. 

### Interpret the log IDs
In Visual Studio solution explorer, search for the string `InterpretInteger`. This will find a class, click on it and open the file `interpretInteger.hpp`.

Get familiar with the class `InterpretInteger` and notice how it derives from `InterpretBase`. Right-click on `InterpretBase` and `Go To Document` and get familiar with the class. Notice how it computes a `ResultValue` of type `ResultType` (a class template parameter).
Go back to `InterpretInteger`. To what type is the template parameter `ResultType` of class `InterpretBase` set? (`llvm::APInt`)

Complete the visitors as follows:
1) in the `VisitIntegerLiteral` set, `ResultValue` equals the integer value from the Integer Literal `expr`.
    Hint: use the method `getValue` available in the class `clang::IntegerLiteral`. 
2) in the `VisitBinaryOperator` 
    - get the values of the LHS and RHS
    Traverse the expression with the same visitor on the right and left-hand sides of the expression

        Hint:  To launch the same visitor on an expression, run `       this->TraverseStmt(rhs)`.
        The class `clang::BinaryOperator` has methods `getRHS()` and `getLHS()` to retrieve the right and left-hand sides, respectively.
        
    - interpret the result of the binary operation depending on the used operator

        hint: The class `clang::BinaryOperator` has the method `getOpcodeStr()` to retrieve the operator of the expression. 
        Compute the result for these operators:
        [+, -, * , +=, -=, *=]

3) In the file `matchers.hpp`, complete the `LogEventsPrinter` to invoke the `InterpretInteger`.

    Hint:
    The class `InterpretInteger` constructor takes the ASTContext as a parameter. This can be found in `Result.Context`;
    Launch the visitor on the il expression using the `TraverseStmt` method of the interpreter.
    You must cast the `il` expression as follows: `const_cast<Expr*>(il)`.
    Use the interpreter method `getVal()` to retrieve the correct value of the `id`. 


Compile and run. You will get the list of events as declared in `log_events` with the correct IDs.

### Detect the logger.log() expressions

In the file `matchers.hpp`, complete the matcher 
`LogMatcher` as follows:

Match the call to a member function (CXXMemberCallExpr) associated with the class declaration (CXXRecordDecl) named "Logger". 
The call expression references a class method (cxxMethodDecl) called "log". 
The call expression has one argument, which is the expression itself. 
Return binds to:
1) the argument of the call expression 
2) to the call expression itself

In the file `LogEventFinder.cpp`, add the matcher callback (`LogPrinter`) to the `MatchFinder Finder`.

Compile and run.  In addition to the list of events as declared in `log_events`, you will get a list of log messages and their code location.
Please notice the message with `id = 0` produced by line 96 of `AFakeLoggingFramework.cpp`. This is an example of the current limits of the `InterpretInteger` class, which does not handle the `for` loops.

### Interpret the log Additional Information
Open the file `interpretString.hpp`.
Get familiar with the `InterpretString` class.
To what type is the template parameter `ResultType` of class `InterpretBase` set?
(std::vector<AdditionalInfo>).

Complete the visitor as follows:

1) in `VisitStringLiteral`, create an `AdditionalInfo` that takes `expr` as a string as a first argument and an empty string as a second argument. 
`push_back` this class in the `ResultValue` vector.
    Hint:
    The class `clang::StringLiteral` has a `getString()` method. 
    You will need to use the method `str()` on the output of `getString()`.
    
2) in `VisitCXXTemporaryObjectExpr`, get the type of `expr` as a string and check if it is equal to `"class AdditionalInfo"`. If yes, traverse the first argument of the expression and record the result. Return `false` because you want to stop the visitor. 
If no, get the children of the expression and traverse each one of them. 

    Hint: The class `clang::CXXTemporaryObjectExpr` has the following useful methods: `getType()`, `getArg(0)` and `children()`. To record the result, use the following lines:

    ```
    ResultValue[ResultValue.size() - 1].tag = ResultValue[ResultValue.size() - 1].ResultValue;
    ResultValue[ResultValue.size() - 1].ResultValue = "%s";
    ```
3) in `VisitCallExpr`, get the callee declaration as a function. Check if its name is equal to `"AdditionalInfo"`. If yes, traverse its first argument, record the results, and return `false`.

    Hint:
    From the calss `clang::CallExpr` you can call the methods: `expr->getCalleeDecl()->getAsFunction()->getName().str()`

Compile and run. You will get the additional info.

The expected final result is something as follows:

```
id: 10 unit: IEC desc: this is the first possible event
id: 37 unit: IEC desc: this is the second possible event
id: 48 unit: IEC desc: this is the third possible event
id: 54 unit: IEC desc: this is the forth possible event
id: 74 unit: IEC desc: this is the fifth possible event
id: 12 unit: IEC desc: this is the sixth possible event
id: 4072 unit: IEC desc: this is the seventh possible event
id: 4082 unit: IEC desc: From within the sender
id: 4092 unit: IEC desc: From within the receiver

id: 4092 unit: IEC descr: From within the receiver From within the receiver constructor /path-to/AFakeLoggingFramework/AFakeLoggingFramework/AFakeLoggingFramework.cpp:47:10
id: 4092 unit: IEC descr: From within the receiver The receiver receives /path-to/AFakeLoggingFramework/AFakeLoggingFramework/AFakeLoggingFramework.cpp:52:10
id: 4082 unit: IEC descr: From within the sender From within the sender constructor /path-to/AFakeLoggingFramework/AFakeLoggingFramework/AFakeLoggingFramework.cpp:69:10
id: 4082 unit: IEC descr: From within the sender The sender sends /path-to/AFakeLoggingFramework/AFakeLoggingFramework/AFakeLoggingFramework.cpp:74:10
id: 10 unit: IEC descr: this is the first possible event  /path-to/AFakeLoggingFramework/AFakeLoggingFramework/AFakeLoggingFramework.cpp:104:10
id: 10 unit: IEC descr: this is the first possible event --FreeText because I am lazy and again and again ...-- /path-to/AFakeLoggingFramework/AFakeLoggingFramework/AFakeLoggingFramework.cpp:92:10
id: 37 unit: IEC descr: this is the second possible event [good=%s] /path-to/AFakeLoggingFramework/AFakeLoggingFramework/AFakeLoggingFramework.cpp:93:10
id: 48 unit: IEC descr: this is the third possible event --Again FreeText because I am really lazy-- /path-to/AFakeLoggingFramework/AFakeLoggingFramework/AFakeLoggingFramework.cpp:94:10
id: 10 unit: IEC descr: this is the first possible event --This is new-- /path-to/AFakeLoggingFramework/AFakeLoggingFramework/AFakeLoggingFramework.cpp:95:10
id: 0 unit:  descr:  --This log() call has a variable arg % i--  /path-to/AFakeLoggingFramework/AFakeLoggingFramework/AFakeLoggingFramework.cpp:96:10
id: 4072 unit: IEC descr: this is the seventh possible event [first tag=%s][second tag=%s][third tag=%s] /path-to/AFakeLoggingFramework/AFakeLoggingFramework/AFakeLoggingFramework.cpp:97:10
id: 4072 unit: IEC descr: this is the seventh possible event [stream tag=%s] /path-to/AFakeLoggingFramework/AFakeLoggingFramework/AFakeLoggingFramework.cpp:100:10
id: 54 unit: IEC descr: this is the forth possible event [User=%s] /path-to/AFakeLoggingFramework/AFakeLoggingFramework/AFakeLoggingFramework.cpp:102:10
id: 4072 unit: IEC descr: this is the seventh possible event [last stream tag=%s] /path-to/AFakeLoggingFramework/AFakeLoggingFramework/AFakeLoggingFramework.cpp:103:10
```

