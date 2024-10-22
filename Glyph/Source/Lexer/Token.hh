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
			Func,		// fun
			Identifier, // [a-zA-Z][a-zA-Z0-9]*
			Number,		// [0-9]+

			// Operators
			Plus,		  // +
			Minus,		  // -
			Star,		  // *
			Slash,		  // /
			Less,		  // <
			Greater,	  // >
			Equal,		  // ==
			NotEqual,	  // !=
			LessEqual,	  // <=
			GreaterEqual, // >=
			Assign,		  // =

			// Punctuation
			LParen,		 // (
			RParen,		 // )
			Semicolon,	 // ;
			Comma,		 // ,
			LBrace,		 // {
			RBrace,		 // }
			Arrow,		 // ->
			Rocket,		 // <->
			Caret,		 // ^
			At,			 // @
			Conditional, // ?

			// Unknown
			Unknown
		};

	  public:
		Token(ID id, const std::string& lexeme)
			: m_id(id)
			, lexeme(lexeme)
		{
		}

		ID GetID() const { return m_id; }
		const std::string& GetLexeme() const { return lexeme; }

		double GetNumber() const { return std::stod(lexeme); }

	  private:
		ID m_id;
		std::string lexeme;
	};

	using Tokens = std::vector<Token>;
} // namespace Glyph

std::ostream& operator<<(std::ostream& out, const Glyph::Token& token);