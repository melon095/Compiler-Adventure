use std::any::Any;

use crate::{chunk::Chunk, op_codes::ByteOp, Value};

use super::{
    token::{Token, TokenType},
    CompileError,
};

#[derive(Debug, PartialEq, Eq)]
enum Precedence {
    None,
    Assignment, // =
    Or,         // or
    And,        // and
    Equality,   // == !=
    Comparison, // < > <= >=
    Term,       // + -
    Factor,     // * /
    Unary,      // ! -
    Call,       // . ()
    Primary,
}

pub struct Parser<'a> {
    chunk: &'a mut Chunk,
    tokens: Vec<Token>,
    current: usize,
}

impl<'a> Parser<'a> {
    pub fn new(tokens: Vec<Token>, chunk: &'a mut Chunk) -> Self {
        Self {
            chunk,
            tokens,
            current: 0,
        }
    }

    pub fn previous_token(&self) -> &Token {
        // Don't think this will ever panic. lol
        self.tokens.get(self.current - 1).unwrap()
    }

    pub fn number(&mut self) -> Result<(), CompileError> {
        let token = self.previous_token();

        let TokenType::Number(value) = token.token_type else {
            // Should panic idk

            return Err(CompileError::CompileError(format!(
                "Expected number on line {}",
                token.line
            )));
        };

        self.emit_constant(value)?;
        return Ok(());
    }

    pub fn grouping(&mut self) -> Result<(), CompileError> {
        self.expression()?;

        self.consume(TokenType::RightParen, "Expect ')' after expression")?;

        Ok(())
    }

    pub fn unary(&mut self) -> Result<(), CompileError> {
        let token = self.previous_token().clone();

        self.parse_precedence(Precedence::Unary)?;

        match token.token_type {
            TokenType::Minus => self.emit_u8(ByteOp::OP_NEGATE),
            _ => {
                return Err(CompileError::CompileError(format!(
                    "Expected unary operator on line {}",
                    token.line
                )))
            }
        }

        Ok(())
    }

    pub fn parse_precedence(&mut self, precedence: Precedence) -> Result<(), CompileError> {
        todo!();
    }

    pub fn advance(&mut self) -> &Token {
        let token = self.tokens.get(self.current);

        if let Some(token) = token {
            self.current += 1;
            return token;
        }

        return &Token {
            token_type: TokenType::Eof,
            line: 0,
        };
    }

    pub fn consume(
        &mut self,
        token_type: TokenType,
        message: &str,
    ) -> Result<&Token, CompileError> {
        let token = self.advance();

        if token.token_type == token_type {
            return Ok(token);
        }

        Err(CompileError::CompileError(format!(
            "{} on line {}",
            message, token.line
        )))
    }

    pub fn expression(&mut self) -> Result<(), CompileError> {
        todo!();
    }

    pub fn emit_u8(&mut self, byte: u8) {
        self.chunk.bytecode.push(byte);
    }

    pub fn emit_constant(&mut self, value: Value) -> anyhow::Result<()> {
        let constant = self.chunk.add_constant(value)?;

        self.emit_two_u8(ByteOp::OP_CONSTANT, constant);

        Ok(())
    }

    pub fn emit_two_u8(&mut self, byte1: u8, byte2: u8) {
        self.chunk.bytecode.push(byte1);
        self.chunk.bytecode.push(byte2);
    }
}
