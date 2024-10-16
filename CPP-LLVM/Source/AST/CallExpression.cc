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
		return Codegen::CodegenResult::NotImplemented(this->shared_from_this());

		// auto &builder = context->GetBuilder();
		// auto &module = context->GetModule();

		// auto callee = module->getFunction(Callee);
		// if (!callee)
		// {
		//     throw std::runtime_error("Unknown function referenced");
		// }

		// if (callee->arg_size() != Args.size())
		// {
		//     throw std::runtime_error("Incorrect number of arguments passed");
		// }

		// std::vector<llvm::Value *> args;
		// for (const auto &arg : Args)
		// {
		//     arg->Codegen(context);
		//     args.push_back(context->GetBuilder().GetInsertBlock());
		// }

		// context->GetBuilder().CreateCall(callee, args, "calltmp");
	}
} // namespace K::AST