#include <Lexer/Lexer.hh>

namespace K
{
	Lexer::Lexer(const std::string& input)
		: m_Start(0)
		, m_Current(0)
		, m_Line(1)
		, m_Column(1)
		, m_Source(input)
	{
	}

	Tokens Lexer::Scan()
	{
		while(!IsAtEnd())
		{
			m_Start = m_Current;

			ScanToken();
		}

		AddToken(TokenID::EOL);

		return m_Tokens;
	}

	void Lexer::ScanToken()
	{
		auto c = Advance();

		if(isspace(c))
		{
			if(c == '\n')
			{
				m_Line++;
				m_Column = 1;
			}
		}
		else if(IsAlpha(c))
		{
			HandleIdentifier();
		}
		else if(IsDigit(c))
		{
			HandleNumber();
		}
		else if(c == '#')
		{
			while(Peek() != '\n' && !IsAtEnd())
			{
				Advance();
			}
		}
		else
		{
			switch(c)
			{
				case '(': AddToken(TokenID::LParen); break;
				case ')': AddToken(TokenID::RParen); break;
				case ',': AddToken(TokenID::Comma); break;
				case ';': AddToken(TokenID::EOL); break;
				case '+': AddToken(TokenID::Plus); break;
				case '-': AddToken(TokenID::Minus); break;
				case '*': AddToken(TokenID::Star); break;
				case '/': AddToken(TokenID::Slash); break;
				case '<': AddToken(TokenID::Less); break;
				case '=':
					if(Match('='))
					{
						AddToken(TokenID::EqualEqual);
					}
					else
					{
						AddToken(TokenID::Equal);
					}
					break;
				default: throw std::runtime_error("Unexpected character");
			}
		}
	}

	void Lexer::HandleNumber()
	{
		while(IsDigit(Peek()))
		{
			Advance();
		}

		if(Peek() == '.')
		{
			Advance();

			while(IsDigit(Peek()))
			{
				Advance();
			}
		}

		auto number = m_Source.substr(m_Start, m_Current - m_Start);

		double value = std::stod(number);

		AddToken(TokenID::Number, number, value);
	}

	void Lexer::HandleIdentifier()
	{
		while(IsAlphaNumeric(Peek()))
		{
			Advance();
		}

		auto identifier = m_Source.substr(m_Start, m_Current - m_Start);

		if(identifier == "def")
		{
			AddToken(TokenID::Def);
		}
		else if(identifier == "extern")
		{
			AddToken(TokenID::Extern);
		}
		else
		{
			AddToken(TokenID::Identifier, identifier);
		}
	}

	void Lexer::AddToken(TokenID token)
	{
		m_Tokens.push_back({
			.Id = token,
			.Line = m_Line,
			.Column = m_Column,
		});
	}

	void Lexer::AddToken(TokenID token, double value)
	{
		m_Tokens.push_back({.Id = token, .Value = value, .Line = m_Line, .Column = m_Column});
	}

	void Lexer::AddToken(TokenID token, const std::string& literal)
	{
		m_Tokens.push_back({.Id = token, .Literal = literal, .Line = m_Line, .Column = m_Column});
	}

	void Lexer::AddToken(TokenID token, const std::string& literal, double value)
	{
		m_Tokens.push_back({.Id = token, .Literal = literal, .Value = value, .Line = m_Line, .Column = m_Column});
	}

	bool Lexer::IsAtEnd() const { return m_Current >= m_Source.size(); }

	bool Lexer::Match(char expected)
	{
		if(IsAtEnd())
		{
			return false;
		}

		if(m_Source[m_Current] != expected)
		{
			return false;
		}

		m_Current++;

		return true;
	}

	char Lexer::Advance()
	{
		m_Current++;
		m_Column++;

		return m_Source[m_Current - 1];
	}

	char Lexer::Peek() const
	{
		if(IsAtEnd())
		{
			return '\0';
		}

		return m_Source[m_Current];
	}

	char Lexer::PeekNext() const
	{
		if(m_Current + 1 >= m_Source.size())
		{
			return '\0';
		}

		return m_Source[m_Current + 1];
	}

	bool Lexer::IsDigit(char c) { return c >= '0' && c <= '9'; }

	bool Lexer::IsAlpha(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'; }

	bool Lexer::IsAlphaNumeric(char c) { return IsAlpha(c) || IsDigit(c); }
} // namespace K