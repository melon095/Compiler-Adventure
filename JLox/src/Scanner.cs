using System.Globalization;

namespace JLox;

internal class Scanner
{
    private static readonly Dictionary<string, TokenType> Keywords = new()
    {
        { "and",        TokenType.And       },
        { "class",      TokenType.Class     },
        { "do",         TokenType.Do        },
        { "else",       TokenType.Else      },
        { "false",      TokenType.False     },
        { "for",        TokenType.For       },
        { "fn",         TokenType.Function  },
        { "if",         TokenType.If        },
        { "nil",        TokenType.Nil       },
        { "or",         TokenType.Or        },
        { "print",      TokenType.Print     },
        { "return",     TokenType.Return    },
        { "super",      TokenType.Super     },
        { "self",       TokenType.Self      },
        { "true",       TokenType.True      },
        { "let",        TokenType.Let       },
        { "mut",        TokenType.Mutable   },
        { "while",      TokenType.While     },
    };

    public string Source { get; private set; }

    private readonly List<Token> Tokens = new();

    private int Start = 0;
    private int Current = 0;
    private int Line = 1;
    private bool AtEnd => Current >= Source.Length;

    public Scanner(string source)
    {
        Source = source;
    }

    public List<Token> ScanTokens()
    {
        while (!AtEnd)
        {
            Start = Current;

            ScanToken();
        }

        Tokens.Add(new(TokenType.EOF, Line: Line));

        return Tokens;
    }

    private void ScanToken()
    {
        var c = Advance();

        // FIXME: Change this

        switch (c)
        {
            case '(':
                {
                    AddToken(TokenType.LeftParen);
                }
                break;

            case ')':
                {
                    AddToken(TokenType.RightParen);
                }
                break;

            case '{':
                {
                    AddToken(TokenType.LeftBrace);
                }
                break;

            case '}':
                {
                    AddToken(TokenType.RightBrace);
                }
                break;

            case ',':
                {
                    AddToken(TokenType.Comma);
                }
                break;

            case '.':
                {
                    AddToken(TokenType.Dot);
                }
                break;

            case '-':
                {
                    AddToken(TokenType.Minus);
                }
                break;

            case '+':
                {
                    AddToken(TokenType.Plus);
                }
                break;

            case ':':
                {
                    AddToken(TokenType.Colon);
                }
                break;

            case ';':
                {
                    AddToken(TokenType.Semicolon);
                }
                break;


            case '*':
                {
                    AddToken(TokenType.Star);
                }
                break;

            case '!':
                {
                    if (Match('='))
                    {
                        AddToken(TokenType.BangEqual);
                    }
                    else
                    {
                        AddToken(TokenType.Bang);
                    }
                }
                break;

            case '=':
                {
                    if (Match('='))
                    {
                        AddToken(TokenType.EqualEqual);
                    }
                    else
                    {
                        AddToken(TokenType.Equal);
                    }
                }
                break;

            case '<':
                {
                    if (Match('='))
                    {
                        AddToken(TokenType.LessEqual);
                    }
                    else
                    {
                        AddToken(TokenType.Less);
                    }
                }

                break;

            case '>':
                {
                    if (Match('='))
                    {
                        AddToken(TokenType.GreaterEqual);
                    }
                    else
                    {
                        AddToken(TokenType.Greater);
                    }
                }
                break;

            case '/':
                {
                    if (Match('/'))
                    {
                        // A comment goes until the end of the line.
                        while (Peek() != '\n' && !AtEnd) Advance();
                    }
                    else if (Match('*'))
                    {
                        // Multi-line comment

                        var depth = 1;

                        while (depth > 0)
                        {
                            if (AtEnd)
                            {
                                Program.Error(Line, "Unterminated comment.");
                                return;
                            }

                            if (Peek() == '\n')
                            {
                                Line++;
                            }

                            if (Peek() == '/' && PeekNext() == '*')
                            {
                                depth++;
                                Advance();
                            }
                            else if (Peek() == '*' && PeekNext() == '/')
                            {
                                depth--;
                                Advance();
                            }

                            Advance();
                        }
                    }
                    else
                    {
                        AddToken(TokenType.Slash);
                    }
                }
                break;

            case ' ':
            case '\r':
            case '\t':
                // Ignore whitespace.
                break;

            case '\n':
                {
                    Line++;
                }
                break;

            case '"':
                {
                    HandleString();
                }
                break;

            case 'o':
                {
                    if (Match('r'))
                    {
                        AddToken(TokenType.Or);
                    }
                }
                break;

            default:
                if (IsDigit(c))
                {
                    HandleNumber();

                    break;
                }

                else if (IsAlpha(c))
                {
                    HandleIdentifier();

                    break;
                }

                // Circular depdency 😋
                Program.Error(Line, $"Unexpected character '{c}'");
                break;

        }
    }

    private static bool IsDigit(char c)
        => c >= '0' && c <= '9';

    private static bool IsAlpha(char c)
        => (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
            c == '$' || // Similar to JS, but i like the $ in JS tbh
            c == '_';

    private static bool IsAlphaNumeric(char c)
        => IsAlpha(c) || IsDigit(c);

    private char Advance()
    {
        return Source[Current++];
    }

    private void AddToken(TokenType t)
    {
        var text = Source[Start..Current];

        Tokens.Add(new(t, text, Line: Line));
    }

    private void AddToken(TokenType type, object literal)
    {
        var text = Source[Start..Current];

        Tokens.Add(new(type, text, literal, Line));
    }

    private bool Match(char expected)
    {
        if (AtEnd) return false;

        // Current is incremented in Advance()
        if (Source[Current] != expected) return false;

        Current++;
        return true;
    }

    private char Peek()
    {
        if (AtEnd) return '\0';

        return Source[Current];
    }

    private char PeekNext()
    {
        if (Current + 1 >= Source.Length) return '\0';

        return Source[Current + 1];
    }

    private void HandleString()
    {
        while (Peek() != '"' && !AtEnd)
        {
            if (Peek() == '\n')
                Line++;

            Advance();
        }

        // Unterminated string.
        if (AtEnd)
        {
            Program.Error(Line, "Unterminated string.");
            return;
        }

        // The closing ".
        Advance();

        // Trim the surrounding quotes.
        var value = Source[(Start + 1)..(Current - 1)];
        AddToken(TokenType.String, value);
    }

    private void HandleNumber()
    {
        while (IsDigit(Peek()))
            Advance();

        if (Peek() == '.' && IsDigit(PeekNext()))
        {
            Advance();

            while (IsDigit(Peek()))
                Advance();
        }

        var text = Source[Start..Current];
        var value = double.Parse(text, NumberStyles.AllowDecimalPoint, CultureInfo.InvariantCulture);

        AddToken(TokenType.Number, value);
    }

    private void HandleIdentifier()
    {
        while (IsAlphaNumeric(Peek()))
            Advance();

        var text = Source[Start..Current];

        if (Keywords.TryGetValue(text, out var type))
        {
            AddToken(type);
        }
        else
        {
            AddToken(TokenType.Identifier);
        }
    }
}
