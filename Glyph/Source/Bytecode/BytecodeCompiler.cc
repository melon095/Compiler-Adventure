#include "BytecodeInstruction.hh"
#include <Bytecode/BytecodeCompiler.hh>
namespace Glyph::Bytecode
{
	ConstantTableIndex BytecodeCompiler::MakeConstant(const Glyph::Value& value)
	{
		return m_Chunk.GetConstantTable().AddValue(value);
	}
} // namespace Glyph::Bytecode
