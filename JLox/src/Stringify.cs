namespace JLox.src;

internal static class Stringify
{
    private static string Double(double d)
    {
        var text = d.ToString();

        if (text.EndsWith(".0"))
            text = text.Substring(0, text.Length - 2);

        return text;
    }

    public static string ToString(object? obj)
    {
        return obj switch
        {
            string s => s,
            null => "nil",
            double d => Double(d),
            bool and true => "true",
            bool and false => "false",
            _ => obj.ToString() ?? "<null>"
        };
    }
}
