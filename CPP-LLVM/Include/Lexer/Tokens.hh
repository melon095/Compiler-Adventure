#pragma once

#include <magic_enum.hpp>

#include <format>
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

template<> struct std::formatter<K::TokenID> : std::formatter<std::string_view>
{
	template<typename FormatContext>
	[[nodiscard]]
	auto format(K::TokenID id, FormatContext& ctx)
	{
		return std::formatter<std::string_view>::format(magic_enum::enum_name(id), ctx);
	}
};

template<> struct std::formatter<K::Token> : std::formatter<std::string>
{
	template<typename FormatContext>
	[[nodiscard]]
	auto format(const K::Token& token, FormatContext& ctx) const
	{
		return std::format_to(ctx.out(), "Token(Id: {}, Literal: {}, Value: {}, Line: {}, Column: {})",
							  magic_enum::enum_name(token.Id), token.SafeLiteral(), token.Value, token.Line,
							  token.Column);
	}
};
