use crate::Value;

pub mod ByteOp {
    pub const OP_RETURN: u8 = 0;
    pub const OP_CONSTANT: u8 = 1;
    pub const OP_ADD: u8 = 2;
    pub const OP_SUBTRACT: u8 = 3;
    pub const OP_MULTIPLY: u8 = 4;
    pub const OP_DIVIDE: u8 = 5;
    pub const OP_NEGATE: u8 = 6;

    pub const OP_INVALID: u8 = 255;
}

// Note: parse_display crate won't work here cause of OP_ and elements containing data.
#[derive(Debug, Clone, Copy, PartialEq)]
pub enum OpCode {
    Return,
    Constant(Value),
    Addition,
    Subtraction,
    Multiplication,
    Division,
    Negate,
    Invalid,
}

impl Into<u8> for OpCode {
    fn into(self) -> u8 {
        use ByteOp::*;

        match self {
            OpCode::Return => OP_RETURN,
            OpCode::Constant(_) => OP_CONSTANT,
            OpCode::Addition => OP_ADD,
            OpCode::Subtraction => OP_SUBTRACT,
            OpCode::Multiplication => OP_MULTIPLY,
            OpCode::Division => OP_DIVIDE,
            OpCode::Negate => OP_NEGATE,
            OpCode::Invalid => OP_INVALID,
        }
    }
}

// TODO: Maybe move this into a Into string so we can return an error for invalid opcodes?
impl std::fmt::Display for OpCode {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "OP_")?;

        match self {
            OpCode::Return => write!(f, "RETURN"),
            OpCode::Constant(_) => write!(f, "CONSTANT"),
            OpCode::Addition => write!(f, "ADD"),
            OpCode::Subtraction => write!(f, "SUBTRACT"),
            OpCode::Multiplication => write!(f, "MULTIPLY"),
            OpCode::Division => write!(f, "DIVIDE"),
            OpCode::Negate => write!(f, "NEGATE"),
            OpCode::Invalid => write!(f, "INVALID"),
        }
    }
}
