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
    Literal(String),
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

    #[error("Unterminated string on line {0}")]
    UnterminatedString(u32),
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

    fn scan_string(&mut self) -> Result<Token, ScanError> {
        let chars: Vec<char> = self.consume_while(|c| *c != '"');

        if self.it.next().is_none() {
            return Err(ScanError::UnterminatedString(self.line));
        }

        Ok(self.make_token(TokenType::String(chars.into_iter().collect())))
    }

    fn scan_number(&mut self, c: char) -> Result<Token, ScanError> {
        let mut number: String = String::from(c);

        let mut rest_num = self.consume_while(|c| c.is_ascii_digit());

        if self.it.peek() == Some(&'.') {
            rest_num.push(self.it.next().unwrap()); // Unwrap is fine here.

            rest_num.extend(self.consume_while(|c| c.is_ascii_digit()));
        }

        number.extend(rest_num);

        let num = number.parse::<f64>().expect("Invalid number"); // TODO: Handle ParseFloatError

        Ok(self.make_token(TokenType::Number(num)))
    }

    fn scan_identifier(&mut self, c: char) -> Result<Token, ScanError> {
        let mut ident = String::from(c);

        ident.push_str(
            &self
                .consume_while(|c| c.is_alphanumeric() || *c == '_')
                .into_iter()
                .collect::<String>(),
        );

        return Ok(self.make_token(token_type_from_ident(ident)));
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
            '"' => self.scan_string(),
            c if c.is_numeric() => self.scan_number(c),
            c if c.is_alphabetic() || c == '_' => self.scan_identifier(c),

            // Unknown
            c => Err(ScanError::InvalidToken(c)),
        }
    }
}

fn token_type_from_ident(ident: String) -> TokenType {
    // TODO: Keywords
    TokenType::Literal(ident)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_scan_token() {
        let expected = vec![
            TokenType::LeftParen,
            TokenType::RightParen,
            TokenType::RightBrace,
            TokenType::LeftBrace,
            TokenType::Comma,
            TokenType::Literal("some_variable".to_string()),
            TokenType::Equal,
            TokenType::String("Hi :)".to_string()),
            TokenType::Minus,
            TokenType::Semicolon,
            TokenType::Comma,
            TokenType::Number(42.0),
            TokenType::Number(12.5),
            TokenType::Comma,
            TokenType::Literal("a".to_string()),
        ];

        let text = r#"()}{,some_variable = "Hi :)"-;, 42 12.5, a"#.to_string();
        let mut scanner = Scanner::new(&text);

        for expected in expected {
            let tok = scanner.scan_token().unwrap();
            assert_eq!(tok.typ, expected);
            assert_eq!(tok.line, 1)
        }
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
