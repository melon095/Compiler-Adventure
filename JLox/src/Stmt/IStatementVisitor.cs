namespace JLox.src.Stmt;

internal interface IStatementVisitor
{
    object? VisitBlockStatement(BlockStatement stmt);
    //object? VisitClassStatement(ClassStatement stmt);
    object? VisitExpressionStatement(ExpressionStatement stmt);
    object? VisitFunctionStatement(FunctionStatement stmt);
    object? VisitIfStatement(IfStatement stmt);
    object? VisitReturnStatement(ReturnStatement stmt);
    object? VisitLetStatement(LetStatement stmt);
    object? VisitWhileStatement(WhileStatement stmt);
}
