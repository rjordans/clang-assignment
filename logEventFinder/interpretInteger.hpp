#include "interpretBase.hpp"
#ifndef _INTERPRETINTEGER_HPP_
#define _INTERPRETINTEGER_HPP_

using namespace llvm;


class InterpretInteger : public InterpretBase<llvm::APInt, InterpretInteger> {
public:
	InterpretInteger(clang::ASTContext* Context)
		: InterpretBase(Context) {}

	bool  VisitIntegerLiteral(const clang::IntegerLiteral* expr) {
		/**
		 * TODO: get the integer value from the Integer Literal
		 */
		return false;
	}

	bool VisitBinaryOperator(const clang::BinaryOperator* expr)
	{
		/**
		 * TODO: get the values of the LHS and RHS and
		 * interpret the result of the binary operation
		 * depending on the used operator
		 */

		return false;
	}

};
#endif
