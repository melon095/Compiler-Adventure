#include <Bytecode/BytecodeCompilerVisitor.hh>
#include <Macros.hh>

namespace Glyph::Bytecode
{
#define N()                                                                                                            \
	do                                                                                                                 \
	{                                                                                                                  \
		m_DebugStream << "\033[1;31mUNINPLEMENTED '" << __PRETTY_FUNCTION__ << "'\033[0m" << std::endl;                \
                                                                                                                       \
		return BytecodeStepResult(&node, absl::InternalError("Not Implemented"));                                      \
	}                                                                                                                  \
	while(0);

	BytecodeCompilerVisitor::BytecodeCompilerVisitor(std::ostream& debugStream)
		: m_DebugStream(debugStream)
	{
	}

	BytecodeStepResult BytecodeCompilerVisitor::Visit(BytecodeCompiler& compiler, AstNode& node)
	{
#define VISIT_CASE(name)                                                                                               \
	case NodeType::name:                                                                                               \
		{                                                                                                              \
			auto result = Visit##name(compiler, dynamic_cast<name&>(node));                                            \
			if(!result.ok())                                                                                           \
			{                                                                                                          \
				return result;                                                                                         \
			}                                                                                                          \
		}                                                                                                              \
		break;

		switch(node.GetType())
		{
			AST_NODE_LIST(VISIT_CASE)
		}

#undef VISIT_CASE

		return {};
	}

	BytecodeStepResult BytecodeCompilerVisitor::VisitStatementNode(BytecodeCompiler& compiler, StatementNode& node)
	{
		N();
	}

	BytecodeStepResult BytecodeCompilerVisitor::VisitExpressionNode(BytecodeCompiler& compiler, ExpressionNode& node)
	{
		N();
	}

	BytecodeStepResult
	BytecodeCompilerVisitor::VisitExpressionStatementNode(BytecodeCompiler& compiler, ExpressionStatementNode& node)
	{
		N();
	}

	BytecodeStepResult BytecodeCompilerVisitor::VisitProgramNode(BytecodeCompiler& compiler, ProgramNode& node)
	{
		for(auto& statement : node.GetStatements())
		{
			TRY(Visit(compiler, *statement));
		}

		return {};
	}

	BytecodeStepResult
	BytecodeCompilerVisitor::VisitArgumentListNode(BytecodeCompiler& compiler, ArgumentListNode& node)
	{
		N();
	}

	BytecodeStepResult
	BytecodeCompilerVisitor::VisitArithmeticExpressionNode(BytecodeCompiler& compiler, ArithmeticExpressionNode& node)
	{
		N();
	}

	BytecodeStepResult BytecodeCompilerVisitor::VisitBlockNode(BytecodeCompiler& compiler, BlockNode& node) { N(); }

	BytecodeStepResult
	BytecodeCompilerVisitor::VisitFunctionCallNode(BytecodeCompiler& compiler, FunctionCallNode& node)
	{
		N();
	}

	BytecodeStepResult
	BytecodeCompilerVisitor::VisitFunctionDeclarationNode(BytecodeCompiler& compiler, FunctionDeclarationNode& node)
	{
		N();
	}

	BytecodeStepResult BytecodeCompilerVisitor::VisitIdentifierNode(BytecodeCompiler& compiler, IdentifierNode& node)
	{
		N();
	}

	BytecodeStepResult
	BytecodeCompilerVisitor::VisitIfExpressionNode(BytecodeCompiler& compiler, IfExpressionNode& node)
	{
		N();
	}

	BytecodeStepResult
	BytecodeCompilerVisitor::VisitLetDeclarationNode(BytecodeCompiler& compiler, LetDeclarationNode& node)
	{
		// Load the value (LOAD_CONST)
		TRY(Visit(compiler, *node.GetExpression()));

		// FIXME: load the identifier (STORE_NAME)
		//		compiler.EmitInstruction(BytecodeInstruction::STORE_NAME, node.GetIdentifier()->GetName());

		return {};
	}

	BytecodeStepResult BytecodeCompilerVisitor::VisitLiteralNode(BytecodeCompiler& compiler, LiteralNode& node)
	{
		auto constant = compiler.MakeConstant(node.GetValue());

		TRY(compiler.Emit<LoadConstInstruction>(constant));

		return {};
	}

	BytecodeStepResult
	BytecodeCompilerVisitor::VisitMatchExpressionNode(BytecodeCompiler& compiler, MatchExpressionNode& node)
	{
		N();
	}

	BytecodeStepResult BytecodeCompilerVisitor::VisitMatchCaseNode(BytecodeCompiler& compiler, MatchCaseNode& node)
	{
		N();
	}

	BytecodeStepResult BytecodeCompilerVisitor::VisitOperatorNode(BytecodeCompiler& compiler, OperatorNode& node)
	{
		N();
	}

	BytecodeStepResult BytecodeCompilerVisitor::VisitPrototypeNode(BytecodeCompiler& compiler, PrototypeNode& node)
	{
		N();
	}

	BytecodeStepResult
	BytecodeCompilerVisitor::VisitReturnStatementNode(BytecodeCompiler& compiler, ReturnStatementNode& node)
	{
		N();
	}

	BytecodeStepResult
	BytecodeCompilerVisitor::VisitLambdaExpressionNode(BytecodeCompiler& compiler, LambdaExpressionNode& node)
	{
		N();
	}

} // namespace Glyph::Bytecode
