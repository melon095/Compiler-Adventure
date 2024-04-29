// use crate::{chunk::Chunk, op_codes::OpCode};
// use std::fmt::Write;

// pub fn disassemble_chunk(chunk: &Chunk, name: &str) -> anyhow::Result<()> {
//     let mut output = String::new();

//     println!("== {} ==", name);

//     let mut offset = 0;
//     for byte in chunk.bytecode.iter() {
//         let r = disassemble_instruction(chunk, byte, offset, &mut output);

//         if let Ok(new_offset) = r {
//             println!("{}", output);
//             output.clear();
//             offset = new_offset;
//         } else if let Err(err) = r {
//             eprintln!("Error disassembling chunk: {}", err);
//         }
//     }

//     Ok(())
// }

// pub fn disassemble_instruction(
//     #[allow(unused_variables)] chunk: &Chunk,
//     op: OpCode,
//     offset: usize,
//     f: &mut String,
// ) -> anyhow::Result<usize> {
//     write!(f, "{:04} ", offset)?;

//     // // TODO: https://github.com/rust-lang/rust/issues/53667
//     // if let Some(last) = chunk.bytecode.last() {
//     //     if instruction.line == last.line {
//     //         write!(f, "   | ")?;
//     //     } else {
//     //         write!(f, "{:04} ", instruction.line)?;
//     //     }
//     // }

//     match op {
//         OpCode::Return
//         | OpCode::Invalid
//         | OpCode::Addition
//         | OpCode::Division
//         | OpCode::Multiplication
//         | OpCode::Subtraction
//         | OpCode::Negate => {
//             write!(f, "{}", op)?;

//             Ok(offset + 1)
//         }
//         OpCode::Constant(value) => {
//             write!(f, "{:<16} '{}'", op, value)?;

//             Ok(offset + 2)
//         }
//     }
// }
