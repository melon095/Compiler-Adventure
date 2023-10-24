namespace JLox.src.Expr;

internal interface IExpressionVisitor
{
    object? VisitAssignmentExpression(AssignmentExpression expr);
    object? VisitBinaryExpression(BinaryExpression expr);
    object? VisitCallExpression(CallExpression expr);
    object? VisitGetExpression(GetExpression expr);
    object? VisitGroupingExpression(GroupingExpression expr);
    object? VisitLiteralExpression(LiteralExpression expr);
    object? VisitLogicalExpression(LogicalExpression expr);
    object? VisitSetExpression(SetExpression expr);
    object? VisitSuperExpression(SuperExpression expr);
    object? VisitSelfExpression(SelfExpression expr);
    object? VisitUnaryExpression(UnaryExpression expr);
    object? VisitVariableExpression(VariableExpression expr);
}
