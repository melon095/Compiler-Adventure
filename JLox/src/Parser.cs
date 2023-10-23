using JLox.src.Exceptions;
using JLox.src.Expr;
using JLox.src.Stmt;

namespace JLox.src;

internal class Parser
{
    private readonly static int MaxFunctionParameters = 255;

    private readonly List<Token> Tokens;
    private int Current = 0;

    public Parser(List<Token> t)
    {
        Tokens = t;
    }

    /// <summary>
    /// Creates a <see cref="ParseException">ParseException</see> and logs the error to STDOUT but does not throw.
    /// </summary>
    private static ParseException Error(Token token, string message)
    {
        Program.Error(token, message);

        return new ParseException();
    }

    public IEnumerable<Statement> Parse()
    {
        List<Statement> statements = new();

        try
        {
            while (!AtEnd)
            {
                statements.Add(Declaration());
            }
        }
        catch (ParseException)
        {
            Synchronize();
        }

        return statements;
    }

    private bool Check(TokenType type)
    {
        if (AtEnd)
        {
            return false;
        }

        return Peek().Type == type;
    }

    private Token Peek() => Tokens[Current];

    private Token Advance()
    {
        if (!AtEnd)
        {
            Current++;
        }

        return Previous;
    }

    private Token Previous => Tokens[Current - 1];

    private bool AtEnd => Peek().Type == TokenType.EOF;

    private Token Consume(TokenType type, string message = "")
    {
        if (Check(type)) return Advance();

        throw Error(Peek(), message);
    }

    private bool Match(TokenType type)
    {
        if (Check(type))
        {
            Advance();
            return true;
        }

        return false;
    }

    private bool Match(params TokenType[] types)
    {
        foreach (var typeo in types)
        {
            if (Match(typeo))
            {
                return true;
            }
        }

        return false;
    }

    private void Synchronize()
    {
        Advance();

        while (!AtEnd)
        {
            if (Previous.Type == TokenType.Semicolon) return;

            var type = Peek().Type;
            switch (type)
            {
                case TokenType.Class:
                case TokenType.Function:
                case TokenType.Let:
                case TokenType.For:
                case TokenType.If:
                case TokenType.While:
                case TokenType.Return:
                    return;
            }

            Advance();
        }
    }

    private Statement Declaration()
    {
        if (Match(TokenType.Function)) return Function("function");
        if (Match(TokenType.Let)) return LetDeclaration();

        return Statement();
    }

    private Statement Statement()
    {
        if (Match(TokenType.For)) return ForStatement();
        if (Match(TokenType.If)) return IfStatement();
        if (Match(TokenType.Return)) return ReturnStatement();
        if (Match(TokenType.While)) return WhileStatement();
        if (Match(TokenType.LeftBrace)) return new BlockStatement(BlockStatement());

        return ExpressionStatement();
    }

    private Expression Expression() => Assignment();

    private Expression Assignment()
    {
        var expr = Or();

        if (Match(TokenType.Equal))
        {
            var equals = Previous;
            var value = Assignment();

            if (expr is VariableExpression varExpr)
            {
                return new AssignmentExpression(varExpr.Name, value);
            }

            Error(equals, "Invalid assignment target.");
        }

        return expr;
    }

    private Expression Or()
    {
        var expr = And();

        while (Match(TokenType.Or))
        {
            var op = Previous;
            var right = And();

            expr = new LogicalExpression(expr, op, right);
        }

        return expr;
    }

    private Expression And()
    {
        var expr = Equality();

        while (Match(TokenType.And))
        {
            var op = Previous;
            var right = Equality();

            expr = new LogicalExpression(expr, op, right);
        }

        return expr;
    }

    private Expression Equality()
    {
        var expr = Comparison();

        while (Match(TokenType.BangEqual, TokenType.EqualEqual))
        {
            var op = Previous;
            var right = Comparison();

            expr = new BinaryExpression(expr, op, right);
        }

        return expr;
    }

    private Expression Comparison()
    {
        var expr = Term();

        while (Match(TokenType.Greater, TokenType.GreaterEqual, TokenType.Less, TokenType.LessEqual))
        {
            var op = Previous;

            var right = Term();

            expr = new BinaryExpression(expr, op, right);
        }

        return expr;
    }

    private Expression Term()
    {
        var expr = Factor();

        while (Match(TokenType.Minus, TokenType.Plus))
        {
            var op = Previous;

            /*
                let mut a = 1;
                a++;

                Converts to:
                a = a + 1;
            */
            if (Match(op.Type))
            {
                // TODO: This might break on object setting
                if (expr is not VariableExpression)
                {
                    throw Error(op, "Unable to ");
                }

                // right is the literal 1
                var right = new LiteralExpression(1.0);

                return new AssignmentExpression(((VariableExpression)expr).Name, new BinaryExpression(expr, op, right));
            }
            else
            {
                var right = Factor();
                expr = new BinaryExpression(expr, op, right);
            }
        }

        return expr;
    }

    private Expression Factor()
    {
        var expr = Unary();

        while (Match(TokenType.Slash, TokenType.Star))
        {
            var op = Previous;

            var right = Unary();

            expr = new BinaryExpression(expr, op, right);
        }

        return expr;
    }

    private Expression Unary()
    {
        if (Match(TokenType.Bang, TokenType.Minus))
        {
            var op = Previous;
            var right = Unary();

            return new UnaryExpression(op, right);
        }

        return Call();
    }

