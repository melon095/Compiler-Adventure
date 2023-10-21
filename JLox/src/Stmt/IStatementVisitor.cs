namespace JLox.src.Stmt;

internal interface IStatementVisitor<T>
{
    T VisitAssignmentStatement(AssignmentStatement stmt);
    T VisitBlockStatement(BlockStatement stmt);
    T VisitExpressionStatement(ExpressionStatement stmt);
    T VisitPrintStatement(PrintStatement stmt);
    T VisitLetStatement(LetStatement stmt);
    T VisitUnaryStatement(UnaryStatement stmt);
    T VisitVariableStatement(VariableStatement stmt);
    //T VisitIfStatement(IfStatement stmt);
    //T VisitWhileStatement(WhileStatement stmt);
    //T VisitFunctionStatement(FunctionStatement stmt);
    //T VisitReturnStatement(ReturnStatement stmt);
    //T VisitClassStatement(ClassStatement stmt);
}
