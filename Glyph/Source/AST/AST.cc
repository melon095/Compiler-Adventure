#include <AST/AST.hh>

#include "AST.hh"
#include <map>

namespace Glyph
{
	namespace
	{

		OperatorNode::Operator OperatorFromString(const std::string& op)
		{
			static std::map<std::string, OperatorNode::Operator> operatorMap
				= {{"+", OperatorNode::Operator::Plus},		  {"-", OperatorNode::Operator::Minus},
				   {"*", OperatorNode::Operator::Star},		  {"/", OperatorNode::Operator::Slash},
				   {"<", OperatorNode::Operator::Less},		  {">", OperatorNode::Operator::Greater},
				   {"==", OperatorNode::Operator::Equal},	  {"!=", OperatorNode::Operator::NotEqual},
				   {"<=", OperatorNode::Operator::LessEqual}, {">=", OperatorNode::Operator::GreaterEqual},
				   {"=", OperatorNode::Operator::Assign}};
			return operatorMap[op];
		}
	} // namespace

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
		, m_expression(expression)
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

	LetDeclarationNode::LetDeclarationNode(IdentifierNodePtr identifier, ExpressionNodePtr expression)
		: StatementNode(NodeType::LetDeclarationNode)
		, m_identifier(identifier)
		, m_expression(expression)
	{
	}

	const IdentifierNodePtr& LetDeclarationNode::GetIdentifier() const { return m_identifier; }

	const ExpressionNodePtr& LetDeclarationNode::GetExpression() const { return m_expression; }

	PrototypeNode::PrototypeNode(IdentifierNodePtr name, const std::vector<std::string>& args)
		: AstNode(NodeType::PrototypeNode)
		, m_name(name)
		, m_args(args)
	{
	}

	const IdentifierNodePtr& PrototypeNode::GetName() const { return m_name; }

	const std::vector<std::string>& PrototypeNode::GetArgs() const { return m_args; }

	FunctionDeclarationNode::FunctionDeclarationNode(PrototypeNodePtr prototype, BlockNodePtr block)
		: StatementNode(NodeType::FunctionDeclarationNode)
		, m_prototype(prototype)
		, m_block(block)
	{
	}

	const PrototypeNodePtr& FunctionDeclarationNode::GetPrototype() const { return m_prototype; }

	const BlockNodePtr& FunctionDeclarationNode::GetBlock() const { return m_block; }

	FunctionCallNode::FunctionCallNode(IdentifierNodePtr name, const std::vector<ExpressionNodePtr>& args)
		: ExpressionNode(NodeType::FunctionCallNode)
		, m_name(name)
		, m_args(args)
	{
	}

	const IdentifierNodePtr& FunctionCallNode::GetName() const { return m_name; }

	const std::vector<ExpressionNodePtr>& FunctionCallNode::GetArgs() const { return m_args; }

	BlockNode::BlockNode()
		: ExpressionNode(NodeType::BlockNode)
		, m_statements()
	{
	}

	BlockNode::BlockNode(const std::vector<StatementNodePtr>& statements)
		: ExpressionNode(NodeType::BlockNode)
		, m_statements(statements)
	{
	}

	void BlockNode::AddStatement(StatementNodePtr statement) { m_statements.push_back(std::move(statement)); }

	const std::vector<StatementNodePtr>& BlockNode::GetStatements() const { return m_statements; }

	ArithmeticExpressionNode::ArithmeticExpressionNode(ExpressionNodePtr lhs, OperatorNodePtr op, ExpressionNodePtr rhs)
		: ExpressionNode(NodeType::ArithmeticExpressionNode)
		, m_lhs(lhs)
		, m_op(op)
		, m_rhs(rhs)
	{
	}

	const ExpressionNodePtr& ArithmeticExpressionNode::GetLhs() const { return m_lhs; }

	const OperatorNodePtr& ArithmeticExpressionNode::GetOp() const { return m_op; }

	const ExpressionNodePtr& ArithmeticExpressionNode::GetRhs() const { return m_rhs; }

	IfExpressionNode::IfExpressionNode(ExpressionNodePtr condition, BlockNodePtr trueBranch, BlockNodePtr falseBranch)
		: ExpressionNode(NodeType::IfExpressionNode)
		, m_condition(condition)
		, m_trueBranch(trueBranch)
		, m_falseBranch(falseBranch)
	{
	}

	IfExpressionNode::IfExpressionNode(ExpressionNodePtr condition, BlockNodePtr trueBranch)
		: ExpressionNode(NodeType::IfExpressionNode)
		, m_condition(condition)
		, m_trueBranch(trueBranch)
		, m_falseBranch(std::make_shared<BlockNode>())
	{
	}

