#pragma once

#include <memory>
#include <string>
#include <variant>
#include <vector>

namespace Glyph
{

#define AST_NODE_LIST(V)                                                                                               \
	V(ProgramNode)                                                                                                     \
	V(ExpressionNode)                                                                                                  \
	V(StatementNode)                                                                                                   \
	V(ExpressionStatementNode)                                                                                         \
	V(LetDeclarationNode)                                                                                              \
	V(PrototypeNode)                                                                                                   \
	V(FunctionDeclarationNode)                                                                                         \
	V(FunctionCallNode)                                                                                                \
	V(BlockNode)                                                                                                       \
	V(ArithmeticExpressionNode)                                                                                        \
	V(IfExpressionNode)                                                                                                \
	V(MatchExpressionNode)                                                                                             \
	V(MatchCaseNode)                                                                                                   \
	V(LambdaExpressionNode)                                                                                            \
	V(IdentifierNode)                                                                                                  \
	V(LiteralNode)                                                                                                     \
	V(OperatorNode)                                                                                                    \
	V(ArgumentListNode)                                                                                                \
	V(ReturnStatementNode)

#define DECLARE_ENUM_TYPE(name) name,
	enum class NodeType
	{
		AST_NODE_LIST(DECLARE_ENUM_TYPE)
	};
#undef DECLARE_ENUM_TYPE

// forward declare each node type
#define DECLARE_NODE_TYPE(name) class name;
	AST_NODE_LIST(DECLARE_NODE_TYPE)
#undef DECLARE_NODE_TYPE

// create unique_ptr for each node type
#define DECLARE_NODE_TYPE(name) using name##Ptr = std::shared_ptr<name>;
	AST_NODE_LIST(DECLARE_NODE_TYPE)
#undef DECLARE_NODE_TYPE

	/// @brief Base class for all AST nodes
	class AstNode
	{
	  public:
		virtual ~AstNode() = default;

		NodeType GetType() const;
		bool IsType(NodeType type) const;

		template<typename TClass> bool Is() const { return dynamic_cast<const TClass*>(this) != nullptr; }
		template<typename TClass> TClass* As() { return dynamic_cast<TClass*>(this); }

	  protected:
		AstNode(NodeType type);

	  private:
		NodeType m_type;
	};

	class ExpressionNode : public AstNode
	{
	  protected:
		ExpressionNode(NodeType type);
	};

	class StatementNode : public AstNode
	{
	  protected:
		StatementNode(NodeType type);
	};

	class ExpressionStatementNode : public StatementNode
	{
	  public:
		ExpressionStatementNode(ExpressionNodePtr expression);

		const ExpressionNodePtr& GetExpression() const;

	  private:
		ExpressionNodePtr m_expression;
	};

	class ProgramNode : public AstNode
	{
	  public:
		ProgramNode();
		ProgramNode(const std::vector<StatementNodePtr>& statements);

		void AddStatement(StatementNodePtr statement);
		std::vector<StatementNodePtr>& GetStatements();

	  private:
		std::vector<StatementNodePtr> m_statements;
	};

	class LetDeclarationNode : public StatementNode
	{
	  public:
		LetDeclarationNode(IdentifierNodePtr identifier, ExpressionNodePtr expression);

		const IdentifierNodePtr& GetIdentifier() const;
		const ExpressionNodePtr& GetExpression() const;

	  private:
		IdentifierNodePtr m_identifier;
		ExpressionNodePtr m_expression;
	};

	class PrototypeNode : public AstNode
	{
	  public:
		PrototypeNode(IdentifierNodePtr name, const std::vector<std::string>& args);

		const IdentifierNodePtr& GetName() const;
		const std::vector<std::string>& GetArgs() const;

	  private:
		IdentifierNodePtr m_name;
		std::vector<std::string> m_args;
	};

	class FunctionDeclarationNode : public StatementNode
	{
	  public:
		FunctionDeclarationNode(PrototypeNodePtr prototype, BlockNodePtr block);

		const PrototypeNodePtr& GetPrototype() const;
		const BlockNodePtr& GetBlock() const;

	  private:
		PrototypeNodePtr m_prototype;
		BlockNodePtr m_block;
	};

	class FunctionCallNode : public ExpressionNode
	{
	  public:
		FunctionCallNode(IdentifierNodePtr name, const std::vector<ExpressionNodePtr>& args);

		const IdentifierNodePtr& GetName() const;
		const std::vector<ExpressionNodePtr>& GetArgs() const;

