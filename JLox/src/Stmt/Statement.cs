using JLox.src.Expr;

namespace JLox.src.Stmt;

internal abstract record Statement
{
    public abstract object? Visit(IStatementVisitor visitor);
};

internal sealed record BlockStatement(IEnumerable<Statement> Statements) : Statement
{
    public override object? Visit(IStatementVisitor visitor)
        => visitor.VisitBlockStatement(this);
}

internal sealed record ExpressionStatement(Expression Expression) : Statement
{
    public override object? Visit(IStatementVisitor visitor)
        => visitor.VisitExpressionStatement(this);
}

internal sealed record FunctionStatement(Token Name, List<Token> Params, List<Statement> Body) : Statement
{
    public override object? Visit(IStatementVisitor visitor)
        => visitor.VisitFunctionStatement(this);
}

internal sealed record IfStatement(Expression Condition, Statement ThenBranch, Statement? ElseBranch) : Statement
{
    public override object? Visit(IStatementVisitor visitor)
        => visitor.VisitIfStatement(this);
}

internal sealed record ReturnStatement(Token Keyword, Expression? Value) : Statement
{
    public override object? Visit(IStatementVisitor visitor)
        => visitor.VisitReturnStatement(this);
}

internal sealed record LetStatement(Token Name, bool Mutable, Expression? Initializer) : Statement
{
    public override object? Visit(IStatementVisitor visitor)
        => visitor.VisitLetStatement(this);
}

internal sealed record WhileStatement(Expression Condition, Statement Body) : Statement
{
    public override object? Visit(IStatementVisitor visitor)
        => visitor.VisitWhileStatement(this);
}
