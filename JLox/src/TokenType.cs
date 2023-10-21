namespace JLox;

internal enum TokenType
{
    #region Single-character tokens
    LeftParen,
    RightParen,
    LeftBrace,
    RightBrace,
    Comma,
    Dot,
    Minus,
    Plus,
    Colon,
    Semicolon,
    Slash,
    Star,

    #endregion

    #region One or two character tokens

    Bang,
    BangEqual,
    Equal,
    EqualEqual,
    Greater,
    GreaterEqual,
    Less,
    LessEqual,

    #endregion

    #region Literals

    Identifier,
    String,
    Number,

    #endregion

    #region Keywords

    And,
    Class,
    Else,
    Function,
    For,
    If,
    Nil,
    Or,
    Print,
    Return,
    Super,
    Self,
    True,
    False,
    Let,
    Mutable,
    While,

    #endregion

    #region Misc

    EOF

    #endregion
}
