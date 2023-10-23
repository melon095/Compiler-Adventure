namespace JLox.src;

internal abstract class NativeFunction
{
    public override string ToString() => $"<native code>";
}

internal sealed class NativeFunctions
{
    internal sealed class Clock : NativeFunction, ICallable
    {
        public int Arity => 0;

        public object? Call(Interpreter ip, List<object?> args)
        {
            _ = ip;
            _ = args;

            return DateTime.Now.Ticks / TimeSpan.TicksPerSecond;
        }
    }

    internal sealed class DebugPrintScope : NativeFunction, ICallable
    {
        public int Arity => 1;

        public object? Call(Interpreter ip, List<object?> args)
        {
            var arg = 1;
            if (args[0] is double d)
                arg = (int)d;

            // go through the stack and print out all the variables
            var currentScope = ip.Environment;
            var scope = ip.Environment;
            var depth = 0;

            while (scope != null)
            {
                if (depth == arg)
                    break;

                foreach (var kvp in scope.GetValues())
                {
                    Console.WriteLine($"[DEBUG] [{depth}] {kvp.Key} = {kvp.Value}");
                }

                scope = scope.Enclosing;
                depth++;
            }

            ip.Environment = currentScope;

            return null;
        }
    }
}
