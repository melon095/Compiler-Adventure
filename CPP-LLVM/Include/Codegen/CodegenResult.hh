#pragma once

#include <IDumpable.hh>

#include <llvm/IR/Value.h>

#include <memory>
#include <string>
#include <variant>

namespace K::AST
{
	class BaseExpression;
};

namespace K::Codegen
{
	using ErrorOrValue = std::variant<llvm::Value*, std::string>;

	class CodegenResult : public IDumpable
	{
	  public:
		static CodegenResult Ok(std::shared_ptr<const AST::BaseExpression> expr, llvm::Value* value)
		{
			return CodegenResult(expr, value);
		}
		static CodegenResult Error(std::shared_ptr<const AST::BaseExpression> expr, const std::string& message)
		{
			return CodegenResult(expr, message);
		}
		static CodegenResult NotImplemented(std::shared_ptr<const AST::BaseExpression> expr)
		{
			return CodegenResult(expr, "!!!! NOT IMPLEMENTED !!!!");
		}

	  public:
		CodegenResult(std::shared_ptr<const AST::BaseExpression> expr, llvm::Value* value);
		CodegenResult(std::shared_ptr<const AST::BaseExpression> expr, const std::string& message);

		bool IsOk() const { return std::holds_alternative<llvm::Value*>(m_Variant); }
		const std::string& GetErrorMessage() const { return std::get<std::string>(m_Variant); }
		llvm::Value* GetValue() { return std::get<llvm::Value*>(m_Variant); }

		void Dump(int indent, std::ostream& os) const override;

	  private:
		std::shared_ptr<const AST::BaseExpression> m_Expression;
		ErrorOrValue m_Variant;
	};
} // namespace K::Codegen

#define CODEGEN_CHECK(expr)                                                                                            \
	({                                                                                                                 \
		auto result = expr;                                                                                            \
		if(!result.IsOk()) [[unlikely]]                                                                                \
			return result;                                                                                             \
		result.GetValue();                                                                                             \
	})
