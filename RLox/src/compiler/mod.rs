pub mod parser;
pub mod scanner;
pub mod token;

use crate::{chunk::Chunk, compiler::token::TokenType, op_codes::ByteOp};

use scanner::Scanner;
use thiserror::Error;

use self::parser::Parser;

#[derive(Debug, Error)]
pub enum ScanError {
    #[error("Invalid token: {0}")]
    InvalidToken(char),

    #[error("Unterminated multi-line comment on line {0}")]
    UnterminatedComment(u32),

    #[error("Unterminated string on line {0}")]
    UnterminatedString(u32),
}

#[derive(Debug, Error)]
pub enum CompileError {
    #[error("Compile error: {0}")]
    CompileError(String),
}

impl From<ScanError> for CompileError {
    fn from(error: ScanError) -> Self {
        Self::CompileError(error.to_string())
    }
}

impl From<anyhow::Error> for CompileError {
    fn from(error: anyhow::Error) -> Self {
        Self::CompileError(error.to_string())
    }
}

pub fn compile(code: String, chunk: &mut Chunk) -> anyhow::Result<()> {
    let scanner = Scanner::new(&code);
    let tokens = scanner.scan_up_to_eof()?;

    let mut parser = Parser::new(tokens, chunk);

    parser.advance();
    parser.expression()?;

    parser.consume(TokenType::Eof, "Expect end of expression")?;
    parser.emit_u8(ByteOp::OP_RETURN);

    // _ = scanner.scan_token()?;
    // expression();

    // let mut line: u32 = 0;
    // loop {
    //     let token = scanner.scan_token()?;

    //     if let TokenType::Eof = token.typ {
    //         break;
    //     }

    //     if token.line != line {
    //         print!("{:04} ", token.line);
    //         line = token.line;
    //     } else {
    //         print!("   | ");
    //     }

    //     println!("{:02}", token.typ);
    // }

    return Ok(());
}
