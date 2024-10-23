#include <AST/AST.hh>
#include <Lexer/Lexer.hh>
#include <Parser/Parser.hh>
#include <Visitors/ASTPrinterVisitor.hh>

#include <fstream>
#include <iostream>

using namespace Glyph;

void TestASTPrinterVisitor(void)
{
	// clang-format off
    // let result = {
    //     let a = 10;
    //     let b = 20;
    //     a + b
    // }
    auto* block = new BlockNode();
    block->AddStatement(LetDeclarationNodePtr(new LetDeclarationNode(
        IdentifierNodePtr(new IdentifierNode("result")),
        BlockNodePtr(new BlockNode(
            std::vector<StatementNodePtr>{
                LetDeclarationNodePtr(new LetDeclarationNode(
                    IdentifierNodePtr(new IdentifierNode("a")),
                    LiteralNodePtr(new LiteralNode(10))
                )),
                LetDeclarationNodePtr(new LetDeclarationNode(
                    IdentifierNodePtr(new IdentifierNode("b")),
                    LiteralNodePtr(new LiteralNode(20))
                )),
                ExpressionStatementNodePtr(new ExpressionStatementNode(
                    ArithmeticExpressionNodePtr(new ArithmeticExpressionNode(
                        IdentifierNodePtr(new IdentifierNode("a")),
                        OperatorNodePtr(new OperatorNode("+")),
                        IdentifierNodePtr(new IdentifierNode("b"))
                    ))
                ))
            }
        ))
    )));

    auto program = ProgramNode();
    program.AddStatement(FunctionDeclarationNodePtr(new FunctionDeclarationNode(
        PrototypeNodePtr(new PrototypeNode(IdentifierNodePtr(new IdentifierNode("foo")), std::vector<std::string>{"x"})),
        BlockNodePtr(block)
    )
    ));

	// clang-format on

	ASTPrinterVisitor visitor(std::cout);
	visitor.Visit(program);
}

void TestLexerParser(void)
{
	std::string input = R"(
let result = {
    let a = 10;
    let b = 20;
    return foo(a, b);
};

let foo = (x, y) -> {
    return x + y;
};

match 5+5 {
    5 ->  { return 10; };
    10 -> 20; 
    42 -> {
        let block_scoped_fun = () -> {
            return 42;
        };

        return block_scoped_fun();
    };
};
    )";

	Lexer lexer(input);
	auto tokens = lexer.Scan();

	for(const auto& token : tokens)
	{
		std::cout << "Token: " << token << std::endl;
	}

	Parser parser(tokens);
	auto program = parser.ParseProgram();

	auto diagnostics = parser.GetDiagnostics();
	if(diagnostics.HasErrors())
	{
		std::cerr << "[Parser] Errors" << std::endl;
		diagnostics.ForEachError(
			[](auto& message, auto line, auto column)
			{ std::cerr << "Error: " << message << " at " << line << ":" << column << std::endl; });

		return;
	}

	ASTPrinterVisitor visitor(std::cout);
	visitor.Visit(*program);

	std::ofstream file("ast.dot");
	ASTPrinterVisitor visitor2(file);
	visitor2.Visit(*program);
}

int main()
{
	TestASTPrinterVisitor();
	TestLexerParser();
}
