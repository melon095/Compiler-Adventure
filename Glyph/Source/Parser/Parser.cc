#include <Parser/Parser.hh>

namespace Glyph
{
	Parser::Parser(Tokens tokens)
		: m_Tokens(tokens)
		, m_Current(0)
	{
	}

	ProgramNodePtr Parser::ParseProgram()
	{
		auto program = ProgramNodePtr(new ProgramNode());

		while(!IsAtEnd())
		{
			program->AddStatement(Statement());
		}

		return program;
	}

	StatementNodePtr Parser::Statement()
	{
		if(CheckToken(Token::ID::Func))
		{
			return FunctionDefinition();
		}
		else if(CheckToken(Token::ID::Identifier))
		{
			return ExpressionStatement();
		}
		else if(CheckToken(Token::ID::Caret))
		{
			return Loop();
		}
		else if(CheckToken(Token::ID::LBrace))
		{
			return Block();
		}
		else
		{
			return ExpressionStatement();
		}
	}

	// 'fun' Identifier '(' parameters? ')' '->' block
	StatementNodePtr Parser::FunctionDefinition()
	{
		auto token = Consume(Token::ID::Func, "Expected 'fun' keyword");

		auto prototype = Prototype();

		Consume(Token::ID::Arrow, "Expected '->' token");

		auto block = Block();

		return CreateASTNode<FunctionDeclarationNode>(prototype, block);
	}

	// expression SEMICOLON
	StatementNodePtr Parser::ExpressionStatement()
	{
		auto expression = Expression();

		Consume(Token::ID::Semicolon, "Expected ';' token");

		return CreateASTNode<ExpressionStatementNode>(expression);
	}

	// expression '?' '->' expression ('->' expression)?
	StatementNodePtr Parser::Conditional() { throw std::runtime_error("Not implemented"); }

	// '^' expression block
	StatementNodePtr Parser::Loop() { throw std::runtime_error("Not implemented"); }

	// expression
	ExpressionNodePtr Parser::Expression() { return Assignment(); }

	// Identifier '->' expression
	ExpressionNodePtr Parser::Assignment()
	{
		if(CheckToken(Token::ID::At))
		{
			return VariableDeclaration();
		}

		return BinaryOperation();
	}

	// expression operator expression
	ExpressionNodePtr Parser::BinaryOperation()
	{
		auto left = UnaryOperation();

		while(CheckToken(Token::ID::Plus) || CheckToken(Token::ID::Minus) || CheckToken(Token::ID::Star)
			  || CheckToken(Token::ID::Slash) || CheckToken(Token::ID::Less) || CheckToken(Token::ID::Greater)
			  || CheckToken(Token::ID::Equal) || CheckToken(Token::ID::NotEqual) || CheckToken(Token::ID::LessEqual)
			  || CheckToken(Token::ID::GreaterEqual))
		{
			auto token = AdvanceToken();

			auto right = UnaryOperation();

			left = CreateASTNode<BinaryExpressionNode>(left, token.GetLexeme(), right);
		}

		return left;
	}

	// operator expression
	ExpressionNodePtr Parser::UnaryOperation()
	{
		if(CheckToken(Token::ID::Plus) || CheckToken(Token::ID::Minus))
		{
			auto token = AdvanceToken();

			auto expression = UnaryOperation();

			return CreateASTNode<UnaryExpressionNode>(token.GetLexeme(), expression);
		}

		return Primary();
	}

	// primary
	ExpressionNodePtr Parser::Primary()
	{
		if(CheckToken(Token::ID::Number))
		{
			return Literal();
		}
		else if(CheckToken(Token::ID::Identifier))
		{
			return FunctionCall();
		}
		else if(CheckToken(Token::ID::LParen))
		{
			AdvanceToken();

			auto expression = Expression();

			Consume(Token::ID::RParen, "Expected ')' token");

			return expression;
		}

		throw std::runtime_error("Expected primary expression");
	}

	// '@' Identifier '->' expression SEMICOLON
	ExpressionNodePtr Parser::VariableDeclaration()
	{
		auto token = Consume(Token::ID::At, "Expected '@' token");

		auto identifier = Identifier();

		Consume(Token::ID::Arrow, "Expected '->' token");

		auto expression = Expression();

		Consume(Token::ID::Semicolon, "Expected ';' token");

		return CreateASTNode<VariableDeclarationNode>(identifier, expression);
	}

	// Identifier '(' arguments? ')'
	ExpressionNodePtr Parser::FunctionCall()
	{
		auto identifier = Identifier();

		Consume(Token::ID::LParen, "Expected '(' token");

		auto arguments = std::vector<ExpressionNodePtr>();

		if(!CheckToken(Token::ID::RParen))
		{
			do
			{
				arguments.push_back(Expression());
			} while(CheckToken(Token::ID::Comma));
		}

		Consume(Token::ID::RParen, "Expected ')' token");

		return CreateASTNode<FunctionCallNode>(identifier, arguments);
	}

	ExpressionNodePtr Parser::Literal()
	{
		auto token = Consume(Token::ID::Number, "Expected number");

		return CreateASTNode<LiteralNode>(std::stoi(token.GetLexeme()));
	}

	// Identifier '(' parameters? ')'
	PrototypeNodePtr Parser::Prototype()
	{
		auto identifier = Identifier();

		Consume(Token::ID::LParen, "Expected '(' token");

		auto parameters = std::vector<std::string>();

		if(!CheckToken(Token::ID::RParen))
		{
			do
			{
				parameters.push_back(Consume(Token::ID::Identifier, "Expected identifier").GetLexeme());
			} while(CheckToken(Token::ID::Comma));
		}

		Consume(Token::ID::RParen, "Expected ')' token");

		return CreateASTNode<PrototypeNode>(identifier, parameters);
	}

	// '{' statement* '}'
	BlockNodePtr Parser::Block()
	{
		Consume(Token::ID::LBrace, "Expected '{' token");

		auto block = CreateASTNode<BlockNode>();

		while(!CheckToken(Token::ID::RBrace))
		{
			block->AddStatement(Statement());
		}

		Consume(Token::ID::RBrace, "Expected '}' token");

		return block;
	}

	// Identifier
	IdentifierNodePtr Parser::Identifier()
	{
		auto token = Consume(Token::ID::Identifier, "Expected identifier");

		return CreateASTNode<IdentifierNode>(token.GetLexeme());
	}

	bool Parser::CheckToken(Token::ID token) const
	{
		if(IsAtEnd())
		{
			return false;
		}

		return PeekToken().GetID() == token;
	}

	Token Parser::PeekToken() const
	{
		if(IsAtEnd())
		{
			return {Token::ID::EOI, ""};
		}

		return m_Tokens[m_Current];
	}

	Token Parser::AdvanceToken()
	{
		if(!IsAtEnd())
		{
			m_Current++;
		}

		return m_Tokens[m_Current - 1];
	}

	Token Parser::GetToken() const { return m_Tokens[m_Current - 1]; }

	Token Parser::Consume(Token::ID token, const std::string& message)
	{
		if(CheckToken(token))
		{
			return AdvanceToken();
		}

		throw std::runtime_error(message);
	}

	bool Parser::IsAtEnd() const { return m_Current >= m_Tokens.size(); }

} // namespace Glyph