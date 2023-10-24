using JLox.src.Exceptions;

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

        // Check first on the ones that don't need a number/double

        switch (Op.Type)
        {
            case TokenType.BangEqual:
                {
                    if (left is null && right is null)
                    {
                        return false;
                    }

                    return !left?.Equals(right) ?? false;
                }

            case TokenType.EqualEqual:
                {
                    // A bit ugly, but you nil is a valid value in Lox, so we need to check for nulls
                    if (left is null && right is null)
                    {
                        return true;
                    }

                    return left?.Equals(right) ?? false;
                }

            case TokenType.Plus:
                {
                    Interpreter.EnsureNumberOperands(Op, left, right);

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
        }

        // I think this looks cleaner to only check/cast once. Cause we know the Op requires a double.
        Interpreter.EnsureNumberOperands(Op, left, right);
        double dleft = (double)left!, dright = (double)right!;

        switch (Op.Type)
        {
            case TokenType.Minus: return dleft - dright;
            case TokenType.Slash:
                {
                    if (dleft == 0.0 || dright == 0.0)
                    {
                        throw new RuntimeException(Op, "Division by zero");
                    }

                    return dleft / dright;
                }

            case TokenType.Star: return dleft * dright;
            case TokenType.Greater: return dleft > dright;
            case TokenType.GreaterEqual: return dleft >= dright;
            case TokenType.Less: return dleft < dright;
            case TokenType.LessEqual: return (double)left! <= (double)right!;

            default: return null;
        }
    }
}

internal sealed record CallExpression(Expression Callee, Token Paren, List<Expression> Arguments) : Expression
{
    public override object? Execute(Interpreter ip)
    {
        var callee = ip.Evaluate(Callee);

        var arguments = Arguments.Select(ip.Evaluate).ToList();

        var function = callee as ICallable
            ?? throw new RuntimeException(Paren, "Can only call functions and classes");

        if (arguments.Count != function.Arity)
            throw new RuntimeException(Paren, $"Expected {function.Arity} arguments but got {arguments.Count}");

        return function.Call(ip, arguments);
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
