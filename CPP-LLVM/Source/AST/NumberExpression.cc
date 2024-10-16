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
		auto* value = llvm::ConstantFP::get(context->GetContext(), llvm::APFloat(Value));
		context->GetValueStack().push(value);

		return Codegen::CodegenResult::Ok(this->shared_from_this());
	}
} // namespace K::AST