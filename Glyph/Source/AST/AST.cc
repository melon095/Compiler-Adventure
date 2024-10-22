#include <AST/AST.hh>

#include "AST.hh"
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

	ExpressionStatementNode::ExpressionStatementNode(ExpressionNodePtr expression)
		: StatementNode(NodeType::ExpressionStatementNode)
		, m_expression(std::move(expression))
	{
	}

	const ExpressionNodePtr& ExpressionStatementNode::GetExpression() const { return m_expression; }

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

	std::vector<StatementNodePtr>& ProgramNode::GetStatements() { return m_statements; }

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

	std::vector<StatementNodePtr>& BlockNode::GetStatements() { return m_statements; }

	IdentifierNode::IdentifierNode(const std::string& name)
		: ExpressionNode(NodeType::IdentifierNode)
		, m_name(name)
	{
	}

	const std::string& IdentifierNode::GetName() const { return m_name; }

	LiteralNode::LiteralNode(int value)
		: ExpressionNode(NodeType::LiteralNode)
		, m_intValue(value)
	{
	}

	VariableDeclarationNode::VariableDeclarationNode(IdentifierNodePtr identifier, ExpressionNodePtr expression)
		: ExpressionNode(NodeType::VariableDeclarationNode)
		, m_identifier(std::move(identifier))
		, m_expression(std::move(expression))
	{
	}

	const IdentifierNodePtr& VariableDeclarationNode::GetIdentifier() const { return m_identifier; }

	const ExpressionNodePtr& VariableDeclarationNode::GetExpression() const { return m_expression; }

	AssignmentNode::AssignmentNode(IdentifierNodePtr name, ExpressionNodePtr expression)
		: ExpressionNode(NodeType::AssignmentNode)
		, m_name(name)
		, m_expression(std::move(expression))
	{
	}

	const IdentifierNodePtr& AssignmentNode::GetName() const { return m_name; }

	const ExpressionNodePtr& AssignmentNode::GetExpression() const { return m_expression; }

	UnaryExpressionNode::UnaryExpressionNode(const std::string& op, ExpressionNodePtr expression)
		: ExpressionNode(NodeType::UnaryExpressionNode)
		, m_op(GetOperatorType(op))
		, m_expression(std::move(expression))
	{
	}

	UnaryExpressionNode::Type UnaryExpressionNode::GetOperatorType(const std::string& op)
	{
		static std::map<std::string, UnaryExpressionNode::Type> operators = {
			{"-", UnaryExpressionNode::Type::Neg},
			{"!", UnaryExpressionNode::Type::Not},
		};

		return operators[op];
	}

	UnaryExpressionNode::Type UnaryExpressionNode::GetOp() const { return m_op; }

	const ExpressionNodePtr& UnaryExpressionNode::GetExpression() const { return m_expression; }

	PrototypeNode::PrototypeNode(IdentifierNodePtr name, const std::vector<std::string>& args)
		: AstNode(NodeType::PrototypeNode)
		, m_name(name)
		, m_args(args)
	{
	}

	const IdentifierNodePtr& PrototypeNode::GetName() const { return m_name; }

	const std::vector<std::string>& PrototypeNode::GetArgs() const { return m_args; }

	FunctionDeclarationNode::FunctionDeclarationNode(PrototypeNodePtr prototype, BlockNodePtr body)
		: StatementNode(NodeType::FunctionDeclarationNode)
		, m_prototype(std::move(prototype))
		, m_body(std::move(body))
	{
	}

	const PrototypeNodePtr& FunctionDeclarationNode::GetPrototype() const { return m_prototype; }

	const BlockNodePtr& FunctionDeclarationNode::GetBody() const { return m_body; }

	FunctionCallNode::FunctionCallNode(IdentifierNodePtr name, const std::vector<ExpressionNodePtr>& args)
		: ExpressionNode(NodeType::FunctionCallNode)
		, m_name(std::move(name))
		, m_args(args)
	{
	}

	const IdentifierNodePtr& FunctionCallNode::GetName() const { return m_name; }

	const std::vector<ExpressionNodePtr>& FunctionCallNode::GetArgs() const { return m_args; }

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

	OperatorNode::Type OperatorNode::GetOp() const { return m_op; }

	BinaryExpressionNode::BinaryExpressionNode(ExpressionNodePtr lhs, OperatorNodePtr op, ExpressionNodePtr rhs)
		: ExpressionNode(NodeType::BinaryExpressionNode)
		, m_lhs(std::move(lhs))
		, m_rhs(std::move(rhs))
		, m_op(std::move(op))
	{
	}

	BinaryExpressionNode::BinaryExpressionNode(ExpressionNodePtr lhs, const std::string& op, ExpressionNodePtr rhs)
		: ExpressionNode(NodeType::BinaryExpressionNode)
		, m_lhs(std::move(lhs))
		, m_rhs(std::move(rhs))
		, m_op(std::make_shared<OperatorNode>(op))
	{
	}

	const ExpressionNodePtr& BinaryExpressionNode::GetLhs() const { return m_lhs; }

	const OperatorNodePtr& BinaryExpressionNode::GetOp() const { return m_op; }

	const ExpressionNodePtr& BinaryExpressionNode::GetRhs() const { return m_rhs; }

	ConditionalNode::ConditionalNode(ExpressionNodePtr condition, ExpressionNodePtr trueBranch,
									 ExpressionNodePtr falseBranch)
		: StatementNode(NodeType::ConditionalNode)
		, m_condition(std::move(condition))
		, m_trueBranch(std::move(trueBranch))
		, m_falseBranch(std::move(falseBranch))
	{
	}

	const ExpressionNodePtr& ConditionalNode::GetCondition() const { return m_condition; }

	const ExpressionNodePtr& ConditionalNode::GetTrueBranch() const { return m_trueBranch; }

	const ExpressionNodePtr& ConditionalNode::GetFalseBranch() const { return m_falseBranch; }

	LoopNode::LoopNode(ExpressionNodePtr condition, ExpressionNodePtr body)
		: StatementNode(NodeType::LoopNode)
		, m_condition(std::move(condition))
		, m_body(std::move(body))
	{
	}

	const ExpressionNodePtr& LoopNode::GetCondition() const { return m_condition; }

	const ExpressionNodePtr& LoopNode::GetBody() const { return m_body; }
} // namespace Glyph
