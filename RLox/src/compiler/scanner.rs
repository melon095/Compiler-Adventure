use parse_display::Display;
use thiserror::Error;

#[derive(Debug, Display, PartialEq)]
#[display(style = "CamelCase")]
pub enum TokenType {
    // Single-character tokens.
    LeftParen,
    RightParen,
    LeftBrace,
    RightBrace,
    Comma,
    Dot,
    Minus,
    Plus,
    Semicolon,
    Slash,
    Star,

    // One or two character tokens.
    Bang,
    BangEqual,
    Equal,
    EqualEqual,
    Greater,
    GreaterEqual,
    Less,
    LessEqual,

    // Literals.
    #[display("{0}")]
    Identifier(String),
    #[display("{0}")]
    String(String),
    #[display("{0}")]
    Number(f64),

    // Keywords.
    And,
    Class,
    Else,
    False,
    Function,
    For,
    If,
    Nil,
    Or,
    Print,
    Return,
    Super,
    /// Self
    This,
    True,
    Let,
    Mutable,
    While,

    Eof,
}

pub struct Token {
    pub typ: TokenType,
    pub line: u32,
}

#[derive(Debug, Error)]
pub enum ScanError {
    #[error("Invalid token: {0}")]
    InvalidToken(char),

    #[error("Unterminated multi-line comment on line {0}")]
    UnterminatedComment(u32),
}

pub struct Scanner<'a> {
    pub source: &'a String,
    pub it: std::iter::Peekable<std::str::Chars<'a>>,
    pub line: u32,
}

impl<'a> Scanner<'a> {
    pub fn new(source: &'a String) -> Self {
        Self {
            source,
            it: source.chars().peekable(),
            line: 1,
        }
    }

    fn make_token(&self, tok: TokenType) -> Token {
        Token {
            typ: tok,
            line: self.line,
        }
    }

    fn consume_multi_line_comment(&mut self) -> Result<(), ScanError> {
        let mut depth = 1;

        while depth > 0 {
            let c = self.it.next();

            if c.is_none() {
                return Err(ScanError::UnterminatedComment(self.line));
            }

            let c = c.unwrap();

            match c {
                '\n' => self.line += 1,
                '/' => {
                    // lol
                    if self.it.peek() == Some(&'*') {
                        depth += 1;
                        self.it.next();
                    }
                }
                '*' => {
                    // lol
                    if self.it.peek() == Some(&'/') {
                        depth -= 1;
                        self.it.next();
                    }
                }
                _ => (),
            };
        }

        Ok(())
    }

    fn consume_while<F>(&mut self, predicate: F) -> Vec<char>
    where
        F: Fn(&char) -> bool,
    {
        let mut chars: Vec<char> = Vec::new();

        loop {
            let c = self.it.peek();

            if c.is_none() {
                break;
            }

            let c = c.unwrap();

            if !predicate(c) {
                break;
            }

            chars.push(*c);
            self.it.next();
        }

        chars
    }

    fn choose_next(&mut self, expected: char, if_true: TokenType, if_false: TokenType) -> Token {
        if let Some(next) = self.it.next() {
            if next == expected {
                return self.make_token(if_true);
            }
        }

        self.make_token(if_false)
    }

    pub fn scan_token(&mut self) -> anyhow::Result<Token, ScanError> {
        let c = match self.it.next() {
            None => return Ok(self.make_token(TokenType::Eof)),
            Some(c) => c,
        };

        match c {
            // Single Char Tokens
            '(' => Ok(self.make_token(TokenType::LeftParen)),
            ')' => Ok(self.make_token(TokenType::RightParen)),
            '{' => Ok(self.make_token(TokenType::LeftBrace)),
            '}' => Ok(self.make_token(TokenType::RightBrace)),
            ',' => Ok(self.make_token(TokenType::Comma)),
            '.' => Ok(self.make_token(TokenType::Dot)),
            '-' => Ok(self.make_token(TokenType::Minus)),
            '+' => Ok(self.make_token(TokenType::Plus)),
            ';' => Ok(self.make_token(TokenType::Semicolon)),
            '*' => Ok(self.make_token(TokenType::Star)),

            // One or two char tokens
            '!' => Ok(self.choose_next('=', TokenType::BangEqual, TokenType::Bang)),
            '=' => Ok(self.choose_next('=', TokenType::EqualEqual, TokenType::Equal)),
            '<' => Ok(self.choose_next('=', TokenType::LessEqual, TokenType::Less)),
            '>' => Ok(self.choose_next('=', TokenType::GreaterEqual, TokenType::Greater)),
            '/' => match self.it.next() {
                Some('/') => {
                    self.consume_while(|c| *c != '\n');
                    self.scan_token()
                }
                Some('*') => {
                    self.consume_multi_line_comment()?;
                    self.scan_token()
                }
                _ => Ok(self.make_token(TokenType::Slash)),
            },

            // Whitespace
            ' ' | '\t' | '\r' => self.scan_token(),
            '\n' => {
                self.line += 1;
                self.scan_token()
            }
            // Long tokens
            '"' => todo!("Strings"),

            // Unknown
            c => Err(ScanError::InvalidToken(c)),
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_scan_token() {
        let text = "()".to_string();
        let mut scanner = Scanner::new(&text);
        let tok = scanner.scan_token().unwrap();
        assert_eq!(tok.typ, TokenType::LeftParen);
        assert_eq!(tok.line, 1);

        let tok = scanner.scan_token().unwrap();
        assert_eq!(tok.typ, TokenType::RightParen);
        assert_eq!(tok.line, 1);

        let tok = scanner.scan_token().unwrap();
        assert_eq!(tok.typ, TokenType::Eof);
        assert_eq!(tok.line, 1);
    }

    #[test]
    fn test_scanner_comment() {
        let text = "// This is a comment\n()".to_string();
        let mut scanner = Scanner::new(&text);
        let tok = scanner.scan_token().unwrap();
        assert_eq!(tok.typ, TokenType::LeftParen);
        assert_eq!(tok.line, 2);

        let tok = scanner.scan_token().unwrap();
        assert_eq!(tok.typ, TokenType::RightParen);
        assert_eq!(tok.line, 2);

        let tok = scanner.scan_token().unwrap();
        assert_eq!(tok.typ, TokenType::Eof);
        assert_eq!(tok.line, 2);
    }

    #[test]
    fn test_multiline_comment() {
        let text =
            "(/* This\n\n is /* \nDepth 2\n\r\t\t\t\t\t */ a\n\n comment\n\n */()".to_string();
        let mut scanner = Scanner::new(&text);
        let tok = scanner.scan_token().unwrap();
        assert_eq!(tok.typ, TokenType::LeftParen);
        assert_eq!(tok.line, 1);

        let tok = scanner.scan_token().unwrap();
        assert_eq!(tok.typ, TokenType::LeftParen);
        assert_eq!(tok.line, 9);

        let tok = scanner.scan_token().unwrap();
        assert_eq!(tok.typ, TokenType::RightParen);
        assert_eq!(tok.line, 9);

        let tok = scanner.scan_token().unwrap();
        assert_eq!(tok.typ, TokenType::Eof);
        assert_eq!(tok.line, 9);
    }
}
