#pragma once

#include <AST/BaseExpression.hh>
#include <vector>

namespace K::AST
{
	class Program : public BaseExpression
	{
	  public:
		Program(std::vector<ExpressionPtr> expressions);

		[[nodiscard]]
		const std::vector<ExpressionPtr>& GetExpressions() const;

		void Dump(int ident, std::ostream& os) const override;
		Codegen::CodegenResult Codegen(Codegen::CodegenContextPtr context) const override;

	  private:
		Codegen::CodegenResult LocateAndMakeEntrypoint(Codegen::CodegenContextPtr context) const;
		Codegen::CodegenResult CreateAdHocMain(Codegen::CodegenContextPtr context) const;

	  private:
		std::vector<ExpressionPtr> m_Expressions;
        ExpressionPtr m_SupposedMain;
	};

	SMARTPTR(Program)
} // namespace K::AST
