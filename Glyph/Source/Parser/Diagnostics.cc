#include <Parser/Diagnostics.hh>

namespace Glyph
{
	void Diagnostics::Error(const std::string& message, int line, int column)
	{
		m_Errors.push_back({message, line, column});
	}

	void Diagnostics::Warning(const std::string& message, int line, int column)
	{
		m_Errors.push_back({message, line, column});
	}

	void Diagnostics::Info(const std::string& message, int line, int column)
	{
		m_Errors.push_back({message, line, column});
	}

	void Diagnostics::ForEachError(const std::function<void(const std::string&, int, int)>& callback) const
	{
		for(const auto& error : m_Errors)
		{
			callback(error.message, error.line, error.column);
		}
	}
} // namespace Glyph