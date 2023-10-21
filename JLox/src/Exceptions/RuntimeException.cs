namespace JLox.src.Exceptions;

// TODO: Maybe remove the Exception part considering it's a Lox specific exception
internal sealed class RuntimeException : Exception
{
    public readonly Token Token;
    public RuntimeException(Token t, string message)
        : base(message)
    {
        Token = t;
    }
}
