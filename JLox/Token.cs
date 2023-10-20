namespace JLox;

internal record Token(TokenType Type, string Lexeme = "", object? Literal = null, int Line = 0);
