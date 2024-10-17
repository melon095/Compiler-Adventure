#include <AST/FunctionStatement.hh>

#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Verifier.h>

namespace K::AST
{
	FunctionStatement::FunctionStatement(PrototypeStatementPtr prototype, ExpressionPtr body)
		: BaseExpression()
		, Prototype(std::move(prototype))
		, Body(std::move(body))
	{
	}

	void FunctionStatement::Dump(int indent, std::ostream& os) const
	{
		os << std::string(indent, ' ') << "FunctionStatement" << std::endl;

		Prototype->Dump(indent + 2, os);
		Body->Dump(indent + 2, os);
	}

	Codegen::CodegenResult FunctionStatement::Codegen(Codegen::CodegenContextPtr context) const
	{
		auto Builder = context->GetBuilder();
		auto& llvmContext = context->GetContext();
		auto& NamedValues = context->GetNamedValues();

		auto* TheFunction = context->GetModule()->getFunction(Prototype->Name);
		if(!TheFunction)
		{
			CODEGEN_CHECK(Prototype->Codegen(context));

			TheFunction = context->GetModule()->getFunction(Prototype->Name); // TODO: ?
		}

		if(!TheFunction)
		{
			return Codegen::CodegenResult::Error(this->shared_from_this(), "Failed to create function");
		}

		auto* BB = llvm::BasicBlock::Create(llvmContext, "entry", TheFunction);
		Builder->SetInsertPoint(BB);

		NamedValues.clear();
		for(auto& Arg : TheFunction->args())
		{
			NamedValues[std::string(Arg.getName())] = &Arg;
		}

		auto ret = Body->Codegen(context);
		if(!ret.IsOk())
			goto c_error;

		{
			auto* retval = ret.GetValue();
			Builder->CreateRet(retval);

			llvm::verifyFunction(*TheFunction);

			return Codegen::CodegenResult::Ok(this->shared_from_this(), TheFunction);
		}

	c_error:
		{
			TheFunction->eraseFromParent();

			return Codegen::CodegenResult::Error(this->shared_from_this(), "Failed to generate function body");
		}
	}
} // namespace K::AST