#pragma once

#include <AST/Program.hh>
#include <Parser/IDiagnostician.hh>

namespace K
{
	class IParse
	{
	  public:
		virtual ~IParse() = default;

		virtual AST::ProgramPtr Parse() = 0;
		virtual std::shared_ptr<IDiagnostician> GetDiagnostician() = 0;
	};
} // namespace K