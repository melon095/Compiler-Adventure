using JLox.src.Exceptions;

namespace JLox.src;

internal class Key
{
    public bool Mutable { get; set; }
    public object? Value { get; set; }

    public Key(bool mutable, object? value)
    {
        Mutable = mutable;
        Value = value;
    }

    public Key(object? value)
    {
        Mutable = false;
        Value = value;
    }
}

// System.Environment xP
internal class LoxEnvironment
{
    private readonly Dictionary<string, Key> GlobalScope = new();

    public void Define(string name, Key key) => GlobalScope[name] = key;

    public object? Get(Token name)
    {
        if (GlobalScope.TryGetValue(name.Lexeme, out var value))
        {
            return value.Value;
        }

        throw new RuntimeException(name, $"Undefined variable '{name.Lexeme}'");
    }

    public void Assign(Token name, object? value)
    {
        if (!GlobalScope.TryGetValue(name.Lexeme, out var key))
        {
            throw new RuntimeException(name, $"Undefined variable '{name.Lexeme}'");
        }

        if (key.Mutable)
        {
            GlobalScope[name.Lexeme].Value = value;
            return;
        }

        throw new RuntimeException(name, $"Cannot assign to immutable variable '{name.Lexeme}'");
    }
}
