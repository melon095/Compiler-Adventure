#include <AST/NumberExpression.hh>

namespace K::AST
{
    NumberExpression::NumberExpression(double value)
        : Value(value)
    {
    }

    void NumberExpression::Dump(int indent, std::ostream &os) const
    {
        os << std::string(indent, ' ') << "NumberExpression(" << Value << ")" << std::endl;
    }
}