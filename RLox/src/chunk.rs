use std::fmt::Write;

type Value = f64;

#[derive(Debug, Clone, Copy, PartialEq)]
pub enum OpCode {
    Return,
    Constant(Value),

    Invalid,
}

// TODO: Maybe move this into a Into string so we can return an error for invalid opcodes?
impl std::fmt::Display for OpCode {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "OP_")?;

        match self {
            OpCode::Return => write!(f, "RETURN"),
            OpCode::Constant(_) => write!(f, "CONSTANT"),
            OpCode::Invalid => write!(f, "INVALID"),
        }
    }
}

// Not the prettiest name

#[derive(Debug, Clone, Copy)]
pub struct Operation {
    pub line: u32,
    pub offset: u32,
    pub op: OpCode,
}

impl Operation {
    pub fn new(op: OpCode, line: u32, offset: u32) -> Self {
        Self { op, line, offset }
    }

    pub fn disassemble(
        &self,
        #[allow(unused_variables)] chunk: &Chunk,
        offset: usize,
        f: &mut String,
    ) -> anyhow::Result<usize> {
        write!(f, "{:04} ", offset)?;

        // TODO: https://github.com/rust-lang/rust/issues/53667
        if let Some(last) = chunk.bytecode.last() {
            if self.line == last.line {
                write!(f, "   | ")?;
            } else {
                write!(f, "{:04} ", self.line)?;
            }
        }

        // TODO: Find a better way to handle default cases. I don't like the idea of a catch-all.

        match self.op {
            OpCode::Return => {
                write!(f, "{}", self.op)?;

                Ok(offset + 1)
            }
            OpCode::Constant(value) => {
                write!(f, "{:<16} '{}'", self.op, value)?;

                Ok(offset + 2)
            }
            OpCode::Invalid => {
                write!(f, "{}", self.op)?;

                Ok(offset + 1)
            }
        }
    }
}

pub struct Chunk {
    pub bytecode: Vec<Operation>,
}

impl Chunk {
    pub fn new() -> Self {
        Self {
            bytecode: Vec::new(),
        }
    }

    pub fn write_op(&mut self, op: OpCode, line: u32) {
        self.bytecode.push(Operation::new(op, line, 0));
    }
}
