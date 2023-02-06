#include <clang/AST/ParentMapContext.h>

#include <clang/AST/RecursiveASTVisitor.h>

#include "lastRelevantStatement.hpp"

#ifndef _INTERPRETBASE_HPP_
#define _INTERPRETBASE_HPP_
using namespace llvm;

template <typename ResultType, typename Derived>
class InterpretBase : public clang::RecursiveASTVisitor<Derived> {
public:
	InterpretBase(clang::ASTContext* Context)
		: Context(Context) {}
	/**
	 * Expressions and statements are the same in Clang
	 * so no differenciation is needed here.
	 * what is considered an unfortunate design
	 * choise in clag is helping us
	 */
	bool VisitReturnStmt(const clang::ReturnStmt* stmt)
	{
		clang::RecursiveASTVisitor<Derived>::TraverseStmt(const_cast<clang::Expr*>(stmt->getRetValue()));
		return false;
	}

	bool VisitCallExpr(const clang::CallExpr* expr)
	{
		auto declaration = expr->getCalleeDecl()->getAsFunction();
		if (declaration->getNumParams() == expr->getNumArgs()) {
			//TODO: check when default values for args
			for (unsigned a = 0; a < expr->getNumArgs(); a++)
			{
				auto arg = expr->getArg(a);
				auto name = declaration->getParamDecl(a)->getName().str();
				auto loc = declaration->getParamDecl(a)->getLocation().printToString(Context->getSourceManager());
				auto key = name + loc;
				clang::RecursiveASTVisitor<Derived>::TraverseStmt(const_cast<clang::Expr*>(arg));
				arguments_of_last_call.insert(std::pair<std::string, ResultType>(key, ResultValue));
			}
		}
		auto body = declaration->getBody();

		//TODO: how to deal with multiple return statements ? 
		for (auto child = body->child_begin(); child != body->child_end(); child++)
		{
			auto statement = (*child);
			if (clang::isa<clang::ReturnStmt>(statement))
			{
				clang::RecursiveASTVisitor<Derived>::TraverseStmt(const_cast<clang::Stmt*>(statement));
			}
		}
		return false;
	}


	bool VisitDeclRefExpr(const clang::DeclRefExpr* expr)
	{

		auto rwsign = RawSignature<const clang::DeclRefExpr*>(expr, Context->getSourceManager());
		if(rwsign != "")
		{
			outs() << _RawSignature<const clang::DeclRefExpr*>(expr, Context->getSourceManager());
		}

		auto identifier = expr;

		auto declaration = identifier->getDecl();


		if (clang::isa<clang::ParmVarDecl>(declaration))
		{
			auto name = expr->getNameInfo().getName().getAsString();
			auto loc = declaration->getLocation().printToString(Context->getSourceManager());
			auto key = name + loc;

			//TODO: check if I can have conflicts with locally defined var
			//TODO: check for recursion
			//and output parameters
			if (arguments_of_last_call.find(key) != arguments_of_last_call.end()) {
				ResultValue = arguments_of_last_call[key];
				arguments_of_last_call.erase(key);
				return false;
			}
		}

		auto previous = getRelevantPreviousStmt(expr->getExprStmt(), expr);
		outs() << RawSignature<const clang::Stmt*>(previous, Context->getSourceManager());

		if (previous != expr->getExprStmt())
		{
			clang::RecursiveASTVisitor<Derived>::TraverseStmt(const_cast<clang::Stmt*>(previous));
		}
		else {
			clang::RecursiveASTVisitor<Derived>::TraverseDecl(const_cast<clang::ValueDecl*>(declaration));
		}
		return false;
	}


	const clang::Stmt* getRelevantPreviousStmt(const clang::Stmt* statement,
		const clang::DeclRefExpr* target)
	{
		LastRelevatStmt lrs(Context, target);

		auto parents = Context->getParents(*statement);
		const clang::Stmt* parent;
		if (!parents.empty())
		{
			for (auto p = 0; p < parents.size(); p++) {
				parent = parents[p].get<clang::Stmt>();
				if (parent) {
					outs() << RawSignature<const clang::Stmt*>(parent, Context->getSourceManager());

					if (clang::isa<clang::CompoundStmt>(parent))
					{
						lrs.TraverseStmt(const_cast<clang::Stmt*>(parent));
						return lrs.get();
					}
					return getRelevantPreviousStmt(parent, target);
				}
			}
		}
		return target;
	}

	ResultType getVal()
	{
		return ResultValue;
	}

protected:
	clang::ASTContext* Context;
	ResultType ResultValue;
	std::map<std::string, ResultType> arguments_of_last_call;
};
#endif