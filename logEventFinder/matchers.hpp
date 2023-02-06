#include "interpretInteger.hpp"
#include "interpretString.hpp"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

#ifndef _MATCHERS_HPP_
#define _MATCHERS_HPP_

using namespace clang;
using namespace clang::ast_matchers;

/**
 * Matchers
 */
DeclarationMatcher LogEventsMatcher =
/**
 * TODO: write a correct matcher to get the list of log_events
 * hint: use clang-query or clang-check to explore the matchers
 */
varDecl(hasName("???"));

class Event
{
public:
	llvm::APInt id;
	StringRef unit;
	StringRef description;
	Event() = default;
	Event(llvm::APInt i, StringRef u, StringRef d) : id(i), unit(u), description(d) {}
	std::string stream()
	{
		return std::string("id: ") +
			std::to_string(*id.getRawData()) +
			std::string(" unit: ") +
			std::string(unit) + std::string(" descr: ") +
			std::string(description);
	}
};

std::map<uint64_t, Event> possible_log_event;

class LogEventsPrinter : public MatchFinder::MatchCallback
{
	static uint64_t  num;
public:
	virtual void run(const MatchFinder::MatchResult& Result)
	{
		llvm::APInt id;
		StringRef unit, description;

		if (const clang::Expr* il = Result.Nodes.getNodeAs<clang::Expr>("id"))
		{
			/**
			 * TODO: using the interpretInteger get the value of id and print it
			 */
			outs() << "id: " << id;
		}
		if (const clang::StringLiteral* il = Result.Nodes.getNodeAs<clang::StringLiteral>("unit"))
		{
			unit = il->getString();
			outs() << " unit: " << unit;
		}
		if (const clang::StringLiteral* il = Result.Nodes.getNodeAs<clang::StringLiteral>("desc"))
		{
			description = il->getString();
			outs() << " desc: " << description << "\n";
		}
		possible_log_event[num] = Event(id, unit, description);
		num++;
	}
};

uint64_t LogEventsPrinter::num = 1;


StatementMatcher LogMatcher =
/**
 * TODO: write a correct the matcher to get the list of log_events
 * hint: use clang-query or clang-check to explore the matchers
 */
expr().bind("root");


void static inline printAdditionalInfo(InterpretString is, const clang::Stmt* statement)
{
	is.TraverseStmt(const_cast<Stmt*>(statement));

	auto ais = is.getVal();
	for (auto ai : ais) {
		if (ai.tag.empty())
		{
			outs() << ai.ResultValue;
		}
		else
		{
			outs() << "[" << ai.tag << "=" << ai.ResultValue << "]";
		}
	}
}

class LogPrinter : public MatchFinder::MatchCallback
{
public:
	virtual void run(const MatchFinder::MatchResult& Result)
	{

		if (const clang::Expr* FD = Result.Nodes.getNodeAs<clang::Expr>("arg"))
		{
			InterpretInteger ib(Result.Context);
			ib.TraverseStmt(const_cast<Expr*>(FD));
			auto value = ib.getVal();
			outs() << "\n" << possible_log_event[*value.getRawData()].stream() << " ";

		}

		if (const clang::CXXMemberCallExpr* FD = Result.Nodes.getNodeAs<clang::CXXMemberCallExpr>("root"))
		{
			auto parents = Result.Context->getParents(*FD);
			std::string loc = FD->getExprLoc().printToString(Result.Context->getSourceManager());
			InterpretString is(Result.Context);

			if (parents.empty() || parents.size() != 1) {
				outs() << "Error: at line " << loc;
				return;
			}

			if (!clang::isa<clang::CXXOperatorCallExpr>(*parents[0].get<clang::Stmt>()))
			{
				printAdditionalInfo(is, FD);
				outs() << " " << loc;
				return;
			}

			auto next_parents = Result.Context->getParents(*parents[0].get<clang::Stmt>());

			while (!next_parents.empty() && next_parents.size() == 1 &&
				clang::isa<clang::CXXOperatorCallExpr>(next_parents[0].get<clang::Stmt>()))
			{
				auto parent = next_parents[0].get<clang::Stmt>();
				outs() << RawSignature<const clang::Stmt*>(parent, Result.Context->getSourceManager());
				parents = next_parents;
				next_parents = Result.Context->getParents(*parent);
			}

			if (!parents.empty())
			{
				for (auto p = 0; p < parents.size(); p++) {
					auto parent = parents[p].get<clang::Stmt>();
					outs() << RawSignature<const clang::Stmt*>(parent, Result.Context->getSourceManager());
					printAdditionalInfo(is, parent);
				}
				outs() << " " << loc;
			}
		}

	}
};
#endif