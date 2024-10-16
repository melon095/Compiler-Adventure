#pragma once

#include <optional>
#include <ostream>
#include <string>

namespace K
{
	enum class TokenID
	{
		EOL,
		Def,
		Extern,
		Identifier,
		Number,
		RParen,
		LParen,
		Comma,
		Plus,
		Minus,
		Star,
		Slash,
		Less,
		EqualEqual,
		Equal,
		Unknown
	};

	struct Token
	{
		TokenID Id;
		std::optional<std::string> Literal = std::nullopt;
		double Value = 0;
		std::size_t Line = 0;
		std::size_t Column = 0;

		[[nodiscard]]

		std::string SafeLiteral() const
		{
			if(!Literal.has_value())
			{
				return "";
			}

			return Literal.value();
		}
	};

} // namespace K

std::ostream& operator<<(std::ostream& os, const K::Token& token);
