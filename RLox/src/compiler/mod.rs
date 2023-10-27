pub mod scanner;

use crate::{chunk::Chunk, compiler::scanner::TokenType};

use scanner::Scanner;

pub fn compile(code: String, chunk: &mut Chunk) -> anyhow::Result<()> {
    let mut scanner = Scanner::new(code);

    let mut line: u32 = 0;
    while let token = scanner.scan_token()? {
        if let TokenType::Eof = token.typ {
            break;
        }

        if token.line != line {
            print!("{:04} ", token.line);
            line = token.line;
        } else {
            print!("   | ");
        }

        println!("{:02} '{}'", token.typ, token.lexeme);
    }

    return Ok(());
}
