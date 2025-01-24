#pragma once

#include <Runtime/ConstantTable.hh>

#include <cstdint>

namespace Glyph::Bytecode
{
	class VM;
#define INSTRUCTION_LIST(V)                                                                                            \
	V(LOAD_CONST, LoadConst)                                                                                           \
	V(STORE_NAME, StoreName)                                                                                           \
	V(ADD, Add)                                                                                                        \
	V(RETURN, Return)                                                                                                  \
	V(END, End)

#define DECLARE_ENUM_TYPE(name, _) name,
	enum class Opcode
	{
		INSTRUCTION_LIST(DECLARE_ENUM_TYPE)
	};
#undef DECLARE_ENUM_TYPE

	class alignas(void*) Instruction
	{
	  public:
		// NOTE: No virtual functions due to it being copied into the bytecode list and sizeof doesn't account for it.
		Instruction(const Instruction&) = default;

		[[nodiscard]] Opcode GetType() const { return m_Type; }
		[[nodiscard]] bool IsType(Opcode type) const { return m_Type == type; }

		//		virtual void Execute(class VM&) = 0;
		//		virtual std::string ToString() const = 0;

	  protected:
		explicit Instruction(Opcode type);

	  private:
		Opcode m_Type;
	};

	class LoadConstInstruction : public Instruction
	{
	  public:
		explicit LoadConstInstruction(ConstantTableIndex index);

		LoadConstInstruction(const LoadConstInstruction&) = default;

		void Execute(VM& vm);
		[[nodiscard]] std::string ToString() const;

		ConstantTableIndex Index() const { return m_Index; }

	  private:
		ConstantTableIndex m_Index;
	};

	class StoreNameInstruction : public Instruction
	{
	  public:
		explicit StoreNameInstruction(ConstantTableIndex index);

		StoreNameInstruction(const StoreNameInstruction&) = default;

		void Execute(VM& vm);
		[[nodiscard]] std::string ToString() const;

	  private:
		ConstantTableIndex m_Index;
	};

	class AddInstruction : public Instruction
	{
	  public:
		AddInstruction();

		AddInstruction(const AddInstruction&) = default;

		void Execute(VM& vm);
		[[nodiscard]] std::string ToString() const;
	};

	class ReturnInstruction : public Instruction
	{
	  public:
		ReturnInstruction();

		ReturnInstruction(const ReturnInstruction&) = default;

		void Execute(VM& vm);
		[[nodiscard]] std::string ToString() const;
	};

	class EndInstruction : public Instruction
	{
	  public:
		EndInstruction();

		EndInstruction(const EndInstruction&) = default;

		void Execute(VM& vm);
		[[nodiscard]] std::string ToString() const;
	};

} // namespace Glyph::Bytecode
