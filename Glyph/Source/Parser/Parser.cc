#include <Lexer/Token.hh>
#include <Parser/Parser.hh>

#include <format>
#include <iostream>

#include <magic_enum/magic_enum.hpp>
#include <memory>
#include <utility>

namespace Glyph
{
	Parser::Parser(Tokens tokens)
		: m_Diagnostics()
		, m_Tokens(std::move(tokens))
		, m_Current(0)
	{
	}

	ProgramNodePtr Parser::ParseProgram()
	{
		auto program = std::make_shared<ProgramNode>();

		try
		{
			while(!IsAtEnd())
			{
				program->AddStatement(ParseStatement());
			}
		}
		catch(const std::exception& e)
		{
			(void)e;
		}

		return program;
	}

	StatementNodePtr Parser::ParseStatement()
	{
		if(CheckToken(Token::ID::Let))
		{
			return ParseLet();
		}

		if(CheckToken(Token::ID::Return))
		{
			return ParseReturn();
		}

		return ParseExpressionStatement();
	}

	StatementNodePtr Parser::ParseLet()
	{
		Consume(Token::ID::Let);

		auto identifier = ParseIdentifier();
		Consume(Token::ID::Equal, "Expected '=' after identifier");

		// TODO: Function declaration should not be determined within the ParseLet function.
		if(CheckToken(Token::ID::LeftParen))
		{
			auto func = ParseFunction(identifier);

			Consume(Token::ID::Semicolon, "Expected ';' after function declaration");

			return func;
		}

		auto expression = ParseExpression();
		Consume(Token::ID::Semicolon, "Expected ';' after expression");

		return CreateASTNode<LetDeclarationNode>(identifier, expression);
	}

	FunctionDeclarationNodePtr Parser::ParseFunction(IdentifierNodePtr identifier)
	{
		auto prototype = ParsePrototype(identifier);
		Consume(Token::ID::Arrow, "Expected '->' after function arguments");

		auto block = ParseBlock();

		return CreateASTNode<FunctionDeclarationNode>(prototype, block);
	}

	StatementNodePtr Parser::ParseReturn()
	{
		Consume(Token::ID::Return);

		auto expression = ParseExpression();
		Consume(Token::ID::Semicolon, "Expected ';' after expression");

		return CreateASTNode<ReturnStatementNode>(expression);
	}

	StatementNodePtr Parser::ParseExpressionStatement()
	{
		auto expression = ParseExpression();
		Consume(Token::ID::Semicolon, "Expected ';' after expression");

		return CreateASTNode<ExpressionStatementNode>(expression);
	}

	ExpressionNodePtr Parser::ParseExpression()
	{
		auto lhs = ParsePrimary();
		if(!lhs)
			return nullptr;

		return ParseBinary(lhs, 0);
	}

	ExpressionNodePtr Parser::ParsePrimary()
	{
		if(CheckToken(Token::ID::Identifier))
		{
			auto id = ParseIdentifier();

			if(CheckToken(Token::ID::LeftParen))
			{
				return ParseCall(id);
			}

			return id;
		}

		if(CheckToken(Token::ID::Number) || CheckToken(Token::ID::True) || CheckToken(Token::ID::False))
		{
			return ParseLiteral();
		}

		if(CheckToken(Token::ID::Match))
		{
			return ParseMatch();
		}

		if(CheckToken(Token::ID::If))
		{
			return ParseIf();
		}

		if(CheckToken(Token::ID::LeftBrace))
		{
			return ParseBlock();
		}

		ReportError("Primary - Unexpected token");

		return nullptr;
	}

	ExpressionNodePtr Parser::ParseBinary(ExpressionNodePtr lhs, int precedence)
	{
		while(true)
		{
			auto token = PeekToken();

			if(GetPrecedence(token.GetID()) <= precedence)
			{
				return (lhs);
			}

			AdvanceToken();

			auto op = CreateASTNode<OperatorNode>(token.GetLexeme());
			auto rhs = ParsePrimary();

			while(true)
			{
				auto nextToken = PeekToken();

				if(GetPrecedence(nextToken.GetID()) > GetPrecedence(token.GetID()))
				{
					rhs = ParseBinary(rhs, GetPrecedence(nextToken.GetID()));
				}
				else
				{
					break;
				}
			}

			lhs = CreateASTNode<ArithmeticExpressionNode>(lhs, op, rhs);
		}

		return nullptr;
	}

