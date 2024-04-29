use std::collections::HashMap;
use std::sync::OnceLock;

use super::{token::Token, ScanError, TokenType};

static GLOBAL_KEYWORDS: OnceLock<HashMap<&str, TokenType>> = OnceLock::new();

fn init_keywords() -> HashMap<&'static str, TokenType> {
    let mut keywords: HashMap<&str, TokenType> = HashMap::new();

    keywords.insert("and", TokenType::And);
    keywords.insert("class", TokenType::Class);
    keywords.insert("else", TokenType::Else);
    keywords.insert("false", TokenType::False);
    keywords.insert("for", TokenType::For);
    keywords.insert("fn", TokenType::Function);
    keywords.insert("if", TokenType::If);
    keywords.insert("nil", TokenType::Nil);
    keywords.insert("or", TokenType::Or);
    // TODO: Remove this
    keywords.insert("print", TokenType::Print);
    keywords.insert("return", TokenType::Return);
    keywords.insert("super", TokenType::Super);
    keywords.insert("self", TokenType::This);
    keywords.insert("true", TokenType::True);
    keywords.insert("let", TokenType::Let);
    keywords.insert("mut", TokenType::Mutable);
    keywords.insert("while", TokenType::While);

    keywords
}

pub struct Scanner<'a> {
    // TODO: Remove 'source'
    source: &'a String,
    it: std::iter::Peekable<std::str::Chars<'a>>,
    line: u32,
}

impl<'a> Scanner<'a> {
    pub fn new(source: &'a String) -> Self {
        _ = GLOBAL_KEYWORDS.get_or_init(init_keywords);

        Self {
            source,
            it: source.chars().peekable(),
            line: 1,
        }
    }

    fn make_token(&self, tok: TokenType) -> Token {
        Token {
            token_type: tok,
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
        let keywords = GLOBAL_KEYWORDS.get().unwrap();
        let mut ident = String::from(c);

        ident.push_str(
            &self
                .consume_while(|c| c.is_alphanumeric() || *c == '_')
                .into_iter()
                .collect::<String>(),
        );

        if let Some(keyword) = keywords.get(&ident.as_str()) {
            return Ok(self.make_token(keyword.clone()));
        }

        return Ok(self.make_token(TokenType::Literal(ident)));
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

    pub fn scan_up_to_eof(mut self) -> anyhow::Result<Vec<Token>, ScanError> {
        let mut tokens = Vec::new();

        loop {
            let token = self.scan_token()?;

            if token.token_type == TokenType::Eof {
                break;
            }

            tokens.push(token);
        }

        Ok(tokens)
    }
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
            TokenType::Semicolon,
            TokenType::Let,
            TokenType::Literal("b".to_string()),
            TokenType::Equal,
            TokenType::Number(5.0),
            TokenType::Star,
            TokenType::Number(20.0),
        ];

        let text = r#"()}{,some_variable = "Hi :)"-;, 42 12.5, a; let b = 5 * 20"#.to_string();
        let mut scanner = Scanner::new(&text);

        for expected in expected {
            let tok = scanner.scan_token().unwrap();
            assert_eq!(tok.token_type, expected);
            assert_eq!(tok.line, 1)
        }
    }

    // TODO: Implement this.
    #[test]
    fn test_string_interpolation() {
        let text = r#"let mut foo = 5; return "The number is ${foo}";"#.to_string();

        let expected = vec![
            TokenType::Let,
            TokenType::Mutable,
            TokenType::Literal("foo".to_string()),
            TokenType::Equal,
            TokenType::Number(5.0),
            TokenType::Semicolon,
            TokenType::Return,
            TokenType::String("The number is ${foo}".to_string()),
            TokenType::Semicolon,
        ];

        let mut scanner = Scanner::new(&text);

        for expected in expected {
            let tok = scanner.scan_token().unwrap();
            assert_eq!(tok.token_type, expected);
            assert_eq!(tok.line, 1)
        }
    }

    #[test]
    fn test_scanner_comment() {
        let text = "// This is a comment\n()".to_string();
        let mut scanner = Scanner::new(&text);
        let tok = scanner.scan_token().unwrap();
        assert_eq!(tok.token_type, TokenType::LeftParen);
        assert_eq!(tok.line, 2);

        let tok = scanner.scan_token().unwrap();
        assert_eq!(tok.token_type, TokenType::RightParen);
        assert_eq!(tok.line, 2);

        let tok = scanner.scan_token().unwrap();
        assert_eq!(tok.token_type, TokenType::Eof);
        assert_eq!(tok.line, 2);
    }

    #[test]
    fn test_multiline_comment() {
        let text =
            "(/* This\n\n is /* \nDepth 2\n\r\t\t\t\t\t */ a\n\n comment\n\n */()".to_string();
        let mut scanner = Scanner::new(&text);
        let tok = scanner.scan_token().unwrap();
        assert_eq!(tok.token_type, TokenType::LeftParen);
        assert_eq!(tok.line, 1);

        let tok = scanner.scan_token().unwrap();
        assert_eq!(tok.token_type, TokenType::LeftParen);
        assert_eq!(tok.line, 9);

        let tok = scanner.scan_token().unwrap();
        assert_eq!(tok.token_type, TokenType::RightParen);
        assert_eq!(tok.line, 9);

        let tok = scanner.scan_token().unwrap();
        assert_eq!(tok.token_type, TokenType::Eof);
        assert_eq!(tok.line, 9);
    }
}
