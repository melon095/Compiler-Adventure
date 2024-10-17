#pragma once

#include <AST/BaseExpression.hh>
#include <AST/PrototypeStatement.hh>

#include <memory>

namespace K::AST
{
	class FunctionStatement : public BaseExpression
	{
	  public:
		FunctionStatement(PrototypeStatementPtr prototype, ExpressionPtr body);

		void Dump(int indent, std::ostream& os) const override;
		Codegen::CodegenResult Codegen(Codegen::CodegenContextPtr context) const override;

		const std::string& GetName() const { return Prototype->Name; }

	  public:
		PrototypeStatementPtr Prototype;
		ExpressionPtr Body;
	};

	SMARTPTR(FunctionStatement)
} // namespace K::AST
