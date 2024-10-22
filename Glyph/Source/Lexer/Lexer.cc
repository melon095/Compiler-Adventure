#include <Lexer/Lexer.hh>

namespace Glyph
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

		AddToken(Token::ID::EOI);

		return m_Tokens;
	}

	void Lexer::ScanToken()
	{
		char c = Advance();

		switch(c)
		{
			case ' ':
			case '\r':
			case '\t': break;
			case '\n':
				m_Line++;
				m_Column = 1;
				break;

			case '+': AddToken(Token::ID::Plus); break;
			case '-':
				{
					if(Match('>'))
					{
						AddToken(Token::ID::Arrow);
					}
					else
					{
						AddToken(Token::ID::Minus);
					}
					break;
				}
			case '*': AddToken(Token::ID::Star); break;
			case '/': AddToken(Token::ID::Slash); break;
			case '(': AddToken(Token::ID::LParen); break;
			case ')': AddToken(Token::ID::RParen); break;
			case '{': AddToken(Token::ID::LBrace); break;
			case '}': AddToken(Token::ID::RBrace); break;
			case ';': AddToken(Token::ID::Semicolon); break;
			case ',': AddToken(Token::ID::Comma); break;
			case '^': AddToken(Token::ID::Caret); break;
			case '@': AddToken(Token::ID::At); break;
			case '?': AddToken(Token::ID::Conditional); break;

			case '<':
				if(Match('-') && Match('>'))
				{
					AddToken(Token::ID::Rocket);
				}
				else if(Match('='))
				{
					AddToken(Token::ID::LessEqual);
				}
				else
				{
					AddToken(Token::ID::Less);
				}
				break;

			case '>':
				if(Match('='))
				{
					AddToken(Token::ID::GreaterEqual);
				}
				else
				{
					AddToken(Token::ID::Greater);
				}
				break;

			case '=':
				if(Match('='))
				{
					AddToken(Token::ID::Equal);
				}
				else
				{
					AddToken(Token::ID::Assign);
				}
				break;

			default:
				if(IsDigit(c))
				{
					HandleNumber();
				}
				else if(IsAlpha(c))
				{
					HandleIdentifier();
				}
				else
				{
					AddToken(Token::ID::Unknown, std::string(1, c));
				}
				break;
		}
	}

	void Lexer::HandleNumber()
	{
		while(IsDigit(Peek()))
			Advance();

		if(Peek() == '.' && IsDigit(PeekNext()))
		{
			Advance();

			while(IsDigit(Peek()))
				Advance();
		}

		AddToken(Token::ID::Number, GetLexemeSubstring());
	}

	void Lexer::HandleIdentifier()
	{
		while(IsAlphaNumeric(Peek()))
			Advance();

		auto lexeme = GetLexemeSubstring();

		if(lexeme == "fun")
			AddToken(Token::ID::Func);
		else
			AddToken(Token::ID::Identifier, lexeme);
	}

	void Lexer::AddToken(Token::ID token) { m_Tokens.emplace_back(token, ""); }

	void Lexer::AddToken(Token::ID token, const std::string& literal) { m_Tokens.emplace_back(token, literal); }

	bool Lexer::IsAtEnd() const { return m_Current >= m_Source.size(); }

	bool Lexer::Match(char expected)
	{
		if(IsAtEnd())
			return false;

		if(m_Source[m_Current] != expected)
			return false;

		m_Current++;
		m_Column++;

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
			return '\0';

		return m_Source[m_Current];
	}

	char Lexer::PeekNext() const
	{
		if(m_Current + 1 >= m_Source.size())
			return '\0';

		return m_Source[m_Current + 1];
	}

	std::string Lexer::GetLexemeSubstring() const { return m_Source.substr(m_Start, m_Current - m_Start); }

	bool Lexer::IsDigit(char c) { return c >= '0' && c <= '9'; }

	bool Lexer::IsAlpha(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }

	bool Lexer::IsAlphaNumeric(char c) { return IsAlpha(c) || IsDigit(c); }

} // namespace Glyph