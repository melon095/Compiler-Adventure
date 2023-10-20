namespace JLox.src.Expr;

internal interface IVisitor<T>
{
    T VisitAssignExpr(AssignExpression expr);
    T VisitBinaryExpr(BinaryExpression expr);
    T VisitCallExpr(CallExpression expr);
    T VisitGetExpr(GetExpression expr);
    T VisitGroupingExpr(GroupingExpression expr);
    T VisitLiteralExpr(LiteralExpression expr);
    T VisitLogicalExpr(LogicalExpression expr);
    T VisitSetExpr(SetExpression expr);
    T VisitSuperExpr(SuperExpression expr);
    T VisitSelfExpr(SelfExpression expr);
    T VisitUnaryExpr(UnaryExpression expr);
    T VisitVariableExpr(VariableExpression expr);
}