	const ExpressionNodePtr& IfExpressionNode::GetCondition() const { return m_condition; }

	const BlockNodePtr& IfExpressionNode::GetTrueBranch() const { return m_trueBranch; }

	const BlockNodePtr& IfExpressionNode::GetFalseBranch() const { return m_falseBranch; }

	MatchExpressionNode::MatchExpressionNode(ExpressionNodePtr expression, const std::vector<MatchCaseNodePtr>& cases)
		: ExpressionNode(NodeType::MatchExpressionNode)
		, m_expression(expression)
		, m_cases(cases)
	{
	}

	const ExpressionNodePtr& MatchExpressionNode::GetExpression() const { return m_expression; }

	const std::vector<MatchCaseNodePtr>& MatchExpressionNode::GetCases() const { return m_cases; }

	MatchCaseNode::MatchCaseNode(ExpressionNodePtr pattern, ExpressionNodePtr block)
		: AstNode(NodeType::MatchCaseNode)
		, m_pattern(pattern)
		, m_block(block)
	{
	}

	const ExpressionNodePtr& MatchCaseNode::GetPattern() const { return m_pattern; }

	const ExpressionNodePtr& MatchCaseNode::GetBlock() const { return m_block; }

	LambdaExpressionNode::LambdaExpressionNode(const std::vector<std::string>& args, BlockNodePtr block)
		: ExpressionNode(NodeType::LambdaExpressionNode)
		, m_args(args)
		, m_block(block)
	{
	}

	const std::vector<std::string>& LambdaExpressionNode::GetArgs() const { return m_args; }

	const BlockNodePtr& LambdaExpressionNode::GetBlock() const { return m_block; }

	IdentifierNode::IdentifierNode(const std::string& name)
		: ExpressionNode(NodeType::IdentifierNode)
		, m_name(name)
	{
	}

	const std::string& IdentifierNode::GetName() const { return m_name; }

	LiteralNode::LiteralNode(const int value)
		: ExpressionNode(NodeType::LiteralNode)
		, m_value(value)

	{
	}

	LiteralNode::LiteralNode(const double value)
		: ExpressionNode(NodeType::LiteralNode)
		, m_value(value)
	{
	}

	LiteralNode::LiteralNode(const bool value)
		: ExpressionNode(NodeType::LiteralNode)
		, m_value(value)
	{
	}

	LiteralNode::LiteralNode(const std::string& value)
		: ExpressionNode(NodeType::LiteralNode)
		, m_value(value)
	{
	}

	std::string LiteralNode::GetString() const
	{
		if(IsInt())
		{
			return std::to_string(std::get<int>(m_value));
		}
		else if(IsDouble())
		{
			return std::to_string(std::get<double>(m_value));
		}
		else if(IsBool())
		{
			return std::get<bool>(m_value) ? "true" : "false";
		}
		else if(IsString())
		{
			return std::get<std::string>(m_value);
		}

		return "Unknown";
	}

	int LiteralNode::GetInt() const { return std::get<int>(m_value); }

	double LiteralNode::GetDouble() const { return std::get<double>(m_value); }

	bool LiteralNode::GetBool() const { return std::get<bool>(m_value); }

	bool LiteralNode::IsInt() const { return std::holds_alternative<int>(m_value); }

	bool LiteralNode::IsDouble() const { return std::holds_alternative<double>(m_value); }

	bool LiteralNode::IsBool() const { return std::holds_alternative<bool>(m_value); }

	bool LiteralNode::IsString() const { return std::holds_alternative<std::string>(m_value); }

	OperatorNode::OperatorNode(Operator op)
		: AstNode(NodeType::OperatorNode)
		, m_op(op)
	{
	}

	OperatorNode::OperatorNode(const std::string& op)
		: AstNode(NodeType::OperatorNode)
		, m_op(OperatorFromString(op))
	{
	}

	OperatorNode::Operator OperatorNode::GetOp() const { return m_op; }

	ArgumentListNode::ArgumentListNode(const std::vector<ExpressionNodePtr>& args)
		: AstNode(NodeType::ArgumentListNode)
		, m_args(args)
	{
	}

	const std::vector<ExpressionNodePtr>& ArgumentListNode::GetArgs() const { return m_args; }

	ReturnStatementNode::ReturnStatementNode(ExpressionNodePtr expression)
		: StatementNode(NodeType::ReturnStatementNode)
		, m_expression(expression)
	{
	}

	const ExpressionNodePtr& ReturnStatementNode::GetExpression() const { return m_expression; }

} // namespace Glyph
