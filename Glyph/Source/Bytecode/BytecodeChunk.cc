#include "BytecodeChunk.hh"

namespace Glyph::Bytecode
{
	void BytecodeChunk::Grow(size_t size)
	{
		m_Bytecode.reserve(m_Bytecode.size() + size);
		m_Bytecode.resize(m_Bytecode.size() + size);
	}
} // namespace Glyph::Bytecode