	  private:
		IdentifierNodePtr m_name;
		std::vector<ExpressionNodePtr> m_args;
	};

	class BlockNode : public ExpressionNode
	{
	  public:
		BlockNode();
		BlockNode(const std::vector<StatementNodePtr>& statements);

		void AddStatement(StatementNodePtr statement);
		const std::vector<StatementNodePtr>& GetStatements() const;

	  private:
		std::vector<StatementNodePtr> m_statements;
	};

	class ArithmeticExpressionNode : public ExpressionNode
	{
	  public:
		ArithmeticExpressionNode(ExpressionNodePtr lhs, OperatorNodePtr op, ExpressionNodePtr rhs);

		const ExpressionNodePtr& GetLhs() const;
		const OperatorNodePtr& GetOp() const;
		const ExpressionNodePtr& GetRhs() const;

	  private:
		ExpressionNodePtr m_lhs;
		OperatorNodePtr m_op;
		ExpressionNodePtr m_rhs;
	};

	class IfExpressionNode : public ExpressionNode
	{
	  public:
		IfExpressionNode(ExpressionNodePtr condition, BlockNodePtr trueBranch, BlockNodePtr falseBranch);
		IfExpressionNode(ExpressionNodePtr condition, BlockNodePtr trueBranch);

		const ExpressionNodePtr& GetCondition() const;
		const BlockNodePtr& GetTrueBranch() const;
		const BlockNodePtr& GetFalseBranch() const;

	  private:
		ExpressionNodePtr m_condition;
		BlockNodePtr m_trueBranch;
		BlockNodePtr m_falseBranch;
	};

	class MatchExpressionNode : public ExpressionNode
	{
	  public:
		MatchExpressionNode(ExpressionNodePtr expression, const std::vector<MatchCaseNodePtr>& cases);

		const ExpressionNodePtr& GetExpression() const;
		const std::vector<MatchCaseNodePtr>& GetCases() const;

	  private:
		ExpressionNodePtr m_expression;
		std::vector<MatchCaseNodePtr> m_cases;
	};

	class MatchCaseNode : public AstNode
	{
	  public:
		MatchCaseNode(ExpressionNodePtr pattern, ExpressionNodePtr block);

		const ExpressionNodePtr& GetPattern() const;
		const ExpressionNodePtr& GetBlock() const;

	  private:
		ExpressionNodePtr m_pattern;
		ExpressionNodePtr m_block;
	};

	class LambdaExpressionNode : public ExpressionNode
	{
	  public:
		LambdaExpressionNode(const std::vector<std::string>& args, BlockNodePtr block);

		const std::vector<std::string>& GetArgs() const;
		const BlockNodePtr& GetBlock() const;

	  private:
		std::vector<std::string> m_args;
		BlockNodePtr m_block;
	};

	class IdentifierNode : public ExpressionNode
	{
	  public:
		IdentifierNode(const std::string& name);

		const std::string& GetName() const;

	  private:
		std::string m_name;
	};

	class LiteralNode : public ExpressionNode
	{
	  public:
		LiteralNode(const int value);
		LiteralNode(const double value);
		LiteralNode(const bool value);
		LiteralNode(const std::string& value);

		int GetInt() const;
		double GetDouble() const;
		bool GetBool() const;
		std::string GetString() const;

		bool IsInt() const;
		bool IsDouble() const;
		bool IsBool() const;
		bool IsString() const;

	  private:
		std::variant<int, double, bool, std::string> m_value;
	};

	class OperatorNode : public AstNode
	{
	  public:
		enum class Operator
		{
			Plus,
			Minus,
			Star,
			Slash,
			Less,
			Greater,
			Equal,
			NotEqual,
			LessEqual,
			GreaterEqual,
			Assign
		};

	  public:
		OperatorNode(Operator op);
		OperatorNode(const std::string& op);

		Operator GetOp() const;

	  private:
		Operator m_op;
	};

	class ArgumentListNode : public AstNode
	{
	  public:
		ArgumentListNode(const std::vector<ExpressionNodePtr>& args);

		const std::vector<ExpressionNodePtr>& GetArgs() const;

	  private:
		std::vector<ExpressionNodePtr> m_args;
	};

	class ReturnStatementNode : public StatementNode
	{
	  public:
		ReturnStatementNode(ExpressionNodePtr expression);

		const ExpressionNodePtr& GetExpression() const;

	  private:
		ExpressionNodePtr m_expression;
	};
} // namespace Glyph
