use rlox::chunk::{Chunk, OpCode};

fn main() -> anyhow::Result<()> {
    let mut vm = rlox::vm::VM::new();

    let mut chunk = Chunk::new();
    // chunk.write_op(OpCode::Constant(1.2_f64), 2);
    // chunk.write_op(OpCode::Constant(2.3_f64), 2);
    // chunk.write_op(OpCode::Constant(1.2_f64), 3);

    // chunk.write_op(OpCode::Addition, 4);
    // chunk.write_op(OpCode::Negate, 5);

    // chunk.write_op(OpCode::Return, 6);

    chunk.write_op(OpCode::Constant(3.4_f64), 1);
    chunk.write_op(OpCode::Constant(3.4_f64), 1);
    chunk.write_op(OpCode::Constant(1.2_f64), 2);

    chunk.write_op(OpCode::Addition, 2);
    // 1.2 + 3.4 = 4.6

    chunk.write_op(OpCode::Subtraction, 3);
    // 4.6 - 3.4 = 1.2

    chunk.write_op(OpCode::Negate, 4);
    // -1.2

    chunk.write_op(OpCode::Return, 5);

    // disassembler::disassemble_chunk(&chunk, "Test Chunk")?;

    vm.interpret(chunk)?;

    return Ok(());
}
