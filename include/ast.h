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

    class Identifier : public Expression
    {
        std::string name;
      public:
        Identifier(const std::string &_name) : name(_name) {}

        const char* getName() { return name.c_str(); }
    };
}

#endif
