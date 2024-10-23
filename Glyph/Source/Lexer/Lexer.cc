#include <Lexer/Lexer.hh>

#include <map>

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
			case '(': AddToken(Token::ID::LeftParen); break;
			case ')': AddToken(Token::ID::RightParen); break;
			case '{': AddToken(Token::ID::LeftBrace); break;
			case '}': AddToken(Token::ID::RightBrace); break;
			case ',': AddToken(Token::ID::Comma); break;
			case '.': AddToken(Token::ID::Dot); break;
			case '-': AddToken(Match('>') ? Token::ID::Arrow : Token::ID::Minus); break;
			case '+': AddToken(Token::ID::Plus); break;
			case ';': AddToken(Token::ID::Semicolon); break;
			case '*': AddToken(Token::ID::Asterisk); break;
			case '!': AddToken(Match('=') ? Token::ID::BangEqual : Token::ID::Bang); break;
			case '=': AddToken(Match('=') ? Token::ID::EqualEqual : Token::ID::Equal); break;
			case '<': AddToken(Match('=') ? Token::ID::LessEqual : Token::ID::Less); break;
			case '>': AddToken(Match('=') ? Token::ID::GreaterEqual : Token::ID::Greater); break;
			case '/':
				if(Match('/'))
				{
					while(Peek() != '\n' && !IsAtEnd())
						Advance();
				}
				else
				{
					AddToken(Token::ID::Slash);
				}
				break;
			case ' ':
			case '\r':
			case '\t': break;
			case '\n':
				m_Line++;
				m_Column = 1;
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
					AddToken(Token::ID::Unknown);
				}
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
		static std::map<std::string, Token::ID> keywords = {
			// clang-format off
            {"let", Token::ID::Let},
            {"return", Token::ID::Return},
            {"match", Token::ID::Match},
            {"if", Token::ID::If},
            {"else", Token::ID::Else},
            {"true", Token::ID::True},
            {"false", Token::ID::False},
			// clang-format on
		};

		while(IsAlphaNumeric(Peek()))
			Advance();

		std::string lexeme = GetLexemeSubstring();

		auto it = keywords.find(lexeme);
		if(it != keywords.end())
		{
			AddToken(it->second);
		}
		else
		{
			AddToken(Token::ID::Identifier, lexeme);
		}
	}

	void Lexer::AddToken(Token::ID token) { m_Tokens.emplace_back(token, "", m_Line, m_Column); }

	void Lexer::AddToken(Token::ID token, const std::string& literal)
	{
		m_Tokens.emplace_back(token, literal, m_Line, m_Column);
	}

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

	bool Lexer::IsAlpha(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'; }

	bool Lexer::IsAlphaNumeric(char c) { return IsAlpha(c) || IsDigit(c); }

} // namespace Glyph