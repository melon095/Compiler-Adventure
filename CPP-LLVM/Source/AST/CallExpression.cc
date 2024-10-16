#include <AST/CallExpression.hh>

namespace K::AST
{
    CallExpression::CallExpression(const std::string &callee, std::vector<ExpressionPtr> args)
        : Callee(callee), Args(std::move(args))
    {
    }

    void CallExpression::Dump(int indent, std::ostream &os) const
    {
        os << std::string(indent, ' ') << "CallExpression(" << Callee << ")" << std::endl;

        for (const auto &arg : Args)
        {
            arg->Dump(indent + 2, os);
        }
    }
}