using JLox.src.Expr;
using JLox.src.Stmt;

namespace JLox.src;

internal class Parser
{
    private readonly List<Token> Tokens;
    private int Current = 0;

    public Parser(List<Token> t)
    {
        Tokens = t;
    }

    private ParseException Error(Token token, string message)
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
                case TokenType.Print:
                case TokenType.Return:
                    return;
            }

            Advance();
        }
    }

    private Statement Declaration()
    {
        if (Match(TokenType.Let)) return LetDeclaration();

        return Statement();
    }

    private Statement Statement()
    {
        if (Match(TokenType.If)) return IfStatement();
        if (Match(TokenType.Print)) return PrintStatement();
        if (Match(TokenType.LeftBrace)) return new BlockStatement(BlockStatement());

        return ExpressionStatement();
    }

    private Expression Expression() => Assignment();

    private Expression Assignment()
    {
        var expr = Equality();

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

            var right = Factor();

            expr = new BinaryExpression(expr, op, right);
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

        return Primary();
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

    private Statement PrintStatement()
    {
        var expr = Expression();

        // Ugly hack to not force semicolon after a print inside of a one-liner if statement
        if (Tokens[Current + 1].Type != TokenType.Colon)
            Consume(TokenType.Semicolon, "Expected ';' after value.");

        return new PrintStatement(expr);
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
}