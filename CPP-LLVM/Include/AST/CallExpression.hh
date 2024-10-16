#pragma once

#include "BaseExpression.hh"

#include <vector>

namespace K::AST
{
	class CallExpression : public BaseExpression
	{
	  public:
		CallExpression(const std::string& callee, std::vector<ExpressionPtr> args);

		void Dump(int indent, std::ostream& os) const override;
		Codegen::CodegenResult Codegen(Codegen::CodegenContextPtr context) const override;

	  public:
		std::string Callee;
		std::vector<ExpressionPtr> Args;
	};

	SMARTPTR(CallExpression)
} // namespace K::AST