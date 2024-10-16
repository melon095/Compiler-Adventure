#include <AST/PrototypeStatement.hh>

namespace K::AST
{
	PrototypeStatement::PrototypeStatement(const std::string& name, std::vector<std::string> args)
		: Name(name)
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
} // namespace K::AST