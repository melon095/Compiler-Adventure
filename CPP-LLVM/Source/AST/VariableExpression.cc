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
		auto builder = context->GetBuilder();
		auto& namedValues = context->GetNamedValues();

		auto* value = namedValues[Name];
		if(!value)
		{
			return Codegen::CodegenResult::Error(this->shared_from_this(), "Unknown variable name");
		}

		context->GetValueStack().push(value);

		return Codegen::CodegenResult::Ok(this->shared_from_this(), value);
	}
} // namespace K::AST