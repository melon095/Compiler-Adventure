use parse_display::Display;
use thiserror::Error;

#[derive(Debug, Display, PartialEq, Eq)]
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
    Identifier,
    String,
    Number,

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

// TODO: Something something &str

pub struct Token {
    pub typ: TokenType,
    pub lexeme: String,
    pub line: u32,
}

#[derive(Debug, Error)]
pub enum ScanError {
    #[error("Invalid token: {0}")]
    InvalidToken(char),
}

pub struct Scanner {
    pub source: String,
    pub start: usize,
    pub current: usize,
    pub line: u32,
}

// TODO: Maybe use Iterator
// TODO: Tokens should not hold the text themselves, rather two integer indices into the source string

impl Scanner {
    pub fn new(source: String) -> Self {
        Self {
            source,
            start: 0,
            current: 0,
            line: 1,
        }
    }

    fn make_token(&self, tok: TokenType) -> Token {
        Token {
            typ: tok,
            lexeme: self.source[self.start..self.current].to_string(),
            line: self.line,
        }
    }

    fn peek(&self) -> Option<char> {
        if self.is_at_end() {
            return None;
        }

        self.source.chars().nth(self.current)
    }

    fn peek_next(&self) -> Option<char> {
        if self.current + 1 >= self.source.len() {
            return None;
        }

        self.source.chars().nth(self.current + 1)
    }

    fn match_char(&mut self, expected: char) -> bool {
        if self.is_at_end() {
            return false;
        }

        let Some(c) = self.source.chars().nth(self.current) else {
            return false;
        };

        if c != expected {
            return false;
        }

        self.current += 1;

        true
    }

    fn is_at_end(&self) -> bool {
        self.current >= self.source.len()
    }

    fn consume(&mut self) -> Option<char> {
        if self.is_at_end() {
            return None;
        }

        let c = self.source.chars().nth(self.current);
        self.current += 1;

        c
    }

    fn consume_while<F>(&mut self, predicate: F) -> Vec<char>
    where
        F: Fn(char) -> bool,
    {
        let mut chars = Vec::new();

        while let Some(c) = self.peek() {
            if !predicate(c) {
                break;
            }

            chars.push(c);
            self.consume();
        }

        chars
    }

    fn choose_next(&mut self, expected: char, if_true: TokenType, if_false: TokenType) -> Token {
        if self.match_char(expected) {
            return self.make_token(if_true);
        }

        self.make_token(if_false)
    }

    pub fn scan_token(&mut self) -> anyhow::Result<Token, ScanError> {
        self.start = self.current;

        let c = match self.consume() {
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
            '/' => {
                if self.match_char('/') {
                    self.consume_while(|c| c != '\n');
                    // New line
                    self.consume();
                    self.line += 1;
                    self.scan_token()
                } else if self.match_char('*') {
                    while let Some(c) = self.peek() {
                        if c == '\n' {
                            self.line += 1;
                        }

                        if c == '*' && self.peek_next() == Some('/') {
                            self.consume();
                            self.consume();
                            break;
                        }

                        self.consume();
                    }

                    self.scan_token()
                } else {
                    Ok(self.make_token(TokenType::Slash))
                }
            }

            // Whitespace
            // TODO: Figure out a better way to do this.
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
        let mut scanner = Scanner::new("()".to_string());
        let tok = scanner.scan_token().unwrap();
        assert_eq!(tok.typ, TokenType::LeftParen);
        assert_eq!(tok.lexeme, "(");
        assert_eq!(tok.line, 1);

        let tok = scanner.scan_token().unwrap();
        assert_eq!(tok.typ, TokenType::RightParen);
        assert_eq!(tok.lexeme, ")");
        assert_eq!(tok.line, 1);

        let tok = scanner.scan_token().unwrap();
        assert_eq!(tok.typ, TokenType::Eof);
        assert_eq!(tok.lexeme, "");
        assert_eq!(tok.line, 1);
    }

    #[test]
    fn test_scanner_comment() {
        let mut scanner = Scanner::new("// This is a comment\n()".to_string());
        let tok = scanner.scan_token().unwrap();
        assert_eq!(tok.typ, TokenType::LeftParen);
        assert_eq!(tok.lexeme, "(");
        assert_eq!(tok.line, 2);

        let tok = scanner.scan_token().unwrap();
        assert_eq!(tok.typ, TokenType::RightParen);
        assert_eq!(tok.lexeme, ")");
        assert_eq!(tok.line, 2);

        let tok = scanner.scan_token().unwrap();
        assert_eq!(tok.typ, TokenType::Eof);
        assert_eq!(tok.lexeme, "");
        assert_eq!(tok.line, 2);
    }

    #[test]
    fn test_multiline_comment() {
        let mut scanner = Scanner::new("/* This\n\n is a\n\n comment\n\n */()".to_string());
        let tok = scanner.scan_token().unwrap();
        assert_eq!(tok.typ, TokenType::LeftParen);
        assert_eq!(tok.lexeme, "(");
        assert_eq!(tok.line, 7);

        let tok = scanner.scan_token().unwrap();
        assert_eq!(tok.typ, TokenType::RightParen);
        assert_eq!(tok.lexeme, ")");
        assert_eq!(tok.line, 7);

        let tok = scanner.scan_token().unwrap();
        assert_eq!(tok.typ, TokenType::Eof);
        assert_eq!(tok.lexeme, "");
        assert_eq!(tok.line, 7);
    }
}
