#include <clang/AST/ParentMapContext.h>

#include "clang/AST/RecursiveASTVisitor.h"

#ifndef _LASTRELEVANTSTATEMENT_HPP_
#define _LASTRELEVANTSTATEMENT_HPP_
using namespace llvm;
//#define DEBUG_RAW
#ifdef DEBUG_RAW
template <typename T>
std::string _RawSignature(T d, clang::SourceManager& sm) {
	auto loc = d->getBeginLoc().printToString(sm);
	std::string text = clang::Lexer::getSourceText(clang::CharSourceRange::getTokenRange(d->getSourceRange()), sm, clang::LangOptions(), 0).str();
	if (text.size() > 0 && (text.at(text.size() - 1) == ',')) //the text can be ""
		return clang::Lexer::getSourceText(clang::CharSourceRange::getCharRange(d->getSourceRange()), sm, clang::LangOptions(), 0).str() + loc + "\n";
	return text + loc + "\n";
}

template <typename T>
std::string Location(T d, clang::SourceManager& sm) {
	return d->getBeginLoc().printToString(sm);
}

template <typename T>
std::string RawSignature(T d, clang::SourceManager& sm) {
	std::string text = clang::Lexer::getSourceText(clang::CharSourceRange::getTokenRange(d->getSourceRange()), sm, clang::LangOptions(), 0).str();
	if (text.size() > 0 && (text.at(text.size() - 1) == ',')) //the text can be ""
		return clang::Lexer::getSourceText(clang::CharSourceRange::getCharRange(d->getSourceRange()), sm, clang::LangOptions(), 0).str();
	return text;
}
#else
template <typename T>
std::string RawSignature(T d, clang::SourceManager& sm) {
	return "";
}

template <typename T>
std::string _RawSignature(T d, clang::SourceManager& sm) {
	return "";
}
#endif


class LastRelevatStmt : public clang::RecursiveASTVisitor<LastRelevatStmt> {
public:
	LastRelevatStmt(clang::ASTContext* Context, const clang::DeclRefExpr* target)
		: Context(Context), target(target)
	{
		lastRelevatStmt = target;
		targetFound = false;
	}

	bool VisitDeclRefExpr(clang::DeclRefExpr* decl) {
		outs()<< RawSignature(decl->getExprStmt(), Context->getSourceManager());
		if (decl == target)
		{
			targetFound = true;
			return false;
		}
		if (decl->getNameInfo().getName() == target->getNameInfo().getName())
		{
			lastRelevatStmt = currentChild;
		}
		return false;
	}

	bool  VisitCompoundStmt(const clang::CompoundStmt* stmt) {
		auto children = stmt->children();
		outs() << RawSignature(stmt, Context->getSourceManager());

		for (auto ch = children.begin(); ch != children.end(); ch++)
		{
			currentChild = *ch;
			if (targetFound)
			{
				return false;
			}
			this->TraverseStmt(const_cast<clang::Stmt*>(*ch));
		}

		return false;
	}


	const clang::Stmt* get()
	{
		return lastRelevatStmt;
	}


private:
	clang::ASTContext* Context;
	const clang::DeclRefExpr* target;
	const clang::Stmt* currentChild;
	const clang::Stmt* lastRelevatStmt;
	bool targetFound;
};
#endif