#pragma once

#include <Parser/Diagnostic.hh>
#include <Parser/IDiagnostician.hh>

namespace K
{
	class Diagnostician : public IDiagnostician
	{
	  public:
		void Report(const Diagnostic& diagnostic) override;
		void Report(const std::string& code, const std::string& message, std::size_t line, std::size_t column) override;

		void ForEachError(std::function<void(const Diagnostic&)> callback) override;

	  private:
		std::vector<Diagnostic> m_Diagnostics;
	};
} // namespace K
