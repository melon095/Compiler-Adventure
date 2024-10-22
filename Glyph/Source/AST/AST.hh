#pragma once

#include <memory>
#include <string>
#include <vector>

namespace Glyph
{

#define AST_NODE_LIST(V)                                                                                               \
	V(ProgramNode)                                                                                                     \
	V(ExpressionNode)                                                                                                  \
	V(StatementNode)                                                                                                   \
	V(VariableDeclarationNode)                                                                                         \
	V(AssignmentNode)                                                                                                  \
	V(UnaryExpressionNode)                                                                                             \
	V(PrototypeNode)                                                                                                   \
	V(FunctionDeclarationNode)                                                                                         \
	V(FunctionCallNode)                                                                                                \
	V(BinaryExpressionNode)                                                                                            \
	V(IdentifierNode)                                                                                                  \
	V(LiteralNode)                                                                                                     \
	V(OperatorNode)                                                                                                    \
	V(BlockNode)                                                                                                       \
	V(ConditionalNode)                                                                                                 \
	V(LoopNode)                                                                                                        \
	V(ExpressionStatementNode)

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

	class BlockNode : public StatementNode
	{
	  public:
		BlockNode();
		BlockNode(const std::vector<StatementNodePtr>& statements);

		void AddStatement(StatementNodePtr statement);
		std::vector<StatementNodePtr>& GetStatements();

	  private:
		std::vector<StatementNodePtr> m_statements;
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
		// TODO
	  public:
		LiteralNode(int value);

	  private:
		union
		{
			int m_intValue;
		};
	};

	class VariableDeclarationNode : public ExpressionNode
	{
	  public:
		VariableDeclarationNode(IdentifierNodePtr identifier, ExpressionNodePtr expression);

		const IdentifierNodePtr& GetIdentifier() const;
		const ExpressionNodePtr& GetExpression() const;

	  private:
		IdentifierNodePtr m_identifier;
		ExpressionNodePtr m_expression;
	};

	class AssignmentNode : public ExpressionNode
	{
	  public:
		AssignmentNode(IdentifierNodePtr name, ExpressionNodePtr expression);

		const IdentifierNodePtr& GetName() const;
		const ExpressionNodePtr& GetExpression() const;

	  private:
		IdentifierNodePtr m_name;
		ExpressionNodePtr m_expression;
	};

	class UnaryExpressionNode : public ExpressionNode
	{
	  public:
		enum class Type
		{
			Neg,
			Not
		};

		UnaryExpressionNode(const std::string& op, ExpressionNodePtr expression);

		Type GetOp() const;
		const ExpressionNodePtr& GetExpression() const;

	  private:
		static Type GetOperatorType(const std::string& op);

	  private:
		Type m_op;
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
		FunctionDeclarationNode(PrototypeNodePtr prototype, BlockNodePtr body);

		const PrototypeNodePtr& GetPrototype() const;
		const BlockNodePtr& GetBody() const;

	  private:
		PrototypeNodePtr m_prototype;
		BlockNodePtr m_body;
	};

	class FunctionCallNode : public ExpressionNode
	{
	  public:
		FunctionCallNode(const IdentifierNodePtr name, const std::vector<ExpressionNodePtr>& args);

		const IdentifierNodePtr& GetName() const;
		const std::vector<ExpressionNodePtr>& GetArgs() const;

	  private:
		IdentifierNodePtr m_name;
		std::vector<ExpressionNodePtr> m_args;
	};

	class OperatorNode : public ExpressionNode
	{
		enum class Type
		{
			Add,
			Sub,
			Mul,
			Div
		};

	  public:
		OperatorNode(const std::string& op);

		Type GetOp() const;

	  private:
		static Type GetOperatorType(const std::string& op);

	  private:
		Type m_op;
	};

	class BinaryExpressionNode : public ExpressionNode
	{
	  public:
		BinaryExpressionNode(ExpressionNodePtr lhs, OperatorNodePtr op, ExpressionNodePtr rhs);
		BinaryExpressionNode(ExpressionNodePtr lhs, const std::string& op, ExpressionNodePtr rhs);

		const ExpressionNodePtr& GetLhs() const;
		const ExpressionNodePtr& GetRhs() const;
		const OperatorNodePtr& GetOp() const;

	  private:
		ExpressionNodePtr m_lhs;
		ExpressionNodePtr m_rhs;
		OperatorNodePtr m_op;
	};

	class ConditionalNode : public StatementNode
	{
	  public:
		ConditionalNode(ExpressionNodePtr condition, ExpressionNodePtr trueBranch, ExpressionNodePtr falseBranch);

		const ExpressionNodePtr& GetCondition() const;
		const ExpressionNodePtr& GetTrueBranch() const;
		const ExpressionNodePtr& GetFalseBranch() const;

	  private:
		ExpressionNodePtr m_condition;
		ExpressionNodePtr m_trueBranch;
		ExpressionNodePtr m_falseBranch;
	};

	class LoopNode : public StatementNode
	{
	  public:
		LoopNode(ExpressionNodePtr condition, ExpressionNodePtr body);

		const ExpressionNodePtr& GetCondition() const;
		const ExpressionNodePtr& GetBody() const;

	  private:
		ExpressionNodePtr m_condition;
		ExpressionNodePtr m_body;
	};
} // namespace Glyph
