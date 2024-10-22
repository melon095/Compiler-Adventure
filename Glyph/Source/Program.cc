#include <AST/AST.hh>
#include <Lexer/Lexer.hh>
#include <Parser/Parser.hh>
#include <Visitors/ASTPrinterVisitor.hh>

#include <iostream>

using namespace Glyph;

void TestASTPrinterVisitor(void)
{
	// clang-format off

	// @x = 42
    // auto x = VariableDeclarationNodePtr(new VariableDeclarationNode(
    //     IdentifierNodePtr(new IdentifierNode("x")),
    //     LiteralNodePtr(new LiteralNode(42))
    // ));
    
	// fun foo(x) x + 1;
    // auto fun = FunctionDeclarationNodePtr(new FunctionDeclarationNode(
    //     PrototypeNodePtr(new PrototypeNode(IdentifierNodePtr(new IdentifierNode("foo")), {"x"})),
    //     BlockNodePtr(BlockNode(BinaryExpressionNodePtr(new BinaryExpressionNode(
    //         IdentifierNodePtr(new IdentifierNode("x")),
    //         OperatorNodePtr(new OperatorNode("+")),
    //         LiteralNodePtr(new LiteralNode(1))
    //     ))))
    // ));
    
    auto block = std::shared_ptr<BlockNode>(new BlockNode());
    block->AddStatement(ExpressionStatementNodePtr(new ExpressionStatementNode(
        BinaryExpressionNodePtr(new BinaryExpressionNode(
            IdentifierNodePtr(new IdentifierNode("x")),
            OperatorNodePtr(new OperatorNode("+")),
            LiteralNodePtr(new LiteralNode(1))
        ))
    )));
    

    auto program = ProgramNode();
    program.AddStatement(std::move(block));

	// clang-format on

	ASTPrinterVisitor visitor(std::cout);
	visitor.Visit(program);
}

void TestLexerParser(void)
{
	std::string input = "fun foo(x) -> { x + 1; };\n"
						"5 @foo <->;\n"
						"5 > 2 ?\n"
						"   -> \"True\"\n"
						"   -> \"False\"\n"
						"   -> @result;\n"
						"@result @print <->;";

	Lexer lexer(input);
	auto tokens = lexer.Scan();

	for(const auto& token : tokens)
	{
		std::cout << "Token: " << token << std::endl;
	}

	Parser parser(tokens);
	auto program = parser.ParseProgram();

	ASTPrinterVisitor visitor(std::cout);
	visitor.Visit(*program);
}

int main()
{
	TestASTPrinterVisitor();
	TestLexerParser();
}
