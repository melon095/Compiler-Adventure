using JLox.src.Exceptions;
using JLox.src.Stmt;

namespace JLox.src;

internal class LoxFunction : ICallable
{
    private readonly FunctionStatement Declaration;
    private readonly LoxEnvironment Closure;

    public LoxFunction(FunctionStatement fs, LoxEnvironment closure)
    {
        Declaration = fs;
        Closure = closure;

    }

    public int Arity => Declaration.Params.Count;

    public object? Call(Interpreter ip, List<object?> args)
    {
        var localStack = new LoxEnvironment(Closure);

        for (var idx = 0; idx < Arity; idx++)
            localStack.Define(Declaration.Params[idx].Lexeme, new(false, args[idx]));

        // Pretty sure most dynamic languages allow arguments to be mutated, so im fine with doing it here too.

        try
        {
            ip.ExecuteBlock(Declaration.Body, localStack);
        }
        catch (ReturnException ex)
        {
            return ex.Value;
        }

        return null;
    }

    public override string ToString() => $"<function>";
}
