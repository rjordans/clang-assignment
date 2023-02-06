// Declares clang::SyntaxOnlyAction.
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
// Declares llvm::cl::extrahelp.
#include "llvm/Support/CommandLine.h"


#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"

using namespace clang::tooling;
using namespace llvm;



#include "matchers.hpp"

/*
 * Tool
 */

namespace logfinder {
/**
 * Options
 */
	static llvm::cl::OptionCategory MyToolCategory("logfinder options");
	static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);
	static cl::extrahelp MoreHelp("\nMore help text...\n");

	int tool_call(int argc, const char** argv) {

		auto ExpectedParser = CommonOptionsParser::create(argc, argv, MyToolCategory);
		if (!ExpectedParser) {
			llvm::errs() << ExpectedParser.takeError();
			return 1;
		}

		CommonOptionsParser& OptionsParser = ExpectedParser.get();
		ClangTool Tool(OptionsParser.getCompilations(),
			OptionsParser.getSourcePathList());

	/**
	 * TODO: create Printers 
	 */
		MatchFinder Finder;
	/**
	 * TODO: add printer and matcher to the finder
	 */
		
		return Tool.run(newFrontendActionFactory(&Finder).get());
	}
}