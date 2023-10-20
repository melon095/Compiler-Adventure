namespace JLox.src.Expr;

internal abstract record Expression
{
    public abstract T Accept<T>(IVisitor<T> visitor);
};

internal sealed record AssignExpression(Token Name, Expression Value) : Expression
{
    public override T Accept<T>(IVisitor<T> visitor)
        => visitor.VisitAssignExpr(this);
}

internal sealed record BinaryExpression(Expression Left, Token Op, Expression Right) : Expression
{
    public override T Accept<T>(IVisitor<T> visitor)
        => visitor.VisitBinaryExpr(this);
}

internal sealed record CallExpression(Expression Callee, Token Paren, List<Expression> Arguments) : Expression
{
    public override T Accept<T>(IVisitor<T> visitor)
        => visitor.VisitCallExpr(this);
}

internal sealed record GetExpression(Expression Object, Token Name) : Expression
{
    public override T Accept<T>(IVisitor<T> visitor)
        => visitor.VisitGetExpr(this);
}

internal sealed record GroupingExpression(Expression Expression) : Expression
{
    public override T Accept<T>(IVisitor<T> visitor)
        => visitor.VisitGroupingExpr(this);
}

// TODO: This should not allow nullable
internal sealed record LiteralExpression(object? Value) : Expression
{
    public override T Accept<T>(IVisitor<T> visitor)
        => visitor.VisitLiteralExpr(this);
}

internal sealed record LogicalExpression(Expression Left, Token Op, Expression Right) : Expression
{
    public override T Accept<T>(IVisitor<T> visitor)
        => visitor.VisitLogicalExpr(this);
}

internal sealed record SetExpression(Expression Object, Token Name, Expression Value) : Expression
{
    public override T Accept<T>(IVisitor<T> visitor)
        => visitor.VisitSetExpr(this);
}

internal sealed record SuperExpression(Token Keyword, Token Method) : Expression
{
    public override T Accept<T>(IVisitor<T> visitor)
        => visitor.VisitSuperExpr(this);
}

internal sealed record SelfExpression(Token Keyword) : Expression
{
    public override T Accept<T>(IVisitor<T> visitor)
        => visitor.VisitSelfExpr(this);
}

internal sealed record UnaryExpression(Token Op, Expression Right) : Expression
{
    public override T Accept<T>(IVisitor<T> visitor)
        => visitor.VisitUnaryExpr(this);
}

internal sealed record VariableExpression(Token Name) : Expression
{
    public override T Accept<T>(IVisitor<T> visitor)
        => visitor.VisitVariableExpr(this);
}
