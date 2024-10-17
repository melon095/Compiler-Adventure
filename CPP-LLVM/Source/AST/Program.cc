#include <AST/FunctionStatement.hh>
#include <AST/Program.hh>

#include <llvm/IR/Verifier.h>

namespace K::AST
{
	Program::Program(std::vector<ExpressionPtr> expressions)
		: m_Expressions()
	{
		m_Expressions.reserve(expressions.size());
		for(const auto& expr : expressions)
		{
			if(expr->Is<FunctionStatement>() && expr->As<FunctionStatement>()->GetName() == "main")
			{
				m_SupposedMain = expr;
			}
			else
			{
				m_Expressions.push_back(expr);
			}
		}
	}

	const std::vector<ExpressionPtr>& Program::GetExpressions() const { return m_Expressions; }

	void Program::Dump(int ident, std::ostream& os) const
	{
		os << std::string(ident, '\t') << "Program" << std::endl;

		for(const auto& expr : m_Expressions)
		{
			expr->Dump(ident + 1, os);
		}

		if(m_SupposedMain)
		{
			m_SupposedMain->Dump(ident + 1, os);
		}

		os << std::string(ident, '\t') << "End Program" << std::endl;
	}

	Codegen::CodegenResult Program::Codegen(Codegen::CodegenContextPtr context) const
	{
		for(const auto& expr : m_Expressions)
		{
			CODEGEN_CHECK(expr->Codegen(context));
		}

		// TOOD: Some sort of second stage linking, so this can reference functions not yet defined.
		CODEGEN_CHECK(LocateAndMakeEntrypoint(context));

		return Codegen::CodegenResult::Ok(this->shared_from_this(), nullptr);
	}

	Codegen::CodegenResult Program::LocateAndMakeEntrypoint(Codegen::CodegenContextPtr context) const
	{
		if(m_SupposedMain == nullptr)
		{
			return CreateAdHocMain(context);
		}

		auto mainResult = m_SupposedMain->Codegen(context);
		if(!mainResult.IsOk())
		{
			return mainResult;
		}

		if(llvm::verifyModule(*context->GetModule(), &llvm::errs()))
		{
			return Codegen::CodegenResult::Error(this->shared_from_this(), "Module verification failed");
		}

		context->GetModule()->getOrInsertFunction("main", llvm::Type::getInt32Ty(context->GetContext()), nullptr);
		context->GetModule()->getFunction("main")->setLinkage(llvm::GlobalValue::ExternalLinkage);

		return Codegen::CodegenResult::Ok(this->shared_from_this(), nullptr);
	}

	Codegen::CodegenResult Program::CreateAdHocMain(Codegen::CodegenContextPtr context) const
	{
		auto mainFunctionType = llvm::FunctionType::get(llvm::Type::getInt32Ty(context->GetContext()), false);
		auto mainFunction
			= llvm::Function::Create(mainFunctionType, llvm::Function::ExternalLinkage, "main", context->GetModule());

		auto entryBlock = llvm::BasicBlock::Create(context->GetContext(), "entry", mainFunction);
		context->GetBuilder()->SetInsertPoint(entryBlock);

		auto ret = llvm::ConstantInt::get(context->GetContext(), llvm::APInt(32, 0));
		context->GetBuilder()->CreateRet(ret);

		llvm::verifyFunction(*mainFunction);

		return Codegen::CodegenResult::Ok(this->shared_from_this(), nullptr);
	}
} // namespace K::AST
