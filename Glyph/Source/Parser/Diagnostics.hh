#pragma once

#include <functional>
#include <string>
#include <vector>

namespace Glyph
{
	class Diagnostics
	{
	  public:
		Diagnostics() = default;
		~Diagnostics() = default;

		void Error(const std::string& message, int line, int column);
		void Warning(const std::string& message, int line, int column);
		void Info(const std::string& message, int line, int column);

		void ForEachError(const std::function<void(const std::string&, int, int)>& callback) const;

		bool HasErrors() const { return !m_Errors.empty(); }

	  private:
		struct Diagnostic
		{
			std::string message;
			int line;
			int column;
		};

		std::vector<Diagnostic> m_Errors;
	};
} // namespace Glyph