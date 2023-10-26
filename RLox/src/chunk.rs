use crate::Value;

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

// Not the prettiest name

#[derive(Debug, Clone, Copy)]
pub struct Instruction {
    pub line: u32,
    pub offset: u32,
    pub op: OpCode,
}

impl Instruction {
    pub fn new(op: OpCode, line: u32, offset: u32) -> Self {
        Self { op, line, offset }
    }
}

pub struct Chunk {
    pub bytecode: Vec<Instruction>,
}

impl Chunk {
    pub fn new() -> Self {
        Self {
            bytecode: Vec::new(),
        }
    }

    pub fn write_op(&mut self, op: OpCode, line: u32) {
        self.bytecode.push(Instruction::new(op, line, 0));
    }
}
