#include <Codegen/CodegenContext.hh>

#include <llvm/IR/IRBuilder.h>

namespace K::Codegen
{
	CodegenContext::CodegenContext(std::shared_ptr<llvm::LLVMContext> context, std::shared_ptr<llvm::Module> module)
		: m_Builder(std::make_shared<llvm::IRBuilder<>>(*context))
		, m_Module(module)
	{
	}
} // namespace K::Codegen
