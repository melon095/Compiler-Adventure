namespace JLox.src.Exceptions;

internal class RuntimeException : Exception
{
    public readonly Token Token;
    public RuntimeException(Token t, string message)
        : base(message)
    {
        Token = t;
    }
}
