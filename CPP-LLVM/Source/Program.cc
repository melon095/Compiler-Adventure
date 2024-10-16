#include <AST/BinaryExpression.hh>
#include <AST/VariableExpression.hh>
#include <Codegen/CodegenContext.hh>
#include <Lexer/Lexer.hh>
#include <Parser/Parser.hh>
#include <iostream>
#include <string>
#include <string_view>

using namespace std::literals;

// #include <llvm/IR/LLVMContext.h>
// #include <llvm/IR/Module.h>
// #include <llvm/IR/IRBuilder.h>
// #include <llvm/Support/raw_ostream.h>

// constexpr std::string_view PROJECT_NAME = "LLVM-Test";

// int main()
// {
//     std::cout << "This is project " << PROJECT_NAME << std::endl;

//     auto context = llvm::LLVMContext();
//     auto *module = new llvm::Module("top", context);

//     auto builder = llvm ::IRBuilder<>(context);

//     auto *function = llvm::FunctionType::get(builder.getInt32Ty(), false);

//     auto *main = llvm::Function::Create(function, llvm::Function::ExternalLinkage, "main", module);

//     auto *entry = llvm::BasicBlock::Create(context, "entry", main);
//     builder.SetInsertPoint(entry);

//     builder.CreateRet(builder.getInt32(42));

//     module->print(llvm::outs(), nullptr);

//     delete module;
//     return 0;
// }

int main()
{
#if 1
	// std::string INPUT = "def fib(x)\n"
	// 					"    if x < 3 then\n"
	// 					"        return 1\n"
	// 					"    else\n"
	// 					"        return fib(x - 1) + fib(x - 2)\n"
	// 					"    end\n"
	//                  "\n"
	// 					"extern sin(arg)\n"
	// 					"extern cos(arg)\n"
	// 					"extern atan2(arg1, arg2)\n";

	std::string INPUT = "def fib(x)\n"
						"   5 * (x * 2)\n"
						"\n"
						"extern sin(arg)\n"
						"extern cos(arg)\n"
						"extern atan2(arg1, arg2)\n";

	std::cout << "Input: " << INPUT << std::endl;

	auto lexer = K::Lexer(INPUT);

	auto tokens = lexer.Scan();

	for(const auto& token : tokens)
	{
		std::cout << token << std::endl;
	}
#endif

	auto parser = K::Parser(tokens);

	K::AST::ProgramPtr program;
	try
	{
		program = parser.Parse();

		program->Dump(0, std::cout);
	}
	catch(const std::runtime_error& e)
	{
		std::cerr << "[Parser Error]" << std::endl;
		parser.GetDiagnostician()->ForEachError([](const K::Diagnostic& diagnostic)
												{ std::cerr << diagnostic << std::endl; });

		return 1;
	}

	auto context = std::make_shared<llvm::LLVMContext>();
	auto module = std::make_shared<llvm::Module>("main", *context);

	auto codegenContext = std::make_shared<K::Codegen::CodegenContext>(context, module);

	auto result = program->Codegen(codegenContext);

	if(result.IsOk())
	{
		std::cout << "Codegen successful" << std::endl;
	}
	else
	{
		std::cerr << "Codegen failed" << std::endl;
		result.Dump(0, std::cout);
	}
}
