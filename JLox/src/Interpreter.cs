using JLox.src.Exceptions;
using JLox.src.Expr;
using JLox.src.Stmt;
using System.Data;

namespace JLox.src;

internal class Interpreter : IExpressionVisitor, IStatementVisitor
{
    public readonly LoxEnvironment GlobalEnvironment = new();
    public LoxEnvironment Environment;
    public readonly Dictionary<Expression, int> Locals = new();

    public Interpreter()
    {
        Environment = GlobalEnvironment;

        GlobalEnvironment.Define("MAX_NUMBER", new Key(true, double.MaxValue));
        GlobalEnvironment.DefineFunction("clock", new NativeFunctions.Clock());
        GlobalEnvironment.DefineFunction("print", new NativeFunctions.Print());

        DefineDebugFunction("StackLog", new NativeFunctions.DebugPrintScope());
    }

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

    public object? LookUpVariable(Token name, Expression expr)
    {
        if (Locals.TryGetValue(expr, out var distance))
        {
            return Environment.GetAt(distance, name.Lexeme);
        }

        return GlobalEnvironment.Get(name);
    }

    public object? Evaluate(Expression expr)
    {
        return expr.Visit(this);
    }

    public void Execute(Statement stmt, bool repl = false)
    {
        var result = stmt.Visit(this);

        // So ugly
        if (repl && result != null)
        {
            Console.WriteLine(Stringify.ToString(result));
        }
    }

    public void Resolve(Expression expr, int depth = 0)
    {
        Console.WriteLine($"Resolve {expr.GetHashCode()} {depth}");

        Locals[expr] = depth;
    }
    //public void Resolve(Expression expr, int depth = 0) => Locals[expr] = depth;

    public void ExecuteBlock(IEnumerable<Statement> statements, LoxEnvironment environment)
    {
        var previous = Environment;

        try
        {
            Environment = environment;

            foreach (var stmt in statements)
                Execute(stmt);

        }
        finally
        {
            Environment = previous;
        }
    }

    private void DefineDebugFunction(string name, ICallable func)
    {
        GlobalEnvironment.DefineFunction($"${name}", func);
    }

    #region Statements

    public object? VisitBlockStatement(BlockStatement stmt)
    {
        ExecuteBlock(stmt.Statements, new(Environment));
        return null;
    }

    public object? VisitExpressionStatement(ExpressionStatement stmt)
        => Evaluate(stmt.Expression);

    public object? VisitFunctionStatement(FunctionStatement stmt)
    {
        var func = new LoxFunction(stmt, Environment);

        Environment.Define(stmt.Name.Lexeme, new(true, func));

        return null;
    }

    public object? VisitIfStatement(IfStatement stmt)
    {
        if (IsTruthy(Evaluate(stmt.Condition)))
        {
            Execute(stmt.ThenBranch);
        }
        else if (stmt.ElseBranch != null)
        {
            Execute(stmt.ElseBranch);
        }

        return null;
    }

    public object? VisitReturnStatement(ReturnStatement stmt)
    {
        object? value = stmt.Value is not null
            ? Evaluate(stmt.Value)
            : null;

        throw new ReturnException(value);
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

    public object? VisitWhileStatement(WhileStatement stmt)
    {
        while (IsTruthy(Evaluate(stmt.Condition)))
        {
            Execute(stmt.Body);
        }

        return null;
    }

    #endregion

    #region Expressions

    public object? VisitAssignmentExpression(AssignmentExpression expr)
    {
        var value = Evaluate(expr.Value);

        if (Locals.TryGetValue(expr, out var distance))
            Environment.AssignAt(distance, expr.Name, value);
        else
            GlobalEnvironment.Assign(expr.Name, value);

        return value;
    }

    public object? VisitBinaryExpression(BinaryExpression expr)
    {
        var left = Evaluate(expr.Left);
        var right = Evaluate(expr.Right);

        // Check first on the ones that don't need a number/double

        switch (expr.Op.Type)
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
                    EnsureNumberOperands(expr.Op, left, right);

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
        }

        // I think this looks cleaner to only check/cast once. Cause we know the Op requires a double.
        EnsureNumberOperands(expr.Op, left, right);
        double dleft = (double)left!, dright = (double)right!;

        switch (expr.Op.Type)
        {
            case TokenType.Minus: return dleft - dright;
            case TokenType.Slash:
                {
                    if (dleft == 0.0 || dright == 0.0)
                    {
                        throw new RuntimeException(expr.Op, "Division by zero");
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

    public object? VisitCallExpression(CallExpression expr)
    {
        var callee = Evaluate(expr.Callee);

        var arguments = expr.Arguments.Select(Evaluate).ToList();

        var function = callee as ICallable
            ?? throw new RuntimeException(expr.Paren, "Can only call functions and classes");

        if (arguments.Count != function.Arity)
            throw new RuntimeException(expr.Paren, $"Expected {function.Arity} arguments but got {arguments.Count}");

        return function.Call(this, arguments);
    }

    public object? VisitGetExpression(GetExpression expr)
    {
        throw new NotImplementedException();
    }

    public object? VisitGroupingExpression(GroupingExpression expr)

        => Evaluate(expr.Expression);



    public object? VisitLiteralExpression(LiteralExpression expr)
        => expr.Value;

    public object? VisitLogicalExpression(LogicalExpression expr)
    {
        var left = Evaluate(expr.Left);

        return expr.Op.Type switch
        {
            TokenType.Or => IsTruthy(left) ? left : Evaluate(expr.Right),
            TokenType.And => !IsTruthy(left) ? left : Evaluate(expr.Right),
            _ => null
        };
    }

    public object? VisitSetExpression(SetExpression expr)
    {
        throw new NotImplementedException();
    }

    public object? VisitSuperExpression(SuperExpression expr)
    {
        throw new NotImplementedException();
    }

    public object? VisitSelfExpression(SelfExpression expr)
    {
        throw new NotImplementedException();
    }

    public object? VisitUnaryExpression(UnaryExpression expr)
    {
        var right = Evaluate(expr.Right);

        switch (expr.Op.Type)
        {
            case TokenType.Bang: return !IsTruthy(right);
            case TokenType.Minus:
                {
                    Interpreter.EnsureNumberOperand(expr.Op, right);

                    return -(double)right!;
                }
            default: return null;
        }
    }

    public object? VisitVariableExpression(VariableExpression expr)
        => LookUpVariable(expr.Name, expr);

    #endregion
}
