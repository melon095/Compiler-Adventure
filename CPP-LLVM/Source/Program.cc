#include <AST/BinaryExpression.hh>
#include <AST/VariableExpression.hh>
#include <Codegen/CodegenContext.hh>
#include <Lexer/Lexer.hh>
#include <Parser/Parser.hh>
#include <iostream>
#include <string>
#include <string_view>

#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/ExecutionEngine/MCJIT.h>
#include <llvm/Support/TargetSelect.h>

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

extern "C" double printd(double x)
{
	std::cout << std::format("[printd] {}", x) << std::endl;

	return x;
}

int main()
{
	llvm::InitializeNativeTarget();
	llvm::InitializeNativeTargetAsmPrinter();
	llvm::InitializeNativeTargetAsmParser();

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

	std::string INPUT = "def square_5(x)\n"
						"   5 * (x * x)\n"
						"\n"
						"\n"
						"def foo(a, b) a*a + 2*a*b + b*b * square_5(a)\n"
						"\n"
						"def main()\n"
						"   printd(foo(5, 2) * sin(1))\n"
						"\n"
						"extern sin(arg)\n"
						"extern cos(arg)\n"
						"extern atan2(arg1, arg2)\n"
						"extern printd(arg)\n";

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
		std::cerr << "[Parser Error] " << e.what() << std::endl;
		parser.GetDiagnostician()->ForEachError([](const K::Diagnostic& diagnostic)
												{ std::cerr << diagnostic << std::endl; });

		return 1;
	}

	auto context = std::make_shared<llvm::LLVMContext>();
	auto module = new llvm::Module("main", *context);

	auto codegenContext = std::make_shared<K::Codegen::CodegenContext>(context, module);

	auto result = program->Codegen(codegenContext);

	if(result.IsOk())
	{
		std::cout << "Codegen successful" << std::endl;

		auto* mainFunction = module->getFunction("main");
		module->print(llvm::outs(), nullptr);

		std::string errorStr;
		// :(
		auto uniqModule = std::unique_ptr<llvm::Module>(module);
		auto* engine = llvm::EngineBuilder(std::move(uniqModule))
						   .setErrorStr(&errorStr)
						   .setEngineKind(llvm::EngineKind::JIT)
						   .create();
		if(!engine)
		{
			std::cerr << "Failed to create engine " << errorStr << std::endl;
			return 1;
		}

		auto* main = engine->FindFunctionNamed("main");
		if(!main)
		{
			std::cerr << "Failed to find main" << std::endl;
			return 1;
		}

		llvm::GenericValue ret = engine->runFunction(main, std::vector<llvm::GenericValue>());

		std::cout << "Result: " << ret.IntVal.getLimitedValue() << std::endl;
	}
	else
	{
		std::cerr << "Codegen failed" << std::endl;
		result.Dump(0, std::cout);
	}
}
