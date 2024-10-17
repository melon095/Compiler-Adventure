#pragma once

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>
#include <map>
#include <memory>
#include <stack>
#include <string>

namespace K::Codegen
{
	using ValueStack = std::stack<llvm::Value*>;

	class CodegenContext
	{
	  public:
		CodegenContext(std::shared_ptr<llvm::LLVMContext> context, llvm::Module* module);
		// No destructor, let m_Module be leaky.

		std::shared_ptr<llvm::IRBuilder<>> GetBuilder() const { return m_Builder; }
		llvm::Module* GetModule() const { return m_Module; }
		llvm::LLVMContext& GetContext() const { return m_Builder->getContext(); }
		ValueStack& GetValueStack() { return m_ValueStack; }
		std::map<std::string, llvm::Value*>& GetNamedValues() { return m_NamedValues; }

		llvm::Value* GetPopValue()
		{
			auto* value = m_ValueStack.top();
			m_ValueStack.pop();
			return value;
		}

	  private:
		std::shared_ptr<llvm::IRBuilder<>> m_Builder;
		llvm::Module* m_Module;
		std::map<std::string, llvm::Value*> m_NamedValues;
		ValueStack m_ValueStack;
	};

	using CodegenContextPtr = std::shared_ptr<CodegenContext>;
} // namespace K::Codegen
