#include <AST/PrototypeStatement.hh>

namespace K::AST
{
	PrototypeStatement::PrototypeStatement(const std::string& name, std::vector<std::string> args)
		: BaseExpression()
		, Name(name)
		, Args(args)
	{
	}

	void PrototypeStatement::Dump(int indent, std::ostream& os) const
	{
		os << std::string(indent, ' ') << "PrototypeStatement(" << Name << ", [";
		for(std::size_t i = 0; i < Args.size(); i++)
		{
			os << Args[i];
			if(i < Args.size() - 1)
			{
				os << ", ";
			}
		}
		os << "])" << std::endl;
	}

	Codegen::CodegenResult PrototypeStatement::Codegen(Codegen::CodegenContextPtr context) const
	{
		auto module = context->GetModule();
		auto& llvmContext = context->GetContext();

		auto* function = module->getFunction(Name);
		if(function)
		{
			return Codegen::CodegenResult::Error(this->shared_from_this(), "Function already exists");
		}

		std::vector<llvm::Type*> doubles(Args.size(), llvm::Type::getDoubleTy(llvmContext));
		auto* ft = llvm::FunctionType::get(llvm::Type::getDoubleTy(llvmContext), doubles, false);
		auto* f = llvm::Function::Create(ft, llvm::Function::ExternalLinkage, Name, module.get());

		if(f->getName() != Name)
		{
			f->eraseFromParent();
			return Codegen::CodegenResult::Error(this->shared_from_this(), "Function name conflict");
		}

		std::size_t idx = 0;
		for(auto& arg : f->args())
		{
			arg.setName(Args[idx++]);
		}

		return Codegen::CodegenResult::Ok(this->shared_from_this());
	}
} // namespace K::AST
