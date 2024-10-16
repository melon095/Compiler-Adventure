#include <AST/FunctionStatement.hh>

namespace K::AST
{
	FunctionStatement::FunctionStatement(PrototypeStatementPtr prototype, ExpressionPtr body)
		: Prototype(std::move(prototype))
		, Body(std::move(body))
	{
	}

	void FunctionStatement::Dump(int indent, std::ostream& os) const
	{
		os << std::string(indent, ' ') << "FunctionStatement" << std::endl;

		Prototype->Dump(indent + 2, os);
		Body->Dump(indent + 2, os);
	}
} // namespace K::AST