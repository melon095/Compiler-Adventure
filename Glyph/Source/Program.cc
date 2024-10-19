#include <AST/AST.hh>
#include <Visitors/ConsoleDebugVisitor.hh>

using namespace Glyph;

int main()
{
	// clang-format off

	// @x = 42
    auto x = std::shared_ptr<VariableDeclarationNode>(new VariableDeclarationNode(
        std::shared_ptr<IdentifierNode>(new IdentifierNode("x")),
        std::shared_ptr<LiteralNode>(new LiteralNode(42))
    ));
    
	// fun foo(x) x + 1;
    auto fun = std::shared_ptr<FunctionDeclarationNode>(new FunctionDeclarationNode(
        std::shared_ptr<PrototypeNode>(new PrototypeNode("foo", {"x"})),
        std::shared_ptr<BinaryExpressionNode>(new BinaryExpressionNode(
            std::shared_ptr<IdentifierNode>(new IdentifierNode("x")),
            std::shared_ptr<OperatorNode>(new OperatorNode("+")),
            std::shared_ptr<LiteralNode>(new LiteralNode(1))
        ))
    ));
    
    auto block = std::shared_ptr<BlockNode>(new BlockNode());
    block->AddStatement(std::move(x));
    block->AddStatement(std::move(fun));
    

    auto program = ProgramNode();
    program.AddStatement(std::move(block));

	// clang-format on

	ConsoleDebugVisitor visitor;
	// visitor.Visit(program);

	return 0;
}