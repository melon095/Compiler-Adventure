
using JLox.src;
using JLox.src.Exceptions;

// TODO: Remove the Visitor thing and instead have each statement/expression handle itself internally

namespace JLox;

internal class Program
{
    static string[] Args = Array.Empty<string>();

    static bool HadError = false;
    static bool HadRuntimeError = false;
    static bool IsRepl = false;

    static readonly Interpreter Interpreter = new();

    static bool HasArg(string arg) => Args.Contains(arg);

    static int Main(string[] args)
    {
        // Why
        Console.OutputEncoding = System.Text.Encoding.Unicode;

        Args = args;

        var hasFile = args.Any(arg => !arg.StartsWith('-'));

        if (hasFile)
        {
            var file = args.First(arg => !arg.StartsWith('-'));

            RunFile(file);
        }
        else
        {
            IsRepl = true;
            RunREPL();
        }

        return 0;
    }

    static void RunFile(string fileName)
    {
        var text = File.ReadAllText(fileName);

        Run(text);

        if (HadError)
        {
            Environment.Exit(65);
        }

        if (HadRuntimeError)
        {
            Environment.Exit(70);
        }
    }

    static void RunREPL()
    {
        var ReplLine = 0;

        while (true)
        {
            Console.Write($"jlox[{ReplLine++}]> ");
            var line = Console.ReadLine();
            if (line == null)
            {
                break;
            }

            Run(line);
        }
    }

    static void Run(string inputCode)
    {
        Scanner scanner = new(inputCode);

        var tokens = scanner.ScanTokens();

        if (HasArg("--token")) // TODO: One time check
        {
            foreach (var token in tokens)
            {
                Console.WriteLine(token);
            }
        }

        Parser parser = new(tokens);
        var statements = parser.Parse();

        if (HadError)
        {
            return;
        }

        var resolver = new Resolver(Interpreter);
        resolver.Resolve(statements);

        if (HadError)
        {
            return;
        }

        Interpreter.Interpret(statements, IsRepl);
    }

    public static void RuntimeError(RuntimeException ex)
    {
        Console.Error.WriteLine($"[Line {ex.Token.Line}]\n{ex.Message}");

        HadRuntimeError = true;
    }

    public static void Error(int line, string message)
    {
        Report(line, string.Empty, message);
    }

    public static void Error(Token token, string message)
    {
        if (token.Type == TokenType.EOF)
        {
            Report(token.Line, " at end", message);
        }
        else
        {
            Report(token.Line, $" at '{token.Lexeme}'", message);
        }
    }

    static void Report(int line, string where, string message)
    {
        Console.Error.WriteLine($"[line {line}] Error{where}: {message}");

        // Only set the error flag if we're not in the REPL
        HadError = !IsRepl;
    }
}