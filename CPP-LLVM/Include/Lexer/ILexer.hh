#pragma once

#include "Tokens.hh"

#include <string>
#include <vector>

namespace K
{
	typedef std::vector<Token> Tokens;

	class ILexer
	{
	  public:
		virtual ~ILexer() = default;

		virtual Tokens Scan() = 0;
	};
} // namespace K