    private Expression Call()
    {
        var expr = Primary();

        while (true)
        {
            if (Match(TokenType.LeftParen))
                expr = FinishCall(expr);
            else
                break;
        }

        return expr;

        Expression FinishCall(Expression callee)
        {
            var arguments = new List<Expression>();

            if (!Check(TokenType.RightParen))
            {
                do
                {
                    if (arguments.Count >= MaxFunctionParameters)
                        Error(Peek(), $"Unable to have {MaxFunctionParameters} or more parameters in a function");

                    arguments.Add(Expression());
                } while (Match(TokenType.Comma));
            }

            var paren = Consume(TokenType.RightParen, "Expected ')' after arguments.");

            return new CallExpression(callee, paren, arguments);
        }
    }

    private Expression Primary()
    {
        if (Match(TokenType.False))
            return new LiteralExpression(false);

        if (Match(TokenType.True))
            return new LiteralExpression(true);

        if (Match(TokenType.Nil))
            return new LiteralExpression(null);

        // TODO: Handle if the Literal is null
        if (Match(TokenType.Number, TokenType.String))
            return new LiteralExpression(Previous.Literal);

        if (Match(TokenType.Identifier))
            return new VariableExpression(Previous);

        if (Match(TokenType.LeftParen))
        {
            var expr = Expression();
            Consume(TokenType.RightParen, "Expected ')' after expression.");
            return new GroupingExpression(expr);
        }

        throw Error(Peek(), "Expected expression.");
    }

    private Statement ExpressionStatement()
    {
        var expr = Expression();

        // kek
        if (expr is VariableExpression vexpr && vexpr.Name.Lexeme == "var")
        {
            throw Error(Peek(), "Unknown keyword 'var' did you mean 'let'?");
        }

        Consume(TokenType.Semicolon, "Expected ';' after expression.");
        return new ExpressionStatement(expr);
    }

    private Statement IfStatement()
    {
        // condition { thenBranch } else { elseBranch }
        // condition do: thenBranch, else: elseBranch

        var condition = Expression();

        if (Match(TokenType.Do))
        {
            Consume(TokenType.Colon, "Expected ':' after 'do'.");

            var thenBranch = Statement();
            Statement? elseBranch = null;

            if (Match(TokenType.Else))
            {
                Consume(TokenType.Colon, "Expected ':' after 'else'.");
                elseBranch = Statement();
            }

            return new IfStatement(condition, thenBranch, elseBranch);
        }
        else // This else branch is only cause of variables with same name.
        {
            var thenBranch = Statement();
            var elseBranch = Match(TokenType.Else)
                ? Statement()
                : null;

            return new IfStatement(condition, thenBranch, elseBranch);
        }
    }

    private FunctionStatement Function(string kind)
    {
        var name = Consume(TokenType.Identifier, $"Expected {kind} name.");

        Consume(TokenType.LeftParen, $"Expected '(' after {kind} name");
        var parameters = new List<Token>();

        if (!Check(TokenType.RightParen))
        {
            do
            {
                if (parameters.Count >= MaxFunctionParameters)
                    Error(Peek(), $"Can't have more than {MaxFunctionParameters} parameters");

                parameters.Add(Consume(TokenType.Identifier, "Expected parameter name"));

            } while (Match(TokenType.Comma));
        }

        Consume(TokenType.RightParen, "Expected ')' after parameters.");

        Consume(TokenType.LeftBrace, $"Expected '{{' before {kind} body.");
        var body = BlockStatement();

        return new FunctionStatement(name, parameters, body);
    }

    private Statement LetDeclaration()
    {
        var isMutable = Match(TokenType.Mutable);

        var name = Consume(TokenType.Identifier, "Expected variable name.");

        Expression? initializer = null;
        if (Match(TokenType.Equal))
        {
            initializer = Expression();
        }

        Consume(TokenType.Semicolon, "Expected ';' after variable declaration.");

        return new LetStatement(name, isMutable, initializer);
    }

    private List<Statement> BlockStatement()
    {
        var statements = new List<Statement>();

        while (!Check(TokenType.RightBrace) && !AtEnd)
            statements.Add(Declaration());

        Consume(TokenType.RightBrace, "Expected '}' after block.");

        return statements;
    }

    private Statement ReturnStatement()
    {
        var keyword = Previous;
        Expression? value = null;

        if (!Check(TokenType.Semicolon))
            value = Expression();

        Consume(TokenType.Semicolon, "Expected ';' after return value.");

        return new ReturnStatement(keyword, value);
    }

    private Statement WhileStatement()
    {
        var condition = Expression();
        var body = Statement();

        return new WhileStatement(condition, body);
    }

    private Statement ForStatement()
    {
        Consume(TokenType.LeftParen, "Expected '(' after 'for'.");

        Statement? initializer;
        if (Match(TokenType.Semicolon))
        {
            initializer = null;
        }
        else if (Match(TokenType.Let))
        {
            initializer = LetDeclaration();
        }
        else
        {
            initializer = ExpressionStatement();
        }

        Expression? condition = null;
        if (!Check(TokenType.Semicolon))
        {
            condition = Expression();
        }

        Consume(TokenType.Semicolon, "Expected ';' after loop condition.");

        Expression? increment = null;
        if (!Check(TokenType.RightParen))
        {
            increment = Expression();
        }

        Consume(TokenType.RightParen, "Expected ')' after for clauses.");

        var body = Statement();

        if (increment is not null)
        {
            body = new BlockStatement(new List<Statement> { body, new ExpressionStatement(increment) });
        }

        condition ??= new LiteralExpression(true);

        body = new WhileStatement(condition, body);

        if (initializer is not null)
        {
            body = new BlockStatement(new List<Statement> { initializer, body });
        }

        // Allow ; at the end of the for loop
        if (Check(TokenType.Semicolon)) Advance();

        return body;
    }
}