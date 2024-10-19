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
	V(BlockNode)

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

	class ProgramNode : public AstNode
	{
	  public:
		ProgramNode();
		ProgramNode(const std::vector<StatementNodePtr>& statements);

		void AddStatement(StatementNodePtr statement);

	  private:
		std::vector<StatementNodePtr> m_statements;
	};

	class BlockNode : public StatementNode
	{
	  public:
		BlockNode();
		BlockNode(const std::vector<StatementNodePtr>& statements);

		void AddStatement(StatementNodePtr statement);

	  private:
		std::vector<StatementNodePtr> m_statements;
	};

	class IdentifierNode : public ExpressionNode
	{
	  public:
		IdentifierNode(const std::string& name);

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

	class VariableDeclarationNode : public StatementNode
	{
	  public:
		VariableDeclarationNode(IdentifierNodePtr identifier, ExpressionNodePtr expression);

	  private:
		IdentifierNodePtr m_identifier;
		ExpressionNodePtr m_expression;
	};

	class AssignmentNode : public StatementNode
	{
	  public:
		AssignmentNode(const std::string& name, ExpressionNodePtr expression);

	  private:
		std::string m_name;
		ExpressionNodePtr m_expression;
	};

	class UnaryExpressionNode : public ExpressionNode
	{
	  public:
		UnaryExpressionNode(const std::string& op, ExpressionNodePtr expression);

	  private:
		std::string m_op;
		ExpressionNodePtr m_expression;
	};

	class PrototypeNode : public AstNode
	{
	  public:
		PrototypeNode(const std::string& name, const std::vector<std::string>& args);

	  private:
		std::string m_name;
		std::vector<std::string> m_args;
	};

	class FunctionDeclarationNode : public StatementNode
	{
	  public:
		FunctionDeclarationNode(PrototypeNodePtr prototype, ExpressionNodePtr body);

	  private:
		PrototypeNodePtr m_prototype;
		ExpressionNodePtr m_body;
	};

	class FunctionCallNode : public ExpressionNode
	{
	  public:
		FunctionCallNode(const IdentifierNodePtr name, const std::vector<ExpressionNodePtr>& args);

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

	  private:
		ExpressionNodePtr m_lhs;
		ExpressionNodePtr m_rhs;
		OperatorNodePtr m_op;
	};
} // namespace Glyph
