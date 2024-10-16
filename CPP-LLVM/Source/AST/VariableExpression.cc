#include <AST/VariableExpression.hh>

namespace K::AST
{
	VariableExpression::VariableExpression(const std::string& name)
		: Name(name)
	{
	}

	void VariableExpression::Dump(int indent, std::ostream& os) const
	{
		os << std::string(indent, ' ') << "VariableExpression(" << Name << ")" << std::endl;
	}
} // namespace K::AST