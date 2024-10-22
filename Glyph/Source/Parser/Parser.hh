#pragma once

#include <AST/AST.hh>
#include <Lexer/Token.hh>

namespace Glyph
{
	class Parser
	{
	  public:
		Parser(Tokens tokens);

		ProgramNodePtr ParseProgram();

	  private:
		StatementNodePtr Statement();
		StatementNodePtr FunctionDefinition();
		StatementNodePtr ExpressionStatement();
		StatementNodePtr Conditional();
		StatementNodePtr Loop();

		ExpressionNodePtr Expression();
		ExpressionNodePtr Literal();
		ExpressionNodePtr Assignment();
		ExpressionNodePtr BinaryOperation();
		ExpressionNodePtr UnaryOperation();

		ExpressionNodePtr Primary();
		ExpressionNodePtr VariableDeclaration();
		ExpressionNodePtr FunctionCall();

		PrototypeNodePtr Prototype();
		BlockNodePtr Block();
		IdentifierNodePtr Identifier();

	  private:
		bool CheckToken(Token::ID token) const;
		Token PeekToken() const;
		Token AdvanceToken();
		Token GetToken() const;
		Token Consume(Token::ID token, const std::string& message = "");
		bool IsAtEnd() const;

		template<typename TClass, typename... TArgs>
		std::shared_ptr<TClass> CreateASTNode(TArgs&&... args) requires std::derived_from<TClass, AstNode>
		{
			auto node = std::make_shared<TClass>(std::forward<TArgs>(args)...);

			// auto token = GetToken();

			// node->SetDebugInfo(token.Line, token.Column);

			return node;
		}

	  private:
		Tokens m_Tokens;
		std::size_t m_Current;
	};
} // namespace Glyph