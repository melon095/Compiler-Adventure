#pragma once

#include <AST/AST.hh>
#include <Bytecode/BytecodeCompiler.hh>
#include <Bytecode/BytecodeStepResult.hh>

namespace Glyph::Bytecode
{
	class BytecodeCompilerVisitor
	{
	  public:
		BytecodeCompilerVisitor(std::ostream& debugStream);

#define DEFINE_VISIT_METHOD(name) BytecodeStepResult Visit##name(BytecodeCompiler& compiler, name& node);
		[[maybe_unused]] AST_NODE_LIST(DEFINE_VISIT_METHOD)
#undef DEFINE_VISIT_METHOD

			BytecodeStepResult Visit(BytecodeCompiler& compiler, AstNode& node);

	  private:
		std::ostream& m_DebugStream;
	};
} // namespace Glyph::Bytecode
