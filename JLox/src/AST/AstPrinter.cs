using JLox.src.Expr;
using System.Text;

namespace JLox.src.AST;

internal class AstPrinter : Interpreter
{
    private string Parenthesize(string name, params Expression[] exprs)
    {
        var builder = new StringBuilder();

        builder.Append('(').Append(name);
        foreach (var expr in exprs)
        {
            builder.Append(' ');
            builder.Append(expr.Execute(this));
        }
        builder.Append(')');

        return builder.ToString();
    }

    public string Print(Expression expr)
    {
        return Stringify.ToString(expr.Execute(this));
    }

    public string VisitAssignmentExpr(AssignmentExpression expr)
    {
        throw new NotImplementedException();
    }

    public string VisitBinaryExpr(BinaryExpression expr)
        => Parenthesize(expr.Op.Lexeme, expr.Left, expr.Right);

    public string VisitCallExpr(CallExpression expr)
    {
        throw new NotImplementedException();
    }

    public string VisitGetExpr(GetExpression expr)
    {
        throw new NotImplementedException();
    }

    public string VisitGroupingExpr(GroupingExpression expr)
        => Parenthesize("group", expr.Expression);

    public string VisitLiteralExpr(LiteralExpression expr)
        => expr.Value?.ToString() ?? "nil";

    public string VisitLogicalExpr(LogicalExpression expr)
    {
        throw new NotImplementedException();
    }

    public string VisitSelfExpr(SelfExpression expr)
    {
        throw new NotImplementedException();
    }

    public string VisitSetExpr(SetExpression expr)
    {
        throw new NotImplementedException();
    }

    public string VisitSuperExpr(SuperExpression expr)
    {
        throw new NotImplementedException();
    }

    public string VisitUnaryExpr(UnaryExpression expr)
        => Parenthesize(expr.Op.Lexeme, expr.Right);

    public string VisitVariableExpr(VariableExpression expr)
    {
        throw new NotImplementedException();
    }
}
