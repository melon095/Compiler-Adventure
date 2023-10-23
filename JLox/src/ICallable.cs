namespace JLox.src;

internal interface ICallable
{
    int Arity { get; }

    object? Call(Interpreter ip, List<object?> args);
}
