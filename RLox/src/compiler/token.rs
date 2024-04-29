use parse_display::Display;

#[derive(Debug, Display, PartialEq, Clone)]
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
    #[display("Literal({0})")]
    Literal(String),
    #[display("String({0})")]
    String(String),
    #[display("Number({0})")]
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
    pub token_type: TokenType,
    pub line: u32,
}
