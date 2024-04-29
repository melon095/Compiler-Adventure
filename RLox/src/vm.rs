use crate::{chunk::Chunk, op_codes::OpCode, Value};
use std::collections::VecDeque;
use thiserror::Error;

const STACK_MAX: usize = 256;

macro_rules! binary_op {
    ($vm:ident, $op:tt) => {
        {
            if $vm.stack.len() < 2 {
                todo!("Stack underflow");
            }

            let b = $vm.stack.pop_back().unwrap();
            let a = $vm.stack.pop_back().unwrap();

            $vm.stack.push_back(a $op b);
        }
    };
}

#[derive(Debug, Error)]
pub enum InterpretError {
    #[error("Stack overflow")]
    StackOverflow,
}

pub struct VM {
    // TODO: Remove 'Option'.
    pub chunk: Option<Chunk>,

    // TODO: We might be able to remove the instruction pointer.
    pub ip: usize,
    pub stack: VecDeque<Value>,
}

impl VM {
    pub fn new() -> Self {
        Self {
            chunk: None,
            ip: 0,
            stack: VecDeque::new(),
        }
    }

    pub fn stack_push(&mut self, value: Value) -> anyhow::Result<(), InterpretError> {
        if self.stack.len() >= STACK_MAX {
            return Err(InterpretError::StackOverflow);
        }

        self.stack.push_back(value);

        Ok(())
    }

    pub fn stack_pop(&mut self) -> Option<Value> {
        self.stack.pop_back()
    }

    pub fn stack_reset(&mut self) {
        self.stack.clear();
    }

    pub fn interpret(&mut self, chunk: Chunk) -> anyhow::Result<(), InterpretError> {
        self.chunk = Some(chunk);
        self.ip = 0;

        // TODO: Fix clone.
        let iter = self.chunk.as_ref().unwrap().bytecode.clone();
        for op in iter.iter() {
            // if cfg!(debug_assertions) {
            //     let mut f = String::new();
            //     f.push_str("Stack: ");

            //     for item in self.stack.iter() {
            //         write!(f, "[ {} ]", item).expect("Failed to write to string");
            //     }

            //     f.push('\n');

            //     disassemble_instruction(
            //         &self.chunk.as_ref().unwrap(),
            //         instruction,
            //         self.ip,
            //         &mut f,
            //     )
            //     .unwrap();
            //     println!("{}", f);
            // }

            // match op {
            //     OpCode::Return => {
            //         if self.stack.is_empty() {
            //             todo!("Pop from empty stack")
            //         }

            //         println!("{}", self.stack_pop().unwrap());

            //         return Ok(());
            //     }
            //     OpCode::Constant(constant) => self.stack_push(constant)?,
            //     OpCode::Addition => binary_op!(self, +),
            //     OpCode::Division => binary_op!(self, /),
            //     OpCode::Multiplication => binary_op!(self, *),
            //     OpCode::Subtraction => binary_op!(self, -),
            //     OpCode::Negate => {
            //         if let Some(number) = self.stack_pop() {
            //             self.stack_push(-number)?;
            //         } else {
            //             todo!("Expected number")
            //         }
            //     }
            //     OpCode::Invalid => todo!(),
            // }
        }

        Ok(())
    }
}
