#pragma once

#include <AST/AST.hh>
#include <Lexer/Token.hh>
#include <Parser/Diagnostics.hh>

namespace Glyph
{
	class Parser
	{
	  public:
		Parser(Tokens tokens);

		ProgramNodePtr ParseProgram();

		const Diagnostics& GetDiagnostics() const { return m_Diagnostics; }

	  private:
		StatementNodePtr ParseStatement();
		StatementNodePtr ParseLet();
		StatementNodePtr ParseReturn();
		StatementNodePtr ParseExpressionStatement();
		ExpressionNodePtr ParseExpression();
		FunctionDeclarationNodePtr ParseFunction(IdentifierNodePtr identifier);
		ExpressionNodePtr ParsePrimary();
		ExpressionNodePtr ParseBinary(ExpressionNodePtr lhs, int precedence);
		FunctionCallNodePtr ParseCall(IdentifierNodePtr callee);
		MatchExpressionNodePtr ParseMatch();
		IfExpressionNodePtr ParseIf();
		BlockNodePtr ParseBlock();
		IdentifierNodePtr ParseIdentifier();
		LiteralNodePtr ParseLiteral();

		PrototypeNodePtr ParsePrototype(IdentifierNodePtr name);

	  private:
		int GetPrecedence(Token::ID token) const;
		bool CheckToken(Token::ID token) const;
		Token PeekToken() const;
		Token AdvanceToken();
		Token GetToken() const;
		Token Consume(Token::ID token, const std::string& message = "");
		bool IsAtEnd() const;

		void ReportError(const std::string& message);

		template<typename TClass, typename... TArgs>
		std::shared_ptr<TClass> CreateASTNode(TArgs&&... args) requires std::derived_from<TClass, AstNode>
		{
			auto node = std::make_shared<TClass>(std::forward<TArgs>(args)...);

			// auto token = GetToken();

			// node->SetDebugInfo(token.Line, token.Column);

			return node;
		}

	  private:
		Diagnostics m_Diagnostics;
		Tokens m_Tokens;
		std::size_t m_Current;
	};
} // namespace Glyph