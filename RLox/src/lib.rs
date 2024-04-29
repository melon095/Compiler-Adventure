pub mod chunk;
pub mod compiler;
pub mod disassembler;
pub mod op_codes;
pub mod vm;

// TODO: Value should become an enum that can hold any type of value
pub type Value = f64;
