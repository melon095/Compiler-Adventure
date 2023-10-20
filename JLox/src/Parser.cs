using JLox.src.Expr;

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

    public Expression? Parse()
    {
        try
        {
            return Expression();
        }
        catch (ParseException)
        {
            return null;
        }
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
        }

        Advance();
    }

    private Expression Expression() => Equality();

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

        if (Match(TokenType.LeftParen))
        {
            var expr = Expression();
            Consume(TokenType.RightParen, "Expected ')' after expression.");
            return new GroupingExpression(expr);
        }

        throw Error(Peek(), "Expected expression.");
    }
}
