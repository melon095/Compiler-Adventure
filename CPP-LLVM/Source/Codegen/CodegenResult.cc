#include <AST/BaseExpression.hh>
#include <Codegen/CodegenResult.hh>

namespace K::Codegen
{
	CodegenResult::CodegenResult(std::shared_ptr<const AST::BaseExpression> expr)
		: m_Expression(expr)
	{
	}

	CodegenResult::CodegenResult(std::shared_ptr<const AST::BaseExpression> expr, const std::string& message)
		: m_ErrorMessage(message)
		, m_Expression(expr)
	{
	}

	bool CodegenResult::IsOk() const { return m_ErrorMessage.empty(); }

	void CodegenResult::Dump(int indent, std::ostream& os) const
	{
		os << std::string(indent, ' ') << "CodegenResult(" << (IsOk() ? "Ok" : "Error") << ')' << std::endl;

		if(!IsOk())
		{
			os << std::string(indent + 2, ' ') << m_ErrorMessage << std::endl;
		}

		if(m_Expression)
		{
			m_Expression->Dump(indent + 2, os);

			os << std::string(indent + 2, ' ') << "Info" << std::endl
			   << std::string(indent + 4, ' ') << "Line: " << m_Expression->GetData().Debug.Line << std::endl
			   << std::string(indent + 4, ' ') << "Column: " << m_Expression->GetData().Debug.Column << std::endl;
		}
	}
} // namespace K::Codegen