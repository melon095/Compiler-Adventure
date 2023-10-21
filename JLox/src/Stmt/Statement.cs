using JLox.src.Expr;

namespace JLox.src.Stmt;

internal abstract record Statement
{
    public abstract T Accept<T>(IStatementVisitor<T> visitor);
};

internal sealed record AssignmentStatement(Token Name, Expression Value) : Statement
{
    public override T Accept<T>(IStatementVisitor<T> visitor)
        => visitor.VisitAssignmentStatement(this);
}

internal sealed record BlockStatement(IEnumerable<Statement> Statements) : Statement
{
    public override T Accept<T>(IStatementVisitor<T> visitor)
        => visitor.VisitBlockStatement(this);
}

internal sealed record ExpressionStatement(Expression Expression) : Statement
{
    public override T Accept<T>(IStatementVisitor<T> visitor)
        => visitor.VisitExpressionStatement(this);
}
internal sealed record PrintStatement(Expression Expression) : Statement
{
    public override T Accept<T>(IStatementVisitor<T> visitor)
        => visitor.VisitPrintStatement(this);
}

internal sealed record LetStatement(Token Name, bool Mutable, Expression? Initializer) : Statement
{
    public override T Accept<T>(IStatementVisitor<T> visitor)
        => visitor.VisitLetStatement(this);
}

internal sealed record UnaryStatement(Token Operator, Expression Right) : Statement
{
    public override T Accept<T>(IStatementVisitor<T> visitor)
        => visitor.VisitUnaryStatement(this);
}

internal sealed record VariableStatement(Token Name) : Statement
{
    public override T Accept<T>(IStatementVisitor<T> visitor)
        => visitor.VisitVariableStatement(this);
}
