#pragma once

#include <Parser/Diagnostic.hh>
#include <functional>
#include <string>

namespace K
{
	class IDiagnostician
	{
	  public:
		virtual ~IDiagnostician() = default;

		virtual void Report(const Diagnostic& diagnostic) = 0;
		virtual void Report(const std::string& code, const std::string& message, std::size_t line, std::size_t column)
			= 0;

		virtual void ForEachError(std::function<void(const Diagnostic&)> callback) = 0;
	};
} // namespace K
