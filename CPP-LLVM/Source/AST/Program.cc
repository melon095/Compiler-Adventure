#include <AST/Program.hh>

namespace K::AST
{
	Program::Program(std::vector<ExpressionPtr> expressions)
		: m_Expressions(std::move(expressions))
	{
	}

	const std::vector<ExpressionPtr>& Program::GetExpressions() const { return m_Expressions; }

	void Program::Dump(int ident, std::ostream& os) const
	{
		for(const auto& expr : m_Expressions)
		{
			expr->Dump(ident, os);
		}
	}
} // namespace K::AST