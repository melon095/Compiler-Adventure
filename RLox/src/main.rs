use rlox::{
    chunk::{Chunk, OpCode},
    disassembler,
};

fn main() {
    let mut chunk = Chunk::new();
    chunk.write_op(OpCode::Return, 1);
    chunk.write_op(OpCode::Constant(1.2), 2);
    chunk.write_op(OpCode::Constant(2.3), 2);
    chunk.write_op(OpCode::Constant(3.4), 3);

    disassembler::disassemble_chunk(&chunk, "Test Chunk").unwrap();
}
