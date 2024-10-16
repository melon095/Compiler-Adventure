#include <AST/AST.hh>
#include <Lexer/ILexer.hh>
#include <Parser/IDiagnostician.hh>
#include <Parser/IParse.hh>

#include <map>

namespace K
{
	class Parser : public IParse
	{
	  private:
		constexpr static std::size_t MaxFunctionParameters = 255;

	  public:
		Parser(Tokens tokens);

		AST::ProgramPtr Parse() override;
		std::shared_ptr<IDiagnostician> GetDiagnostician() override { return m_Diagnostician; }

	  private:
		AST::ExpressionPtr ParseIdentifierExpression();
		AST::ExpressionPtr ParseNumberExpression();
		AST::ExpressionPtr ParseParenExpression();
		AST::ExpressionPtr ParsePrimary();
		AST::ExpressionPtr ParseBinOpRHS(int exprPrec, AST::ExpressionPtr lhs);
		AST::ExpressionPtr ParseExpression();
		AST::ExpressionPtr ParsePrototype();
		AST::ExpressionPtr ParseDefinition();
		AST::ExpressionPtr ParseExtern();
		AST::ExpressionPtr ParseTopLevelExpression();

		bool CheckToken(TokenID token) const;
		Token PeekToken() const;
		Token AdvanceToken();
		Token GetToken() const;
		Token Consume(TokenID token, const std::string& message = "");
		bool IsAtEnd() const;
		int GetPrecedence(TokenID op);

	  private:
		Tokens m_Tokens;
		std::size_t m_CurrentToken = 0;
		std::map<TokenID, int> m_BinOpPrecedence {
			{TokenID::Less, 10}, {TokenID::Plus, 20}, {TokenID::Minus, 20}, {TokenID::Star, 40}, {TokenID::Slash, 40},
		};
		std::shared_ptr<IDiagnostician> m_Diagnostician;
	};
} // namespace K
