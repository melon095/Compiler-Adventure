#include <Lexer/Tokens.hh>

#include <magic_enum.hpp>

std::ostream& operator<<(std::ostream& os, const K::Token& token)
{
	os << "Token(" << magic_enum::enum_name(token.Id) << ", " << token.Line;

	if(token.Literal)
	{
		os << ", " << *token.Literal;
	}

	if(token.Value)
	{
		os << ", " << token.Value;
	}

	os << ")";

	return os;
}
