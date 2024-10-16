#include <Parser/Diagnostician.hh>
#include <iostream>

namespace K
{
	void Diagnostician::Report(const Diagnostic& diagnostic)
	{
		Report(diagnostic.Code, diagnostic.Message, diagnostic.Line, diagnostic.Column);
	}

	void
	Diagnostician::Report(const std::string& code, const std::string& message, std::size_t line, std::size_t column)
	{
		m_Diagnostics.push_back({code, message, line, column});
	}

	void Diagnostician::ForEachError(std::function<void(const Diagnostic&)> callback)
	{
		for(const auto& diagnostic : m_Diagnostics)
		{
			callback(diagnostic);
		}
	}
} // namespace K

std::ostream& operator<<(std::ostream& os, const K::Diagnostic& diagnostic)
{
	os << "Diagnostic(" << diagnostic.Code << ", " << diagnostic.Message << ", " << diagnostic.Line << ", "
	   << diagnostic.Column << ")";

	return os;
}