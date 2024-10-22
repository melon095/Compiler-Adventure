#include <Visitors/ASTPrinterVisitor.hh>

#include <magic_enum.hpp>

#include <iostream>

namespace Glyph
{
	ASTPrinterVisitor::ASTPrinterVisitor(std::ostream& out)
		: out(out)
		, ident(0)
	{
	}

	void ASTPrinterVisitor::Incr() { ident += 2; }

	void ASTPrinterVisitor::Decr() { ident -= 2; }

	void ASTPrinterVisitor::VisitProgramNode(ProgramNode& node)
	{
		out << std::string(ident, ' ') << "ProgramNode" << std::endl;
		Incr();
		for(auto& statement : node.GetStatements())
		{
			Visit(*statement);
		}
		Decr();
	}

	void ASTPrinterVisitor::VisitExpressionNode(ExpressionNode& node)
	{
		(void)node;

		out << std::string(ident, ' ') << "ExpressionNode()" << std::endl;
	}

	void ASTPrinterVisitor::VisitStatementNode(StatementNode& node)
	{
		(void)node;

		out << std::string(ident, ' ') << "StatementNode()" << std::endl;
	}

	void ASTPrinterVisitor::VisitAssignmentNode(AssignmentNode& node)
	{
		out << std::string(ident, ' ') << "AssignmentNode(" << node.GetName()->GetName() << ")" << std::endl;
		Incr();
		Visit(*node.GetExpression());
		Decr();
	}

	void ASTPrinterVisitor::VisitBinaryExpressionNode(BinaryExpressionNode& node)
	{
		out << std::string(ident, ' ') << "BinaryExpressionNode(" << magic_enum::enum_name(node.GetOp()->GetOp()) << ")"
			<< std::endl;
		Incr();
		Visit(*node.GetLhs());
		Visit(*node.GetRhs());
		Decr();
	}

	void ASTPrinterVisitor::VisitBlockNode(BlockNode& node)
	{
		out << std::string(ident, ' ') << "BlockNode()" << std::endl;
		Incr();
		for(auto& statement : node.GetStatements())
		{
			Visit(*statement);
		}
		Decr();
	}

	void ASTPrinterVisitor::VisitFunctionCallNode(FunctionCallNode& node)
	{
		out << std::string(ident, ' ') << "FunctionCallNode(" << node.GetName()->GetName() << ")" << std::endl;
		Incr();
		for(auto& arg : node.GetArgs())
		{
			Visit(*arg);
		}
		Decr();
	}

	void ASTPrinterVisitor::VisitFunctionDeclarationNode(FunctionDeclarationNode& node)
	{
		out << std::string(ident, ' ') << "FunctionDeclarationNode()" << std::endl;
		Incr();
		Visit(*node.GetPrototype());
		Visit(*node.GetBody());
		Decr();
	}

	void ASTPrinterVisitor::VisitIdentifierNode(IdentifierNode& node)
	{
		out << std::string(ident, ' ') << "IdentifierNode(" << node.GetName() << ")" << std::endl;
	}

	void ASTPrinterVisitor::VisitLiteralNode(LiteralNode& node)
	{
		out << std::string(ident, ' ') << "LiteralNode(" << /*node.GetValue()*/ "TODO" << ")" << std::endl;
	}

	void ASTPrinterVisitor::VisitOperatorNode(OperatorNode& node)
	{
		out << std::string(ident, ' ') << "OperatorNode(" << magic_enum::enum_name(node.GetOp()) << ")" << std::endl;
	}

	void ASTPrinterVisitor::VisitUnaryExpressionNode(UnaryExpressionNode& node)
	{
		out << std::string(ident, ' ') << "UnaryExpressionNode(" << magic_enum::enum_name(node.GetOp()) << ")"
			<< std::endl;
		Incr();
		Visit(*node.GetExpression());
		Decr();
	}

	void ASTPrinterVisitor::VisitVariableDeclarationNode(VariableDeclarationNode& node)
	{
		out << std::string(ident, ' ') << "VariableDeclarationNode(" << node.GetIdentifier()->GetName() << ")"
			<< std::endl;
		Incr();
		Visit(*node.GetExpression());
		Decr();
	}

	void ASTPrinterVisitor::VisitPrototypeNode(PrototypeNode& node)
	{
		auto& args = node.GetArgs();
		auto aSize = args.size();
		out << std::string(ident, ' ') << "PrototypeNode(" << node.GetName() << ", [";
		for(std::size_t i = 0; i < aSize; i++)
		{
			out << args[i];
			if(i < aSize - 1)
			{
				out << ", ";
			}
		}

		out << "])" << std::endl;
	}

	void ASTPrinterVisitor::VisitConditionalNode(ConditionalNode& node)
	{
		out << std::string(ident, ' ') << "ConditionalNode()" << std::endl;
		Incr();
		Visit(*node.GetCondition());
		Visit(*node.GetTrueBranch());
		Visit(*node.GetFalseBranch());
		Decr();
	}

	void ASTPrinterVisitor::VisitLoopNode(LoopNode& node)
	{
		out << std::string(ident, ' ') << "LoopNode()" << std::endl;
		Incr();
		Visit(*node.GetCondition());
		Visit(*node.GetBody());
		Decr();
	}

	void ASTPrinterVisitor::VisitExpressionStatementNode(ExpressionStatementNode& node)
	{
		out << std::string(ident, ' ') << "ExpressionStatementNode()" << std::endl;
		Incr();
		Visit(*node.GetExpression());
		Decr();
	}
} // namespace Glyph
