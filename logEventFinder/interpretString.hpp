#include "interpretBase.hpp"
#ifndef _INTERPRETSTRING_HPP_
#define _INTERPRETSTRING_HPP_

struct AdditionalInfo {
public:
	std::string ResultValue;
	std::string tag;
	AdditionalInfo(std::string v, std::string t) :ResultValue(v), tag(t) {};
};

class InterpretString : public InterpretBase<std::vector<AdditionalInfo>, InterpretString> {
public:
	InterpretString(clang::ASTContext* Context)
		: InterpretBase(Context) {}

	bool VisitStringLiteral(const clang::StringLiteral* expr) {
		/**
		 * TODO: get the string from the String Literal
		 */
		return false;
	}


	bool VisitCXXTemporaryObjectExpr(clang::CXXTemporaryObjectExpr* expr) {
		/**
		 * TODO: get the tag and value set by the class AdditionalInfo
		 */
		
		return false;
	}


	bool VisitCallExpr(const clang::CallExpr* expr) {
		if (clang::isa<clang::CXXOperatorCallExpr>(expr)) {
			VisitCXXOperatorCallExpr(static_cast<const clang::CXXOperatorCallExpr*>(expr));
			return false;
		}
		if (clang::isa<clang::CXXMemberCallExpr>(expr)) {
			VisitCXXMemberCallExpr(static_cast<const clang::CXXMemberCallExpr*>(expr));
			return false;
		}
		/**
		 * TODO: get tag and value as set by the function AdditionalInfo
		 */
		
		InterpretBase::VisitCallExpr(expr);
		return false;
	}

	bool VisitCXXMemberCallExpr(const clang::CXXMemberCallExpr* expr)
	{
		auto methodName = expr->getMethodDecl()->getName().str();
		auto className = expr->getRecordDecl()->getName();
		if (methodName == "stream" && className == "AdditionalInfo")
		{
			auto owner = expr->getCallee();
			TraverseStmt(const_cast<clang::Expr*>(owner));
		}
		return false;
	}

	bool VisitCXXOperatorCallExpr(const clang::CXXOperatorCallExpr* expr) {
		auto opcode = expr->getOperator();
		//operator == "<<"
		if (opcode == clang::OO_LessLess) {
			//always two arguments
			if (clang::isa<clang::CXXOperatorCallExpr>(expr->getArg(0))) {
				VisitCXXOperatorCallExpr(static_cast<const clang::CXXOperatorCallExpr*>(expr->getArg(0)));
			}
			TraverseStmt(const_cast<clang::Expr*>(expr->getArg(1)));
		}
		return false;
	}

};
#endif