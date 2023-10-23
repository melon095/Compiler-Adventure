namespace JLox.src.Exceptions;

internal class ReturnException : RuntimeException
{
    public readonly object? Value = null;

    public ReturnException(object? value)
        // TODO: Hella ugly but whatever
        : base(null!, string.Empty)
    {
        Value = value;
    }
}
