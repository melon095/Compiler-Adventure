#pragma once

#include "ILexer.hh"

#include <cstddef>
#include <string>

namespace K
{
	class Lexer : public ILexer
	{
	  public:
		Lexer(const std::string& input);

		Tokens Scan() override;

	  private:
		void ScanToken();
		void HandleNumber();
		void HandleIdentifier();

		void AddToken(TokenID token);
		void AddToken(TokenID token, double value);
		void AddToken(TokenID token, const std::string& literal);
		void AddToken(TokenID token, const std::string& literal, double value);

		bool IsAtEnd() const;
		bool Match(char expected);

		char Advance();
		char Peek() const;
		char PeekNext() const;

		static bool IsDigit(char c);
		static bool IsAlpha(char c);
		static bool IsAlphaNumeric(char c);

	  private:
		std::size_t m_Start, m_Current, m_Line, m_Column;
		std::string m_Source;
		Tokens m_Tokens;
	};
} // namespace K