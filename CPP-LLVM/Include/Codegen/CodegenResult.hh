#pragma once

#include <IDumpable.hh>

#include <memory>
#include <string>

namespace K::AST
{
	class BaseExpression;
};

namespace K::Codegen
{

	class CodegenResult : public IDumpable
	{
	  public:
		static CodegenResult Ok(std::shared_ptr<const AST::BaseExpression> expr) { return CodegenResult(expr); }
		static CodegenResult Error(std::shared_ptr<const AST::BaseExpression> expr, const std::string& message)
		{
			return CodegenResult(expr, message);
		}
		static CodegenResult NotImplemented(std::shared_ptr<const AST::BaseExpression> expr)
		{
			return CodegenResult(expr, "!!!! NOT IMPLEMENTED !!!!");
		}

	  public:
		CodegenResult(std::shared_ptr<const AST::BaseExpression> expr);
		CodegenResult(std::shared_ptr<const AST::BaseExpression> expr, const std::string& message);

		bool IsOk() const;

		void Dump(int indent, std::ostream& os) const override;

	  private:
		std::string m_ErrorMessage;
		std::shared_ptr<const AST::BaseExpression> m_Expression;
	};
} // namespace K::Codegen
