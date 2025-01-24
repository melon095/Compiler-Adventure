#include <Visitors/ASTPrinterVisitor.hh>

#include <magic_enum/magic_enum.hpp>

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
		out << std::string(ident, ' ') << "ProgramNode()" << std::endl;
		Incr();
		for(auto& statement : node.GetStatements())
		{
			Visit(*statement);
		}
		Decr();
	}

	void ASTPrinterVisitor::VisitExpressionStatementNode(ExpressionStatementNode& node)
	{
		out << std::string(ident, ' ') << "ExpressionStatementNode()" << std::endl;
		Incr();
		{
			Visit(*node.GetExpression());
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

	void ASTPrinterVisitor::VisitLetDeclarationNode(LetDeclarationNode& node)
	{
		out << std::string(ident, ' ') << "LetDeclarationNode()" << std::endl;
		Incr();
		{
			Visit(*node.GetIdentifier());
			Visit(*node.GetExpression());
		}
		Decr();
	}

	void ASTPrinterVisitor::VisitPrototypeNode(PrototypeNode& node)
	{
		out << std::string(ident, ' ') << "PrototypeNode(" << node.GetName()->GetName() << ", [";
		for(std::size_t i = 0; i < node.GetArgs().size(); i++)
		{
			out << node.GetArgs()[i];
			if(i != node.GetArgs().size() - 1)
			{
				out << ", ";
			}
		}
		out << "])" << std::endl;
	}

	void ASTPrinterVisitor::VisitFunctionDeclarationNode(FunctionDeclarationNode& node)
	{
		out << std::string(ident, ' ') << "FunctionDeclarationNode()" << std::endl;
		Incr();
		{
			Visit(*node.GetPrototype());
			Visit(*node.GetBlock());
		}
		Decr();
	}

	void ASTPrinterVisitor::VisitFunctionCallNode(FunctionCallNode& node)
	{
		out << std::string(ident, ' ') << "FunctionCallNode()" << std::endl;
		Incr();
		{
			Visit(*node.GetName());
			Incr();
			for(auto& arg : node.GetArgs())
			{
				Visit(*arg);
			}
			Decr();
		}
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

	void ASTPrinterVisitor::VisitIdentifierNode(IdentifierNode& node)
	{
		out << std::string(ident, ' ') << "IdentifierNode(" << node.GetName() << ")" << std::endl;
	}

	void ASTPrinterVisitor::VisitLiteralNode(LiteralNode& node)
	{
		out << std::string(ident, ' ') << "LiteralNode(" << node.GetValue().ToString() << ")" << std::endl;
	}

	void ASTPrinterVisitor::VisitIfExpressionNode(IfExpressionNode& node)
	{
		out << std::string(ident, ' ') << "IfExpressionNode()" << std::endl;
		Incr();
		{
			Visit(*node.GetCondition());
			Visit(*node.GetTrueBranch());
			if(auto& falseBr = node.GetFalseBranch())
			{
				Visit(*falseBr);
			}
		}
		Decr();
	}

	void ASTPrinterVisitor::VisitMatchExpressionNode(MatchExpressionNode& node)
	{
		out << std::string(ident, ' ') << "MatchExpressionNode()" << std::endl;
		Incr();
		{
			Visit(*node.GetExpression());
			out << std::string(ident, ' ') << "MatchCases()" << std::endl;
			Incr();
			for(auto& matchCase : node.GetCases())
			{
				Visit(*matchCase);
			}
			Decr();
		}
		Decr();
	}

	void ASTPrinterVisitor::VisitMatchCaseNode(MatchCaseNode& node)
	{
		out << std::string(ident, ' ') << "MatchCaseNode()" << std::endl;
		Incr();
		{
			out << std::string(ident, ' ') << "Pattern()" << std::endl;
			Incr();
			{
				Visit(*node.GetPattern());
			}
			Decr();
			out << std::string(ident, ' ') << "Expression()" << std::endl;
			Incr();
			{
				Visit(*node.GetBlock());
			}
			Decr();
		}
		Decr();
	}

	void ASTPrinterVisitor::VisitLambdaExpressionNode(LambdaExpressionNode& node)
	{
		out << std::string(ident, ' ') << "LambdaExpressionNode()" << std::endl;
		Incr();
		{
			for(auto& arg : node.GetArgs())
			{
				out << std::string(ident, ' ') << arg << std::endl;
			}
			Visit(*node.GetBlock());
		}
		Decr();
	}

	void ASTPrinterVisitor::VisitOperatorNode(OperatorNode& node)
	{
		out << std::string(ident, ' ') << "OperatorNode(" << magic_enum::enum_name(node.GetOp()) << ")" << std::endl;
	}

	void ASTPrinterVisitor::VisitArgumentListNode(ArgumentListNode& node)
	{
		out << std::string(ident, ' ') << "ArgumentListNode()" << std::endl;
		Incr();
		for(auto& arg : node.GetArgs())
		{
			Visit(*arg);
		}
		Decr();
	}

	void ASTPrinterVisitor::VisitReturnStatementNode(ReturnStatementNode& node)
	{
		out << std::string(ident, ' ') << "ReturnStatementNode()" << std::endl;
		Incr();
		{
			Visit(*node.GetExpression());
		}
		Decr();
	}

	void ASTPrinterVisitor::VisitArithmeticExpressionNode(ArithmeticExpressionNode& node)
	{
		out << std::string(ident, ' ') << "ArithmeticExpressionNode()" << std::endl;
		Incr();
		{
			Visit(*node.GetLhs());
			Visit(*node.GetOp());
			Visit(*node.GetRhs());
		}
		Decr();
	}
} // namespace Glyph
