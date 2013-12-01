// AST nodes

#ifndef AST_H
#define AST_H

#include <string>

namespace AST
{
    class Expression
    {
      public:
        virtual ~Expression() {}
    };

    class NumberLiteral : public Expression
    {
        double val;
      public:
        NumberLiteral(double _val) : val(_val) {}

        double getValue() { return val; }
    };

    class VariableReference : public Expression
    {
        std::string name;
      public:
        VariableReference(const std::string &_name) : name(_name) {}
    };
}

#endif
