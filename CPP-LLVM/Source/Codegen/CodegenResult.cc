#include <AST/BaseExpression.hh>
#include <Codegen/CodegenResult.hh>

namespace K::Codegen
{
	CodegenResult::CodegenResult(std::shared_ptr<const AST::BaseExpression> expr, llvm::Value* value)
		: m_Expression(expr)
		, m_Variant(value)
	{
	}

	CodegenResult::CodegenResult(std::shared_ptr<const AST::BaseExpression> expr, const std::string& message)
		: m_Expression(expr)
		, m_Variant(message)
	{
	}

	void CodegenResult::Dump(int indent, std::ostream& os) const
	{
		os << std::string(indent, ' ') << "CodegenResult(" << (IsOk() ? "Ok" : "Error") << ')' << std::endl;

		if(!IsOk())
		{
			os << std::string(indent + 2, ' ') << "Error: " << GetErrorMessage() << std::endl;
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