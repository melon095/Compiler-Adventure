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

    public override string ToString() => Stringify.ToString(Value);
}

// System.Environment xP
internal class LoxEnvironment
{
    /// <summary>
    /// Enclosing holds the parents scope if this is a subscope.
    /// 
    /// When the scope is global, this is null.
    /// </summary>
    public readonly LoxEnvironment? Enclosing;

    private readonly Dictionary<string, Key> LocalScope = new();

    /// <summary>
    /// Executed for the global scope.
    /// </summary>
    public LoxEnvironment()
    {
        Enclosing = null;
    }

    /// <summary>
    /// Executed when entering a new scope. For example when entering a function or a block.
    /// </summary>
    public LoxEnvironment(LoxEnvironment parentScope)
    {
        Enclosing = parentScope;
    }

    private LoxEnvironment Ancestor(int distance)
    {
        var env = this;

        for (int i = 0; i < distance; i++)
        {
            env = env!.Enclosing;
        }

        // TODO: Fix nullable
        return env!;
    }

    public IReadOnlyDictionary<string, Key> GetValues() => LocalScope;

    public void Define(string name, Key key) => LocalScope[name] = key;

    public void DefineFunction(string name, ICallable function)
        => Define(name, new Key(true, function));

    // TODO: This has null reference exception.
    public object? GetAt(int distance, string name)
        => Ancestor(distance).LocalScope[name].Value;

    public object? Get(Token name)
    {
        if (LocalScope.TryGetValue(name.Lexeme, out var value))
        {
            return value.Value;
        }

        // Check the parents scope for the variable. This is a recursive call that eventually reaches the global scope.
        if (Enclosing is not null) return Enclosing.Get(name);

        throw new RuntimeException(name, $"Undefined variable '{name.Lexeme}'");
    }

    public void AssignAt(int distance, Token name, object? value)
        => Ancestor(distance).LocalScope[name.Lexeme].Value = value;

    public void Assign(Token name, object? value)
    {
        if (!LocalScope.TryGetValue(name.Lexeme, out var key))
        {
            if (Enclosing is not null)
            {
                // This will throw if the parent/global scope does not contain the variable.
                Enclosing.Assign(name, value);
                return;
            }

            throw new RuntimeException(name, $"Undefined variable '{name.Lexeme}'");
        }

        if (key.Mutable)
        {
            LocalScope[name.Lexeme].Value = value;
            return;
        }

        throw new RuntimeException(name, $"Cannot assign to immutable variable '{name.Lexeme}'");
    }
}
