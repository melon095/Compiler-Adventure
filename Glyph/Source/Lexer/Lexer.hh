#pragma once

#include <Lexer/Token.hh>

#include <string>

namespace Glyph
{
	class Lexer
	{
	  public:
		Lexer(const std::string& input);

		Tokens Scan();

	  private:
		void ScanToken();
		void HandleNumber();
		void HandleIdentifier();

		void AddToken(Token::ID token);
		void AddToken(Token::ID token, const std::string& literal);

		bool IsAtEnd() const;
		bool Match(char expected);

		char Advance();
		char Peek() const;
		char PeekNext() const;

		std::string GetLexemeSubstring() const;

		static bool IsDigit(char c);
		static bool IsAlpha(char c);
		static bool IsAlphaNumeric(char c);

	  private:
		std::size_t m_Start, m_Current, m_Line, m_Column;
		std::string m_Source;
		Tokens m_Tokens;
	};
} // namespace Glyph