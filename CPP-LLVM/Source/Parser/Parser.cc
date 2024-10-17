#include <AST/AST.hh>
#include <Lexer/Tokens.hh>
#include <Parser/Diagnostician.hh>
#include <Parser/Parser.hh>
#include <PtrUtils.hh>

#include <format>
#include <iostream>

namespace K
{
	Parser::Parser(Tokens tokens)
		: m_Tokens(tokens)
		, m_Diagnostician(std::make_shared<Diagnostician>())
	{
	}

	AST::ProgramPtr Parser::Parse()
	{
		std::vector<AST::ExpressionPtr> expressions;

		while(!IsAtEnd())
		{
			auto t = PeekToken();

			switch(t.Id)
			{
				case TokenID::Def: expressions.push_back(ParseDefinition()); break;
				case TokenID::Extern: expressions.push_back(ParseExtern()); break;
				case TokenID::EOL: AdvanceToken(); break;
				default: expressions.push_back(ParseTopLevelExpression()); break;
			}
		}

		return std::make_unique<AST::Program>(std::move(expressions));
	}

	AST::ExpressionPtr Parser::ParseParenExpression()
	{
		Consume(TokenID::LParen, "Expected '('");

		auto expr = ParseExpression();
		if(!expr)
		{
			return nullptr;
		}

		Consume(TokenID::RParen, "Expected ')'");

		return expr;
	}

	AST::ExpressionPtr Parser::ParseNumberExpression()
	{
		auto token = Consume(TokenID::Number, "Expected number");

		return CreateASTNode<AST::NumberExpression>(token.Value);
	}

	AST::ExpressionPtr Parser::ParseIdentifierExpression()
	{
		auto token = Consume(TokenID::Identifier, "Expected identifier");
		auto id = token.SafeLiteral();

		if(!CheckToken(TokenID::LParen))
		{
			return CreateASTNode<AST::VariableExpression>(id);
		}

		AdvanceToken(); // (

		std::vector<AST::ExpressionPtr> args;
		if(CheckToken(TokenID::RParen))
		{
			AdvanceToken();
		}
		else
		{
			while(true)
			{
				args.push_back(ParseExpression());

				if(CheckToken(TokenID::RParen))
				{
					AdvanceToken(); // )
					break;
				}

				Consume(TokenID::Comma, "Expected ','");
			}
		}

		return CreateASTNode<AST::CallExpression>(id, std::move(args));
	}

	AST::ExpressionPtr Parser::ParsePrimary()
	{
		auto token = GetToken();

		switch(token.Id)
		{
			case TokenID::Identifier: return ParseIdentifierExpression();
			case TokenID::Number: return ParseNumberExpression();
			case TokenID::LParen: return ParseParenExpression();
			default:
				auto error = std::format("Unknown token when expecting an expression: {}", token);

				m_Diagnostician->Report("Parser", error, token.Line, token.Column);
				throw std::runtime_error(error);
		}
	}

	AST::ExpressionPtr Parser::ParseExpression()
	{
		auto lhs = ParsePrimary();
		if(!lhs)
		{
			return nullptr;
		}

		return ParseBinOpRHS(0, std::move(lhs));
	}

	AST::ExpressionPtr Parser::ParseBinOpRHS(int exprPrec, AST::ExpressionPtr lhs)
	{
		while(true)
		{
			auto token = GetToken();

			if(token.Id == TokenID::EOL || token.Id == TokenID::RParen)
			{
				return lhs;
			}

			auto tokenPrec = GetPrecedence(token.Id);

			if(tokenPrec < exprPrec)
			{
				return lhs;
			}

			AdvanceToken();

			auto rhs = ParsePrimary();
			if(!rhs)
			{
				return nullptr;
			}

			auto nextPrec = GetPrecedence(GetToken().Id);

			if(tokenPrec < nextPrec)
			{
				rhs = ParseBinOpRHS(tokenPrec + 1, std::move(rhs));
				if(!rhs)
				{
					return nullptr;
				}
			}

			lhs = CreateASTNode<AST::BinaryExpression>(token.Id, std::move(lhs), std::move(rhs));
		}
	}

	AST::ExpressionPtr Parser::ParsePrototype()
	{
		auto id = Consume(TokenID::Identifier, "Expected function name");
		auto name = id.SafeLiteral();

		Consume(TokenID::LParen, "Expected '('");

		std::vector<std::string> args;

		if(!CheckToken(TokenID::RParen))
		{
			while(CheckToken(TokenID::Identifier))
			{
				auto id = AdvanceToken();

				args.push_back(id.SafeLiteral());

				if(CheckToken(TokenID::RParen))
				{
					break;
				}

				Consume(TokenID::Comma, "Expected ','");
			}
		}

		Consume(TokenID::RParen, "Expected ')'");

		return CreateASTNode<AST::PrototypeStatement>(name, std::move(args));
	}

	AST::ExpressionPtr Parser::ParseDefinition()
	{
		Consume(TokenID::Def, "Expected 'def'");

		auto prototype = ParsePrototype();
		if(!prototype)
		{
			return nullptr;
		}

		auto body = ParseExpression();
		if(!body)
		{
			return nullptr;
		}

		auto prototypeStatement = std::dynamic_pointer_cast<AST::PrototypeStatement>(prototype);
		return CreateASTNode<AST::FunctionStatement>(std::move(prototypeStatement), std::move(body));
	}

	AST::ExpressionPtr Parser::ParseExtern()
	{
		Consume(TokenID::Extern, "Expected 'extern'");

		return ParsePrototype();
	}

	AST::ExpressionPtr Parser::ParseTopLevelExpression()
	{
		auto expr = ParseExpression();
		if(!expr)
		{
			return nullptr;
		}

		auto prototype = CreateASTNode<AST::PrototypeStatement>("", std::vector<std::string>());

		return CreateASTNode<AST::FunctionStatement>(std::move(prototype), std::move(expr));
	}

	bool Parser::CheckToken(TokenID token) const { return GetToken().Id == token; }

	Token Parser::PeekToken() const { return m_Tokens[m_CurrentToken]; }

	Token Parser::AdvanceToken() { return m_Tokens[m_CurrentToken++]; }

	Token Parser::GetToken() const { return m_Tokens[m_CurrentToken]; }

	Token Parser::Consume(TokenID token, const std::string& message)
	{
		if(CheckToken(token))
		{
			return AdvanceToken();
		}

		auto tokenData = GetToken();

		m_Diagnostician->Report("Parser", message, tokenData.Line, tokenData.Column);
		throw std::runtime_error(message);
	}

	bool Parser::IsAtEnd() const { return m_CurrentToken >= m_Tokens.size(); }

	int Parser::GetPrecedence(TokenID op)
	{
		if(m_BinOpPrecedence.find(op) == m_BinOpPrecedence.end())
		{
			return -1;
		}

		return m_BinOpPrecedence[op];
	}
} // namespace K
