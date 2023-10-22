using JLox.src.Exceptions;
using JLox.src.Expr;
using JLox.src.Stmt;

namespace JLox.src;

internal class Interpreter
{
    public LoxEnvironment Environment = new();

    public void Interpret(IEnumerable<Statement> statements, bool repl = false)
    {
        try
        {
            foreach (var stmt in statements)
            {
                Execute(stmt, repl);
            }
        }
        catch (RuntimeException ex)
        {
            Program.RuntimeError(ex);
        }
    }

    // TODO: Move this to a helper class
    public static void EnsureNumberOperand(Token op, object? operand)
    {
        if (operand is double)
        {
            return;
        }

        throw new RuntimeException(op, "Operand must be a number");
    }

    // TODO: Move this to a helper class
    public static void EnsureNumberOperands(Token op, object? left, object? right)
    {
        if (left is double && right is double)
        {
            return;
        }

        throw new RuntimeException(op, "Operands must be numbers");
    }

    // TODO: Move this to a helper class
    public static bool IsTruthy(object? obj)
    {
        if (obj == null)
        {
            return false;
        }

        if (obj is bool b)
        {
            return b;
        }

        return true;
    }

    public object? Evaluate(Expression expr)
    {
        return expr.Execute(this);
    }

    public void Execute(Statement stmt, bool repl = false)
    {
        var result = stmt.Execute(this);

        // So ugly
        if (repl && result != null)
        {
            Console.WriteLine(Stringify.ToString(result));
        }
    }

    public void ExecuteBlock(IEnumerable<Statement> statements, LoxEnvironment environment)
    {
        var previous = Environment;

        try
        {
            Environment = environment;

            foreach (var stmt in statements)
            {
                Execute(stmt);
            }
        }
        finally
        {
            Environment = previous;
        }
    }
}
