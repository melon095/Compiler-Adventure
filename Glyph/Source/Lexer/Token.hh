#pragma once

#include <ostream>
#include <string>
#include <vector>

namespace Glyph
{
	class Token
	{
	  public:
		enum class ID
		{
			// End of file
			EOI,

			// Keywords
			Let,	// let
			Return, // return
			Match,	// match
			If,		// if
			Else,	// else

			// Literals
			Identifier, // [a-zA-Z_][a-zA-Z0-9_]*
			Number,		// [0-9]+
			True,		// true
			False,		// false

			// Operators
			Plus,		  // +
			Minus,		  // -
			Asterisk,	  // *
			Slash,		  // /
			Equal,		  // =
			EqualEqual,	  // ==
			Bang,		  // !
			BangEqual,	  // !=
			Less,		  // <
			LessEqual,	  // <=
			Greater,	  // >
			GreaterEqual, // >=
			And,		  // &&
			Or,			  // ||
			LeftParen,	  // (
			RightParen,	  // )
			LeftBrace,	  // {
			RightBrace,	  // }
			Comma,		  // ,
			Semicolon,	  // ;
			Arrow,		  // ->
			Dot,		  // .

			// Unknown
			Unknown
		};

	  public:
		Token(ID id, const std::string& lexeme, std::size_t line, std::size_t column)
			: m_id(id)
			, lexeme(lexeme)
			, line(line)
			, column(column)
		{
		}

		Token() = default;

		ID GetID() const { return m_id; }
		const std::string& GetLexeme() const { return lexeme; }
		std::size_t GetLine() const { return line; }
		std::size_t GetColumn() const { return column; }

		double GetNumber() const { return std::stod(lexeme); }

	  private:
		ID m_id;
		std::string lexeme;
		std::size_t line, column;
	};

	using Tokens = std::vector<Token>;

} // namespace Glyph

std::ostream& operator<<(std::ostream& out, const Glyph::Token& token);
