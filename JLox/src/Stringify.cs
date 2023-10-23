namespace JLox.src;

internal static class Stringify
{
    private static string Double(double d)
    {
        var text = d.ToString();

        if (text.EndsWith(".0"))
            text = text[..^2];

        return text;
    }

    public static string ToString(object? obj)
        => obj switch
        {
            string s => s,
            null => "nil",
            double d => Double(d),
            bool and true => "true",
            bool and false => "false",
            _ => obj.ToString() ?? "nil"
        };

}
