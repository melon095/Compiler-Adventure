#pragma once

#include <Bytecode/BytecodeInstruction.hh>
#include <Runtime/ConstantTable.hh>
#include <vector>

namespace Glyph::Bytecode
{
	class BytecodeChunk
	{
	  public:
		BytecodeChunk() = default;
		~BytecodeChunk() = default;

		ConstantTable& GetConstantTable() { return m_ConstantTable; }
		[[nodiscard]] uint8_t* Bytecode() { return m_Bytecode.data(); }
		[[nodiscard]] size_t BytecodeSize() const { return m_Bytecode.size(); }
		void Grow(size_t size);

	  private:
		ConstantTable m_ConstantTable;
		std::vector<uint8_t> m_Bytecode;
	};

} // namespace Glyph::Bytecode
