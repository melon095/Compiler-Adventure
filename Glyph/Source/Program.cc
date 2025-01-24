#include <AST/AST.hh>
#include <Bytecode/BytecodeCompilerVisitor.hh>
#include <Bytecode/BytecodeInstruction.hh>
#include <Lexer/Lexer.hh>
#include <Macros.hh>
#include <Parser/Parser.hh>
#include <Visitors/ASTPrinterVisitor.hh>

#include <fstream>
#include <iostream>

#include <magic_enum/magic_enum.hpp>

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
                    LiteralNodePtr(new LiteralNode(10.0))
                )),
                LetDeclarationNodePtr(new LetDeclarationNode(
                    IdentifierNodePtr(new IdentifierNode("b")),
                    LiteralNodePtr(new LiteralNode(20.0))
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
		diagnostics.ForEachError([](auto& message, auto line, auto column) {
			std::cerr << "Error: " << message << " at " << line << ":" << column << std::endl;
		});

		return;
	}

	ASTPrinterVisitor visitor(std::cout);
	visitor.Visit(*program);

	std::ofstream file("ast.dot");
	ASTPrinterVisitor visitor2(file);
	visitor2.Visit(*program);
}

absl::Status TestBytecode(void)
{
	ASTPrinterVisitor astVisitor(std::cout);

	std::string input = R"(
    let a = 10;
    let b = 20;

    a + b;
    )";

	// Load Constant Integer 10
	// Store Local 'a'
	// Load Constant Integer 20
	// Store Local 'b'
	// Load Local 'a'
	// Load Local 'b'
	// Add Operation
	// Return

	Lexer lexer(input);
	auto tokens = lexer.Scan();

	Parser parser(tokens);
	auto program = parser.ParseProgram();

	auto diagnostics = parser.GetDiagnostics();
	if(diagnostics.HasErrors())
	{
		std::cerr << "[Parser] Errors" << std::endl;
		diagnostics.ForEachError([](auto& message, auto line, auto column) {
			std::cerr << "Error: " << message << " at " << line << ":" << column << std::endl;
		});

		return absl::OkStatus();
	}

	using namespace Bytecode;

	BytecodeCompiler compiler {};
	BytecodeCompilerVisitor visitor(std::cout);
	if(auto result = visitor.Visit(compiler, *program); !result.ok())
	{
		std::cout << "Bytecode Compiler Error: " << result.Status << std::endl;

		astVisitor.Visit(const_cast<AstNode&>(*result.Node));
	}

	TRY(compiler.Emit<EndInstruction>());

	int programCounter = 0;
	auto* bytecode = compiler.m_Chunk.Bytecode();

#define NEXT(instr) programCounter += sizeof(instr);
	auto i = new EndInstruction();
	while(true)
	{
		auto opcode = static_cast<Opcode>(bytecode[programCounter]);

		switch(opcode)
		{
			case Opcode::LOAD_CONST:
				{
					auto& instruction = *reinterpret_cast<LoadConstInstruction*>(&bytecode[programCounter]);
					auto constant = TRY_RET(compiler.m_Chunk.GetConstantTable().GetValue(instruction.Index()));

					std::cout << instruction.ToString() << " " << constant->ToString() << std::endl;

					NEXT(LoadConstInstruction);
				}
				break;
			case Opcode::END:
				{
					auto& instruction = *reinterpret_cast<EndInstruction*>(&bytecode[programCounter]);

					std::cout << instruction.ToString() << std::endl;

					goto end;
				}

			default: std::cerr << "Opcode not implemented: " << magic_enum::enum_name(opcode) << std::endl;
		}
	}
end:

	return absl::OkStatus();
}

int main()
{
	// TestASTPrinterVisitor();
	//	TestLexerParser();
	MUST(TestBytecode());
}
