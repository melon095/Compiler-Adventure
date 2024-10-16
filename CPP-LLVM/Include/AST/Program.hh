#pragma once

#include <AST/BaseExpression.hh>
#include <vector>

namespace K::AST
{
	class Program : public IDumpable
	{
	  public:
		Program(std::vector<ExpressionPtr> expressions);

		[[nodiscard]]
		const std::vector<ExpressionPtr>& GetExpressions() const;

		void Dump(int ident, std::ostream& os) const override;

	  private:
		std::vector<ExpressionPtr> m_Expressions;
	};

	SMARTPTR(Program)
} // namespace K::AST
