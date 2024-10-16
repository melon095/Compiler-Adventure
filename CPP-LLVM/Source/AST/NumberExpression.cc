#include <AST/NumberExpression.hh>

namespace K::AST
{
	NumberExpression::NumberExpression(double value)
		: BaseExpression()
		, Value(value)
	{
	}

	void NumberExpression::Dump(int indent, std::ostream& os) const
	{
		os << std::string(indent, ' ') << "NumberExpression(" << Value << ")" << std::endl;
	}

	Codegen::CodegenResult NumberExpression::Codegen(Codegen::CodegenContextPtr context) const
	{
		return Codegen::CodegenResult::NotImplemented(this->shared_from_this());
	}
} // namespace K::AST