	FunctionCallNodePtr Parser::ParseCall(IdentifierNodePtr callee)
	{
		Consume(Token::ID::LeftParen);

		std::vector<ExpressionNodePtr> args;

		if(!CheckToken(Token::ID::RightParen))
		{
			while(!CheckToken(Token::ID::RightParen))
			{
				args.push_back(ParseExpression());

				if(CheckToken(Token::ID::Comma))
				{
					AdvanceToken();
				}
			}
		}

		if(CheckToken(Token::ID::RightParen))
		{
			Consume(Token::ID::RightParen);
		}

		return CreateASTNode<FunctionCallNode>(callee, args);
	}

	MatchExpressionNodePtr Parser::ParseMatch()
	{
		Consume(Token::ID::Match);

		auto expression = ParseExpression();
		Consume(Token::ID::LeftBrace);

		std::vector<MatchCaseNodePtr> cases;

		while(!CheckToken(Token::ID::RightBrace))
		{
			auto pattern = ParseExpression();
			Consume(Token::ID::Arrow, "Expected '->' after match pattern");
			auto expr = ParseExpression();
			Consume(Token::ID::Semicolon, "Expected ';' after match case");

			cases.push_back(CreateASTNode<MatchCaseNode>(pattern, expr));
		}

		Consume(Token::ID::RightBrace);

		return CreateASTNode<MatchExpressionNode>(expression, cases);
	}

	IfExpressionNodePtr Parser::ParseIf()
	{
		Consume(Token::ID::If);

		auto condition = ParseExpression();
		auto trueBranch = ParseBlock();

		if(CheckToken(Token::ID::Else))
		{
			AdvanceToken();
			auto falseBranch = ParseBlock();

			return CreateASTNode<IfExpressionNode>(condition, trueBranch, falseBranch);
		}

		return CreateASTNode<IfExpressionNode>(condition, trueBranch);
	}

	BlockNodePtr Parser::ParseBlock()
	{
		Consume(Token::ID::LeftBrace);

		auto block = CreateASTNode<BlockNode>();

		while(!CheckToken(Token::ID::RightBrace))
		{
			block->AddStatement(ParseStatement());
		}

		Consume(Token::ID::RightBrace);

		return block;
	}

	IdentifierNodePtr Parser::ParseIdentifier()
	{
		auto token = Consume(Token::ID::Identifier);

		return CreateASTNode<IdentifierNode>(token.GetLexeme());
	}

	LiteralNodePtr Parser::ParseLiteral()
	{
		auto token = AdvanceToken();

		if(token.GetID() == Token::ID::Number)
		{
			return CreateASTNode<LiteralNode>(std::stod(token.GetLexeme()));
		}

		if(token.GetID() == Token::ID::True)
		{
			return CreateASTNode<LiteralNode>(true);
		}

		if(token.GetID() == Token::ID::False)
		{
			return CreateASTNode<LiteralNode>(false);
		}

		ReportError("Literal - Unexpected token");

		return nullptr;
	}

	PrototypeNodePtr Parser::ParsePrototype(IdentifierNodePtr name)
	{
		std::vector<std::string> args;

		Consume(Token::ID::LeftParen);

		if(!CheckToken(Token::ID::RightParen))
		{
			while(!CheckToken(Token::ID::RightParen))
			{
				auto arg = Consume(Token::ID::Identifier).GetLexeme();
				args.push_back(arg);

				if(CheckToken(Token::ID::Comma))
				{
					AdvanceToken();
				}
			}
		}

		Consume(Token::ID::RightParen);

		return CreateASTNode<PrototypeNode>(name, args);
	}

	int Parser::GetPrecedence(Token::ID token) const
	{
		switch(token)
		{
			case Token::ID::Plus:
			case Token::ID::Minus: return 1;
			case Token::ID::Asterisk:
			case Token::ID::Slash: return 2;
			case Token::ID::Equal:
			case Token::ID::EqualEqual:
			case Token::ID::BangEqual:
			case Token::ID::Less:
			case Token::ID::LessEqual:
			case Token::ID::Greater:
			case Token::ID::GreaterEqual: return 3;
			case Token::ID::And:
			case Token::ID::Or: return 4;
			default: return 0;
		}
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
			return {Token::ID::EOI, "", 0, 0};
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

		if(message.empty())
			ReportError(std::format("Expected token '{}' but got '{}'", magic_enum::enum_name(token),
									magic_enum::enum_name(PeekToken().GetID())));
		else
			ReportError(message);

		throw std::runtime_error("Expected token");
	}

	bool Parser::IsAtEnd() const
	{
		return m_Current >= m_Tokens.size() || m_Tokens[m_Current].GetID() == Token::ID::EOI;
	}

	void Parser::ReportError(const std::string& message)
	{
		auto token = GetToken();
		m_Diagnostics.Error(message, token.GetLine(), token.GetColumn());
	}
} // namespace Glyph
