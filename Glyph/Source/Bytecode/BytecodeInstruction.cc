#include <Bytecode/BytecodeInstruction.hh>

#include <magic_enum/magic_enum.hpp>

namespace Glyph::Bytecode
{
	Instruction::Instruction(Opcode type)
		: m_Type(type)
	{
	}

	LoadConstInstruction::LoadConstInstruction(ConstantTableIndex index)
		: Instruction(Opcode::LOAD_CONST)
		, m_Index(index)
	{
	}

	void LoadConstInstruction::Execute(VM& vm) {}

	std::string LoadConstInstruction::ToString() const { return "LOAD_CONST(" + std::to_string(m_Index) + ")"; }

	StoreNameInstruction::StoreNameInstruction(ConstantTableIndex index)
		: Instruction(Opcode::STORE_NAME)
		, m_Index(index)
	{
	}

	void StoreNameInstruction::Execute(VM& vm) {}

	std::string StoreNameInstruction::ToString() const { return "STORE_NAME(" + std::to_string(m_Index) + ")"; }

	EndInstruction::EndInstruction()
		: Instruction(Opcode::END)
	{
	}

	void EndInstruction::Execute(VM& vm) {}

	std::string EndInstruction::ToString() const { return "END"; }
} // namespace Glyph::Bytecode
