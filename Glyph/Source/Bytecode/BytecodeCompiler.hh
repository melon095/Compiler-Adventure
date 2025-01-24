#pragma once

#include <Bytecode/BytecodeChunk.hh>
#include <Bytecode/BytecodeInstruction.hh>
#include <Runtime/ConstantTable.hh>

#include <absl/status/status.h>

namespace Glyph::Bytecode
{
	class BytecodeCompiler
	{
	  public:
		template<typename TInstruction, typename... Args> absl::Status Emit(Args&&... args)
		{
			auto instrStart = m_Chunk.BytecodeSize();
			auto instructionSize = sizeof(TInstruction);
			m_Chunk.Grow(instructionSize);
			auto* instructionArena = m_Chunk.Bytecode() + instrStart;

			new(instructionArena) TInstruction(std::forward<Args>(args)...);

			return absl::OkStatus();
		}

		ConstantTableIndex MakeConstant(const Value& value);

	  public:
		BytecodeChunk m_Chunk; // FIXME: Multiple chunks(frames) per functions and scopes. One for now.
	};
} // namespace Glyph::Bytecode
