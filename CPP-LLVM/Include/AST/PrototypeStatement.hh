#pragma once

#include <AST/BaseExpression.hh>

#include <ostream>
#include <string>
#include <vector>

namespace K::AST
{
	class PrototypeStatement : public BaseExpression
	{
	  public:
		PrototypeStatement(const std::string& name, std::vector<std::string> args);

		void Dump(int indent, std::ostream& os) const override;
		Codegen::CodegenResult Codegen(Codegen::CodegenContextPtr context) const override;

	  public:
		std::string Name;
		std::vector<std::string> Args;
	};

	SMARTPTR(PrototypeStatement)
} // namespace K::AST
