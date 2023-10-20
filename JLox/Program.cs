﻿namespace JLox;

internal class Program
{
    static bool HadError = false;

    static int Main(string[] args)
    {
        if (args.Length > 1)
        {
            Console.WriteLine("Usage: jlox [script]");
            return 1;
        }
        else if (args.Length == 1)
        {
            Console.WriteLine($"Running file {args[0]}");

            RunFile(args[0]);
        }
        else
        {
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
    }

    static void RunREPL()
    {
        while (true)
        {
            Console.Write("> ");
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
        Scanner scanner = new()
        {
            Source = inputCode
        };

        var tokens = scanner.ScanTokens();

        foreach (var token in tokens)
        {
            Console.WriteLine(token);
        }
    }

    public static void Error(int line, string message)
    {
        Report(line, string.Empty, message);
    }

    static void Report(int line, string where, string message)
    {
        Console.WriteLine($"[line {line}] Error{where}: {message}");
        HadError = true;
    }
}