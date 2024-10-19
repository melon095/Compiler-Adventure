#include <AST/AST.hh>

#include <map>

namespace Glyph
{
	AstNode::AstNode(NodeType type)
		: m_type(type)
	{
	}

	NodeType AstNode::GetType() const { return m_type; }
	bool AstNode::IsType(NodeType type) const { return m_type == type; }

	ExpressionNode::ExpressionNode(NodeType type)
		: AstNode(type)
	{
	}

	StatementNode::StatementNode(NodeType type)
		: AstNode(type)
	{
	}

	ProgramNode::ProgramNode()
		: AstNode(NodeType::ProgramNode)
		, m_statements()
	{
	}

	ProgramNode::ProgramNode(const std::vector<StatementNodePtr>& statements)
		: AstNode(NodeType::ProgramNode)
		, m_statements(statements)
	{
	}

	void ProgramNode::AddStatement(StatementNodePtr statement) { m_statements.push_back(std::move(statement)); }

	BlockNode::BlockNode()
		: StatementNode(NodeType::BlockNode)
		, m_statements()
	{
	}

	BlockNode::BlockNode(const std::vector<StatementNodePtr>& statements)
		: StatementNode(NodeType::BlockNode)
		, m_statements(statements)
	{
	}

	void BlockNode::AddStatement(StatementNodePtr statement) { m_statements.push_back(std::move(statement)); }

	IdentifierNode::IdentifierNode(const std::string& name)
		: ExpressionNode(NodeType::IdentifierNode)
		, m_name(name)
	{
	}

	LiteralNode::LiteralNode(int value)
		: ExpressionNode(NodeType::LiteralNode)
		, m_intValue(value)
	{
	}

	VariableDeclarationNode::VariableDeclarationNode(IdentifierNodePtr identifier, ExpressionNodePtr expression)
		: StatementNode(NodeType::VariableDeclarationNode)
		, m_identifier(std::move(identifier))
		, m_expression(std::move(expression))
	{
	}

	AssignmentNode::AssignmentNode(const std::string& name, ExpressionNodePtr expression)
		: StatementNode(NodeType::AssignmentNode)
		, m_name(name)
		, m_expression(std::move(expression))
	{
	}

	UnaryExpressionNode::UnaryExpressionNode(const std::string& op, ExpressionNodePtr expression)
		: ExpressionNode(NodeType::UnaryExpressionNode)
		, m_op(op)
		, m_expression(std::move(expression))
	{
	}

	PrototypeNode::PrototypeNode(const std::string& name, const std::vector<std::string>& args)
		: AstNode(NodeType::PrototypeNode)
		, m_name(name)
		, m_args(args)
	{
	}

	FunctionDeclarationNode::FunctionDeclarationNode(PrototypeNodePtr prototype, ExpressionNodePtr body)
		: StatementNode(NodeType::FunctionDeclarationNode)
		, m_prototype(std::move(prototype))
		, m_body(std::move(body))
	{
	}

	FunctionCallNode::FunctionCallNode(IdentifierNodePtr name, const std::vector<ExpressionNodePtr>& args)
		: ExpressionNode(NodeType::FunctionCallNode)
		, m_name(std::move(name))
		, m_args(args)
	{
	}

	OperatorNode::OperatorNode(const std::string& op)
		: ExpressionNode(NodeType::OperatorNode)
		, m_op(GetOperatorType(op))
	{
	}

	OperatorNode::Type OperatorNode::GetOperatorType(const std::string& op)
	{
		static std::map<std::string, OperatorNode::Type> operators = {
			{"+", OperatorNode::Type::Add},
			{"-", OperatorNode::Type::Sub},
			{"*", OperatorNode::Type::Mul},
			{"/", OperatorNode::Type::Div},
		};

		return operators[op];
	}

	BinaryExpressionNode::BinaryExpressionNode(ExpressionNodePtr lhs, OperatorNodePtr op, ExpressionNodePtr rhs)
		: ExpressionNode(NodeType::BinaryExpressionNode)
		, m_lhs(std::move(lhs))
		, m_rhs(std::move(rhs))
		, m_op(std::move(op))
	{
	}

} // namespace Glyph