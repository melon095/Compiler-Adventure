use crate::Value;

pub struct Chunk {
    pub bytecode: Vec<u8>,
    pub constants: Vec<Value>,
}

impl Chunk {
    pub fn new() -> Self {
        Self {
            bytecode: Vec::new(),
            constants: Vec::new(),
        }
    }

    pub fn add_constant(&mut self, value: Value) -> anyhow::Result<u8> {
        self.constants.push(value);
        if self.constants.len() > u8::MAX as usize {
            return Err(anyhow::anyhow!("Too many constants in one chunk"));
        }

        Ok((self.constants.len() - 1) as u8)
    }

    // pub fn write_op(&mut self, op: OpCode, line: u32) {
    //     self.bytecode.push(Instruction::new(op, line, 0));
    // }

    // pub fn write8(&mut self, byte: u8, line: u32) {
    //     self.bytecode
    //         .push(Instruction::new(OpCode::Constant(byte.into()), line, 0));
    // }
}
