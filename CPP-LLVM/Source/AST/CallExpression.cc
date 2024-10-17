#include <AST/CallExpression.hh>

namespace K::AST
{
	CallExpression::CallExpression(const std::string& callee, std::vector<ExpressionPtr> args)
		: BaseExpression()
		, Callee(callee)
		, Args(std::move(args))
	{
	}

	void CallExpression::Dump(int indent, std::ostream& os) const
	{
		os << std::string(indent, ' ') << "CallExpression(" << Callee << ")" << std::endl;

		for(const auto& arg : Args)
		{
			arg->Dump(indent + 2, os);
		}
	}

	Codegen::CodegenResult CallExpression::Codegen(Codegen::CodegenContextPtr context) const
	{
		auto Builder = context->GetBuilder();
		auto& NamedValues = context->GetNamedValues();

		auto* CalleeF = context->GetModule()->getFunction(Callee);
		if(!CalleeF)
		{
			return Codegen::CodegenResult::Error(this->shared_from_this(), "Unknown function referenced");
		}

		if(CalleeF->arg_size() != Args.size())
		{
			return Codegen::CodegenResult::Error(this->shared_from_this(), "Incorrect number of arguments passed");
		}

		std::vector<llvm::Value*> ArgsV;
		for(const auto& arg : Args)
		{
			auto ret = arg->Codegen(context);
			if(!ret.IsOk())
			{
				return ret;
			}

			ArgsV.push_back(ret.GetValue());
		}

		return Codegen::CodegenResult::Ok(this->shared_from_this(), Builder->CreateCall(CalleeF, ArgsV));
	}
} // namespace K::AST