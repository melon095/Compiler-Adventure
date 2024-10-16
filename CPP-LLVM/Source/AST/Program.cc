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
		os << std::string(ident, '\t') << "Program" << std::endl;

		for(const auto& expr : m_Expressions)
		{
			expr->Dump(ident + 1, os);
		}

		os << std::string(ident, '\t') << "End Program" << std::endl;
	}

	Codegen::CodegenResult Program::Codegen(Codegen::CodegenContextPtr context) const
	{
		for(const auto& expr : m_Expressions)
		{
			auto result = expr->Codegen(context);
			if(!result.IsOk())
			{
				return result;
			}
		}

		return Codegen::CodegenResult::Ok(this->shared_from_this());
	}
} // namespace K::AST
