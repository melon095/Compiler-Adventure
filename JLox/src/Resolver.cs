using JLox.src.Expr;
using JLox.src.Stmt;

namespace JLox.src;

internal enum FunctionType
{
    None,
    Function
};

internal class Resolver : IExpressionVisitor, IStatementVisitor
{
    private readonly Interpreter Interpreter;
    private readonly Stack<Dictionary<string, bool>> Scopes = new();
    private FunctionType CurrentFunction = FunctionType.None;

    public Resolver(Interpreter interpreter)
    {
        Interpreter = interpreter;
    }

    public void Resolve(Statement s)
        => s.Visit(this);

    public void Resolve(Expression x)
        => x.Visit(this);

    public void Resolve(IEnumerable<Statement> statements)
    {
        foreach (var s in statements) Resolve(s);
    }

    private void ResolveLocal(Expression expr, Token name)
    {
        /*
        
            Fuck stacks in Java.

            Since it extends Vector new elements are pushed to the back. Whereas in C# it's properly implemented and the new elements are pushed to the front.

            So to get the correct scope we need to iterate through the stack from the back to the front.

            // This has been taken from https://github.com/andrewtyped/cslox/blob/master/src/lox/Resolver.cs#L459
            // Primarily cause i really can't be asked to fix this myself...
        */

        var tempScopes = new Stack<Dictionary<string, bool>>();

        try
        {
            var origScopeSize = Scopes.Count;

            for (int i = origScopeSize - 1; i >= 0; i--)
            {
                var tempScope = Scopes.Pop();

                tempScopes.Push(tempScope);

                if (tempScope.ContainsKey(name.Lexeme))
                {
                    Interpreter.Resolve(expr, origScopeSize - 1 - i);
                    return;
                }
            }
        }
        finally
        {
            while (tempScopes.TryPop(out var scope))
                Scopes.Push(scope);
        }


        //for (int i = 0; i < Scopes.Count; i++)
        //{
        //    if (Scopes.ElementAt(i).ContainsKey(name.Lexeme))
        //    {
        //        Interpreter.Resolve(expr, i);
        //        return;
        //    }
        //}
    }

    private void ResolveFunction(FunctionStatement function, FunctionType functionType)
    {
        var enclosingFunction = CurrentFunction;
        CurrentFunction = functionType;

        try
        {
            BeginScope();

            foreach (var param in function.Params)
            {
                Declare(param);
                Define(param);
            }

            Resolve(function.Body);
        }
        finally
        {
            EndScope();
            CurrentFunction = enclosingFunction;
        };
    }

    private void BeginScope() => Scopes.Push(new());
    private void EndScope() => Scopes.Pop();

    private void Declare(Token name)
    {
        if (Scopes.Count == 0)
            return;

        var scope = Scopes.Peek();

        if (scope.ContainsKey(name.Lexeme))
            Program.Error(name, "Variable with this name already declared in this scope.");

        _ = scope[name.Lexeme] = false;
    }

    private void Define(Token name)
    {
        if (Scopes.Count == 0)
            return;

        _ = Scopes.Peek()[name.Lexeme] = true;
    }

    #region Statement

    public object? VisitBlockStatement(BlockStatement stmt)
    {
        try
        {
            BeginScope();
            Resolve(stmt.Statements);
        }
        finally { EndScope(); }

        return null;
    }

    public object? VisitExpressionStatement(ExpressionStatement stmt)
    {
        Resolve(stmt.Expression);

        return null;
    }

    public object? VisitFunctionStatement(FunctionStatement stmt)
    {
        Declare(stmt.Name);
        Define(stmt.Name);

        ResolveFunction(stmt, FunctionType.Function);

        return null;
    }

    public object? VisitIfStatement(IfStatement stmt)
    {
        Resolve(stmt.Condition);
        Resolve(stmt.ThenBranch);

        if (stmt.ElseBranch is not null)
            Resolve(stmt.ElseBranch);

        return null;
    }

    public object? VisitLetStatement(LetStatement stmt)
    {
        Declare(stmt.Name);

        if (stmt.Initializer is not null)
            Resolve(stmt.Initializer);

        Define(stmt.Name);

        return null;
    }

    public object? VisitReturnStatement(ReturnStatement stmt)
    {
        if (CurrentFunction == FunctionType.None)
            Program.Error(stmt.Keyword, "Can't return from top-level code.");

        if (stmt.Value is not null)
            Resolve(stmt.Value);

        return null;
    }

    public object? VisitWhileStatement(WhileStatement stmt)
    {
        Resolve(stmt.Condition);
        Resolve(stmt.Body);

        return null;
    }

    #endregion

    #region Expression

    public object? VisitAssignmentExpression(AssignmentExpression expr)
    {
        Resolve(expr.Value);
        ResolveLocal(expr, expr.Name);

        return null;
    }

    public object? VisitBinaryExpression(BinaryExpression expr)
    {
        Resolve(expr.Left);
        Resolve(expr.Right);

        return null;
    }

    public object? VisitCallExpression(CallExpression expr)
    {
        Resolve(expr.Callee);

        expr.Arguments.ForEach(Resolve);

        return null;
    }

    public object? VisitGetExpression(GetExpression expr)
    {
        throw new NotImplementedException();
    }

    public object? VisitGroupingExpression(GroupingExpression expr)
    {
        Resolve(expr.Expression);

        return null;
    }

    public object? VisitLiteralExpression(LiteralExpression expr) => null;

    public object? VisitLogicalExpression(LogicalExpression expr)
    {
        Resolve(expr.Left);
        Resolve(expr.Right);

        return null;
    }

    public object? VisitSelfExpression(SelfExpression expr)
    {
        throw new NotImplementedException();
    }

    public object? VisitSetExpression(SetExpression expr)
    {
        throw new NotImplementedException();
    }

    public object? VisitSuperExpression(SuperExpression expr)
    {
        throw new NotImplementedException();
    }

    public object? VisitUnaryExpression(UnaryExpression expr)
    {
        Resolve(expr.Right);

        return null;
    }

    public object? VisitVariableExpression(VariableExpression expr)
    {
        if (Scopes.TryPeek(out var scope) && scope.TryGetValue(expr.Name.Lexeme, out var b) && b == false)
        {
            Program.Error(expr.Name, "Can't read local variable in it's own initalizer");
        }

        ResolveLocal(expr, expr.Name);

        return null;
    }

    #endregion
}
