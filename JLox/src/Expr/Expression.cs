using JLox.src.Exceptions;
using JLox.src.Stmt;

namespace JLox.src.Expr;

internal abstract record Expression
{
    public abstract object? Visit(IExpressionVisitor visitor);
};

internal sealed record AssignmentExpression(Token Name, Expression Value) : Expression
{
    public override object? Visit(IExpressionVisitor visitor)
        => visitor.VisitAssignmentExpression(this);
}

internal sealed record BinaryExpression(Expression Left, Token Op, Expression Right) : Expression
{
    public override object? Visit(IExpressionVisitor visitor)
        => visitor.VisitBinaryExpression(this);
}

internal sealed record CallExpression(Expression Callee, Token Paren, List<Expression> Arguments) : Expression
{
    public override object? Visit(IExpressionVisitor visitor)
        => visitor.VisitCallExpression(this);
}

internal sealed record GetExpression(Expression Object, Token Name) : Expression
{
    public override object? Visit(IExpressionVisitor visitor)
        => visitor.VisitGetExpression(this);
}

internal sealed record GroupingExpression(Expression Expression) : Expression
{
    public override object? Visit(IExpressionVisitor visitor)
        => visitor.VisitGroupingExpression(this);
}

internal sealed record LiteralExpression(object? Value) : Expression
{
    public override object? Visit(IExpressionVisitor visitor)
        => visitor.VisitLiteralExpression(this);
}

internal sealed record LogicalExpression(Expression Left, Token Op, Expression Right) : Expression
{
    public override object? Visit(IExpressionVisitor visitor)
        => visitor.VisitLogicalExpression(this);
}

internal sealed record SetExpression(Expression Object, Token Name, Expression Value) : Expression
{
    public override object? Visit(IExpressionVisitor visitor)
        => visitor.VisitSetExpression(this);
}

internal sealed record SuperExpression(Token Keyword, Token Method) : Expression
{
    public override object? Visit(IExpressionVisitor visitor)
        => visitor.VisitSuperExpression(this);
}

internal sealed record SelfExpression(Token Keyword) : Expression
{
    public override object? Visit(IExpressionVisitor visitor)
        => visitor.VisitSelfExpression(this);
}

internal sealed record UnaryExpression(Token Op, Expression Right) : Expression
{
    public override object? Visit(IExpressionVisitor visitor)
        => visitor.VisitUnaryExpression(this);
}

internal sealed record VariableExpression(Token Name) : Expression
{
    public override object? Visit(IExpressionVisitor visitor)
        => visitor.VisitVariableExpression(this);

    public override int GetHashCode()
    {
        return Name.GetHashCode();
    }
}
