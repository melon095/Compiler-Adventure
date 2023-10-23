using JLox.src.Exceptions;
using System.Collections;
using System.Diagnostics.CodeAnalysis;

namespace JLox.src.Expr;

internal abstract record Expression
{
    public abstract object? Execute(Interpreter ip);
};

internal sealed record AssignmentExpression(Token Name, Expression Value) : Expression
{
    public override object? Execute(Interpreter ip)
    {
        var value = ip.Evaluate(Value);

        ip.Environment.Assign(Name, value);

        return value;
    }
}

internal sealed record BinaryExpression(Expression Left, Token Op, Expression Right) : Expression
{
    public override object? Execute(Interpreter ip)
    {
        var left = ip.Evaluate(Left);
        var right = ip.Evaluate(Right);

        // TODO: This should raise an exception
        if (left == null || right == null)
        {
            return null;
        }

        switch (Op.Type)
        {
            case TokenType.Minus:
                {
                    Interpreter.EnsureNumberOperands(Op, left, right);

                    return (double)left - (double)right;
                }

            case TokenType.Slash:
                {
                    Interpreter.EnsureNumberOperands(Op, left, right);

                    var l = (double)left;
                    var r = (double)right;

                    if (l == 0.0 || r == 0.0)
                    {
                        throw new RuntimeException(Op, "Division by zero");
                    }

                    return (double)left / (double)right;
                }

            case TokenType.Star:
                {
                    Interpreter.EnsureNumberOperands(Op, left, right);

                    return (double)left * (double)right;
                }

            case TokenType.Greater:
                {
                    Interpreter.EnsureNumberOperands(Op, left, right);

                    return (double)left > (double)right;
                }

            case TokenType.GreaterEqual:
                {
                    Interpreter.EnsureNumberOperands(Op, left, right);

                    return (double)left >= (double)right;
                }

            case TokenType.Less:
                {
                    Interpreter.EnsureNumberOperands(Op, left, right);

                    return (double)left < (double)right;
                }

            case TokenType.LessEqual:
                {
                    Interpreter.EnsureNumberOperands(Op, left, right);

                    return (double)left <= (double)right;
                }

            case TokenType.BangEqual: return !left?.Equals(right) ?? false;
            case TokenType.EqualEqual: return left?.Equals(right) ?? false;

            case TokenType.Plus:
                {

                    if (left is double l && right is double r)
                    {
                        return l + r;
                    }

                    if (left is string && right is string)
                    {
                        return string.Concat(left, right);
                    }

                    throw new RuntimeException(Op, "Unable to concatenate operands");
                }
            default: return null;
        }
    }
}

internal sealed record CallExpression(Expression Callee, Token Paren, List<Expression> Arguments) : Expression
{
    public override object? Execute(Interpreter ip)
    {
        throw new NotImplementedException();
    }
}

internal sealed record GetExpression(Expression Object, Token Name) : Expression
{
    public override object? Execute(Interpreter ip)
    {
        throw new NotImplementedException();
    }
}

internal sealed record GroupingExpression(Expression Expression) : Expression
{
    public override object? Execute(Interpreter ip)
    {
        return ip.Evaluate(Expression);
    }
}

// TODO: This should not allow nullable
internal sealed record LiteralExpression(object? Value) : Expression
{
    public override object? Execute(Interpreter ip) => Value;
}

internal sealed record LogicalExpression(Expression Left, Token Op, Expression Right) : Expression
{
    public override object? Execute(Interpreter ip)
    {
        var left = ip.Evaluate(Left);

        return Op.Type switch
        {
            TokenType.Or => Interpreter.IsTruthy(left) ? left : ip.Evaluate(Right),
            TokenType.And => !Interpreter.IsTruthy(left) ? left : ip.Evaluate(Right),
            _ => null
        };
    }
}

internal sealed record SetExpression(Expression Object, Token Name, Expression Value) : Expression
{
    public override object? Execute(Interpreter ip)
    {
        throw new NotImplementedException();
    }
}

internal sealed record SuperExpression(Token Keyword, Token Method) : Expression
{
    public override object? Execute(Interpreter ip)
    {
        throw new NotImplementedException();
    }
}

internal sealed record SelfExpression(Token Keyword) : Expression
{
    public override object? Execute(Interpreter ip)
    {
        throw new NotImplementedException();
    }
}

internal sealed record UnaryExpression(Token Op, Expression Right) : Expression
{
    public override object? Execute(Interpreter ip)
    {
        var right = ip.Evaluate(Right);

        switch (Op.Type)
        {
            case TokenType.Bang: return !Interpreter.IsTruthy(right);
            case TokenType.Minus:
                {
                    Interpreter.EnsureNumberOperand(Op, right);

                    return -(double)right!;
                }
            default: return null;
        }
    }
}

internal sealed record VariableExpression(Token Name) : Expression
{
    public override object? Execute(Interpreter ip)
        => ip.Environment.Get(Name);
}
