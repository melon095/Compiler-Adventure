#include <Lexer/Token.hh>

#include <magic_enum.hpp>

std::ostream& operator<<(std::ostream& out, const Glyph::Token& token)
{
	out << "Token(" << magic_enum::enum_name(token.GetID()) << ", ";
	if(token.GetID() == Glyph::Token::ID::Number)
	{
		out << token.GetNumber();
	}
	else if(token.GetLexeme().empty())
	{
		out << "''";
	}
	else
	{
		out << token.GetLexeme();
	}

	out << ")";

	return out;
}