using JLox.src.Expr;

namespace JLox.src.Stmt;

internal abstract record Statement
{
    public abstract object? Execute(Interpreter ip);
};

internal sealed record AssignmentStatement(Token Name, Expression Value) : Statement
{
    public override object? Execute(Interpreter ip)
    {
        throw new NotImplementedException();
    }
}

internal sealed record BlockStatement(IEnumerable<Statement> Statements) : Statement
{
    public override object? Execute(Interpreter ip)
    {
        ip.ExecuteBlock(Statements, new(ip.Environment));
        return null;
    }
}

internal sealed record ExpressionStatement(Expression Expression) : Statement
{
    public override object? Execute(Interpreter ip) => ip.Evaluate(Expression);
}

internal sealed record IfStatement(Expression Condition, Statement ThenBranch, Statement? ElseBranch) : Statement
{
    public override object? Execute(Interpreter ip)
    {
        if (Interpreter.IsTruthy(ip.Evaluate(Condition)))
        {
            ip.Execute(ThenBranch);
        }
        else if (ElseBranch != null)
        {
            ip.Execute(ElseBranch);
        }

        return null;
    }
}

internal sealed record PrintStatement(Expression Expression) : Statement
{
    public override object? Execute(Interpreter ip)
    {
        var value = ip.Evaluate(Expression);

        Console.WriteLine(Stringify.ToString(value));

        return null;
    }
}

internal sealed record LetStatement(Token Name, bool Mutable, Expression? Initializer) : Statement
{
    public override object? Execute(Interpreter ip)
    {
        object? value = null;

        if (Initializer != null)
        {
            value = ip.Evaluate(Initializer);
        }

        ip.Environment.Define(Name.Lexeme, new(Mutable, value));

        return null;
    }
}

internal sealed record UnaryStatement(Token Operator, Expression Right) : Statement
{
    public override object? Execute(Interpreter ip)
    {
        throw new NotImplementedException();
    }
}

internal sealed record VariableStatement(Token Name) : Statement
{
    public override object? Execute(Interpreter ip)
    {
        throw new NotImplementedException();
    }
}
