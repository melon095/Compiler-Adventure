using JLox.src.Exceptions;
using JLox.src.Expr;
using JLox.src.Stmt;

namespace JLox.src;

internal class Interpreter : IExpressionVisitor<object?>, IStatementVisitor<object?>
{
    private LoxEnvironment Environment = new();

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

    private static void EnsureNumberOperand(Token op, object? operand)
    {
        if (operand is double)
        {
            return;
        }

        throw new RuntimeException(op, "Operand must be a number");
    }

    private static void EnsureNumberOperands(Token op, object? left, object? right)
    {
        if (left is double && right is double)
        {
            return;
        }

        throw new RuntimeException(op, "Operands must be numbers");
    }

    private static bool IsTruthy(object? obj)
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

    private object? Evaluate(Expression expr)
    {
        return expr.Accept(this);
    }

    private void Execute(Statement stmt, bool repl = false)
    {
        var result = stmt.Accept(this);

        // So ugly
        if (repl && result != null)
        {
            Console.WriteLine(Stringify.ToString(result));
        }
    }

    private void ExecuteBlock(IEnumerable<Statement> statements, LoxEnvironment environment)
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

    #region ExpressionVisitor

    public object? VisitAssignmentExpr(AssignmentExpression expr)
    {
        var value = Evaluate(expr.Value);

        Environment.Assign(expr.Name, value);

        return value;
    }

    public object? VisitBinaryExpr(BinaryExpression expr)
    {
        var left = Evaluate(expr.Left);
        var right = Evaluate(expr.Right);

        // TODO: This should raise an exception
        if (left == null || right == null)
        {
            return null;
        }

        switch (expr.Op.Type)
        {
            case TokenType.Minus:
                {
                    EnsureNumberOperands(expr.Op, left, right);

                    return (double)left - (double)right;
                }

            case TokenType.Slash:
                {
                    EnsureNumberOperands(expr.Op, left, right);

                    var l = (double)left;
                    var r = (double)right;

                    if (l == 0.0 || r == 0.0)
                    {
                        throw new RuntimeException(expr.Op, "Division by zero");
                    }

                    return (double)left / (double)right;
                }

            case TokenType.Star:
                {
                    EnsureNumberOperands(expr.Op, left, right);

                    return (double)left * (double)right;
                }

            case TokenType.Greater:
                {
                    EnsureNumberOperands(expr.Op, left, right);

                    return (double)left > (double)right;
                }

            case TokenType.GreaterEqual:
                {
                    EnsureNumberOperands(expr.Op, left, right);

                    return (double)left >= (double)right;
                }

            case TokenType.Less:
                {
                    EnsureNumberOperands(expr.Op, left, right);

                    return (double)left < (double)right;
                }

            case TokenType.LessEqual:
                {
                    EnsureNumberOperands(expr.Op, left, right);

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

                    throw new RuntimeException(expr.Op, "Unable to concatenate operands");
                }
            default: return null;
        }
    }

    public object? VisitCallExpr(CallExpression expr)
    {
        throw new NotImplementedException();
    }

    public object? VisitGetExpr(GetExpression expr)
    {
        throw new NotImplementedException();
    }

    public object? VisitGroupingExpr(GroupingExpression expr)
    {
        return Evaluate(expr.Expression);
    }

    public object? VisitLiteralExpr(LiteralExpression expr)
    {
        return expr.Value;
    }

    public object? VisitLogicalExpr(LogicalExpression expr)
    {
        throw new NotImplementedException();
    }

    public object? VisitSelfExpr(SelfExpression expr)
    {
        throw new NotImplementedException();
    }

    public object? VisitSetExpr(SetExpression expr)
    {
        throw new NotImplementedException();
    }

    public object? VisitSuperExpr(SuperExpression expr)
    {
        throw new NotImplementedException();
    }

    public object? VisitUnaryExpr(UnaryExpression expr)
    {
        var right = Evaluate(expr.Right);

        switch (expr.Op.Type)
        {
            case TokenType.Bang: return !IsTruthy(right);
            case TokenType.Minus:
                {
                    EnsureNumberOperand(expr.Op, right);

                    return -(double)right!;
                }
            default: return null;
        }
    }

    public object? VisitVariableExpr(VariableExpression expr)
          => Environment.Get(expr.Name);

    public object? VisitExpressionStatement(ExpressionStatement stmt)
    {
        return Evaluate(stmt.Expression);
    }

    #endregion

    #region StatementVisitor

    public object? VisitPrintStatement(PrintStatement stmt)
    {
        var value = Evaluate(stmt.Expression);

        Console.WriteLine(Stringify.ToString(value));

        return null;
    }

    public object? VisitBlockStatement(BlockStatement stmt)
    {
        ExecuteBlock(stmt.Statements, new(Environment));
        return null;
    }

    public object? VisitLetStatement(LetStatement stmt)
    {
        object? value = null;

        if (stmt.Initializer != null)
        {
            value = Evaluate(stmt.Initializer);
        }

        Environment.Define(stmt.Name.Lexeme, new(stmt.Mutable, value));

        return null;
    }

    public object? VisitUnaryStatement(UnaryStatement stmt)
    {
        throw new NotImplementedException();
    }

    public object? VisitVariableStatement(VariableStatement stmt)
    {
        throw new NotImplementedException();
    }

    public object? VisitAssignmentStatement(AssignmentStatement stmt)
    {
        throw new NotImplementedException();
    }

    #endregion
}
