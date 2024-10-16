#pragma once

#include "BaseExpression.hh"

namespace K::AST
{
    class VariableExpression : public BaseExpression
    {
    public:
        VariableExpression(const std::string &name);

        void Dump(int indent, std::ostream &os) const override;

    public:
        std::string Name;
    };

    SMARTPTR(VariableExpression)
}