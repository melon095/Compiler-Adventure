#pragma once

#include "BaseExpression.hh"
#include "Lexer/Tokens.hh"

namespace K::AST
{
	enum class BinaryOperator
	{
		Add,
		Sub,
		Mul,
		Div,
		Less,
	};

	class BinaryExpression : public BaseExpression
	{
	  public:
		BinaryExpression(BinaryOperator op, ExpressionPtr lhs, ExpressionPtr rhs);
		BinaryExpression(TokenID op, ExpressionPtr lhs, ExpressionPtr rhs);

		void Dump(int indent, std::ostream& os) const override;
		Codegen::CodegenResult Codegen(Codegen::CodegenContextPtr context) const override;

	  public:
		BinaryOperator Op;
		ExpressionPtr LHS, RHS;
	};

	SMARTPTR(BinaryExpression)
} // namespace K::AST
