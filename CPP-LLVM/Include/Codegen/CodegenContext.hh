#pragma once

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>
#include <map>
#include <memory>
#include <string>

namespace K::Codegen
{
	class CodegenContext
	{
	  public:
		CodegenContext(std::shared_ptr<llvm::LLVMContext> context, std::shared_ptr<llvm::Module> module);

		std::shared_ptr<llvm::IRBuilder<>> GetBuilder() const { return m_Builder; }
		std::shared_ptr<llvm::Module> GetModule() const { return m_Module; }
		llvm::LLVMContext& GetContext() const { return m_Builder->getContext(); }

	  private:
		std::shared_ptr<llvm::IRBuilder<>> m_Builder;
		std::shared_ptr<llvm::Module> m_Module;
		std::map<std::string, llvm::Value*> m_NamedValues;
	};

	using CodegenContextPtr = std::shared_ptr<CodegenContext>;
} // namespace K::Codegen
