#pragma once

#include "BaseExpression.hh"

namespace K::AST
{
	class NumberExpression : public BaseExpression
	{
	  public:
		NumberExpression(double value);

		void Dump(int indent, std::ostream& os) const override;
		Codegen::CodegenResult Codegen(Codegen::CodegenContextPtr context) const override;

	  public:
		double Value;
	};

	SMARTPTR(NumberExpression)
} // namespace K::AST