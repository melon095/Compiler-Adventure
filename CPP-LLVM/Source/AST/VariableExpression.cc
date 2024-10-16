#include <AST/VariableExpression.hh>

namespace K::AST
{
	VariableExpression::VariableExpression(const std::string& name)
		: BaseExpression()
		, Name(name)
	{
	}

	void VariableExpression::Dump(int indent, std::ostream& os) const
	{
		os << std::string(indent, ' ') << "VariableExpression(" << Name << ")" << std::endl;
	}

	Codegen::CodegenResult VariableExpression::Codegen(Codegen::CodegenContextPtr context) const
	{
		return Codegen::CodegenResult::NotImplemented(this->shared_from_this());
	}
} // namespace K::AST